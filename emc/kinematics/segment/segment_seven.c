#include "segment_seven.h"
#include "segmentqueue.h"

#include "hal.h"
#include "rtapi_math.h"
#include "rtapi.h"		/* rtapi_print_msg */
#include "rtapi_string.h"       /* NULL */
#include <float.h>		/* for DBL_MAX */
#include <stdio.h>


#ifndef max
#define max(A,B)  ( (A) > (B) ? (A) : (B))
#endif

#ifndef min
#define min(A,B)  ( (A) < (B) ? (A) : (B))
#endif


static int time_forward,time_plan,time_run;

/*****************************************************************************/
//add by heyuebang in 2015-12-13
//预申明函数
static int sqLinkSegments(SEGMENT * s1, SEGMENT * s2, int priority) ;

//static double sqMeanFilterV(SEGMENTQUEUE * sq); //均值滤波
static int sqGeneratePosition(SEGMENTQUEUE *sq);//计算滤波前位置和位置增量

static void EmcPose_to_pmpose(EmcPose e, PmPose * p) ;

static void pmpose_to_EmcPose(PmPose p, EmcPose *e) ;
static int sqLinePoint(SEGMENTQUEUE *sq, double length, SEGMENT *s, EmcPose *lastpoint);
static int sqCirclePoint(SEGMENTQUEUE *sq, double length, SEGMENT *s, EmcPose *lastpoint);
static void sqSetValidAxis( SEGMENTQUEUE *sq,EmcPose *s);
static int sqCircleFitting(SEGMENT * s1, SEGMENT * s2, SEGMENTQUEUE * sq,double *vmax);

/* Function Declarations */
// 输入X=[s,v，a]，time=[t1,t2,t3],加加速度j，时间t
//输出 y=[s,v,a,j];
//该函数计算从点X开始t时间后的状态y
static void GetOneStepPVAG(const double X[3], const double time[3], double j,
  double t, double y[4]);

//输入开始速度vstart，末点速度vend，开始加速度a，最大速度
//vmax，最大加速度amax，最大加加速度jmax；
//输出时间t=[t1,t2,t3]，加速度j；
//该函数完成速度规划
static void VelocityPlan(double vStart, double vEnd, double a,
  double amax, double jmax, double t[3], double *j);
/*****************************************************************************/

/* some static variables for debugging */
//static EmcPose oldPos, oldVel, newVel, newAcc;
//static double oldDist;
//static double defaultVel;

//计算两点长度
static double sqGiveLength(EmcPose p1, EmcPose p2) {
    PmCartesian disp;
    double mag;

    pmCartCartSub(p1.tran, p2.tran, &disp);
    pmCartMag(disp, &mag);

    return mag;
}

static double sqGiveLength9(EmcPose start, EmcPose end,SEGMENTQUEUE * sq, double *k) {
    double mag;
    //求差值
    k[0] = end.tran.x - start.tran.x;
    k[1] = end.tran.y - start.tran.y;
    k[2] = end.tran.z - start.tran.z;
    k[3] = end.a - start.a;
    k[4] = end.b - start.b;
    k[5] = end.c - start.c;
    k[6] = end.u - start.u;
    k[7]= end.v - start.v;
    k[8]= end.w - start.w;
    int i;
    //按给定比率求九轴长度
    mag = 0.0;
    if(sq->abGoAlongXYZ){
        for (i=0;i<3;i++)
        {
            mag += sq->axis_rate[i]*sq->axis_rate[i]*k[i]*k[i];
        }

        if(mag<1e-6)
        {
            mag=0.0;
            for (i=3;i<9;i++)
            {
                mag += sq->axis_rate[i]*sq->axis_rate[i]*k[i]*k[i];
            }
        }
    }else{
        mag=0.0;
        for (i=0;i<9;i++)
        {
            mag += sq->axis_rate[i]*sq->axis_rate[i]*k[i]*k[i];
        }
    }
    mag = sqrt(mag);

    if(mag<1e-6) return 0.0;
    // 获取各轴分量
    for (i=0;i<9;i++) k[i]/=mag;

    return mag;
}


/*******************************************************************/
//采用分段设置不同系数，可提高效率（采用此功能，可进一步提高拐点速度）
static double sqGiveCornerMixVelocity(double vmax, SEGMENT * s1, SEGMENT * s2,SEGMENTQUEUE * sq){
    double givetheta1;
    double givetheta2;
    double givetheta3;
    double r1,r2,r3,r4;
    double A;


    givetheta1 =PM_PI * sq->angle1 / 180;
    givetheta2 =PM_PI * sq->angle2 / 180;
    givetheta3 = PM_PI_2;

    r1 = vmax; //夹角是180,vmax就是算出来的vmax
    r2 = vmax * sq->coeff1; //夹角是(180-15)度，vmax也不缩小,对应givetheta1
    r3 = vmax * sq->coeff2;//夹角是(180-75)度，vmax缩小0.2,对应givetheta2
    r4 = r3 ;//夹角是(180-90)度，对应givetheta3

    r4 =min(r4,s1->length/(p_h + 5.0)/sq->cycleTime);//ghf-2014/10/23,length<(180*0.005/2)*0.005*11=0.0275
    r4 =min(r4,s2->length/(p_h + 5.0)/sq->cycleTime);//ghf-2014/10/23

     A = s2->angle;
    if (A<=givetheta3){
         vmax = r4;
    }else if (A<=givetheta2){
        vmax = r4 + (A-givetheta3)/(givetheta2-givetheta3)*(r3-r4);
    }else if (A<=givetheta1){
        vmax = r3 + (A-givetheta2)/(givetheta1-givetheta2)*(r2-r3);
    }else if (A<=PM_PI){
        vmax = r2 + (A-givetheta1)/(PM_PI-givetheta1)*(r1-r2);
    }

   return vmax;

}

//求两段之间的拐点速度
static double  sqGiveCornerVelocity(SEGMENT * s1, SEGMENT * s2,
        double maxAcc_nLine, SEGMENTQUEUE * sq) {
    /* s1 and s2 must have been initialized correctly and must have a nonzero
     lenght in order to get a valid corner */

    PmCartesian v1;   //line vector of s1
    PmCartesian v2;   //line vector of s2
    PmCartesian me;
    PmCartesian diff;
    PmCartesian helix;
    double axis[9];
//**************************************************************************
//此处增加一些变量定义
    double vmax; //add by heyuebang in 1/21/2014 to restrain the variation of the acceleration
    PmCartesian a1;
    PmCartesian a2;
    double r1;
    double r2;
    double costheta1;
    double costheta2;
    double A;
//**************************************************************************
    double maxdiff;

    if (s1 == 0 || s2 == 0) {
        diagnostics("Error1 in sqGiveCornerVelocity()\n");
        return -1;
    }

    /* check if s1 or s2 has a zero length or amax or cycleTime is zero */
    //*******************************************************************

     //有圆弧，按三轴算
    /* check if s1 or s2 has a zero length or amax or cycleTime is zero */
    //*******************************************************************
      if (s1->type == SQ_LINEAR) {
        v1 = s1->line.uVec;
      } else {
        pmCartCartSub(s1->end.tran, s1->circle.center, &me);
        pmCartCartCross(s1->circle.normal, me, &v1); //v1方向为垂直于normal，与垂直于normal的圆弧相切，大小为r1。注意：当是螺旋线时，这个半径r1与me的模不一样，是指me投影在垂直于normal的平面上的大小。
         pmCartMag(v1,&r1); //r1=R；
        pmCartScalDiv(s1->circle.rHelix, s1->circle.angle, &helix);//helix为单位角度对应的高度，螺旋线才有；
        pmCartCartAdd(v1, helix, &v1); //结果v1指向螺线线切线方向，大小为单位角度对应的长度
        pmCartPlaneProj(me, s1->circle.normal, &me); //得到圆心到end的方向
        pmCartUnit(me, &me); //单位化
        pmCartScalMult(me, s1->circle.spiral, &me); //获取半径方向增量
        pmCartScalDiv(me, s1->circle.angle, &me);//单位角度对应的增量

        pmCartCartAdd(v1, me, &v1); //合速度方向
        pmCartUnit(v1, &v1);//单位化

       pmCartCartCross(s1->circle.normal, v1, &a1); //a1方向仍然为从s1->end.tran指向normal，并与normal垂直，但大小为cos（theta1），其中theta1为v1于垂直于normal的平面的夹角。
         pmCartMag(a1,&costheta1);  //获取cos（theta1）
          pmCartUnit(a1, &a1); //单位化a1，注意，a1方向正好是圆弧向心力方向。
      }

      if (s2->type == SQ_LINEAR) {
        v2 = s2->line.uVec;
      } else {
        pmCartScalDiv(s2->circle.rHelix, s2->circle.angle, &helix);
        pmCartCartAdd(s2->circle.rPerp, helix, &v2); //结果v2指向螺线线切线方向，大小为单位角度对应的长度
        pmCartUnit(s2->circle.rTan, &me); //单位化
        pmCartScalMult(me, s2->circle.spiral, &me); //获取半径方向增量
        pmCartScalDiv(me, s2->circle.angle, &me);//单位角度对应的增量
               pmCartCartAdd(v2, me, &v2); //合速度方向
                  pmCartUnit(v2, &v2);
    //*******************************************************************

         pmCartMag(s2->circle.rTan,&r2);
        pmCartCartCross(s2->circle.normal, v2, &a2);  //a2方向仍然为从s2->start.tran指向normal，并与normal垂直，但大小为cos（theta2），其中theta2为v2于垂直于normal的平面的夹角。
         pmCartMag(a2,&costheta2); //获取cos（theta2）
          pmCartUnit(a2, &a2); //单位化a2，注意，a2方向正好是圆弧向心力方向。
    //*******************************************************************
      }
    /* calculate the difference between v1 and v2 */
    pmCartCartSub(v2, v1, &diff);
    /* select the largest element in diff */
    maxdiff = max(fabs(diff.x), fabs(diff.y)); //calculate the largest normal vector of cartesian
    maxdiff = max(maxdiff, fabs(diff.z));

//************************************************************************
    vmax = DBL_MAX;

    //如果是3轴及以下，按下面计算拐角速度
    if ( sq->axis9_flag==0){
// 如果s1是圆弧或螺旋线，s2是直线，则按向心力公式v*cos(theta1)=sqrt(amaxNorm*r1)计算;
//注意，如果不相切，计算过于复杂，这里做了简化，即不相切时，分别考虑相切时向心加速度限制和衔接点
//夹角限制。此外，乘以cos(theta1)是考虑螺旋线情况。
    if ((s1->type != SQ_LINEAR) && (s2->type == SQ_LINEAR))
        if (costheta1 != 0)
            vmax = sqrt(s1->amaxNorm * r1) / (costheta1);

//如果s2是圆弧，s1是直线，则按向心力公式v*cos(theta2)=sqrt(amaxNorm*r2);
    if ((s2->type != SQ_LINEAR) && (s1->type == SQ_LINEAR))
        if (costheta2 != 0)
            vmax = sqrt(s2->amaxNorm * r2 ) / (costheta2);

//如果都是圆弧，则以两个圆弧所需向心力（矢量）之差来计算，
//即差值的模表示加速度的改变量，不能大于给定值amaxNorm;||(v*cos(theta2))^2/r2*a2-(v*cos(theta1))^2/r1*a1||<=amaxNorm.
    if ((s2->type != SQ_LINEAR) && (s1->type != SQ_LINEAR)) {
                pmCartScalMult(a1, costheta1 * costheta1 / r1, &a1);
                pmCartScalMult(a2, costheta2 * costheta2 / r2, &a2);
                pmCartCartSub(a2, a1, &a1);
                pmCartMag(a1, &A);
                if (A > 0)
                    vmax = sqrt(min(s1->amaxNorm,s2->amaxNorm) / A);
    }


// 如果不相切，则计算夹角限制速度maxAcc_nLine** cycleTime / maxdiff;

        if (maxdiff > 1e-6) //ghf-2014/12/11
            vmax = min(vmax, maxAcc_nLine * sq->cycleTime / maxdiff);

          //将直线拐角看成曲率，采用圆弧最大速度限制其最大速度和拐角速度
        //夹角是0,v1 v2是一条同向的直线;夹角是30度，v1到v2是以150度过度过来的
        pmCartCartDot(v1,v2,&costheta1);
        if(costheta1 > 1)
            costheta1 = 1;
        else if(costheta1 < -1)
            costheta1 = -1;

        A = PM_PI - acos(costheta1);
         s2->angle = A;

//diagnostics("~conrnervelocity in: s2->Id=%d,s2->angle=%d\n",s2->ID,(int)(s2->angle*1000*180/3.1415926));
    //ghf-2014/9/27,g0不作系数限制
    if((s1->gmodetype == 0) && (s1->type == SQ_LINEAR) && (s2->gmodetype == 0) && (s2->type == SQ_LINEAR)) {
        return vmax;
    }

    if (A < sq->optimized_angle) vmax = 0.0;

    if (sq->corner_mix_flag == 1){
        vmax = sqGiveCornerMixVelocity(vmax, s1, s2,sq);
    }

    //采用圆弧估计拐角速度
    double vmaxForCircle;
    int flagForCircle;
    if(sq->tolerance >= 0.001 && sq->small_seg_cornerV_mode !=0){
        flagForCircle = sqCircleFitting(s1, s2, sq,&vmaxForCircle);
        if (flagForCircle == 1){
        if (sq->small_seg_cornerV_mode == 1)
            vmax =  vmaxForCircle;
        else if (sq->small_seg_cornerV_mode == 2)
            vmax =min(vmax, vmaxForCircle); //增加圆弧限制
        else if (sq->small_seg_cornerV_mode == 3)
             vmax =max(vmax, vmaxForCircle); //增加圆弧限制

        }
    }

 //   if (s1->ID>506 && s1->ID<510)
   //  printf(" sqGiveCornerVelocity()4:s1->id=%d,s2->id=%d,vmax=%f,maxAcc_nLine=%f\n",s1->ID,s2->ID,vmax,maxAcc_nLine);
    return vmax;

  }//3轴结束

    /********************************************************************/
    //该部分为5轴增加rtcp和变刀长功能，要求变化时速度为0,不能有速度和位置滤波功能
    if(fabs(s1->Kinematics5Axis.KnifeLength - s2->Kinematics5Axis.KnifeLength) > 1e-6  || s1->Kinematics5Axis.PlanMode != s2->Kinematics5Axis.PlanMode)
    {
        s2->angle = 0.0;
        return 0.0;
    }
        /********************************************************************/
    //4轴及以上，不能以圆弧计算拐角速度，不能插入圆弧
    double coefficeint1[9],coefficeint2[9];
    memcpy(coefficeint1,s1->length_coefficient,9*sizeof(double));
    memcpy(coefficeint2,s2->length_coefficient,9*sizeof(double));

    if (s1->type == SQ_CIRCULAR){
        coefficeint1[0] *= v1.x;
        coefficeint1[1] *= v1.y;
        coefficeint1[2] *= v1.z;
    }
    if (s2->type == SQ_CIRCULAR){
        coefficeint2[0] *= v2.x;
        coefficeint2[1] *= v2.y;
        coefficeint2[2] *= v2.z;
    }

/***************************************************************************/
            int i;
            for (i=0;i<9;i++)
            {
            //   axis[i] = sq->axis_rate[i]*(coefficeint2[i] - coefficeint1[i]);
                 axis[i] = coefficeint2[i] - coefficeint1[i];
              }

            //xyz轴拐弯加速度限制
            maxdiff = 0.0;
            for (i=0;i<3;i++)
            {
                maxdiff = max(maxdiff,fabs(axis[i]));
            }

        vmax = DBL_MAX ;
            if (maxdiff > 1e-6) //ghf-2014/12/11
                vmax = min(vmax, maxAcc_nLine * sq->cycleTime / maxdiff);

            //角度轴拐弯加速度限制
            maxdiff = 0.0;
            for (i=3;i<6;i++)
            {
                maxdiff = fabs(axis[i]);
                if(maxdiff >1e-6)
                {
                  //   printf("cornervelocity:s1.id=%d,vmax=%f,",s1->ID,vmax);
                    if(s1->gmodetype == 1 || s2->gmodetype == 1)
                    {
                        //只要有一条是g1,则使用g1数据
                        vmax = min(vmax, sq->G_mode_1.maxAngleAcc_n_Axis[i]*sq->cycleTime/maxdiff);
                    }else{
                        //使用g0数据
                        vmax = min(vmax, sq->G_mode_0.maxAngleAcc_n_Axis[i]*sq->cycleTime/maxdiff);
                    }

                //    printf("cornervelocity:s1.id=%d,angle_vmax=%f,maxAngleAcc_n_Axis[%d]=%f,maxdiff=%f,sq->cycleTime=%f\n",s1->ID,vmax,i,sq->G_mode_0.maxAngleAcc_n_Axis[i],maxdiff,sq->cycleTime);
                }
            }

            //如果是跟随模式，则规划以xyz轴规划切换到角度轴规划，或角度轴规划切换到xyz轴规划，拐弯速度设为0
            if(sq->abGoAlongXYZ == 1)
            {
                double temp1 = 0.0;
                double temp2 = 0.0;
                for(i=0;i<3;i++)
                {
                    temp1 = max(temp1,fabs(coefficeint1[i]));
                    temp2 = max(temp2,fabs(coefficeint2[i]));
                }
                if((temp1 <1e-6 && temp2 >1e-6) || (temp1 >1e-6 && temp2 < 1e-6))
                {
                    //s1 xyz轴没数据而s2有数据，或s1 xyz有数据而s2没数据
                    vmax = 0.0;
                }
            }


/***************************************************************************/
            double temp0=0.0;
            double temp1=0.0;
            double temp2 = 0.0;
            for (i=0;i<3;i++){
                temp0 += sq->axis_rate[i]*coefficeint1[i]*sq->axis_rate[i]*coefficeint1[i];
                temp1 += sq->axis_rate[i]*coefficeint2[i]*sq->axis_rate[i]*coefficeint2[i];
                temp2 += sq->axis_rate[i]*coefficeint1[i]*sq->axis_rate[i]*coefficeint2[i];
            }

            if (temp0>1e-6 && temp1>1e-6){
                temp0 = sqrt(temp0);
                temp1 = sqrt(temp1);
                costheta1 = temp2/temp0/temp1;

                    if(costheta1 > 1)
                        costheta1 = 1;
                    else if(costheta1 < -1)
                        costheta1 = -1;

                    A = PM_PI - acos(costheta1);
                     s2->angle = A;

            //diagnostics("~conrnervelocity in: s2->Id=%d,s2->angle=%d\n",s2->ID,(int)(s2->angle*1000*180/3.1415926));
                //ghf-2014/9/27,g0不作系数限制
                if((s1->gmodetype == 0) && (s1->type == SQ_LINEAR) && (s2->gmodetype == 0) && (s2->type == SQ_LINEAR)) {
                    return vmax;
                }

                if (A < sq->optimized_angle) vmax = 0.0;

       }
//            if (s1->ID < 20){
//                printf("s1-->id=%d,s2->id=%d,vmax=%f,maxdiff=%f,maxacc=%f,s1->totlength=%f\n",s1->ID,s2->ID,vmax,maxdiff,maxAcc_nLine,s1->totLength);
//                printf("coefficinet1:k0=%f,k1=%f,k2=%f,k3=%f,k4=%f,k5=%f,k6=%f,k7=%f,k8=%f\n",coefficeint1[0],coefficeint1[1],coefficeint1[2],coefficeint1[3],coefficeint1[4],coefficeint1[5],coefficeint1[6],coefficeint1[7],coefficeint1[8]);
//                printf("coefficinet2:k0=%f,k1=%f,k2=%f,k3=%f,k4=%f,k5=%f,k6=%f,k7=%f,k8=%f\n",coefficeint2[0],coefficeint2[1],coefficeint2[2],coefficeint2[3],coefficeint2[4],coefficeint2[5],coefficeint2[6],coefficeint2[7],coefficeint2[8]);
//                printf("axis_rate:k0=%f,k1=%f,k2=%f,k3=%f,k4=%f,k5=%f,k6=%f,k7=%f,k8=%f\n",sq->axis_rate[0],sq->axis_rate[1],sq->axis_rate[2],sq->axis_rate[3],sq->axis_rate[4],sq->axis_rate[5],sq->axis_rate[6],sq->axis_rate[7],sq->axis_rate[8]);
//              }

            return vmax;

}

