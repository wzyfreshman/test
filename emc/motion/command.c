/********************************************************************
 * Description: command.c
 *   emcmotCommandhandler() takes commands passed from user space and
 *   performs various functions based on the value in emcmotCommand->command.
 *   For the full list, see the EMCMOT_COMMAND enum in motion.h
 *
 * pc says:
 *
 *   Most of the configs would be better off being passed via an ioctl
 *   implimentation leaving pure realtime data to be handled by
 *   emcmotCommmandHandler() - This would provide a small performance
 *   increase on slower systems.
 *
 * jmk says:
 *
 *   Using commands to set config parameters is "undesireable", because
 *   of the large amount of code needed for each parameter.  Today you
 *   need to do the following to add a single new parameter called foo:
 *
 *   1)  Add a member 'foo' to the config or joint structure in motion.h
 *   2)  Add a command 'EMCMOT_SET_FOO" to the cmd_code_t enum in motion.h
 *   3)  Add a field to the command_t struct for the value used by
 *       the set command (if there isn't already one that can be used.)
 *   4)  Add a case to the giant switch statement in command.c to
 *       handle the 'EMCMOT_SET_FOO' command.
 *   5)  Write a function emcSetFoo() in taskintf.cc to issue the command.
 *   6)  Add a prototype for emcSetFoo() to emc.hh
 *   7)  Add code to iniaxis.cc (or one of the other inixxx.cc files) to
 *       get the value from the ini file and call emcSetFoo().  (Note
 *       that each parameter has about 16 lines of code, but the code
 *       is identical except for variable/parameter names.)
 *   8)  Add more code to iniaxis.cc to write the new value back out
 *       to the ini file.
 *   After all that, you have the abililty to get a number from the
 *   ini file to a structure in shared memory where the motion controller
 *   can actually use it.  However, if you want to manipulate that number
 *   using NML, you have to do more:
 *   9)  Add a #define EMC_SET_FOO_TYPE to emc.hh
 *   10) Add a class definition for EMC_SET_FOO to emc.hh
 *   11) Add a case to a giant switch statement in emctaskmain.cc to
 *       call emcSetFoo() when the NML command is received.  (Actually
 *       there are about 6 switch statements that need at least a
 *       case label added.
 *   12) Add cases to two giant switch statements in emc.cc, associated
 *       with looking up and formating the command.
 *
 *
 *   Derived from a work by Fred Proctor & Will Shackleford
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

//ghf-2015/1/17
//#include <linux/timer.h>
//#include <linux/timex.h>
#include <linux/random.h>

#include <linux/types.h>
#include <float.h>
#include "posemath.h"
#include "rtapi.h"
#include "hal.h"
#include "motion.h"
#include "motion_debug.h"
#include "motion_struct.h"
#include "emcmotglb.h"
#include "mot_priv.h"
#include "rtapi_math.h"
#include "motion_types.h"
#include "des.h"

//#include <asm-generic/rtc.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Mark strings for translation, but defer translation to userspace
#define _(s) (s)


#ifndef hypot
#define hypot(a,b) (sqrt((a)*(a)+(b)*(b)))
#endif

#define Min(a,b) (((a)<(b))?(a):(b))

#define TINY 1e-12              /* for arc_data_r */

#define M_PI_2l	1.5707963267948966192313216916397514L
#define M_PIl		3.1415926535897932384626433832795029L  /* pi */

/* debugging functions */
extern void print_pose(EmcPose *pos);
extern void check_stuff(const char *msg);

/* kinematics flags */
KINEMATICS_FORWARD_FLAGS fflags = 0;
KINEMATICS_INVERSE_FLAGS iflags = 0;

#define mmygetidtonum(void) do { \
    int a=0;\
    unsigned char xor=0x55,add=0;\
    unsigned short temp;\
    for(a=0;a<8;a++) {\
        xor ^= emcmotStatus->encrychipid[a];\
        add += emcmotStatus->encrychipid[a];\
    }\
    temp = (add << 8) + xor;\
} while(0);

//final是fpga读上来的密文密钥
void func_17(unsigned char final[64]) {

    unsigned char cpu[8];//存放密文密钥中的cpuid密文(fpga串号)
    unsigned char hd[8];//存放密文密钥中的硬盘id密文
    unsigned char chip[8];//存放密文密钥中的加密芯片id密文
    unsigned char key[16];////存放密文密钥中的密钥
    des3_context ctx;//用于解密用的密钥结构体
    int i;
    static unsigned char cpuidl[8];//解密后的cpuid
    static unsigned char hdid[8];//解密后的hdid
    static unsigned char chipid[8];//解密后的加密芯片id

    rtapi_print_msg(RTAPI_MSG_DBG,
            "MOTION: get into pos cmd\n");

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
    hal_ppmc_7(&ctx,hd,hdid);
    hal_ppmc_7(&ctx,chip,chipid);

    for( i = 0;i<8;i++) {
        //rtapi_print("chipid[%d]=%02x  %02X\n",i,chipid[i],emcmotStatus->encrychipid[i]);
        mmygetidtonum();
    }

    for( i = 0;i<8;i++) {
        if(emcmotStatus->encrychipid[i] != chipid[i]){
            emcmotStatus->rencryflag = 1; //decrypt error
            break;
        }
        else {
        }
    }
    rtapi_print_msg(RTAPI_MSG_DBG,
            "MOTION: get out pos cmd\n");
}


/* loops through the active joints and checks if any are not homed */
int func_11(void) {
    int joint_num;
    emcmot_joint_t *joint;

    /* bail out if the allHomed flag is already set */

    if (0 != emcmotDebug) {
        if (emcmotDebug->allHomed){
            return 1;
        }
    }

    for (joint_num = 0; joint_num < num_joints; joint_num++) {
        /* point to joint data */
        joint = &joints[joint_num];
        if (!GET_JOINT_ACTIVE_FLAG(joint)) {
        }
        if (!GET_JOINT_HOMED_FLAG(joint)) {
            /* if any of the joints is not homed return false */
            return 0;
        }
    }
    if (0 != emcmotDebug) {
        emcmotDebug->allHomed = 1;
    }
    /* return true if all active joints are homed*/
    return 1;
}

/* limits_ok() returns 1 if none of the hard limits are set,
 0 if any are set. Called on a linear and circular move. */
static int limits_ok(void) {
    int joint_num;
    emcmot_joint_t *joint;

    for (joint_num = 0; joint_num < num_joints; joint_num++) {
        /* point to joint data */
        joint = &joints[joint_num];
        if (!GET_JOINT_ACTIVE_FLAG(joint)) {
            /* if joint is not active, don't even look at its limits */
            continue;
        }

        if (GET_JOINT_PHL_FLAG(joint) || GET_JOINT_NHL_FLAG(joint)) {
            return 0;
        }
    }

    return 1;
}

/* check the value of the joint and velocity against current position,
 returning 1 (okay) if the request is to jog off the limit, 0 (bad)
 if the request is to jog further past a limit. */
static double oldvel=0;//ghf-2015-2-26
static int jog_ok(int joint_num, double vel) {
    emcmot_joint_t *joint;
    int neg_limit_override, pos_limit_override;

    /* point to joint data */
    joint = &joints[joint_num];
    /* are any limits for this joint overridden? */
    neg_limit_override = emcmotStatus->overrideLimitMask
            & (1 << (joint_num * 2));
    pos_limit_override = emcmotStatus->overrideLimitMask
            & (2 << (joint_num * 2));
    if (neg_limit_override && pos_limit_override) {
        /* both limits have been overridden at the same time.  This
         happens only when they both share an input, but means it
         is impossible to know which direction is safe to move.  So
         we skip the following tests... */
        return 1;
    }
    if (joint_num < 0 || joint_num >= num_joints) {
        reportError(_("Can't jog invalid joint number %d."), joint_num);
        return 0;
    }


    if(GET_JOINT_PHL_FLAG(joint)) {
        if(emcmotStatus->firvel[joint_num]*vel>0) {
            reportError(_("Can't jog (joint %d) further past max hard limit."),
                    joint_num);
            rtapi_print("curvel=%d\n",(int)(vel*1000));
            return 0;
        } else if(emcmotStatus->firvel[joint_num]==0){
            if(vel>0){
                reportError(_("Can't jog (joint %d) further past max hard limit."),
                        joint_num);
                rtapi_print("curvel=%d\n",(int)(vel*1000));
                return 0;
            }
        }
    }
    else if(GET_JOINT_NHL_FLAG(joint)) {
        if(emcmotStatus->firvel[joint_num]*vel>0) {
            reportError(_("Can't jog (joint %d) further past min hard limit."),
                    joint_num);
            return 0;
        }
        else if(emcmotStatus->firvel[joint_num]==0){
            if(vel<0){
                reportError(_("Can't jog (joint %d) further past min hard limit."),
                        joint_num);
                rtapi_print("curvel=%d\n",(int)(vel*1000));
                return 0;
            }
        }
    } else if(emcmotStatus->probe_alarm > 0 && joint_num > 1 && vel < 0) {
            reportError(_("对刀仪报警不能移动"),
                    joint_num);
            return 0;
    }

    refresh_jog_limits(joint); //set limit again to ensure homed ok,use this function,if not homed, limits is unused, if homed, position limit is ok;

    if (vel > 0.0 && (joint->pos_cmd > joint->max_jog_limit + 0.5)) {//ghf-2014/10/28
        reportError(_("Can't jog (joint %d) further past max soft limit."),
                joint_num);
        return 0;
    }
    if (vel < 0.0 && (joint->pos_cmd < joint->min_jog_limit - 0.5)) {//ghf-2014/10/28
        reportError(_("Can't jog (joint %d) further past min soft limit."),
                joint_num);
        return 0;
    }

    oldvel = vel;

    /* okay to jog */
    return 1;
}

/* Jogs limits change, based on whether the machine is homed or
 or not.  If not homed, the limits are relative to the current
 position by +/- the full range of travel.  Once homed, they
 are absolute.
 */
void func_12(emcmot_joint_t *joint) {
    double range;

    if (GET_JOINT_HOMED_FLAG(joint)) {
        /* if homed, set jog limits using soft limits */

        /* now use the second Y axis, so reset the softlimit */
//        if(num_joints > 6) {
//            joints[1].max_pos_limit =
//                    joints[1].max_pos_limit > joints[6].max_pos_limit?joints[6].max_pos_limit:joints[1].max_pos_limit;
//           joints[1].min_pos_limit =
//                    joints[1].min_pos_limit > joints[6].min_pos_limit?joints[1].min_pos_limit:joints[6].min_pos_limit;
//        }

        joint->max_jog_limit = joint->max_pos_limit;
        joint->min_jog_limit = joint->min_pos_limit;
    } else {
        /* not homed, set limits based on current position */
        range = joint->max_pos_limit - joint->min_pos_limit;
        joint->max_jog_limit = joint->pos_fb + range;
        joint->min_jog_limit = joint->pos_fb - range;
    }
}

/* inRange() returns non-zero if the position lies within the joint
 limits, or 0 if not.  It also reports an error for each joint limit
 violation.  It's possible to get more than one violation per move. */
static int inRange(EmcPose pos, int id, char *move_type) {
    double joint_pos[EMCMOT_MAX_JOINTS];
    int joint_num;
    emcmot_joint_t *joint;
    int in_range = 1;


    if(!SIMULATE && emcmotStatus->dateerror < 0 ) {
            reportError("注册时间已到!");
            return 0;
    }
    /* fill in all joints with 0 */
    for (joint_num = 0; joint_num < num_joints; joint_num++) {
        joint_pos[joint_num] = 0.0;
    }

    /* now fill in with real values, for joints that are used */
    kinematicsInverse(&pos, joint_pos, &iflags, &fflags);

    for (joint_num = 0; joint_num < 6; joint_num++) {
        /* point to joint data */
        joint = &joints[joint_num];



        if (!GET_JOINT_ACTIVE_FLAG(joint)) {
            /* if joint is not active, don't even look at its limits */
            continue;
        }

        //printf("joint_pos[%d]=%f joint->max_pos_limit=%f, x=%f y=%f z=%f id=%d\n",
         //joint_num,joint_pos[joint_num],joint->max_pos_limit,pos.tran.x,pos.tran.y,pos.tran.z,id);

        if (joint_pos[joint_num] > joint->max_pos_limit + 0.5) {
            printf("pos--now I get axis %d  pos:%f  lim:%f\n",joint_num,joint_pos[joint_num] , joint->max_pos_limit );

            in_range = 0;
            if (id > 0)
                reportError(_("%s move on line %d would exceed joint %d's positive limit"),
                        move_type, id, joint_num);
            else
                reportError(_("%s move in MDI would exceed joint %d's positive limit"),
                        move_type, joint_num);
            sqClearQueue(&queue);
        }

        if (joint_pos[joint_num] < joint->min_pos_limit - 0.5) {
             printf("neg--now I get axis %d  pos:%f  lim:%f\n",joint_num,joint_pos[joint_num] , joint->max_pos_limit );
            in_range = 0;
            if (id > 0)
                reportError(_("%s move on line %d would exceed joint %d's negative limit"),
                        move_type, id, joint_num);
            else
                reportError(_("%s move in MDI would exceed joint %d's negative limit"),
                        move_type, joint_num);
            sqClearQueue(&queue);
        }
    }
    return in_range;
}

/* clearHomes() will clear the homed flags for joints that have moved
 since homing, outside coordinated control, for machines with no
 forward kinematics. This is used in conjunction with the rehomeAll
 flag, which is set for any coordinated move that in general will
 result in all joints moving. The flag is consulted whenever a joint
 is jogged in joint mode, so that either its flag can be cleared if
 no other joints have moved, or all have to be cleared. */
void func_13(int joint_num) {
    int n;
    emcmot_joint_t *joint;

    if (kinType == KINEMATICS_INVERSE_ONLY) {
        if (rehomeAll) {
            for (n = 0; n < num_joints; n++) {
                /* point at joint data */
                joint = &(joints[n]);
                /* clear flag */
                SET_JOINT_HOMED_FLAG(joint, 0);
            }
        } else {
            /* point at joint data */
            joint = &joints[joint_num];
            /* clear flag */
            SET_JOINT_HOMED_FLAG(joint, 0);
        }
    }
    if (0 != emcmotDebug) {
        emcmotDebug->allHomed = 0;
    }
}

void func_16(int axis, int unlock) {
    *(emcmot_hal_data->joint[axis].unlock) = unlock;
}

int emcmotGetRotaryIsUnlocked(int axis) {
    return *(emcmot_hal_data->joint[axis].is_unlocked);
}

/*! \function emcmotDioWrite()

 sets or clears a HAL DIO pin,
 pins get exported at runtime

 index is valid from 0 to num_dio <= EMCMOT_MAX_DIO, defined in emcmotcfg.h

 */
