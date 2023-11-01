/********************************************************************
 * Description: emctaskmain.cc
 *   Main program for EMC task level
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
/*
 Principles of operation:

 1.  The main program calls emcTaskPlan() and emcTaskExecute() cyclically.

 2.  emcTaskPlan() reads the new command, and decides what to do with
 it based on the mode (manual, auto, mdi) or state (estop, on) of the
 machine. Many of the commands just go out immediately to the
 subsystems (motion and IO). In auto mode, the interpreter is called
 and as a result the interp_list is appended with NML commands.


 3.  emcTaskExecute() executes a big switch on execState. If it's done,
 it gets the next item off the interp_list, and sets execState to the
 preconditions for that. These preconditions include waiting for motion,
 waiting for IO, etc. Once they are satisfied, it issues the command, and
 sets execState to the postconditions. Once those are satisfied, it gets
 the next item off the interp_list, and so on.

 4.  preconditions and postconditions are only looked at in conjunction
 with commands on the interp_list. Immediate commands won't have any
 pre- or postconditions associated with them looked at.

 5.  At this point, nothing in this file adds anything to the interp_list.
 This could change, for example, when defining pre- and postconditions for
 jog or home commands. If this is done, make sure that the corresponding
 abort command clears out the interp_list.

 6. Single-stepping is handled in checkPreconditions() as the first
 condition. If we're in single-stepping mode, as indicated by the
 variable 'stepping', we set the state to waiting-for-step. This
 polls on the variable 'steppingWait' which is reset to zero when a
 step command is received, and set to one when the command is
 issued.
 */

#include <stdio.h>		// vsprintf()
#include <string.h>		// strcpy()
#include <stdarg.h>		// va_start()
#include <stdlib.h>		// exit()
#include <signal.h>		// signal(), SIGINT
#include <float.h>		// DBL_MAX
#include <sys/types.h>		// pid_t
#include <unistd.h>		// fork()
#include <sys/wait.h>		// waitpid(), WNOHANG, WIFEXITED
#include <ctype.h>		// isspace()
#include <libintl.h>
#include <locale.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/rtc.h>

#include "lkt5103.h"
#include "epp.h"

#if 0
// Enable this to niftily trap floating point exceptions for debugging
#include <fpu_control.h>
fpu_control_t __fpu_control = _FPU_IEEE & ~(_FPU_MASK_IM | _FPU_MASK_ZM | _FPU_MASK_OM);
#endif

#include "des.h"
#include "rcs.hh"		// NML classes, nmlErrorFormat()
#include "emc.hh"		// EMC NML
#include "emc_nml.hh"
#include "canon.hh"		// CANON_TOOL_TABLE stuff
#include "inifile.hh"		// INIFILE
#include "interpl.hh"		// NML_INTERP_LIST, interp_list
#include "emcglb.h"		// EMC_INIFILE,NMLFILE, EMC_TASK_CYCLE_TIME
#include "interp_return.hh"	// public interpreter return values
#include "interp_internal.hh"	// interpreter private definitions
#include "rcs_print.hh"
#include "timer.hh"
#include "nml_oi.hh"
#include "task.hh"		// emcTaskCommand etc
#include "hash.h"
#include "motion.h"



// command line args-- global so that other modules can access
int Argc;
char **Argv;

// NML channels
static RCS_CMD_CHANNEL *emcCommandBuffer = 0;
static RCS_STAT_CHANNEL *emcStatusBuffer = 0;
static NML *emcErrorBuffer = 0;

// NML command channel data pointer
static RCS_CMD_MSG *emcCommand = 0;

// global EMC status
EMC_STAT *emcStatus = 0;

// timer stuff
static RCS_TIMER *timer = 0;

// flag signifying that ini file [TASK] CYCLE_TIME is <= 0.0, so
// we should not delay at all between cycles. This means also that
// the EMC_TASK_CYCLE_TIME global will be set to the measured cycle
// time each cycle, in case other code references this.
static int emcTaskNoDelay = 0;
// flag signifying that on the next loop, there should be no delay.
// this is set when transferring trajectory data from userspace to kernel
// space, annd reset otherwise.
static int emcTaskEager = 0;
static int waitforzchangeend = 0;

static double EMC_TASK_CYCLE_TIME_ORIG = 0.0;

// delay counter
static double taskExecDelayTimeout = 0.0;



// emcTaskIssueCommand issues command immediately
static int emcTaskIssueCommand(NMLmsg * cmd);

static EMC_DOUBLEZ_CMD *emcDoublezmsg; // NEW DOUBLEZ TYPE

static EMC_PRO_VERIFY_CMD *emcProVerifymsg; // program verify

static EMC_JOGWHEELS_GUIDE_CMD *emcJogwheels_guidemsg;

static EMC_SETGMODE_CMD *emcSetgmodemsg;

static EMC_SETLIGHT_CMD *emcSetlightmsg;

static EMC_ENCRYPT_CMD *emcEncryptmsg;

static EMC_FILTERTIME_CMD *emcFiltertimemsg;

static EMC_TASK_EXECUTE_BLOCK *emcExecuteblockmsg;

static EMC_TASK_AUTOREPEAT_PAR *emcAutorepeatmsg;

static EMC_TASK_SET_LEFTTIME   *emcSetlefttime;



// pending command to be sent out by emcTaskExecute()
NMLmsg *emcTaskCommand = 0;

// signal handling code to stop main loop
static int done;
static int emctask_shutdown(void);
static int iniLoad(const char *filename);
static int iniFlush(const char *filename);
static void clearDoublez();
static int emctask_startup();
static long kernel_mktime (struct rtc_time *tm); //compare the time
static long mydifftime(unsigned int *tm1, unsigned int *tm2);
static int toolchangeend();
static int pseudoMdiLineNumber = INT_MIN;

//hong 2014-03-24
static bool flag_run_inwaiting = false;
static bool flag_pro_run = false;
static bool flag_mdi_run = false;
static double pro_run_time = 0;    //hong 140324 mark the program running time
static bool doublez_enable = false;
static double spindle_delay = 0;
static int spindleon_before_run = 0;
static int coolanton_before_run = 0;
static double spindleDelaytimeout = 0;
static int spindlealreadyon = 0;
static int aux_m6_flag = 0;
static int oldset_which_z = 0;
static int changezSuspend = 0;
static int zchangeflag = EMC_NO_CHANGE;
static double aux_zdiff = 0;
static char zmdibuf[32] = { 0 };
static char fmdibuf[32] = { 0 };
static double dfspindlespeed = 0;
static int sMode = 0;
static int abortSpindleoff = 0;
static int abortcooloff = 0;
static int axes = 3;
static double maxzdiff = 30;
static int changeSpindleAndStop = 1;
static int checkexternIo = 0;
static int zmode = 0;
static int randnumcount = 0;
static bool verifycontrol = false;
static bool runlinedelay = false;
static const char *subncpath[LINELEN];
static bool runsubabortz = false;
static bool flushmotionini = false;
static bool runprobe = false;
static int g4delaytype = 0;
static int manualtoautochangez = 0;
static char startcmd[500][LINELEN]={""};
static int startsize = 0;
static char finishcmd[500][LINELEN]={""};
static int finishsize = 0;
static int startspindleflag = 0;
static int execerrorflag = 0;
static char tblfile[512];

static double static_max_follow_err = 0.000; //ghf-2014/5/19,通过iniload上来 mm

static double change_zaxis_vel = 3000; //ghf-2014/5/19,通过iniload上来 mm/min

// shorthand typecasting ptrs
static EMC_AXIS_HALT *axis_halt_msg;
static EMC_AXIS_DISABLE *disable_msg;
static EMC_AXIS_ENABLE *enable_msg;
static EMC_AXIS_HOME *home_msg;
static EMC_AXIS_UNHOME *unhome_msg;
static EMC_AXIS_JOG *jog_msg;
static EMC_AXIS_ABORT *axis_abort_msg;
static EMC_AXIS_INCR_JOG *incr_jog_msg;
static EMC_AXIS_ABS_JOG *abs_jog_msg;
static EMC_AXIS_SET_BACKLASH *set_backlash_msg;
static EMC_AXIS_SET_HOMING_PARAMS *set_homing_params_msg;
static EMC_AXIS_SET_FERROR *set_ferror_msg;
static EMC_AXIS_SET_MIN_FERROR *set_min_ferror_msg;
static EMC_AXIS_SET_MAX_POSITION_LIMIT *set_max_limit_msg;
static EMC_AXIS_SET_MIN_POSITION_LIMIT *set_min_limit_msg;
static EMC_AXIS_OVERRIDE_LIMITS *axis_lim_msg;
//static EMC_AXIS_SET_OUTPUT *axis_output_msg;
static EMC_AXIS_LOAD_COMP *axis_load_comp_msg;
//static EMC_AXIS_SET_STEP_PARAMS *set_step_params_msg;

static EMC_TRAJ_SET_SCALE *emcTrajSetScaleMsg;
static EMC_TRAJ_SET_MAX_VELOCITY *emcTrajSetMaxVelocityMsg;
static EMC_TRAJ_SET_SPINDLE_SCALE *emcTrajSetSpindleScaleMsg;
static EMC_TRAJ_SET_VELOCITY *emcTrajSetVelocityMsg;
static EMC_TRAJ_SET_ACCELERATION *emcTrajSetAccelerationMsg;
static EMC_TRAJ_LINEAR_MOVE *emcTrajLinearMoveMsg;
static EMC_TRAJ_CIRCULAR_MOVE *emcTrajCircularMoveMsg;
static EMC_TRAJ_DELAY *emcTrajDelayMsg;
static EMC_TRAJ_SET_TERM_COND *emcTrajSetTermCondMsg;
static EMC_TRAJ_SET_SPINDLESYNC *emcTrajSetSpindlesyncMsg;
static EMC_TRAJ_MINLINEAR_MOVE *emcTrajMinLinearMsg;
static EMC_TASK_WAIT_FOR_SIGNAL *emcWaitforsignalMsg;
static EMC_TASK_SET_SIGNAL *emcSetsignalMsg;
static EMC_TASK_SET_PARAMETER *emcSetparameterMsg;
static EMC_TRAJ_MULTI_FUNCTION *emcMultifuncMsg;

// These classes are commented out because the compiler
// complains that they are "defined but not used".
//static EMC_MOTION_SET_AOUT *emcMotionSetAoutMsg;
//static EMC_MOTION_SET_DOUT *emcMotionSetDoutMsg;

static EMC_SPINDLE_SPEED *spindle_speed_msg;
static EMC_SPINDLE_ON *spindle_on_msg;
static EMC_TOOL_PREPARE *tool_prepare_msg;
static EMC_TOOL_LOAD_TOOL_TABLE *load_tool_table_msg;
static EMC_TOOL_SET_OFFSET *emc_tool_set_offset_msg;
static EMC_TOOL_SET_NUMBER *emc_tool_set_number_msg;
static EMC_TASK_SET_MODE *mode_msg;
static EMC_TASK_SET_STATE *state_msg;
static EMC_TASK_PLAN_RUN *run_msg;
static EMC_TASK_PLAN_EXECUTE *execute_msg;
static EMC_TASK_PLAN_OPEN *open_msg;
static EMC_TASK_PLAN_SET_OPTIONAL_STOP *os_msg;
static EMC_TASK_PLAN_SET_BLOCK_DELETE *bd_msg;
static EMC_TASK_FLUSH_PAR *flushparmsg;

static EMC_AUX_INPUT_WAIT *emcAuxInputWaitMsg;
static int emcAuxInputWaitType = 0;
static int emcAuxInputWaitIndex = -1;

// commands we compose here
static EMC_TASK_PLAN_RUN taskPlanRunCmd;	// 16-Aug-1999 FMP
static EMC_TASK_PLAN_INIT taskPlanInitCmd;
static EMC_TASK_PLAN_SYNCH taskPlanSynchCmd;
static EMC_TASK_PLAN_NULL taskPlanNullCmd;

static EMC_SETLIGHT_CMD setlightmsg;


static int interpResumeState = EMC_TASK_INTERP_IDLE;
static int programStartLine = 0;	// which line to run program from
// how long the interp list can be

int stepping = 0;
int steppingWait = 0;
static int steppedLine = 0;

// Variables to handle MDI call interrupts
// Depth of call level before interrupted MDI call
static int mdi_execute_level = -1;
// Schedule execute(0) command
static int mdi_execute_next = 0;
// Wait after interrupted command
static int mdi_execute_wait = 0;
// Side queue to store MDI commands
static NML_INTERP_LIST mdi_execute_queue;
static int execstartngcflag = 0;
char substartfile[LINELEN];


static int taskdecryptInfo(unsigned char final[64]) {

    emcStatus->task.hardwarecheck = 0;
    unsigned char cpu[8];//存放密文密钥中的cpuid密文(fpga串号)
    unsigned char hd[8];//存放密文密钥中的硬盘id密文
    unsigned char chip[8];//存放密文密钥中的加密芯片id密文
    unsigned char key[16];////存放密文密钥中的密钥
    des3_context ctx;//用于解密用的密钥结构体
    int i=0,j=0;
    static unsigned char cpuidl[8];//解密后的cpuid
    static unsigned char hdid[8];//解密后的hdid
    static unsigned char chipid[8];//解密后的加密芯片id

    static unsigned char orghdid[20];
    static unsigned char hdidcrc[8];

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

    des3_set_2keys(&ctx,key,&key[8]);
    des3_decrypt(&ctx,cpu,cpuidl);
    des3_decrypt(&ctx,hd,hdid);
    des3_decrypt(&ctx,chip,chipid);



    struct hd_driveid id;
    int  fd = open("/dev/sda", O_RDONLY|O_NONBLOCK);

    if (fd < 0) {
        emcStatus->task.hardwarecheck = (0 == SIMULATE?-1:0);
        emcOperatorError(0, _("read /dev/sda error"));
        return -1;
    } else {
        if(!ioctl(fd, HDIO_GET_IDENTITY, &id)) {
            for(j=0; j<20; j++) {
                orghdid[j] = id.serial_no[j];
            }
        }
    }
    calculateCrc64(orghdid,hdidcrc,20);
    for(j=0; j<8; j++) {
        if(j > 8)
            break;
        if(hdid[j] != hdidcrc[j])
            break;
        else{}
            //printf("now hardid is ok\n");
    }

    for(int h=0;h<8;h++) {
        emcStatus->task.getcpuid[h] = hdid[h];
    }
    for(int h=0;h<8;h++) {
        emcStatus->motion.fpgaversion[h] = cpuidl[h];
    }


    for( i = 0;i<8;i++) {
        if(i > 8)
            break;
        if(emcStatus->motion.encrychipid[i] != chipid[i]) {
            break;
        }
        else {
            //printf("now encrychip is ok\n");
        }
    }
    if(i!=8){
//        emcOperatorError(0, _("the encrychip is error")); //暂时屏蔽 20170225
        emcStatus->task.hardwarecheck = (0 == SIMULATE?-1:0);
    }
    if(j != 8) {
//        emcOperatorError(0, _("the hdid is error"));  //暂时屏蔽 20170225
        emcStatus->task.hardwarecheck = (0 == SIMULATE?-1:0);
    }
    /*
    printf("now get the code 1 %x  %x %x %x\n",emcStatus->motion.encrychipid[0],chipid[0],hdid[0],id.serial_no[0]);
    printf("now get the code 2 %x  %x %x %x \n",emcStatus->motion.encrychipid[1],chipid[1],hdid[1],id.serial_no[1]);
    printf("now get the code 3 %x  %x %x %x\n",emcStatus->motion.encrychipid[2],chipid[2],hdid[2],id.serial_no[2]);
    printf("now get the code 4 %x  %x %x %x\n",emcStatus->motion.encrychipid[3],chipid[3],hdid[3],id.serial_no[3]);
    printf("now get the code 5 %x  %x %x %x\n",emcStatus->motion.encrychipid[4],chipid[4],hdid[4],id.serial_no[4]);
    printf("now get the code 6 %x  %x %x %x\n",emcStatus->motion.encrychipid[5],chipid[5],hdid[5],id.serial_no[5]);
    printf("now get the code 7 %x  %x %x %x\n",emcStatus->motion.encrychipid[6],chipid[6],hdid[6],id.serial_no[6]);
    printf("now get the code 8 %x  %x %x %x\n",emcStatus->motion.encrychipid[7],chipid[7],hdid[7],id.serial_no[7]);
*/
}

static int all_homed(void) {
    int checkmaxaxis = axes;
    for (int i = 0; i < checkmaxaxis; i++ ) {
        if (!emcStatus->motion.axis[i].homed)
            return 0;
    }
    return 1;
}

static void emctask_quit(int sig) {
    // set main's done flag
    done = 1;
    // restore signal handler
    printf("now I get the signal\n");
    signal(sig, emctask_quit);
}
static int emcReadtimeleft() {

    if(0 == emcStatus->motion.timetype) {

        emcStatus->task.timeleft = -1;
        printf("Now I get this tlemleft is %.3f\n",emcStatus->task.timeleft);
    } else if (1 == emcStatus->motion.timetype) {
        long int secleft = mydifftime(emcStatus->motion.Enddate,emcStatus->motion.Currentdate);
        emcStatus->task.timeleft = secleft;
        printf("Now I get this tlemleft in date is %.3f\n",emcStatus->task.timeleft);

    }
    return 0;
}



static int emcTimecount() {
    static double oldtime = etime();
    static double timeold2 = etime();
    static double timeold3 = etime();
    static int i = 0;

    unsigned char refbuf[300];
    int reflen;
    unsigned char funcno[1] = {0x08};

//    sendLKcmd(funcno,1,refbuf,&reflen);
    if(etime() - timeold2 < 1 && etime() > timeold2) {
        return 1;
    }
    timeold2 = etime();

    if(etime() - timeold3 > 8*60*60) {
       if(1 == emcStatus->motion.timetype && emcStatus->task.timeleft > 0 && emcStatus->task.mode == EMC_TASK_MODE_MANUAL) {
           printf("now before time:%d\n",etime());
            emcgetrtctime();
            writeFlushTime();
            printf("now after time:%d\n",etime());
            timeold3 = etime();
       }
    }

    if(emcStatus->motion.traj.linktoEth == 0) {
        if( SIMULATE ) return 1;
        emcStatus->task.autorepeat = false;
        if(emcStatus->task.state == EMC_TASK_STATE_ON &&
                emcStatus->task.mode == EMC_TASK_MODE_MANUAL) {
            emcOperatorError(0, _("the ethercat is not connected,please restart"));
            emcTaskSetState(EMC_TASK_STATE_ESTOP);
        }
    }

    if(emcStatus->motion.dateerror == -4) {
        if( SIMULATE ) return 1;
        emcStatus->task.autorepeat = false;
        if(emcStatus->task.state == EMC_TASK_STATE_ON &&
                emcStatus->task.mode == EMC_TASK_MODE_MANUAL) {
            emcOperatorError(0, _("The machine is locked.Please input the unlock code"));
            emcTaskSetState(EMC_TASK_STATE_ESTOP);
        }
        return 1;

    }

    static bool readonceflag = true;
    if(1 == emcStatus->motion.timetype && readonceflag) {
        long int secleft = mydifftime(emcStatus->motion.Enddate,emcStatus->motion.Currentdate);
        emcStatus->task.timeleft = secleft;
//        printf("---- Now I get the timeleft in date is %.3f\n",emcStatus->task.timeleft);
        readonceflag = false;
    }
//printf("---- Now I get the timeleft in date is %.3f  type:%d \n",emcStatus->task.timeleft,emcStatus->motion.timetype);
   if (1 == emcStatus->motion.timetype) {
            if(1 == emcStatus->motion.remaintimestatus) {
                emcStatus->task.autorepeat = false;
                if(emcStatus->task.state == EMC_TASK_STATE_ON &&
                        emcStatus->task.mode == EMC_TASK_MODE_MANUAL) {
                    if(emcStatus->task.timeleft > 86400)
                        emcOperatorError(0, _("now BIOS time is error,the machine will be locked"));
                    else
                        emcOperatorError(0, _("now the lefttime is over,the machine will be locked"));
                    emcTaskSetState(EMC_TASK_STATE_ESTOP);
                }
                return 1;
            } else {
                if(emcStatus->motion.dateerror != 0) {
                     emcStatus->task.autorepeat = false;
                     if(emcStatus->task.state == EMC_TASK_STATE_ON &&
                             emcStatus->task.mode == EMC_TASK_MODE_MANUAL) {
                             emcOperatorError(0, _("RTC Time checked error,error code :%d"),
                                              emcStatus->motion.dateerror);
                         emcTaskSetState(EMC_TASK_STATE_ESTOP);
                    }
                     return 1;
                }
            }
    }

   if(emcStatus->task.timeleft > 60) {
            //count
            if(fabs(etime() - oldtime) > 1) {
                if(fabs(etime() - oldtime) > 100)   //solve the time change
                    emcStatus->task.timeleft -= 1;
                else
                    emcStatus->task.timeleft -= fabs(etime() - oldtime);
                oldtime = etime();
            }

            if(emcStatus->task.timeleft < 60*60*24*3 && emcStatus->task.timeleft > 60*60) {
                static int warnflagy = 0;
                if(warnflagy == 0) {
                    emcOperatorDisplay(0, _("now the lefttime is less than %d day"),
                                       (int)(emcStatus->task.timeleft/(60*60*24))+1);
                    warnflagy  = 1;
                }
            }
            if(emcStatus->task.timeleft <= 60*30 && emcStatus->task.timeleft > 60) {
                static int warnflagz = 0;
                warnflagz++;
                if(warnflagz == 60) {
//                    emcOperatorDisplay(0, _("now the lefttime is less than %d minutes"),
//                                       (int)(emcStatus->task.timeleft/60));
                    warnflagz = 0;
                }
            }

            if(emcStatus->task.timeleft < 7200 && emcStatus->task.timeleft > 6000) {
                static int warnflag = 0;
                if(warnflag == 0) {
                    emcOperatorDisplay(0, _("now the lefttime is less than 2 hours"));
                    warnflag  = 1;
                }
            }

            if(emcStatus->task.timeleft < 60) {
                emcStatus->task.timeleft = 50;
            }
        } else if(emcStatus->task.timeleft < 0) {
            //unlock
        } else {
            //lock
            emcStatus->task.autorepeat = false;
            if(emcStatus->task.state == EMC_TASK_STATE_ON &&
                    emcStatus->task.mode == EMC_TASK_MODE_MANUAL) {
                emcOperatorError(0, _("now the hours is over,the machine will be locked"));
                emcTaskSetState(EMC_TASK_STATE_ESTOP);
            }
        }
    return 0;
}
#define SEC_PER_DAY        86400
#define SEC_PER_HOUR    3600
#define DAY_PER_4YEAR    1461
static const int __day_in_leapyear[13]        = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};
static const int __day_in_commonyear[13]    = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
static const int __day_in_4year[5]            = { 0, 365, 730, 1096, 1461};
static int secToRTCtime(long n_Sec, struct rtc_time *rtc)
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

