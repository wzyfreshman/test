/********************************************************************
 * Description: motion.c
 *   Main module initialisation and cleanup routines.
 *
 * Author:
 * License: GPL Version 2
 * System: Linux
 *
 * Copyright (c) 2004 All rights reserved.
 ********************************************************************/

//ghf-2015/1/17
//#include <linux/timer.h>
//#include <linux/timex.h>
#include <linux/random.h>
#include <unistd.h>             // unlink()
#include <stdio.h>

#include <stdarg.h>
#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */
#include "rtapi_string.h"       /* memset */
#include "hal.h"		/* decls for HAL implementation */
#include "emcmotglb.h"
#include "motion.h"
#include "motion_debug.h"
#include "motion_struct.h"
#include "mot_priv.h"
#include "rtapi_math.h"
#include "des.h"
#include <float.h>


extern int writeRealData(unsigned short addrin,unsigned char *data,int size) ;
extern void readRealData(unsigned short addrin,unsigned char *data,int size);


#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/uio.h>

// Mark strings for translation, but defer translation to userspace
#define _(s) (s)

/***********************************************************************
 *                    KERNEL MODULE PARAMETERS                          *
 ************************************************************************/

static int key = DEFAULT_SHMEM_KEY; /* the shared memory key, default value */

/* module information */
/* register symbols to be modified by insmod
 see "Linux Device Drivers", Alessandro Rubini, p. 385
 (p.42-44 in 2nd edition) */
MODULE_AUTHOR("Matt Shaver/John Kasunich");
MODULE_DESCRIPTION("Motion Controller for EMC");
MODULE_LICENSE("GPL");

/*! \todo FIXME - find a better way to do this */
int DEBUG_MOTION = 0;
RTAPI_MP_INT(DEBUG_MOTION, "debug motion");

/* RTAPI shmem key - for comms with higher level user space stuff */
RTAPI_MP_INT(key, "shared memory key");

static long base_period_nsec = 0; /* fastest thread period */
RTAPI_MP_LONG(base_period_nsec, "fastest thread period (nsecs)");
int base_thread_fp = 0; /* default is no floating point in base thread */
RTAPI_MP_INT(base_thread_fp, "floating point in base thread?");
static long servo_period_nsec = 2000000; /* servo thread period */
RTAPI_MP_LONG(servo_period_nsec, "servo thread period (nsecs)");
static long traj_period_nsec = 20000000; /* trajectory planner period */
RTAPI_MP_LONG(traj_period_nsec, "trajectory planner period (nsecs)");
int num_joints = EMCMOT_MAX_JOINTS; /* default number of joints present */
RTAPI_MP_INT(num_joints, "number of joints");
int num_dio = 64; /* default number of motion synched DIO */
RTAPI_MP_INT(num_dio, "number of digital inputs/outputs");
int num_aio = 64; /* default number of motion synched AIO */
RTAPI_MP_INT(num_aio, "number of analog inputs/outputs");

/***********************************************************************
 *                  GLOBAL VARIABLE DEFINITIONS                         *
 ************************************************************************/

/* pointer to emcmot_hal_data_t struct in HAL shmem, with all HAL data */
emcmot_hal_data_t *emcmot_hal_data = 0;

/* pointer to joint data */
emcmot_joint_t *joints = 0;

#ifndef STRUCTS_IN_SHMEM
/* allocate array for joint data */
emcmot_joint_t joint_array[EMCMOT_MAX_JOINTS];
#endif

int mot_comp_id; /* component ID for motion module */
int first_pass = 1; /* used to set initial conditions */
int kinType = 0;



typedef struct version_struct {
    char errorbuf[1024*10];
    int ethercatVersion;
    int IOboardVersion;
    int segVersion;
} version_struct;
struct version_struct *versiondata;

emcmot_struct_t *emcmotStruct = 0;
/* ptrs to either buffered copies or direct memory for
 command and status */
struct emcmot_command_t *emcmotCommand = 0;
struct emcmot_status_t *emcmotStatus = 0;
struct emcmot_config_t *emcmotConfig = 0;
struct emcmot_debug_t *emcmotDebug = 0;
struct emcmot_internal_t *emcmotInternal = 0;
struct emcmot_error_t *emcmotError = 0; /* unused for RT_FIFO */

struct emcdata_struct *motData;

/***********************************************************************
 *                  LOCAL VARIABLE DECLARATIONS                         *
 ************************************************************************/

/* RTAPI shmem ID - for comms with higher level user space stuff */
static int emc_shmem_id; /* the shared memory ID */
static int emcdata_id;
/***********************************************************************
 *                   LOCAL FUNCTION PROTOTYPES                          *
 ************************************************************************/

/* init_hal_io() exports HAL pins and parameters making data from
 the realtime control module visible and usable by the world
 */
static int init_hal_io(void);

/* functions called by init_hal_io() */
static int export_joint(int num, joint_hal_t * addr);

/* init_comm_buffers() allocates and initializes the command,
 status, and error buffers used to communicate witht the user
 space parts of emc.
 */
//static int init_comm_buffers(void);
static int func_7(void);

/* functions called by init_comm_buffers() */

/* init_threads() creates realtime threads, exports functions to
 do the realtime control, and adds the functions to the threads.
 */
static int func_8(void);

/* functions called by init_threads() */
//static int setTrajCycleTime(double secs);
//static int setServoCycleTime(double secs);

static int func_4(double secs);
static int func_5(double secs);

//add by ghf 2014-1-25
SEGMENTQUEUE queue;
SEGMENT sqSpace[DEFAULT_TC_QUEUE_SIZE];

#define BASEADDR            0X378
#define STATUSPORT(addr)    addr+1      //SPP状态只能读
#define CONTROLPORT(addr)   addr+2      //SPP控制寄存器 只能写
#define ADDRPORT(addr)      addr+3      //EPP地址寄存器 可读写
#define DATAPORT(addr)      addr+4      //EPP数据寄存器 可读写
static int myClrTimeout(unsigned int port_addr)
{
    unsigned char r;

    r = inb(STATUSPORT(port_addr));

    if  (!(r & 0x01)) { //如果r的最低位为0，直接返回
        return 0;
    }

    r = inb(STATUSPORT(port_addr));//如果r的最低位为1，写1清零
    outb(r | 0x01, STATUSPORT(port_addr)); /* Some reset by writing 1 */
    r = inb(STATUSPORT(port_addr));

    return !(r & 0x01);
}

static unsigned char mySelRead(unsigned char epp_addr, unsigned int port_addr)
{
    myClrTimeout(port_addr);
    /* set port direction to output */
    outb(0x04,CONTROLPORT(port_addr));//设置方向为输出
    /* write epp address to port */
    outb(epp_addr,ADDRPORT(port_addr)); //写地址
    /* set port direction to input */
    outb(0x24,CONTROLPORT(port_addr)); //设置方向为输入

    return 0;
}

static unsigned char myReadMore(unsigned int port_addr)
{
    unsigned char b;
    b = inb(DATAPORT(port_addr));
    return b;
}

static void mySelWrt(unsigned char epp_addr, unsigned int port_addr)
{
    myClrTimeout(port_addr);
    /* set port direction to output */
    outb(0x04,CONTROLPORT(port_addr));//设置方向为输出
    /* write epp address to port */
    outb(epp_addr,ADDRPORT(port_addr)); //写地址
    return;
}

static void myWrtMore(unsigned char byte, unsigned int port_addr)
{
    outb(byte,DATAPORT(port_addr));
    return;
}

unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
/* CRC低位字节值表*/
char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

/* 要进行CRC校验的消息 */ /* 消息中字节数 */
void calculateCrc16(unsigned char *puchMsg,unsigned char *out,unsigned short usDataLen )
{
    unsigned char uchCRCHi = 0xFF ; /* 高CRC字节初始化 */
    unsigned char uchCRCLo = 0xFF ; /* 低CRC字节初始化 */
    unsigned uIndex ; /* CRC循环中的索引 */
    while (usDataLen--) /* 传输消息缓冲区 */
    {
        uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex] ;
    }
    //return (uchCRCHi < < 8 | uchCRCLo) ;
   // return (uchCRCHi<<8 | uchCRCLo);

    out[0] = uchCRCHi;
    out[1] = uchCRCLo;

}
#define SEC_PER_DAY        86400
#define SEC_PER_HOUR    3600
#define DAY_PER_4YEAR    1461
static const int __day_in_leapyear[13]        = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
static const int __day_in_commonyear[13]    = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
static const int __day_in_4year[5]            = { 0, 365, 730, 1096, 1461};
int secToRTCtime(long n_Sec, struct rtc_time *rtc)
{
        struct rtc_time p_Time;
        long t;
        int i;
        const int * day_in_year;
        if( n_Sec < 0 )
            return 0;
        p_Time.tm_year = 1970;
        p_Time.tm_mon = 1;
        p_Time.tm_mday = 1;
        p_Time.tm_hour = 0;
        p_Time.tm_min = 0;
        p_Time.tm_sec = 0;

        t = n_Sec % SEC_PER_DAY;
        p_Time.tm_hour = t / SEC_PER_HOUR;
        t %= SEC_PER_HOUR;
        p_Time.tm_min = t / 60;
        p_Time.tm_sec = t % 60;


        t = n_Sec / SEC_PER_DAY;
        p_Time.tm_year += 4 * (t/DAY_PER_4YEAR);
        t %= DAY_PER_4YEAR;
        for(i=0;i<4;i++)
        {
            if(t >= __day_in_4year[i] && t < __day_in_4year[i+1])
                break;
        }
        p_Time.tm_year += i;
        if(i == 2)
            day_in_year = __day_in_leapyear;
        else
            day_in_year = __day_in_commonyear;
        t -= __day_in_4year[i];

        for(i=0;i<12;i++)
        {
            if(t >= day_in_year[i] && t < day_in_year[i+1])
                break;
        }
        p_Time.tm_mon += i; //算月份

        t -= day_in_year[i];
        p_Time.tm_mday += t;
        rtc->tm_year = p_Time.tm_year;
        rtc->tm_mon = p_Time.tm_mon;
        rtc->tm_mday = p_Time.tm_mday;
        rtc->tm_hour = p_Time.tm_hour;
        rtc->tm_min = p_Time.tm_min;
        rtc->tm_sec = p_Time.tm_sec;
        return 1;
}

