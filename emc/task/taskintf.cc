/********************************************************************
* Description: taskintf.cc
*   Interface functions for motion.
*
*   Derived from a work by Fred Proctor & Will Shackleford
*
* Author:
* License: GPL Version 2
* System: Linux
*    
* Copyright (c) 2004 All rights reserved.
*
********************************************************************/

#include <math.h>		// isnan()
#include <float.h>		// DBL_MAX
#include <string.h>		// memcpy() strncpy()
#include <unistd.h>             // unlink()

#include "usrmotintf.h"		// usrmotInit(), usrmotReadEmcmotStatus(),
				// etc.
#include "motion.h"		// emcmot_command_t,STATUS, etc.
#include "motion_debug.h"
#include "emc.hh"
#include "emcglb.h"		// EMC_INIFILE
#include "emc_nml.hh"
#include "rcs_print.hh"
#include "timer.hh"
#include "inifile.hh"
#include "iniaxis.hh"
#include "initraj.hh"

/* define this to catch isnan errors, for rtlinux FPU register 
   problem testing */
#define ISNAN_TRAP

// MOTION INTERFACE

/*! \todo FIXME - this decl was originally much later in the file, moved
here temporarily for debugging */
static emcmot_status_t emcmotStatus;//此定义在motion.h中//ghf-2014/2/21
static emcdata_struct motdata;
/*
  Implementation notes:


  Initing:  the emcmot interface needs to be inited once, but nml_traj_init()
  and nml_servo_init() can be called in any order. Similarly, the emcmot
  interface needs to be exited once, but nml_traj_exit() and nml_servo_exit()
  can be called in any order. They can also be called multiple times. Flags
  are used to signify if initing has been done, or if the final exit has
  been called.
  */

static emcmot_command_t emcmotCommand;

static int emcmotTrajInited = 0;	// non-zero means traj called init
/* FIXME is this supposed to be axes, or joints? */
static int emcmotAxisInited[EMCMOT_MAX_JOINTS] = { 0 };	// non-zero means axis called init

__attribute__ ((unused))
static int emcmotIoInited = 0;	// non-zero means io called init
static int emcmotion_initialized = 0;	// non-zero means both
						// emcMotionInit called.

// EMC_AXIS functions

// local status data, not provided by emcmot
static unsigned long localMotionHeartbeat = 0;
static int localMotionCommandType = 0;
static int localMotionEchoSerialNumber = 0;

/* FIXME axes or joints? */
static unsigned char localEmcAxisAxisType[EMCMOT_MAX_JOINTS];
static double localEmcAxisUnits[EMCMOT_MAX_JOINTS];
static double localEmcMaxAcceleration = DBL_MAX;

// axes are numbered 0..NUM-1

/*
  In emcmot, we need to set the cycle time for traj, and the interpolation
  rate, in any order, but both need to be done. 
 */
/* FIXME: all of this stuff is really JOINTS not AXES!!! */

int emcAxisSetAxis(int axis, unsigned char axisType)
{
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    localEmcAxisAxisType[axis] = axisType;

    return 0;
}

int emcAxisSetUnits(int axis, double units)
{
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    localEmcAxisUnits[axis] = units;

    return 0;
}