static long kernel_mktime (struct rtc_time *tm)
{
  long res;
  int year;
  year = tm->tm_year - 70;	// 从70 年到现在经过的年数(2 位表示方式)，
  // 因此会有2000 年问题。
  /* magic offsets (y+1) needed to get leapyears right. */
  /* 为了获得正确的闰年数，这里需要这样一个魔幻偏值(y+1) */
  res = YEAR * year + DAY * ((year + 1) / 4);	// 这些年经过的秒数时间 + 每个闰年时多1 天
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

static long mydifftime(unsigned int *tm1,unsigned int *tm2) {
    long res;
    struct rtc_time time1,time2;
    time1.tm_year = tm1[0] - 1900;
    time1.tm_mon = tm1[1] - 1;
    time1.tm_mday = tm1[2];
    time1.tm_hour = tm1[3];
    time1.tm_min = tm1[4];
    time1.tm_sec = tm1[5];

    time2.tm_year = tm2[0] - 1900;
    time2.tm_mon = tm2[1] - 1;
    time2.tm_mday = tm2[2];
    time2.tm_hour = tm2[3];
    time2.tm_min = tm2[4];
    time2.tm_sec = tm2[5];

    res=kernel_mktime(&time1) - kernel_mktime(&time2) ;
    printf("now sec is %d %d %d %d %d %d\n",tm1[0],tm1[1],tm1[2],tm1[3],tm1[4],tm1[5]);
    printf("now sec is current %d %d %d %d %d %d\n",tm2[0],tm2[1],tm2[2],tm2[3],tm2[4],tm2[5]);
    printf("now left is %ld\n",res);
    return res;
}

//check the commulication
int emcCheckIoCommulication() {
    return 0;
    unsigned int getInfo = 0xff;
    getInfo = emcStatus->motion.fpgastatus;

    emcStatus->task.fpgacheck[0]  = (getInfo & 0x01) > 0?1:0;
    emcStatus->task.fpgacheck[1] = (getInfo & (0x01 << 1)) > 0?1:0;
    emcStatus->task.fpgacheck[2] = (getInfo & (0x01 << 2)) > 0?1:0;
    emcStatus->task.fpgacheck[3] = (getInfo & (0x01 << 3)) > 0?1:0;
    emcStatus->task.fpgacheck[4] = (getInfo & (0x01 << 4)) > 0?1:0;
    emcStatus->task.fpgacheck[5] = (getInfo & (0x01 << 5)) > 0?1:0;
    emcStatus->task.fpgacheck[6] = (getInfo & (0x01 << 6)) > 0?1:0;
    emcStatus->task.fpgacheck[7] = (getInfo & (0x01 << 7)) > 0?1:0;

    if(1 == emcStatus->task.fpgacheck[0]) {
        if(0 == emcStatus->task.fpgacheck[1]) {
            if(EMC_TASK_STATE_ON == emcStatus->task.state) {
                emcTaskSetState(EMC_TASK_STATE_OFF);
                emcOperatorError(0, _("LTK Reset failed,Please Solve it first"));
            }
        }

        if(0 == emcStatus->task.fpgacheck[3]) {
            if(EMC_TASK_STATE_ON == emcStatus->task.state &&
                    emcStatus->task.mode == EMC_TASK_MODE_AUTO) {
                emcTaskSetState(EMC_TASK_STATE_OFF);
                emcOperatorError(0, _("IO_AA Communication is error,Please Solve it first"));
            }
        }

        if(0 == emcStatus->task.fpgacheck[4] && 1 == checkexternIo) {
            if(EMC_TASK_STATE_ON == emcStatus->task.state &&
                    emcStatus->task.mode == EMC_TASK_MODE_AUTO) {
                emcTaskSetState(EMC_TASK_STATE_OFF);
                emcOperatorError(0, _("Extern IO_BB Communication is error,Please Solve it first"));
            }
        }
    } else {
        if(EMC_TASK_STATE_ON == emcStatus->task.state) {
            emcTaskSetState(EMC_TASK_STATE_OFF);
            emcOperatorError(0, _("Epp Communication is error,Please Solve it first"));
        }
        return -1;
    }

    return 0;
}

// implementation of EMC error logger
int emcOperatorError(int id, const char *fmt, ...) {
    EMC_OPERATOR_ERROR error_msg;
    va_list ap;

    // check channel for validity
    if (emcErrorBuffer == NULL)
        return -1;
    if (!emcErrorBuffer->valid())
        return -1;

    if (NULL == fmt) {
        return -1;
    }
    if (0 == *fmt) {
        return -1;
    }
    // prepend error code, leave off 0 ad-hoc code
    error_msg.error[0] = 0;
    if (0 != id) {
        snprintf(error_msg.error, sizeof(error_msg.error), "[%d] ", id);
    }
    // append error string
    va_start(ap, fmt);
    vsnprintf(&error_msg.error[strlen(error_msg.error)],
            sizeof(error_msg.error) - strlen(error_msg.error), fmt, ap);
    va_end(ap);

    // force a NULL at the end for safety
    error_msg.error[LINELEN - 1] = 0;

    // write it
    rcs_print("%s\n", error_msg.error);
    return emcErrorBuffer->write(error_msg);
}

int emcOperatorText(int id, const char *fmt, ...) {
    EMC_OPERATOR_TEXT text_msg;
    va_list ap;

    // check channel for validity
    if (emcErrorBuffer == NULL)
        return -1;
    if (!emcErrorBuffer->valid())
        return -1;

    // write args to NML message (ignore int text code)
    va_start(ap, fmt);
    vsnprintf(text_msg.text, sizeof(text_msg.text), fmt, ap);
    va_end(ap);

    // force a NULL at the end for safety
    text_msg.text[LINELEN - 1] = 0;

    // write it
    return emcErrorBuffer->write(text_msg);
}

int emcOperatorDisplay(int id, const char *fmt, ...) {
    EMC_OPERATOR_DISPLAY display_msg;
    va_list ap;

    // check channel for validity
    if (emcErrorBuffer == NULL)
        return -1;
    if (!emcErrorBuffer->valid())
        return -1;

    // write args to NML message (ignore int display code)
    va_start(ap, fmt);
    vsnprintf(display_msg.display, sizeof(display_msg.display), fmt, ap);
    va_end(ap);

    // force a NULL at the end for safety
    display_msg.display[LINELEN - 1] = 0;

    // write it
    return emcErrorBuffer->write(display_msg);
}

/*
 handling of EMC_SYSTEM_CMD
 */

/* convert string to arg/argv set */

static int argvize(const char *src, char *dst, char *argv[], int len) {
    char *bufptr;
    int argvix;
    char inquote;
    char looking;

    strncpy(dst, src, len);
    dst[len - 1] = 0;
    bufptr = dst;
    inquote = 0;
    argvix = 0;
    looking = 1;

    while (0 != *bufptr) {
        if (*bufptr == '"') {
            *bufptr = 0;
            if (inquote) {
                inquote = 0;
                looking = 1;
            } else {
                inquote = 1;
            }
        } else if (isspace(*bufptr) && !inquote) {
            looking = 1;
            *bufptr = 0;
        } else if (looking) {
            looking = 0;
            argv[argvix] = bufptr;
            argvix++;
        }
        bufptr++;
    }

    argv[argvix] = 0;		// null-terminate the argv list

    return argvix;
}

static pid_t emcSystemCmdPid = 0;

int emcSystemCmd(char *s) {
    char buffer[EMC_SYSTEM_CMD_LEN];
    char *argv[EMC_SYSTEM_CMD_LEN / 2 + 1];

    if (0 != emcSystemCmdPid) {
        // something's already running, and we can only handle one
        if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
            rcs_print("emcSystemCmd: abandoning process %d, running ``%s''\n",
                    emcSystemCmdPid, s);
        }
    }

    emcSystemCmdPid = fork();

    if (-1 == emcSystemCmdPid) {
        // we're still the parent, with no child created
        if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
            rcs_print("system command ``%s'' can't be executed\n", s);
        }
        return -1;
    }

    if (0 == emcSystemCmdPid) {
        // we're the child
        // convert string to argc/argv
        argvize(s, buffer, argv, EMC_SYSTEM_CMD_LEN);
        // drop any setuid privileges
        setuid (getuid());execvp
        (argv[0], argv);
        // if we get here, we didn't exec
        if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
            rcs_print("emcSystemCmd: can't execute ``%s''\n", s);
        }
        exit(-1);
    }
    // else we're the parent
    return 0;
}

static int checkHardlimit() {
    int rev = 0;
    if (emcGetxyzFlag()) {
        double x, y, z,a,b,c;
        int g53_flag = 0;
        //emcCheckLimits(&x, &y, &z,&g53_flag);
        SET_COOR_Z(&x,&y,&z,&g53_flag);
        if (x - emcStatus->motion.axis[0].maxPositionLimit > 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超X轴正限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds +X limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }
        if (y - emcStatus->motion.axis[1].maxPositionLimit > 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超Y轴正限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds +Y limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }

//        if(((g53_flag == 530) || (g53_flag == 280) || (g53_flag == 300))
//                && 0 > GET_Z_DIFF() && (z-GET_Z_DIFF())>0 ) {
//            if(z <= 0)
//                z = GET_Z_DIFF();
//            else if(z > 0)
//                z = z-fabs(GET_Z_DIFF());
//        } else if (!((g53_flag == 530) || (g53_flag == 280) || (g53_flag == 300))) {
//            z += GET_Z_OFFSET();
//        }


        if (z - emcStatus->motion.axis[2].maxPositionLimit > 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超Z轴正限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
                emcOperatorError(0, _("line %d: exceeds +Z limit"),emcTaskPlanLine());
            }
            rev = -1;
        }
        if (a - emcStatus->motion.axis[3].maxPositionLimit > 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超A轴正限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds +A limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }
        if (b - emcStatus->motion.axis[4].maxPositionLimit > 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超B轴正限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds +B limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }
        if (c - emcStatus->motion.axis[5].maxPositionLimit > 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超C轴正限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds +C limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }
        if (x - emcStatus->motion.axis[0].minPositionLimit < 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超X轴负限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds -X limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }
        if (y - emcStatus->motion.axis[1].minPositionLimit < 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超Y轴负限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds -Y limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }
        if (z - emcStatus->motion.axis[2].minPositionLimit < 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超Z轴负限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds -Z limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }
        if (a - emcStatus->motion.axis[3].minPositionLimit < 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超A轴负限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds -A limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }
        if (b - emcStatus->motion.axis[4].minPositionLimit < 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超B轴负限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds -B limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }
        if (c - emcStatus->motion.axis[5].minPositionLimit < 0) {
            if(emcTaskSaveLine(0) >= 0) {
                emcOperatorError(0, "主程序行: %d 子程序行:%d : 超C轴负限位",emcTaskSaveLine(0),emcTaskPlanLine()-emcTaskSaveLine(0));
            }  else {
            emcOperatorError(0, _("line %d: exceeds -C limit"),
                    emcTaskPlanLine());
            }
            rev = -1;
        }

        if(fabs(emcStatus->task.parameters[4227]) > 0.0001) {
            if(z < emcStatus->task.parameters[4227]) {
                if(x - emcStatus->motion.axis[0].secNegLimit < 0) {
                    emcOperatorError(0, _("line %d: exceeds sec -X limit %.3f  %.3f"), emcTaskPlanLine(),x,emcStatus->motion.axis[0].secNegLimit);
                    rev = -1;
                }
                if(x - emcStatus->motion.axis[0].secPosLimit > 0) {
                    emcOperatorError(0, _("line %d: exceeds sec +X limit %.3f  %.3f"), emcTaskPlanLine(),x,emcStatus->motion.axis[0].secPosLimit);
                    rev = -1;
                }
                if(y - emcStatus->motion.axis[1].secNegLimit < 0) {
                    emcOperatorError(0, _("line %d: exceeds sec -Y limit %.3f  %.3f"), emcTaskPlanLine(),y,emcStatus->motion.axis[1].secNegLimit);
                    rev = -1;
                }
                if(y - emcStatus->motion.axis[1].secPosLimit > 0) {
                    emcOperatorError(0, _("line %d: exceeds sec +Y limit %.3f  %.3f"), emcTaskPlanLine(),y,emcStatus->motion.axis[1].secPosLimit);
                    rev = -1;
                }
                /************************************************************************/
                if(fabs(emcStatus->motion.axis[0].seclimitOffset) > 0.00001 && fabs(emcStatus->motion.axis[1].seclimitOffset) > 0.00001) {
                    if((x > emcStatus->motion.axis[0].secNegLimit+emcStatus->motion.axis[0].seclimitOffset) &&
                           (x < emcStatus->motion.axis[0].secPosLimit-emcStatus->motion.axis[0].seclimitOffset) &&
                            (y > emcStatus->motion.axis[1].secNegLimit+emcStatus->motion.axis[1].seclimitOffset) &&
                            (y < emcStatus->motion.axis[1].secPosLimit-emcStatus->motion.axis[1].seclimitOffset)) {
                        emcOperatorError(0, _("line %d: exceeds third  limit %.3f  %.3f"), emcTaskPlanLine(),x,y);
                        rev = -1;
                    }
                }
            }
        }

    }
    if(emcStatus->task.singlecompverify) {
        emcStatus->task.singlecompverify = false;
        emcOperatorError(0, _("near the line %d single compensation error"),
                emcTaskPlanLine());
        rev = -2;
    }



    return rev;
}

void readahead_verify(void) {
    int readRetval;
    int execRetval;
    static int errNum = 0,count = 0;
interpret_again:
    readRetval = emcTaskPlanRead();
    emcStatus->task.readLine = emcTaskPlanLine();
    interp_list.set_line_number(emcStatus->task.readLine);
    emcTaskPlanCommand((char *) &emcStatus->task.command);
    execRetval = emcTaskPlanExecute(0);
    if (emcGetM99Flag()) {
        execRetval = INTERP_EXIT;
    }
    count++;
    if (readRetval == INTERP_ENDFILE || readRetval == INTERP_EXIT || verifycontrol) {
        int was_open = taskplanopen;
        if (was_open)
            emcTaskPlanClose();
        verifycontrol = false;
        emcAbortcmd();
        emcSetOnlyReading(0);
        interp_list.clear();
        emcStatus->task.readLine = 0;
        interp_list.set_line_number(0);
        emcStatus->task.currentLine = 0;
        errNum = 0;
        emcTaskCommand = 0;
        emcInitM99Status();
        interpStatusInit(); // add by hong
        emcStatus->task.interpState = EMC_TASK_INTERP_IDLE;
        emcTaskSetMode (EMC_TASK_MODE_MANUAL);

    } else if( emcGetOnlyReading()  != 2){

        if (-1 == checkHardlimit()) {
            errNum++;
        }
        if (3 < errNum) {
            errNum = 0;
            emcOperatorError(0,
                    _("More overLimit error int the program,please check the coordinate system"));
            int was_open = taskplanopen;
            if (was_open)
                emcTaskPlanClose();
            verifycontrol = false;
            emcAbortcmd();
            emcSetOnlyReading(0);
            interp_list.clear();
            emcStatus->task.readLine = 0;
            interp_list.set_line_number(0);
            emcStatus->task.currentLine = 0;
            emcTaskCommand = 0;
            emcInitM99Status();
            interpStatusInit(); // add by hong
            emcStatus->task.interpState = EMC_TASK_INTERP_IDLE;
            emcTaskSetMode (EMC_TASK_MODE_MANUAL);

        }
    }


    if (emcStatus->task.interpState == EMC_TASK_INTERP_VERIFY && count < 50) {
        goto interpret_again;
    } else {
        count = 0;
    }

}

void readahead_reading(void) {
    int readRetval;
    int execRetval;

    if (interp_list.len() <= EMC_TASK_INTERP_MAX_LEN) {
        int count = 0;

interpret_again:
        if (emcTaskPlanIsWait()) {
            // delay reading of next line until all is done
            static int oldlen = -1,oldstate = -1,oldwait = -1;
            if(oldlen != interp_list.len() || emcStatus->task.execState != oldstate || emcTaskCommand == 0 || oldwait != emcStatus->task.waitflag) {
                //printf("now I get the len %d %d %d\n",interp_list.len(),emcStatus->task.execState,emcStatus->task.waitflag);
                oldlen = interp_list.len();
                oldstate = emcStatus->task.execState;
                oldwait = emcStatus->task.waitflag;
            }
            if (interp_list.len() == 0 && emcTaskCommand == 0
                    && emcStatus->task.execState == EMC_TASK_EXEC_DONE
                    && emcStatus->motion.changezend == 1) {
                emcTaskPlanClearWait();
            }
        } else {
            readRetval = emcTaskPlanRead();
            if (readRetval > INTERP_MIN_ERROR || readRetval == INTERP_ENDFILE
                    || readRetval == INTERP_EXIT
                    || readRetval == INTERP_EXECUTE_FINISH) {
                if (readRetval > INTERP_MIN_ERROR) {
                    emcStatus->task.interpState = EMC_TASK_INTERP_IDLE;
                    emcTaskAbort();
                    interp_list.clear();
                    emcTaskPlanReset();
                } else {
                    if(execstartngcflag > 0) {
                        execstartngcflag = 0;
                        if (taskplanopen)
                            emcTaskPlanClose();
                        emcTaskPlanOpen(emcStatus->task.file);
                     } else {
                        if (readRetval == INTERP_ENDFILE) {
                            emcStatus->task.motionLine = 0;
                            emcStatus->task.readLine = 0;
                        }
                        emcStatus->task.interpState = EMC_TASK_INTERP_WAITING;
                    }
                }
            } else {
                // got a good line
                // record the line number and command
                emcStatus->task.readLine = emcTaskPlanLine();
                interp_list.set_line_number(emcStatus->task.readLine);
                emcTaskPlanCommand((char *) &emcStatus->task.command);
                // and execute it

                execRetval = emcTaskPlanExecute(0);

                if(emcStatus->task.runendline > 0 &&
                        emcStatus->task.runendline < emcStatus->task.readLine) {
                    execRetval = INTERP_EXIT;
                    emcStatus->task.runendline = 0;
                }
                if (0 != checkHardlimit()) {
                    emcStatus->task.interpState = EMC_TASK_INTERP_IDLE;
                    emcTaskAbort();
                    interp_list.clear();
                    emcTaskPlanReset();
                    emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
                    execRetval = INTERP_ERROR;
                    execerrorflag = 1;
                }
                if (emcGetM99Flag()) {
                    execRetval = INTERP_EXIT;
                }
                if (emcGetChangezFlag()) {
                    execRetval = INTERP_EXIT;
                }
                if (execRetval > INTERP_MIN_ERROR) {
                    emcStatus->task.interpState = EMC_TASK_INTERP_IDLE;
                    emcTaskAbort();
                    emcTaskPlanReset();

                    interp_list.clear();
                } else if (execRetval == INTERP_EXIT
                        || execRetval == INTERP_ENDFILE) {
                    emcStatus->task.interpState = EMC_TASK_INTERP_WAITING;
                    emcStatus->task.motionLine = 0;
                    emcStatus->task.readLine = 0;
                } else if (execRetval == INTERP_EXECUTE_FINISH) {
                    // INTERP_EXECUTE_FINISH signifies
                    // that no more reading should be done until everything  outstanding is completed
                    emcTaskPlanSetWait();

                    // while the runfromline is not zero ,then we cannot sync the actual coordinate to interp,

                     emcTaskQueueCommand(&taskPlanNullCmd);
                } else {

                    // executed a good line
                }
                if (count++ < EMC_TASK_INTERP_MAX_LEN
                        && emcStatus->task.interpState
                                == EMC_TASK_INTERP_READING
                        && interp_list.len()
                                <= EMC_TASK_INTERP_MAX_LEN * 2 / 3) {
                    goto interpret_again;
                }

            }	// else read was OK, so execute

        }		// else not emcTaskPlanIsWait

    }		// if interp len is less than max
}

static void mdi_execute_abort(void) {
    // XXX: Reset needed?
    if (mdi_execute_wait || mdi_execute_next)
        emcTaskPlanReset();
    mdi_execute_level = -1;
    mdi_execute_wait = 0;
    mdi_execute_next = 0;

    mdi_execute_queue.clear();
}

static void mdi_execute_hook(void) {
    if (mdi_execute_wait && emcTaskPlanIsWait()) {
        // delay reading of next line until all is done
        if (interp_list.len() == 0 && emcTaskCommand == 0
                && emcStatus->task.execState == EMC_TASK_EXEC_DONE) {
            emcTaskPlanClearWait();
            mdi_execute_wait = 0;
            mdi_execute_hook();

        }
        return;
    }

    if ((mdi_execute_level < 0) && (mdi_execute_wait == 0)
            && (mdi_execute_queue.len() > 0) && (interp_list.len() == 0)
            && (emcTaskCommand == NULL)) {

        interp_list.append(mdi_execute_queue.get());
        return;
    }

    if (!mdi_execute_next)
        return;

    if (interp_list.len() > EMC_TASK_INTERP_MAX_LEN)
        return;

    mdi_execute_next = 0;

    EMC_TASK_PLAN_EXECUTE msg;
    msg.command[0] = (char) 0xff;

    interp_list.append(msg);
}

void readahead_waiting(void) {
    // now handle call logic
    // check for subsystems done
   // printf("1=%d,2=%d,3=%d,4=%d,5=%d\n",interp_list.len(),emcTaskCommand,(int)emcStatus->motion.traj.queue,emcStatus->io.status,waitforzchangeend);
    if (0 == waitforzchangeend && interp_list.len() == 0 && emcTaskCommand == 0
            && emcStatus->motion.traj.queue == 0
            && emcStatus->io.status == RCS_DONE
            && flag_run_inwaiting
            && zchangeflag  == EMC_NO_CHANGE) { // finished

        int was_open = taskplanopen;
        if (was_open) {
            emcTaskPlanClose();
            if (EMC_DEBUG & EMC_DEBUG_INTERP && was_open) {
                rcs_print("emcTaskPlanClose() called at %s:%d\n",
                __FILE__, __LINE__);
            }
            // then resynch interpreter
            emcTaskQueueCommand(&taskPlanSynchCmd);
        } else {
            printf("in wait over\n");

            //execute finish.ngc
            if (0 == emcGetOnlyReading() && !emcRunsub()
                    && !emcStatus->task.autorepeat) {
                for(int i=0;i<finishsize;i++) {
                    emcTaskPlanExecute(finishcmd[i]);
                }
            }
            if (!emcRunsub() && !emcStatus->task.autorepeat) {
//                emcMotionSetDout(5,0,0,1);
                emcStatus->task.work_piece++;
                if (emcStatus->task.work_piece > 10000)
                    emcStatus->task.work_piece = 0;
            }

            //Autorepeat run,and will be control by the parameter 4050
            if (emcStatus->task.autorepeat && !emcRunsub()) {
                    if(emcStatus->task.parameters[4050] != 0) {
                        if(emcStatus->task.parameters[4050]-1 > 0 || emcStatus->task.parameters[4050] < 0)
                            emcTaskPlanExecute("m99");      //will call the finish in m99
                        else {
                            emcStatus->task.autorepeat = false;
                            emcStatus->task.work_piece++;

                            emcMotionSetDout(5,0,0,1);
                        }
                        if(emcStatus->task.parameters[4050] > 0)
                            emcSetparameter(4050,emcStatus->task.parameters[4050]-1,1);
                    }//else emcTaskPlanExecute("m99");
            }

            emcStatus->task.interpState = EMC_TASK_INTERP_IDLE;
          }
        emcStatus->task.readLine = 0;
        interp_list.set_line_number(0);
    } else {
        // still executing
    }
}

/*
 emcTaskPlan()

 Planner for NC code or manual mode operations
 */
