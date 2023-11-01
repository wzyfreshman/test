/********************************************************************
 * Description: control.c
 *   emcmotController() is the main loop running at the servo cycle
 *   rate. All state logic and trajectory calcs are called from here.
 *
 *   Derived from a work by Fred Proctor & Will Shackleford
 *
 * Author:
 * License: GPL Version 2
 * Created on:
 * System: Linux
 *
 * Copyright (c) 2004 All rights reserved.
 ********************************************************************/

#include "posemath.h"
#include "rtapi.h"
#include "hal.h"
#include "emcmotglb.h"
#include "motion.h"
#include "mot_priv.h"
#include "rtapi_math.h"
#include "tp.h"
#include "tc.h"
#include "motion_debug.h"
#include "config.h"

#include <stdlib.h>

// Mark strings for translation, but defer translation to userspace
#define _(s) (s)

/***********************************************************************
 *                  LOCAL VARIABLE DECLARATIONS                         *
 ************************************************************************/

/*! \todo FIXME - this is a leftover global, it will eventually go away */
int rehomeAll;

/* the (nominal) period the last time the motion handler was invoked */
unsigned long last_period = 0;

/* these variables have the servo cycle time and 1/cycle time */
double servo_period;
double servo_freq;

/*! \todo FIXME - debugging - uncomment the following line to log changes in
 JOINT_FLAG and MOTION_FLAG */
// #define WATCH_FLAGS 1

/* debugging function - it watches a particular variable and
 prints a message when the value changes.  Right now there are
 calls to this scattered throughout this and other files.
 To disable them, comment out the following define:
 */
// #define ENABLE_CHECK_STUFF
#ifdef ENABLE_CHECK_STUFF
void check_stuff(const char *location)
{
    static char *target, old = 0xFF;
    /*! \todo Another #if 0 */
#if 0
    /* kludge to look at emcmotDebug->enabling and emcmotStatus->motionFlag
     at the same time - we simply use a high bit of the flags to
     hold "enabling" */
    short tmp;
    if ( emcmotDebug->enabling )
    tmp = 0x1000;
    else
    tmp = 0x0;
    tmp |= emcmotStatus->motionFlag;
    target = &tmp;
    /* end of kluge */
#endif

    target = (emcmot_hal_data->enable);
    if ( old != *target ) {
        rtapi_print ( "%d: watch value %02X (%s)\n", emcmotStatus->heartbeat, *target, location );
        old = *target;
    }
}
#else /* make it disappear */
void check_stuff(const char *location) {
    /* do nothing (I wonder if gcc is smart
     enough to optimize the calls away?) */
}
#endif /* ENABLE_CHECK_STUFF */

/***********************************************************************
 *                      LOCAL FUNCTION PROTOTYPES                       *
 ************************************************************************/

/* the following functions are called (in this order) by the main
 controller function.  They are an attempt to break the huge
 function (originally 1600 lines) into something a little easier
 to understand.
 */

/* 'process_inputs()' is responsible for reading hardware input
 signals (from the HAL) and doing basic processing on them.  In
 the case of position feedback, that means removing backlash or
 screw error comp and calculating the following error.  For
 switches, it means debouncing them and setting flags in the
 emcmotStatus structure.
 */
static void process_inputs(void);

/* 'do forward kins()' takes the position feedback in joint coords
 and applies the forward kinematics to it to generate feedback
 in Cartesean coordinates.  It has code to handle machines that
 don't have forward kins, and other special cases, such as when
 the joints have not been homed.
 */
static void do_forward_kins(void);

/* probe inputs need to be handled after forward kins are run, since
 cartesian feedback position is latched when the probe fires, and it
 should be based on the feedback read in on this servo cycle.
 */

static void process_probe_inputs(int axis);

/* 'check_for_faults()' is responsible for detecting fault conditions
 such as limit switches, amp faults, following error, etc.  It only
 checks active axes.  It is also responsible for generating an error
 message.  (Later, once I understand the cmd/status/error interface
 better, it will probably generate error codes that can be passed
 up the architecture toward the GUI - printing error messages
 directly seems a little messy)
 */
static void check_for_faults(void);

/* 'set_operating_mode()' handles transitions between the operating
 modes, which are free, coordinated, and teleop.  This stuff needs
 to be better documented.  It is basically a state machine, with
 a current state, a desired state, and rules determining when the
 state can change.  It should be rewritten as such, but for now
 it consists of code copied exactly from emc1.
 */
static void set_operating_mode(void);

/* 'handle_jogwheels()' reads jogwheels, decides if they should be
 enabled, and if so, changes the free mode planner's target position
 when the jogwheel(s) turn.
 */
static void handle_jogwheels(void);

static void handle_jogwheels_guide(void); //ghf-2014/3/10

/* 'do_homing_sequence()' looks at emcmotStatus->homingSequenceState
 to decide what, if anything, needs to be done related to multi-joint
 homing.

 no prototype here, implemented in homing.c, proto in mot_priv.h
 */

/* 'do_homing()' looks at the home_state field of each joint struct
 to decide what, if anything, needs to be done related to homing
 the joint.  Homing is implemented as a state machine, the exact
 sequence of states depends on the machine configuration.  It
 can be as simple as immediately setting the current position to
 zero, or a it can be a multi-step process (find switch, set
 approximate zero, back off switch, find index, set final zero,
 rapid to home position), or anywhere in between.

 no prototype here, implemented in homing.c, proto in mot_priv.h
 */

/* 'get_pos_cmds()' generates the position setpoints.  This includes
 calling the trajectory planner and interpolating its outputs.
 */

static void get_pos_cmds(long period);

static double meanfilter(int joint_num, double realval);

static double SimpleTraj(emcmot_joint_t *joint);        //simple traj

static double SpindleControl();

static double spindleMoveVel();
static double simpleSpTraj();

static void compute_screw_comp(void);

//heyuebang 2014-11-20
// compute_fric_comp函数执行如下，当检测到给定速度为正时，延时一段时间后输出指数衰减函数；为负时，反向输出指数衰减函数。
static void compute_fric_comp(void);
// compute_fric_comp1函数执行如下：当检测到给定速度为正时，输出两个指数衰减函数，衰减慢的正向输出，衰减快的负向输出；为负时，反向输出。
static void compute_fric_comp1(void);

/* 'output_to_hal()' writes the handles the final stages of the
 control function.  It applies screw comp and writes the
 final motor position to the HAL (which routes it to the PID
 loop).  It also drives other HAL outputs, and it writes a
 number of internal variables to HAL parameters so they can
 be observed with halscope and halmeter.
 */
static void output_to_hal(void);

/* 'update_status()' copies assorted status information to shared
 memory (the emcmotStatus structure) so that it is available to
 higher level code.
 */
static void func_19(void);

/***********************************************************************
 *                        PUBLIC FUNCTION CODE                          *
 ************************************************************************/

/*
 emcmotController() runs the trajectory and interpolation calculations
 each control cycle

 This function gets called at regular intervals - therefore it does NOT
 have a loop within it!

 Inactive axes are still calculated, but the PIDs are inhibited and
 the amp enable/disable are inhibited
 */
static int handletimes = 0;
void emcmotController(void *arg, long period) {
    // - overrun detection -
    // maintain some records of how long it's been between calls.  The
    // first time we see a delay that's much longer than the records show
    // is normal, report an error.  This might detect bogus realtime
    // performance caused by ACPI, onboard video, etc.  It can be reliably
    // triggered by maximizing glxgears on my nvidia system, which also
    // causes the rtai latency test to show overruns.

    // check below if you set this under 5
#define CYCLE_HISTORY 5

    static long int cycles[CYCLE_HISTORY];
    static long long int last = 0;

    static int index = 0, priming = 1;

    //long long int now = rtapi_get_clocks();
    long long int now = rtapi_get_time();
    long int this_run = (long int) (now - last);

    if(this_run > emcmot_hal_data->last_period && this_run < 5000000) {
        emcmot_hal_data->last_period = this_run;
    }


    if(this_run < emcmot_hal_data->mintime) {
        emcmot_hal_data->mintime = this_run;
    }
//#ifdef HAVE_CPU_KHZ
   // emcmot_hal_data->last_period_ns = this_run * 1e6 / cpu_khz;
//#endif

#ifndef RTAPI_SIM
    if (!priming) {
        // we have CYCLE_HISTORY samples, so check for this call being
        // anomolously late
        int i;

        for (i = 0; i < CYCLE_HISTORY; i++) {
            if (this_run > 1.3 * 2000000) {
                emcmot_hal_data->overruns++;
                // print message on first overrun only
                if (emcmot_hal_data->overruns == 1) {
                    int saved_level = rtapi_get_msg_level();
                    rtapi_set_msg_level (RTAPI_MSG_ALL);
                    //reportError(
                    //        _("Unexpected realtime delay: check dmesg for details."));
                    rtapi_print_msg(RTAPI_MSG_WARN,
                            _("\nIn recent history there were\n"
                                            "%ld, %ld, %ld, %ld, and %ld\n"
                                            "elapsed clocks between calls to the motion controller.\n"),
                            cycles[0], cycles[1], cycles[2], cycles[3],
                            cycles[4]);
                    rtapi_print_msg(RTAPI_MSG_WARN,
                            _("This time, there were %ld which is so anomalously\n"
                                            "large that it probably signifies a problem with your\n"
                                            "realtime configuration.  For the rest of this run of\n"
                                            "EMC, this message will be suppressed.\n\n"),
                            this_run);
                    rtapi_set_msg_level(saved_level);
                }

                break;
            }
        }
    }
#endif
    if (last) {
        cycles[index++] = this_run;
    }
    if (index == CYCLE_HISTORY) {
        // wrap around to the start of the array
        index = 0;
        // we now have CYCLE_HISTORY good samples, so start checking times
        priming = 0;
    }
    // we need this for next time
    last = now;

    // end of overrun detection

    /* calculate servo period as a double - period is in integer nsec */
    servo_period = period * 0.000000001;
    servo_period = 0.002;       //2ms

    if (period != last_period) {
        emcmotSetCycleTime(period);
        last_period = period;
    }

    /* calculate servo frequency for calcs like vel = Dpos / period */
    /* it's faster to do vel = Dpos * freq */
    servo_freq = 1.0 / servo_period;
    /* increment head count to indicate work in progress */
    emcmotStatus->head++;
    /* here begins the core of the controller */

    check_stuff("before process_inputs()");
    process_inputs();
    check_stuff("after process_inputs()");
    do_forward_kins();
    //rtapi_print_msg(RTAPI_MSG_ERR,"emcmotStatus->carte_pos_fb.tran.x=%d\n",(int)(emcmotStatus->carte_pos_fb.tran.x*1000));
    check_stuff("after do_forward_kins()");
    int i = 0;
    for(i = 2;i<EMCMOT_MAX_JOINTS;i++) {
            process_probe_inputs(i);
    }
    check_stuff("after process_probe_inputs()");
    check_for_faults();
    check_stuff("after check_for_faults()");
    set_operating_mode();
    check_stuff("after set_operating_mode()");

    if (emcmotStatus->jogwheels_guide == '1') {        //再次点击要清零3/11
        /*if (handletimes < 200) {
            handletimes++;
        } else {
            handle_jogwheels_guide();
            handletimes = 0;
        }*/
        handle_jogwheels_guide();
    } else {
        handle_jogwheels();
    }
    check_stuff("after handle_jogwheels()");

    do_homing_sequence();
    check_stuff("after do_homing_sequence()");
    do_homing();
    check_stuff("after do_homing()");
    //rtapi_print("222,joint3->coarse_pos=%d,poscmd=%d\n",(int)(joint->coarse_pos*1000),(int)(joint->pos_cmd*1000));
    get_pos_cmds(period);
    check_stuff("after get_pos_cmds()");
    compute_screw_comp();
    //check_stuff("after compute_screw_comp()");

    /***************************************************/
    // heyuebang 2014-11-20
    if(emcmotStatus->fric_type==1)
        compute_fric_comp();
    else if(emcmotStatus->fric_type==2)
        compute_fric_comp1();

    output_to_hal();
    //rtapi_print("111,joint3->coarse_pos=%d,poscmd=%d\n",(int)(joint->coarse_pos*1000),(int)(joint->pos_cmd*1000));
    check_stuff("after output_to_hal()");
    update_status();
    check_stuff("after update_status()");
    /* here ends the core of the controller */
    emcmotStatus->heartbeat++;
    /* set tail to head, to indicate work complete */
    emcmotStatus->tail = emcmotStatus->head;
    /* clear init flag */
    first_pass = 0;
    SpindleControl();

    /* end of controller function */
}

/***********************************************************************
 *                         LOCAL FUNCTION CODE                          *
 ************************************************************************/

/* The protoypes and documentation for these functions are located
 at the top of the file in the section called "local function
 prototypes"
 */

static void process_probe_inputs(int axis) {
    static int old_probeVal = 0;
    unsigned char probe_type = emcmotStatus->probe_type;

    // don't error
    char probe_suppress = probe_type & 1;

    // trigger when the probe clears, instead of the usual case of triggering when it trips
    char probe_whenclears = !!(probe_type & 2);

    /* read probe input */
    emcmotStatus->probeVal = !!*(emcmot_hal_data->probe_input);
    if (emcmotStatus->probing) {
        /* check if the probe has been tripped */
        //g38.2时，probe_whenclears=0,probeVal=1表示碰到,g38.4时，probe_whenclears=1,probeVal=0表示离开,也会执行以下的^
        if (emcmotStatus->probeVal ^ probe_whenclears) {
            /* remember the current position */
            if(!SIMULATE)
                emcmotStatus->probedPos = emcmotStatus->carte_pos_fb; //这个时posfb变换过来的5/30
            else
                emcmotStatus->probedPos = emcmotStatus->carte_pos_cmd; //这个时posfb变换过来的5/30
            /* stop! */
            emcmotStatus->probing = 0;
            emcmotStatus->probeTripped = 1;
            //tpAbort(&emcmotDebug->queue);
            sqAbort (&queue); //ghf-2014/5/26,stop probe move


            /* check if the probe hasn't tripped, but the move finished */
            //} else if (GET_MOTION_INPOS_FLAG() && tpQueueDepth(&emcmotDebug->queue) == 0) {
        } else if (GET_MOTION_INPOS_FLAG() && sqGetDepth(&queue) == 0) { //ghf，g38.2或g38.4执行完了,对刀信号没发生跳变，会进这里
        /* we are already stopped, but we need to remember the current
         position here, because it will still be queried */
            emcmotStatus->probedPos = emcmotStatus->carte_pos_fb;

            emcmotStatus->probing = 0;
            if (probe_suppress) {
                emcmotStatus->probeTripped = 0;
            } else if (probe_whenclears) {
                reportError(_("G38.4 move finished without breaking contact."));
                SET_MOTION_ERROR_FLAG(1);
            } else {
                reportError(_("G38.2 move finished without making contact."));
                SET_MOTION_ERROR_FLAG(1);
            }
        }
    } else {
        if(old_probeVal != *(emcmot_hal_data->probe_input)) {

            if(*(emcmot_hal_data->probe_input) > 0 && emcmotStatus->dynamicparameter[4] > 0) {

                if(GET_MOTION_ERROR_FLAG() == 0) {
                    reportError(_("不在对刀过程触发对刀信号"));
                    SET_MOTION_ERROR_FLAG(1);
                    sqAbort(&queue);
                }
            }
        }
    }
    old_probeVal = emcmotStatus->probeVal;
}

static double switchfb[9] = { 0 }, oldswitchfb[9] = { 0 }; //ghf-2014/6/23
static int homeiwt[9] = { 0 };
static int home_err_flag[9] = { 0 };

