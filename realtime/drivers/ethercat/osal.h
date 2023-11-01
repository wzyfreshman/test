/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

#ifndef _osal_
#define _osal_

#ifdef __cplusplus
extern "C"
{
#endif

//#include "osal_defs.h"
//#include <stdint.h>

/* General types */
#ifndef TRUE
#define TRUE                1
#endif
#ifndef FALSE
#define FALSE               0
#endif

#define boolean unsigned char

#define uint8_t unsigned char
#define int8_t  char

#define uint16_t unsigned short
#define int16_t  short

#define uint32_t unsigned int
#define int32_t  int

#define uint64_t unsigned long long
#define int64_t  long long;


#define uint8 unsigned char
#define int8  char

#define uint16 unsigned short
#define int16  short

#define uint32 unsigned int
#define int32  int

#define uint64 unsigned long long
#define int64  long long
 
#define EC_PRINT printk

/*
typedef uint8_t             boolean;
typedef int8_t              int8;
typedef int16_t             int16;
typedef int32_t             int32;
typedef uint8_t             uint8;
typedef uint16_t            uint16;
typedef uint32_t            uint32;
typedef int64_t             int64;
typedef uint64_t            uint64;
typedef float               float32;
typedef double              float64;
*/

typedef struct
{
    uint32 sec;     /*< Seconds elapsed since the Epoch (Jan 1, 1970) */
    uint32 usec;    /*< Microseconds elapsed since last second boundary */
} ec_timet;

typedef struct osal_timer
{
    ec_timet stop_time;
} osal_timert;

void osal_timer_start(osal_timert * self, uint32 timeout_us);
boolean osal_timer_is_expired(osal_timert * self);
int osal_usleep(uint32 usec);
int osal_nsleep(long long nsec);
int osal_nsleep_abs(long long nsec);
ec_timet osal_current_time(void);
long long osal_current_ns_time(void);
void osal_time_diff(ec_timet *start, ec_timet *end, ec_timet *diff);
int osal_thread_create(void *thandle, int stacksize, void *func, void *param);
int osal_thread_create_rt(void *thandle, int stacksize, void *func, void *param);

#ifdef __cplusplus
}
#endif

#endif
