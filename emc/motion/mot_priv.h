/********************************************************************
* Description: mot_priv.h
*   Macros and declarations local to the realtime sources.
*
* Author: 
* License: GPL Version 2
* System: Linux
*    
* Copyright (c) 2004 All rights reserved.
*
* Last change:
*
********************************************************************/
#ifndef MOT_PRIV_H
#define MOT_PRIV_H

/***********************************************************************
*                       TYPEDEFS, ENUMS, ETC.                          *
************************************************************************/

/* First we define structures for data shared with the HAL */

/* HAL visible data notations:
   RPA:  read only parameter
   WPA:  write only parameter
   WRPA: read/write parameter
   RPI:  read only pin
   WPI:  write only pin
   WRPI: read/write pin
*/

/* joint data */

typedef struct {
    hal_float_t *coarse_pos_cmd;/* RPI: commanded position, w/o comp */
    hal_float_t *joint_vel_cmd;	/* RPI: commanded velocity, w/o comp */
    hal_float_t *backlash_corr;	/* RPI: correction for backlash */
    hal_float_t *backlash_filt;	/* RPI: filtered backlash correction */
    hal_float_t *backlash_vel;	/* RPI: backlash speed variable */
    hal_float_t *motor_offset;	/* RPI: motor offset, for checking homing stability */
    hal_float_t *motor_pos_cmd;	/* WPI: commanded position, with comp */
    hal_float_t *motor_pos_fb;	/* RPI: position feedback, with comp */
    hal_float_t *joint_pos_cmd;	/* WPI: commanded position w/o comp, mot ofs */
    hal_float_t *joint_pos_fb;	/* RPI: position feedback, w/o comp */
    hal_float_t *f_error;	/* RPI: following error */
    hal_float_t *f_error_lim;	/* RPI: following error limit */
    hal_float_t *joint_limit_vel;

    hal_float_t *free_pos_cmd;	/* RPI: free traj planner pos cmd */
    hal_float_t *free_vel_lim;	/* RPI: free traj planner vel limit */
    hal_bit_t *free_tp_enable;	/* RPI: free traj planner is running */
    hal_bit_t *kb_jog_active;   /* RPI: executing keyboard jog */
    hal_bit_t *wheel_jog_active;/* RPI: executing handwheel jog */

    hal_bit_t *active;		/* RPI: joint is active, whatever that means */
    hal_bit_t *in_position;	/* RPI: joint is in position */
    hal_bit_t *error;		/* RPI: joint has an error */
    hal_bit_t *phl;		/* RPI: joint is at positive hard limit */
    hal_bit_t *nhl;		/* RPI: joint is at negative hard limit */
    hal_bit_t *homing;		/* RPI: joint is homing */
    hal_bit_t *homed;		/* RPI: joint was homed */
    hal_bit_t *f_errored;	/* RPI: joint had too much following error */
    hal_bit_t *faulted;		/* RPI: joint amp faulted */
    hal_bit_t *pos_lim_sw;	/* RPI: positive limit switch input */
    hal_bit_t *neg_lim_sw;	/* RPI: negative limit switch input */
    hal_bit_t *home_sw;		/* RPI: home switch input */
    hal_bit_t *index_enable;	/* RPIO: motmod sets: request reset on index
				         encoder clears: index arrived */
    hal_bit_t *homeclearpos;
    hal_bit_t *amp_fault;	/* RPI: amp fault input */
    hal_bit_t *amp_enable;	/* WPI: amp enable output */
    hal_s32_t home_state;	/* RPA: homing state machine state */

    hal_bit_t *unlock;          /* WPI: command that axis should unlock for rotation */
    hal_bit_t *is_unlocked;     /* RPI: axis is currently unlocked */

    hal_s32_t *jog_counts;	/* WPI: jogwheel position input */
    hal_bit_t *jog_enable;	/* RPI: enable jogwheel */
    hal_float_t *jog_scale;	/* RPI: distance to jog on each count */
    hal_bit_t *jog_vel_mode;	/* RPI: true for "velocity mode" jogwheel */

    hal_float_t *fric_out_u;//ghf-2014/12/30
    hal_float_t *absZeropos;//hong-绝对值零点位置

    hal_s32_t *errorcode;
    

} joint_hal_t;

/* machine data */