static int oldlink = -1;
static void process_inputs(void) {
    int joint_num;
    double abs_ferror, tmp, scale;
    joint_hal_t *joint_data;
    emcmot_joint_t *joint;
    unsigned char enables;
    int n;

    /* read spindle angle (for threading, etc) */
    emcmotStatus->spindleRevs = *emcmot_hal_data->spindle_revs; //emcmot_hal_data是整个hal层的运动数据，定义在mot_priv.h中，变量定义在motion.c中，是emcmot_hal_data_t型的
    emcmotStatus->spindleSpeedIn = *emcmot_hal_data->spindle_speed_in;
    emcmotStatus->spindle_is_atspeed = *emcmot_hal_data->spindle_is_atspeed;
    emcmotStatus->spindle.fbnowpos = *emcmot_hal_data->spindle_fbpos;
    //ghf2014/3/13
    emcmotStatus->run_on = *emcmot_hal_data->run_on;
    emcmotStatus->pause_on = *emcmot_hal_data->pause_on;

    //ghf2014/5/6
    emcmotStatus->electric_cabinet_alarm =
            *emcmot_hal_data->electric_cabinet_alarm;
    emcmotStatus->spindle0_overcurrent_alarm =
            *emcmot_hal_data->spindle0_overcurrent_alarm;
    emcmotStatus->spindle1_overcurrent_alarm =
            *emcmot_hal_data->spindle1_overcurrent_alarm;
    emcmotStatus->spindle0_converter_alarm =
            *emcmot_hal_data->spindle0_converter_alarm;
    emcmotStatus->spindle1_converter_alarm =
            *emcmot_hal_data->spindle1_converter_alarm;
    emcmotStatus->probe_alarm = *emcmot_hal_data->probe_alarm;
    emcmotStatus->pressure_alarm = *emcmot_hal_data->pressure_alarm;
    emcmotStatus->coolant_alarm = *emcmot_hal_data->coolant_alarm;
    emcmotStatus->lube_alarm = *emcmot_hal_data->lube_alarm;
    emcmotStatus->handwheel_Z2_active = *emcmot_hal_data->handwheel_Z2_active;
    //ghf2014/5/6
    //emcmotStatus->data_in_before = *emcmot_hal_data->data_in_before;

    for (n = 0; n < EMCMOT_MAX_USR_INIO; n++) {
        emcmotStatus->user_defin[n] = *(emcmot_hal_data->user_defin[n]);
    }
    for (n = 0; n < EMCMOT_MAX_USR_OUTIO; n++) {
        emcmotStatus->user_defout[n] = *(emcmot_hal_data->user_defout[n]);
    }
    for (n = 0; n < EMCMOT_MAX_USR_WARNIO; n++) {
        emcmotStatus->user_defwarn[n] = *(emcmot_hal_data->user_defwarn[n]);
    }

    for (n = 0; n < EMCMOT_MAX_JOINTS; n++) {
        emcmotStatus->encoder_index[n] = *(emcmot_hal_data->encoder_index[n]);
    }

    for (n = 0; n < 48; n++) {
        emcmotStatus->mot_extern_in[n] = *(emcmot_hal_data->motion_extern_in[n]);
    }

    //ghf-2014/8/25
    emcmotStatus->fpgastatus = *(emcmot_hal_data->fpgastatus);

    emcmotStatus->jogVal = *emcmot_hal_data->jogVal;    //
    emcmotStatus->jogCount = (emcmotStatus->jogVal & 0x0000ffff);   //获取的脉冲
    if(emcmotStatus->jogVal & 0x00020000) {   //获取手轮当前处于哪个档位
        emcmotStatus->jogScale = 10 * emcmotStatus->joguseScale;
    } else if(emcmotStatus->jogVal & 0x00080000) {
        emcmotStatus->jogScale = 100 * emcmotStatus->joguseScale;
    } else {
        emcmotStatus->jogScale = 1 * emcmotStatus->joguseScale;
    }
    if(emcmotStatus->jogVal & 0x00040000) {   //获取手轮当前处于哪个轴
        emcmotStatus->jog_state = 0;
    } else if(emcmotStatus->jogVal & 0x00010000) {
        emcmotStatus->jog_state = 1;
    } else if(emcmotStatus->jogVal & 0x00100000)  {
        emcmotStatus->jog_state = 2;
    } else if(emcmotStatus->jogVal & 0x00200000)  {
        emcmotStatus->jog_state = 4;
    } else if(emcmotStatus->jogVal & 0x00400000 ) {
        emcmotStatus->jog_state = 3;
    }  else if(emcmotStatus->jogVal & 0x00800000 ) {
        emcmotStatus->jog_state = 5;
    } else {
        emcmotStatus->jog_state = -1;
    }


    /* compute net feed and spindle scale factors */
    if (emcmotStatus->motion_state == EMCMOT_MOTION_COORD) {
        /* use the enables that were queued with the current move */
        enables = emcmotStatus->enables_queued;
    } else {
        /* use the enables that are in effect right now */
        enables = emcmotStatus->enables_new;
    }
    //rtapi_print("in control.c,enables = %x\n",enables);
    /* feed scaling first:  feed_scale, adaptive_feed, and feed_hold */
    scale = 1.0;
//    if (enables & FS_ENABLED) {
        scale *= emcmotStatus->feed_scale;    //跟踪进给因子如何给进来的？
        //rtapi_print("in control.c,scale = %d\n",(int)(scale*1000));
//    }
    if (enables & AF_ENABLED) {
        /* read and clamp (0.0 to 1.0) adaptive feed HAL pin */
        tmp = *emcmot_hal_data->adaptive_feed;
        if (tmp > 1.0) {
            tmp = 1.0;
        } else if (tmp < 0.0) {
            tmp = 0.0;
        }
        scale *= tmp;
        //rtapi_print("in control.c,scale1 = %d\n",(int)(scale*1000));
    }
    if (enables & FH_ENABLED) {
        /* read feed hold HAL pin */
        if (*emcmot_hal_data->feed_hold) {
            scale = 0;
        }
        //rtapi_print("in control.c,scale2 = %d\n",(int)(scale*1000));
    }
    /* save the resulting combined scale factor */
    emcmotStatus->net_feed_scale = scale;
    //rtapi_print("in control.c,emcmotStatus->net_feed_scale = %d\n",(int)(emcmotStatus->net_feed_scale*1000));

    /* now do spindle scaling: only one item to consider */
    scale = 1.0;
    if (enables & SS_ENABLED) {
        scale *= emcmotStatus->spindle_scale;
    }
    /* save the resulting combined scale factor */
    emcmotStatus->net_spindle_scale = scale;

    if(*(emcmot_hal_data->linktoEth) != oldlink) {
        printf("now link ----------------------------------:%d\n",*(emcmot_hal_data->linktoEth));
        if(*(emcmot_hal_data->linktoEth) == 0 && oldlink == 1) {
            if(GET_MOTION_ENABLE_FLAG()) {
                SET_MOTION_ENABLE_FLAG(0);
            }
        }
        oldlink = *(emcmot_hal_data->linktoEth);
    }

    static int firstwait = 0;
    if(*(emcmot_hal_data->linktoEth) > 0  || SIMULATE){

    firstwait = 1;
    /* read and process per-joint inputs */
    for (joint_num = 0; joint_num < num_joints; joint_num++) {	//开始处理hal上来的轴数据
        /* point to joint HAL data */
        joint_data = &(emcmot_hal_data->joint[joint_num]);	//joint_data是hal上来的
        /* point to joint data */
        joint = &joints[joint_num];	//emcmot_joint_t *joint;joints定义在motion.c中emcmot_joint_t *joints = 0，它指向emcmot_joint_t joint_array[EMCMOT_MAX_JOINTS];
        if (!GET_JOINT_ACTIVE_FLAG(joint)) {
            /* if joint is not active, skip it */
            continue;
        }
        /* copy data from HAL to joint structure */
        joint->index_enable = *(joint_data->index_enable);
        joint->homeclearpos = *(joint_data->homeclearpos);

        joint->errorCode = (*(joint_data->errorcode) & 0xffff);

        /* read home switch input */
        if (*(joint_data->home_sw)) {
            SET_JOINT_HOME_SWITCH_FLAG(joint, 1);
        } else {
            SET_JOINT_HOME_SWITCH_FLAG(joint, 0);
        }

        //modify by hong
        if(joint->useposfb)
            joint->motor_pos_fb = *(joint_data->motor_pos_fb);
        else
            joint->motor_pos_fb = *(joint_data->motor_pos_cmd);

        /* calculate pos_fb */
        if (joint->home_state == HOME_INDEX_SEARCH_WAIT || joint->homeclearpos > 0) {

            /* special case - we're homing the joint, and it just
             hit the index.  The encoder count might have made a
             step change.  The homing code will correct for it
             later, so we ignore motor_pos_fb and set pos_fb
             to match the commanded value instead.
             在回零的最后一步，编码器零点信号一到，ppmc中将index_enable置零，此条件就进来了，从现在开始算40ms，joint->motor_pos_fb才会清零，在此期间，cmd一直等于fb，以防止跟随误差ghf-2014/4/19
             */
//            printf("Now I clear the final pos\n");
            joint->pos_fb = joint->pos_cmd; //在此期间编码器的值会突然变为0，而cmd的值不是0，如果没这句，就会报跟随错误ghf-2014/5/15



        } else if (joint->home_state == HOME_SET_INDEX_POSITION) {//如果没有这一句，home中HOME_INDEX_SEARCH_WAIT状态下刚刚赋值为HOME_SET_INDEX_POSITION，还没有执行此状态下的动作，而此时反馈值已经清零，此时执行以下的else，就会报跟随错误ghf-2014/5/15，事实上，这里没进来，不知为何？,此状态中加入200ms的延时，就进来了，这个延时是等清零前的脉冲彻底走完，虽然fpga清零了，但脉冲真正走完要100多ms，所以就又走了0.008的距离，ghf-2014/5/16
            joint->pos_fb = joint->pos_cmd;
            //joint->pos_fb = joint->coarse_pos;

            //ghf-2015/1/20
            switchfb[joint_num] = 0;
            oldswitchfb[joint_num] = 0;
            homeiwt[joint_num] = 0;


        } else {
            /* normal case: subtract backlash comp and motor offset */
            if(joint->useAbsPos > 0) {
                joint->pos_fb = joint->motor_pos_fb	-  joint->motor_offset -  joint->abszeroPos;//ghf-2014/7/28
            } else
                joint->pos_fb = joint->motor_pos_fb	-  joint->motor_offset ;//ghf-2014/7/28

            if(emcmotStatus->linktoEth == 0 && !SIMULATE) {
                joint->pos_cmd = joint->pos_fb;
                joint->free_pos_cmd = joint->pos_fb;
             }

        }
        //ghf-2014/6/26
        if (home_err_flag[joint_num] == 1) {
                rtapi_print(
                        "motion disabled when HOME_INDEX_SEARCH_WAIT,joint->motor_pos_fb=%d\n",
                        (int) (joint->motor_pos_fb * 1000));
                joint->index_enable = 0; //ghf-2014/8/18
                home_err_flag[joint_num] = 0;
                joint->homeclearpos = 0;

        }

        //初次开机获取坐标
        if(emcmotStatus->getAbsposflag == 0) {

        joint->ferror = joint->pos_cmd - joint->pos_fb;
        abs_ferror = fabs(joint->ferror);
        /* update maximum ferror if needed */
        if (abs_ferror > joint->ferror_high_mark) {
            joint->ferror_high_mark = abs_ferror;
        }

        /* calculate following error limit */
        if (joint->vel_limit > 0.0) {
            joint->ferror_limit = joint->max_ferror * fabs(joint->vel_cmd)
                    / joint->vel_limit;
        } else {
            joint->ferror_limit = 0;
        }
        if (joint->ferror_limit < joint->min_ferror) {
            joint->ferror_limit = joint->min_ferror;
        }
        /* update following error flag */
        if (abs_ferror > joint->ferror_limit) {	  //以很小的速度在很短的时间内走很大距离时，就会报跟随错误。

            rtapi_print(
                    "in control,joint[%d]->pos_cmd=%f\n",joint_num,
                    joint->pos_cmd);
            rtapi_print(
                    "in control,joint->coarse_pos=%f\n",
                    joint->coarse_pos);

            rtapi_print(
                    "in control,joint->free_pos_cmd=%f\n",
                    joint->free_pos_cmd );

            rtapi_print(
                    "in control,joint->motor_offset=%f\n",
                    joint->motor_offset );

            rtapi_print( "in control,joint->pos_fb=%f\n",
                    joint->pos_fb );
            rtapi_print(
                    "in control,joint->motor_pos_fb=%f\n",
                    joint->motor_pos_fb );

            rtapi_print(
                    "abs_ferror=%.3f,joint->vel_limit=%.3f,joint->max_ferror=%.3f,fabs(joint->vel_cmd)=%.3f jointnum:%d\n",
                   abs_ferror, joint->vel_limit ,
                    joint->max_ferror,fabs(joint->vel_cmd),joint_num);

            if(joint->abszeroSt == 1) {
            } else {
                SET_JOINT_FERROR_FLAG(joint, 1);
            }
             joint->pos_cmd = joint->pos_fb;    // add by hong ,use for break the loop ,but I do not know whether it is right
        } else {
            SET_JOINT_FERROR_FLAG(joint, 0);
        }
    } else {
             if(joint->pos_cmd == joint->pos_fb) {
                emcmotStatus->getAbsposflag = 0;
            }
             joint->pos_cmd = joint->pos_fb;
    }


        /* read limit switches */
        if (*(joint_data->pos_lim_sw) && (!(joint->home_flags & HOME_IGNORE_LIMITS) ||
                ((joint->home_flags & HOME_IGNORE_LIMITS) && !GET_JOINT_HOMING_FLAG(joint)))) {
                    SET_JOINT_PHL_FLAG(joint, 1);
                    if(joint->oldphlflag==0){
                        emcmotStatus->firvel[joint_num]=joint->vel_cmd;
                        rtapi_print("emcmotStatus->firvel=%d\n",(int)(emcmotStatus->firvel[joint_num] * 1000));
                    }
                } else {
                    SET_JOINT_PHL_FLAG(joint, 0);
                    //emcmotStatus->firvel[joint_num] = 0;
                }

                joint->oldphlflag = GET_JOINT_PHL_FLAG(joint);

                if (*(joint_data->neg_lim_sw) && (!(joint->home_flags & HOME_IGNORE_LIMITS) ||
                                                  ((joint->home_flags & HOME_IGNORE_LIMITS) && !GET_JOINT_HOMING_FLAG(joint)))) {
                    SET_JOINT_NHL_FLAG(joint, 1);
                    if(joint->oldnhlflag==0){
                        emcmotStatus->firvel[joint_num]=joint->vel_cmd;
                        rtapi_print("emcmotStatus->firvel=%d\n",(int)(emcmotStatus->firvel[joint_num] * 1000));
                    }
                } else {
                    SET_JOINT_NHL_FLAG(joint, 0);
                    //emcmotStatus->firvel[joint_num] = 0;
                }

                joint->oldnhlflag = GET_JOINT_NHL_FLAG(joint);


        joint->on_pos_limit = GET_JOINT_PHL_FLAG(joint);
        joint->on_neg_limit = GET_JOINT_NHL_FLAG(joint);
        /* read amp fault input */
        if (*(joint_data->amp_fault)) {
            SET_JOINT_FAULT_FLAG(joint, 1);
        } else {
            SET_JOINT_FAULT_FLAG(joint, 0);
        }


        /* end of read and process joint inputs loop */
    }

    //判断Y轴是否出现不同步的情况
    if(emcmotDebug->enabling > 0) {
        if(fabs(joints[1].motor_pos_fb - joints[7].motor_pos_fb - emcmotStatus->axesdiffval) > 15 && joints[1].useposfb > 0 && joints[7].useposfb > 0) {
            if (!GET_JOINT_ERROR_FLAG(&joints[1])) {
                reportError("Y轴同步出错！差值:%f  Y1:%f  Y2:%f\n",emcmotStatus->axesdiffval,joints[1].motor_pos_fb , joints[7].motor_pos_fb);
            }
            SET_JOINT_ERROR_FLAG(&joints[1], 1);
            emcmotDebug->enabling = 0;
        }
    }
    } else {
        /* read amp fault input */
        for (joint_num = 0; joint_num < num_joints; joint_num++) {	//开始处理hal上来的轴数据
            /* point to joint HAL data */
            joint_data = &(emcmot_hal_data->joint[joint_num]);	//joint_data是hal上来的
            /* point to joint data */
            joint = &joints[joint_num];	//emcmot_joint_t *joint;joints定义在motion.c中emcmot_joint_t *joints = 0，它指向emcmot_joint_t joint_array[EMCMOT_MAX_JOINTS];
            if (!GET_JOINT_ACTIVE_FLAG(joint)) {
                /* if joint is not active, skip it */
                continue;
            }

            if (*(joint_data->amp_fault) && firstwait > 0 ) {
                SET_JOINT_FAULT_FLAG(joint, 1);
            } else {
                SET_JOINT_FAULT_FLAG(joint, 0);
            }
        }
    }

    emcmotStatus->linktoEth = *(emcmot_hal_data->linktoEth);
    emcmotStatus->ethready = *(emcmot_hal_data->EthReady);

}