void func_14(int index, char value) {
    if ((index >= num_dio) || (index < 0)) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "ERROR: index out of range, %d not in [0..%d] (increase num_dio/EMCMOT_MAX_DIO=%d)\n",
                index, num_dio, EMCMOT_MAX_DIO);
    } else {
        if (value != 0) {
            *(emcmot_hal_data->synch_do[index]) = 1;
        } else {
            *(emcmot_hal_data->synch_do[index]) = 0;
        }
    }
}

/*! \function emcmotAioWrite()

 sets or clears a HAL AIO pin,
 pins get exported at runtime

 \todo Implement function, it doesn't do anything right now
 RS274NGC doesn't support it now, only defined/used in emccanon.cc

 */
void func_15(int index, double value) {
    if ((index >= num_aio) || (index < 0)) {
        rtapi_print_msg(RTAPI_MSG_ERR,
                "ERROR: index out of range, %d not in [0..%d] (increase num_aio/EMCMOT_MAX_AIO=%d)\n",
                index, num_aio, EMCMOT_MAX_AIO);
    } else {
        *(emcmot_hal_data->analog_output[index]) = value;
    }
}

static int is_feed_type(int motion_type) {
    switch (motion_type) {
    case EMC_MOTION_TYPE_ARC:
    case EMC_MOTION_TYPE_FEED:
    case EMC_MOTION_TYPE_PROBING:
        return 1;
    default:
        rtapi_print_msg(RTAPI_MSG_ERR,
                "Internal error: unhandled motion type %d\n", motion_type);
    case EMC_MOTION_TYPE_TOOLCHANGE:
    case EMC_MOTION_TYPE_TRAVERSE:
    case EMC_MOTION_TYPE_INDEXROTARY:
        return 0;
    }
}


//ghf-2014/11/3
int emcGetCenterPoint(EmcPose *startpos,EmcPose *endpos,EmcPose *point2,EmcPose *point3,double r) {

    double x0=0,y0=0,x1=0,y1=0,k=0;
    double x2=0,y2=0,x3=0,y3=0;

    x0 = startpos->tran.x;
    y0 = startpos->tran.y;
    x1 = endpos->tran.x;
    y1 = endpos->tran.y;
    k = (y1-y0)/(x1-x0);

    if((k>0)&&(k<1)) {
        if(x0<x1){
            y3 = y1;
            y2 = y3 - tan(M_PIl/8)*r*1.414213562/2;
            x2 = x0 + (y2-y0);
            x3 = x0 + (y3-y0) + tan(M_PIl/8)*r;
            if(x3>x1){
                x3=x1;
            }
        }
        else{
            y3 = y1;
            y2 = y3 + tan(M_PIl/8)*r*1.414213562/2;
            x2 = x0 + (y2-y0);
            x3 = x0 + (y3-y0) - tan(M_PIl/8)*r;
            if(x3<x1){
                x3=x1;
            }
        }
    }
    else if(k>1) {
        if(x0<x1){
            x2 = x1 - tan(M_PIl/8)*r*1.414213562/2;
            y2 = y0 + x2 - x0;
            x3 = x1;
            y3 = y0 + (x3-x0) + tan(M_PIl/8)*r;
            if(y3>y1){
                y3=y1;
            }
        }
        else{
            x2 = x1 + tan(M_PIl/8)*r*1.414213562/2;
            y2 = y0 + x2 - x0;
            x3 = x1;
            y3 = y0 + (x3-x0) - tan(M_PIl/8)*r;
            if(y3<y1){
                y3=y1;
            }
        }
    }
    else if(k<-1) {
        if(x0>x1){
            x3 = x1;
            x2 = x3 + tan(M_PIl/8)*r*1.414213562/2;
            y2 = y0 - x2 + x0;
            y3 = y0 + (x0-x3) + tan(M_PIl/8)*r;
            if(y3>y1){
                y3=y1;
            }
        }
        else{
            x3 = x1;
            x2 = x3 - tan(M_PIl/8)*r*1.414213562/2;
            y2 = y0 - x2 + x0;
            y3 = y0 + (x0-x3) - tan(M_PIl/8)*r;
            if(y3<y1){
                y3=y1;
            }
        }
    }
    else if((k<0)&&(k>-1)) {
        if(x0>x1){
            y3 = y1;
            y2 = y3 - tan(M_PIl/8)*r*1.414213562/2;
            x2 = x0 - y2 + y0;
            x3 = x0 - (y3-y0) - tan(M_PIl/8)*r;
            if(x3<x1){
                x3=x1;
            }
        }
        else {
            y3 = y1;
            y2 = y3 + tan(M_PIl/8)*r*1.414213562/2;
            x2 = x0 - y2 + y0;
            x3 = x0 - (y3-y0) + tan(M_PIl/8)*r;
            if(x3>x1){
                x3=x1;
            }
        }
    }


    point2->tran.x = x2;
    point2->tran.y = y2;
    point2->tran.z = endpos->tran.z;

    point3->tran.x = x3;
    point3->tran.y = y3;
    point3->tran.z = endpos->tran.z;

    return 0;
}

//g2 move=0 g3:move=1
int arc_data_r(int move,
                       double current_x, //!< first coordinate of current point
                      double current_y, //!< second coordinate of current point
                      double end_x,     //!< first coordinate of arc end point
                      double end_y,     //!< second coordinate of arc end point
                      double radius,    //!< radius of arc
                      double *center_x, //!< pointer to first coordinate of center of arc
                      double *center_y, //!< pointer to second coordinate of center of arc
                      int *turn)        //!< pointer to number of full or partial circles CCW
{
  double abs_radius;            /* absolute value of given radius */
  double half_length;           /* distance from M to end point   */
  double mid_x;                 /* first coordinate of M          */
  double mid_y;                 /* second coordinate of M         */
  double offset;                /* distance from M to center      */
  double theta;                 /* angle of line from M to center */
  double turn2;                 /* absolute value of half of turn */

  abs_radius = fabs(radius);
  mid_x = (end_x + current_x) / 2.0;
  mid_y = (end_y + current_y) / 2.0;
  half_length = hypot((mid_x - end_x), (mid_y - end_y));

  //rtapi_print("mid_x=%d,mid_y=%d,half_length=%d\n",(int)(mid_x*1000),(int)(mid_y*1000),(int)(half_length*1000));

  if ((half_length / abs_radius) > (1 - TINY))
    half_length = abs_radius;   /* allow a small error for semicircle */
  /* check needed before calling asin   */
  if (((move == 0) && (radius > 0)) || ((move == 1) && (radius < 0)))
    theta = atan2((end_y - current_y), (end_x - current_x)) - M_PI_2l;
  else
    theta = atan2((end_y - current_y), (end_x - current_x)) + M_PI_2l;

  turn2 = asin(half_length / abs_radius);
  offset = abs_radius * cos(turn2);
  *center_x = mid_x + (offset * cos(theta));
  *center_y = mid_y + (offset * sin(theta));
  *turn = (move == 0) ? -1  : 0 ;
 return 0;
}


//ghf-2014/6/4
void sqpreprocsegController(void *arg, long period) {
    char issue_atspeed = 0;

    //static int m50 = 1;

    static double r = 40;

    static double center_x=0,center_y=0;
    static int turn=0;
    static PmCartesian center;	/* center for circle */
    static PmCartesian normal;	/* normal vec for circle */

    double length=0;

    static EmcPose startpos,endpos,point2,point3;

    double k=0,move=0;

    static int flag = 1;
    if(flag==1) {
        memset(&startpos,0,sizeof(startpos));
        memset(&endpos,0,sizeof(endpos));
        memset(&point2,0,sizeof(point2));
        memset(&point3,0,sizeof(point3));
        flag = 0;
    }

    check_stuff("before sqpreprocsegController()");

    if (emcmotCommand->head != emcmotCommand->tail) {
        emcmotDebug->split++;
        return; /* not really an error */
    }
    if (emcmotCommand->commandNum != emcmotStatus->commandNumEcho) {

        if ((emcmotCommand->command != EMCMOT_SET_LINE)
                && (emcmotCommand->command != EMCMOT_SET_CIRCLE)
                ) {
            return;
        }
        /* increment head count-- we'll be modifying emcmotStatus */
        emcmotStatus->head++;
        emcmotDebug->head++;

        /* got a new command-- echo command and number... */
        emcmotStatus->commandEcho = emcmotCommand->command;
        emcmotStatus->commandNumEcho = emcmotCommand->commandNum;

        /* clear status value by default */
        emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_COMMAND;//ghf-2014/10/20

        if(sqGetDepth(&queue) == 0) {
            sqGetPosition(&queue,&startpos);
        }

        switch (emcmotCommand->command) {


        case EMCMOT_SET_LINE:            
            /* emcmotDebug->queue up a linear move */
            /* requires coordinated mode, enable off, not on limits */


            //rtapi_print_msg(RTAPI_MSG_ERR,"SET_LINE %d\n",emcmotCommand->id);

            if (!GET_MOTION_COORD_FLAG() || !GET_MOTION_ENABLE_FLAG() ||
                    (emcmotStatus->rencryflag > 0 && !SIMULATE) ) {
                reportError(
                        _("need to be enabled, in coord mode for linear move"));
                emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_COMMAND;
                SET_MOTION_ERROR_FLAG(1);
                break;
            } else if (!inRange(emcmotCommand->pos, emcmotCommand->id,
                    "Linear")) {
                emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_PARAMS;
                //tpAbort(&emcmotDebug->queue);
                sqAbort (&queue); //ghf
                SET_MOTION_ERROR_FLAG(1);
                break;
            } else if (!limits_ok()) {
                reportError(_("can't do linear move with limits exceeded"));
                emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_PARAMS;
                //tpAbort(&emcmotDebug->queue);
                sqAbort (&queue); //ghf
                SET_MOTION_ERROR_FLAG(1);
                break;
            }
            if (emcmotStatus->atspeed_next_feed
                    && is_feed_type(emcmotCommand->motion_type)) {
                issue_atspeed = 1;
                emcmotStatus->atspeed_next_feed = 0;
            }
            if (!is_feed_type(emcmotCommand->motion_type)
                    && emcmotStatus->spindle.css_factor) {
                emcmotStatus->atspeed_next_feed = 1;
            }

            memcpy(&endpos,&(emcmotCommand->pos),sizeof(endpos));

            r = Min(fabs(startpos.tran.x-endpos.tran.x),fabs(startpos.tran.y-endpos.tran.y));
            if(r > 80){
                r = 80;
            }

            if(fabs(startpos.tran.x-endpos.tran.x) > 0.0001)
                k = (startpos.tran.y-endpos.tran.y)/(startpos.tran.x-endpos.tran.x);
            else
                k = 1;

            length = hypot((startpos.tran.x-endpos.tran.x), (startpos.tran.y-endpos.tran.y));

            if((emcmotStatus->g0enableflag==1)&&(emcmotCommand->gmodetype==0)&&(fabs(fabs(k)-1)>0.05)
                    &&(length>20)&&(fabs(startpos.tran.x-endpos.tran.x)>1e-6)&&(fabs(startpos.tran.y-endpos.tran.y)>1e-6)) {
                sqSetFeed(&queue, 500*emcmotStatus->coeff3);    

                emcGetCenterPoint(&startpos,&endpos,&point2,&point3,r);



                if((k>0)&&(k<1)) {
                    move = 0;
                }
                else if((k<0)&&(k>-1)) {
                    move = 1;
                }
                else if(k>1) {
                    move = 1;
                }
                else if(k<-1) {
                    move = 0;
                }

                arc_data_r(move,point2.tran.x,point2.tran.y,point3.tran.x,point3.tran.y,
                                      r,&center_x,&center_y,&turn);

                center.x = center_x;
                center.y = center_y;
                center.z = emcmotCommand->pos.tran.z;

                normal.x = 0;
                normal.y = 0;
                normal.z = 1;

                if (-1
                        == sqAddLine(&queue, point2, emcmotCommand->id,
                                0, &emcmotCommand->dynamicparameter[0])
                        ) {
                    reportError(_("can't add linear move"));
                    emcmotStatus->commandStatus = EMCMOT_COMMAND_BAD_EXEC;
                    sqAbort (&queue);
                    SET_MOTION_ERROR_FLAG(1);
                    break;
                } else {
                    SET_MOTION_ERROR_FLAG(0);
                    rehomeAll = 1;
                }

                if (-1
                        == sqAddCircle(&queue, point3,
                                center, normal, turn, emcmotCommand->id, 0,
                                       &emcmotCommand->dynamicparameter[0])) {
                    reportError(_("can't add circular move"));
                    emcmotStatus->commandStatus = EMCMOT_COMMAND_BAD_EXEC;
                    sqAbort (&queue);            //ghf
                    SET_MOTION_ERROR_FLAG(1);
                    break;
                } else {
                    SET_MOTION_ERROR_FLAG(0);
                    rehomeAll = 1;
                }

                if (-1
                        == sqAddLine(&queue, endpos, emcmotCommand->id,
                                0,&emcmotCommand->dynamicparameter[0])) {
                    reportError(_("can't add linear move"));
                    emcmotStatus->commandStatus = EMCMOT_COMMAND_BAD_EXEC;
                    sqAbort (&queue);
                    SET_MOTION_ERROR_FLAG(1);
                    break;
                } else {
                    SET_MOTION_ERROR_FLAG(0);
                    rehomeAll = 1;
                }
            }
            else {
                /* append it to the emcmotDebug->queue */
                sqSetFeed(&queue, emcmotCommand->vel);


                if (-1
                        == sqAddLine(&queue, emcmotCommand->pos, emcmotCommand->id,
                                emcmotCommand->gmodetype,
                                     &emcmotCommand->dynamicparameter[0])) {         //ghf-2014/3/22
                    reportError(_("can't add linear move"));
                    emcmotStatus->commandStatus = EMCMOT_COMMAND_BAD_EXEC;
                    //tpAbort(&emcmotDebug->queue);
                    sqAbort (&queue);
                    SET_MOTION_ERROR_FLAG(1);
                    break;
                } else {
                    SET_MOTION_ERROR_FLAG(0);
                    /* set flag that indicates all joints need rehoming, if any
                     joint is moved in joint mode, for machines with no forward
                     kins */
                    rehomeAll = 1;
                }
            }

            emcmotStatus->commandStatus = EMCMOT_COMMAND_LINECYCLEOK;//ghf-2014/10/20

            break;

        case EMCMOT_SET_CIRCLE:
            /* emcmotDebug->queue up a circular move */
            /* requires coordinated mode, enable on, not on limits */
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_CIRCLE");
            if (!GET_MOTION_COORD_FLAG() || !GET_MOTION_ENABLE_FLAG()) {
                reportError(
                        _(
                                "need to be enabled, in coord mode for circular move"));
                emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_COMMAND;
                SET_MOTION_ERROR_FLAG(1);
                break;
            } else if (!inRange(emcmotCommand->pos, emcmotCommand->id,
                    "Circular")) {
                emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_PARAMS;
                //tpAbort(&emcmotDebug->queue);
                sqAbort (&queue);            //ghf
                SET_MOTION_ERROR_FLAG(1);
                break;
            } else if (!limits_ok()) {
                reportError(_("can't do circular move with limits exceeded"));
                emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_PARAMS;
                //tpAbort(&emcmotDebug->queue);
                sqAbort (&queue);            //ghf
                SET_MOTION_ERROR_FLAG(1);
                break;
            }
            if (emcmotStatus->atspeed_next_feed) {
                issue_atspeed = 1;
                emcmotStatus->atspeed_next_feed = 0;
            }


            PmCartesian v1,v2;
            v1.x = startpos.tran.x - emcmotCommand->center.x;
            v1.y = startpos.tran.y - emcmotCommand->center.y;
            v1.z = startpos.tran.z - emcmotCommand->center.z;
            v2.x = emcmotCommand->pos.tran.x - emcmotCommand->center.x;
            v2.y = emcmotCommand->pos.tran.y - emcmotCommand->center.y;
            v2.z = emcmotCommand->pos.tran.z - emcmotCommand->center.z;

            double radius0 = sqrt(v1.x*v1.x + v1.y*v1.y);
            double radius1 = sqrt(v2.x*v2.x + v2.y*v2.y);
            if(fabs(v1.z - v2.z) < 1e-6 && fabs(radius0 - radius1) > 1e-4) {      //z相等时，当圆心在起点和末点的一侧则认为该圆弧无效
                v1.z = 0;
                v2.z = 0;
                double dot;
                pmCartCartDot(v1,v2, &dot);
                if(radius0 > 0 && radius1 > 0) {
                    double dt = dot / (radius0 * radius1);
                       if(dt >= 1) {
                           printf("忽略线段: dt = %.12f  radius:%f %f  dot:%.12f   v1:%f %f  v2:%f %f\n",dt,radius0,radius1,dot,v1.x,v1.y,v2.x,v2.y);
                           emcmotStatus->commandStatus = EMCMOT_COMMAND_LINECYCLEOK;//ghf-2014/10/20
                        break;
                    }
                }
            }

            sqSetFeed(&queue, emcmotCommand->vel);
           // printf("add circlr/n");
            if (-1
                    == sqAddCircle(&queue, emcmotCommand->pos,
                            emcmotCommand->center, emcmotCommand->normal,
                            emcmotCommand->turn, emcmotCommand->id, 1,
                                   &emcmotCommand->dynamicparameter[0])) {
                reportError(_("can't add circular move"));
                emcmotStatus->commandStatus = EMCMOT_COMMAND_BAD_EXEC;
                //tpAbort(&emcmotDebug->queue);
                sqAbort (&queue);            //ghf
                SET_MOTION_ERROR_FLAG(1);
                break;
            } else {
                SET_MOTION_ERROR_FLAG(0);
                /* set flag that indicates all joints need rehoming, if any
                 joint is moved in joint mode, for machines with no forward
                 kins */
                rehomeAll = 1;
            }
            emcmotStatus->commandStatus = EMCMOT_COMMAND_LINECYCLEOK;//ghf-2014/10/20
            break;

        default:
            rtapi_print_msg(RTAPI_MSG_DBG, "UNKNOWN");
            reportError(_("unrecognized command %d"), emcmotCommand->command);
            emcmotStatus->commandStatus = EMCMOT_COMMAND_UNKNOWN_COMMAND;
            break;

        } /* end of: command switch */

        rtapi_print_msg(RTAPI_MSG_DBG, "\n");

        memcpy(&startpos,&(emcmotCommand->pos),sizeof(startpos));

        //及时更新队列深度ghf-2014/6/18
        emcmotStatus->depth = sqGetDepth(&queue);
        emcmotStatus->activeDepth = sqGetDepth(&queue);

        /* synch tail count */
        emcmotStatus->tail = emcmotStatus->head;
        emcmotConfig->tail = emcmotConfig->head;
        emcmotDebug->tail = emcmotDebug->head;


    }

    /* end of: if-new-command */
    check_stuff("after sqpreprocsegController()");

    return;

}