static int emcTaskPlan(void) {
    NMLTYPE type;
    int retval = 0;

    // check for new command
    if (emcCommand->serial_number != emcStatus->echo_serial_number) {
        // flag it here locally as a new command
        type = emcCommand->type;
    } else {
        // no new command-- reset local flag
        type = 0;
    }
    if (0 > emcStatus->task.hardwarecheck && type != 0) {
        //emcStatus->task.hardwarecheck = 0;
        return retval;
    }

    // handle any new command
    switch (emcStatus->task.state) {
    case EMC_TASK_STATE_OFF:
    case EMC_TASK_STATE_ESTOP:
    case EMC_TASK_STATE_ESTOP_RESET:      //解除报警

        // now switch on the mode
        switch (emcStatus->task.mode) {
        case EMC_TASK_MODE_MANUAL:
        case EMC_TASK_MODE_AUTO:
        case EMC_TASK_MODE_MDI:

            // now switch on the command
            switch (type) {
            case 0:
            case EMC_NULL_TYPE:
                // no command
                break;

                // immediate commands
            case EMC_NULLEVENT_TYPE:    //hong -use for append a null cmd
            case EMC_AXIS_SET_BACKLASH_TYPE:
            case EMC_AXIS_SET_HOMING_PARAMS_TYPE:
            case EMC_AXIS_DISABLE_TYPE:
            case EMC_AXIS_ENABLE_TYPE:
            case EMC_AXIS_SET_FERROR_TYPE:
            case EMC_AXIS_SET_MIN_FERROR_TYPE:
            case EMC_AXIS_ABORT_TYPE:
            case EMC_AXIS_LOAD_COMP_TYPE:
            case EMC_AXIS_UNHOME_TYPE:
            case EMC_TRAJ_SET_SCALE_TYPE:
            case EMC_TRAJ_SET_MAX_VELOCITY_TYPE:
            case EMC_TRAJ_SET_SPINDLE_SCALE_TYPE:
            case EMC_TRAJ_SET_FO_ENABLE_TYPE:
            case EMC_TRAJ_SET_FH_ENABLE_TYPE:
            case EMC_TRAJ_SET_SO_ENABLE_TYPE:
            case EMC_TRAJ_SET_G0_ENABLE_TYPE:
            case EMC_TRAJ_SET_DEFAULT_FEED_TYPE:
            case EMC_TRAJ_SET_VELOCITY_TYPE:
            case EMC_TRAJ_SET_ACCELERATION_TYPE:
            case EMC_TASK_INIT_TYPE:
            case EMC_TASK_SET_MODE_TYPE:
            case EMC_TASK_SET_STATE_TYPE:
            case EMC_TASK_PLAN_INIT_TYPE:
            case EMC_TASK_PLAN_OPEN_TYPE:
            case EMC_TASK_PLAN_SET_OPTIONAL_STOP_TYPE:
            case EMC_TASK_PLAN_SET_BLOCK_DELETE_TYPE:
            case EMC_TASK_ABORT_TYPE:
            case EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE:
            case EMC_TRAJ_PROBE_TYPE:
            case EMC_AUX_INPUT_WAIT_TYPE:
            case EMC_MOTION_SET_DOUT_TYPE:
            case EMC_MOTION_SET_EXTERN_OUT_TYPE:
            case EMC_MOTION_ADAPTIVE_TYPE:
            case EMC_MOTION_SET_AOUT_TYPE:
            case EMC_TRAJ_RIGID_TAP_TYPE:
            case EMC_TRAJ_SET_TELEOP_ENABLE_TYPE:
            case EMC_SET_DEBUG_TYPE:
            case EMC_RESET_CMD_TYPE:  // HONG 2014-05-07
            case EMC_LUBE_OFF_TYPE:
            case EMC_TASK_FLUSH_PAR_TYPE:
            case EMC_TASK_AUTOREPEAT_PAR_TYPE:  //hong 2014-06-24
            case EMC_TASK_SET_LEFTTIME_TYPE:
            case EMC_TASK_SET_STARTLINE_TYPE:
            case EMC_TASK_SET_SIGNAL_TYPE:
            case EMC_TASK_SET_PARAMETER_TYPE:
            case EMC_SPINDLE_OFF_TYPE:
            case EMC_TRAJ_MULTI_FUNCTION_TYPE:
            case EMC_TASK_PLAN_EXECUTE_TYPE:
                retval = emcTaskIssueCommand(emcCommand);
                break;

            case EMC_SETLIGHT_CMD_TYPE:  //急停模式下，报警了也要点亮红灯ghf-2014/3/17
                retval = emcTaskIssueCommand(emcCommand);
                break;

            case EMC_ENCRYPT_CMD_TYPE:  //界面一上电就往下发,motion中做比较，ghf-2014/3/28
                retval = emcTaskIssueCommand(emcCommand);
                break;

                // one case where we need to be in manual mode
            case EMC_AXIS_OVERRIDE_LIMITS_TYPE:
                retval = 0;
                if (emcStatus->task.mode == EMC_TASK_MODE_MANUAL) {
                    retval = emcTaskIssueCommand(emcCommand);
                }
                break;

            case EMC_TOOL_LOAD_TOOL_TABLE_TYPE:
            case EMC_TOOL_SET_OFFSET_TYPE:
                // send to IO
                emcTaskQueueCommand(emcCommand);
                // signify no more reading
                emcTaskPlanSetWait();
                // then resynch interpreter
                emcTaskQueueCommand(&taskPlanSynchCmd);
                break;

            case EMC_TOOL_SET_NUMBER_TYPE:
                // send to IO
                emcTaskQueueCommand(emcCommand);
                // then resynch interpreter
                emcTaskQueueCommand(&taskPlanSynchCmd);
                break;

            default:
                emcOperatorError(0,
                        _("command (%s) cannot be executed until the machine is out of E-stop and turned on"),
                        emc_symbol_lookup(type));
                retval = -1;
                break;

            }			// switch (type)

        default:
            // invalid mode
            break;

        }			// switch (mode)

        break;			// case EMC_TASK_STATE_OFF,ESTOP,ESTOP_RESET

    case EMC_TASK_STATE_ON:

        /* we can do everything (almost) when the machine is on, so let's
         switch on the execution mode */
        switch (emcStatus->task.mode) {
        case EMC_TASK_MODE_MANUAL:	// ON, MANUAL
            if(emcStatus->task.interpState == EMC_TASK_INTERP_VERIFY) {
                switch (type) {
                case 0:
                case EMC_NULL_TYPE:
                    break;
                case EMC_TASK_ABORT_TYPE:
                    printf("now I get in stop the verify control\n");
                    verifycontrol = true;
                    break;
                default:
                    break;
                }
                readahead_verify();
                break;
            }
            switch (type) {
            case 0:
            case EMC_NULL_TYPE:
                // no command
                break;

                // immediate commands
            case EMC_NULLEVENT_TYPE:    //hong -use for append a null cmd
            case EMC_AXIS_DISABLE_TYPE:
            case EMC_AXIS_ENABLE_TYPE:
            case EMC_AXIS_SET_BACKLASH_TYPE:
            case EMC_AXIS_SET_HOMING_PARAMS_TYPE:
            case EMC_AXIS_SET_FERROR_TYPE:
            case EMC_AXIS_SET_MIN_FERROR_TYPE:
            case EMC_AXIS_SET_MAX_POSITION_LIMIT_TYPE:
            case EMC_AXIS_SET_MIN_POSITION_LIMIT_TYPE:
            case EMC_AXIS_ABORT_TYPE:
            case EMC_AXIS_HALT_TYPE:
            case EMC_AXIS_HOME_TYPE:
            case EMC_AXIS_UNHOME_TYPE:
            case EMC_AXIS_JOG_TYPE:
            case EMC_AXIS_INCR_JOG_TYPE:
            case EMC_AXIS_ABS_JOG_TYPE:
            case EMC_AXIS_OVERRIDE_LIMITS_TYPE:
            case EMC_TRAJ_PAUSE_TYPE:
            case EMC_TRAJ_RESUME_TYPE:
            case EMC_TRAJ_ABORT_TYPE:
            case EMC_TRAJ_SET_SCALE_TYPE:
            case EMC_TRAJ_SET_MAX_VELOCITY_TYPE:
            case EMC_TRAJ_SET_SPINDLE_SCALE_TYPE:
            case EMC_TRAJ_SET_FO_ENABLE_TYPE:
            case EMC_TRAJ_SET_FH_ENABLE_TYPE:
            case EMC_TRAJ_SET_SO_ENABLE_TYPE:
            case EMC_TRAJ_SET_G0_ENABLE_TYPE:
            case EMC_TRAJ_SET_DEFAULT_FEED_TYPE:
            case EMC_SPINDLE_SPEED_TYPE:
            case EMC_SPINDLE_ON_TYPE:
            case EMC_SPINDLE_OFF_TYPE:
            case EMC_SPINDLE_BRAKE_RELEASE_TYPE:
            case EMC_SPINDLE_BRAKE_ENGAGE_TYPE:
            case EMC_SPINDLE_INCREASE_TYPE:
            case EMC_SPINDLE_DECREASE_TYPE:
            case EMC_SPINDLE_CONSTANT_TYPE:
            case EMC_COOLANT_MIST_ON_TYPE:
            case EMC_COOLANT_MIST_OFF_TYPE:
            case EMC_COOLANT_FLOOD_ON_TYPE:
            case EMC_COOLANT_FLOOD_OFF_TYPE:
            case EMC_LUBE_ON_TYPE:
            case EMC_LUBE_OFF_TYPE:
            case EMC_TASK_SET_MODE_TYPE:
            case EMC_TASK_SET_STATE_TYPE:
            case EMC_TASK_ABORT_TYPE:
            case EMC_TASK_PLAN_PAUSE_TYPE:
            case EMC_TASK_PLAN_RESUME_TYPE:
            case EMC_TASK_PLAN_INIT_TYPE:
            case EMC_TASK_PLAN_SYNCH_TYPE:
            case EMC_TASK_PLAN_SET_OPTIONAL_STOP_TYPE:
            case EMC_TASK_PLAN_SET_BLOCK_DELETE_TYPE:
            case EMC_TASK_PLAN_OPTIONAL_STOP_TYPE:
            case EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE:
            case EMC_TRAJ_PROBE_TYPE:
            case EMC_AUX_INPUT_WAIT_TYPE:
            case EMC_MOTION_SET_DOUT_TYPE:
            case EMC_MOTION_SET_AOUT_TYPE:
            case EMC_MOTION_ADAPTIVE_TYPE:
            case EMC_TRAJ_RIGID_TAP_TYPE:
            case EMC_TRAJ_SET_TELEOP_ENABLE_TYPE:
            case EMC_TRAJ_SET_TELEOP_VECTOR_TYPE:
            case EMC_SET_DEBUG_TYPE:
            case EMC_DOUBLEZ_CMD_TYPE:
            case EMC_PRO_VERIFY_TYPE:       //hong-2014/04/14
            case EMC_RESET_CMD_TYPE:  // HONG 2014-05-07
            case EMC_TASK_FLUSH_PAR_TYPE:
            case EMC_TASK_AUTOREPEAT_PAR_TYPE:  //hong 2014-06-24
            case EMC_TASK_SET_LEFTTIME_TYPE:
            case EMC_TASK_SET_STARTLINE_TYPE:
            case EMC_MOTION_SET_EXTERN_OUT_TYPE:
            case EMC_TASK_SET_SIGNAL_TYPE:
            case EMC_TASK_SET_PARAMETER_TYPE:
            case EMC_TRAJ_MULTI_FUNCTION_TYPE:  //hong 2015-10-23
            case EMC_TASK_PLAN_OPEN_TYPE:
            case EMC_TASK_PLAN_NULL_TYPE:
                retval = emcTaskIssueCommand(emcCommand);
                break;

            case EMC_SETGMODE_CMD_TYPE:
                retval = emcTaskIssueCommand(emcCommand);
                break;

                //ghf-2014/3/17,这个在手动自动模式下都要能进行
            case EMC_SETLIGHT_CMD_TYPE:
                retval = emcTaskIssueCommand(emcCommand);
                break;

            case EMC_JOGWHEELS_GUIDE_CMD_TYPE:  //ghf-2014/5/13

                retval = emcTaskIssueCommand(emcCommand);
                break;

                // queued commands

            case EMC_TASK_PLAN_EXECUTE_TYPE:
                // resynch the interpreter, since we may have moved
                // externally
                //emcTaskIssueCommand(&taskPlanSynchCmd);
                // and now call for interpreter execute
                retval = emcTaskIssueCommand(emcCommand);
                break;

            case EMC_TOOL_LOAD_TOOL_TABLE_TYPE:
            case EMC_TOOL_SET_OFFSET_TYPE:
                // send to IO
                emcTaskQueueCommand(emcCommand);
                // signify no more reading
                emcTaskPlanSetWait();
                // then resynch interpreter
                emcTaskQueueCommand(&taskPlanSynchCmd);
                break;

            case EMC_TOOL_SET_NUMBER_TYPE:
                // send to IO
                emcTaskQueueCommand(emcCommand);
                // then resynch interpreter
                emcTaskQueueCommand(&taskPlanSynchCmd);
                break;

                // otherwise we can't handle it

            case EMC_FILTERTIME_CMD_TYPE:        //ghf-2014/5/6
                retval = emcTaskIssueCommand(emcCommand);
                break;

            default:
                //emcOperatorError(0, _("can't do that (%s) in manual mode"),
                //        emc_symbol_lookup(type));
                retval = -1;
                break;

            }			// switch (type) in ON, MANUAL

            break;		// case EMC_TASK_MODE_MANUAL

        case EMC_TASK_MODE_AUTO:	// ON, AUTO
            switch (emcStatus->task.interpState) {
            case EMC_TASK_INTERP_IDLE:	// ON, AUTO, IDLE
                switch (type) {
                case 0:
                case EMC_NULL_TYPE:
                    // no command
                    break;

                    // immediate commands

                case EMC_NULLEVENT_TYPE:    //hong -use for append a null cmd
                case EMC_AXIS_SET_BACKLASH_TYPE:
                case EMC_AXIS_SET_HOMING_PARAMS_TYPE:
                case EMC_AXIS_SET_FERROR_TYPE:
                case EMC_AXIS_SET_MIN_FERROR_TYPE:
                case EMC_AXIS_UNHOME_TYPE:
                case EMC_TRAJ_PAUSE_TYPE:
                case EMC_TRAJ_RESUME_TYPE:
                case EMC_TRAJ_ABORT_TYPE:
                case EMC_TRAJ_SET_SCALE_TYPE:
                case EMC_TRAJ_SET_MAX_VELOCITY_TYPE:
                case EMC_TRAJ_SET_SPINDLE_SCALE_TYPE:
                case EMC_TRAJ_SET_FO_ENABLE_TYPE:
                case EMC_TRAJ_SET_FH_ENABLE_TYPE:
                case EMC_TRAJ_SET_SO_ENABLE_TYPE:
                case EMC_TRAJ_SET_G0_ENABLE_TYPE:
                case EMC_TRAJ_SET_DEFAULT_FEED_TYPE:
                case EMC_SPINDLE_SPEED_TYPE:
                case EMC_SPINDLE_ON_TYPE:
                case EMC_SPINDLE_OFF_TYPE:
                case EMC_SPINDLE_BRAKE_RELEASE_TYPE:
                case EMC_SPINDLE_BRAKE_ENGAGE_TYPE:
                case EMC_SPINDLE_INCREASE_TYPE:
                case EMC_SPINDLE_DECREASE_TYPE:
                case EMC_SPINDLE_CONSTANT_TYPE:
                case EMC_COOLANT_MIST_ON_TYPE:
                case EMC_COOLANT_MIST_OFF_TYPE:
                case EMC_COOLANT_FLOOD_ON_TYPE:
                case EMC_COOLANT_FLOOD_OFF_TYPE:
                case EMC_LUBE_ON_TYPE:
                case EMC_LUBE_OFF_TYPE:
                case EMC_TASK_SET_MODE_TYPE:
                case EMC_TASK_SET_STATE_TYPE:
                case EMC_TASK_ABORT_TYPE:
                case EMC_TASK_PLAN_INIT_TYPE:
                case EMC_TASK_PLAN_OPEN_TYPE:
                case EMC_TASK_PLAN_RUN_TYPE:
                case EMC_TASK_PLAN_EXECUTE_TYPE:
                case EMC_TASK_PLAN_PAUSE_TYPE:
                case EMC_TASK_PLAN_RESUME_TYPE:
                case EMC_TASK_PLAN_SET_OPTIONAL_STOP_TYPE:
                case EMC_TASK_PLAN_SET_BLOCK_DELETE_TYPE:
                case EMC_TASK_PLAN_OPTIONAL_STOP_TYPE:
                case EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE:
                case EMC_TRAJ_PROBE_TYPE:
                case EMC_AUX_INPUT_WAIT_TYPE:
                case EMC_TRAJ_RIGID_TAP_TYPE:
                case EMC_SET_DEBUG_TYPE:
                case EMC_PRO_VERIFY_TYPE: //hong-2014/04/14
                case EMC_RESET_CMD_TYPE:  // HONG 2014-05-07
                case EMC_JOGWHEELS_GUIDE_CMD_TYPE:
                case EMC_TASK_EXECUTE_BLOCK_TYPE: //hong-2014/05/24
                case EMC_TASK_FLUSH_PAR_TYPE:
                case EMC_TASK_AUTOREPEAT_PAR_TYPE:  //hong 2014-06-24
                case EMC_TOOL_SET_NUMBER_TYPE: //hong 2014-06-30
                case EMC_TASK_SET_LEFTTIME_TYPE:
                case EMC_TASK_SET_STARTLINE_TYPE:
                case EMC_MOTION_SET_EXTERN_OUT_TYPE:
                case EMC_TASK_SET_SIGNAL_TYPE:
                case EMC_TASK_SET_PARAMETER_TYPE:
                case EMC_MOTION_SET_DOUT_TYPE:
                case EMC_TRAJ_MULTI_FUNCTION_TYPE:
                    retval = emcTaskIssueCommand(emcCommand);
                    break;

                    //ghf-2014/3/17,这个在手动自动模式下都要能进行
                case EMC_SETLIGHT_CMD_TYPE:
                    retval = emcTaskIssueCommand(emcCommand);
                    break;

                case EMC_TASK_PLAN_STEP_TYPE: //on,auto,执行这个ghf-2014/4/26
                    // handles case where first action is to step the program
                    taskPlanRunCmd.line = 1;	// run from start
                    /*! \todo FIXME-- can have GUI set this; send a run instead of a
                     step */
                    retval = emcTaskIssueCommand(&taskPlanRunCmd);
                    if (retval != 0)
                        break;

                    emcTrajPause();

                    if (emcStatus->task.interpState != EMC_TASK_INTERP_PAUSED) {
                        interpResumeState = emcStatus->task.interpState;
                    }
                    emcStatus->task.interpState = EMC_TASK_INTERP_PAUSED;
                    emcStatus->task.task_paused = 1;
                    retval = 0;
                    break;

                case EMC_TOOL_LOAD_TOOL_TABLE_TYPE:
                case EMC_TOOL_SET_OFFSET_TYPE:
                    // send to IO
                    emcTaskQueueCommand(emcCommand);
                    // signify no more reading
                    emcTaskPlanSetWait();
                    // then resynch interpreter
                    emcTaskQueueCommand(&taskPlanSynchCmd);
                    break;

                    // otherwise we can't handle it
                default:
                    //emcOperatorError(0,
                    //        _("can't do that (%s) in auto mode with the interpreter idle"),
                    //        emc_symbol_lookup(type));
                    retval = -1;
                    break;

                }		// switch (type) in ON, AUTO, IDLE

                break;		// EMC_TASK_INTERP_IDLE

            case EMC_TASK_INTERP_READING:	// ON, AUTO, READING

                switch (type) {
                case 0:
                case EMC_NULL_TYPE:
                    // no command
                    break;
                case EMC_TOOL_LOAD_TOOL_TABLE_TYPE:
                case EMC_TOOL_SET_OFFSET_TYPE:
                    // send to IO
                    emcTaskQueueCommand(emcCommand);
                    // signify no more reading
                    emcTaskPlanSetWait();
                    // then resynch interpreter
                    emcTaskQueueCommand(&taskPlanSynchCmd);
                    break;

                    // immediate commands

                case EMC_NULLEVENT_TYPE:    //hong -use for append a null cmd
                case EMC_AXIS_SET_BACKLASH_TYPE:
                case EMC_AXIS_SET_HOMING_PARAMS_TYPE:
                case EMC_AXIS_SET_FERROR_TYPE:
                case EMC_AXIS_SET_MIN_FERROR_TYPE:
                case EMC_AXIS_UNHOME_TYPE:
                case EMC_TRAJ_PAUSE_TYPE:
                case EMC_TRAJ_RESUME_TYPE:
                case EMC_TRAJ_ABORT_TYPE:
                case EMC_TRAJ_SET_SCALE_TYPE:
                case EMC_TRAJ_SET_MAX_VELOCITY_TYPE:
                case EMC_TRAJ_SET_SPINDLE_SCALE_TYPE:
                case EMC_TRAJ_SET_FO_ENABLE_TYPE:
                case EMC_TRAJ_SET_FH_ENABLE_TYPE:
                case EMC_TRAJ_SET_SO_ENABLE_TYPE:
                case EMC_TRAJ_SET_G0_ENABLE_TYPE:
                case EMC_TRAJ_SET_DEFAULT_FEED_TYPE:
                case EMC_SPINDLE_INCREASE_TYPE:
                case EMC_SPINDLE_DECREASE_TYPE:
                case EMC_SPINDLE_CONSTANT_TYPE:
                case EMC_TASK_PLAN_PAUSE_TYPE:
                case EMC_TASK_PLAN_RESUME_TYPE:
                case EMC_TASK_PLAN_SET_OPTIONAL_STOP_TYPE:
                case EMC_TASK_PLAN_SET_BLOCK_DELETE_TYPE:
                case EMC_TASK_PLAN_OPTIONAL_STOP_TYPE:
                case EMC_TASK_SET_MODE_TYPE:
                case EMC_TASK_SET_STATE_TYPE:
                case EMC_TASK_ABORT_TYPE:
                case EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE:
                case EMC_TRAJ_PROBE_TYPE:
                case EMC_AUX_INPUT_WAIT_TYPE:
                case EMC_TRAJ_RIGID_TAP_TYPE:
                case EMC_SET_DEBUG_TYPE:
                case EMC_DOUBLEZ_CMD_TYPE:  // hong 2014-03-27
                case EMC_M99_CMD_TYPE:      // hong 2014-04/15
                case EMC_RESET_CMD_TYPE:  // HONG 2014-05-07
                case EMC_COOLANT_MIST_ON_TYPE:
                case EMC_COOLANT_MIST_OFF_TYPE:
                case EMC_COOLANT_FLOOD_ON_TYPE:
                case EMC_COOLANT_FLOOD_OFF_TYPE:
                case EMC_LUBE_ON_TYPE:
                case EMC_LUBE_OFF_TYPE: // all add by hong
                case EMC_SETLIGHT_CMD_TYPE:
                case EMC_TASK_EXECUTE_BLOCK_TYPE: //hong-2014/05/24
                case EMC_TASK_FLUSH_PAR_TYPE:
                case EMC_TASK_AUTOREPEAT_PAR_TYPE:  //hong 2014-06-24
                case EMC_TOOL_SET_NUMBER_TYPE: //hong 2014-06-30
                case EMC_MOTION_SET_EXTERN_OUT_TYPE:
                case EMC_TASK_SET_SIGNAL_TYPE:
                case EMC_TASK_SET_PARAMETER_TYPE:
                case EMC_MOTION_SET_DOUT_TYPE:
                case EMC_TRAJ_MULTI_FUNCTION_TYPE:
                    retval = emcTaskIssueCommand(emcCommand);
                    return retval;
                    break;

                case EMC_TASK_PLAN_STEP_TYPE: //解释器在读的状态下，点击了step，执行这里ghf-2014/4/26
                    stepping = 1;	// set stepping mode in case it's not
                    steppingWait = 0;	// clear the wait
                    break;

                case EMC_JOGWHEELS_GUIDE_CMD_TYPE:
                    retval = emcTaskIssueCommand(emcCommand);
                    break;

                    // otherwise we can't handle it
                default:
                    /*emcOperatorError(0,
                            _("can't do that (%s) in auto mode with the interpreter reading"),
                            emc_symbol_lookup(type));*/
                    retval = 0;
                    break;

                }		// switch (type) in ON, AUTO, READING

                // handle interp readahead logic
                readahead_reading();

                break;		// EMC_TASK_INTERP_READING

            case EMC_TASK_INTERP_VERIFY:
                switch (type) {
                case 0:
                case EMC_NULL_TYPE:
                    break;
                case EMC_TASK_ABORT_TYPE:
                    verifycontrol = true;
                    break;
                default:
                    break;
                }
                readahead_verify();
                break;

            case EMC_TASK_INTERP_PAUSED:	// ON, AUTO, PAUSED
                switch (type) {
                case 0:
                case EMC_NULL_TYPE:
                    // no command
                    break;
                case EMC_TOOL_LOAD_TOOL_TABLE_TYPE:
                case EMC_TOOL_SET_OFFSET_TYPE:
                    // send to IO
                    emcTaskQueueCommand(emcCommand);
                    // signify no more reading
                    emcTaskPlanSetWait();
                    // then resynch interpreter
                    emcTaskQueueCommand(&taskPlanSynchCmd);
                    break;
                    // immediate commands

                case EMC_NULLEVENT_TYPE:    //hong -use for append a null cmd
                case EMC_AXIS_SET_BACKLASH_TYPE:
                case EMC_AXIS_SET_HOMING_PARAMS_TYPE:
                case EMC_AXIS_SET_FERROR_TYPE:
                case EMC_AXIS_SET_MIN_FERROR_TYPE:
                case EMC_AXIS_UNHOME_TYPE:
                case EMC_TRAJ_PAUSE_TYPE:
                case EMC_TRAJ_RESUME_TYPE:
                case EMC_TRAJ_ABORT_TYPE:
                case EMC_TRAJ_SET_SCALE_TYPE:
                case EMC_TRAJ_SET_MAX_VELOCITY_TYPE:
                case EMC_TRAJ_SET_SPINDLE_SCALE_TYPE:
                case EMC_TRAJ_SET_FO_ENABLE_TYPE:
                case EMC_TRAJ_SET_FH_ENABLE_TYPE:
                case EMC_TRAJ_SET_SO_ENABLE_TYPE:
                case EMC_TRAJ_SET_G0_ENABLE_TYPE:
                case EMC_TRAJ_SET_DEFAULT_FEED_TYPE:
                case EMC_SPINDLE_SPEED_TYPE:
                case EMC_SPINDLE_ON_TYPE:
                case EMC_SPINDLE_OFF_TYPE:
                case EMC_SPINDLE_BRAKE_RELEASE_TYPE:
                case EMC_SPINDLE_BRAKE_ENGAGE_TYPE:
                case EMC_SPINDLE_INCREASE_TYPE:
                case EMC_SPINDLE_DECREASE_TYPE:
                case EMC_SPINDLE_CONSTANT_TYPE:
                case EMC_COOLANT_MIST_ON_TYPE:
                case EMC_COOLANT_MIST_OFF_TYPE:
                case EMC_COOLANT_FLOOD_ON_TYPE:
                case EMC_COOLANT_FLOOD_OFF_TYPE:
                case EMC_LUBE_ON_TYPE:
                case EMC_LUBE_OFF_TYPE:
                case EMC_TASK_SET_MODE_TYPE:
                case EMC_TASK_SET_STATE_TYPE:
                case EMC_TASK_ABORT_TYPE:
                case EMC_TASK_PLAN_EXECUTE_TYPE:
                case EMC_TASK_PLAN_PAUSE_TYPE:
                case EMC_TASK_PLAN_RESUME_TYPE:
                case EMC_TASK_PLAN_SET_OPTIONAL_STOP_TYPE:
                case EMC_TASK_PLAN_SET_BLOCK_DELETE_TYPE:
                case EMC_TASK_PLAN_OPTIONAL_STOP_TYPE:
                case EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE:
                case EMC_TRAJ_PROBE_TYPE:
                case EMC_AUX_INPUT_WAIT_TYPE:
                case EMC_TRAJ_RIGID_TAP_TYPE:
                case EMC_SET_DEBUG_TYPE:
                case EMC_M99_CMD_TYPE:      // hong 2014-04/15
                case EMC_RESET_CMD_TYPE:  // HONG 2014-05-07
                case EMC_JOGWHEELS_GUIDE_CMD_TYPE:
                case EMC_SETLIGHT_CMD_TYPE:
                case EMC_TASK_EXECUTE_BLOCK_TYPE: //hong-2014/05/24
                case EMC_TASK_FLUSH_PAR_TYPE:
                case EMC_TASK_AUTOREPEAT_PAR_TYPE:  //hong 2014-06-24
                case EMC_TOOL_SET_NUMBER_TYPE: //hong 2014-06-30
                case EMC_MOTION_SET_EXTERN_OUT_TYPE:
                case EMC_TASK_SET_SIGNAL_TYPE:
                case EMC_TASK_SET_PARAMETER_TYPE:
                case EMC_MOTION_SET_DOUT_TYPE:
                case EMC_TRAJ_MULTI_FUNCTION_TYPE:
                    retval = emcTaskIssueCommand(emcCommand);
                    break;

                case EMC_TASK_PLAN_STEP_TYPE: //正在运行过程中，点击暂停，再点击单步，会执行这里，command中也就是执行resume,ghf-4/26
                    //printf("pause,steping,emcStatus->motion.traj.queue=%d,emcStatus->motion.traj.paused=%d\n",emcStatus->motion.traj.queue,emcStatus->motion.traj.paused);
                    stepping = 1;
                    steppingWait = 0;
                    if (emcStatus->motion.traj.paused
                            && emcStatus->motion.traj.queue > 0) {
                        // there are pending motions paused; step them
                        //printf("pause111,steping\n");
                        emcTrajStep();
                    } else {
                        emcStatus->task.interpState =
                                (enum EMC_TASK_INTERP_ENUM) interpResumeState;
                    }
                    emcStatus->task.task_paused = 1;
                    break;

                    // otherwise we can't handle it
                default:
                    //emcOperatorError(0,
                    //        _("can't do that (%s) in auto mode with the interpreter paused"),
                     //       emc_symbol_lookup(type));
                    retval = 0;
                    break;

                }		// switch (type) in ON, AUTO, PAUSED

                break;		// EMC_TASK_INTERP_PAUSED

            case EMC_TASK_INTERP_WAITING:
                // interpreter ran to end
                // handle input commands
                switch (type) {
                case 0:
                case EMC_NULL_TYPE:
                    // no command
                    break;
                case EMC_TOOL_LOAD_TOOL_TABLE_TYPE:
                case EMC_TOOL_SET_OFFSET_TYPE:
                    // send to IO
                    emcTaskQueueCommand(emcCommand);
                    // signify no more reading
                    emcTaskPlanSetWait();
                    // then resynch interpreter
                    emcTaskQueueCommand(&taskPlanSynchCmd);
                    break;

                    // immediate commands

                case EMC_NULLEVENT_TYPE:    //hong -use for append a null cmd
                case EMC_AXIS_SET_BACKLASH_TYPE:
                case EMC_AXIS_SET_HOMING_PARAMS_TYPE:
                case EMC_AXIS_SET_FERROR_TYPE:
                case EMC_AXIS_SET_MIN_FERROR_TYPE:
                case EMC_AXIS_UNHOME_TYPE:
                case EMC_TRAJ_PAUSE_TYPE:
                case EMC_TRAJ_RESUME_TYPE:
                case EMC_TRAJ_ABORT_TYPE:
                case EMC_TRAJ_SET_SCALE_TYPE:
                case EMC_TRAJ_SET_MAX_VELOCITY_TYPE:
                case EMC_TRAJ_SET_SPINDLE_SCALE_TYPE:
                case EMC_TRAJ_SET_FO_ENABLE_TYPE:
                case EMC_TRAJ_SET_FH_ENABLE_TYPE:
                case EMC_TRAJ_SET_SO_ENABLE_TYPE:
                case EMC_TRAJ_SET_G0_ENABLE_TYPE:
                case EMC_TRAJ_SET_DEFAULT_FEED_TYPE:
                case EMC_SPINDLE_INCREASE_TYPE:
                case EMC_SPINDLE_DECREASE_TYPE:
                case EMC_SPINDLE_CONSTANT_TYPE:
                case EMC_TASK_PLAN_EXECUTE_TYPE:
                case EMC_TASK_PLAN_PAUSE_TYPE:
                case EMC_TASK_PLAN_RESUME_TYPE:
                case EMC_TASK_PLAN_SET_OPTIONAL_STOP_TYPE:
                case EMC_TASK_PLAN_SET_BLOCK_DELETE_TYPE:
                case EMC_TASK_PLAN_OPTIONAL_STOP_TYPE:
                case EMC_TASK_SET_MODE_TYPE:
                case EMC_TASK_SET_STATE_TYPE:
                case EMC_TASK_ABORT_TYPE:
                case EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE:
                case EMC_TRAJ_PROBE_TYPE:
                case EMC_AUX_INPUT_WAIT_TYPE:
                case EMC_TRAJ_RIGID_TAP_TYPE:
                case EMC_SET_DEBUG_TYPE:
                case EMC_DOUBLEZ_CMD_TYPE:  // hong 2014-03-27
                case EMC_M99_CMD_TYPE:      // hong 2014-04/15
                case EMC_RESET_CMD_TYPE:  // HONG 2014-05-07
                case EMC_COOLANT_MIST_ON_TYPE:
                case EMC_COOLANT_MIST_OFF_TYPE:
                case EMC_COOLANT_FLOOD_ON_TYPE:
                case EMC_COOLANT_FLOOD_OFF_TYPE:
                case EMC_LUBE_ON_TYPE:
                case EMC_LUBE_OFF_TYPE: //all add by hong
                case EMC_SETLIGHT_CMD_TYPE:
                case EMC_TASK_EXECUTE_BLOCK_TYPE: //hong-2014/05/24
                case EMC_TASK_FLUSH_PAR_TYPE:
                case EMC_TASK_AUTOREPEAT_PAR_TYPE:  //hong 2014-06-24
                case EMC_TOOL_SET_NUMBER_TYPE: //hong 2014-06-30
                case EMC_MOTION_SET_EXTERN_OUT_TYPE:
                case EMC_TASK_SET_SIGNAL_TYPE:
                case EMC_TASK_SET_PARAMETER_TYPE:
                case EMC_MOTION_SET_DOUT_TYPE:
                case EMC_TRAJ_MULTI_FUNCTION_TYPE:
                    retval = emcTaskIssueCommand(emcCommand);
                    break;

                case EMC_TASK_PLAN_STEP_TYPE: //解释器在等待的状态下，点击了step，执行这里ghf-2014/4/26
                    stepping = 1;	// set stepping mode in case it's not
                    steppingWait = 0;	// clear the wait
                    break;

                case EMC_JOGWHEELS_GUIDE_CMD_TYPE:	//ghf-2014/3/11,是否合适，有待验证

                    retval = emcTaskIssueCommand(emcCommand);
                    break;

                    // otherwise we can't handle it
                default:
                    //emcOperatorError(0,
                    //        _("can't do that (%s) in auto mode with the interpreter waiting"),
                     //       emc_symbol_lookup(type));
                    retval = 0;
                    break;

                }		// switch (type) in ON, AUTO, WAITING

                // handle interp readahead logic
                readahead_waiting();

                break;		// end of case EMC_TASK_INTERP_WAITING

            default:
                // coding error
                rcs_print_error("invalid mode(%d)", emcStatus->task.mode);
                retval = 0;
                break;

            }			// switch (mode) in ON, AUTO

            break;		// case EMC_TASK_MODE_AUTO

        case EMC_TASK_MODE_MDI:	// ON, MDI
            switch (type) {
            case 0:
            case EMC_NULL_TYPE:
                // no command
                break;

                // immediate commands

            case EMC_NULLEVENT_TYPE:    //hong -use for append a null cmd
            case EMC_AXIS_SET_BACKLASH_TYPE:
            case EMC_AXIS_SET_HOMING_PARAMS_TYPE:
            case EMC_AXIS_SET_FERROR_TYPE:
            case EMC_AXIS_SET_MIN_FERROR_TYPE:
            case EMC_AXIS_UNHOME_TYPE:
            case EMC_TRAJ_SET_SCALE_TYPE:
            case EMC_TRAJ_SET_MAX_VELOCITY_TYPE:
            case EMC_TRAJ_SET_SPINDLE_SCALE_TYPE:
            case EMC_TRAJ_SET_FO_ENABLE_TYPE:
            case EMC_TRAJ_SET_FH_ENABLE_TYPE:
            case EMC_TRAJ_SET_SO_ENABLE_TYPE:
            case EMC_TRAJ_SET_G0_ENABLE_TYPE:
            case EMC_TRAJ_SET_DEFAULT_FEED_TYPE:
            case EMC_SPINDLE_SPEED_TYPE:
            case EMC_SPINDLE_ON_TYPE:
            case EMC_SPINDLE_OFF_TYPE:
            case EMC_SPINDLE_BRAKE_RELEASE_TYPE:
            case EMC_SPINDLE_BRAKE_ENGAGE_TYPE:
            case EMC_SPINDLE_INCREASE_TYPE:
            case EMC_SPINDLE_DECREASE_TYPE:
            case EMC_SPINDLE_CONSTANT_TYPE:
            case EMC_COOLANT_MIST_ON_TYPE:
            case EMC_COOLANT_MIST_OFF_TYPE:
            case EMC_COOLANT_FLOOD_ON_TYPE:
            case EMC_COOLANT_FLOOD_OFF_TYPE:
            case EMC_LUBE_ON_TYPE:
            case EMC_LUBE_OFF_TYPE:
            case EMC_TASK_SET_MODE_TYPE:
            case EMC_TASK_SET_STATE_TYPE:
            case EMC_TASK_PLAN_INIT_TYPE:
            case EMC_TASK_PLAN_OPEN_TYPE:
            case EMC_TASK_PLAN_PAUSE_TYPE:
            case EMC_TASK_PLAN_SET_OPTIONAL_STOP_TYPE:
            case EMC_TASK_PLAN_SET_BLOCK_DELETE_TYPE:
            case EMC_TASK_PLAN_RESUME_TYPE:
            case EMC_TASK_PLAN_OPTIONAL_STOP_TYPE:
            case EMC_TASK_ABORT_TYPE:
            case EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE:
            case EMC_TRAJ_PROBE_TYPE:
            case EMC_AUX_INPUT_WAIT_TYPE:
            case EMC_MOTION_SET_DOUT_TYPE:
            case EMC_MOTION_SET_AOUT_TYPE:
            case EMC_MOTION_ADAPTIVE_TYPE:
            case EMC_TRAJ_RIGID_TAP_TYPE:
            case EMC_SET_DEBUG_TYPE:
            case EMC_DOUBLEZ_CMD_TYPE:  // hong 2014-03-27
            case EMC_RESET_CMD_TYPE:  // HONG 2014-05-07
            case EMC_SETLIGHT_CMD_TYPE:
            case EMC_TASK_EXECUTE_BLOCK_TYPE: //hong-2014/05/24
            case EMC_TOOL_SET_NUMBER_TYPE: //hong 2014-06-30
            case EMC_MOTION_SET_EXTERN_OUT_TYPE:
            case EMC_TASK_SET_SIGNAL_TYPE:
            case EMC_TASK_SET_PARAMETER_TYPE:
            case EMC_TRAJ_MULTI_FUNCTION_TYPE:
                retval = emcTaskIssueCommand(emcCommand);
                break;

            case EMC_TASK_PLAN_EXECUTE_TYPE:
                // If there are no queued MDI commands and no commands
                // in interp_list, then this new incoming MDI command
                // can just be issued directly.  Otherwise we need to
                // queue it and deal with it later.
                if ((mdi_execute_queue.len() == 0) && (interp_list.len() == 0)
                        && (emcTaskCommand == NULL)) {
                    retval = emcTaskIssueCommand(emcCommand);
                    if (0 == retval && flag_mdi_run) {
                        emcOperatorError(emcCommand->serial_number, "MDI_OK");
                    } else if(flag_mdi_run)
                        emcOperatorError(emcCommand->serial_number,
                                "MDI_ERROR");
                } else {
                    mdi_execute_queue.append(emcCommand);
                    retval = 0;
                }
                break;

            case EMC_TOOL_LOAD_TOOL_TABLE_TYPE:
            case EMC_TOOL_SET_OFFSET_TYPE:
                // send to IO
                emcTaskQueueCommand(emcCommand);
                // signify no more reading
                emcTaskPlanSetWait();
                // then resynch interpreter
                emcTaskQueueCommand(&taskPlanSynchCmd);
                break;

                // otherwise we can't handle it
            default:
                //emcOperatorError(0, _("can't do that (%s) in MDI mode"),
                //       emc_symbol_lookup(type));

                retval = -1;
                break;

            }			// switch (type) in ON, MDI

            mdi_execute_hook();

            break;		// case EMC_TASK_MODE_MDI

        default:
            break;

        }			// switch (mode)

        break;			// case EMC_TASK_STATE_ON

    default:
        break;

    }				// switch (task.state)

    return retval;
}

