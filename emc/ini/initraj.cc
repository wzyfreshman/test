/********************************************************************
* Description: initraj.cc
*   INI file initialization for trajectory level
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

#include <stdio.h>		// NULL
#include <stdlib.h>		// atol()
#include <string.h>		// strlen()
#include <ctype.h>		// isspace()

#include "emc.hh"
#include "emcpos.h"             // EmcPose
#include "rcs_print.hh"
#include "posemath.h"		// PM_POSE, PM_RPY
#include "emcIniFile.hh"
#include "initraj.hh"		// these decls
#include "emcglb.h"		/*! \todo TRAVERSE_RATE (FIXME) */

static TGMODE g0;
static TGMODE g1;

/*
  loadTraj()

  Loads ini file params for traj

  AXES <int>                    number of axes in system
  LINEAR_UNITS <float>          units per mm
  ANGULAR_UNITS <float>         units per degree
  CYCLE_TIME <float>            cycle time for traj calculations
  DEFAULT_VELOCITY <float>      default velocity
  MAX_VELOCITY <float>          max velocity
  MAX_ACCELERATION <float>      max acceleration
  DEFAULT_ACCELERATION <float>  default acceleration
  HOME <float> ...              world coords of home, in X Y Z R P W

  calls:

  emcTrajSetAxes(int axes);
  emcTrajSetUnits(double linearUnits, double angularUnits);
  emcTrajSetCycleTime(double cycleTime);
  emcTrajSetVelocity(double vel);
  emcTrajSetAcceleration(double acc);
  emcTrajSetMaxVelocity(double vel);
  emcTrajSetMaxAcceleration(double acc);
  emcTrajSetHome(EmcPose home);
  */