#define MINUTE 60	// 1 分钟的秒数。
#define HOUR (60*MINUTE)	// 1 小时的秒数。
#define DAY (24*HOUR)	// 1 天的秒数。
#define YEAR (365*DAY)	// 1 年的秒数。
/* interestingly, we assume leap-years */
/* 有趣的是我们考虑进了闰年 */
// 下面以年为界限，定义了每个月开始时的秒数时间数组。
static int month[12] = {
  0,
  DAY * (31),
  DAY * (31 + 29),
  DAY * (31 + 29 + 31),
  DAY * (31 + 29 + 31 + 30),
  DAY * (31 + 29 + 31 + 30 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
};
// 该函数计算从1970 年1 月1 日0 时起到开机当日经过的秒数，作为开机时间。
long kernel_mktime (struct rtc_time *tm)
{
  long res;
  int year;
  year = tm->tm_year - 70;	// 从70 年到现在经过的年数(2 位表示方式)，
  // 因此会有2000 年问题。
  /* magic offsets (y+1) needed to get leapyears right. */
  /* 为了获得正确的闰年数，这里需要这样一个魔幻偏值(y+1) */
  res = YEAR * year + DAY * ((year + 1) / 4);	// 这些年经过的秒数时间 + 每个闰年时多1 天
  if( tm->tm_mon < 0 ) tm->tm_mon = 0;
  if( tm->tm_mon > 12 ) tm->tm_mon = 12;

  res += month[tm->tm_mon];	// 的秒数时间，在加上当年到当月时的秒数。
  /* and (y+2) here. If it wasn't a leap-year, we have to adjust */
  /* 以及(y+2)。如果(y+2)不是闰年，那么我们就必须进行调整(减去一天的秒数时间)。 */
  if (tm->tm_mon > 1 && ((year + 2) % 4))
    res -= DAY;
  res += DAY * (tm->tm_mday - 1);	// 再加上本月过去的天数的秒数时间。
  res += HOUR * tm->tm_hour;	// 再加上当天过去的小时数的秒数时间。
  res += MINUTE * tm->tm_min;	// 再加上1 小时内过去的分钟数的秒数时间。
  res += tm->tm_sec;	// 再加上1 分钟内已过的秒数。
  return res;	// 即等于从1970 年以来经过的秒数时间。
}

long mydifftime(struct rtc_time *tm1,struct rtc_time *tm2){
    long res;
    res=kernel_mktime(tm1) - kernel_mktime(tm2) ;
    return res;
}

static int getManuTimeTm(unsigned char *from,unsigned char *key_buf,struct rtc_time *tm) {

    unsigned char buf1[8];
    unsigned char buf2[2];
    int i,ret = -1;
    int date[5];

    des3_context ctx;//用于解密用的密钥结构体
    hal_ppmc_1(&ctx,key_buf);//setkey
    hal_ppmc_3(&ctx,from,buf1);

    calculateCrc16(buf1,buf2,6);

    if( buf1[6] == buf2[0] &&  buf1[7] == buf2[1]  ) {
        ret = 0;
        for( i = 0; i < 5; i++) {
            buf1[i] ^= buf1[5];
        }

        date[0]=buf1[0]*256+buf1[1];
        date[1]=buf1[2];
        date[2]=buf1[3];
        date[3]=buf1[4];
        date[4]=0;

        tm->tm_year=date[0]-1900;
        tm->tm_mon=date[1]-1;
        tm->tm_mday=date[2];
        tm->tm_hour=date[3];
        tm->tm_min=date[4];
        tm->tm_sec=0;
    }

    for( i = 0; i < 4; i++)
        emcmotStatus->MachineLockdate[i] = date[i];

    if( date[0]>=2000 && date[0]<=2500 \
        && date[1]>=1 && date[1]<=12 \
        && date[2]>=1 && date[2]<=31\
        && date[3]>=0 && date[3]<=24) {     //得到的到期时间正常
        return ret;
    } else if(date[0] == 0xFFFF) {      //永久有效
        return 1;
    } else {                //数据异常
        return -1;
    }

}

static int func_1(void) {
    long int max_ns_delay;  //2014-8-12
    unsigned char testbuf[400]; //2014-8-12
    unsigned char revbuf[400];//2015/1/4
    unsigned char realtestbuf[153]; //2014-8-15

    unsigned char manu_time_data[16]; //2016-3-2
    unsigned char manu_time_data_rv[16]; //2016-3-2
    unsigned char manu_time[16]; //2016-3-2
    unsigned char manu_code[16]; //2016-3-2
    int manu_time_return = -1;

    unsigned int currentdate[8];//2015/1/5
    unsigned int writedate[8];//first write date
    unsigned int freshdate[8];//write current date to single chip
    unsigned int enddate[8];//end date

    unsigned char tmp[8];
    int hours;
    int ret = 0;
    int a;
    int fd;
    int retval;



    //ghf-2015/1/4
    //struct timex  txc;
    struct rtc_time tm;
    struct rtc_time currenttm;
    struct rtc_time endtm;
    struct rtc_time freshtm;
    struct rtc_time writetm;
    struct rtc_time manutm;

    fd = open("/dev/rtc0" , 0);//only_rd
    if (fd ==  -1) {
        rtapi_print("open /dev/rtc0 error\n");
        return -1;
    }

    retval = ioctl(fd, RTC_RD_TIME, &tm);
    if (retval == -1) {
        rtapi_print("RTC_RD_TIME ioctl");
        return -1;
    }

    close(fd);

    //  RTC读上来的时间是 1900到当前年份，月份从0开始 (HONG-2021-09-25)
    rtapi_print("bios RTC time  :%d-%d-%d-%d-%d-%d\n",tm.tm_year,tm.tm_mon,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
    long nowsec = kernel_mktime(&tm);
    nowsec += 8*3600;       //时区问题，相差8小时

    struct rtc_time realrtc;
    secToRTCtime(nowsec,&realrtc);
    currentdate[0] = realrtc.tm_year;
    currentdate[1] = realrtc.tm_mon;
    currentdate[2] = realrtc.tm_mday;
    currentdate[3] = realrtc.tm_hour;
    currentdate[4] = realrtc.tm_min;
    currentdate[5] = realrtc.tm_sec;
    int i = 0;
    for(;i<8;i++) {
        emcmotStatus->Currentdate[i] = currentdate[i];
    }


    emcmotStatus->dateerror = 0;
    emcmotStatus->timetype = 0;

    rtapi_print("Current Time sec:%ld :%d-%d-%d-%d-%d-%d\n",nowsec,realrtc.tm_year,realrtc.tm_mon,realrtc.tm_mday,realrtc.tm_hour,realrtc.tm_min,realrtc.tm_sec);


    //read the mcu chip id
    ret = readChipId(emcmotStatus->encrychipid);
    if(ret != 0 && SIMULATE == 0) {
        reportError(_("Encoder Chip reading error"));
    }



    // read the mcu software version
    LKT4200_reset(emcmotStatus->fpgasn);

    // read the cpuid key
    readRealData(0x0020,realtestbuf,19*8);

    //****************系统厂商的截止日期******************0x0020+19*8
    readRealData(0x0020+19*8,manu_time,8);

    //****************系统厂商的校验号码**********************
    readRealData(0x0020+16*8,manu_code,8);

    manu_code[6] = 4;
    manu_code[7] = 4;

    manu_time_return = getManuTimeTm(manu_time,manu_code,&manutm);

    for(a=0;a<64;a++) {
        emcmotStatus->encryptbuf[a] = realtestbuf[a];
    }

//    for(a=64;a<72;a++) {
//        emcmotStatus->hddid[a-64] = realtestbuf[a];
//    }

    for(a=72;a<80;a++) {
        emcmotStatus->customid[a-72] = realtestbuf[a];
    }

    //80-83是剩余时间，84-85是硬件校验，86是单片机程序版本号，ghf-2014/9/27
    for(a=80;a<88;a++) {
        emcmotStatus->rlefttime[a-80] = realtestbuf[a];
        //rtapi_print("emcmotStatus->rlefttime[%d]=%x\n",a-80,emcmotStatus->rlefttime[a-80]);
    }


    enddate[0]=realtestbuf[88]*256+realtestbuf[89];  //year
    enddate[1]=realtestbuf[90];     //mon
    enddate[2]=realtestbuf[91];     //day
    enddate[3]=12;
    enddate[4]=0;
    enddate[5]=0;

    int k = 0;
    for(;k<8;k++) {
        emcmotStatus->Enddate[k] = enddate[k];
    }

    int j=0;
    for(j=0;j<8;j++) {
//        printf("%.2X", emcmotStatus->Enddate[j]);
    }
//    printf(" read enddate ret %d\n",ret);

    endtm.tm_year=enddate[0]-1900;
    endtm.tm_mon=enddate[1]-1;
    endtm.tm_mday=enddate[2];
    endtm.tm_hour=enddate[3];
    endtm.tm_min=enddate[4];
    endtm.tm_sec=0;


    freshdate[0]=realtestbuf[96]*256+realtestbuf[97];
    freshdate[1]=realtestbuf[98];
    freshdate[2]=realtestbuf[99];
    freshdate[3]=realtestbuf[100];
    freshdate[4]=realtestbuf[101];

    freshtm.tm_year=freshdate[0]-1900;
    freshtm.tm_mon=freshdate[1]-1;
    freshtm.tm_mday=freshdate[2];
    freshtm.tm_hour=freshdate[3];
    freshtm.tm_min=freshdate[4];
    freshtm.tm_sec=0;

    currenttm.tm_year=currentdate[0]-1900;
    currenttm.tm_mon=currentdate[1]-1;
    currenttm.tm_mday=currentdate[2];
    currenttm.tm_hour=currentdate[3];
    currenttm.tm_min=currentdate[4];
    currenttm.tm_sec=0;

    writedate[0]=realtestbuf[104]*256+realtestbuf[105];
    writedate[1]=realtestbuf[106];
    writedate[2]=realtestbuf[107];
    writedate[3]=realtestbuf[108];
    writedate[4]=realtestbuf[109];
    //rtapi_print("writedate:%d-%d-%d , %d-%d\n",writedate[0],writedate[1],writedate[2],writedate[3],writedate[4]);

    writetm.tm_year=writedate[0]-1900;
    writetm.tm_mon=writedate[1]-1;
    writetm.tm_mday=writedate[2];
    writetm.tm_hour=writedate[3];
    writetm.tm_min=writedate[4];
    writetm.tm_sec=0;

    //暂时没有用
    unsigned int remainhours;
    remainhours = (realtestbuf[112]*256*256*256+realtestbuf[113]*256*256+realtestbuf[114]*256+realtestbuf[115])/12;
    rtapi_print("remainhours=%d\n",remainhours);

    for(a=120;a<128;a++) {
         emcmotStatus->tregenc[a-120] = realtestbuf[a];
         //rtapi_print("emcmotStatus->tregenc[%d]=%x\n",a-120,emcmotStatus->tregenc[a-120]);
     }

    for(a=128;a<136;a++) {
         emcmotStatus->checkandsys[a-128] = realtestbuf[a];
         //rtapi_print("emcmotStatus->checkandsys[%d]=%x\n",a-128,emcmotStatus->checkandsys[a-128]);
     }

    emcmotStatus->timetype = realtestbuf[136];
    emcmotStatus->remaintimestatus = realtestbuf[137];

    rtapi_print("currentdate:%d-%d-%d , %d-%d\n",currenttm.tm_year,currenttm.tm_mon,currenttm.tm_mday,currenttm.tm_hour,currenttm.tm_min);
    rtapi_print("enddate:%d-%d-%d , %d-%d\n",endtm.tm_year,endtm.tm_mon,endtm.tm_mday,endtm.tm_hour,endtm.tm_min);
    rtapi_print("manutm:%d-%d-%d , %d-%d\n",manutm.tm_year,manutm.tm_mon,manutm.tm_mday,manutm.tm_hour,manutm.tm_min);
    rtapi_print("writetm:%d-%d-%d , %d-%d\n",writetm.tm_year,writetm.tm_mon,writetm.tm_mday,writetm.tm_hour,writetm.tm_min);
    rtapi_print("freshtm:%d-%d-%d , %d-%d\n",freshtm.tm_year,freshtm.tm_mon,freshtm.tm_mday,freshtm.tm_hour,freshtm.tm_min);
    printf("Now in motion and get the lefttime is %ld emcmotStatus->timetype=%d remaintimestatus=%d   \n",(kernel_mktime(&currenttm)-kernel_mktime(&endtm)),emcmotStatus->timetype,emcmotStatus->remaintimestatus);

    if( emcmotStatus->timetype > 0) {
//    if( (kernel_mktime(&currenttm)-kernel_mktime(&writetm)) >= 0 ) {
          if( (kernel_mktime(&currenttm)-kernel_mktime(&endtm)) <=0 ) {
              if( (kernel_mktime(&currenttm)-kernel_mktime(&freshtm)) <=-20*60) {
                  //当前时间与上次写入时间滞后了20min
                 emcmotStatus->dateerror = -1;
              } else {
                 //***should fresh    freshtime and maxlefttime

                 tmp[0] = currentdate[0] >> 8;
                 tmp[1] = currentdate[0] & 0x00ff;
                 tmp[2] = currentdate[1];
                 tmp[3] = currentdate[2];
                 tmp[4] = currentdate[3];
                 tmp[5] = currentdate[4];

                 writeRealData(0x0080,tmp,8);


                 if(emcmotStatus->timetype > 0) {   //timetype读上来会突变为82,处理一下
                     emcmotStatus->timetype = 1;
                 } else
                     emcmotStatus->timetype = 0;
                 tmp[0] = emcmotStatus->timetype;
                 tmp[1] = 0;                        //这个为emcmotStatus->remaintimestatus 标志位，未到期，写零下去
                 tmp[2] = 0;
                 tmp[3] = 0;
                 writeRealData(0x00a8,tmp,4);
              }
          } else {
                emcmotStatus->dateerror = -2;
          }
//        } else  {     //  不检测出密码时的时间，跟当前时间没关系 (HONG-2021-09-26)
//
//            emcmotStatus->dateerror = -3
//        }
    } else { //  永久有效 (HONG-2021-09-25)
        tmp[0] = 0;
        tmp[1] = 0;                        //这个为emcmotStatus->remaintimestatus 标志位，未到期，写零下去
        tmp[2] = 0;
        tmp[3] = 0;
        writeRealData(0x00a8,tmp,4);
        emcmotStatus->remaintimestatus = 0;
    }


    if(emcmotStatus->dateerror<0) { //出错将终止日期写入刷新日期，并将剩余时间设为0

        printf("Now I in set dateerror to err is %d\n",emcmotStatus->remaintimestatus);
        memcpy(tmp,realtestbuf+88,4);
        tmp[4]=12;
        tmp[5]=0;
        writeRealData(0x0080,tmp,8);

        hours = 0;
        tmp[0] = hours>>24;
        tmp[1] = (hours&0x00ffffff)>>16;
        tmp[2] = (hours&0x0000ffff)>>8;
        tmp[3] = (hours&0x000000ff)>>0;
        writeRealData(0x0090,tmp,4);

        //日期到期后写入到期标志

        emcmotStatus->timetype = 1;
        tmp[0] = emcmotStatus->timetype;
        tmp[1] = 1;
        tmp[2] = 0;
        tmp[3] = 0;
        writeRealData(0x00a8,tmp,4);
        emcmotStatus->remaintimestatus = 1;
    }

    if( manu_time_return == 1 ) {

    }else if( manu_time_return == -1 )
        emcmotStatus->dateerror = - 4;
    else if( (kernel_mktime(&currenttm)-kernel_mktime(&manutm)) >= 0 ) {
        emcmotStatus->dateerror = - 4;
    }

    printf("Now dateerror %d  remaintimestatus:%d  manutm:%ld\n",emcmotStatus->dateerror,emcmotStatus->remaintimestatus,(kernel_mktime(&currenttm)-kernel_mktime(&manutm)));


    emcmotStatus->suspend = 1;//ghf-2014/8/14,通知command去解密

    return 0;
}

#define mygetidtonum(void) do { \
    int a=0;\
    unsigned char xor=0x55,add=0;\
    unsigned short temp;\
    for(a=0;a<8;a++) {\
        xor ^= emcmotStatus->encrychipid[a];\
        add += emcmotStatus->encrychipid[a];\
    }\
    temp = (add << 8) + xor;\
} while(0);

#define decrynum 8
//final是fpga读上来的密文密钥
void func_2(unsigned char final[64]) {

    unsigned char cpu[8];//存放密文密钥中的cpuid密文(fpga串号)
    unsigned char hd[8];//存放密文密钥中的硬盘id密文
    unsigned char chip[8];//存放密文密钥中的加密芯片id密文
    unsigned char key[16];//存放密文密钥中的密钥
    des3_context ctx;//用于解密用的密钥结构体
    int i,j;
    static unsigned char cpuidl[8];//解密后的cpuid

    rtapi_print_msg(RTAPI_MSG_DBG,
            "MOTION: setting Traj cycle time to %ld nsecs\n",
            (long) (0.01 * 1e9));


    for( i = 0;i<8;i++) {

        cpu[i] = final[5*i];
        hd[i]= final[5*i+1];
        chip[i] = final[5*i+2];
        key[i*2] = final[5*i+3];
        key[i*2+1] = final[5*i+4];

    }

    for( i = 0;i<8;i++) {
        hd[i] ^= cpu[i];
        chip[i] ^= cpu[i];
    }

    hal_ppmc_4(&ctx,key,&key[8]);
    hal_ppmc_7(&ctx,cpu,cpuidl);

    for( i = 0;i<8;i++) {
        //rtapi_print("cpuidl[%d]=%02x\n",i,cpuidl[i]);
        mygetidtonum();
    }

    for( i = 0;i<8;i++) {
        if(emcmotStatus->fpgasn[i] == cpuidl[i]){
            //rtapi_print("fpgasn[%d]=%02x\n",i,cpuidl[i]);
            continue;
        }
        else {
            sqSetFeedOverride(&queue,1.0);
            break;
        }
    }

}

/***********************************************************************
 *                     PUBLIC FUNCTION CODE                             *
 ************************************************************************/

void func_3(void) {
    if (emcmotConfig->head == emcmotConfig->tail) {
		emcmotConfig->config_num++;
		emcmotStatus->config_num = emcmotConfig->config_num;
		emcmotConfig->head++;
	}
}

void reportError(const char *fmt, ...) {
	va_list args;

	va_start(args, fmt);
	emcmotErrorPutfv(emcmotError, fmt, args);
	va_end(args);
}

#ifndef va_copy
#define va_copy(dest, src) ((dest)=(src))
#endif

static rtapi_msg_handler_t old_handler = NULL;
static void emc_message_handler(msg_level_t level, const char *fmt,
		va_list ap) {
	va_list apc;
	va_copy(apc, ap);
	if (level == RTAPI_MSG_ERR)
		emcmotErrorPutfv(emcmotError, fmt, apc);
	if (old_handler)
		old_handler(level, fmt, ap);
	va_end(apc);
}

int rtapi_app_main(void) {
    int retval;

    /* set flag */
    first_pass = 1;


    /* connect to the HAL and RTAPI */
    mot_comp_id = hal_init("motmod");
    if (mot_comp_id < 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, _("MOTION: hal_init() failed\n"));
        return -1;
    }



	if ((num_joints < 1) || (num_joints > EMCMOT_MAX_JOINTS)) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                _("MOTION: num_joints is %d, must be between 1 and %d\n"),
                num_joints, EMCMOT_MAX_JOINTS);
		return -1;
	}

	if ((num_dio < 1) || (num_dio > EMCMOT_MAX_DIO)) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                _("MOTION: num_dio is %d, must be between 1 and %d\n"), num_dio,
                EMCMOT_MAX_DIO);
		return -1;
	}

	if ((num_aio < 1) || (num_aio > EMCMOT_MAX_AIO)) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                _("MOTION: num_aio is %d, must be between 1 and %d\n"), num_aio,
                EMCMOT_MAX_AIO);
		return -1;
	}

	/* initialize/export HAL pins and parameters */
	retval = init_hal_io();
	if (retval != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, _("MOTION: init_hal_io() failed\n"));
		hal_exit(mot_comp_id);
		return -1;
	}

	/* allocate/initialize user space comm buffers (cmd/status/err) */
	retval = init_comm_buffers();
	if (retval != 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                _("MOTION: init_comm_buffers() failed\n"));
		hal_exit(mot_comp_id);
		return -1;
	}

    //ghf-2015/1/20
    /*emcmotStatus->rtc = rtc_class_open(CONFIG_RTC_HCTOSYS_DEVICE);
    if (emcmotStatus->rtc == NULL) {
        rtapi_print_msg(RTAPI_MSG_ERR, _("MOTION: rtc_class_open() failed\n"));
        //hal_exit(mot_comp_id);
        //return -1;
    }*/

    /* set up for realtime execution of code */
	retval = init_threads();
	if (retval != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, _("MOTION: init_threads() failed\n"));
		hal_exit(mot_comp_id);
		return -1;
    }

    //iopl(3);
    retval = init_encry_ppmc(); //这个提前，读数据不正确，郁闷！ghf-2014/8/29
    if (retval != 0) {
        hal_exit(mot_comp_id);
        return -1;
    }

    firstdecryptInfo(emcmotStatus->encryptbuf);//校验fpgasn

    hal_ready(mot_comp_id);



	old_handler = rtapi_get_msg_handler();
	rtapi_set_msg_handler(emc_message_handler);
	return 0;
}

