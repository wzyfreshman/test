#include "config.h"
#include "rtapi.h"
#include "rtapi_common.h"

#include <unistd.h>		// getpid(), syscall()
#include <time.h>               // clock_nanosleep()
#include <stdlib.h>		// malloc(), sizeof(), free()
#include <string.h>		// memset()
#include <signal.h>		// sigaction(), sigemptyset(), SIGXCPU...
#include <sys/resource.h>	// rusage, getrusage(), RUSAGE_SELF

int  thread_max_latency[MAX_THREAD_LATENCY_NUM] = {0,0,0,0};
static int  max_latency;

#ifdef __USE_PREEMPT__

//using preempt native api to do this

static pthread_key_t task_key;
static pthread_once_t task_key_once = PTHREAD_ONCE_INIT;

typedef struct {

    int deleted;
    int destroyed;
    int deadline_scheduling;
    struct timespec next_time;

    /* The realtime thread. */
    pthread_t thread;
    pthread_barrier_t thread_init_barrier;
    void *stackaddr;

    /* Statistics */
    unsigned long minfault_base;
    unsigned long majfault_base;
    unsigned int failures;

} extra_task_data_t;

extra_task_data_t extra_task_data[RTAPI_MAX_TASKS + 1];


static inline int task_id(task_data *task) {
    return (int)(task - task_array);
}

static void rtapi_advance_time(struct timespec *tv, unsigned long ns,
                   unsigned long s) {
    ns += tv->tv_nsec;
    while (ns > 1000000000) {
        s++;
        ns -= 1000000000;
    }
    tv->tv_nsec = ns;
    tv->tv_sec += s;
}

static void rtapi_key_alloc() {
    pthread_key_create(&task_key, NULL);
}

static void rtapi_set_task(task_data *t) {
    pthread_once(&task_key_once, rtapi_key_alloc);
    pthread_setspecific(task_key, (void *)t);
}

static task_data *rtapi_this_task() {
    pthread_once(&task_key_once, rtapi_key_alloc);
    return (task_data *)pthread_getspecific(task_key);
}

int rtapi_task_new_hook(task_data *task, int task_id) {

    void *stackaddr;

    stackaddr = malloc(task->stacksize);
    if (!stackaddr) {
    rtapi_print_msg(RTAPI_MSG_ERR,
            "Failed to allocate realtime thread stack\n");
    return -ENOMEM;
    }
    memset(stackaddr, 0, task->stacksize);
    extra_task_data[task_id].stackaddr = stackaddr;
    extra_task_data[task_id].destroyed = 0;
    return task_id;

}


void rtapi_task_delete_hook(task_data *task, int task_id) {

    int err;
    void *returncode;

    /* Signal thread termination and wait for the thread to exit. */
    if (!extra_task_data[task_id].deleted) {
        extra_task_data[task_id].deleted = 1;
        err = pthread_join(extra_task_data[task_id].thread, &returncode);
        if (err)
            rtapi_print_msg(RTAPI_MSG_ERR, "pthread_join() on realtime thread failed\n");
    }
    /* Free the thread stack. */
    free(extra_task_data[task_id].stackaddr);
    extra_task_data[task_id].stackaddr = NULL;
    task->unuse = 0;

}


static int realtime_set_affinity(task_data *task) {

    cpu_set_t set;
    int err, cpu_nr, use_cpu = 0;

    return 0 ;
    pthread_getaffinity_np(extra_task_data[task_id(task)].thread,
               sizeof(set), &set);
    /*
    if (task->cpu > -1) { // CPU set explicitly

        if (!CPU_ISSET(task->cpu, &set)) {

            rtapi_print_msg(RTAPI_MSG_ERR,
                    "RTAPI: ERROR: realtime_set_affinity(%s): "
                    "CPU %d not available\n",
                    task->name, task->cpu);
            return -EINVAL;

        }
        use_cpu = task->cpu;

    } else {
        // select last CPU as default
        for (cpu_nr = CPU_SETSIZE - 1; cpu_nr >= 0; cpu_nr--) {
            if (CPU_ISSET(cpu_nr, &set)) {
            use_cpu = cpu_nr;
            break;
            }
        }
        if (use_cpu < 0) {
            rtapi_print_msg(RTAPI_MSG_ERR,
                    "Unable to get ID of the last CPU\n");
            return -EINVAL;
        }
        rtapi_print_msg(RTAPI_MSG_DBG, "task %s: using default CPU %d\n",
                                    task->name, use_cpu);
    }*/
    CPU_ZERO(&set);
    CPU_SET(use_cpu, &set);

    err = pthread_setaffinity_np(extra_task_data[task_id(task)].thread,
                 sizeof(set), &set);
    if (err) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "%d %s: Failed to set CPU affinity to CPU %d (%s)\n",
                task_id(task), task->name, use_cpu, strerror(errno));
        return -EINVAL;
    }
    rtapi_print_msg(RTAPI_MSG_DBG,
            "realtime_set_affinity(): task %s assigned to CPU %d\n",
            task->name, use_cpu);
    return 0;

}


