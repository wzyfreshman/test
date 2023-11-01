#include "rtapi.h"
#include "rtapi_common.h"

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <sys/ipc.h>		/* IPC_* */
#include <sys/shm.h>		/* shmget() */
#include <unistd.h>

int period = 0;
int msg_level = RTAPI_MSG_ERR;	/* message printing level */

static rtapi_data_t local_rtapi_data;
rtapi_data_t *rtapi_data = &local_rtapi_data;
task_data *task_array =  local_rtapi_data.task_array;
shmem_data *shmem_array = local_rtapi_data.shmem_array;
module_data *module_array = local_rtapi_data.module_array;

#define MAX_SHM 64
#define MODULE_OFFSET		32768
#define SHMEM_MAGIC   25453	/* random numbers used as signatures */

int rtapi_shmem_new(int key, int module_id, unsigned long int size) {

    shmem_data *shmem;
    struct shmid_ds d;
    int i, ret;
    int is_new = 0;

    rtapi_mutex_get(&(rtapi_data->mutex));
    for (i=0 ; i < RTAPI_MAX_SHMEMS; i++) {
    if (shmem_array[i].magic == SHMEM_MAGIC && shmem_array[i].key == key) {
        shmem_array[i].count ++;
        rtapi_mutex_give(&(rtapi_data->mutex));
        return i;
    }
    if (shmem_array[i].magic != SHMEM_MAGIC)
        break;
    }
    if (i == RTAPI_MAX_SHMEMS) {
    rtapi_mutex_give(&(rtapi_data->mutex));
    rtapi_print_msg(RTAPI_MSG_ERR,
            "rtapi_shmem_new failed due to RTAPI_MAX_SHMEMS\n");
    return -ENOMEM;
    }
    shmem = &shmem_array[i];

    /* now get shared memory block from OS */

    // try to attach
    shmem->id = shmget((key_t)key, size, SHM_PERMISSIONS);
    if (shmem->id == -1) {
        if(errno == ENOENT) {
            // nope, doesnt exist - create
            shmem->id = shmget((key_t)key, size, SHM_PERMISSIONS | IPC_CREAT);
            is_new = 1;
        }
        if(shmem->id == -1) {
            rtapi_mutex_give(&(rtapi_data->mutex));
            rtapi_print_msg(RTAPI_MSG_ERR,
                    "Failed to allocate shared memory, "
                    "key=0x%x size=%ld\n", key, size);
            return -ENOMEM;
        }
    }

    // get actual user/group and drop to ruid/rgid so removing is
    // always possible
    if ((ret = shmctl(shmem->id, IPC_STAT, &d)) < 0) {
    rtapi_print_msg(RTAPI_MSG_ERR,
            "rtapi_shmem_new: shm_ctl(key=0x%x, IPC_STAT) "
            "failed: %d '%s'\n",
            key, errno, strerror(errno));
    } else {
    // drop permissions of shmseg to real userid/group id
    if (!d.shm_perm.uid) { // uh, root perms
        d.shm_perm.uid = getuid();
        d.shm_perm.gid = getgid();
        if ((ret = shmctl(shmem->id, IPC_SET, &d)) < 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "rtapi_shmem_new: shm_ctl(key=0x%x, IPC_SET) "
                "failed: %d '%s'\n",
                key, errno, strerror(errno));
        }
    }
    }
    /* and map it into process space */
    shmem->mem = shmat(shmem->id, 0, 0);
    if ((ssize_t) (shmem->mem) == -1) {
    rtapi_mutex_give(&(rtapi_data->mutex));
    rtapi_print_msg(RTAPI_MSG_ERR,
            "rtapi_shmem_new: shmat(%d) failed: %d '%s'\n",
            shmem->id, errno, strerror(errno));
    return -errno;
    }
    /* Touch each page by either zeroing the whole mem (if it's a new
       SHM region), or by reading from it. */
    if (is_new) {
        memset(shmem->mem, 0, size);
    } else {
        unsigned int i, pagesize;

        pagesize = sysconf(_SC_PAGESIZE);
        for (i = 0; i < size; i += pagesize) {
            unsigned int x = *(volatile unsigned int *)
            ((unsigned char *)shmem->mem + i);
            /* Use rand_r to clobber the read so GCC won't optimize it
               out. */
            rand_r(&x);
        }
    }

    /* label as a valid shmem structure */
    shmem->magic = SHMEM_MAGIC;
    /* fill in the other fields */
    shmem->size = size;
    shmem->key = key;
    shmem->count = 1;

    rtapi_mutex_give(&(rtapi_data->mutex));

    /* return handle to the caller */
    is_new += module_id;

    return i;
}


