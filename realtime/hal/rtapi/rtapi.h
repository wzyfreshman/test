#ifndef RTAPI_H
#define RTAPI_H

/** This file, 'rtapi.h', defines the RTAPI for both realtime and
    non-realtime code.  This is a change from Rev 2, where the non-
    realtime (user space) API was defined in ulapi.h and used
    different function names.  The symbols RTAPI and ULAPI are used
    to determine which mode is being compiled, RTAPI for realtime
    and ULAPI for non-realtime.  The API is implemented in files
    named 'xxx_rtapi.c', where xxx is the RTOS.
*/

#include "rtapi_bitops.h"	/* atomic bit ops for lightweight mutex */
#include "rtapi_errno.h"

#include <stddef.h>
#include <linux/types.h>
#include <string.h>
#include <stdarg.h>
#include <sched.h>		/* for blocking when needed */

typedef __u8		u8;
typedef __u16		u16;
typedef __u32		u32;
typedef __u64		u64;
typedef __s8		s8;
typedef __s16		s16;
typedef __s32		s32;
typedef __s64		s64;


#define RTAPI_NAME_LEN   51	/* length for module, etc, names */

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
*                    INIT AND EXIT FUNCTIONS                           *
************************************************************************/
/* implemented in $THREADS.c (rt-preempt-user.c, xenomai-user.c, etc.) */

/** 'rtapi_init() sets up the RTAPI.  It must be called by any
    module that intends to use the API, before any other RTAPI
    calls.
    'modname' can optionally point to a string that identifies
    the module.  The string will be truncated at RTAPI_NAME_LEN
    characters.  If 'modname' is NULL, the system will assign a
    name.
    On success, returns a positive integer module ID, which is
    used for subsequent calls to rtapi_xxx_new, rtapi_xxx_delete,
    and rtapi_exit.  On failure, returns an error code as defined
    above.  Call only from within user or init/cleanup code, not
    from realtime tasks.
*/
extern int rtapi_init(const char *modname);

/** 'rtapi_exit()' shuts down and cleans up the RTAPI.  It must be
    called prior to exit by any module that called rtapi_init.
    'module_id' is the ID code returned when that module called
    rtapi_init().
    Returns a status code.  rtapi_exit() may attempt to clean up
    any tasks, shared memory, and other resources allocated by the
    module, but should not be relied on to replace proper cleanup
    code within the module.  Call only from within user or
    init/cleanup code, not from realtime tasks.
*/
extern int rtapi_exit(int module_id);

/***********************************************************************
*                      MESSAGING FUNCTIONS                             *
************************************************************************/
/* implemented in rtapi_msg.c */

/** 'rtapi_snprintf()' works like 'snprintf()' from the normal
    C library, except that it may not handle long longs.
    It is provided here because some RTOS kernels don't provide
    a realtime safe version of the function, and those that do don't provide
    support for printing doubles.  On systems with a
    good kernel snprintf(), or in user space, this function
    simply calls the normal snprintf().  May be called from user,
    init/cleanup, and realtime code.
*/
extern int rtapi_snprintf(char *buf, unsigned long int size,
	const char *fmt, ...)
	    __attribute__((format(printf,3,4)));

/** 'rtapi_vsnprintf()' works like 'vsnprintf()' from the normal
    C library, except that it doesn't handle long longs.
    It is provided here because some RTOS kernels don't provide
    a realtime safe version of the function, and those that do don't provide
    support for printing doubles.  On systems with a
    good kernel vsnprintf(), or in user space, this function
    simply calls the normal vsnrintf().  May be called from user,
    init/cleanup, and realtime code.
*/
extern int rtapi_vsnprintf(char *buf, unsigned long size,const char *fmt, va_list ap);

/** 'rtapi_print()' prints a printf style message.  Depending on the
    RTOS and whether the program is being compiled for user space
    or realtime, the message may be printed to stdout, stderr, or
    to a kernel message log, etc.  The calling syntax and format
    string is similar to printf except that floating point and
    longlongs are NOT supported in realtime and may not be supported
    in user space.  For some RTOS's, a 80 byte buffer is used, so the
    format line and arguments should not produce a line more than
    80 bytes long.  (The buffer is protected against overflow.)
    Does not block, but  can take a fairly long time, depending on
    the format string and OS.  May be called from user, init/cleanup,
    and realtime code.
*/
extern void rtapi_print(const char *fmt, ...)
	    __attribute__((format(printf,1,2)));

