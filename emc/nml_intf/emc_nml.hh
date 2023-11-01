/********************************************************************
* Description: emc_nml.hh
*   Declarations for EMC NML vocabulary
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
********************************************************************/
#ifndef EMC_NML_HH
#define EMC_NML_HH
#include "emc.hh"
#include "rcs.hh"
#include "cmd_msg.hh"
#include "stat_msg.hh"
#include "emcpos.h"
#include "canon.hh"		// CANON_TOOL_TABLE, CANON_UNITS
#include "rs274ngc.hh"		// ACTIVE_G_CODES, etc

// ------------------
// CLASS DECLARATIONS
// ------------------

// declarations for EMC general classes



/**
 * Send a textual error message to the operator.
 * The message is put in the errlog buffer to be read by the GUI.
 * This allows the controller a generic way to send error messages to
 * the operator.
 */
class EMC_OPERATOR_ERROR:public RCS_CMD_MSG {
  public:
    EMC_OPERATOR_ERROR():RCS_CMD_MSG(EMC_OPERATOR_ERROR_TYPE,
				     sizeof(EMC_OPERATOR_ERROR)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int id;
    char error[LINELEN];
};

/**
 * Send a textual information message to the operator.
 * This is similiar to EMC_OPERATOR_ERROR message except that the messages are
 * sent in situations not necessarily considered to be errors.
 */
class EMC_OPERATOR_TEXT:public RCS_CMD_MSG {
  public:
    EMC_OPERATOR_TEXT():RCS_CMD_MSG(EMC_OPERATOR_TEXT_TYPE,
				    sizeof(EMC_OPERATOR_TEXT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int id;
    char text[LINELEN];
};

/**
 * Send the URL or filename of a document to display.
 * This message is placed in the errlog buffer  to be read by the GUI.
 * If the GUI is capable of doing so it will show the operator a
 * previously created document, using the URL or filename provided.
 * This message is placed in the errlog channel to be read by the GUI.
 * This provides a general means of reporting an error from within the
 * controller without having to program the GUI to recognize each error type.
 */
class EMC_OPERATOR_DISPLAY:public RCS_CMD_MSG {
  public:
    EMC_OPERATOR_DISPLAY():RCS_CMD_MSG(EMC_OPERATOR_DISPLAY_TYPE,
				       sizeof(EMC_OPERATOR_DISPLAY)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int id;
    char display[LINELEN];
};

#define EMC_SYSTEM_CMD_LEN 256
/*
  execute a system command
*/
class EMC_SYSTEM_CMD:public RCS_CMD_MSG {
  public:
    EMC_SYSTEM_CMD():RCS_CMD_MSG(EMC_SYSTEM_CMD_TYPE,
				 sizeof(EMC_SYSTEM_CMD)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    char string[EMC_SYSTEM_CMD_LEN];
};

class EMC_NULL:public RCS_CMD_MSG {
  public:
    EMC_NULL():RCS_CMD_MSG(EMC_NULL_TYPE, sizeof(EMC_NULL)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_NULLEVENT:public RCS_CMD_MSG {
  public:
    EMC_NULLEVENT():RCS_CMD_MSG(EMC_NULLEVENT_TYPE, sizeof(EMC_NULL)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_SET_DEBUG:public RCS_CMD_MSG {
  public:
    EMC_SET_DEBUG():RCS_CMD_MSG(EMC_SET_DEBUG_TYPE, sizeof(EMC_SET_DEBUG)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int debug;
};

// declarations for EMC_AXIS classes

/*
 * AXIS command base class.
 * This is the base class for all commands that operate on a single axis.
 * The axis parameter specifies which axis the command affects.
 * These commands are sent to the emcCommand buffer to be read by the
 * TASK program that will then pass along corresponding messages to the
 * motion system.
 */
class EMC_AXIS_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_AXIS_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    // 0 = X, 1 = Y, 2 = Z, etc.
    int axis;
    int flag;
};

/**
 * Set the axis type to linear or angular.
 * Similiar to the AXIS_TYPE field in the ".ini" file.
 */
class EMC_AXIS_SET_AXIS:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_SET_AXIS():EMC_AXIS_CMD_MSG(EMC_AXIS_SET_AXIS_TYPE,
					 sizeof(EMC_AXIS_SET_AXIS)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    // EMC_AXIS_LINEAR, EMC_AXIS_ANGULAR
    unsigned char axisType;
};

/**
 * Set the units conversion factor.
 * @see EMC_AXIS_SET_INPUT_SCALE
 */
class EMC_AXIS_SET_UNITS:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_SET_UNITS():EMC_AXIS_CMD_MSG(EMC_AXIS_SET_UNITS_TYPE,
					  sizeof(EMC_AXIS_SET_UNITS)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    // units per mm, deg for linear, angular
    double units;
};


/**
 * Set the Axis backlash.
 * This command sets the backlash value.
 */
class EMC_AXIS_SET_BACKLASH:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_SET_BACKLASH():EMC_AXIS_CMD_MSG(EMC_AXIS_SET_BACKLASH_TYPE,
					     sizeof(EMC_AXIS_SET_BACKLASH))
    {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double backlash;
};

class EMC_AXIS_SET_MIN_POSITION_LIMIT:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_SET_MIN_POSITION_LIMIT():EMC_AXIS_CMD_MSG
	(EMC_AXIS_SET_MIN_POSITION_LIMIT_TYPE,
	 sizeof(EMC_AXIS_SET_MIN_POSITION_LIMIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double limit;
};

class EMC_AXIS_SET_MAX_POSITION_LIMIT:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_SET_MAX_POSITION_LIMIT():EMC_AXIS_CMD_MSG
	(EMC_AXIS_SET_MAX_POSITION_LIMIT_TYPE,
	 sizeof(EMC_AXIS_SET_MAX_POSITION_LIMIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double limit;
};

class EMC_AXIS_SET_FERROR:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_SET_FERROR():EMC_AXIS_CMD_MSG(EMC_AXIS_SET_FERROR_TYPE,
					   sizeof(EMC_AXIS_SET_FERROR)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double ferror;
};

class EMC_AXIS_SET_MIN_FERROR:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_SET_MIN_FERROR():EMC_AXIS_CMD_MSG
	(EMC_AXIS_SET_MIN_FERROR_TYPE, sizeof(EMC_AXIS_SET_MIN_FERROR)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double ferror;
};

class EMC_AXIS_SET_HOMING_PARAMS:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_SET_HOMING_PARAMS():EMC_AXIS_CMD_MSG
	(EMC_AXIS_SET_HOMING_PARAMS_TYPE,
	 sizeof(EMC_AXIS_SET_HOMING_PARAMS)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double home;
    double offset;
    double home_final_vel;
    double search_vel;
    double latch_vel;
    int use_index;
    int ignore_limits;
    int is_shared;
    int home_sequence;
    int volatile_home;
    int locking_indexer;
    double pitchvalue;//ghf-2014/5/29
};

class EMC_AXIS_SET_MAX_VELOCITY:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_SET_MAX_VELOCITY():EMC_AXIS_CMD_MSG
	(EMC_AXIS_SET_MAX_VELOCITY_TYPE,
	 sizeof(EMC_AXIS_SET_MAX_VELOCITY)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double vel;
};

class EMC_AXIS_INIT:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_INIT():EMC_AXIS_CMD_MSG(EMC_AXIS_INIT_TYPE,
				     sizeof(EMC_AXIS_INIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AXIS_HALT:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_HALT():EMC_AXIS_CMD_MSG(EMC_AXIS_HALT_TYPE,
				     sizeof(EMC_AXIS_HALT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AXIS_ABORT:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_ABORT():EMC_AXIS_CMD_MSG(EMC_AXIS_ABORT_TYPE,
				      sizeof(EMC_AXIS_ABORT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AXIS_ENABLE:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_ENABLE():EMC_AXIS_CMD_MSG(EMC_AXIS_ENABLE_TYPE,
				       sizeof(EMC_AXIS_ENABLE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AXIS_DISABLE:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_DISABLE():EMC_AXIS_CMD_MSG(EMC_AXIS_DISABLE_TYPE,
					sizeof(EMC_AXIS_DISABLE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AXIS_HOME:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_HOME():EMC_AXIS_CMD_MSG(EMC_AXIS_HOME_TYPE,
				     sizeof(EMC_AXIS_HOME)) {
    };

    int flag;
    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AXIS_UNHOME:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_UNHOME():EMC_AXIS_CMD_MSG(EMC_AXIS_UNHOME_TYPE,
				     sizeof(EMC_AXIS_UNHOME)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AXIS_JOG:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_JOG():EMC_AXIS_CMD_MSG(EMC_AXIS_JOG_TYPE,
				    sizeof(EMC_AXIS_JOG)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double vel;
};

class EMC_AXIS_INCR_JOG:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_INCR_JOG():EMC_AXIS_CMD_MSG(EMC_AXIS_INCR_JOG_TYPE,
					 sizeof(EMC_AXIS_INCR_JOG)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double incr;
    double vel;
};

class EMC_AXIS_ABS_JOG:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_ABS_JOG():EMC_AXIS_CMD_MSG(EMC_AXIS_ABS_JOG_TYPE,
					sizeof(EMC_AXIS_ABS_JOG)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double pos;
    double vel;
};

class EMC_AXIS_ACTIVATE:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_ACTIVATE():EMC_AXIS_CMD_MSG(EMC_AXIS_ACTIVATE_TYPE,
					 sizeof(EMC_AXIS_ACTIVATE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AXIS_DEACTIVATE:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_DEACTIVATE():EMC_AXIS_CMD_MSG(EMC_AXIS_DEACTIVATE_TYPE,
					   sizeof(EMC_AXIS_DEACTIVATE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AXIS_OVERRIDE_LIMITS:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_OVERRIDE_LIMITS():EMC_AXIS_CMD_MSG
	(EMC_AXIS_OVERRIDE_LIMITS_TYPE, sizeof(EMC_AXIS_OVERRIDE_LIMITS)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AXIS_LOAD_COMP:public EMC_AXIS_CMD_MSG {
  public:
    EMC_AXIS_LOAD_COMP():EMC_AXIS_CMD_MSG(EMC_AXIS_LOAD_COMP_TYPE,
					  sizeof(EMC_AXIS_LOAD_COMP)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    char file[LINELEN];
    int type; // type of the comp file. type==0 means nom, forw, rev triplets
              // type != 0 means nom, forw_trim, rev_trim triplets
};


// AXIS status base class
class EMC_AXIS_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_AXIS_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int axis;
};

class EMC_AXIS_STAT:public EMC_AXIS_STAT_MSG {
  public:
    EMC_AXIS_STAT();

    // For internal NML/CMS use only.
    void update(CMS * cms);

    // configuration parameters
    unsigned char axisType;	// EMC_AXIS_LINEAR, EMC_AXIS_ANGULAR
    double units;		// units per mm, deg for linear, angular
    double backlash;
    double minPositionLimit;
    double maxPositionLimit;
    double secNegLimit;
    double secPosLimit;
    double seclimitOffset;
    double maxFerror;
    double minFerror;

    // dynamic status
    double ferrorCurrent;	// current following error
    double ferrorHighMark;	// magnitude of max following error
    /*! \todo FIXME - is this really position, or the DAC output? */
    double output;		// commanded output position
    double input;		// current input position
    double motor_fbpos; // 读电机反馈数值
    double velocity;		// current velocity
    unsigned char inpos;	// non-zero means in position
    unsigned char homing;	// non-zero means homing
    unsigned char homed;	// non-zero means has been homed
    unsigned char homeswitch;	// non-zero means has been triger home switch ghf-2014/3/13
    unsigned char fault;	// non-zero means axis amp fault
    unsigned char enabled;	// non-zero means enabled
    unsigned char minSoftLimit;	// non-zero means min soft limit exceeded
    unsigned char maxSoftLimit;	// non-zero means max soft limit exceeded
    unsigned char minHardLimit;	// non-zero means min hard limit exceeded
    unsigned char maxHardLimit;	// non-zero means max hard limit exceeded
    unsigned char overrideLimits; // non-zero means limits are overridden
    double backlash_filt;//ghf-2014/7/29,坐标值显示时，output-此值
    double axisoffset;
    int errorcode;  //电机错误码
};

// declarations for EMC_TRAJ classes

// EMC_TRAJ command base class
class EMC_TRAJ_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_TRAJ_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_SET_UNITS:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_UNITS():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_UNITS_TYPE,
					  sizeof(EMC_TRAJ_SET_UNITS)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double linearUnits;		// units per mm
    double angularUnits;	// units per degree
};

class EMC_TRAJ_SET_AXES:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_AXES():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_AXES_TYPE,
					 sizeof(EMC_TRAJ_SET_AXES)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int axes;
};

class EMC_TRAJ_SET_CYCLE_TIME:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_CYCLE_TIME():EMC_TRAJ_CMD_MSG
	(EMC_TRAJ_SET_CYCLE_TIME_TYPE, sizeof(EMC_TRAJ_SET_CYCLE_TIME)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double cycleTime;
};

class EMC_TRAJ_SET_MODE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_MODE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_MODE_TYPE,
					 sizeof(EMC_TRAJ_SET_MODE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    enum EMC_TRAJ_MODE_ENUM mode;
};

class EMC_TRAJ_SET_VELOCITY:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_VELOCITY():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_VELOCITY_TYPE,
					     sizeof(EMC_TRAJ_SET_VELOCITY))
    {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double velocity;
    double ini_maxvel;
};

class EMC_TRAJ_SET_ACCELERATION:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_ACCELERATION():EMC_TRAJ_CMD_MSG
	(EMC_TRAJ_SET_ACCELERATION_TYPE,
	 sizeof(EMC_TRAJ_SET_ACCELERATION)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double acceleration;
};

class EMC_TRAJ_SET_MAX_VELOCITY:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_MAX_VELOCITY():EMC_TRAJ_CMD_MSG
	(EMC_TRAJ_SET_MAX_VELOCITY_TYPE,
	 sizeof(EMC_TRAJ_SET_MAX_VELOCITY)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double velocity;
};

class EMC_TRAJ_SET_MAX_ACCELERATION:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_MAX_ACCELERATION():EMC_TRAJ_CMD_MSG
	(EMC_TRAJ_SET_MAX_ACCELERATION_TYPE,
	 sizeof(EMC_TRAJ_SET_MAX_ACCELERATION)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double acceleration;
};

class EMC_TRAJ_SET_SCALE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_SCALE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_SCALE_TYPE,
					  sizeof(EMC_TRAJ_SET_SCALE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double scale;
};

class EMC_TRAJ_SET_SPINDLE_SCALE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_SPINDLE_SCALE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_SPINDLE_SCALE_TYPE,
					  sizeof(EMC_TRAJ_SET_SPINDLE_SCALE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double scale;
};

class EMC_TRAJ_SET_FO_ENABLE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_FO_ENABLE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_FO_ENABLE_TYPE,
					  sizeof(EMC_TRAJ_SET_FO_ENABLE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned char mode; //mode=0, override off (will work with 100% FO), mode != 0, override on, user can change FO
};

class EMC_TRAJ_SET_SO_ENABLE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_SO_ENABLE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_SO_ENABLE_TYPE,
					  sizeof(EMC_TRAJ_SET_SO_ENABLE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned char mode; //mode=0, override off (will work with 100% SO), mode != 0, override on, user can change SO
};

class EMC_TRAJ_SET_G0_ENABLE:public EMC_TRAJ_CMD_MSG {
    public:
      EMC_TRAJ_SET_G0_ENABLE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_G0_ENABLE_TYPE,
                        sizeof(EMC_TRAJ_SET_G0_ENABLE)) {
      };
      void update(CMS * cms);

      int mode; //mode=0, g0 is used as usually, mode = 1, then g0 move is under control by motion
};


class EMC_TRAJ_SET_DEFAULT_FEED:public EMC_TRAJ_CMD_MSG {
    public:
      EMC_TRAJ_SET_DEFAULT_FEED():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_DEFAULT_FEED_TYPE,
                        sizeof(EMC_TRAJ_SET_DEFAULT_FEED)) {
      };
      void update(CMS * cms);

      double feed; //mode=0, g0 is used as usually, mode = 1, then g0 move is under control by motion
};

class EMC_TRAJ_SET_FH_ENABLE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_FH_ENABLE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_FH_ENABLE_TYPE,
					  sizeof(EMC_TRAJ_SET_FH_ENABLE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned char mode; //mode=0, override off (feedhold is disabled), mode != 0, override on, user can use feedhold
};

class EMC_TRAJ_SET_MOTION_ID:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_MOTION_ID():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_MOTION_ID_TYPE,
					      sizeof
					      (EMC_TRAJ_SET_MOTION_ID)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int id;
};

class EMC_TRAJ_INIT:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_INIT():EMC_TRAJ_CMD_MSG(EMC_TRAJ_INIT_TYPE,
				     sizeof(EMC_TRAJ_INIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_HALT:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_HALT():EMC_TRAJ_CMD_MSG(EMC_TRAJ_HALT_TYPE,
				     sizeof(EMC_TRAJ_HALT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_ENABLE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_ENABLE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_ENABLE_TYPE,
				       sizeof(EMC_TRAJ_ENABLE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_DISABLE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_DISABLE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_DISABLE_TYPE,
					sizeof(EMC_TRAJ_DISABLE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_ABORT:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_ABORT():EMC_TRAJ_CMD_MSG(EMC_TRAJ_ABORT_TYPE,
				      sizeof(EMC_TRAJ_ABORT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_PAUSE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_PAUSE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_PAUSE_TYPE,
				      sizeof(EMC_TRAJ_PAUSE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_STEP:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_STEP():EMC_TRAJ_CMD_MSG(EMC_TRAJ_STEP_TYPE,
				     sizeof(EMC_TRAJ_STEP)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_RESUME:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_RESUME():EMC_TRAJ_CMD_MSG(EMC_TRAJ_RESUME_TYPE,
				       sizeof(EMC_TRAJ_RESUME)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_DELAY:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_DELAY():EMC_TRAJ_CMD_MSG(EMC_TRAJ_DELAY_TYPE,
				      sizeof(EMC_TRAJ_DELAY)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double delay;		// delay in seconds
    int spindleflag;
};

class EMC_TRAJ_MINLINEAR_MOVE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_MINLINEAR_MOVE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_MINLINEAR_MOVE_TYPE,
                        sizeof(EMC_TRAJ_MINLINEAR_MOVE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);


    int tool[MAXMINLEN];
    int mininum;
    int cmdtype[MAXMINLEN];
    int minlinenum[MAXMINLEN];
    double minx[MAXMINLEN],miny[MAXMINLEN],minz[MAXMINLEN];
    double mina[MAXMINLEN],minb[MAXMINLEN],minc[MAXMINLEN];
    double minu[MAXMINLEN],minv[MAXMINLEN],minw[MAXMINLEN];
    double vel[MAXMINLEN], acc[MAXMINLEN];
    int gmodetype[MAXMINLEN];

    PM_CARTESIAN center[MAXMINLEN];
    PM_CARTESIAN normal[MAXMINLEN];
    int turn[MAXMINLEN];
    int type[MAXMINLEN];
    double ini_maxvel[MAXMINLEN];
    int feed_mode[MAXMINLEN];
    double dynamiccomp[MAXMINLEN];
    
};

class EMC_TRAJ_LINEAR_MOVE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_LINEAR_MOVE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_LINEAR_MOVE_TYPE,
                        sizeof(EMC_TRAJ_LINEAR_MOVE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int type;
    EmcPose end;		// end point
    double vel, ini_maxvel, acc;
    int feed_mode;
    int indexrotary;
    int gmodetype;//ghf-2014/3/22
    double dynamiccomp;

};


class EMC_TRAJ_CIRCULAR_MOVE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_CIRCULAR_MOVE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_CIRCULAR_MOVE_TYPE,
					      sizeof
					      (EMC_TRAJ_CIRCULAR_MOVE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    EmcPose end;
    PM_CARTESIAN center;
    PM_CARTESIAN normal;
    int turn;
    int type;
    double vel, ini_maxvel, acc;
    int feed_mode;
    double dynamiccomp;

};

class EMC_TRAJ_SET_TERM_COND:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_TERM_COND():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_TERM_COND_TYPE,
					      sizeof
					      (EMC_TRAJ_SET_TERM_COND)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int cond;
    double tolerance; // used to set the precision/tolerance of path deviation 
		      // during CONTINUOUS motion mode. 
};

class EMC_TRAJ_SET_SPINDLESYNC:public EMC_TRAJ_CMD_MSG {
    public:
        EMC_TRAJ_SET_SPINDLESYNC():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_SPINDLESYNC_TYPE,
                sizeof(EMC_TRAJ_SET_SPINDLESYNC)) {
        };

        void update(CMS * cms);
        double feed_per_revolution;
    int velocity_mode;
};

class EMC_TRAJ_SET_OFFSET:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_OFFSET():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_OFFSET_TYPE,
					   sizeof(EMC_TRAJ_SET_OFFSET)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    EmcPose offset;
    int useflag;
};

class EMC_TRAJ_SET_G5X:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_G5X():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_G5X_TYPE,
					   sizeof(EMC_TRAJ_SET_G5X)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
    int g5x_index;
    EmcPose origin;
};

class EMC_TRAJ_SET_G92:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_G92():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_G92_TYPE,
					   sizeof(EMC_TRAJ_SET_G92)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    EmcPose origin;
};

class EMC_TRAJ_SET_ROTATION:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_ROTATION():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_ROTATION_TYPE,
					   sizeof(EMC_TRAJ_SET_ROTATION)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double rotation;
};

class EMC_TRAJ_SET_HOME:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_HOME():EMC_TRAJ_CMD_MSG(EMC_TRAJ_SET_HOME_TYPE,
					 sizeof(EMC_TRAJ_SET_HOME)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    EmcPose home;
};

class EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG():EMC_TRAJ_CMD_MSG
	(EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE,
	 sizeof(EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_SET_TELEOP_ENABLE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_TELEOP_ENABLE():EMC_TRAJ_CMD_MSG
	(EMC_TRAJ_SET_TELEOP_ENABLE_TYPE,
	 sizeof(EMC_TRAJ_SET_TELEOP_ENABLE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int enable;
};

class EMC_TRAJ_SET_TELEOP_VECTOR:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_SET_TELEOP_VECTOR():EMC_TRAJ_CMD_MSG
	(EMC_TRAJ_SET_TELEOP_VECTOR_TYPE,
	 sizeof(EMC_TRAJ_SET_TELEOP_VECTOR)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    EmcPose vector;
};

class EMC_TRAJ_PROBE:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_PROBE():EMC_TRAJ_CMD_MSG(EMC_TRAJ_PROBE_TYPE,
				      sizeof(EMC_TRAJ_PROBE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    EmcPose pos;
    int type;
    double vel, ini_maxvel, acc;
    unsigned char probe_type;
};

class EMC_TRAJ_RIGID_TAP:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_RIGID_TAP():EMC_TRAJ_CMD_MSG(EMC_TRAJ_RIGID_TAP_TYPE,
				      sizeof(EMC_TRAJ_RIGID_TAP)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    EmcPose pos;
    double vel, ini_maxvel, acc;
};

class EMC_TRAJ_MULTI_FUNCTION:public EMC_TRAJ_CMD_MSG {
  public:
    EMC_TRAJ_MULTI_FUNCTION():EMC_TRAJ_CMD_MSG(EMC_TRAJ_MULTI_FUNCTION_TYPE,
                     sizeof(EMC_TRAJ_MULTI_FUNCTION)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
    int type;
    int intpar;
    double doublepar;

};

// EMC_TRAJ status base class
class EMC_TRAJ_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_TRAJ_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TRAJ_STAT:public EMC_TRAJ_STAT_MSG {
  public:
    EMC_TRAJ_STAT();

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double linearUnits;		// units per mm
    double angularUnits;	// units per degree
    double cycleTime;		// cycle time, in seconds
    int axes;			// maximum axis number
    int axis_mask;		// mask of axes actually present
    enum EMC_TRAJ_MODE_ENUM mode;	// EMC_TRAJ_MODE_FREE,
    // EMC_TRAJ_MODE_COORD
    bool enabled;		// non-zero means enabled

    bool inpos;			// non-zero means in position
    int queue;			// number of pending motions, counting
    // current
    int activeQueue;		// number of motions blending
    bool queueFull;		// non-zero means can't accept another motion
    int id;			// id of the currently executing motion
    bool paused;			// non-zero means motion paused
    double scale;		// velocity scale factor
    double spindle_scale;	// spindle velocity scale factor

    EmcPose position;		// current commanded position
    EmcPose actualPosition;	// current actual position, from forward kins
    double velocity;		// system velocity, for subsequent motions
    double acceleration;	// system acceleration, for subsequent
    // motions
    double maxVelocity;		// max system velocity
    double maxAcceleration;	// system acceleration

    EmcPose probedPosition;	// last position where probe was tripped.
    bool probe_tripped;		// Has the probe been tripped since the last
    // clear.
    bool probing;		// Are we currently looking for a probe
    // signal.
    int probeval;		// Current value of probe input.
    int kinematics_type;	// identity=1,serial=2,parallel=3,custom=4
    int motion_type;
    double distance_to_go;         // in current move
    double distance_to_go_x;//ghf-2014/6/24
    double distance_to_go_y;
    double distance_to_go_z;
    int tool;//ghf-2014/10/27
    int compside;//hong-2014/10/29
    double dynamiccomp;
    double tool_length_comp;
    EmcPose dtg;
    double current_vel;         // in current move
    bool feed_override_enabled;
    bool spindle_override_enabled;
    bool adaptive_feed_enabled;
    bool feed_hold_enabled;
    unsigned char run_on; //ghf-2014/3/13
    unsigned char pause_on;//ghf-2014/3/13
    
    unsigned char electric_cabinet_alarm;//ghf-2014/5/6,HAL_IN
    unsigned char spindle0_overcurrent_alarm;//ghf-2014/5/6,HAL_IN
    unsigned char spindle1_overcurrent_alarm;//ghf-2014/5/6,HAL_IN
    unsigned char spindle0_converter_alarm;//ghf-2014/5/6,HAL_IN
    unsigned char spindle1_converter_alarm;//ghf-2014/5/6,HAL_IN
    unsigned char probe_alarm;//ghf-2014/5/6,HAL_IN
    unsigned char pressure_alarm;//ghf-2014/5/6,HAL_IN
    unsigned char coolant_alarm;//ghf-2014/5/6,HAL_IN
    unsigned char lube_alarm;//ghf-2014/5/6,HAL_IN

    int linktoEth;      //Ethercat链接状态
    int ethready;


};

// emc_MOTION is aggregate of all EMC motion-related status classes

// EMC_MOTION command base class
class EMC_MOTION_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_MOTION_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_MOTION_INIT:public EMC_MOTION_CMD_MSG {
  public:
    EMC_MOTION_INIT():EMC_MOTION_CMD_MSG(EMC_MOTION_INIT_TYPE,
					 sizeof(EMC_MOTION_INIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_MOTION_HALT:public EMC_MOTION_CMD_MSG {
  public:
    EMC_MOTION_HALT():EMC_MOTION_CMD_MSG(EMC_MOTION_HALT_TYPE,
					 sizeof(EMC_MOTION_HALT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_MOTION_ABORT:public EMC_MOTION_CMD_MSG {
  public:
    EMC_MOTION_ABORT():EMC_MOTION_CMD_MSG(EMC_MOTION_ABORT_TYPE,
					  sizeof(EMC_MOTION_ABORT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_MOTION_SET_AOUT:public EMC_MOTION_CMD_MSG {
  public:
    EMC_MOTION_SET_AOUT():EMC_MOTION_CMD_MSG(EMC_MOTION_SET_AOUT_TYPE,
					     sizeof(EMC_MOTION_SET_AOUT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned char index;	// which to set
    double start;		// value at start
    double end;			// value at end
    unsigned char now;		// wether command is imediate or synched with motion
};

class EMC_MOTION_SET_DOUT:public EMC_MOTION_CMD_MSG {
  public:
    EMC_MOTION_SET_DOUT():EMC_MOTION_CMD_MSG(EMC_MOTION_SET_DOUT_TYPE,
					     sizeof(EMC_MOTION_SET_DOUT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned char index;	// which to set
    unsigned char start;	// binary value at start
    unsigned char end;		// binary value at end
    unsigned char now;		// wether command is imediate or synched with motion
};

class EMC_MOTION_SET_EXTERN_OUT:public EMC_MOTION_CMD_MSG {
  public:
    EMC_MOTION_SET_EXTERN_OUT():EMC_MOTION_CMD_MSG(EMC_MOTION_SET_EXTERN_OUT_TYPE,
                         sizeof(EMC_MOTION_SET_DOUT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned char index;	// which to set
    unsigned char status;	// binary value at start
};

class EMC_MOTION_ADAPTIVE:public EMC_MOTION_CMD_MSG {
  public:
    EMC_MOTION_ADAPTIVE():EMC_MOTION_CMD_MSG(EMC_MOTION_ADAPTIVE_TYPE,
					     sizeof(EMC_MOTION_ADAPTIVE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned char status;		// status=0 stop; status=1 start.
};

// EMC_MOTION status base class
class EMC_MOTION_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_MOTION_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
	heartbeat = 0;
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned long int heartbeat;
};


// EMC_SPINDLE status base class
class EMC_SPINDLE_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_SPINDLE_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_SPINDLE_STAT:public EMC_SPINDLE_STAT_MSG {
  public:
    EMC_SPINDLE_STAT();

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double speed;		// spindle speed in RPMs
    int direction;		// 0 stopped, 1 forward, -1 reverse
    int brake;			// 0 released, 1 engaged
    int increasing;		// 1 increasing, -1 decreasing, 0 neither
    int enabled;		// non-zero means enabled

    int inpos;      //主轴转到位信号
    double lefttime; //spindle on delay time left
};

//HONG 2014-05-07 use for the reset function
class EMC_RESET_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_RESET_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_RESET_CMD:public EMC_RESET_CMD_MSG {
  public:
    EMC_RESET_CMD():EMC_RESET_CMD_MSG(EMC_RESET_CMD_TYPE, sizeof(EMC_RESET_CMD)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

//ghf-2014/2/24
// DOUBLE_Z command base class
class EMC_DOUBLEZ_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_DOUBLEZ_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};


class EMC_DOUBLEZ_CMD:public EMC_DOUBLEZ_CMD_MSG {
  public:
    EMC_DOUBLEZ_CMD():EMC_DOUBLEZ_CMD_MSG(EMC_DOUBLEZ_CMD_TYPE, sizeof(EMC_DOUBLEZ_CMD)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int set_which_z;
    int m6_flag;
    int q_flag;
    double z_diff[9];  //the value is z1-z2
};

class EMC_DOUBLEZ_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_DOUBLEZ_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_DOUBLEZ_STAT:public EMC_DOUBLEZ_STAT_MSG {
  public:
    EMC_DOUBLEZ_STAT():EMC_DOUBLEZ_STAT_MSG(EMC_DOUBLEZ_STAT_TYPE, sizeof(EMC_DOUBLEZ_STAT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int whichz_flag;
    double zdiff;
    bool z_changing;
};
//ghf-2014/2/24


//ghf-2014/3/11
// JOGWHEELS_GUIDE command base class
class EMC_JOGWHEELS_GUIDE_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_JOGWHEELS_GUIDE_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_JOGWHEELS_GUIDE_CMD:public EMC_JOGWHEELS_GUIDE_CMD_MSG {
  public:
    EMC_JOGWHEELS_GUIDE_CMD():EMC_JOGWHEELS_GUIDE_CMD_MSG(EMC_JOGWHEELS_GUIDE_CMD_TYPE, sizeof(EMC_JOGWHEELS_GUIDE_CMD)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned char jogwheels_guide;
};
//ghf-2014/3/11

//ghf-2014/3/12
class EMC_SETGMODE_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_SETGMODE_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_SETGMODE_CMD:public EMC_SETGMODE_CMD_MSG {
  public:
    EMC_SETGMODE_CMD():EMC_SETGMODE_CMD_MSG(EMC_SETGMODE_CMD_TYPE, sizeof(EMC_SETGMODE_CMD)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    TGMODE gmode0;
    TGMODE gmode1;
};
//ghf-2014/3/12


//ghf-2014/3/17
class EMC_SETLIGHT_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_SETLIGHT_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_SETLIGHT_CMD:public EMC_SETLIGHT_CMD_MSG {
  public:
    EMC_SETLIGHT_CMD():EMC_SETLIGHT_CMD_MSG(EMC_SETLIGHT_CMD_TYPE, sizeof(EMC_SETLIGHT_CMD)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned char setlight; //点亮相应的灯;1：运行，绿灯
                                              // 2： 空闲，黄灯
                                              // 3：报警，红灯  
};
//ghf-2014/3/17


//ghf-2014/3/28
class EMC_ENCRYPT_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_ENCRYPT_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_ENCRYPT_CMD:public EMC_ENCRYPT_CMD_MSG {
  public:
    EMC_ENCRYPT_CMD():EMC_ENCRYPT_CMD_MSG(EMC_ENCRYPT_CMD_TYPE, sizeof(EMC_ENCRYPT_CMD)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    char encryptbuf[32]; //ghf-2014/3/28  
};



class EMC_FILTERTIME_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_FILTERTIME_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_FILTERTIME_CMD:public EMC_FILTERTIME_CMD_MSG {
  public:
    EMC_FILTERTIME_CMD():EMC_FILTERTIME_CMD_MSG(EMC_FILTERTIME_CMD_TYPE, sizeof(EMC_FILTERTIME_CMD)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    char filtertimebuf[64]; //ghf-2014/5/6
};

//ghf-2014/3/28

class EMC_MOTION_STAT:public EMC_MOTION_STAT_MSG {
  public:
    EMC_MOTION_STAT();

    // For internal NML/CMS use only.
    void update(CMS * cms);

    // aggregate of motion-related status classes
    EMC_DOUBLEZ_STAT doublez;//ghf-2014/2/24
    unsigned char lightvalue;//ghf-2014/3/17,表示当前运行状态，1：运行/暂停，绿灯亮
                                                                          // 2：空闲，黄灯亮
                                                                          // 3：报警，红灯亮
                                                                          // 0：都不亮，刚上电是这种状态，如果上电报警，红灯要点亮


    double switch_posfb[EMCMOT_MAX_JOINTS]; //ghf-2014/5/5,第二次碰到回零开关后，反向寻找编码器零点要走过的距离
    
    char encoder_index[EMCMOT_MAX_JOINTS];//ghf-20150624
    char user_defin[EMCMOT_MAX_USR_INIO]; //ghf-2014/5/24
    char user_defout[EMCMOT_MAX_USR_OUTIO]; //hong-2014/05/29
    char user_defwarn[EMCMOT_MAX_USR_WARNIO];//hong-2014/05/29


    unsigned char mot_extern_in[48];//hong-2014/08/23
    unsigned char mot_extern_out[24];
    unsigned int jog_state;//ghf-2014/6/6
    
    unsigned char spindle_on[4];//ghf-2014/5/27
    unsigned char spindle_on1;//ghf-2014/5/27

    unsigned int data_in_before;//ghf-2014/5/6,滤波前的32路输入信号
    
    EMC_TRAJ_STAT traj;
    EMC_AXIS_STAT axis[EMC_AXIS_MAX];
    EMC_SPINDLE_STAT spindle;

    int synch_di[EMC_MAX_DIO];  // motion inputs queried by interp
    int synch_do[EMC_MAX_DIO];  // motion outputs queried by interp
    double analog_input[EMC_MAX_AIO]; //motion analog inputs queried by interp
    double analog_output[EMC_MAX_AIO]; //motion analog outputs queried by interp
    int debug;			// copy of EMC_DEBUG global

    unsigned int timeleft;  //hong-2014/07/21
    unsigned char customernum[8];  //hong-2014/07/21

    //ghf-2014/7/24
    unsigned char customid[8];//客户编号
    unsigned char rlefttime[8];//剩余时间
    unsigned char encrychipid[8];//加密芯片id
    unsigned char encryptbuf[64];//密文密钥
    unsigned char fpgaversion[8];//高4位为硬件版本号，低4位为软件版本号
    unsigned int fpgastatus;//ghf-2014/8/25
    int runcycleflag;

    //ghf-2015/1/5
    unsigned int Enddate[8]; //20150105
                                                //后面4字节是没有用的，比较的时候加上12个小时即可
    unsigned int MachineLockdate[8];//
    unsigned int Currentdate[8];//上层可以实时获取rtc时间
    char dateerror; //-1:小于刷新日期;-2超过了截止日期;-3小于了写入日期
    unsigned char remaintimestatus;//当前剩余时间状态位 (0还没到期，1表示到期)
    unsigned char timetype;//当前采用哪一套计时系统标志 （0表示以小时 1表示以日期）

    unsigned char tregenc[8];//时间注册编码ghf-2015/1/13
    unsigned char checkandsys[8];//校验号码和系统编号ghf-2015/1/13

    unsigned char cpuid[8];//cpuid
    unsigned char hddid[4];//硬盘id
    unsigned char wlefttime[4];//重置剩余时间
    unsigned char wlefttimeflag;//为1,终止现有读写，写时间下去
    int pausefinish;
    int changezend;

    int selectedz;      //hong show the selected z use in handle-wheel
    int set_which_z;    //hong real active z in 4z axis system


    unsigned int  handwheel_Z2_active;

};

// declarations for EMC_TASK classes

// EMC_TASK command base class
class EMC_TASK_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_TASK_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_INIT:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_INIT():EMC_TASK_CMD_MSG(EMC_TASK_INIT_TYPE,
				     sizeof(EMC_TASK_INIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_HALT:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_HALT():EMC_TASK_CMD_MSG(EMC_TASK_HALT_TYPE,
				     sizeof(EMC_TASK_HALT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_ABORT:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_ABORT():EMC_TASK_CMD_MSG(EMC_TASK_ABORT_TYPE,
				      sizeof(EMC_TASK_ABORT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_SET_MODE:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_SET_MODE():EMC_TASK_CMD_MSG(EMC_TASK_SET_MODE_TYPE,
					 sizeof(EMC_TASK_SET_MODE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    enum EMC_TASK_MODE_ENUM mode;
};

class EMC_TASK_SET_STATE:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_SET_STATE():EMC_TASK_CMD_MSG(EMC_TASK_SET_STATE_TYPE,
					  sizeof(EMC_TASK_SET_STATE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    enum EMC_TASK_STATE_ENUM state;
};

class EMC_TASK_PLAN_OPEN:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_OPEN():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_OPEN_TYPE,
					  sizeof(EMC_TASK_PLAN_OPEN)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    char file[LINELEN];
};

class EMC_TASK_PLAN_RUN:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_RUN():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_RUN_TYPE,
					 sizeof(EMC_TASK_PLAN_RUN)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int line;			// line to run from; 0 or 1 means from start,
    int endline;        // line to run end;
    // negative means run through to verify
};


class EMC_TASK_PLAN_READ:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_READ():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_READ_TYPE,
					  sizeof(EMC_TASK_PLAN_READ)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_PLAN_EXECUTE:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_EXECUTE():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_EXECUTE_TYPE,
					     sizeof(EMC_TASK_PLAN_EXECUTE))
    {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    char command[LINELEN];
    int reset_flag;
};

class EMC_TASK_PLAN_PAUSE:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_PAUSE():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_PAUSE_TYPE,
					   sizeof(EMC_TASK_PLAN_PAUSE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_PLAN_STEP:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_STEP():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_STEP_TYPE,
					  sizeof(EMC_TASK_PLAN_STEP)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_PLAN_RESUME:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_RESUME():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_RESUME_TYPE,
					    sizeof(EMC_TASK_PLAN_RESUME)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_PLAN_END:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_END():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_END_TYPE,
					 sizeof(EMC_TASK_PLAN_END)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_PLAN_CLOSE:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_CLOSE():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_CLOSE_TYPE,
					   sizeof(EMC_TASK_PLAN_CLOSE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_PLAN_INIT:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_INIT():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_INIT_TYPE,
					  sizeof(EMC_TASK_PLAN_INIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_PLAN_SYNCH:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_SYNCH():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_SYNCH_TYPE,
					   sizeof(EMC_TASK_PLAN_SYNCH)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_PLAN_NULL:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_NULL():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_NULL_TYPE,
                       sizeof(EMC_TASK_PLAN_NULL)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_EXECUTE_BLOCK:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_EXECUTE_BLOCK():EMC_TASK_CMD_MSG(EMC_TASK_EXECUTE_BLOCK_TYPE,
                       sizeof(EMC_TASK_EXECUTE_BLOCK)) {
    };

    // For internal NML/CMS use only.
    char command[LINELEN];
    void update(CMS * cms);
};

class EMC_TASK_FLUSH_PAR:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_FLUSH_PAR():EMC_TASK_CMD_MSG(EMC_TASK_FLUSH_PAR_TYPE,
                       sizeof(EMC_TASK_FLUSH_PAR)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_AUTOREPEAT_PAR:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_AUTOREPEAT_PAR():EMC_TASK_CMD_MSG(EMC_TASK_AUTOREPEAT_PAR_TYPE,
                       sizeof(EMC_TASK_AUTOREPEAT_PAR)) {
    };
    bool enable;
    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_PLAN_SET_OPTIONAL_STOP:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_SET_OPTIONAL_STOP():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_SET_OPTIONAL_STOP_TYPE,
					   sizeof(EMC_TASK_PLAN_SET_OPTIONAL_STOP)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
    
    bool state; //state == ON, optional stop is on (e.g. we stop on any stops)
};

class EMC_TASK_PLAN_SET_BLOCK_DELETE:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_SET_BLOCK_DELETE():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_SET_BLOCK_DELETE_TYPE,
					   sizeof(EMC_TASK_PLAN_SET_BLOCK_DELETE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
    
    bool state; //state == ON, block delete is on, we ignore lines starting with "/"
};

class EMC_TASK_PLAN_OPTIONAL_STOP:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_PLAN_OPTIONAL_STOP():EMC_TASK_CMD_MSG(EMC_TASK_PLAN_OPTIONAL_STOP_TYPE,
					   sizeof(EMC_TASK_PLAN_OPTIONAL_STOP)) {
    };
    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TASK_SET_LEFTTIME:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_SET_LEFTTIME():EMC_TASK_CMD_MSG(EMC_TASK_SET_LEFTTIME_TYPE,
                       sizeof(EMC_TASK_SET_LEFTTIME)) {
    };
    int lefthours;
    void update(CMS * cms);
};

class EMC_TASK_SET_STARTLINE:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_SET_STARTLINE():EMC_TASK_CMD_MSG(EMC_TASK_SET_STARTLINE_TYPE,
                       sizeof(EMC_TASK_SET_STARTLINE)) {
    };
    int startline;
    void update(CMS * cms);
};


class EMC_TASK_WAIT_FOR_SIGNAL:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_WAIT_FOR_SIGNAL():EMC_TASK_CMD_MSG(EMC_TASK_WAIT_FOR_SIGNAL_TYPE,
                       sizeof(EMC_TASK_WAIT_FOR_SIGNAL)) {
    };
    double delaytime;
    char msg[256];
    void update(CMS * cms);
};

class EMC_TASK_SET_SIGNAL:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_SET_SIGNAL():EMC_TASK_CMD_MSG(EMC_TASK_SET_SIGNAL_TYPE,
                       sizeof(EMC_TASK_SET_SIGNAL)) {
    };
    int signal;
    void update(CMS * cms);
};

class EMC_TASK_SET_PARAMETER:public EMC_TASK_CMD_MSG {
  public:
    EMC_TASK_SET_PARAMETER():EMC_TASK_CMD_MSG(EMC_TASK_SET_PARAMETER_TYPE,
                       sizeof(EMC_TASK_SET_PARAMETER)) {
    };
    int index;
    double val;
    void update(CMS * cms);
};

// EMC_TASK status base class
class EMC_TASK_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_TASK_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
	heartbeat = 0;
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned long int heartbeat;
};

class EMC_TASK_STAT:public EMC_TASK_STAT_MSG {
  public:
    EMC_TASK_STAT();

    // For internal NML/CMS use only.
    void update(CMS * cms);

    enum EMC_TASK_MODE_ENUM mode;	// EMC_TASK_MODE_MANUAL, etc.
    enum EMC_TASK_STATE_ENUM state;	// EMC_TASK_STATE_ESTOP, etc.

    enum EMC_TASK_EXEC_ENUM execState;	// EMC_DONE,WAITING_FOR_MOTION, etc.
    enum EMC_TASK_INTERP_ENUM interpState;	// EMC_IDLE,READING,PAUSED,WAITING

    enum EMC_PRO_MODE_ENUM runState; //running paused idle add by hong 2014-11-07
    int motionLine;		// line motion is executing-- may lag
    int currentLine;		// line currently executing
    int readLine;		// line interpreter has read to
    bool optional_stop_state;	// state of optional stop (== ON means we stop on M1)
    bool block_delete_state;	// state of block delete (== ON means we ignore lines starting with "/")
    int input_timeout;		// has a timeout happened on digital input
    char file[LINELEN];
    char command[LINELEN];
    EmcPose g5x_offset;		// in user units, currently active
    int g5x_index;              // index of active g5x system
    EmcPose g92_offset;		// in user units, currently active
    double rotation_xy;
    EmcPose toolOffset;		// tool offset, in general pose form
    int activeGCodes[ACTIVE_G_CODES];
    int activeMCodes[ACTIVE_M_CODES];
    double activeSettings[ACTIVE_SETTINGS];
    CANON_UNITS programUnits;	// CANON_UNITS_INCHES,MM,CM

    int interpreter_errcode;	// return value from rs274ngc function 
    // (only useful for new interpreter.)
    int task_paused;		// non-zero means task is paused
    double delayLeft;           // delay time left of G4, M66..

    double parameters[RS274NGC_MAX_PARAMETERS];   // system parameters
    double feed_rate;       //actual speed in g1 mode by hong 2014-03-22
    double feed_default;    //feed_choice_mode 1 use this feed_default
    double feed_choice_mode;    //feed mode,0 means nc feed,1 means default feed
    int program_running_time; //hong 140326 get the program running time
    int work_piece;    //when exec sucess,it ++1
    bool mdi_running; // the flag of the mdirun,1 means mdi is running,0 means stop;
    bool handlewheel_enable; //hong true means handleguide open ,else close ,update in taskmain
    bool only_reading; // 1 means verify is in the process,0 means verify is over
    int light[3]; //3 light of red,green,yellow;
    int tool_comp_side; //read the cutter side and its on or off
    bool autorepeat;    //auto repeat flag
    bool singlecompverify;
    double timeleft;    //the left time;

    int setstartline; //when the program is interrupted,the pro line is record
    int runfromline;    //run the program from this line
    int runendline;     //run end when this line ends
    int hardwarecheck;  //check the hardware 2014-08-22
    int fpgacheck[5];   //check for the fpga signal
    unsigned char getcpuid[8];   //get the hard disk ID
    unsigned short lefttimeval;     //

    int waitsignal;     //1 means waiting for the signal,0 means get the signal
    int toolnow;    //now the tool in spindle
    int needreloadtool;
    int waitflag;

    double rotationModex;
    double rotationModey;
    double rotationModez;
};

// declarations for EMC_TOOL classes

// EMC_TOOL command base class
class EMC_TOOL_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_TOOL_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TOOL_INIT:public EMC_TOOL_CMD_MSG {
  public:
    EMC_TOOL_INIT():EMC_TOOL_CMD_MSG(EMC_TOOL_INIT_TYPE,
				     sizeof(EMC_TOOL_INIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TOOL_HALT:public EMC_TOOL_CMD_MSG {
  public:
    EMC_TOOL_HALT():EMC_TOOL_CMD_MSG(EMC_TOOL_HALT_TYPE,
				     sizeof(EMC_TOOL_HALT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TOOL_ABORT:public EMC_TOOL_CMD_MSG {
  public:
    EMC_TOOL_ABORT():EMC_TOOL_CMD_MSG(EMC_TOOL_ABORT_TYPE,
				      sizeof(EMC_TOOL_ABORT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TOOL_PREPARE:public EMC_TOOL_CMD_MSG {
  public:
    EMC_TOOL_PREPARE():EMC_TOOL_CMD_MSG(EMC_TOOL_PREPARE_TYPE,
					sizeof(EMC_TOOL_PREPARE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int tool;
};

class EMC_TOOL_LOAD:public EMC_TOOL_CMD_MSG {
  public:
    EMC_TOOL_LOAD():EMC_TOOL_CMD_MSG(EMC_TOOL_LOAD_TYPE,
				     sizeof(EMC_TOOL_LOAD)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TOOL_UNLOAD:public EMC_TOOL_CMD_MSG {
  public:
    EMC_TOOL_UNLOAD():EMC_TOOL_CMD_MSG(EMC_TOOL_UNLOAD_TYPE,
				       sizeof(EMC_TOOL_UNLOAD)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TOOL_LOAD_TOOL_TABLE:public EMC_TOOL_CMD_MSG {
  public:
    EMC_TOOL_LOAD_TOOL_TABLE():EMC_TOOL_CMD_MSG
	(EMC_TOOL_LOAD_TOOL_TABLE_TYPE, sizeof(EMC_TOOL_LOAD_TOOL_TABLE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    char file[LINELEN];		// name of tool table, empty means default
};

class EMC_TOOL_SET_OFFSET:public EMC_TOOL_CMD_MSG {
  public:
    EMC_TOOL_SET_OFFSET():EMC_TOOL_CMD_MSG(EMC_TOOL_SET_OFFSET_TYPE,
					   sizeof(EMC_TOOL_SET_OFFSET)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int pocket;
    int toolno;
    EmcPose offset;
    double diameter;
    double frontangle;
    double backangle;
    int    orientation;
};

class EMC_TOOL_SET_NUMBER:public EMC_TOOL_CMD_MSG {
  public:
    EMC_TOOL_SET_NUMBER():EMC_TOOL_CMD_MSG(EMC_TOOL_SET_NUMBER_TYPE,
					   sizeof(EMC_TOOL_SET_NUMBER)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int tool; //number to use for currently loaded tool
    int toolcompflag;       //add by hong
    double toolcompvalue;
};

// EMC_TOOL status base class
class EMC_TOOL_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_TOOL_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_TOOL_STAT:public EMC_TOOL_STAT_MSG {
  public:
    EMC_TOOL_STAT();

    // For internal NML/CMS use only.
    void update(CMS * cms);
    EMC_TOOL_STAT operator =(EMC_TOOL_STAT s);	// need this for [] members

    int pocketPrepped;		// pocket ready for loading from
    int toolInSpindle;		// tool loaded, 0 is no tool
    int toolcompflag;       //add by hong
    int tooloffsetflag;       //add by hong
    double toolcompvalue[CANON_POCKETS_MAX];
    CANON_TOOL_TABLE toolTable[CANON_POCKETS_MAX];
};

// EMC_AUX type declarations

// EMC_AUX command base class
class EMC_AUX_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_AUX_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AUX_ESTOP_ON:public EMC_AUX_CMD_MSG {
  public:
    EMC_AUX_ESTOP_ON():EMC_AUX_CMD_MSG(EMC_AUX_ESTOP_ON_TYPE,
				       sizeof(EMC_AUX_ESTOP_ON)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AUX_ESTOP_OFF:public EMC_AUX_CMD_MSG {
  public:
    EMC_AUX_ESTOP_OFF():EMC_AUX_CMD_MSG(EMC_AUX_ESTOP_OFF_TYPE,
					sizeof(EMC_AUX_ESTOP_OFF)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AUX_ESTOP_RESET:public EMC_AUX_CMD_MSG {
  public:
    EMC_AUX_ESTOP_RESET():EMC_AUX_CMD_MSG(EMC_AUX_ESTOP_RESET_TYPE,
					sizeof(EMC_AUX_ESTOP_RESET)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AUX_INPUT_WAIT:public EMC_AUX_CMD_MSG {
  public:
    EMC_AUX_INPUT_WAIT():EMC_AUX_CMD_MSG(EMC_AUX_INPUT_WAIT_TYPE,
					sizeof(EMC_AUX_INPUT_WAIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int index;			// input channel to wait for
    int input_type;		// DIGITAL or ANALOG
    int wait_type;		// 0 - immediate, 1- rise, 2 - fall, 3 - be high, 4 - be low
    double timeout;		// timeout for waiting
};


// EMC_AUX status base class
class EMC_AUX_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_AUX_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_AUX_STAT:public EMC_AUX_STAT_MSG {
  public:
    EMC_AUX_STAT();

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int estop;			// non-zero means estopped
};

// EMC_SPINDLE type declarations

// EMC_SPINDLE command base class
class EMC_SPINDLE_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_SPINDLE_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_SPINDLE_SPEED:public EMC_SPINDLE_CMD_MSG {
  public:
    EMC_SPINDLE_SPEED():EMC_SPINDLE_CMD_MSG(EMC_SPINDLE_SPEED_TYPE,
					    sizeof(EMC_SPINDLE_SPEED)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
    double speed;   // commanded speed in RPMs or maximum speed for CSS
    double factor;  // Zero for constant RPM.  numerator of speed for CSS
    double xoffset; // X axis offset compared to center of rotation, for CSS
};

class EMC_SPINDLE_ON:public EMC_SPINDLE_CMD_MSG {
  public:
    EMC_SPINDLE_ON():EMC_SPINDLE_CMD_MSG(EMC_SPINDLE_ON_TYPE,
					 sizeof(EMC_SPINDLE_ON)),
	speed(0), factor(0), xoffset(0) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double speed;   // commanded speed in RPMs or maximum speed for CSS
    double factor;  // Zero for constant RPM.  numerator of speed for CSS
    double xoffset; // X axis offset compared to center of rotation, for CSS
    int pin;

};

class EMC_SPINDLE_OFF:public EMC_SPINDLE_CMD_MSG {
  public:
    EMC_SPINDLE_OFF():EMC_SPINDLE_CMD_MSG(EMC_SPINDLE_OFF_TYPE,
					  sizeof(EMC_SPINDLE_OFF)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_SPINDLE_INCREASE:public EMC_SPINDLE_CMD_MSG {
  public:
    EMC_SPINDLE_INCREASE():EMC_SPINDLE_CMD_MSG(EMC_SPINDLE_INCREASE_TYPE,
					       sizeof
					       (EMC_SPINDLE_INCREASE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double speed;		// commanded speed in RPMs
};

class EMC_SPINDLE_DECREASE:public EMC_SPINDLE_CMD_MSG {
  public:
    EMC_SPINDLE_DECREASE():EMC_SPINDLE_CMD_MSG(EMC_SPINDLE_DECREASE_TYPE,
					       sizeof
					       (EMC_SPINDLE_DECREASE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double speed;		// commanded speed in RPMs
};

class EMC_SPINDLE_CONSTANT:public EMC_SPINDLE_CMD_MSG {
  public:
    EMC_SPINDLE_CONSTANT():EMC_SPINDLE_CMD_MSG(EMC_SPINDLE_CONSTANT_TYPE,
					       sizeof
					       (EMC_SPINDLE_CONSTANT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double speed;		// commanded speed in RPMs
};

class EMC_SPINDLE_BRAKE_RELEASE:public EMC_SPINDLE_CMD_MSG {
  public:
    EMC_SPINDLE_BRAKE_RELEASE():EMC_SPINDLE_CMD_MSG
	(EMC_SPINDLE_BRAKE_RELEASE_TYPE,
	 sizeof(EMC_SPINDLE_BRAKE_RELEASE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_SPINDLE_BRAKE_ENGAGE:public EMC_SPINDLE_CMD_MSG {
  public:
    EMC_SPINDLE_BRAKE_ENGAGE():EMC_SPINDLE_CMD_MSG
	(EMC_SPINDLE_BRAKE_ENGAGE_TYPE, sizeof(EMC_SPINDLE_BRAKE_ENGAGE)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

// EMC_COOLANT type declarations

// EMC_COOLANT command base class
class EMC_COOLANT_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_COOLANT_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_COOLANT_MIST_ON:public EMC_COOLANT_CMD_MSG {
  public:
    EMC_COOLANT_MIST_ON():EMC_COOLANT_CMD_MSG(EMC_COOLANT_MIST_ON_TYPE,
					      sizeof(EMC_COOLANT_MIST_ON))
    {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_COOLANT_MIST_OFF:public EMC_COOLANT_CMD_MSG {
  public:
    EMC_COOLANT_MIST_OFF():EMC_COOLANT_CMD_MSG(EMC_COOLANT_MIST_OFF_TYPE,
					       sizeof
					       (EMC_COOLANT_MIST_OFF)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_COOLANT_FLOOD_ON:public EMC_COOLANT_CMD_MSG {
  public:
    EMC_COOLANT_FLOOD_ON():EMC_COOLANT_CMD_MSG(EMC_COOLANT_FLOOD_ON_TYPE,
					       sizeof
					       (EMC_COOLANT_FLOOD_ON)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_COOLANT_FLOOD_OFF:public EMC_COOLANT_CMD_MSG {
  public:
    EMC_COOLANT_FLOOD_OFF():EMC_COOLANT_CMD_MSG(EMC_COOLANT_FLOOD_OFF_TYPE,
						sizeof
						(EMC_COOLANT_FLOOD_OFF)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

// EMC_COOLANT status base class
class EMC_COOLANT_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_COOLANT_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_COOLANT_STAT:public EMC_COOLANT_STAT_MSG {
  public:
    EMC_COOLANT_STAT();

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int mist;			// 0 off, 1 on
    int flood;			// 0 off, 1 on
};

// EMC_LUBE type declarations

// EMC_LUBE command base class
class EMC_LUBE_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_LUBE_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_LUBE_ON:public EMC_LUBE_CMD_MSG {
  public:
    EMC_LUBE_ON():EMC_LUBE_CMD_MSG(EMC_LUBE_ON_TYPE, sizeof(EMC_LUBE_ON)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_LUBE_OFF:public EMC_LUBE_CMD_MSG {
  public:
    EMC_LUBE_OFF():EMC_LUBE_CMD_MSG(EMC_LUBE_OFF_TYPE,
				    sizeof(EMC_LUBE_OFF)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

// EMC_LUBE status base class
class EMC_LUBE_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_LUBE_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_LUBE_STAT:public EMC_LUBE_STAT_MSG {
  public:
    EMC_LUBE_STAT();

    // For internal NML/CMS use only.
    void update(CMS * cms);

    int on;			// 0 off, 1 on
    int level;			// 0 low, 1 okay
};

// EMC_IO is aggregate of all EMC IO-related status classes

// EMC_IO command base class
class EMC_IO_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_IO_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_IO_INIT:public EMC_IO_CMD_MSG {
  public:
    EMC_IO_INIT():EMC_IO_CMD_MSG(EMC_IO_INIT_TYPE, sizeof(EMC_IO_INIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_IO_HALT:public EMC_IO_CMD_MSG {
  public:
    EMC_IO_HALT():EMC_IO_CMD_MSG(EMC_IO_HALT_TYPE, sizeof(EMC_IO_HALT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_IO_ABORT:public EMC_IO_CMD_MSG {
  public:
    EMC_IO_ABORT():EMC_IO_CMD_MSG(EMC_IO_ABORT_TYPE, sizeof(EMC_IO_ABORT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_IO_SET_CYCLE_TIME:public EMC_IO_CMD_MSG {
  public:
    EMC_IO_SET_CYCLE_TIME():EMC_IO_CMD_MSG(EMC_IO_SET_CYCLE_TIME_TYPE,
					   sizeof(EMC_IO_SET_CYCLE_TIME)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    double cycleTime;
};

// EMC_IO status base class
class EMC_IO_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_IO_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
	heartbeat = 0;
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    unsigned long int heartbeat;
};

class EMC_IO_STAT:public EMC_IO_STAT_MSG {
  public:
    EMC_IO_STAT():EMC_IO_STAT_MSG(EMC_IO_STAT_TYPE, sizeof(EMC_IO_STAT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);

    // top-level stuff
    double cycleTime;
    int debug;			// copy of EMC_DEBUG global

    // aggregate of IO-related status classes
    EMC_TOOL_STAT tool;
    EMC_COOLANT_STAT coolant;
    EMC_AUX_STAT aux;
    EMC_LUBE_STAT lube;
  
};

// EMC is aggregate of EMC_TASK, EMC_TRAJ, EMC_IO, etc.

// EMC command base class
class EMC_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_INIT:public EMC_CMD_MSG {
  public:
    EMC_INIT():EMC_CMD_MSG(EMC_INIT_TYPE, sizeof(EMC_INIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_HALT:public EMC_CMD_MSG {
  public:
    EMC_HALT():EMC_CMD_MSG(EMC_HALT_TYPE, sizeof(EMC_HALT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_ABORT:public EMC_CMD_MSG {
  public:
    EMC_ABORT():EMC_CMD_MSG(EMC_ABORT_TYPE, sizeof(EMC_ABORT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

// EMC status base class

class EMC_STAT_MSG:public RCS_STAT_MSG {
  public:
    EMC_STAT_MSG(NMLTYPE t, size_t s):RCS_STAT_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_STAT:public EMC_STAT_MSG {
  public:
    EMC_STAT();

    // For internal NML/CMS use only.
    void update(CMS * cms);

    // the top-level EMC_TASK status class
    EMC_TASK_STAT task;

    // subordinate status classes
    EMC_MOTION_STAT motion;
    EMC_IO_STAT io;
    //EMC_DOUBLEZ_STAT doublez;//ghf-2014/2/24
    int debug;			// copy of EMC_DEBUG global
    unsigned int randtime;//ghf-2014/7/8
    unsigned int randbuf;//ghf-2014/7/8
};

//hong-create for program verify
class EMC_PRO_VERIFY_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_PRO_VERIFY_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    int verify_flag ;
    void update(CMS * cms);
};

class EMC_PRO_VERIFY_CMD:public EMC_PRO_VERIFY_CMD_MSG {
  public:
    EMC_PRO_VERIFY_CMD():EMC_PRO_VERIFY_CMD_MSG(EMC_PRO_VERIFY_TYPE, sizeof(EMC_INIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

//hong-2014/04/15 create for M99
class EMC_M99_CMD_MSG:public RCS_CMD_MSG {
  public:
    EMC_M99_CMD_MSG(NMLTYPE t, size_t s):RCS_CMD_MSG(t, s) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};

class EMC_M99_CMD:public EMC_M99_CMD_MSG {
  public:
    EMC_M99_CMD():EMC_M99_CMD_MSG(EMC_M99_CMD_TYPE, sizeof(EMC_INIT)) {
    };

    // For internal NML/CMS use only.
    void update(CMS * cms);
};


/*
   Declarations of EMC status class implementations, for major subsystems.
   These are defined in the appropriate main() files, and referenced
   by code in other files to get EMC status.
   */


#endif