int rtapi_shmem_getptr(int handle, void **ptr) {
    shmem_data *shmem;
    if (handle < 0 || handle >= RTAPI_MAX_SHMEMS)
    return -EINVAL;

    shmem = &shmem_array[handle];

    /* validate shmem handle */
    if (shmem->magic != SHMEM_MAGIC)
    return -EINVAL;

    /* pass memory address back to caller */
    *ptr = shmem->mem;
    return 0;
}


int rtapi_shmem_delete(int handle, int module_id) {
    struct shmid_ds d;
    int r1, r2;
    shmem_data *shmem;

    if(handle < 0 || handle >= RTAPI_MAX_SHMEMS)
    return -EINVAL;

    rtapi_mutex_get(&(rtapi_data->mutex));
    shmem = &shmem_array[handle];

    /* validate shmem handle */
    if (shmem->magic != SHMEM_MAGIC) {
    rtapi_mutex_give(&(rtapi_data->mutex));
    return -EINVAL;
    }

    shmem->count --;
    if(shmem->count) {
    rtapi_mutex_give(&(rtapi_data->mutex));
    rtapi_print_msg(RTAPI_MSG_DBG,
            "rtapi_shmem_delete: handle=%d module=%d key=0x%x:  "
            "%d remaining users\n",
            handle, module_id, shmem->key, shmem->count);
    return 0;
    }

    /* unmap the shared memory */
    r1 = shmdt(shmem->mem);
    if (r1 < 0) {
    rtapi_print_msg(RTAPI_MSG_ERR,
            "rtapi_shmem_delete: shmdt(key=0x%x) "
            "failed: %d '%s'\n",
            shmem->key, errno, strerror(errno));
    }
    /* destroy the shared memory */
    r2 = shmctl(shmem->id, IPC_STAT, &d);
    if (r2 < 0) {
    rtapi_print_msg(RTAPI_MSG_ERR,
            "rtapi_shmem_delete: shm_ctl(0x%x, IPC_STAT) "
            "failed: %d '%s'\n",
            shmem->key, errno, strerror(errno));
    }
    if(r2 == 0 && d.shm_nattch == 0) {
    r2 = shmctl(shmem->id, IPC_RMID, &d);
    if (r2 < 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "rtapi_shmem_delete: shm_ctl(0x%x, IPC_RMID) "
                "failed: %d '%s'\n",
                shmem->key, errno, strerror(errno));
    }
    }

    /* free the shmem structure */
    shmem->magic = 0;

    rtapi_mutex_give(&(rtapi_data->mutex));

    if ((r1 != 0) || (r2 != 0))
    return -EINVAL;
    return 0;
}

#ifdef ULAPI
int rtapi_init(const char *modname) {
    /* do nothing for ULAPI */
    /* say hello */
    rtapi_print_msg(RTAPI_MSG_DBG, "ULAPI: initing module %s\n", modname);
    return getpid();
}
int rtapi_exit(int module_id) {
    /* do nothing for ULAPI */
    rtapi_print_msg(RTAPI_MSG_DBG, "ULAPI: rtapi_exit %d\n", module_id);
    return 0;
}

#else

int rtapi_init(const char *modname) {

    int n, module_id = -ENOMEM;
    module_data *module;

    /* say hello */
    rtapi_print_msg(RTAPI_MSG_DBG, "RTAPI: initing module %s\n", modname);

    /* get the mutex */
    rtapi_mutex_get(&(rtapi_data->mutex));
    /* find empty spot in module array */
    n = 1;
    while ((n <= RTAPI_MAX_MODULES) && (module_array[n].state != NO_MODULE)) {
    n++;
    }
    if (n > RTAPI_MAX_MODULES) {
    /* no room */
    rtapi_mutex_give(&(rtapi_data->mutex));
    rtapi_print_msg(RTAPI_MSG_ERR, "RTAPI: ERROR: reached module limit %d\n",
            n);
    return -EMFILE;
    }
    /* we have space for the module */
    module_id = n + MODULE_OFFSET;
    module = &(module_array[n]);
    /* update module data */
    module->state = REALTIME;
    if (modname != NULL) {
    /* use name supplied by caller, truncating if needed */
    rtapi_snprintf(module->name, RTAPI_NAME_LEN, "%s", modname);
    } else {
    /* make up a name */
    rtapi_snprintf(module->name, RTAPI_NAME_LEN, "ULMOD%03d", module_id);
    }
    rtapi_data->ul_module_count++;
    rtapi_print_msg(RTAPI_MSG_DBG, "RTAPI: module '%s' loaded, ID: %d\n",
    module->name, module_id);
    rtapi_mutex_give(&(rtapi_data->mutex));
    return module_id;
}