/** 'rtapi_print_msg()' prints a printf-style message when the level
    is less than or equal to the current message level set by
    rtapi_set_msg_level().  May be called from user, init/cleanup,
    and realtime code.
*/
typedef enum {
    RTAPI_MSG_NONE = 0,
    RTAPI_MSG_ERR,
    RTAPI_MSG_WARN,
    RTAPI_MSG_INFO,
    RTAPI_MSG_DBG,
    RTAPI_MSG_ALL
} msg_level_t;

extern void rtapi_print_msg(int level, const char *fmt, ...)
    __attribute__((format(printf,2,3)));

/** Set the maximum level of message to print.  In userspace code,
    each component has its own independent message level.  In realtime
    code, all components share a single message level.  Returns 0 for
    success or -EINVAL if the level is out of range. */
extern int rtapi_set_msg_level(int level);

/** Retrieve the message level set by the last call to rtapi_set_msg_level */
extern int rtapi_get_msg_level(void);

/** 'rtapi_get_msg_handler' and 'rtapi_set_msg_handler' access the function
    pointer used by rtapi_print and rtapi_print_msg.  By default, messages
    appear in the kernel log, but by replacing the handler a user of the rtapi
    library can send the messages to another destination.  Calling
    rtapi_set_msg_handler with NULL restores the default handler. Call from
    real-time init/cleanup code only.  When called from rtapi_print(),
    'level' is RTAPI_MSG_ALL, a level which should not normally be used
    with rtapi_print_msg().
*/
typedef void(*rtapi_msg_handler_t)(msg_level_t level, const char *fmt,
				       va_list ap);
extern void rtapi_set_msg_handler(rtapi_msg_handler_t handler);
extern rtapi_msg_handler_t rtapi_get_msg_handler(void);


/***********************************************************************
*                  LIGHTWEIGHT MUTEX FUNCTIONS                         *
************************************************************************/

/** These three functions provide a very simple way to do mutual
    exclusion around shared resources.  They do _not_ replace
    semaphores, and can result in significant slowdowns if contention
    is severe.  However, unlike semaphores they can be used from both
    user and kernel space.  The 'try' and 'give' functions are non-
    blocking, and can be used anywhere.  The 'get' function blocks if
    the mutex is already taken, and can only be used in user space or
    the init code of a realtime module, _not_ in realtime code.
*/

/** 'rtapi_mutex_give()' releases the mutex pointed to by 'mutex'.
    The release is unconditional, even if the caller doesn't have
    the mutex, it will be released.
*/
    static __inline__ void rtapi_mutex_give(unsigned long *mutex) {
	test_and_clear_bit(0, mutex);
    }
/** 'rtapi_mutex_try()' makes a non-blocking attempt to get the
    mutex pointed to by 'mutex'.  If the mutex was available, it
    returns 0 and the mutex is no longer available, since the
    caller now has it.  If the mutex is not available, it returns
    a non-zero value to indicate that someone else has the mutex.
    The programer is responsible for "doing the right thing" when
    it returns non-zero.  "Doing the right thing" almost certainly
    means doing something that will yield the CPU, so that whatever
    other process has the mutex gets a chance to release it.
*/ static __inline__ int rtapi_mutex_try(unsigned long *mutex) {
	return test_and_set_bit(0, mutex);
    }

/** 'rtapi_mutex_get()' gets the mutex pointed to by 'mutex',
    blocking if the mutex is not available.  Because of this,
    calling it from a realtime task is a "very bad" thing to
    do.
*/
static __inline__ void rtapi_mutex_get(unsigned long *mutex) {

    while (test_and_set_bit(0, mutex)) {
        sched_yield();
    }

}

/***********************************************************************
*                      TIME RELATED FUNCTIONS                          *
************************************************************************/
/* implemented in rtapi_time.c */

/** NOTE: These timing related functions are only available in
    realtime modules.  User processes may not call them!
*/