/*****************************************************************************/
//add by heyuebang in 2015-12-13
static int sqCircleFitting(SEGMENT * s1, SEGMENT * s2, SEGMENTQUEUE * sq, double *vmax){
        double r1,r2,r3;
       // double vmax = 1e6;
        double temp;
        int flag=0;
    //第一步：以弓高误差 sq->tolerance计算直线段看成圆弧需要的最小半径R1 = (（L1/2)^2/sq->tolerance + sq->tolerance)/2
    //R2 = (（L2/2)^2/sq->tolerance + sq->tolerance)/2,如果两条直线的三个端点共圆半径R3=||s2->end-s1->start||/2/sin(s2->angle)
    //大于等于R1,R2，则可认为可以由R3半径的圆弧拟合这两条直线。实际为排除夹角过小和过大（直线180度），增加两个条件，及夹角>135度和拐点速度不大于这两条直线的最大速度
    if (s1->gmodetype == 1 && s1->type == SQ_LINEAR  &&s2->gmodetype==1 && s2->type == SQ_LINEAR && sq->tolerance >= 0.001)
    {
       //计算s1的弓高误差容许的最小圆弧半径
        r1 = s1->length*s1->length*0.25/sq->tolerance + sq->tolerance;
        r1 *= 0.5;
       //计算s2的弓高误差容许的最小圆弧半径
        r2 = s1->length*s1->length*0.25/sq->tolerance + sq->tolerance;
        r2 *= 0.5;

            //计算s1和s2三个端点共圆半径
            r3 = sqGiveLength(s2->end,s1->start);
            if (sin(s2->angle)>1e-6)
                 r3 /= 2.0*sin(s2->angle);
            else
                r3 = 1e6;

            if (r3>=r1 && r3>=r2 && ( s2->angle > (PM_PI-PM_PI/4.0)))
            {  //该半径大于弓高容许最小半径，就认为s1和s2是圆弧的两段
                //增加判断，拐点速度不能大于直线最大速度，大于认为是直线，没有必要再拟合成圆了。

                        if (sq->omega_limit_flag){
                            temp = max(sq->G_mode_1.maxJerk,sq->G_mode_1.maxAcc_circle*sqrt(sq->G_mode_1.maxAcc_circle*0.2));
                            *vmax = pow(temp* r3* r3,0.333333);
                             *vmax =min(*vmax, sqrt(sq->G_mode_1.maxAcc_nCircle * r3)); //计算圆弧最大速度
                        }else{
                            *vmax = sqrt(sq->G_mode_1.maxAcc_nCircle* r3);
                        }

                  flag = 1;
            }
    }

    return flag;
}

//计算s逻辑段上最小的切线加速度，每一段的切线加速度在addline时给出 ghf-2014/6/10
static double sqGiveMinAmaxTan(SEGMENT * s) {
    /* give the minimum tangential acceleration for the chain of linked that starts with s. */
    SEGMENT *cursor;
    double minAmax;

    if (s == 0) {
        diagnostics("Error1 in sqGiveMinAmaxTan()\n");
        return -1;
    }

// the next tell us the nextsegment is not linked to the nowsegment(a2014-3-24)
    if (s->numLinkedSegs == 0)
        return s->amaxTan;

    minAmax = s->amaxTan;
    cursor = s;

// the next tell us the nextsegment is linked to the nowsegment(a2014-3-24)
    while (cursor->nextSegment != 0 && cursor->nextSegment->linkedToPrevSeg == 1) {
        cursor = cursor->nextSegment;
        if (cursor->amaxTan < minAmax) {
            minAmax = cursor->amaxTan;
        }
    }

    return minAmax;

}

//计算最小加加速度
static double sqGiveMinBetamaxTan(SEGMENT * s) {
    /* give the minimum tangential acceleration for the chain of linked that starts with s. */
    SEGMENT *cursor;
    double minJmax;

    if (s == 0) {
        diagnostics("Error in sqGiveMinAmaxTan()\n");
        return -1;
    }

// the next tell us the nextsegment is not linked to the nowsegment(a2014-3-24)
    if (s->numLinkedSegs == 0)
        return s->jmaxTan;

    minJmax = s->jmaxTan;
    cursor = s;

// the next tell us the nextsegment is linked to the nowsegment(a2014-3-24)
    while (cursor->nextSegment != 0 && cursor->nextSegment->linkedToPrevSeg == 1) {
        cursor = cursor->nextSegment;
        if (cursor->jmaxTan < minJmax) {
            minJmax = cursor->jmaxTan;
        }
    }

    return minJmax;
}


//当g0不使用倍率调整时，返回最大增量，否则返回结果乘以倍率
static double sqGiveMaxVecWithFeedOverrate(SEGMENTQUEUE * sq, SEGMENT *s){
    if (s->gmodetype == 0 &&  sq->segpara_i[0] == 0) //g0是否受倍率控制
        return s->maxVec;
    else {
        if (s->gmodetype == 0) {
            return min(s->maxVec*sq->feedOverrideFactor,s->maxVec);
        } else {
            return min(s->maxVec*sq->feedOverrideFactor,sq->limit_vmax);
        }
    }
}

//计算最大速度,  只能用于sqplan函数中
static double sqGiveMaxVec(SEGMENTQUEUE * sq, SEGMENT * s) {
    /* give the right maxInc from the chain of linked segments that start with s. */
    SEGMENT *cursor;
    double maxVec;

    if (s == 0) {
        diagnostics("Error in sqGiveMinAmaxTan()\n");
        return -1;
    }

// the next tell us the nextsegment is not linked to the nowsegment(a2014-3-24)
    if (s->numLinkedSegs == 0)
        return  sqGiveMaxVecWithFeedOverrate( sq, s);

    maxVec = sqGiveMaxVecWithFeedOverrate(sq, s);
    cursor = s;

// the next tell us the nextsegment is linked to the nowsegment(a2014-3-24)
    while (cursor->nextSegment != 0 && cursor->nextSegment->linkedToPrevSeg == 1) {
        cursor = cursor->nextSegment;
        if (sqGiveMaxVecWithFeedOverrate(sq, cursor) > maxVec && cursor->prevSegment->lastSegFlag == 0) {
            maxVec = sqGiveMaxVecWithFeedOverrate(sq, cursor) ;
        }
    }

    return maxVec;
}

static int sqLinkCriterion(SEGMENTQUEUE * sq, SEGMENT * s, SEGMENT * end,
        double length, double feedOverride) {

    double amaxTan;
    double initVec, finalVec;
    SEGMENT *lastSeg;
    double jmaxTan;
    double t1,t2;
    double temp;

    if (sq == 0 || s == 0) {
        diagnostics("Error1 in sqLinkCriterion\n");
        return -1;
    }

    if (s->linkedToPrevSeg == 1) {
        diagnostics(
                "Error2,Segment s is linked to its preceding segment in sqLinkCriterion,s->ID=%d\n",
                s->ID);
        return -1;
    }

    /* find the last segment of the chain */
    lastSeg = end;
    while(lastSeg->nextSegment!=0 && lastSeg->nextSegment->linkedToPrevSeg==1){
        lastSeg = lastSeg->nextSegment;
        diagnostics("Warning, Segment is linked to its next segment in sqLinkCriterion,s->Id=%d\n",s->ID);
    }

    amaxTan = sqGiveMinAmaxTan(s);
    jmaxTan = sqGiveMinBetamaxTan(s);

        initVec = s->initVec;
    /* find the correct value of the final increment (with taking a feedoverride
     into account). */
    if (feedOverride >= 1.0) {
        finalVec = lastSeg->finalVec;
    } else {
        if (lastSeg->nextSegment != 0
                && lastSeg->nextSegment->maxVec < lastSeg->maxVec)
            finalVec = min(lastSeg->finalVec,
                    lastSeg->nextSegment->maxVec * feedOverride);
        else
            finalVec = min(lastSeg->finalVec, lastSeg->maxVec * feedOverride);
    }

    temp = fabs(finalVec - initVec);
      if (temp*jmaxTan < amaxTan*amaxTan){
          t1 = sqrt(temp*jmaxTan)/jmaxTan;
          t2 = t1;
      }else{
          t1 = amaxTan/jmaxTan;
          t2 =temp/amaxTan;
      }

      temp=0.5*fabs(finalVec + initVec)*(t1+t2);
                if (temp > length-1e-6) {
          diagnosticsOff("Error3 in sqLinkCriterion: s=%d need link. \n", s->ID);
              return SQ_LINKING_NEEDED;
                }
                //够长，但如果所需时间不足1步呢？

                if (t1+t2 <sq->cycleTime){ //加速无需一步+p_h步，判断长度是否够最大速度一步
                    if (max(finalVec,initVec)*(sq->cycleTime - t1 - t2) + temp > length-1e-6){ // 不够一步
                          return SQ_LINKING_NEEDED;
                    }
                   // 够一步，但如果前后速度为0呢?此处认为不需要链接，但规划时需要考虑。
                }

          diagnosticsOff("Error4: Chain starting at %d is NOT too short - linking NOT needed\n", s->ID);
          return SQ_LINKING_NOT_NEEDED;
   }

/*****************************************************************************/
//vEnd为末点速度，time为规划时间，jplan为规划加加速度，sEnd为最终达到的位置
//返回值，0 为无解，1为有解
static int sqTestLastVec(SEGMENTQUEUE * sq, SEGMENT * s, double vEnd,double vmax,double amax,double jmax, double time[3],double *jplan,double *sEnd){
    /* Function Declarations */
    // 输入X=[s,v，a]，time=[t1,t2,t3],加加速度j，时间t
    //输出 y=[s,v,a,j];
    //该函数计算从点X开始t时间后的状态y
    //extern void GetOneStepPVAG(const double X[3], const double time[3], double j,double t, double y[4]);

    //输入开始速度vstart，末点速度vend，开始加速度a，最大速度
    //vmax，最大加速度amax，最大加加速度jmax；
    //输出时间t=[t1,t2,t3]，加速度j；
    //该函数完成速度规划
   // extern void VelocityPlan(double vStart, double vEnd, double a, double amax, double jmax, double t[3], double *j);
    double vStart,a,t[3],j;
    double X[3],y[4];

    vStart = sq->lastState[1]; //获取当前速度
    a = sq->lastState[2]; // 获取当前加速度
    VelocityPlan( vStart, vEnd, a, amax,  jmax,  t,  &j); //计算达到vEnd且加速度为0所需要的时间和加加速度
    X[0] = sq->lastState[0]; //获取当前位置
    X[1] = vStart;
    X[2] = a;
    GetOneStepPVAG(X, t, j,t[2], y); //求达到vEnd时的位置
    *sEnd = y[0]; //输出当前位置，有解时有效
    *jplan = j;//输出规划的加加速度，有解时有效
    memcpy(time,t,3*sizeof(double));//输出规划时间，有解时有效
    if (y[0] > s->totLength){ //判断，如果达到的位置大于当前段长度，则表示无解
        return 0;
    }

    if (t[2] < sq->cycleTime){ // 如果t[2]小于一个周期，需要保证至少能走一步
        if (vEnd*(sq->cycleTime - t[2])+y[0] > s->totLength){ //不能走一步
            return 0;
        }
    }

    if (a!= 0){ // 初始加速度不为0,则要测试最大速度是否大于vmax，最小速度是否小于0
        a = fabs(a / jmax);
         GetOneStepPVAG(X, t, j,a, y); //求达到a时的位置
         if (y[1]<-1e-9){ // 理论上不可能发生
             diagnostics("s->id=%d plan min velocity < 0\n",s->ID);
             return 0;
         }
         if (y[1]>vmax+1e-9){ // 理论上不可能发生
           //  diagnostics("s->id=%d plan max velocity > vmax,vmax*1000=%d,y[1]*1000=%d\n",s->ID,(int)(vmax*1000),(int)(y[1]*1000));
            // return 0;
         }

    }

    return 1;   //有解
}

// 搜索最大值,二值法搜索，最大20次，精度为10^-6*（vEndmax-vEndmin）
//假设 SolutionVec是一个可行解，需要在调此函数前确认
// 输出参数：time[3] 规划加减速时刻，jplan规划加加速度，EndPosVec[0]末点位置，EndPosVec[1]末点速度；
static int sqSearchEndVec(SEGMENTQUEUE * sq, SEGMENT * s, double SolutionVec,double plFinalVec,double vmax,double amax,double jmax, double time[3],double *jplan,double EndPosVec[2],int *search_number){
    double solution,Nsolution, newVec;
    double sEnd;
    double  tPlan[3], jerkPlan;
    int i,temp,flag;

    solution = SolutionVec;
    Nsolution = plFinalVec;
    newVec = SolutionVec; //开始最优解为初解
   flag = 0;

    for (i=0;i<20;i++){
        temp = sqTestLastVec(sq, s, newVec,vmax,amax,jmax, tPlan,&jerkPlan,&sEnd);//测试
       *search_number = i;
        if (temp == 0){//无解
            Nsolution = newVec;
        }else{
            flag = 1;
            solution = newVec;
            time[0] = tPlan[0]; time[1] = tPlan[1]; time[2] = tPlan[2];
            *jplan = jerkPlan;
            EndPosVec[0] = sEnd;
            EndPosVec[1] = solution;

            // 设置提前结束条件，剩余长度不足一步了
            if (s->totLength - sEnd < newVec*sq->cycleTime){ //剩下的长度不够一步了；规划ok
                return 1; //找到解了，提前提出
            }
         }

            //还不是最优解，继续搜索
                  newVec = 0.5*(Nsolution + solution); //更新 新解 循环测试
    }//end for

    //运行到这，20次还没退出，有问题
     if (flag ==0){
         //无解，初解肯定是解，可能不够一步
          temp = sqTestLastVec(sq, s, SolutionVec,vmax,amax,jmax, tPlan,&jerkPlan,&sEnd);//
          solution = newVec;
          time[0] = tPlan[0]; time[1] = tPlan[1]; time[2] = tPlan[2];
          *jplan = jerkPlan;
          EndPosVec[0] = sEnd;
          EndPosVec[1] = solution;
          if (sEnd - s->totLength > 1e-6){
         diagnostics("error in sqSearchEndVec: s->id=%d ,sEnd=%f,sq->lastState[0]=%f,sq->lastState[1]=%f,sq->lastState[2]=%f\n",s->ID,sEnd,sq->lastState[0],sq->lastState[1],sq->lastState[2]);
         return 0;
          }

          if(sEnd - s->totLength > 0.0) sEnd = s->totLength;

          EndPosVec[0] = sEnd;


          diagnostics("warning in sqSearchEndVec: s->id=%d ,sEnd=%f,sq->lastState[0]=%f,sq->lastState[1]=%f,sq->lastState[2]=%f\n",s->ID,sEnd,sq->lastState[0],sq->lastState[1],sq->lastState[2]);

     //    return 0;
     }

    return 1;
}

