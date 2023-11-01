#ifndef RTAPI_COMMON_H
#define RTAPI_COMMON_H

#include  <sched.h>

#include "rtapi_bitops.h"	/* test_bit() et al. */
#include "rtapi_implementate.h"

#ifndef NULL
#define NULL 0
#endif

#undef RTAPI_FIFO  // drop support for RTAPI fifos

/* maximum number of various resources */
#define RTAPI_MAX_MODULES	64
#define RTAPI_MAX_TASKS		64
#define RTAPI_MAX_SHMEMS	32

#define DEFAULT_MAX_DELAY	10000

/* random numbers used as signatures */
#define TASK_MAGIC		21979
#define MODULE_MAGIC		30812

#define MIN_STACKSIZE		32768

/* This file contains data structures that live in shared memory and
   are accessed by multiple different programs, both user processes
   and kernel modules.  If the structure layouts used by various
   programs don't match, that's bad.  So we have revision checking.
   Whenever a module or program is loaded, the rev_code is checked
   against the code in the shared memory area.  If they don't match,
   the rtapi_init() call will fail.

   Thread system header files should define the macro REV_CODE with a
   unique integer value.
*/

/* These structs hold data associated with objects like tasks, etc. */

typedef enum {
    NO_MODULE = 0,
    REALTIME,
    USERSPACE
} mod_type_t;

typedef struct {
    mod_type_t state;
    char name[RTAPI_NAME_LEN + 1];
} module_data;

typedef enum {
    EMPTY = 0,
    PAUSED,
    PERIODIC,
    FREERUN,
    ENDED,
    USERLAND,
    DELETE_LOCKED	// task ready to be deleted; mutex already obtained
} task_state_t;

typedef struct {
    char name[RTAPI_NAME_LEN];
    int magic;
    int uses_fp;
    size_t stacksize;
    int period;
    int ratio;
    task_state_t state;		/* task state */
    int prio;			/* priority */
    int owner;			/* owning module */
    void (*taskcode) (void *);	/* task code */
    void *arg;			/* task argument */
    int cpu;
    int unuse;          //for remove the c compiler warnning
} task_data;

typedef struct {
    int magic;			/* to check for valid handle */
    int key;			/* key to shared memory area */
    int id;			/* OS identifier for shmem */
    int count;                  /* count of maps in this process */
    int rtusers;		/* number of realtime modules using block */
    int ulusers;		/* number of user processes using block */
    unsigned long size;		/* size of shared memory area */
    _DECLARE_BITMAP(bitmap, RTAPI_MAX_SHMEMS+1); /* which modules are using block */
    void *mem;			/* pointer to the memory */
} shmem_data;

/* Master RTAPI data structure
   There is a single instance of this structure in the machine.
   It resides in shared memory, where it can be accessed by both
   realtime (RTAPI) and non-realtime (ULAPI) code.  It contains
   all information about the current state of RTAPI/ULAPI and
   the associated resources (tasks, etc.).
*/

typedef struct {
    int magic;			/* magic number to validate data */
    int rev_code;		/* revision code for matching */
    unsigned long mutex;	/* mutex against simultaneous access */
    int rt_module_count;	/* loaded RT modules */
    int ul_module_count;	/* running UL processes */
    int task_count;		/* task IDs in use */
    int shmem_count;		/* shared memory blocks in use */
    int timer_running;		/* state of HW timer */
    int rt_cpu;			/* CPU to use for RT tasks */
    long int timer_period;	/* HW timer period */
    module_data module_array[RTAPI_MAX_MODULES + 1];	/* data for modules */
    task_data task_array[RTAPI_MAX_TASKS + 1];	/* data for tasks */
    shmem_data shmem_array[RTAPI_MAX_SHMEMS + 1];	/* data for shared
							   memory */
} rtapi_data_t;


/* rtapi_common.c and implatementate.c*/

extern rtapi_data_t *rtapi_data;
extern void init_rtapi_data(rtapi_data_t * data);
extern task_data *task_array;
extern int msg_level;		/* needed in rtapi_proc.h */
extern int period;


/* rtapi_shmem.c */
#define RTAPI_KEY   0x90280A48	/* key used to open RTAPI shared memory */
#define RTAPI_MAGIC 0x12601409	/* magic number used to verify shmem */
#define SHMEM_MAGIC_DEL_LOCKED 25454  /* don't obtain mutex when deleting */
#define SHM_PERMISSIONS	0666

extern shmem_data *shmem_array;
extern void *shmem_addr_array[];
extern module_data *module_array;

#endif /* RTAPI_COMMON_H */