/*
 emcTaskCheckPreconditions() is called for commands on the interp_list.
 Immediate commands, i.e., commands sent from calls to emcTaskIssueCommand()
 in emcTaskPlan() directly, are not handled here.

 The return value is a state for emcTaskExecute() to wait on, e.g.,
 EMC_TASK_EXEC_WAITING_FOR_MOTION, before the command can be sent out.
 */
static int emcTaskCheckPreconditions(NMLmsg * cmd) {
    if (0 == cmd) {
        return EMC_TASK_EXEC_DONE;
    }

    switch (cmd->type) {
    // operator messages, if queued, will go out when everything before
    // them is done
    case EMC_OPERATOR_ERROR_TYPE:
    case EMC_OPERATOR_TEXT_TYPE:
    case EMC_OPERATOR_DISPLAY_TYPE:
    case EMC_SYSTEM_CMD_TYPE:
    case EMC_NULLEVENT_TYPE:
    case EMC_TRAJ_PROBE_TYPE:	// prevent blending of this
    case EMC_TRAJ_RIGID_TAP_TYPE: //and this
    case EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE:	// and this
    case EMC_AUX_INPUT_WAIT_TYPE:
    case EMC_SETLIGHT_CMD_TYPE:   // wait for io
    case EMC_TRAJ_MULTI_FUNCTION_TYPE:
    case EMC_TRAJ_SET_SPINDLESYNC_TYPE:
        return EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO;
        break;

    case EMC_TRAJ_SET_OFFSET_TYPE:      //change from EMC_TASK_EXEC_WAITING_FOR_MOTION
    case EMC_TRAJ_LINEAR_MOVE_TYPE:
    case EMC_TRAJ_MINLINEAR_MOVE_TYPE:
    case EMC_TRAJ_CIRCULAR_MOVE_TYPE:
    case EMC_TRAJ_SET_VELOCITY_TYPE:
    case EMC_TRAJ_SET_ACCELERATION_TYPE:
    case EMC_TRAJ_SET_TERM_COND_TYPE:
    case EMC_TRAJ_SET_FO_ENABLE_TYPE:
    case EMC_TRAJ_SET_FH_ENABLE_TYPE:
    case EMC_TRAJ_SET_SO_ENABLE_TYPE:
    case EMC_TRAJ_SET_G0_ENABLE_TYPE:
    case EMC_TOOL_SET_NUMBER_TYPE:
        return EMC_TASK_EXEC_WAITING_FOR_IO;
        break;

    //case EMC_TRAJ_SET_OFFSET_TYPE:
        // this applies the tool length offset variable after previous
        // motions
    case EMC_TRAJ_SET_G5X_TYPE:
    case EMC_TRAJ_SET_G92_TYPE:
    case EMC_TRAJ_SET_ROTATION_TYPE:
        // this applies the program origin after previous motions
        return EMC_TASK_EXEC_WAITING_FOR_MOTION;
        break;

    case EMC_TOOL_LOAD_TYPE:
    case EMC_TOOL_UNLOAD_TYPE:
    case EMC_COOLANT_MIST_ON_TYPE:
    case EMC_COOLANT_MIST_OFF_TYPE:
    case EMC_COOLANT_FLOOD_ON_TYPE:
    case EMC_COOLANT_FLOOD_OFF_TYPE:
    case EMC_SPINDLE_SPEED_TYPE:
    case EMC_SPINDLE_ON_TYPE:
    case EMC_SPINDLE_OFF_TYPE:
    case EMC_DOUBLEZ_CMD_TYPE:  // hong 2014-03-27
    case EMC_M99_CMD_TYPE:      // hong 2014-04/15
        return EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO;
        break;

    case EMC_TOOL_PREPARE_TYPE:
    case EMC_LUBE_ON_TYPE:
    case EMC_LUBE_OFF_TYPE:
        return EMC_TASK_EXEC_WAITING_FOR_IO;
        break;

    case EMC_TOOL_LOAD_TOOL_TABLE_TYPE:
    case EMC_TOOL_SET_OFFSET_TYPE:
        return EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO;
        break;

//forbid by hong ,it will exec EMC_TOOL_SET_NUMBER_TYPE in auto
//    case EMC_TOOL_SET_NUMBER_TYPE:
//    return EMC_TASK_EXEC_WAITING_FOR_IO;
//    break;

    case EMC_TASK_PLAN_PAUSE_TYPE:
    case EMC_TASK_PLAN_OPTIONAL_STOP_TYPE:
        /* pause on the interp list is queued, so wait until all are done */
        return EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO;
        break;

    case EMC_TASK_PLAN_END_TYPE:
        return EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO;
        break;

    case EMC_TASK_PLAN_INIT_TYPE:
    case EMC_TASK_PLAN_RUN_TYPE:
    case EMC_TASK_PLAN_SYNCH_TYPE:
    case EMC_TASK_PLAN_EXECUTE_TYPE:
    case EMC_TASK_SET_PARAMETER_TYPE:
    case EMC_TASK_PLAN_NULL_TYPE:
        return EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO;
        break;

    case EMC_TRAJ_DELAY_TYPE:
    case EMC_TASK_WAIT_FOR_SIGNAL_TYPE:     //hong-2014-09-23
    case EMC_TASK_SET_SIGNAL_TYPE:
        return EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO;
        break;

    case EMC_MOTION_SET_EXTERN_OUT_TYPE:
        return EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO;
        break;

    case EMC_MOTION_SET_AOUT_TYPE:
        if (((EMC_MOTION_SET_AOUT *) cmd)->now) {
            return EMC_TASK_EXEC_WAITING_FOR_MOTION;
        }
        return EMC_TASK_EXEC_DONE;
        break;

    case EMC_MOTION_SET_DOUT_TYPE:
        if (((EMC_MOTION_SET_DOUT *) cmd)->now) {
            return EMC_TASK_EXEC_WAITING_FOR_MOTION;
        }
        return EMC_TASK_EXEC_DONE;
        break;



    case EMC_MOTION_ADAPTIVE_TYPE:
        return EMC_TASK_EXEC_WAITING_FOR_MOTION;
        break;

    default:
        // unrecognized command
        if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
            rcs_print_error("preconditions: unrecognized command %d:%s\n",
                    (int) cmd->type, emc_symbol_lookup(cmd->type));
        }
        return EMC_TASK_EXEC_ERROR;
        break;
    }

    return EMC_TASK_EXEC_DONE;

}

// puts command on interp list
int emcTaskQueueCommand(NMLmsg * cmd) {
    if (0 == cmd) {
        return 0;
    }

    interp_list.append(cmd);

    return 0;
}

// issues command immediately

