/********************************************************************
* Description: motion.h
*   Data structures used throughout emc2.
*
* Author:
* License: GPL Version 2
* System: Linux
*
* Copyright (c) 2004 All rights reserved
*
* Last change:
********************************************************************/

/* jmk says: This file is a mess! */

/*

Misc ramblings:

The terms axis and joint are used inconsistently throughout EMC.
For all new code, the usages are as follows:

    axis - one of the nine degrees of freedom, x, y, z, a, b, c, u, v, w
	these refer to axes in Cartesian space, which may or
	may not match up with joints (see below). On Cartesian
	machines they do match up, but for hexapods, robots, and
	other non-Cartesian machines they don't.
    joint - one of the physical degrees of freedom of the machine
	these might be linear (leadscrews) or rotary (rotary
	tables, robot arm joints).  There can be any number of
	joints.  The kinematics code is responsible for translating
	from axis space to joint space and back.

There are three main kinds of data needed by the motion controller

1) data shared with higher level stuff - commands, status, etc.
2) data that is local to the motion controller
3) data shared with lower level stuff - hal pins

In addition, some internal data (2) should be shared for trouble
shooting purposes, even though it is "internal" to the motion
controller.  Depending on the type of data, it can either be
treated as type (1), and made available to the higher level
code, or it can be treated as type (3), and made available to
the hal, so that halscope can monitor it.

This file should ONLY contain structures and declarations for
type (1) items - those that are shared with higher level code.

Type (2) items should be declared in mot_priv.h, along
with type (3) items.

In the interest of retaining my sanity, I'm not gonna attempt
to move everything to its proper location yet....

However, all new items will be defined in the proper place,
and some existing items may be moved from one struct definition
to another.

*/

/* the following line can be used to control where some of the
   "internal" motion controller data is stored.  By default,
   it is stored in staticlly allocated kernel memory.  However,
   if STRUCTS_IN_SHMEM is defined, it will be stored in the
   emcmotStruct shared memory area, for debugging purposes.
*/

// #define STRUCTS_IN_SHMEM



#ifndef MOTION_H
#define MOTION_H

#include "posemath.h"		/* PmCartesian, PmPose, pmCartMag() */
#include "emcpos.h"		/* EmcPose */
#include "../kinematics/segment/cubic.h"		/* CUBIC_STRUCT, CUBIC_COEFF */

#include "emcmotcfg.h"		/* EMCMOT_MAX_JOINTS */
#include "kinematics.h"
#include <stdarg.h>
#include "config.h" //ghf-2014/9/27

#include <linux/string.h> //ghf 2014/3/29,linux字符串函数库

#include <linux/rtc.h>

#include "../kinematics/segment/segmentqueue.h" //ghf 2014-1-25

extern SEGMENTQUEUE queue;  
extern SEGMENT sqSpace[DEFAULT_TC_QUEUE_SIZE];

