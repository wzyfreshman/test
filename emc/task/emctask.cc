/********************************************************************
* Description: emctask.cc
*   Mode and state management for EMC_TASK class
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

#include <stdlib.h>
#include <string.h>		// strncpy()
#include <sys/stat.h>		// struct stat
#include <unistd.h>		// stat()
#include <limits.h>		// PATH_MAX

#include "rcs.hh"		// INIFILE
#include "emc.hh"		// EMC NML
#include "emc_nml.hh"
#include "emcglb.h"		// EMC_INIFILE
#include "interpl.hh"		// NML_INTERP_LIST, interp_list
#include "canon.hh"		// CANON_VECTOR, GET_PROGRAM_ORIGIN()
#include "rs274ngc_interp.hh"	// the interpreter
#include "interp_return.hh"	// INTERP_FILE_NOT_OPEN
#include "inifile.hh"
#include "../ini/emcIniFile.hh"
#include "rcs_print.hh"
#include "task.hh"		// emcTaskCommand etc
#include "motion.h"

#define USER_DEFINED_FUNCTION_MAX_DIRS 5
#define MAX_M_DIRS (USER_DEFINED_FUNCTION_MAX_DIRS+1)





//note:the +1 is for the PROGRAM_PREFIX or default directory==nc_files
/* flag for how we want to interpret traj coord mode, as mdi or auto */
static int mdiOrAuto = EMC_TASK_MODE_AUTO;

Interp interp;

// EMC_TASK interface

/*
  format string for user-defined programs, e.g., "programs/M1%02d" means
  user-defined programs are in the programs/ directory and are named
  M1XX, where XX is a two-digit string.
*/
static double feed_old = 0.0;
static bool flag_run_sub = false;
static int lubeAfterrun = 0;


static char user_defined_fmt[MAX_M_DIRS][EMC_SYSTEM_CMD_LEN]; // ex: "dirname/M1%02d"


// index to directory for each user defined function:
static int user_defined_function_dirindex[USER_DEFINED_FUNCTION_NUM];

static void user_defined_add_m_code(int num, double arg1, double arg2)
{
    // num      is the m_code number, typically 00-99 corresponding to M100-M199
    char fmt[EMC_SYSTEM_CMD_LEN];
    EMC_SYSTEM_CMD system_cmd;

    //we call FINISH() to flush any linked motions before the M1xx call,
    //otherwise they would mix badly
    FINISH();
    strcpy(fmt, user_defined_fmt[user_defined_function_dirindex[num]]);
    strcat(fmt, " %f %f");
    snprintf(system_cmd.string, sizeof(system_cmd.string), fmt, num, arg1, arg2);
    interp_list.append(system_cmd);
}

int emcTaskInit()
{
    char mdir[MAX_M_DIRS][PATH_MAX+1];
    int num,dct,dmax;
    char path[EMC_SYSTEM_CMD_LEN];
    struct stat buf;
    IniFile inifile;
    const char *inistring;

    inifile.Open(EMC_INIFILE);

    // Identify user_defined_function directories
    if (NULL != (inistring = inifile.Find("PROGRAM_PREFIX", "DISPLAY"))) {
        strncpy(mdir[0],inistring, sizeof(mdir[0]));
        if (mdir[0][sizeof(mdir[0])-1] != '\0') {
            rcs_print("[DISPLAY]PROGRAM_PREFIX too long (max len %zu)\n", sizeof(mdir[0]));
            return -1;
        }
    } else {
        // default dir if no PROGRAM_PREFIX
        strncpy(mdir[0],"nc_files", sizeof(mdir[0]));
        if (mdir[0][sizeof(mdir[0])-1] != '\0') {
            rcs_print("default nc_files too long (max len %zu)\n", sizeof(mdir[0]));
            return -1;
        }
    }
    dmax = 1; //one directory mdir[0],  USER_M_PATH specifies additional dirs

    // user can specify a list of directories for user defined functions
    // with a colon (:) separated list
    if (NULL != (inistring = inifile.Find("USER_M_PATH", "RS274NGC"))) {
        char* nextdir;
        char tmpdirs[PATH_MAX];

        for (dct=1; dct < MAX_M_DIRS; dct++) mdir[dct][0] = 0;

        strncpy(tmpdirs,inistring, sizeof(tmpdirs));
        if (tmpdirs[sizeof(tmpdirs)-1] != '\0') {
            rcs_print("[RS274NGC]USER_M_PATH too long (max len %zu)\n", sizeof(tmpdirs));
            return -1;
        }

        nextdir = strtok(tmpdirs,":");  // first token
        dct = 1;
        while (dct < MAX_M_DIRS) {
            if (nextdir == NULL) break; // no more tokens
            strncpy(mdir[dct],nextdir, sizeof(mdir[dct]));
            if (mdir[dct][sizeof(mdir[dct])-1] != '\0') {
                rcs_print("[RS274NGC]USER_M_PATH component (%s) too long (max len %zu)\n", nextdir, sizeof(mdir[dct]));
                return -1;
            }
            nextdir = strtok(NULL,":");
            dct++;
        }
        dmax=dct;
    }
    inifile.Close();

    /* check for programs named programs/M100 .. programs/M199 and add
       any to the user defined functions list */
    for (num = 0; num < USER_DEFINED_FUNCTION_NUM; num++) {
    for (dct=0; dct < dmax; dct++) {
        if (!mdir[dct][0]) continue;
        snprintf(path, sizeof(path), "%s/M1%02d",mdir[dct],num);
        if (0 == stat(path, &buf)) {
            if (buf.st_mode & S_IXUSR) {
            // set the user_defined_fmt string with dirname
            // note the %%02d means 2 digits after the M code
            // and we need two % to get the literal %
            snprintf(user_defined_fmt[dct], sizeof(user_defined_fmt[0]),
                 "%s/M1%%02d", mdir[dct]); // update global

            USER_DEFINED_FUNCTION_ADD(user_defined_add_m_code,num);
            if (EMC_DEBUG & EMC_DEBUG_CONFIG) {
                rcs_print("emcTaskInit: adding user-defined function %s\n",
                 path);
            }
                user_defined_function_dirindex[num] = dct;
                break; // use first occurrence found for num
            } else {
            if (EMC_DEBUG & EMC_DEBUG_CONFIG) {
                rcs_print("emcTaskInit: user-defined function %s found, but not executable, so ignoring\n",
                 path);
            }
            }
        }
    }
    }

    return 0;
}