static int emcTaskIssueCommand(NMLmsg * cmd) {
    int retval = 0;
    int execRetval = 0;
    int testfollowerr = 0;
    //static double currenttime=0,oldtime=0;

    if (0 == cmd) {
        if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
            rcs_print("emcTaskIssueCommand() null command\n");
        }
        return 0;
    }
    if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
        rcs_print("Issuing %s -- \t (%s)\n", emcSymbolLookup(cmd->type),
                emcCommandBuffer->msg2str(cmd));
    }
    switch (cmd->type) {
    // general commands

    case EMC_NULLEVENT_TYPE:    //hong -use for append a null cmd
        retval = 0;
        break;

    case EMC_OPERATOR_ERROR_TYPE:
        retval = emcOperatorError(((EMC_OPERATOR_ERROR *) cmd)->id, "%s",
                ((EMC_OPERATOR_ERROR *) cmd)->error);
        break;

    case EMC_OPERATOR_TEXT_TYPE:
        retval = emcOperatorText(((EMC_OPERATOR_TEXT *) cmd)->id, "%s",
                ((EMC_OPERATOR_TEXT *) cmd)->text);
        break;

    case EMC_OPERATOR_DISPLAY_TYPE:
        static double recordtime = 0;
        printf("now I record the time %s  %f\n",((EMC_OPERATOR_DISPLAY *) cmd)->display,etime()-recordtime);
        recordtime = etime();
        retval = emcOperatorDisplay(((EMC_OPERATOR_DISPLAY *) cmd)->id, "%s",
                ((EMC_OPERATOR_DISPLAY *) cmd)->display);
        break;

    case EMC_SYSTEM_CMD_TYPE:
        retval = emcSystemCmd(((EMC_SYSTEM_CMD *) cmd)->string);
        break;

        // axis commands

    case EMC_AXIS_DISABLE_TYPE:
        disable_msg = (EMC_AXIS_DISABLE *) cmd;
        retval = emcAxisDisable(disable_msg->axis);
        break;

    case EMC_AXIS_ENABLE_TYPE:
        enable_msg = (EMC_AXIS_ENABLE *) cmd;
        retval = emcAxisEnable(enable_msg->axis);
        break;

    case EMC_AXIS_HOME_TYPE:
        home_msg = (EMC_AXIS_HOME *) cmd;
        retval = emcAxisHome(home_msg->axis,home_msg->flag);
        if(emcStatus->task.parameters[4070] > 0)    //绝对值电机保存零点
            emcZeroSave();
        break;

    case EMC_AXIS_UNHOME_TYPE:
        unhome_msg = (EMC_AXIS_UNHOME *) cmd;
        retval = emcAxisUnhome(unhome_msg->axis,unhome_msg->flag);
        break;

    case EMC_AXIS_JOG_TYPE:
        jog_msg = (EMC_AXIS_JOG *) cmd;
        retval = emcAxisJog(jog_msg->axis, jog_msg->vel);
        break;

    case EMC_AXIS_ABORT_TYPE:
        axis_abort_msg = (EMC_AXIS_ABORT *) cmd;
        retval = emcAxisAbort(axis_abort_msg->axis);
        break;

    case EMC_AXIS_INCR_JOG_TYPE:
        incr_jog_msg = (EMC_AXIS_INCR_JOG *) cmd;
        retval = emcAxisIncrJog(incr_jog_msg->axis, incr_jog_msg->incr,
                incr_jog_msg->vel);
        break;

    case EMC_AXIS_ABS_JOG_TYPE:
        abs_jog_msg = (EMC_AXIS_ABS_JOG *) cmd;
        retval = emcAxisAbsJog(abs_jog_msg->axis, abs_jog_msg->pos,
                abs_jog_msg->vel);
        break;

    case EMC_AXIS_SET_BACKLASH_TYPE:
        set_backlash_msg = (EMC_AXIS_SET_BACKLASH *) cmd;
        retval = emcAxisSetBacklash(set_backlash_msg->axis,
                set_backlash_msg->backlash);
        break;

    case EMC_AXIS_SET_HOMING_PARAMS_TYPE:
        set_homing_params_msg = (EMC_AXIS_SET_HOMING_PARAMS *) cmd;
        retval = emcAxisSetHomingParams(set_homing_params_msg->axis,
                set_homing_params_msg->home, set_homing_params_msg->offset,
                set_homing_params_msg->home_final_vel,
                set_homing_params_msg->search_vel,
                set_homing_params_msg->latch_vel,
                set_homing_params_msg->use_index,
                set_homing_params_msg->ignore_limits,
                set_homing_params_msg->is_shared,
                set_homing_params_msg->home_sequence,
                set_homing_params_msg->volatile_home,
                set_homing_params_msg->locking_indexer,
                set_homing_params_msg->pitchvalue,0,0,0,0,0,0);
        break;

    case EMC_AXIS_SET_FERROR_TYPE:
        set_ferror_msg = (EMC_AXIS_SET_FERROR *) cmd;
        retval = emcAxisSetFerror(set_ferror_msg->axis, set_ferror_msg->ferror);
        break;

    case EMC_AXIS_SET_MIN_FERROR_TYPE:
        set_min_ferror_msg = (EMC_AXIS_SET_MIN_FERROR *) cmd;
        retval = emcAxisSetMinFerror(set_min_ferror_msg->axis,
                set_min_ferror_msg->ferror);
        break;

    case EMC_AXIS_SET_MAX_POSITION_LIMIT_TYPE:
//        set_max_limit_msg = (EMC_AXIS_SET_MAX_POSITION_LIMIT *) cmd;
//        retval = emcAxisSetMaxPositionLimit(set_max_limit_msg->axis,
//                set_max_limit_msg->limit);
        break;

    case EMC_AXIS_SET_MIN_POSITION_LIMIT_TYPE:
//        set_min_limit_msg = (EMC_AXIS_SET_MIN_POSITION_LIMIT *) cmd;
//        retval = emcAxisSetMinPositionLimit(set_min_limit_msg->axis,
//                set_min_limit_msg->limit);
        break;

    case EMC_AXIS_HALT_TYPE:
        axis_halt_msg = (EMC_AXIS_HALT *) cmd;
        retval = emcAxisHalt(axis_halt_msg->axis);
        break;

    case EMC_AXIS_OVERRIDE_LIMITS_TYPE:
        axis_lim_msg = (EMC_AXIS_OVERRIDE_LIMITS *) cmd;
        retval = emcAxisOverrideLimits(axis_lim_msg->axis);
        break;

    case EMC_AXIS_LOAD_COMP_TYPE:
        axis_load_comp_msg = (EMC_AXIS_LOAD_COMP *) cmd;
        retval = emcAxisLoadComp(axis_load_comp_msg->axis,
                axis_load_comp_msg->file, axis_load_comp_msg->type);
        break;

        // traj commands

    case EMC_TRAJ_SET_SCALE_TYPE:
        emcTrajSetScaleMsg = (EMC_TRAJ_SET_SCALE *) cmd;
        retval = emcTrajSetScale(emcTrajSetScaleMsg->scale);
        break;

    case EMC_TRAJ_SET_MAX_VELOCITY_TYPE:
        emcTrajSetMaxVelocityMsg = (EMC_TRAJ_SET_MAX_VELOCITY *) cmd;
        retval = emcTrajSetMaxVelocity(emcTrajSetMaxVelocityMsg->velocity);
        break;

    case EMC_TRAJ_SET_SPINDLE_SCALE_TYPE:
        emcTrajSetSpindleScaleMsg = (EMC_TRAJ_SET_SPINDLE_SCALE *) cmd;
        retval = emcTrajSetSpindleScale(emcTrajSetSpindleScaleMsg->scale);
        break;

    case EMC_TRAJ_SET_FO_ENABLE_TYPE:
        retval = emcTrajSetFOEnable(((EMC_TRAJ_SET_FO_ENABLE *) cmd)->mode); // feed override enable/disable
        break;

    case EMC_TRAJ_SET_FH_ENABLE_TYPE:
        retval = emcTrajSetFHEnable(((EMC_TRAJ_SET_FH_ENABLE *) cmd)->mode); //feed hold enable/disable
        break;

    case EMC_TRAJ_SET_SO_ENABLE_TYPE:
        retval = emcTrajSetSOEnable(((EMC_TRAJ_SET_SO_ENABLE *) cmd)->mode); //spindle speed override enable/disable
        break;

    case EMC_TRAJ_SET_G0_ENABLE_TYPE:
        retval = emcTrajSetG0Enable(((EMC_TRAJ_SET_G0_ENABLE *) cmd)->mode); //g0 is under control by motion

    case EMC_TRAJ_SET_DEFAULT_FEED_TYPE:
        retval = emcSetChoiceDefaultFeed(((EMC_TRAJ_SET_DEFAULT_FEED *)cmd)->feed);
        break;

    case EMC_TRAJ_SET_VELOCITY_TYPE:
        emcTrajSetVelocityMsg = (EMC_TRAJ_SET_VELOCITY *) cmd;
        retval = emcTrajSetVelocity(emcTrajSetVelocityMsg->velocity,
                emcTrajSetVelocityMsg->ini_maxvel);
        break;

    case EMC_TRAJ_SET_ACCELERATION_TYPE:
        emcTrajSetAccelerationMsg = (EMC_TRAJ_SET_ACCELERATION *) cmd;
        retval = emcTrajSetAcceleration(
                emcTrajSetAccelerationMsg->acceleration);
        break;

    case EMC_TRAJ_MULTI_FUNCTION_TYPE:
        emcMultifuncMsg = (EMC_TRAJ_MULTI_FUNCTION*) cmd;
        if(1 == emcMultifuncMsg->type) {
            emcsetSelectZ(emcMultifuncMsg->intpar);
            emcSetparameter(4112,emcMultifuncMsg->intpar,1);
        }
        if(emcMultifuncMsg->type == 99) {
            //set the flush the tblfile flag while get the flush cmd in running status
            emcStatus->task.needreloadtool = 1;
        }

        if(emcMultifuncMsg->type == 20) {

            //4227 安全高度不为零，则开启限位
//            emcMultifuncMsg->doublepar = emcStatus->task.parameters[4227];
//            if(fabs(emcStatus->task.parameters[4227]) > 0.0001) {
                double xmax,xmin,ymax,ymin,xoffset,yoffset;
                xmax = emcStatus->task.parameters[4225] + emcStatus->task.parameters[4221]/2-emcStatus->task.parameters[4228]/2;
                xmin = emcStatus->task.parameters[4225] - emcStatus->task.parameters[4221]/2+emcStatus->task.parameters[4228]/2;
                ymax = emcStatus->task.parameters[4226] + emcStatus->task.parameters[4222]/2-emcStatus->task.parameters[4228]/2;
                ymin = emcStatus->task.parameters[4226] - emcStatus->task.parameters[4222]/2+emcStatus->task.parameters[4228]/2;
                xoffset = (emcStatus->task.parameters[4221]-emcStatus->task.parameters[4223]-emcStatus->task.parameters[4228])/2;
                yoffset = (emcStatus->task.parameters[4222]-emcStatus->task.parameters[4224]-emcStatus->task.parameters[4228])/2;
                emcAxisSetSecLimit(0,xmax,xmin,xoffset);
                emcAxisSetSecLimit(1,ymax,ymin,yoffset);
                emcAxisSetSecLimit(2,emcStatus->task.parameters[4055],emcStatus->task.parameters[4056],0);
                emcAxisSetSecLimit(3,emcStatus->task.parameters[4055],emcStatus->task.parameters[4056],0);
                emcAxisSetSecLimit(4,1000000,-1000000,0);
                emcAxisSetSecLimit(5,1000000,-1000000,0);
                emcAxisSetSecLimit(6,1000000,-1000000,0);
                emcAxisSetSecLimit(7,1000000,-1000000,0);
//            }
        }

        //换刀结束

        printf("now I get end multifunc:%d \n",emcMultifuncMsg->type);
        if(emcMultifuncMsg->type == 30) {
            toolchangeend();
        }
        emcSetMultifunc(emcMultifuncMsg->type,emcMultifuncMsg->intpar,emcMultifuncMsg->doublepar);

        break;

    case EMC_TRAJ_MINLINEAR_MOVE_TYPE:
        emcTrajMinLinearMsg = (EMC_TRAJ_MINLINEAR_MOVE *) cmd;
        //printf("in the mini world5 %d gettime %d\n",emcTrajMinLinearMsg->mininum,getval(emcStatus->task.lefttimeval));

        if(emcStatus->motion.traj.mode != EMC_TRAJ_MODE_COORD)
            emcTrajSetMode(EMC_TRAJ_MODE_COORD);

        EmcPose end[MAXMINLEN];
        static double oldx,oldy,oldz;
        for (int i = 0; i < emcTrajMinLinearMsg->mininum; i++) {
            /* we wait for the specify line in smart run mode */
            if(emcTrajMinLinearMsg->minlinenum[i] < emcStatus->task.runfromline &&
            emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
            emcStatus->task.currentLine != 0) {
                continue;
            }

            double dynamicp[DYNAMIC_PAR_NUM];
            for(int i = 0;i<DYNAMIC_PAR_NUM;i++)
                dynamicp[i] = 0;

            dynamicp[0] = emcStatus->task.toolnow;               // current tool in used
            dynamicp[1] = emcStatus->io.tool.toolcompflag;              // curent cutter compensation direction
            dynamicp[2] = emcTrajMinLinearMsg->dynamiccomp[i];  // current dynamic compensation
            dynamicp[3] = emcStatus->task.toolOffset.tran.z;
            dynamicp[4] = runprobe;                 //把对刀仪程序运行的标志发下去
            /* we lift z first to ensure not to crash */
            if(emcTrajMinLinearMsg->minlinenum[i] >= emcStatus->task.runfromline &&
                    emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
                    emcStatus->task.runfromline > 0) {
                double tempz = emcTrajMinLinearMsg->minz[i];
                if(emcStatus->motion.doublez.zdiff < 0 &&
                       2 == emcStatus->motion.doublez.whichz_flag)
                emcTrajMinLinearMsg->minz[i] = emcStatus->motion.doublez.zdiff;
                else
                    emcTrajMinLinearMsg->minz[i] = 0;

                end[i].tran.x = emcTrajMinLinearMsg->minx[i];
                end[i].tran.y = emcTrajMinLinearMsg->miny[i];
                end[i].tran.z = emcTrajMinLinearMsg->minz[i];
                end[i].a = emcTrajMinLinearMsg->mina[i];; end[i].b = emcTrajMinLinearMsg->minb[i];
                end[i].c = emcTrajMinLinearMsg->minc[i]; end[i].u = emcTrajMinLinearMsg->minu[i];
                end[i].v = emcTrajMinLinearMsg->minv[i]; end[i].w = emcTrajMinLinearMsg->minw[i];
                emcTrajminiLinearMove(end[i],
                                  EMC_MOTION_TYPE_FEED, emcStatus->task.parameters[4023],
                                emcStatus->task.parameters[4023], 400,-1,
                                      0,
                        &dynamicp[0]);
                end[i].tran.z = tempz;

                emcTrajminiLinearMove(end[i],
                                  EMC_MOTION_TYPE_FEED, emcStatus->task.parameters[4023],
                                emcStatus->task.parameters[4023], 400,-1,
                                      emcTrajMinLinearMsg->gmodetype[i],
                        &dynamicp[0]);
                emcStatus->task.runfromline = 0;
                continue;
            }


           end[i].tran.x = emcTrajMinLinearMsg->minx[i];
           end[i].tran.y = emcTrajMinLinearMsg->miny[i];
           end[i].tran.z = emcTrajMinLinearMsg->minz[i];
           end[i].a = emcTrajMinLinearMsg->mina[i];; end[i].b = emcTrajMinLinearMsg->minb[i];
           end[i].c = emcTrajMinLinearMsg->minc[i]; end[i].u = emcTrajMinLinearMsg->minu[i];
           end[i].v = emcTrajMinLinearMsg->minv[i]; end[i].w = emcTrajMinLinearMsg->minw[i];


           if(emcTrajMinLinearMsg->cmdtype[i]==0) {

               if(fabs(end[i].tran.x - oldx) < 0.001 && fabs(end[i].tran.y - oldy) < 0.001
                       && (end[i].tran.z - oldz  < 0) &&
                       0 == emcTrajMinLinearMsg->gmodetype[i] &&
                       1 == emcTrajMinLinearMsg->gmodetype[i+1]
                       && emcStatus->task.parameters[4080] > 0) {
                   /* Approach distance control mode -- add by hong on 2015-10-21 */
                   if (EMC_DEBUG & EMC_DEBUG_SENDPOINT)
                    printf("get in approach distance control:distance:%.3f speed:%.3f\n",end[i].tran.z,oldz);
                   if((oldz - end[i].tran.z )> emcStatus->task.parameters[4080]) {
                       /* g0 seg is splited into two parts , make the closing speed effects in the second part */
                       double old_tempz = end[i].tran.z;
                       end[i].tran.z = end[i].tran.z+emcStatus->task.parameters[4080];
                       emcTrajminiLinearMove(end[i], EMC_MOTION_TYPE_FEED,
                                                                     emcTrajMinLinearMsg->vel[i], 0, 0,
                                                                     emcTrajMinLinearMsg->minlinenum[i],
                                                                     emcTrajMinLinearMsg->gmodetype[i],
                                             &dynamicp[0]);
                       end[i].tran.z = old_tempz;

                       retval = emcTrajminiLinearMove(end[i], EMC_MOTION_TYPE_FEED,
                                                 emcStatus->task.parameters[4081], 0, 0,
                                                 emcTrajMinLinearMsg->minlinenum[i],
                                                 1,                                                                     //now we control the feed vel
                               &dynamicp[0]);
                   } else {
                       retval = emcTrajminiLinearMove(end[i], EMC_MOTION_TYPE_FEED,
                                                 emcStatus->task.parameters[4081], 0, 0,
                                                 emcTrajMinLinearMsg->minlinenum[i],
                                                 1,
                                                &dynamicp[0]);
                   }
               } else {
                   /* nomal command work */
                    retval = emcTrajminiLinearMove(end[i], EMC_MOTION_TYPE_FEED,
                                              emcTrajMinLinearMsg->vel[i], 0, 0,
                                              emcTrajMinLinearMsg->minlinenum[i],
                                              emcTrajMinLinearMsg->gmodetype[i],
                                                   &dynamicp[0]);
               }
           } else {
              retval = emcTrajminiCircularMove(end[i],
                                           emcTrajMinLinearMsg->center[i], emcTrajMinLinearMsg->normal[i],
                                           emcTrajMinLinearMsg->turn[i], EMC_MOTION_TYPE_ARC,
                                           emcTrajMinLinearMsg->vel[i], 0, 0,
                                           emcTrajMinLinearMsg->minlinenum[i],
                                               &dynamicp[0]);
           }
           oldx = end[i].tran.x;oldy = end[i].tran.y;oldz = end[i].tran.z;

           if(retval != 0)
               break;
        }

        break;

    case EMC_TRAJ_LINEAR_MOVE_TYPE:
        //currenttime = etime();
        //printf("time is %f\n",(currenttime - oldtime));
        //oldtime = currenttime;

        emcTrajLinearMoveMsg = (EMC_TRAJ_LINEAR_MOVE *) cmd;

        if(emcStatus->task.currentLine < emcStatus->task.runfromline &&
        emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
        emcStatus->task.currentLine > 0)
            break;

        if(emcStatus->motion.traj.mode != EMC_TRAJ_MODE_COORD)
            emcTrajSetMode(EMC_TRAJ_MODE_COORD);

//        printf("send x=%.3f,y=%.3f,z=%.3f\n",emcTrajLinearMoveMsg->end.tran.x,emcTrajLinearMoveMsg->end.tran.y,emcTrajLinearMoveMsg->end.tran.z);
        if(emcStatus->task.currentLine >= emcStatus->task.runfromline &&
                emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
                emcStatus->task.runfromline > 0) {
            double tempz = emcTrajLinearMoveMsg->end.tran.z;
            if(emcStatus->motion.doublez.zdiff < 0 &&
                   2 == emcStatus->motion.doublez.whichz_flag)
            emcTrajLinearMoveMsg->end.tran.z = emcStatus->motion.doublez.zdiff;
            else
                emcTrajLinearMoveMsg->end.tran.z = 0;
            emcTrajLinearMove(emcTrajLinearMoveMsg->end,
                              EMC_MOTION_TYPE_TRAVERSE, emcStatus->task.parameters[4023],
                            emcStatus->task.parameters[4023], 400,-1,1);
            emcTrajLinearMoveMsg->end.tran.z = tempz;
            emcTrajLinearMove(emcTrajLinearMoveMsg->end,
                              EMC_MOTION_TYPE_TRAVERSE, emcStatus->task.parameters[4023],
                            emcStatus->task.parameters[4031], 400,-1,1);
            emcStatus->task.runfromline = 0;
            break;
        }


        emcTrajLinearMoveMsg->end.a = 0;
        emcTrajLinearMoveMsg->end.b = 0;
        emcTrajLinearMoveMsg->end.c = 0;
        emcTrajLinearMoveMsg->end.u = 0;
        emcTrajLinearMoveMsg->end.v = 0;
        emcTrajLinearMoveMsg->end.w = 0;
        retval = emcTrajLinearMove(emcTrajLinearMoveMsg->end,
                emcTrajLinearMoveMsg->type, emcTrajLinearMoveMsg->vel,
                emcTrajLinearMoveMsg->ini_maxvel, emcTrajLinearMoveMsg->acc,
                emcTrajLinearMoveMsg->indexrotary,
                emcTrajLinearMoveMsg->gmodetype); //ghf-2014/3/22

        break;

    case EMC_TRAJ_CIRCULAR_MOVE_TYPE:
        emcTrajCircularMoveMsg = (EMC_TRAJ_CIRCULAR_MOVE *) cmd;
//        printf("++++++ x=%.3f,y=%.3f,z=%.3f \n",emcTrajCircularMoveMsg->end.tran.x,emcTrajCircularMoveMsg->end.tran.y,emcTrajCircularMoveMsg->end.tran.z);
//        printf("------ x=%.3f,y=%.3f,z=%.3f \n",emcTrajCircularMoveMsg->center.x,emcTrajCircularMoveMsg->center.y,emcTrajCircularMoveMsg->center.z);

        if(emcStatus->task.currentLine < emcStatus->task.runfromline &&
                emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
                emcStatus->task.currentLine > 0)
            break;

        if(emcStatus->motion.traj.mode != EMC_TRAJ_MODE_COORD)
            emcTrajSetMode(EMC_TRAJ_MODE_COORD);

        if(emcStatus->task.currentLine < emcStatus->task.runfromline &&
                emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
                emcStatus->task.runfromline > 0) {
            double tempz = emcTrajCircularMoveMsg->end.tran.z;
            if(emcStatus->motion.doublez.zdiff < 0 &&
                   2 == emcStatus->motion.doublez.whichz_flag)
            emcTrajCircularMoveMsg->end.tran.z = emcStatus->motion.doublez.zdiff;
            else
                emcTrajCircularMoveMsg->end.tran.z = 0;
            emcTrajLinearMove(emcTrajCircularMoveMsg->end,
                              EMC_MOTION_TYPE_TRAVERSE, emcStatus->task.parameters[4023],
                              emcStatus->task.parameters[4023], 400,-1,1);
            emcTrajCircularMoveMsg->end.tran.z = tempz;
            emcTrajLinearMove(emcTrajCircularMoveMsg->end,
                              EMC_MOTION_TYPE_TRAVERSE, emcStatus->task.parameters[4023],
                              emcStatus->task.parameters[4031], 400,-1,1);
            emcStatus->task.runfromline = 0;
            break;
        }


        emcTrajCircularMoveMsg->end.a = 0;
        emcTrajCircularMoveMsg->end.b = 0;
        emcTrajCircularMoveMsg->end.c = 0;
        emcTrajCircularMoveMsg->end.u = 0;
        emcTrajCircularMoveMsg->end.v = 0;
        emcTrajCircularMoveMsg->end.w = 0;
        retval = emcTrajCircularMove(emcTrajCircularMoveMsg->end,
                emcTrajCircularMoveMsg->center, emcTrajCircularMoveMsg->normal,
                emcTrajCircularMoveMsg->turn, emcTrajCircularMoveMsg->type,
                emcTrajCircularMoveMsg->vel, emcTrajCircularMoveMsg->ini_maxvel,
                emcTrajCircularMoveMsg->acc);
        break;

    case EMC_TRAJ_PAUSE_TYPE:
        emcStatus->task.task_paused = 1;
        retval = emcTrajPause();
        break;

    case EMC_TRAJ_RESUME_TYPE:
        emcStatus->task.task_paused = 0;
        retval = emcTrajResume();
        break;

    case EMC_TRAJ_ABORT_TYPE:
        retval = emcTrajAbort();
        break;

    case EMC_TRAJ_DELAY_TYPE: {
        emcTrajDelayMsg = (EMC_TRAJ_DELAY *) cmd;
        //run from line

        printf("now I in set delay time:%d   %f %d\n",emcTrajDelayMsg->spindleflag,spindle_delay,startspindleflag);
        if(1 == emcTrajDelayMsg->spindleflag) {
            emcTrajDelayMsg->delay = spindle_delay;
            if(startspindleflag == 0) {
                taskExecDelayTimeout = etime();
                break;
            } else {
                startspindleflag = 0;
                if(emcStatus->motion.spindle.speed > 0) {
                    emcTrajDelayMsg->delay = emcStatus->motion.spindle.lefttime;
                }
            }

        }

        if(!runlinedelay && emcStatus->task.currentLine < emcStatus->task.runfromline &&
                emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
                0 < emcStatus->task.runfromline) {
            taskExecDelayTimeout = etime();
            retval = 0;
            break;
        }
        if(runlinedelay)
            runlinedelay = false;
        // set the timeout clock to expire at 'now' + delay time
        double delaytime = 0;
        if(1 == g4delaytype) {
            delaytime = emcTrajDelayMsg->delay*0.001;
        } else
            delaytime = emcTrajDelayMsg->delay;

        printf("now I in delay time:%f \n",delaytime);
        taskExecDelayTimeout = etime() + delaytime;
        retval = 0;
        break;
    }
    case EMC_TASK_WAIT_FOR_SIGNAL_TYPE:
        char msg[256];
        emcWaitforsignalMsg = (EMC_TASK_WAIT_FOR_SIGNAL *)cmd;
        taskExecDelayTimeout = etime() + emcWaitforsignalMsg->delaytime;
        emcStatus->task.waitsignal = 1;
        emcAuxInputWaitIndex = 0;
        emcAuxInputWaitType = WAIT_MODE_SIGNAL;
        //printf("time = %.3f msg=%s\n",emcWaitforsignalMsg->delaytime,
     //          emcWaitforsignalMsg->msg);
        sprintf(msg,"M91:%s",emcWaitforsignalMsg->msg);
        emcOperatorText(0,msg);
        retval = 0;
        break;

    case EMC_TASK_SET_SIGNAL_TYPE:
        emcSetsignalMsg = (EMC_TASK_SET_SIGNAL *)cmd;
        emcStatus->task.waitsignal = 0;
        if(emcStatus->task.mode == EMC_TASK_MODE_MANUAL) {
            emcSetparameter(4100, emcSetsignalMsg->signal,0);
        } else {
            emcSetparameter(4100, emcSetsignalMsg->signal,1);
        }
        printf("now set signal is %d\n",emcSetsignalMsg->signal);
        retval = 0;
        break;

    case EMC_TASK_SET_PARAMETER_TYPE:
        emcSetparameterMsg = (EMC_TASK_SET_PARAMETER *)cmd;
        printf("set par %d %.3f\n",emcSetparameterMsg->index,
               emcSetparameterMsg->val);
        if(emcStatus->task.mode == EMC_TASK_MODE_MANUAL) {
            emcSetparameter(emcSetparameterMsg->index,emcSetparameterMsg->val,0);
        } else {
            emcSetparameter(emcSetparameterMsg->index,emcSetparameterMsg->val,1);
        }
        emcTaskUpdate(&emcStatus->task);
        retval = 0;
        break;

    case EMC_TRAJ_SET_TERM_COND_TYPE:
        emcTrajSetTermCondMsg = (EMC_TRAJ_SET_TERM_COND *) cmd;
        retval = emcTrajSetTermCond(emcTrajSetTermCondMsg->cond,
                emcTrajSetTermCondMsg->tolerance);
        break;

    case EMC_TRAJ_SET_SPINDLESYNC_TYPE:
        emcTrajSetSpindlesyncMsg = (EMC_TRAJ_SET_SPINDLESYNC *) cmd;
        retval = emcTrajSetSpindleSync(
                emcTrajSetSpindlesyncMsg->feed_per_revolution,
                emcTrajSetSpindlesyncMsg->velocity_mode);

        if(emcTrajSetSpindlesyncMsg->feed_per_revolution == 0) {// 主轴已经停止
            spindlealreadyon = 0;
        }
        break;

    case EMC_TRAJ_SET_OFFSET_TYPE:
        // update tool offset
        emcStatus->task.toolOffset = ((EMC_TRAJ_SET_OFFSET *) cmd)->offset;
        emcStatus->io.tool.toolcompflag =  ((EMC_TRAJ_SET_OFFSET *) cmd)->useflag;
        retval = emcTrajSetOffset(emcStatus->task.toolOffset);
        break;

    case EMC_TRAJ_SET_ROTATION_TYPE:
        emcStatus->task.rotation_xy = ((EMC_TRAJ_SET_ROTATION *) cmd)->rotation;
        retval = 0;
        break;

    case EMC_TRAJ_SET_G5X_TYPE: {
        // struct-copy program origin
        int old_g5x_index = emcStatus->task.g5x_index;
        emcStatus->task.g5x_offset = ((EMC_TRAJ_SET_G5X *) cmd)->origin;
        emcStatus->task.g5x_index = ((EMC_TRAJ_SET_G5X *) cmd)->g5x_index;

        retval = 0;
        emcStatus->task.g5x_offset.tran.z = emcStatus->task.parameters[5203+emcStatus->task.g5x_index*20];
        printf("now in set g5x offset %.3f\n",emcStatus->task.g5x_offset.tran.z);

        emcSetMultifunc(55,0,emcStatus->task.g5x_offset.tran.x);
        emcSetMultifunc(56,0,emcStatus->task.g5x_offset.tran.y);
        emcSetMultifunc(57,0,emcStatus->task.g5x_offset.tran.z);


        // while the runfromline is not zero ,then we cannot sync the actual coordinate to interp,
        //otherwise,it will cause error
        if(emcStatus->task.currentLine < emcStatus->task.runfromline &&
        emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
        emcStatus->task.currentLine > 0)
            break;

    }break;
    case EMC_TRAJ_SET_G92_TYPE:
        // struct-copy program origin
        emcStatus->task.g92_offset = ((EMC_TRAJ_SET_G92 *) cmd)->origin;
        retval = 0;
        break;
    case EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE:
        retval = emcTrajClearProbeTrippedFlag();
        break;

    case EMC_TRAJ_PROBE_TYPE:
        if(emcStatus->motion.traj.mode != EMC_TRAJ_MODE_COORD)
            emcTrajSetMode(EMC_TRAJ_MODE_COORD);
        printf("send probe target zpos : %f\n",((EMC_TRAJ_PROBE *) cmd)->pos.tran.z);
        retval = emcTrajProbe(((EMC_TRAJ_PROBE *) cmd)->pos,
                ((EMC_TRAJ_PROBE *) cmd)->type, ((EMC_TRAJ_PROBE *) cmd)->vel,
                ((EMC_TRAJ_PROBE *) cmd)->ini_maxvel,
                ((EMC_TRAJ_PROBE *) cmd)->acc,
                ((EMC_TRAJ_PROBE *) cmd)->probe_type);
        break;

    case EMC_AUX_INPUT_WAIT_TYPE:
        emcAuxInputWaitMsg = (EMC_AUX_INPUT_WAIT *) cmd;
        if (emcAuxInputWaitMsg->timeout == WAIT_MODE_IMMEDIATE) { //nothing to do, CANON will get the needed value when asked by the interp
            emcStatus->task.input_timeout = 0; // no timeout can occur
            emcAuxInputWaitIndex = -1;
            taskExecDelayTimeout = 0.0;
        } else {
            emcAuxInputWaitType = emcAuxInputWaitMsg->wait_type; // remember what we are waiting for
            emcAuxInputWaitIndex = emcAuxInputWaitMsg->index; // remember the input to look at
            emcStatus->task.input_timeout = 2; // set timeout flag, gets cleared if input changes before timeout happens
            // set the timeout clock to expire at 'now' + delay time
            taskExecDelayTimeout = etime() + emcAuxInputWaitMsg->timeout;
        }
        break;

    case EMC_TRAJ_RIGID_TAP_TYPE:
        retval = emcTrajRigidTap(((EMC_TRAJ_RIGID_TAP *) cmd)->pos,
                ((EMC_TRAJ_RIGID_TAP *) cmd)->vel,
                ((EMC_TRAJ_RIGID_TAP *) cmd)->ini_maxvel,
                ((EMC_TRAJ_RIGID_TAP *) cmd)->acc);
        break;

    case EMC_TRAJ_SET_TELEOP_ENABLE_TYPE:
        if (((EMC_TRAJ_SET_TELEOP_ENABLE *) cmd)->enable) {
            retval = emcTrajSetMode(EMC_TRAJ_MODE_TELEOP);
        } else {
            retval = emcTrajSetMode(EMC_TRAJ_MODE_FREE);
        }
        break;

    case EMC_TRAJ_SET_TELEOP_VECTOR_TYPE:
        retval = emcTrajSetTeleopVector(
                ((EMC_TRAJ_SET_TELEOP_VECTOR *) cmd)->vector);
        break;

    case EMC_MOTION_SET_AOUT_TYPE:
        retval = emcMotionSetAout(((EMC_MOTION_SET_AOUT *) cmd)->index,
                ((EMC_MOTION_SET_AOUT *) cmd)->start,
                ((EMC_MOTION_SET_AOUT *) cmd)->end,
                ((EMC_MOTION_SET_AOUT *) cmd)->now);
        break;

    case EMC_MOTION_SET_EXTERN_OUT_TYPE:{
        int index = ((EMC_MOTION_SET_EXTERN_OUT *) cmd)->index;
        int status = ((EMC_MOTION_SET_EXTERN_OUT *) cmd)->status;
        emcSetIo(index,status);
        int getcount = 0;
        do {
            usleep(1000*1);
            emcMotionUpdate(&emcStatus->motion);
            getcount++;
            //printf("now I in get the io status %d\n",emcStatus->motion.mot_extern_out[index]);
            if(getcount > 50) {
                getcount = 0;
                retval = -1;
                printf("now cannot get the io status\n");
                emcOperatorError(0,
                        _("Now the Set io status error!"));
                break;
            }
            emcSetIo(index,status);
        } while(emcStatus->motion.mot_extern_out[index] != status);
    }break;

    case EMC_MOTION_SET_DOUT_TYPE:
        if(((EMC_MOTION_SET_DOUT *) cmd)->index == 10 && ((EMC_MOTION_SET_DOUT *) cmd)->end == 1) {
            if(emcStatus->task.parameters[4293] == 0 && emcStatus->motion.synch_di[15] > 0) {       //有松刀信号不能主轴定位
                retval = -1;
                emcOperatorError(0, _("存在松刀信号不能主轴定位"));
                break;
            }
        }
        retval = emcMotionSetDout(((EMC_MOTION_SET_DOUT *) cmd)->index,
                ((EMC_MOTION_SET_DOUT *) cmd)->start,
                ((EMC_MOTION_SET_DOUT *) cmd)->end,
                ((EMC_MOTION_SET_DOUT *) cmd)->now);
        break;

    case EMC_MOTION_ADAPTIVE_TYPE:
        retval = emcTrajSetAFEnable(((EMC_MOTION_ADAPTIVE *) cmd)->status);
        break;

    case EMC_SET_DEBUG_TYPE:
        /* set the debug level here */
        EMC_DEBUG = ((EMC_SET_DEBUG *) cmd)->debug;
        /* and in IO and motion */
        emcIoSetDebug (EMC_DEBUG);
        emcMotionSetDebug(EMC_DEBUG);
        /* and reflect it in the status-- this isn't updated continually */
        emcStatus->debug = EMC_DEBUG;
        break;

        // unimplemented ones

        // IO commands

    case EMC_SPINDLE_SPEED_TYPE:
        spindle_speed_msg = (EMC_SPINDLE_SPEED *) cmd;
        dfspindlespeed = spindle_speed_msg->speed;
        //printf("now set spindle speed %.3f\n",spindle_speed_msg->speed);
        retval = emcSpindleSpeed(spindle_speed_msg->speed,
                spindle_speed_msg->factor, spindle_speed_msg->xoffset);
        break;

    case EMC_SPINDLE_ON_TYPE:

        if(spindlealreadyon == 0) {
            spindleDelaytimeout = etime() + spindle_delay;
        }
        spindle_on_msg = (EMC_SPINDLE_ON *) cmd;

        if(emcStatus->task.parameters[4293] == 0 && emcStatus->motion.synch_di[15] > 0) {       //有松刀信号不能开主轴
            retval = -1;
            emcOperatorError(0, _("存在松刀信号不能开主轴"));
            break;
        }

        if(((emcStatus->motion.spindle.speed != 0 &&  emcStatus->motion.spindle.lefttime > 0 )||
            emcStatus->motion.spindle.speed == 0 ) && spindleon_before_run != 1 || emcStatus->motion.spindle.direction == 0)
            startspindleflag = 1;
        else {
            startspindleflag = 0;
            spindleDelaytimeout = etime();
        }
        printf("now get spindle on is %d  dir:%d\n",startspindleflag,emcStatus->motion.spindle.direction);
        if (emcStatus->motion.spindle.direction != 0)
            break;
        double speed;
        if (1 == sMode) {
            emcSetspindlespeed(dfspindlespeed);
            speed = dfspindlespeed;
        } else {
            emcSetspindlespeed(spindle_on_msg->speed);
            speed = spindle_on_msg->speed;
        }
        retval = emcSpindleOn(speed, spindle_on_msg->factor,
                spindle_on_msg->xoffset);
        spindlealreadyon = 1;
        emcTrajSetSOEnable(1);//ghf-2015/1/27

        break;

    case EMC_SPINDLE_OFF_TYPE:
        retval = emcSpindleOff();
        emcTrajSetSOEnable(0);//ghf-2015/1/27
        spindlealreadyon = 0;
        break;

    case EMC_SPINDLE_BRAKE_RELEASE_TYPE:
        retval = emcSpindleBrakeRelease();
        break;

    case EMC_SPINDLE_INCREASE_TYPE:
        retval = emcSpindleIncrease();
        break;

    case EMC_SPINDLE_DECREASE_TYPE:
        retval = emcSpindleDecrease();
        break;

    case EMC_SPINDLE_CONSTANT_TYPE:
        retval = emcSpindleConstant();
        break;

    case EMC_SPINDLE_BRAKE_ENGAGE_TYPE:
        retval = emcSpindleBrakeEngage();
        break;

    case EMC_COOLANT_MIST_ON_TYPE:
        retval = emcCoolantMistOn();
        break;

    case EMC_COOLANT_MIST_OFF_TYPE:
        retval = emcCoolantMistOff();
        break;

    case EMC_COOLANT_FLOOD_ON_TYPE:
        retval = emcCoolantFloodOn();
        break;

    case EMC_COOLANT_FLOOD_OFF_TYPE:
        retval = emcCoolantFloodOff();
        break;

    case EMC_LUBE_ON_TYPE:
        retval = emcLubeOn();
        break;

    case EMC_LUBE_OFF_TYPE:
        retval = emcLubeOff();
        break;

    case EMC_TOOL_PREPARE_TYPE:
//        tool_prepare_msg = (EMC_TOOL_PREPARE *) cmd;
//        retval = emcToolPrepare(tool_prepare_msg->tool);
        break;

    case EMC_TOOL_LOAD_TYPE:

//        if(emcStatus->task.parameters[4127] > 0) {
//            break;
//        }
         if(emcStatus->motion.doublez.whichz_flag == 2 && (int)emcStatus->task.parameters[4300] == (int)emcStatus->task.parameters[4283]  &&
                  emcStatus->task.mode == EMC_TASK_MODE_AUTO) {
            if((int)emcStatus->task.parameters[4283] != (int)emcStatus->task.parameters[4284]) {
                retval = -1;
                emcOperatorError(0, _("双Z轴模式下刀具号不一致"));
                break;
            }
              break;
        }

         if(emcStatus->task.parameters[4127] == 0) {
             emcSetparameter(4294,1,1);
         }

        emcSetparameter(4227,0,1);
        emcSetMultifunc(20,0,0);

        break;

    case EMC_TOOL_UNLOAD_TYPE:
        retval = emcToolUnload();
        break;

    case EMC_TOOL_LOAD_TOOL_TABLE_TYPE:
        load_tool_table_msg = (EMC_TOOL_LOAD_TOOL_TABLE *) cmd;
        strcpy(tblfile,load_tool_table_msg->file);
        retval = emcToolLoadToolTable(load_tool_table_msg->file);
        break;

    case EMC_TOOL_SET_OFFSET_TYPE:
        emc_tool_set_offset_msg = (EMC_TOOL_SET_OFFSET *) cmd;

        retval = emcToolSetOffset(emc_tool_set_offset_msg->pocket,
                emc_tool_set_offset_msg->toolno,
                emc_tool_set_offset_msg->offset,
                emc_tool_set_offset_msg->diameter,
                emc_tool_set_offset_msg->frontangle,
                emc_tool_set_offset_msg->backangle,
                emc_tool_set_offset_msg->orientation);
        break;

    case EMC_TOOL_SET_NUMBER_TYPE:
        emc_tool_set_number_msg = (EMC_TOOL_SET_NUMBER *) cmd;
        emcStatus->io.tool.toolcompflag = emc_tool_set_number_msg->toolcompflag;
        if(emc_tool_set_number_msg->toolcompflag != 0)
            emcStatus->io.tool.toolcompvalue[emc_tool_set_number_msg->tool] =
                emc_tool_set_number_msg->toolcompvalue;
        //retval = emcToolSetNumber(emc_tool_set_number_msg->tool);
        if(emcStatus->task.parameters[4299] == 0)
            emcStatus->task.toolnow = emc_tool_set_number_msg->tool;
        break;

        // task commands

    case EMC_RESET_CMD_TYPE:  // HONG 2014-05-07
        emcInitM99Status();
        emcTaskAbort();
        emcIoAbort();
        emcSpindleAbort();
        spindlealreadyon = 0;
        mdi_execute_abort();

        iniLoad (EMC_INIFILE);
        //emcMotionInit();
        emcTaskInit();
        emcTaskSetMode (EMC_TASK_MODE_MANUAL);
        break;

    case EMC_TASK_INIT_TYPE:
        retval = emcTaskInit();
        break;

    case EMC_TASK_ABORT_TYPE:
        // abort everything

        flag_run_inwaiting = false;
        emcInitM99Status();
        interpStatusInit(); // add by hong
        emcTaskAbort(); //程序执行过程中点击退出，会执行一次这里，segment会打印222not paused，执行sqPause(sq); ghf-2014/5/28
        emcIoAbort();
        mdi_execute_abort();
        retval = 0;
        emcTaskPlanExecute("g40g49g92.1");
        break;

        // mode and state commands

    case EMC_TASK_SET_MODE_TYPE:
        mode_msg = (EMC_TASK_SET_MODE *) cmd;

        if (emcStatus->task.mode == EMC_TASK_MODE_AUTO
                && emcStatus->task.interpState != EMC_TASK_INTERP_IDLE
                && mode_msg->mode != EMC_TASK_MODE_AUTO) {
            emcOperatorError(0,
                    _(
                            "Can't switch mode while mode is AUTO and interpreter is not IDLE"));
        } else { // we can honour the modeswitch
            if (mode_msg->mode == EMC_TASK_MODE_MANUAL
                    && emcStatus->task.mode != EMC_TASK_MODE_MANUAL) { //从非手动切到手动
                    // leaving auto or mdi mode for manual

                /*! \todo FIXME-- duplicate code for abort,
                 also near end of main, when aborting on subordinate errors,
                 and in emcTaskExecute() */

                // abort motion
                emcTaskAbort();
                interpStatusInit(); // add by hong
                mdi_execute_abort();
                emcInitM99Status();
                // without emcTaskPlanClose(), a new run command resumes at
                // aborted line-- feature that may be considered later
                {
                    int was_open = taskplanopen;
                    emcTaskPlanClose();
                    if (EMC_DEBUG & EMC_DEBUG_INTERP && was_open) {
                        rcs_print("emcTaskPlanClose() called at %s:%d\n",
                        __FILE__, __LINE__);
                    }
                }

                // clear out the pending command
                emcTaskCommand = 0;
                interp_list.clear();

                emcStatus->task.currentLine = 0;
                // clear out the interpreter state
                emcStatus->task.interpState = EMC_TASK_INTERP_IDLE;
                emcStatus->task.execState = EMC_TASK_EXEC_DONE;
                stepping = 0;
                steppingWait = 0;

                // now queue up command to resynch interpreter
                emcTaskQueueCommand(&taskPlanSynchCmd);
            }
            retval = emcTaskSetMode(mode_msg->mode);
        }
        break;

    case EMC_TASK_SET_STATE_TYPE:
        state_msg = (EMC_TASK_SET_STATE *) cmd;
        retval = emcTaskSetState(state_msg->state);
        break;

        // interpreter commands

    case EMC_TASK_PLAN_OPEN_TYPE:
        open_msg = (EMC_TASK_PLAN_OPEN *) cmd;

        //unload the program
        if (open_msg->file[0] == 0) {
            emcStatus->task.file[0] = 0;
            retval = 0;
            break;
        }

        retval = emcTaskPlanOpen(open_msg->file);
        if (retval > INTERP_MIN_ERROR) {
            retval = -1;
        }

        if (-1 == retval) {
            emcOperatorError(0, _("can't open file:%s"), open_msg->file);
            emcStatus->task.file[0] = 0; //just in case of the file is not exist that it
        } else {                              //run a specify file u do not know
            char file[LINELEN];
            emcGetInterpFile(file);
            strcpy(emcStatus->task.file, file);
            printf("set file %s\n",emcStatus->task.file);
            retval = 0;
        }
        break;

    case EMC_TASK_PLAN_EXECUTE_TYPE:
        execute_msg = (EMC_TASK_PLAN_EXECUTE *) cmd;
        if (execute_msg->reset_flag == 1) {
            flag_mdi_run = true;
            emcTaskPlanSynch();
        }
        stepping = 0;
        steppingWait = 0;


        if (emcStatus->task.mode != EMC_TASK_MODE_MDI) {
            printf("Now I get MDI msg:%s \n",execute_msg->command);
            emcOperatorError(0, _("Must be in MDI mode to issue MDI command"));
            retval = -1;
            break;
        }
        if (execute_msg->command[0] != 0) {
            char * command = execute_msg->command;
            if (command[0] == (char) 0xff) {
                // Empty command recieved. Consider it is NULL
                command = NULL;
            }

            int level = emcTaskPlanLevel();
            if (emcStatus->task.mode == EMC_TASK_MODE_MDI) {
                interp_list.set_line_number(++pseudoMdiLineNumber);
                if (mdi_execute_level < 0)
                    mdi_execute_level = level;
            }

            execRetval = emcTaskPlanExecute(command, pseudoMdiLineNumber);

            level = emcTaskPlanLevel();

            if (emcStatus->task.mode == EMC_TASK_MODE_MDI) {
                if (mdi_execute_level == level) {
                    mdi_execute_level = -1;
                } else if (level > 0) {
                    // Still insude call. Need another execute(0) call
                    // but only if we didnt encounter an error
                    if (execRetval == INTERP_ERROR) {
                        mdi_execute_next = 0;
                    } else {
                        mdi_execute_next = 1;
                    }
                }
            }
            if (execRetval == INTERP_EXECUTE_FINISH) {
                // Flag MDI wait
                mdi_execute_wait = 1;
                // need to flush execution, so signify no more reading
                // until all is done
                emcTaskPlanSetWait();
                // and resynch the interpreter WM
                emcTaskQueueCommand(&taskPlanSynchCmd);
                // it's success, so retval really is 0
                retval = 0;
            } else if (execRetval != 0) {
                retval = -1;
            } else {
                // other codes are OK
                retval = 0;
            }

        }
        break;

    case EMC_TASK_PLAN_RUN_TYPE:
    {
        flag_pro_run = true;    //hong 2014-03-24
        flag_run_inwaiting = true;
        //if the file has been unload,return
        if (0 == emcStatus->task.file[0]) {
            emcOperatorError(0, _("cannot run without a specify file\n"));
            retval = 0;
            emcTaskSetMode(EMC_TASK_MODE_MANUAL);
            break;
        }

        if (!all_homed()) { //!no_force_homing = force homing before Auto
            emcOperatorError(0, _("Can't run a program when not homed"));
            retval = -1;
            break;
        }

        stepping = 0;
        steppingWait = 0;
        if (!taskplanopen && emcStatus->task.file[0] != 0) {
            retval = emcTaskPlanOpen(emcStatus->task.file);
            if (retval == -1) {
                emcOperatorError(0, "cannot run with this specify file\n");
                emcTaskSetMode(EMC_TASK_MODE_MANUAL);
                break;
            }
        }
        run_msg = (EMC_TASK_PLAN_RUN *) cmd;
        if (0 <= run_msg->line) {             //edit by hong 2014-05-31
            programStartLine = run_msg->line;
            emcStatus->task.runfromline = run_msg->line - 1;
        } else {
            programStartLine = 0;
            emcStatus->task.runfromline = -1;
            emcSetrunsub(true);
            if(-2 == run_msg->line)
                    runsubabortz = true;
            printf("now in set probe %s\n",emcStatus->task.file);
            if(NULL != strstr(emcStatus->task.file,"Probe") && NULL != strstr(emcStatus->task.file,"subNgc")) {
                printf("now in set probe\n");
                runprobe = true;
            }
        }
        emcStatus->task.runendline = run_msg->endline;
        emcStatus->task.interpState = EMC_TASK_INTERP_READING;
        emcStatus->task.task_paused = 0;

        emcsetRunStatus(emcRunsub()?0:1);

        //turn on the green before run
        setlightmsg.setlight = 1;
        setlightmsg.serial_number = -1;
        emcTaskQueueCommand(&setlightmsg);

        emcTaskPlanClearWait();  //clear the waitflag to ignore loodtool

        //hong 2014-06-24 flood on before run
        if (0 < coolanton_before_run && 0 <= run_msg->line) {
            if (!emcStatus->io.coolant.flood) {
                //emcCoolantFloodOn();
                emcTaskPlanExecute("m8");
            }
        }
        //hong 2014-06-06 start spindle before run,and caculate the delay time of the spindle in a good status
        if (0 < spindleon_before_run && 0 <= run_msg->line) {
            if (0 != emcStatus->motion.spindle.direction) {
                if (emcStatus->motion.spindle.lefttime > 0) {
                    char temp[50];
                    if(1 == g4delaytype) {
                        sprintf(temp, "g4p%.3f",emcStatus->motion.spindle.lefttime * 1000);
                    } else {
                        sprintf(temp, "g4p%.3f",emcStatus->motion.spindle.lefttime);
                    }
                    emcTaskPlanExecute(temp);
                    printf("now I set spindle lefttime is %.3f\n",emcStatus->motion.spindle.lefttime);
                }
            } else {
                runlinedelay = true;
                char temp[50];
                if(1 == g4delaytype) {
                    sprintf(temp, "m3s%.3f g4p%.3f", dfspindlespeed, spindle_delay * 1000);
                } else {
                    sprintf(temp, "m3s%.3f g4p%.3f", dfspindlespeed, spindle_delay);
                }
                emcTaskPlanExecute(temp);
            }
        }

        emcTaskPlanSynch();
        if (0 == emcGetOnlyReading() && !emcRunsub()) {
            if(emcStatus->task.parameters[4032] == 1) {
            char cmd[256];          
            sprintf(cmd,"G53G0Z%f",emcStatus->task.parameters[4088]);
            printf("now cmd : %s\n",cmd);
            emcTaskPlanExecute(cmd);
            }
        }
        int nostart = 0;
        if(1 == emcStatus->task.parameters[4037] &&
                emcStatus->task.runfromline > 0) {
            setSmartrunLine(emcStatus->task.runfromline);
            nostart = 1;
        }

        if (0 == emcGetOnlyReading() && !emcRunsub() && 0 == nostart) {
            if (taskplanopen)
                emcTaskPlanClose();
            emcTaskPlanOpen(substartfile);
            execstartngcflag = 1;
        }


        retval = 0;

        pro_run_time = etime();
//        if (emcStatus->task.handlewheel_enable) {
//            emcTrajPause();
//            if (emcStatus->task.interpState != EMC_TASK_INTERP_PAUSED) {
//                interpResumeState = emcStatus->task.interpState;
//            }
//            emcStatus->task.interpState = EMC_TASK_INTERP_PAUSED;
//            emcStatus->task.task_paused = 1;
//            retval = 0;
//        }
    }
        break;

    case EMC_TASK_PLAN_PAUSE_TYPE:

        if(emcStatus->task.currentLine < emcStatus->task.runfromline &&
        emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
        emcStatus->task.currentLine > 0) {
            retval = 0;
            break;
        }

        if(0 == emcStatus->motion.changezend) {
            changezSuspend = 2;
            retval = 0;
            break;
        }
        emcTrajPause();
        if (emcStatus->task.interpState != EMC_TASK_INTERP_PAUSED) {
            interpResumeState = emcStatus->task.interpState;
        }
        emcStatus->task.interpState = EMC_TASK_INTERP_PAUSED;
        emcStatus->task.task_paused = 1;
        retval = 0;
        break;

    case EMC_TASK_PLAN_OPTIONAL_STOP_TYPE:

        if (GET_OPTIONAL_PROGRAM_STOP() == ON) {
            if(emcStatus->task.currentLine < emcStatus->task.runfromline &&
            emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
            emcStatus->task.currentLine > 0) {
                retval = 0;
                break;
            }

            emcTrajPause();
            if (emcStatus->task.interpState != EMC_TASK_INTERP_PAUSED) {
                interpResumeState = emcStatus->task.interpState;
            }
            emcStatus->task.interpState = EMC_TASK_INTERP_PAUSED;
            emcStatus->task.task_paused = 1;
        }
        retval = 0;
        break;

    case EMC_TASK_SET_LEFTTIME_TYPE:
        //emcSetlefttime = (EMC_TASK_SET_LEFTTIME *) cmd;
        //emcWritetimeleft(emcSetlefttime->lefthours * 3600,1);
        //emcReadtimeleft();
        retval = 0;
        break;

    case EMC_TASK_SET_STARTLINE_TYPE:
       EMC_TASK_SET_STARTLINE *emcSetLine;
       emcSetLine = (EMC_TASK_SET_STARTLINE *) cmd;
       emcStatus->task.setstartline = emcSetLine->startline;
       retval = 0;
       break;

    case EMC_TASK_PLAN_RESUME_TYPE:
        emcTrajResume();
        emcStatus->task.interpState =
                (enum EMC_TASK_INTERP_ENUM) interpResumeState;
        emcStatus->task.task_paused = 0;
        stepping = 0;
        steppingWait = 0;
        retval = 0;
        break;

    case EMC_TASK_PLAN_END_TYPE:
        emcTaskAbort();
        retval = 0;
        break;

    case EMC_TASK_PLAN_INIT_TYPE:
        retval = emcTaskPlanInit();
        if (retval > INTERP_MIN_ERROR) {
            retval = -1;
        }
        break;

    case EMC_TASK_PLAN_SYNCH_TYPE:
        printf("now I in run synch cmd\n");
        retval = emcTaskPlanSynch();
        if (retval > INTERP_MIN_ERROR) {
            retval = -1;
        }
        break;

    case EMC_TASK_PLAN_NULL_TYPE:
        printf("now I get in null event to wait\n");
        retval = 0;
        break;

    case EMC_TASK_PLAN_SET_OPTIONAL_STOP_TYPE:
        os_msg = (EMC_TASK_PLAN_SET_OPTIONAL_STOP *) cmd;
        emcTaskPlanSetOptionalStop(os_msg->state);
        retval = 0;
        break;

    case EMC_TASK_PLAN_SET_BLOCK_DELETE_TYPE:
        bd_msg = (EMC_TASK_PLAN_SET_BLOCK_DELETE *) cmd;
        emcTaskPlanSetBlockDelete(bd_msg->state);
        retval = 0;
        break;

    case EMC_M99_CMD_TYPE:
        if (0 == emcGetOnlyReading() && !emcRunsub()) {
            for(int i=0;i<finishsize;i++) {
                emcTaskPlanExecute(finishcmd[i]);
            }
            if (taskplanopen)
                emcTaskPlanClose();
            emcTaskPlanOpen(substartfile);
            execstartngcflag = 1;
        }
        emcInitM99Status();
        emcTaskQueueCommand(&taskPlanSynchCmd);
        emcTaskPlanSetWait();   //we need this to wait for the last finish
         emcSetparameter(4040,1,0); //reset the m99 flag
         //add by hong on 2016-01-29 use for flush the tblfile after setting value in running status
         if(emcStatus->task.needreloadtool > 0) {
             emcStatus->task.needreloadtool = 0;
             emcToolLoadToolTable(tblfile);
         }
        emcStatus->task.interpState = EMC_TASK_INTERP_READING;
        if (!emcRunsub()) {
            emcStatus->task.work_piece++;
            if (emcStatus->task.work_piece > 1000)
                emcStatus->task.work_piece = 0;
        }
        retval = 0;
        printf("in exec m99\n");
        break;

    case EMC_PRO_VERIFY_TYPE:

        //if the file has been unload,return
        if (0 == emcStatus->task.file[0]) {
            emcOperatorError(0, _("cannot run without a specify file\n"));
            retval = 0;
            emcTaskSetMode(EMC_TASK_MODE_MANUAL);
            break;
        }
        if (!taskplanopen && emcStatus->task.file[0] != 0) {
            retval = emcTaskPlanOpen(emcStatus->task.file);
            if (retval == -1) {
                emcOperatorDisplay(0, "cannot run with this specify file\n");
                break;
            }
        }

        emcProVerifymsg = (EMC_PRO_VERIFY_CMD *) cmd;
        emcSetOnlyReading(emcProVerifymsg->verify_flag);
        printf("now emcverify flag is %d\n",emcProVerifymsg->verify_flag);

        stepping = 0;
        steppingWait = 0;

        programStartLine = 0;
        emcStatus->task.interpState = EMC_TASK_INTERP_VERIFY;
        emcStatus->task.task_paused = 0;
        retval = 0;
        break;

    case EMC_DOUBLEZ_CMD_TYPE:

        break;

case EMC_JOGWHEELS_GUIDE_CMD_TYPE: //ghf-2014/3/11
    emcJogwheels_guidemsg = (EMC_JOGWHEELS_GUIDE_CMD *) cmd;
    emcjogwheels_guide(emcJogwheels_guidemsg->jogwheels_guide);
    if (emcJogwheels_guidemsg->jogwheels_guide == '0')
        emcStatus->task.handlewheel_enable = false;
    else if (emcJogwheels_guidemsg->jogwheels_guide == '1')
        emcStatus->task.handlewheel_enable = true;
    retval = 0;
    break;

case EMC_SETGMODE_CMD_TYPE: //ghf-2014/3/12
    emcSetgmodemsg = (EMC_SETGMODE_CMD *) cmd;
    emcsetgmode(&(emcSetgmodemsg->gmode0), &(emcSetgmodemsg->gmode1),165,105,1,1,1);
    retval = 0;
    break;

case EMC_SETLIGHT_CMD_TYPE: //ghf-2014/3/17
    emcSetlightmsg = (EMC_SETLIGHT_CMD *) cmd;
    emcsetlight(emcSetlightmsg->setlight);
    retval = 0;
    break;

case EMC_ENCRYPT_CMD_TYPE: //ghf-2014/3/28
    emcEncryptmsg = (EMC_ENCRYPT_CMD *) cmd;
    //emcencrypt(emcEncryptmsg->encryptbuf,1000,100);
    retval = 0;
    break;

case EMC_FILTERTIME_CMD_TYPE: //ghf-2014/5/6
    emcFiltertimemsg = (EMC_FILTERTIME_CMD *) cmd;
    emcfiltertime(emcFiltertimemsg->filtertimebuf);
    retval = 0;
    break;

case EMC_TASK_EXECUTE_BLOCK_TYPE: //hong-2014/05/24
    emcExecuteblockmsg = (EMC_TASK_EXECUTE_BLOCK *) cmd;
    if (emcStatus->task.mode == EMC_TASK_MODE_MANUAL) {
        retval = -1;
        emcOperatorError(0, _("can not execute M code %s in manual"),
                emcExecuteblockmsg->command);
    }
    if (emcExecuteblockmsg->command[0] != 0)
        emcTaskPlanExecute(emcExecuteblockmsg->command);
    retval = 0;
    break;

case EMC_TASK_AUTOREPEAT_PAR_TYPE:  //hong 2014-06-24
    emcAutorepeatmsg = (EMC_TASK_AUTOREPEAT_PAR *) cmd;
    emcStatus->task.autorepeat = emcAutorepeatmsg->enable;
    retval = 0;
    break;

case EMC_TASK_FLUSH_PAR_TYPE: //hong-2014/05/24
    flushparmsg = (EMC_TASK_FLUSH_PAR *) cmd;
    if(emcStatus->status == RCS_DONE && emcStatus->task.status == RCS_DONE) {
        emcMotionInit();
        usleep(1000*180);
    } else {
        flushmotionini = true;
    }
    emcFlushpar();
    usleep(1000*170);
    iniFlush(EMC_INIFILE);

    retval = 0;
    break;

default:
    // unrecognized command
    if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
        rcs_print_error("ignoring issue of unknown command %d:%s\n",
                (int) cmd->type, emc_symbol_lookup(cmd->type));
    }
    retval = 0;		// don't consider this an error
    break;
    }

    if (retval == -1) {
        if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
            rcs_print_error("error executing command %d:%s\n", (int) cmd->type,
                    emc_symbol_lookup(cmd->type));
        }
    }
    /* debug */
    if ((EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) && retval) {
        rcs_print("emcTaskIssueCommand() returning: %d\n", retval);
    }
    return retval;
}