static void do_forward_kins(void) {
    /* there are four possibilities for kinType:

     IDENTITY: Both forward and inverse kins are available, and they
     can used without an initial guess, even if one or more joints
     are not homed.  In this case, we apply the forward kins to the
     joint->pos_fb to produce carte_pos_fb, and if all axes are homed
     we set carte_pos_fb_ok to 1 to indicate that the feedback data
     is good.

     BOTH: Both forward and inverse kins are available, but the forward
     kins need an initial guess, and/or the kins require all joints to
     be homed before they work properly.  Here we must tread carefully.
     IF all the joints have been homed, we apply the forward kins to
     the joint->pos_fb to produce carte_pos_fb, and set carte_pos_fb_ok
     to indicate that the feedback is good.  We use the previous value
     of carte_pos_fb as the initial guess.  If all joints have not been
     homed, we don't call the kinematics, instead we set carte_pos_fb to
     the cartesean coordinates of home, as stored in the global worldHome,
     and we set carte_fb_ok to 0 to indicate that the feedback is invalid.
     \todo  FIXME - maybe setting to home isn't the right thing to do.  We need
     it to be set to home eventually, (right before the first attemt to
     run the kins), but that doesn't mean we should say we're at home
     when we're not.

     INVERSE_ONLY: Only inverse kinematics are available, forward
     kinematics cannot be used.  So we have to fake it, the question is
     simply "what way of faking it is best".  In free mode, or if all
     axes have not been homed, the feedback position is unknown.  If
     we are in teleop or coord mode, or if we are in free mode and all
     axes are homed, and haven't been moved since they were homed, then
     we set carte_pos_fb to carte_pos_cmd, and set carte_pos_fb_ok to 1.
     If we are in free mode, and any joint is not homed, or any joint has
     moved since it was homed, we leave cart_pos_fb alone, and set
     carte_pos_fb_ok to 0.

     FORWARD_ONLY: Only forward kinematics are available, inverse kins
     cannot be used.  This exists for completeness only, since EMC won't
     work without inverse kinematics.

     */

    /*! \todo FIXME FIXME FIXME - need to put a rate divider in here, run it
     at the traj rate */

    double joint_pos[EMCMOT_MAX_JOINTS] = { 0, };
    int joint_num, result;
    emcmot_joint_t *joint;
    int ret = -1;

    /* copy joint position feedback to local array */
    for (joint_num = 0; joint_num < num_joints; joint_num++) {
        /* point to joint struct */
        joint = &joints[joint_num];
        /* copy feedback */
        joint_pos[joint_num] = joint->pos_fb;

        /* check for homed, only if the current joint is active */
        if (!GET_JOINT_ACTIVE_FLAG(joint)) {
            /* if joint is not active, don't even look at its limits */
            continue;
        }
    }
    switch (kinType) {

    case KINEMATICS_IDENTITY:{
        kinematicsForward(joint_pos, &emcmotStatus->carte_pos_fb, &fflags,&iflags);

        ret = checkAllHomed();
        if (ret) {
            emcmotStatus->carte_pos_fb_ok = 1;
        } else {
            emcmotStatus->carte_pos_fb_ok = 0;
        }
    }
        break;

    default:
        emcmotStatus->carte_pos_fb_ok = 0;
        break;
    }
}

static void check_for_faults(void) {
    int joint_num;
    emcmot_joint_t *joint;
    int neg_limit_override, pos_limit_override;
    static unsigned char oldenabling=0;

    /* check for various global fault conditions */
    /* only check enable input if running */

    if(*(emcmot_hal_data->linktoEth) > 0) {

    if (GET_MOTION_ENABLE_FLAG() != 0) {
        if (*(emcmot_hal_data->enable) == 0) {
            reportError(_("motion stopped by enable input"));
            emcmotDebug->enabling = 0;
        }
    }
    }
    /* check for various joint fault conditions */
    for (joint_num = 0; joint_num < num_joints; joint_num++) {
        /* point to joint data */
        joint = &joints[joint_num];
        /* only check active, enabled axes */
        if (GET_JOINT_ACTIVE_FLAG(joint) && GET_JOINT_ENABLE_FLAG(joint)) {
            /* are any limits for this joint overridden? */
            neg_limit_override = emcmotStatus->overrideLimitMask
                    & (1 << (joint_num * 2));
            pos_limit_override = emcmotStatus->overrideLimitMask
                    & (2 << (joint_num * 2));
            /* check for hard limits */
            if ((GET_JOINT_PHL_FLAG(joint) && !pos_limit_override)
                    || (GET_JOINT_NHL_FLAG(joint) && !neg_limit_override)) {
                /* joint is on limit switch, should we trip? */
                if (GET_JOINT_HOMING_FLAG(joint)) {
                    /* no, ignore limits */
                } else {
                    /* trip on limits */
                    if (!GET_JOINT_ERROR_FLAG(joint)) {
                        /* report the error just this once */
                        //reportError(_("(joint %d) on limit switch error"),
                        //        joint_num);
                    }
                    SET_JOINT_ERROR_FLAG(joint, 1);
                    emcmotDebug->enabling = 0; //ghf-20150713
                }
            }
            /* check for amp fault */
            if (GET_JOINT_FAULT_FLAG(joint)) { //伺服报警时进这里
                /* joint is faulted, trip */
                if (!GET_JOINT_ERROR_FLAG(joint)) {
                    /* report the error just this once */
                    reportError(_("(joint %d) amplifier fault"), joint_num);
                    if(joint->useAbsPos == 0)
                        SET_JOINT_HOMED_FLAG(joint, 0);
                }

                SET_JOINT_ERROR_FLAG(joint, 1);
                emcmotDebug->enabling = 0;
            }
            /* check for excessive following error */
            if (GET_JOINT_FERROR_FLAG(joint)) {
                if (!GET_JOINT_ERROR_FLAG(joint)) {
                    /* report the error just this once */
                    reportError(_("(joint %d) following error"),
                            joint_num);
                }

                SET_JOINT_ERROR_FLAG(joint, 1);
                emcmotDebug->enabling = 0;
            }
            /* end of if JOINT_ACTIVE_FLAG(joint) */
        }
        if((oldenabling!=0) && (emcmotDebug->enabling == 0)) {
            //ghf-2014/7/21
            SET_JOINT_INPOS_FLAG(joint, 1);
            SET_MOTION_INPOS_FLAG(1);

        }
        /* end of check for joint faults loop */
    }
    oldenabling = emcmotDebug->enabling;
}

static void set_operating_mode(void) {
    int joint_num;
    emcmot_joint_t *joint;

    /* check for disabling */
    if (!emcmotDebug->enabling && GET_MOTION_ENABLE_FLAG()) {//ghf-2014/4/4,伺服打开时,enabling=1;当前从伺服on切到伺服off，就会执行这里
            /* clear out the motion emcmotDebug->queue and interpolators */
        //tpClear(&emcmotDebug->queue);
        sqClearQueue (&queue);	    //ghf-2014/3/4
        for (joint_num = 0; joint_num < num_joints; joint_num++) {
            /* point to joint data */
            joint = &joints[joint_num];
            /* disable free mode planner */
            joint->free_tp_enable = 0;
            /* drain coord mode interpolators */
            cubicDrain(&(joint->cubic));
            if (GET_JOINT_ACTIVE_FLAG(joint)) {
                SET_JOINT_ENABLE_FLAG(joint, 0);
                SET_JOINT_HOMING_FLAG(joint, 0);
                //ghf-2014/6/26
                //rtapi_print_msg(RTAPI_MSG_ERR,"1111joint->home_state=%d,joint->motor_pos_fb=%d\n",joint->home_state,(int)(joint->motor_pos_fb*1000));
                if (joint->home_state == HOME_INDEX_SEARCH_WAIT) {
                    home_err_flag[joint_num] = 1;
                }
                joint->home_state = HOME_IDLE;
            }
            /* don't clear the joint error flag, since that may signify why
             we just went into disabled state */
        }
        /* reset the trajectory interpolation counter, so that the kinematics
         functions called during the disabled state run at the nominal
         trajectory rate rather than the servo rate. It's loaded with
         emcmotConfig->interpolationRate when it goes to zero. */
        /*! \todo FIXME - interpolation is still under construction */
        /*! \todo Another #if 0 */
#if 0
        interpolationCounter = 0;
#endif
        SET_MOTION_ENABLE_FLAG(0);
        /* don't clear the motion error flag, since that may signify why we
         just went into disabled state */
    }

    /* check for emcmotDebug->enabling */
    if (emcmotDebug->enabling && !GET_MOTION_ENABLE_FLAG()) { //当前伺服从off切到on，执行这里
        if(emcmotStatus->useWitchY == 0)
            emcmotStatus->carte_pos_cmd.tran.y = joints[1].pos_cmd;
        else
            emcmotStatus->carte_pos_cmd.tran.y = joints[6].pos_cmd;

        sqSetPos(&queue, emcmotStatus->carte_pos_cmd);//ghf-2014/3/4，将当前的位置写入到queue的lastpoint中
        for (joint_num = 0; joint_num < num_joints; joint_num++) {
            /* point to joint data */
            joint = &joints[joint_num];

            if(emcmotStatus->linktoEth > 0 || SIMULATE == 1){
            joint->pos_cmd = joint->pos_fb;
            joint->free_pos_cmd = joint->pos_cmd;
            }


            if (GET_JOINT_ACTIVE_FLAG(joint)) {
                SET_JOINT_ENABLE_FLAG(joint, 1);
                SET_JOINT_HOMING_FLAG(joint, 0);
                joint->home_state = HOME_IDLE;
            }
            /* clear any outstanding joint errors when going into enabled
             state */
            SET_JOINT_ERROR_FLAG(joint, 0);
        }
        if(emcmotStatus->linktoEth > 0 || SIMULATE == 1) {

            SET_MOTION_ENABLE_FLAG(1);
            /* clear any outstanding motion errors when going into enabled state */
            SET_MOTION_ERROR_FLAG(0);
        } else  {
            emcmotDebug->enabling = 0;
        }
    }

    /* check for entering teleop mode */
    if (emcmotDebug->teleoperating && !GET_MOTION_TELEOP_FLAG()) {
        if (GET_MOTION_INPOS_FLAG()) {
            if(emcmotStatus->useWitchY == 0)
                emcmotStatus->carte_pos_cmd.tran.y = joints[1].pos_cmd;
            else
                emcmotStatus->carte_pos_cmd.tran.y = joints[6].pos_cmd;

            /* update coordinated emcmotDebug->queue position */
            //tpSetPos(&emcmotDebug->queue, emcmotStatus->carte_pos_cmd);
            sqSetPos(&queue, emcmotStatus->carte_pos_cmd);	//ghf-2014/3/4
            /* drain the cubics so they'll synch up */
            for (joint_num = 0; joint_num < num_joints; joint_num++) {
                /* point to joint data */
                joint = &joints[joint_num];
                cubicDrain(&(joint->cubic));
            }
            /* Initialize things to do when starting teleop mode. */
            ZERO_EMC_POSE(emcmotDebug->teleop_data.currentVel);
            ZERO_EMC_POSE(emcmotDebug->teleop_data.desiredVel);
            ZERO_EMC_POSE(emcmotDebug->teleop_data.currentAccell);
            ZERO_EMC_POSE(emcmotDebug->teleop_data.desiredAccell);
            SET_MOTION_TELEOP_FLAG(1);
            SET_MOTION_ERROR_FLAG(0);
        } else {
            /* not in position-- don't honor mode change */
            emcmotDebug->teleoperating = 0;
        }
    } else {	//当前为非远程模式，包括mdi，自动，手动
        if (GET_MOTION_INPOS_FLAG()) {	//当前已经到位
            if (!emcmotDebug->teleoperating && GET_MOTION_TELEOP_FLAG()) {//当前是从远程模式切换过来的
                SET_MOTION_TELEOP_FLAG(0);
                if (!emcmotDebug->coordinating) {	//当前是远程切换到了手动模式
                    for (joint_num = 0; joint_num < num_joints; joint_num++) {
                        /* point to joint data */
                        joint = &joints[joint_num];
                        /* update free planner positions */
                        joint->free_pos_cmd = joint->pos_cmd;
                        rtapi_print("---2\n");
                    }
                }
            }

        }

        /* check for entering coordinated mode */
        if (emcmotDebug->coordinating && !GET_MOTION_COORD_FLAG()) {//当前模式要切到自动模式，一上电，界面要打开程序，切到自动模式，所以这里会执行一次

            if (GET_MOTION_INPOS_FLAG()) {	//如果到位

#if 0
                if(func_11() > 0) {
                    emcmotStatus->ManulInAutoflag = 1;
                    emcmotStatus->forcflag = 1;    //add by hong
                    emcmotStatus->freevel = emcmotStatus->changingzvel;
                    int i=0;
                    for(i=0;i<EMCMOT_MAX_JOINTS;i++) {
                        joints[i].axis_offset = joints[i].switch_offset;
                        joints[i].freepos = joints[i].switch_offset;
                    }
                }
#endif
                if(emcmotStatus->useWitchY == 0)
                    emcmotStatus->carte_pos_cmd.tran.y = joints[1].pos_cmd;
                else
                    emcmotStatus->carte_pos_cmd.tran.y = joints[6].pos_cmd;

 //               kinedata.start_rtcp = 0;
                sqSetPos(&queue, emcmotStatus->carte_pos_cmd);	//ghf-2014/3/4
                /* drain the cubics so they'll synch up */
                for (joint_num = 0; joint_num < num_joints; joint_num++) {
                    /* point to joint data */
                    joint = &joints[joint_num];
                    cubicDrain(&(joint->cubic));
                }
                /* clear the override limits flags */
                emcmotDebug->overriding = 0;
                emcmotStatus->overrideLimitMask = 0;
                SET_MOTION_COORD_FLAG(1);
                SET_MOTION_TELEOP_FLAG(0);
                SET_MOTION_ERROR_FLAG(0);
            } else {
                /* not in position-- don't honor mode change */
                emcmotDebug->coordinating = 0;
            }
        }

        static int waitabortmode = 0;
        /* check entering free space mode */
        if (!emcmotDebug->coordinating && GET_MOTION_COORD_FLAG()) {//自动切到手动时执行


            if (GET_MOTION_INPOS_FLAG()) {
                sqClearQueue(&queue);

                for (joint_num = 0; joint_num < num_joints; joint_num++) {
                    joint = &joints[joint_num];
                    joint->free_pos_cmd = joint->pos_cmd;
                    joint->free_tp_enable = 0;

                    //自动切换手动，对旋转轴最短路径变换的坐标做处理
                    //  最短路径模式下需要处理
                    if(joint->circle_shortest == 1) {
                        kinedata.lastP = emcmotStatus->carte_pos_cmd;
                        if(joint_num == 3) {
                            kinedata.circle_count[joint_num] = (int)emcmotStatus->carte_pos_cmd.a/360;
                            kinedata.lastP.a -= kinedata.circle_count[joint_num]*360;
                            //printf("kinedata.lastP %.3f  motor_poscmd:%.3f  count:%d\n",kinedata.lastP.c,emcmotStatus->carte_pos_cmd.c,kinedata.circle_count[joint_num]);
                        } else if(joint_num == 4) {
                            kinedata.circle_count[joint_num] = (int)emcmotStatus->carte_pos_cmd.b/360;
                            kinedata.lastP.b -= kinedata.circle_count[joint_num]*360;
                            //printf("kinedata.lastP %.3f  motor_poscmd:%.3f  count:%d\n",kinedata.lastP.c,emcmotStatus->carte_pos_cmd.c,kinedata.circle_count[joint_num]);
                        } else if(joint_num == 5) {
                            kinedata.circle_count[joint_num] = (int)emcmotStatus->carte_pos_cmd.c/360;
                            kinedata.lastP.c -= kinedata.circle_count[joint_num]*360;
                            printf("kinedata.lastP %.3f  motor_poscmd:%.3f  count:%d\n",kinedata.lastP.c,emcmotStatus->carte_pos_cmd.c,kinedata.circle_count[joint_num]);
                        }
                    }

                }
                //rtapi_print("---3\n");
                SET_MOTION_COORD_FLAG(0);
                SET_MOTION_TELEOP_FLAG(0);
                SET_MOTION_ERROR_FLAG(0);
            } else {
                /* not in position-- don't honor mode change */
                emcmotDebug->coordinating = 1;
                waitabortmode = 1;
            }

        }
        if(waitabortmode > 0) {
            if (GET_MOTION_INPOS_FLAG()) {
                printf("wait for motion end\n");
                SET_MOTION_COORD_FLAG(0);
                SET_MOTION_TELEOP_FLAG(0);
                SET_MOTION_ERROR_FLAG(0);
                waitabortmode = 0;
            }
        }
    }
    /*! \todo FIXME - this code is temporary - eventually this function will be
     cleaned up and simplified, and 'motion_state' will become the master
     for this info, instead of having to gather it from several flags */
    if (!GET_MOTION_ENABLE_FLAG()) {
        emcmotStatus->motion_state = EMCMOT_MOTION_DISABLED;
    } else if (GET_MOTION_TELEOP_FLAG()) {
        emcmotStatus->motion_state = EMCMOT_MOTION_TELEOP;
    } else if (GET_MOTION_COORD_FLAG()) {
        emcmotStatus->motion_state = EMCMOT_MOTION_COORD;
    } else {
        emcmotStatus->motion_state = EMCMOT_MOTION_FREE;
    }
}