#define ENABLE_SCHED_DEADLINE	0 /*XXX set to 1 to enable deadline scheduling. */

static inline int sched_wait_interval(int flags, const struct timespec *rqtp,
                      struct timespec *rmtp) {
    //FIXME THIS FUN!!
    rtapi_print_msg(RTAPI_MSG_ERR,"ERROR.should never be here %x \n",
                    (int)flags);
    int shit = rqtp->tv_nsec +rmtp->tv_nsec;
    shit = -ENOSYS;
    return shit;


}


static int realtime_set_priority(task_data *task) {

    struct sched_param schedp;

    extra_task_data[task_id(task)].deadline_scheduling = 0;
    if (ENABLE_SCHED_DEADLINE) {
        //FIXME--MHC
    }

    memset(&schedp, 0, sizeof(schedp));
    schedp.sched_priority = task->prio;

    if (sched_setscheduler(0, SCHED_FIFO, &schedp)) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "Unable to set FIFO scheduling policy: %s",
                strerror(errno));
        return 1;
    }
    return 0;

}


static void *realtime_thread(void *arg) {
    task_data *task = arg;

    rtapi_set_task(task);

    /* The task should not pagefault at all. So reset the counter now.
     * Note that currently we _do_ receive a few pagefaults in the
     * taskcode init. This is noncritical and probably not worth
     * fixing. */
    //rtapi_reset_pagefault_count(task);

    if (task->period < period)
        task->period = period;

    task->ratio = task->period / period;

    rtapi_print_msg(RTAPI_MSG_DBG, "task %p period = %d ratio=%d\n",
                task, task->period, task->ratio);

    if (realtime_set_affinity(task))
        goto error;
    if (realtime_set_priority(task))
        goto error;

    /* We're done initializing. Open the barrier. */
    pthread_barrier_wait(&extra_task_data[task_id(task)].thread_init_barrier);

    clock_gettime(CLOCK_MONOTONIC, &extra_task_data[task_id(task)].next_time);
    rtapi_advance_time(&extra_task_data[task_id(task)].next_time,
               task->period, 0);

    /* call the task function with the task argument */
    task->taskcode(task->arg);

    rtapi_print_msg(RTAPI_MSG_ERR, "ERROR: reached end of realtime thread for task %d\n",
                                task_id(task));
    extra_task_data[task_id(task)].deleted = 1;

    return NULL;
 error:
    /* Signal that we're dead and open the barrier. */
    extra_task_data[task_id(task)].deleted = 1;
    pthread_barrier_wait(&extra_task_data[task_id(task)].thread_init_barrier);
    return NULL;

}

int rtapi_task_start_hook(task_data *task, int task_id) {

    pthread_attr_t attr;
    int retval;

    extra_task_data[task_id].deleted = 0;

    pthread_barrier_init(&extra_task_data[task_id].thread_init_barrier,
             NULL, 2);
    pthread_attr_init(&attr);
    pthread_attr_setstack(&attr, extra_task_data[task_id].stackaddr,
              task->stacksize);
    rtapi_print_msg(RTAPI_MSG_DBG,
            "About to pthread_create task %d\n", task_id);
    retval = pthread_create(&extra_task_data[task_id].thread,
                &attr, realtime_thread, (void *)task);
    rtapi_print_msg(RTAPI_MSG_DBG,"Created task %d\n", task_id);
    pthread_attr_destroy(&attr);
    if (retval) {
    pthread_barrier_destroy
        (&extra_task_data[task_id].thread_init_barrier);
    rtapi_print_msg(RTAPI_MSG_ERR, "Failed to create realtime thread\n");
    return -ENOMEM;
    }
    /* Wait for the thread to do basic initialization. */
    pthread_barrier_wait(&extra_task_data[task_id].thread_init_barrier);
    pthread_barrier_destroy(&extra_task_data[task_id].thread_init_barrier);
    if (extra_task_data[task_id].deleted) {
    /* The thread died in the init phase. */
    rtapi_print_msg(RTAPI_MSG_ERR,
            "Realtime thread initialization failed\n");
    return -ENOMEM;
    }
    rtapi_print_msg(RTAPI_MSG_DBG,
            "Task %d finished its basic init\n", task_id);

    return 0;

}