int emcTaskHalt()
{
    return 0;
}

int emcTaskAbort()
{
    printf("now in send abort\n");
    emcMotionAbort();    
    // clear out the pending command
    emcTaskCommand = 0;
    interp_list.clear();

    emcTrajSetG0Enable(0);
    // record the endline;
    if(0 != emcStatus->task.motionLine)
        emcStatus->task.setstartline = emcStatus->task.motionLine;
    // clear out the interpreter state
    emcStatus->task.interpState = EMC_TASK_INTERP_IDLE;
    emcStatus->task.execState = EMC_TASK_EXEC_DONE;
    emcStatus->task.task_paused = 0;
    emcStatus->task.motionLine = 0;
    emcStatus->task.readLine = 0;
    emcStatus->task.runState = EMC_IDLE;

    stepping = 0;
    steppingWait = 0;

    //clear the changing z flag add by hong-2014-07-11
    emcStatus->motion.doublez.z_changing = false;
    emcSpindleIgnore(0);

    // now queue up command to resynch interpreter
//    EMC_TASK_PLAN_SYNCH taskPlanSynchCmd;
//    emcTaskQueueCommand(&taskPlanSynchCmd);
     emcTaskPlanSynch();

    // without emcTaskPlanClose(), a new run command resumes at
    // aborted line-- feature that may be considered later
    {
    int was_open = taskplanopen;
    emcTaskPlanClose();
    if (EMC_DEBUG & EMC_DEBUG_INTERP && was_open) {
        rcs_print("emcTaskPlanClose() called at %s:%d\n", __FILE__,
              __LINE__);
    }
    }

    return 0;
}

int emcTaskSetMode(int mode)
{
    int retval = 0;

    switch (mode) {
    case EMC_TASK_MODE_MANUAL:
    // go to manual mode
    emcTrajSetMode(EMC_TRAJ_MODE_FREE);
    mdiOrAuto = EMC_TASK_MODE_AUTO;	// we'll default back to here
    break;

    case EMC_TASK_MODE_MDI:

        emcTaskAbort();//ghf-2015-4-10
        // go to mdi mode
    emcTrajSetMode(EMC_TRAJ_MODE_FREE);
        //emcTrajSetMode(EMC_TRAJ_MODE_COORD);//ghf-2015-4-11


//    emcTaskPlanSynch();
    mdiOrAuto = EMC_TASK_MODE_MDI;
    break;

    case EMC_TASK_MODE_AUTO:

        emcTaskAbort();//ghf-2015-4-10
        // go to auto mode
    emcTrajSetMode(EMC_TRAJ_MODE_COORD);


    //emcTaskPlanSynch();
    mdiOrAuto = EMC_TASK_MODE_AUTO;
    break;

    default:
    retval = -1;
    break;
    }

    return retval;
}

int emcTaskSetState(int state)
{
    int t;
    int retval = 0;
    switch (state) {
    case EMC_TASK_STATE_OFF:
        emcMotionAbort();
    // turn the machine servos off-- go into READY state
        emcSpindleAbort();
    for (t = 0; t < emcStatus->motion.traj.axes; t++) {
        emcAxisDisable(t);
    }
    emcTrajDisable();
    emcIoAbort();
    emcLubeOff();

    emcTaskAbort();
//        emcAxisUnhome(-2); // only those joints which are volatile_home
    //emcTaskPlanSynch();
    break;

    case EMC_TASK_STATE_ON:
    // turn the machine servos on
    emcTrajEnable();
    for (t = 0; t < emcStatus->motion.traj.axes; t++) {
        emcAxisEnable(t);
    }
    if(0 != lubeAfterrun)
        emcLubeOn();
    break;

    case EMC_TASK_STATE_ESTOP_RESET:
    // reset the estop
    emcAuxEstopOff();
    emcLubeOff();

    emcTaskAbort();
        emcIoAbort();
        emcSpindleAbort();
   // emcTaskPlanSynch();
    break;

    case EMC_TASK_STATE_ESTOP:
        emcMotionAbort();
        emcSpindleAbort();
    // go into estop-- do both IO estop and machine servos off
    emcAuxEstopOn();
    for (t = 0; t < emcStatus->motion.traj.axes; t++) {
        emcAxisDisable(t);
    }

    emcTrajDisable();
    emcLubeOff();
    emcTaskAbort();
        emcIoAbort();
//        emcAxisUnhome(-2); // only those joints which are volatile_home
    //emcTaskPlanSynch();
    break;

    default:
    retval = -1;
    break;
    }

    return retval;
}

// WM access functions