void rtapi_app_exit(void) {
	int retval;

	rtapi_set_msg_handler(old_handler);

    //rtapi_print_msg(RTAPI_MSG_INFO, "MOTION: cleanup_module() started.\n");



	retval = hal_stop_threads();
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                _("MOTION: hal_stop_threads() failed, returned %d\n"), retval);
	}

    //rtc_class_close(emcmotStatus->rtc );//ghf-2015/1/17

	/* free shared memory */
	retval = rtapi_shmem_delete(emc_shmem_id, mot_comp_id);
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                _("MOTION: rtapi_shmem_delete() failed, returned %d\n"),
                retval);
	}



	/* disconnect from HAL and RTAPI */
	retval = hal_exit(mot_comp_id);
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                _("MOTION: hal_exit() failed, returned %d\n"), retval);
	}
    //rtapi_print_msg(RTAPI_MSG_INFO, "MOTION: cleanup_module() finished.\n");
}

/***********************************************************************
 *                         LOCAL FUNCTION CODE                          *
 ************************************************************************/

/* init_hal_io() exports HAL pins and parameters making data from
 the realtime control module visible and usable by the world
 */
static int init_hal_io(void) {
	int n, retval;
	joint_hal_t *joint_data;

	char buf[HAL_NAME_LEN + 2];

    rtapi_print_msg(RTAPI_MSG_DBG, "MOTION: init_hal_io() starting...\n");

	/* allocate shared memory for machine data */
	emcmot_hal_data = hal_malloc(sizeof(emcmot_hal_data_t));
	if (emcmot_hal_data == 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                _("MOTION: emcmot_hal_data malloc failed\n"));
		return -1;
	}

    /* export machine wide hal pins */
        if ((retval = hal_pin_bit_newf(HAL_IN, &(emcmot_hal_data->probe_input),
            mot_comp_id, "motion.probe-input")) < 0)
        goto error;


	if ((retval = hal_pin_bit_newf(HAL_IO,
			&(emcmot_hal_data->spindle_index_enable), mot_comp_id,
			"motion.spindle-index-enable")) < 0)
		goto error;


	if ((retval = hal_pin_bit_newf(HAL_OUT, &(emcmot_hal_data->spindle_forward),
			mot_comp_id, "motion.spindle-forward")) < 0)
		goto error;
	if ((retval = hal_pin_bit_newf(HAL_OUT, &(emcmot_hal_data->spindle_reverse),
			mot_comp_id, "motion.spindle-reverse")) < 0)
		goto error;
	if ((retval = hal_pin_bit_newf(HAL_OUT, &(emcmot_hal_data->spindle_brake),
			mot_comp_id, "motion.spindle-brake")) < 0)
		goto error;
    if ((retval = hal_pin_bit_newf(HAL_OUT, &(emcmot_hal_data->spindle_enable),
            mot_comp_id, "motion.spindle-enable")) < 0)
        goto error;
    if ((retval = hal_pin_bit_newf(HAL_IN, &(emcmot_hal_data->spindle_getenabled),
            mot_comp_id, "motion.spindle-getenabled")) < 0)
        goto error;

	if ((retval = hal_pin_float_newf(HAL_OUT,
			&(emcmot_hal_data->spindle_speed_out), mot_comp_id,
			"motion.spindle-speed-out")) < 0)
		goto error;
	if ((retval = hal_pin_float_newf(HAL_OUT,
			&(emcmot_hal_data->spindle_speed_out_rps), mot_comp_id,
			"motion.spindle-speed-out-rps")) < 0)
		goto error;
	if ((retval = hal_pin_float_newf(HAL_OUT,
			&(emcmot_hal_data->spindle_speed_cmd_rps), mot_comp_id,
			"motion.spindle-speed-cmd-rps")) < 0)
		goto error;
    if ((retval = hal_pin_float_newf(HAL_OUT,
            &(emcmot_hal_data->spindle_nowpos), mot_comp_id,
            "motion.spindle-nowpos")) < 0)
        goto error;
    if ((retval = hal_pin_float_newf(HAL_OUT,
            &(emcmot_hal_data->spindle_nowvel), mot_comp_id,
            "motion.spindle-nowvel")) < 0)
        goto error;

    if ((retval = hal_pin_float_newf(HAL_IN,
            &(emcmot_hal_data->spindle_fbpos), mot_comp_id,
            "motion.spindle-fbpos")) < 0)
        goto error;