typedef struct {
    hal_bit_t *probe_input;	/* RPI: probe switch input */
    hal_bit_t *enable;		/* RPI: motion inhibit input */
    hal_bit_t *spindle_index_enable;
    hal_bit_t *spindle_is_atspeed;
    hal_float_t *spindle_revs;
    hal_float_t *adaptive_feed;	/* RPI: adaptive feedrate, 0.0 to 1.0 */
    hal_bit_t *feed_hold;	/* RPI: set TRUE to stop motion */
    hal_bit_t *motion_enabled;	/* RPI: motion enable for all joints */
    hal_bit_t *in_position;	/* RPI: all joints are in position */
//    hal_bit_t *inpos_output;	/* WPI: all joints are in position (used to power down steppers for example) */
    hal_bit_t *coord_mode;	/* RPA: TRUE if coord, FALSE if free */
    hal_bit_t *teleop_mode;	/* RPA: TRUE if teleop mode */
    hal_bit_t *coord_error;	/* RPA: TRUE if coord mode error */
    hal_bit_t *on_soft_limit;	/* RPA: TRUE if outside a limit */

    hal_s32_t *program_line;    /* RPA: program line causing current motion */
    hal_float_t *current_vel;   /* RPI: velocity magnitude in machine units */
    hal_float_t *requested_vel;   /* RPI: requested velocity magnitude in machine units */
    hal_float_t *distance_to_go;/* RPI: distance to go in current move*/

    hal_bit_t debug_bit_0;	/* RPA: generic param, for debugging */
    hal_bit_t debug_bit_1;	/* RPA: generic param, for debugging */
    hal_float_t debug_float_0;	/* RPA: generic param, for debugging */
    hal_float_t debug_float_1;	/* RPA: generic param, for debugging */
    hal_float_t debug_float_2;	/* RPA: generic param, for debugging */
    hal_float_t debug_float_3;	/* RPA: generic param, for debugging */
    hal_s32_t debug_s32_0;	/* RPA: generic param, for debugging */
    hal_s32_t debug_s32_1;	/* RPA: generic param, for debugging */
    
    hal_bit_t *synch_do[EMCMOT_MAX_DIO]; /* WPI array: output pins for motion synched IO */
    hal_bit_t *synch_di[EMCMOT_MAX_DIO]; /* RPI array: input pins for motion synched IO */
    hal_float_t *analog_input[EMCMOT_MAX_AIO]; /* RPI array: input pins for analog Inputs */
    hal_float_t *analog_output[EMCMOT_MAX_AIO]; /* RPI array: output pins for analog Inputs */


    // creating a lot of pins for spindle control to be very flexible
    // the user needs only a subset of these

    // simplest way of spindle control (output start/stop)
//    hal_bit_t *spindle_on[4];	/* spindle spin output */ // modify by hong , suspend it to four io
    //hal_bit_t *spindle_on1;	/* spindle spin output ghf-2014/5/23*/

//    hal_bit_t *z_enable[4];	/* spindle spin output */ // modify by hong , suspend it to four io
    //hal_bit_t *z2_enable;	/* spindle spin output ghf-2014/5/23*/

    // same thing for 2 directions
    hal_bit_t *spindle_forward;	/* spindle spin-forward output */
    hal_bit_t *spindle_reverse;	/* spindle spin-reverse output */

    // simple velocity control (as long as the output is active the spindle
    //                          should accelerate/decelerate
    hal_bit_t *spindle_incr_speed;	/* spindle spin-increase output */
    hal_bit_t *spindle_decr_speed;	/* spindle spin-decrease output */

    // simple output for brake
    hal_bit_t *spindle_brake;	/* spindle brake output */

    // output of a prescribed speed (to hook-up to a velocity controller)
    hal_float_t *spindle_speed_out;	/* spindle speed output */
    hal_float_t *spindle_speed_out_rps;	/* spindle speed output */
    hal_float_t *spindle_speed_cmd_rps;	/* spindle speed command without SO applied */
    hal_float_t *spindle_speed_in;	/* spindle speed measured */

    hal_float_t *spindle_fbpos;	/* spindle csp mode fbpos */
    hal_float_t *spindle_nowpos;	/* spindle csp mode pos */
    hal_float_t *spindle_nowvel;	/* spindle csp mode speed */
    hal_bit_t *spindle_enable;
    hal_bit_t *spindle_getenabled;
    hal_u32_t *move_type;   //轴的运动模式，可切换
    
    // FIXME - debug only, remove later
    hal_float_t traj_pos_out;	/* RPA: traj internals, for debugging */
    hal_float_t traj_vel_out;	/* RPA: traj internals, for debugging */
    hal_u32_t traj_active_tc;	/* RPA: traj internals, for debugging */
    hal_float_t tc_pos[4];	/* RPA: traj internals, for debugging */
    hal_float_t tc_vel[4];	/* RPA: traj internals, for debugging */
    hal_float_t tc_acc[4];	/* RPA: traj internals, for debugging */

    // realtime overrun detection
    hal_u32_t last_period;	/* param: last period in clocks */
    hal_float_t last_period_ns;	/* param: last period in nanoseconds */
    hal_u32_t overruns;		/* param: count of RT overruns */
    hal_s32_t mintime;

    hal_float_t *tooloffset_x;
    hal_float_t *tooloffset_y;
    hal_float_t *tooloffset_z;
    hal_float_t *tooloffset_a;
    hal_float_t *tooloffset_b;
    hal_float_t *tooloffset_c;
    hal_float_t *tooloffset_u;
    hal_float_t *tooloffset_v;
    hal_float_t *tooloffset_w;
    hal_s32_t   *set_which_z;//ghf-2014/2/24
    hal_s32_t   *feedrate;//ghf-2014/12/17
    hal_s32_t   *selected_z;    //hong 2015-10-23
    
    hal_bit_t *run_on;//ghf-2014/3/13,HAL_IN
    hal_bit_t *pause_on;//ghf-2014/3/13
    
    hal_bit_t *green_on;//ghf-2014/3/17,HAL_OUT
    hal_bit_t *yellow_on;//ghf-2014/3/17
    hal_bit_t *red_on;//ghf-2014/3/17

    hal_bit_t *runStatus; //hong-2014/11/03

    hal_u32_t *filtertimebuf[64];//ghf-2014/3/28
    hal_u32_t *fpgastatus;

    //ghf-2014/10/24,界面卡死时，查看这几个值进而分析原因
    hal_u32_t *numSegments;
    hal_u32_t *aborting;
    hal_u32_t *paused;
    hal_u32_t *done;


    //ghf-2014/7/24
    /*hal_bit_t *customid[2];//客户编号
    hal_bit_t *rlefttime[4];//剩余时间
    hal_bit_t *encrychipid[8];//加密芯片id
    hal_bit_t *encryptbuf[64];//密文密钥
    hal_bit_t *rencryflag;

    hal_bit_t *cpuid[8];//cpuid
    hal_bit_t *hddid[4];//硬盘id
    hal_bit_t *wlefttime[4];//重置剩余时间
    hal_bit_t *wlefttimeflag;//为1,终止现有读写，写时间下去
    hal_bit_t *widflag;//为1,写id下去*/
    
    hal_bit_t *electric_cabinet_alarm;//ghf-2014/5/6,HAL_IN
    hal_bit_t *spindle0_overcurrent_alarm;//ghf-2014/5/6,HAL_IN
    hal_bit_t *spindle1_overcurrent_alarm;//ghf-2014/5/6,HAL_IN
    hal_bit_t *spindle0_converter_alarm;//ghf-2014/5/6,HAL_IN
    hal_bit_t *spindle1_converter_alarm;//ghf-2014/5/6,HAL_IN
    hal_bit_t *probe_alarm;//ghf-2014/5/6,HAL_IN
    hal_bit_t *pressure_alarm;//ghf-2014/5/6,HAL_IN
    hal_bit_t *coolant_alarm;//ghf-2014/5/6,HAL_IN
    hal_bit_t *lube_alarm;//ghf-2014/5/6,HAL_IN
    hal_bit_t *handwheel_Z2_active;//hong-2016/3/1,HAL_IN
    
    //hal_u32_t *data_in_before;//ghf-2014/5/6,由ppmc中out过来，motion中应该定义为in
    
    hal_u32_t *jog_state; //ghf-2014/5/14
    hal_u32_t *jogVal;      //hong handlewheel val in ethercat
    hal_float_t *jogScale;      //hong handlewheel val in ethercat
    hal_bit_t *linktoEth;   //the flag is already link to the ethercat   
    hal_bit_t *EthReady;           //ethercat正常启动状态
    
    hal_bit_t *motion_extern_out[24];   //hong-2014/08/23
    hal_bit_t *motion_extern_in[48];   //hong-2014/08/23

    hal_bit_t *user_defin[EMCMOT_MAX_USR_INIO];//ghf-2014/5/24
    hal_bit_t *user_defout[EMCMOT_MAX_USR_OUTIO];//hong-2014/05/29
    hal_bit_t *user_defwarn[EMCMOT_MAX_USR_WARNIO];//hong-2014/05/29

    hal_bit_t *encoder_index[EMCMOT_MAX_JOINTS];//ghf-2015/6/24


    hal_float_t *feed_scale;

    joint_hal_t joint[EMCMOT_MAX_JOINTS];	/* data for each joint */

} emcmot_hal_data_t;