static void handle_jogwheels(void) {
    int joint_num;
    emcmot_joint_t *joint;
    joint_hal_t *joint_data;
    int new_jog_counts, delta;
    double distance, pos, stop_dist;
    static int flag=1;

    for (joint_num = 0; joint_num < num_joints; joint_num++) {


        /* point to joint data */
        joint_data = &(emcmot_hal_data->joint[joint_num]);
        joint = &joints[joint_num];


        if(emcmotStatus->handleuseType == 0) {
            if(emcmotStatus->jog_state == 2) {
                if(joint_num > 1 ) {
                    if(joints[joint_num].axis_active == 0){
                        joint->old_jog_counts = emcmotStatus->jogCount;
                       continue;
                   }
                }
                if(joint_num < 2) {
                     joint->old_jog_counts = emcmotStatus->jogCount;
                    continue;
                }
            } else {
                if(joint_num != emcmotStatus->jog_state || emcmotStatus->jog_state > 2) {
                     joint->old_jog_counts = emcmotStatus->jogCount;
                    continue;
                }
            }

        } else {
            if(joint_num != emcmotStatus->jog_state) {
                 joint->old_jog_counts = emcmotStatus->jogCount;
                continue;
            }
        }
        if (!GET_JOINT_ACTIVE_FLAG(joint)) {
            /* if joint is not active, skip it */
            continue;
        }
        new_jog_counts = emcmotStatus->jogCount;
        delta = new_jog_counts - joint->old_jog_counts;
        joint->old_jog_counts = new_jog_counts;
        if (delta == 0) {
            continue;
        }

               //ghf-2014/12/17
        if(delta != 1 && delta != -1) {
//            rtapi_print("delta is too large,ignore delta=%d\n",delta);
            delta = 0;
            continue;
        }

        /* must be in free mode and enabled */
        if (GET_MOTION_COORD_FLAG()) {
            continue;
        }
        if (!GET_MOTION_ENABLE_FLAG()) {
            continue;
        }
        /* the jogwheel input for this joint must be enabled */
        if (*(joint_data->jog_enable) == 0) {
            continue;
        }
        /* must not be homing */
        if (emcmotStatus->homing_active) {
            continue;
        }
        /* must not be doing a keyboard jog */
        if (joint->kb_jog_active) {
            continue;
        }
        if (emcmotStatus->net_feed_scale < 0.0001) {
            /* don't jog if feedhold is on or if feed override is zero */
            break;
        }
        /* calculate distance to jog */
        if(emcmotStatus->handleDir == 0) {
            distance = delta * emcmotStatus->jogScale;
        } else {
            distance = -delta * emcmotStatus->jogScale;
        }

        //ghf-20150716
        if(GET_JOINT_PHL_FLAG(joint)) {
            if(emcmotStatus->firvel[joint_num] == 0) {
                if(distance > 0) {
                    reportError(_("Can't jog (joint %d) further past max hard limit."),
                            joint_num);
                    return;
                }
            } else if(emcmotStatus->firvel[joint_num]*distance>0) {//如果还是往碰到限制位开关的方向行进
                if(flag==1) {
                    reportError(_("Can't jog (joint %d) further past max hard limit."),
                            joint_num);
                    flag=0;
                }
                //rtapi_print("curdistance=%d\n",(int)(distance*1000));
                return;
            } else {
                flag=1;
            }
        }
        else if(GET_JOINT_NHL_FLAG(joint)) {
            if(emcmotStatus->firvel[joint_num] == 0) {
                if(distance < 0) {
                    reportError(_("Can't jog (joint %d) further past max hard limit."),
                            joint_num);
                    return;
                }
            } else if(emcmotStatus->firvel[joint_num]*distance>0) {//如果还是往碰到限制位开关的方向行进
                if(flag==1){
                    reportError(_("Can't jog (joint %d) further past min hard limit."),
                            joint_num);
                    flag=0;
                }
                //rtapi_print("curdistance=%d\n",(int)(distance*1000));
                return;
            }
            else {
                flag=1;
            }
        } else if(emcmotStatus->probe_alarm > 0 && joint_num > 1 && distance < 0) {
            if(flag == 1) {
                reportError(_("对刀报警信号已触发"),
                        joint_num);
                flag=0;
            }
            return;
        } else {
            flag = 1;
        }

        /* calc target position for jog */
        pos = joint->free_pos_cmd + distance;
        refresh_jog_limits(joint);
        if (pos > joint->max_jog_limit && joint->circle_shortest == 0 && distance > 0) {
            continue;
        }
        if (pos < joint->min_jog_limit && joint->circle_shortest == 0 && distance < 0) {
            continue;
        }
        /* The default is to move exactly as far as the wheel commands,
         even if that move takes much longer than the wheel movement
         that commanded it.  Some folks prefer that the move stop as
         soon as the wheel does, even if that means not moving the
         commanded distance.  Velocity mode is for those folks.  If
         the command is faster than the machine can track, excess
         command is simply dropped. */
        if (*(joint_data->jog_vel_mode)) {
            double v = joint->vel_limit * emcmotStatus->net_feed_scale;
            /* compute stopping distance at max speed */
            stop_dist = v * v / (2 * joint->acc_limit);
            /* if commanded position leads the actual position by more
             than stopping distance, discard excess command */

//            if(delta != 0) {
//                printf("now I get the pos:%f  dis:%f  %d   joint:%d  stop:%f  acc:%f  v:%f freepos:%f  vellim:%f\n",pos,distance,delta,joint_num,
//                       stop_dist, joint->acc_limit,v,joint->free_pos_cmd,joint->vel_limit);
//            }

            if (pos > joint->pos_cmd + stop_dist) {
                pos = joint->pos_cmd + stop_dist;
            } else if (pos < joint->pos_cmd - stop_dist) {
                pos = joint->pos_cmd - stop_dist;
            }
        }
        /* set target position and use full velocity */
        joint->free_pos_cmd = pos;
        if(fabs(emcmotStatus->useSeclimit) > 0.0001 && GET_JOINT_HOMED_FLAG(joint)) {
            if(joint_num < 2) {
                if(joint->free_pos_cmd > joint->sec_pos_limit  || joint->free_pos_cmd < joint->sec_neg_limit) {
                    if(joint->free_pos_cmd > joint->sec_pos_limit && distance > 0)
                     joint->free_pos_cmd = joint->pos_cmd;
                    if(joint->free_pos_cmd < joint->sec_neg_limit && distance < 0)
                     joint->free_pos_cmd = joint->pos_cmd;
                } else {
                    if(fabs(joint->sec_limit_offset) > 0.0001) {  //the third limit
                            if(joints[0].free_pos_cmd > (joints[0].sec_neg_limit+joints[0].sec_limit_offset) &&
                                   joints[0].free_pos_cmd < (joints[0].sec_pos_limit-joints[0].sec_limit_offset) &&
                                    joints[1].free_pos_cmd > (joints[1].sec_neg_limit+joints[1].sec_limit_offset) &&
                                    joints[1].free_pos_cmd < (joints[1].sec_pos_limit-joints[1].sec_limit_offset) )  {
                                    joint->free_pos_cmd = joint->pos_cmd;
                            } else
                                 joint->free_pos_cmd = pos;
                    } else
                        joint->free_pos_cmd = pos;
                }
            }
        }

        joint->free_vel_lim = joint->vel_limit;
        /* lock out other jog sources */
        joint->wheel_jog_active = 1;
        /* and let it go */
        joint->free_tp_enable = 1;


        SET_JOINT_ERROR_FLAG(joint, 0);
        /* clear joint homed flag(s) if we don't have forward kins.
         Otherwise, a transition into coordinated mode will incorrectly
         assume the homed position. Do all if they've all been moved
         since homing, otherwise just do this one */
        clearHomes(joint_num);
    }
}

static void handle_jogwheels_guide(void)	//ghf-2014/3/10
{
    int joint_num = 0;
    emcmot_joint_t *joint;
    joint_hal_t *joint_data;
    int new_jog_counts = 0;
    double scale = 1;
    double delta = 0;

    int number=0;
    static double average_v1 = 0;
    static int filter_timer = 200,initdelta = 1;
    static double olddelta[200];

    emcmotStatus->feed_scale = 0;
    if(initdelta > 0) {
        initdelta = 0;
        int k = 0;
        for(k=0;k<200;k++)
            olddelta[k] = 0;
    }

    for (joint_num = 0; joint_num < num_joints; joint_num++) {
        /* point to joint data */
        joint_data = &(emcmot_hal_data->joint[joint_num]);
        joint = &joints[joint_num];
        if (!GET_JOINT_ACTIVE_FLAG(joint)) {
            /* if joint is not active, skip it */
            continue;
        }

        if(0 > emcmotStatus->jog_state) {
             joint->old_jog_counts = emcmotStatus->jogCount;
            continue;
        }

        /* get counts from jogwheel */
        new_jog_counts = emcmotStatus->jogCount;
        delta = new_jog_counts - joint->old_jog_counts;
        /* save value for next time */
        joint->old_jog_counts = new_jog_counts;

        /* must be in coord mode and enabled */
        if (GET_MOTION_COORD_FLAG() == 0) {	//当前为手动模式
            continue;
        }
        if (!GET_MOTION_ENABLE_FLAG()) {
            continue;
        }
        /* did the wheel move? */
        if (delta != 0) {	//有一个轴不为0,就要退出，同一时刻只能有一个轴在动,不然后面的0会冲掉前面的非0值
            break;
        }
    }

    if (fabs(delta) > 20) { //第一次有时会读到错误数据
        return;
    }

    if (delta < 0)
        delta = -delta;

    if(emcmotStatus->jogScale == 1 * emcmotStatus->joguseScale) {
        delta = delta / 100;
        scale = 0.8;
    } else if(emcmotStatus->jogScale == 10 * emcmotStatus->joguseScale) {
        delta = delta / 10;
        scale = 0.8;
    } else if(emcmotStatus->jogScale == 100 * emcmotStatus->joguseScale) {
        scale = 1;
    }

    average_v1 = 0.0;
    for (number=0;number < filter_timer-1;number++){
        olddelta[filter_timer- number -1] = olddelta[filter_timer - number -2];
        average_v1 += olddelta[filter_timer - number -1];
    }
    olddelta[0] = delta;
    average_v1 += olddelta[0];



    if (average_v1 == 0) { //当前所有轴都为0ghf-2014/5/13
        emcmotStatus->feed_scale = 0;
        sqSetFeedOverride(&queue, emcmotStatus->feed_scale);
    } else {
        if(average_v1 > 100) {
            emcmotStatus->feed_scale = 1.0;
        }
        else {
            emcmotStatus->feed_scale = average_v1 / 100;
        }
                //ghf-2014/11/7当前指令是g0z-10时，进给速度为600mm/min
        if ((fabs(emcmotStatus->currentuVel.x) <= 1e-6) &&(fabs(emcmotStatus->currentuVel.y) <= 1e-6)&&(emcmotStatus->currentuVel.z < 1e-6)) {

            emcmotStatus->feed_scale =  emcmotStatus->feed_scale*scale*0.5;
        }

        sqSetFeedOverride(&queue, emcmotStatus->feed_scale*scale);
    }
}