// 计算规划后运行到末速位置
//Xinput初始位置，vEnd为结束速度，vmax，amax，jmax为给定最大速度，加速度，加加速度
//time为规划时间，jplan为规划加加速度，Yout为末速位置
static int sqComputerPositionForLastVec(double Xinput[4], double vEnd,double vmax,double amax,double jmax, double time[3],double *jplan,double Yout[4]){
    double X[3], a,Y[4];

    X[0] = Xinput[0]; //获取当前位置
    X[1] = Xinput[1]; //获取当前速度
    X[2] = Xinput[2]; //获取当前加速度

    VelocityPlan( X[1], vEnd, X[2], amax,  jmax,  time,  jplan); //计算达到vEnd且加速度为0所需要的时间和加加速度
    GetOneStepPVAG(X, time, *jplan,time[2], Yout); //求达到vEnd时的位置

    //以下代码可能不需要测试，为减小计算量，多次试验后，如不进入可注释掉
    a = X[2];
    if (a != 0){ // 初始加速度不为0,则要测试最大速度是否大于vmax，最小速度是否小于0
        a = fabs(a / jmax);
         GetOneStepPVAG(X, time, *jplan,a, Y); //求达到a时的位置
         if (Y[1]<-1e-9){ // 理论上不可能发生
             diagnostics(" sqComputerPositionForLastVec: plan min velocity < 0\n");
             return 0; //无解
         }
         if (Y[1]>vmax+1e-9){ // 理论上不可能发生
           //     diagnostics(" sqComputerPositionForLastVec: plan min velocity > vmax \n");
            // return 0; // 无解
         }
    }

    return 1;   //有解
}
//二值法搜索最大速度，
//输出 7段加减速时间time[7],加速与减速区加加速度jplan[2]，最大规划速度VmaxPlan,匀速起点和末点VmaxStartEndPos
static int sqSearchVmax(SEGMENTQUEUE * sq, SEGMENT * s, double initVec,double finalVec,double vmax,double amax,double jmax, double time[7], double jplan[2],double *VmaxPlan, double VmaxStartEndPos[2],int *search_number){
    double  Yout[4], Ytemp[4];
    double jerk1,jerk2;
    double time1[3],time2[3];
    int flag, solution;
    double CruiseStartPos;

    if (vmax < min(initVec,finalVec)){ //最大速度不能小于最小始末点速度
        //正常不可能发生
        diagnostics("Warning 0 in sqSearchVmax");
        vmax = min(initVec,finalVec);
    }
    if (fabs(sq->lastState[1] - initVec) > 1e-6){ // 初速设置必须和当前速度一致
        diagnostics("Error 1 in sqSearchVmax\n");
        return 0;
    }

    //无解，二值搜索最大规划速度
   double solutionVec, NsolutionVec,newVec;
   solutionVec = finalVec; //该解必须预先测试ok
   NsolutionVec = vmax;
   newVec = NsolutionVec;
   solution = 0; //假设没解
   int i;

   for ( i=0;i<20;i++){
       *search_number = i;
          flag = sqComputerPositionForLastVec(sq->lastState, newVec, vmax, amax, jmax, time1,&jerk1,Ytemp);//加速
          CruiseStartPos = Ytemp[0]; //获取匀速起点
          if (flag == 1){ //有解
                  flag = sqComputerPositionForLastVec(Ytemp, finalVec, vmax, amax, jmax, time2,&jerk2,Yout);//减速
          }

          if (flag == 0 || Yout[0] > s->totLength) { // 条件为规划最大速度大于给定最大速度或最小速度小于0或位置超过段长
              //无解
              NsolutionVec = newVec; //无解速度取当前测试速度
          }else{
              //有解
              solutionVec = newVec;//有解速度取当前测试速度
              // 记录优化解
              solution = 1; //有解

              jplan[0] = jerk1; jplan[1]= jerk2; // 加减加加速度

              time[0] = time1[0];time[1] = time1[1]; time[2]=time1[2];
              if (solutionVec < 1e-6){//认为有问题，最大速度为0.
                   time[3] =time[2] + 1e9;
                     *VmaxPlan = 0.0; //最大速度
              }else{
                    time[3] =time[2] +  (s->totLength - Yout[0]) / solutionVec;
                      *VmaxPlan = solutionVec; //最大速度
              }
              time[4] = time[3] + time2[0];time[5] =time[3] + time2[1]; time[6]=time[3] + time2[2]; //7段时刻

               VmaxStartEndPos[0] =CruiseStartPos; //输出匀速起点
               VmaxStartEndPos[1] = VmaxStartEndPos[0] + s->totLength - Yout[0]; //输出匀速末点
            //   diagnostics("s->id=%d,Yout[0]=%d,initVec=%d,finalVel=%d,CruiseStartPos=%d\n",s->ID,(int)(Yout[0]*1000),(int)(initVec*1000),(int)(finalVec*1000),(int)(CruiseStartPos*1000));
              //判断提前退出条件
              if (vmax - solutionVec < 1e-3)  return 1; //有解，最大速度vmax
              if (s->totLength - Yout[0] <= solutionVec * sq->cycleTime) return 1; //匀速不足一步了，退出
          }

          //更新当前速度为一半
           newVec = 0.5*(NsolutionVec + solutionVec);
   }

    //没有提前退出
                       if (solution == 1) return 1; //搜索过程有解，直接退出
    // 无解
                       //没有解，可能情况：1. solutionVec本身就不是解，该条件在调用此函数前需要确认；
                       //2. 始末速度太小，而长度又太短，导致搜索次数不够，这种情况极少发生；确保不发生措施有1)新段长度必须大于一个小数（0.01,0.001）,对应0.001s采样时间速度为1-10,20次搜索可保证最大速度在100000mm/s以上有解，2)调速时剩余长度太短，措施是调速时需要保证剩余长度足够走几个周期且剩余长度必须大于0.01ms，3)暂停后剩余长度，如果太短如小于0.001,需要链接后续新段，避免进入这里。
                       //此处直接认为
                       if (max(initVec,finalVec) > 0.0 || vmax <1e-9){ // 调速为0或起点，末点速度有一个不为0,表明无解
                           //vmax = max(initVec,finalVec)就是解
                        //   printf("warning 1 in sqSearchVmax: s->id=%d,initVex=%f, finalVec=%f,s->plinitV=%f,s->plfinalV=%f\n",s->ID,initVec,finalVec,s->plInitVec,s->plFinalVec);
                      //      printf("warning 1 in sqSearchVmax: s->totLenth=%f,solutionVec=%f,NsolutionVec=%f\n",s->totLength,solutionVec,NsolutionVec);
                           return 0; //无解
                       }else{  // 调速不为0,起点/末点速度又为零，则无解表明此线段太短。
                           //极少可能发生，但也可能，比如最后一段，暂停后剩下一小段，非常短。
                            diagnostics("warning 2 in sqSearchVmax: initVex, finalVec are zero, no solution for search\n");
                            sq->lastState[1] = 0.0; //重设为0
                            sq->lastState[2] = 0.0;
                            printf("sqSearch :laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);

                            double temp;
                            temp = s->totLength-sq->lastState[0];
                            if (temp < 0.0) temp = 0.0;
                            jerk1 = (temp)*0.5 /sq->ctPow3;
                            if (jerk1 > jmax){
                                jerk1 = jmax;jerk2 = -jmax;
                                time[0] = pow(0.5*(temp)/jmax,1.0/3.0);
                            }else{
                                jerk2 = - jerk1;
                                time[0] = sq->cycleTime;
                            }

                            time[1] = time[0]; time[2]=time[1] + time[0];
                            time[3] =time[2]; //无匀速
                            time[4] = time[3] + time[0];time[5] =time[4]; time[6]=time[5] + time[0]; //7段时刻
                            *VmaxPlan = jerk1*time[0]*time[0]; //最大速度
                            VmaxStartEndPos[0] =0.5*(temp)+sq->lastState[0]; //输出匀速起点
                             VmaxStartEndPos[1] = VmaxStartEndPos[0]; //输出匀速末点
                       }

   return 1;
}


//规划，获取七段加减速时间
static int sqPlanSegment(SEGMENTQUEUE * sq, SEGMENT * s) {

    int startclocks = 0, endclocks = 0;

    double maxVec;
    double amaxTan;
    double jmaxTan;

    SEGMENT *cursor;

  //  unsigned char id[20];

//    int flagdisk = 1;
//    int idisk;
//    for (idisk=0;idisk<20;idisk++){
//        id[idisk] = 'a';
//    }
//   flagdisk = getdiskid (id, 20);
//   diagnostics("2sqPlanSegment:flag=%d,id=%s\n",flagdisk,id);





    startclocks = rtapi_get_time();

    if (sq == 0) {
        diagnostics("Error1  in sqPlanSegment()\n");
        return -1;
    }

    /* check is s is a valid pointer */
    if (s == 0) {
        diagnostics("Error2 in sqPlanSegment()\n");
        return -1;
    }

    /* check if this segment is linked to its predecessor (which should never
     happen!) */
    if (s->linkedToPrevSeg == 1) { //小线段不停启动暂停可出现此情况ghf-2014/6/16
        diagnostics("s->ID=%d,s->numsegment=%d,Error3 in sqPlanSegment()\n",s->ID,s->numLinkedSegs);
        sq->done = 1; //ghf-2014/6/16
        return -1;
    }

    if ((maxVec = sqGiveMaxVec(sq, s) ) < 1e-6) {
        maxVec = 0.0; //调速为0;
         diagnostics("s->ID=%d,s->numsegment=%d,Error4 in sqPlanSegment()\n",s->ID,s->numLinkedSegs);
      //  return -1;
    }

    //defaultVel = maxVec; //add by lsj, on 08.2.27，该值会随着最大速度的调整而改变

    /* find the minimum value for the maximum tangential acceleration for this
     chain of segments beginning with s */
    amaxTan = sqGiveMinAmaxTan(s); //获取逻辑段加速度
    jmaxTan = sqGiveMinBetamaxTan(s);//获取逻辑段加加速度
    if (amaxTan<=1e-6){
         diagnostics("Error in sqPlanSegment():amaxTan=0\n");
         return -1;
    }
    if (jmaxTan<=1e-6){
         diagnostics("Error in sqPlanSegment():betamaxTan=0\n");
         return -1;
    }

  //  diagnosticsOff("AmaxTan in sqplansegment  = %d \n", (int) (ceil(amaxTan)));
  //找到末点速度
    cursor = s;
    while (cursor->nextSegment != 0 && cursor->nextSegment->linkedToPrevSeg) {
        cursor = cursor->nextSegment;
    }

    if (sq->feedOverrideFactor >=1.0) {
         s->plFinalVec = cursor->finalVec;
    } else {
        if (cursor->nextSegment != 0
                &&  sqGiveMaxVecWithFeedOverrate(sq,cursor->nextSegment) < sqGiveMaxVecWithFeedOverrate(sq,cursor))
            s->plFinalVec = min(cursor->finalVec,  sqGiveMaxVecWithFeedOverrate(sq,cursor->nextSegment));
        else
            s->plFinalVec = min(cursor->finalVec, sqGiveMaxVecWithFeedOverrate(sq,cursor));
    }

//    if (s->type == SQ_CIRCULAR){
//        printf("Circular:s->Id=%d,sq->feedoverrid = %f, s->initVec=%f,s->plFinalvel=%f,maxVec=%f,s->totlength=%f\n",s->ID,sq->feedOverrideFactor,s->initVec,s->plFinalVec,maxVec,s->totLength);
//    }
//    if (sq->feedOverrideFactor>1){
//           printf("feedOverrideFactor:sq->feedoverrid = %f, s->initVec=%f,s->plFinalvel=%f,maxVec=%f,s->givenmaxvec=%f\n",sq->feedOverrideFactor,s->initVec,s->plFinalVec,maxVec,s->givemaxVec);
//    }

    if (s->plFinalVec > maxVec) s->plFinalVec = maxVec;
    //vEnd为末点速度，time为规划时间，jplan为规划加加速度，sEnd为最终达到的位置
    //返回值，0 为无解，1为有解
    int flag, flag1;
    double time[3];
    double jplan;
    double sEnd;
    double time1[7];
    double jplan1[2];
     double VmaxPlan;
    double VmaxStartEndPos[2];
    double EndPosVec[2];
    int search_number;
    //s->plInitVec * sq->cycleTime < s->totLength 必须由牵连函数保证
    flag = sqTestLastVec(sq,  s, s->plFinalVec,maxVec,amaxTan,jmaxTan,  time,&jplan,&sEnd);
   // printf("sqPlanSegment 11: s->id=%d, s->numlinked=%d,cursor->ID=%d,s->initVec=%f,s->plFinalVec=%f,s->totLength=%f,maxVec=%f,amaxTan=%f,jmaxTan=%f,send=%f\n",s->ID,s->numLinkedSegs,cursor->ID,s->initVec,s->plFinalVec,s->totLength,maxVec,amaxTan,jmaxTan,sEnd);

    if (flag == 0){//无解
            // 输出参数：time[3] 规划加减速时刻，jplan规划加加速度，EndPosVec[0]末点位置，EndPosVec[1]末点速度；
        //进入这里，初始加速度肯定为0,即s->initVec为有效解
            flag1 = sqSearchEndVec(sq, s, s->initVec,s->plFinalVec,maxVec,amaxTan,jmaxTan,  time,&jplan, EndPosVec,&search_number);
            printf("sqPlanSegment 22: s->id=%d, s->numlinked=%d,cursor->ID=%d,s->initVec=%f,s->plFinalVec=%f,s->totLength=%f,maxVec=%f,amaxTan=%f,jmaxTan=%f,send=%f\n",s->ID,s->numLinkedSegs,cursor->ID,s->initVec,s->plFinalVec,s->totLength,maxVec,amaxTan,jmaxTan,EndPosVec[0]);

            if (flag1 == 0){ // 正常不可能发生,发生s->initVec不是解的可能有
               // 无需一步，不处理，
                    //长度不够
                    if(EndPosVec[0] - s->totLength > 1e-6){
                       printf("sqPlanSegment error 5 no solution: s->id=%d, s->numlinked=%d,cursor->ID=%d,s->initVec=%f,s->plFinalVec=%f,s->totLength=%f,maxVec=%f,amaxTan=%f,jmaxTan=%f\n",s->ID,s->numLinkedSegs,cursor->ID,s->initVec,s->plFinalVec,s->totLength,maxVec,amaxTan,jmaxTan);
                       return -1; //出错
                    }
            }
    }else{ //有解，搜索最大速度
        //二值法搜索最大速度，
        //输出 7段加减速时间time[7],加速与减速区加加速度jplan[2]，最大规划速度VmaxPlan,匀速起点和末点VmaxStartEndPos , return 0 无解， 1 有解
       flag1 = sqSearchVmax(sq, s, s->plInitVec,s->plFinalVec,maxVec,amaxTan,jmaxTan, time1,  jplan1,&VmaxPlan,  VmaxStartEndPos,&search_number);
     //  printf("sqPlanSegment221: Vmax=%f,jplan1[0]=%f,time[6]=%f,s->plInitVec=%f,s->plFinalVec = %f, s->totlength=%f,amaxTan=%f,jmaxTan = %f,maxVec=%f,s->inivec=%f,sq->laststate[1]=%f\n",VmaxPlan,jplan1[0],time1[6],s->plInitVec,s->plFinalVec,s->totLength,amaxTan,jmaxTan,maxVec,s->initVec,sq->lastState[1]);
    }

    //记录规划结果
    if (flag == 0){ // 无解，搜索了末点速度，记录
        s->plFinalVec = EndPosVec[1];
            sq->timeForRun[0] = time[0];  sq->timeForRun[1] = time[1];  sq->timeForRun[2] = time[2];
            if (s->plFinalVec < 1e-9){
            //    sq->timeForRun[3] = 1e9;//无穷大????
                sq->timeForRun[3] = time[2]; //直接跳过
                s->plFinalVec = 0.0;
            }else{
            sq->timeForRun[3] = time[2] + (s->totLength - EndPosVec[0])/ s->plFinalVec;
            }
            sq->timeForRun[4] =  sq->timeForRun[3];sq->timeForRun[5] =  sq->timeForRun[3];sq->timeForRun[6] =  sq->timeForRun[3];
            sq->CruiseStarEndPos[0] = EndPosVec[0]; sq->CruiseStarEndPos[1] = s->totLength;
            sq->incJerk = jplan; sq->decJerk = 0.0;
            sq->vmaxForPlan = s->plFinalVec;
            printf("sqplan3:s->id=%d,sq->timeForRun[0]=%f,timeForRun[1]=%f,timeForRun[2]=%f,timeForRun[3]=%f,timeForRun[4]=%f,timeForRun[5]=%f,timeForRun[6]=%f\n",s->ID,sq->timeForRun[0],sq->timeForRun[1],sq->timeForRun[2],sq->timeForRun[3],sq->timeForRun[4],sq->timeForRun[5],sq->timeForRun[6]);

    }else{
        if (flag1 != 0){ // 搜索最大速度有解
            memcpy(sq->timeForRun,time1,7*sizeof(double));
            sq->CruiseStarEndPos[0] = VmaxStartEndPos[0]; sq->CruiseStarEndPos[1] = VmaxStartEndPos[1];
            sq->incJerk = jplan1[0]; sq->decJerk = jplan1[1];
            sq->vmaxForPlan = VmaxPlan;

          // printf("sqplan4:s->id=%d,sq->timeForRun[0]=%f,timeForRun[1]=%f,timeForRun[2]=%f,timeForRun[3]=%f,timeForRun[4]=%f,timeForRun[5]=%f,timeForRun[6]=%f\n",s->ID,sq->timeForRun[0],sq->timeForRun[1],sq->timeForRun[2],sq->timeForRun[3],sq->timeForRun[4],sq->timeForRun[5],sq->timeForRun[6]);

           // diagnostics("flag1=%d\n",flag1);
         //   diagnostics("22: Vmax=%d,jplan1[0]=%d,time[6]=%d\n",(int)(VmaxPlan*1000),(int)(jplan1[0]),(int)(time1[6]));
        }else{ //搜索最大速度无解，此时s->plFinalVec就是解
         //        diagnostics("33\n");
                sq->timeForRun[0] = time[0];  sq->timeForRun[1] = time[1];  sq->timeForRun[2] = time[2];
                if ( maxVec <1e-9){ // 调速为0,让其停在中途
                     sq->timeForRun[3] = 1e9;//无穷大
                     s->plFinalVec = 0.0;
                }else if(s->plFinalVec <1e-9){
                    sq->timeForRun[3] = time[2]; // 调速不为0,但没搜到解，后续的就直接跳过
                }else{
                    sq->timeForRun[3] = time[2] + (s->totLength - sEnd)/ s->plFinalVec;
                }
                sq->timeForRun[4] =  sq->timeForRun[3];sq->timeForRun[5] =  sq->timeForRun[3];sq->timeForRun[6] =  sq->timeForRun[3];
                sq->CruiseStarEndPos[0] = sEnd; sq->CruiseStarEndPos[1] = s->totLength;
                sq->incJerk = jplan; sq->decJerk = 0.0;
                sq->vmaxForPlan = s->plFinalVec;

                printf("sqplan5:s->id=%d,sq->timeForRun[0]=%f,timeForRun[1]=%f,timeForRun[2]=%f,timeForRun[3]=%f,timeForRun[4]=%f,timeForRun[5]=%f,timeForRun[6]=%f\n",s->ID,sq->timeForRun[0],sq->timeForRun[1],sq->timeForRun[2],sq->timeForRun[3],sq->timeForRun[4],sq->timeForRun[5],sq->timeForRun[6]);

        }
    }

   // diagnostics("as->id=%d,as->linked=%d,as->totlength=%d,sq->incJerk=%d,as->decjerk=%d,sq->start[0]=%d,start[1]=%d,start[2]=%d\n",s->ID,s->linkedToPrevSeg,(int)(s->totLength*1000),(int)(sq->incJerk),(int)(sq->decJerk),(int)(sq->startState[0]*1000),(int)(sq->startState[1]*1000),(int)(sq->startState[2]*1000));
  //  diagnostics("t0=%d,t1=%d,t2=%d,t3=%d,t4=%d,t5=%d,t6=%d\n",(int)(sq->timeForRun[0]*1000000),(int)(sq->timeForRun[1]*1000000),(int)(sq->timeForRun[2]*1000000),(int)(sq->timeForRun[3]*1000000),(int)(sq->timeForRun[4]*1000000),(int)(sq->timeForRun[5]*1000000),(int)(sq->timeForRun[6]*1000000));
  //    diagnostics("as->id=%d,as->plInitVec=%d,as->plFinalVec=%d,sq->vmaxForPlan=%d,CruiseStarEndPos[0]=%d,CruiseStarEndPos[1]=%d\n",s->ID,(int)(s->plInitVec*1000),(int)(s->plFinalVec*1000),(int)(sq->vmaxForPlan*1000),(int)(sq->CruiseStarEndPos[0]*1000),(int)(sq->CruiseStarEndPos[1]*1000));

    // 当前末速已规划，下一段初速需要修改
    if(cursor->nextSegment !=0){
        cursor ->nextSegment->plInitVec = s->plFinalVec;
    }

        memcpy(sq->startState,sq->lastState,4*sizeof(double));

   //     printf("sqPlan: laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);



//统计运行时间
  //  s->planningDone = 1;


    endclocks = rtapi_get_time() - startclocks;
    time_plan=endclocks;

    return 0;
}

static int sqPlanSegmentForAjustFeed(SEGMENTQUEUE * sq, SEGMENT * s) {

    int startclocks = 0, endclocks = 0;

    double maxVec;
    double amaxTan;
    double jmaxTan;

    SEGMENT *cursor;
    double FinalVec;

    startclocks = rtapi_get_time();

    if (sq == 0) {
        diagnostics("Error1  in sqPlanSegmentForAjustFeed()\n");
        return -1;
    }

    /* check is s is a valid pointer */
    if (s == 0) {
        diagnostics("Error2 in sqPlanSegmentForAjustFeed()\n");
        return -1;
    }

    /* check if this segment is linked to its predecessor (which should never
     happen!) */
    if (s->active == 0) { //调速必须在正在运行过程中
        diagnostics("s->ID=%d,s->numsegment=%d,Error3 in sqPlanSegmentForAjustFeed()\n",s->ID,s->numLinkedSegs);
        return -1;
    }

    if ((maxVec = sqGiveMaxVec(sq, s) ) < 1e-3) {
      //调速为0
        maxVec = 0.0;
     //   diagnostics("s->ID=%d,s->numsegment=%d,Error4 in sqPlanSegmentForAjustFeed()\n",s->ID,s->numLinkedSegs);
      //  return -1;
    }

    //defaultVel = maxVec; //add by lsj, on 08.2.27，该值会随着最大速度的调整而改变

    /* find the minimum value for the maximum tangential acceleration for this
     chain of segments beginning with s */
    amaxTan = sqGiveMinAmaxTan(s); //获取逻辑段加速度
    jmaxTan = sqGiveMinBetamaxTan(s);//获取逻辑段加加速度
    if (amaxTan<=1e-6){
         diagnostics("Error in sqPlanSegmentForAjustFeed():amaxTan=0\n");
         return -1;
    }
    if (jmaxTan<=1e-6){
         diagnostics("Error in sqPlanSegmentForAjustFeed():betamaxTan=0\n");
         return -1;
    }

  //  diagnosticsOff("AmaxTan in sqplansegment  = %d \n", (int) (ceil(amaxTan)));
  //找到末点速度

    cursor = s;
    while (cursor->nextSegment != 0 && cursor->nextSegment->linkedToPrevSeg) {
        cursor = cursor->nextSegment;
    }



    if (sq->feedOverrideFactor >=1.0) {
        FinalVec = cursor->finalVec;
    } else {
        if (cursor->nextSegment != 0
                && sqGiveMaxVecWithFeedOverrate(sq,cursor->nextSegment) < sqGiveMaxVecWithFeedOverrate(sq,cursor))
            FinalVec = min(cursor->finalVec,  sqGiveMaxVecWithFeedOverrate(sq,cursor->nextSegment));
        else
            FinalVec = min(cursor->finalVec, sqGiveMaxVecWithFeedOverrate(sq,cursor));
    }

    if (FinalVec > maxVec) FinalVec = maxVec;

    //vEnd为末点速度，time为规划时间，jplan为规划加加速度，sEnd为最终达到的位置
    //返回值，0 为无解，1为有解
    int flag, flag1;
    double time[3];
    double jplan;
    double sEnd;
    double time1[7];
    double jplan1[2];
     double VmaxPlan;
    double VmaxStartEndPos[2];
    double EndPosVec[2];
    int search_number;
    //测试新速是否ok？
    flag = sqTestLastVec(sq,  s, FinalVec,maxVec,amaxTan,jmaxTan,  time,&jplan,&sEnd);
    if (flag == 0){//无解

            // 输出参数：time[3] 规划加减速时刻，jplan规划加加速度，EndPosVec[0]末点位置，EndPosVec[1]末点速度；
            flag1 = sqSearchEndVec(sq, s, s->plFinalVec,FinalVec,maxVec,amaxTan,jmaxTan,  time,&jplan, EndPosVec,&search_number);
            if (flag1 == 0){ // 正常不可能发生
                diagnostics("sqPlanSegmentForAjustFeed : s->id=%d no solution，adjust is negect\n",s->ID);
                return -1; //不再调速
            }
    }else{ //有解，搜索最大速度
        //二值法搜索最大速度，
        //输出 7段加减速时间time[7],加速与减速区加加速度jplan[2]，最大规划速度VmaxPlan,匀速起点和末点VmaxStartEndPos , return 0 无解， 1 有解
        flag1 = sqSearchVmax(sq, s, sq->lastState[1],FinalVec,maxVec,amaxTan,jmaxTan, time1,  jplan1,&VmaxPlan,  VmaxStartEndPos,&search_number);// sq->startState[1] 有问题
    }

    //记录规划结果
    if (flag == 0){ // 无解，搜索了末点速度，记录
        s->plFinalVec = EndPosVec[1];
            sq->timeForRun[0] = time[0];  sq->timeForRun[1] = time[1];  sq->timeForRun[2] = time[2];
            if (s->plFinalVec < 1e-9){
              //  sq->timeForRun[3] = 1e9;//无穷大?????
                 sq->timeForRun[3] = time[2]; //直接跳过
                s->plFinalVec = 0.0;
            }else{
            sq->timeForRun[3] = time[2] + (s->totLength - EndPosVec[0])/ s->plFinalVec;
            }
            sq->timeForRun[4] =  sq->timeForRun[3];sq->timeForRun[5] =  sq->timeForRun[3];sq->timeForRun[6] =  sq->timeForRun[3];
            sq->CruiseStarEndPos[0] = EndPosVec[0]; sq->CruiseStarEndPos[1] = s->totLength;
            sq->incJerk = jplan; sq->decJerk = 0.0;
            sq->vmaxForPlan = s->plFinalVec;

    }else{
        if (flag1 == 1){ // 搜索最大速度有解
            memcpy(sq->timeForRun,time1,7*sizeof(double));
            sq->CruiseStarEndPos[0] = VmaxStartEndPos[0]; sq->CruiseStarEndPos[1] = VmaxStartEndPos[1];
            sq->incJerk = jplan1[0]; sq->decJerk = jplan1[1];
            sq->vmaxForPlan = VmaxPlan;
            s->plFinalVec = FinalVec;
           // diagnostics("flag1=%d\n",flag1);
         //   diagnostics("22: Vmax=%d,jplan1[0]=%d,time[6]=%d\n",(int)(VmaxPlan*1000),(int)(jplan1[0]),(int)(time1[6]));
        }else{ //搜索最大速度无解，此时s->plFinalVec就是解
         //        diagnostics("33\n");
               return -1;
        }
    }

    // 当前末速已规划，下一段初速需要修改
    if(cursor->nextSegment !=0){
        cursor ->nextSegment->plInitVec = s->plFinalVec;
    }

    memcpy(sq->startState,sq->lastState,4*sizeof(double));
printf("sqPlanSegmentForAjustFeed:laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);
//统计运行时间
    endclocks = rtapi_get_time() - startclocks;
    if (endclocks > 250000) {
        rtapi_print("s->ID = %d,sq->numsegment=%d,sqPlanSegmentForAjustFeed time = %d,search_number=%d\n",	 s->ID, sq->numSegments, endclocks,search_number);
    }

    return 0;
}


/* function to link s2 to s1
 Here it is assumed that s1 and s2 are indeed successive segments */
static int sqLinkSegments(SEGMENT * s1, SEGMENT * s2, int priority) {

    //diagnostics("Linking %d to %d,s1->numLinkedSegs=%d,s2->numLinkedSegs=%d\n", s1->ID, s2->ID,s1->numLinkedSegs,s2->numLinkedSegs);
    if (s1 == 0) {
        diagnostics("Error 1 in sqLinkSegments() \n");
        return -1;
    }
    if (s2 == 0) {
        diagnostics("Error 2 in sqLinkSegments() \n");
        return -1;
    }

/*******************************************************************/
 //add by heyuebang in 2015-12-13
    if (s2->linkedToPrevSeg == 1) {
     //diagnostics("Error 2 in sqLinkSegments()\n");
        return 0;
     }
/*******************************************************************/
    /* if s1 is active, linking should not be done */
    //if(iii==0){
    if (s1->active == 1) {
        diagnostics(
                "Error 3 in sqLinkSegments(),s1->active == 1,s1->ID=%d,s2->ID=%d\n",
                s1->ID, s2->ID);//打印此错没什么影响，只是证明线段加的不够快，或者一开始的时候会报一次ghf-2014/10/24
        //return -1;
        return 0;
    }
    //}

    /* find the first element in the series of linked segments of which s1 is
     the last one */
    while (s1->linkedToPrevSeg) {
        if (s1 == 0) {
            diagnostics("Error 4,Panic: NULL pointer in sqLinkSegments\n");
            return -1;
        }
        s1 = s1->prevSegment;
    }

    if (((s1->numLinkedSegs > SQ_MAX_NUM_LINKED_SEGS) || (s2->numLinkedSegs > SQ_MAX_NUM_LINKED_SEGS))
            && priority == SQ_LOW_LINKING_PRIORITY)        //预处理时段不够长，必须连，
        return 0;

    s1->numLinkedSegs += s2->numLinkedSegs + 1;
    s1->totLength += s2->totLength;
    s2->linkedToPrevSeg = 1;
    s2->numLinkedSegs = 0;
    s2->totLength = s2->length;

    return 0;
}

/*
 强制牵连，保证开始运行段够长。一般调整调整倍率后可能造成运行时，线段不够长。如，本来够长，但调整倍率，使得末点速度改变，而初始速度没有改变，前段不能调速。则可能造成不够长。
*/
static int sqForceForwardLinkSegment(SEGMENTQUEUE * sq, SEGMENT * s) {

    SEGMENT *cursor,*temp;
    int done, counter;

    int flag;
    double time[3];
    double jplan;
    double sEnd;
    double amaxTan,jmaxTan;

    int startclocks = 0, endclocks = 0;
 startclocks = rtapi_get_time();
//    if (s->ID >=125){
//        printf("in sqForceForwardLinkSegment");
//    }

    if (sq == 0 || s == 0) {
        diagnostics("Error1 in sqForceForwardLinkSegment,1");
        return -1;
    }

    cursor = s; //hyb-2014/7/2
    done = 0;
    counter = 0;

    s->initVec = s->plInitVec; //前段按着这个速度结束，没法改变

    while (!done) {
        counter++;
        if (counter > sq->size) {
            /* we have browsed through the whole queue and can't
             get out of the loop.... */
            diagnostics(
                    "Error2,Can't get out of the loop in sqForceForwardLinkSegment\n");
            return -1;
        }
        if (cursor->nextSegment == 0) {
            done = 1;
        } else if (cursor->nextSegment->linkedToPrevSeg == 1 )
        {
            cursor = cursor->nextSegment;
            if (cursor == 0) {
                /* this is VERY unlikely to happen, since we already checked
                 that a few lines before, but just in case...... */
                diagnostics( "Error3,Panic: cursor = NULL in sqForceForwardLinkSegment\n");
                return -1;
            }
        }else {
            int iflag=0;
                if (max(s->initVec,cursor->finalVec) *sq->cycleTime > s->totLength- sq->startState[0] || (cursor->lastSegFlag == 1)|| s->totLength - sq->startState[0]< 1e-6){
                    iflag = 1;
                }else{
                  amaxTan = sqGiveMinAmaxTan(s);
                  jmaxTan = sqGiveMinBetamaxTan(s);

            flag = sqTestLastVec(sq,  s, cursor->finalVec,0.0,amaxTan,jmaxTan,  time,&jplan,&sEnd);

            if (flag==0)
            {
                iflag = 2;
                //段过多，且足够一步走，就不加了，免得计算量过大
                if (s->numLinkedSegs > 50 && s->initVec < cursor->finalVec) break;

                if ( cursor->nextSegment!=0 && cursor->nextSegment->lastSegFlag==1){ //当前段的下一段为最后一段，则没必要链接，
                                        break;

                   }
                 }
                }

            if(iflag != 0){
               // 一步不够走, s-cursor段最大速度肯定达不到，修改掉
                if (cursor->lastSegFlag !=1 && s->initVec <= cursor->finalVec){
                        temp = s;
                        while (temp != cursor) {
                            temp->maxVec = min(temp->maxVec,cursor->finalVec);
                            temp = temp->nextSegment;
                        }
                          cursor->maxVec = max(cursor->initVec,cursor->finalVec);


                          }

                //链接
                cursor = cursor->nextSegment;
                if (cursor == 0) {
                    /* again, this is very unlikely to happen */
                    diagnostics(
                            "Warning,Panic: cursor = NULL in sqForceForwardLinkSegment\n");
                    return 0;
                }

//                if(s->ID <12)
//                    printf("bef: forcelink: s->id=%d,s->numlink=%d,iflag=%d,s->totlength=%f,s->intV=%f,curcor->finalV=%f\n",s->ID,s->numLinkedSegs,iflag,s->totLength,s->initVec,cursor->finalVec);

                if (-1 == sqLinkSegments(s, cursor, SQ_HIGH_LINKING_PRIORITY)) {
                    diagnostics("Error6 in sqForceForwardLinkSegment,3\n");
                    return -1;
                }
//                if(s->ID <12)
//                    printf("aft: forcelink: s->id=%d,s->numlink=%d,iflag=%d,s->intV=%f,curcor->finalV=%f\n",s->ID,s->numLinkedSegs,iflag,s->initVec,cursor->finalVec);


            }else
                /* 够走一步 */
                done = 1; /* no further linking needed */
        }
        if (cursor == sq->queue + sq->end) {
            /* sq->end points to the first free position in the ring buffer
             so there's no segment at that position. So if cursor points to
             that position, something is very wrong */
            diagnosticsOff("End of queue reached, no further linking possible\n");
            done = 1;
        }
    }diagnosticsOff("... done linking forward\n");

    double T;

    T = sq->cycleTime;

    double totlength;
    int number=10,j=0;

    if (s->numLinkedSegs <9 && sq->numSegments>20){
        totlength = 0;
        cursor = s;
        for (j=0;j<number;j++){
            totlength += cursor->length;
            cursor = cursor->nextSegment;
            if (cursor == 0) break;
        }
        if (j==0){
            //不可能发生
            number = 1;
        }else
            number = j;
    }else{
        totlength = s->totLength;
        number = s->numLinkedSegs + 1;
    }

       if(cursor->nextSegment == 0 || cursor->nextSegment->lastSegFlag==1)
            sq->limit_vmax =totlength/number/T;
        else  if(sq->numSegments < max(s->numLinkedSegs * 3.0,100))
           sq->limit_vmax = 1.5*totlength/number/T; //限制最大速度，保证平均两段够一步走
       else if (sq->numSegments < max(s->numLinkedSegs * 5.0,250))
            sq->limit_vmax = 2.0*totlength/number/T;//限制最大速度，保证平均两段够一步走
       else if (sq->numSegments < max(s->numLinkedSegs * 10.0,500))
            sq->limit_vmax = 2.5*totlength/number/T;//限制最大速度，保证平均两段够一步走
       else
           sq->limit_vmax =  5.0*totlength/number/T;

      // printf("sqFordwardLinkSegment:s->ID=%d,s->totLength=%f,s->numLinkedSegs=%d,sq->limit_vmax=%f,sq->numSegments=%d,s->initV=%f,cursor->finalV=%f\n",s->ID,s->totLength,s->numLinkedSegs,sq->limit_vmax,sq->numSegments,s->initVec,cursor->finalVec);

    endclocks = rtapi_get_time() - startclocks;
     time_forward=endclocks;
    if (endclocks > 250000) {
      //  rtapi_print("sqForceForwardLinkSegment:s->ID = %d,s->numlinked=%d,endclocks=%d\n",	s->ID,s->numLinkedSegs,endclocks);
    }
    return 0;
}

//后连，保证够长
static int sqBackwardLinkSegment(SEGMENTQUEUE * sq, SEGMENT * start, SEGMENT * s,
        double feedOverride) {
    /* function that checks whether segment s needs to be linked to its
     predecessor(s) based upon the finalInc. It is possible that
     this requires linking of a set of preceding segments to make sure that
     the final increment can be reached. */

    SEGMENT *cursor, *temp,*prevset_start;
    int done, counter;
    int linkcrit;
    int flag = 0;

    double detaV,t1,t2;
    double amaxTan,jmaxTan;

    diagnosticsOff("Checking whether segment %d needs to be linked backward...\n", s->ID);

    if (sq == 0 || s == 0) {
        diagnostics("Error1 in sqBackwardLinkSegment\n");
        return -1;
    }

    cursor = start; //hyb-2014/7/2
    done = 0;
    counter = 0;

    while (!done) {
        counter++;
        if (counter > sq->size) {
            /* we have browsed through the whole queue and can't
             get out of the loop.... */
            diagnostics("Error2,Can't get out of the loop in sqBackwardLinkSegment\n");
            return -1;
        }

        if (cursor->active == 1){
            return 0;
        }else if (cursor->linkedToPrevSeg == 1)
        /* it is already linked */
        {
            cursor = cursor->prevSegment;
            if (cursor == 0) {
             //   diagnostics("Error3,Panic: cursor = NULL in sqBackwardLinkSegment\n");
                return -1;
            }
        } else if (cursor->initVec < s->finalVec + 1e-6)
            /* 有可能一直连下去，但这种可能性几乎为0 */
            done = 1;
        else {
            if (-1
                    == (linkcrit = sqLinkCriterion(sq, cursor, s,
                            cursor->totLength, feedOverride))) { //hyb-2014/7/2
              //  diagnostics("Error4 in sqBackWardLinkSegment\n");
                return -1;
            } else if (linkcrit == SQ_LINKING_NEEDED) {
                //不够长，其最大速度没有意义，改掉。
                temp = cursor;
                s->maxVec = min(s->maxVec,cursor->initVec);
                while (temp != s){
                    temp ->maxVec = min(cursor->initVec,temp->maxVec);
                    temp=temp->nextSegment;
                }

                cursor = cursor->prevSegment;
                if (cursor == 0 || cursor->active==1) {
                //    diagnostics(
                  //          "Warning,Panic: cursor = NULL or active=1 in sqBackwardLinkSegment\n");
                    return -1;
                  //  return 0;
                }

                prevset_start = cursor;
                //找到前段的头
                while(prevset_start->linkedToPrevSeg){
                    prevset_start = prevset_start->prevSegment;
                    if (prevset_start==0 || prevset_start->active==1){
                     //   diagnostics("Warning: prevset_start=NULL or active=1 in sqBackwardLinkSegment\n");
                        return -1;
                    }
                }

                    // 测试prevset_start走到s->finalVec够不够长，如果够长，表明该段足够长了，再连接就多了
                    //这种情况就不应该连接cursor和cursor->nextSegment,否则有可能造成一直后连。

                    amaxTan = sqGiveMinAmaxTan(prevset_start);
                    jmaxTan = sqGiveMinBetamaxTan(prevset_start);

                    detaV = fabs(s->finalVec - prevset_start->initVec);
                      if (detaV*jmaxTan < amaxTan*amaxTan){
                          t1 = sqrt(detaV*jmaxTan)/jmaxTan;
                          t2 = t1;
                      }else{
                          t1 = amaxTan/jmaxTan;
                          t2 =detaV/amaxTan;
                      }

                      detaV=0.5*fabs(s->finalVec + prevset_start->initVec)*(t1+t2);
                      if (detaV < prevset_start->totLength &&prevset_start->numLinkedSegs >= 50) { // 够长不再连接
                          flag = 1;
                            done = 1;
                    }else{
                            if (-1 == sqLinkSegments(cursor, cursor->nextSegment,
                                    SQ_HIGH_LINKING_PRIORITY)) { //当前逻辑段跟前面一个逻辑段链接ghf
                             //   diagnostics("Error6 in sqBackwardLinkSegment\n");
                                return -1;
                            }diagnosticsOff("backward %d",cursor->nextSegment->ID);
                      }
            } else
                done = 1; /* no further linking needed */
        }
        if (cursor == sq->queue + sq->start) {
            diagnosticsOff
            ("Beginning of queue reached, no further linking possible\n");
            done = 1;
        }
    }diagnosticsOff("... done linking backward\n");

    if (flag == 1){
        if (cursor == 0 || cursor->active==1) {
       //     diagnostics("Warning 1: cursor = NULL or active=1 in sqBackwardLinkSegment\n");
           // return -1;
            return -1;
        }
        cursor = cursor->nextSegment;
        //前段够长，cursor ----->s 需要调整初始速度
        if(cursor->initVec <= s->finalVec) return 0;
        if (cursor->totLength < s->finalVec*sq->cycleTime){
            //不够一步走，将初速改成末速一致就好了。
            cursor->initVec = s->finalVec;
            cursor->prevSegment->finalVec = s->finalVec;
            return 0 ;
        }

        // 搜索初速
        double solutionV = s->finalVec;
        double NsolutionV = cursor->initVec;
        double NewV;
        int i;
        amaxTan = sqGiveMinAmaxTan(cursor);
        jmaxTan = sqGiveMinBetamaxTan(cursor);

        for (i=0;i<10;i++){
            NewV = 0.5*(NsolutionV + solutionV);
        detaV = fabs(s->finalVec - NewV);
          if (detaV*jmaxTan < amaxTan*amaxTan){
              t1 = sqrt(detaV*jmaxTan)/jmaxTan;
              t2 = t1;
          }else{
              t1 = amaxTan/jmaxTan;
              t2 =detaV/amaxTan;
          }

          detaV=0.5*fabs(s->finalVec + NewV)*(t1+t2);
          if (detaV < cursor->totLength) { // 够长
              solutionV = NewV;
              if (cursor->totLength - detaV < NewV * sq->cycleTime){ // 剩下的不够一步,提前退出
                  break;
              }

        }else{
              NsolutionV = NewV;
          }
        } // end for

        // NewV 就是解
        cursor = cursor->prevSegment;
        if (cursor == 0 || cursor->active==1) {
         //   diagnostics( "Warning 2: cursor = NULL or active=1 in sqBackwardLinkSegment\n");
            return -1;
          //  return 0;
        }

        cursor->finalVec = NewV;
        cursor->nextSegment->initVec = NewV;
    }

    return 0;
}

//插入圆弧参数初始化
static int sqInsertInitCircle(SEGMENTQUEUE * sq, EmcPose start, EmcPose end,
                  PmCartesian center, PmCartesian normal, int ID, int G_number,
                  SEGMENT *insertseg){

 //  SEGMENT *newseg;
  PmCircle circle;
 // EmcPose start;
  PmCartesian helix;
  double absHelix;
  PmPose start_pose, end_pose;




/********************************************************************************/
  GMODE G_mode;

/* please add this parameter in function sqAddLine, such as
     sqAddLine(SEGMENTQUEUE * sq, EmcPose end, int ID,int G_mode)
  and delete the following two line.
*/

   if (G_number == 0){
       sq->G_mode_0.maxAcc_circle= sq->G_mode_0.maxAcc;
       sq->G_mode_0.maxAcc_nCircle= sq->G_mode_0.maxAcc;
       G_mode = sq->G_mode_0;

   }
   else{
        G_mode = sq->G_mode_1;

   }

/********************************************************************************/
  /* used to calculate the maximum tangential acceleration */
 // double rpow2, A, topIncPow2;

  /* check for full */
  if (sq->numSegments == sq->size) {
    diagnostics("Panic!!!!, segmentqueue overflows!!!\n");
    return -1;
  }


  EmcPose_to_pmpose(start, &start_pose);
  EmcPose_to_pmpose(end, &end_pose);
  pmCircleInit(&circle, start_pose, end_pose, center, normal, 0);


  if (circle.angle == 0) {
    /* only set ID of last appended motion */
    sq->lastAppMotionID = ID;
    return -1;
  }



  /* Calculate the helix gradient in normal direction */
  pmCartScalDiv(circle.rHelix, circle.angle, &helix);
  pmCartMag(helix, &absHelix);


  /* fill segment parameter fields */
  insertseg->ID = ID;
  insertseg->type = SQ_CIRCULAR;
  insertseg->circle = circle;

  insertseg->helixRadius = sqrt(circle.radius * circle.radius
                 + absHelix * absHelix);


  //计算总长
  double circle_length;
  double k[9];
  double mag;
  circle_length = circle.angle * insertseg->helixRadius;
  k[0] = circle_length;
  k[1] = circle_length;
  k[2] = circle_length;
  k[3] = end.a - start.a;
  k[4] = end.b - start.b;
  k[5] = end.c - start.c;
  k[6] = end.u - start.u;
  k[7]= end.v - start.v;
  k[8]= end.w - start.w;
  int i;
  //按给定比率求九轴长度
  mag = fabs(circle_length);
//  for (i=2;i<9;i++)
//  {
//      mag += sq->axis_rate[i]*sq->axis_rate[i]*k[i]*k[i];
//  }
//  mag = sqrt(mag);

  // 获取各轴分量
  for (i=0;i<9;i++) k[i]/=mag;

  memcpy(insertseg->length_coefficient,k,9*sizeof(double));

  insertseg->length = mag;


  insertseg->totLength = insertseg->length;
  insertseg->givenLength = insertseg->length;

  insertseg->start = start;
  insertseg->end = end;

    //  diagnostics("insertseg->ID=%d, insertseg->length=%d,insertseg->R=%d,insertseg->angle=%d\n",insertseg->ID,(int)(insertseg->length*1000),(int)(insertseg->circle.radius*1000),(int)(insertseg->circle.angle*1000*180/3.1415926));
 // newseg->maxInc = min(sq->feed * sq->cycleTime, sqrt(sq->maxAcc *circle.radius) * sq->cycleTime);
//****************************************************************************************************
  double kabc = 0;
  for (i=3;i<9;i++)
  {
      kabc = max(kabc,fabs(insertseg->length_coefficient[i])*sq->axis_rate[i]);
  }
    if((kabc>1) &&(sq->abGoAlongXYZ !=0)) kabc = 1.0/kabc;
  else kabc = 1.0;

//changed by heyuebang in 2/17/2014
     if (G_number == 0){
            insertseg->maxVec = min(G_mode.maxV, sqrt(G_mode.maxAcc_circle * circle.radius ));

      }
     else{
            insertseg->maxVec = sqrt(G_mode.maxAcc_circle * circle.radius) ;

                  }
     insertseg->maxVec = min(insertseg->maxVec, G_mode.maxV)*kabc;

     insertseg->givemaxVec = insertseg->maxVec;
//***************************************************************************************************

  insertseg->finalVec = 0;
  insertseg->plInitVec = 0;
  insertseg->plFinalVec = 0;
 // insertseg->planningDone = 0;
  insertseg->active = 0;
  insertseg->numLinkedSegs = 0;
  insertseg->linkedToPrevSeg = 0;
  insertseg->nextSegment = 0;
  insertseg->gmodetype = G_number;

  insertseg->amaxTan = G_mode.maxAcc_circle;

insertseg->amaxNorm =G_mode.maxAcc_nCircle;
  insertseg->jmaxTan= G_mode.maxJerk;//this is wrong,not used.

// insertseg->type_circle_seg=0;

 insertseg->lastSegFlag = 1;


 if (absHelix != 0) {
    insertseg->amaxTan = min(insertseg->amaxTan, G_mode.maxAcc * insertseg->helixRadius / absHelix);
  }

  /* set last Appended Motion ID */
  sq->lastAppMotionID = ID;
//  diagnostics("add circle!\n");
  return 0;
}


//***************************************************************************************************
//插入圆弧
static int sqInsertCircle(SEGMENTQUEUE * sq,  SEGMENT *s1, SEGMENT *s2){
            double costheta;
            double sintheta;
            double l,R;
            PmCartesian center;
            PmCartesian normal;
            PmCartesian v1;
          //  PmCartesian v2;
          //  PmCartesian v3;
             EmcPose start, end;
             SEGMENT *newseg;
             SEGMENT *cursor;
            PmPose start_pose, end_pose;

             double tolerance;


          //   if (s2->ID < 30)
          //    diagnostics("bef:s1->ID=%d,s2->ID=%d\n",s1->ID,s2->ID);

             if (s1->type != SQ_LINEAR) return 0;//如果s1不是直线，退出
             if (s2->type != SQ_LINEAR) return 0;//如果s2不是直线，退出
                //如果之间夹角小于90度，或者大于180度，退出,这个不能删除，删除可能导致多走一圈
             if( s2->angle > PM_PI - 0.001 || s2->angle < 0.001)
                 return 0;


             if((s2->angle <= sq->optimized_angle) &&((s1->gmodetype==1)&&(s2->gmodetype==1))) return 0;

             if (sq->numSegments < 2) return 0; //如果剩下的段只有1段了，退出

//             if (s2->ID >343 && s2->ID<376)
//                 printf(
//                         "Warning: s2->ID=%d,newseg->gtype=%d,prevseg->gtype=%d\n",s2->ID,s2->gmodetype,s1->gmodetype);

             //根据类型不同(g0,g1)设置不同的容差
             if (s2->gmodetype == 0 && s1->gmodetype==0){
                 tolerance =  sq->circle_tolerance_g0;
             }
             else{
                  tolerance =  sq->circle_tolerance_g1;
                //  printf("instert circle s1=%d,s2=%d,tolerance=%f\n",s1->ID,s2->ID,tolerance);
             }


             if (tolerance < 0.001) return 0;
              //printf("now get the filter parameter:%f\n",tolerance);

             //新增加一段
            newseg = sq->queue + sq->end;
               /* ...and update the ring buffer properties */
               sq->end = (sq->end + 1) % sq->size;

            // 将s2赋值给新段，s2将作为插入的圆弧段
               *newseg = *s2;

            //计算插入圆弧半径和需要的直线长度
             costheta = cos(s2->angle*0.5);
             sintheta = sin(s2->angle*0.5);
             R = tolerance * sintheta / (1.0- sintheta);
             l = R*costheta/sintheta;

             //长度不能大于s1和s2一半长度
            if (s1->prevSegment == 0 || s1->prevSegment->active == 1 || s1->prevSegment->gmodetype != s1->gmodetype){
                 l = min(l,s1->length *0.5);
            }else{
                  l = min(l,s1->length);
            }
             l = min(l, 0.5*s2->length);
             //根据l重新计算插入圆弧半径
             R = l*sintheta / costheta;

             if (R >10000.0) return 0; //半径太大，忽略。

              //计算圆心和方向
             pmCartCartCross(s1->line.uVec, s2->line.uVec, &normal);
             pmCartUnit(normal, &normal);

             pmCartScalMult(s1->line.uVec, l, &v1);
             pmCartCartSub(s1->end.tran, v1, &end.tran);
             pmCartCartCross(normal, s1->line.uVec, &v1);
             pmCartUnit(v1, &v1);

             pmCartScalMult(v1, R, &v1);

             pmCartCartAdd(end.tran, v1, &center);
             pmCartScalMult(s2->line.uVec, l, &v1);
             pmCartCartAdd(s2->start.tran, v1, &start.tran);

//changing the length of the preseg;
             int gmodetype;
             gmodetype = s1->gmodetype || s2->gmodetype;
            if (-1==sqInsertInitCircle(sq, end, start, center, normal, s2->ID, gmodetype, s2)){
                diagnostics("Error 4 in sqInsertCircle()\n");
                return -1;
               }
             //*s2 = *cursor;
             s2->amaxTan = min(s1->amaxTan,newseg->amaxTan);
             s2->jmaxTan = min(s1->jmaxTan,newseg->jmaxTan);
             s2->maxVec = max(s2->maxVec,s1->finalVec);
             s2->maxVec  = min(s2->maxVec,s1->maxVec);
              s2->maxVec  = min(s2->maxVec,newseg->maxVec);
              s2->givemaxVec = s2->maxVec;
             s2->initVec = min(s1->maxVec,s2->maxVec);
             s1->finalVec = s2->initVec;
             s2->finalVec = min(s2->maxVec,newseg->maxVec);
             newseg->initVec = s2->finalVec;
           //  if (newseg->ID < 30)
           //   diagnostics("newseg->ID=%d\n",newseg->ID);
             s1->nextSegment = s2;
             s2->prevSegment = s1;
             s2->nextSegment = newseg;
             newseg->prevSegment = s2;

             s1->end = s2->start;
             newseg->start = s2->end;

             EmcPose_to_pmpose(s1->start, &start_pose);
             EmcPose_to_pmpose(s1->end, &end_pose);
             pmLineInit(&s1->line, start_pose, end_pose);
             EmcPose_to_pmpose(newseg->start, &start_pose);
             EmcPose_to_pmpose(newseg->end, &end_pose);
             pmLineInit(&newseg->line, start_pose, end_pose);

          //   newseg->length -= l ;
             costheta = newseg->length - l;
             newseg->length = sqGiveLength(newseg->start,newseg->end);
             if (fabs(newseg->length-costheta) > 1e-6)
                 diagnostics("newseg:newseg->ID=%d,newseg->length=%d,costheta=%d\n",newseg->ID,(int)(newseg->length*1000),(int)(costheta*1000));
             newseg->givenLength = newseg->length;
             newseg->totLength = newseg->length;

            // s1->length -= l;
             costheta = s1->length - l;
             s1->length = sqGiveLength(s1->start,s1->end);
             if (fabs(s1->length-costheta) > 1e-6)
                 diagnostics("newseg:s1->ID=%d,s1->length=%d,s1=%d\n",s1->ID,(int)(s1->length*1000),(int)(costheta*1000));
             s1->givenLength = s1->length;

             s1->totLength = s1->length;
             if (s1->linkedToPrevSeg){
                 cursor = s1;
                 while(cursor->linkedToPrevSeg)
                     cursor = cursor->prevSegment;
                             cursor->totLength -= l;
             }

        return 0;
}


//static int zerofeedoverflag = -1;

//后连，保证够长
static int sqBackwardForLastSegment(SEGMENTQUEUE * sq, SEGMENT * newseg) {

    SEGMENT *cursor, *temp;
    int done, counter;
    int linkcrit;
    double totlength;

    if (sq == 0 || newseg == 0) {
        diagnostics("Error1 in sqBackwardLinkForLastSegment\n");
        return -1;
    }

    cursor = newseg; //hyb-2014/7/2
    done = 0;
    counter = 0;

    while (!done) {
        counter++;
        if (counter > sq->size) {
            /* we have browsed through the whole queue and can't
             get out of the loop.... */
            diagnostics("Error2,Can't get out of the loop in sqBackwardLinkForLastSegment\n");
            return -1;
        }

        if (cursor->active == 1){
            return 0;
        }else if (cursor->linkedToPrevSeg == 1)
        /* it is already linked */
        {
            cursor = cursor->prevSegment;

            if (cursor == 0) {
                diagnostics("Error3,Panic: cursor = NULL in sqBackwardLinkForLastSegment\n");
                return -1;
            }
        } else if (cursor->initVec < newseg->finalVec + 1e-6)
            /* 有可能一直连下去，但这种可能性几乎为0 */
            done = 1;
        else {
            //计算cursor到newseg的总长
            temp = cursor;
            totlength = cursor->length;
            while (temp!=newseg){
                temp = temp->nextSegment;
                totlength += temp->length;
            }

            if (-1  == (linkcrit = sqLinkCriterion(sq, cursor, newseg, totlength, sq->maxFeedOverrideFactor))) {
                diagnostics("Error4 in sqBackwardLinkForLastSegment\n");
                return -1;
            } else if (linkcrit == SQ_LINKING_NEEDED) {
                cursor = cursor->prevSegment;

                if (cursor == 0 || cursor->active==1) {
                    diagnostics(
                            "Warning,Panic: cursor = NULL or active=1 in sqBackwardLinkForLastSegment\n");
                   // return -1;
                    return 0;
                }

            } else
                done = 1; /* no further linking needed */
        }
        if (cursor == sq->queue + sq->start) {
            diagnosticsOff
            ("Beginning of queue reached, no further linking possible in sqBackwardLinkForLastSegment\n\n");
            done = 1;
        }
    }

    //cursor 是最后一逻辑段的开始，设置标志
                    temp = newseg;
                        counter = 1;
               //         double length;
               //         length = newseg->length;
                    while (temp !=0 && temp->active !=1){

                        temp->lastSegFlag = 1;// 设置最后一逻辑段标志
                        if (temp == cursor) break;
                        temp = temp->prevSegment;
                //        length+= temp->length;

                        counter++;
                        if (counter > sq->size) {
                            diagnostics("Error5,Can't get out of the loop in sqBackwardLinkForLastSegment\n");
                            return -1;
                        }
                    }//end while

         //清除cursor前的标志lastSegFlag
                    if (cursor!=0 && cursor->active !=1){
                        cursor = cursor->prevSegment;
                        counter = 0;
                        while(cursor !=0 && cursor->active !=1){
                            if (cursor->lastSegFlag == 0) break;
                            cursor->lastSegFlag = 0;
                            cursor = cursor->prevSegment;

                            counter++;
                            if (counter > sq->size) {
                                diagnostics("Error6,Can't get out of the loop in sqBackwardLinkForLastSegment\n");
                                return -1;
                            }

                        }
                    }


    diagnosticsOff("... done linking backward\n");
    return 0;
}

//链接处理
static int sqPreprocessSegment(SEGMENTQUEUE * sq, SEGMENT * newseg, GMODE *G_mode) {

    /* this is the stuff that's needs to be done for both linear and
     circular motions. Only sqAddLine and sqAddCircle should call this
     function and it must be used after that sq->end+=1 .
     */

    double cornerVec;

    SEGMENT *prevseg;

    /* check if segment queue has been initialized and if newseg is valid */
    if (sq == 0 || sq->queue == 0 || newseg == 0) {
        diagnostics("Error 1 in sqPreprocessSegment()-- sq not init\n");
        return -1;
    }

   // sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
    /* if this is the first segment.... */
    //第一段或者中途加段不够快，造成numSegments=1;
    if (((sq->end+sq->size-sq->start)%sq->size) == 1) {
        newseg->initVec = 0;
        newseg->start = sq->lastPoint;
        newseg->prevSegment = 0;
        sq->limit_vmax = newseg->length/sq->cycleTime*0.5; //限制最大速度，保证够一步走
        //if (sq->paused != 1 && sq->stepping != 1)
        if (sq->paused != 1 && sq->stepping != 1 && sq->feedOverrideFactor != 0)
        {
            sq->done = 0;//启动runcycle的开关
        }

    } else {
        /* 有两段以上*/
        prevseg = sq->queue + ((sq->end + sq->size - 2) % sq->size);
        //2 means this function only can be used after that the sq->end has been increased by 1 in function addline or addcircle.
        newseg->start = prevseg->end;
        prevseg->nextSegment = newseg;
        newseg->prevSegment = prevseg;

        if (prevseg->active == 1) {
            newseg->initVec = prevseg->finalVec;//前面一段正在跑，新加的不做处理，初速为0
               sq->limit_vmax = newseg->length/sq->cycleTime*0.5; //限制最大速度，保证够一步走
        } else { //开始处理
           //计算拐角速度
            if(prevseg->gmodetype != newseg->gmodetype) {
                if (-1
                        == (cornerVec = sqGiveCornerVelocity(prevseg, newseg, sq->G_mode_1.maxAcc_nLine,sq))) {
                    diagnostics( "Error 2 in sqPreprocessSegment()--sqgivecornerVel error!\n");
                    return -1;
                }
            }
            else {
                if (-1
                        == (cornerVec = sqGiveCornerVelocity(prevseg, newseg, G_mode->maxAcc_nLine,sq) )) {
                    diagnostics( "Error 2_1 in sqPreprocessSegment()--sqgivecornerVel error!\n");
                    return -1;
                }
            }

//****************************************************************************************
            /* the corner velocity shouldn't exceed the maximum
             speeds of both segments */
               newseg->lastSegFlag=1;
            if ((cornerVec > prevseg->maxVec) || (cornerVec > newseg->maxVec))
                cornerVec = min(prevseg->maxVec, newseg->maxVec);
      //调整新段起点速度和prev段末点速度
            newseg->initVec = cornerVec;
            prevseg->finalVec = cornerVec;

              //circlelinkflag为0表示圆弧拟合失败，在这个条件下，如果插入圆弧标志为1,则调用圆弧插入函数
            if ( sq->circle_insert_flag == 1 && sq->axis9_flag==0){
                  if(-1 == sqInsertCircle(sq,  prevseg, newseg)){
                       diagnostics(
                               "Error  in sqPreprocessSegment()--sqInsertCircle error!\n");
                   }
            }

//            if (newseg->ID >343 && newseg->ID<376)
//                printf(
//                        "Warning: newseg->ID=%d,circle_insert_flag=%d,axis9_flag=%d,newseg->angle=%f,newseg->gtype=%d,prevseg->gtype=%d\n",newseg->ID,sq->circle_insert_flag,sq->axis9_flag,newseg->angle,newseg->gmodetype,prevseg->gmodetype);

           //圆弧插入后newseg会向后增加一段，调整之
           if (newseg->nextSegment !=0) newseg= newseg->nextSegment;

                        //  printf("sqPreprocess pre backward:sq->numSegments=%d, newseg->ID=%d,prevseg->id=%d\n",sq->numSegments,newseg->ID,prevseg->ID);

           if (prevseg->nextSegment != newseg){
               //插入了圆弧
             //  prevseg->nextSegment->lastSegFlag = 1; //插入的圆弧最后标志设为1.
               sqBackwardLinkSegment(sq,prevseg->nextSegment,prevseg->nextSegment,sq->maxFeedOverrideFactor);
               if (prevseg->nextSegment->linkedToPrevSeg==0){
                   //prevseg没有链接，可能也需要后连
                   sqBackwardLinkSegment(sq,prevseg,prevseg,sq->maxFeedOverrideFactor);
               }

           }else{
               //没有插入圆弧
                 sqBackwardLinkSegment(sq,prevseg,prevseg,sq->maxFeedOverrideFactor);
           }
           //最后一段需要确保减速到0,但该段在后续加入新段后末速会改变，因此设计标志来标识
          //     printf("sqPreprocess pre backwardforlastseg:sq->numSegments=%d, newseg->ID=%d,prevseg->id=%d\n",sq->numSegments,newseg->ID,prevseg->ID);

           if (-1==sqBackwardForLastSegment(sq, newseg)){
               newseg->initVec = 0;
               prevseg->finalVec = 0;
           }
  //  printf("sqPreprocess last :sq->numSegments=%d, newseg->ID=%d,prevseg->id=%d\n",sq->numSegments,newseg->ID,prevseg->ID);
//****************************************************************************************
        }            //end of prevseg is not active
    }            //end of newseg is not the first seg in sq



    return 0;
}


/* interface functions */
/* ------------------- */

int sqInitQueue_seven(SEGMENTQUEUE * sq, SEGMENT * first, int size) {
    if (size <= 0 || sq == 0 || first == 0) {
        diagnostics("Error in sqInitQueue_seven()\n");
        return -1;
    }

    sq->queue = first;
    sq->size = size;

    sq->start = 0;
    sq->end = 0;

    sq->full = 0;
    sq->numSegments = 0;

    sq->initXYZ.tran.x = 0;
    sq->initXYZ.tran.y = 0;
    sq->initXYZ.tran.z = 0;
    sq->lastPoint = sq->initXYZ;

    sq->maxFeedOverrideFactor = 1.2;
    sq->feedOverrideFactor = 1.0;
    sq->cycleTime = 0;
    sq->currentID = 0;

    sq->done = 1; /* the queue is empty, so by definition we're done */
    sq->done_filter=1;
    sq->paused = 0;
    sq->stepping = 0;
    sq->feedAdjusting = 0;
    sq->aborting = 0;
    sq->delayTime = 0;
    sq->cornerlimit = 0;

    /* initializing debug variables */
//    oldPos = sq->lastPoint;
//    oldVel.tran.x = 0;
//    oldVel.tran.y = 0;
//    oldVel.tran.z = 0;
    //oldDist = 0;

//*****************************************************************************
    //add by heyuebang in 2015-12-13
    sq->tolerance = 0.025; //容许使用误差，小线段

    //sq->queue[sq->size - 1].end = sq->lastPoint; //初始化时将最后一段的末点设为开始点

        sq->filter_const = 5; //初始定义5个数据滤波
        sq->is_or_not_filterV = 1; //滤波使能
    sqInitFilter(sq); //速度滤波参数

     sqInitPointFilter(sq);

     sq->G_mode_0.maxJerk = 20000.0;
      sq->G_mode_0.maxAcc_circle= 3000.0;
      sq->G_mode_0.maxAcc_nCircle= 3000.0;
     sq->G_mode_1.maxJerk = 10000.0;

      sq->tolerance = 0.025; //容许使用误差，小线段

     sq->corner_mix_flag = 0; //拐点速度调整（根据拐角大小调整放大缩小因子）

     sq->circle_insert_flag = 0; //插入圆弧标志
     sq->circle_tolerance_g1 = 0.01; //g1插入圆弧容许误差
     sq->circle_tolerance_g0 = 0.1;//g0插入圆弧容许误差
     //sq->dist_has_gone = 0.0;
    // sq->dec_velocity_factor = 1.0; //没用

    // sq->not_long_flag = 0;//小线段标志


     sq->omega_limit_flag = 0;//角速度限制标志

     sq->optimized_angle = PM_PI_2 + 0.001;

     sq->startState[0] = 0.0;
     sq->startState[1] = 0.0;
     sq->startState[2] = 0.0;
     sq->startState[3] = 0.0;
    memcpy(sq->lastState,sq->startState,4*sizeof(double));
    printf("sqInitQueue:laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);

     sq->timeForRun[0]= 0.0; sq->timeForRun[1]= 0.0; sq->timeForRun[2]= 0.0; sq->timeForRun[3]= 0.0;
      sq->timeForRun[4]= 0.0; sq->timeForRun[5]= 0.0; sq->timeForRun[6]= 0.0;
      sq->CruiseStarEndPos[0] = 0.0; sq->CruiseStarEndPos[1] =0.0;
      sq->vmaxForPlan = 0.0; sq->incJerk = 0.0;sq->decJerk = 0.0;
      sq->timeLost = 0.0;

      sq->remainderLength = 0.0;
//*****************************************************************************
      //2017.04.02 九轴参数
   //   sq->rate_angle_axis = 1.0;
     // memset(sq->axis_rate,0,9*sizeof(double));
   //   diagnostics("init: sq->axis[0]=%d‘",(int)(sq->axis_rate[0]*1000));
      sq->axis_rate[0] = 1.0;
       sq->axis_rate[1] = 1.0;
       sq->axis_rate[2] = 1.0;
         sq->axis_rate[3] = 0.0;
          sq->axis_rate[4] = 0.0;
           sq->axis_rate[5] = 0.0;
            sq->axis_rate[6] = 0.0;
             sq->axis_rate[7] = 0.0;
              sq->axis_rate[8] = 0.0;

         sq->axis9_flag = 0; //0表示3轴，1表示大于3轴
         sq->limit_vmax = DBL_MAX;

    return 0;
}



/*类型转换，只有tran有用*/
static void EmcPose_to_pmpose(EmcPose e, PmPose * p) {
    p->tran = e.tran;
    p->rot.s = 1;
    p->rot.x = e.a;
    p->rot.y = e.b;
    p->rot.z = e.c;
}

/*类型转换，tran有效*/
static void pmpose_to_EmcPose(PmPose p, EmcPose  *e) {
    e->tran = p.tran;
//    e->a = p.rot.x;
//    e->b = p.rot.y;
//    e->c = p.rot.z;
}

int sqAddLine_seven(SEGMENTQUEUE * sq, EmcPose end, int ID, unsigned char gmodetype,double *dynamicpar) //ghf-2014/10/27
        {
    double length;
    SEGMENT *newseg;
    EmcPose start;
    PmPose start_pose, end_pose;
    //static int idflag = 1;


   double maxUVec;
    /********************************************************************************/
    GMODE G_mode;

   // diagnostics("Error 1 sqAddline\n");
// G_number应该由sqAddLine()函数提供，0表示模式0(G00)，其他表示模式1(G01/G02/G03)；
    int G_number;  //0 is G0 mode,1 is G1 mode;
    //G_number =1;

    G_number = gmodetype;    //ghf-2014/3/22

    //根据模式的不同，提取不同的参数。
    if (G_number == 0)
        G_mode = sq->G_mode_0;
    else
        G_mode = sq->G_mode_1;

    /********************************************************************************/
    /* check if segment queue has been initialized */
    if (sq == 0 || sq->queue == 0) {
        diagnostics("Error 1 in sqAddLine()\n");
        return -1;
    }

    //getidtonum();
    //rtapi_print("getum_1=%d\n",sqGetid(emcmotStatus->restartagain,emcmotStatus->idstonum));
    /* check for full */
    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
    if (sq->numSegments == sq->size - 1) {
        diagnostics(
                "Error 2 Panic!!!!, segmentqueue overflows!!!,sq->numSegments = %d,ID=%d\n",
                sq->numSegments, ID);
        return -1;
    }

  //  printf("Addline00:sq->numSegments=%d,newseg->ID=%d,sq->circle_insert_flag=%d\n",sq->numSegments,newseg->ID,sq->circle_insert_flag);
/*
    if(idflag == 1) {
        idflag = 0;
        getidtonum();
    }
*/
    /********************************************************************************************************/
    if (sq->numSegments == 0)
    {
        start = sq->lastPoint;
        /*********************************************************/
        //5轴功能
        if (sq->axis9_flag == 2)
        {
            //lastpoint是5轴位置信息，需要转换到该段规划坐标系下
            EmcPose startTemp = KinematicsForwardFor5axisPlan(sq,start,sq->kinematics.KnifeLength);
            start = startTemp;
        }
         /*********************************************************/
    }
    else
    {
        start = sq->queue[(sq->end + sq->size - 1) % sq->size].end;
        /*********************************************************/
        //5轴功能
        if (sq->axis9_flag == 2)
        {
            //lastpoint是5轴位置信息，需要转换到该段规划坐标系下
            double knifeLength =  sq->queue[(sq->end + sq->size - 1) % sq->size].Kinematics5Axis.KnifeLength;
            EmcPose startTemp = KinematicsInverseFor5axisPlan(sq,start,knifeLength); //先求其5轴位置
            start = KinematicsForwardFor5axisPlan(sq,startTemp,sq->kinematics.KnifeLength); //再求当前坐标系下位置
            start = startTemp;
        }
         /*********************************************************/
    }
    /********************************************************************************************************/
 sqSetValidAxis( sq,&start);
 sqSetValidAxis( sq,&end);
    //9轴长度
     double k[9];
     length = sqGiveLength9(start,end,sq,k);

    if (length <= 0.01) {
        sq->lastAppMotionID = ID;
      //  printf("sqAddLine: ID=%d is negected\n", ID);
        return 0;
    }


    newseg = sq->queue + sq->end;
    /* ...and update the ring buffer properties */
    sq->end = (sq->end + 1) % sq->size;

    /* fill segment parameter fields */
    newseg->ID = ID;
       /*********************************************************/
    //5轴功能
      if (sq->axis9_flag == 2){
            newseg->Kinematics5Axis.KnifeLength = sq->kinematics.KnifeLength;
            newseg->Kinematics5Axis.mode = sq->kinematics.mode;
            newseg->Kinematics5Axis.PlanMode = sq->kinematics.PlanMode;
      }
           /*********************************************************/
    //newseg->type = sqGetid(emcmotStatus->restartagain,emcmotStatus->idstonum);//ghf-2014/9/16 通过fpga设置了密钥
   newseg->type =SQ_LINEAR;
    newseg->length = length;
    /**************************************************************/
    newseg->givenLength = length;
    /**************************************************************/
    //add by heyuebang in 2015-12-25
    memcpy( newseg->length_coefficient,k,9*sizeof(double)); //设置各轴分量，用于从length中分配各轴位置
    /**************************************************************/
    /* let newseg point to the first empty place in the ring buffer... */
    newseg->totLength = length;
    newseg->start = start;
    newseg->end = end;

    newseg->finalVec = 0;
    newseg->plInitVec = 0;
    newseg->plFinalVec = 0;
  //  newseg->planningDone = 0;
    newseg->active = 0;
    newseg->numLinkedSegs = 0;
    newseg->linkedToPrevSeg = 0;
    newseg->nextSegment = 0;
  //  newseg->type_circle_seg=0;//ghf-2014/11/27

     //设置补偿量
    int jj=0;
    for(;jj<10;jj++) {
        newseg->dynamicGetpar[jj] = dynamicpar[jj];
    }

    /* initialize line */
     //初始化直线
    EmcPose_to_pmpose(newseg->start, &start_pose);
    EmcPose_to_pmpose(newseg->end, &end_pose);
    pmLineInit(&newseg->line, start_pose, end_pose); // line->qVec


    /* set the maximum tangential acceleration for this line */
    maxUVec = max(fabs(newseg->line.uVec.x), fabs(newseg->line.uVec.y));
    maxUVec = max(fabs(newseg->line.uVec.z), maxUVec);
    // newseg->amaxTan = sq->maxAcc / maxUVec;

    /******************************************************************************/
//add by heyuebang in 2/17/2014
//根据模式不同，给newseg设定不同的最大速度，最大加速度，最大法向加速度（处理拐点受限用）

    if (G_number == 0) {
            newseg->maxVec = G_mode.maxV ;
            newseg->gmodetype = 0;
    }
    else {
        newseg->maxVec = min(sq->feed,G_mode.maxV) ;
        newseg->gmodetype = 1;
    }

/************************************************************************************************/
    //其他轴abcuvw限制，feed不起作用，注意这里G_mode有G0,G1两种模式，即角度轴也有这两种最大速度设置
    int i;
    for(i = 0; i<6;i++)
    {
        if(fabs(newseg->length_coefficient[i])> 1e-6){
           // printf("now I get max Vel:%.3f %.3f %d\n",G_mode.maxAngleV_Axis[i],G_mode.maxAngleAcc_n_Axis[i],i);
            newseg->maxVec = min(newseg->maxVec,G_mode.maxAngleV_Axis[i]/fabs(newseg->length_coefficient[i]));
        }
    }
/************************************************************************************************/

     newseg->givemaxVec = newseg->maxVec;

    newseg->amaxTan = G_mode.maxAcc; //最大切向加速度，设置的最大加速度/单位向量的最大矢量ghf-2014/6/17
    newseg->amaxNorm = G_mode.maxAcc_nLine; //最大法向加速度
    newseg->jmaxTan = G_mode.maxJerk; //最大加加速度


    /******************************************************************************/
  //  printf("Addline00 pre :sq->numSegments=%d,newseg->ID=%d,sq->circle_insert_flag=%d\n",sq->numSegments,ID,sq->circle_insert_flag);

    if (-1 == sqPreprocessSegment(sq, newseg, &G_mode)) {
        diagnostics("Error 3 in sqAddLine()\n");
        return -1;
    }

    /* set last Appended Motion ID */
    sq->lastAppMotionID = ID;

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
    if (sq->numSegments >= sq->size - SQ_SAFETY_MARGIN) {
        sq->full = 1;
    }

   // printf("Addline11 aft:sq->numSegments=%d,newseg->ID=%d,sq->circle_insert_flag=%d\n",sq->numSegments,ID,sq->circle_insert_flag);
    return 0;
}

//加入圆弧
int sqAddCircle_seven(SEGMENTQUEUE * sq, EmcPose end, PmCartesian center,
        PmCartesian normal, int turn, int ID, unsigned char gmodetype, double *dynamicpar) {
    SEGMENT *newseg;
    PmCircle circle;
    EmcPose start;
    PmCartesian helix;
    double absHelix;
    PmPose start_pose, end_pose;
    double temp;

//diagnostics("Error 1 sqAddcircle\n");
    /********************************************************************************/
    GMODE G_mode;
   // double maxV;
    /* please add this parameter in function sqAddLine, such as
     sqAddLine(SEGMENTQUEUE * sq, EmcPose end, int ID,int G_mode)
     and delete the following two line.
     */
//根据模式不同，提取不同的参数，G_number参数应该由函数sqAddCircle（）提供；
    int G_number;  //0 is G0 mode,1 is G1 mode;
    G_number = gmodetype;

    if (G_number == 0)
        G_mode = sq->G_mode_0;
    else
        G_mode = sq->G_mode_1;

    /********************************************************************************/
    /* used to calculate the maximum tangential acceleration */
    // double rpow2, A, topIncPow2;
    /* check if segment queue has been initialized */
    if (sq == 0 || sq->queue == 0) {
        diagnostics("Error 1 in sqAddCircle()\n");
        return -1;
    }

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31

    /* check for full */
    if (sq->numSegments == sq->size -1) {
        diagnostics("Error 2 Panic!!!!, segmentqueue overflows!!!\n");
        return -1;
    }

    //如果轴0,1,2系数不相等，则禁止出现圆弧。
    if (fabs(sq->axis_rate[0]-sq->axis_rate[1])>1e-6 || fabs(sq->axis_rate[0]-sq->axis_rate[2])>1e-6)
    {
        diagnostics("Error in sqAddCircle: axis_rate[0] != axis_rate[1] or axis_rate[0] !=axis_rate[2] or axis_rate[1] != axis_rate[2]\n");
        return -1;
    }

/*************************************************************************************/
    if (sq->numSegments == 0)
    {
        start = sq->lastPoint;
        /*********************************************************/
        //5轴功能
        if (sq->axis9_flag == 2)
        {
            //lastpoint是5轴位置信息，需要转换到该段规划坐标系下
            EmcPose startTemp = KinematicsForwardFor5axisPlan(sq,start,sq->kinematics.KnifeLength);
            start = startTemp;
        }
         /*********************************************************/
    }
    else
    {
        start = sq->queue[(sq->end + sq->size - 1) % sq->size].end;
        /*********************************************************/
        //5轴功能
        if (sq->axis9_flag == 2)
        {
            //lastpoint是5轴位置信息，需要转换到该段规划坐标系下
            double knifeLength =  sq->queue[(sq->end + sq->size - 1) % sq->size].Kinematics5Axis.KnifeLength;
            EmcPose startTemp = KinematicsInverseFor5axisPlan(sq,start,knifeLength); //先求其5轴位置
            start = KinematicsForwardFor5axisPlan(sq,startTemp,sq->kinematics.KnifeLength); //再求当前坐标系下位置
            start = startTemp;
        }
         /*********************************************************/
    }
/*************************************************************************************/
    sqSetValidAxis( sq,&start); //禁止无效轴，设为0;
    sqSetValidAxis( sq,&end); //禁止无效轴，设为0;
 //初始化圆弧
    EmcPose_to_pmpose(start, &start_pose);
    EmcPose_to_pmpose(end, &end_pose);
    pmCircleInit(&circle, start_pose, end_pose, center, normal, turn);
//    printf("now endp:%f %f %f  st:%f %f %f turn:%d  angle:%f       ID:%d\n",end_pose.tran.x,end_pose.tran.y,end_pose.tran.z,start_pose.tran.x,start_pose.tran.y,start_pose.tran.z,turn,circle.angle,ID);

    /*
  //反转，多加了个1,因为0表示正转不足1圈，-1表示反转不足1圈
    if (turn < 0)
        turn = -1- turn;
  //太小认为正转1圈，pmCircleInit函数中有对应设计（删除），这里不加会导致错误
    if((circle.angle - turn * PM_2_PI > -CIRCLE_FUZZ)&&(circle.angle -turn * PM_2_PI < CIRCLE_FUZZ)&& (circle.angle - turn * PM_2_PI)*circle.radius < 0.005) {
        circle.angle += PM_PI * 2;
    }
*/
    //太短忽略掉
    if (fabs(circle.angle) <= 1e-4 || fabs(circle.angle)*circle.radius<0.002) {
        //only set ID of last appended motion
        sq->lastAppMotionID = ID;
        return 0;
    }

    /* Calculate the helix gradient in normal direction */
  //计算单位角度下的高度，螺旋线用，与单位角度下的弧度（半径）对应，方向垂直
    pmCartScalDiv(circle.rHelix, circle.angle, &helix);
    pmCartMag(helix, &absHelix);

    /* let newseg point to the first empty place in the ring buffer... */
    newseg = sq->queue + sq->end;
    /* ...and update the ring buffer properties */
    sq->end = (sq->end + 1) % sq->size;

    /* fill segment parameter fields */
    newseg->ID = ID;
    /*********************************************************/
 //5轴功能
   if (sq->axis9_flag == 2){
         newseg->Kinematics5Axis.KnifeLength = sq->kinematics.KnifeLength;
         newseg->Kinematics5Axis.mode = sq->kinematics.mode;
         newseg->Kinematics5Axis.PlanMode = sq->kinematics.PlanMode;
   }
        /*********************************************************/
    //开始对一段不能是圆弧，因为emcmotStatus->idstonum需要addline初始化
    //newseg->type = sqGetid(emcmotStatus->restartagain,emcmotStatus->idstonum)+1;//ghf-2014/9/16
    newseg->type =SQ_CIRCULAR;
    newseg->circle = circle;

    //计算螺旋线单位角度对应的弧线长度
    newseg->helixRadius = sqrt(circle.radius * circle.radius + absHelix * absHelix);

    //计算总长
    double circle_length;
    double k[9];
    double mag;
    circle_length = circle.angle * newseg->helixRadius;
    k[0] = circle_length;
    k[1] = circle_length;
    k[2] = circle_length;
    k[3] = end.a - start.a;
    k[4] = end.b - start.b;
    k[5] = end.c - start.c;
    k[6] = end.u - start.u;
    k[7]= end.v - start.v;
    k[8]= end.w - start.w;
    int i;
    //按给定比率求九轴长度
    mag = fabs(circle_length);
//    for (i=2;i<9;i++)
//    {
//        mag += sq->axis_rate[i]*sq->axis_rate[i]*k[i]*k[i];
//    }
//    mag = sqrt(mag);

    // 获取各轴分量
    for (i=0;i<9;i++) k[i]/=mag;

    memcpy(newseg->length_coefficient,k,9*sizeof(double));

    newseg->length = mag;
    newseg->totLength = newseg->length;
    /**************************************************************/
    newseg->givenLength =  newseg->length;
    /**************************************************************/
    newseg->start = start;
    newseg->end = end;
//****************************************************************************************************
//changed by heyuebang in 2/17/2014

// 根据模式的不同，给newseg赋值最大速度
    if (G_number == 0) {
        newseg->maxVec =
        min(G_mode.maxV, sqrt(G_mode.maxAcc_circle * circle.radius));
       newseg->gmodetype = 0;

    } else {
        double temp,sinangle,temp1;
                temp = sqrt(G_mode.maxAcc_circle * circle.radius);
            //    printf("newseg->id=%d,vmax=%f,R=%f",newseg->ID,temp,circle.radius);
            //如果圆弧半径太小，有可能造成速度太小,调整下
                if ((sinangle = sin(circle.angle*0.5)) > 1e-6){
                    temp1 = 0.5*G_mode.maxAcc_nLine*sq->cycleTime / sinangle;
                    if (temp1*sq->cycleTime > newseg->length) temp1 = newseg->length/sq->cycleTime;
                   temp = max(temp,temp1);
                }
        // printf(" newseg->id=%d,vmaxaft=%f,R=%f\n",newseg->ID,temp,circle.radius);

        newseg->maxVec =  min(sq->feed, temp) ;
        newseg->gmodetype = 1;//上层手轮引导用的

    }
    newseg->maxVec = min(newseg->maxVec, G_mode.maxV);

    /************************************************************************************************/
        //其他轴abcuvw限制，feed不起作用，注意这里G_mode有G0,G1两种模式，即角度轴也有这两种最大速度设置
        for(i = 0; i<6;i++)
        {
            if(fabs(newseg->length_coefficient[i])> 1e-6){
              //  printf("---- now I get max Vel:%.3f %.3f %d\n",G_mode.maxAngleV_Axis[i],G_mode.maxAngleAcc_n_Axis[i],i);
                newseg->maxVec = min(newseg->maxVec,G_mode.maxAngleV_Axis[i]/fabs(newseg->length_coefficient[i]));
            }
        }
    /************************************************************************************************/



    newseg->givemaxVec = newseg->maxVec;

    newseg->finalVec = 0;
    newseg->plInitVec = 0;
    newseg->plFinalVec = 0;
   // newseg->planningDone = 0;
    newseg->active = 0;
    newseg->numLinkedSegs = 0;
    newseg->linkedToPrevSeg = 0;
    newseg->nextSegment = 0;

 //   newseg->type_circle_seg=0;//ghf-2014/11/27
    int kk=0;

    for(kk=0;kk<10;kk++) {
        newseg->dynamicGetpar[kk] = dynamicpar[kk];
    }

//***************************************************************************
// Changed by heyuebang in 1/23/2014;
// A conservative value is used to satisfy the acceleration and jerk limit.
//根据模式不同，给newseg赋值不同的线加速度amaxTan,法向加速度amaxNorm，加加速度jmaxTan；
    newseg->amaxTan = G_mode.maxAcc;

    /**************************************************************************/
    //add by heyuebang in 2015-12-25
    if (sq->omega_limit_flag){
        temp = max(G_mode.maxJerk,sqrt(newseg->amaxTan*0.2)*newseg->amaxTan);
        newseg->maxVec =min(newseg->maxVec,pow(temp* circle.radius* circle.radius,0.333333));
    }
    /**************************************************************************/
    newseg->amaxNorm = G_mode.maxAcc_nCircle;
    newseg->jmaxTan =  G_mode.maxJerk;
//***************************************************************************

    if (-1 == sqPreprocessSegment(sq, newseg, &G_mode)) {
        diagnostics("Error 3 in sqAddCircle()\n");
        return -1;
    }

    /* set last Appended Motion ID */
    sq->lastAppMotionID = ID;

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
    if (sq->numSegments >= sq->size - SQ_SAFETY_MARGIN) {
        sq->full = 1;
    }
//  diagnostics("add circle!\n");
  //  printf("sqAddLine: newseg->ID\n",newseg->ID);
    return 0;
}

/**********************************************************************/
//修改：heyuebang 2015-12-15
//增加均值滤波
int sqRunCycle_seven(SEGMENTQUEUE * sq)
{
   //     printf("in sqRunCycle\n");
   // double deta_s;
     SEGMENT *as = sq->queue;

 //   PmPose last_point_pose;
    static int number=0;
    int startclocks = 0, endclocks = 0;
 startclocks = rtapi_get_time();

    if (sq == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,"Error 1 in sqRunCycle(): Segmentqueue doesn't exist!\n");
        return -1;
    }

//    if(sq->done==1 && sq->paused==1)
//        diagnostics("Error 2 in sqrun, pausefinish==%d,sq->done_filter=%d\n",emcmotStatus->pausefinish,sq->done_filter);
/*注意，done为滤波前结束标志，done_filter为滤波后结束标志
    两者都满足，表明发送位置给电机已停止，什么都不需要做了。
    当需要重启时，需要将done_filter,done置0;
*/
    if(sq->done_filter == 1 &&sq->done==1){
    //    printf("in sqRunCycle(): pausefinish=%d,done_filter=%d,sq->done=%d,pause=%d\n",emcmotStatus->pausefinish,sq->done_filter,sq->done,sq->paused);

        return 0;
    }

    //运行sqGeneratePosition函数（原sqRunCycle函数，获得滤波前的位置增量sq->deta_s）
    if (-1 == sqGeneratePosition(sq))
    {
        rtapi_print_msg(RTAPI_MSG_ERR,"Error 1 in sqRunCycle(): sqGeneratePosition(sq)!\n");
        return -1;
    }

//如果滤波前已停止，延时设置停止标志done_filter
    if (sq->done == 1){

        if(number < sq->filter_const + sq->point_filtered_const){
             number ++;
             sq->done_filter = 0;
        }else{
              sq->done_filter = 1;
        }
    }else{
        number = 0;
        sq->done_filter = 0;

    }

//now真正停止了，如果设置为终止，则清除队列。注意，该程序段只会运行1次。
    if (sq->done_filter == 1 && sq->aborting == 1)
    {
         //emcmotStatus->pausefinish = 0;
          diagnostics("sqrun:in sqAbort, sqrun\n");
        if (-1 == sqClearQueue(sq)) {
            diagnostics("Error 2 in sqAbort, sqrun\n");
            return -1;
        }
    }

//如果停止了，是暂停造成的，则设置暂停已结束标志。注意，该程序段只会运行1次。
    if (sq->done_filter == 1 && sq->paused != 0){
            sq->pausefinish = 1;
        //    diagnostics("Error 3 in sqrun, pausefinish==1\n");
    }


   // deta_s = sqMeanFilterV(sq);
    //获取当前段地址，注意，第一次运行该函数，as需要指向sq->queque最后一段，因为sq->dist_to_go（剩下要走的路径初值为0）表明该段走完。
    //所以sq->cursor_aft_filter初值指向最后一段的下标；
     if (sq->is_or_not_filterV == 1)
     {
    as = sq->queue + sq->cursor_aft_filter;  //第一次指向最后一段，可能没有初始化，但这句话没有意义，增加只是让程序不报警告
    //计算滤波后要走的增量，并计算该段剩下要走的长度
    sq->dist_to_go -= sqMeanFilterV(sq); //规划速度滤波
//    if(fabs(sq->deta_s - sq->currentVel )>1e-9){
//        diagnostics("error in sqruncycle");
//    }
  //第一次，如果开始运行，则sq->dist_to_go一定小于0，所以以上计算的as没有用，
 //如果第一次没走，则while不会执行。as指向最后一段，而最后一段没有初始化，是错的
    while(sq->dist_to_go < -1e-9)
   {  //sq->dist_to_go < 0.0 表明当前段不够走，需要跳到下一段
        sq->cursor_aft_filter =  (sq->size +sq->cursor_aft_filter +1) % sq->size;

        as = sq->queue + sq->cursor_aft_filter;
       //往下跳了一段，则需要走的长度需要加上该段长度
       sq->dist_to_go += as->givenLength;
   }

   //as如果=0,表明指针指错了
    if (as == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,"Error 2 Panic: as = NULL in sqRunCycle\n");
        return -1;
    }
    //计算新的位置点
    if (as->type == SQ_LINEAR) {
        sqLinePoint(sq, as->givenLength - sq->dist_to_go,as,&sq->lastPoint_aft_filter);
    } else if (as->type == SQ_CIRCULAR){
          sqCirclePoint(sq, as->givenLength- sq->dist_to_go, as, &sq->lastPoint_aft_filter);//得到规划速度滤波后的结果
    }else{
        //第一次运行，而while没运行，则as指向最后一段，此时由于没有初始化，是错的，这里认为没有初始化时type=0;
        //需要确认？？？？？
         return 0;

    }


     }
    //需要考虑暂停，调整倍率时，运行剩下小线段运行问题，即滤波处理前将as段改变了。加入滤波器是不容许修改as段
  //如果使用了规划速度滤波，则单轴滤波使用规划速度滤波后结果，否则使用规划速度结果
   if (sq->is_or_not_filterV == 0)
    {
       sq->Point_Filtered_LastPoint = sqPointFilter(sq,sq->lastPoint);

     }else{
       sq->Point_Filtered_LastPoint = sqPointFilter(sq,sq->lastPoint_aft_filter);

     }

   endclocks = rtapi_get_time() - startclocks;
   if (endclocks > 500000) {
       rtapi_print(" sqRunCycle:s->ID = %d,as->numlinked=%d,sqRun Time=%d,sqForward=%d,sqPlan=%d,sqGenerate=%d\n",	as->ID,as->numLinkedSegs,endclocks, time_forward,time_plan,time_run);
   }

    return 0;
}

/**********************************************************************/

static int sqGeneratePosition(SEGMENTQUEUE *sq)
{
  //  printf("in sqGeneratePosition\n");

    int startclocks = 0, endclocks = 0;
 startclocks = rtapi_get_time();

    SEGMENT *as; /* to use instead of sq->queue[sq->start],
     stands for Active Segment */
    int i;
    SEGMENT *cursor;
    double finalVec;
    //PmCartesian normal, center;
   // PmPose last_point_pose;
    //int linkcrit,p;//hyb-2014/11/27

   // static int mynumlink=0;
    /****************************************/
    sq->deta_s = sq->remainderLength;  //初始化位置增量，用于速度滤波
    sq->remainderLength = 0.0; //更新为0;
    /****************************************/
    if (sq == 0) {
        printf("Error 1 in sqGeneratePosition(): Segmentqueue doesn't exist!\n");
        return -1;
    }

   // sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31

    if (sq->full == 1 && sq->numSegments < sq->size - SQ_SAFETY_MARGIN) {
        printf("sq->full == 1 although queue's not full!!!!\n");
    }


    //hong fix bug on 20160106, before that,sqAbort() cannot clear the queue,and retrun in here
    //规划已停止
    if (sq->done == 1) {
        /* do nothing */
        sq->delayTime = 0; //初始化滞后时间
        return 0;
    }

 //开始运行时，延时
    if (sq->done == 0 && sq->delayTime <2 ){        //减少了等待延时，加快规划速度
        sq->delayTime ++;
        return 0;
    }else{
        sq->delayTime = 3;
    }

    //延时已过，开始运行
    /* if buffer is empty give last point (i.e. do nothing) */
    if (sq->numSegments == 0) {
        /* set the 'done' flag ..... */
        sq->done = 1;

        /* check if currentID is the same as the ID of the last appended motion.
         The only case in which this will not be true is when the last motion
         has a zero length */
        if (sq->currentID != sq->lastAppMotionID)
            sq->currentID = sq->lastAppMotionID;

        return 0;
    }

    //获取开始段
    as = sq->queue + sq->start; /* much shorter ..... */

    if (as == 0) {
        printf("Error 2 Panic: as = NULL in sqRunCycle\n");
        return -1;
    }

//开始规划
    if (as->active == 0) //开始运行，初始化参数
    {
        if (as->linkedToPrevSeg) //测试
           {
            printf("in sqGeneratePosition,as->linkedToPrevSeg=%d\n",
                    as->linkedToPrevSeg);
            return -1;
        }

   /*强制前连, 针对段数太短，不够一步走的情况。
*/
              //    printf("Run bef forwardlink:as->cursor->ID=%d,as->numlinkedseg=%d, timeRun=%f,sq->numSeg=%d\n",as->ID, as->numLinkedSegs,sq->timeForRun[6],sq->numSegments);


        if (-1  == sqForceForwardLinkSegment(sq,as)) { //hyb-2017/7/7
            printf("Error 5 in sqGeneratePosition add by hyb \n ");
            sq->done = 1; //ghf-2014/6/30

            return -1;
        }

         //    printf("Run bef plan:as->cursor->ID=%d,as->numlinkedseg=%d, timeRun=%f,sq->numSeg=%d\n",as->ID, as->numLinkedSegs,sq->timeForRun[6],sq->numSegments);

        cursor = as;
        //将此逻辑段设为正在运行，防止预处理对其进行处理
              for (i = 0; i <= as->numLinkedSegs; i++) {
                  if (cursor == 0) {
                      rtapi_print_msg(RTAPI_MSG_ERR,"Error 7 Panic: cursor 1 = NULL in sqGeneratePosition\n");
                      return -1;
                  }
                  cursor->active = 1;
                  if (as!=cursor && cursor->linkedToPrevSeg !=1){
                       rtapi_print_msg(RTAPI_MSG_ERR,"Error 71 Panic: cursor 1 = nolink in sqGeneratePosition\n");
                        return -1;
                  }
                  cursor = cursor->nextSegment;
              }

               //初始化一些参数

              /* reset base, cursor, offset and cumlength and n */
              sq->base = as->start;
              sq->cursor = as;
              sq->offset = 0;
              sq->cumLength = as->length;

              sq->currentID = as->ID;

              /************************************************************/
//              if (as->nextSegment !=0){
//                           printf("Run bef plan11:as->ID=%d,as->next->ID=%d,as->numlinkedseg=%d, timeRun=%f,sq->numSeg=%d,as->totLength=%f,as->plInitVec=%f,as->plFinalVec=%f,as->initVec=%f,as->finalVec=%f\n",as->ID, as->nextSegment->ID,as->numLinkedSegs,sq->timeForRun[6],sq->numSegments,as->totLength,as->plInitVec,as->plFinalVec,as->initVec,as->finalVec);
//              }

          if (-1 == sqPlanSegment(sq, as)) {
              printf("Error 6 in sqGeneratePosition\n");
              return -1;
          }

//          if (as->numLinkedSegs>10)
//          printf("as->ID=%d,as->numLinkedSegs=%d,sq->vmaxForPlan=%f\n",as->ID,as->numLinkedSegs,sq->vmaxForPlan);

//         if (as->prevSegment !=0){
//                          printf("Run aft plan:as->ID=%d,as->preseg->id=%d,as->numlinkedseg=%d, timeRun=%f,sq->numSeg=%d,as->totLength=%f,as->plInitVec=%f,as->plFinalVec=%f,as->initVec=%f,as->finalVec=%f\n",as->ID, as->prevSegment->ID,as->numLinkedSegs,sq->timeForRun[6],sq->numSegments,as->totLength,as->plInitVec,as->plFinalVec,as->initVec,as->finalVec);
//          }

    }

    /*******************************************************/

    // 计算下一步距离
    sq->timeLost += sq->cycleTime;

//    if (as->ID >=125 && sq->timeLost >= sq->timeForRun[6] - sq->cycleTime){
//        printf("Run1:as->ID=%d, timelost=%f,as->totLength=%f,CruiseStartPos=%f, endPos=%f\n",as->ID, sq->timeLost,as->totLength,sq->CruiseStarEndPos[0],sq->CruiseStarEndPos[1]);
//        printf("Run1:as->ID=%d, t1=%f, t2=%f, t3=%f, t4=%f, t5=%f, t6=%f, t7=%f\n",as->ID, sq->timeForRun[0],sq->timeForRun[1],sq->timeForRun[2],sq->timeForRun[3],sq->timeForRun[4],sq->timeForRun[5],sq->timeForRun[6]);
//    }
    double dist;
 dist = sq->lastState[0]; //记录上次位置

    double time[3];
    double X[3];

    //加速段
    if (sq->timeLost <= sq->timeForRun[2]){ //加速段
        // 输入X=[s,v，a]，time=[t1,t2,t3],加加速度j，时间t
        //输出 y=[s,v,a,j];
        //该函数计算从点X开始t时间后的状态y
        X[0] = sq->startState[0]; X[1] = sq->startState[1]; X[2] = sq->startState[2];
        time[0] = sq->timeForRun[0];time[1] = sq->timeForRun[1];time[2] = sq->timeForRun[2];
        GetOneStepPVAG(X, time, sq->incJerk, sq->timeLost, sq->lastState);
    }else if (sq->timeLost <= sq->timeForRun[3]){ //匀速段
        sq->lastState[0] = sq->CruiseStarEndPos[0] +sq->vmaxForPlan * (sq->timeLost - sq->timeForRun[2]);
        sq->lastState[1] = sq->vmaxForPlan;
        sq->lastState[2] = 0.0;
    }else{ //减速段
        X[0] = sq->CruiseStarEndPos[1]; X[1] = sq->vmaxForPlan; X[2] = 0.0;
        time[0] = sq->timeForRun[4] - sq->timeForRun[3];
        time[1] = sq->timeForRun[5] - sq->timeForRun[3];
        time[2] = sq->timeForRun[6] - sq->timeForRun[3];
        double tempTime;
        if (sq->timeLost > sq->timeForRun[6]){
            tempTime =  time[2] ;
        }else{
            tempTime = sq->timeLost - sq->timeForRun[3];
        }
        GetOneStepPVAG(X, time, sq->decJerk, tempTime, sq->lastState);
    }

    /****************************************/
    sq->deta_s +=  sq->lastState[0] - dist; //将规划的增量附加上
    /****************************************/
    sq->dist = sq->lastState[0]; //记录当前位置

    /* transform the dist scalar into a XYZ triplet */
    if (as->nextSegment != 0
            && (as->nextSegment->linkedToPrevSeg == 1 || sq->paused == 1 || sq->aborting == 1))
            /* the active segment is the first segment of a chain */
            /* the sq->paused == 1 test is added to make sure that if a pause
             command is given just before the end of the segment, that the
             following segment is used to finish decelerating to zero */
            {
        while ((sq->dist > sq->cumLength) && (sq->cursor->nextSegment != 0)
                && ((sq->cursor->nextSegment->linkedToPrevSeg == 1)
                        || sq->paused == 1 || sq->aborting == 1)) {
            sq->offset = sq->cumLength;
            sq->base = sq->cursor->end;
            sq->cursor = sq->cursor->nextSegment;
            sq->cumLength += sq->cursor->length;

            /**********************************************/
            //5轴功能,记录当前规划模式
            sq->realtimeKinematics.KnifeLength = sq->cursor->Kinematics5Axis.KnifeLength;
            sq->realtimeKinematics.mode = sq->cursor->Kinematics5Axis.mode;
            sq->realtimeKinematics.PlanMode = sq->cursor->Kinematics5Axis.PlanMode;
             /**********************************************/
          }
        /* set currentID */
        sq->currentID = sq->cursor->ID;           //next segment

        //rtapi_print_msg(RTAPI_MSG_ERR,"111sq->dist - sq->offset=%d,sq->cursor->type=%d\n",(int)((sq->dist - sq->offset)*1000),sq->cursor->type);

        if (sq->cursor->type == SQ_LINEAR) {
            sqLinePoint(sq, sq->dist - sq->offset,sq->cursor,&sq->lastPoint);
        } else {
              sqCirclePoint(sq, sq->dist - sq->offset,sq->cursor,&sq->lastPoint);
        }
    }

    else
    /* the active segment has no other segments linked to it, which makes
     things much easier... */
    {
        if (sq->cursor->type == SQ_LINEAR) {
                       sqLinePoint(sq, sq->dist,as,&sq->lastPoint);
        } else {
                      sqCirclePoint(sq, sq->dist,as,&sq->lastPoint);
        }

    }

    //sq->lastPoint.a = 0.0;

    if (sq->feedAdjusting == 1){
        sq->feedAdjusting = 0;
      //   printf("in run: as->ID=%d,lastS[0]=%f,lastS[1]=%f,lastS[2]=%f,lastS[3]=%f, timelost=%f\n",as->ID,sq->lastState[0],sq->lastState[1],sq->lastState[2],sq->lastState[3],sq->timeLost);
    }
//    if (as->ID >=16 && as->ID <=20){
//   printf("00as->ID=%d,numLinkedSegs=%d,x=%f,y=%f,z=%f,a=%f,as->totlength=%f,sq->dist=%f\n",as->ID,as->numLinkedSegs,sq->lastPoint.tran.x,sq->lastPoint.tran.y,sq->lastPoint.tran.z,sq->lastPoint.a,as->totLength,sq->dist);
//    }

    sq->currentVel = sq->lastState[1]; //实时速度计算

 //剩下不够一步走了
    if (sq->timeLost + sq->cycleTime > sq->timeForRun[6]) {
        if (sq->aborting == 1) { //如果终止，走完剩下的时间后，规划停止
            if(sq->timeLost >= sq->timeForRun[6])
                {
                sq->done = 1;
                sq->startState[0] =0.0; //清除
                sq->startState[1] = 0.0;
                sq->startState[2] = 0.0;
                memcpy(sq->lastState,sq->startState,4*sizeof(double));
              //  printf("sqGeneratePosition: aborting: laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);
                sq->timeLost = 0.0; //行走时间归零，下次重新开始

            }
        }
        else if (sq->paused == 1) { //如果暂停，必须在终止后
/*
 *暂停需要注意：随时暂停，导致该段并没有走完，剩下的长度必须重新计算
*/
            if(sq->timeLost >= sq->timeForRun[6]) {
                    sq->done = 1;
                    finalVec = 0;

                    /* remove all segments preceding the current segment */
                    cursor = as;
                    //rtapi_print("as->id=%d,sq->cursor=%d\n",as->ID,sq->cursor->ID);
                    while (cursor != sq->cursor) { //segments before sq->cursor is done,and remove them.making cursor point to current segment
                        cursor = cursor->nextSegment;
                        if (cursor == 0) {
                            rtapi_print_msg(RTAPI_MSG_ERR,"Error 10 Panic: cursor 2 = NULL in sqRunCycle\n");
                            return -1;
                        }
                    }

                    //as->length = as->length -  (sq->dist - sq->offset);

                        //断掉cursor之前的联系
                          cursor->prevSegment = 0;
                          cursor->linkedToPrevSeg = 0;
                          sq->start = sq->cursor - sq->queue;
                          as = cursor;

                          sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
                          if (sq->numSegments < sq->size - SQ_SAFETY_MARGIN)
                              sq->full = 0;
                          if (sq->numSegments < 0) {
                              rtapi_print_msg(RTAPI_MSG_ERR,
                                      "Error 11 Panic: sq->numSegments <0  in sqRunCycle\n");
                              return -1;
                          }

                        //  as->planningDone = 0;
                          as->plInitVec = finalVec;

                          as->initVec = finalVec; //ghf-2014/6/21

                    sq->startState[0] = sq->dist - sq->offset; //记录当前位置
                    sq->startState[1] = 0.0;
                    sq->startState[2] = 0.0;
                     memcpy(sq->lastState,sq->startState,4*sizeof(double));
                //     printf("sqgenerateposition: pause:laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);

                    sq->timeLost = 0.0; //行走时间归零，下次重新开始
                    as->totLength = as->length;

                    as->active = 0; /* mark the first segment of the chain
                     as not active */

                    /* determine how many segments are linked to as */
                    as->numLinkedSegs = 0;
                    while (cursor->nextSegment != 0
                            && cursor->nextSegment->linkedToPrevSeg == 1) {
                        cursor = cursor->nextSegment;
                        as->numLinkedSegs++;
                        as->totLength += cursor->length;
                        cursor->active = 0;//ghf-2014/11/19
                    }

                    if (as->totLength < 1e-6) //剩下太短了，跳过
                    {
                        sq->startState[0] = 0.0; //停止
                        sq->startState[1] = finalVec;
                        sq->startState[2] = 0.0;
                         memcpy(sq->lastState,sq->startState,4*sizeof(double));
                        sq->timeLost = 0.0;
                 //       printf("sqgenerateposition: plfinalve==0:laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);

                        sq->start = (sq->start + as->numLinkedSegs + 1) % sq->size;
                        sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;
                        if (sq->numSegments < sq->size - SQ_SAFETY_MARGIN)
                            sq->full = 0; // we just removed some segments
                        if (sq->numSegments < 0) {
                            rtapi_print_msg(RTAPI_MSG_ERR,"Error 16 Panic: sq->numSegments <0  in sqRunCycle\n");
                            return -1;
                        }
                    }



           }
        } else if (sq->stepping == 1) //未用
        /* we are at the end of the segment, but shouldn't go on
         with the next one */
        {
            //sq->numSegments--;
                if(sq->timeLost >= sq->timeForRun[6]) {
                        sq->start = (sq->start + 1) % sq->size;
                        sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;
                        if (sq->numSegments < sq->size - SQ_SAFETY_MARGIN)
                            sq->full = 0;
                        if (sq->numSegments < 0) {
                            rtapi_print_msg(RTAPI_MSG_ERR,"Error 13 Panic: sq->numSegments < 0  in sqRunCycle\n");
                            return -1;
                        }

                        as = sq->queue + sq->start;
                        as->plInitVec = 0;
                        as->active = 0;
                        //rtapi_print_msg(RTAPI_MSG_ERR,"333sq->stepping == 1\n");
                        sq->done = 1;
                        sq->startState[0] =0.0; //记录当前位置
                        sq->startState[1] = 0.0;
                        sq->startState[2] = 0.0;
                         memcpy(sq->lastState,sq->startState,4*sizeof(double));
                 //        printf("sqgenerateposition: stepping:laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);
                        sq->timeLost = 0.0; //行走时间归零，下次重新开始
                }
        }
        else {
            /* end of segment reached */
            /*找到最后一段*/

               if( as->plFinalVec !=0) { //后续还有段，接上走
                   //当前位置跳到下段开始处
                   sq->startState[0] = 0.0; //记录当前位置
                   sq->startState[1] = as->plFinalVec;
                   sq->startState[2] = 0.0;
                    memcpy(sq->lastState,sq->startState,4*sizeof(double));
             //       printf("sqgenerateposition: plfinalve!=0:laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);
                sq->timeLost -= sq->timeForRun[6]; //记录剩下时间，下次一步需要少走这个时间
                sq->remainderLength = as->totLength - sq->dist; //记录剩下的距离，需要累加到下一步中

                sq->start = (sq->start + as->numLinkedSegs + 1) % sq->size;
                sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
                if (sq->numSegments < sq->size - SQ_SAFETY_MARGIN)
                    sq->full = 0; // we just removed some segments
                if (sq->numSegments < 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR,"Error 16 Panic: sq->numSegments <0  in sqRunCycle\n");
                    return -1;
                }

                     //     printf("Run0:as->ID=%d,as->plFinalVec=%f, timelost=%f\n",as->ID, as->plFinalVec,sq->timeLost);

            }else{ //后续没有段了，等着走完
                   if(sq->timeLost >= sq->timeForRun[6]){ //走完剩下的时间
                       sq->startState[0] = 0.0; //停止
                       sq->startState[1] = as->plFinalVec;
                       sq->startState[2] = 0.0;
                        memcpy(sq->lastState,sq->startState,4*sizeof(double));
                       sq->timeLost = 0.0;
                 //      printf("sqgenerateposition: plfinalve==0:laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);

                       sq->start = (sq->start + as->numLinkedSegs + 1) % sq->size;
                       sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;
                       if (sq->numSegments < sq->size - SQ_SAFETY_MARGIN)
                           sq->full = 0; // we just removed some segments
                       if (sq->numSegments < 0) {
                           rtapi_print_msg(RTAPI_MSG_ERR,"Error 16 Panic: sq->numSegments <0  in sqRunCycle\n");
                           return -1;
                       }
                   }

                //     printf("Run1:as->ID=%d,as->plFinalVec=%f, timelost=%f,sq->numSegments=%d\n",as->ID, as->plFinalVec,sq->timeLost,sq->numSegments);
                //     as = sq->queue + sq->start;
              //       printf("Run2: nest as->ID=%d\n",as->ID);
               }


        }
    }

    /* for debugging */

//    oldVel = newVel;
//    newVel.tran.x = sq->lastPoint.tran.x - oldPos.tran.x;
//    newVel.tran.y = sq->lastPoint.tran.y - oldPos.tran.y;
//    newVel.tran.z = sq->lastPoint.tran.z - oldPos.tran.z;

//    newAcc.tran.x = newVel.tran.x - oldVel.tran.x;
//    newAcc.tran.y = newVel.tran.y - oldVel.tran.y;
//    newAcc.tran.z = newVel.tran.z - oldVel.tran.z;

    /*

*/
//    oldPos = sq->lastPoint;

    //在程序运行是获取动态变化的信息值
    sq->dynamicparameter[0] = sq->cursor->dynamicGetpar[0];
    sq->dynamicparameter[1] = sq->cursor->dynamicGetpar[1];
    sq->dynamicparameter[2] = sq->cursor->dynamicGetpar[2];
    sq->dynamicparameter[3] = sq->cursor->dynamicGetpar[3];
    sq->dynamicparameter[4] = sq->cursor->dynamicGetpar[4];
    sq->dynamicparameter[5] = sq->cursor->dynamicGetpar[5];
    sq->dynamicparameter[6] = sq->cursor->dynamicGetpar[6];
    sq->dynamicparameter[7] = sq->cursor->dynamicGetpar[7];
    sq->dynamicparameter[8] = sq->cursor->dynamicGetpar[8];
    sq->dynamicparameter[9] = sq->cursor->dynamicGetpar[9];
    sq->dynamicparameter[10] = sq->cursor->dynamicGetpar[10];

    if (sq->feedAdjusting == 0) {  //这个值不会依赖倍率而变
        sq->dynamicparameter[18] = sq->cursor->givemaxVec ;  //ghf-2014/10/10
    }
    sq->dynamicparameter[19] = sq->currentVel; //这样手动自动就统一到这个变量了

    sq->dynamicparameter[10] = sq->cursor->length - sq->dist;

    sq->dynamicparameter[11] = sq->cursor->line.end.tran.x
            - sq->lastPoint.tran.x;
    sq->dynamicparameter[12] = sq->cursor->line.end.tran.y
            - sq->lastPoint.tran.y;
    sq->dynamicparameter[13] = sq->cursor->line.end.tran.z
            - sq->lastPoint.tran.z;

    sq->dynamicparameter[14] = sq->cursor->gmodetype;
    sq->dynamicparameter[15] = sq->cursor->line.uVec.x;
    sq->dynamicparameter[16] = sq->cursor->line.uVec.y;
    sq->dynamicparameter[17] = sq->cursor->line.uVec.z;



     endclocks = rtapi_get_time() - startclocks;
     time_run= endclocks;
    return 0;
}