#define NSEC_PER_SEC (1000000000L)
#define PERIOD_NS (NSEC_PER_SEC / FREQUENCY)

#define DIFF_NS(A, B) (((B).tv_sec - (A).tv_sec) * NSEC_PER_SEC + \
    (B).tv_nsec - (A).tv_nsec)

#define TIMESPEC2NS(T) ((uint64_t) (T).tv_sec * NSEC_PER_SEC + (T).tv_nsec)


void rtapi_task_stop_hook(task_data *task, int task_id) {
    extra_task_data[task_id].destroyed = 1;
    task->unuse = 0;
}



int rtapi_wait_hook(void) {


    struct timespec ts;

    task_data *task = rtapi_this_task();
    int id = task_id(task);

    if (extra_task_data[task_id(task)].deleted)
        pthread_exit(0);

    if (extra_task_data[task_id(task)].deadline_scheduling)
        sched_wait_interval(TIMER_ABSTIME,
                            &extra_task_data[task_id(task)].next_time, NULL);
    else
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
                &extra_task_data[task_id(task)].next_time, NULL);

    clock_gettime(CLOCK_MONOTONIC, &ts);
    if( id < MAX_THREAD_LATENCY_NUM && id>=0) {

        max_latency = DIFF_NS(extra_task_data[id].next_time,ts);
        if( fabs(max_latency) > thread_max_latency[id] )
            thread_max_latency[id] = fabs(max_latency);

    }

    rtapi_advance_time(&extra_task_data[task_id(task)].next_time,
               task->period, 0);

    if (ts.tv_sec > extra_task_data[task_id(task)].next_time.tv_sec
    || (ts.tv_sec == extra_task_data[task_id(task)].next_time.tv_sec
        && ts.tv_nsec > extra_task_data[task_id(task)].next_time.tv_nsec)) {

        extra_task_data[task_id(task)].failures++;

        if (extra_task_data[task_id(task)].failures == 1)
            msg_level = RTAPI_MSG_ERR;
        else if (extra_task_data[task_id(task)].failures < 10)
            msg_level = RTAPI_MSG_WARN;
        else
            msg_level = RTAPI_MSG_NONE;

//            if (msg_level != RTAPI_MSG_NONE) {
//                rtapi_print_msg
//                (msg_level, "ERROR: Missed scheduling deadline for task %s [%d times]\n"
//                 "Now is %ld.%09ld, deadline was %ld.%09ld\n"
//                 "\n",
//                 task->name, extra_task_data[task_id(task)].failures,
//                 (long)ts.tv_sec, (long)ts.tv_nsec,
//                 (long)extra_task_data[task_id(task)].next_time.tv_sec,
//                 (long)extra_task_data[task_id(task)].next_time.tv_nsec);
//            }

    }

    return 0;
}

long long  rtapi_get_time_hook(void) {

    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec * 1000 * 1000 * 1000 + tv.tv_usec * 1000;

}

//#define rdtscll(val) \
//         __asm__ __volatile__("rdtsc" : "=A" (val))

long long rtapi_get_clocks_hook(void) {

    //long long int retval;
    //rdtscll(retval);
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec * 1000 * 1000 * 1000 + tv.tv_usec * 1000;
    //return retval;

}

void rtapi_delay_hook(long int nsec)
{

    struct timespec t;
    t.tv_nsec = nsec;
    t.tv_sec = 0;
    clock_nanosleep(CLOCK_MONOTONIC, 0, &t, NULL);

}

#else  //using xenomai native api to do this


#include <native/mutex.h>
#include <native/task.h>
#include <rtdk.h>
#include <stdlib.h>		// abort()

#define MAX_ERRORS 3

/*  RTAPI task functions  */
RT_TASK ostask_array[RTAPI_MAX_TASKS + 1];