#ifdef __cplusplus
extern "C" {
#endif

//ghf-2014/10/09
#ifndef _FILER_H
    #define number_first_filter 90
    #define number_second_filter 10
#endif

//ghf-2014/8/29
#define init_encry_ppmc() func_1();
#define firstdecryptInfo(n) func_2(n);
#define emcmot_config_change() func_3();
#define setTrajCycleTime(n) func_4(n);
#define setServoCycleTime(n) func_5(n);
#define emcmotSetCycleTime(n) func_6(n);
#define init_comm_buffers() func_7();
#define init_threads() func_8();

#define checkAllHomed() func_11();
#define refresh_jog_limits(n) func_12(n);
#define clearHomes(n) func_13(n);
#define emcmotDioWrite(n1,n2) func_14(n1,n2);
#define emcmotAioWrite(n1,n2) func_15(n1,n2);
#define emcmotSetRotaryUnlock(n1,n2) func_16(n1,n2);
#define decryptInfo(n) func_17(n);

//#define compute_screw_comp() func_18();
#define update_status() func_19();

#define home_do_moving_checks(n) func_21(n);
#define do_homing_sequence() func_22();
#define do_homing() func_23();
extern void sendDatatoMem(EmcPose *allpos);
extern int emczeroPosLoad();
//把当前绝对值坐标保存为零点时的绝对值坐标
extern int emcZeroPosSave(int axis);

    typedef struct _EMC_TELEOP_DATA {
	EmcPose currentVel;
	EmcPose currentAccell;
	EmcPose desiredVel;
	EmcPose desiredAccell;
    } EMC_TELEOP_DATA;

/* This enum lists all the possible commands */

    typedef enum {
	EMCMOT_ABORT = 1,	/* abort all motion */
	EMCMOT_AXIS_ABORT,	/* abort one axis */ //FIXME-AJ: replace command name to EMCMOT_JOINT_ABORT
	EMCMOT_ENABLE,		/* enable servos for active joints */
	EMCMOT_DISABLE,		/* disable servos for active joints */
	EMCMOT_ENABLE_AMPLIFIER,	/* enable amp outputs */
	EMCMOT_DISABLE_AMPLIFIER,	/* disable amp outputs */
	EMCMOT_ENABLE_WATCHDOG,	/* enable watchdog sound, parport */
	EMCMOT_DISABLE_WATCHDOG,	/* enable watchdog sound, parport */
	EMCMOT_ACTIVATE_JOINT,	/* make joint active */
	EMCMOT_DEACTIVATE_JOINT,	/* make joint inactive */

	EMCMOT_PAUSE,		/* pause motion */
	EMCMOT_RESUME,		/* resume motion */
	EMCMOT_STEP,		/* resume motion until id encountered */
	EMCMOT_FREE,		/* set mode to free (joint) motion */
	EMCMOT_COORD,		/* set mode to coordinated motion */
	EMCMOT_TELEOP,		/* set mode to teleop */

	EMCMOT_SPINDLE_SCALE,	/* set scale factor for spindle speed */
	EMCMOT_SS_ENABLE,	/* enable/disable scaling the spindle speed */
	EMCMOT_FEED_SCALE,	/* set scale factor for feedrate */
	EMCMOT_FS_ENABLE,	/* enable/disable scaling feedrate */
	EMCMOT_FH_ENABLE,	/* enable/disable feed_hold */
	EMCMOT_AF_ENABLE,	/* enable/disable adaptive feedrate */
	EMCMOT_OVERRIDE_LIMITS,	/* temporarily ignore limits until jog done */
    EMCMOT_FG0_ENABLE, /* the feed rate and move of g0 is stable*/

	EMCMOT_HOME,		/* home a joint or all joints */
	EMCMOT_UNHOME,		/* unhome a joint or all joints*/
	EMCMOT_JOG_CONT,	/* continuous jog */
	EMCMOT_JOG_INCR,	/* incremental jog */
	EMCMOT_JOG_ABS,		/* absolute jog */
	EMCMOT_SET_LINE,	/* queue up a linear move */
	EMCMOT_SET_CIRCLE,	/* queue up a circular move */
	EMCMOT_SET_TELEOP_VECTOR,	/* Move at a given velocity but in
					   world cartesian coordinates, not
					   in joint space like EMCMOT_JOG_* */

	EMCMOT_CLEAR_PROBE_FLAGS,	/* clears probeTripped flag */
	EMCMOT_PROBE,		/* go to pos, stop if probe trips, record
				   trip pos */
	EMCMOT_RIGID_TAP,	/* go to pos, with sync to spindle speed, 
				   then return to initial pos */

    EMCMOT_SET_SEC_LIMITS,	/* set the joint position +/- limits */
	EMCMOT_SET_POSITION_LIMITS,	/* set the joint position +/- limits */
	EMCMOT_SET_BACKLASH,	/* set the joint backlash */
	EMCMOT_SET_MIN_FERROR,	/* minimum following error, input units */
	EMCMOT_SET_MAX_FERROR,	/* maximum following error, input units */
	EMCMOT_SET_VEL,		/* set the velocity for subsequent moves */
	EMCMOT_SET_VEL_LIMIT,	/* set the max vel for all moves (tooltip) */
	EMCMOT_SET_JOINT_VEL_LIMIT,	/* set the max joint vel */
	EMCMOT_SET_JOINT_ACC_LIMIT,	/* set the max joint accel */
	EMCMOT_SET_ACC,		/* set the max accel for moves (tooltip) */
	EMCMOT_SET_TERM_COND,	/* set termination condition (stop, blend) */
	EMCMOT_SET_NUM_AXES,	/* set the number of joints */ //FIXME-AJ: function needs to get renamed
	EMCMOT_SET_WORLD_HOME,	/* set pose for world home */
	EMCMOT_SET_HOMING_PARAMS,	/* sets joint homing parameters */
	EMCMOT_SET_DEBUG,       /* sets the debug level */
	EMCMOT_SET_DOUT,        /* sets or unsets a DIO, this can be imediate or synched with motion */
	EMCMOT_SET_AOUT,	/* sets or unsets a AIO, this can be imediate or synched with motion */
        EMCMOT_SET_SPINDLESYNC, /* syncronize motion to spindle encoder */
	
    EMCMOT_SPINDLE_IGNORE,/* ignore the spindle control status,use in sending diff to doublez change*/
	EMCMOT_SPINDLE_ON,	/* start the spindle */
	EMCMOT_SPINDLE_OFF,	/* stop the spindle */
	EMCMOT_SPINDLE_INCREASE,	/* spindle faster */
	EMCMOT_SPINDLE_DECREASE,	/* spindle slower */
	EMCMOT_SPINDLE_BRAKE_ENGAGE,	/* engage the spindle brake */
	EMCMOT_SPINDLE_BRAKE_RELEASE,	/* release the spindle brake */
	EMCMOT_SET_MOTOR_OFFSET,	/* set the offset between joint and motor */
	EMCMOT_SET_JOINT_COMP,	/* set a compensation triplet for a joint (nominal, forw., rev.) */
        EMCMOT_SET_OFFSET, /* set tool offsets */
      EMCMOT_DOUBLEZ, /* set which z to go. ghf-2014/2/24 */
      EMCMOT_JOGWHEELS_GUIDE,/*  ghf-2014/3/11 */
      EMCMOT_SETGMODE,/*  ghf-2014/3/12 */
      EMCMOT_SETLIGHT,/*  ghf-2014/3/17 */
      EMCMOT_RUNSTATUS,/* hong-2014/11/03*/
      EMCMOT_ENCRYPT,/*  ghf-2014/3/28 */
      EMCMOT_FILTERTIME,/*  ghf-2014/5/6 */

    EMCMOT_SETLEFTTIME,  /*  hong-2014/07/21*/
    EMCMOT_WRITEID, //ghf-2014/7/24
    EMCMOT_GETRTC, //ghf-2015/1/6
    EMCMOT_SETIO,
    EMCMOT_SETZMODE,//hong-2014/09/11
    EMCMOT_FREECHANGEOK,//ghf-2015-4-13
    EMCMOT_SET_PAR,
    EMCMOT_MULTIFUNC    //HONG 2015-10-23

    } cmd_code_t;

/* this enum lists the possible results of a command */

    typedef enum {
	EMCMOT_COMMAND_OK = 0,	/* cmd honored */
	EMCMOT_COMMAND_UNKNOWN_COMMAND,	/* cmd not understood */
	EMCMOT_COMMAND_INVALID_COMMAND,	/* cmd can't be handled now */
	EMCMOT_COMMAND_INVALID_PARAMS,	/* bad cmd params */
    EMCMOT_COMMAND_BAD_EXEC,	/* error trying to initiate */
    EMCMOT_COMMAND_DOUBLEZOK, //ghf-2014/7/30
    EMCMOT_COMMAND_LINECYCLEOK //ghf-2014/10/20
    } cmd_status_t;

    typedef enum {
    EMCSP_VELCONTROL = 0,
    EMCSP_STARTPOSCTL,
    EMCSP_MOVEORIGIN,
    EMCSP_SYNCMOVEIN,
    EMCSP_SYNCMOVEWAIT,
    EMCSP_SYNCMOVEOUT,
    EMCSP_SYNCMOVEEND

    } spindle_status_t;

/* termination conditions for queued motions */
#define EMCMOT_TERM_COND_STOP 1
#define EMCMOT_TERM_COND_BLEND 2

/*********************************
       COMMAND STRUCTURE
*********************************/

/* This is the command structure.  There is one of these in shared
   memory, and all commands from higher level code come thru it.
*/
    typedef struct emcmot_command_t {
	unsigned char head;	/* flag count for mutex detect */
	cmd_code_t command;	/* command code (enum) */
	int commandNum;		/* increment this for new command */
	double motor_offset;    /* offset from joint to motor position */
	double maxLimit;	/* pos value for position limit, output */
    double minLimit;	/* neg value for position limit, output */
    double secmaxLimit;	/* pos value for position limit, output */
    double secminLimit;	/* neg value for position limit, output */
    double secLimitoffset;
	EmcPose pos;		/* line/circle endpt, or teleop vector */
	PmCartesian center;	/* center for circle */
	PmCartesian normal;	/* normal vec for circle */
	int turn;		/* turns for circle or which rotary to unlock for a line */
	double vel;		/* max velocity */
        double ini_maxvel;      /* max velocity allowed by machine
                                   constraints (the ini file) */
        int motion_type;        /* this move is because of traverse, feed, arc, or toolchange */
        double spindlesync;     /* user units per spindle revolution, 0 = no sync */

        int spindleignore; /* ignore the spindle control  */
	double acc;		/* max acceleration */
	double backlash;	/* amount of backlash */
	int id;			/* id for motion */
	int termCond;		/* termination condition */
	double tolerance;	/* tolerance for path deviation in CONTINUOUS mode */
	int axis;		/* which index to use for below */ //FIXME-AJ: replace with joint
	double scale;		/* velocity scale or spindle_speed scale arg */
	double offset;		/* input, output, or home offset arg */
	double home;		/* joint home position */
	double home_final_vel;	/* joint velocity for moving from OFFSET to HOME */
    int homeflag;
    double search_vel;	/* home search velocity */
	double latch_vel;	/* home latch velocity */
	int flags;		/* homing config flags, other boolean args */
	int home_sequence;      /* order in homing sequence */        
        double pitchvalue;  //ghf-2014/6/24
        double distancecheck;
        int volatile_home;      /* joint should get unhomed when we get unhome -2 
                                   (generated by task upon estop, etc) */
	double minFerror;	/* min following error */
	double maxFerror;	/* max following error */
	int wdWait;		/* cycle to wait before toggling wd */
	int debug;		/* debug level, from DEBUG in .ini file */
	unsigned char now, out, start, end;	/* these are related to synched AOUT/DOUT. now=wether now or synched, out = which gets set, start=start value, end=end value */
	unsigned char mode;	/* used for turning overrides etc. on/off */
	double comp_nominal, comp_forward, comp_reverse; /* compensation triplet, nominal, forward, reverse */
        unsigned char probe_type; /* ~1 = error if probe operation is unsuccessful (ngc default)
                                     |1 = suppress error, report in # instead
                                     ~2 = move until probe trips (ngc default)
                                     |2 = move until probe clears */
        EmcPose tool_offset;        /* TLO */
	int set_which_z;//ghf-2014/2/24
	int m6_flag; //ghf-2014/4/2
	double zdiff;//ghf-2014/4/28
    int forcflag;//ghf-2015-4-9
    double freevel;
    int selectz;//hong - use in handlewheel to mark which z is selected
    double maxdoublezpos;
	unsigned char jogwheels_guide;//ghf-2014/3/11,1代表开启手轮引导
        GMODE G_mode_0;//ghf-2014/3/12
        GMODE G_mode_1;

        int tool; //ghf-2014/10/27,segment要实时反馈刀具号
        int compside; //hong-2014/10/28,反馈的补偿方向
        double dynamiccomp; //hong 2015-07-4 dynamic compensation value
        double dynamicparameter[DYNAMIC_PAR_NUM];

        //ghf-2014/10/13
        double angle1; //相邻两线段的夹角1,范围（90-180度）
        double angle2; //相邻两线段的夹角2,范围（90-angle1度）
        double coeff1; //夹角1系数限制，范围（0-1）
        double coeff2; //夹角2系数限制，范围（0-1）
        double coeff3; //m54g0系数

        unsigned char setlight;//ghf-2014/3/17,根据当前运行状态，点亮相应的灯;1：运行，绿灯
                                                                                           // 2： 暂停，黄灯
                                                                                           // 3：报警，红灯  
        int run_status;
        int gmodetype;//ghf-2014/3/22
        
        int g0enableflag;//hong-2014/11/06

        unsigned int filtertimebuf[64];//ghf-2014/5/6
        unsigned int randtime;//ghf-2014/7/8
        unsigned int randbuf;//ghf-2014/7/8

        //ghf-2014/7/24
        unsigned char customid[2];//客户编号
        unsigned char rlefttime[8];//剩余时间
        unsigned char encrychipid[8];//加密芯片id
        unsigned char encryptbuf[64];//密文密钥

        unsigned char cpuid[8];//cpuid
        unsigned char hddid[4];//硬盘id
        unsigned char wlefttime[4];//重置剩余时间
        unsigned char wlefttimeflag;//为1,终止现有读写，写时间下去

        int externionum;
        int externiosta;
        int zmode;  //hong-2014/09/11 0 means linkage,1 means rotatios
        int spindlezmode;

        /****************************************************************/
        //heyuebang 2014-11-20
         double fric_delay_t;//延时时间
         double fric_lag_ts0, fric_lag_ts1;		/* 指数函数的衰减时间常数 ts0为正向*/
         double fric_max_u0, fric_max_u1;		/* 指数函数的峰值 */
         int fric_type;//0:no comp  1:comp1  2:comp2
        /****************************************************************/
         int intmotpar[50];                 //hong 2015-08-07  use for pass the parameter
         double doumotpar[50];      //hong 2015-08-07  use for pass the parameter
         int axisuseposfb;          //hong 2015-08-11 use for pass the parameter useposfb
         int useabspos;

         double axis_offset[EMCMOT_MAX_JOINTS];
         /* MULTI_FUNC variant */
         int multifunctype;
         int multifuncintval;
         double multifuncdoubleval;
         unsigned char tail;	/* flag count for mutex detect */
    } emcmot_command_t;

/*! \todo FIXME - these packed bits might be replaced with chars
   memory is cheap, and being able to access them without those
   damn macros would be nice
*/

/* motion flag type */
    typedef unsigned short EMCMOT_MOTION_FLAG;

/*
  motion status flag structure-- looks like:

  MSB                             LSB
  v---------------v------------------v
  |   |   |   | T | CE | C | IP | EN |
  ^---------------^------------------^

  where:

  EN is 1 if calculations are enabled, 0 if not
  IP is 1 if all joints in position, 0 if not
  C is 1 if coordinated mode, 0 if in free mode
  CE is 1 if coordinated mode error, 0 if not
  T is 1 if we are in teleop mode.
  */

/* bit masks */
#define EMCMOT_MOTION_ENABLE_BIT      0x0001
#define EMCMOT_MOTION_INPOS_BIT       0x0002
#define EMCMOT_MOTION_COORD_BIT       0x0004
#define EMCMOT_MOTION_ERROR_BIT       0x0008
#define EMCMOT_MOTION_TELEOP_BIT      0x0010

/* joint flag type */
    typedef unsigned short EMCMOT_JOINT_FLAG;
/*
  joint status flag structure-- looks like:

  MSB                                                          LSB
  ----------v-----------------v--------------------v-------------------v
  | AF | FE | AH | HD | H | HS | NHL | PHL | - | - | ER | IP | AC | EN |
  ----------^-----------------^--------------------^-------------------^
               

  x = unused

  where:

  EN  is 1 if joint amplifier is enabled, 0 if not
  AC  is 1 if joint is active for calculations, 0 if not
  IP  is 1 if joint is in position, 0 if not (free mode only)
  ER  is 1 if joint has an error, 0 if not

  PHL is 1 if joint is on maximum hardware limit, 0 if not
  NHL is 1 if joint is on minimum hardware limit, 0 if not

  HS  is 1 if joint home switch is tripped, 0 if not
  H   is 1 if joint is homing, 0 if not
  HD  is 1 if joint has been homed, 0 if not
  AH  is 1 if joint is at home position, 0 if not

  FE  is 1 if joint exceeded following error, 0 if not
  AF  is 1 if amplifier is faulted, 0 if not

Suggestion: Split this in to an Error and a Status flag register..
             Then a simple test on each of the two flags can be performed
             rather than testing each bit... Saving on a global per joint
             fault and ready status flag.
  */

/* bit masks */
#define EMCMOT_JOINT_ENABLE_BIT         0x0001
#define EMCMOT_JOINT_ACTIVE_BIT         0x0002
#define EMCMOT_JOINT_INPOS_BIT          0x0004
#define EMCMOT_JOINT_ERROR_BIT          0x0008

#define EMCMOT_JOINT_MAX_HARD_LIMIT_BIT 0x0040
#define EMCMOT_JOINT_MIN_HARD_LIMIT_BIT 0x0080

#define EMCMOT_JOINT_HOME_SWITCH_BIT    0x0100
#define EMCMOT_JOINT_HOMING_BIT         0x0200
#define EMCMOT_JOINT_HOMED_BIT          0x0400

/*! \todo FIXME - I'm not sure AT_HOME is being reported correctly.
   AT_HOME is cleared when you jog in free mode, but not if
   you do a coordinated move... perhaps that is the intended
   behavior.
*/
#define EMCMOT_JOINT_AT_HOME_BIT        0x0800

#define EMCMOT_JOINT_FERROR_BIT         0x1000
#define EMCMOT_JOINT_FAULT_BIT          0x2000

/*! \todo FIXME - the terms "teleop", "coord", and "free" are poorly
   documented.  This is my feeble attempt to understand exactly
   what they mean.

   According to Fred, teleop is never used with machine tools,
   although that may not be true for machines with non-trivial
   kinematics.

   "coord", or coordinated mode, means that all the joints are
   synchronized, and move together as commanded by the higher
   level code.  It is the normal mode when machining.  In
   coordinated mode, commands are assumed to be in the cartesean
   reference frame, and if the machine is non-cartesean, the
   commands are translated by the kinematics to drive each
   joint in joint space as needed.

   "free" mode means commands are interpreted in joint space.
   It is used for jogging individual joints, although
   it does not preclude multiple joints moving at once (I think).
   Homing is also done in free mode, in fact machines with
   non-trivial kinematics must be homed before they can go
   into either coord or teleop mode.

   'teleop' is what you probably want if you are 'jogging'
   a hexapod.  The jog commands as implemented by the motion
   controller are joint jogs, which work in free mode.  But
   if you want to jog a hexapod or similar machine along
   one particular cartesean axis, you need to operate more
   than one joint.  That's what 'teleop' is for.

*/

/* compensation structures */
    typedef struct {
	double nominal;		/* nominal (command) position */
	float fwd_trim;		/* correction for forward movement */
	float rev_trim;		/* correction for reverse movement */
	float fwd_slope;	/* slopes between here and next pt */
	float rev_slope;
    } emcmot_comp_entry_t; 


#define EMCMOT_COMP_SIZE 256
    typedef struct {
	int entries;		/* number of entries in the array */
	emcmot_comp_entry_t *entry;  /* current entry in array */
	emcmot_comp_entry_t array[EMCMOT_COMP_SIZE+2];
	/* +2 because array has -HUGE_VAL and +HUGE_VAL entries at the ends */
    } emcmot_comp_t;

/* motion controller states */

    typedef enum {
	EMCMOT_MOTION_DISABLED = 0,
	EMCMOT_MOTION_FREE,
	EMCMOT_MOTION_TELEOP,
	EMCMOT_MOTION_COORD
    } motion_state_t;

/* states for homing */
    typedef enum {
	HOME_IDLE = 0,
	HOME_START,			// 1 
	HOME_UNLOCK,			// 2 
	HOME_UNLOCK_WAIT,		// 3 
	HOME_INITIAL_BACKOFF_START,	// 4 
	HOME_INITIAL_BACKOFF_WAIT,	// 5 
	HOME_INITIAL_SEARCH_START,	// 6 
	HOME_INITIAL_SEARCH_WAIT,	// 7 
	HOME_SET_COARSE_POSITION,	// 8 
	HOME_FINAL_BACKOFF_START,	// 9 
	HOME_FINAL_BACKOFF_WAIT,	// 10
	HOME_RISE_SEARCH_START,		// 11
	HOME_RISE_SEARCH_WAIT,		// 12
	HOME_FALL_SEARCH_START,		// 13
	HOME_FALL_SEARCH_WAIT,		// 14
	HOME_SET_SWITCH_POSITION,	// 15
	HOME_INDEX_ONLY_START,		// 16
	HOME_INDEX_SEARCH_START,	// 17
	HOME_INDEX_SEARCH_WAIT,		// 18
	HOME_SET_INDEX_POSITION,	// 19
	HOME_FINAL_MOVE_START,		// 20
	HOME_FINAL_MOVE_WAIT,		// 21
	HOME_LOCK,			// 22
	HOME_LOCK_WAIT,			// 23
	HOME_FINISHED,			// 24
    HOME_ABORT,			// 25
    HOME_WAIT_SETCOARSEPOS, //总线时粗定位清零等待下面通讯
    HOME_WAIT_SETPOS //总线时粗定位清零等待下面通讯

    } home_state_t;

    typedef enum {
	HOME_SEQUENCE_IDLE = 0,
	HOME_SEQUENCE_START,
	HOME_SEQUENCE_START_JOINTS,
	HOME_SEQUENCE_WAIT_JOINTS,
    } home_sequence_state_t;

/* flags for homing */
#define HOME_IGNORE_LIMITS	1
#define HOME_USE_INDEX		2
#define HOME_IS_SHARED		4
#define HOME_UNLOCK_FIRST       8

/* flags for enabling spindle scaling, feed scaling,
   adaptive feed, and feed hold */

#define SS_ENABLED 0x01
#define FS_ENABLED 0x02
#define AF_ENABLED 0x04
#define FH_ENABLED 0x08

/* This structure contains all of the data associated with
   a single joint.  Note that this structure does not need
   to be in shared memory (but it can, if desired for debugging
   reasons).  The portions of this structure that are considered
   "status" and need to be made available to user space are
   copied to a much smaller struct called emcmot_joint_status_t
   which is located in shared memory.

*/
    typedef struct {

	/* configuration info - changes rarely */
	int type;		/* 0 = linear, 1 = rotary */
	double max_pos_limit;	/* upper soft limit on joint pos */
	double min_pos_limit;	/* lower soft limit on joint pos */
    double sec_pos_limit;	/* the second postive soft limit  */
    double sec_neg_limit;	/* the second negative soft limit */
    double sec_limit_offset; // the third soft limit
	double max_jog_limit;	/* jog limits change when not homed */
	double min_jog_limit;
	double vel_limit;	/* upper limit of joint speed */
	double acc_limit;	/* upper limit of joint accel */
	double min_ferror;	/* zero speed following error limit */
	double max_ferror;	/* max speed following error limit */
	double home_search_vel;	/* dir/spd to look for home switch */
	double home_final_vel;  /* speed to travel from OFFSET to HOME position */
	double home_latch_vel;	/* dir/spd to latch switch/index pulse */

    double oldhome_searchvel;//ghf-2014/10/11
    double oldhome_latchvel;

	double home_offset;	/* dir/dist from switch to home point */
	double home;		/* joint coordinate of home point */
	int home_flags;		/* flags for various homing options */
    int homeflag;
    int volatile_home;      /* joint should get unhomed when we get unhome -2
                                   (generated by task upon estop, etc) */
	double backlash;	/* amount of backlash */
	int home_sequence;      /* Order in homing sequence */
	
	double pitchvalue;  //ghf-2014/6/24

    double distancecheck;   //hong-20160420
	
	emcmot_comp_t comp;	/* leadscrew correction data */

	/* status info - changes regularly */
	/* many of these need to be made available to higher levels */
	/* they can either be copied to the status struct, or an array of
	   joint structs can be made part of the status */
	EMCMOT_JOINT_FLAG flag;	/* see above for bit details */
	double coarse_pos;	/* trajectory point, before interp */
    double coarsevel_cmd;//ghf-2014/9/18,此值用于计算螺距补偿
    double program_cmd;		/* commanded joint position */
	double pos_cmd;		/* commanded joint position */
    double freepos;         //用于双Z轴切换,the position that it need to move
	double vel_cmd;		/* comanded joint velocity */
	double backlash_corr;	/* correction for backlash */
	double backlash_filt;	/* filtered backlash correction */
	double backlash_vel;	/* backlash velocity variable */
	double motor_pos_cmd;	/* commanded position, with comp */
	double motor_pos_fb;	/* position feedback, with comp */
	double pos_fb;		/* position feedback, comp removed */
	double ferror;		/* following error */
	double ferror_limit;	/* limit depends on speed */
	double ferror_high_mark;	/* max following error */
	double free_pos_cmd;	/* position command for free mode TP */
	double free_vel_lim;	/* velocity limit for free mode TP */
	int free_tp_enable;	/* if zero, joint stops ASAP */
	int free_tp_active;	/* if non-zero, move in progress */
	int kb_jog_active;	/* non-zero during a keyboard jog */
	int wheel_jog_active;	/* non-zero during a wheel jog */

	/* internal info - changes regularly, not usually accessed from user
	   space */
	CUBIC_STRUCT cubic;	/* cubic interpolator data */

	int on_pos_limit;	/* non-zero if on limit */
	int on_neg_limit;	/* non-zero if on limit */
	double home_sw_pos;	/* latched position of home sw */
	int home_pause_timer;	/* used to delay between homing states */
	int index_enable;	/* current state of index enable pin */
    int homeclearpos;   //回零时坐标改变的信号

	home_state_t home_state;	/* state machine for homing */
	double motor_offset;	/* diff between internal and motor pos, used
				   to set position to zero during homing */
	int old_jog_counts;	/* prior value, used for deltas */

	/* stuff moved from the other structs that might be needed (or might
	   not!) */
	double big_vel;		/* used for "debouncing" velocity */

    //ghf-2014/10/09
    /****************************************************************/
    //add by heyuebang in 2014-09-29
    // Two arrays is defined for mean filtering
    // Purpose: two grade mean filter is used for limitling the maximal jerk

    double First_filter[number_first_filter];
    double Second_filter[number_second_filter];
  //  double First_filter[40];
  //  double Second_filter[10];
    double old_pos_cmd;
    double old_v_cmd;

    //ghf-20150618
    int oldphlflag;
    int oldnhlflag;


    int useAbsPos;      //该轴是绝对值编码器

    int abszeroSt;          //绝对值编码器设定零点状态
    double abszeroPos;  //零点的绝对值的坐标
    int errorCode;  // error code form the motor

    /****************************************************************/
    //heyuebang 2014-11-20
     double positive_vel_t,negitive_vel_t;	/* 速度从0往正向过度时开始计时 */
     double fric_delay_t;//延时时间
     double fric_lag_ts0, fric_lag_ts1;		/* 指数函数的衰减时间常数 ts0为正向*/
     double fric_max_u0, fric_max_u1;		/* 指数函数的峰值 */
     double fric_out_u;		/* 指数函数的输出，作为实际的补偿值*/
   /****************************************************************/
    int useposfb;       //0 means do not use the feedback ,that it connect to the cmd directly,1 connect to the encoder.position   hong 20150811
        /****************************************************************/
    double axis_offset; //the offset while on the zero position
    double switch_offset; //the offset while on the zero position
    int axis_active;        //means is used as the the active axis currently,it moves follow the ngc codes
    int circle_shortest;        //圆弧轴最短路径移动

    } emcmot_joint_t;

/* This structure contains only the "status" data associated with
   a joint.  "Status" data is that data that should be reported to
   user space on a continuous basis.  An array of these structs is
   part of the main status structure, and is filled in with data
   copied from the emcmot_joint_t structs every servo period.

   For now this struct contains more data than it really needs, but
   paring it down will take time (and probably needs to be done one
   or two items at a time, with much testing).  My main goal right
   now is to get get the large joint struct out of status.

*/
    typedef struct {

	EMCMOT_JOINT_FLAG flag;	/* see above for bit details */
	double pos_cmd;		/* commanded joint position */
	double pos_fb;		/* position feedback, comp removed */
    double motor_fbpos; //电机驱动器反馈数值
    double coarse_pos;//ghf-2014/8/7
	double vel_cmd;         /* current velocity */
	double ferror;		/* following error */
	double ferror_high_mark;	/* max following error */

/*! \todo FIXME - the following are not really "status", but taskintf.cc expects
   them to be in the status structure.  I don't know how or if they are
   used by the user space code.  Ideally they will be removed from here,
   but each one will need to be investigated individually.
*/
	double backlash;	/* amount of backlash */
    double backlash_filt;//ghf-2014/7/29
	double max_pos_limit;	/* upper soft limit on joint pos */
	double min_pos_limit;	/* lower soft limit on joint pos */
    double sec_limit_offset; // the third soft limit
    double sec_pos_limit;	/* sec soft limit on joint pos */
    double sec_neg_limit;	/* sec soft limit on joint pos */
	double min_ferror;	/* zero speed following error limit */
	double max_ferror;	/* max speed following error limit */
	double home_offset;	/* dir/dist from switch to home point */
    int errorCode;  // error code form the motor
    double abszeropos;  //record the zero position
    } emcmot_joint_status_t;


    typedef struct {
    int ignoreflag;  //when 1 then ignore the spinble control;
	double speed;		// spindle speed in RPMs
	double css_factor;
	double xoffset;
	int direction;		// 0 stopped, 1 forward, -1 reverse
	int brake;		// 0 released, 1 engaged

    double speedrpm;
    double maxvelocity;
    double acclimit;
    double setpos;
    double fbnowpos;
    double endpos;
    double nowpos;
    double nowvel;
    double recordvel;
    int spindlestatus;
    int startmove;
    int syncjoint;
    int inpos;
    int stopcount;
    int startcount;
    int stopmove;   //主轴M5执行
    int moveType;   //轴速度位置模式切换

    } spindle_status;
    

//ghf-2014/3/11,in segmentqueue.h
/*typedef struct _GMode
{
  double maxV; 
  double maxAcc;
  double maxJerk;
  double maxAcc_nLine;
  double maxAcc_circle;
  double maxAcc_nCircle;
} GMODE;*/

/*********************************
        STATUS STRUCTURE
*********************************/

/* This is the status structure.  There is one of these in shared
   memory, and it reports motion controller status to higher level
   code in user space.  For the most part, this structure contains
   higher level variables - low level stuff is made visible to the
   HAL and troubleshooting, etc, is done using the HAL oscilliscope.
*/

/*! \todo FIXME - this struct is broken into two parts... at the top are
   structure members that I understand, and that are needed for emc2.
   Other structure members follow.  All the later ones need to be
   evaluated - either they move up, or they go away.
*/
typedef struct emcdata_struct {
    EmcPose setpos[EMCMOT_MAX_SAVEPOS];
    EmcPose lastPos;
    int index;
    int locksig;
} emcdata_struct;

    typedef struct emcmot_status_t {
	unsigned char head;	/* flag count for mutex detect */
	/* these three are updated only when a new command is handled */
	cmd_code_t commandEcho;	/* echo of input command */
	int commandNumEcho;	/* echo of input command number */
	cmd_status_t commandStatus;	/* result of most recent command */
	/* these are config info, updated when a command changes them */
	double feed_scale;	/* velocity scale factor for all motion */
	double spindle_scale;	/* velocity scale factor for spindle speed */
	unsigned char enables_new;	/* flags for FS, SS, etc */
		/* the above set is the enables in effect for new moves */
	/* the rest are updated every cycle */
	double net_feed_scale;	/* net scale factor for all motion */
	double net_spindle_scale;	/* net scale factor for spindle */
	unsigned char enables_queued;	/* flags for FS, SS, etc */
		/* the above set is the enables in effect for the
		   currently executing move */
	motion_state_t motion_state; /* operating state: FREE, COORD, etc. */
	EMCMOT_MOTION_FLAG motionFlag;	/* see above for bit details */
	EmcPose carte_pos_cmd;	/* commanded Cartesian position */
	int carte_pos_cmd_ok;	/* non-zero if command is valid */
	EmcPose carte_pos_fb;	/* actual Cartesian position */
	int carte_pos_fb_ok;	/* non-zero if feedback is valid */
	EmcPose world_home;	/* cartesean coords of home position */
	int homing_active;	/* non-zero if any joint is homing */
	home_sequence_state_t homingSequenceState;
	emcmot_joint_status_t joint_status[EMCMOT_MAX_JOINTS];	/* all joint status data */

	int on_soft_limit;	/* non-zero if any joint is on soft limit */

    int probeVal;		/* debounced value of probe input */
    double useSeclimit;  //According to the position of axis z,if less than this value,than second limit make effect


    int probeTripped;	/* Has the probe signal changed since start
				   of probe command? */
    int probing;		/* Currently looking for a probe signal? */
        unsigned char probe_type;
	EmcPose probedPos;	/* Axis positions stored as soon as possible
				   after last probeTripped */
        int spindle_index_enable;  /* hooked to a canon encoder index-enable */
        int spindleSync;        /* we are doing spindle-synced motion */
        double spindleRevs;     /* position of spindle in revolutions */
        double spindleSpeedIn;  /* velocity of spindle in revolutions per minute */

	spindle_status spindle;	/* data types for spindle status */
	
	int synch_di[EMCMOT_MAX_DIO]; /* inputs to the motion controller, queried by g-code */
	int synch_do[EMCMOT_MAX_DIO]; /* outputs to the motion controller, queried by g-code */
	double analog_input[EMCMOT_MAX_AIO]; /* inputs to the motion controller, queried by g-code */
	double analog_output[EMCMOT_MAX_AIO]; /* outputs to the motion controller, queried by g-code */

/*! \todo FIXME - all structure members beyond this point are in limbo */

	/* dynamic status-- changes every cycle */
	unsigned int heartbeat;
	int config_num;		/* incremented whenever configuration
				   changed. */
	double computeTime;
	int id;			/* id for executing motion */
	int depth;		/* motion queue depth */
	int activeDepth;	/* depth of active blend elements */
	int queueFull;		/* Flag to indicate the tc queue is full */
	int paused;		/* Flag to signal motion paused */
	int overrideLimitMask;	/* non-zero means one or more limits ignored */
				/* 1 << (joint-num*2) = ignore neg limit */
				/* 2 << (joint-num*2) = ignore pos limit */


	/* static status-- only changes upon input commands, e.g., config */
	double vel;		/* scalar max vel */
	double acc;		/* scalar max accel */

	int level;
        int motionType;
        double distance_to_go;  /* in this move */
        double distance_to_go_x;//ghf-2014/6/24
        double distance_to_go_y;
        double distance_to_go_z;
        EmcPose dtg;
        double current_vel;
        double requested_vel;

        unsigned int tcqlen;
        EmcPose tool_offset;
        int atspeed_next_feed;  /* at next feed move, wait for spindle to be at speed  */
        int spindle_is_atspeed; /* hal input */

    int doublezok; //ghf-2014/7/30,这个是反馈到上面的用于判断是否发送成功
	int set_which_z;//ghf-2014/2/24，这个是上面传递下来的
    int nousedoublez;//是否使用双Z轴标志
    double doublez_base_pos; //hong
	int m6_flag;//ghf-2014/4/2
	double zdiff;//ghf-2014/4/28
    unsigned char jogwheels_guide; //ghf-2014/3/11

        GMODE G_mode_0;
        GMODE G_mode_1;
        unsigned char run_on;//外部启动按钮按下为1,ghf-2014/3/13
        unsigned char pause_on;//外部暂停按钮按下为1,ghf-2014/3/13
        
        unsigned char lightvalue;//ghf-2014/3/17,表示当前运行状态，1：运行/暂停，绿灯亮
                                                                          // 2：空闲，黄灯亮
                                                                          // 3：报警，红灯亮
                                                                          // 0：都不亮，刚上电是这种状态，如果上电报警，红灯要点亮  

    int run_status;
        unsigned int filtertimebuf[64];//ghf-2014/5/6

        double switch_posfb[EMCMOT_MAX_JOINTS];//ghf-2014/5/5
        char user_defin[EMCMOT_MAX_USR_INIO]; //ghf-2014/5/24
        char user_defout[EMCMOT_MAX_USR_OUTIO];//hong-2014/05/29
        char user_defwarn[EMCMOT_MAX_USR_WARNIO];//hong-2014/05/29

        char mot_extern_in[48];//hong-2014/08/23
        char mot_extern_out[24];//hong-2014/08/23

        unsigned int fpgastatus;//ghf-2014/8/25
        char encoder_index[EMCMOT_MAX_JOINTS];//ghf-20150624
        
        unsigned char electric_cabinet_alarm;//ghf-2014/5/6,HAL_IN
        unsigned char spindle0_overcurrent_alarm;//ghf-2014/5/6,HAL_IN
        unsigned char spindle1_overcurrent_alarm;//ghf-2014/5/6,HAL_IN
        unsigned char spindle0_converter_alarm;//ghf-2014/5/6,HAL_IN
        unsigned char spindle1_converter_alarm;//ghf-2014/5/6,HAL_IN
        unsigned char probe_alarm;//ghf-2014/5/6,HAL_IN
        unsigned char pressure_alarm;//ghf-2014/5/6,HAL_IN
        unsigned char coolant_alarm;//ghf-2014/5/6,HAL_IN
        unsigned char lube_alarm;//ghf-2014/5/6,HAL_IN
        unsigned int handwheel_Z2_active;
        
        unsigned int data_in_before;//ghf-2014/5/6,滤波前的32路输入信号

        unsigned char spindle_on[4];//ghf-2014/5/27
        unsigned char spindle_on1;//ghf-2014/5/27
        double indexpos[EMCMOT_MAX_JOINTS];//ghf-2014/6/24,回零时编码器清零前的值
        unsigned int randtime;//ghf-2014/7/8
        unsigned int randbuf;//ghf-2014/7/8


        int jog_state;//ghf-2014/6/6 手轮当前处于哪个轴
        unsigned int jogVal;// hong -20161108 手轮值
        float jogScale;   //手轮当前处于哪个档位
        float joguseScale;   //手轮当前处于哪个档位
        unsigned int jogCount;   //手轮获取的脉冲
        int handleuseType;  //值为0时则手轮Z档控制所有Z轴，值为1时则Z轴单独控制
        int handleDir;  //手轮方向


        //ghf-2014/7/24
        unsigned char customid[8];//客户编号
        unsigned char rlefttime[8];//剩余时间
        unsigned char encrychipid[8];//加密芯片id
        unsigned char encryptbuf[64];//密文密钥
        unsigned char rencryflag;       //motion解密成功标志位
        unsigned char fpgasn[8];//fpga唯一串号ghf-2014/8/14

        //ghf-2015/1/5
        unsigned int Enddate[8];//
                                //年月日时分秒后面4字节是没有用的，比较的时候加上12个小时即可
        unsigned int MachineLockdate[8];//
        unsigned int Currentdate[8];//上层可以实时获取rtc时间
        unsigned char tregenc[8];//时间注册编码ghf-2015/1/13
        unsigned char checkandsys[8];//校验号码和系统编号ghf-2015/1/13


        char dateerror; //-1:小于刷新日期;-2超过了截止日期;-3小于了写入日期
        unsigned char remaintimestatus;//当前剩余时间状态位 (0还没到期，1表示到期)
        unsigned char timetype;//当前采用哪一套计时系统标志 （0表示以小时 1表示以日期）

        unsigned char cpuid[8];//cpuid
        unsigned char hddid[8];//硬盘id
        unsigned char wlefttime[4];//重置剩余时间
        unsigned char wlefttimeflag;//为1,终止现有读写，写时间下去
        unsigned char widflag;

        char suspend;//ghf-2014/8/14,数据读完置1

        PmCartesian currentuVel;//ghf-2014/11/7
        int gmodetype;

        int pausefinish; //ghf-2014/8/4

        int externIonum;
        int externIosta;
        int zmode;      //hong-2014/09/11 0 means linkage 1 means rotation
        int spindlezmode;   //hong-2014/09/11 use in rotation mode
        int g0enableflag;   //hong-2014/11/06,m54p1=1 m54p0=0
        double coeff3; //m54g0系数
        int fric_type;//0:no comp  1:comp1  2:comp2

        struct rtc_device *rtc ;//ghf-2015/1/17
        int runcycleflag;//ghf-2015-2-26

        double g0_usefeedrate;//ghf-2015-4-8

        int forcflag;
        double freevel;

        double changingzvel; //add by hong to record the changing z vel
        int selectz;        //hong-2015-10-23 use in handle-wheel to distinguish now selected which z
        double maxdoublezpos;
        int changezend;//ghf-20150603
        int changingz;   //hong-2015-10-14
        int ManulInAutoflag;    //the flag means then I start the ManuInAutoflag


        double firvel[EMCMOT_MAX_JOINTS];//ghf-20150618  // the velocity of the moment tha trap the hardlimit signal

        /* MULTI_FUNC variant */
        int multifunctype;
        int multifuncintval;
        double multifuncdoubleval;
        int sponflag[4]; //manual enable the spindle

        double dynamicparameter[DYNAMIC_PAR_NUM];

        int useAbsEncoder;  //使用绝对值编码器
        int getAbsposflag;      //开机正在获取反馈坐标值标志
        int ethready;          //Ethercat链接状态
        int linktoEth;          //Ethercat链接状态
        double axesdiffval;     //两个同动轴的差值

        int axesType;    //记录轴类型标志位 轴配置选型  0:六轴  1:四轴 2:AB五轴 3:AC五轴
        int useWitchY;  //记录当前使用哪个Y轴
        unsigned char tail;	/* flag count for mutex detect */

} emcmot_status_t;



/*********************************
        CONFIG STRUCTURE
*********************************/

/* This is the config structure.  This is currently in shared memory,
   but I have no idea why... there are commands to set most of the
   items in this structure.  It seems we should either put the struct
   in private memory and manipulate it with commands, or we should
   put it in shared memory and manipulate it directly - not both.
   The structure contains static or rarely changed information that
   describes the machine configuration.

   later: I think I get it now - the struct is in shared memory so
   user space can read the config at any time, but commands are used
   to change the config so they only take effect when the realtime
   code processes the command.
*/

/*! \todo FIXME - this struct is broken into two parts... at the top are
   structure members that I understand, and that are needed for emc2.
   Other structure members follow.  All the later ones need to be
   evaluated - either they move up, or they go away.
*/
    typedef struct emcmot_config_t {
	unsigned char head;	/* flag count for mutex detect */

/*! \todo FIXME - all structure members beyond this point are in limbo */

	int config_num;		/* Incremented everytime configuration
				   changed, should match status.config_num */
	int numJoints;		/* The number of joints in the system (which
				   must be between 1 and EMCMOT_MAX_JOINTS,
				   inclusive). Allegedly, holds a copy of the
				   global num_joints - seems daft to maintain
				   duplicates ! */

	double trajCycleTime;	/* the rate at which the trajectory loop
				   runs.... (maybe) */
	double servoCycleTime;	/* the rate of the servo loop - Not the same
				   as the traj time */

	int interpolationRate;	/* grep control.c for an explanation....
				   approx line 50 */

	double limitVel;	/* scalar upper limit on vel */
	KINEMATICS_TYPE kinematics_type;
	int debug;		/* copy of DEBUG, from .ini file */
	unsigned char tail;	/* flag count for mutex detect */
    } emcmot_config_t;

/*********************************
      INTERNAL STRUCTURE
*********************************/

/* This is the internal structure.  It contains stuff that is used
   internally by the motion controller that does not need to be in
   shared memory.  It will wind up with a lot of the stuff that got
   tossed into the debug structure.

   FIXME - so far most if the stuff that was tossed in here got
   moved back out, maybe don't need it after all?
*/

    typedef struct emcmot_internal_t {
	unsigned char head;	/* flag count for mutex detect */

	int probe_debounce_cntr;
	unsigned char tail;	/* flag count for mutex detect */
    } emcmot_internal_t;

/* error structure - A ring buffer used to pass formatted printf stings to usr space */
    typedef struct emcmot_error_t {
	unsigned char head;	/* flag count for mutex detect */
	char error[EMCMOT_ERROR_NUM][EMCMOT_ERROR_LEN];
	int start;		/* index of oldest error */
	int end;		/* index of newest error */
	int num;		/* number of items */
	unsigned char tail;	/* flag count for mutex detect */
    } emcmot_error_t;

/*
  function prototypes for emcmot code
*/

/* error ring buffer access functions */
    extern int emcmotErrorInit(emcmot_error_t * errlog);
    extern int emcmotErrorPut(emcmot_error_t * errlog, const char *error);
    extern int emcmotErrorPutfv(emcmot_error_t * errlog, const char *fmt, va_list ap);
    extern int emcmotErrorPutf(emcmot_error_t * errlog, const char *fmt, ...);
    extern int emcmotErrorGet(emcmot_error_t * errlog, char *error);

#ifdef __cplusplus
}
#endif
#endif	/* MOTION_H */