/* function to change the feed overide */
//调倍率
int sqSetFeedOverride_seven(SEGMENTQUEUE * sq, double fo) {

    SEGMENT *as; /* as = Active Segment */

    if (sq == 0 || sq->queue == 0) {
        diagnostics("Error 1 in sqSetFeedOverride\n");
        return -1;
    }

    /* if fo is out of the valid range: clamp it */
    if (fo < 0.01)
        fo = 0.0;
    else if (fo > sq->maxFeedOverrideFactor)
        fo = sq->maxFeedOverrideFactor;

    //如果调整倍率和现有倍率一致，则无需调整
    if (sq->feedOverrideFactor == fo){
        /* don't do anything, just return */
        //  diagnostics("Warning 1 in sqSetFeedOverride\n");
        return 0;
     }
    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31

    if (sq->numSegments == 0) {    //队列为空，直接返回
        /* the queue is empty */
        sq->feedOverrideFactor = fo;
      //     diagnostics("Warning 2 in sqSetFeedOverride\n");
        return 0;
    }

    as = sq->queue + sq->start;

    //当前处于暂停或终止过程，什么都不要做
    if ((sq->paused != 0)||sq->aborting == 1) {
            sq->feedOverrideFactor = fo;
        //               diagnostics("Warning 3 in sqSetFeedOverride\n");
            return 0;
    }

        if (sq->done == 1 && sq->feedOverrideFactor == 0.0)  {
      //      diagnostics("Warning 4 in sqSetFeedOverride,sq->done=%d,sq->numSegment=%d\n",sq->done,sq->numSegments);
            sq->done= 0; //重新启动
        }

    sq->feedOverrideFactor = fo;

    if (as->active == 0) {    //当前队列头开始的逻辑段都还没有激活，也是直接返回，因为sqplan会考虑倍率因子
        //           diagnostics("Warning 5 in sqSetFeedOverride,sq->done=%d,sq->numSegment=%d\n",sq->done,sq->numSegments);
        return 0;
    }
    //少于10步不处理，等待新段开始自动按新调速因子规划
    if (sq->timeForRun[6] - sq->timeLost < sq->cycleTime* 10) {
     //              diagnostics("Warning 6 in sqSetFeedOverride\n");
        return 0;
    }

    //正在运行，且不少于10步，距离应该够了

//    printf("bef Plan:as->id=%d,startS[0]=%f,startS[1]=%f,startS[2]=%f,startS[3]=%f\n",as->ID,sq->startState[0],sq->startState[1],sq->startState[2],sq->startState[3]);
//       printf("bef Plan:as->id=%d,time[0]=%f,time[1]=%f,time[2]=%f,time[3]=%f,time[4]=%f,time[5]=%f,time[6]=%f\n",as->ID,sq->timeForRun[0],sq->timeForRun[1],sq->timeForRun[2],sq->timeForRun[3],sq->timeForRun[4],sq->timeForRun[5],sq->timeForRun[6]);
//        printf("bef Plan:as->id=%d,Cruisestart=%f,CruiseEnd=%f,vmaxForPlan=%f,lastS[0]=%f,lastS[1]=%f,lastS[2]=%f,lastS[3]=%f\n",as->ID,sq->CruiseStarEndPos[0],sq->CruiseStarEndPos[1],sq->vmaxForPlan,sq->lastState[0],sq->lastState[1],sq->lastState[2],sq->lastState[3]);
    if ( sqPlanSegmentForAjustFeed( sq, as) == -1){ //规划失败
    //    diagnostics("Warning 7 in sqSetFeedOverride\n");
        return 0;
    }

    sq->timeLost = 0.0; //重新开始计时
    //sq->feedAdjusting = 1;
//    printf("aft Plan:as->id=%d,startS[0]=%f,startS[1]=%f,startS[2]=%f,startS[3]=%f,T=%f\n",as->ID,sq->startState[0],sq->startState[1],sq->startState[2],sq->startState[3],sq->cycleTime);
//       printf("aft Plan:as->id=%d,time[0]=%f,time[1]=%f,time[2]=%f,time[3]=%f,time[4]=%f,time[5]=%f,time[6]=%f\n",as->ID,sq->timeForRun[0],sq->timeForRun[1],sq->timeForRun[2],sq->timeForRun[3],sq->timeForRun[4],sq->timeForRun[5],sq->timeForRun[6]);
//        printf("aft Plan:as->id=%d,Cruisestart=%f,CruiseEnd=%f,vmaxForPlan=%f,lastS[0]=%f,lastS[1]=%f,lastS[2]=%f,lastS[3]=%f\n",as->ID,sq->CruiseStarEndPos[0],sq->CruiseStarEndPos[1],sq->vmaxForPlan,sq->lastState[0],sq->lastState[1],sq->lastState[2],sq->lastState[3]);
//调速成功
    return 0;

}