int rtapi_exit(int module_id) {
    module_id -= MODULE_OFFSET;

    if (module_id < 0 || module_id >= RTAPI_MAX_MODULES)
    return -1;
    /* Remove the module from the module_array. */
    rtapi_mutex_get(&(rtapi_data->mutex));
    module_array[module_id].state = NO_MODULE;
    rtapi_print_msg(RTAPI_MSG_DBG,
            "rtapi_exit: freed module slot %d, was %s\n",
            module_id, module_array[module_id].name);
    rtapi_mutex_give(&(rtapi_data->mutex));

    return 0;
}
#endif


void default_rtapi_msg_handler(msg_level_t level, const char *fmt, va_list ap) {
    if(level == RTAPI_MSG_ALL)
	vfprintf(stdout, fmt, ap);
    else
	vfprintf(stderr, fmt, ap);
}
static rtapi_msg_handler_t rtapi_msg_handler = default_rtapi_msg_handler;

rtapi_msg_handler_t rtapi_get_msg_handler(void) {
    return rtapi_msg_handler;
}

void rtapi_set_msg_handler(rtapi_msg_handler_t handler) {
    if(handler == NULL) rtapi_msg_handler = default_rtapi_msg_handler;
    else rtapi_msg_handler = handler;
}


void rtapi_print(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rtapi_msg_handler(RTAPI_MSG_ALL, fmt, args);
    va_end(args);
}


void rtapi_print_msg(int level, const char *fmt, ...)
{
    va_list args;

    if ((level <= msg_level) && (msg_level != RTAPI_MSG_NONE)) {
	va_start(args, fmt);
	rtapi_msg_handler(level, fmt, args);
	va_end(args);
    }
}

int rtapi_snprintf(char *buffer, unsigned long int size, const char *msg, ...) {
    va_list args;
    int result;

    va_start(args, msg);
    /* call the normal library vnsprintf() */
    result = vsnprintf(buffer, size, msg, args);
    va_end(args);
    return result;
}

int rtapi_vsnprintf(char *buffer, unsigned long int size, const char *fmt,
	va_list args) {
    return vsnprintf(buffer, size, fmt, args);
}

int rtapi_set_msg_level(int level) {
    msg_level = level;
    return 0;
}

int rtapi_get_msg_level() { 
    return msg_level;
}


#ifdef HAVE_RTAPI_GETTIME_HOOK

extern long long rtapi_get_time_hook(void);
long long rtapi_get_time(void) {
    return rtapi_get_time_hook();
}

#else

long long rtapi_get_time(void) {

    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec * 1000 * 1000 * 1000 + tv.tv_usec * 1000;

}

#endif



#ifdef HAVE_RTAPI_GETCLOCKS_HOOK

long long rtapi_get_clocks_hook(void);
long long rtapi_get_clocks(void) {
    return rtapi_get_clocks_hook();
}

#else

#define rdtscll(val) \
         __asm__ __volatile__("rdtsc" : "=A" (val))
long long rtapi_get_clocks(void)
{
    long long int retval;

    rdtscll(retval);
    return retval;
}

#endif

extern void rtapi_delay_hook(long nsec);

void rtapi_delay(long nsec) {
    rtapi_delay_hook(nsec);
}


long int rtapi_delay_max(void) {
    return 10000; //FIXME
}

long int rtapi_clock_set_period( long int nsecs) {

  if(nsecs == 0) return period;
  if(period != 0) {
      rtapi_print_msg(RTAPI_MSG_ERR, "attempt to set period twice\n");
      return -EINVAL;
  }
  period = nsecs;
  return period;

}