/***********************************************************************
*                   GLOBAL VARIABLE DECLARATIONS                       *
************************************************************************/

/* HAL component ID for motion module */
extern int mot_comp_id;

/* userdefined number of joints. default is EMCMOT_MAX_JOINTS(=8), 
   but can be altered at motmod insmod time */
extern int num_joints;

/* userdefined number of digital IO. default is 4. (EMCMOT_MAX_DIO=64), 
   but can be altered at motmod insmod time */
extern int num_dio;

/* userdefined number of analog IO. default is 4. (EMCMOT_MAX_AIO=16), 
   but can be altered at motmod insmod time */
extern int num_aio;

/* pointer to emcmot_hal_data_t struct in HAL shmem, with all HAL data */
extern emcmot_hal_data_t *emcmot_hal_data;

/* pointer to array of joint structs with all joint data */
/* the actual array may be in shared memory or in kernel space, as
   determined by the init code in motion.c
*/
extern emcmot_joint_t *joints;

/* flag used to indicate that this is the very first pass thru the
   code.  Various places in the code use this to set initial conditions
   and avoid startup glitches.
*/
extern int first_pass;

/* Variable defs */
extern int kinType;
extern int rehomeAll;
extern int DEBUG_MOTION;
extern int EMCMOT_NO_FORWARD_KINEMATICS;
extern KINEMATICS_FORWARD_FLAGS fflags;
extern KINEMATICS_INVERSE_FLAGS iflags;
/* these variables have the servo cycle time and 1/cycle time */
extern double servo_period;
extern double servo_freq;