static void get_pos_cmds(long period) {
    int joint_num;
    emcmot_joint_t *joint;
    double positions[EMCMOT_MAX_JOINTS];
    /*! \todo Another #if 0 */
#if 0
    static int interpolationCounter = 0;
#endif
    double old_pos_cmd,z_old_poscmd;


    double max_dv, tiny_dp, pos_err, vel_req, vel_lim;

    int number;                  // heyuebang-2014-09-29
 //   double average_v1, average_v2; //heyuebang-2014-09-29

    static int test = 0;
    int onlimit;
    int ret = -1;

//    servo_period = 0.001; //ghf-2014/5/7,解决第一次走，少走一个的问题
//    servo_freq = 1000;

    for (joint_num = 0; joint_num < num_joints; joint_num++) {
        joint = &joints[joint_num];
        positions[joint_num] = joint->coarse_pos;

        if (emcmotStatus->motion_state != EMCMOT_MOTION_FREE)  // heyuebang in 2-14-09-29
        {
            joint->old_pos_cmd = joint->pos_cmd;
            joint->old_v_cmd = joint->vel_cmd;

            for (number =0; number<number_first_filter;number++)
            {
                joint->First_filter[number] = joint->old_v_cmd;
            }

            for (number =0; number<number_second_filter;number++)
            {
                joint->Second_filter[number] = joint->old_v_cmd;
            }
        }
    }
    /* if less than a full complement of joints, zero out the rest */
    while (joint_num < EMCMOT_MAX_JOINTS) {
        positions[joint_num++] = 0.0;
    }


    sqSyncPar(&queue,&emcmotStatus->dynamicparameter[0]);
    emcmotStatus->distance_to_go = emcmotStatus->dynamicparameter[10];

    emcmotStatus->distance_to_go_x = emcmotStatus->dynamicparameter[11];
    emcmotStatus->distance_to_go_y =  emcmotStatus->dynamicparameter[12];
    emcmotStatus->distance_to_go_z =  emcmotStatus->dynamicparameter[13];

    emcmotStatus->gmodetype =  emcmotStatus->dynamicparameter[14];
    emcmotStatus->currentuVel.x =  emcmotStatus->dynamicparameter[15];
    emcmotStatus->currentuVel.y =  emcmotStatus->dynamicparameter[16];
    emcmotStatus->currentuVel.z = emcmotStatus->dynamicparameter[17];
    emcmotStatus->vel = emcmotStatus->dynamicparameter[18];
    emcmotStatus->current_vel = emcmotStatus->dynamicparameter[19];
    emcmotStatus->pausefinish = emcmotStatus->dynamicparameter[20];

    /* run traj planner code depending on the state */
    switch (emcmotStatus->motion_state) {
    case EMCMOT_MOTION_FREE: //手动模式

        /* in free mode, each joint is planned independently */
        /* Each joint has a very simple "trajectory planner".  If the planner
         is enabled (free_tp_enable), then it moves toward free_pos_cmd at
         free_vel_lim, obeying the joint's accel and velocity limits, and
         stopping when it gets there.  If it is not enabled, it stops as
         quickly as possible, again obeying the accel limit.  When
         disabled, free_pos_cmd is set to the current position. */
        /* initial value for flag, if needed it will be cleared below */
        SET_MOTION_INPOS_FLAG(1); //1表示轴都在允许的运动范围内

        for (joint_num = 0; joint_num < num_joints; joint_num++) {
            /* point to joint struct */
            joint = &joints[joint_num];

            //Y2轴不规划
            if(joint_num == 7)
                continue;

            if (joint->acc_limit > emcmotStatus->acc)
                joint->acc_limit = emcmotStatus->acc;
            //AJ: only need to worry about free mode if joint is active
            if (GET_JOINT_ACTIVE_FLAG(joint)) {
                joint->free_tp_active = 0;
                max_dv = joint->acc_limit * servo_period;
                tiny_dp = max_dv * servo_period * 0.001;
                if (joint->free_tp_enable) {
                    pos_err = joint->free_pos_cmd -  joint->old_pos_cmd; // heyuebang in 2014-09-29

                    if (pos_err > tiny_dp) {
                        vel_req = -max_dv + sqrt(2.0 * joint->acc_limit * pos_err+ max_dv * max_dv);
                        joint->free_tp_active = 1;
                    } else if (pos_err < -tiny_dp) {
                        vel_req = max_dv - sqrt(-2.0 * joint->acc_limit * pos_err + max_dv * max_dv);
                        joint->free_tp_active = 1;
                    } else {
                        vel_req = 0.0;
                    }
                } else {
                    vel_req = 0.0;
                    joint->free_pos_cmd = joint->old_pos_cmd; //heyuebang 2014-09-29
                }
                if (joint->free_tp_active) {
                    SET_JOINT_AT_HOME_FLAG(joint, 0);
                }
                if (joint->home_state == HOME_IDLE) {
                    vel_lim = joint->free_vel_lim
                            * emcmotStatus->net_feed_scale; //此处表明手动规划速度与倍率有关ghf-2014/5/23
                } else {
                    vel_lim = joint->free_vel_lim;
                }

                if (vel_lim > joint->vel_limit) {
                    vel_lim = joint->vel_limit;
                }
                /* limit velocity request */
                if (vel_req > vel_lim) {
                    vel_req = vel_lim;
                } else if (vel_req < -vel_lim) {
                    vel_req = -vel_lim;
                }
                if (vel_req > joint->old_v_cmd + max_dv) {
                    joint->old_v_cmd += max_dv;
                } else if (vel_req < joint->old_v_cmd - max_dv) {
                    joint->old_v_cmd -= max_dv;
                } else {
                    joint->old_v_cmd = vel_req;
                }
                if (joint->old_v_cmd != 0.0) {
                    joint->free_tp_active = 1;
                }


                if(emcmotStatus->homing_active == 0) {
                    joint->vel_cmd = meanfilter(joint_num,joint->old_v_cmd);  //采用均值滤波
                } else  if (emcmotStatus->homing_active == 1) {
                    joint->vel_cmd = joint->old_v_cmd;                 //回零时不采用均值滤波  add by hong
                }

                if (joint->vel_cmd != 0.0) { //这里表示100次之后才能停止
                    joint->free_tp_active = 1;
                }



                joint->old_pos_cmd += joint->old_v_cmd * servo_period;

                joint->pos_cmd += joint->vel_cmd * servo_period;
                joint->coarse_pos = joint->pos_cmd;

                joint->coarsevel_cmd = joint->vel_cmd;//ghf-2014/9/18



                // Y2轴坐标跟随
                if(joint_num == 1) {
                    joints[7].pos_cmd += joint->vel_cmd * servo_period;
                    joints[7].vel_cmd = joint->vel_cmd;
                }


                static int posonlimit[9] = {1,1,1,1,1,1,1,1,1};
                ret = checkAllHomed();
                if (ret) {//ghf-20150716,回零期间忽略软限位
                    if (fabs(joint->pos_cmd - joint->max_pos_limit)<=1e-4) {
                        if (posonlimit[joint_num]==1)
                            reportError(_("(joint %d) on positive soft limit"),joint_num);
                        posonlimit[joint_num] = 0;
                    }
                    else {
                        posonlimit[joint_num]=1;
                    }
                }

                static int negonlimit[9] = {1,1,1,1,1,1,1,1,1};

                if (ret) {//ghf-20150716,回零期间忽略软限位
                    if (fabs(joint->pos_cmd - joint->min_pos_limit)<=1e-4) {
                        if (negonlimit[joint_num]==1)
                            reportError(_("(joint %d) on negtive soft limit"),joint_num);
                        negonlimit[joint_num] = 0;
                    }
                    else {
                        negonlimit[joint_num]=1;
                    }
                }

                /* update joint status flag and overall status flag */
                if (joint->free_tp_active) {
                    /* active TP means we're moving, so not in position */
                    SET_JOINT_INPOS_FLAG(joint, 0);
                    SET_MOTION_INPOS_FLAG(0);
                    /* is any limit disabled for this move? */
                    if (emcmotStatus->overrideLimitMask) {
                        emcmotDebug->overriding = 1;
                    }
                } else {
                    SET_JOINT_INPOS_FLAG(joint, 1);
                    /* joint has stopped, so any outstanding jogs are done */
                    joint->kb_jog_active = 0;
                    joint->wheel_jog_active = 0;

                }
            }		    //if (GET_JOINT_ACTIVE_FLAG(join))

        }		    //for loop for joints

        /* if overriding is true and we're in position, the jog
         is complete, and the limits should be re-enabled */
        if ((emcmotDebug->overriding) && (GET_MOTION_INPOS_FLAG())) {
            emcmotStatus->overrideLimitMask = 0;
            emcmotDebug->overriding = 0;
        }

        /*! \todo FIXME - this should run at the traj rate */


        switch (kinType) {

        case KINEMATICS_IDENTITY:

            kinematicsForward(positions, &emcmotStatus->carte_pos_cmd, &fflags,
                    &iflags);
            ret = checkAllHomed();
            if (ret) {
                emcmotStatus->carte_pos_cmd_ok = 1;
            } else {
                emcmotStatus->carte_pos_cmd_ok = 0;
            }
            break;

        default:
            emcmotStatus->carte_pos_cmd_ok = 0;
            break;
        }
        /* end of FREE mode */
        break;

    case EMCMOT_MOTION_COORD:


        if(emcmotStatus->forcflag==0) {
            while (cubicNeedNextPoint(&(joints[0].cubic))) { //为什么是joints[0]呢？？，第一个轴插补完了，其他轴也就都插补完了;总之，每精插补10次，while就会执行一次ghf-2014/4/4

                ret = sqRunCycle (&queue); //ghf 2015-2-26

                if(ret!=0) {
                    sqAbort(&queue);
                    emcmotStatus->runcycleflag=-1;
                    //emcmotDebug->enabling = 0;//ghf-20150624
                }
                else {
                    emcmotStatus->runcycleflag=0;
                }
                sqGetPosition(&queue, &emcmotStatus->carte_pos_cmd); //ghf 2014-1-25

                /* OUTPUT KINEMATICS - convert to joints in local array */
                kinematicsInverse(&emcmotStatus->carte_pos_cmd, positions, &iflags,&fflags);

                /* copy to joint structures and spline them up */

                for (joint_num = 0; joint_num < num_joints; joint_num++) {
                    /* point to joint struct */
                    joint = &joints[joint_num];
                    joint->coarse_pos = positions[joint_num];

                    cubicAddPoint(&(joint->cubic), joint->coarse_pos);//界面显示joint->coarse_pos的值

                }

                /* END OF OUTPUT KINS */
            }

            /* there is data in the interpolators */
            /* run interpolation */
            for (joint_num = 0; joint_num < num_joints; joint_num++) {
                if(joint_num == 6 || joint_num == 7)
                    continue;
                /* point to joint struct */
                joint = &joints[joint_num];
                /* save old command */
                old_pos_cmd = joint->pos_cmd;

                /* interpolate to get new one */
                joint->pos_cmd = cubicInterpolate(&(joint->cubic), 0, 0, 0, 0); //这里会导致突变ghf-2014/4/29
                if(joint_num == 1) {
                    if(emcmotStatus->useWitchY == 0) {
                    } else {
                        joints[6].pos_cmd = joint->pos_cmd;
                        joint->pos_cmd = joint->old_pos_cmd;
                    }
                }
                joint->vel_cmd = (joint->pos_cmd - old_pos_cmd) * servo_freq;

                // Y2轴坐标跟随
                if(joint_num == 1) {
                    joints[7].pos_cmd += joint->vel_cmd * servo_period;
                    joints[7].vel_cmd = joint->vel_cmd;
                }
            }

            /* report motion status */
            SET_MOTION_INPOS_FLAG(0);

            if (sqIsDone (&queue)) {//ghf-2014/10/24，正常运行完此处成立，暂停后此处也成立
                 //rtapi_print("in abort print\n");
                SET_MOTION_INPOS_FLAG(1);
            }

        }
        else if(emcmotStatus->forcflag==1) {

            for (joint_num = 2; joint_num < 6; joint_num++) {
                /* point to joint struct */
                joint = &joints[joint_num];
                if (joint->acc_limit > emcmotStatus->acc)
                    joint->acc_limit = emcmotStatus->acc;

                joint->vel_cmd = SimpleTraj(joint);

                if (joint->vel_cmd == 0.0) {
                    SET_JOINT_INPOS_FLAG(joint, 1);
                }
                else {
                    SET_JOINT_INPOS_FLAG(joint, 0);
                }

                /* integrate velocity to get new position */
                joint->old_pos_cmd = joint->pos_cmd;
                joint->pos_cmd += joint->vel_cmd * servo_period;
                joint->coarse_pos = joint->pos_cmd;

            }

            static int checkcount = 0;
            if(fabs(joints[2].freepos-joints[2].pos_cmd) == 0 &&
                    fabs(joints[3].freepos-joints[3].pos_cmd) == 0 &&
                    fabs(joints[4].freepos-joints[4].pos_cmd) == 0 &&
                    fabs(joints[5].freepos-joints[5].pos_cmd) == 0 &&
                    emcmotStatus->changingz == 1) {
                checkcount++;
                if(checkcount > 50) {
                    checkcount = 0;
                    SET_MOTION_INPOS_FLAG(1);
                    emcmotStatus->carte_pos_cmd.tran.z=joints[2].pos_cmd;
                    emcmotStatus->carte_pos_cmd.a=joints[3].pos_cmd;
                    emcmotStatus->carte_pos_cmd.b=joints[4].pos_cmd;
                    emcmotStatus->carte_pos_cmd.c=joints[5].pos_cmd;
                    sqSetPos(&queue, emcmotStatus->carte_pos_cmd);
                    cubicDrain(&(joints[0].cubic));
                    cubicDrain(&(joints[1].cubic));
                    cubicDrain(&(joints[2].cubic));
                    cubicDrain(&(joints[3].cubic));
                    cubicDrain(&(joints[4].cubic));
                    cubicDrain(&(joints[5].cubic));
                    emcmotStatus->forcflag=0;
                    emcmotStatus->changezend=1;
                    emcmotStatus->changingz = 0;
                    printf("now I finish changing z  %f   %f \n",joints[2].freepos,joints[2].pos_cmd);
                    test = 1;
                    sqClearQueue(&queue);
                }
            }  else {
                SET_MOTION_INPOS_FLAG(0);
                emcmotStatus->changezend=0;
                emcmotStatus->changingz = 1;
            }
        }

        break;

    case EMCMOT_MOTION_DISABLED:
        /* set position commands to match feedbacks, this avoids
         disturbances and/or following errors when enabling */
        emcmotStatus->carte_pos_cmd = emcmotStatus->carte_pos_fb;
        for (joint_num = 0; joint_num < num_joints; joint_num++) {
            /* point to joint struct */
            joint = &joints[joint_num];
            /* save old command */

            //joint->pos_cmd = joint->pos_fb; //ghf-20150716
            //joint->coarse_pos = joint->pos_cmd;//ghf-2014/8/12

            /* set joint velocity to zero */
            joint->vel_cmd = 0.0;

            //ghf-2014/6/26,回零过程中出错时作此处理
            switchfb[joint_num] = 0;
            oldswitchfb[joint_num] = 0;
            homeiwt[joint_num] = 0;
        }

        break;
    default:
        break;
    }

    for (joint_num = 0; joint_num < num_joints; joint_num++) {
        joint = &joints[joint_num];
        joint->program_cmd = joint->pos_cmd;
        if(joint_num == 5)
            joint->program_cmd = joint->pos_cmd - (int)joint->pos_cmd/360 * 360;
    }

    if (sqGetPlotData(&queue) == 0)
          sendDatatoMem(&emcmotStatus->carte_pos_cmd); //显示粗插补数据
    else if (sqGetPlotData(&queue) == 1)
    {
        double temp[9];
        EmcPose data_dikaer;
        for (joint_num = 0; joint_num < num_joints; joint_num++) {
            /* point to joint struct */
            joint = &joints[joint_num];
            temp[joint_num] = joint->pos_cmd;
        }
        kinematicsForward(temp, &data_dikaer, &fflags,&iflags); //将精插补单轴数据转换为笛卡尔坐标系下数据
        sendDatatoMem(&data_dikaer);  //显示精插补数据
    }else if (sqGetPlotData(&queue) == 2)
    {
        sendDatatoMem(&emcmotStatus->carte_pos_fb); //显示编码器返回数据
    }else{
         sendDatatoMem(&emcmotStatus->carte_pos_cmd); //默认为 0
    }


    static int recordprobealarm = 0;
    if(recordprobealarm != emcmotStatus->probe_alarm) {
        if(emcmotStatus->probe_alarm > 0) {
            if(GET_MOTION_ERROR_FLAG() == 0) {
                reportError(_("触发对刀仪报警"));
                SET_MOTION_ERROR_FLAG(1);
                sqAbort(&queue);
            }
        }
        recordprobealarm = emcmotStatus->probe_alarm;
    }


    ret = checkAllHomed();

    onlimit = 0;
    for (joint_num = 0; joint_num < num_joints; joint_num++) {

        /* point to joint data */
        joint = &joints[joint_num];
        /* skip inactive or unhomed axes */
        if (GET_JOINT_ACTIVE_FLAG(joint) && GET_JOINT_HOMED_FLAG(joint) && joint->circle_shortest == 0) {
            /* check for soft limits */
            if (ret) {
                if (joint->pos_cmd > joint->max_pos_limit + 0.5) {
                    onlimit = 1;
                    if (!emcmotStatus->on_soft_limit)
                        reportError(_("轴%d触发正软限位"),joint_num);//这里不要随便改，不然翻译文件设别不到ghf-2014/10/23
                }
                if (joint->pos_cmd < joint->min_pos_limit - 0.5) {
                    onlimit = 1;
                    if (!emcmotStatus->on_soft_limit)
                        reportError(_("轴%d触发负软限位"),joint_num);
                }
            }


            if(fabs(emcmotStatus->useSeclimit) > 0.0001 && GET_JOINT_HOMED_FLAG(joint)) {
                if(joint_num < 2) {     //只限制XY
                    if(joint->pos_cmd < joint->sec_neg_limit) {
                        onlimit = 1;
                        if (!emcmotStatus->on_soft_limit)
                        reportError(_("轴%d触发第二负软限位"),joint_num);
                    }
                    if(joint->pos_cmd > joint->sec_pos_limit) {
                        onlimit = 1;
                        if (!emcmotStatus->on_soft_limit)
                        reportError(_("轴%d触发第二正软限位"),joint_num);
                    }

                    if(fabs(joints[0].sec_limit_offset) > 0.00001 || fabs(joints[1].sec_limit_offset) > 0.00001) {
                        if(joints[0].pos_cmd > (joints[0].sec_neg_limit+joints[0].sec_limit_offset) &&
                               joints[0].pos_cmd < (joints[0].sec_pos_limit-joints[0].sec_limit_offset) &&
                                joints[1].pos_cmd > (joints[1].sec_neg_limit+joints[1].sec_limit_offset) &&
                                joints[1].pos_cmd < (joints[1].sec_pos_limit-joints[1].sec_limit_offset) )  {
                            onlimit = 1;
                            if (!emcmotStatus->on_soft_limit)
                            reportError(_("Exceeded the third soft limit on joint %d"),joint_num);
                        }
                    }
                }


            }

        }
    }
    if (onlimit) { //自动模式下进到这里，一般都是规划出错导致的，这时都会报跟随错误，所以上层不会卡死ghf-2014/11/8
        if (!emcmotStatus->on_soft_limit) {
            /* just hit the limit */
            sqAbort (&queue);
            SET_MOTION_ERROR_FLAG(1);
            emcmotStatus->on_soft_limit = 1;
        }

    } else {
        emcmotStatus->on_soft_limit = 0;
    }
}