static int loadTraj(EmcIniFile *trajInifile)
{
    const char *inistring;
    EmcLinearUnits linearUnits;
    EmcAngularUnits angularUnits;
    double vel,g0_maxvel,g1_maxvel,g0_maxjerk,g1_maxjerk,g0_maxnline;
    double acc,g0_maxacc,g1_maxacc,g1_maxnline,g1_maxcircle,g1_maxncircle;
    double angle1,angle2,coeff1,coeff2,coeff3;
    double angleAcc[6],angleVel[6];
    unsigned char g0_usefeedrate;
    unsigned char coordinateMark[6] = { 1, 1, 1, 0, 0, 0 };
    int t;
    int len;
    char homes[LINELEN];
    char home[LINELEN];
    EmcPose homePose = { {0.0, 0.0, 0.0}, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double d;

    trajInifile->EnableExceptions(EmcIniFile::ERR_CONVERSION);

    try{
	int axes = 0;
	int axismask = 0;
	const char *coord = trajInifile->Find("COORDINATES", "TRAJ");
	if(coord) {
	    if(strchr(coord, 'x') || strchr(coord, 'X')) axismask |= 1;
	    if(strchr(coord, 'y') || strchr(coord, 'Y')) axismask |= 2;
	    if(strchr(coord, 'z') || strchr(coord, 'Z')) axismask |= 4;
	    if(strchr(coord, 'a') || strchr(coord, 'A')) axismask |= 8;
	    if(strchr(coord, 'b') || strchr(coord, 'B')) axismask |= 16;
	    if(strchr(coord, 'c') || strchr(coord, 'C')) axismask |= 32;
	    if(strchr(coord, 'u') || strchr(coord, 'U')) axismask |= 64;
	    if(strchr(coord, 'v') || strchr(coord, 'V')) axismask |= 128;
	    if(strchr(coord, 'w') || strchr(coord, 'W')) axismask |= 256;
	} else {
	    axismask = 1 | 2 | 4;		// default: XYZ machine
	}
	trajInifile->Find(&axes, "AXES", "TRAJ");

        if (0 != emcTrajSetAxes(axes, axismask)) {
            if (EMC_DEBUG & EMC_DEBUG_CONFIG) {
                rcs_print("bad return value from emcTrajSetAxes\n");
            }
            return -1;
        }

        linearUnits = 1;
        trajInifile->FindLinearUnits(&linearUnits, "LINEAR_UNITS", "TRAJ");
        angularUnits = 0;
        trajInifile->FindAngularUnits(&angularUnits, "ANGULAR_UNITS", "TRAJ");

        if (0 != emcTrajSetUnits(linearUnits, angularUnits)) {
            if (EMC_DEBUG & EMC_DEBUG_CONFIG) {
                rcs_print("bad return value from emcTrajSetUnits\n");
            }
            return -1;
        }

        int zmode = 0,spindlemode = 0;
        trajInifile->Find(&zmode, "Z_MODE", "RS274NGC");
        trajInifile->Find(&spindlemode, "SPINDLE_MODE", "MOTO");
        emcSetSpindleMode(zmode,spindlemode);

        trajInifile->Find(&maxlinearnum, "MAXLINENUM", "TASK");
        if(0 >= maxlinearnum)
            maxlinearnum = 20;
        else if (500 < maxlinearnum)
            maxlinearnum = 500;

        g0_maxvel = 1.0;
        trajInifile->Find(&g0_maxvel, "G0_MAXVEL", "MOTO");

        g0_maxacc = 1000;
        trajInifile->Find(&g0_maxacc, "G0_MAXACC", "MOTO");

        g0_maxjerk = 50000;
        trajInifile->Find(&g0_maxjerk, "G0_MAXJERK", "MOTO");

        g0_maxnline = 300;
        trajInifile->Find(&g0_maxnline, "G0_MAXACC_NLINE", "MOTO");

        int temp = 1;
        trajInifile->Find(&temp, "G0_USE_FEEDRATE", "MOTO");
        if(0 == temp)
            g0_usefeedrate = 0;
        else
            g0_usefeedrate = 1;

        g1_maxvel = 1.0;
        trajInifile->Find(&g1_maxvel, "G1_MAXVEL", "MOTO");

        g1_maxacc = 1000;
        trajInifile->Find(&g1_maxacc, "G1_MAXACC", "MOTO");

        g1_maxjerk = 20000;
        trajInifile->Find(&g1_maxjerk, "G1_MAXJERK", "MOTO");

        g1_maxnline = 0;
        trajInifile->Find(&g1_maxnline, "G1_MAXACC_NLINE", "MOTO");

        g1_maxcircle = 3000;
        trajInifile->Find(&g1_maxcircle, "G1_MAXACC_CIRCLE", "MOTO");
        g1_maxcircle = g1_maxcircle*g1_maxcircle/5;//ghf-20150623

        g1_maxncircle = g1_maxcircle;
       // trajInifile->Find(&g1_maxncircle, "G1_MAXACC_NCIRCLE", "MOTO");

        for(int i = 0;i<6;i++) {
            angleAcc[i] = 800;
            angleVel[i] = 1200;
        }

//        if(g1_maxnline>1e-6)
        angleAcc[0] = g1_maxnline;
        angleAcc[1] = g1_maxnline;
        angleAcc[2] = g1_maxnline;

//        double factor;
//         trajInifile->Find(&factor,"ROTATE_RATE","TRAJ");
//         if(factor>1e-6)
//         {
//             angleAcc[3] = g1_maxnline/factor;
//             angleAcc[4] = g1_maxnline/factor;
//             angleAcc[5] = g1_maxnline/factor;
//         }
    //   trajInifile->Find(&angleAcc[0],"MAX_ACCELERATION","AXIS_3");
    //   trajInifile->Find(&angleAcc[1],"MAX_ACCELERATION","AXIS_4");
    //    trajInifile->Find(&angleAcc[2],"MAX_ACCELERATION","AXIS_5");
        trajInifile->Find(&angleAcc[3],"MAX_ACCNORMAL","AXIS_3");
        trajInifile->Find(&angleAcc[4],"MAX_ACCNORMAL","AXIS_4");
        trajInifile->Find(&angleAcc[5],"MAX_ACCNORMAL","AXIS_5");

        trajInifile->Find(&angleVel[0],"MAX_VELOCITY","AXIS_0");
        trajInifile->Find(&angleVel[1],"MAX_VELOCITY","AXIS_1");
        trajInifile->Find(&angleVel[2],"MAX_VELOCITY","AXIS_2");
        trajInifile->Find(&angleVel[3],"MAX_VELOCITY","AXIS_3");
        trajInifile->Find(&angleVel[4],"MAX_VELOCITY","AXIS_4");
        trajInifile->Find(&angleVel[5],"MAX_VELOCITY","AXIS_5");


        g0.maxAcc = g0_maxacc;
        g0.maxV = g0_maxvel;
        g0.maxJerk = g0_maxjerk;
        g0.maxAcc_nLine = g0_maxnline;


        g1.maxAcc = g1_maxacc;
        g1.maxV = g1_maxvel;
        g1.maxJerk = g1_maxjerk;
        g1.maxAcc_circle = g1_maxcircle;
        g1.maxAcc_nCircle = g1_maxncircle;
        g1.maxAcc_nLine = g1_maxnline;

        for(int i=0;i<6;i++) {
            g0.maxAngleAcc_n_Axis[i] = angleAcc[i];
            g0.maxAngleV_Axis[i] = angleVel[i];

            g1.maxAngleAcc_n_Axis[i] = angleAcc[i];
            g1.maxAngleV_Axis[i] = angleVel[i];
        }

        angle1 = 165;
        trajInifile->Find(&angle1,"ANGLE1","TRAJ");
        if(fabs(angle1 - 0.0) < 0.0001)
            angle1 = 165;
        angle2 = 105;
        trajInifile->Find(&angle2,"ANGLE2","TRAJ");
        if(fabs(angle2 - 0.0) < 0.0001)
            angle2 = 105;
        coeff1 = 1.0;
        trajInifile->Find(&coeff1,"COEFF1","TRAJ");
        if(fabs(coeff1 - 0.0) < 0.0001)
            coeff1 = 1.0;
        coeff2 = 0.2;
        trajInifile->Find(&coeff2,"COEFF2","TRAJ");
        if(fabs(coeff2 - 0.0) < 0.0001)
            coeff2 = 1.0;

        coeff3 = 1.0;//m54之间g0快速移动约定系数ghf-2014/11/10
        trajInifile->Find(&coeff3,"COEFF3","TRAJ");
        if(fabs(coeff3 - 0.0) < 0.0001)
            coeff3 = 1.0;

        emcsetgmode(&g0,&g1,angle1,angle2,coeff1,coeff2,coeff3);//ghf-2014/10/13
        emcTrajSetFOEnable(g0_usefeedrate);



        vel = 1.0;
        trajInifile->Find(&vel, "DEFAULT_VELOCITY", "TRAJ");

        // set the corresponding global
        TRAJ_DEFAULT_VELOCITY = vel; //此值没用，没进control ghf-2014/5/23

        // and set dynamic value
        if (0 != emcTrajSetVelocity(0, vel)) { //default velocity on startup 0，设置起始阶段的进给速度？
            if (EMC_DEBUG & EMC_DEBUG_CONFIG) {
                rcs_print("bad return value from emcTrajSetVelocity\n");
            }
            return -1;
        }

        /*
        vel = 1e99; // by default, use AXIS limit
        trajInifile->Find(&vel, "MAX_VELOCITY", "TRAJ");

        // set the corresponding global
        TRAJ_MAX_VELOCITY = vel;

        // and set dynamic value
        if (0 != emcTrajSetMaxVelocity(vel)) {
            if (EMC_DEBUG & EMC_DEBUG_CONFIG) {
                rcs_print("bad return value from emcTrajSetMaxVelocity\n");
            }
            return -1;
        }
        */

        acc = 1e99; // let the axis values apply
        trajInifile->Find(&acc, "DEFAULT_ACCELERATION", "TRAJ");
        
        //printf("DEFAULT_ACCELERATION=%f\n",acc);

        if (0 != emcTrajSetAcceleration(acc)) {
            if (EMC_DEBUG & EMC_DEBUG_CONFIG) {
                rcs_print("bad return value from emcTrajSetAcceleration\n");
            }
            return -1;
        }

        /*
        acc = 1e99; // let the axis values apply
        trajInifile->Find(&acc, "MAX_ACCELERATION", "TRAJ");

        if (0 != emcTrajSetMaxAcceleration(acc)) {
            if (EMC_DEBUG & EMC_DEBUG_CONFIG) {
                rcs_print("bad return value from emcTrajSetMaxAcceleration\n");
            }
            return -1;
        }        
        */

    }

    catch(EmcIniFile::Exception &e){
        e.Print();
        return -1;
    }

    // set coordinateMark[] to hold 1's for each coordinate present,
    // so that home position can be interpreted properly
    if ((inistring = trajInifile->Find("COORDINATES", "TRAJ")) != NULL) {
        len = strlen(inistring);
        // there's an entry in ini file, so clear all the marks out first
        // so that defaults don't apply at all
        for (t = 0; t < 6; t++) {
            coordinateMark[t] = 0;
        }
        // now set actual marks
        for (t = 0; t < len; t++) {
            if (inistring[t] == 'X')
                coordinateMark[0] = 1;
            else if (inistring[t] == 'Y')
                coordinateMark[1] = 1;
            else if (inistring[t] == 'Z')
                coordinateMark[2] = 1;
            else if (inistring[t] == 'R')
                coordinateMark[3] = 1;
            else if (inistring[t] == 'P')
                coordinateMark[4] = 1;
            else if (inistring[t] == 'W')
                coordinateMark[5] = 1;
        }
    }

    if (NULL != (inistring = trajInifile->Find("HOME", "TRAJ"))) {
	// found it, now interpret it according to coordinateMark[]
	strcpy(homes, inistring);
	len = 0;
	for (t = 0; t < 6; t++) {
	    if (!coordinateMark[t]) {
		continue;	// position t at index of next non-zero mark
	    }
	    // there is a mark, so read the string for a value
	    if (1 == sscanf(&homes[len], "%s", home) &&
		1 == sscanf(home, "%lf", &d)) {
		// got an entry, index into coordinateMark[] is 't'
		if (t == 0)
		    homePose.tran.x = d;
		else if (t == 1)
		    homePose.tran.y = d;
		else if (t == 2)
		    homePose.tran.z = d;
		else if (t == 3)
		    homePose.a = d;
		else if (t == 4)
		    homePose.b = d;
		else
		    homePose.c = d;

		// position string ptr past this value
		len += strlen(home);
		// and at start of next value
		while ((len < LINELEN) && (homes[len] == ' ' || homes[len] == '\t')) {
		    len++;
		}
		if (len >= LINELEN) {
		    break;	// out of for loop
		}
	    } else {
		// badly formatted entry
		rcs_print("invalid inifile value for [TRAJ] HOME: %s\n",
			  inistring);
                return -1;
	    }
	}			// end of for-loop on coordinateMark[]
    }

    if (0 != emcTrajSetHome(homePose)) {
	if (EMC_DEBUG & EMC_DEBUG_CONFIG) {
	    rcs_print("bad return value from emcTrajSetHome\n");
	}
	return -1;
    }

    int intpar[50];
    double doupar[50];
    for(int i=0;i<50;i++) {
        intpar[i] = 0;
        doupar[i] = 0;
    }

    int cornerlimit = 0;
    trajInifile->Find(&cornerlimit, "CORNERVELLIMIT", "TRAJ");
    intpar[10] = cornerlimit;

    trajInifile->Find(&intpar[0], "FILTER_CONST", "TRAJ");
    trajInifile->Find(&intpar[1], "CIRCLE_OMEGA_LIMIT", "TRAJ");
      trajInifile->Find(&intpar[2], "CONTROL_TYPE", "TRAJ");
    trajInifile->Find(&intpar[3], "POINT_FILTERED_CONST", "TRAJ");
    trajInifile->Find(&intpar[4], "CIRCLE_INSERT_FLAG", "TRAJ");
    trajInifile->Find(&intpar[5], "SMALL_SEG_MODE", "TRAJ");
    trajInifile->Find(&intpar[6], "INTERPOLATE_METHOD", "TRAJ");
    trajInifile->Find(&intpar[7], "PLOT_DATA", "TRAJ");

    trajInifile->Find(&intpar[11], "HANDLE_TYPE", "TRAJ");
    trajInifile->Find(&intpar[12], "HANDLE_DIRECTION", "TRAJ");
    trajInifile->Find(&intpar[13], "USE_ABCENCODER", "TRAJ");

    trajInifile->Find(&intpar[15], "AXES_TYPE", "EMCMOT");
    trajInifile->Find(&intpar[16], "ABC_MOVETYPE", "EMCMOT");


    trajInifile->Find(&doupar[2], "SEG_CIRCLE_TOLERANCE", "TRAJ");
    trajInifile->Find(&doupar[5], "CIRCLE_TOLERANCE_G0", "TRAJ");
    trajInifile->Find(&doupar[6], "CIRCLE_TOLERANCE_G1", "TRAJ");
    trajInifile->Find(&doupar[7], "OPTIMIZED_ANGLE", "TRAJ");
    trajInifile->Find(&doupar[8], "HANDLE_SCALE", "TRAJ");
    trajInifile->Find(&doupar[9], "ROTATE_RATE", "TRAJ");

    trajInifile->Find(&doupar[11], "MAX_SPINDLE_SPEED", "TRAJ");        //主轴最大转速
    trajInifile->Find(&doupar[12], "SPINDLE_ACC", "TRAJ");      //主轴加速度

    printf("now I get double 11 12 %.3f  %.3f\n",doupar[11],doupar[12]);
    emcMotSetPar(&intpar[0],&doupar[0]);



    return 0;
}

/*
  iniTraj(const char *filename)

  Loads ini file parameters for trajectory, from [TRAJ] section
 */
int iniTraj(const char *filename)
{
    EmcIniFile trajInifile;

    if (trajInifile.Open(filename) == false) {
	return -1;
    }
    // load trajectory values
    if (0 != loadTraj(&trajInifile)) {
	return -1;
    }

    return 0;
}