/* Struct pointers */
extern struct emcmot_struct_t *emcmotStruct;
extern struct emcmot_command_t *emcmotCommand;
extern struct emcmot_status_t *emcmotStatus;
extern struct emcmot_config_t *emcmotConfig;
extern struct emcmot_debug_t *emcmotDebug;
extern struct emcmot_internal_t *emcmotInternal;
extern struct emcmot_error_t *emcmotError;

/***********************************************************************
*                    PUBLIC FUNCTION PROTOTYPES                        *
************************************************************************/

/* function definitions */
extern void emcmotCommandHandler(void *arg, long period);
extern void emcmotController(void *arg, long period);
extern void emcmotSetCycleTime(unsigned long nsec);

extern void sqpreprocsegController(void *arg, long period);//ghf-2014/6/4

/* these are related to synchronized I/O */
extern void func_14(int index, char value);
extern void func_15(int index, double value);

extern void func_16(int axis, int unlock);
extern int emcmotGetRotaryIsUnlocked(int axis);

/* homing is no longer in control.c, make functions public */
extern void func_22(void);
extern void func_23(void);

/* loops through the active joints and checks if any are not homed */
extern int func_11(void);
/* recalculates jog limits */
extern void func_12(emcmot_joint_t *joint);
/* handles 'homed' flags, see command.c for details */
extern void func_13(int joint_num);

//extern void emcmot_config_change(void);
extern void func_3(void);
extern void reportError(const char *fmt, ...) __attribute((format(printf,1,2))); /* Use the rtapi_print call */

 /* rtapi_get_time() returns a nanosecond value. In time, we should use a u64
    value for all calcs and only do the conversion to seconds when it is
    really needed. */
#define etime() (((double) rtapi_get_time()) / 1.0e9)

/* macros for reading, writing bit flags */

/* motion flags */

#define GET_MOTION_ERROR_FLAG() (emcmotStatus->motionFlag & EMCMOT_MOTION_ERROR_BIT ? 1 : 0)

#define SET_MOTION_ERROR_FLAG(fl) if (fl) emcmotStatus->motionFlag |= EMCMOT_MOTION_ERROR_BIT; else emcmotStatus->motionFlag &= ~EMCMOT_MOTION_ERROR_BIT;

#define GET_MOTION_COORD_FLAG() (emcmotStatus->motionFlag & EMCMOT_MOTION_COORD_BIT ? 1 : 0)

#define SET_MOTION_COORD_FLAG(fl) if (fl) emcmotStatus->motionFlag |= EMCMOT_MOTION_COORD_BIT; else emcmotStatus->motionFlag &= ~EMCMOT_MOTION_COORD_BIT;

#define GET_MOTION_TELEOP_FLAG() (emcmotStatus->motionFlag & EMCMOT_MOTION_TELEOP_BIT ? 1 : 0)

