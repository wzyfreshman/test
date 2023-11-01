#ifndef RTAPI_IMPLEMENTATE_H
#define RTAPI_IMPLEMENTATE_H

#include "config.h"
#include <sched.h>		// sched_get_priority_*()
#include <pthread.h>		/* pthread_* */

#define PRIO_LOWEST 1
#define PRIO_HIGHEST 99

#define HAVE_RTAPI_TASK_NEW_HOOK
#define HAVE_RTAPI_TASK_DELETE_HOOK
#define HAVE_RTAPI_TASK_STOP_HOOK
#define HAVE_RTAPI_WAIT_HOOK
#define HAVE_RTAPI_TASK_START_HOOK

#define HAVE_RTAPI_TASK_FREE
#define HAVE_DROP_RESTORE_PRIVS


#define HAVE_RTAPI_GETTIME_HOOK
#define HAVE_RTAPI_GETCLOCKS_HOOK




#endif // RTAPI_IMPLEMENTATE_H