/** 'rtapi_clock_set_period() sets the basic time interval for realtime
    tasks.  All periodic tasks will run at an integer multiple of this
    period.  The first call to 'rtapi_clock_set_period() with 'nsecs'
    greater than zero will start the clock, using 'nsecs' as the clock
    period in nano-seconds.  Due to hardware and RTOS limitations, the
    actual period may not be exactly what was requested.  On success,
    the function will return the actual clock period if it is available,
    otherwise it returns the requested period.  If the requested period
    is outside the limits imposed by the hardware or RTOS, it returns
    -EINVAL and does not start the clock.  Once the clock is started,
    subsequent calls with non-zero 'nsecs' return -EINVAL and have
    no effect.  Calling 'rtapi_clock_set_period() with 'nsecs' set to
    zero queries the clock, returning the current clock period, or zero
    if the clock has not yet been started.  Call only from within
    init/cleanup code, not from realtime tasks.  This function is not
    available from user (non-realtime) code.
*/
extern long int rtapi_clock_set_period(long int nsecs);

/** rtapi_delay() is a simple delay.  It is intended only for short
    delays, since it simply loops, wasting CPU cycles.  'nsec' is the
    desired delay, in nano-seconds.  'rtapi_delay_max() returns the
    max delay permitted (usually approximately 1/4 of the clock period).
    Any call to 'rtapi_delay()' requesting a delay longer than the max
    will delay for the max time only.  'rtapi_delay_max()' should be
    called befure using 'rtapi_delay()' to make sure the required delays
    can be achieved.  The actual resolution of the delay may be as good
    as one nano-second, or as bad as a several microseconds.  May be
    called from init/cleanup code, and from within realtime tasks.
*/
extern void rtapi_delay(long int nsec);
extern long int rtapi_delay_max(void);


/** rtapi_get_time returns the current time in nanoseconds.  Depending
    on the RTOS, this may be time since boot, or time since the clock
    period was set, or some other time.  Its absolute value means
    nothing, but it is monotonically increasing and can be used to
    schedule future events, or to time the duration of some activity.
    Returns a 64 bit value.  The resolution of the returned value may
    be as good as one nano-second, or as poor as several microseconds.
    May be called from init/cleanup code, and from within realtime tasks.
    
    Experience has shown that the implementation of this function in
    some RTOS/Kernel combinations is horrible.  It can take up to 
    several microseconds, which is at least 100 times longer than it
    should, and perhaps a thousand times longer.  Use it only if you
    MUST have results in seconds instead of clocks, and use it sparingly.
    See rtapi_get_clocks() instead.

    Note that longlong math may be poorly supported on some platforms,
    especially in kernel space. Also note that rtapi_print() will NOT
    print longlongs.  Most time measurements are relative, and should
    be done like this:  deltat = (long int)(end_time - start_time);
    where end_time and start_time are longlong values returned from
    rtapi_get_time, and deltat is an ordinary long int (32 bits).
    This will work for times up to about 2 seconds.
*/
extern long long int rtapi_get_time(void);

/** rtapi_get_clocks returns the current time in CPU clocks.  It is 
    fast, since it just reads the TSC in the CPU instead of calling a
    kernel or RTOS function.  Of course, times measured in CPU clocks
    are not as convenient, but for relative measurements this works
    fine.  Its absolute value means nothing, but it is monotonically
    increasing* and can be used to schedule future events, or to time
    the duration of some activity.  (* on SMP machines, the two TSC's
    may get out of sync, so if a task reads the TSC, gets swapped to
    the other CPU, and reads again, the value may decrease.  RTAPI
    tries to force all RT tasks to run on one CPU.)
    Returns a 64 bit value.  The resolution of the returned value is
    one CPU clock, which is usually a few nanoseconds to a fraction of
    a nanosecond.
    May be called from init/cleanup code, and from within realtime tasks.
    
    Note that longlong math may be poorly supported on some platforms,
    especially in kernel space. Also note that rtapi_print() will NOT
    print longlongs.  Most time measurements are relative, and should
    be done like this:  deltat = (long int)(end_time - start_time);
    where end_time and start_time are longlong values returned from
    rtapi_get_time, and deltat is an ordinary long int (32 bits).
    This will work for times up to a second or so, depending on the
    CPU clock frequency.  It is best used for millisecond and 
    microsecond scale measurements though.
*/
extern long long int rtapi_get_clocks(void);