//    if ((retval = hal_pin_bit_newf(HAL_OUT, &(emcmot_hal_data->inpos_output), mot_comp_id, "motion.motion-inpos")) < 0) goto error;
	if ((retval = hal_pin_float_newf(HAL_IN, &(emcmot_hal_data->spindle_revs),
			mot_comp_id, "motion.spindle-revs")) < 0)
		goto error;
	if ((retval = hal_pin_float_newf(HAL_IN,
			&(emcmot_hal_data->spindle_speed_in), mot_comp_id,
			"motion.spindle-speed-in")) < 0)
		goto error;
	if ((retval = hal_pin_bit_newf(HAL_IN,
			&(emcmot_hal_data->spindle_is_atspeed), mot_comp_id,
			"motion.spindle-at-speed")) < 0)
		goto error;
	*emcmot_hal_data->spindle_is_atspeed = 1;
	if ((retval = hal_pin_float_newf(HAL_IN, &(emcmot_hal_data->adaptive_feed),
			mot_comp_id, "motion.adaptive-feed")) < 0)
		goto error;


    retval = hal_pin_u32_newf(HAL_OUT, &(emcmot_hal_data->move_type), mot_comp_id,
            "motion.spindle.move_type");
    if (retval != 0) {
        return retval;
    }


	*(emcmot_hal_data->adaptive_feed) = 1.0;
	if ((retval = hal_pin_bit_newf(HAL_IN, &(emcmot_hal_data->feed_hold),
			mot_comp_id, "motion.feed-hold")) < 0)
		goto error;
	*(emcmot_hal_data->feed_hold) = 0;

	if ((retval = hal_pin_bit_newf(HAL_IN, &(emcmot_hal_data->enable),
			mot_comp_id, "motion.enable")) < 0)
		goto error;

	/* export motion-synched digital output pins */
	/* export motion digital input pins */
	for (n = 0; n < num_dio; n++) {
		if ((retval = hal_pin_bit_newf(HAL_OUT, &(emcmot_hal_data->synch_do[n]),
				mot_comp_id, "motion.digital-out-%02d", n)) < 0)
			goto error;
		if ((retval = hal_pin_bit_newf(HAL_IN, &(emcmot_hal_data->synch_di[n]),
				mot_comp_id, "motion.digital-in-%02d", n)) < 0)
			goto error;
	}

	/* export motion analog input pins */
	for (n = 0; n < num_aio; n++) {
		if ((retval = hal_pin_float_newf(HAL_OUT,
				&(emcmot_hal_data->analog_output[n]), mot_comp_id,
				"motion.analog-out-%02d", n)) < 0)
			goto error;
		if ((retval = hal_pin_float_newf(HAL_IN,
				&(emcmot_hal_data->analog_input[n]), mot_comp_id,
				"motion.analog-in-%02d", n)) < 0)
			goto error;
	}

	/* export machine wide hal parameters */
	retval = hal_pin_bit_new("motion.motion-enabled", HAL_IN,
			&(emcmot_hal_data->motion_enabled), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_new("motion.in-position", HAL_OUT,
			&(emcmot_hal_data->in_position), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_new("motion.coord-mode", HAL_OUT,
			&(emcmot_hal_data->coord_mode), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_new("motion.teleop-mode", HAL_OUT,
			&(emcmot_hal_data->teleop_mode), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_new("motion.coord-error", HAL_OUT,
			&(emcmot_hal_data->coord_error), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	//ghf-2014/3/17
	retval = hal_pin_bit_new("motion.green-on", HAL_OUT,
			&(emcmot_hal_data->green_on), mot_comp_id);
	if (retval != 0) {
		return retval;
	}

	retval = hal_pin_bit_new("motion.yellow-on", HAL_OUT,
			&(emcmot_hal_data->yellow_on), mot_comp_id);
	if (retval != 0) {
		return retval;
	}

	retval = hal_pin_bit_new("motion.red-on", HAL_OUT,
			&(emcmot_hal_data->red_on), mot_comp_id);
	if (retval != 0) {
		return retval;
	}


    retval = hal_pin_bit_new("motion.runStatus", HAL_OUT,
            &(emcmot_hal_data->runStatus), mot_comp_id);
    if (retval != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "in init_hal_io,emcmot_hal_data->runStatus error\n");
        return retval;
    }

	//ghf-2014/5/6
    for (n = 0; n < 64; n++) {
		rtapi_snprintf(buf, HAL_NAME_LEN, "motion.filtertimebuf.%02d", n);
		retval = hal_pin_u32_new(buf, HAL_OUT,
				&(emcmot_hal_data->filtertimebuf[n]), mot_comp_id);
		if (retval != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR,
					"in init_hal_io,emcmot_hal_data->filtertimebuf error\n");
			return retval;
		}
	}

    //ghf-2014/8/25
    rtapi_snprintf(buf, HAL_NAME_LEN, "motion.fpgastatus");
    retval = hal_pin_u32_new(buf, HAL_IN,
            &(emcmot_hal_data->fpgastatus), mot_comp_id);
    if (retval != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "in init_hal_io,emcmot_hal_data->fpgastatus error\n");
        return retval;
    }

    //ghf-2014/10/24
    rtapi_snprintf(buf, HAL_NAME_LEN, "motion.numSegments");
    retval = hal_pin_u32_new(buf, HAL_IN,
            &(emcmot_hal_data->numSegments), mot_comp_id);
    if (retval != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "in init_hal_io,emcmot_hal_data->numSegments error\n");
        return retval;
    }
    rtapi_snprintf(buf, HAL_NAME_LEN, "motion.done");
    retval = hal_pin_u32_new(buf, HAL_IN,
            &(emcmot_hal_data->done), mot_comp_id);
    if (retval != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "in init_hal_io,emcmot_hal_data->done error\n");
        return retval;
    }
    rtapi_snprintf(buf, HAL_NAME_LEN, "motion.paused");
    retval = hal_pin_u32_new(buf, HAL_IN,
            &(emcmot_hal_data->paused), mot_comp_id);
    if (retval != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "in init_hal_io,emcmot_hal_data->paused error\n");
        return retval;
    }



    /*rtapi_snprintf(buf, HAL_NAME_LEN, "motion.aborting");
    retval = hal_pin_u32_new(buf, HAL_IN,
            &(emcmot_hal_data->aborting), mot_comp_id);
    if (retval != 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "in init_hal_io,emcmot_hal_data->aborting error\n");
        return retval;
    }*/

    for (n = 0; n < 24; n++) {
        rtapi_snprintf(buf, HAL_NAME_LEN, "motion.extern.out.%02d", n);
        retval = hal_pin_bit_new(buf, HAL_OUT,
                &(emcmot_hal_data->motion_extern_out[n]), mot_comp_id);
        if (retval != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR,
                    "in init_hal_io,emcmot_hal_data->motion_extern_out error\n");
            return retval;
        }
    }

    for (n = 0; n < 48; n++) {
        rtapi_snprintf(buf, HAL_NAME_LEN, "motion.extern.in.%02d", n);
        retval = hal_pin_bit_new(buf, HAL_IN,
                &(emcmot_hal_data->motion_extern_in[n]), mot_comp_id);
        if (retval != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR,
                    "in init_hal_io,emcmot_hal_data->motion_extern_in error\n");
            return retval;
        }
    }

    //ghf-2015/6/24
    for (n = 0; n < EMCMOT_MAX_JOINTS; n++) {
        rtapi_snprintf(buf, HAL_NAME_LEN, "motion.encoder.%02d.index", n);
        retval = hal_pin_bit_new(buf, HAL_IN, &(emcmot_hal_data->encoder_index[n]),
                mot_comp_id);
        if (retval != 0) {
            rtapi_print_msg(RTAPI_MSG_ERR,
                    "in init_hal_io,emcmot_hal_data->encoder_index error\n");
            return retval;
        }
    }

	//ghf-2014/5/24  
    for (n = 0; n < EMCMOT_MAX_USR_INIO; n++) {
		rtapi_snprintf(buf, HAL_NAME_LEN, "motion.user-define-in.%02d", n);
		retval = hal_pin_bit_new(buf, HAL_IN, &(emcmot_hal_data->user_defin[n]),
				mot_comp_id);
		if (retval != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR,
					"in init_hal_io,emcmot_hal_data->user_defin error\n");
			return retval;
		}
    }
        for (n = 0; n < EMCMOT_MAX_USR_OUTIO; n++) {
		rtapi_snprintf(buf, HAL_NAME_LEN, "motion.user-define-out.%02d", n);
        retval = hal_pin_bit_new(buf, HAL_IN,
				&(emcmot_hal_data->user_defout[n]), mot_comp_id);
		if (retval != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR,
					"in init_hal_io,emcmot_hal_data->user_defout error\n");
			return retval;
		}
        }
        for (n = 0; n < EMCMOT_MAX_USR_WARNIO; n++) {
		rtapi_snprintf(buf, HAL_NAME_LEN, "motion.user-define-warn.%02d", n);
		retval = hal_pin_bit_new(buf, HAL_IN,
				&(emcmot_hal_data->user_defwarn[n]), mot_comp_id);
		if (retval != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR,
					"in init_hal_io,emcmot_hal_data->user_defwarn error\n");
			return retval;
		}
	}

	retval = hal_pin_bit_new("motion.on-soft-limit", HAL_OUT,
			&(emcmot_hal_data->on_soft_limit), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.current-vel", HAL_OUT,
			&(emcmot_hal_data->current_vel), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.requested-vel", HAL_OUT,
			&(emcmot_hal_data->requested_vel), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.distance-to-go", HAL_OUT,
			&(emcmot_hal_data->distance_to_go), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_s32_new("motion.program-line", HAL_OUT,
			&(emcmot_hal_data->program_line), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	/* export debug parameters */
	/* these can be used to view any internal variable, simply change a line
	 in control.c:output_to_hal() and recompile */
	retval = hal_param_bit_new("motion.debug-bit-0", HAL_RO,
			&(emcmot_hal_data->debug_bit_0), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_param_bit_new("motion.debug-bit-1", HAL_RO,
			&(emcmot_hal_data->debug_bit_1), mot_comp_id);
	if (retval != 0) {
		return retval;
	}

	retval = hal_param_float_new("motion.debug-float-0", HAL_RO,
			&(emcmot_hal_data->debug_float_0), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_param_float_new("motion.debug-float-1", HAL_RO,
			&(emcmot_hal_data->debug_float_1), mot_comp_id);
	if (retval != 0) {
		return retval;
	}

	retval = hal_param_float_new("motion.debug-float-2", HAL_RO,
			&(emcmot_hal_data->debug_float_2), mot_comp_id);
	if (retval != 0) {
		return retval;
	}

	retval = hal_param_float_new("motion.debug-float-3", HAL_RO,
			&(emcmot_hal_data->debug_float_3), mot_comp_id);
	if (retval != 0) {
		return retval;
	}

	retval = hal_param_s32_new("motion.debug-s32-0", HAL_RO,
			&(emcmot_hal_data->debug_s32_0), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_param_s32_new("motion.debug-s32-1", HAL_RO,
			&(emcmot_hal_data->debug_s32_1), mot_comp_id);
	if (retval != 0) {
		return retval;
	}

	// FIXME - debug only, remove later
	// export HAL parameters for some trajectory planner internal variables
	// so they can be scoped
	retval = hal_param_float_new("traj.pos_out", HAL_RO,
			&(emcmot_hal_data->traj_pos_out), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_param_float_new("traj.vel_out", HAL_RO,
			&(emcmot_hal_data->traj_vel_out), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_param_u32_new("traj.active_tc", HAL_RO,
			&(emcmot_hal_data->traj_active_tc), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	for (n = 0; n < 4; n++) {
		retval = hal_param_float_newf(HAL_RO, &(emcmot_hal_data->tc_pos[n]),
				mot_comp_id, "tc.%d.pos", n);
		if (retval != 0) {
			return retval;
		}
		retval = hal_param_float_newf(HAL_RO, &(emcmot_hal_data->tc_vel[n]),
				mot_comp_id, "tc.%d.vel", n);
		if (retval != 0) {
			return retval;
		}
		retval = hal_param_float_newf(HAL_RO, &(emcmot_hal_data->tc_acc[n]),
				mot_comp_id, "tc.%d.acc", n);
		if (retval != 0) {
			return retval;
		}
	}
	// end of exporting trajectory planner internals

	// export timing related HAL parameters so they can be scoped
	retval = hal_param_u32_new("motion.servo.last-period", HAL_RO,
			&(emcmot_hal_data->last_period), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	//ghf-2014/2/24
	retval = hal_pin_s32_new("motion.set_which_z", HAL_IO,
			&(emcmot_hal_data->set_which_z), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
    //hong 2015-10-23
    retval = hal_pin_s32_new("motion.z_selected", HAL_IO,
            &(emcmot_hal_data->selected_z), mot_comp_id);
    if (retval != 0) {
        return retval;
    }

    //ghf-2014/12/17
    retval = hal_pin_s32_new("motion.feedrate", HAL_IO,
            &(emcmot_hal_data->feedrate), mot_comp_id);
    if (retval != 0) {
        return retval;
    }


	/*if ((retval = hal_pin_bit_newf(HAL_IN, &(emcmot_hal_data->run_on), mot_comp_id, "motion.run-on")) < 0) goto error;
	 
	 if ((retval = hal_pin_bit_newf(HAL_IN, &(emcmot_hal_data->pause_on), mot_comp_id, "motion.pause-on")) < 0) goto error;*/

	if ((retval = hal_pin_bit_new("motion.run-on", HAL_IN,
			&(emcmot_hal_data->run_on), mot_comp_id)) < 0)
		goto error;
	if ((retval = hal_pin_bit_new("motion.pause-on", HAL_IN,
			&(emcmot_hal_data->pause_on), mot_comp_id)) < 0)
		goto error;
	//ghf-2014/5/6
	if ((retval = hal_pin_bit_new("motion.electric-cabinet-alarm", HAL_IN,
			&(emcmot_hal_data->electric_cabinet_alarm), mot_comp_id)) < 0)
		goto error;

	if ((retval = hal_pin_bit_new("motion.spindle0-overcurrent-alarm", HAL_IN,
			&(emcmot_hal_data->spindle0_overcurrent_alarm), mot_comp_id)) < 0)
		goto error;

	if ((retval = hal_pin_bit_new("motion.spindle1-overcurrent-alarm", HAL_IN,
			&(emcmot_hal_data->spindle1_overcurrent_alarm), mot_comp_id)) < 0)
		goto error;

	if ((retval = hal_pin_bit_new("motion.spindle0-converter-alarm", HAL_IN,
			&(emcmot_hal_data->spindle0_converter_alarm), mot_comp_id)) < 0)
		goto error;

	if ((retval = hal_pin_bit_new("motion.spindle1-converter-alarm", HAL_IN,
			&(emcmot_hal_data->spindle1_converter_alarm), mot_comp_id)) < 0)
		goto error;

	if ((retval = hal_pin_bit_new("motion.probe-alarm", HAL_IN,
			&(emcmot_hal_data->probe_alarm), mot_comp_id)) < 0)
		goto error;

	if ((retval = hal_pin_bit_new("motion.pressure-alarm", HAL_IN,
			&(emcmot_hal_data->pressure_alarm), mot_comp_id)) < 0)
		goto error;

	if ((retval = hal_pin_bit_new("motion.coolant-alarm", HAL_IN,
			&(emcmot_hal_data->coolant_alarm), mot_comp_id)) < 0)
		goto error;

	if ((retval = hal_pin_bit_new("motion.lube-alarm", HAL_IN,
			&(emcmot_hal_data->lube_alarm), mot_comp_id)) < 0)
		goto error;

    if ((retval = hal_pin_bit_new("motion.handwheel-z2-active", HAL_IN,
            &(emcmot_hal_data->handwheel_Z2_active), mot_comp_id)) < 0)
        goto error;

	//ghf-2014/5/6
    /*retval =
	 hal_pin_u32_new("motion.data-in-before", HAL_IN, &(emcmot_hal_data->data_in_before), mot_comp_id);
	 if (retval != 0) {
	 return retval;
     }*/

#ifdef HAVE_CPU_KHZ
	retval =
	hal_param_float_new("motion.servo.last-period-ns", HAL_RO, &(emcmot_hal_data->last_period_ns), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
#endif
    retval = hal_param_s32_new("motion.servo.mintime", HAL_RW,
            &(emcmot_hal_data->mintime), mot_comp_id);
    if (retval != 0) {
        return retval;
    }

	retval = hal_param_u32_new("motion.servo.overruns", HAL_RW,
            &(emcmot_hal_data->overruns), mot_comp_id);
	if (retval != 0) {
		return retval;
	}

	retval = hal_pin_float_new("motion.tooloffset.x", HAL_OUT,
			&(emcmot_hal_data->tooloffset_x), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.tooloffset.y", HAL_OUT,
			&(emcmot_hal_data->tooloffset_y), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.tooloffset.z", HAL_OUT,
			&(emcmot_hal_data->tooloffset_z), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.tooloffset.a", HAL_OUT,
			&(emcmot_hal_data->tooloffset_a), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.tooloffset.b", HAL_OUT,
			&(emcmot_hal_data->tooloffset_b), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.tooloffset.c", HAL_OUT,
			&(emcmot_hal_data->tooloffset_c), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.tooloffset.u", HAL_OUT,
			&(emcmot_hal_data->tooloffset_u), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.tooloffset.v", HAL_OUT,
			&(emcmot_hal_data->tooloffset_v), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_new("motion.tooloffset.w", HAL_OUT,
			&(emcmot_hal_data->tooloffset_w), mot_comp_id);
	if (retval != 0) {
		return retval;
	}

	//ghf-2014/5/14   
	retval = hal_pin_u32_new("motion.jog-state", HAL_IN,
			&(emcmot_hal_data->jog_state), mot_comp_id);
	if (retval != 0) {
		return retval;
	}
    //hong-20161108
    retval = hal_pin_u32_new("motion.jogVal", HAL_IN,
            &(emcmot_hal_data->jogVal), mot_comp_id);
    if (retval != 0) {
        return retval;
    }
    //hong-20161108
    retval = hal_pin_float_new("motion.jogScale", HAL_OUT,
            &(emcmot_hal_data->jogScale), mot_comp_id);
    if (retval != 0) {
        return retval;
    }
    //hong-20161108
    retval = hal_pin_bit_new("motion.linktoEth", HAL_IN,
            &(emcmot_hal_data->linktoEth), mot_comp_id);
    if (retval != 0) {
        return retval;
    }

    retval = hal_pin_bit_new("motion.EthReady", HAL_IN,
            &(emcmot_hal_data->EthReady), mot_comp_id);
    if (retval != 0) {
        return retval;
    }




    //hong-20150415
    retval = hal_pin_float_new("motion.feed-scale", HAL_OUT,
            &(emcmot_hal_data->feed_scale), mot_comp_id);
    if (retval != 0) {
        return retval;
    }

	/* default value of enable is TRUE, so simple machines
	 can leave it disconnected */
	*(emcmot_hal_data->enable) = 1;

	/* motion synched dio, init to not enabled */
	for (n = 0; n < num_dio; n++) {
		*(emcmot_hal_data->synch_do[n]) = 0;
		*(emcmot_hal_data->synch_di[n]) = 0;
	}

	for (n = 0; n < num_aio; n++) {
		*(emcmot_hal_data->analog_output[n]) = 0.0;
		*(emcmot_hal_data->analog_input[n]) = 0.0;
	}

	/*! \todo FIXME - these don't really need initialized, since they are written
	 with data from the emcmotStatus struct */
    *(emcmot_hal_data->motion_enabled) = 0;
	*(emcmot_hal_data->in_position) = 0;
	*(emcmot_hal_data->coord_mode) = 0;
	*(emcmot_hal_data->teleop_mode) = 0;
	*(emcmot_hal_data->coord_error) = 0;
	*(emcmot_hal_data->on_soft_limit) = 0;

	/* init debug parameters */
	emcmot_hal_data->debug_bit_0 = 0;
	emcmot_hal_data->debug_bit_1 = 0;
	emcmot_hal_data->debug_float_0 = 0.0;
	emcmot_hal_data->debug_float_1 = 0.0;
	emcmot_hal_data->debug_float_2 = 0.0;
	emcmot_hal_data->debug_float_3 = 0.0;

    emcmot_hal_data->mintime = 100000000;
	emcmot_hal_data->overruns = 0;
	emcmot_hal_data->last_period = 0;

	*(emcmot_hal_data->set_which_z) = 0;    //ghf-2014/2/24
    *(emcmot_hal_data->selected_z) = 0;    //ghf-2014/2/24

	*(emcmot_hal_data->run_on) = 0;    //ghf-2014/3/13
	*(emcmot_hal_data->pause_on) = 0;    //ghf-2014/3/13
	*(emcmot_hal_data->green_on) = 0;    //ghf-2014/3/17
	*(emcmot_hal_data->yellow_on) = 0;    //ghf-2014/3/17
	*(emcmot_hal_data->red_on) = 0;    //ghf-2014/3/17


	//ghf-2014/3/28
    for (n = 0; n < 64; n++) {
		*(emcmot_hal_data->filtertimebuf[n]) = 0;    //ghf-2014/5/6
    }

    //*(emcmot_hal_data->fpgastatus) = 0xff;

    for (n = 0; n < EMCMOT_MAX_USR_INIO; n++) {
        *(emcmot_hal_data->user_defin[n]) = 0;
    }

    for (n = 0; n < EMCMOT_MAX_USR_OUTIO; n++) {
        *(emcmot_hal_data->user_defout[n]) = 0;
    }

    for (n = 0; n < EMCMOT_MAX_USR_WARNIO; n++) {
        *(emcmot_hal_data->user_defwarn[n]) = 0;
    }

    for (n = 0; n < 24; n++) {
        *(emcmot_hal_data->motion_extern_out[n]) = 0;
    }

    for (n = 0; n < 48; n++) {
        *(emcmot_hal_data->motion_extern_in[n]) = 0;
    }

	/* export joint pins and parameters */
	//rtapi_print_msg(RTAPI_MSG_ERR, "num_joints = %d \n",num_joints);
	for (n = 0; n < num_joints; n++) {
		/* point to axis data */
		joint_data = &(emcmot_hal_data->joint[n]);
		/* export all vars */
		retval = export_joint(n, joint_data);
		if (retval != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR,
                    _("MOTION: (joint %d) pin/param export failed\n"), n);
			return -1;
		}
		/* init axis pins and parameters */
		/* FIXME - struct members are in a state of flux - make sure to
		 update this - most won't need initing anyway */
		*(joint_data->amp_enable) = 0;
		joint_data->home_state = 0;
        *(joint_data->amp_fault) = 0;

		/* We'll init the index model to EXT_ENCODER_INDEX_MODEL_RAW for now,
         because it is always supported. */

        /* initialize machine wide pins and parameters */
	}
    *(emcmot_hal_data->probe_input) = 0;
	/* Done! */
    rtapi_print_msg(RTAPI_MSG_DBG,
            "MOTION: init_hal_io() complete, %d axes.\n", n);
	return 0;

	error: return retval;

}

static int export_joint(int num, joint_hal_t * addr) {
	int retval, msg;

	/* This function exports a lot of stuff, which results in a lot of
	 logging if msg_level is at INFO or ALL. So we save the current value
	 of msg_level and restore it later.  If you actually need to log this
	 function's actions, change the second line below */
	msg = rtapi_get_msg_level();
	rtapi_set_msg_level (RTAPI_MSG_WARN);

	/* export joint pins */ //FIXME-AJ: changing these will bork configs, still we should do it
	retval = hal_pin_float_newf(HAL_OUT, &(addr->joint_pos_cmd), mot_comp_id,
			"axis.%d.joint-pos-cmd", num);
	if (retval != 0) {
		return retval;
	}

    retval = hal_pin_float_newf(HAL_OUT, &(addr->joint_limit_vel), mot_comp_id,
            "axis.%d.joint-limit-vel", num);
    if (retval != 0) {
        return retval;
    }

	retval = hal_pin_float_newf(HAL_OUT, &(addr->joint_pos_fb), mot_comp_id,
			"axis.%d.joint-pos-fb", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_OUT, &(addr->motor_pos_cmd), mot_comp_id,
			"axis.%d.motor-pos-cmd", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_OUT, &(addr->motor_offset), mot_comp_id,
			"axis.%d.motor-offset", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_IN, &(addr->motor_pos_fb), mot_comp_id,
			"axis.%d.motor-pos-fb", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_IN, &(addr->pos_lim_sw), mot_comp_id,
			"axis.%d.pos-lim-sw-in", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_IN, &(addr->neg_lim_sw), mot_comp_id,
			"axis.%d.neg-lim-sw-in", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_IN, &(addr->home_sw), mot_comp_id,
			"axis.%d.home-sw-in", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_IO, &(addr->index_enable), mot_comp_id,
			"axis.%d.index-enable", num);
	if (retval != 0) {
		return retval;
	}
    retval = hal_pin_bit_newf(HAL_IO, &(addr->homeclearpos), mot_comp_id,
            "axis.%d.homeclearpos", num);
    if (retval != 0) {
        return retval;
    }

	retval = hal_pin_bit_newf(HAL_OUT, &(addr->amp_enable), mot_comp_id,
			"axis.%d.amp-enable-out", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_IN, &(addr->amp_fault), mot_comp_id,
			"axis.%d.amp-fault-in", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_s32_newf(HAL_IN, &(addr->jog_counts), mot_comp_id,
			"axis.%d.jog-counts", num);
	if (retval != 0) {
		return retval;
	}

	retval = hal_pin_bit_newf(HAL_IN, &(addr->jog_enable), mot_comp_id,
			"axis.%d.jog-enable", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_IN, &(addr->jog_scale), mot_comp_id,
			"axis.%d.jog-scale", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_IN, &(addr->jog_vel_mode), mot_comp_id,
			"axis.%d.jog-vel-mode", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->homing), mot_comp_id,
			"axis.%d.homing", num);
	if (retval != 0) {
		return retval;
	}
	/* export joint parameters */ //FIXME-AJ: changing these to joints will break configs.
	retval = hal_pin_float_newf(HAL_OUT, &(addr->coarse_pos_cmd), mot_comp_id,
			"axis.%d.coarse-pos-cmd", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_OUT, &(addr->joint_vel_cmd), mot_comp_id,
			"axis.%d.joint-vel-cmd", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_OUT, &(addr->backlash_corr), mot_comp_id,
			"axis.%d.backlash-corr", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_OUT, &(addr->backlash_filt), mot_comp_id,
			"axis.%d.backlash-filt", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_OUT, &(addr->backlash_vel), mot_comp_id,
			"axis.%d.backlash-vel", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_OUT, &(addr->f_error), mot_comp_id,
			"axis.%d.f-error", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_OUT, &(addr->f_error_lim), mot_comp_id,
			"axis.%d.f-error-lim", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_OUT, &(addr->free_pos_cmd), mot_comp_id,
			"axis.%d.free-pos-cmd", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_float_newf(HAL_OUT, &(addr->free_vel_lim), mot_comp_id,
			"axis.%d.free-vel-lim", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->free_tp_enable), mot_comp_id,
			"axis.%d.free-tp-enable", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->kb_jog_active), mot_comp_id,
			"axis.%d.kb-jog-active", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->wheel_jog_active), mot_comp_id,
			"axis.%d.wheel-jog-active", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->active), mot_comp_id,
			"axis.%d.active", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->in_position), mot_comp_id,
			"axis.%d.in-position", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->error), mot_comp_id,
			"axis.%d.error", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->phl), mot_comp_id,
			"axis.%d.pos-hard-limit", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->nhl), mot_comp_id,
			"axis.%d.neg-hard-limit", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->homed), mot_comp_id,
			"axis.%d.homed", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->f_errored), mot_comp_id,
			"axis.%d.f-errored", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_pin_bit_newf(HAL_OUT, &(addr->faulted), mot_comp_id,
			"axis.%d.faulted", num);
	if (retval != 0) {
		return retval;
	}
	retval = hal_param_s32_newf(HAL_RO, &(addr->home_state), mot_comp_id,
			"axis.%d.home-state", num);
	if (retval != 0) {
		return retval;
	}

    retval = hal_pin_float_newf(HAL_IN, &(addr->fric_out_u), mot_comp_id,
            "axis.%d.fric_out_u", num);
    if (retval != 0) {
        return retval;
    }

    retval = hal_pin_float_newf(HAL_IN, &(addr->absZeropos), mot_comp_id,
            "axis.%d.absZeropos", num);
    if (retval != 0) {
        return retval;
    }

    retval = hal_pin_s32_newf(HAL_IN, &(addr->errorcode), mot_comp_id,
            "axis.%d.jog_errorcode", num);
    if (retval != 0) {
        return retval;
    }


	if (num >= 3 && num <= 5) {
		// for rotaries only...
		retval = hal_pin_bit_newf(HAL_OUT, &(addr->unlock), mot_comp_id,
				"axis.%d.unlock", num);
		if (retval != 0)
			return retval;
		retval = hal_pin_bit_newf(HAL_IN, &(addr->is_unlocked), mot_comp_id,
				"axis.%d.is-unlocked", num);
		if (retval != 0)
			return retval;
	}
	/* restore saved message level */
	rtapi_set_msg_level(msg);
	return 0;
}

/* init_comm_buffers() allocates and initializes the command,
 status, and error buffers used to communicate with the user
 space parts of emc.
 */
static int func_7(void) {
	int joint_num, n;
	emcmot_joint_t *joint;
	int retval;

    rtapi_print_msg(RTAPI_MSG_DBG,"MOTION: func_7() starting...\n");

	emcmotStruct = 0;
	emcmotDebug = 0;
	emcmotStatus = 0;
	emcmotCommand = 0;
	emcmotConfig = 0;

	/* record the kinematics type of the machine */
	kinType = kinematicsType();
    //运动学参数初始化
    kinedata.a_length = 0;
    kinedata.direct = 0;
    kinedata.method = 0;
    kinedata.offset_x=0;
    kinedata.offset_y=0;
    kinedata.flag = 0;
    kinedata.limit = 10000;
    ZERO_EMC_POSE(kinedata.lastP);
    int jointcounts=0;
    for(jointcounts=0;jointcounts<EMCMOT_MAX_JOINTS;jointcounts++) {
        kinedata.circle_type[jointcounts] = 0;
        kinedata.circle_count[jointcounts] = 0;
    }

	/* allocate and initialize the shared memory structure */
	emc_shmem_id = rtapi_shmem_new(key, mot_comp_id, sizeof(emcmot_struct_t));
	if (emc_shmem_id < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
				"MOTION: rtapi_shmem_new failed, returned %d\n", emc_shmem_id);
		return -1;
	}
	retval = rtapi_shmem_getptr(emc_shmem_id, (void **) &emcmotStruct);
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
				"MOTION: rtapi_shmem_getptr failed, returned %d\n", retval);
		return -1;
	}

    emcdata_id = rtapi_shmem_new(54321,mot_comp_id,sizeof(emcdata_struct));
    if(emcdata_id < 0) {
        printf("now the emcdata space create error,abort!");
        return -1;
    }

    printf("now I get the motod emcdata_id:%d    shmem_id:%d\n",emcdata_id,emc_shmem_id);
    retval = rtapi_shmem_getptr(emcdata_id,(void **)&motData);
    if (retval < 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "Get the emcData error, returned %d\n", retval);
        return -1;
    }

    int errorid = rtapi_shmem_new(22222,mot_comp_id,sizeof(version_struct));
    rtapi_shmem_getptr(errorid,(void **)&versiondata);

    int datacount = 0;
    for(datacount = 0;datacount < EMCMOT_MAX_SAVEPOS;datacount++) {
        motData->setpos[datacount].tran.x = 0;
        motData->setpos[datacount].tran.y = 0;
        motData->setpos[datacount].tran.z = 0;
        motData->setpos[datacount].a = 0;
        motData->setpos[datacount].b = 0;
        motData->setpos[datacount].c = 0;
    }
    motData->index = 0;

	/* zero shared memory before doing anything else. */
	memset(emcmotStruct, 0, sizeof(emcmot_struct_t));

	/* we'll reference emcmotStruct directly */
	emcmotCommand = &emcmotStruct->command;
	emcmotStatus = &emcmotStruct->status;
	emcmotConfig = &emcmotStruct->config;
	emcmotDebug = &emcmotStruct->debug;
	emcmotInternal = &emcmotStruct->internal;
	emcmotError = &emcmotStruct->error;

	/* init error struct */
	emcmotErrorInit(emcmotError);

	/* init command struct */
	emcmotCommand->head = 0;
	emcmotCommand->command = 0;
	emcmotCommand->commandNum = 0;
	emcmotCommand->tail = 0;
	emcmotCommand->spindlesync = 0.0;

	/* init status struct */
	emcmotStatus->head = 0;
	emcmotStatus->commandEcho = 0;
	emcmotStatus->commandNumEcho = 0;
	emcmotStatus->commandStatus = 0;

	/* init more stuff */

    emcmotDebug->head = 0;
    emcmotConfig->head = 0;

	emcmotStatus->motionFlag = 0;
	SET_MOTION_ERROR_FLAG(0);
	SET_MOTION_COORD_FLAG(0);
	SET_MOTION_TELEOP_FLAG(0);
	emcmotDebug->split = 0;
	emcmotStatus->heartbeat = 0;
	emcmotStatus->computeTime = 0.0;
	emcmotConfig->numJoints = num_joints;

	ZERO_EMC_POSE(emcmotStatus->carte_pos_cmd);
	ZERO_EMC_POSE(emcmotStatus->carte_pos_fb);
	emcmotStatus->vel = VELOCITY;
	emcmotConfig->limitVel = VELOCITY;
	emcmotStatus->acc = ACCELERATION;
	emcmotStatus->feed_scale = 1.0;
	emcmotStatus->spindle_scale = 1.0;
	emcmotStatus->net_feed_scale = 1.0;
    emcmotStatus->run_status = 0;
    emcmotStatus->doublez_base_pos = 0;

    emcmotStatus->forcflag=0;//ghf-2015-4-10  //0 means not changing z
    emcmotStatus->changingz = 0;
    emcmotStatus->changezend = 1;

    emcmotStatus->useWitchY = 0;

	/* adaptive feed is off by default, feed override, spindle 
	 override, and feed hold are on */
	emcmotStatus->enables_new = FS_ENABLED | SS_ENABLED | FH_ENABLED;
	emcmotStatus->enables_queued = emcmotStatus->enables_new;
	emcmotStatus->id = 0;
	emcmotStatus->depth = 0;
	emcmotStatus->activeDepth = 0;
	emcmotStatus->paused = 0;
	emcmotStatus->overrideLimitMask = 0;
	emcmotStatus->spindle.speed = 0.0;
    emcmotStatus->spindle.stopmove = 0;
    emcmotStatus->spindle.moveType =0;
    emcmotStatus->spindle.speedrpm = 0;
	SET_MOTION_INPOS_FLAG(1);
    SET_MOTION_ENABLE_FLAG(1);
	emcmotConfig->kinematics_type = kinType;

	emcmotDebug->oldPos = emcmotStatus->carte_pos_cmd;
	ZERO_EMC_POSE(emcmotDebug->oldVel);

    emcmotStatus->useAbsEncoder = 1;
    emcmotStatus->getAbsposflag = 1;

    emcmotStatus->rencryflag = 0;
    emcmotStatus->axesdiffval = 0;

	emcmot_config_change();

	/* init pointer to joint structs */