// this is needed due to the weirdness of the rt_task_self return value -
// it does _not_ match the address of the RT_TASK structure it was
// created with
RT_TASK *ostask_self[RTAPI_MAX_TASKS + 1];

static struct rt_stats_struct {
    int rt_wait_error;		/* release point missed */
    int rt_last_overrun;	/* last number of overruns reported by
                   Xenomai */
    int rt_total_overruns;	/* total number of overruns reported
                   by Xenomai */
} rt_stats;

//void init_rtapi_data_hook(rtapi_data_t * data) {
//    rt_stats.rt_wait_error = 0;
//    rt_stats.rt_last_overrun = 0;
//    rt_stats.rt_total_overruns = 0;
//}

int rtapi_task_delete_hook(task_data *task, int task_id) {
    int retval = 0;

    if ((retval = rt_task_delete( &ostask_array[task_id] )) < 0)
    rtapi_print_msg(RTAPI_MSG_ERR,"ERROR: rt_task_delete() = %d %s\n",
            retval, strerror(retval));

    return retval;
}

void rtapi_task_wrapper(void * task_id_hack) {
    int ret;
    int task_id = (int)(long) task_id_hack; // ugly, but I ain't gonna fix it
    task_data *task = &task_array[task_id];

    /* use the argument to point to the task data */
    if (task->period < period) task->period = period;
    task->ratio = task->period / period;
    rtapi_print_msg(RTAPI_MSG_DBG,
            "rtapi_task_wrapper: task %p '%s' period=%d "
            "prio=%d ratio=%d\n",
            task, task->name, task->ratio * period,
            task->prio, task->ratio);

    ostask_self[task_id]  = rt_task_self();

    if ((ret = rt_task_set_periodic(NULL, TM_NOW, task->ratio * period)) < 0) {
    rtapi_print_msg(RTAPI_MSG_ERR,
            "ERROR: rt_task_set_periodic(%d,%s) failed %d\n",
            task_id, task->name, ret);
    abort();
    }

    /* call the task function with the task argument */
    (task->taskcode) (task->arg);

    /* if the task ever returns, we record that fact */
    task->state = ENDED;
    rtapi_print_msg(RTAPI_MSG_ERR,
            "ERROR: reached end of wrapper for task %d '%s'\n",
            task_id, task->name);
}


int rtapi_task_start_hook(task_data *task, int task_id) {
    int which_cpu = 0;
    int retval;

#if !defined(BROKEN_XENOMAU_CPU_AFFINITY)
    // seems to work for me
    // not sure T_CPU(n) is possible - see:
    // http://www.xenomai.org/pipermail/xenomai-help/2010-09/msg00081.html

    if (task->cpu > -1)  // explicitly set by threads, motmod
    which_cpu = T_CPU(task->cpu);
#endif

    // sanity check
    if (strlen(task->name) > XNOBJECT_NAME_LEN-1) {
    rtapi_print_msg(RTAPI_MSG_ERR,
            "rt_task_create: task name '%s' too long for Xenomai, length limit %d chars\n",
            task->name, XNOBJECT_NAME_LEN-1 );
    return -EINVAL;
    }

    // http://www.xenomai.org/documentation/trunk/html/api/group__task.html#ga03387550693c21d0223f739570ccd992
    // Passing T_FPU|T_CPU(1) in the mode parameter thus creates a
    // task with FPU support enabled and which will be affine to CPU #1
    // the task will start out dormant; execution begins with rt_task_start()

    // since this is a usermode RT task, it will be FP anyway
    if ((retval = rt_task_create (&ostask_array[task_id], task->name,
                  task->stacksize, task->prio,
                  (task->uses_fp ? T_FPU : 0) | which_cpu )
     ) != 0) {
    rtapi_print_msg(RTAPI_MSG_ERR,
            "rt_task_create failed, rc = %d\n", retval );
    return -ENOMEM;
    }

    if ((retval = rt_task_start( &ostask_array[task_id],
                 rtapi_task_wrapper, (void *)(long)task_id))) {
    rtapi_print_msg(RTAPI_MSG_INFO,
            "rt_task_start failed, rc = %d\n", retval );
    return -ENOMEM;
    }
    return 0;
}