/***********************************************************************
*                     TASK RELATED FUNCTIONS                           *
************************************************************************/
/* implemented in rtapi_task.c */

/** NOTE: These realtime task related functions are only available in
    realtime modules.  User processes may not call them!
*/

/** NOTE: The RTAPI is designed to be a _simple_ API.  As such, it uses
    a very simple strategy to deal with SMP systems.  It ignores them!
    All tasks are scheduled on the first CPU.  That doesn't mean that
    additional CPUs are wasted, they will be used for non-realtime code.
*/

/** The 'rtapi_prio_xxxx()' functions provide a portable way to set
    task priority.  The mapping of actual priority to priority number
    depends on the RTOS.  Priorities range from 'rtapi_prio_lowest()'
    to 'rtapi_prio_highest()', inclusive. To use this API, use one of
    two methods:

    1) Set your lowest priority task to 'rtapi_prio_lowest()', and for
       each task of the next lowest priority, set their priorities to
       'rtapi_prio_next_higher(previous)'.

    2) Set your highest priority task to 'rtapi_prio_highest()', and
       for each task of the next highest priority, set their priorities
       to 'rtapi_prio_next_lower(previous)'.

    A high priority task will preempt a lower priority task.  The linux kernel
    and userspace are always a lower priority than all rtapi tasks.

    Call these functions only from within init/cleanup code, not from
    realtime tasks.
*/
    extern int rtapi_prio_highest(void);
    extern int rtapi_prio_lowest(void);
    extern int rtapi_prio_next_higher(int prio);
    extern int rtapi_prio_next_lower(int prio);



/** 'rtapi_task_new()' creates but does not start a realtime task.
    The task is created in the "paused" state.  To start it, call
    either rtapi_task_start() for periodic tasks, or rtapi_task_resume()
    for free-running tasks.
    On success, returns a positive integer task ID.  This ID is used
    for all subsequent calls that need to act on the task.  On failure,
    returns a negative error code as listed above.  'taskcode' is the
    name of a function taking one int and returning void, which contains
    the task code.  'arg' will be passed to 'taskcode' as an abitrary
    void pointer when the task is started, and can be used to pass
    any amount of data to the task (by pointing to a struct, or other
    such tricks).
    'prio' is the  priority, as determined by one of the priority
    functions above.  'owner' is the module ID of the module that
    is making the call (see rtapi_init).  'stacksize' is the amount
    of stack to be used for the task - be generous, hardware
    interrupts may use the same stack.  'uses_fp' is a flag that
    tells the OS whether the task uses floating point so it can
    save the FPU registers on a task switch.  Failing to save
    registers when needed causes the dreaded "NAN bug", so most
    tasks should set 'uses_fp' to RTAPI_USES_FP.  If a task
    definitely does not use floating point, setting 'uses_fp' to
    RTAPI_NO_FP saves a few microseconds per task switch.  Call
    only from within init/cleanup code, not from realtime tasks.
*/
#define RTAPI_NO_FP   0
#define RTAPI_USES_FP 1

extern int rtapi_task_new(void (*taskcode) (void *), void *arg,
			      int prio, int owner, unsigned long int stacksize, 
			      int uses_fp, char *name, int cpu_id);

/** 'rtapi_task_delete()' deletes a task.  'task_id' is a task ID
    from a previous call to rtapi_task_new().  It frees memory
    associated with 'task', and does any other cleanup needed.  If
    the task has been started, you should pause it before deleting
    it.  Returns a status code.  Call only from within init/cleanup
    code, not from realtime tasks.
*/
extern int rtapi_task_delete(int task_id);

/** 'rtapi_task_start()' starts a task in periodic mode.  'task_id' is
    a task ID from a call to rtapi_task_new().  The task must be in
    the "paused" state, or it will return -EINVAL.
    'period_nsec' is the task period in nanoseconds, which will be
    rounded to the nearest multiple of the global clock period.  A
    task period less than the clock period (including zero) will be
    set equal to the clock period.
    Call only from within init/cleanup code, not from realtime tasks.
*/
extern int rtapi_task_start(int task_id, unsigned long int period_nsec);