#ifdef STRUCTS_IN_SHMEM
	joints = &(emcmotDebug->joints[0]);
#else
	joints = &(joint_array[0]);
#endif

	/* init per-joint stuff */
    for (joint_num = 0; joint_num < EMCMOT_MAX_JOINTS; joint_num++) {
		/* point to structure for this joint */
		joint = &joints[joint_num];

		/* init the config fields with some "reasonable" defaults" */

		joint->type = 0;
		joint->max_pos_limit = 1.0;
		joint->min_pos_limit = -1.0;
        joint->sec_neg_limit = -1.0;
        joint->sec_pos_limit = 1.0;
		joint->vel_limit = 1.0;
		joint->acc_limit = 1.0;
		joint->min_ferror = 0.01;
		joint->max_ferror = 1.0;
		joint->home_search_vel = 0.0;
		joint->home_latch_vel = 0.0;
		joint->home_final_vel = -1;
		joint->home_offset = 0.0;
		joint->home = 0.0;
		joint->home_flags = 0;
		joint->home_sequence = -1;
		joint->backlash = 0.0;

		joint->comp.entries = 0;
		joint->comp.entry = &(joint->comp.array[0]);
		/* the compensation code has -DBL_MAX at one end of the table
		 and +DBL_MAX at the other so _all_ commanded positions are
		 guaranteed to be covered by the table */
		joint->comp.array[0].nominal = -DBL_MAX;
		joint->comp.array[0].fwd_trim = 0.0;
		joint->comp.array[0].rev_trim = 0.0;
		joint->comp.array[0].fwd_slope = 0.0;
		joint->comp.array[0].rev_slope = 0.0;
		for (n = 1; n < EMCMOT_COMP_SIZE + 2; n++) {
			joint->comp.array[n].nominal = DBL_MAX;
			joint->comp.array[n].fwd_trim = 0.0;
			joint->comp.array[n].rev_trim = 0.0;
			joint->comp.array[n].fwd_slope = 0.0;
			joint->comp.array[n].rev_slope = 0.0;
		}

		/* init status info */
		joint->flag = 0;
		joint->coarse_pos = 0.0;
        joint->freepos = 0.0;
        joint->pos_cmd = 0.0;
		joint->vel_cmd = 0.0;
		joint->backlash_corr = 0.0;
		joint->backlash_filt = 0.0;
		joint->backlash_vel = 0.0;
		joint->motor_pos_cmd = 0.0;
		joint->motor_pos_fb = 0.0;
		joint->pos_fb = 0.0;
		joint->ferror = 0.0;
		joint->ferror_limit = joint->min_ferror;
		joint->ferror_high_mark = 0.0;
        joint->old_pos_cmd = 0.0;
        joint->old_v_cmd = 0.0;
        joint->circle_shortest=0;

		/* init internal info */
		cubicInit(&(joint->cubic));

		/* init misc other stuff in joint structure */
		joint->big_vel = 10.0 * joint->vel_limit;
        joint->home_state = 0;

		/* init joint flags (reduntant, since flag = 0 */

		SET_JOINT_ENABLE_FLAG(joint, 0);
		SET_JOINT_ACTIVE_FLAG(joint, 0);
		SET_JOINT_NHL_FLAG(joint, 0);
		SET_JOINT_PHL_FLAG(joint, 0);
		SET_JOINT_INPOS_FLAG(joint, 1);
        SET_JOINT_HOMING_FLAG(joint, 0);
		SET_JOINT_FERROR_FLAG(joint, 0);
		SET_JOINT_FAULT_FLAG(joint, 0);
		SET_JOINT_ERROR_FLAG(joint, 0);

        if(joint->useAbsPos == 1){
            SET_JOINT_HOMED_FLAG(joint, 1);
        }else{
            SET_JOINT_HOMED_FLAG(joint, 0);
        }

	}

	/*! \todo FIXME-- add emcmotError */

	emcmotDebug->tMin = 0.0;
	emcmotDebug->tMax = 0.0;
	emcmotDebug->tAvg = 0.0;
	emcmotDebug->sMin = 0.0;
	emcmotDebug->sMax = 0.0;
	emcmotDebug->sAvg = 0.0;
	emcmotDebug->nMin = 0.0;
	emcmotDebug->nMax = 0.0;
	emcmotDebug->nAvg = 0.0;
	emcmotDebug->yMin = 0.0;
	emcmotDebug->yMax = 0.0;
	emcmotDebug->yAvg = 0.0;
	emcmotDebug->fyMin = 0.0;
	emcmotDebug->fyMax = 0.0;
	emcmotDebug->fyAvg = 0.0;
	emcmotDebug->fMin = 0.0;
	emcmotDebug->fMax = 0.0;
	emcmotDebug->fAvg = 0.0;

	emcmotDebug->cur_time = emcmotDebug->last_time = 0.0;
	emcmotDebug->start_time = etime();
	emcmotDebug->running_time = 0.0;

	/* init motion emcmotDebug->queue
	 if (-1 == tpCreate(&emcmotDebug->queue, DEFAULT_TC_QUEUE_SIZE,
	 emcmotDebug->queueTcSpace)) {
	 rtapi_print_msg(RTAPI_MSG_ERR,
	 "MOTION: failed to create motion emcmotDebug->queue\n");
	 return -1;
	 }
	 //    tpInit(&emcmotDebug->queue); // tpInit called from tpCreate
	 tpSetCycleTime(&emcmotDebug->queue, emcmotConfig->trajCycleTime);
	 tpSetPos(&emcmotDebug->queue, emcmotStatus->carte_pos_cmd);
	 tpSetVmax(&emcmotDebug->queue, emcmotStatus->vel, emcmotStatus->vel);
	 tpSetAmax(&emcmotDebug->queue, emcmotStatus->acc);*/

	//add by ghf 2014-1-25
	if (-1 == sqInitQueue(&queue, &sqSpace[0], DEFAULT_TC_QUEUE_SIZE)) {
		rtapi_print_msg(RTAPI_MSG_ERR,
				"MOTION: failed to create motion emcmotDebug->queue\n");
		return -1;
    }
    versiondata->segVersion = queue.version;        //算法版本

	sqSetCycleTime(&queue, emcmotConfig->trajCycleTime);
	sqSetPos(&queue, emcmotStatus->carte_pos_cmd);
	sqSetFeed(&queue, emcmotStatus->vel);
	sqSetMaxAcc(&queue, emcmotStatus->acc);
    sqSetMaxFeedOverride(&queue, 2.0);
	sqSetFeedOverride(&queue, 1.0);
    emczeroPosLoad();

	emcmotStatus->tail = 0;

    rtapi_print_msg(RTAPI_MSG_DBG, "MOTION: func_7() complete\n");
	return 0;
}