/* NOTES:  These notes are just my understanding of how things work.

 There are seven sets of position information.

 1) emcmotStatus->carte_pos_cmd
 2) emcmotStatus->joints[n].coarse_pos
 3) emcmotStatus->joints[n].pos_cmd
 4) emcmotStatus->joints[n].motor_pos_cmd
 5) emcmotStatus->joints[n].motor_pos_fb
 6) emcmotStatus->joints[n].pos_fb
 7) emcmotStatus->carte_pos_fb

 Their exact contents and meaning are as follows:

 1) This is the desired position, in Cartesean coordinates.  It is
 updated at the traj rate, not the servo rate.
 In coord mode, it is determined by the traj planner
 In teleop mode, it is determined by the traj planner?
 In free mode, it is not used, since free mode motion takes
 place in joint space, not cartesean space.  It may be
 displayed by the GUI however, so it is updated by
 applying forward kins to (2), unless forward kins are
 not available, in which case it is copied from (7).

 2) This is the desired position, in joint coordinates, but
 before interpolation.  It is updated at the traj rate, not
 the servo rate..
 In coord mode, it is generated by applying inverse kins to (1)
 In teleop mode, it is generated by applying inverse kins to (1)
 In free mode, it is not used, since the free mode planner generates
 a new (3) position every servo period without interpolation.
 However, it is used indirectly by GUIs, so it is copied from (3).

 3) This is the desired position, in joint coords, after interpolation.
 A new set of these coords is generated every servo period.
 In coord mode, it is generated from (2) by the interpolator.
 In teleop mode, it is generated from (2) by the interpolator.
 In free mode, it is generated by the simple free mode traj planner.

 4) This is the desired position, in motor coords.  Motor coords are
 generated by adding backlash compensation, lead screw error
 compensation, and offset (for homing) to (3).
 It is generated the same way regardless of the mode, and is the
 output to the PID loop or other position loop.

 5) This is the actual position, in motor coords.  It is the input from
 encoders or other feedback device (or from virtual encoders on open
 loop machines).  It is "generated" by reading the feedback device.

 6) This is the actual position, in joint coordinates.  It is generated
 by subtracting offset, lead screw error compensation, and backlash
 compensation from (5).  It is generated the same way regardless of
 the operating mode.

 7) This is the actual position, in Cartesean coordinates.  It is updated
 at the traj rate, not the servo rate.
 OLD VERSION:
 In the old version, there are four sets of code to generate actualPos.
 One for each operating mode, and one for when motion is disabled.
 The code for coord and teleop modes is identical.  The code for free
 mode is somewhat different, in particular to deal with the case where
 one or more axes are not homed.  The disabled code is quite similar,
 but not identical, to the coord mode code.  In general, the code
 calculates actualPos by applying the forward kins to (6).  However,
 where forward kins are not available, actualPos is either copied
 from (1) (assumes no following error), or simply left alone.
 These special cases are handled differently for each operating mode.
 NEW VERSION:
 I would like to both simplify and relocate this.  As far as I can
 tell, actualPos should _always_ be the best estimate of the actual
 machine position in Cartesean coordinates.  So it should always be
 calculated the same way.
 In addition to always using the same code to calculate actualPos,
 I want to move that code.  It is really a feedback calculation, and
 as such it belongs with the rest of the feedback calculations early
 in control.c, not as part of the output generation code as it is now.
 Ideally, actualPos would always be calculated by applying forward
 kinematics to (6).  However, forward kinematics may not be available,
 or they may be unusable because one or more axes aren't homed.  In
 that case, the options are: A) fake it by copying (1), or B) admit
 that we don't really know the Cartesean coordinates, and simply
 don't update actualPos.  Whatever approach is used, I can see no
 reason not to do it the same way regardless of the operating mode.
 I would propose the following:  If there are forward kins, use them,
 unless they don't work because of unhomed axes or other problems,
 in which case do (B).  If no forward kins, do (A), since otherwise
 actualPos would _never_ get updated.

 */

/*****************************************************************************************/
//heyuebang 2014-11-20
//the equation of the fuction compute_fric_comp(void) is
//u = umax*exp(-(t-t_delay)/ts)*sign(vr);

static void compute_fric_comp(void){
    int joint_num;
    emcmot_joint_t *joint;

    for (joint_num = 0; joint_num < 2; joint_num++) {  //实际上只有x、y做了friction补偿
        /* point to joint struct */
        joint = &joints[joint_num];
        if (!GET_JOINT_ACTIVE_FLAG(joint)) {
            /* if joint is not active, skip it */
            continue;
        }

        if (joint ->fric_lag_ts0 <= 0 ){
            reportError(_("friction compensation lag time 0 must be positive %d <= 0"),(int)(joint->fric_lag_ts0 *1000));
            joint ->fric_lag_ts0 = 1.0;
        }
        if (joint ->fric_lag_ts1 <= 0 ){
            reportError(_("friction compensation lag time 1 must be negitive %d <= 0"),(int)(joint->fric_lag_ts1 *1000));
            joint ->fric_lag_ts1 = 1.0;
        }
        if (joint ->fric_delay_t <= 0 ){
            reportError(_("friction compensation delay time must be positive %d <= 0"),(int)(joint->fric_delay_t *1000));
            joint ->fric_delay_t = 1.0;
        }
        if (joint ->fric_max_u0 < 0 ){
            reportError(_("max friction compensation value 0 must be not negitive %d <= 0"),(int)(joint->fric_max_u0 *1000));
            joint ->fric_max_u0 = 0.0;
        }
        if (joint ->fric_max_u1 < 0 ){
            reportError(_("max friction compensation value 1 must be not negitive %d <= 0"),(int)(joint->fric_max_u1 *1000));
            joint ->fric_max_u1 = 0.0;
        }

        if (joint->vel_cmd == 0){

            joint->positive_vel_t = 0.0;
            joint ->negitive_vel_t = 0.0;
            joint ->fric_out_u = 0.0;

        }else if (joint->vel_cmd >0){
            joint->positive_vel_t += 1;  //1 is 1ms;
            joint ->negitive_vel_t = 0.0;

            if (joint->positive_vel_t >= joint->fric_delay_t){
                joint->fric_out_u = joint->fric_max_u0 * exp(-(joint->positive_vel_t-joint->fric_delay_t)/joint->fric_lag_ts0);
            }else{
                joint->fric_out_u = 0.0;
            }

        }else{
            joint->positive_vel_t = 0.0;
            joint ->negitive_vel_t +=1; //1 is 1ms;

            if (joint->negitive_vel_t >= joint->fric_delay_t){
                joint->fric_out_u = -joint->fric_max_u1 * exp(-(joint->negitive_vel_t-joint->fric_delay_t)/joint->fric_lag_ts1);
            }else{
                joint->fric_out_u = 0.0;
            }
        }

    }

}

//heyuebang 2014-11-20
//the equation of the fuction compute_fric_comp1(void) is
//u = umax*(exp(-t/ts)- exp(-t/t_delay))*sign(vr);
/****************************************************************/
//heyuebang 2014-11-20
// double positive_vel_t,negitive_vel_t;	/* 速度从0往正向过度时开始计时 */
// double fric_delay_t;//延时时间
// double fric_lag_ts0, fric_lag_ts1;		/* 指数函数的衰减时间常数 ts0为正向*/
// double fric_max_u0, fric_max_u1;		/* 指数函数的峰值 */
// double fric_out_u;		/* 指数函数的输出，作为实际的补偿值*/
/****************************************************************/

static void compute_fric_comp1(void){
    int joint_num;
    emcmot_joint_t *joint;

    for (joint_num = 0; joint_num < 2; joint_num++) {  //实际上只有x、y做了friction补偿
        /* point to joint struct */
        joint = &joints[joint_num];
        if (!GET_JOINT_ACTIVE_FLAG(joint)) {
            /* if joint is not active, skip it */
            continue;
        }

        if (joint ->fric_lag_ts0 <= 0 ){
            reportError(_("friction compensation lag time 0 must be positive %d <= 0"),(int)(joint->fric_lag_ts0 *1000));
            joint ->fric_lag_ts0 = 1.0;
        }
        if (joint ->fric_lag_ts1 <= 0 ){
            reportError(_("friction compensation lag time 1 must be negitive %d <= 0"),(int)(joint->fric_lag_ts1 *1000));
            joint ->fric_lag_ts1 = 1.0;
        }
        if (joint ->fric_delay_t <= 0 ){
            reportError(_("friction compensation delay time must be positive %d <= 0"),(int)(joint->fric_delay_t *1000));
            joint ->fric_delay_t = 1.0;
        }
        if (joint ->fric_max_u0 < 0 ){
            reportError(_("max friction compensation value 0 must be not negitive %d <= 0"),(int)(joint->fric_max_u0 *1000));
            joint ->fric_max_u0 = 0.0;
        }
        if (joint ->fric_max_u1 < 0 ){
            reportError(_("max friction compensation value 1 must be not negitive %d <= 0"),(int)(joint->fric_max_u1 *1000));
            joint ->fric_max_u1 = 0.0;
        }

        if (joint->vel_cmd == 0){

            joint->positive_vel_t = 0.0;
            joint ->negitive_vel_t = 0.0;
            joint ->fric_out_u = 0.0;

        }else if (joint->vel_cmd >0){
            joint->positive_vel_t += 1;  //1 is 1ms;
            joint ->negitive_vel_t = 0.0;

            joint->fric_out_u = joint->fric_max_u0 * ( exp(-joint->positive_vel_t/joint->fric_lag_ts0) - exp(-joint->positive_vel_t/joint->fric_delay_t));


        }else{
            joint->positive_vel_t = 0.0;
            joint ->negitive_vel_t +=1; //1 is 1ms;

            joint->fric_out_u = - joint->fric_max_u1 * ( exp(-joint->negitive_vel_t/joint->fric_lag_ts1) - exp(-joint->negitive_vel_t/joint->fric_delay_t));

        }

    }

}