/** 'rtapi_wait()' suspends execution of the current task until the
    next period.  The task must be periodic, if not, the result is
    undefined.  The function will return at the beginning of the
    next period.  Call only from within a realtime task.
*/
extern void rtapi_wait(void);

/** 'rtapi_task_resume() starts a task in free-running mode. 'task_id'
    is a task ID from a call to rtapi_task_new().  The task must be in
    the "paused" state, or it will return -EINVAL.
    A free running task runs continuously until either:
    1) It is prempted by a higher priority task.  It will resume as
       soon as the higher priority task releases the CPU.
    2) It calls a blocking function, like rtapi_sem_take().  It will
       resume when the function unblocks.
    3) it is returned to the "paused" state by rtapi_task_pause().
    May be called from init/cleanup code, and from within realtime tasks.
*/
extern int rtapi_task_resume(int task_id);

/** 'rtapi_task_pause() causes 'task_id' to stop execution and change
    to the "paused" state.  'task_id' can be free-running or periodic.
    Note that rtapi_task_pause() may called from any task, or from init
    or cleanup code, not just from the task that is to be paused.
    The task will resume execution when either rtapi_task_resume() or
    rtapi_task_start() is called.  May be called from init/cleanup code,
    and from within realtime tasks.
*/
extern int rtapi_task_pause(int task_id);

/** 'rtapi_task_self()' returns the task ID of the current task.
    Call only from a realtime task.
*/
extern int rtapi_task_self(void);



/***********************************************************************
*                  SHARED MEMORY RELATED FUNCTIONS                     *
************************************************************************/
/* implemented in rtapi_shmem.c */

/** 'rtapi_shmem_new()' allocates a block of shared memory.  'key'
    identifies the memory block, and must be non-zero.  All modules
    wishing to access the same memory must use the same key.
    'module_id' is the ID of the module that is making the call (see
    rtapi_init).  The block will be at least 'size' bytes, and may
    be rounded up.  Allocating many small blocks may be very wasteful.
    When a particular block is allocated for the first time, the first
    4 bytes are zeroed.  Subsequent allocations of the same block
    by other modules or processes will not touch the contents of the
    block.  Applications can use those bytes to see if they need to
    initialize the block, or if another module already did so.
    On success, it returns a positive integer ID, which is used for
    all subsequent calls dealing with the block.  On failure it
    returns a negative error code.  Call only from within user or
    init/cleanup code, not from realtime tasks.
*/
extern int rtapi_shmem_new(int key, int module_id,unsigned long int size);

/** 'rtapi_shmem_delete()' frees the shared memory block associated
    with 'shmem_id'.  'module_id' is the ID of the calling module.
    Returns a status code.  Call only from within user or init/cleanup
    code, not from realtime tasks.
*/
extern int rtapi_shmem_delete(int shmem_id, int module_id);

/** 'rtapi_shmem_getptr()' sets '*ptr' to point to shared memory block
    associated with 'shmem_id'.  Returns a status code.  May be called
    from user code, init/cleanup code, or realtime tasks.
*/
extern int rtapi_shmem_getptr(int shmem_id, void **ptr);


/***********************************************************************
*                        I/O RELATED FUNCTIONS                         *
************************************************************************/

/** 'rtapi_outb() writes 'byte' to 'port'.  May be called from
    init/cleanup code, and from within realtime tasks.
    Note: This function does nothing on the simulated RTOS.
    Note: Many platforms provide an inline outb() that is faster.
*/
extern void rtapi_outb(unsigned char byte, unsigned int port);

/** 'rtapi_inb() gets a byte from 'port'.  Returns the byte.  May
    be called from init/cleanup code, and from within realtime tasks.
    Note: This function always returns zero on the simulated RTOS.
    Note: Many platforms provide an inline inb() that is faster.
*/
extern unsigned char rtapi_inb(unsigned int port);

/** 'rtapi_outw() writes 'word' to 'port'.  May be called from
    init/cleanup code, and from within realtime tasks.
    Note: This function does nothing on the simulated RTOS.
    Note: Many platforms provide an inline outw() that is faster.
*/
extern void rtapi_outw(unsigned short word, unsigned int port);