int emcAxisSetBacklash(int axis, double backlash)
{
#ifdef ISNAN_TRAP
    if (isnan(backlash)) {
	printf("isnan error in emcAxisSetBacklash()\n");
	return -1;
    }
#endif

    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_SET_BACKLASH;
    emcmotCommand.axis = axis;
    emcmotCommand.backlash = backlash;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

// saved values of limits, since emcmot expects them to be set in
// pairs and we set them individually.
static double saveMinLimit[EMCMOT_MAX_JOINTS];
static double saveMaxLimit[EMCMOT_MAX_JOINTS];

int emcAxisSetSecLimit(int axis, double maxpos,double minpos,double offset)
{

    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
    return 0;
    }

    emcmotCommand.command = EMCMOT_SET_SEC_LIMITS;
    emcmotCommand.axis = axis;
    emcmotCommand.secmaxLimit = maxpos;
    emcmotCommand.secminLimit = minpos;
    emcmotCommand.secLimitoffset = offset;


    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisSetMinPositionLimit(int axis, double limit)
{
#ifdef ISNAN_TRAP
    if (isnan(limit)) {
	printf("isnan error in emcAxisSetMinPosition()\n");
	return -1;
    }
#endif

    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_SET_POSITION_LIMITS;
    emcmotCommand.axis = axis;
    emcmotCommand.maxLimit = saveMaxLimit[axis];
    emcmotCommand.minLimit = limit;
    saveMinLimit[axis] = limit;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisSetMaxPositionLimit(int axis, double limit)
{
#ifdef ISNAN_TRAP
    if (isnan(limit)) {
	printf("isnan error in emcAxisSetMaxPosition()\n");
	return -1;
    }
#endif

    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_SET_POSITION_LIMITS;
    emcmotCommand.axis = axis;
    emcmotCommand.minLimit = saveMinLimit[axis];
    emcmotCommand.maxLimit = limit;
    saveMaxLimit[axis] = limit;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisSetMotorOffset(int axis, double offset) 
{
#ifdef ISNAN_TRAP
    if (isnan(offset)) {
	printf("isnan error in emcAxisSetMotorOffset()\n");
	return -1;
    }
#endif

    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }
    emcmotCommand.command = EMCMOT_SET_MOTOR_OFFSET;
    emcmotCommand.axis = axis;
    emcmotCommand.motor_offset = offset;
    
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisSetFerror(int axis, double ferror)
{
#ifdef ISNAN_TRAP
    if (isnan(ferror)) {
	printf("isnan error in emcAxisSetFerror()\n");
	return -1;
    }
#endif

    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_SET_MAX_FERROR;
    emcmotCommand.axis = axis;
    emcmotCommand.maxFerror = ferror;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisSetMinFerror(int axis, double ferror)
{
#ifdef ISNAN_TRAP
    if (isnan(ferror)) {
	printf("isnan error in emcAxisSetMinFerror()\n");
	return -1;
    }
#endif

    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }
    emcmotCommand.command = EMCMOT_SET_MIN_FERROR;
    emcmotCommand.axis = axis;
    emcmotCommand.minFerror = ferror;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisSetHomingParams(int axis, double home, double offset, double home_final_vel,
			   double search_vel, double latch_vel,
			   int use_index, int ignore_limits, int is_shared,
               int sequence,int volatile_home, int locking_indexer,double pitchvalue,
               double fric_delay_t,double fric_lag_ts0,double fric_lag_ts1,
                           double fric_max_u0,double fric_max_u1,double fric_type,double checkdistance)//ghf-2014/5/29
{
#ifdef ISNAN_TRAP
    if (isnan(home) || isnan(offset) || isnan(home_final_vel) ||
	isnan(search_vel) || isnan(latch_vel)) {
	printf("isnan error in emcAxisSetHoming()\n");
	return -1;
    }
#endif

    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_SET_HOMING_PARAMS;
    emcmotCommand.axis = axis;
    emcmotCommand.home = home;
    emcmotCommand.offset = offset;
    emcmotCommand.home_final_vel = home_final_vel;
    emcmotCommand.search_vel = search_vel;
    emcmotCommand.latch_vel = latch_vel;
    emcmotCommand.flags = 0;
    emcmotCommand.home_sequence = sequence;
    emcmotCommand.volatile_home = volatile_home;  
    
    if(checkdistance != 0)
        emcmotCommand.distancecheck = checkdistance;
    else
        emcmotCommand.distancecheck = pitchvalue;
    emcmotCommand.pitchvalue = pitchvalue;
    emcmotCommand.fric_delay_t = fric_delay_t;
    emcmotCommand.fric_lag_ts0 = fric_lag_ts0;
    emcmotCommand.fric_lag_ts1 = fric_lag_ts1;
    emcmotCommand.fric_max_u0 = fric_max_u0;
    emcmotCommand.fric_max_u1 = fric_max_u1;
    emcmotCommand.fric_type = fric_type;
      
    if (use_index) {
	emcmotCommand.flags |= HOME_USE_INDEX;
    }
    if (ignore_limits) {
	emcmotCommand.flags |= HOME_IGNORE_LIMITS;
    }
    if (is_shared) {
	emcmotCommand.flags |= HOME_IS_SHARED;
    }
    if (locking_indexer) {
        emcmotCommand.flags |= HOME_UNLOCK_FIRST;
    }

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisSetMaxVelocity(int axis, double vel,int useposfbflag,int useabs)
{
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    if (vel < 0.0) {
	vel = 0.0;
    }

    AXIS_MAX_VELOCITY[axis] = vel;

    emcmotCommand.command = EMCMOT_SET_JOINT_VEL_LIMIT;
    emcmotCommand.axis = axis;
    emcmotCommand.vel = vel;
    emcmotCommand.axisuseposfb = useposfbflag;
    emcmotCommand.useabspos = useabs;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisSetMaxAcceleration(int axis, double acc)
{

    if (axis < 0 || axis >= EMC_AXIS_MAX) {
	return 0;
    }
    if (acc < 0.0) {
	acc = 0.0;
    }
    AXIS_MAX_ACCELERATION[axis] = acc;
    emcmotCommand.command = EMCMOT_SET_JOINT_ACC_LIMIT;
    emcmotCommand.axis = axis;
    emcmotCommand.acc = acc;
    printf("now set maxacc is %f  axis:%d\n",acc,axis);
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

/* This function checks to see if any axis or the traj has
   been inited already.  At startup, if none have been inited,
   usrmotIniLoad and usrmotInit must be called first.  At
   shutdown, after all have been halted, the usrmotExit must
   be called.
*/

static int AxisOrTrajInited(void)
{
    int axis;

    for (axis = 0; axis < EMCMOT_MAX_JOINTS; axis++) {
	if (emcmotAxisInited[axis]) {
	    return 1;
	}
    }
    if (emcmotTrajInited) {
	return 1;
    }
    return 0;
}

int emcAxisInit(int axis)
{
    int retval = 0;
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }
    // init emcmot interface
    //usleep(1000*50);
    if (!AxisOrTrajInited()) {
	usrmotIniLoad(EMC_INIFILE);
	if (0 != usrmotInit("emc2_task")) {
	    return -1;
	}
    }
    emcmotAxisInited[axis] = 1;
    //usleep(1000*50);
    if (0 != iniAxis(axis, EMC_INIFILE)) {
	retval = -1;
    }
    return retval;
}

int emcAxisHalt(int axis)
{
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }
    /*! \todo FIXME-- refs global emcStatus; should make EMC_AXIS_STAT an arg here */
    if (NULL != emcStatus && emcmotion_initialized
	&& emcmotAxisInited[axis]) {
	dumpAxis(axis, EMC_INIFILE, &emcStatus->motion.axis[axis]);
    }
    emcmotAxisInited[axis] = 0;

    if (!AxisOrTrajInited()) {
	usrmotExit();		// ours is final exit
    }

    return 0;
}

int emcAxisAbort(int axis)
{
    if (axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }
    emcmotCommand.command = EMCMOT_AXIS_ABORT;
    emcmotCommand.axis = axis;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisActivate(int axis)
{
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_ACTIVATE_JOINT;
    emcmotCommand.axis = axis;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisDeactivate(int axis)
{
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_DEACTIVATE_JOINT;
    emcmotCommand.axis = axis;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisOverrideLimits(int axis)
{
    // can have axis < 0, for resuming normal limit checking
    if (axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_OVERRIDE_LIMITS;
    emcmotCommand.axis = axis;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisEnable(int axis)
{
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_ENABLE_AMPLIFIER;
    emcmotCommand.axis = axis;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisDisable(int axis)
{
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_DISABLE_AMPLIFIER;
    emcmotCommand.axis = axis;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisHome(int axis,int flag)
{
    if (axis < -1 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    emcmotCommand.command = EMCMOT_HOME;
    emcmotCommand.axis = axis;
    emcmotCommand.homeflag = flag;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisUnhome(int axis,int flag)
{
	if (axis < -2 || axis >= EMCMOT_MAX_JOINTS) {
		return 0;
    }

	emcmotCommand.command = EMCMOT_UNHOME;
	emcmotCommand.axis = axis;
    emcmotCommand.homeflag = flag;

	return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisJog(int axis, double vel)
{
    if (axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    int pickvel = axis;
    if(axis == -1)
        pickvel = 2;
    if (vel > AXIS_MAX_VELOCITY[pickvel]) {
    vel = AXIS_MAX_VELOCITY[pickvel];
    } else if (vel < -AXIS_MAX_VELOCITY[pickvel]) {
    vel = -AXIS_MAX_VELOCITY[pickvel];
    }

    emcmotCommand.command = EMCMOT_JOG_CONT;
    emcmotCommand.axis = axis;
    emcmotCommand.vel = vel;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisIncrJog(int axis, double incr, double vel)
{
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    if (vel > AXIS_MAX_VELOCITY[axis]) {
	vel = AXIS_MAX_VELOCITY[axis];
    } else if (vel < -AXIS_MAX_VELOCITY[axis]) {
	vel = -AXIS_MAX_VELOCITY[axis];
    }

    emcmotCommand.command = EMCMOT_JOG_INCR;
    emcmotCommand.axis = axis;
    emcmotCommand.vel = vel;
    emcmotCommand.offset = incr;


    if(emcStatus->motion.doublez.z_changing == true){
        //printf("now incr = %.3f\n",incr);
        //usleep(100000);
    }
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisAbsJog(int axis, double pos, double vel)
{
    if (axis < 0 || axis >= EMCMOT_MAX_JOINTS) {
	return 0;
    }

    if (vel > AXIS_MAX_VELOCITY[axis]) {
	vel = AXIS_MAX_VELOCITY[axis];
    } else if (vel < -AXIS_MAX_VELOCITY[axis]) {
	vel = -AXIS_MAX_VELOCITY[axis];
    }

    emcmotCommand.command = EMCMOT_JOG_ABS;
    emcmotCommand.axis = axis;
    emcmotCommand.vel = vel;
    emcmotCommand.offset = pos;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcAxisLoadComp(int axis, const char *file, int type)
{
    return usrmotLoadComp(axis, file, type);
}

static emcmot_config_t emcmotConfig;
int get_emcmot_debug_info = 0;

/*
  these globals are set in emcMotionUpdate(), then referenced in
  emcAxisUpdate(), emcTrajUpdate() to save calls to usrmotReadEmcmotStatus
 */
static emcmot_debug_t emcmotDebug;
static char errorString[EMCMOT_ERROR_LEN];
static int new_config = 0;

/*! \todo FIXME - debugging - uncomment the following line to log changes in
   AXIS_FLAG */
// #define WATCH_FLAGS 1

int emcAxisUpdate(EMC_AXIS_STAT stat[], int numAxes)
{
/*! \todo FIXME - this function accesses data that has been
   moved.  Once I know what it is used for I'll fix it */

    int axis;
    emcmot_joint_status_t *joint;
#ifdef WATCH_FLAGS
    static int old_joint_flag[8];
#endif

    // check for valid range
    if (numAxes <= 0 || numAxes > EMCMOT_MAX_JOINTS) {
	return -1;
    }

    for (axis = 0; axis < numAxes; axis++) {
	/* point to joint data */

	joint = &(emcmotStatus.joint_status[axis]);

	stat[axis].axisType = localEmcAxisAxisType[axis];
	stat[axis].units = localEmcAxisUnits[axis];
	if (new_config) {
	    stat[axis].backlash = joint->backlash;
	    stat[axis].minPositionLimit = joint->min_pos_limit;
	    stat[axis].maxPositionLimit = joint->max_pos_limit;
        stat[axis].secNegLimit = joint->sec_neg_limit;
        stat[axis].seclimitOffset = joint->sec_limit_offset;
        stat[axis].secPosLimit = joint->sec_pos_limit;
	    stat[axis].minFerror = joint->min_ferror;
	    stat[axis].maxFerror = joint->max_ferror;
	}
    stat[axis].output = joint->pos_cmd;
    //stat[axis].output = joint->coarse_pos;//切到z2模式时，此值会在0和非0间跳ghf-2014/8/11

    stat[axis].input = joint->pos_fb;//这个是带螺距补偿值的
    stat[axis].motor_fbpos = joint->motor_fbpos;
    stat[axis].backlash_filt = joint->backlash_filt;//ghf-2014/7/29,坐标值显示时，output-此值
	
    //printf("stat[%d].input=%f\n",axis,stat[axis].input);
	

	stat[axis].velocity = joint->vel_cmd;
    stat[axis].ferrorCurrent = joint->ferror;
    stat[axis].ferrorHighMark = joint->ferror_high_mark;
    stat[axis].errorcode = joint->errorCode;


	stat[axis].homing = (joint->flag & EMCMOT_JOINT_HOMING_BIT ? 1 : 0);
	stat[axis].homed = (joint->flag & EMCMOT_JOINT_HOMED_BIT ? 1 : 0);

	stat[axis].homeswitch = (joint->flag & EMCMOT_JOINT_HOME_SWITCH_BIT ? 1 : 0);//ghf-2013/3/13
    //printf("stat[%d].homing=%d\n",axis,stat[axis].homing);
	
	stat[axis].fault = (joint->flag & EMCMOT_JOINT_FAULT_BIT ? 1 : 0);
	stat[axis].enabled = (joint->flag & EMCMOT_JOINT_ENABLE_BIT ? 1 : 0);
	stat[axis].inpos = (joint->flag & EMCMOT_JOINT_INPOS_BIT ? 1 : 0);

/* FIXME - soft limits are now applied to the command, and should never
   happen */
	stat[axis].minSoftLimit = 0;
	stat[axis].maxSoftLimit = 0;
	stat[axis].minHardLimit =
	    (joint->flag & EMCMOT_JOINT_MIN_HARD_LIMIT_BIT ? 1 : 0);
	stat[axis].maxHardLimit =
	    (joint->flag & EMCMOT_JOINT_MAX_HARD_LIMIT_BIT ? 1 : 0);
	stat[axis].overrideLimits = !!(emcmotStatus.overrideLimitMask);	// one
	// for
    // all

/*! \todo Another #if 0 */
#if 0				/*! \todo FIXME - per-axis Vscale temporarily? removed */
	stat[axis].scale = emcmotStatus.axVscale[axis];
#endif
#ifdef WATCH_FLAGS
	if (old_joint_flag[axis] != joint->flag) {
	    printf("(joint %d) flag: %04X -> %04X\n", axis,
		   old_joint_flag[axis], joint->flag);
	    old_joint_flag[axis] = joint->flag;
	}
#endif
	if (joint->flag & EMCMOT_JOINT_ERROR_BIT) {
	    if (stat[axis].status != RCS_ERROR) {
		rcs_print_error("Error on axis %d, command number %d\n",
				axis, emcmotStatus.commandNumEcho);
		stat[axis].status = RCS_ERROR;
	    }
	} else if (joint->flag & EMCMOT_JOINT_INPOS_BIT) {
	    stat[axis].status = RCS_DONE;
	} else {
	    stat[axis].status = RCS_EXEC;
	}
    }
    return 0;
}

// EMC_TRAJ functions

// local status data, not provided by emcmot

static int localEmcTrajAxisMask = 0;
static double localEmcTrajLinearUnits = 1.0;
static double localEmcTrajAngularUnits = 1.0;
static int localEmcTrajMotionId = 0;

int emcTrajSetAxes(int axes, int axismask)
{
    if(axes == 0) {
	if(axismask & 256) axes = 9;
	else if(axismask & 128) axes = 8;
	else if(axismask & 64) axes = 7;
	else if(axismask & 32) axes = 6;
	else if(axismask & 16) axes = 5;
	else if(axismask & 8) axes = 4;
	else if(axismask & 4) axes = 3;
	else if(axismask & 2) axes = 2;
	else if(axismask & 1) axes = 1;
    }
    if (axes <= 0 || axes > EMCMOT_MAX_JOINTS || axismask >= (1<<axes)) {
	rcs_print("emcTrajSetAxes failing: axes=%d axismask=%x\n",
		axes, axismask);
	return -1;
    }

    localEmcTrajAxes = axes;
    localEmcTrajAxisMask = axismask;
    emcmotCommand.command = EMCMOT_SET_NUM_AXES;
    emcmotCommand.axis = axes;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetUnits(double linearUnits, double angularUnits)
{
    if (linearUnits <= 0.0 || angularUnits <= 0.0) {
	return -1;
    }

    localEmcTrajLinearUnits = linearUnits;
    localEmcTrajAngularUnits = angularUnits;

    return 0;
}

int emcTrajSetMode(int mode)
{
   // printf("now set mode %d\n",mode);
    switch (mode) {
    case EMC_TRAJ_MODE_FREE:
	emcmotCommand.command = EMCMOT_FREE;
	return usrmotWriteEmcmotCommand(&emcmotCommand);

    case EMC_TRAJ_MODE_COORD:
	emcmotCommand.command = EMCMOT_COORD;
	return usrmotWriteEmcmotCommand(&emcmotCommand);

    case EMC_TRAJ_MODE_TELEOP:
	emcmotCommand.command = EMCMOT_TELEOP;
	return usrmotWriteEmcmotCommand(&emcmotCommand);

    default:
	return -1;
    }
}

int emcTrajSetTeleopVector(EmcPose vel)
{
    emcmotCommand.command = EMCMOT_SET_TELEOP_VECTOR;
    emcmotCommand.pos = vel;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetVelocity(double vel, double ini_maxvel)
{
    int retval;

    if (vel < 0.0) {
	vel = 0.0;
    } else if (vel > TRAJ_MAX_VELOCITY) {
	vel = TRAJ_MAX_VELOCITY;
    }

    if (ini_maxvel < 0.0) {
	    ini_maxvel = 0.0;
    } else if (vel > TRAJ_MAX_VELOCITY) {
	    ini_maxvel = TRAJ_MAX_VELOCITY;
    }

    emcmotCommand.command = EMCMOT_SET_VEL;
    emcmotCommand.vel = vel;
    emcmotCommand.ini_maxvel = ini_maxvel;

    retval = usrmotWriteEmcmotCommand(&emcmotCommand);

    return retval;
}

int emcTrajSetAcceleration(double acc)
{
    if (acc < 0.0) {
	acc = 0.0;
    } else if (acc > localEmcMaxAcceleration) {
	acc = localEmcMaxAcceleration;
    }

    emcmotCommand.command = EMCMOT_SET_ACC;
    emcmotCommand.acc = acc;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

/*
  emcmot has no limits on max velocity, acceleration so we'll save them
  here and apply them in the functions above
  */
int emcTrajSetMaxVelocity(double vel)
{
    if (vel < 0.0) {
	vel = 0.0;
    }

    TRAJ_MAX_VELOCITY = vel;

    emcmotCommand.command = EMCMOT_SET_VEL_LIMIT;
    emcmotCommand.vel = vel;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetMaxAcceleration(double acc)
{
    if (acc < 0.0) {
	acc = 0.0;
    }

    localEmcMaxAcceleration = acc;

    return 0;
}

int emcMotSetPar(int *intpar,double *doupar) {
    emcmotCommand.command = EMCMOT_SET_PAR;
    for(int i=0;i<50;i++) {
        emcmotCommand.intmotpar[i] = intpar[i];
        emcmotCommand.doumotpar[i] = doupar[i];
    }
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetHome(EmcPose home)
{
#ifdef ISNAN_TRAP
    if (isnan(home.tran.x) || isnan(home.tran.y) || isnan(home.tran.z) ||
	isnan(home.a) || isnan(home.b) || isnan(home.c) ||
	isnan(home.u) || isnan(home.v) || isnan(home.w)) {
	printf("isnan error in emcTrajSetHome()\n");
	return 0;		// ignore it for now, just don't send it
    }
#endif

    emcmotCommand.command = EMCMOT_SET_WORLD_HOME;
    emcmotCommand.pos = home;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetScale(double scale)
{
    if (scale < 0.0) {
	scale = 0.0;
    }

    emcmotCommand.command = EMCMOT_FEED_SCALE;
    emcmotCommand.scale = scale;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetSpindleScale(double scale)
{
    if (scale < 0.0) {
	scale = 0.0;
    }

    emcmotCommand.command = EMCMOT_SPINDLE_SCALE;
    emcmotCommand.scale = scale;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetG0Enable(int mode) {
    emcmotCommand.command = EMCMOT_FG0_ENABLE;
    emcmotCommand.g0enableflag = mode;   //hong-2014/11/06 = mode;
    //printf("now send mode = %d\n",mode);
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetFOEnable(unsigned char mode)
{
    emcmotCommand.command = EMCMOT_FS_ENABLE;
    emcmotCommand.mode = mode;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetFHEnable(unsigned char mode)
{
    emcmotCommand.command = EMCMOT_FH_ENABLE;
    emcmotCommand.mode = mode;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetSOEnable(unsigned char mode)
{
    emcmotCommand.command = EMCMOT_SS_ENABLE;
    emcmotCommand.mode = mode;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetAFEnable(unsigned char enable)
{
    emcmotCommand.command = EMCMOT_AF_ENABLE;

    if ( enable ) {
	emcmotCommand.flags = 1;
    } else {
	emcmotCommand.flags = 0;
    }
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetMotionId(int id)
{

    if (EMC_DEBUG_MOTION_TIME & EMC_DEBUG) {
	if (id != localEmcTrajMotionId) {
	    rcs_print("Outgoing motion id is %d.\n", id);
	}
    }

    localEmcTrajMotionId = id;

    return 0;
}

int emcTrajInit()
{
    int retval = 0;

    // init emcmot interface
    if (!AxisOrTrajInited()) {
	usrmotIniLoad(EMC_INIFILE);
	if (0 != usrmotInit("emc2_task")) {
	    return -1;
	}
    }
    emcmotTrajInited = 1;
    // initialize parameters from ini file
    if (0 != iniTraj(EMC_INIFILE)) {
	retval = -1;
    }
    return retval;
}

int emcTrajHalt()
{
    emcmotTrajInited = 0;

    if (!AxisOrTrajInited()) {
	usrmotExit();		// ours is final exit
    }

    return 0;
}

int emcTrajEnable()
{
    emcmotCommand.command = EMCMOT_ENABLE;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajDisable()
{
    emcmotCommand.command = EMCMOT_DISABLE;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajAbort()
{    
    emcmotCommand.command = EMCMOT_ABORT;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajPause()
{
    emcmotCommand.command = EMCMOT_PAUSE;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajStep()
{
    emcmotCommand.command = EMCMOT_STEP;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajResume()
{
    emcmotCommand.command = EMCMOT_RESUME;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajDelay(double delay)
{
    /* nothing need be done here - it's done in task controller */

    return 0;
}

double emcTrajGetLinearUnits()
{
    return localEmcTrajLinearUnits;
}

double emcTrajGetAngularUnits()
{
    return localEmcTrajAngularUnits;
}

int emcTrajSetOffset(EmcPose tool_offset)
{
    emcmotCommand.command = EMCMOT_SET_OFFSET;
    emcmotCommand.tool_offset = tool_offset;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetSpindleSync(double fpr, int wait_for_index)
{
    emcmotCommand.command = EMCMOT_SET_SPINDLESYNC;
    emcmotCommand.spindlesync = fpr;
    emcmotCommand.flags = wait_for_index;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajSetTermCond(int cond, double tolerance)
{
    emcmotCommand.command = EMCMOT_SET_TERM_COND;
    emcmotCommand.termCond =
	(cond ==
	 EMC_TRAJ_TERM_COND_STOP ? EMCMOT_TERM_COND_STOP :
	 EMCMOT_TERM_COND_BLEND);
    emcmotCommand.tolerance = tolerance;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

//ghf-2014/6/7
//hong-2014/10/25 add the parameter tool
int emcTrajminiLinearMove(EmcPose end, int type, double vel, double ini_maxvel, double acc,
                      int indexrotary, int gmodetype, double *dynamicpar)
{
#ifdef ISNAN_TRAP
    if (isnan(end.tran.x) || isnan(end.tran.y) || isnan(end.tran.z)) {
        printf("x = %lf y = %lf z = %lf\n",
                   end.tran.x,
                   end.tran.y,
                   end.tran.z);
	printf("isnan error in emcTrajminiLinearMove()\n");
    return -1;		// ignore it for now, just don't send it
    }
#endif
    if(emcStatus->task.parameters[3999] > 0) {
        if(emcStatus->task.parameters[3999] == 1 || emcStatus->task.parameters[3999] == 6) {
            end.b = 0;
            end.c = 0;
        } else if(emcStatus->task.parameters[3999] == 2){
            end.c = 0;
        } else if(emcStatus->task.parameters[3999] == 3){
            end.b = 0;
        } else if(emcStatus->task.parameters[3999] == 4 || emcStatus->task.parameters[3999] == 5 || emcStatus->task.parameters[3999] == 7) {
            end.a = 0;
//            end.b = 0;
            end.c = 0;
        }
    }

    emcmotCommand.command = EMCMOT_SET_LINE;
    emcmotCommand.pos = end;
    emcmotCommand.id = indexrotary;

    if (EMC_DEBUG & EMC_DEBUG_SENDPOINT) {
        printf("miniLin::id=%d  x = %lf y = %lf z = %lf a = %lf b=%lf c=%lf u=%lf v=%lf w=%lf vel = %lf mode:%d\n",
               emcmotCommand.id,
                       end.tran.x,
                       end.tran.y,
                       end.tran.z,end.a,end.b,end.c,
               end.u,end.v,end.w,vel,gmodetype);
    }
    emcmotCommand.motion_type = type;
    emcmotCommand.vel = vel;
    emcmotCommand.ini_maxvel = ini_maxvel;
    emcmotCommand.acc = acc;
    emcmotCommand.turn = 0;
    emcmotCommand.gmodetype = gmodetype;//ghf-2014/3/22

    for(int i=0;i<DYNAMIC_PAR_NUM;i++)
        emcmotCommand.dynamicparameter[i] = dynamicpar[i];



    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

//hong-2014/09/01
int emcTrajminiCircularMove(EmcPose end, PM_CARTESIAN center,
            PM_CARTESIAN normal, int turn, int type, double vel,
                            double ini_maxvel, double acc,int lineid,double *dynamicpar)
{
#ifdef ISNAN_TRAP
    if (isnan(end.tran.x) || isnan(end.tran.y) || isnan(end.tran.z) ||
    isnan(end.a) || isnan(end.b) || isnan(end.c) ||
    isnan(end.u) || isnan(end.v) || isnan(end.w) ||
    isnan(center.x) || isnan(center.y) || isnan(center.z) ||
    isnan(normal.x) || isnan(normal.y) || isnan(normal.z)) {
    printf("isnan error in emcTrajCircularMove()\n");
    return -1;		// ignore it for now, just don't send it
    }
#endif

    if(emcStatus->task.parameters[3999] > 0) {
        if(emcStatus->task.parameters[3999] == 1 || emcStatus->task.parameters[3999] == 6) {
            end.b = 0;
            end.c = 0;
        } else if(emcStatus->task.parameters[3999] == 2){
            end.c = 0;
        } else if(emcStatus->task.parameters[3999] == 3){
            end.b = 0;
        } else if(emcStatus->task.parameters[3999] == 4 || emcStatus->task.parameters[3999] == 5 || emcStatus->task.parameters[3999] == 7){
            end.a = 0;
//            end.b = 0;
            end.c = 0;
        }
    }

    emcmotCommand.command = EMCMOT_SET_CIRCLE;
    emcmotCommand.pos = end;
    emcmotCommand.motion_type = type;
    emcmotCommand.center.x = center.x;
    emcmotCommand.center.y = center.y;
    emcmotCommand.center.z = center.z;

    emcmotCommand.normal.x = normal.x;
    emcmotCommand.normal.y = normal.y;
    emcmotCommand.normal.z = normal.z;
    emcmotCommand.turn = turn;
    emcmotCommand.id = lineid;

    if (EMC_DEBUG & EMC_DEBUG_SENDPOINT) {
        printf("circircircir,id=%d x = %lf y = %lf z = %lf vel = %lf normal: %.3f %.3f %.3f center: %.3f %.3f %.3f\n",emcmotCommand.id,end.tran.x,
               end.tran.y,end.tran.z,vel,normal.x,normal.y,normal.z,
               center.x,center.y,center.z);
    }

    emcmotCommand.vel = vel;
    emcmotCommand.ini_maxvel = ini_maxvel;
    emcmotCommand.acc = acc;

    for(int i=0;i<DYNAMIC_PAR_NUM;i++)
        emcmotCommand.dynamicparameter[i] = dynamicpar[i];


    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajLinearMove(EmcPose end, int type, double vel, double ini_maxvel, double acc,
                      int indexrotary, int gmodetype)
{
#ifdef ISNAN_TRAP
    if (isnan(end.tran.x) || isnan(end.tran.y) || isnan(end.tran.z) ||
        isnan(end.a) || isnan(end.b) || isnan(end.c) ||
        isnan(end.u) || isnan(end.v) || isnan(end.w)) {
    printf("isnan error x=%.3f y=%.3f z=%.3f a=%.3f b=%.3f c=%.3f\n",
           end.tran.x,end.tran.y,end.tran.z,end.a,end.b,end.c);
    return -1;		// ignore it for now, just don't send it
    }
#endif

    emcmotCommand.command = EMCMOT_SET_LINE;
    emcmotCommand.pos = end;
    emcmotCommand.id = localEmcTrajMotionId;
    if (EMC_DEBUG & EMC_DEBUG_SENDPOINT) {
        printf("in emcTrajLinearMove,id=%d x = %lf y = %lf z = %lf vel = %lf\n",emcmotCommand.id,end.tran.x,
               end.tran.y,
                end.tran.z,vel);
    }
    emcmotCommand.motion_type = type;
    emcmotCommand.vel = vel;
    emcmotCommand.ini_maxvel = ini_maxvel;
    emcmotCommand.acc = acc;
    emcmotCommand.turn = indexrotary;
    emcmotCommand.gmodetype = gmodetype;//ghf-2014/3/22
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajCircularMove(EmcPose end, PM_CARTESIAN center,
			PM_CARTESIAN normal, int turn, int type, double vel, double ini_maxvel, double acc)
{
#ifdef ISNAN_TRAP
    if (isnan(end.tran.x) || isnan(end.tran.y) || isnan(end.tran.z) ||
	isnan(end.a) || isnan(end.b) || isnan(end.c) ||
	isnan(end.u) || isnan(end.v) || isnan(end.w) ||
	isnan(center.x) || isnan(center.y) || isnan(center.z) ||
	isnan(normal.x) || isnan(normal.y) || isnan(normal.z)) {
	printf("isnan error in emcTrajCircularMove()\n");
    return -1;		// ignore it for now, just don't send it
    }
#endif

    emcmotCommand.command = EMCMOT_SET_CIRCLE;

    emcmotCommand.pos = end;
    emcmotCommand.motion_type = type;

    emcmotCommand.center.x = center.x;
    emcmotCommand.center.y = center.y;
    emcmotCommand.center.z = center.z;

    emcmotCommand.normal.x = normal.x;
    emcmotCommand.normal.y = normal.y;
    emcmotCommand.normal.z = normal.z;

    emcmotCommand.turn = turn;
    emcmotCommand.id = localEmcTrajMotionId;

    if (EMC_DEBUG & EMC_DEBUG_SENDPOINT) {
        printf("circircircir,id=%d x = %lf y = %lf z = %lf vel = %lf  %.3f %.3f %.3f\n",emcmotCommand.id,end.tran.x,
               end.tran.y,
                end.tran.z,vel,normal.x,normal.y,normal.z);
    }
    emcmotCommand.vel = vel;
    emcmotCommand.ini_maxvel = ini_maxvel;
    emcmotCommand.acc = acc;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajClearProbeTrippedFlag()
{
    emcmotCommand.command = EMCMOT_CLEAR_PROBE_FLAGS;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajProbe(EmcPose pos, int type, double vel, double ini_maxvel, double acc, unsigned char probe_type)
{
#ifdef ISNAN_TRAP
    if (isnan(pos.tran.x) || isnan(pos.tran.y) || isnan(pos.tran.z) ||
        isnan(pos.a) || isnan(pos.b) || isnan(pos.c) ||
        isnan(pos.u) || isnan(pos.v) || isnan(pos.w)) {
	printf("isnan error in emcTrajProbe()\n");
	return 0;		// ignore it for now, just don't send it
    }
#endif

    emcmotCommand.command = EMCMOT_PROBE;
    emcmotCommand.pos = pos;
    emcmotCommand.id = localEmcTrajMotionId;
    emcmotCommand.motion_type = type;
    emcmotCommand.vel = vel;
    emcmotCommand.ini_maxvel = ini_maxvel;
    emcmotCommand.acc = acc;
    emcmotCommand.probe_type = probe_type;
    printf("now I send posz is %f\n",pos.tran.z);
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcTrajRigidTap(EmcPose pos, double vel, double ini_maxvel, double acc)
{
#ifdef ISNAN_TRAP
    if (isnan(pos.tran.x) || isnan(pos.tran.y) || isnan(pos.tran.z)) {
	printf("isnan error in emcTrajRigidTap()\n");
	return 0;		// ignore it for now, just don't send it
    }
#endif

    emcmotCommand.command = EMCMOT_RIGID_TAP;
    emcmotCommand.pos.tran = pos.tran;
    emcmotCommand.id = localEmcTrajMotionId;
    emcmotCommand.vel = vel;
    emcmotCommand.ini_maxvel = ini_maxvel;
    emcmotCommand.acc = acc;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}


static int last_id = 0;
static int last_id_printed = 0;
static int last_status = 0;
static double last_id_time;

int emcTrajUpdate(EMC_TRAJ_STAT * stat)
{
    int axis, enables;

    stat->axes = localEmcTrajAxes;
    stat->axis_mask = localEmcTrajAxisMask;
    stat->linearUnits = localEmcTrajLinearUnits;
    stat->angularUnits = localEmcTrajAngularUnits;

    stat->mode =
	emcmotStatus.
	motionFlag & EMCMOT_MOTION_TELEOP_BIT ? EMC_TRAJ_MODE_TELEOP
	: (emcmotStatus.
	   motionFlag & EMCMOT_MOTION_COORD_BIT ? EMC_TRAJ_MODE_COORD :
	   EMC_TRAJ_MODE_FREE);

    /* enabled if motion enabled and all axes enabled */
    stat->enabled = 0;		/* start at disabled */
    if (emcmotStatus.motionFlag & EMCMOT_MOTION_ENABLE_BIT) {
	for (axis = 0; axis < localEmcTrajAxes; axis++) {
/*! \todo Another #if 0 */
#if 0				/*! \todo FIXME - the axis flag has been moved to the joint struct */
	    if (!emcmotStatus.axisFlag[axis] & EMCMOT_JOINT_ENABLE_BIT) {
		break;
	    }
#endif
	    /* got here, then all are enabled */
	    stat->enabled = 1;
	}
    }

    stat->inpos = emcmotStatus.motionFlag & EMCMOT_MOTION_INPOS_BIT;
    stat->queue = emcmotStatus.depth;
    stat->activeQueue = emcmotStatus.activeDepth;
    stat->queueFull = emcmotStatus.queueFull;
    stat->id = emcmotStatus.id;

/*    static int old = -1;
    if(old != stat->id)
    printf("stat->id=%d\n",stat->id);
    old = stat->id;
*/
    stat->motion_type = emcmotStatus.motionType;
    stat->distance_to_go = emcmotStatus.distance_to_go;
    stat->distance_to_go_x = emcmotStatus.distance_to_go_x;
    stat->distance_to_go_y = emcmotStatus.distance_to_go_y;
    stat->distance_to_go_z = emcmotStatus.distance_to_go_z;



    for(int i=0;i<DYNAMIC_PAR_NUM;i++)
        emcmotCommand.dynamicparameter[i] = emcmotStatus.dynamicparameter[i];

    stat->tool = emcmotCommand.dynamicparameter[0]; //当前运行行的刀具号ghf-2014/10/27
    stat->compside = emcmotCommand.dynamicparameter[1]; // 当前运行的补偿方向
    stat->dynamiccomp = emcmotCommand.dynamicparameter[2];
    stat->tool_length_comp = emcmotCommand.dynamicparameter[3];

    stat->dtg = emcmotStatus.dtg;
    stat->current_vel = emcmotStatus.current_vel;
    if (EMC_DEBUG_MOTION_TIME & EMC_DEBUG) {
	if (stat->id != last_id) {
	    if (last_id != last_id_printed) {
		rcs_print("Motion id %d took %f seconds.\n", last_id,
			  etime() - last_id_time);
		last_id_printed = last_id;
	    }
	    last_id = stat->id;
	    last_id_time = etime();
	}
    }

    //ghf-2014/3/13
    stat->run_on = emcmotStatus.run_on;
    stat->pause_on = emcmotStatus.pause_on;
    //printf("stat->run_on=%d\n",stat->run_on);//打印16
    //printf("stat->pause_on=%d\n",stat->pause_on);//打印32
    
    
    //ghf-2014/5/6
    stat->electric_cabinet_alarm = emcmotStatus.electric_cabinet_alarm;
    stat->spindle0_overcurrent_alarm = emcmotStatus.spindle0_overcurrent_alarm;
    stat->spindle1_overcurrent_alarm = emcmotStatus.spindle1_overcurrent_alarm;
    stat->spindle0_converter_alarm = emcmotStatus.spindle0_converter_alarm;
    stat->spindle1_converter_alarm = emcmotStatus.spindle1_converter_alarm;
    stat->probe_alarm = emcmotStatus.probe_alarm;
    stat->pressure_alarm = emcmotStatus.pressure_alarm;
    stat->coolant_alarm = emcmotStatus.coolant_alarm;
    stat->lube_alarm = emcmotStatus.lube_alarm;

    
    
    stat->paused = emcmotStatus.paused;
    stat->scale = emcmotStatus.feed_scale;
    stat->spindle_scale = emcmotStatus.spindle_scale;

    stat->position = emcmotStatus.carte_pos_cmd;


    /*static double old = 0;
    if(old!=emcmotStatus.carte_pos_cmd.tran.z){
        printf("emcmotStatus.carte_pos_cmd.tran.z=%f\n",emcmotStatus.carte_pos_cmd.tran.z);
    }
    old = emcmotStatus.carte_pos_cmd.tran.z;*/

    stat->actualPosition = emcmotStatus.carte_pos_fb;

    stat->velocity = emcmotStatus.vel;
    
    stat->velocity = stat->velocity * 60;//指定进给速度
    
    //if(stat->velocity != 0)
    //  printf("stat->velocity=%f\n",stat->velocity);

    stat->acceleration = emcmotStatus.acc;
    stat->maxAcceleration = localEmcMaxAcceleration;

    if (emcmotStatus.motionFlag & EMCMOT_MOTION_ERROR_BIT) {
	stat->status = RCS_ERROR;
    } else if (stat->inpos && (stat->queue == 0)) {
	    stat->status = RCS_DONE;
    } else {
        stat->status = RCS_EXEC;
    }

   // printf("stat->inpos=%d,stat->queue=%d  %d\n",stat->inpos,stat->queue,stat->status);

    if (EMC_DEBUG_MOTION_TIME & EMC_DEBUG) {
        if (stat->status == RCS_DONE && last_status != RCS_DONE
            && stat->id != last_id_printed) {
            rcs_print("Motion id %d took %f seconds.\n", last_id,
                  etime() - last_id_time);
            last_id_printed = last_id = stat->id;
            last_id_time = etime();
        }
    }

    stat->probedPosition = emcmotStatus.probedPos;
        stat->probeval = emcmotStatus.probeVal;
        stat->probing = emcmotStatus.probing;
        stat->probe_tripped = emcmotStatus.probeTripped;
    if (emcmotStatus.motionFlag & EMCMOT_MOTION_COORD_BIT)
        enables = emcmotStatus.enables_queued;
    else
        enables = emcmotStatus.enables_new;
    
    stat->feed_override_enabled = enables & FS_ENABLED;
    stat->spindle_override_enabled = enables & SS_ENABLED;
    stat->adaptive_feed_enabled = enables & AF_ENABLED;
    stat->feed_hold_enabled = enables & FH_ENABLED;

    if (new_config) {
	stat->cycleTime = emcmotConfig.trajCycleTime;
	stat->kinematics_type = emcmotConfig.kinematics_type;
	stat->maxVelocity = emcmotConfig.limitVel;
    }
    stat->linktoEth = emcmotStatus.linktoEth;
    stat->ethready = emcmotStatus.ethready;
    return 0;
}



int emcPositionLoad() {
    double positions[EMCMOT_MAX_JOINTS];
    IniFile ini;
    ini.Open(EMC_INIFILE);
    const char *posfile = ini.Find("POSITION_FILE", "TRAJ");
    ini.Close();
    if(!posfile || !posfile[0]) return 0;
    FILE *f = fopen(posfile, "r");
    if(!f) return 0;
    for(int i=0; i<EMCMOT_MAX_JOINTS; i++) {
	int r = fscanf(f, "%lf", &positions[i]);
	if(r != 1) { fclose(f); return -1; }
    }
    fclose(f);
    int result = 0;
    for(int i=0; i<EMCMOT_MAX_JOINTS; i++) {
	if(emcAxisSetMotorOffset(i, -positions[i]) != 0) result = -1;;
    }
    return result;
}


int emcPositionSave() {
    IniFile ini;
    const char *posfile;

    ini.Open(EMC_INIFILE);
    try {
        posfile = ini.Find("POSITION_FILE", "TRAJ");
    } catch (IniFile::Exception e) {
        ini.Close();
        return -1;
    }
    ini.Close();

    if(!posfile || !posfile[0]) return 0;
    // like the var file, make sure the posfile is recreated according to umask
    unlink(posfile);
    FILE *f = fopen(posfile, "w");
    if(!f) return -1;
    for(int i=0; i<EMCMOT_MAX_JOINTS; i++) {
	int r = fprintf(f, "%.17f\n", emcmotStatus.joint_status[i].pos_fb);
	if(r < 0) { fclose(f); return -1; }
    }
    fclose(f);
    return 0;
}

// EMC_MOTION functions
int emcMotionInit()
{
    int r1;
    int r2;
    int r3;
    int axis;

    r2 = emcTrajInit(); // we want to check Traj first, the sane defaults for units are there

    //usleep(1000*100);
    r1 = 0;
    aux_which_z = 0;        //add by hong to reset the active axes
    for (axis = 0; axis < localEmcTrajAxes; axis++) {
	if (0 != emcAxisInit(axis)) {
	    r1 = -1;		// at least one is busted
	}
    }

    r3 = emcPositionLoad();

    if (r1 == 0 && r2 == 0 && r3 == 0) {
	emcmotion_initialized = 1;
    }

    return (r1 == 0 && r2 == 0 && r3 == 0) ? 0 : -1;
}

int emcMotionHalt()
{
    int r1, r2, r3, r4;
    int t;

    r1 = -1;
    for (t = 0; t < EMCMOT_MAX_JOINTS; t++) {
	if (0 == emcAxisHalt(t)) {
	    r1 = 0;		// at least one is okay
	}
    }

    r2 = emcTrajDisable();
    r3 = emcTrajHalt();
    r4 = emcPositionSave();
    emcmotion_initialized = 0;

    return (r1 == 0 && r2 == 0 && r3 == 0 && r4 == 0) ? 0 : -1;
}

int emcMotionAbort()
{
    int r1;
    int r2;
    int r3 = 0;
    int t;

    r1 = -1;
    for (t = 0; t < EMCMOT_MAX_JOINTS; t++) {
	if (0 == emcAxisAbort(t)) {
	    r1 = 0;		// at least one is okay
	}
    }

    r2 = emcTrajAbort();

    return (r1 == 0 && r2 == 0 && r3 == 0) ? 0 : -1;
}

int emcMotionSetDebug(int debug)
{
    emcmotCommand.command = EMCMOT_SET_DEBUG;
    emcmotCommand.debug = debug;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

/*! \function emcMotionSetAout()
    
    This function sends a EMCMOT_SET_AOUT message to the motion controller.
    That one plans a AOUT command when motion starts or right now.

    @parameter	index	which output gets modified
    @parameter	now	wheather change is imediate or synched with motion
    @parameter	start	value set at start of motion
    @parameter	end	value set at end of motion
*/
int emcMotionSetAout(unsigned char index, double start, double end, unsigned char now)
{
    emcmotCommand.command = EMCMOT_SET_AOUT;
    emcmotCommand.now = now;
    emcmotCommand.out = index;
  /*! \todo FIXME-- if this works, set up some dedicated cmd fields instead of
     borrowing these */
    emcmotCommand.minLimit = start;
    emcmotCommand.maxLimit = end;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

/*! \function emcMotionSetDout()
    
    This function sends a EMCMOT_SET_DOUT message to the motion controller.
    That one plans a DOUT command when motion starts or right now.

    @parameter	index	which output gets modified
    @parameter	now	wheather change is imediate or synched with motion
    @parameter	start	value set at start of motion
    @parameter	end	value set at end of motion
*/
int emcMotionSetDout(unsigned char index, unsigned char start,
		     unsigned char end, unsigned char now)
{
    emcmotCommand.command = EMCMOT_SET_DOUT;
    emcmotCommand.now = now;
    emcmotCommand.out = index;
    emcmotCommand.start = start;
    emcmotCommand.end = end;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcSpindleAbort(void)
{
    return emcSpindleOff();
}

int emcSpindleSpeed(double speed, double css_factor, double offset)
{

    if (emcmotStatus.spindle.speed == 0)
	return 0; //spindle stopped, not updating speed

    return emcSpindleOn(speed, css_factor, offset);
}

int emcSpindleOn(double speed, double css_factor, double offset)
{

    emcmotCommand.command = EMCMOT_SPINDLE_ON;
    emcmotCommand.vel = speed;
    //printf("spindle speed %.3f\n",speed);
    emcmotCommand.ini_maxvel = css_factor;
    emcmotCommand.acc = offset;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcSpindleIgnore(int flag) {
    emcmotCommand.command = EMCMOT_SPINDLE_IGNORE;
    emcmotCommand.spindleignore = flag;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcSpindleOff()
{
    emcmotCommand.command = EMCMOT_SPINDLE_OFF;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcSpindleBrakeRelease()
{
    emcmotCommand.command = EMCMOT_SPINDLE_BRAKE_RELEASE;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcSpindleBrakeEngage()
{
    emcmotCommand.command = EMCMOT_SPINDLE_BRAKE_ENGAGE;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcSpindleIncrease()
{
    emcmotCommand.command = EMCMOT_SPINDLE_INCREASE;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcSpindleDecrease()
{
    emcmotCommand.command = EMCMOT_SPINDLE_DECREASE;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}
//######ghf-2014/2/24
//forceflag  1 means manual ,2 means auto
//m6_flag 1 means send in manual mode ,2 means send in auto
int emcdoublez(int set_which_z, double *axis_offset,double freevel)
{
    emcmotCommand.command = EMCMOT_DOUBLEZ;
    emcmotCommand.set_which_z = set_which_z;
    emcmotCommand.freevel = freevel;
    printf("now in task set which z %d\n",emcmotCommand.set_which_z);
    double maxvalue = 0;
    int maxindex = 2;
    double zoffset = axis_offset[2];
    for(int i=0;i<6;i++) {
        if((set_which_z & (0x01 << i)) >0) {
            maxvalue = axis_offset[i];
        } else {
            axis_offset[i] = 0;
        }
    }


    for(int i=0;i<6;i++) {
        if((set_which_z & (0x01 << i)) >0) {
            if((maxvalue - axis_offset[i]) < 0) {
                maxvalue = axis_offset[i];
                maxindex = i;
            }
        }

        //printf("now I get the maxvalue is %.3f offset:%.3f\n",maxvalue,axis_offset[i]);
    }
    for(int i=0;i<6;i++) {
        if((set_which_z & (0x01 << i)) >0) {
            emcmotCommand.axis_offset[i] = axis_offset[i] - maxvalue+emcStatus->task.parameters[4088];
        } else
            emcmotCommand.axis_offset[i] = axis_offset[i]+emcStatus->task.parameters[4088];
    }
    /*  ensure not to crush the maxlimit of z axis while lifting the z axis */

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}
//######ghf-2014/2/24

int emcSetMultifunc(int type,int par,double val) {
    emcmotCommand.command = EMCMOT_MULTIFUNC;
    emcmotCommand.multifunctype = type;
    emcmotCommand.multifuncintval = par;
    emcmotCommand.multifuncdoubleval = val;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcsetSelectZ(int selectz)
{
    emcmotCommand.command = EMCMOT_FREECHANGEOK;
    emcmotCommand.selectz = selectz;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

//######ghf-2014/3/11
int emcjogwheels_guide(unsigned char jogwheels_guide)
{
    emcmotCommand.command = EMCMOT_JOGWHEELS_GUIDE;
    emcmotCommand.jogwheels_guide = jogwheels_guide;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}
//######ghf-2014/3/11


//######ghf-2014/3/12
int emcsetgmode(TGMODE *gmode0, TGMODE *gmode1, double angle1, double angle2, double coeff1, double coeff2, double coeff3)
{
    emcmotCommand.command = EMCMOT_SETGMODE;
    memcpy(&(emcmotCommand.G_mode_0),gmode0,sizeof(emcmotCommand.G_mode_0));
    memcpy(&(emcmotCommand.G_mode_1),gmode1,sizeof(emcmotCommand.G_mode_1));
    emcmotCommand.angle1 = angle1;
    emcmotCommand.angle2 = angle2;
    emcmotCommand.coeff1 = coeff1;
    emcmotCommand.coeff2 = coeff2;
    emcmotCommand.coeff3 = coeff3;

    return usrmotWriteEmcmotCommand(&emcmotCommand);
}
//######ghf-2014/3/12


//######ghf-2014/3/17
int emcsetlight(unsigned char setlight)
{
    emcmotCommand.command = EMCMOT_SETLIGHT;
    emcmotCommand.setlight = setlight;
    switch(setlight){
    case 2:
        emcStatus->task.light[0] = 1;
        emcStatus->task.light[1] = 0;
        emcStatus->task.light[2] = 0;   break;
    case 1:
        emcStatus->task.light[0] = 0;
        emcStatus->task.light[1] = 1;
        emcStatus->task.light[2] = 0;   break;
    case 3:
        emcStatus->task.light[0] = 0;
        emcStatus->task.light[1] = 0;
        emcStatus->task.light[2] = 1;   break;
    default:
        emcStatus->task.light[0] = 1;
        emcStatus->task.light[1] = 0;
        emcStatus->task.light[2] = 0;   break;
    }
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}
//######ghf-2014/3/17

int emcsetRunStatus(int state) {
    emcmotCommand.command = EMCMOT_RUNSTATUS;
    emcmotCommand.run_status = state;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}


//######ghf-2014/3/28
int emcencrypt(char* encryptbuf,unsigned int time,unsigned int buf)
{
    emcmotCommand.command = EMCMOT_ENCRYPT;
    memcpy(emcmotCommand.encryptbuf,encryptbuf,sizeof(emcmotCommand.encryptbuf));
    emcmotCommand.randtime = time;
    emcmotCommand.randbuf = buf;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcfiltertime(char* filtertimebuf)
{
    emcmotCommand.command = EMCMOT_FILTERTIME;
    memcpy(emcmotCommand.filtertimebuf,filtertimebuf,sizeof(emcmotCommand.filtertimebuf));
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

//######ghf-2014/3/28

int emcSpindleConstant()
{
    return 0; // nothing to do
}


int emcWritetimeleft(long int sec, char wlefttimeflag) {
    emcmotCommand.command = EMCMOT_SETLEFTTIME;

    emcmotCommand.wlefttime[0] = sec&0xff;
    emcmotCommand.wlefttime[1] = (sec>>8) & 0xff;
    emcmotCommand.wlefttime[2] = (sec>>16) & 0xff;
    emcmotCommand.wlefttime[3] = (sec>>24) & 0xff;
    //printf("emcmotCommand.wlefttime[3]=%x\n",emcmotCommand.wlefttime[3]);
    //printf("emcmotCommand.wlefttime[2]=%x\n",emcmotCommand.wlefttime[2]);
    //printf("emcmotCommand.wlefttime[1]=%x\n",emcmotCommand.wlefttime[1]);
    //printf("emcmotCommand.wlefttime[0]=%x\n",emcmotCommand.wlefttime[0]);

    emcmotCommand.wlefttimeflag = wlefttimeflag;//ghf-2014/7/24,设置完后马上清零
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcWriteid(char *cpuid, char *hddid) {
    emcmotCommand.command = EMCMOT_WRITEID;
    int i;
    for(i=0;i<8;i++){
        emcmotCommand.cpuid[i] = cpuid[i];
        //printf("cpuid[%d]=%d\n",i,emcmotCommand.cpuid[i]);
    }
    for(i=0;i<4;i++){
        emcmotCommand.hddid[i] = hddid[i];
    }
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcSetIo(int io,bool st) {
    emcmotCommand.command = EMCMOT_SETIO;
    emcmotCommand.externionum = io;
    emcmotCommand.externiosta = st;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}

int emcSetSpindleMode(int zmode,int spindlezmode) {
    emcmotCommand.command = EMCMOT_SETZMODE;
    emcmotCommand.zmode = zmode;
    emcmotCommand.spindlezmode = spindlezmode;
    return usrmotWriteEmcmotCommand(&emcmotCommand);
}




int emcMotionUpdate(EMC_MOTION_STAT * stat)
{
    int r1;
    int r2;
    int axis;
    int error;
    int exec;
    int dio, aio;
    int i;

    usrmotReadEmcData(&motdata);



    // read the emcmot status
    if (0 != usrmotReadEmcmotStatus(&emcmotStatus)) {
	return -1;
    }  
	
    new_config = 0;
    if (emcmotStatus.config_num != emcmotConfig.config_num) {
	if (0 != usrmotReadEmcmotConfig(&emcmotConfig)) {
        printf("1111%d %d\n",emcmotStatus.config_num , emcmotConfig.config_num);
	    return -1;
	}
	new_config = 1;
    }

    if (get_emcmot_debug_info) {
	if (0 != usrmotReadEmcmotDebug(&emcmotDebug)) {
	    return -1;
	}
    }
    // read the emcmot error
    if (0 != usrmotReadEmcmotError(errorString)) {
	// no error, so ignore
    } else {
	// an error to report
	emcOperatorError(0, "%s", errorString);
    }

    // save the heartbeat and command number locally,
    // for use with emcMotionUpdate
    localMotionHeartbeat = emcmotStatus.heartbeat;
    localMotionCommandType = emcmotStatus.commandEcho;	/*! \todo FIXME-- not NML one! */
    localMotionEchoSerialNumber = emcmotStatus.commandNumEcho;

    r1 = emcAxisUpdate(&stat->axis[0], EMCMOT_MAX_JOINTS);
    r2 = emcTrajUpdate(&stat->traj);
    stat->heartbeat = localMotionHeartbeat;
    stat->command_type = localMotionCommandType;
    stat->echo_serial_number = localMotionEchoSerialNumber;
    stat->debug = emcmotConfig.debug;
    
    stat->selectedz = emcmotStatus.selectz;
    stat->set_which_z = emcmotStatus.set_which_z;

    stat->spindle.enabled = emcmotStatus.spindle.speed != 0;
    stat->spindle.speed = emcmotStatus.spindle.speedrpm;
    stat->spindle.brake = emcmotStatus.spindle.brake;
    stat->spindle.direction = emcmotStatus.spindle.direction;

    stat->spindle.inpos = emcmotStatus.spindle.inpos;

    stat->doublez.zdiff = emcmotStatus.zdiff;
     stat->doublez.whichz_flag = emcmotStatus.set_which_z;
    
    //printf("in emcMotionUpdate,stat->spindle.speed=%f\n",stat->spindle.speed);

    for (dio = 0; dio < EMC_MAX_DIO; dio++) {
	stat->synch_di[dio] = emcmotStatus.synch_di[dio];
	stat->synch_do[dio] = emcmotStatus.synch_do[dio];
    }

    for (aio = 0; aio < EMC_MAX_AIO; aio++) {
	stat->analog_input[aio] = emcmotStatus.analog_input[aio];
	stat->analog_output[aio] = emcmotStatus.analog_output[aio];
    }

    // set the status flag
    error = 0;
    exec = 0;

    for (axis = 0; axis < stat->traj.axes; axis++) {
	if (stat->axis[axis].status == RCS_ERROR) {
	    error = 1;
	    break;
	}
    if (stat->axis[axis].status == RCS_EXEC) {
	    exec = 1;
	    break;
	}
    }
    if (stat->traj.status == RCS_ERROR) {
	error = 1;
    } else if (stat->traj.status == RCS_EXEC) {
	exec = 1;
    }

    if(stat->spindle.inpos == 0) {
        exec = 1;
    }
    if (error) {
	stat->status = RCS_ERROR;
    } else if (exec) {
	stat->status = RCS_EXEC;
    } else {
	stat->status = RCS_DONE;
    }


    /*printf("now get the error:%d  %d %d %d %d %d\n",stat->status,stat->traj.status,stat->axis[0].status,stat->axis[1].status,stat->axis[2].status,stat->axis[3].status );*/
    
    stat->lightvalue = emcmotStatus.lightvalue;//ghf-2014/3/17

    stat->handwheel_Z2_active = emcmotStatus.handwheel_Z2_active;
    
    for (axis = 0; axis < EMCMOT_MAX_JOINTS; axis++) {
    	stat->switch_posfb[axis] = emcmotStatus.switch_posfb[axis];//ghf-2014/6/24
    }
    
    //stat->data_in_before = emcmotStatus.data_in_before;//ghf-2014/5/6
    

    for(i=0;i<EMCMOT_MAX_USR_OUTIO;i++) {
        stat->user_defout[i] = emcmotStatus.user_defout[i];
    }
    for(i=0;i<EMCMOT_MAX_USR_INIO;i++) {
        stat->user_defin[i] = emcmotStatus.user_defin[i];
    }
    for(i=0;i<EMCMOT_MAX_USR_WARNIO;i++) {
        stat->user_defwarn[i] = emcmotStatus.user_defwarn[i];
    }

    for(i=0;i<EMCMOT_MAX_JOINTS;i++) {
        stat->encoder_index[i] = emcmotStatus.encoder_index[i];
    }

    for(i=0;i<48;i++) {
        stat->mot_extern_in[i] = emcmotStatus.mot_extern_in[i];
    }


    for(i=0;i<24;i++) {
        stat->mot_extern_out[i] = emcmotStatus.mot_extern_out[i];
    }


    //ghf-2014/6/25 手动模式下，检测到从0跳到1时，切到手轮模式
    if(emcmotStatus.jog_state < 0) {
        stat->jog_state = 0;
    } else {
        stat->jog_state = 1;
    }

    for(int i=0;i<4;i++) {
        stat->spindle_on[i] = emcmotStatus.spindle_on[i];
    stat->spindle_on1 = emcmotStatus.spindle_on1;
    }

    for(i=0;i<8;i++){
        stat->customid[i] = emcmotStatus.customid[i];//ghf-2014/7/24
        //printf("%x\n",stat->customid[i]);
    }
    for(i=0;i<8;i++){
        stat->rlefttime[i] = emcmotStatus.rlefttime[i];//ghf-2014/7/24
        //printf("stat->rlefttime[0]=%x\n",stat->rlefttime[i]);
    }
    for(i=0;i<8;i++){
        stat->encrychipid[i] = emcmotStatus.encrychipid[i];//ghf-2014/7/24
    }
    for(i=0;i<64;i++){
        stat->encryptbuf[i] = emcmotStatus.encryptbuf[i];//ghf-2014/7/24
    }

    stat->pausefinish = emcmotStatus.pausefinish;//ghf-2014/8/4
    stat->fpgastatus = emcmotStatus.fpgastatus;//ghf-2014/8/25
    stat->runcycleflag = emcmotStatus.runcycleflag;//ghf-2015-2-26

    //ghf-2015/1/5
    for(int i = 0;i<8;i++) {
//        stat->Currentdate[i] = emcmotStatus.Currentdate[i];
        stat->Enddate[i] = emcmotStatus.Enddate[i];
        stat->MachineLockdate[i] = emcmotStatus.MachineLockdate[i];
    }

    //ghf-2014/1/13
    memcpy(stat->tregenc,emcmotStatus.tregenc,sizeof(emcmotStatus.tregenc));
    memcpy(stat->checkandsys,emcmotStatus.checkandsys,sizeof(emcmotStatus.checkandsys));

    //printf("enddate %d-%d-%d-%d-%d-%d\n",stat->Enddate[0],stat->Enddate[1],stat->Enddate[2],stat->Enddate[3],stat->Enddate[4],stat->Enddate[5]);
    //printf("rtctime %d-%d-%d-%d-%d-%d\n",stat->Currentdate[0],stat->Currentdate[1],stat->Currentdate[2],stat->Currentdate[3],stat->Currentdate[4],stat->Currentdate[5]);

    stat->dateerror=emcmotStatus.dateerror;
    stat->remaintimestatus=emcmotStatus.remaintimestatus;
    stat->timetype=emcmotStatus.timetype;
    stat->changezend=emcmotStatus.changezend;



    return (r1 == 0 && r2 == 0) ? 0 : -1;
}

//把当前绝对值坐标保存为零点时的绝对值坐标
int emcZeroSave() {
    const char *posfile = "/home/.config/zero_pos";
    const char *posfilebk = "/home/.config/zero_pos.bak";
    unlink(posfile);

    FILE *f = fopen(posfile, "w");
    if(!f) return -1;
    int i=0;
    for(i=0; i<EMCMOT_MAX_JOINTS; i++) {
            int r = fprintf(f, "%.17f\n", emcmotStatus.joint_status[i].abszeropos);
            if(r < 0) { fclose(f); return -1; }
    }
    fclose(f);

    FILE *bakf = fopen(posfilebk, "w");
    if(!bakf) return -1;
    i=0;
    for(i=0; i<EMCMOT_MAX_JOINTS; i++) {
            int r = fprintf(bakf, "%.17f\n", emcmotStatus.joint_status[i].abszeropos);
            if(r < 0) { fclose(bakf); return -1; }
    }
    fclose(bakf);

    return 0;
}