static void compute_screw_comp(void) {
    int joint_num;
    emcmot_joint_t *joint;
    emcmot_comp_t *comp;
    double dpos;
    double a_max, v_max, v, s_to_go, ds_stop, ds_vel, ds_acc, dv_acc;

    /* compute the correction */
    for (joint_num = 0; joint_num < num_joints; joint_num++) {  //实际上只有x、y做了螺距补偿
        /* point to joint struct */
        joint = &joints[joint_num];
        if (!GET_JOINT_ACTIVE_FLAG(joint)) {
            /* if joint is not active, skip it */
            //continue;
            return; //ghf-2014/7/28
        }
        /* point to compensation data */
        comp = &(joint->comp);
        //反向间隙为0时，螺距补偿才有效
        if (comp->entries > 0  && fabs(joint->backlash) < 0.000001 ) {
            /* there is data in the comp table, use it */
            /* first make sure we're in the right spot in the table */
            while (joint->pos_cmd < comp->entry->nominal) {
                comp->entry--;
            }
            while (joint->pos_cmd >= (comp->entry + 1)->nominal) {
                comp->entry++;
            }
            /* now interpolate */
            dpos = joint->pos_cmd - comp->entry->nominal;
            if (joint->vel_cmd > 0.0) {
                /* moving "up". apply forward screw comp */
                joint->backlash_corr = comp->entry->fwd_trim
                        + comp->entry->fwd_slope * dpos;
            } else if (joint->vel_cmd < 0.0) {
                /* moving "down". apply reverse screw comp */
                joint->backlash_corr = comp->entry->rev_trim
                        + comp->entry->rev_slope * dpos;
            } else {
                /* not moving, use whatever was there before */
            }
        } else {
            /* no compensation data, just use +/- 1/2 of backlash */
            /** FIXME: this can actually be removed - if the user space code
             sends a single compensation entry with any nominal value,
             and with fwd_trim = +0.5 times the backlash value, and
             rev_trim = -0.5 times backlash, the above screw comp code
             will give exactly the same result as this code. */
            /* determine which way the compensation should be applied */
            if (joint->vel_cmd > 0.0) {
                /* moving "up". apply positive backlash comp */
                joint->backlash_corr = 0.5 * joint->backlash;
            } else if (joint->vel_cmd < 0.0) {
                /* moving "down". apply negative backlash comp */
                joint->backlash_corr = -0.5 * joint->backlash;
            } else {
                /* not moving, use whatever was there before */
            }
        }
        /* at this point, the correction has been computed, but
         the value may make abrupt jumps on direction reversal */
        /*
         * 07/09/2005 - S-curve implementation by Bas Laarhoven
         *
         * Implementation:
         *   Generate a ramped velocity profile for backlash or screw error comp.
         *   The velocity is ramped up to the maximum speed setting (if possible),
         *   using the maximum acceleration setting.
         *   At the end, the speed is ramped dowm using the same acceleration.
         *   The algorithm keeps looking ahead. Depending on the distance to go,
         *   the speed is increased, kept constant or decreased.
         *
         * Limitations:
         *   Since the compensation adds up to the normal movement, total
         *   accelleration and total velocity may exceed maximum settings!
         *   Currently this is limited to 150% by implementation.
         *   To fix this, the calculations in get_pos_cmd should include
         *   information from the backlash corection. This makes things
         *   rather complicated and it might be better to implement the
         *   backlash compensation at another place to prevent this kind
         *   of interaction.
         *   More testing under different circumstances will show if this
         *   needs a more complicate solution.
         *   For now this implementation seems to generate smoother
         *   movements and less following errors than the original code.
         */

        /* Limit maximum accelleration and velocity 'overshoot'
         * to 150% of the maximum settings.
         * The TP and backlash shouldn't use more than 100%
         * (together) but this requires some interaction that
         * isn't implemented yet.
         */
        v_max = 0.5 * joint->vel_limit * emcmotStatus->net_feed_scale;
        a_max = 0.5 * joint->acc_limit;
        v = joint->backlash_vel;
        if (joint->backlash_corr >= joint->backlash_filt) {
            //ghf-2014/5/27joint->backlash_filt初始化为0,这个才是每毫秒实际补偿的数据，joint->backlash_corr是当前周期应该补偿的数据
            s_to_go = joint->backlash_corr - joint->backlash_filt; /* abs val */
            if (s_to_go > 0) {
                // off target, need to move
                ds_vel = v * servo_period; /* abs val */
                dv_acc = a_max * servo_period; /* abs val */
                ds_stop = 0.5 * (v + dv_acc) * (v + dv_acc) / a_max; /* abs val */
                if (s_to_go <= ds_stop + ds_vel) {
                    // ramp down
                    if (v > dv_acc) {
                        // decellerate one period
                        ds_acc = 0.5 * dv_acc * servo_period; /* abs val */
                        joint->backlash_vel -= dv_acc;
                        joint->backlash_filt += ds_vel - ds_acc;
                    } else {
                        // last step to target
                        joint->backlash_vel = 0.0;
                        joint->backlash_filt = joint->backlash_corr;
                    }
                } else {
                    if (v + dv_acc > v_max) {
                        dv_acc = v_max - v; /* abs val */
                    }
                    ds_acc = 0.5 * dv_acc * servo_period; /* abs val */
                    ds_stop = 0.5 * (v + dv_acc) * (v + dv_acc) / a_max; /* abs val */
                    if (s_to_go > ds_stop + ds_vel + ds_acc) {
                        // ramp up
                        joint->backlash_vel += dv_acc;
                        joint->backlash_filt += ds_vel + ds_acc;
                    } else {
                        // constant velocity
                        joint->backlash_filt += ds_vel;
                    }
                }
            } else if (s_to_go < 0) {
                // safely handle overshoot (should not occur)
                joint->backlash_vel = 0.0;
                joint->backlash_filt = joint->backlash_corr;
            }
        } else { /* joint->backlash_corr < 0.0 */
            s_to_go = joint->backlash_filt - joint->backlash_corr; /* abs val */
            if (s_to_go > 0) {
                // off target, need to move
                ds_vel = -v * servo_period; /* abs val */
                dv_acc = a_max * servo_period; /* abs val */
                ds_stop = 0.5 * (v - dv_acc) * (v - dv_acc) / a_max; /* abs val */
                if (s_to_go <= ds_stop + ds_vel) {
                    // ramp down
                    if (-v > dv_acc) {
                        // decellerate one period
                        ds_acc = 0.5 * dv_acc * servo_period; /* abs val */
                        joint->backlash_vel += dv_acc; /* decrease */
                        joint->backlash_filt -= ds_vel - ds_acc;
                    } else {
                        // last step to target
                        joint->backlash_vel = 0.0;
                        joint->backlash_filt = joint->backlash_corr;
                    }
                } else {
                    if (-v + dv_acc > v_max) {
                        dv_acc = v_max + v; /* abs val */
                    }
                    ds_acc = 0.5 * dv_acc * servo_period; /* abs val */
                    ds_stop = 0.5 * (v - dv_acc) * (v - dv_acc) / a_max; /* abs val */
                    if (s_to_go > ds_stop + ds_vel + ds_acc) {
                        // ramp up
                        joint->backlash_vel -= dv_acc; /* increase */
                        joint->backlash_filt -= ds_vel + ds_acc;
                    } else {
                        // constant velocity
                        joint->backlash_filt -= ds_vel;
                    }
                }
            } else if (s_to_go < 0) {
                // safely handle overshoot (should not occur)
                joint->backlash_vel = 0.0;
                joint->backlash_filt = joint->backlash_corr;
            }
        }
        /* backlash (and motor offset) will be applied to output later */
        /* end of joint loop */
    }
}

/*! \todo FIXME - once the HAL refactor is done so that metadata isn't stored
 in shared memory, I want to seriously consider moving some of the
 structures into the HAL memory block.  This will eliminate most of
 this useless copying, and make nearly everything accessible to
 halscope and halmeter for debugging.
 */

static void output_to_hal(void) {
    int joint_num;
    emcmot_joint_t *joint;
    joint_hal_t *joint_data;
    static int old_motion_index = 0, old_hal_index = 0;
    int n;
    static int time = 0;

    /* output machine info to HAL for scoping, etc */
    *(emcmot_hal_data->motion_enabled) = GET_MOTION_ENABLE_FLAG();
    *(emcmot_hal_data->in_position) = GET_MOTION_INPOS_FLAG();
    *(emcmot_hal_data->coord_mode) = GET_MOTION_COORD_FLAG();
    *(emcmot_hal_data->teleop_mode) = GET_MOTION_TELEOP_FLAG();
    *(emcmot_hal_data->coord_error) = GET_MOTION_ERROR_FLAG();
    *(emcmot_hal_data->on_soft_limit) = emcmotStatus->on_soft_limit;
    *(emcmot_hal_data->set_which_z) = emcmotStatus->set_which_z; //ghf-2014/2/24
    *(emcmot_hal_data->selected_z) = emcmotStatus->selectz; //hong 2015-10-23
    *(emcmot_hal_data->feedrate) = emcmotStatus->net_feed_scale*100; //ghf-2014/12/17,shou dong bei lv

    //ghf-2014/10/24
    *(emcmot_hal_data->numSegments) = sqGetDepth(&queue);
    *(emcmot_hal_data->done) = sqIsDone(&queue);
    *(emcmot_hal_data->paused) = sqIsPaused(&queue);
    //*(emcmot_hal_data->aborting) = sqIsAborting(&queue);


    *(emcmot_hal_data->runStatus) = emcmotStatus->run_status; //hong-2014/11/03

    for (n = 0; n < 64; n++) {
        *(emcmot_hal_data->filtertimebuf[n]) = emcmotStatus->filtertimebuf[n];//ghf-2014/5/6
    }

    for (n = 0; n < 24; n++) {
        *(emcmot_hal_data->motion_extern_out[n]) = emcmotStatus->mot_extern_out[n];//hong-2014/08/23
    }

    /*for (n = 0; n < 8; n++) {
        *(emcmot_hal_data->cpuid[n]) = emcmotStatus->cpuid[n];//ghf-2014/7/24
    }
    for (n = 0; n < 4; n++) {
        *(emcmot_hal_data->hddid[n]) = emcmotStatus->hddid[n];//ghf-2014/7/24
    }
    for (n = 0; n < 4; n++) {
        *(emcmot_hal_data->wlefttime[n]) = emcmotStatus->wlefttime[n];//ghf-2014/7/24
    }
    for (n = 0; n < 1; n++) {
        *(emcmot_hal_data->wlefttimeflag) = emcmotStatus->wlefttimeflag;//ghf-2014/7/24
    }
    for (n = 0; n < 1; n++) {
        *(emcmot_hal_data->widflag) = emcmotStatus->widflag;//ghf-2014/7/24
    }*/


    //ghf-2014/3/17
    if (emcmotStatus->lightvalue == 1) {  //运行 绿灯
        *(emcmot_hal_data->green_on) = 1;
        *(emcmot_hal_data->yellow_on) = 0;
        *(emcmot_hal_data->red_on) = 0;
    } else if (emcmotStatus->lightvalue == 2) {  //空闲 黄灯
        *(emcmot_hal_data->yellow_on) = 1;
        *(emcmot_hal_data->green_on) = 0;
        *(emcmot_hal_data->red_on) = 0;
    } else if (emcmotStatus->lightvalue == 3) {  //报警 红灯
        *(emcmot_hal_data->red_on) = 1;
        *(emcmot_hal_data->green_on) = 0;
        *(emcmot_hal_data->yellow_on) = 0;
    }


    *(emcmot_hal_data->spindle_speed_out) = emcmotStatus->spindle.speed
            * emcmotStatus->net_spindle_scale;
    *(emcmot_hal_data->spindle_speed_out_rps) = emcmotStatus->spindle.nowvel;

    *(emcmot_hal_data->spindle_speed_cmd_rps) = emcmotStatus->spindle.nowvel ;

    emcmotStatus->spindle.speedrpm =  emcmotStatus->spindle.speed*60/(10000*servo_period);

    *(emcmot_hal_data->spindle_nowpos) = emcmotStatus->spindle.nowpos;
    *(emcmot_hal_data->spindle_nowvel) = emcmotStatus->spindle.nowvel;

    emcmotStatus->spindle_on1 = *(emcmot_hal_data->spindle_forward);

    *(emcmot_hal_data->move_type) = emcmotStatus->spindle.moveType;

    *(emcmot_hal_data->spindle_forward) =
            (fabs(*(emcmot_hal_data->spindle_nowvel)) > 1e-6) ? 1 : 0;
    *(emcmot_hal_data->spindle_reverse) =
            (*(emcmot_hal_data->spindle_nowvel) < 0) ? 1 : 0;
    *(emcmot_hal_data->spindle_brake) =
            (emcmotStatus->spindle.brake != 0) ? 1 : 0;

    *(emcmot_hal_data->program_line) = emcmotStatus->id;
    *(emcmot_hal_data->distance_to_go) = emcmotStatus->distance_to_go;
    if (GET_MOTION_COORD_FLAG()) {
        *(emcmot_hal_data->current_vel) = emcmotStatus->current_vel;
        *(emcmot_hal_data->requested_vel) = emcmotStatus->requested_vel;
    } else if (GET_MOTION_TELEOP_FLAG()) {
        PmCartesian t = emcmotDebug->teleop_data.currentVel.tran;
        *(emcmot_hal_data->requested_vel) = 0.0;
        emcmotStatus->current_vel = (*emcmot_hal_data->current_vel) = sqrt(
                t.x * t.x + t.y * t.y + t.z * t.z);
    } else {
        int i;
        double v2 = 0.0;


        for (i = 0; i < num_joints; i++)//ghf-2015/1/27
            if (GET_JOINT_ACTIVE_FLAG(&(joints[i])) && joints[i].free_tp_active)
                v2 += joints[i].vel_cmd * joints[i].vel_cmd;


        if (v2 > 0.0)
            emcmotStatus->current_vel = (*emcmot_hal_data->current_vel) = sqrt(
                    v2);
        else
            emcmotStatus->current_vel = (*emcmot_hal_data->current_vel) = 0.0;
        *(emcmot_hal_data->requested_vel) = 0.0;
    }

    /* These params can be used to examine any internal variable. */
    /* Change the following lines to assign the variable you want to observe
     to one of the debug parameters.  You can also comment out these lines
     and copy elsewhere if you want to observe an automatic variable that
     isn't in scope here. */
    emcmot_hal_data->debug_bit_0 = joints[1].free_tp_active;
    emcmot_hal_data->debug_bit_1 = emcmotStatus->enables_new & AF_ENABLED;
    emcmot_hal_data->debug_float_0 = emcmotStatus->net_feed_scale;
    emcmot_hal_data->debug_float_1 = emcmotStatus->spindleRevs;
    emcmot_hal_data->debug_float_2 = emcmotStatus->spindleSpeedIn;
    emcmot_hal_data->debug_float_3 = emcmotStatus->net_spindle_scale;
    emcmot_hal_data->debug_s32_0 = emcmotStatus->overrideLimitMask;
    emcmot_hal_data->debug_s32_1 = emcmotStatus->tcqlen;

    *(emcmot_hal_data->feed_scale) = emcmotStatus->feed_scale;

    /* two way handshaking for the spindle encoder */
    if (emcmotStatus->spindle_index_enable && !old_motion_index) {
        *emcmot_hal_data->spindle_index_enable = 1;
    }

    if (!*emcmot_hal_data->spindle_index_enable && old_hal_index) {
        emcmotStatus->spindle_index_enable = 0;
    }

    old_motion_index = emcmotStatus->spindle_index_enable;
    old_hal_index = *emcmot_hal_data->spindle_index_enable;

    *(emcmot_hal_data->tooloffset_x) = emcmotStatus->tool_offset.tran.x;
    *(emcmot_hal_data->tooloffset_y) = emcmotStatus->tool_offset.tran.y;
    *(emcmot_hal_data->tooloffset_z) = emcmotStatus->tool_offset.tran.z;
    *(emcmot_hal_data->tooloffset_a) = emcmotStatus->tool_offset.a;
    *(emcmot_hal_data->tooloffset_b) = emcmotStatus->tool_offset.b;
    *(emcmot_hal_data->tooloffset_c) = emcmotStatus->tool_offset.c;
    *(emcmot_hal_data->tooloffset_u) = emcmotStatus->tool_offset.u;
    *(emcmot_hal_data->tooloffset_v) = emcmotStatus->tool_offset.v;
    *(emcmot_hal_data->tooloffset_w) = emcmotStatus->tool_offset.w;

    *(emcmot_hal_data->jogScale) = emcmotStatus->jogScale;
    *(emcmot_hal_data->jog_state) = emcmotStatus->jog_state;

    if(*(emcmot_hal_data->linktoEth) > 0 || SIMULATE) {

    /* output joint info to HAL for scoping, etc */
    for (joint_num = 0; joint_num < num_joints; joint_num++) {
        /* point to joint struct */
        joint = &joints[joint_num];


        /* apply backlash and motor offset to output */
        if(joint->abszeroSt > 0) {
            return;
        }

        if(joint->useAbsPos > 0)
        joint->motor_pos_cmd = joint->pos_cmd + joint->backlash_filt
                + joint->motor_offset + joint->abszeroPos;
        else
            joint->motor_pos_cmd = joint->pos_cmd+ joint->motor_offset + joint->backlash_filt;


        //joint->motor_pos_cmd = joint->pos_cmd + joint->motor_offset + joint ->fric_out_u;//ghf-2014/7/28
        /* point to HAL data */
        joint_data = &(emcmot_hal_data->joint[joint_num]);
        /* write to HAL pins */

        //ghf-2014/12/30
        *(joint_data->fric_out_u) = joint->fric_out_u;

        *(joint_data->motor_offset) = joint->motor_offset;
        *(joint_data->motor_pos_cmd) = joint->motor_pos_cmd;

        *(joint_data->joint_limit_vel) = emcmotStatus->firvel[joint_num];//ghf-20150710

        *(joint_data->joint_pos_cmd) = joint->pos_cmd;
        *(joint_data->joint_pos_fb) = joint->pos_fb; //joint_data是hal层的，joint是motion中间层ghf-2014/2/26
        *(joint_data->amp_enable) = GET_JOINT_ENABLE_FLAG(joint);
        *(joint_data->index_enable) = joint->index_enable;
        *(joint_data->homeclearpos) = joint->homeclearpos;
        *(joint_data->homing) = GET_JOINT_HOMING_FLAG(joint);

        *(joint_data->coarse_pos_cmd) = joint->coarse_pos;
        *(joint_data->joint_vel_cmd) = joint->vel_cmd;
        *(joint_data->backlash_corr) = joint->backlash_corr;
        *(joint_data->backlash_filt) = joint->backlash_filt;
        *(joint_data->backlash_vel) = joint->backlash_vel;
        *(joint_data->f_error) = joint->ferror;
        *(joint_data->f_error_lim) = joint->ferror_limit;

        *(joint_data->free_pos_cmd) = joint->free_pos_cmd;
        *(joint_data->free_vel_lim) = joint->free_vel_lim;
        *(joint_data->free_tp_enable) = joint->free_tp_enable;
        *(joint_data->kb_jog_active) = joint->kb_jog_active;
        *(joint_data->wheel_jog_active) = joint->wheel_jog_active;

        *(joint_data->active) = GET_JOINT_ACTIVE_FLAG(joint);
        *(joint_data->in_position) = GET_JOINT_INPOS_FLAG(joint);
        *(joint_data->error) = GET_JOINT_ERROR_FLAG(joint);
        *(joint_data->phl) = GET_JOINT_PHL_FLAG(joint);
        *(joint_data->nhl) = GET_JOINT_NHL_FLAG(joint);
        *(joint_data->homed) = GET_JOINT_HOMED_FLAG(joint);
        *(joint_data->f_errored) = GET_JOINT_FERROR_FLAG(joint);
        *(joint_data->faulted) = GET_JOINT_FAULT_FLAG(joint);
        joint_data->home_state = joint->home_state;
        *(joint_data->absZeropos) = joint->abszeroPos;
    }

    }
}

