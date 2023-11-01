/********************************************************************
* Description: trivkins.c
*   Trivial kinematics for 3 axis Cartesian machine
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

#include "kinematics.h"		/* these decls */


KINEMATICDATA kinedata;
int kinematicsForward(const double *joints,
		      EmcPose * pos,
		      const KINEMATICS_FORWARD_FLAGS * fflags,
		      KINEMATICS_INVERSE_FLAGS * iflags)
{
        pos->tran.x = joints[0];
        pos->tran.y = joints[1];
        pos->tran.z = joints[2];
        pos->a = joints[3];
        pos->b = joints[4];
        pos->c = joints[5];
        pos->u = joints[6];
        pos->v = joints[7];
        pos->w = joints[8];


    return 0;
}

int kinematicsInverse(const EmcPose * pos,
		      double *joints,
		      const KINEMATICS_INVERSE_FLAGS * iflags,
		      KINEMATICS_FORWARD_FLAGS * fflags)
{

        joints[0] = pos->tran.x;
        joints[1] = pos->tran.y;
        joints[2] = pos->tran.z;
        joints[3] = pos->a;
        joints[4] = pos->b;
        joints[5] = pos->c;
        joints[6] = pos->u;
        joints[7] = pos->v;
        joints[8] = pos->w;

//    printf("now in kinema type:%d xpos:%.3f ypos:%.3f zpos:%.3f a:%.3f\n",kinedata.method,pos->tran.y,joints[1],pos->tran.z,joints[3]);
    return 0;
}

/* implemented for these kinematics as giving joints preference */
int kinematicsHome(EmcPose * world,
		   double *joint,
		   KINEMATICS_FORWARD_FLAGS * fflags,
		   KINEMATICS_INVERSE_FLAGS * iflags)
{
    *fflags = 0;
    *iflags = 0;

    return kinematicsForward(joint, world, fflags, iflags);
}

KINEMATICS_TYPE kinematicsType()
{
    return KINEMATICS_IDENTITY;
}

#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */
#include "hal.h"

EXPORT_SYMBOL(kinematicsType);
EXPORT_SYMBOL(kinematicsForward);
EXPORT_SYMBOL(kinematicsInverse);
MODULE_LICENSE("GPL");

int comp_id;
int rtapi_app_main(void) {
    comp_id = hal_init("trivkins");
    if(comp_id > 0) {
	hal_ready(comp_id);
	return 0;
    }
    return comp_id;
}

void rtapi_app_exit(void) { hal_exit(comp_id); }

int reCaculatePos(EmcPose * pos,double *joints)
{
    double posin[EMCMOT_MAX_JOINTS],lastposin[EMCMOT_MAX_JOINTS];
    posin[0] = pos->tran.x;
    posin[1] = pos->tran.y;
    posin[2] = pos->tran.z;
    posin[3] = pos->a;
    posin[4] = pos->b;
    posin[5] = pos->c;

    lastposin[0] = kinedata.lastP.tran.x;
    lastposin[1] = kinedata.lastP.tran.y;
    lastposin[2] = kinedata.lastP.tran.z;
    lastposin[3] = kinedata.lastP.a;
    lastposin[4] = kinedata.lastP.b;
    lastposin[5] = kinedata.lastP.c;

    if(kinedata.method == 0) {
        joints[0] = pos->tran.x;
        joints[1] = pos->tran.y;
        joints[2] = pos->tran.z;
    } else if(kinedata.method == 1) {
        joints[0] = pos->tran.x;
        joints[1] = pos->tran.y - kinedata.a_length*sin(pos->a*2*PM_PI/360);
        joints[2] = pos->tran.z + kinedata.a_length*cos(pos->a*2*PM_PI/360) -kinedata.a_length;
    }

    //三个旋转轴处理
    int jnum=3;
    for(jnum=3;jnum<EMCMOT_MAX_JOINTS;jnum++) {
        if(kinedata.circle_type[jnum] > 0) {
            if(fabs(posin[jnum]) > 360) {
                    posin[jnum] = posin[jnum] - (int)posin[jnum]/360*360;
            }
            if(fabs(posin[jnum] - lastposin[jnum]) > 180) {
                kinedata.circle_count[jnum] += 1;
                printf("now I in here and set circlecount %d  %.3f  last.c %.3f\n",kinedata.circle_count[jnum],posin[jnum],lastposin[jnum]);
            }
            lastposin[jnum] = posin[jnum];
            posin[jnum] = 360*kinedata.circle_count[jnum]+posin[jnum];
            joints[jnum] = posin[jnum];
        } else {
            joints[jnum] = posin[jnum];
            lastposin[jnum] = posin[jnum];
        }
    }

    joints[6] = pos->u;
    joints[7] = pos->v;
    joints[8] = pos->w;

//    printf("X%.3f Y%.3f Z%.3f A%.3f B%.3f C%.3f\n",joints[0],joints[1],joints[2],joints[3],joints[4],joints[5]);

    kinedata.lastP.tran.x = joints[0];
    kinedata.lastP.tran.y = joints[1];
    kinedata.lastP.tran.z = joints[2];
    kinedata.lastP.a = lastposin[3];
    kinedata.lastP.b = lastposin[4];
    kinedata.lastP.c = lastposin[5];
    kinedata.lastP.u = joints[6];
    kinedata.lastP.v = joints[7];
    kinedata.lastP.w = joints[8];


    return 0;
}