/*
 emcTaskCheckPostconditions() is called for commands on the interp_list.
 Immediate commands, i.e., commands sent from calls to emcTaskIssueCommand()
 in emcTaskPlan() directly, are not handled here.

 The return value is a state for emcTaskExecute() to wait on, e.g.,
 EMC_TASK_EXEC_WAITING_FOR_MOTION, after the command has finished and
 before any other commands can be sent out.
 */
static int emcTaskCheckPostconditions(NMLmsg * cmd) {
    if (0 == cmd) {
        return EMC_TASK_EXEC_DONE;
    }

    switch (cmd->type) {
    case EMC_OPERATOR_ERROR_TYPE:
    case EMC_OPERATOR_TEXT_TYPE:
    case EMC_OPERATOR_DISPLAY_TYPE:
    case EMC_NULLEVENT_TYPE:
        return EMC_TASK_EXEC_DONE;
        break;

    case EMC_SYSTEM_CMD_TYPE:
        return EMC_TASK_EXEC_WAITING_FOR_SYSTEM_CMD;
        break;

    case EMC_TRAJ_MINLINEAR_MOVE_TYPE:
    case EMC_TRAJ_LINEAR_MOVE_TYPE:
    case EMC_TRAJ_CIRCULAR_MOVE_TYPE:
    case EMC_TRAJ_SET_VELOCITY_TYPE:
    case EMC_TRAJ_SET_ACCELERATION_TYPE:
    case EMC_TRAJ_SET_TERM_COND_TYPE:
    case EMC_TRAJ_SET_SPINDLESYNC_TYPE:
    case EMC_TRAJ_SET_OFFSET_TYPE:
    case EMC_TRAJ_SET_G5X_TYPE:
    case EMC_TRAJ_SET_G92_TYPE:
    case EMC_TRAJ_SET_ROTATION_TYPE:
    case EMC_TRAJ_PROBE_TYPE:
    case EMC_TRAJ_RIGID_TAP_TYPE:
    case EMC_TRAJ_CLEAR_PROBE_TRIPPED_FLAG_TYPE:
    case EMC_TRAJ_SET_TELEOP_ENABLE_TYPE:
    case EMC_TRAJ_SET_TELEOP_VECTOR_TYPE:
    case EMC_TRAJ_SET_FO_ENABLE_TYPE:
    case EMC_TRAJ_SET_FH_ENABLE_TYPE:
    case EMC_TRAJ_SET_SO_ENABLE_TYPE:
    case EMC_TRAJ_SET_G0_ENABLE_TYPE:
        return EMC_TASK_EXEC_DONE;
        break;

    case EMC_TOOL_PREPARE_TYPE:
    case EMC_TOOL_LOAD_TYPE:
    case EMC_TOOL_UNLOAD_TYPE:
    case EMC_TOOL_LOAD_TOOL_TABLE_TYPE:
    case EMC_TOOL_SET_OFFSET_TYPE:
    case EMC_TOOL_SET_NUMBER_TYPE:
    case EMC_SPINDLE_SPEED_TYPE:
    case EMC_SPINDLE_ON_TYPE:
    case EMC_SPINDLE_OFF_TYPE:
    case EMC_COOLANT_MIST_ON_TYPE:
    case EMC_COOLANT_MIST_OFF_TYPE:
    case EMC_COOLANT_FLOOD_ON_TYPE:
    case EMC_COOLANT_FLOOD_OFF_TYPE:
    case EMC_LUBE_ON_TYPE:
    case EMC_LUBE_OFF_TYPE:
    case EMC_TASK_PLAN_NULL_TYPE:
        return EMC_TASK_EXEC_DONE;
        break;

    case EMC_TASK_PLAN_RUN_TYPE:
    case EMC_TASK_PLAN_PAUSE_TYPE:
    case EMC_TASK_PLAN_END_TYPE:
    case EMC_TASK_PLAN_INIT_TYPE:
    case EMC_TASK_PLAN_SYNCH_TYPE:
    case EMC_TASK_PLAN_EXECUTE_TYPE:
    case EMC_TASK_PLAN_OPTIONAL_STOP_TYPE:
    case EMC_TASK_SET_SIGNAL_TYPE:
    case EMC_TASK_SET_PARAMETER_TYPE:
    case EMC_TASK_ABORT_TYPE:
        return EMC_TASK_EXEC_DONE;
        break;

    case EMC_TRAJ_DELAY_TYPE:
    case EMC_AUX_INPUT_WAIT_TYPE:
    case EMC_TASK_WAIT_FOR_SIGNAL_TYPE: //hong-2014/09/23
        return EMC_TASK_EXEC_WAITING_FOR_DELAY;
        break;

    case EMC_MOTION_SET_EXTERN_OUT_TYPE:
    case EMC_MOTION_SET_AOUT_TYPE:
    case EMC_MOTION_SET_DOUT_TYPE:
    case EMC_MOTION_ADAPTIVE_TYPE:
        return EMC_TASK_EXEC_DONE;
        break;

    default:
        // unrecognized command
        if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
            rcs_print_error("postconditions: unrecognized command %d:%s\n",
                    (int) cmd->type, emc_symbol_lookup(cmd->type));
        }
        return EMC_TASK_EXEC_DONE;
        break;
    }
    return EMC_TASK_EXEC_DONE; // unreached
}