#define SET_MOTION_TELEOP_FLAG(fl) if (fl) emcmotStatus->motionFlag |= EMCMOT_MOTION_TELEOP_BIT; else emcmotStatus->motionFlag &= ~EMCMOT_MOTION_TELEOP_BIT;

#define GET_MOTION_INPOS_FLAG() (emcmotStatus->motionFlag & EMCMOT_MOTION_INPOS_BIT ? 1 : 0)

#define SET_MOTION_INPOS_FLAG(fl) if (fl) emcmotStatus->motionFlag |= EMCMOT_MOTION_INPOS_BIT; else emcmotStatus->motionFlag &= ~EMCMOT_MOTION_INPOS_BIT;

#define GET_MOTION_ENABLE_FLAG() (emcmotStatus->motionFlag & EMCMOT_MOTION_ENABLE_BIT ? 1 : 0)

#define SET_MOTION_ENABLE_FLAG(fl) if (fl) emcmotStatus->motionFlag |= EMCMOT_MOTION_ENABLE_BIT; else emcmotStatus->motionFlag &= ~EMCMOT_MOTION_ENABLE_BIT;

/* joint flags */

#define GET_JOINT_ENABLE_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_ENABLE_BIT ? 1 : 0)

#define SET_JOINT_ENABLE_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_ENABLE_BIT; else (joint)->flag &= ~EMCMOT_JOINT_ENABLE_BIT;

#define GET_JOINT_ACTIVE_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_ACTIVE_BIT ? 1 : 0)

#define SET_JOINT_ACTIVE_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_ACTIVE_BIT; else (joint)->flag &= ~EMCMOT_JOINT_ACTIVE_BIT;

#define GET_JOINT_INPOS_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_INPOS_BIT ? 1 : 0)

#define SET_JOINT_INPOS_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_INPOS_BIT; else (joint)->flag &= ~EMCMOT_JOINT_INPOS_BIT;

#define GET_JOINT_ERROR_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_ERROR_BIT ? 1 : 0)

#define SET_JOINT_ERROR_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_ERROR_BIT; else (joint)->flag &= ~EMCMOT_JOINT_ERROR_BIT;

#define GET_JOINT_PHL_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_MAX_HARD_LIMIT_BIT ? 1 : 0)

#define SET_JOINT_PHL_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_MAX_HARD_LIMIT_BIT; else (joint)->flag &= ~EMCMOT_JOINT_MAX_HARD_LIMIT_BIT;

#define GET_JOINT_NHL_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_MIN_HARD_LIMIT_BIT ? 1 : 0)

#define SET_JOINT_NHL_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_MIN_HARD_LIMIT_BIT; else (joint)->flag &= ~EMCMOT_JOINT_MIN_HARD_LIMIT_BIT;

#define GET_JOINT_HOME_SWITCH_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_HOME_SWITCH_BIT ? 1 : 0)

#define SET_JOINT_HOME_SWITCH_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_HOME_SWITCH_BIT; else (joint)->flag &= ~EMCMOT_JOINT_HOME_SWITCH_BIT;

#define GET_JOINT_HOMING_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_HOMING_BIT ? 1 : 0)

#define SET_JOINT_HOMING_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_HOMING_BIT; else (joint)->flag &= ~EMCMOT_JOINT_HOMING_BIT;

#define GET_JOINT_HOMED_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_HOMED_BIT ? 1 : 0)

#define SET_JOINT_HOMED_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_HOMED_BIT; else (joint)->flag &= ~EMCMOT_JOINT_HOMED_BIT;

#define GET_JOINT_AT_HOME_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_AT_HOME_BIT ? 1 : 0)

#define SET_JOINT_AT_HOME_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_AT_HOME_BIT; else (joint)->flag &= ~EMCMOT_JOINT_AT_HOME_BIT;

#define GET_JOINT_FERROR_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_FERROR_BIT ? 1 : 0)

#define SET_JOINT_FERROR_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_FERROR_BIT; else (joint)->flag &= ~EMCMOT_JOINT_FERROR_BIT;

#define GET_JOINT_FAULT_FLAG(joint) ((joint)->flag & EMCMOT_JOINT_FAULT_BIT ? 1 : 0)

#define SET_JOINT_FAULT_FLAG(joint,fl) if (fl) (joint)->flag |= EMCMOT_JOINT_FAULT_BIT; else (joint)->flag &= ~EMCMOT_JOINT_FAULT_BIT;

#if defined(LINUX_VERSION_CODE) && !defined(SIM)
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
#define HAVE_CPU_KHZ
#endif
#endif
#endif /* MOT_PRIV_H */
