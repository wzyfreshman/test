#include "config.h"		// build configuration
#include "rtapi.h"		// these functions
#include "rtapi_common.h"	// RTAPI macros and decls


#ifdef INVERSE_PRIO
#    define PRIO_INCR --
#    define PRIO_DECR ++
#    define PRIO_GT(a,b) (a<b)
#    define PRIO_LT(a,b) (a>b)
#else // normal priorities
#    define PRIO_INCR ++
#    define PRIO_DECR --
#    define PRIO_GT(a,b) (a>b)
#    define PRIO_LT(a,b) (a<b)
#endif

int rtapi_prio_highest(void) {
    return PRIO_HIGHEST;
}

int rtapi_prio_lowest(void) {
    return PRIO_LOWEST;
}

int rtapi_prio_next_higher(int prio) {

    /* next higher priority for arg */
    prio PRIO_INCR;

    /* return a valid priority for out of range arg */
    if (PRIO_GT(prio,rtapi_prio_highest()))
	return rtapi_prio_highest();
    if (PRIO_GT(rtapi_prio_lowest(),prio))
	return rtapi_prio_lowest();

    return prio;
}

int rtapi_prio_next_lower(int prio) {

    /* next lower priority for arg */
    prio PRIO_DECR;

    /* return a valid priority for out of range arg */
    if (PRIO_GT(prio,rtapi_prio_highest()))
	return rtapi_prio_highest();
    if (PRIO_GT(rtapi_prio_lowest(),prio))
	return rtapi_prio_lowest();

    return prio;
}


/* task setup and teardown functions */
#ifdef HAVE_RTAPI_TASK_NEW_HOOK
int rtapi_task_new_hook(task_data *task, int task_id);
#endif

int rtapi_task_new(void (*taskcode) (void*), void *arg,
		   int prio, int owner, unsigned long int stacksize, 
           int uses_fp, char *name, int cpu_id) {
    int task_id;
    int retval = 0;
    task_data *task;

    /* get the mutex */
    rtapi_mutex_get(&(rtapi_data->mutex));

    /* find an empty entry in the task array */
    task_id = 1; // tasks start at one!
    // go through task_array until an empty task slot is found
    while ((task_id < RTAPI_MAX_TASKS) &&
	   (task_array[task_id].magic == TASK_MAGIC))
	task_id++;
    // if task_array is full, release lock and return error
    if (task_id == RTAPI_MAX_TASKS) {
	rtapi_mutex_give(&(rtapi_data->mutex));
	return -ENOMEM;
    }
    task = &(task_array[task_id]);

    // if requested priority is invalid, release lock and return error
    if (PRIO_LT(prio,rtapi_prio_lowest()) ||
	PRIO_GT(prio,rtapi_prio_highest())) {
	
	rtapi_print_msg(RTAPI_MSG_ERR,
			"New task  %d  '%s': invalid priority %d "
			"(highest=%d lowest=%d)\n",
			task_id, name, prio,
			rtapi_prio_highest(),
			rtapi_prio_lowest());
	rtapi_mutex_give(&(rtapi_data->mutex));
	return -EINVAL;
    }

    // task slot found; reserve it and release lock
    rtapi_print_msg(RTAPI_MSG_DBG,
		    "Creating new task %d  '%s': "
		    "req prio %d (highest=%d lowest=%d)\n",
		    task_id, name, prio,
		    rtapi_prio_highest(),
		    rtapi_prio_lowest());
    task->magic = TASK_MAGIC;

    /* fill out task structure */
    if(stacksize < MIN_STACKSIZE) stacksize = MIN_STACKSIZE;
    task->owner = owner;
    task->arg = arg;
    task->stacksize = stacksize;
    task->taskcode = taskcode;
    task->prio = 99;
    task->uses_fp = uses_fp;
    /*  hopefully this works for userland thread systems too  */
    task->cpu = cpu_id > -1 ? cpu_id : rtapi_data->rt_cpu;
    rtapi_print_msg(RTAPI_MSG_DBG,
		    "Task CPU:  %d\n", task->cpu);
    /*    task->cpu = cpu_id;  */
    strncpy(task->name, name, sizeof(task->name));
    task->name[sizeof(task->name) - 1] = '\0';

    task->state = USERLAND;	// userland threads don't track this


#ifdef HAVE_RTAPI_TASK_NEW_HOOK
    retval = rtapi_task_new_hook(task,task_id);
#else
    rtapi_print_msg(RTAPI_MSG_ERR,
            "TASK rtapi_task_new Fail!!!!!: \n");
#endif

    if( retval != task_id ) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "TASK rtapi_task_new_hook Fail!!!!!: \n");
    }

    rtapi_data->task_count++;

    rtapi_mutex_give(&(rtapi_data->mutex));

    /* announce the birth of a brand new baby task */
    rtapi_print_msg(RTAPI_MSG_DBG,
	"RTAPI: task %02d installed by module %02d, priority %d, code: %p\n",
	task_id, task->owner, task->prio, taskcode);

    return task_id;
}