/* init_threads() creates realtime threads, exports functions to
 do the realtime control, and adds the functions to the threads.
 */
static int func_8(void) {
	double base_period_sec, servo_period_sec;
	int servo_base_ratio;
	int retval;


    rtapi_print_msg(RTAPI_MSG_DBG, "MOTION: func_8() starting...\n");

	/* if base_period not specified, assume same as servo_period */
	if (base_period_nsec == 0) {
		base_period_nsec = servo_period_nsec;
	}
	if (traj_period_nsec == 0) {
		traj_period_nsec = servo_period_nsec;
	}

	//traj_period_nsec = 1000000;
	base_period_nsec = 50000;
    traj_period_nsec = traj_period_nsec*1000000;//ghf-20150623

	//rtapi_print_msg(RTAPI_MSG_ERR,
	//    "MOTION: servo_period_nsec %ld nsec\n", servo_period_nsec);	    

	/* servo period must be greater or equal to base period */
	if (servo_period_nsec < base_period_nsec) {
        rtapi_print_msg(RTAPI_MSG_ERR, "MOTION: bad servo period %ld nsec\n",
                servo_period_nsec);
		return -1;
	}
	/* convert desired periods to floating point */
	base_period_sec = base_period_nsec * 0.000000001;
	servo_period_sec = servo_period_nsec * 0.000000001;
	/* calculate period ratios, round to nearest integer */
	servo_base_ratio = (servo_period_sec / base_period_sec) + 0.5;
	/* revise desired periods to be integer multiples of each other */
	servo_period_nsec = base_period_nsec * servo_base_ratio;

    rtapi_print_msg(RTAPI_MSG_ERR,
            "MOTION: traj_period_nsec %ld nsec,base_period_nsec = %ld ,servo_period_nsec = %ld\n",
            traj_period_nsec, base_period_nsec,servo_period_nsec);
	/* create HAL threads for each period */
	/* only create base thread if it is faster than servo thread */
	/*if (servo_base_ratio > 1) {
	 retval = hal_create_thread("base-thread", base_period_nsec, base_thread_fp);
	 if (retval < 0) {
	 rtapi_print_msg(RTAPI_MSG_ERR,
	 "MOTION: failed to create %ld nsec base thread\n",
	 base_period_nsec);
	 return -1;
	 }
	 }*/

	//ghf-2014/6/4
    retval = hal_create_thread("sqpreprocseg-thread", servo_period_nsec / 5, 1,0);
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                "MOTION: failed to create %ld nsec sqpreprocseg thread\n",
                servo_period_nsec / 5);
		return -1;
	}

    retval = hal_export_funct("sqpreprocseg-controller", sqpreprocsegController,
            0 , 1 , 0 , mot_comp_id);
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                "MOTION: failed to export sqpreprocsegController function\n");
		return -1;
    }

    printf("now creat thread time is %ld \n",servo_period_nsec);
    retval = hal_create_thread("servo-thread", servo_period_nsec, 1,1);
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                "MOTION: failed to create %ld nsec servo thread\n",
                servo_period_nsec);
		return -1;
	}

	/* export realtime functions that do the real work */
	retval = hal_export_funct("motion-controller", emcmotController, 0 /* arg 
	 */, 1 /* uses_fp */, 0 /* reentrant */, mot_comp_id);
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                "MOTION: failed to export controller function\n");
		return -1;
	}
	retval = hal_export_funct("motion-command-handler", emcmotCommandHandler, 0 /* arg 
	 */, 1 /* uses_fp */, 0 /* reentrant */, mot_comp_id);
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                "MOTION: failed to export command handler function\n");
		return -1;
	}
	/*! \todo Another #if 0 */