/* 暂停*/
int sqPause_seven(SEGMENTQUEUE * sq) {

    int i;
    SEGMENT *as,*cursor; /* as = Active Segment */ // add cursor by heyuebang in 2014-07-11

    printf("into sqPause\n");
    if (sq == 0 || sq->queue == 0) {
        diagnostics("Error 1 in sqPause\n");
        return -1;
    }

    if (sq->paused == 1 || sq->stepping == 1 || sq->aborting ==1) {
        /* don't do anything, system is already paused or stepping will stop soon */
        diagnostics("--don't do anything, system is already paused,sq->done=%d\n",sq->done);
        return 0;
    }

    /* set paused flag */
    sq->paused = 1;
    //diagnostics("\nsq->paused=%d\n", (sq->paused));
    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31

    if (sq->numSegments == 0) {
        /* the queue is empty */
        sq->done = 1; /* propably redundant */
        sq->pausefinish = 1; //start the run, if 0 will run error;
        diagnostics("00--don't do anything, system is Null,sq->done=%d\n",sq->done);

        return 0;
    }

    as = sq->queue + sq->start; //as指向的是当前队列中的第一个逻辑段

    if (as->active == 0 && as->plInitVec == 0)
    /* 如果当前段还没规划，且初速为0 */
    {
        sq->done = 1;
        sq->pausefinish = 1;
        diagnostics("11--don't do anything, system is not begun,sq->done=%d\n",sq->done);

        return 0;
    } else if (as->active == 0 && sq->stepping == 1) {
        //diagnostics("22222\n");
        return 0;
        //no need to pause when stepping is done!
    }

    if ( as->active == 0){ //当前段未运行，初始化并设为已规划
        cursor = as;
        for (i = 0; i <= as->numLinkedSegs; i++) {
            if (cursor == 0) {
                diagnostics("Panic: cursor  = NULL in sqPause\n");
                return -1;
            }
            cursor->active = 1;
            cursor = cursor->nextSegment;
      //      if (cursor->linkedToPrevSeg != 1)
         //       diagnostics("Panic: as->numLinkedSegs is wrong in sqPause\n");
    }



    /* reset base, cursor, offset and cumlength and n */
    sq->base = as->start;
    sq->cursor = as;
    sq->offset = 0;
    sq->cumLength = as->length;
  //  sq->n = 1;
    sq->currentID = as->ID;

    /************************************************************/
//    sq->startState[0] = 0.0; //开始位置为0，该参数记录as路径行走距离
//    sq->startState[1] = as->plInitVec; //速度为规划初速
//    sq->startState[2] = 0.0; //开始加速度为0;
//    sq->startState[3] = 0.0; //开始加加速度为0;该参数可不设，没用。
 }else{
        memcpy(sq->startState,sq->lastState,4*sizeof(double)); //将当前状态设为开始状态
     //   printf("sqpause: laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);

       sq->timeLost = 0.0; //重新开始
    }

    //以末速为零规划
    double amax,jmax;
    double time[3], jerk,sEnd;
    amax = sqGiveMinAmaxTan(as);
    jmax = sqGiveMinBetamaxTan(as);
     //计算达到末速0.0所需要的时间和加加速度
    sqTestLastVec(sq, as, 0.0,0.0,amax, jmax, time, &jerk,&sEnd);
   // printf("sqpause:as->id=%d,send=%f,sq->laststate[0]=%f,laststate[1]=%f,laststate[2]=%f,laststate[3]=%f,as->totlength=%f\n",as->ID,sEnd,sq->lastState[0],sq->lastState[1],sq->lastState[2],sq->lastState[3],as->totLength);
    sq->timeForRun[4] = time[2]; sq->timeForRun[5] = time[2]; sq->timeForRun[6] =time[2]; sq->timeForRun[3] = time[2];
    sq->timeForRun[0] = time[0]; sq->timeForRun[1] = time[1]; sq->timeForRun[2] = time[2];
    sq->incJerk = jerk; sq->decJerk = 0.0;
    sq->vmaxForPlan = 0.0;
    sq->CruiseStarEndPos[0] = sEnd;
    sq->CruiseStarEndPos[1] = sEnd;
   // printf("sqpause:as->id=%d,sq->timeForRun[0]=%f,timeForRun[1]=%f,timeForRun[2]=%f,timeForRun[3]=%f,timeForRun[4]=%f,timeForRun[5]=%f,timeForRun[6]=%f\n",as->ID,sq->timeForRun[0],sq->timeForRun[1],sq->timeForRun[2],sq->timeForRun[3],sq->timeForRun[4],sq->timeForRun[5],sq->timeForRun[6]);

    return 0;
}