/*
 STEPPING_CHECK() is a macro that prefaces a switch-case with a check
 for stepping. If stepping is active, it waits until the step has been
 given, then falls through to the rest of the case statement.
 */

#define STEPPING_CHECK()                                                   \
if (stepping) {                                                            \
  if (! steppingWait) {                                                    \
    steppingWait = 1;                                                      \
    steppedLine = emcStatus->task.currentLine;                             \
  }                                                                        \
  else {                                                                   \
    if (emcStatus->task.currentLine != steppedLine) {                      \
      break;                                                               \
    }                                                                      \
  }                                                                        \
}

// executor function
static int emcTaskExecute(void) {
    int retval = 0;
    int status;			// status of child from EMC_SYSTEM_CMD
    pid_t pid;			// pid returned from waitpid()

    // first check for an abandoned system command and abort it
    if (emcSystemCmdPid != 0
            && emcStatus->task.execState
                    != EMC_TASK_EXEC_WAITING_FOR_SYSTEM_CMD) {
        if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
            rcs_print("emcSystemCmd: abandoning process %d\n", emcSystemCmdPid);
        }
        kill(emcSystemCmdPid, SIGINT);
        emcSystemCmdPid = 0;
    }


    if (emcGetOnlyReading() > 0)
        return retval;


    switch (emcStatus->task.execState) {
    case EMC_TASK_EXEC_ERROR:

        /*! \todo FIXME-- duplicate code for abort,
         also near end of main, when aborting on subordinate errors,
         and in emcTaskIssueCommand() */

        // abort everything
        emcTaskAbort();
        emcIoAbort();
        emcSpindleAbort();
        mdi_execute_abort();
        interpStatusInit(); // add by hong

        spindlealreadyon = 0;

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

        // clear out pending command
        emcTaskCommand = 0;
        interp_list.clear();

        emcStatus->task.currentLine = 0;
        // clear out the interpreter state
        emcStatus->task.interpState = EMC_TASK_INTERP_IDLE;
        emcStatus->task.execState = EMC_TASK_EXEC_DONE;
        stepping = 0;
        steppingWait = 0;

        // now queue up command to resynch interpreter
        emcTaskQueueCommand(&taskPlanSynchCmd);

        retval = -1;
        break;

    case EMC_TASK_EXEC_DONE:
        STEPPING_CHECK()
        ;

        //printf("now I get here wait for mode:%d queue:%d interpState:%d end:%d %d\n",emcStatus->task.mode,emcStatus->motion.traj.queue,emcStatus->task.interpState,(emcTaskCommand == 0),emcTaskPlanIsWait());
        if (emcStatus->task.mode != EMC_TASK_MODE_AUTO
                || (emcStatus->motion.traj.queue < (9000 - maxlinearnum) &&
                    emcStatus->task.mode == EMC_TASK_MODE_AUTO)
                && emcStatus->task.interpState != EMC_TASK_INTERP_PAUSED) {
            if (0 == emcTaskCommand) {
                // need a new command
                emcTaskCommand = interp_list.get();
                //printf("now get command %d %d\n",emcTaskCommand==0,interp_list.len());
                // interp_list now has line number associated with this-- get
                // it
                if (0 != emcTaskCommand) {
                    //printf("now type is %ld %d\n",
                    //      emcTaskCommand->type,interp_list.len());
                    emcTaskEager = 1;
                    emcStatus->task.currentLine = interp_list.get_line_number();

                    // and set it for all subsystems which use queued ids

                    emcTrajSetMotionId(emcStatus->task.currentLine);
                    if (emcStatus->motion.traj.queueFull) {
                        emcStatus->task.execState =
                                EMC_TASK_EXEC_WAITING_FOR_MOTION_QUEUE;
                    } else {
                        emcStatus->task.execState =
                                (enum EMC_TASK_EXEC_ENUM) emcTaskCheckPreconditions(
                                        emcTaskCommand);
                    }
                }
            } else {
                // have an outstanding command

                if (0 != emcTaskIssueCommand(emcTaskCommand)) {
                    emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
                    retval = -1;
                } else {
                    emcStatus->task.execState =
                            (enum EMC_TASK_EXEC_ENUM) emcTaskCheckPostconditions(
                                    emcTaskCommand);
                    emcTaskEager = 1;
                }
                emcTaskCommand = 0;	// reset it
            }
        }
        break;

    case EMC_TASK_EXEC_WAITING_FOR_MOTION_QUEUE:
        STEPPING_CHECK()
        ;
        if (!emcStatus->motion.traj.queueFull) {
            if (0 != emcTaskCommand) {
                emcStatus->task.execState =
                        (enum EMC_TASK_EXEC_ENUM) emcTaskCheckPreconditions(
                                emcTaskCommand);
                emcTaskEager = 1;
            } else {
                emcStatus->task.execState = EMC_TASK_EXEC_DONE;
                emcTaskEager = 1;
            }
        }
        break;

    case EMC_TASK_EXEC_WAITING_FOR_PAUSE:
        STEPPING_CHECK()
        ;
        if (emcStatus->task.interpState != EMC_TASK_INTERP_PAUSED) {
            if (0 != emcTaskCommand) {
                if (emcStatus->motion.traj.queue > 0) {
                    emcStatus->task.execState =
                            EMC_TASK_EXEC_WAITING_FOR_MOTION_QUEUE;
                } else {
                    emcStatus->task.execState =
                            (enum EMC_TASK_EXEC_ENUM) emcTaskCheckPreconditions(
                                    emcTaskCommand);
                    emcTaskEager = 1;
                }
            } else {
                emcStatus->task.execState = EMC_TASK_EXEC_DONE;
                emcTaskEager = 1;
            }
        }
        break;

    case EMC_TASK_EXEC_WAITING_FOR_MOTION:
        STEPPING_CHECK()
        ;
        if (emcStatus->motion.status == RCS_ERROR) {
            // emcOperatorError(0, "error in motion controller");
            emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
        } else if (emcStatus->motion.status == RCS_DONE) {
            emcStatus->task.execState = EMC_TASK_EXEC_DONE;
            emcTaskEager = 1;
        }
        break;

    case EMC_TASK_EXEC_WAITING_FOR_IO:
        STEPPING_CHECK()
        ;
        if (emcStatus->io.status == RCS_ERROR) {
            // emcOperatorError(0, "error in IO controller");
            emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
        } else if (emcStatus->io.status == RCS_DONE) {
            emcStatus->task.execState = EMC_TASK_EXEC_DONE;
            emcTaskEager = 1;
        }
        break;

    case EMC_TASK_EXEC_WAITING_FOR_MOTION_AND_IO:
        STEPPING_CHECK()
        ;

          if (emcStatus->motion.status == RCS_ERROR) {
            emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
        } else if (emcStatus->io.status == RCS_ERROR) {
            emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
        } else if (emcStatus->motion.status == RCS_DONE
                && emcStatus->io.status == RCS_DONE) {
                emcStatus->task.execState = EMC_TASK_EXEC_DONE;
                emcTaskEager = 1;
        }

        break;

    case EMC_TASK_EXEC_WAITING_FOR_DELAY:
        STEPPING_CHECK();

        // check if delay has passed
        emcStatus->task.delayLeft = taskExecDelayTimeout - etime();
        if (etime() >= taskExecDelayTimeout) {
            emcStatus->task.execState = EMC_TASK_EXEC_DONE;
            emcStatus->task.delayLeft = 0;
            if (emcStatus->task.input_timeout != 0)
                emcStatus->task.input_timeout = 1; // timeout occured
            emcTaskEager = 1;

            if(emcAuxInputWaitIndex == 10) {        //主轴定位完成信号
                emcOperatorError(0, "等待主轴定位完成信号超时");
                emcTaskAbort();
                emcAuxInputWaitIndex = -1;
                emcMotionSetDout(10,0,0,1);
            }
        }
        // delay can be also be because we wait for an input
        // if the index is set (not -1)
        if (emcAuxInputWaitIndex >= 0) {
            switch (emcAuxInputWaitType) {
            case WAIT_MODE_HIGH:
                if (emcStatus->motion.synch_di[emcAuxInputWaitIndex] != 0) {
                    emcStatus->task.input_timeout = 0; // clear timeout flag
                    emcAuxInputWaitIndex = -1;
                    emcStatus->task.execState = EMC_TASK_EXEC_DONE;
                    emcStatus->task.delayLeft = 0;
                }
                break;

            case WAIT_MODE_RISE:
                if (emcStatus->motion.synch_di[emcAuxInputWaitIndex] == 0) {
                    emcAuxInputWaitType = WAIT_MODE_HIGH;
                }
                break;

            case WAIT_MODE_LOW:
                if (emcStatus->motion.synch_di[emcAuxInputWaitIndex] == 0) {
                    emcStatus->task.input_timeout = 0; // clear timeout flag
                    emcAuxInputWaitIndex = -1;
                    emcStatus->task.execState = EMC_TASK_EXEC_DONE;
                    emcStatus->task.delayLeft = 0;
                }
                break;

            case WAIT_MODE_FALL: //FIXME: implement different fall mode if needed
                if (emcStatus->motion.synch_di[emcAuxInputWaitIndex] != 0) {
                    emcAuxInputWaitType = WAIT_MODE_LOW;
                }
                break;

            case WAIT_MODE_IMMEDIATE:
                emcStatus->task.input_timeout = 0; // clear timeout flag
                emcAuxInputWaitIndex = -1;
                emcStatus->task.execState = EMC_TASK_EXEC_DONE;
                emcStatus->task.delayLeft = 0;
                break;

            case WAIT_MODE_SIGNAL:
                if (emcStatus->task.waitsignal == 0) {
                    emcStatus->task.input_timeout = 0; // clear timeout flag
                    emcAuxInputWaitIndex = -1;
                    emcStatus->task.execState = EMC_TASK_EXEC_DONE;
                    emcStatus->task.delayLeft = 0;
                }
                break;

            default:
                emcOperatorError(0, "Unknown Wait Mode");
            }
        }
        break;

    case EMC_TASK_EXEC_WAITING_FOR_SYSTEM_CMD:
        STEPPING_CHECK()
        ;

        // if we got here without a system command pending, say we're done
        if (0 == emcSystemCmdPid) {
            emcStatus->task.execState = EMC_TASK_EXEC_DONE;
            break;
        }
        // check the status of the system command
        pid = waitpid(emcSystemCmdPid, &status, WNOHANG);

        if (0 == pid) {
            // child is still executing
            break;
        }

        if (-1 == pid) {
            // execution error
            if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
                rcs_print("emcSystemCmd: error waiting for %d\n",
                        emcSystemCmdPid);
            }
            emcSystemCmdPid = 0;
            emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
            break;
        }

        if (emcSystemCmdPid != pid) {
            // somehow some other child finished, which is a coding error
            if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
                rcs_print(
                        "emcSystemCmd: error waiting for system command %d, we got %d\n",
                        emcSystemCmdPid, pid);
            }
            emcSystemCmdPid = 0;
            emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
            break;
        }
        // else child has finished
        if (WIFEXITED(status)) {
            if (0 == WEXITSTATUS(status)) {
                // child exited normally
                emcSystemCmdPid = 0;
                emcStatus->task.execState = EMC_TASK_EXEC_DONE;
                emcTaskEager = 1;
            } else {
                // child exited with non-zero status
                if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
                    rcs_print(
                            "emcSystemCmd: system command %d exited abnormally with value %d\n",
                            emcSystemCmdPid, WEXITSTATUS(status));
                }
                emcSystemCmdPid = 0;
                emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
            }
        } else if (WIFSIGNALED(status)) {
            // child exited with an uncaught signal
            if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
                rcs_print("system command %d terminated with signal %d\n",
                        emcSystemCmdPid, WTERMSIG(status));
            }
            emcSystemCmdPid = 0;
            emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
        } else if (WIFSTOPPED(status)) {
            // child is currently being traced, so keep waiting
        } else {
            // some other status, we'll call this an error
            emcSystemCmdPid = 0;
            emcStatus->task.execState = EMC_TASK_EXEC_ERROR;
        }
        break;

    default:
        // coding error
        if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
            rcs_print_error("invalid execState");
        }
        retval = -1;
        break;
    }
    return retval;
}

// called to allocate and init resources
static int emctask_startup() {
    double end;
    int good;

#define RETRY_TIME 10.0		// seconds to wait for subsystems to come up
#define RETRY_INTERVAL 1.0	// seconds between wait tries for a subsystem

    // get our status data structure
    emcStatus = new EMC_STAT;

    // get the NML command buffer
    if (!(EMC_DEBUG & EMC_DEBUG_NML)) {
        set_rcs_print_destination (RCS_PRINT_TO_NULL);	// inhibit diag
        // messages
    }
    end = RETRY_TIME;
    good = 0;
    do {
        if (NULL != emcCommandBuffer) {
            delete emcCommandBuffer;
        }
        emcCommandBuffer = new RCS_CMD_CHANNEL(emcFormat, "emcCommand", "emc",
                EMC_NMLFILE);
        if (emcCommandBuffer->valid()) {
            good = 1;
            break;
        }
        esleep(RETRY_INTERVAL);
        end -= RETRY_INTERVAL;
        if (done) {
            emctask_shutdown();
            exit(1);
        }
    } while (end > 0.0);
    set_rcs_print_destination (RCS_PRINT_TO_STDOUT);	// restore diag
    // messages
    if (!good) {
        rcs_print_error("can't get emcCommand buffer\n");
        return -1;
    }
    // get our command data structure
    emcCommand = emcCommandBuffer->get_address();
    // get the NML status buffer
    if (!(EMC_DEBUG & EMC_DEBUG_NML)) {
        set_rcs_print_destination (RCS_PRINT_TO_NULL);	// inhibit diag
        // messages
    }
    end = RETRY_TIME;
    good = 0;
    do {
        if (NULL != emcStatusBuffer) {
            delete emcStatusBuffer;
        }
        emcStatusBuffer = new RCS_STAT_CHANNEL(emcFormat, "emcStatus", "emc",
                EMC_NMLFILE);
        if (emcStatusBuffer->valid()) {
            good = 1;
            break;
        }
        esleep(RETRY_INTERVAL);
        end -= RETRY_INTERVAL;
        if (done) {
            emctask_shutdown();
            exit(1);
        }
    } while (end > 0.0);
    set_rcs_print_destination(RCS_PRINT_TO_STDOUT);	// restore diag
    // messages
    if (!good) {
        rcs_print_error("can't get emcStatus buffer\n");
        return -1;
    }

    if (!(EMC_DEBUG & EMC_DEBUG_NML)) {
        set_rcs_print_destination (RCS_PRINT_TO_NULL);	// inhibit diag
        // messages
    }
    end = RETRY_TIME;
    good = 0;
    do {
        if (NULL != emcErrorBuffer) {
            delete emcErrorBuffer;
        }
        emcErrorBuffer = new NML(nmlErrorFormat, "emcError", "emc",
                EMC_NMLFILE);
        if (emcErrorBuffer->valid()) {
            good = 1;
            break;
        }
        esleep(RETRY_INTERVAL);
        end -= RETRY_INTERVAL;
        if (done) {
            emctask_shutdown();
            exit(1);
        }
    } while (end > 0.0);
    set_rcs_print_destination(RCS_PRINT_TO_STDOUT);	// restore diag
    // messages
    if (!good) {
        rcs_print_error("can't get emcError buffer\n");
        return -1;
    }
    // get the timer
    if (!emcTaskNoDelay) {
        timer = new RCS_TIMER(EMC_TASK_CYCLE_TIME, "", "");
    }
    // initialize the subsystems

    // IO first

    if (!(EMC_DEBUG & EMC_DEBUG_NML)) {
        set_rcs_print_destination (RCS_PRINT_TO_NULL);	// inhibit diag
        // messages
    }
    end = RETRY_TIME;
    good = 0;
    do {
        if (0 == emcIoInit()) {
            good = 1;
            break;
        }
        esleep(RETRY_INTERVAL);
        end -= RETRY_INTERVAL;
        if (done) {
            emctask_shutdown();
            exit(1);
        }
    } while (end > 0.0);
    set_rcs_print_destination(RCS_PRINT_TO_STDOUT);	// restore diag
    // messages
    if (!good) {
        rcs_print_error("can't initialize IO\n");
        return -1;
    }

    end = RETRY_TIME;
    good = 0;
    do {
        if (0 == emcIoUpdate(&emcStatus->io)) {
            good = 1;
            break;
        }
        esleep(RETRY_INTERVAL);
        end -= RETRY_INTERVAL;
        if (done) {
            emctask_shutdown();
            exit(1);
        }
    } while (end > 0.0);
    if (!good) {
        rcs_print_error("can't read IO status\n");
        return -1;
    }
    // now motion

    end = RETRY_TIME;
    good = 0;
    do {
        if (0 == emcMotionInit()) {
            good = 1;
            break;
        }
        esleep(RETRY_INTERVAL);
        end -= RETRY_INTERVAL;
        if (done) {
            emctask_shutdown();
            exit(1);
        }
    } while (end > 0.0);
    if (!good) {
        rcs_print_error("can't initialize motion\n");
        return -1;
    }

    end = RETRY_TIME;
    good = 0;
    do {
        if (0 == emcMotionUpdate(&emcStatus->motion)) {
            good = 1;
            break;
        }
        esleep(RETRY_INTERVAL);
        end -= RETRY_INTERVAL;
        if (done) {
            emctask_shutdown();
            exit(1);
        }
    } while (end > 0.0);
    if (!good) {
        rcs_print_error("can't read motion status\n");
        return -1;
    }
    // now the interpreter
    if (0 != emcTaskPlanInit()) {
        rcs_print_error("can't initialize interpreter\n");
        return -1;
    }

    if (done) {
        emctask_shutdown();
        exit(1);
    }
    // now task
    if (0 != emcTaskInit()) {
        rcs_print_error("can't initialize task\n");
        return -1;
    }
    emcTaskUpdate(&emcStatus->task);

    return 0;
}

// called to deallocate resources
static int emctask_shutdown(void) {
    // shut down the subsystems
    if (0 != emcStatus) {
        emcTaskHalt();
        emcTaskPlanExit();
        emcMotionHalt();
        emcIoHalt();
    }
    // delete the timer
    if (0 != timer) {
        delete timer;
        timer = 0;
    }
    // delete the NML channels

    if (0 != emcErrorBuffer) {
        delete emcErrorBuffer;
        emcErrorBuffer = 0;
    }

    if (0 != emcStatusBuffer) {
        delete emcStatusBuffer;
        emcStatusBuffer = 0;
        emcStatus = 0;
    }

    if (0 != emcCommandBuffer) {
        delete emcCommandBuffer;
        emcCommandBuffer = 0;
        emcCommand = 0;
    }

    if (0 != emcStatus) {
        delete emcStatus;
        emcStatus = 0;
    }
    return 0;
}

static int iniLoad(const char *filename) {
    IniFile inifile;
    const char *inistring;
    char version[LINELEN], machine[LINELEN];
    double saveDouble;
    int saveInt;

    // open it
    if (inifile.Open(filename) == false) {
        return -1;
    }

    if (NULL != (inistring = inifile.Find("DEBUG", "DISPLAY"))) {
        // copy to global
        if (1 != sscanf(inistring, "%i", &EMC_DEBUG)) {
            EMC_DEBUG = 0;
        }
    } else {
        // not found, use default
        EMC_DEBUG = 0;
    }
    if (EMC_DEBUG & EMC_DEBUG_RCS) {
        // set_rcs_print_flag(PRINT_EVERYTHING);
        max_rcs_errors_to_print = -1;
    }

    if (EMC_DEBUG & EMC_DEBUG_VERSIONS) {
        if (NULL != (inistring = inifile.Find("VERSION", "EMC"))) {
            if (sscanf(inistring, "$Revision: %s", version) != 1) {
                strncpy(version, "unknown", LINELEN - 1);
            }
        } else {
            strncpy(version, "unknown", LINELEN - 1);
        }

        if (NULL != (inistring = inifile.Find("MACHINE", "EMC"))) {
            strncpy(machine, inistring, LINELEN - 1);
        } else {
            strncpy(machine, "unknown", LINELEN - 1);
        }
        rcs_print("task: machine: '%s'  version '%s'\n", machine, version);
    }

    if (NULL != (inistring = inifile.Find("NML_FILE", "EMC"))) {
        // copy to global
        strcpy(EMC_NMLFILE, inistring);
    } else {
        // not found, use default
    }

    saveInt = EMC_TASK_INTERP_MAX_LEN; //remember default or previously set value
    if (NULL != (inistring = inifile.Find("INTERP_MAX_LEN", "TASK"))) {
        if (1 == sscanf(inistring, "%d", &EMC_TASK_INTERP_MAX_LEN)) {
            if (EMC_TASK_INTERP_MAX_LEN <= 0) {
                EMC_TASK_INTERP_MAX_LEN = saveInt;
            }
        } else {
            EMC_TASK_INTERP_MAX_LEN = saveInt;
        }
    }

    if (NULL != (inistring = inifile.Find("RS274NGC_STARTUP_CODE", "EMC"))) {
        // copy to global
        strcpy(RS274NGC_STARTUP_CODE, inistring);
    } else {
        if (NULL
                != (inistring = inifile.Find("RS274NGC_STARTUP_CODE",
                        "RS274NGC"))) {
            // copy to global
            strcpy(RS274NGC_STARTUP_CODE, inistring);
        } else {
            // not found, use default
        }
    }
    saveDouble = EMC_TASK_CYCLE_TIME;
    EMC_TASK_CYCLE_TIME_ORIG = EMC_TASK_CYCLE_TIME;
    emcTaskNoDelay = 0;
    if (NULL != (inistring = inifile.Find("CYCLE_TIME", "TASK"))) {
        if (1 == sscanf(inistring, "%lf", &EMC_TASK_CYCLE_TIME)) {
            // found it
            // if it's <= 0.0, then flag that we don't want to
            // wait at all, which will set the EMC_TASK_CYCLE_TIME
            // global to the actual time deltas
            if (EMC_TASK_CYCLE_TIME <= 0.0) {
                emcTaskNoDelay = 1;
            }
        } else {
            // found, but invalid
            EMC_TASK_CYCLE_TIME = saveDouble;
            rcs_print(
                    "invalid [TASK] CYCLE_TIME in %s (%s); using default %f\n",
                    filename, inistring, EMC_TASK_CYCLE_TIME);
        }
    } else {
        // not found, using default
        rcs_print("[TASK] CYCLE_TIME not found in %s; using default %f\n",
                filename, EMC_TASK_CYCLE_TIME);
    }


    inifile.Close();

    return 0;
}