#if 0
	/*! \todo FIXME - currently the traj planner is called from the controller */
	/* eventually it will be a separate function */
	retval = hal_export_funct("motion-traj-planner", emcmotTrajPlanner, 0 /* arg 
			 */, 1 /* uses_fp */,
			0 /* reentrant */, mot_comp_id);
	if (retval < 0) {
		rtapi_print_msg(RTAPI_MSG_ERR,
                "MOTION: failed to export traj planner function\n");
		return -1;
	}
#endif

	// if we don't set cycle times based on these guesses, emc doesn't
	// start up right
	setServoCycleTime(servo_period_nsec * 1e-9);
	setTrajCycleTime(traj_period_nsec * 1e-9);

    rtapi_print_msg(RTAPI_MSG_DBG, "MOTION: func_8() complete\n");

	return 0;
}

void func_6(unsigned long nsec) {
	int servo_mult;
	//servo_mult = traj_period_nsec / nsec;
	servo_mult = ceil(traj_period_nsec / nsec);    //ghf-2014/4/3
	if (servo_mult < 0)
		servo_mult = 1;
	//setTrajCycleTime(nsec * 1e-9);
	//setServoCycleTime(nsec * servo_mult * 1e-9);
	setTrajCycleTime(nsec * servo_mult * 1e-9);    //ghf-2014/4/3
	setServoCycleTime(nsec * 1e-9);
}
/* call this when setting the trajectory cycle time */
static int func_4(double secs) {
	static int t;

    rtapi_print_msg(RTAPI_MSG_DBG,
			"MOTION: setting Traj cycle time to %ld nsecs\n",
			(long) (secs * 1e9));

	/* make sure it's not zero */
	if (secs <= 0.0) {

		return -1;
	}

	emcmot_config_change();

	/* compute the interpolation rate as nearest integer to traj/servo */
	if (emcmotConfig->servoCycleTime)
		emcmotConfig->interpolationRate = (int) (secs
				/ emcmotConfig->servoCycleTime + 0.5);
	else
		emcmotConfig->interpolationRate = 1;

	/* set traj planner */
	//tpSetCycleTime(&emcmotDebug->queue, secs);
	sqSetCycleTime(&queue, secs);

	/* set the free planners, cubic interpolation rate and segment time */
	for (t = 0; t < num_joints; t++) {
		cubicSetInterpolationRate(&(joints[t].cubic),
				emcmotConfig->interpolationRate);
	}

	//rtapi_print_msg(RTAPI_MSG_ERR,
	//    "in setTrajCycleTime,emcmotConfig->interpolationRate %d nsec\n", emcmotConfig->interpolationRate);

	/* copy into status out */
	emcmotConfig->trajCycleTime = secs;

    //rtapi_print_msg(RTAPI_MSG_ERR,"emcmotConfig->trajCycleTime=%ld\n",(long)(emcmotConfig->trajCycleTime*1000000000));

	return 0;
}