/*
  determineMode()

  Looks at mode of subsystems, and returns associated mode

  Depends on traj mode, and mdiOrAuto flag

  traj mode   mdiOrAuto     mode
  ---------   ---------     ----
  FREE        XXX           MANUAL
  COORD       MDI           MDI
  COORD       AUTO          AUTO
  */
static int determineMode()
{
    // if traj is in free mode, then we're in manual mode
    if ((emcStatus->motion.traj.mode == EMC_TRAJ_MODE_FREE &&
            mdiOrAuto != EMC_TASK_MODE_MDI ) ||
    emcStatus->motion.traj.mode == EMC_TRAJ_MODE_TELEOP) {
    return EMC_TASK_MODE_MANUAL;
    }
    // else traj is in coord mode-- we can be in either mdi or auto
    return mdiOrAuto;
}

/*
  determineState()

  Looks at state of subsystems, and returns associated state

  Depends on traj enabled, io estop, and desired task state

  traj enabled   io estop      state
  ------------   --------      -----
  DISABLED       ESTOP         ESTOP
  ENABLED        ESTOP         ESTOP
  DISABLED       OUT OF ESTOP  ESTOP_RESET
  ENABLED        OUT OF ESTOP  ON
  */
static int determineState()
{
    if (emcStatus->io.aux.estop) {
    return EMC_TASK_STATE_ESTOP;
    }

    if (!emcStatus->motion.traj.enabled) {
    return EMC_TASK_STATE_ESTOP_RESET;
    }

    return EMC_TASK_STATE_ON;
}

static int waitFlag = 0;

static char interp_error_text_buf[LINELEN];
static char interp_stack_buf[LINELEN];

static void print_interp_error(int retval)
{
    int index = 0;
    if (retval == 0) {
    return;
    }

    if (0 != emcStatus) {
    emcStatus->task.interpreter_errcode = retval;
    }

    interp_error_text_buf[0] = 0;
    interp.error_text(retval, interp_error_text_buf, LINELEN);
    if (0 != interp_error_text_buf[0]) {
    rcs_print_error("interp_error: %s\n", interp_error_text_buf);
    }
    emcOperatorError(0, "%s", interp_error_text_buf);
    index = 0;
    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
    rcs_print("Interpreter stack: \t");
    while (index < 5) {
        interp_stack_buf[0] = 0;
        interp.stack_name(index, interp_stack_buf, LINELEN);
        if (0 == interp_stack_buf[0]) {
        break;
        }
        rcs_print(" - %s ", interp_stack_buf);
        index++;
    }
    rcs_print("\n");
    }
}

void interpStatusInit()
{
    interp.interpStatusInit();
    PROGRAM_END();              //hong add to clear the currentToolOffset
}

int emcTaskPlanInit()
{
    interp.ini_load(EMC_INIFILE);
    waitFlag = 0;

    int retval = interp.init();
    if (retval > INTERP_MIN_ERROR) {
    print_interp_error(retval);
    } else {
    if (0 != RS274NGC_STARTUP_CODE[0]) {
        retval = interp.execute(RS274NGC_STARTUP_CODE);
        if (retval > INTERP_MIN_ERROR) {
        print_interp_error(retval);
        }
    }
    }

    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanInit() returned %d\n", retval);
    }

    return retval;
}

int emcTaskPlanSetWait()
{
    waitFlag = 1;
    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanSetWait() called\n");
    }

    return 0;
}

int emcTaskPlanIsWait()
{
    return waitFlag;
}

int emcTaskPlanClearWait()
{
    waitFlag = 0;
    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanClearWait() called\n");
    }

    return 0;
}

int emcTaskPlanSetOptionalStop(bool state)
{
    SET_OPTIONAL_PROGRAM_STOP(state);
    return 0;
}

int emcTaskPlanSetBlockDelete(bool state)
{
    SET_BLOCK_DELETE(state);
    return 0;
}

int emcTaskPlanSynch()
{
    if(emcStatus->task.currentLine < emcStatus->task.runfromline &&
    emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
    emcStatus->task.currentLine > 0)
        return 0;

    int retval = interp.synch();
    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanSynch() returned %d\n", retval);
    }
    return retval;
}

int emcTaskPlanExit()
{
    return interp.exit();
}

int emcTaskPlanOpen(const char *file)
{
    if (emcStatus != 0) {
    emcStatus->task.motionLine = 0;
    emcStatus->task.currentLine = 0;
    emcStatus->task.readLine = 0;
    }

    int retval = interp.interopen(file);
    if (retval > INTERP_MIN_ERROR) {
    print_interp_error(retval);
    return retval;
    }
    taskplanopen = 1;

    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanOpen(%s) returned %d\n", file, retval);
    }

    return retval;
}

int emcTaskPlanRead()
{
    int retval = interp.read();

    if (retval == INTERP_FILE_NOT_OPEN) {
    if (emcStatus->task.file[0] != 0) {
        retval = interp.interopen(emcStatus->task.file);
        if (retval > INTERP_MIN_ERROR) {
        print_interp_error(retval);
        }
        retval = interp.read();
    }
    }
    if (retval > INTERP_MIN_ERROR) {
    print_interp_error(retval);
    }

    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanRead() returned %d\n", retval);
    }

    return retval;
}

int emcTaskPlanExecute(const char *command)
{
    //printf("now get cmd:%s \n",command);
    int inpos = emcStatus->motion.traj.inpos;	// 1 if in position, 0 if not.
    if (command != 0) {		// Command is 0 if in AUTO mode, non-null if in MDI mode.
    // Don't sync if not in position.
    if ((*command != 0) && (inpos)) {

  //      interp.synch();       //forbid it because it return the error Z value
                                //lead to the first move include Z
    }
    }
    int retval = interp.execute(command);
    if (retval > INTERP_MIN_ERROR) {
    print_interp_error(retval);
    }
    if(command != 0) {
    FINISH();
    }

    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanExecute(0) return %d\n", retval);
    }

    return retval;
}