int rtapi_task_stop_hook(task_data *task, int task_id) {
    int retval;

    if ((retval = rt_task_delete( &ostask_array[task_id] )) < 0) {
    rtapi_print_msg(RTAPI_MSG_ERR,"rt_task_delete() = %d\n", retval);
    return retval;
    }

    return 0;
}

int rtapi_task_pause_hook(task_data *task, int task_id) {
    return rt_task_suspend( &ostask_array[task_id] );
}

int rtapi_task_resume_hook(task_data *task, int task_id) {
    return rt_task_resume( &ostask_array[task_id] );
}

void rtapi_wait_hook() {
    unsigned long overruns;
    static int error_printed = 0;
    int task_id;
    task_data *task;

    int result =  rt_task_wait_period(&overruns);
    switch (result) {
    case 0: // ok - no overruns;
    break;

    case -ETIMEDOUT: // release point was missed
    rt_stats.rt_wait_error++;
    rt_stats.rt_last_overrun = overruns;
    rt_stats.rt_total_overruns += overruns;

    if (error_printed < MAX_ERRORS) {
        task_id = rtapi_task_self();
        task = &(task_array[task_id]);

        rtapi_print_msg
        (RTAPI_MSG_ERR,
         "RTAPI: ERROR: Unexpected realtime delay on task %d - "
         "'%s' (%lu overruns)\n"
         "This Message will only display once per session.\n"
         "Run the Latency Test and resolve before continuing.\n",
         task_id, task->name, overruns);
    }
    error_printed++;
    if(error_printed == MAX_ERRORS)
        rtapi_print_msg(RTAPI_MSG_ERR,
                "RTAPI: (further messages will be suppressed)\n");
    break;

    case -EWOULDBLOCK:
    rtapi_print_msg(error_printed == 0 ? RTAPI_MSG_ERR : RTAPI_MSG_WARN,
            "RTAPI: ERROR: rt_task_wait_period() without "
            "previous rt_task_set_periodic()\n");
    error_printed++;
    break;

    case -EINTR:
    rtapi_print_msg(error_printed == 0 ? RTAPI_MSG_ERR : RTAPI_MSG_WARN,
            "RTAPI: ERROR: rt_task_unblock() called before "
            "release point\n");
    error_printed++;
    break;

    case -EPERM:
    rtapi_print_msg(error_printed == 0 ? RTAPI_MSG_ERR : RTAPI_MSG_WARN,
            "RTAPI: ERROR: cannot rt_task_wait_period() from "
            "this context\n");
    error_printed++;
    break;
    default:
    rtapi_print_msg(error_printed == 0 ? RTAPI_MSG_ERR : RTAPI_MSG_WARN,
            "RTAPI: ERROR: unknown error code %d\n", result);
    error_printed++;
    break;
    }
}

int rtapi_task_self_hook(void) {
    RT_TASK *ptr;
    int n;

    /* ask OS for pointer to its data for the current task */
    ptr = rt_task_self();

    if (ptr == NULL) {
    /* called from outside a task? */
    return -EINVAL;
    }
    /* find matching entry in task array */
    n = 1;
    while (n <= RTAPI_MAX_TASKS) {
    if (ostask_self[n] == ptr) {
        /* found a match */
        return n;
    }
    n++;
    }
    return -EINVAL;
}


int rtapi_task_new_hook(task_data *task, int task_id) {

    //void *stackaddr;
    //stackaddr = malloc(task->stacksize);
    //do nothing in xenomai2 native
    rt_stats.rt_wait_error = 0;
    rt_stats.rt_last_overrun = 0;
    rt_stats.rt_total_overruns = 0;
    return task_id;

}



long long  rtapi_get_time_hook(void) {
    /* The value returned will represent a count of jiffies if the
       native skin is bound to a periodic time base (see
       CONFIG_XENO_OPT_NATIVE_PERIOD), or nanoseconds otherwise.  */
    return rt_timer_read();
}

/* This returns a result in clocks instead of nS, and needs to be used
   with care around CPUs that change the clock speed to save power and
   other disgusting, non-realtime oriented behavior.  But at least it
   doesn't take a week every time you call it.
*/
long long rtapi_get_clocks_hook(void) {
    // Gilles says: do this - it's portable
    return rt_timer_tsc();
}

void rtapi_delay_hook(long int nsec) {
    long long int release = rt_timer_tsc() + nsec;
    while (rt_timer_tsc() < release);
}

#endif