static int iniFlush(const char *filename) {
    IniFile inifile;
    const char *inistring;

    // open it
    if (inifile.Open(filename) == false) {
        return -1;
    }


    if (NULL != (inistring = inifile.Find("CHANGEZ_VEL", "TASK"))) {
        if (1 == sscanf(inistring, "%lf", &change_zaxis_vel)) {
        }
    } else {
        change_zaxis_vel = 55;
        emcOperatorError(0,"参数文件丢失，请重新启动");
        return -1;
    }

    if (NULL != (inistring = inifile.Find("STATIC_FOLLOWERROR", "TASK"))) {
        if (1 == sscanf(inistring, "%lf", &static_max_follow_err)) {
        }
    } else {
        static_max_follow_err = 0.005;
    }


    if (NULL != (inistring = inifile.Find("SPINDLE_DELAY", "TASK"))) {
        if (1 == sscanf(inistring, "%lf", &spindle_delay)) {
        }
    } else {
        spindle_delay = 7;
    }
    emcSetparameter(4071,spindle_delay,1);

    double spindle_stop_delay = 5;
    if (NULL != (inistring = inifile.Find("SPINDLE_STOP_DELAY", "TRAJ"))) {
        if (1 == sscanf(inistring, "%lf", &spindle_stop_delay)) {
        }
    } else {
        spindle_stop_delay = 5;
    }
    emcSetparameter(4072,spindle_stop_delay,1);

    if (NULL != (inistring = inifile.Find("SPON_BEFORE_RUN", "TASK"))) {
        if (1 == sscanf(inistring, "%d", &spindleon_before_run)) {
        }
    } else {
        spindleon_before_run = 0;
    }

    if (NULL != (inistring = inifile.Find("SPINDLE_SPEED_FLAG", "RS274NGC"))) {
        sMode = atoi(inistring);
    }

    if (NULL != (inistring = inifile.Find("COOL_ON_BEFORE_RUN", "TASK"))) {
        if (1 == sscanf(inistring, "%d", &coolanton_before_run)) {
        }
    } else {
        coolanton_before_run = 0;
    }

    static double fistspinspeed = 0;
    double tempspinspeed = 0;
    if (NULL != (inistring = inifile.Find("DEFAULT_SPINDLE_SPEED", "TRAJ"))) {
        sscanf(inistring, "%lf", &tempspinspeed);
        if(fabs(tempspinspeed - fistspinspeed) > 0.001) {
            dfspindlespeed = tempspinspeed;
            fistspinspeed = tempspinspeed;
        }
    } else {
        printf("get defaultspindle fail\n");
        dfspindlespeed = 50000;
    }

    if (NULL != (inistring = inifile.Find("SPINDLE_OFF_AFTER_ABORT", "TASK"))) {
        if (1 == sscanf(inistring, "%d", &abortSpindleoff)) {
        }
    } else {
        abortSpindleoff = 0;
    }

    if (NULL != (inistring = inifile.Find("COOL_OFF_AFTER_ABORT", "TASK"))) {
        if (1 == sscanf(inistring, "%d", &abortcooloff)) {
        }
    } else {
        abortcooloff = 0;
    }

    if (NULL != (inistring = inifile.Find("AXES", "RS274NGC"))) {
        if (1 == sscanf(inistring, "%d", &axes)) {
        }
    } else {
        axes = 3;
    }

    if(NULL != (inistring = inifile.Find("MAXZDIFF", "RS274NGC")))
    {
        maxzdiff = fabs(atof(inistring));
    }else{
        maxzdiff = 30;             //default value is 30
    }


    if (NULL != (inistring = inifile.Find("CHANGE_AND_STOP", "TASK"))) {
        changeSpindleAndStop=atoi(inistring);
    } else {
        changeSpindleAndStop = 1;
    }

    if (NULL != (inistring = inifile.Find("EXTERN_IO", "TASK"))) {
        checkexternIo=atoi(inistring);
    } else {
        checkexternIo = 0;
    }

    if (NULL != (inistring = inifile.Find("Z_MODE", "RS274NGC"))) {
        zmode=atoi(inistring);
        SET_ROTATION_ZMODE(zmode);
        emcSetparameter(4082,zmode,1);
    } else {
        zmode = 0;
    }

    if (NULL != (inistring = inifile.Find("PROGRAM_PREFIX", "DISPLAY"))) {
        strcpy((char *)subncpath,inistring);
    } else {
        strcpy((char *)subncpath,"/home/.config/subNgc/");
    }

    if (NULL != (inistring = inifile.Find("G4MODE", "RS274NGC"))) {
        g4delaytype=atoi(inistring);
        emcSetparameter(4084,(double)g4delaytype,1);
    } else {
        g4delaytype = 0;
        emcSetparameter(4084,0,1);
    }

    if (NULL != (inistring = inifile.Find("LINE_TOLERANCE", "RS274NGC"))) {
        emcSetparameter(4083,atof(inistring),1);
    } else {
        emcSetparameter(4083,0,1);
    }

    memset(startcmd,0,sizeof(startcmd));
    memset(finishcmd,0,sizeof(finishcmd));

    sprintf(substartfile,"%sstart.ngc",subncpath);
    int i=0;

    char subfinishfile[LINELEN];
    sprintf(subfinishfile,"%sfinish.ngc",subncpath);
    FILE *finishngc;
    i=0;
    if((finishngc = fopen(subfinishfile,"r")) != NULL) {
        while(fgets(finishcmd[i],256,finishngc) != NULL) {
            //printf("now getcmd %s \n",finishcmd[i]);
            i++;
        }
        finishsize = i;
        fclose(finishngc);
    }

    // close it
    inifile.Close();
    return 0;
}

void update_randtime(void)
{
    srand((int)(etime()+2468));
    emcStatus->randtime = 10000+rand()%10000; //[10000,20000],放在motion中，10ms-20ms
}
void update_randbuf(void)
{
    srand((int)(etime()+13579));
    emcStatus->randbuf = 100+rand()%50; //[100,150],放在预处理线程，100-150的地址之间的某一个为起始地址
}

/*
 syntax: a.out {-d -ini <inifile>} {-nml <nmlfile>} {-shm <key>}
 */
int main(int argc, char *argv[]) {
    int taskPlanError = 0;
    int taskExecuteError = 0;
    double startTime, endTime, deltaTime;
    //double lasttime=0,currenttime,maxtime=0;
    //static int flag = 0;

    //static int delaytime = 0;
    static int randnum=0;


    double minTime, maxTime;
    int ret;

    double currenttime = etime();

    bindtextdomain("linuxcnc", EMC2_PO_DIR);
    setlocale(LC_MESSAGES, "");
    setlocale(LC_CTYPE, "");
    textdomain("linuxcnc");

    currenttime = etime();

    // copy command line args
    Argc = argc;
    Argv = argv;

    // loop until done
    done = 0;
    // trap ^C
    signal(SIGINT, emctask_quit);
    // and SIGTERM (used by runscript to shut down)
    signal(SIGTERM, emctask_quit);

    // set print destination to stdout, for console apps
    set_rcs_print_destination (RCS_PRINT_TO_STDOUT);
    // process command line args
    if (0 != emcGetArgs(argc, argv)) {
        rcs_print_error("error in argument list\n");
        exit(1);
    }

    if (done) {
        emctask_shutdown();
        exit(1);
    }
    // initialize globals
    emcInitGlobals();

    if (done) {
        emctask_shutdown();
        exit(1);
    }

    // get configuration information
    iniLoad (EMC_INIFILE);

    if (done) {
        emctask_shutdown();
        exit(1);
    }
    // initialize everything
    if (0 != emctask_startup()) {
        emctask_shutdown();
        exit(1);
    }

    // set the default startup modes
    emcTaskSetState (EMC_TASK_STATE_ESTOP);
    emcTaskSetMode (EMC_TASK_MODE_MANUAL);

    // cause the interpreter's starting offset to be reflected
    emcTaskPlanInit();

    // reflect the initial value of EMC_DEBUG in emcStatus->debug
    emcStatus->debug = EMC_DEBUG;

    startTime = etime();	// set start time before entering loop;
    // it will be set at end of loop from now on
    minTime = DBL_MAX;		// set to value that can never be exceeded
    maxTime = 0.0;		// set to value that can never be underset

    srand((int)(etime()));
    randnum = 1000+rand()%1000; //[1000,2000]
    update_randtime();
    update_randbuf();


    emcAxisLoadComp(0, "/home/comp/X", 1);
    emcAxisLoadComp(1, "/home/comp/Y", 1);
    emcAxisLoadComp(2, "/home/comp/Z", 1);
    emcAxisLoadComp(6, "/home/comp/Y2", 1);


    emcFlushpar();
    iniFlush(EMC_INIFILE);

    if(axes == 3) {
        emcSetMultifunc(5,1,0);
    } else {
        emcSetMultifunc(5,0,0);
    }
    emcgetrtctime();//ghf-2015/1/6

    emcMotionUpdate(&emcStatus->motion);
    emcReadtimeleft();
    taskdecryptInfo(emcStatus->motion.encryptbuf);



    randnumcount = random()%1000;
    emcTaskPlanExecute("g40g49g92.1");
    emcTaskPlanExecute("g54");

    emcSetMultifunc(10,(int)emcStatus->task.parameters[4000],0);

    while (!done) {
        // read command
        if (0 != emcCommandBuffer->peek()) {
            // got a new command, so clear out errors
            taskPlanError = 0;
            taskExecuteError = 0;
        }
        // run control cycle
        if (0 != emcTaskPlan()) {
            taskPlanError = 1;
        }
        if (0 != emcTaskExecute()) {
            taskExecuteError = 1;
        }

        //get the verify flag
        emcStatus->task.only_reading = emcGetOnlyReading();

        // update subordinate status
        emcIoUpdate(&emcStatus->io);
        // emcMotionUpdate(&emcStatus->motion);
        // synchronize subordinate states
        if (emcStatus->io.aux.estop) {
            if (emcStatus->motion.traj.enabled) {
                emcTrajDisable();
                emcTaskAbort();
                emcIoAbort();
                emcSpindleAbort();
                spindlealreadyon = 0;
               // emcAxisUnhome(-1); // only those joints which are volatile_home
                mdi_execute_abort();
                emcTaskPlanSynch();
                interpStatusInit(); // add by hong
            }
            if (emcStatus->io.coolant.mist) {
                emcCoolantMistOff();
            }
            if (emcStatus->io.coolant.flood) {
                emcCoolantFloodOff();
            }
            if (emcStatus->io.lube.on) {
                emcLubeOff();
            }
            if (emcStatus->motion.spindle.enabled) {
                emcSpindleOff();
                spindlealreadyon = 0;
            }
//            if(2 == emcStatus->motion.doublez.whichz_flag) {//ghf-2014/8/18
//                clearDoublez();
//            }
            //printf("get here,estop!\n");
            //oldset_which_z = 0;//报跟随错误，就会急停，进到这里ghf-2014/5/6
            emcTaskSetMode(EMC_TASK_MODE_MANUAL); //ghf-2014/5/12,motion急停后切到手动模式
        }



        // handle RCS_STAT_MSG base class members explicitly, since this
        // is not an NML_MODULE and they won't be set automatically

        // do task
        emcStatus->task.command_type = emcCommand->type;
        emcStatus->task.echo_serial_number = emcCommand->serial_number;
        //hong get the mdi run status
        emcStatus->task.mdi_running = flag_mdi_run;
        //printf("status now = %d   %d\n",emcStatus->status,emcStatus->task.execState);
        // do top level
        emcStatus->command_type = emcCommand->type;
        emcStatus->echo_serial_number = emcCommand->serial_number;



//        printf("now mode is task:%d motion:%d io:%d\n",emcStatus->task.execState,
//               emcStatus->motion.status,emcStatus->io.status);
        if (taskPlanError || taskExecuteError
                || emcStatus->task.execState == EMC_TASK_EXEC_ERROR
                || emcStatus->motion.status == RCS_ERROR
                || emcStatus->io.status == RCS_ERROR
                || execerrorflag > 0) {
            emcStatus->status = RCS_ERROR;
            emcStatus->task.status = RCS_ERROR;

        } else if (!taskPlanError && !taskExecuteError
                && emcStatus->task.execState == EMC_TASK_EXEC_DONE
                && emcStatus->motion.status == RCS_DONE
                && emcStatus->io.status == RCS_DONE && interp_list.len() == 0
                && emcTaskCommand == 0
                && emcStatus->task.interpState == EMC_TASK_INTERP_IDLE) {
            emcStatus->status = RCS_DONE;
            emcStatus->task.status = RCS_DONE;
            if(flushmotionini) {
                emcMotionInit();
                flushmotionini = false;
            }
        } else {
            emcStatus->status = RCS_EXEC;
            emcStatus->task.status = RCS_EXEC;
        }

        if(emcStatus->status == RCS_DONE || emcStatus->status == RCS_ERROR) {
            emcStatus->task.runState = EMC_IDLE;
        } else if(emcStatus->status == RCS_EXEC) {
            if(emcStatus->task.interpState == EMC_TASK_INTERP_PAUSED) {
                emcStatus->task.runState = EMC_PAUSED;
            } else  {
                emcStatus->task.runState = EMC_RUNNING;
            }
        }

        static int clearz = 0;
        if(clearz != emcStatus->motion.changezend) {

            if(emcStatus->motion.changezend == 1) {
                if(changezSuspend == 1) {
                    emcTaskAbort();
                    emcIoAbort();
                    changezSuspend = 0;
                } else if(changezSuspend == 2) {
                    emcTrajPause();
                    if (emcStatus->task.interpState != EMC_TASK_INTERP_PAUSED) {
                        interpResumeState = emcStatus->task.interpState;
                    }
                    emcStatus->task.interpState = EMC_TASK_INTERP_PAUSED;
                    emcStatus->task.task_paused = 1;
                    changezSuspend = 0;
                }
            }
            clearz = emcStatus->motion.changezend;
        }

        static int zwaitcount = 0;
        if(waitforzchangeend == 1 && emcStatus->motion.changezend == 1) {
            zwaitcount++;
            if(zwaitcount > 50) {
                zwaitcount = 0;
                waitforzchangeend = 0;
            }
        }

    //ghf-2014/7/10,只要是导致运动停止的错误，就要重新回零
    if((emcStatus->motion.status == RCS_ERROR)
            &&(emcStatus->motion.traj.enabled == 0)){
       //emcAxisUnhome(-1);
       //ghf-2014/8/18
       if(2 == emcStatus->motion.doublez.whichz_flag) {
           clearDoublez();
       }
    }

    //pro running time
    if (flag_pro_run) {
        emcStatus->task.program_running_time = (int) (etime() - pro_run_time);
    } else
        emcStatus->task.program_running_time = 0;

    //hong 2015-02-26 abort while
    static int algorithmcheck = -1;
    if(algorithmcheck != emcStatus->motion.runcycleflag) {
        if (-1 == emcStatus->motion.runcycleflag) {
            emcTaskAbort();
            taskPlanError=1;//ghf-20150708
            emcOperatorError(0, _("Algorithm occure errors while running"));
        }
        algorithmcheck = emcStatus->motion.runcycleflag;
    }

    //hong 2014-03-24
    if ((emcStatus->status == RCS_DONE && (flag_mdi_run || flag_pro_run) )||
         emcStatus->status == RCS_ERROR) {
        static int abortcmd_flag = true;
        if ((flag_pro_run && abortcmd_flag && (!emcRunsub() || (runsubabortz && emcRunsub())) &&
                emcStatus->status == RCS_DONE) ||(runprobe && flag_pro_run && emcRunsub() &&
                                                  emcStatus->status != RCS_EXEC) ) { //autoRepeat will not in
            if(runprobe)
                emcTaskAbort(); //clear the probe errors
            emcAbortcmd();
            emcTaskPlanSynch();
            if(emcStatus->motion.synch_do[11] != 1)     //攻牙时主轴处于位置模式,不能抬刀
                emcAbortliftz();
            emcAbortstatus(abortSpindleoff, abortcooloff);
            abortcmd_flag = false;
            runsubabortz = false;
            runprobe = false;
            goto continue_here;
        }
        abortcmd_flag = true;
        if (emcStatus->status == RCS_DONE && flag_pro_run && emcStatus->task.light[1] > 0) {
            //turn on the yello after run
            emcsetlight(2);
        }
        if(execstartngcflag > 0) {
            execstartngcflag = 0;
        }
        emcTaskPlanSynch();
        //ghf-2014/6/14 // abort motion
        emcTaskAbort(); //程序执行完会执行一次这里，mdi时segment会打印222paused，不会执行sqClearQueue ghf-2014/6/14导致界面打印“the state is not the require state”,这个在runcycle中跑完了会执行sqClearQueue的，所以可以屏蔽
        mdi_execute_abort();
        emcInitM99Status();
        interpStatusInit(); // add by hong
        emcMotionSetDout(0,0,0,1);
        int was_open = taskplanopen;
        emcTaskPlanClose();

        if (EMC_DEBUG & EMC_DEBUG_INTERP && was_open) {
            rcs_print("emcTaskPlanClose() called at %s:%d\n",
            __FILE__, __LINE__);
        }
        // clear out the pending command
        emcTaskCommand = 0;
        interp_list.clear();
        // we need to flush after abort
//        SET_Z_DIFF(emcStatus->task.g5x_offset.tran.z-
//                   emcStatus->task.g5x_offset.a);
        emcStatus->task.currentLine = 0;
        // clear out the interpreter state
        emcStatus->task.interpState = EMC_TASK_INTERP_IDLE;
        emcStatus->task.execState = EMC_TASK_EXEC_DONE;
        emcStatus->task.runfromline = 0;
        stepping = 0;
        steppingWait = 0;
        emcsetRunStatus(0);
        // now queue up command to resynch interpreter
        //emcTaskQueueCommand(&taskPlanSynchCmd);
        //SET_ROTATION_DIS(0,0);

        do{
            static int count = 0;
            usleep(100000);
            emcMotionUpdate(&emcStatus->motion);
            count ++;
            if(count > 5) {
                count =0;
                break;
            }
        }while(emcStatus->motion.status != RCS_DONE);

        printf("finish exec \n");
        flag_pro_run = false;
        flag_run_inwaiting = false;
        flag_mdi_run = false;
        taskPlanError = 0;
        execerrorflag = 0;
        taskExecuteError = 0;
        emcSetrunsub(false); //hong -2014-05-31
        emcTaskSetMode(EMC_TASK_MODE_MANUAL);
    }
    continue_here:


    emcTimecount();
 //   toolchangeexec();

    static int startcheckfpga = -1;
    if(emcStatus->task.state == EMC_TASK_STATE_ON) {
        startcheckfpga = 0;
    }
    if(-1 != startcheckfpga)
        emcCheckIoCommulication();

    // wait on timer cycle, if specified, or calculate actual
    // interval if ini file says to run full out via
    // [TASK] CYCLE_TIME <= 0.0
    // emcTaskEager = 0;

    emcMotionUpdate(&emcStatus->motion);
    // update task-specific status
    emcTaskUpdate(&emcStatus->task);
        emcStatusBuffer->write(emcStatus);
    if (spindleDelaytimeout - etime() > 0)
        emcStatus->motion.spindle.lefttime = spindleDelaytimeout - etime();
    else
        emcStatus->motion.spindle.lefttime = 0;
    if (emcTaskNoDelay) {
        endTime = etime();
        deltaTime = endTime - startTime;
        if (deltaTime < minTime)
            minTime = deltaTime;
        else if (deltaTime > maxTime)
            maxTime = deltaTime;
        startTime = endTime;
    }

    if ((emcTaskNoDelay) || (emcTaskEager)) {
        emcTaskEager = 0;
    } else {
        timer->wait();
    }

}				// end of while (! done)

// clean up everything
emctask_shutdown();
/* debugging */
if (emcTaskNoDelay) {
    if (EMC_DEBUG & EMC_DEBUG_TASK_ISSUE) {
        rcs_print("cycle times (seconds): %f min, %f max\n", minTime,
                maxTime);
    }
}
// and leave
exit(0);
}

void clearDoublez()
{
    //zchangeflag = EMC_NO_CHANGE;
    emcSpindleIgnore(0);
    emcStatus->motion.doublez.z_changing = false;
    aux_m6_flag = -1;
    oldset_which_z = 0;
    doublez_enable = false;
    emcStatus->motion.doublez.whichz_flag = 0;
    //emcdoublez(0,1,0);
    emcClearChangezFlag(0);
}

//换刀结束
int toolchangeend() {
    printf("now I get end toolchange\n");
        if(1 == emcStatus->task.parameters[4303]) {

            //emcToolSetNumber(emc_tool_set_number_msg->tool);
            emcSetparameter(4301,emcStatus->task.parameters[4300],1);
            emcSetparameter(4303,0,1);
            emcSetparameter(4294,0,1);
            emcStatus->task.toolnow = (int)emcStatus->task.parameters[4300];
            //双Z轴模式设置当前的刀具号
            if(emcStatus->motion.doublez.whichz_flag == 0) {
                emcSetparameter(4288,(int)emcStatus->task.parameters[4300],1);
                emcSetparameter(4283,(int)emcStatus->task.parameters[4300],1);
            } else if(emcStatus->motion.doublez.whichz_flag == 1) {
                emcSetparameter(4289,(int)emcStatus->task.parameters[4300],1);
                emcSetparameter(4284,(int)emcStatus->task.parameters[4300],1);
            } else if(emcStatus->motion.doublez.whichz_flag == 2) {
                emcSetparameter(4288,(int)emcStatus->task.parameters[4300],1);
                emcSetparameter(4283,(int)emcStatus->task.parameters[4300],1);
                emcSetparameter(4289,(int)emcStatus->task.parameters[4300],1);
                emcSetparameter(4284,(int)emcStatus->task.parameters[4300],1);
            }

            //恢复第二限位
            if(emcStatus->task.parameters[4287] == 1) {
                emcSetparameter(4227,1,1);
                emcSetMultifunc(20,0,1);
            }
        } else if (2 == emcStatus->task.parameters[4303]) {
            printf("now I get end toolchange  22\n");
            emcSetparameter(4303,0,1);
            emcSetparameter(4294,0,1);
            printf("change tool fail !!!\n");
            //恢复第二限位
            if(emcStatus->task.parameters[4287] == 1) {
                emcSetparameter(4227,1,1);
                emcSetMultifunc(20,0,1);
            }
        }
    return 0;
}


int emcgetrtctime(void) {

    struct rtc_time tm;
    int fd = open("/dev/rtc0" , 0);//only_rd
    if (fd ==  -1) {
        rtapi_print("open /dev/rtc0 error\n");
        return -1;
    }
    int retval = ioctl(fd, RTC_RD_TIME, &tm);
    if (retval == -1) {
        rtapi_print("RTC_RD_TIME ioctl");
        return -1;
    }
    close(fd);

    printf("bios RTC time  :%d-%d-%d-%d-%d-%d\n",tm.tm_year,tm.tm_mon,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
    long nowsec = kernel_mktime(&tm);
    nowsec += 8*3600;       //时区问题，相差8小时

    struct rtc_time realrtc;
    secToRTCtime(nowsec,&realrtc);
    emcStatus->motion.Currentdate[0] = realrtc.tm_year;
    emcStatus->motion.Currentdate[1] = realrtc.tm_mon;
    emcStatus->motion.Currentdate[2] = realrtc.tm_mday;
    emcStatus->motion.Currentdate[3] = realrtc.tm_hour;
    emcStatus->motion.Currentdate[4] = realrtc.tm_min;
    emcStatus->motion.Currentdate[5] = realrtc.tm_sec;

}

int writeFlushTime() {
    unsigned char tmp[8];
    tmp[0] = emcStatus->motion.Currentdate[0] >> 8;
    tmp[1] = emcStatus->motion.Currentdate[0] & 0x00ff;
    tmp[2] = emcStatus->motion.Currentdate[1];
    tmp[3] = emcStatus->motion.Currentdate[2];
    tmp[4] = emcStatus->motion.Currentdate[3];
    tmp[5] = emcStatus->motion.Currentdate[4];
    writeRealData(0x0080,tmp,8);
    printf("更新刷新时间:%d-%d-%d  %d-%d-%d\n",emcStatus->motion.Currentdate[0],emcStatus->motion.Currentdate[1],emcStatus->motion.Currentdate[2],emcStatus->motion.Currentdate[3],emcStatus->motion.Currentdate[4],emcStatus->motion.Currentdate[5]);
}