int emcTaskPlanExecute(const char *command, int line_number)
{
    int retval = interp.execute(command, line_number);
    if (retval > INTERP_MIN_ERROR) {
    print_interp_error(retval);
    }
    if(command != 0) { // this means MDI
    FINISH();
    }

    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanExecute(%s) returned %d\n", command, retval);
    }

    return retval;
}

int emcTaskPlanClose()
{
    int retval = interp.interclose();
    if (retval > INTERP_MIN_ERROR) {
    print_interp_error(retval);
    }

    taskplanopen = 0;
    return retval;
}

int emcTaskPlanReset()
{
    int retval = interp.reset();
    if (retval > INTERP_MIN_ERROR) {
    print_interp_error(retval);
    }
    CLEAR_ENDPOINTS();

    interp.synch();
    return retval;
}

int emcTaskPlanLine()
{
    int retval = interp.line();

    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanLine() returned %d\n", retval);
    }

    return retval;
}

int emcTaskSaveLine(int level = 0) {
    int retval = interp.osaveline(level);
    return retval;
}

int emcTaskPlanLevel()
{
    int retval = interp.call_level();

    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanLevel() returned %d\n", retval);
    }

    return retval;
}

int emcTaskPlanCommand(char *cmd)
{
    char buf[LINELEN];

    strcpy(cmd, interp.command(buf, LINELEN));

    if (EMC_DEBUG & EMC_DEBUG_INTERP) {
        rcs_print("emcTaskPlanCommand(%s) called. (line_number=%d)\n",
          cmd, emcStatus->task.readLine);
    }

    return 0;
}

int emcTaskUpdate(EMC_TASK_STAT * stat)
{
    stat->mode = (enum EMC_TASK_MODE_ENUM) determineMode();
    int oldstate = stat->state;
    stat->state = (enum EMC_TASK_STATE_ENUM) determineState();

    if(oldstate == EMC_TASK_STATE_ON && oldstate != stat->state) {

        emcTaskAbort();
        emcSpindleAbort();
        emcIoAbort();
    }

    // execState set in main
    // interpState set in main
    if (emcStatus->motion.traj.id > 0 && !flag_run_sub) {
        stat->motionLine = emcStatus->motion.traj.id;
    }
    // currentLine set in main
    // readLine set in main

 //   char buf[LINELEN];
//    strcpy(stat->file, interp.file(buf, LINELEN));
    // command set in main

    // update active G and M codes
    interp.active_g_codes(&stat->activeGCodes[0]);
    interp.active_m_codes(&stat->activeMCodes[0]);
    interp.active_settings(&stat->activeSettings[0]);

    //update state of optional stop
    stat->optional_stop_state = GET_OPTIONAL_PROGRAM_STOP();

    //update state of block delete
    stat->block_delete_state = GET_BLOCK_DELETE();

    //updte state of var parameters
    interp.get_var(&stat->parameters[0]);

    //get the actual feed speed when in g1 mode  by hong 2014-03-22
    if(!emcGetChangezFlag())
    stat->feed_rate = interp.get_feed_rate();


    stat->tool_comp_side = emcGetcompside();

    stat->heartbeat++;

    return 0;
}

bool emcRunsub()
{
    return flag_run_sub;
}

void emcSetrunsub(bool val)
{
    flag_run_sub = val;
}

//hong-2014/04/09 create for the Progran Verify Function
void emcSetOnlyReading(int val)
{
    interp.set_only_reading_flag(val);
}

int emcGetOnlyReading()
{
    return interp.get_only_reading_flag();
}

int emcGetcompside()
{

    return interp.get_comp_side();
}

int emcGettoolpocket()
{
    return GET_SINGLE_OFFSET_TOOL();
}


//hong use by program verify function
bool emcGetxyzFlag()
{
    return interp.get_xyz_flag();
}
//hong use by program verify function
void emcCheckLimits(double *x,double *y,double *z,int *gmode)
{
    if(interp.get_xyz_flag()){
        interp.get_read_end_pos(x,y,z,gmode);
    }
}
//hong get the m99_flag in the inter
bool emcGetM99Flag()
{
    return interp.get_m99_flag();
}

//hong init the status after exec the m99
//prepare for the next loop
void emcInitM99Status()
{
    interp.m99_init();
    interp.setParameter(4040,0);
}

bool emcGetChangezFlag()
{
    return interp.get_changez_flag();
}


void emcClearChangezFlag(int z_flag)
{
    emcSpindleIgnore(0);
    emcStatus->motion.doublez.z_changing = false;

    SET_Z_NOW(z_flag);

   SET_FEED_RATE(feed_old);
   interp.clear_changez_flag();
   interp.setParameter(4000,z_flag);
   int index = emcStatus->task.parameters[5220];

   SET_G5X_OFFSET(index,emcStatus->task.parameters[5201+index*20],
           emcStatus->task.parameters[5202+index*20],
           emcStatus->task.parameters[5203+index*20],
           emcStatus->task.parameters[5204+index*20],
           emcStatus->task.parameters[5205+index*20],
           emcStatus->task.parameters[5206+index*20],
           emcStatus->task.parameters[5207+index*20],
           emcStatus->task.parameters[5208+index*20],
           emcStatus->task.parameters[5209+index*20]);

   interp.synch();

}