/*
 emcmotCommandHandler() is called each main cycle to read the
 shared memory buffer
 */
void emcmotCommandHandler(void *arg, long period) {
    int joint_num;
    int n;
    emcmot_joint_t *joint;
    double tmp1;
    emcmot_comp_entry_t *comp_entry;
 //   static double oldzdiff = 0;

    int i=0;
    int ret = -1;

    static double oldz1maxlimit = 0;


    unsigned int currentdate[8];//2015/1/5
    struct rtc_time tm;
    int fd;
    int retval;

    if(emcmotStatus->suspend == 1) {
        decryptInfo(emcmotStatus->encryptbuf);
        emcmotStatus->suspend = 0;
    }

    check_stuff("before command_handler()");


    /* check for split read */
    if (emcmotCommand->head != emcmotCommand->tail) {
        emcmotDebug->split++;
        return; /* not really an error */
    }
    if (emcmotCommand->commandNum != emcmotStatus->commandNumEcho) {

        if ((emcmotCommand->command == EMCMOT_SET_LINE)
                || (emcmotCommand->command == EMCMOT_SET_CIRCLE)
                ) {
            return;
        }

        /* increment head count-- we'll be modifying emcmotStatus */
        emcmotStatus->head++;
        emcmotDebug->head++;

        /* got a new command-- echo command and number... */
        emcmotStatus->commandEcho = emcmotCommand->command;
        emcmotStatus->commandNumEcho = emcmotCommand->commandNum;

        /* clear status value by default */
        emcmotStatus->commandStatus = EMCMOT_COMMAND_OK;

        /* ...and process command */

        /* Many commands uses "command->axis" to indicate which joint they
         wish to operate on.  This code eliminates the need to copy
         command->axis to "joint_num", limit check it, and then set "joint"
         to point to the joint data.  All the individual commands need to do
         is verify that "joint" is non-zero. */
        joint_num = emcmotCommand->axis;
        if (joint_num >= 0 && joint_num < num_joints) {
            /* valid joint, point to it's data */
            joint = &joints[joint_num];
        } else {
            /* bad joint number */
            joint = 0;
        }

        /* printing of commands for troubleshooting */
        rtapi_print_msg(RTAPI_MSG_DBG, "%d: CMD %d, code %3d ",
                emcmotStatus->heartbeat, emcmotCommand->commandNum,
                emcmotCommand->command);

        switch (emcmotCommand->command) {
        case EMCMOT_SET_PAR:
            sqSetCornerLimit(&queue,emcmotCommand->intmotpar[10]); //corner limit vel;
            sqSetFilterNumber(&queue,emcmotCommand->intmotpar[0]); 
            sqSetPointFilterNumber(&queue,emcmotCommand->intmotpar[3]);
            sqSetCircleInsertFlag(&queue,emcmotCommand->intmotpar[4]);
            sqSetOmegaLimit(&queue,emcmotCommand->intmotpar[1]);
           sqSetControlType(&queue,emcmotCommand->intmotpar[2]);
           sqSetSmallSegMode(&queue, emcmotCommand->intmotpar[5]);


            setInterpolateMethod(emcmotCommand->intmotpar[6]);
            sqSetPlotData(&queue, emcmotCommand->intmotpar[7]);

           emcmotStatus->handleuseType = emcmotCommand->intmotpar[11];
           emcmotStatus->handleDir = emcmotCommand->intmotpar[12];
             emcmotStatus->useAbsEncoder = emcmotCommand->intmotpar[13];             
             emcmotStatus->axesType = emcmotCommand->intmotpar[15];

             setsqAbGoAlongXYZ(&queue,emcmotCommand->intmotpar[16]);

            sqSetTolerance(&queue,emcmotCommand->doumotpar[2]);
           sqSetCircleToleranceG0(&queue,emcmotCommand->doumotpar[5]);
           sqSetCircleToleranceG1(&queue,emcmotCommand->doumotpar[6]);
           sqSetOptimizedAngle(&queue,emcmotCommand->doumotpar[7]);//optimized_angle单位度



            emcmotStatus->joguseScale = emcmotCommand->doumotpar[8];
            if(emcmotStatus->joguseScale == 0)
                emcmotStatus->joguseScale = 0.001;

            sqSetParaDouble(&queue,1,emcmotCommand->doumotpar[9]);

            emcmotStatus->spindle.acclimit = emcmotCommand->doumotpar[12];
//            emcmotStatus->spindle.maxvelocity = emcmotCommand->doumotpar[11]*360/60*servo_period;

            emcmotStatus->spindle.maxvelocity = emcmotCommand->doumotpar[11]*10000/60*servo_period;
            printf("Now Spindel acclimit=%.3f vel:%.3f\n",emcmotStatus->spindle.acclimit,emcmotStatus->spindle.maxvelocity);

//             emcmotStatus->spindle.acclimit = 200;
//              emcmotStatus->spindle.maxvelocity  = 1000;
/*
            //get the useabs flag to clear the homed flag
            static int oldabsflag = -1;
            //切换模式时防止坐标值变化导致电机的突然转动，直接设为跟随错误
            for (joint_num = 0; joint_num < num_joints; joint_num++) {
                if(oldabsflag != -1 && oldabsflag != emcmotStatus->useAbsEncoder){
                    SET_JOINT_FERROR_FLAG (&joints[joint_num], 1);}
            }
            if(emcmotStatus->useAbsEncoder == 0) {
                for (joint_num = 0; joint_num < num_joints; joint_num++) {
                    SET_JOINT_HOMED_FLAG(&joints[joint_num], 0);
                }
                if(emcmotStatus->axesType == 1) {
                    SET_JOINT_HOMED_FLAG(&joints[4], 1);
                    SET_JOINT_HOMED_FLAG(&joints[5], 1);
                } else if(emcmotStatus->axesType == 2) {
                    SET_JOINT_HOMED_FLAG(&joints[5], 1);
                } else if(emcmotStatus->axesType == 3) {
                    SET_JOINT_HOMED_FLAG(&joints[4], 1);
                } else if(emcmotStatus->axesType == 4) {
                    SET_JOINT_HOMED_FLAG(&joints[3], 1);
                }
            }
            oldabsflag = emcmotStatus->useAbsEncoder;
*/

            break;

        case EMCMOT_ABORT:
            /* abort motion */
            /* can happen at any time */
            /* this command attempts to stop all machine motion. it looks at
             the current mode and acts accordingly, if in teleop mode, it
             sets the desired velocities to zero, if in coordinated mode,
             it calls the traj planner abort function (don't know what that
             does yet), and if in free mode, it disables the free mode traj
             planners which stops joint motion */
            rtapi_print_msg(RTAPI_MSG_DBG, "ABORT");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            /* check for coord or free space motion active */
            if (GET_MOTION_TELEOP_FLAG()) {
                ZERO_EMC_POSE(emcmotDebug->teleop_data.desiredVel);
            } else if (GET_MOTION_COORD_FLAG()) {
                printf("now I in here to stop changing z\n");
                emcmotStatus->changezend=1;
                emcmotStatus->changingz = 0;
                sqAbort (&queue);            //ghf
                //sqClearQueue(&queue); //ghf-20150518 解决软限位引起的界面卡死
                if(emcmotStatus->forcflag > 0) {

                    printf("now I get abort to stop forcflag\n");
                    //emcmotStatus->forcflag=0;//hong to stop changing z
                    //SET_MOTION_INPOS_FLAG(1);
                    emcmotStatus->ManulInAutoflag = 0;
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
                    int i=0;
                    for(i=0;i<EMCMOT_MAX_JOINTS;i++) {
                        joints[i].axis_offset =  joints[i].pos_cmd;
                        joints[i].freepos = joints[i].pos_cmd;
                        //SET_JOINT_INPOS_FLAG(&joints[i],1);
                    }
                }

            } else {
                for (joint_num = 0; joint_num < num_joints; joint_num++) {
                    /* point to joint struct */
                    joint = &joints[joint_num];

                    //ghf-2014/8/18,回零的最后一步点击退出，会进到这里 ,再进到HOME_ABORT状态去处理，这里将index_enable清零
                    /* tell joint planner to stop */
                    joint->free_tp_enable = 0;
                    /* stop homing if in progress */
                    if (joint->home_state != HOME_IDLE) {
                        joint->home_state = HOME_ABORT;
                    }
                }
            }
            SET_MOTION_ERROR_FLAG(0);
            /* clear joint errors (regardless of mode */
            for (joint_num = 0; joint_num < num_joints; joint_num++) {
                joint = &joints[joint_num];
                SET_JOINT_ERROR_FLAG(joint, 0);
                SET_JOINT_FAULT_FLAG(joint, 0);
                SET_JOINT_INPOS_FLAG(joint, 1);

                //  最短路径模式下需要处理
                if(joint->circle_shortest == 1) {
                    kinedata.lastP = emcmotStatus->carte_pos_cmd;
                    if(joint_num == 3) {
                        kinedata.circle_count[joint_num] = (int)emcmotStatus->carte_pos_cmd.a/360;
                        kinedata.lastP.a -= kinedata.circle_count[joint_num]*360;
                        printf("kinedata.lastP %.3f  motor_poscmd:%.3f  count:%d\n",kinedata.lastP.c,emcmotStatus->carte_pos_cmd.c,kinedata.circle_count[joint_num]);
                    } else if(joint_num == 4) {
                        kinedata.circle_count[joint_num] = (int)emcmotStatus->carte_pos_cmd.b/360;
                        kinedata.lastP.b -= kinedata.circle_count[joint_num]*360;
                        printf("kinedata.lastP %.3f  motor_poscmd:%.3f  count:%d\n",kinedata.lastP.c,emcmotStatus->carte_pos_cmd.c,kinedata.circle_count[joint_num]);
                    } else if(joint_num == 5) {
                        kinedata.circle_count[joint_num] = (int)emcmotStatus->carte_pos_cmd.c/360;
                        kinedata.lastP.c -= kinedata.circle_count[joint_num]*360;
                        printf("kinedata.lastP %.3f  motor_poscmd:%.3f  count:%d\n",kinedata.lastP.c,emcmotStatus->carte_pos_cmd.c,kinedata.circle_count[joint_num]);
                    }
                }
            }
            emcmotStatus->paused = 0;
            emcmotStatus->runcycleflag=0;//ghf-2015-2-26



            SET_MOTION_INPOS_FLAG(1);
            break;

        case EMCMOT_AXIS_ABORT: //FIXME-AJ: rename
            /* abort one joint */
            /* can happen at any time */
            /* this command stops a single joint.  It is only usefull
             in free mode, so in coord or teleop mode it does
             nothing. */
            rtapi_print_msg(RTAPI_MSG_DBG, "AXIS_ABORT");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            if (GET_MOTION_TELEOP_FLAG()) {
                /* do nothing in teleop mode */
            } else if (GET_MOTION_COORD_FLAG()) {
                /* do nothing in coord mode */
            } else {
                if(joint_num == -1) {
                    for (joint_num = 0; joint_num < num_joints; joint_num++) {
                        joint = &joints[joint_num];
                        joint->free_tp_enable = 0;
                        if (joint->home_state != HOME_IDLE) {
                            joint->home_state = HOME_ABORT;
                        }
                        SET_JOINT_ERROR_FLAG(joint, 0);
                    }
                } else {
                    /* validate joint */
                    if (joint == 0) {
                        break;
                    }
                    /* tell joint planner to stop */
                    joint->free_tp_enable = 0;
                    /* stop homing if in progress */
                    if (joint->home_state != HOME_IDLE) {
                        joint->home_state = HOME_ABORT;
                    }
                    /* update status flags */
                    SET_JOINT_ERROR_FLAG(joint, 0);
                }
            }
            break;

        case EMCMOT_FREE:
            /* change the mode to free mode motion (joint mode) */
            /* can be done at any time */
            /* this code doesn't actually make the transition, it merely
             requests the transition by clearing a couple of flags */
            /* reset the emcmotDebug->coordinating flag to defer transition
             to controller cycle */
            rtapi_print_msg(RTAPI_MSG_DBG, "FREE");
            emcmotDebug->coordinating = 0;
            emcmotDebug->teleoperating = 0;
            break;

        case EMCMOT_COORD:
            /* change the mode to coordinated axis motion */
            /* can be done at any time */
            /* this code doesn't actually make the transition, it merely
             tests a condition and then sets a flag requesting the
             transition */
            /* set the emcmotDebug->coordinating flag to defer transition to
             controller cycle */
            rtapi_print_msg(RTAPI_MSG_DBG, "COORD");
            emcmotDebug->coordinating = 1;
            emcmotDebug->teleoperating = 0;
            //if (kinType != KINEMATICS_IDENTITY) { //ghf-2014/9/27防止没回零完成也能切到自动模式，进而导致跟随错误
              /*  ret = checkAllHomed();
                if (!ret) {
                    reportError(
                            _("all joints must be homed before going into coordinated mode"));
                    emcmotDebug->coordinating = 0;
                    break;
                }*/
            //}
            break;

        case EMCMOT_TELEOP:
            /* change the mode to teleop motion */
            /* can be done at any time */
            /* this code doesn't actually make the transition, it merely
             tests a condition and then sets a flag requesting the
             transition */
            /* set the emcmotDebug->teleoperating flag to defer transition to
             controller cycle */
            rtapi_print_msg(RTAPI_MSG_DBG, "TELEOP");
            emcmotDebug->teleoperating = 1;
            if (kinType != KINEMATICS_IDENTITY) {

                ret = checkAllHomed();
                if (!ret) {
                    reportError(
                            _(
                                    "all joints must be homed before going into teleop mode"));
                    emcmotDebug->teleoperating = 0;
                    break;
                }

            }
            break;

        case EMCMOT_SET_NUM_AXES: //FIXME-AJ: we'll want to rename this to EMCMOT_SET_NUM_JOINTS
            /* set the global NUM_JOINTS, which must be between 1 and
             EMCMOT_MAX_JOINTS, inclusive */
            /* this sets a global - I hate globals - hopefully this can be
             moved into the config structure, or dispensed with completely */
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_NUM_AXES");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", emcmotCommand->axis);
            if ((emcmotCommand->axis <= 0)
                    || (emcmotCommand->axis > EMCMOT_MAX_JOINTS)) {
                break;
            }
            num_joints = emcmotCommand->axis;
            emcmotConfig->numJoints = num_joints;
            break;

        case EMCMOT_SET_WORLD_HOME:
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_WORLD_HOME");
            emcmotStatus->world_home = emcmotCommand->pos;
            break;

        case EMCMOT_SET_HOMING_PARAMS:
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_HOMING_PARAMS");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            emcmot_config_change();
            if (joint == 0) {
                break;
            }
            joint->home_offset = emcmotCommand->offset;
            joint->home = emcmotCommand->home;
            joint->home_final_vel = emcmotCommand->home_final_vel;
            joint->home_search_vel = emcmotCommand->search_vel;            
            joint->home_latch_vel = emcmotCommand->latch_vel;
            joint->oldhome_searchvel = joint->home_search_vel;
            joint->oldhome_latchvel = joint->home_latch_vel;
            joint->home_flags = emcmotCommand->flags;
            joint->home_sequence = emcmotCommand->home_sequence;
            joint->volatile_home = emcmotCommand->volatile_home;
            joint->pitchvalue = emcmotCommand->pitchvalue; //ghf-2014/5/29
            joint->distancecheck = emcmotCommand->distancecheck;
            //rtapi_print_msg(RTAPI_MSG_ERR, "joint->pitchvalue = %d\n",
            //		(int) joint->pitchvalue);
            if ((emcmotCommand->axis >= 0)&&(emcmotCommand->axis <= 1)){
                joint->fric_delay_t = emcmotCommand->fric_delay_t; //ghf-2014/11/28
                joint->fric_lag_ts0 = emcmotCommand->fric_lag_ts0; //ghf-2014/11/28
                joint->fric_lag_ts1 = emcmotCommand->fric_lag_ts1; //ghf-2014/11/28
                joint->fric_max_u0 = emcmotCommand->fric_max_u0; //ghf-2014/11/28
                joint->fric_max_u1 = emcmotCommand->fric_max_u1; //ghf-2014/11/28
            }
            if(emcmotCommand->axis == 0)
                emcmotStatus->fric_type = emcmotCommand->fric_type;
            break;

        case EMCMOT_OVERRIDE_LIMITS:
            /* this command can be issued with axix < 0 to re-enable
             limits, but they are automatically re-enabled at the
             end of the next jog */
            rtapi_print_msg(RTAPI_MSG_DBG, "OVERRIDE_LIMITS");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            if (joint_num < 0) {
                /* don't override limits */
                rtapi_print_msg(RTAPI_MSG_DBG, "override off");
                emcmotStatus->overrideLimitMask = 0;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "override on");
                emcmotStatus->overrideLimitMask = 0;
                for (joint_num = 0; joint_num < num_joints; joint_num++) {
                    /* point at joint data */
                    joint = &joints[joint_num];
                    /* only override limits that are currently tripped */
                    if (GET_JOINT_NHL_FLAG(joint)) {
                        emcmotStatus->overrideLimitMask |=
                                (1 << (joint_num * 2));
                    }
                    if (GET_JOINT_PHL_FLAG(joint)) {
                        emcmotStatus->overrideLimitMask |=
                                (2 << (joint_num * 2));
                    }
                }
            }
            emcmotDebug->overriding = 0;
            for (joint_num = 0; joint_num < num_joints; joint_num++) {
                /* point at joint data */
                joint = &joints[joint_num];
                /* clear joint errors */
                SET_JOINT_ERROR_FLAG(joint, 0);
            }
            break;

        case EMCMOT_SET_MOTOR_OFFSET:
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_MOTOR_OFFSET");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            if (joint == 0) {
                break;
            }
            joint->motor_offset = emcmotCommand->motor_offset;
            break;

        case EMCMOT_SET_POSITION_LIMITS:
            /* sets soft limits for a joint */
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_POSITION_LIMITS");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            emcmot_config_change();
            /* set the position limits for the joint */
            /* can be done at any time */
            if (joint == 0) {
                break;
            }
            joint->min_pos_limit = emcmotCommand->minLimit;
            joint->max_pos_limit = emcmotCommand->maxLimit;
            printf("Now I set limit:%f  %f\n",joint->min_pos_limit,joint->max_pos_limit);

            oldz1maxlimit = joint->max_pos_limit; //ghf-2014/4/30

            break;
        case EMCMOT_SET_SEC_LIMITS:

            emcmot_config_change();

            if (joint == 0) {
                break;
            }
            joint->sec_neg_limit = emcmotCommand->secminLimit;
            joint->sec_pos_limit = emcmotCommand->secmaxLimit;
            joint->sec_limit_offset = emcmotCommand->secLimitoffset;

            printf("now I set sec limit:jointnum:%d sec_neg:%f  sec_pos:%f  sec_offset:%f\n",joint_num,joint->sec_neg_limit,joint->sec_pos_limit,joint->sec_limit_offset);
            break;


        case EMCMOT_SET_BACKLASH:
            /* sets backlash for a joint */
            //rtapi_print_msg(RTAPI_MSG_ERR, "SET_BACKLASH,emcmotCommand->backlash=%d\n",(int)(emcmotCommand->backlash*1000));
            //rtapi_print_msg(RTAPI_MSG_ERR, " %d", joint_num);
            emcmot_config_change();
            /* set the backlash for the joint */
            /* can be done at any time */
            if (joint == 0) {
                break;
            }
            joint->backlash = emcmotCommand->backlash;
            break;

            /*
             Max and min ferror work like this: limiting ferror is
             determined by slope of ferror line, = maxFerror/limitVel ->
             limiting ferror = maxFerror/limitVel * vel. If ferror <
             minFerror then OK else if ferror < limiting ferror then OK
             else ERROR */
        case EMCMOT_SET_MAX_FERROR:
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_MAX_FERROR");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            emcmot_config_change();
            if (joint == 0 || emcmotCommand->maxFerror < 0.0) {
                break;
            }
            joint->max_ferror = emcmotCommand->maxFerror;
            break;

        case EMCMOT_SET_MIN_FERROR:
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_MIN_FERROR");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            emcmot_config_change();
            if (joint == 0 || emcmotCommand->minFerror < 0.0) {
                break;
            }
            joint->min_ferror = emcmotCommand->minFerror;
            break;

        case EMCMOT_JOG_CONT: //+号每按下一次，此处只进来一次，一直按下也只进来一次ghf-2014/4/14
            /* do a continuous jog, implemented as an incremental jog to the
             limit.  When the user lets go of the button an abort will
             stop the jog. */

            rtapi_print_msg(RTAPI_MSG_DBG, "JOG_CONT");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);

            if(emcmotStatus->axesType != 0) {
                if(emcmotStatus->axesType == 1) {
                    if(joint_num == 4 || joint_num == 5) {
                        break;
                    }
                } else if(emcmotStatus->axesType == 2) {
                    if( joint_num == 5) {
                        break;
                    }
                } else if(emcmotStatus->axesType == 3) {
                    if( joint_num == 4) {
                        break;
                    }
                }
            }
            /* must be in free mode and enabled */
            if (GET_MOTION_COORD_FLAG()) {
                reportError(_("Can't jog joint in coordinated mode."));
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            if (!GET_MOTION_ENABLE_FLAG()) {
                reportError(_("Can't jog joint when not enabled."));
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            if (emcmotStatus->homing_active) {
                reportError(_("Can't jog any joints while homing."));
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            if(joint_num == -1) {
                for (joint_num = 2; joint_num < num_joints; joint_num++) {
                    joint = &joints[joint_num];
                    if(0 == joint->axis_active)
                        continue;
                    if (!jog_ok(joint_num, emcmotCommand->vel)) {
                        SET_JOINT_ERROR_FLAG(joint, 1);
                        break;
                    }
                    refresh_jog_limits(joint);
                    if (emcmotCommand->vel > 0.0) {
                        joint->free_pos_cmd = joint->max_jog_limit;
                    } else {
                        joint->free_pos_cmd = joint->min_jog_limit;
                    }
                    joint->free_vel_lim = fabs(emcmotCommand->vel);
                    joint->kb_jog_active = 1;
                    joint->free_tp_enable = 1;
                    SET_JOINT_ERROR_FLAG(joint, 0);
                    clearHomes(joint_num);
                }
                break;
            } else {
            if (joint->wheel_jog_active) {
                /* can't do two kinds of jog at once */
                break;
            }
            if (emcmotStatus->net_feed_scale < 0.0001) {
                /* don't jog if feedhold is on or if feed override is zero */
                break;
            }
            if (joint->home_flags & HOME_UNLOCK_FIRST) {
                reportError("Can't jog a locking axis.");
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }

            /* don't jog further onto limits */
            if (!jog_ok(joint_num, emcmotCommand->vel)) {
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            /* set destination of jog */
            refresh_jog_limits(joint);

            //ghf-2014/5/5,回零的运动不进这里
            if(GET_JOINT_HOMED_FLAG(joint)) {
                if (emcmotCommand->vel > 0.0) {
                    joint->free_pos_cmd = joint->max_jog_limit;
                } else {
                    joint->free_pos_cmd = joint->min_jog_limit;
                }
                if(fabs(emcmotStatus->useSeclimit) > 0.0001 && GET_JOINT_HOMED_FLAG(joint)) {
                    if(joint_num < 2) {
                        if (emcmotCommand->vel > 0.0) {
                                if(joint->pos_cmd > joint->sec_pos_limit) {
                                     joint->free_pos_cmd = joint->pos_cmd;
                                     reportError(_("轴%d已触发第二正软限位"),joint_num);
                                } else {
                                    if(fabs(joint->sec_limit_offset) > 0.0001) {  //the third limit
                                        if(joint->pos_cmd > joint->sec_pos_limit-joint->sec_limit_offset) {
                                            joint->free_pos_cmd = joint->sec_pos_limit;
                                        } else if(joint->pos_cmd < (joint->sec_neg_limit+joint->sec_limit_offset)) {
                                            if(joint_num == 0) {
                                                if(joints[1].pos_cmd < (joints[1].sec_pos_limit-joints[1].sec_limit_offset) &&
                                                        joints[1].pos_cmd > (joints[1].sec_neg_limit+joints[1].sec_limit_offset)) {
                                                    joint->free_pos_cmd = joint->sec_neg_limit+joint->sec_limit_offset;
                                                }else {
                                                    joint->free_pos_cmd = joint->sec_pos_limit;
                                                }
                                            } else if(joint_num == 1) {
                                                if(joints[0].pos_cmd < (joints[0].sec_pos_limit-joints[0].sec_limit_offset) &&
                                                        joints[0].pos_cmd > (joints[0].sec_neg_limit+joints[0].sec_limit_offset)) {
                                                    joint->free_pos_cmd = joint->sec_neg_limit+joint->sec_limit_offset;
                                                }else {
                                                    joint->free_pos_cmd = joint->sec_pos_limit;
                                                }
                                            }
                                        } else {
                                            if(joints[0].pos_cmd > (joints[0].sec_neg_limit+joints[0].sec_limit_offset) &&
                                                   joints[0].pos_cmd < (joints[0].sec_pos_limit-joints[0].sec_limit_offset) &&
                                                    joints[1].pos_cmd > (joints[1].sec_neg_limit+joints[1].sec_limit_offset) &&
                                                    joints[1].pos_cmd < (joints[1].sec_pos_limit-joints[1].sec_limit_offset) )  {
                                                    joint->free_pos_cmd = joint->pos_cmd;
                                            } else
                                                 joint->free_pos_cmd = joint->sec_pos_limit;

                                        }
                                    } else
                                        joint->free_pos_cmd = joint->sec_pos_limit;
                                }
                        } else {
                                if(joint->pos_cmd < joint->sec_neg_limit) {
                                     joint->free_pos_cmd = joint->pos_cmd;
                                     reportError(_("轴%d已触发第二负软限位"),joint_num);
                                } else {
                                    if(fabs(joint->sec_limit_offset) > 0.0001) {  //the third limit
                                        if(joint->pos_cmd > joint->sec_pos_limit-joint->sec_limit_offset) {
                                            if(joint_num == 0) {
                                                if(joints[1].pos_cmd < (joints[1].sec_pos_limit-joints[1].sec_limit_offset) &&
                                                        joints[1].pos_cmd > (joints[1].sec_neg_limit+joints[1].sec_limit_offset)) {
                                                    joint->free_pos_cmd = joint->sec_pos_limit-joint->sec_limit_offset;
                                                }else {
                                                    joint->free_pos_cmd = joint->sec_neg_limit;
                                                }
                                            } else if(joint_num == 1) {
                                                if(joints[0].pos_cmd < (joints[0].sec_pos_limit-joints[0].sec_limit_offset) &&
                                                        joints[0].pos_cmd > (joints[0].sec_neg_limit+joints[0].sec_limit_offset)) {
                                                    joint->free_pos_cmd = joint->sec_pos_limit-joint->sec_limit_offset;
                                                }else {
                                                    joint->free_pos_cmd = joint->sec_neg_limit;
                                                }
                                            }
                                        } else if(joint->pos_cmd < joint->sec_neg_limit+joint->sec_limit_offset) {
                                            joint->free_pos_cmd = joint->sec_neg_limit;
                                        } else {
                                            if(joints[0].pos_cmd > (joints[0].sec_neg_limit+joints[0].sec_limit_offset) &&
                                                   joints[0].pos_cmd < (joints[0].sec_pos_limit-joints[0].sec_limit_offset) &&
                                                    joints[1].pos_cmd > (joints[1].sec_neg_limit+joints[1].sec_limit_offset) &&
                                                    joints[1].pos_cmd < (joints[1].sec_pos_limit-joints[1].sec_limit_offset) )  {
                                                joint->free_pos_cmd = joint->pos_cmd;
                                            } else
                                                 joint->free_pos_cmd = joint->sec_neg_limit;
                                        }
                                    } else
                                        joint->free_pos_cmd = joint->sec_neg_limit;
                                }
                        }
                    }
                }
            } else {
                double range = joint->max_jog_limit-joint->min_jog_limit;
                if (emcmotCommand->vel > 0.0) {
                    joint->free_pos_cmd = joint->old_pos_cmd+range;
                } else {
                    joint->free_pos_cmd = joint->old_pos_cmd-range;
                }

                printf("Now I in no seclimit:%f \n",joint->free_pos_cmd);
            }
            //end ghf-2014/5/5

            /* set velocity of jog */
            joint->free_vel_lim = fabs(emcmotCommand->vel);
            /* lock out other jog sources */
            joint->kb_jog_active = 1;
            /* and let it go */
            joint->free_tp_enable = 1;
            /*! \todo FIXME - should we really be clearing errors here? */
            SET_JOINT_ERROR_FLAG(joint, 0);
            printf("now I in set jogcount:%d freepos:%f  vel:%.3f  pos:%.3f   %.3f\n",joint_num,joint->free_pos_cmd,emcmotCommand->vel,joint->max_jog_limit,joint->min_jog_limit);
            /* clear joints homed flag(s) if we don't have forward kins.
             Otherwise, a transition into coordinated mode will incorrectly
             assume the homed position. Do all if they've all been moved
             since homing, otherwise just do this one */
            clearHomes(joint_num);
            }
            break;

        case EMCMOT_JOG_INCR:
            /* do an incremental jog */

            /* check joints range */
            rtapi_print_msg(RTAPI_MSG_DBG, "JOG_INCR");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            if (joint == 0) {
                break;
            }
            if(emcmotStatus->axesType != 0) {
                if(emcmotStatus->axesType == 1) {
                    if(joint_num == 4 || joint_num == 5) {
                        break;
                    }
                } else if(emcmotStatus->axesType == 2) {
                    if( joint_num == 5) {
                        break;
                    }
                } else if(emcmotStatus->axesType == 3) {
                    if( joint_num == 4) {
                        break;
                    }
                }
            }
            /* must be in free mode and enabled */
            if (GET_MOTION_COORD_FLAG()) {
                reportError(_("Can't jog joint in coordinated mode."));
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            if (!GET_MOTION_ENABLE_FLAG()) {
                reportError(_("Can't jog joint when not enabled."));
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            if (emcmotStatus->homing_active) {
                reportError(_("Can't jog any joint while homing."));
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            if (joint->wheel_jog_active) {
                /* can't do two kinds of jog at once */
                break;
            }
            if (emcmotStatus->net_feed_scale < 0.0001) {
                /* don't jog if feedhold is on or if feed override is zero */
                break;
            }
            if (joint->home_flags & HOME_UNLOCK_FIRST) {
                reportError("Can't jog a locking axis.");
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }

            /* don't jog further onto limits */
            if (!jog_ok(joint_num, emcmotCommand->vel)) {
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            /* set target position for jog */
            if (emcmotCommand->vel > 0.0) {
                tmp1 = joint->free_pos_cmd + emcmotCommand->offset;
            } else {
                tmp1 = joint->free_pos_cmd - emcmotCommand->offset;
            }
            /* don't jog past limits */
            refresh_jog_limits(joint);

            if (tmp1 > joint->max_jog_limit && emcmotCommand->vel > 0 && joint->circle_shortest == 0) {
                break;
            }
            if (tmp1 < joint->min_jog_limit && emcmotCommand->vel < 0 && joint->circle_shortest == 0) {
                break;
            }

            //end ghf-2014/5/5

            /* set target position */
            joint->free_pos_cmd = tmp1;
            if(fabs(emcmotStatus->useSeclimit) > 0.0001 && GET_JOINT_HOMED_FLAG(joint)) {

                if(joint_num < 2 ){//只限制XY
                    if(joint->free_pos_cmd > joint->sec_pos_limit || joint->free_pos_cmd < joint->sec_neg_limit) {
                         joint->free_pos_cmd = joint->pos_cmd;
                         reportError(_("轴%d已触发第二软限位"),joint_num);
                    } else {
                        if(fabs(joint->sec_limit_offset) > 0.0001) {  //the third limit
                                if(joints[0].free_pos_cmd > (joints[0].sec_neg_limit+joints[0].sec_limit_offset) &&
                                       joints[0].free_pos_cmd < (joints[0].sec_pos_limit-joints[0].sec_limit_offset) &&
                                        joints[1].free_pos_cmd > (joints[1].sec_neg_limit+joints[1].sec_limit_offset) &&
                                        joints[1].free_pos_cmd < (joints[1].sec_pos_limit-joints[1].sec_limit_offset) )  {
                                        joint->free_pos_cmd = joint->pos_cmd;
                                } else
                                     joint->free_pos_cmd = tmp1;
                        } else
                            joint->free_pos_cmd = tmp1;
                    }
                }
            }


            /* set velocity of jog */
            joint->free_vel_lim = fabs(emcmotCommand->vel);
            /* lock out other jog sources */
            joint->kb_jog_active = 1;
            /* and let it go */
            joint->free_tp_enable = 1;
            SET_JOINT_ERROR_FLAG(joint, 0);
            /* clear joint homed flag(s) if we don't have forward kins.
             Otherwise, a transition into coordinated mode will incorrectly
             assume the homed position. Do all if they've all been moved
             since homing, otherwise just do this one */
            clearHomes(joint_num);
            break;

        case EMCMOT_JOG_ABS:
            /* do an absolute jog */

            /* check joint range */
            rtapi_print_msg(RTAPI_MSG_DBG, "JOG_ABS");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            if (joint == 0) {
                break;
            }
            /* must be in free mode and enabled */
            if (GET_MOTION_COORD_FLAG()) {
                reportError(_("Can't jog joint in coordinated mode."));
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            if (!GET_MOTION_ENABLE_FLAG()) {
                reportError(_("Can't jog joint when not enabled."));
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            if (emcmotStatus->homing_active) {
                reportError(_("Can't jog any joints while homing."));
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            if (joint->wheel_jog_active) {
                /* can't do two kinds of jog at once */
                break;
            }
            if (emcmotStatus->net_feed_scale < 0.0001) {
                /* don't jog if feedhold is on or if feed override is zero */
                break;
            }

            /*if(joint_num == 2) {
                 if(emcmotStatus->set_which_z == 2) {   //ghf-2014/4/30
                     if(emcmotStatus->zdiff < 0) {
                        joint->max_pos_limit = emcmotStatus->zdiff;
                     }
                 }
                 else {
                    joint->max_pos_limit = oldz1maxlimit;
                 }
             }*/

            /* don't jog further onto limits */
            if (!jog_ok(joint_num, emcmotCommand->vel)) {
                SET_JOINT_ERROR_FLAG(joint, 1);
                break;
            }
            /*! \todo FIXME-- use 'goal' instead */
            joint->free_pos_cmd = emcmotCommand->offset;
            /* don't jog past limits */
            refresh_jog_limits(joint);
            if (joint->free_pos_cmd > joint->max_jog_limit) {
                joint->free_pos_cmd = joint->max_jog_limit;
            }
            if (joint->free_pos_cmd < joint->min_jog_limit) {
                joint->free_pos_cmd = joint->min_jog_limit;
            }
            /* set velocity of jog */
            joint->free_vel_lim = fabs(emcmotCommand->vel);
            /* lock out other jog sources */
            joint->kb_jog_active = 1;
            /* and let it go */
            joint->free_tp_enable = 1;
            SET_JOINT_ERROR_FLAG(joint, 0);
            /* clear joint homed flag(s) if we don't have forward kins.
             Otherwise, a transition into coordinated mode will incorrectly
             assume the homed position. Do all if they've all been moved
             since homing, otherwise just do this one */
            clearHomes(joint_num);
            break;

        case EMCMOT_SET_TERM_COND:
            /* sets termination condition for motion emcmotDebug->queue */
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_TERM_COND");
            //tpSetTermCond(&emcmotDebug->queue, emcmotCommand->termCond, emcmotCommand->tolerance);
            break;

        case EMCMOT_SET_SPINDLESYNC:

            emcmotStatus->spindle.css_factor = emcmotCommand->spindlesync;
            emcmotStatus->spindle.syncjoint = 2;
            emcmotStatus->spindle.startcount=0;
            if(emcmotCommand->spindlesync == 0) {
//                emcmotStatus->spindle.endpos = emcmotStatus->spindle.fbnowpos;
//                emcmotStatus->spindle.speed = 0;
                emcmotStatus->spindle.startmove = 3;
                *(emcmot_hal_data->spindle_enable) = 0;
                emcmotStatus->spindle.direction = 0;
                printf("Now In stop spindle sync\n");
            } else if(emcmotCommand->flags == 2) {
                emcmotStatus->spindle.startmove = 2;
                emcmotStatus->spindle.moveType = 0;
//                emcmotStatus->spindle.nowpos = emcmotStatus->spindle.fbnowpos;
                *(emcmot_hal_data->spindle_enable) = 1; //主轴开使能
            } else if(emcmotCommand->flags == -1) {
                emcmotStatus->spindle.startmove = 3;
                emcmotStatus->spindle.direction = 0;
            }


            printf("nwo I set spindle move:%d  %.3f  nowp:%f  fb:%f  flag:%d \n",emcmotStatus->spindle.startmove,emcmotStatus->spindle.css_factor,emcmotStatus->spindle.nowpos,emcmotStatus->spindle.fbnowpos,emcmotCommand->flags);
            break;

        case EMCMOT_SET_VEL:
            /* set the velocity for subsequent moves */
            /* can do it at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_VEL");
            emcmotStatus->vel = emcmotCommand->vel;
            //tpSetVmax(&emcmotDebug->queue, emcmotStatus->vel,
            //	    emcmotCommand->ini_maxvel);
            sqSetFeed(&queue, emcmotStatus->vel);		//ghf


            break;

        case EMCMOT_SET_VEL_LIMIT://此命令已无效，直接EMCMOT_SETGMODE中全部搞定，包括最大速度，最大加速度，最大加加速度
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_VEL_LIMIT");
            emcmot_config_change();
            /* set the absolute max velocity for all subsequent moves */
            /* can do it at any time */
            emcmotConfig->limitVel = emcmotCommand->vel;
            //tpSetVlimit(&emcmotDebug->queue, emcmotConfig->limitVel);
            sqSetVmax(&queue, emcmotConfig->limitVel);		//ghf
            break;

        case EMCMOT_SET_JOINT_VEL_LIMIT:
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_JOINT_VEL_LIMIT");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            emcmot_config_change();
            /* check joint range */
            if (joint == 0) {
                break;
            }
            joint->vel_limit = emcmotCommand->vel;
            joint->big_vel = 10 * emcmotCommand->vel;
            joint->useposfb = emcmotCommand->axisuseposfb;
            joint->useAbsPos = emcmotCommand->useabspos;
            printf("now I get joint:%d useabsPos:%d\n",joint_num,joint->useAbsPos);

            static int oldabsflag[EMCMOT_MAX_JOINTS] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
             //切换模式时防止坐标值变化导致电机的突然转动，直接设为跟随错误
            if(oldabsflag[joint_num] != -1 && oldabsflag != joint->useAbsPos)
                     SET_JOINT_FERROR_FLAG (&joints[joint_num], 1);

              if(joint->useAbsPos > 0) {
                     SET_JOINT_HOMED_FLAG(&joints[joint_num], 1);
             }


              //开机第一次绝对值编码器设置为回零状态
              if(oldabsflag[joint_num] == -1 &&( joint->useAbsPos > 0)) {
                    SET_JOINT_HOMED_FLAG(&joints[joint_num], 1);
              }

              oldabsflag[joint_num] = joint->useAbsPos;
            break;

        case EMCMOT_SET_JOINT_ACC_LIMIT:		//设置各轴的最大加速度，手动时有效ghf-2014/5/24
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_JOINT_ACC_LIMIT");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            emcmot_config_change();
            /* check joint range */
            if (joint == 0) {
                break;
            }
            joint->acc_limit = emcmotCommand->acc;
            break;

        case EMCMOT_SET_ACC:		//设置手动规划器的最大加速度，自动时不用此值，用gmode中的
            /* set the max acceleration */
            /* can do it at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_ACCEL");
            emcmotStatus->acc = emcmotCommand->acc;
            //tpSetAmax(&emcmotDebug->queue, emcmotStatus->acc);
            //sqSetMaxAcc(&queue,emcmotStatus->acc);//ghf-2014/5/23,不然会把gmode中的最大加速度替换掉
            //rtapi_print_msg(RTAPI_MSG_ERR, "emcmotStatus->acc=%d\n",(int)emcmotStatus->acc);
            break;

        case EMCMOT_PAUSE:
            /* pause the motion */
            /* can happen at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "PAUSE");
            //tpPause(&emcmotDebug->queue);
            sqPause (&queue);	    //ghf
            emcmotStatus->paused = 1;
            break;

        case EMCMOT_RESUME:
            /* resume paused motion */
            /* can happen at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "RESUME");
            emcmotDebug->stepping = 0;
            //tpResume(&emcmotDebug->queue);
            sqResume(&queue);	    //ghf
            emcmotStatus->paused = 0;
            break;

        case EMCMOT_STEP:	    //diff
            /* resume paused motion until id changes */
            /* can happen at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "STEP");
            if (emcmotStatus->paused) {
                emcmotDebug->idForStep = emcmotStatus->id;
                emcmotDebug->stepping = 1;
                //tpResume(&emcmotDebug->queue);
                sqResume(&queue);	    //ghf
                emcmotStatus->paused = 1;
            } else {
                reportError(_("MOTION: can't STEP while already executing"));
            }
            break;

        case EMCMOT_FEED_SCALE:
            /* override speed */
            /* can happen at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "FEED SCALE");
            if (emcmotCommand->scale < 0.0) {
                emcmotCommand->scale = 0.0; /* clamp it */
            }
            emcmotStatus->feed_scale = emcmotCommand->scale;

            sqSetFeedOverride(&queue, emcmotStatus->feed_scale);

            //rtapi_print("in command.c,emcmotStatus->feed_scale = %d\n",(int)(emcmotStatus->feed_scale*1000));

            break;

        case EMCMOT_FG0_ENABLE:
            emcmotStatus->g0enableflag = emcmotCommand->g0enableflag;
            sqSetParaInt(&queue,1,emcmotStatus->g0enableflag);
            break;

        case EMCMOT_FS_ENABLE:
            /* enable/disable overriding speed */
            /* can happen at any time */
            if (emcmotCommand->mode != 0) {
                rtapi_print_msg(RTAPI_MSG_DBG, "FEED SCALE: ON");
                emcmotStatus->enables_new |= FS_ENABLED;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "FEED SCALE: OFF");
                emcmotStatus->enables_new &= ~FS_ENABLED;
            }

            emcmotStatus->g0_usefeedrate = emcmotCommand->mode;//ghf-2015-4-8
            sqSetParaInt(&queue,0,emcmotStatus->g0_usefeedrate);

            break;

        case EMCMOT_FH_ENABLE:
            /* enable/disable feed hold */
            /* can happen at any time */
            if (emcmotCommand->mode != 0) {
                rtapi_print_msg(RTAPI_MSG_DBG, "FEED HOLD: ENABLED");
                emcmotStatus->enables_new |= FH_ENABLED;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "FEED HOLD: DISABLED");
                emcmotStatus->enables_new &= ~FH_ENABLED;
            }
            break;

        case EMCMOT_SPINDLE_SCALE: //拖动主轴倍率，此处会进来，ghf-2014/5/28
            /* override spindle speed */
            /* can happen at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "SPINDLE SCALE");
            if (emcmotCommand->scale < 0.0) {
                emcmotCommand->scale = 0.0; /* clamp it */
            }
            emcmotStatus->spindle_scale = emcmotCommand->scale;
            break;

        case EMCMOT_SS_ENABLE: //界面还需发使能主轴倍率调整才能改变倍率，ghf-2014/5/28
            /* enable/disable overriding spindle speed */
            /* can happen at any time */
            if (emcmotCommand->mode != 0) {
                rtapi_print_msg(RTAPI_MSG_DBG, "SPINDLE SCALE: ON");
                emcmotStatus->enables_new |= SS_ENABLED;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "SPINDLE SCALE: OFF");
                emcmotStatus->enables_new &= ~SS_ENABLED;
            }
            break;

        case EMCMOT_AF_ENABLE:
            /* enable/disable adaptive feedrate override from HAL pin */
            /* can happen at any time */
            if (emcmotCommand->flags != 0) {
                rtapi_print_msg(RTAPI_MSG_DBG, "ADAPTIVE FEED: ON");
                emcmotStatus->enables_new |= AF_ENABLED;
            } else {
                rtapi_print_msg(RTAPI_MSG_DBG, "ADAPTIVE FEED: OFF");
                emcmotStatus->enables_new &= ~AF_ENABLED;
            }
            break;

        case EMCMOT_DISABLE:
            /* go into disable */
            /* can happen at any time */
            /* reset the emcmotDebug->enabling flag to defer disable until
             controller cycle (it *will* be honored) */
            rtapi_print_msg(RTAPI_MSG_DBG, "DISABLE");
            emcmotDebug->enabling = 0;
            if (kinType == KINEMATICS_INVERSE_ONLY) {
                emcmotDebug->teleoperating = 0;
                emcmotDebug->coordinating = 0;
            }
            break;

        case EMCMOT_ENABLE:
            /* come out of disable */
            /* can happen at any time */
            /* set the emcmotDebug->enabling flag to defer enable until
             controller cycle */
            rtapi_print_msg(RTAPI_MSG_DBG, "ENABLE");

            if( emcmotDebug->enabling == 0) {
                for (n = 0; n < num_joints; n++) {
                    joint = &joints[n];
                    if(joint->useAbsPos > 0) {
                    joint->old_pos_cmd = joint->pos_fb;
                    joint->pos_cmd = joint->pos_fb;
                    joint->free_pos_cmd = joint->pos_fb;
                    }
                }
                printf("now I in reset the joint pos before run\n");

                emcmotStatus->spindle.nowpos = emcmotStatus->spindle.fbnowpos;
            }
            if (*(emcmot_hal_data->enable) == 0) {
                reportError(_("can't enable motion, enable input is false"));
            } else {

                if(emcmotStatus->linktoEth > 0 || SIMULATE) {  //使能时记录同步轴的差值
                    emcmotStatus->axesdiffval = joints[1].motor_pos_fb - joints[7].motor_pos_fb;
                    printf("now reset the  axesdiff:%f    X1:%f   X2:%f\n",emcmotStatus->axesdiffval,joints[0].motor_pos_fb , joints[6].motor_pos_fb);
                }

                emcmotDebug->enabling = 1;
                if (kinType == KINEMATICS_INVERSE_ONLY) {
                    emcmotDebug->teleoperating = 0;
                    emcmotDebug->coordinating = 0;
                }
            }
            break;

        case EMCMOT_ACTIVATE_JOINT:
            /* make joint active, so that amps will be enabled when system is
             enabled or disabled */
            /* can be done at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "ACTIVATE_JOINT");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            if (joint == 0) {
                break;
            }
            SET_JOINT_ACTIVE_FLAG(joint, 1);
            break;

        case EMCMOT_DEACTIVATE_JOINT:
            /* make joint inactive, so that amps won't be affected when system
             is enabled or disabled */
            /* can be done at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "DEACTIVATE_JOINT");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            if (joint == 0) {
                break;
            }
            SET_JOINT_ACTIVE_FLAG(joint, 0);
            break;
            /*! \todo FIXME - need to replace the ext function */
        case EMCMOT_ENABLE_AMPLIFIER:
            /* enable the amplifier directly, but don't enable calculations */
            /* can be done at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "ENABLE_AMP");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            if (joint == 0) {
                break;
            }
            /*! \todo Another #if 0 */
#if 0
            extAmpEnable(joint_num, 1);
#endif
            break;

        case EMCMOT_DISABLE_AMPLIFIER:
            /* disable the joint calculations and amplifier, but don't disable
             calculations */
            /* can be done at any time */
            rtapi_print_msg(RTAPI_MSG_DBG, "DISABLE_AMP");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);
            if (joint == 0) {
                break;
            }
            /*! \todo Another #if 0 */
#if 0
            extAmpEnable(joint_num, 0);
#endif
            break;

        case EMCMOT_HOME:
            /* home the specified joint */
            /* need to be in free mode, enable on */
            /* this just sets the initial state, then the state machine in
             control.c does the rest */
            rtapi_print_msg(RTAPI_MSG_DBG, "HOME");
            rtapi_print_msg(RTAPI_MSG_DBG, " %d", joint_num);




            if(joint_num < 0) {
                printf("now I in set home 1\n");
                for (n = 0; n < num_joints; n++) {
                    joint = &joints[n];
                    if(1 == emcmotCommand->homeflag) {
                        joint->home_search_vel = joint->oldhome_searchvel;
                        joint->home_latch_vel = joint->oldhome_latchvel;
                        if(joint->useAbsPos == 1) {
                            joint->home_search_vel = 0.0;
                            joint->home_latch_vel = 0.0;
                            printf("now I in set joint homed %d\n",n);
                        }
                    } else if (0 == emcmotCommand->homeflag) {
                        joint->home_search_vel = 0.0;
                        joint->home_latch_vel = 0.0;
                    }
                }
                printf("now I in set home 2\n");
            } else {
                 joint = &joints[joint_num];
//                printf("now I in set home ++++++++++++++++++++++++++++++++++++++++++++++\n");
                if(joint->useAbsPos > 0) {
//                    printf("now I in set home +++++++++++++++++++++1111+++++++++++++++++++++++++\n");
                    SET_JOINT_HOMING_FLAG(joint, 0);
                    SET_JOINT_HOMED_FLAG(joint, 1);//表示回零完毕，上层据此将颜色变为黑色ghf-2014/8/14
                    SET_JOINT_AT_HOME_FLAG(joint, 1);
                    joint->coarse_pos = 0;
                    joint->old_pos_cmd = 0;
                    joint->pos_cmd = 0;
                    joint->free_pos_cmd = 0;
                    joint->abszeroSt = 0;   //绝对值编码器
                    break;
                } else {
                if(joint_num == -1) {
                    for (n = 0; n < num_joints; n++) {
                        joint = &joints[n];
                        if(1 == emcmotCommand->homeflag) {
                            joint->home_search_vel = joint->oldhome_searchvel;
                            joint->home_latch_vel = joint->oldhome_latchvel;
                        } else if (0 == emcmotCommand->homeflag) {
                            joint->home_search_vel = 0.0;
                            joint->home_latch_vel = 0.0;
                        }
                    }
                }
                else {
                    if(1 == emcmotCommand->homeflag) {
                        joint->home_search_vel = joint->oldhome_searchvel;
                        joint->home_latch_vel = joint->oldhome_latchvel;
                    } else if (0 == emcmotCommand->homeflag) {
                        joint->home_search_vel = 0.0;
                        joint->home_latch_vel = 0.0;
                    }
                }
                }
            }
            if (emcmotStatus->motion_state != EMCMOT_MOTION_FREE) {
                /* can't home unless in free mode */
                reportError(_("must be in joint mode to home"));
                return;
            }
            if (!GET_MOTION_ENABLE_FLAG()) {
                break;
            }

            printf("now I in set home 2\n");
            if (joint_num == -1) { //这里可能为-1吗？ghf-2014/3/5
                if (emcmotStatus->homingSequenceState == HOME_SEQUENCE_IDLE) {
                    emcmotStatus->homingSequenceState = HOME_SEQUENCE_START;
//                    joint->free_tp_enable = 0;
//                    joint->home_state = HOME_START;
                    break;
                }else {
                    reportError(_("homing sequence already in progress"));
                    break;
                }
            }

            printf("now I in set home 3\n");
            if (joint == NULL) {
                break;
            }

            if (joint->home_state != HOME_IDLE) {
                reportError(_("homing already in progress"));
            } else if (emcmotStatus->homingSequenceState
                    != HOME_SEQUENCE_IDLE) {
                reportError(_("homing sequence already in progress"));
            } else {
                /* abort any movement (jog, etc) that is in progress */
                joint->free_tp_enable = 0;

                /* prime the homing state machine */
                joint->home_state = HOME_START;
            }
            break;

        case EMCMOT_ENABLE_WATCHDOG:
            rtapi_print_msg(RTAPI_MSG_DBG, "ENABLE_WATCHDOG");
            /*! \todo Another #if 0 */
#if 0
            emcmotDebug->wdEnabling = 1;
            emcmotDebug->wdWait = emcmotCommand->wdWait;
            if (emcmotDebug->wdWait < 0) {
                emcmotDebug->wdWait = 0;
            }
#endif
            break;

        case EMCMOT_UNHOME:
            /* unhome the specified joint, or all joints if -1 */
            //rtapi_print_msg(RTAPI_MSG_ERR, "UNHOME");
            //rtapi_print_msg(RTAPI_MSG_ERR, " %d", joint_num);

            if ((emcmotStatus->motion_state != EMCMOT_MOTION_FREE)
                    && (emcmotStatus->motion_state != EMCMOT_MOTION_DISABLED)) {
                reportError(_("must be in joint mode or disabled to unhome"));
                return;
            }
            if(joint_num < 0) {
                printf("now I in set unhome 1\n");
                for (n = 0; n < num_joints; n++) {
                    joint = &joints[n];
                    if (GET_JOINT_ACTIVE_FLAG(joint)) {
                        if (GET_JOINT_HOMING_FLAG(joint)) {
                            reportError(_("Cannot unhome while homing, joint %d"),n);
                            return;
                        }
                        if (!GET_JOINT_INPOS_FLAG(joint)) {
                            reportError(_("Cannot unhome while moving, joint %d"),n);
                            return;
                        }
                    }
                }
                printf("now I in set unhome 2\n");
                /* we made it through the checks, so unhome them all */
                for (n = 0; n < num_joints; n++) {
                    joint = &joints[n];
                    if (GET_JOINT_ACTIVE_FLAG(joint)) {
                        /* if -2, only unhome the volatile_home joints */
                        if (joint_num != -2 || joint->volatile_home) {
                            //rtapi_print_msg(RTAPI_MSG_ERR, "UNHOME AXIS %d",n);
                            SET_JOINT_HOMED_FLAG(joint, 0);
                            emcmotDebug->allHomed=0;
                        }
                    }
                }
                printf("now I in set unhome 3\n");
            } else {
             joint = &joints[joint_num];
            if(joint->useAbsPos > 0) {
                if(joint_num < 0) {
                    for (n = 0; n < num_joints; n++) {
                        joint = &joints[n];
                        if (GET_JOINT_ACTIVE_FLAG(joint)) {
                                SET_JOINT_HOMED_FLAG(joint, 0);
                                emcmotDebug->allHomed=0;
                                printf("now I in send unhome\n ");
                                if(0 == emcmotCommand->homeflag) {
                                    rtapi_print_msg(RTAPI_MSG_ERR, _("(joint %d) set the current pos:%.3f to zero pos\n"),n,joint->pos_fb);
                                    joint->abszeroSt = 1;   //绝对值编码器开始设定零点
                                     joint->pos_cmd = 0;
                                    emcZeroPosSave(n);
                                    SET_JOINT_HOMING_FLAG(joint, 1);
                                }
                        }
                    }
                } else {
                    SET_JOINT_HOMED_FLAG(joint, 0);
                    emcmotDebug->allHomed=0;
                    if(0 == emcmotCommand->homeflag) {
                        joint->abszeroSt = 1;   //绝对值编码器开始设定零点
                        joint->pos_cmd = 0;
                        emcZeroPosSave(joint_num);
                        printf("now I in send unhome 1\n");
                        rtapi_print_msg(RTAPI_MSG_ERR, _("(joint %d) set the current pos:%.3f to zero pos\n"),joint_num,joint->motor_pos_fb);
                        SET_JOINT_HOMING_FLAG(joint, 1);
                    }
                }
                break;
            } else {
                if (joint_num < 0) {

                } else if (joint_num < num_joints) {
                    /* request was for only one joint */
                    if (GET_JOINT_ACTIVE_FLAG(joint)) {
                        if (GET_JOINT_HOMING_FLAG(joint)) {
                            reportError(_("Cannot unhome while homing, joint %d"),
                                    joint_num);
                            return;
                        }
                        if (!GET_JOINT_INPOS_FLAG(joint)) {
                            reportError(_("Cannot unhome while moving, joint %d"),
                                    joint_num);
                            return;
                        }
                        SET_JOINT_HOMED_FLAG(joint, 0);
                        emcmotDebug->allHomed=0;
                    } else {
                        reportError(_("Cannot unhome inactive joint %d"),
                                joint_num);
                    }
                } else {
                    /* invalid joint number specified */
                    reportError(_("Cannot unhome invalid (joint %d) (max %d)"),
                            joint_num, (num_joints - 1));
                    return;
                }
            }
            }


            break;

        case EMCMOT_DISABLE_WATCHDOG:
            rtapi_print_msg(RTAPI_MSG_DBG, "DISABLE_WATCHDOG");
            /*! \todo Another #if 0 */
#if 0
            emcmotDebug->wdEnabling = 0;
#endif
            break;

        case EMCMOT_CLEAR_PROBE_FLAGS:
            rtapi_print_msg(RTAPI_MSG_DBG, "CLEAR_PROBE_FLAGS");
                emcmotStatus->probing = 0;
            emcmotStatus->probeTripped = 0;

            break;

        case EMCMOT_PROBE:
            /* most of this is taken from EMCMOT_SET_LINE */
            /* emcmotDebug->queue up a linear move */
            /* requires coordinated mode, enable off, not on limits */
            rtapi_print_msg(RTAPI_MSG_DBG, "PROBE");
            if (!GET_MOTION_COORD_FLAG() || !GET_MOTION_ENABLE_FLAG()) {
                reportError(
                        _("need to be enabled, in coord mode for probe move"));
                emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_COMMAND;
                SET_MOTION_ERROR_FLAG(1);
                break;
            } else if (!inRange(emcmotCommand->pos, emcmotCommand->id,
                    "Probe")) {
                emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_PARAMS;
                //tpAbort(&emcmotDebug->queue);
                sqAbort(&queue); //ghf
                SET_MOTION_ERROR_FLAG(1);
                break;
            } else if (!limits_ok()) {
                reportError(_("can't do probe move with limits exceeded"));
                emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_PARAMS;
                //tpAbort(&emcmotDebug->queue);
                sqAbort(&queue); //ghf
                SET_MOTION_ERROR_FLAG(1);
                break;
            } else if (!(emcmotCommand->probe_type & 1)) { //第0位为0,进来，表示要返回错误
                // if suppress errors = off...

                int probeval = !!*(emcmot_hal_data->probe_input);
                int probe_whenclears = !!(emcmotCommand->probe_type & 2); //第1位为0,表示碰到就停，1表示离开就停止

                if (probeval != probe_whenclears) {
                    // the probe is already in the state we're seeking.
                    if (probe_whenclears)
                        reportError(_("Probe is already clear when starting G38.4 or G38.5 move"));
                    else
                        reportError( _("Probe is already tripped when starting G38.2 or G38.3 move"));

                    emcmotStatus->commandStatus = EMCMOT_COMMAND_BAD_EXEC;
                    //tpAbort(&emcmotDebug->queue);
                    sqAbort(&queue);                //ghf
                    SET_MOTION_ERROR_FLAG(1);
                    break;
                }
            }

            /* append it to the emcmotDebug->queue */
            //tpSetId(&emcmotDebug->queue, emcmotCommand->id);
            /*if (-1 == tpAddLine(&emcmotDebug->queue, emcmotCommand->pos, emcmotCommand->motion_type, emcmotCommand->vel, emcmotCommand->ini_maxvel, emcmotCommand->acc, emcmotStatus->enables_new, 0, -1)) {*/
            sqSetFeed(&queue, emcmotCommand->vel);                //ghf
            if (-1== sqAddLine(&queue, emcmotCommand->pos, emcmotCommand->id,
                            emcmotCommand->gmodetype
                                 ,&emcmotCommand->dynamicparameter[0])) {         //ghf-2014/3/22
                reportError(_("can't add probe move"));
                emcmotStatus->commandStatus = EMCMOT_COMMAND_BAD_EXEC;
                //tpAbort(&emcmotDebug->queue);
                sqAbort(&queue);                //ghf
                SET_MOTION_ERROR_FLAG(1);
                break;
            } else {                //执行addline之后，将probing设为1
                emcmotStatus->probing = 1;
                emcmotStatus->probe_type = emcmotCommand->probe_type;
                SET_MOTION_ERROR_FLAG(0);
                /* set flag that indicates all joints need rehoming, if any
                 joint is moved in joint mode, for machines with no forward
                 kins */
                rehomeAll = 1;
            }
            break;

            /*
             case EMCMOT_RIGID_TAP:

             rtapi_print_msg(RTAPI_MSG_DBG, "RIGID_TAP");
             if (!GET_MOTION_COORD_FLAG() || !GET_MOTION_ENABLE_FLAG()) {
             reportError
             (_("need to be enabled, in coord mode for rigid tap move"));
             emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_COMMAND;
             SET_MOTION_ERROR_FLAG(1);
             break;
             } else if (!inRange(emcmotCommand->pos, emcmotCommand->id, "Rigid tap")) {
             emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_PARAMS;
             tpAbort(&emcmotDebug->queue);
             SET_MOTION_ERROR_FLAG(1);
             break;
             } else if (!limits_ok()) {
             reportError(_("can't do rigid tap move with limits exceeded"));
             emcmotStatus->commandStatus = EMCMOT_COMMAND_INVALID_PARAMS;
             tpAbort(&emcmotDebug->queue);
             SET_MOTION_ERROR_FLAG(1);
             break;
             }


             tpSetId(&emcmotDebug->queue, emcmotCommand->id);
             if (-1 == tpAddRigidTap(&emcmotDebug->queue, emcmotCommand->pos, emcmotCommand->vel, emcmotCommand->ini_maxvel, emcmotCommand->acc, emcmotStatus->enables_new)) {
             emcmotStatus->atspeed_next_feed = 0;
             reportError(_("can't add rigid tap move"));
             emcmotStatus->commandStatus = EMCMOT_COMMAND_BAD_EXEC;
             tpAbort(&emcmotDebug->queue);
             SET_MOTION_ERROR_FLAG(1);
             break;
             } else {
             SET_MOTION_ERROR_FLAG(0);
             }
             break;
             */
        case EMCMOT_SET_TELEOP_VECTOR:
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_TELEOP_VECTOR");
            if (!GET_MOTION_TELEOP_FLAG() || !GET_MOTION_ENABLE_FLAG()) {
                reportError(
                        _(
                                "need to be enabled, in teleop mode for teleop move"));
            } else {
                double velmag;
                emcmotDebug->teleop_data.desiredVel = emcmotCommand->pos;
                pmCartMag(emcmotDebug->teleop_data.desiredVel.tran, &velmag);
                if (fabs(emcmotDebug->teleop_data.desiredVel.a) > velmag) {
                    velmag = fabs(emcmotDebug->teleop_data.desiredVel.a);
                }
                if (fabs(emcmotDebug->teleop_data.desiredVel.b) > velmag) {
                    velmag = fabs(emcmotDebug->teleop_data.desiredVel.b);
                }
                if (fabs(emcmotDebug->teleop_data.desiredVel.c) > velmag) {
                    velmag = fabs(emcmotDebug->teleop_data.desiredVel.c);
                }
                if (velmag > emcmotConfig->limitVel) {
                    pmCartScalMult(emcmotDebug->teleop_data.desiredVel.tran,
                            emcmotConfig->limitVel / velmag,
                            &emcmotDebug->teleop_data.desiredVel.tran);
                    emcmotDebug->teleop_data.desiredVel.a *=
                            emcmotConfig->limitVel / velmag;
                    emcmotDebug->teleop_data.desiredVel.b *=
                            emcmotConfig->limitVel / velmag;
                    emcmotDebug->teleop_data.desiredVel.c *=
                            emcmotConfig->limitVel / velmag;
                }
                /* flag that all joints need to be homed, if any joint is
                 jogged individually later */
                rehomeAll = 1;
            }
            break;

        case EMCMOT_SET_DEBUG:
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_DEBUG");
            emcmotConfig->debug = emcmotCommand->debug;
            emcmot_config_change();
            break;

            /* needed for synchronous I/O */
        case EMCMOT_SET_AOUT:
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_AOUT");
            if (emcmotCommand->now) { //we set it right away
                emcmotAioWrite(emcmotCommand->out, emcmotCommand->minLimit);
            } else { // we put it on the TP queue, warning: only room for one in there, any new ones will overwrite
                //tpSetAout(&emcmotDebug->queue, emcmotCommand->out,
                //    emcmotCommand->minLimit, emcmotCommand->maxLimit);
            }
            break;

        case EMCMOT_SET_DOUT:
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_DOUT");
            if (emcmotCommand->now) { //we set it right away
                emcmotDioWrite(emcmotCommand->out, emcmotCommand->start);
            } else { // we put it on the TP queue, warning: only room for one in there, any new ones will overwrite
                //tpSetDout(&emcmotDebug->queue, emcmotCommand->out,
                //    emcmotCommand->start, emcmotCommand->end);
            }
            break;

        case EMCMOT_SPINDLE_ON:
            rtapi_print_msg(RTAPI_MSG_DBG, "SPINDLE_ON");
//            emcmotStatus->spindle.speed = emcmotCommand->vel*360/60*servo_period;     //总线的
            emcmotStatus->spindle.speed = emcmotCommand->vel*10000/60*servo_period;

            emcmotStatus->spindle.xoffset = emcmotCommand->acc;
            printf("Now I start to spindle on  speed:%.3f\n",emcmotStatus->spindle.speed);
            if (emcmotCommand->vel >= 0) {
                emcmotStatus->spindle.direction = 1;
            } else {
                emcmotStatus->spindle.direction = -1;
            }
            emcmotStatus->spindle.brake = 0; //disengage brake
            emcmotStatus->atspeed_next_feed = 1;

            emcmotStatus->spindle.endpos = emcmotStatus->spindle.nowpos;
            emcmotStatus->spindle.startmove = 3;
            emcmotStatus->spindle.moveType = 1;
            emcmotStatus->spindle.startcount=0;

            emcmotStatus->spindle.recordvel = 0;
            *(emcmot_hal_data->spindle_enable) = 1; //主轴开使能
//            emcmotStatus->spindle.nowpos = emcmotStatus->spindle.fbnowpos;
            break;

        case EMCMOT_SPINDLE_IGNORE://联动期间改变zdiff时此值为1
            emcmotStatus->spindle.ignoreflag = emcmotCommand->spindleignore;
            break;

        case EMCMOT_SPINDLE_OFF:
            rtapi_print_msg(RTAPI_MSG_DBG, "SPINDLE_OFF");
            if(emcmotStatus->spindle.nowvel == 0 && emcmotStatus->motion_state == EMCMOT_MOTION_FREE)
                *(emcmot_hal_data->spindle_enable) = 0;
            emcmotStatus->spindle.speed = 0;
            emcmotStatus->spindle.direction = 0;
            emcmotStatus->spindle.brake = 1; // engage brake

            emcmotStatus->spindle.stopmove = 1;
            emcmotStatus->spindle.startmove = 3;
            emcmotStatus->spindle.startcount=0;
            emcmotStatus->spindle.endpos = emcmotStatus->spindle.nowpos;

            break;

        case EMCMOT_SPINDLE_INCREASE:
            rtapi_print_msg(RTAPI_MSG_DBG, "SPINDLE_INCREASE");
            if (emcmotStatus->spindle.speed > 0) {
                emcmotStatus->spindle.speed += 100; //FIXME - make the step a HAL parameter
            } else if (emcmotStatus->spindle.speed < 0) {
                emcmotStatus->spindle.speed -= 100;
            }
            break;

        case EMCMOT_SPINDLE_DECREASE:
            rtapi_print_msg(RTAPI_MSG_DBG, "SPINDLE_DECREASE");
            if (emcmotStatus->spindle.speed > 100) {
                emcmotStatus->spindle.speed -= 100; //FIXME - make the step a HAL parameter
            } else if (emcmotStatus->spindle.speed < -100) {
                emcmotStatus->spindle.speed += 100;
            }
            break;

        case EMCMOT_SPINDLE_BRAKE_ENGAGE:
            rtapi_print_msg(RTAPI_MSG_DBG, "SPINDLE_BRAKE_ENGAGE");
            emcmotStatus->spindle.speed = 0;
            emcmotStatus->spindle.direction = 0;
            emcmotStatus->spindle.brake = 1;
            break;

        case EMCMOT_SPINDLE_BRAKE_RELEASE:
            rtapi_print_msg(RTAPI_MSG_DBG, "SPINDLE_BRAKE_RELEASE");
            emcmotStatus->spindle.brake = 0;
            break;

        case EMCMOT_SET_JOINT_COMP: //螺距补偿
            rtapi_print_msg(RTAPI_MSG_DBG, "SET_JOINT_COMP for joint %d",
                    joint_num);
            if (joint == 0) {
                break;
            }
            if (joint->comp.entries >= EMCMOT_COMP_SIZE) {
                reportError(_("joint %d: too many compensation entries"),
                        joint_num);
                break;
            }
            /* point to last entry */
            comp_entry = &(joint->comp.array[joint->comp.entries]);
            if (emcmotCommand->comp_nominal <= comp_entry[0].nominal) {
                reportError(_("joint %d: compensation values must increase"),
                        joint_num);
                break;
            }
            /* store data to new entry */
            comp_entry[1].nominal = emcmotCommand->comp_nominal;
            comp_entry[1].fwd_trim = emcmotCommand->comp_forward;
            comp_entry[1].rev_trim = emcmotCommand->comp_reverse;
            /* calculate slopes from previous entry to the new one */
            if (comp_entry[0].nominal != -DBL_MAX) {
                /* but only if the previous entry is "real" */
                tmp1 = comp_entry[1].nominal - comp_entry[0].nominal;
                comp_entry[0].fwd_slope = (comp_entry[1].fwd_trim
                        - comp_entry[0].fwd_trim) / tmp1;
                comp_entry[0].rev_slope = (comp_entry[1].rev_trim
                        - comp_entry[0].rev_trim) / tmp1;
            } else {
                /* previous entry is at minus infinity, slopes are zero */
                comp_entry[0].fwd_trim = comp_entry[1].fwd_trim;
                comp_entry[0].rev_trim = comp_entry[1].rev_trim;
            }
            joint->comp.entries++;
            break;

        case EMCMOT_SET_OFFSET:
            emcmotStatus->tool_offset = emcmotCommand->tool_offset;
            break;

        case EMCMOT_FREECHANGEOK:
            emcmotStatus->selectz = emcmotCommand->selectz;

            break;

        //ghf-2014/8/11
        case EMCMOT_DOUBLEZ:

            break;

        case EMCMOT_JOGWHEELS_GUIDE:
            emcmotStatus->jogwheels_guide = emcmotCommand->jogwheels_guide;
            //rtapi_print_msg(RTAPI_MSG_ERR,"in emcmotCommandHandler,emcmotStatus->jogwheels_guide=%d\n",emcmotStatus->jogwheels_guide);
            break;

        case EMCMOT_SETGMODE:
        {
            queue.G_mode_0.maxV = emcmotCommand->G_mode_0.maxV;
            queue.G_mode_0.maxAcc = emcmotCommand->G_mode_0.maxAcc;
            queue.G_mode_0.maxJerk = emcmotCommand->G_mode_0.maxJerk;
            //queue.G_mode_0.maxAcc_nLine = emcmotCommand->G_mode_0.maxAcc_nLine;
            queue.G_mode_0.maxAcc_nLine = emcmotCommand->G_mode_0.maxAcc;//ghf-20150504
            queue.G_mode_0.maxAcc_circle = emcmotCommand->G_mode_0.maxAcc_circle;
            queue.G_mode_0.maxAcc_nCircle = emcmotCommand->G_mode_0.maxAcc_nCircle;

            queue.G_mode_1.maxV = emcmotCommand->G_mode_1.maxV;
            queue.G_mode_1.maxAcc = emcmotCommand->G_mode_1.maxAcc;
            queue.G_mode_1.maxJerk = emcmotCommand->G_mode_1.maxJerk;
            queue.G_mode_1.maxAcc_nLine = emcmotCommand->G_mode_1.maxAcc_nLine;
            queue.G_mode_1.maxAcc_circle = emcmotCommand->G_mode_1.maxAcc_circle;
            //queue.G_mode_1.maxAcc_nCircle = emcmotCommand->G_mode_1.maxAcc_nCircle;
            queue.G_mode_1.maxAcc_nCircle = emcmotCommand->G_mode_1.maxAcc_circle;//ghf-20150504
            int i = 0;
            for(i=0;i<6;i++) {
                queue.G_mode_0.maxAngleAcc_n_Axis[i] = emcmotCommand->G_mode_0.maxAngleAcc_n_Axis[i];
                queue.G_mode_0.maxAngleV_Axis[i] = emcmotCommand->G_mode_0.maxAngleV_Axis[i];

                queue.G_mode_1.maxAngleAcc_n_Axis[i] = emcmotCommand->G_mode_1.maxAngleAcc_n_Axis[i];
                queue.G_mode_1.maxAngleV_Axis[i] = emcmotCommand->G_mode_1.maxAngleV_Axis[i];
                printf("now I get maxAngleAcc_n_Axis=%.3f ,i=%d\n",queue.G_mode_1.maxAngleAcc_n_Axis[i],i);
            }

            queue.angle1 = emcmotCommand->angle1;
            queue.angle2 = emcmotCommand->angle2;

            queue.coeff1 = emcmotCommand->coeff1;
            queue.coeff2 = emcmotCommand->coeff2;
            emcmotStatus->coeff3 = emcmotCommand->coeff3;

            /*rtapi_print_msg(RTAPI_MSG_ERR,
                    "in emcmotCommandHandler,queue.G_mode_0.maxAcc=%d\n",
                    (int) queue.G_mode_0.maxAcc);
            rtapi_print_msg(RTAPI_MSG_ERR,
                    "in emcmotCommandHandler,queue.G_mode_0.maxAcc_nLine=%d\n",
                    (int) queue.G_mode_0.maxAcc_nLine);

            rtapi_print_msg(RTAPI_MSG_ERR,
                    "in emcmotCommandHandler,queue.G_mode_1.maxAcc=%d\n",
                    (int) queue.G_mode_1.maxAcc);
            rtapi_print_msg(RTAPI_MSG_ERR,
                    "in emcmotCommandHandler,queue.G_mode_1.maxAcc_nLine=%d\n",
                    (int) queue.G_mode_1.maxAcc_nLine);*/

            break;
        }
        case EMCMOT_ENCRYPT:
            memcpy(emcmotStatus->encryptbuf, emcmotCommand->encryptbuf,
                    sizeof(emcmotStatus->encryptbuf));
            emcmotStatus->randtime = emcmotCommand->randtime;
            emcmotStatus->randbuf = emcmotCommand->randbuf;
            //rtapi_print_msg(RTAPI_MSG_ERR,"in emcmotCommandHandler,emcmotStatus->randtime=%d\n",emcmotStatus->randtime);
            //rtapi_print_msg(RTAPI_MSG_ERR,"in emcmotCommandHandler,emcmotStatus->randbuf=%d\n",emcmotStatus->randbuf);
            break;

        case EMCMOT_FILTERTIME:            //ghf-2014/5/6
            memcpy(emcmotStatus->filtertimebuf, emcmotCommand->filtertimebuf,
                    sizeof(emcmotStatus->filtertimebuf));
            break;

        case EMCMOT_SETLIGHT:
            emcmotStatus->lightvalue = emcmotCommand->setlight;
            //rtapi_print_msg(RTAPI_MSG_ERR,"in emcmotCommandHandler,emcmotStatus->lightvalue=%d\n",(int)emcmotStatus->lightvalue);
            break;

        case EMCMOT_RUNSTATUS:
            emcmotStatus->run_status = emcmotCommand->run_status;
            break;

        //ghf-2014/7/24
        case EMCMOT_SETLEFTTIME:
            for(i=0;i<4;i++) {
                emcmotStatus->wlefttime[i] = emcmotCommand->wlefttime[i];
                //rtapi_print_msg(RTAPI_MSG_ERR, "--%x\n",emcmotCommand->wlefttime[i]);
            }
            emcmotStatus->wlefttimeflag = emcmotCommand->wlefttimeflag;
            break;

        case EMCMOT_WRITEID:
            for(i=0;i<8;i++) {
                emcmotStatus->cpuid[i] = emcmotCommand->cpuid[i];
                //rtapi_print_msg(RTAPI_MSG_ERR, "--%d\n",emcmotCommand->cpuid[i]);
            }
            for(i=0;i<4;i++) {
                emcmotStatus->hddid[i] = emcmotCommand->hddid[i];
            }
            emcmotStatus->widflag = 1;
            break;

        case EMCMOT_GETRTC:



            break;

        case EMCMOT_SETIO:
            //rtapi_print_msg(RTAPI_MSG_ERR, "SETIO %d  %d",emcmotCommand->externionum,emcmotCommand->externiosta);
            emcmotStatus->externIonum = emcmotCommand->externionum;
            emcmotStatus->externIosta = emcmotCommand->externiosta;
            emcmotStatus->mot_extern_out[emcmotStatus->externIonum]
                    = emcmotStatus->externIosta;
            break;

        case EMCMOT_SETZMODE:
            emcmotStatus->zmode = emcmotCommand->zmode;
            emcmotStatus->spindlezmode = emcmotCommand->spindlezmode;
            break;

        case EMCMOT_MULTIFUNC:
            emcmotStatus->multifunctype = emcmotCommand->multifunctype;
            emcmotStatus->multifuncintval = emcmotCommand->multifuncintval;
            emcmotStatus->multifuncdoubleval = emcmotCommand->multifuncdoubleval;

            if(emcmotStatus->multifunctype == 2 ) {
                emcmotStatus->sponflag[emcmotStatus->multifuncintval] = (int)emcmotStatus->multifuncdoubleval;
                printf("now setspinle %d %d\n",emcmotStatus->multifuncintval,(int)emcmotStatus->multifuncdoubleval);
            } else if (emcmotStatus->multifunctype == 3) {
                emcmotStatus->maxdoublezpos = emcmotStatus->multifuncdoubleval;
                printf("now I get the maxdoublezpos %f\n",emcmotStatus->maxdoublezpos);
            }
            //判断是否单Z轴标志
            if(emcmotStatus->multifunctype == 5) {
                emcmotStatus->nousedoublez = emcmotCommand->multifuncintval;

            }
            if(emcmotStatus->multifunctype == 10) {
                emcmotStatus->set_which_z = emcmotCommand->multifuncintval;
                printf("now I in set which z is %d\n",emcmotStatus->set_which_z);
            }
            if(emcmotStatus->multifunctype == 20) {

                emcmotStatus->useSeclimit = emcmotStatus->multifuncdoubleval;
                printf("now sec limit flag:%f \n",emcmotStatus->useSeclimit);

            }
            if(emcmotStatus->multifunctype == 31) {
                if(emcmotStatus->motion_state == EMCMOT_MOTION_COORD) {
                    reportError("自动模式下不能切换Y轴");
                    sqAbort(&queue);
                    SET_MOTION_ERROR_FLAG(1);
                    break;
                }
                emcmotStatus->useWitchY = emcmotStatus->multifuncintval;
                printf("now prepare to switch Y %d\n",emcmotStatus->useWitchY);
                if(emcmotStatus->useWitchY == 0) {
                    emcmotStatus->carte_pos_cmd.tran.y=joints[1].pos_cmd;
                } else {
                    emcmotStatus->carte_pos_cmd.tran.y=joints[6].pos_cmd;
                }
                emcmotStatus->carte_pos_cmd.tran.x=joints[0].pos_cmd;
                emcmotStatus->carte_pos_cmd.tran.z=joints[2].pos_cmd;
                emcmotStatus->carte_pos_cmd.a=joints[3].pos_cmd;
                emcmotStatus->carte_pos_cmd.b=joints[4].pos_cmd;
                emcmotStatus->carte_pos_cmd.c=joints[5].pos_cmd;
                sqSetPos(&queue, emcmotStatus->carte_pos_cmd);
                int j=0;
                for(j=0;j<num_joints;j++) {
                    cubicDrain(&(joints[0].cubic));
                }
            }
            if(emcmotStatus->multifunctype == 39) {
                emcmotStatus->spindle.startcount=0;
                emcmotStatus->spindle.startmove = 1;
                *(emcmot_hal_data->spindle_enable) = 1; //主轴开使能
                emcmotStatus->spindle.moveType = 0;
                if(emcmotStatus->multifuncdoubleval == 0) {
                    if(fabs(fmod(emcmotStatus->spindle.nowpos,360)) > 180) {
                        if(emcmotStatus->spindle.nowpos > 0)
                            emcmotStatus->spindle.endpos = emcmotStatus->spindle.nowpos + (360-fabs(fmod(emcmotStatus->spindle.nowpos,360)));
                        else
                            emcmotStatus->spindle.endpos = emcmotStatus->spindle.nowpos - (360-fabs(fmod(emcmotStatus->spindle.nowpos,360)));
                    } else {
                            emcmotStatus->spindle.endpos = emcmotStatus->spindle.nowpos - fmod(emcmotStatus->spindle.nowpos,360);
                    }
                } else {
                    emcmotStatus->spindle.endpos += emcmotStatus->multifuncdoubleval;
                }
                printf("now I start to set endpos:%.3f  nowpos:%.3f\n",emcmotStatus->spindle.endpos,emcmotStatus->spindle.nowpos);
            }

            /********
             *运动学转化参数
             ********/
            if(emcmotStatus->multifunctype == 50) {
               // kinedata.method = emcmotCommand->multifuncintval;
            }
            if(emcmotStatus->multifunctype == 51) {
                kinedata.a_length = emcmotCommand->multifuncdoubleval;
            }
            if(emcmotStatus->multifunctype == 52) {
                kinedata.method = emcmotCommand->multifuncintval;
                printf("now I set method is %d \n",kinedata.method);
            }
            //工件零点
            if(emcmotStatus->multifunctype == 55) {
                kinedata.offset_x = emcmotCommand->multifuncdoubleval;
            }
            if(emcmotStatus->multifunctype == 56) {
                kinedata.offset_y = emcmotCommand->multifuncdoubleval;
            }
            if(emcmotStatus->multifunctype == 57) {
                kinedata.offset_z = emcmotCommand->multifuncdoubleval;
            }
            //设置轴圆弧最短路径(最大值不超过360)
            if(emcmotStatus->multifunctype == 60) {
                joints[emcmotStatus->multifuncintval].circle_shortest = (int)emcmotCommand->multifuncdoubleval;
                kinedata.circle_type[emcmotStatus->multifuncintval] = (int)emcmotCommand->multifuncdoubleval;
            }
            if(emcmotStatus->multifunctype == 70) {
                   double nowpos = fmod(joints[3].program_cmd,360.0-1e-4);
                   if(nowpos > 0) {
                           if(nowpos > 180) {
                               nowpos = -360+nowpos;
                            }
                     } else {
                         if(nowpos < -180) {
                              nowpos = 360+nowpos;
                          }
                     }
                    double offset = joints[3].home_offset - joints[3].pos_fb + nowpos ;
                    printf("Now I in recalculate the A pos  %.3f  offset: %.3f\n",nowpos,offset);
                    joints[3].pos_cmd += offset;
                    joints[3].coarse_pos += offset;
                    joints[3].pos_fb += offset;
                    joints[3].free_pos_cmd += offset;
                    joints[3].motor_offset -= offset;
                    emcmotStatus->carte_pos_cmd.a=joints[3].pos_cmd;
                    sqSetPos(&queue, emcmotStatus->carte_pos_cmd);
                    int i=0;
                    for(i=0;i<num_joints;i++) {
                        cubicDrain(&(joints[i].cubic));
                    }
             }
            if(emcmotStatus->multifunctype == 71) {
                int axis = emcmotStatus->multifuncintval;
                SET_JOINT_HOMED_FLAG(&joints[axis], 1);
                double offset =emcmotStatus->multifuncdoubleval - joints[axis].pos_cmd;
                joints[axis].pos_cmd += offset;
                joints[axis].coarse_pos += offset;
                joints[axis].pos_fb += offset;
                joints[axis].free_pos_cmd += offset;
                joints[axis].motor_offset -= offset;
                joints[axis].old_pos_cmd = joints[axis].pos_cmd;
            }
            break;

        default:
            rtapi_print_msg(RTAPI_MSG_DBG, "UNKNOWN");
            reportError(_("unrecognized command %d"), emcmotCommand->command);
            emcmotStatus->commandStatus = EMCMOT_COMMAND_UNKNOWN_COMMAND;
            break;

        } /* end of: command switch */

        rtapi_print_msg(RTAPI_MSG_DBG, "\n");
        /* synch tail count */
        emcmotStatus->tail = emcmotStatus->head;
        emcmotConfig->tail = emcmotConfig->head;
        emcmotDebug->tail = emcmotDebug->head;

    }
    /* end of: if-new-command */
    check_stuff("after command_handler()");

    return;
}