/** 'rtapi_inw() gets a word from 'port'.  Returns the word.  May
    be called from init/cleanup code, and from within realtime tasks.
    Note: This function always returns zero on the simulated RTOS.
    Note: Many platforms provide an inline inw() that is faster.
*/
extern unsigned short rtapi_inw(unsigned int port);


/* thread_max_latency,calculate eveythread latency,maxnum=
*/
#define MAX_THREAD_LATENCY_NUM 4
extern int  thread_max_latency[MAX_THREAD_LATENCY_NUM];

/***********************************************************************
*                      MODULE PARAMETER MACROS                         *
************************************************************************/

/* The API for module parameters has changed as the kernel evolved,
   and will probably change again.  We define our own macro for
   declaring parameters, so the code that uses RTAPI can ignore
   the issue.
*/

/* make sure a given kernel module is loaded.
   might be needed for some usermode PCI drivers
*/
int rtapi_assure_module_loaded(const char *module);

/** RTAPI_MP_INT() declares a single integer module parameter.
    RTAPI_MP_LONG() declares a single long module parameter.
    RTAPI_MP_STRING() declares a single string module parameter.
    RTAPI_MP_ARRAY_INT() declares an array of integer module parameters.
    RTAPI_MP_ARRAY_LONG() declares an array of long module parameters.
    RTAPI_MP_ARRAY_STRING() declares a single string module parameters.
    'var' is the name of the variable used for the parameter, which
    should be initialized with the default value(s) when it is declared.
    'descr' is a short description of the parameter.
    'num' is the number of elements in an array.
*/

//#if defined(BUILD_SYS_USER_DSO)
#define MODULE_INFO1(t, a, c) __attribute__((section(".modinfo"))) \
    t rtapi_info_##a = c; EXPORT_SYMBOL(rtapi_info_##a);
#define MODULE_INFO2(t, a, b, c) __attribute__((section(".modinfo"))) \
    t rtapi_info_##a##_##b = c; EXPORT_SYMBOL(rtapi_info_##a##_##b);
#define MODULE_PARM(v,t) MODULE_INFO2(const char*, type, v, t) MODULE_INFO2(void*, address, v, &v)
#define MODULE_PARM_DESC(v,t) MODULE_INFO2(const char*, description, v, t)
#define MODULE_LICENSE(s) MODULE_INFO1(const char*, license, s)
#define MODULE_AUTHOR(s) MODULE_INFO1(const char*, author, s)
#define MODULE_DESCRIPTION(s) MODULE_INFO1(const char*, description, s)
#define MODULE_SUPPORTED_DEVICE(s) MODULE_INFO1(const char*, supported_device, s)
#define EXPORT_SYMBOL(x) __attribute__((section(".rtapi_export"))) \
    char rtapi_exported_##x[] = #x;
#define EXPORT_SYMBOL_GPL(sym) EXPORT_SYMBOL(sym)
#define MODULE_DEVICE_TABLE(type, name)
//#endif


#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE 0
#endif


#define RTAPI_STRINGIFY(x)    #x

   
#define RTAPI_MP_INT(var,descr)    \
  MODULE_PARM(var,"i");            \
  MODULE_PARM_DESC(var,descr);

#define RTAPI_MP_LONG(var,descr)   \
  MODULE_PARM(var,"l");            \
  MODULE_PARM_DESC(var,descr);

#define RTAPI_MP_STRING(var,descr) \
  MODULE_PARM(var,"s");            \
  MODULE_PARM_DESC(var,descr);

#define RTAPI_MP_ARRAY_INT(var,num,descr)          \
  MODULE_PARM(var,"1-" RTAPI_STRINGIFY(num) "i");  \
  MODULE_PARM_DESC(var,descr);

#define RTAPI_MP_ARRAY_LONG(var,num,descr)         \
  MODULE_PARM(var,"1-" RTAPI_STRINGIFY(num) "l");  \
  MODULE_PARM_DESC(var,descr);

#define RTAPI_MP_ARRAY_STRING(var,num,descr)       \
  MODULE_PARM(var,"1-" RTAPI_STRINGIFY(num) "s");  \
  MODULE_PARM_DESC(var,descr);


extern long int simple_strtol(const char *nptr, char **endptr, int base);


#ifdef __cplusplus
}
#endif

#endif /* RTAPI_H */