void emcManualchangez(int val)
{
    emcSpindleIgnore(0);

    emcStatus->motion.doublez.z_changing = false;
    interp.setParameter(4000,val);

    printf("in send the manualchangez\n");

    switch(val)
    {
    case 0:interp.execute("G59.4"); break;
    case 1:interp.execute("G59.5");break;
    case 2:interp.execute("G59.6");break;
    default:break;
    }

    interp.synch();


}

void emcSetparameter(int index,double val,int noflush = 0)
{
    interp.setParameter(index,val);
    if(noflush == 0) {
        interp.synch();
    }
}

void emcSetspindlespeed(double val)
{
    SET_SPINDLE_SPEED_INCANNON(val);
}

void setOldfeed(double val)
{
    feed_old = val;
}

void setSmartrunLine(int line) {
    interp.setSmartrunLine(line);
}

void emcFlushpar()
{
//    IniFile inifile;

//    inifile.Open(EMC_INIFILE);

    const char *inistring;
    EmcIniFile inifile(EmcIniFile::ERR_TAG_NOT_FOUND |
                           //EmcIniFile::ERR_SECTION_NOT_FOUND |
                           EmcIniFile::ERR_CONVERSION);


    inifile.Open(EMC_INIFILE);

    try{


    if (NULL != (inistring = inifile.Find("AXES_TYPE", "EMCMOT"))) {
        emcSetparameter(3999,atof(inistring),1);
    } else {
        emcSetparameter(3999,0,1);
    }

    if (NULL != (inistring = inifile.Find("PROBE_TIMES", "TRAJ"))) {
        emcSetparameter(4001,atof(inistring),1);
    } else {
        // default dir if no PROBE_TIMES
        emcSetparameter(4001,1,1);
    }

    if (NULL != (inistring = inifile.Find("DOUBLEZ_X_DISTANCE", "RS274NGC"))) {
        emcSetparameter(4002,atof(inistring),1);
    } else {
         emcSetparameter(4002,0,1);
    }

    if (NULL != (inistring = inifile.Find("DOUBLEZ_Y_DISTANCE", "RS274NGC"))) {
        emcSetparameter(4003,atof(inistring),1);
    } else {
        emcSetparameter(4003,0,1);
    }

    if (NULL != (inistring = inifile.Find("Z3_X_DISTANCE", "RS274NGC"))) {
        emcSetparameter(4113,atof(inistring),1);
    } else {
        emcSetparameter(4113,0,1);
    }

    if (NULL != (inistring = inifile.Find("Z3_Y_DISTANCE", "RS274NGC"))) {
        emcSetparameter(4114,atof(inistring),1);
    } else {
        emcSetparameter(4114,0,1);
    }

    if (NULL != (inistring = inifile.Find("Z4_X_DISTANCE", "RS274NGC"))) {
        emcSetparameter(4115,atof(inistring),1);
    } else {
        emcSetparameter(4115,0,1);
    }

    if (NULL != (inistring = inifile.Find("Z4_Y_DISTANCE", "RS274NGC"))) {
        emcSetparameter(4116,atof(inistring),1);
    } else {
        emcSetparameter(4116,0,1);
    }

    if (NULL != (inistring = inifile.Find("Z5_X_DISTANCE", "RS274NGC"))) {
        emcSetparameter(4117,atof(inistring),1);
    } else {
        emcSetparameter(4117,0,1);
    }

    if (NULL != (inistring = inifile.Find("Z5_Y_DISTANCE", "RS274NGC"))) {
        emcSetparameter(4118,atof(inistring),1);
    } else {
        emcSetparameter(4118,0,1);
    }

    if (NULL != (inistring = inifile.Find("PROBE_VELOCITY", "TRAJ"))) {
        emcSetparameter(4004,atof(inistring),1);
    } else {
        // default dir if no PROBE_TIMES
        emcSetparameter(4004,2000,1);
    }

    if (NULL != (inistring = inifile.Find("PROBE_X", "TRAJ"))) {
        emcSetparameter(4005,atof(inistring),1);
    } else {
        rcs_print("[TRAJ]PROBE_X is zero!!!");
    }

    if (NULL != (inistring = inifile.Find("PROBE_Y", "TRAJ"))) {
        emcSetparameter(4006,atof(inistring),1);
    } else {
        rcs_print("[TRAJ]PROBE_Y is zero!!!");
    }

    if (NULL != (inistring = inifile.Find("PROBE_Z", "TRAJ"))) {
        emcSetparameter(4007,atof(inistring),1);
    } else {
        rcs_print("[TRAJ]PROBE_Z is zero!!!");
    }

    if (NULL != (inistring = inifile.Find("PROBE_LATCH_VELOCITY", "TRAJ"))) {
        emcSetparameter(4008,atof(inistring),1);
    } else {
        // default dir if no PROBE_LATCH_VELOCITY
        emcSetparameter(4008,30,1);
    }

    if (NULL != (inistring = inifile.Find("MAX_PROBE_DEEP", "TRAJ"))) {
        emcSetparameter(4009,atof(inistring),1);
    } else {
    }

    if (NULL != (inistring = inifile.Find("FLOATING_PROBE_HEIGHT", "TRAJ"))) {
        emcSetparameter(4018,atof(inistring),1);
    } else {

    }

    if (NULL != (inistring = inifile.Find("FAST_PROBE_VELOCITY", "TRAJ"))) {
        emcSetparameter(4089,atof(inistring),1);
        printf("now I get in #4089 = %.3f\n",atof(inistring));
    } else {
        emcSetparameter(4089,480,1);
    }

    if (NULL != (inistring = inifile.Find("PROBE_TOLERANCE", "TRAJ"))) {
        emcSetparameter(4015,atof(inistring),1);
    } else {
        emcSetparameter(4015,0.1,1);
    }

    if (NULL != (inistring = inifile.Find("PROBE_Z_SPEED", "TRAJ"))) {
        emcSetparameter(4016,atof(inistring),1);
    } else {
        emcSetparameter(4016,1500,1);
    }

    if (NULL != (inistring = inifile.Find("MAX_JOGSPEED_BEFORE_HOME", "TRAJ"))) {
        emcSetparameter(4017,atof(inistring),1);
    } else {
        emcSetparameter(4017,1500,1);
    }

    if (NULL != (inistring = inifile.Find("RUNWITHON", "OPERATOR"))) {
        lubeAfterrun = atoi(inistring);
    } else {
        lubeAfterrun = 0;
    }

    if (NULL != (inistring = inifile.Find("ABORT_LIFT_Z", "TASK"))) {
        emcSetparameter(4020,atof(inistring),1);
    } else {
        emcSetparameter(4020,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_AFTER_PROBE", "TASK"))) {
        emcSetparameter(4021,atof(inistring),1);
    } else {
        emcSetparameter(4021,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_AFTER_ABORT", "TASK"))) {
        emcSetparameter(4022,atof(inistring),1);
    } else {
        emcSetparameter(4022,0,1);
    }

    if (NULL != (inistring = inifile.Find("SPEED_OF_MOVE", "TASK"))) {
        emcSetparameter(4023,atof(inistring),1);
    } else {
        emcSetparameter(4023,7000,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_COOR_X", "TASK"))) {
        emcSetparameter(4024,atof(inistring),1);
    } else {
        emcSetparameter(4024,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_COOR_Y", "TASK"))) {
        emcSetparameter(4025,atof(inistring),1);
    } else {
        emcSetparameter(4025,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_COOR_Z", "TASK"))) {
        emcSetparameter(4026,atof(inistring),1);
    } else {
        emcSetparameter(4026,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_COOR_A", "TASK"))) {
        emcSetparameter(4140,atof(inistring),1);
    } else {
        emcSetparameter(4140,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_COOR_B", "TASK"))) {
        emcSetparameter(4141,atof(inistring),1);
    } else {
        emcSetparameter(4141,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_COOR_C", "TASK"))) {
        emcSetparameter(4142,atof(inistring),1);
    } else {
        emcSetparameter(4142,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_G5X_X", "TASK"))) {
        emcSetparameter(4027,atof(inistring),1);
    } else {
        emcSetparameter(4027,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_G5X_Y", "TASK"))) {
        emcSetparameter(4028,atof(inistring),1);
    } else {
        emcSetparameter(4028,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_G5X_Z", "TASK"))) {
        emcSetparameter(4029,atof(inistring),1);
    } else {
        emcSetparameter(4029,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_G5X_A", "TASK"))) {
        emcSetparameter(4143,atof(inistring),1);
    } else {
        emcSetparameter(4143,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_G5X_B", "TASK"))) {
        emcSetparameter(4144,atof(inistring),1);
    } else {
        emcSetparameter(4144,0,1);
    }

    if (NULL != (inistring = inifile.Find("MOVE_G5X_C", "TASK"))) {
        emcSetparameter(4145,atof(inistring),1);
    } else {
        emcSetparameter(4145,0,1);
    }


    if (NULL != (inistring = inifile.Find("SAFE_COOR_Z", "TASK"))) {
        emcSetparameter(4030,atof(inistring),1);
    } else {
        emcSetparameter(4030,0,1);
    }

    if (NULL != (inistring = inifile.Find("GO_OFFSET_SPEEDZ", "TASK"))) {
        emcSetparameter(4031,atof(inistring),1);
    } else {
        emcSetparameter(4031,1000,1);
    }

    if (NULL != (inistring = inifile.Find("LIFTZ_BERORE_RUN", "TASK"))) {
        emcSetparameter(4032,atof(inistring),1);
    } else {
        emcSetparameter(4032,0,1);
    }

    if (NULL != (inistring = inifile.Find("SPINDLE_OFF_AFTER_ABORT", "TASK"))) {
        emcSetparameter(4033,atof(inistring),1);
    } else {
        emcSetparameter(4033,0,1);
    }

    if (NULL != (inistring = inifile.Find("COOL_ON_BEFORE_RUN", "TASK"))) {
        emcSetparameter(4034,atof(inistring),1);
    } else {
        emcSetparameter(4034,0,1);
    }

    if (NULL != (inistring = inifile.Find("COOL_OFF_AFTER_ABORT", "TASK"))) {
        emcSetparameter(4035,atof(inistring),1);
    } else {
        emcSetparameter(4035,0,1);
    }

    //*********janmi20150807********//
    if(NULL!=(inistring=inifile.Find("MAX_LIMIT","AXIS_0"))){
        emcSetparameter(4051,atof(inistring),1);
    }else {
        emcSetparameter(4051,0,1);
    }

    if(NULL!=(inistring=inifile.Find("MIN_LIMIT","AXIS_0"))){
        emcSetparameter(4052,atof(inistring),1);
    }else {
        emcSetparameter(4052,0,1);
    }

    if(NULL!=(inistring=inifile.Find("MAX_LIMIT","AXIS_1"))){
        emcSetparameter(4053,atof(inistring),1);
    }else {
        emcSetparameter(4053,0,1);
    }

    if(NULL!=(inistring=inifile.Find("MIN_LIMIT","AXIS_1"))){
        emcSetparameter(4054,atof(inistring),1);
    }else {
        emcSetparameter(4054,0,1);
    }

    if(NULL!=(inistring=inifile.Find("MAX_LIMIT","AXIS_2"))){
        emcSetparameter(4055,atof(inistring),1);
    }else {
        emcSetparameter(4055,-0,1);
    }

    if(NULL!=(inistring=inifile.Find("MIN_LIMIT","AXIS_2"))){
        emcSetparameter(4056,atof(inistring),1);
    }else {
        emcSetparameter(4056,-0,1);
    }

    if(NULL!=(inistring=inifile.Find("WARMZ","TASK"))){
        emcSetparameter(4067,atof(inistring),1);
    }else {
        emcSetparameter(4067,-0,1);
    }

    if(NULL!=(inistring=inifile.Find("WARM_SPEED","TASK"))){
        emcSetparameter(4068,atof(inistring),1);
    }else {
        emcSetparameter(4068,-0,1);
    }
  //*********janmi20150807********^//

    if (NULL != (inistring = inifile.Find("OPTIONSTOP", "TASK"))) {
        emcTaskPlanSetOptionalStop(atoi(inistring));
    } else {
        emcTaskPlanSetOptionalStop(0);
    }

    if (NULL != (inistring = inifile.Find("FEED_CHOICE_MODE", "OPERATOR"))) {
        interp.setFeedChoiceMode(atoi(inistring));
    } else {
        interp.setFeedChoiceMode(0);
    }

    double feedz = 0,feedxy = 0;
    if (NULL != (inistring = inifile.Find("DEFAULT_VELOCITY", "TRAJ"))) {
        feedxy = atof(inistring)*60;
    } else {
        feedxy = 6000;
    }
    if (NULL != (inistring = inifile.Find("DEFAULT_FEED_Z_VELOCITY", "TRAJ"))) {
        feedz = atof(inistring)*60;
    } else {
        feedz = 1000;
    }
    interp.setFeedDefaultZ(feedz,feedxy);

    if (NULL != (inistring = inifile.Find("MULTISYS_ENABLE", "TASK"))) {
        emcSetparameter(4061,atof(inistring),1);
    } else {
        emcSetparameter(4061,0,1);
    }

//    if (NULL != (inistring = inifile.Find("TOOLDELAY", "RS274"))) {
//        emcSetparameter(4302,atof(inistring),1);
//    } else {
//        emcSetparameter(4302,0,1);
//    }

    if (NULL != (inistring = inifile.Find("APPROACH_DISTANCE", "TASK"))) {
        emcSetparameter(4080,atof(inistring),1);
    } else {
        emcSetparameter(4080,0,1);
    }

    if (NULL != (inistring = inifile.Find("APPROACH_SPEED", "TASK"))) {
        emcSetparameter(4081,atof(inistring),1);
    } else {
        emcSetparameter(4081,20,1);
    }

    // set aux_which_z value to 4111
    emcSetparameter(4111,aux_which_z,1);

    if (NULL != (inistring = inifile.Find("BASE_CHANGE_POS", "TASK"))) {
        emcSetparameter(4088,atof(inistring),1);
        emcSetMultifunc(3,0,atof(inistring));
    } else {
        emcSetparameter(4088,0,1);
        emcSetMultifunc(3,0,0);
    }

    if (NULL != (inistring = inifile.Find("ROBOT_SAFE_POS_Y", "TASK"))) {
        emcSetparameter(4049,atof(inistring),1);
    } else {
        emcSetparameter(4049,0,1);
    }

    if (NULL != (inistring = inifile.Find("USE_CCD", "TASK"))) {
        emcSetparameter(4069,atoi(inistring),0);
    } else {
        emcSetparameter(4069,0,1);
    }

    if (NULL != (inistring = inifile.Find("HIDE_WORKSTATION", "TASK"))) {
        emcSetparameter(3120,atoi(inistring),0);
    } else {
        emcSetparameter(3120,0,1);
    }





    char tempbuf[256];
    for(int i=0;i<10;i++) {
        sprintf(tempbuf,"USRDEF_PAR_%d",i);
        if (NULL != (inistring = inifile.Find(tempbuf, "TASK"))) {
            emcSetparameter(4090+i,atof(inistring),1);
        } else {
            emcSetparameter(4090+i,0,1);
        }

    }


    } catch(EmcIniFile::Exception &e) {
        e.Print();
        inifile.Close();
        return;
    }
    

    if (NULL != (inistring = inifile.Find("ARTIFACT_ZERO_Z", "TASK"))) {
        emcSetparameter(4101,atof(inistring),1);
    } else {
        emcSetparameter(4101,0,1);
    }
    if (NULL != (inistring = inifile.Find("SEPERATE_XYZ", "TASK"))) {
        emcSetparameter(4129,atof(inistring),1);
    } else {
        emcSetparameter(4129,0,1);
    }
    if (NULL != (inistring = inifile.Find("G0SEPERATE_XYZ", "TASK"))) {
        emcSetparameter(4125,atof(inistring),1);
    } else {
        emcSetparameter(4125,0,1);
    }
    if (NULL != (inistring = inifile.Find("IGNORE_M5", "TASK"))) {
        emcSetparameter(4128,atof(inistring),1);
    } else {
        emcSetparameter(4128,0,1);
    }
    //是否使用坐标系
    if (NULL != (inistring = inifile.Find("USE_OFFSET", "TASK"))) {
        emcSetparameter(3007,atof(inistring),1);
    } else {
        emcSetparameter(3007,0,1);
    }

    if (NULL != (inistring = inifile.Find("USE_OFFSET", "TASK"))) {
        emcSetparameter(3007,atof(inistring),1);
    } else {
        emcSetparameter(3007,0,1);
    }

    if (NULL != (inistring = inifile.Find("G86OFFSET_DIR", "RS274"))) {
        emcSetparameter(4150,atof(inistring),1);
    } else {
        emcSetparameter(4150,0,1);
    }
//    if (NULL != (inistring = inifile.Find("IGNORE_TOOL", "TASK"))) {
//        emcSetparameter(4127,atof(inistring),1);
//    } else {
//        emcSetparameter(4127,0,1);
//    }

    if (NULL != (inistring = inifile.Find("USE_ABCENCODER", "TRAJ"))) {
        emcSetparameter(4070,atof(inistring),1);
    } else {
        emcSetparameter(4070,0,0);
    }

    if (NULL != (inistring = inifile.Find("A_SHORTESTMOVE", "TRAJ"))) {
        emcSetMultifunc(60,3,atof(inistring));
    } else {
        emcSetMultifunc(60,3,0);
    }
    if (NULL != (inistring = inifile.Find("B_SHORTESTMOVE", "TRAJ"))) {
        emcSetMultifunc(60,4,atof(inistring));
    } else {
        emcSetMultifunc(60,4,0);
    }
    if (NULL != (inistring = inifile.Find("C_SHORTESTMOVE", "TRAJ"))) {
        emcSetMultifunc(60,5,atof(inistring));
    } else {
        emcSetMultifunc(60,5,0);
    }

    inifile.Close();
    interp.ini_load(EMC_INIFILE);


}

int emcAbortcmd()
{
    int rev = 0;
    rev = emcTaskPlanExecute("g40g49g92.1");
    rev = emcTaskPlanExecute("#4041=0 #4042=0 #4043=0 #4044=0");
    if(emcStatus->task.parameters[4061] > 0  && emcStatus->task.g5x_index != emcStatus->task.parameters[5220] && emcStatus->task.g5x_index > 0) {
        char cmd[256];
        sprintf(cmd,"G%d",53+emcStatus->task.g5x_index);
        rev = emcTaskPlanExecute(cmd);
    }
   return rev;
}

int *emcGettoolexist()
{
    return interp.tool_exist();
}


int emcAbortliftz()
{

    int rev = 0;
    if(0 == emcStatus->task.parameters[4020]  && NULL==strstr(emcStatus->task.file,"Probe.ngc"))
        return rev;

    if(0 == emcStatus->motion.traj.scale) {
            return rev;
    }

    double l = emcStatus->task.parameters[4030];
    double cx,cy,cz;
    double x,y,z,a;
    cx = emcStatus->motion.axis[0].output;
    cy = emcStatus->motion.axis[1].output;
    cz = emcStatus->motion.axis[2].output;
    a = emcStatus->motion.axis[3].output;
    x = cx;
    y = cy - l*sin(2*M_PI*a/360);
    z = cz + l*cos(2*M_PI*a/360);
    if(z > emcStatus->task.parameters[4055] - 1) {
        z =  emcStatus->task.parameters[4055] - 1;
    }
    if(y < emcStatus->task.parameters[4054] + 1) {
        y = emcStatus->task.parameters[4054] + 1;
    }

    char cmd[256];
//    sprintf(cmd,"g53g90g1x%.3fY%.3fz%.3f F3000",x,y,z);
    sprintf(cmd,"g53g90g1z%.3f F3000",z);
    printf("nwo I get abortz is %.3f %.3f %.3f %.3f\n",x,y,z,a);
    rev = emcTaskPlanExecute(cmd);
    /*
    if(emcStatus->task.parameters
            [5203+(int)emcStatus->task.parameters[5220]*20]
            + emcStatus->task.parameters[4030] >= emcStatus->task.parameters[4055]) {
            rev = emcTaskPlanExecute("g53g90g0z[[#4055]-1]");
    } else if(emcStatus->motion.traj.actualPosition.tran.z < emcStatus->task.parameters
              [5203+(int)emcStatus->task.parameters[5220]*20] + emcStatus->task.parameters[4030]) {
        char temp[200];
        sprintf(temp,"G53G90G0Z%.3f",emcStatus->task.parameters[5203+
                (int)emcStatus->task.parameters[5220]*20]
                + emcStatus->task.parameters[4030]);
        rev = emcTaskPlanExecute(temp);
    }else {
        rev = emcTaskPlanExecute("g53g90g0z[[#4055]-1]");
    }
    */
    return rev;
}

int emcAbortstatus(int spindle,int cool)
{
    int rev = 0;
    if(0 < spindle) {
        rev = emcSpindleAbort();
    }
    if(0 < cool) {
        rev = emcCoolantFloodOff();
    }

    return rev;
}

void emcSetcurrentx(double val)
{
    printf("now Set Current Z %f\n",val);
    interp.set_current_x(val);
}

void emcSetcurrenty(double val)
{
    interp.set_current_y(val);
}

void emcSetcurrentz(double val)
{
    interp.set_current_z(val);
}

void emcSetcurrenta(double val)
{
    interp.set_current_a(val);
}
void emcGetInterpFile(char *file) {
    interp.file_name(file,LINELEN);
}

int emcSetChoiceDefaultFeed(double feed) {
    interp.set_choice_default_feed(feed);
    return 0;
}