/* call this when setting the servo cycle time */
static int func_5(double secs) {
	static int t;

    rtapi_print_msg(RTAPI_MSG_DBG,
			"MOTION: setting Servo cycle time to %ld nsecs\n",
			(long) (secs * 1e9));

	/* make sure it's not zero */
	if (secs <= 0.0) {
		return -1;
	}

	emcmot_config_change();

	/* compute the interpolation rate as nearest integer to traj/servo */
	emcmotConfig->interpolationRate = (int) (emcmotConfig->trajCycleTime / secs
			+ 0.5);

	//rtapi_print_msg(RTAPI_MSG_ERR,
	//    "in setServoCycleTime,emcmotConfig->interpolationRate %d nsec\n", emcmotConfig->interpolationRate);	 

	/* set the cubic interpolation rate and PID cycle time */
	for (t = 0; t < num_joints; t++) {
		cubicSetInterpolationRate(&(joints[t].cubic),
				emcmotConfig->interpolationRate);
		cubicSetSegmentTime(&(joints[t].cubic), secs);
	}

	/* copy into status out */
	emcmotConfig->servoCycleTime = secs;

	//rtapi_print_msg(RTAPI_MSG_ERR,"emcmotConfig->servoCycleTime=%ld\n",(long)(emcmotConfig->servoCycleTime*1000000000));

	return 0;
}

void sendDatatoMem(EmcPose *allpos) {


    EmcPose *pos;
    pos = allpos;

    if(motData->lastPos.tran.x == pos->tran.x &&
            motData->lastPos.tran.y == pos->tran.y &&
            motData->lastPos.tran.z == pos->tran.z &&
            motData->lastPos.a == pos->a &&
            motData->lastPos.b == pos->b &&
            motData->lastPos.c == pos->c)
        return;


    motData->setpos[motData->index].tran.x = pos->tran.x;
    motData->setpos[motData->index].tran.y = pos->tran.y;
    motData->setpos[motData->index].tran.z = pos->tran.z;
    motData->setpos[motData->index].a = pos->a;
    motData->setpos[motData->index].b = pos->b;
    motData->setpos[motData->index].c = pos->c;

    motData->index++;

    if(motData->index >= EMCMOT_MAX_SAVEPOS)
        motData->index = 0;

    memcpy(&motData->lastPos,pos,sizeof(EmcPose));

}

//初始化时获取零点时的绝对值坐标
int emczeroPosLoad() {
    double positions[EMCMOT_MAX_JOINTS];
    const char *posfile = "/home/.config/zero_pos";

    FILE *f = fopen(posfile, "r");
    if(!f) return 0;
    int i=0;
    for(i=0; i<EMCMOT_MAX_JOINTS; i++) {
    int r = fscanf(f, "%lf", &positions[i]);
    if(r != 1) { fclose(f); return -1; }
    }
    fclose(f);
    int result = 0;
    for(i=0; i<EMCMOT_MAX_JOINTS; i++) {
        joints[i].abszeroPos = positions[i];
    }
    return result;
}

//把当前绝对值坐标保存为零点时的绝对值坐标
int emcZeroPosSave(int axis) {
//    const char *posfile = "/home/.config/zero_pos";
//    unlink(posfile);

//    FILE *f = fopen(posfile, "w");
//    if(!f) return -1;
//    int i=0;
//    for(i=0; i<EMCMOT_MAX_JOINTS; i++) {
//        if(i == axis) {
//            int r = fprintf(f, "%.17f\n", joints[i].motor_pos_fb);
//             joints[i].abszeroPos = joints[i].motor_pos_fb;
//            if(r < 0) { fclose(f); return -1; }
//        } else {
//            int r = fprintf(f, "%.17f\n", joints[i].abszeroPos);
//            if(r < 0) { fclose(f); return -1; }
//        }
//    }
//    fclose(f);
    joints[axis].abszeroPos = joints[axis].motor_pos_fb;
    return 0;
}