int rtapi_task_delete_hook(task_data *task, int task_id);

int rtapi_task_delete(int task_id) {

    task_data *task;
    int retval = 0;

    if(task_id < 0 || task_id >= RTAPI_MAX_TASKS) return -EINVAL;

    task = &(task_array[task_id]);
    /* validate task handle */
    if (task->magic != TASK_MAGIC)	// nothing to delete
	return -EINVAL;

    if (task->state != DELETE_LOCKED)	// we don't already hold mutex
	rtapi_mutex_get(&(rtapi_data->mutex));

    retval = rtapi_task_delete_hook(task,task_id);

    if (task->state != DELETE_LOCKED)	// we don't already hold mutex
	rtapi_mutex_give(&(rtapi_data->mutex));
    task->state = EMPTY;
    task->magic = 0;

    rtapi_print_msg(RTAPI_MSG_DBG, "rt_task_delete %d \"%s\"\n", task_id, 
		    task->name );

    return retval;
}


#ifdef HAVE_RTAPI_TASK_START_HOOK
int rtapi_task_start_hook(task_data *task, int task_id,
			  unsigned long int period_nsec);
#endif

int rtapi_task_start(int task_id, unsigned long int period_nsec) {
    task_data *task;

    if (task_id < 0 || task_id >= RTAPI_MAX_TASKS) return -EINVAL;
    
    task = &task_array[task_id];

    /* validate task handle */
    if (task->magic != TASK_MAGIC)
	return -EINVAL;

    if (period_nsec < (unsigned int)period) period_nsec = period;

    task->period = period_nsec;
    task->ratio = period_nsec / period;

    rtapi_print_msg(RTAPI_MSG_DBG,
		    "rtapi_task_start:  starting task %d '%s'\n",
		    task_id, task->name);
    rtapi_print_msg(RTAPI_MSG_DBG, "RTAPI: period_nsec: %ld\n", period_nsec);

    #ifdef HAVE_RTAPI_TASK_START_HOOK
    return rtapi_task_start_hook(task,task_id,0);
    #else
    return  -EINVAL;
    #endif

}

#ifdef HAVE_RTAPI_TASK_STOP_HOOK
int rtapi_task_stop_hook(task_data *task, int task_id);
#endif

int rtapi_task_stop(int task_id) {
    task_data *task;

    if(task_id < 0 || task_id >= RTAPI_MAX_TASKS) return -EINVAL;
    
    task = &task_array[task_id];

    /* validate task handle */
    if (task->magic != TASK_MAGIC)
	return -EINVAL;

#ifdef HAVE_RTAPI_TASK_STOP_HOOK
    rtapi_task_stop_hook(task,task_id);
#endif

    return 0;
}

#ifdef HAVE_RTAPI_TASK_PAUSE_HOOK
int rtapi_task_pause_hook(task_data *task, int task_id);
#endif

int rtapi_task_pause(int task_id) {

    task_data *task;
    if(task_id < 0 || task_id >= RTAPI_MAX_TASKS) return -EINVAL;

    task = &task_array[task_id];
  
    /* validate task handle */
    if (task->magic != TASK_MAGIC)
	return -EINVAL;

#ifdef HAVE_RTAPI_TASK_PAUSE_HOOK
    return rtapi_task_pause_hook(task,task_id);
#else
    return -ENOSYS;
#endif

}

#ifdef HAVE_RTAPI_WAIT_HOOK
extern void rtapi_wait_hook(void);
#endif

void rtapi_wait(void) {
#ifdef HAVE_RTAPI_WAIT_HOOK
    rtapi_wait_hook();
#endif
    return;
}

#ifdef HAVE_RTAPI_TASK_RESUME_HOOK
int rtapi_task_resume_hook(task_data *task, int task_id);
#endif

int rtapi_task_resume(int task_id) {

    task_data *task;

    if(task_id < 0 || task_id >= RTAPI_MAX_TASKS) return -EINVAL;
    
    task = &task_array[task_id];
  
    /* validate task handle */
    if (task->magic != TASK_MAGIC)
	return -EINVAL;

#ifdef HAVE_RTAPI_TASK_RESUME_HOOK
    return rtapi_task_resume_hook(task,task_id);
#else
    return -ENOSYS;
#endif

}


/* not defined in rt-preempt */
#ifdef HAVE_RTAPI_TASK_SELF_HOOK
int rtapi_task_self_hook(void);
#endif

int rtapi_task_self(void) {
#ifdef HAVE_RTAPI_TASK_SELF_HOOK
    return rtapi_task_self_hook();
#else
    /* not implemented */
    return -EINVAL;
#endif
}