/* function to resume with a paused segmentqueue */
int sqResume_seven(SEGMENTQUEUE * sq) {

    if (sq == 0 || sq->queue == 0) {
        diagnostics("Error 1 in sqResume\n");
        return -1;
    }

    diagnostics("get into resume,sq->done=%d,sq->numSegments=%d\n",sq->done,sq->numSegments);



    if (sq->done_filter != 1)

    /* we can't resume if the systems is not done yet with a step or pause
     action */
    {
        diagnostics("Can't resume if not done with pause or step action\n");
        /* this is not a critical error, so we'll just ignore the command */
        return 0;
    }

    sq->stepping = 0; //resume no matter the status is stepping or pause
    sq->paused = 0;
    sq->done = 0;
    sq->done_filter = 0;
    sq->pausefinish = 0;
    return 0;
}

/* function to abort */
int sqAbort_seven(SEGMENTQUEUE * sq) //ghf-2014/5/17,此函数出错时再跟5/28
{

    if (sq == 0 || sq->queue == 0) {
        diagnostics("Error 1 in sqAbort\n");
        return -1;
    }

    if (1 == sq->aborting)
        /* we are already aborting, so let's just ignore it */
        return 0;

    diagnostics("in sqAbort,sq->paused=%d,sq->done=%d,sq->donefilter=%d\n",sq->paused,sq->done,sq->done_filter);
    //emcmotStatus->pausefinish = 0;//ghf-2014/8/5
    if (sq->done ==1 && sq->done_filter==1){
        //has done after filter
         sq->aborting = 1;
        if (-1 == sqClearQueue(sq)) {
            diagnostics(" in sqAbort: clerarqueue\n");
            return -1;
        }

        diagnostics(" in sqAbort: clerarqueue\n");

        return 0;
    }

    if (sq->paused != 0 || sq->done == 1) {
          sq->aborting = 1;

        rtapi_print("in abort,run waiting\n");
    } else {
       // emcmotStatus->pausefinish = 0;
        sqPause_seven(sq);
       sq->paused = 0; //取消暂停标志
         sq->aborting = 1; //程序还在运行中点击退出，进到这里，
        rtapi_print("in abort,run sqPause\n");
    }
    return 0;
}