static void func_19(void) {
    int joint_num, dio, aio;
    emcmot_joint_t *joint;
    emcmot_joint_status_t *joint_status;
#ifdef WATCH_FLAGS
    static int old_joint_flags[8];
    static int old_motion_flag;
#endif

    /* copy status info from private joint structure to status
     struct in shared memory */
    for (joint_num = 0; joint_num < num_joints; joint_num++) {
        /* point to joint data */
        joint = &joints[joint_num];
        /* point to joint status */
        joint_status = &(emcmotStatus->joint_status[joint_num]);
        /* copy stuff */
#ifdef WATCH_FLAGS
        /*! \todo FIXME - this is for debugging */
        if ( old_joint_flags[joint_num] != joint->flag ) {
            rtapi_print ( "(joint %d) flag %04X -> %04X\n", joint_num, old_joint_flags[joint_num], joint->flag );
            old_joint_flags[joint_num] = joint->flag;
        }
#endif
        joint_status->flag = joint->flag;
//            joint_status->pos_cmd = joint->pos_cmd;
        joint_status->pos_cmd = joint->program_cmd;
        joint_status->pos_fb = joint->pos_fb;
        joint_status->vel_cmd = joint->vel_cmd;
        joint_status->ferror = joint->ferror;
        joint_status->errorCode = joint->errorCode;
        joint_status->motor_fbpos = joint->motor_pos_fb;
        joint_status->abszeropos = joint->abszeroPos;
        joint_status->ferror_high_mark = joint->ferror_high_mark;
        joint_status->backlash = joint->backlash;
        joint_status->backlash_filt = joint->backlash_filt;//ghf-2014/7/29
        joint_status->coarse_pos = joint->coarse_pos;//ghf-2014/8/7
        joint_status->max_pos_limit = joint->max_pos_limit;
        joint_status->min_pos_limit = joint->min_pos_limit;
        joint_status->sec_limit_offset = joint->sec_limit_offset;
        joint_status->sec_pos_limit = joint->sec_pos_limit;
        joint_status->sec_neg_limit = joint->sec_neg_limit;
        joint_status->min_ferror = joint->min_ferror;
        joint_status->max_ferror = joint->max_ferror;
        joint_status->home_offset = joint->home_offset;
    }

    for (dio = 0; dio < num_dio; dio++) {
        emcmotStatus->synch_di[dio] = *(emcmot_hal_data->synch_di[dio]);
        emcmotStatus->synch_do[dio] = *(emcmot_hal_data->synch_do[dio]);
    }

    for (aio = 0; aio < num_aio; aio++) {
        emcmotStatus->analog_input[aio] = *(emcmot_hal_data->analog_input[aio]);
        emcmotStatus->analog_output[aio] =
                *(emcmot_hal_data->analog_output[aio]);
    }

    /*! \todo FIXME - the rest of this function is stuff that was apparently
     dropped in the initial move from emcmot.c to control.c.  I
     don't know how much is still needed, and how much is baggage.
     */

    /* motion emcmotDebug->queue status */
    //emcmotStatus->depth = tpQueueDepth(&emcmotDebug->queue);
    //emcmotStatus->activeDepth = tpActiveDepth(&emcmotDebug->queue);
    //emcmotStatus->id = tpGetExecId(&emcmotDebug->queue);
    //emcmotStatus->motionType = tpGetMotionType(&emcmotDebug->queue);
    //emcmotStatus->queueFull = tcqFull(&emcmotDebug->queue.queue);
    //ghf
    emcmotStatus->depth = sqGetDepth(&queue);

    emcmotStatus->activeDepth = sqGetDepth(&queue);

    emcmotStatus->id = sqGetID(&queue);
    //rtapi_print("emcmotStatus->id=%d\n",emcmotStatus->id);
    //emcmotStatus->motionType = tpGetMotionType(&emcmotDebug->queue);
    emcmotStatus->queueFull = sqIsQueueFull(&queue);

    /* check to see if we should pause in order to implement
     single emcmotDebug->stepping */
    if (emcmotDebug->stepping && emcmotDebug->idForStep != emcmotStatus->id) {
        //tpPause(&emcmotDebug->queue);
        sqPause (&queue);    //ghf
        emcmotDebug->stepping = 0;
        emcmotStatus->paused = 1;
    }
#ifdef WATCH_FLAGS
    /*! \todo FIXME - this is for debugging */
    if ( old_motion_flag != emcmotStatus->motionFlag ) {
        rtapi_print ( "Motion flag %04X -> %04X\n", old_motion_flag, emcmotStatus->motionFlag );
        old_motion_flag = emcmotStatus->motionFlag;
    }
#endif
}

double meanfilter(int joint_num, double realval) {
    double average_v1 = 0.0;
    int number=0;
    for (number=0;number<number_first_filter-1;number++){
        joints[joint_num].First_filter[number_first_filter - number -1] = joints[joint_num].First_filter[number_first_filter - number -2];
        average_v1 += joints[joint_num].First_filter[number_first_filter - number -1];
    }
    joints[joint_num].First_filter[0] = realval;
    average_v1 += joints[joint_num].First_filter[0];

    average_v1 = average_v1/number_first_filter;//ghf-2014/10/28,代表最近90次的平均值

    double average_v2 =0.0;

    for (number=0; number<number_second_filter-1; number++){
        joints[joint_num].Second_filter[number_second_filter - number -1] = joints[joint_num].Second_filter[number_second_filter - number -2];
        average_v2 += joints[joint_num].Second_filter[number_second_filter - number -1];

    }
    joints[joint_num].Second_filter[0] = average_v1;
    average_v2 += joints[joint_num].Second_filter[0];//最近10次average_v1的平均值
    return average_v2/number_second_filter;
}

double SimpleTraj(emcmot_joint_t *joint) {

    /* compute max change in velocity per servo period */
    double max_dv = joint->acc_limit * servo_period;
    /* compute a tiny position range, to be treated as zero */
    double tiny_dp = max_dv * servo_period * 0.001;

    double pos_err = joint->freepos - joint->pos_cmd;
    double vel_lim = emcmotStatus->freevel * emcmotStatus->feed_scale;
    emcmotStatus->current_vel =emcmotStatus->freevel;

    double vel_req = 0;
    /* positive and negative errors require some sign flipping to
       avoid sqrt(negative) */
    if(emcmotStatus->ManulInAutoflag > 0) {
        if (pos_err > tiny_dp) {
            vel_req =
                -max_dv + sqrt(2.0 * joint->acc_limit * pos_err + max_dv * max_dv);
        } else if (pos_err < -tiny_dp) {
            vel_req =
                max_dv - sqrt(-2.0 * joint->acc_limit * pos_err + max_dv * max_dv);

        } else {
            /* within 'tiny_dp' of desired pos, no need to move */
            vel_req = 0.0;
            joint->freepos = joint->pos_cmd;
        }
    } else {
        vel_req = 0.0;
        joint->freepos = joint->pos_cmd;
    }

    /* limit velocity request */
    if (vel_req > vel_lim) {
        vel_req = vel_lim;
    } else if (vel_req < -vel_lim) {
        vel_req = -vel_lim;
    }
    /* ramp velocity toward request at joint accel limit */
    if (vel_req > joint->vel_cmd + max_dv) {
        joint->vel_cmd += max_dv;
    } else if (vel_req < joint->vel_cmd - max_dv) {
        joint->vel_cmd -= max_dv;
    } else {
        joint->vel_cmd = vel_req;
    }
    return joint->vel_cmd;
}

double SpindleControl() {

        if(emcmotStatus->spindle.startmove == 1) {
            emcmotStatus->spindle.startcount++;
            if(emcmotStatus->spindle.startcount<50)      //主轴使能延时,伺服使能需要时间，可以在这里设置
                return;
            if(*(emcmot_hal_data->spindle_getenabled) > 0) {
                emcmotStatus->spindle.nowvel = simpleSpTraj();
                emcmotStatus->spindle.nowpos += emcmotStatus->spindle.nowvel* servo_period;
                if(fabs(emcmotStatus->spindle.nowpos - emcmotStatus->spindle.endpos) < 1e-5) {
                    emcmotStatus->spindle.inpos = 1;
                    emcmotStatus->spindle.startmove = 3;
                    emcmotStatus->spindle.moveType = 1;
                    emcmotStatus->spindle.startcount = 1;
                    emcmotStatus->spindle.recordvel = 0;
                } else {
                    emcmotStatus->spindle.inpos = 0;
                }
            }
        } else if (emcmotStatus->spindle.startmove == 2) {


            emcmotStatus->spindle.nowvel = emcmotStatus->spindle.css_factor*joints[emcmotStatus->spindle.syncjoint].vel_cmd * servo_period;

            if(fabs(emcmotStatus->spindle.nowvel) > emcmotStatus->spindle.maxvelocity) {
                reportError("主轴超速");
                sqAbort (&queue);
                *(emcmot_hal_data->spindle_enable) = 0;
                emcmotStatus->spindle.startmove = 3;
                emcmotStatus->spindle.moveType = 1;
            }

            emcmotStatus->spindle.nowpos += emcmotStatus->spindle.nowvel;
            emcmotStatus->spindle.endpos = emcmotStatus->spindle.nowpos;
            emcmotStatus->spindle.inpos = 1;
        } else if (emcmotStatus->spindle.startmove == 3) {

            emcmotStatus->spindle.nowvel = spindleMoveVel();
            emcmotStatus->spindle.nowpos += emcmotStatus->spindle.nowvel * 0.036 ;
            emcmotStatus->spindle.endpos = emcmotStatus->spindle.nowpos ;
            emcmotStatus->spindle.inpos = 1;
//            printf("now I get spindle nowvel:%.3f  nowpos:%.3f  speed:%.3f\n",emcmotStatus->spindle.nowvel,emcmotStatus->spindle.nowpos,emcmotStatus->spindle.speed);
        } else {

            emcmotStatus->spindle.inpos = 1;
        }

        if( (emcmotStatus->spindle.startcount == 0  && emcmotStatus->spindle.nowvel == 0 &&  emcmotStatus->spindle.recordvel != 0  && emcmotStatus->spindle.startmove == 3 \
                && *(emcmot_hal_data->spindle_getenabled) > 0 && emcmotStatus->motion_state == EMCMOT_MOTION_FREE )||
                (emcmotStatus->spindle.stopmove > 0 && emcmotStatus->spindle.nowvel == 0)) {        //M5
            emcmotStatus->spindle.stopmove = 0;
            printf("nwo I in end enabled ---- \n");
            emcmotStatus->spindle.stopcount = 50;
            emcmotStatus->spindle.startcount = 0;
        }
        emcmotStatus->spindle.recordvel = emcmotStatus->spindle.nowvel;

        if(emcmotStatus->spindle.stopcount > 0) {
            emcmotStatus->spindle.stopcount--;
            if(emcmotStatus->spindle.stopcount == 1 && emcmotStatus->spindle.startmove == 3 && emcmotStatus->spindle.nowvel == 0) { //保证是需要停才停
                *(emcmot_hal_data->spindle_enable) = 0;
            }
        }
        emcmotStatus->synch_di[0] = (emcmotStatus->spindle.nowvel == 0);
    static double recordpos;
    if(recordpos != emcmotStatus->spindle.nowpos) {
//        printf("now I get spindle pos:%.3f  %.3f  %.3f  double:%ld int:%ld\n",emcmotStatus->spindle.nowvel,emcmotStatus->spindle.endpos,emcmotStatus->spindle.nowpos,sizeof(double),emcmotStatus->spindle.startmove);
        recordpos = emcmotStatus->spindle.nowpos;
    }

    return emcmotStatus->spindle.startmove;
}

double spindleMoveVel() {
//    emcmotStatus->spindle.acclimit = 200;
//    emcmotStatus->spindle.maxvelocity = 1000;

    if(*(emcmot_hal_data->spindle_getenabled) == 0 && !SIMULATE) {
        return 0;
    }
    double max_dv = emcmotStatus->spindle.acclimit * servo_period;

    double vel_lim =  emcmotStatus->spindle.maxvelocity * emcmotStatus->net_spindle_scale;
    double speed = emcmotStatus->spindle.speed * emcmotStatus->net_spindle_scale;
    if(speed > emcmotStatus->spindle.nowvel ) {
        emcmotStatus->spindle.nowvel += max_dv;
        if (emcmotStatus->spindle.nowvel > speed) {
            emcmotStatus->spindle.nowvel = speed;
        }
        if (emcmotStatus->spindle.nowvel > vel_lim) {
            emcmotStatus->spindle.nowvel = vel_lim;
        }
    } else {
        emcmotStatus->spindle.nowvel -= max_dv;
        if (emcmotStatus->spindle.nowvel < speed) {
            emcmotStatus->spindle.nowvel = speed;
        }
        if (emcmotStatus->spindle.nowvel < -vel_lim) {
            emcmotStatus->spindle.nowvel = -vel_lim;
        }
    }

    return emcmotStatus->spindle.nowvel;
}



double simpleSpTraj()
{
//    emcmotStatus->spindle.acclimit = 200;
//    emcmotStatus->spindle.maxvelocity = 1000;
    double max_dv = emcmotStatus->spindle.acclimit * servo_period;
    double tiny_dp = max_dv * servo_period * 0.001;

    double pos_err = 0;
    pos_err = emcmotStatus->spindle.endpos - emcmotStatus->spindle.nowpos;


    double vel_lim =  emcmotStatus->spindle.maxvelocity*2 * emcmotStatus->net_spindle_scale;        //嫌速度太慢，定位速度加快两倍

    double vel_req = 0;
    if (emcmotStatus->spindle.startmove ==  1) {
        if (pos_err > tiny_dp) {
            vel_req = -max_dv + sqrt(2.0 * emcmotStatus->spindle.acclimit * pos_err + max_dv * max_dv);
        } else if (pos_err < -tiny_dp) {
            vel_req = max_dv - sqrt(-2.0 * emcmotStatus->spindle.acclimit * pos_err + max_dv * max_dv);

        } else {
            vel_req = 0.0;
            emcmotStatus->spindle.endpos = emcmotStatus->spindle.nowpos;
        }
    } else {
        vel_req = 0.0;
        emcmotStatus->spindle.endpos = emcmotStatus->spindle.nowpos;
    }

//    printf("now pos_err:%.3f nowpos:%.3f  endpos:%.3f vel_req = %.3f maxdv:%.3f %.3f\n",pos_err,emcmotStatus->spindle.nowpos,emcmotStatus->spindle.nowvel,vel_req,max_dv,2.0 * emcmotStatus->spindle.acclimit * pos_err);

    if (vel_req > vel_lim) {
        vel_req = vel_lim;
    } else if (vel_req < -vel_lim) {
        vel_req = -vel_lim;
    }
    if (vel_req > emcmotStatus->spindle.nowvel + max_dv) {
        emcmotStatus->spindle.nowvel += max_dv;
    } else if (vel_req < emcmotStatus->spindle.nowvel - max_dv) {
        emcmotStatus->spindle.nowvel -= max_dv;
    } else {
        emcmotStatus->spindle.nowvel = vel_req;
    }
    return emcmotStatus->spindle.nowvel ;
}