/* function to do execute one motion from a stop and stop again */
int sqStep_seven(SEGMENTQUEUE * sq) {
    SEGMENT *as; /* as = Active Segment */
    if (sq == 0) {
        diagnostics("Error in sqStep\n");
        return -1;
    }

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31

    if (sq->done != 1 || sq->numSegments == 0)
    /* step should only be used when system is paused and waiting... */
    {
        diagnosticsOff
        ("Stepping can only be done when system is paused and waiting\n");
        /* not a critical error, let's ignore it */
        return 0;
    }

    if (sq->feedOverrideFactor == 0) {
        /* we can't step if the feed override factor is zero. To step
         the user should set a new non-zero value for this factor first.
         This will immediately result in a resume action. */
        diagnostics("Can't resume if feed override is zero\n");
        /* not a critical error, so ignore it */
        return 0;
    }

    /* make finalInc of the current segment 0, set sq->stepping and resume.
     This means that after this is segment is done, it will
     wait for the next step or resume command */

    as = sq->queue + sq->start;
    as->finalVec = 0;

    /* if the next segment is linked to the current one, unlink it */
    if (as->nextSegment != 0 && as->nextSegment->linkedToPrevSeg == 1) {
        as->nextSegment->linkedToPrevSeg = 0;
        as->nextSegment->numLinkedSegs = as->numLinkedSegs - 1;
        as->nextSegment->totLength = as->totLength - as->length;
        as->numLinkedSegs = 0;
        as->totLength = as->length;
    }
    sq->done = 0;
    sq->stepping = 1;
    sq->paused = 0;
    return 0;
}





static void GetOneStepPVAG(const double X[3], const double time[3], double j, double t,
                    double y[4])
{
  double xr;
  double jr;
  double vr;
  double ar;
  double a;

  double t2,t3;
  double time2,time3;
  double time22,time32;
  /* X=[x,v,a];��ʼλ�ã���ʼ�ٶȣ���ʼ���ٶ� */
  /* time=[t1;t2;t3]; */
  /* jΪ���ٶ� */
  /* tΪʱ�� */
  //��������ʱ��t��������ʱ�䣬������ʱ��
  if (t > time[2]) {
    t = time[2];
  }

  t2 = t*t; t3=t*t2;
//��������ʱ��С��0,��ȡ��ǰ��XΪ����
  if (t < 0.0) {
    xr = X[0];
    vr = X[1];
    ar = X[2];
    jr = 0.0;
  } else if (t <= time[0]) {
    jr = j;
    ar = X[2] + j * t;
    vr = (X[1] + X[2] * t) + 0.5 * j * t2;
    xr = ((X[0] + X[1] * t) + 0.5 * X[2] * t2) + j * t3 /6.0;
  } else if (t <= time[1]) {
    time2 = time[0] * time[0];time3=time2*time[0];
    jr = 0.0;
    ar = X[2] + j * time[0];
    vr = ((X[1] + X[2] * t) - 0.5 * j * time2) + j * time[0] * t;
    xr = ((((X[0] + X[1] * t) + 0.5 * X[2] * t2) + 0.16666666666666666 * j *
           time3) - 0.5 * j * time2 * t) + 0.5 * j * time[0] * t2;
  } else if (t <= time[2]) {
    time2 = time[0] * time[0];time3=time2*time[0];
    time22 = time[1] * time[1];time32=time22*time[1];
    jr = -j;
    ar = (X[2] + j * time[0]) - j * (t - time[1]);
    a = t - time[1];
    vr = (((X[1] + X[2] * t) + j * time[0] * t) - 0.5 * j * time2)
      - 0.5 * j * (a * a);
    xr = (((((X[0] + X[1] * t) + 0.5 * X[2] * t2) - 0.16666666666666666 * j
            *t3) + 0.16666666666666666 * j * (time3 + time32)) + 0.5 * j * (time[0] + time[1]) * t2) - 0.5 * j * (time2 + time22)
      * t;
  } else {

    time2 = time[0] * time[0];time3=time2*time[0];
    time22 = time[1] * time[1];time32=time22*time[1];
    t2 = time[2] * time[2];t3=t2*time[2];
    ar = 0.0;

    /* t=time(3); */
    jr = 0.0;
    a = time[2] - time[1];
    vr = (((X[1] + X[2] * time[2]) + j * time[0] * time[2]) - 0.5 * j * (time2)) - 0.5 * j * (a * a);
    xr = ((((((vr * (t - time[2]) + X[0]) + X[1] * time[2]) + 0.5 * X[2] *
             (t2)) - 0.16666666666666666 * j * t3) + 0.16666666666666666 * j * (time3 + time32)) + 0.5 * j * (time[0] + time[1]) * (t2)) - 0.5 * j * (time2 + time22) * time[2];
  }

  y[0] = xr;
  y[1] = vr;
  y[2] = ar;
  y[3] = jr;
}

static void VelocityPlan(double vStart, double vEnd, double a,  double amax,
                  double jmax, double t[3], double *j)
{
  double v;
  int flag;
  double b;
  double jv;
  double t1;
  double t2;
  double y;
double temp;
  /* X=[x,v,a];λ�ã�-�ٶ��������ʼ�ٶ�-ĩ���ٶȣ������ٶ� */
  /* amax�������ٶ� */
  /* jmax�����Ӽ��ٶ� */
  //����ĩ���ٶ�Ϊ�����������ٶ�


//�����ٶ����
  v = vStart - vEnd;
  if (v >= 0.0) {
    flag = 1;
  } else {
    flag = -1;
  }

  a *= (double)flag;

  /* �޸ķ��ţ�ʹ����vͬ�� */
  v *= (double)flag;
  if (a >= 0.0) {
    *j = -jmax;
    b = a * a * 0.5;
    jv = jmax * v;
    if (jv <= amax * amax - b) {
        temp = b +jv;
        if (temp <0.0) temp=0.0;
      t1 = a / jmax + sqrt(temp) / jmax;
      t2 = t1;
    } else {
      t1 = (a + amax) / jmax;
      t2 = (v / amax + 0.5 * (a * a) / (amax * jmax)) + a / jmax;
    }
  } else {
    b = a * a * 0.5;
    jv = jmax * v;
    if (jv <= b) {
      *j = jmax;
        temp = b - jv;
        if (temp <0.0) temp=0.0;

      t1 = -a / jmax + sqrt(temp) / jmax;
      t2 = t1;
    } else if (jv <= amax * amax - b) {
      *j = -jmax;
        temp = b +jv;
        if (temp <0.0) temp=0.0;
      t1 = a / jmax + sqrt(temp) / jmax;
      t2 = t1;
    } else {
      *j = -jmax;
      t1 = (a + amax) / jmax;
      t2 = (v / amax + 0.5 * (a * a) / amax / jmax) + a / jmax;
    }
  }

  y = a / *j;
  *j *= (double)flag;
  t[0] = t1;
  t[1] = t2;
  t[2] = (t1 + t2) + y;

}

//计算各轴位置
static int sqLinePoint( SEGMENTQUEUE *sq,double length, SEGMENT *s, EmcPose *lastpoint)
{
    if (length < 0)
    {
        diagnostics("'Error in sqLinePoint, length<0\n");
        return -1;
    }
    if (length > s->givenLength) length = s->givenLength;

     EmcPose lastpoint1;
    lastpoint1.tran.x =  s->length_coefficient[0] *length + s->start.tran.x;
     lastpoint1.tran.y =  s->length_coefficient[1] *length+ s->start.tran.y;
   lastpoint1.tran.z =  s->length_coefficient[2] *length + s->start.tran.z;
     lastpoint1.a =  s->length_coefficient[3] *length + s->start.a;
   lastpoint1.b =  s->length_coefficient[4] *length + s->start.b;
     lastpoint1.c =  s->length_coefficient[5] *length + s->start.c;
    lastpoint1.u =  s->length_coefficient[6] *length + s->start.u;
    lastpoint1.v =  s->length_coefficient[7] *length + s->start.v;
    lastpoint1.w =  s->length_coefficient[8] *length + s->start.w;

    *lastpoint = KinematicsInverseFor5axisRun(sq,lastpoint1,s->Kinematics5Axis.KnifeLength);

    return 0;

}

static int sqCirclePoint( SEGMENTQUEUE *sq,double length, SEGMENT *s, EmcPose *lastpoint)
{
    PmPose last_point_pose;

    if (length < 0)
    {
        diagnostics("'Error in sqCirclePoint, length<0\n");
        return -1;
    }
    if (length > s->givenLength) length = s->givenLength;

    pmCirclePoint(&s->circle, s->length_coefficient[2]*length / s->helixRadius,
              &last_point_pose);

    EmcPose lastpoint1;
        pmpose_to_EmcPose(last_point_pose, &lastpoint1);

     lastpoint1.a =  s->length_coefficient[3] *length + s->start.a;
      lastpoint1.b =  s->length_coefficient[4] *length + s->start.b;
      lastpoint1.c =  s->length_coefficient[5] *length + s->start.c;
     lastpoint1.u =  s->length_coefficient[6] *length + s->start.u;
      lastpoint1.v =  s->length_coefficient[7] *length + s->start.v;
      lastpoint1.w =  s->length_coefficient[8] *length + s->start.w;

         *lastpoint = KinematicsInverseFor5axisRun(sq,lastpoint1,s->Kinematics5Axis.KnifeLength);

       return 0;
}

static void sqSetValidAxis( SEGMENTQUEUE *sq,EmcPose *s){
    //将无效的轴设为0,防止未初始化造成随即数干扰
    //至少x,y,z有效；
    if(fabs(sq->axis_rate[3]) < 1e-6){ //默认为0
        s->a = 0.0;
    }
    if(fabs(sq->axis_rate[4]) < 1e-6){ //默认为0
        s->b = 0.0;
    }
    if(fabs(sq->axis_rate[5]) < 1e-6){ //默认为0
        s->c = 0.0;
    }
    if(fabs(sq->axis_rate[6]) < 1e-6){ //默认为0
        s->u = 0.0;
    }
    if(fabs(sq->axis_rate[7]) < 1e-6){ //默认为0
        s->v = 0.0;
    }
    if(fabs(sq->axis_rate[8]) < 1e-6){ //默认为0
        s->w = 0.0;
    }

}
