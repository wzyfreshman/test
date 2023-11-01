#include "segment_five.h"
#include "segmentqueue.h"

#include "hal.h"
#include "rtapi_math.h"
#include "rtapi.h"		/* rtapi_print_msg */
#include "rtapi_string.h"       /* NULL */
#include <float.h>		/* for DBL_MAX */
#include <stdio.h>

/* some static variables for debugging */
//static EmcPose oldPos, oldVel, newVel, newAcc;
//static double sq->oldDist;
//static double defaultVel;
static EmcPose segEndPos;

static  int sqLinkSegments_five(SEGMENT * s1, SEGMENT * s2, int priority) ;


static int sqGeneratePosition_five(SEGMENTQUEUE *sq);//计算滤波前位置和位置增量

static int sqCircleFitting_five(SEGMENT * s1, SEGMENT * s2, SEGMENTQUEUE * sq);

static double sqGiveCornerMixVelocity_five(double vmax, SEGMENT * s1, SEGMENT * s2,SEGMENTQUEUE * sq);
static double  sqGiveCornerVelocity_five(SEGMENT * s1, SEGMENT * s2,double maxAcc_nLine, SEGMENTQUEUE * sq);
static int sqPreprocessSegment_five(SEGMENTQUEUE * sq, SEGMENT * newseg, GMODE *G_mode);
static int sqInsertCircle_five(SEGMENTQUEUE * sq,  SEGMENT *s1, SEGMENT *s2);

static int sqForceForwardLinkSegment_five(SEGMENTQUEUE * sq, SEGMENT * s, SEGMENT * end, double feedOverride);
static int sqBackwardLinkSegment_five(SEGMENTQUEUE * sq, SEGMENT * start, SEGMENT * s,double feedOverride);
static int sqInsertInitCircle_five(SEGMENTQUEUE * sq, EmcPose start, EmcPose end,
                              PmCartesian center, PmCartesian normal, int ID, int G_number,
                              SEGMENT *insertseg);
static void sqLengthFlagSet_five(SEGMENT * s, int director );
static int sqPlanSegment_five(SEGMENTQUEUE * sq, SEGMENT * s);
static int sqLinkCriterion_five(SEGMENTQUEUE * sq, SEGMENT * s, SEGMENT * end,
                           double length, double feedOverride);
static double sqGiveMaxInc_five(SEGMENTQUEUE * sq, SEGMENT * s);
static double sqGiveMaxIncWithFeedOverrate_five(SEGMENTQUEUE * sq, SEGMENT *s);
static double sqGiveMinAmaxTan_five(SEGMENT * s);
//static int sqGetGoalPosition_five(EmcPose * p);


//计算两点长度
static double sqGiveLength(EmcPose p1, EmcPose p2) {
    PmCartesian disp;
    double mag;

    pmCartCartSub(p1.tran, p2.tran, &disp);
    pmCartMag(disp, &mag);

    return mag;
}

static void EmcPose_to_pmpose(EmcPose e, PmPose * p) {
    p->tran = e.tran;
    p->rot.s = 1;
    p->rot.x = e.a;
    p->rot.y = e.b;
    p->rot.z = e.c;

    return;
}

static void pmpose_to_EmcPose(PmPose p, EmcPose * e) {
    e->tran = p.tran;
//    e->a = p.rot.x;
//    e->b = p.rot.y;
//    e->c = p.rot.z;

    return;
}

/*******************************************************************/
/*******************************************************************/
//采用分段设置不同系数，可提高效率（采用此功能，可进一步提高拐点速度）
static double sqGiveCornerMixVelocity_five(double vmax, SEGMENT * s1, SEGMENT * s2,SEGMENTQUEUE * sq){
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
static double  sqGiveCornerVelocity_five(SEGMENT * s1, SEGMENT * s2,
        double maxAcc_nLine, SEGMENTQUEUE * sq) {
    /* s1 and s2 must have been initialized correctly and must have a nonzero
     lenght in order to get a valid corner */

    PmCartesian v1;   //line vector of s1
    PmCartesian v2;   //line vector of s2
    PmCartesian me;
    PmCartesian diff;
    PmCartesian helix;

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
        diagnostics("Error1 in sqGiveCornerVelocity_five()\n");
        return -1;
    }

    /* check if s1 or s2 has a zero length or amax or cycleTime is zero */
    //*******************************************************************
      if (s1->type == SQ_LINEAR) {
        v1 = s1->line.uVec;
      } else {

        pmCartCartSub(s1->end.tran, s1->circle.center, &me);
        pmCartCartCross(s1->circle.normal, me, &v1); //v1方向为垂直于normal，与垂直于normal的圆弧相切，大小为r1。注意：当是螺旋线时，这个半径r1与me的模不一样，是指me投影在垂直于normal的平面上的大小。
         pmCartMag(v1,&r1); //r1=R；


      //   if(s2->ID>83 && s2->ID<86 )  printf("~conrnervelocity0 in: s2->Id=%d,v1.x=%f,v1.y=%f,v1.z=%f\n",s2->ID,v1.x/r1,v1.y/r1,v1.z/r1);

    //*******************************************************************
        pmCartScalDiv(s1->circle.rHelix, s1->circle.angle, &helix);//helix为单位角度对应的高度，螺旋线才有；
        pmCartCartAdd(v1, helix, &v1); //结果v1指向螺线线切线方向，大小为单位角度对应的长度
        pmCartPlaneProj(me, s1->circle.normal, &me); //得到圆心到end的方向
        pmCartUnit(me, &me); //单位化
        pmCartScalMult(me, s1->circle.spiral, &me); //获取半径方向增量
        pmCartScalDiv(me, s1->circle.angle, &me);//单位角度对应的增量
        pmCartCartAdd(v1, me, &v1); //合速度方向
        pmCartUnit(v1, &v1);//单位化

     //   if(s2->ID>83 && s2->ID<86)  printf("~conrnervelocity1 in: s2->Id=%d,v1.x=%f,v1.y=%f,v1.z=%f\n",s2->ID,v1.x,v1.y,v1.z);

    //*******************************************************************
      // add by heyuebang in 1/23/2014

       pmCartCartCross(s1->circle.normal, v1, &a1); //a1方向仍然为从s1->end.tran指向normal，并与normal垂直，但大小为cos（theta1），其中theta1为v1于垂直于normal的平面的夹角。
         pmCartMag(a1,&costheta1);  //获取cos（theta1）
          pmCartUnit(a1, &a1); //单位化a1，注意，a1方向正好是圆弧向心力方向。

    //*******************************************************************
      }

      if (s2->type == SQ_LINEAR) {
        v2 = s2->line.uVec;
      } else {
        pmCartScalDiv(s2->circle.rHelix, s2->circle.angle, &helix);
        pmCartCartAdd(s2->circle.rPerp, helix, &v2); //结果v2指向螺线线切线方向，大小为单位角度对应的长度

      //  pmCartUnit(v2, &v2);

   //     if(s2->ID>83 && s2->ID<86 )  printf("~conrnervelocity0 in: s2->Id=%d,v2.x=%f,v2.y=%f,v2.z=%f\n",s2->ID,v2.x,v2.y,v2.z);

       // pmCartPlaneProj(me, s1->circle.normal, &me); //得到圆心到end的方向
        //与me相关的是spiral曲线的切线计算ghf-2014/10/23
        pmCartUnit(s2->circle.rTan, &me); //单位化
        pmCartScalMult(me, s2->circle.spiral, &me); //获取半径方向增量
        pmCartScalDiv(me, s2->circle.angle, &me);//单位角度对应的增量
        pmCartCartAdd(v2, me, &v2); //合速度方向
        pmCartUnit(v2, &v2);
     //   if(s2->ID>83 && s2->ID<86 )  printf("~conrnervelocity1 in: s2->Id=%d,v2.x=%f,v2.y=%f,v2.z=%f\n",s2->ID,v2.x,v2.y,v2.z);

    //*******************************************************************
      // add by heyuebang in 1/23/2014

     // pmCartCartCross(s2->circle.normal, s2->circle.rPerp, &a2); //a2大小等于r2，方向为从s2->start.tran指向normal，并与normal垂直。
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
// changed by heyuebang in 1/21/2014
// This program is not right strictly, but still can be effective for most time
    vmax = DBL_MAX;




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

  //  if(s2->ID>83 && s2->ID<86 && vmax < 1e5)  printf("~conrnervelocity0 in: s2->Id=%d,vmax=%f\n",s2->ID,vmax);

// 如果不相切，则计算夹角限制速度maxAcc_nLine** cycleTime / maxdiff;
    if(sq->cornerlimit == 1) {
        if (maxdiff > 1e-6) //ghf-2014/12/11
            vmax = min(vmax, maxAcc_nLine * sq->cycleTime / maxdiff);
     //    if(s2->ID>83 && s2->ID<86 && vmax < 1e5)    printf("~conrnervelocity1 in: s2->Id=%d,s2->type=%d,vmax=%f,maxAcc_nLine=%f,T=%f,maxdiff=%f\n",s2->ID,s2->type,vmax,maxAcc_nLine,sq->cycleTime,maxdiff);
        return vmax;
    }
    if (maxdiff > 1e-6) //ghf-2014/12/11
    {
  /*****************************************************************************/
   //add by heyuebang in 2015-12-12
//   将sq->cycleTime固定为0.001,避免调整sq->cycleTime时，拐点速度变化
      // vmax = min(vmax, 0.7*maxAcc_nLine * sq->cycleTime / maxdiff);
        vmax = min(vmax, maxAcc_nLine * 0.001 / maxdiff);

/*****************************************************************************/
    }
  //  if(s2->ID>83 && s2->ID<86 && vmax < 1e5)    printf("~conrnervelocity2 in: s2->Id=%d,s2->type=%d,vmax=%f,maxAcc_nLine=%f,T=%f,maxdiff=%f\n",s2->ID,s2->type,vmax,maxAcc_nLine,sq->cycleTime,maxdiff);
     //将直线拐角看成曲率，采用圆弧最大速度限制其最大速度和拐角速度
        //夹角是0,v1 v2是一条同向的直线;夹角是30度，v1到v2是以150度过度过来的
        pmCartCartDot(v1,v2,&costheta1);

        //防止在+-1.000000左右发生错误ghf-2014/9/18
        if(costheta1 > 1)
            costheta1 = 1;
        else if(costheta1 < -1)
            costheta1 = -1;


        A = PM_PI - acos(costheta1);
         s2->angle = A;



     if (A < sq->optimized_angle) vmax = 0.0;

//diagnostics("~conrnervelocity in: s2->Id=%d,s2->angle=%d\n",s2->ID,(int)(s2->angle*1000*180/3.1415926));
    //ghf-2014/9/27,g0不作系数限制
    if((s1->gmodetype == 0) && (s1->type == SQ_LINEAR) && (s2->gmodetype == 0) && (s2->type == SQ_LINEAR)) {

        return vmax;
        //return DBL_MAX;//ghf-2014/10/31,相邻的G0拐角速度不受限,实际上什么都不用改，将g0的法向加速度设为【5000-40000】即可,40000对应最大拐角速度也即g0的最大速度
    }

    if (sq->corner_mix_flag == 1){
        vmax = sqGiveCornerMixVelocity_five(vmax, s1, s2,sq);
    }

    /* return cornerVelocity */

     /*****************************************************************************/
    return vmax;
}


/*****************************************************************************/
//add by heyuebang in 2015-12-13
static int sqCircleFitting_five(SEGMENT * s1, SEGMENT * s2, SEGMENTQUEUE * sq){
        double r1,r2,r3,r4;
        int i;
        double w1;
        SEGMENT *cursor,*preseg;
        double vmax;
        double temp;
    //第一步：以弓高误差 sq->tolerance计算直线段看成圆弧需要的最小半径R1 = (（L1/2)^2/sq->tolerance + sq->tolerance)/2
    //R2 = (（L2/2)^2/sq->tolerance + sq->tolerance)/2,如果两条直线的三个端点共圆半径R3=||s2->end-s1->start||/2/sin(s2->angle)
    //大于等于R1,R2，则可认为可以由R3半径的圆弧拟合这两条直线。实际为排除夹角过小和过大（直线180度），增加两个条件，及夹角>135度和拐点速度不大于这两条直线的最大速度
   // sq->tolerance=0.0;
   //  if (!sq->seglinkflag) return vmax; //如果不使用小线段看成圆弧功能,退出

    if (s1->type == SQ_LINEAR  && s2->type == SQ_LINEAR && sq->tolerance >= 0.001)
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
                r3 = DBL_MAX;

            if (r3>=r1 && r3>=r2 && ( s2->angle > (PM_PI-PM_PI/4.0)))
            {  //该半径大于弓高容许最小半径，就认为s1和s2是圆弧的两段
                //增加判断，拐点速度不能大于直线最大速度，大于认为是直线，没有必要再拟合成圆了。
              if (( (s1->type_circle_seg ==0) && (s1->finalInc < min(s1->maxInc,s2->maxInc))) || ((s1->type_circle_seg == 1) && (s2->initInc < s2->maxInc)))
               {    //ok，设置s1,s2小线段看成圆弧标志
                        s1->type_circle_seg =1;
                        s2->type_circle_seg = 1;

                        if (sq->omega_limit_flag){
                        temp = max(sq->G_mode_1.maxJerk,sq->G_mode_1.maxAcc_circle*sqrt(sq->G_mode_1.maxAcc_circle*0.2));
                        vmax = pow(temp* r3* r3,0.333333);
                         vmax =min(vmax, sqrt(sq->G_mode_1.maxAcc_nCircle * r3)); //计算圆弧最大速度
                        }else{
                            vmax = sqrt(sq->G_mode_1.maxAcc_nCircle* r3);
                        }

                        s2->maxInc = min(s2->maxInc,vmax * sq->cycleTime);
                         s1->maxInc = min(s1->maxInc,vmax * sq->cycleTime);
                        s2->initInc = min(s2->maxInc,s1->maxInc);
                        s1->finalInc = s2->maxInc;

               }

            }
    }


  //  diagnostics("s2->Id=%d,s2->angle=%d,s2->type_circle_seg=%d,s2->length=%d,vmax = %d, r3=%d,r1=%d,r2=%d\n",s2->ID,(int)(s2->angle*180/PM_PI*1000),s2->type_circle_seg,(int)(s2->length*1000),(int)(vmax*1000),(int)(r3*1000),(int)(r1*1000),(int)(r2*1000));
   //if (s2->ID >1 && s2->ID <650){
  //  diagnostics("s2->Id=%d,s2->angle=%d,s2->type_circle_seg=%d,s2->length=%d, s2->initInc=%d\n",s2->ID,(int)(s2->angle*180/PM_PI*1000),s2->type_circle_seg,(int)(s2->length*1000),(int)(s2->initInc*1000000));
  //  }

//第二步：圆弧拟合，为了减小误差引入导致R3变化太大，以10（该数据需要根据实际适当调整，太小，速度变化还是可能较大，太大，又难以正确描述该处曲率）个数据做最小二乘法，求R3
    i=0;
    if (s2->type_circle_seg == 1)
    {
      //  i=0;
        r1 = 0.0;
        r2 = 0.0;
        cursor = s2;
        preseg = s1;
        while((i<10) &&(preseg->type_circle_seg >=1)&&(cursor->angle>(PM_PI-PM_PI/4.0))){
                  i++;
                 //以长度做权重（这样处理可以避免线段太小造成的误差，但某条长线段会严重影响结果）
                  w1 = min(cursor->length,preseg->length);
                 //为避免太长或错误引入线段造成曲率拟合较大误差，限制权重最大值
                  if (w1> 0.02)
                      w1 = 0.02;
                  //最小二乘拟合
                  r1 += w1*sin(cursor->angle) * w1*sin(cursor->angle) *2.0;
                  r2 += w1*sin(cursor->angle) * w1* sqGiveLength(cursor->end,preseg->start);
                  cursor= cursor->prevSegment;
                  preseg = preseg->prevSegment;
                  if ((preseg == 0) ||(preseg->active == 1))
                      break;
        }
    //程序运行到这里，i即为拟合的直线段数
        //达到10段拟合时，小线段够多了，测试第一段和最后一段是否引入合理，尽量消除错误连接
        if ((i >= 5) && (r1 != 0.0) && (r2 !=0.0) )
        {
            //测试新段，第一步计算权重
            w1 = min(s2->length,s1->length);
             if (w1> 0.02)
                 w1 = 0.02;
              //将需要拟合的参数减掉新段
             r3= r1 - w1*sin(s2->angle) * w1*sin(s2->angle) *2.0;
             r4 = r2 - w1*sin(s2->angle) * w1* sqGiveLength(s2->end,s1->start);

             if ((r3 == 0) || (r4 == 0))
             {
                 return 0;
             }
             //计算有新段和没新段的拟合结果差别，此处1.1需要调整，1.1估计太大了
             if (fabs(r2/r1 - r4/r3) > 0.5*sq->tolerance)
             { //差别太大，新段应该不属于此处曲率包含部分
                s2->type_circle_seg = -1;
            //    diagnostics("ss:s2->Id=%d,s2->angle=%d,s2->type_circle_seg=%d,s2->length=%d, s2->initInc=%d,r2/r1=%d,r4/r3=%d\n",s2->ID,(int)(s2->angle*180/PM_PI*1000),s2->type_circle_seg,(int)(s2->length*1000),(int)(s2->initInc*1000000),(int)(r2/r1*1000),(int)(r4/r3*1000));
                return 0;
             }

             if ((s2->length > (s2->prevSegment->length + s2->prevSegment->prevSegment->length)*0.3)
                     && (PM_PI-s2->angle > (PM_PI-min(s2->prevSegment->angle , s2->prevSegment->prevSegment->angle))*1.5 ))
             {
                     s2->type_circle_seg = -1;
                     return 0;
             }

             //程序运行到这，表明新段s2需要参与拟合；

           //去掉第一段，看拟合结果是否相近
            w1 = min(cursor->nextSegment->length,cursor->length);
             if (w1> 0.02)
                 w1 = 0.02;
            r3= r1 - w1*sin(cursor->nextSegment->angle) * w1*sin(cursor->nextSegment->angle) *2.0;
            r4 = r2 - w1*sin(cursor->nextSegment->angle) * w1* sqGiveLength(cursor->nextSegment->end,cursor->start);
         if ((r3 == 0) || (r4 == 0))
         {
             return 0;
         }
         //如果差别大，表明第一段不合适，去掉

                     if (((cursor->length > (cursor->nextSegment->length + cursor->nextSegment->nextSegment->length)*0.3)
                             && (PM_PI-cursor->angle > (PM_PI-min(cursor->nextSegment->angle , cursor->nextSegment->nextSegment->angle))*1.5 )) ||(fabs(r2/r1 -r4/r3) > 0.5*sq->tolerance))
         {
             //cursor是第一段，preseg是其前一段
             if ((preseg !=0) &&(preseg->active !=0) && (preseg->type_circle_seg == 0))
             {   //cursor不是小线段，取回其最大值，此处修改最大速度，但没有修改末点速度（理论上需要，这里为不搞那么复杂，就不调了，因为能被看成小线段，其拐点角度》135度）
                 cursor->maxInc = max(cursor->givemaxInc,cursor->maxInc);//此处取max，这样就可以不要调其初始速度
             }
             //去掉cursor这段，这里放在if外面，表示不管cursor最大值调了没，圆弧拟合都不再使用这段
             cursor = cursor->nextSegment;
             r1 = r3;
             r2 = r4;
          //   diagnostics("a:s2->Id=%d,s2->angle=%d,s2->type_circle_seg=%d,s2->length=%d, s2->initInc=%d\n",s2->ID,(int)(s2->angle*180/PM_PI*1000),s2->type_circle_seg,(int)(s2->length*1000),(int)(s2->initInc*1000000));
         }



        }

  // 程序运行到这，基本上排除了一些异常，接xia来需要修改小线段最大速度和拐点速度了。

           //计算圆弧r3 = r2/r1

           if ((r1 != 0.0)&&(r2 !=0.0)){
               r3 = r2/r1;  // the circle radius;
               if (sq->omega_limit_flag){
               r1 = max(sq->G_mode_1.maxJerk,sq->G_mode_1.maxAcc_circle*sqrt(sq->G_mode_1.maxAcc_circle*0.2));
               vmax = pow(r1* r3* r3,0.333333);
                vmax =min(vmax, sqrt(sq->G_mode_1.maxAcc_nCircle * r3)); //计算圆弧最大速度
               }else{
                   vmax = sqrt(sq->G_mode_1.maxAcc_nCircle* r3);
               }


//              if (s2->ID >196 && s2->ID <205){
//               rtapi_print_msg(RTAPI_MSG_ERR," cirlcefitting:as->ID=%d, vmax=%d,sq->G_mode_1.maxAcc_nCircle=%d,r3=%d,\n",s2->ID,(int)(vmax*1000),(int)(sq->G_mode_1.maxAcc_nCircle*1000),(int)(r3*1000));
//      }

                if (vmax<=0.0){
                    diagnostics("error in conrnervelocity: s2->Id=%d\n",s2->ID);
                    return -1;
                    }
                 //调整s2最大速度，s1和s2的拐点速度（此处不调整拐点速度也可以，因为预处理函数会调整）
               if (i == 10)
                {  //线段足够多，拟合曲率应该准确，为减小预处理结果为太短线段，应该尽量让这些小线段速度一致
                    if (fabs(vmax * sq->cycleTime - s1->maxInc) < 0.1*s1->maxInc)
                    { //新拟合速度与s1速度误差不大时，将s2的最大速度设为和s1一致
                        s2->maxInc = min(s2->maxInc,s1->maxInc);
                        s2->initInc = s2->maxInc;
                        s1->finalInc = s2->maxInc;
                        return 1;
                    }
                        //否则表示新拟合的圆弧半径和之前拟合的不一样，需要更新曲率了，因此最大速度设为新拟合的
                    s2->maxInc = min(s2->maxInc,vmax * sq->cycleTime);
                    s2->initInc = s2->maxInc;
                    s1->finalInc = s2->maxInc;
                            return 1;
                }
                 //程序运行到这，表明i<10,为防止i太小拟合误差大，以及过多小线段速度不一致，将所有参与拟合的小线段速度设为一致
                 //cursor指向参与拟合的第一段
                        s2->maxInc = min(s2->maxInc,vmax * sq->cycleTime);
                        preseg = s1;//从s1开始
                 if(i>=5){
                        i = 0;
                       // i为防止进入死循环
                       while (i<10){
                           i++;
                       //    diagnostics("i=%d",i);
                            //测试前一段是否正在运行或刚刚运行完，ok，则调整到第一段cursor为止
                           if ((preseg->prevSegment == 0) ||(preseg->prevSegment->active == 1)|| (preseg == cursor))
                           {  //第一段了
                               preseg->maxInc = max(preseg->initInc,s2->maxInc);//由于其初始速度不好修改，为保证规划不出问题，简化处理
                               break;
                           }
                           //不是第一段，修改了最大速度和拐点速度可能会造成链接错误，将其和前面一段链接，注意，所有和s1链接了的，即使错误也不要紧，因为预处理函数就是将其看成错误的。
                           sqLinkSegments_five(preseg->prevSegment,preseg,SQ_HIGH_LINKING_PRIORITY);
                           if (preseg->linkedToPrevSeg !=1){ //这个应该不会发生
                               preseg->maxInc = max(preseg->initInc,s2->maxInc);
                               break;
                           }else{ //链接成功
                               //调整最大速度
                                   preseg->maxInc = min(s2->maxInc,preseg->givemaxInc);
                                //调整preseg起点和preseg前段末点速度
                           }

                         preseg = preseg->prevSegment;

                       }


                         //程序运行到这，最大速度都调整好了，现在需要调整拐点速度了。
                       while(preseg != s2)
                       {
                           preseg->finalInc = min(preseg->maxInc,preseg->nextSegment->maxInc);
                           preseg->nextSegment->initInc = preseg->finalInc;
                           preseg->lengthFlag = 0;
                           preseg = preseg->nextSegment;
                       }
                 }
                 return 1;
        }
    }
    return 0;
}

//计算s逻辑段上最小的切线加速度，每一段的切线加速度在addline时给出 ghf-2014/6/10
static double sqGiveMinAmaxTan_five(SEGMENT * s) {
    /* give the minimum tangential acceleration for the chain of linked that starts with s. */
    SEGMENT *cursor;
    double minAmax;

    if (s == 0) {
        diagnostics("Error1 in sqGiveMinAmaxTan_five()\n");
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

//当g0不使用倍率调整时，返回最大增量，否则返回结果乘以倍率
static double sqGiveMaxIncWithFeedOverrate_five(SEGMENTQUEUE * sq, SEGMENT *s){
    if (s->gmodetype == 0 && sq->segpara_i[0] == 0) //g0是否受倍率控制
        return s->maxInc;
    else
        return s->maxInc*sq->feedOverrideFactor;
}

static double sqGiveMaxInc_five(SEGMENTQUEUE * sq, SEGMENT * s) {
    /* give the right maxInc from the chain of linked segments that start with s. */
    int done = 0;
    int minNumSteps;
    double startInc, finalInc;
    SEGMENT *cursor,*prevseg;
    double l;
    double maxAccTan;
    //int i = 0;
   // int tmp;
    double maxInc;
    int count=0;

    if (sq == 0 || s == 0) {
        diagnostics("Error 1 in sqGiveMaxInc_five()\n");
        return -1;
    }

    if (s->numLinkedSegs == 0) //如果当前逻辑段只有一段，直接返回
        return sqGiveMaxIncWithFeedOverrate_five(sq,s);
    // return s->maxInc;



    maxAccTan = sqGiveMinAmaxTan_five(s); //否则先找出逻辑段上最小切线加速度

    startInc = s->plInitInc; //初始增量
    cursor = s;
    l = s->length; //判断够不够长的长度
    while (!done) {
        count++;
        if (count > 200) break;

        if (cursor->nextSegment == 0|| cursor->nextSegment->linkedToPrevSeg!=1) { //如果到逻辑段的末尾还不够长时，直接返回末尾的maxinc
                    // return cursor->maxInc;
                     return sqGiveMaxIncWithFeedOverrate_five(sq,cursor);
        }
        if (cursor->nextSegment != 0 //找够走的终止速度
        && cursor->nextSegment->maxInc < cursor->maxInc)
            finalInc = min(cursor->finalInc,
                    cursor->nextSegment->maxInc * sq->feedOverrideFactor);
        else
            finalInc = min(cursor->finalInc,
                    cursor->maxInc * sq->feedOverrideFactor);

//        minNumSteps = ceil(
//                3 * fabs(finalInc - startInc) / (2 * maxAccTan * sq->ctPow2));

//        minNumSteps = max(3, minNumSteps); //ghf-2014/6/20

//        if ((minNumSteps + 1) * (startInc + finalInc) / 2
//                - startInc> l - 2 * startInc - finalInc - (p_h+2) * max(finalInc, startInc)) { //公式4.4.35，不够继续找 //ghf-2014/6/20

            minNumSteps = ( 3 * fabs(finalInc - startInc) / (2 * maxAccTan * sq->ctPow2));

            if ((minNumSteps + 1) * (startInc + finalInc) / 2.0  +(p_h+1+1)*max(startInc, finalInc) +(finalInc- startInc)/2.0>
                  l){
        /* this segment was too short and therefore linked to the next one */
            cursor = cursor->nextSegment;
            l += cursor->length;
         //   i++;
        } else {
            done = 1;
        }
    }

    maxInc = sqGiveMaxIncWithFeedOverrate_five(sq,cursor); //够走，返回够走了的末尾段的maxinc，那么这个maxinc可能是整个逻辑段中最大的增量，那么路过其他段的时候，不会超速吗？？ghf-2014/6/10
    /*********************************************************************/
    //add by heyuebang 2015-12-25
    //找到段尾
    prevseg = cursor;
     while(prevseg->nextSegment !=0 && prevseg->nextSegment->linkedToPrevSeg == 1)
           prevseg = prevseg->nextSegment;

     if (prevseg->nextSegment != 0 //找够走的终止速度
     && prevseg->nextSegment->maxInc < prevseg->maxInc)
         finalInc = min(prevseg->finalInc,
                 prevseg->nextSegment->maxInc * sq->feedOverrideFactor);
     else
         finalInc = min(prevseg->finalInc,
                 prevseg->maxInc * sq->feedOverrideFactor);

       //从尾开始往回搜索够长减速到finalInc的段
      done =0;
      l = prevseg->length;
      count = 0;
      while (!done) {
          count++;
          if (count > 200) break;

          if (prevseg == cursor) { //如果到逻辑段的末尾还不够长时，直接返回末尾的maxinc

              //  return cursor->maxInc;
                return sqGiveMaxIncWithFeedOverrate_five(sq,cursor);
          }
          if (prevseg->prevSegment->maxInc < prevseg->maxInc)
              startInc = min(prevseg->initInc,
                      prevseg->prevSegment->maxInc * sq->feedOverrideFactor);
          else
              startInc = min(prevseg->initInc,
                      prevseg->maxInc * sq->feedOverrideFactor);

//          minNumSteps = ceil(3 * fabs(finalInc - startInc) / (2 * maxAccTan * sq->ctPow2));

//          minNumSteps = max(3, minNumSteps); //ghf-2014/6/20

//          if ((minNumSteps + 1) * (startInc + finalInc) / 2
//                  - startInc> l - 2 * startInc - finalInc - (p_h+2)* max(finalInc, startInc)) { //公式4.4.35，不够继续找 //ghf-2014/6/20
              minNumSteps = ( 3 * fabs(finalInc - startInc) / (2 * maxAccTan * sq->ctPow2));

              if ((minNumSteps + 1) * (startInc + finalInc) / 2.0  +(p_h+1+1)*max(startInc, finalInc) +(finalInc- startInc)/2.0>
                    l ){

          // this segment was too short and therefore linked to the next one
              prevseg = prevseg->prevSegment;
              l += prevseg->length;
             // i++;
          } else {
              done = 1;
          }
      }


    //ghf-2014/10/25,找到当前逻辑段的最大值,原来就是这段代码导致程序最大速度不受控的ghf-2014/10/30
   // tmp = s->numLinkedSegs -i;


   // for (i = 0; i <= tmp; i++) {//ghf-2015/7/9

        //diagnostics("cursor->ID=%d\n", cursor->ID);
   //好了，到这，最大速度应该在cursor和prevseg之间，
  while (cursor  != prevseg)
  {
            if (sqGiveMaxIncWithFeedOverrate_five(sq, cursor->nextSegment) > maxInc){
                    maxInc = sqGiveMaxIncWithFeedOverrate_five(sq, cursor->nextSegment);
            }

        //if (cursor->nextSegment != 0 && cursor->nextSegment->linkedToPrevSeg == 1) {
            cursor = cursor->nextSegment;
      //  }

    }

/********************************************************************************************************/
    //diagnosticsOff("max increment for chain starting at %d is %g\n", s->ID, cursor->maxInc);

    return maxInc;

    rtapi_print_msg(RTAPI_MSG_ERR,"maxInc=%d ",(int)(maxInc*1000));
}

static int sqLinkCriterion_five(SEGMENTQUEUE * sq, SEGMENT * s, SEGMENT * end,
        double length, double feedOverride) {
    /* this function returns SQ_LINKING_NEEDED if the chain of segments starting
     with s is not long enough to accelerate or decelerate from the initial
     increment of the first segment in the chain to the final increment of the
     last segment in the chain
     */
    double amaxTan;
    double initInc, finalInc;
    int minNumSteps;
    SEGMENT *lastSeg;

    if (sq == 0 || s == 0) {
        diagnostics("Error1 in sqLinkCriterion_five\n");
        return -1;
    }

    if (s->linkedToPrevSeg == 1) {
        diagnostics(
                "Error2,Segment s is linked to its preceding segment in sqLinkCriterion_five,s->ID=%d\n",
                s->ID);
        return -1;
    }

    /* find the last segment of the chain */
    lastSeg = end; //hyb-2014/7/2

    /* find the minimum boundary for the acceleration in tangential direction */
    /* FIXME: this can be combined with the previous for statement that
     looks up the last segment of the chain */
    amaxTan = sqGiveMinAmaxTan_five(s);

    /* find the correct value for the initial increment: when s->plInitInc is
     non zero, this means that the segment has been processed before, and that
     value should be used */
    if (0 != s->plInitInc)
        initInc = s->plInitInc;
    else //hyb-2014/7/2
    {
        initInc = s->initInc;
        if (feedOverride >= 1.0) {
            initInc = s->initInc;
        } else {
            if (s->prevSegment != 0 && s->prevSegment->maxInc < s->maxInc)
                initInc = min(s->initInc,
                        s->prevSegment->maxInc * feedOverride);
            else
                initInc = min(s->initInc, s->maxInc * feedOverride);
        }
    }

    /* find the correct value of the final increment (with taking a feedoverride
     into account). */
    if (feedOverride >= 1.0) {
        finalInc = lastSeg->finalInc;
    } else {
        if (lastSeg->nextSegment != 0
                && lastSeg->nextSegment->maxInc < lastSeg->maxInc)
            finalInc = min(lastSeg->finalInc,
                    lastSeg->nextSegment->maxInc * feedOverride);
        else
            finalInc = min(lastSeg->finalInc, lastSeg->maxInc * feedOverride);
    }

   // minNumSteps = ceil(3 * fabs(initInc - finalInc) / (2 * amaxTan * sq->ctPow2));

    /********************************************************************************/
//此处修改主要考虑sqPlanSegment_five()函数将最小步数进行了限制（限制为2）。
//增加最小步数限制为3，这个参数不能小于2.
//    minNumSteps = max(3, minNumSteps);
    minNumSteps = (3 * fabs(initInc - finalInc) / (2 * amaxTan * sq->ctPow2));
//    if ((minNumSteps + 1) * (initInc + finalInc) / 2 - initInc>
//    length - 2 * initInc - finalInc - 5 * max(initInc, finalInc)) { //此处跟givemaxinc相比，多了2步初始，1步终止，切最小步数不能小于3 ghf-2014/6/10？？？？
    //-------------------------------------
    // p_h+1 maxV is set
        if ((minNumSteps + 1) * (initInc + finalInc) / 2.0  +(p_h+1+1)*max(initInc, finalInc) +(finalInc- initInc)/2.0>
                length ) {
           //   length - 2 * initInc - finalInc ) {
    //----------------------------------
        diagnosticsOff("Chain starting at %d is too short - linking needed\n", s->ID);
        return SQ_LINKING_NEEDED;
    } else {
        diagnosticsOff("Chain starting at %d is NOT too short - linking NOT needed\n", s->ID);
        return SQ_LINKING_NOT_NEEDED;
    }
    /********************************************************************************/
}
//static int iii = 0;
static int sqPlanSegment_five(SEGMENTQUEUE * sq, SEGMENT * s) {
    /* variable declaration */

    /* 2014-5-31 for test */
    int startclocks = 0, endclocks = 0;

    startclocks = rtapi_get_time();

    double length;
    double maxInc;
    double amaxTan;
    //  double maxIncNextSeg;
    int tmp = 0;
    SEGMENT *cursor;
     double vi;
     int n;
    int validSolutionFound;
    double intmaxInc; //heyuebang 2014-10-09
    //int minNumSteps;

    if (sq == 0) {
        diagnostics("Error1  in sqPlanSegment_five()\n");
        return -1;
    }

    /* check is s is a valid pointer */
    if (s == 0) {
        diagnostics("Error2 in sqPlanSegment_five()\n");
        return -1;
    }

    /* check if this segment is linked to its predecessor (which should never
     happen!) */
    if (s->linkedToPrevSeg == 1) { //小线段不停启动暂停可出现此情况ghf-2014/6/16
        diagnostics("s->ID=%d,s->numsegment=%d,Error3 in sqPlanSegment_five()\n",
                s->ID, s->numLinkedSegs);
        sq->done = 1; //ghf-2014/6/16
        return -1;
    }


    cursor = s;

    //rtapi_print("2014--s->ID= %d;s->numsegment=%d ,s->start.tran.x=%d,s->start.tran.y=%d,s->start.tran.z=%d\n",s->ID, s->numLinkedSegs,(int)(s->start.tran.x*1000),(int)(s->start.tran.y*1000),(int)(s->start.tran.z*1000));

    /* check if there are any segments linked to s, find the total length of the
     linked segments and find the finalInc of the last segment in the chain */


    //cursor = s + cursor->numLinkedSegs;//不用while用此句cursor->ID可能出现负值。ghf-2014/6/17

    //rtapi_print("2214--cursor->ID= %d;cursor->numsegment=%d\n",cursor->ID, cursor->numLinkedSegs);


    if ( (maxInc = sqGiveMaxInc_five(sq, s) ) < 0.0) { //sqGiveMaxInc_five不是已经考虑了倍率了吗？为什么这里还要再考虑ghf-2014/6/10
        diagnostics("Error4 in sqPlanSegment_five()\n");
        sq->done = 1;//控制runcycle不停地打印ghf-2014/10/25
        return -1;
    }

    tmp =0;
    if (cursor->gmodetype == 0) tmp=1;
    while (cursor->nextSegment != 0 && cursor->nextSegment->linkedToPrevSeg) {
        cursor = cursor->nextSegment;
        if(cursor->gmodetype != 0) tmp=0;
    }

//    //ghf-20150713
////    if(emcmotStatus->g0_usefeedrate == 0) {
////        if(cursor->gmodetype==0){//如果逻辑段的末尾一段为g0，那么这一段的最大速度应该为g0_maxv
////            maxInc = sq->G_mode_0.maxV * sq->cycleTime;
////        }
////        if(maxInc > (sq->G_mode_0.maxV * sq->cycleTime)) {
////            maxInc = sq->G_mode_0.maxV * sq->cycleTime;
////        }
////    }

//    if (tmp == 0 || (emcmotStatus->g0_usefeedrate != 0 && tmp== 1)){
//        maxInc *= sq->feedOverrideFactor;
//    }

    //maxInc = 0.13333;

    //diagnostics("  maxInc = %d\n", (int) (1000000.0 * maxInc));

    /* maxInc should never exceed the the system's maximum increment
     (maxV*cycleTime) */
    //maxInc = min(maxInc, sq->maxV * sq->cycleTime);
    //defaultVel = maxInc / sq->cycleTime; //add by lsj, on 08.2.27

    /* find the minimum value for the maximum tangential acceleration for this
     chain of segments beginning with s */

    amaxTan = sqGiveMinAmaxTan_five(s);
    if(amaxTan == -1){
        diagnostics("Error5 in sqPlanSegment_five()\n");
        return -1;
    }
    //amaxTan = 500;

    //diagnosticsOff("AmaxTan in sqPlanSegment_five  = %d \n", (int) (ceil(amaxTan)));
    /* finalInc ( = initInc of nextSegment) should never be larger than
     the maxInc * feedOveride of this segment and the next segment */

    if (cursor->finalInc != 0) { /* if true: this is not the last segment */
        if (cursor->nextSegment->maxInc < cursor->maxInc)
            //if (fabs(cursor->nextSegment->maxInc - cursor->maxInc) < 1e-6)
            s->plFinalInc = min(cursor->finalInc,
                    cursor->nextSegment->maxInc * sq->feedOverrideFactor);
        else
            s->plFinalInc = min(cursor->finalInc, maxInc);
        cursor->nextSegment->plInitInc = s->plFinalInc;
    } else
        s->plFinalInc = 0;

    /* the first two steps and the last step are already defined */
    //length = s->totLength - s->plInitInc;
   // length = s->totLength - 2 * s->plInitInc - s->plFinalInc;
    length = s->totLength;
    if (length < 0) {
        diagnostics("Error6,Panic: length negative in sqPlanSegment_five\n");
        diagnostics("plInitInc = %ld plFinalInc = %ld  length = %ld\n", (long)(s->plInitInc*10000),(long)(s->plFinalInc*10000),(long)(length*10000));
        sq->done = 1; //控制runcycle不停地打印ghf-2014/10/25
        return -1;
    }

/**************************hyb-2014/12/17**********************************************************
    minNumSteps = ceil(
            3 * fabs(s->plInitInc - s->plFinalInc) / (2 * amaxTan * sq->ctPow2));
    minNumSteps = max(3, minNumSteps);
    if ((minNumSteps + 1) * (s->plInitInc + s->plFinalInc) / 2 - s->plInitInc>
    length - 5 * max(s->plInitInc, s->plFinalInc)) { //此处跟givemaxinc相比，多了2步初始，1步终止，切最小步数不能小于3 ghf-2014/6/10？？？？
        diagnosticsOff("Chain starting at %d is too short - linking needed\n", s->ID);

        minNumSteps  = floor( 2.0*(s->totLength - s->plInitInc - s->plFinalInc - 5 * max(s->plInitInc, s->plFinalInc))/(s->plInitInc + s->plFinalInc)-1);
        if (minNumSteps>=2){
           amaxTan = 3 * fabs(s->plInitInc - s->plFinalInc) / (2 * minNumSteps * sq->ctPow2);
        }
    }
*//****************************************************************************************************/



    //length = s->totLength - s->plInitInc;
    //if ( length < 0 ) length = 0;

    /* calculate minimal number of steps for the first phase (acc) */
    //if (s->plInitInc == maxInc)
    if (fabs(s->plInitInc - maxInc) < 1e-6)
        s->m = 0;
    else {
        /* m should be always be two steps or more */
        tmp = ceil(
                3 * fabs(maxInc - s->plInitInc)
                        / (double) (2 * amaxTan * sq->ctPow2));
        s->m = max(1, tmp);
    }

//	  ceil(3 * fabs(maxInc - s->plInitInc) / (double)
//		  (2 * amaxTan * sq->ctPow2)));

    /* calulate minimal number of steps for the third phase (dec) */
    //if (s->plFinalInc == maxInc)
    if (fabs(s->plFinalInc - maxInc) < 1e-6)
        s->q = 0;
    else {
        tmp = ceil(
                3 * fabs(maxInc - s->plFinalInc)
                        / (double) (2 * amaxTan * sq->ctPow2));
        s->q = max(1, tmp);
    }

//	  ceil(3 * fabs(maxInc - s->plFinalInc) / (double)
//	       (2 * amaxTan * sq->ctPow2)));

    /* calculate the minimal number of steps needed for the cruising phase */

    //rtapi_print("length=%d,s->totLength=%d,maxInc=%d\n",(int)(length*1000),(int)(s->totLength*1000),(int)(maxInc*1000));
    //rtapi_print("m=%d,p=%d,q=%d\n",s->m,s->p,s->q);
    //rtapi_print("in sqplan,s->ID=%d,s->plInitInc=%d,s->cruiseInc=%d,s->plFinalInc=%d\n",s->ID,(int)(s->plInitInc*1000),(int)(s->cruiseInc*1000),(int)(s->plFinalInc*1000));
//if条件中增加了3.0*maxInc，论文中建议5.0*maxInc。注意：系数越大，会造成计算的maxInc越小。
//如果改成5，则上一个函数sqLinkCriterion_five对应需增加2.
    if (length
            - ((s->m + 1) * (s->plInitInc + maxInc) / 2 - s->plInitInc)
                    * (s->m != 0)
            - ((s->q + 1) * (maxInc + s->plFinalInc) / 2 - maxInc) * (s->q != 0)
            - (double)p_h * maxInc < 0) { //为什么要加上3个maxinc呢？？ghf-2014/6/21 保证一定要有3个匀速
        /* true: there is no cruising phase */
        s->p = p_h;
        validSolutionFound = 0;
        /****************************************************************************************/
//verified by Heyuebang in 1/27/2014
//这段原始程序实现有问题，没有很好理解论文中的意思。因此做了修改。
//论文中原意是假设maxInc处于哪个范围，然后按照这个范围的计算方法计算，最后比较结果是否处于该范围。
//因此应该先计算再比较，而不是先设置if条件。
        //if (maxInc > s->plInitInc && maxInc > s->plFinalInc) {
        maxInc = -s->p * amaxTan * sq->ctPow2 / 3
                + sqrt(
                        4 * s->p * s->p * amaxTan * amaxTan * sq->ctPow2
                                * sq->ctPow2
                                + 18
                                        * (s->plInitInc * s->plInitInc
                                                + s->plFinalInc * s->plFinalInc)
                                + 12 * amaxTan * sq->ctPow2
                                        * (s->plInitInc - s->plFinalInc
                                                + 2 * length)) / 6;
        //diagnosticsOff("1: maxInc = %d\n", (int) (100000.0 * maxInc));
        //if(iii == 1)
        //    rtapi_print("000maxInc=%d,s->plInitInc=%d,s->plFinalInc=%d\n",(int)(maxInc*1000),(int)(s->plInitInc*1000),(int)(s->plFinalInc*1000));
        //if (maxInc > s->plInitInc && maxInc > s->plFinalInc) {
        if (maxInc > s->plInitInc && maxInc > s->plFinalInc) {
            validSolutionFound = 1;
            //rtapi_print( "000\n");
        }
        //}

        if (!validSolutionFound) { //ghf-2014/6/11
            maxInc = (3
                    * (s->plFinalInc * s->plFinalInc
                            - s->plInitInc * s->plInitInc)
                    + 2 * amaxTan * sq->ctPow2
                            * (s->plInitInc - s->plFinalInc + 2 * length))
                    / (4 * s->p * amaxTan * sq->ctPow2);
            diagnosticsOff("2: maxInc = %d\n", (int) (100000.0 * maxInc));
            //diagnostics("2: maxInc = %d\n", (int) (100000.0 * maxInc));
            //if(iii == 1)
            //    rtapi_print("111maxInc=%d,s->plInitInc=%d,s->plFinalInc=%d\n",(int)(maxInc*1000),(int)(s->plInitInc*1000),(int)(s->plFinalInc*1000));
            if (s->plInitInc >= maxInc && maxInc > s->plFinalInc) {
                validSolutionFound = 1;
                //rtapi_print( "111\n");
            }
        }

        if (!validSolutionFound) {
            maxInc = (3
                    * (s->plInitInc * s->plInitInc
                            - s->plFinalInc * s->plFinalInc)
                    + 2 * amaxTan * sq->ctPow2
                            * (s->plInitInc - s->plFinalInc + 2 * length))
                    / (4 * s->p * amaxTan * sq->ctPow2);
            diagnosticsOff("3: maxInc = %d\n", (int) (100000.0 * maxInc));
            //if(iii == 1)
            //    rtapi_print("222maxInc=%d,s->plInitInc=%d,s->plFinalInc=%d\n",(int)(maxInc*1000),(int)(s->plInitInc*1000),(int)(s->plFinalInc*1000));
            if (s->plInitInc < maxInc && maxInc <= s->plFinalInc) {
                validSolutionFound = 1;
                //rtapi_print( "222\n");
            }
        }

        if (!validSolutionFound) {
            maxInc = s->p * amaxTan * sq->ctPow2 / 3
                    + sqrt(
                            4 * s->p * s->p * amaxTan * amaxTan * sq->ctPow2
                                    * sq->ctPow2
                                    + 18
                                            * (s->plInitInc * s->plInitInc
                                                    + s->plFinalInc
                                                            * s->plFinalInc)
                                    + 12 * amaxTan * sq->ctPow2
                                            * (-s->plInitInc + s->plFinalInc
                                                    - 2 * length)) / 6;
            /*rtapi_print("4: sqrtmaxInc = %d\n", (int) ((4 * s->p * s->p * amaxTan * amaxTan * sq->ctPow2
             * sq->ctPow2
             + 18
             * (s->plInitInc * s->plInitInc
             + s->plFinalInc
             * s->plFinalInc)
             + 12 * amaxTan * sq->ctPow2
             * (-s->plInitInc + s->plFinalInc
             - 2 * length)))*1000000000);*/

            //if(iii == 1)
            //    rtapi_print("333maxInc=%d,s->plInitInc=%d,s->plFinalInc=%d\n",(int)(maxInc),(int)(s->plInitInc*1000),(int)(s->plFinalInc*1000));
            if (maxInc <= s->plInitInc && maxInc <= s->plFinalInc) {
                validSolutionFound = 1;
                //rtapi_print( "333\n");
            }
        }
/*************************hyb-2014/12/17**********************************/
        if (!validSolutionFound) {
            diagnostics("Error7,No solution found in sqPlanSegment_five,s->id = %d,s->type=%d,s->totLength=%f\n", s->ID,s->type,s->totLength);
            if ( s->plFinalInc == 0 && s->plInitInc == 0)
            {
                vi = sqrt (4 * amaxTan * length / 3);
                n =  3 * vi / (4 * amaxTan * sq->cycleTime);
                maxInc = 4 * amaxTan * n * sq->ctPow2 / 3 ;
                validSolutionFound = 1;
            }else{
                n = 2.0*length/(s->plInitInc+s->plFinalInc)-1;//ghf-2014/12/18
                s->q = ceil(n);
                s->m=0;s->p=0;
                if (s->q > 0) {
                    s->b3 = 0;
                    s->a3 = 0;
                    s->c3 = (s->plFinalInc-s->plInitInc)/s->q/sq->cycleTime;
                    s->d3 = s->plInitInc;
                } else
                    s->a3 = s->b3 = s->c3 = s->d3 = 0;

                s->cruiseInc = s->plInitInc;


                //s->totNumPoints = s->m + s->p + s->q + 3;
               s->totNumPoints = s->m + s->p + s->q;
                s->planningDone = 1;

                return 0;
            }
        }
/***********************************************************/

        /****************************************************************************************/
        /* recalculate m and q */
        s->m = ceil(
                3 * fabs(maxInc - s->plInitInc) / (2 * amaxTan * sq->ctPow2));
        s->q = ceil(
                3 * fabs(maxInc - s->plFinalInc) / (2 * amaxTan * sq->ctPow2));
        s->p = ceil(
                (length
                        - (s->m != 0)
                                * ((s->m + 1) * (s->plInitInc + maxInc) / 2
                                        - s->plInitInc)
                        - (s->q != 0)
                                * ((s->q + 1) * (s->plFinalInc + maxInc) / 2
                                        - maxInc)) / maxInc);
    } else
        s->p = ceil(
                (length
                        - (s->m != 0)
                                * ((s->m + 1) * (s->plInitInc + maxInc) / 2
                                        - s->plInitInc)
                        - (s->q != 0)
                                * ((s->q + 1) * (s->plFinalInc + maxInc) / 2
                                        - maxInc)) / maxInc);

    /* adjust maxInc a little */
    // heyuebang 2014-10-09
    intmaxInc = (2 * length - (s->m != 0) * (s->m - 1) * s->plInitInc
            - (s->q != 0) * (s->q + 1) * s->plFinalInc)
            / ((s->m != 0) * (s->m + 1) + 2 * s->p + (s->q != 0) * (s->q - 1));

    //GHF-2014/11/20,调整p导致剩下的一点会跟后面的连，哪怕是直角
    /*if (intmaxInc>=max(s->plInitInc,s->plFinalInc)||
            (intmaxInc >= s->plFinalInc && s->plInitInc > s->plFinalInc && (maxInc - intmaxInc) < (s->m+1)*amaxTan *sq->ctPow2/10)||
              sq->stepping == 1){ //
          s->cruiseInc = intmaxInc;
    }else{
        s->p -= 1;
        if (s->p<0){
            diagnostics("Error8 in sqPlanSegment_five: s->p<0.\n");
        }
        s->cruiseInc = maxInc;
    }*/

    s->cruiseInc = intmaxInc;//ghf-2014/1/14


    //s->totNumPoints = s->m + s->p + s->q + 3;
    s->totNumPoints = s->m + s->p + s->q;
   // s->cruiseInc = maxInc;

    /* ok, we've found (sub)-optimal values for m, p and q. Let's go on
     with the actual planning */

    if (s->m != 0) {
        s->b1 = 3 * (s->cruiseInc - s->plInitInc) / ((s->m * s->m * sq->ctPow2));
        s->a1 = -2 * s->b1 / (3 * s->m * sq->cycleTime);
        s->c1 = 0;
        s->d1 = s->plInitInc;
    } else
        s->a1 = s->b1 = s->c1 = s->d1 = 0;

    if (s->q != 0) {
        s->b3 = 3 * (s->plFinalInc - s->cruiseInc) / ((s->q * s->q * sq->ctPow2));
        s->a3 = -2 * s->b3 / (3 * s->q * sq->cycleTime);
        s->c3 = 0;
        s->d3 = s->cruiseInc;
    } else
        s->a3 = s->b3 = s->c3 = s->d3 = 0;

    s->planningDone = 1;

    endclocks = rtapi_get_time() - startclocks;
    if (endclocks > 250000) {
        rtapi_print("amaxTan = %d,s->ID = %d,s->numsegment=%d，Plansegment time = %d\n",	(int)(amaxTan*1000), s->ID, sq->numSegments, endclocks);
    }

    //if (sq->numSegments <= 100) {
    //if(s->ID == 1843)
    //if((s->plInitInc==0)||(s->plFinalInc==0))
        //rtapi_print("in sqplan,s->ID=%d,maxinc=%d,s->numLinkedSegs = %d,s->totLength = %d,sq->numsegment=%d，sq->feedOverrideFactor = %d,amaxTan = %d,s->ID=%d,s->plInitInc=%d,s->cruiseInc=%d,s->plFinalInc=%d,cursor->ID=%d,s->initInc=%d,CURSOR->finalInc=%d\n\n",s->ID,(int)(s->maxInc*1000),s->numLinkedSegs,(int)(s->totLength*1000),sq->numSegments, (int)(sq->feedOverrideFactor*1000),(int) (amaxTan*1000),s->ID,(int)(s->plInitInc*1000000),(int)(s->cruiseInc*1000000),(int)(s->plFinalInc*1000000),cursor->ID,(int)(s->initInc*1000000),(int)(cursor->finalInc*1000000));
        //rtapi_print("end of sqplan,m=%d,p=%d,q=%d,end.x=%d,end.y=%d,end.z=%d,cursor->ID=%d\n\n",s->m,s->p,s->q,(int)(cursor->end.tran.x*1000),(int)(cursor->end.tran.y*1000),(int)(cursor->end.tran.z*1000),cursor->ID);
    //}


   // reportError("in sqplan,s->ID=%d,maxinc=%d,s->numLinkedSegs = %d,s->plInitInc=%d,s->cruiseInc=%d,s->plFinalInc=%d\n",s->ID,(int)(s->maxInc*1000),s->numLinkedSegs,(int)(s->plInitInc*1000),(int)(s->cruiseInc*1000),(int)(s->plFinalInc*1000));
    //rtapi_print("in sqplan,start.x=%d,start.y=%d,start.z = %d,s->gmodetype=%d\n",(int)(s->start.tran.x*1000),(int)(s->start.tran.y*1000),(int)(s->start.tran.z*1000),s->gmodetype);
    //rtapi_print("end of sqplan,m=%d,p=%d,q=%d,end.x=%d,end.y=%d,end.z=%d,cursor->ID=%d\n\n",s->m,s->p,s->q,(int)(cursor->end.tran.x*1000),(int)(cursor->end.tran.y*1000),(int)(cursor->end.tran.z*1000),cursor->ID);

    //iii = 0;

    return 0;
}

/* function to link s2 to s1
 Here it is assumed that s1 and s2 are indeed successive segments */
static int sqLinkSegments_five(SEGMENT * s1, SEGMENT * s2, int priority) {

    //diagnostics("Linking %d to %d,s1->numLinkedSegs=%d,s2->numLinkedSegs=%d\n", s1->ID, s2->ID,s1->numLinkedSegs,s2->numLinkedSegs);
    if (s1 == 0) {
        diagnostics("Error 1 in sqLinkSegments_five() \n");
        return -1;
    }
    if (s2 == 0) {
        diagnostics("Error 2 in sqLinkSegments_five() \n");
        return -1;
    }

    /* if s2 is not the segment that immediately follows s1, then linking
     can not be done */

    //直接传头进来hyb-2014/7/2
  /*  if (s1->nextSegment != s2) {
     diagnostics("Error 2 in sqLinkSegments_five()\n");
     return -1;
     }*/
/*******************************************************************/
 //add by heyuebang in 2015-12-13
    if (s2->linkedToPrevSeg == 1) {
     //diagnostics("Error 2 in sqLinkSegments_five()\n");
        return 0;
     }
/*******************************************************************/
    /* if s1 is active, linking should not be done */
    //if(iii==0){
    if (s1->active == 1) {
        diagnostics(
                "Error 3 in sqLinkSegments_five(),s1->active == 1,s1->ID=%d,s2->ID=%d\n",
                s1->ID, s2->ID);//打印此错没什么影响，只是证明线段加的不够快，或者一开始的时候会报一次ghf-2014/10/24
        //return -1;
        return 0;
    }
    //}

    /* find the first element in the series of linked segments of which s1 is
     the last one */
    while (s1->linkedToPrevSeg) {
        if (s1 == 0) {
            diagnostics("Error 4,Panic: NULL pointer in sqLinkSegments_five");
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


    /*    if (priority==SQ_HIGH_LINKING_PRIORITY) */
    /*      {   */
    /*        s2->amaxTan = s2->amaxTan * 0.50; */
    /*      } */

    return 0;
}

//director = 1, forward; director = -1, backward, others, return;
static void sqLengthFlagSet_five(SEGMENT * s, int director ){
    SEGMENT *cursor;
    cursor = s;
     cursor->lengthFlag = 1;
     if (director == 1){
         while (cursor->nextSegment !=0 && cursor->nextSegment->linkedToPrevSeg == 1){
             cursor = cursor->nextSegment;
             cursor->lengthFlag = 1;
         }
     }

     if (director == -1){
         while (cursor->linkedToPrevSeg==1 && cursor->prevSegment !=0 && cursor->prevSegment->active == 0){
             cursor = cursor->prevSegment;
             cursor->lengthFlag = 1;
         }
     }
}


static int sqForceForwardLinkSegment_five(SEGMENTQUEUE * sq, SEGMENT * s, SEGMENT * end,
        double feedOverride) {
    /* function that checks whether segment s needs to be linked to its
     successor(s) based upon its initial increment. It is possible that
     this requires linking of a set of succeding segments to make sure that
     the final increment can be reached. */

    SEGMENT *cursor;
    int done, counter;
    int linkcrit;

   // int i = 0;

    //diagnostics("s->id = %d end->id = %d\n", s->ID,end->ID);

    if (sq == 0 || s == 0) {
        diagnostics("Error1 in sqForceForwardLinkSegment_five,1");
        return -1;
    }

    cursor = end; //hyb-2014/7/2
    done = 0;
    counter = 0;

    s->initInc = s->plInitInc; //ghf-2014/6/21

    while (!done) {
        counter++;
        if (counter > sq->size) {
            /* we have browsed through the whole queue and can't
             get out of the loop.... */
            diagnostics(
                    "Error2,Can't get out of the loop in sqForceForwardLinkSegment_five\n");
            return -1;
        }
        if (cursor->nextSegment == 0) {
            /* then this is the last segment in the queue, which always has
             a final velocity of zero that can always be reached. Ergo: we're
             done */
            done = 1;
        } else if (cursor->nextSegment->linkedToPrevSeg == 1)
        /* the next segment is already linked to cursor, so let's go on */
        {

            cursor = cursor->nextSegment;
            if (cursor == 0) {
                /* this is VERY unlikely to happen, since we already checked
                 that a few lines before, but just in case...... */
                diagnostics(
                        "Error3,Panic: cursor = NULL in sqForceForwardLinkSegment_five\n");
                return -1;
            }
        } else {
            if (-1
                    == (linkcrit = sqLinkCriterion_five(sq, s, cursor, s->totLength,
                            feedOverride))) { //hyb-2014/7/2
                diagnostics("Error4 in sqForceForwardLinkSegment_five,2\n");
                return -1;
            } else if (linkcrit == SQ_LINKING_NEEDED) {
               // sqLengthFlagSet_five(cursor, -1); // start-- cursor set not long flag;
                cursor = cursor->nextSegment;
                if (cursor == 0) {
                    /* again, this is very unlikely to happen */
                    diagnostics(
                            "Error5,Panic: cursor = NULL in sqForceForwardLinkSegment_five\n");
                    return -1;
                }
                if (-1 == sqLinkSegments_five(s, cursor, //hyb-2014/7/2
                        SQ_HIGH_LINKING_PRIORITY)) {
                    diagnostics("Error6 in sqForceForwardLinkSegment_five,3\n");
                    return -1;
                }

            } else
                /* linkcrit==SQ_NO_LINKING_NEEDED */
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
    return 0;
}

static int sqBackwardLinkSegment_five(SEGMENTQUEUE * sq, SEGMENT * start, SEGMENT * s,
        double feedOverride) {
    /* function that checks whether segment s needs to be linked to its
     predecessor(s) based upon the finalInc. It is possible that
     this requires linking of a set of preceding segments to make sure that
     the final increment can be reached. */

    SEGMENT *cursor;
    int done, counter;
    int linkcrit;

    diagnosticsOff("Checking whether segment %d needs to be linked backward...\n", s->ID);

    if (sq == 0 || s == 0) {
        diagnostics("Error1 in sqBackwardLinkSegment_five\n");
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
            diagnostics("Error2,Can't get out of the loop in sqBackwardLinkSegment_five\n");
            return -1;
        }

        if (cursor->linkedToPrevSeg == 1)
        /* it is already linked */
        {
            cursor = cursor->prevSegment;
            if (cursor == 0) {
                diagnostics("Error3,Panic: cursor = NULL in sqBackwardLinkSegment_five\n");
                return -1;
            }
        } else if (cursor->initInc < s->finalInc + 1e-6) //ghf-2014/12/16
            /* then we're done. This is always true as long as the set of
             segments that precede s is correctly processed, since the
             total length of the segments from cursor to the last segment
             before s is large enough to reach s's initial increment. This
             length is of course large enough too to reach s->finalInc
             because that is smaller than s->initInc (otherwise this
             function wouldn't have called). */
            done = 1;

        else {
            if (-1
                    == (linkcrit = sqLinkCriterion_five(sq, cursor, s,
                            cursor->totLength, feedOverride))) { //hyb-2014/7/2
                diagnostics("Error4 in sqBackwardLinkSegment_five\n");
                return -1;
            } else if (linkcrit == SQ_LINKING_NEEDED) {
               // sqLengthFlagSet_five(cursor, 1 ); //set cursor--> end not long enough flag;
                cursor = cursor->prevSegment;
                if (cursor == 0) {
                    diagnostics(
                            "Error5,Panic: cursor = NULL in sqBackwardLinkSegment_five\n");
                    return -1;
                }
                if (-1 == sqLinkSegments_five(cursor, cursor->nextSegment, //当前逻辑段跟前面一个逻辑段链接ghf
                        SQ_HIGH_LINKING_PRIORITY)) {
                    diagnostics("Error6 in sqBackwardLinkSegment_five\n");
                    return -1;
                }diagnosticsOff("backward %d",cursor->nextSegment->ID);
            } else
                done = 1; /* no further linking needed */
        }
        if (cursor == sq->queue + sq->start) {
            diagnosticsOff
            ("Beginning of queue reached, no further linking possible\n");
            done = 1;
        }
    }diagnosticsOff("... done linking backward\n");
    return 0;
}


static int sqInsertInitCircle_five(SEGMENTQUEUE * sq, EmcPose start, EmcPose end,
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

/* please add this parameter in function sqAddLine_five, such as
     sqAddLine_five(SEGMENTQUEUE * sq, EmcPose end, int ID,int G_mode)
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
  insertseg->length = circle.angle * insertseg->helixRadius;

  insertseg->totLength = insertseg->length;
  insertseg->givenLength = insertseg->length;

  insertseg->start = start;
  insertseg->end = end;

    //  diagnostics("insertseg->ID=%d, insertseg->length=%d,insertseg->R=%d,insertseg->angle=%d\n",insertseg->ID,(int)(insertseg->length*1000),(int)(insertseg->circle.radius*1000),(int)(insertseg->circle.angle*1000*180/3.1415926));
 // newseg->maxInc = min(sq->feed * sq->cycleTime, sqrt(sq->maxAcc *circle.radius) * sq->cycleTime);
//****************************************************************************************************

//changed by heyuebang in 2/17/2014
     if (G_number == 0){
            insertseg->maxInc = min(G_mode.maxV, sqrt(G_mode.maxAcc_circle * circle.radius ))* sq->cycleTime;

      }
     else{
            insertseg->maxInc = sqrt(G_mode.maxAcc_circle * circle.radius) * sq->cycleTime;

                  }
     insertseg->maxInc = min(insertseg->maxInc, G_mode.maxV* sq->cycleTime);

     insertseg->givemaxInc = insertseg->maxInc;
//***************************************************************************************************

  insertseg->finalInc = 0;
  insertseg->plInitInc = 0;
  insertseg->plFinalInc = 0;
  insertseg->cruiseInc = 0;
  insertseg->planningDone = 0;
  insertseg->active = 0;
  insertseg->numLinkedSegs = 0;
  insertseg->linkedToPrevSeg = 0;
  insertseg->totNumPoints = 0;
  insertseg->nextSegment = 0;
  insertseg->gmodetype = G_number;

  insertseg->amaxTan = G_mode.maxAcc_circle;

insertseg->amaxNorm =G_mode.maxAcc_nCircle;
  insertseg->jmaxTan= G_mode.maxJerk;//this is wrong,not used.

  insertseg->lengthFlag = 0;

 // if (insertseg->ID == 29){
    //  diagnostics("bef:insertseg->ID=%d,amax=%d,jmax=%d,gmode.maxacc=%d,g=%d\n",insertseg->ID,(int)(insertseg->amaxTan),(int)(insertseg->jmaxTan),(int)(G_mode.maxAcc),G_number);
 //}

 if (absHelix != 0) {
    insertseg->amaxTan = min(insertseg->amaxTan,
              G_mode.maxAcc * insertseg->helixRadius / absHelix);

  }

 //if (insertseg->ID == 29){
  //   diagnostics("aft:insertseg->ID=%d,amax=%d,jmax=%d,gmode.maxacc=%d,g=%d\n",insertseg->ID,(int)(insertseg->amaxTan),(int)(insertseg->jmaxTan),(int)(G_mode.maxAcc),G_number);
 //}
//***************************************************************************


  /* set last Appended Motion ID */
  sq->lastAppMotionID = ID;
//  diagnostics("add circle!\n");
  return 0;
}


//***************************************************************************************************

static int sqInsertCircle_five(SEGMENTQUEUE * sq,  SEGMENT *s1, SEGMENT *s2){
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
                //如果s1和s2类型不一致，或者之间夹角小于90度，或者大于180度，退出
             if(s1->gmodetype != s2->gmodetype || s2->angle > PM_PI - 0.001)
                 return 0;


             if(s2->angle < sq->optimized_angle &&(s1->gmodetype==1)) return 0;

             if (sq->numSegments < 2) return 0; //如果剩下的段只有1段了，退出

             //根据类型不同(g0,g1)设置不同的容差
             if (s2->gmodetype == 0){
                 tolerance =  sq->circle_tolerance_g0;
             }
             else{
                  tolerance =  sq->circle_tolerance_g1;
             }


             if (tolerance < 0.001) return 0;
              //printf("now get the filter parameter:%f\n",tolerance);

             //新增加一段
            newseg = sq->queue + sq->end;
               /* ...and update the ring buffer properties */
               sq->end = (sq->end + 1) % sq->size;
//               sq->numSegments++;
//              if (sq->numSegments >= sq->size - SQ_SAFETY_MARGIN) {
//                 sq->full = 1;
//               }
           // if (s2->ID < 30)
            // diagnostics("s1->ID=%d,s2->ID=%d\n",s1->ID,s2->ID);
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

              //计算圆心和方向
             pmCartCartCross(s1->line.uVec, s2->line.uVec, &normal);
             pmCartUnit(normal, &normal);

 //diagnostics("l=%d\n,",(int)(l*1000));

             pmCartScalMult(s1->line.uVec, l, &v1);
            // pmCartMag(v1, &costheta);
             //diagnostics("s1 v1.x=%d,v1.y=%d,v1.z=%d,|v1|=%d,l=%d\n,",(int)(v1.x*1000),(int)(v1.y*1000),(int)(v1.z*1000),(int)(costheta*1000),(int)(l*1000));
             pmCartCartSub(s1->end.tran, v1, &end.tran);
             pmCartCartCross(normal, s1->line.uVec, &v1);
             pmCartUnit(v1, &v1);
             //pmCartMag(v1, &costheta);
            // diagnostics("||r v1=%d||,",(int)(costheta*1000));
             pmCartScalMult(v1, R, &v1);
            // pmCartMag(v1, &costheta);
            // diagnostics("||r v1=%d\n||,",(int)(costheta*1000));
             pmCartCartAdd(end.tran, v1, &center);
             pmCartScalMult(s2->line.uVec, l, &v1);
             pmCartCartAdd(s2->start.tran, v1, &start.tran);


        //     diagnostics("be: s1->ID=%d, s1->length=%d,l=%d,r=%d,s2->length=%d,s2->angle=%d\n",s1->ID,(int)(s1->length*1000),(int)(l*1000),(int)(R*1000),(int)(s2->length*1000),(int)(s2->angle*1000*180/3.1415926));
          //  diagnostics("be:s1->end.x=%d, s1->end.y=%d,s2->start.x=%d,s2->start.y=%d,s2->end.x=%d,s2->end.y=%d\n",(int)(s1->end.tran.x*1000),(int)(s1->end.tran.y*1000),(int)(s2->start.tran.x*1000),(int)(s2->start.tran.y*1000),(int)(s2->end.tran.x*1000),(int)(s2->end.tran.y*1000));
           //  diagnostics("s2->line.uVec.x=%d,s2->line.uVec.y=%d,s2->line.uVec.z=%d\n", (int)(s2->line.uVec.x*1000),, (int)(s2->line.uVec.y*1000) ,, (int)(s2->line.uVec.z*1000)   );

//changing the length of the preseg;
            if (-1==sqInsertInitCircle_five(sq, end, start, center, normal, s2->ID, s2->gmodetype, s2)){
                diagnostics("Error 4 in sqInsertCircle_five()\n");
                return -1;
               }
             //*s2 = *cursor;
             s2->amaxTan = min(s1->amaxTan,newseg->amaxTan);
             s2->jmaxTan = min(s1->jmaxTan,newseg->jmaxTan);
             s2->maxInc = max(s2->maxInc,s1->finalInc);
             s2->initInc = min(s1->maxInc,s2->maxInc);
             s1->finalInc = s2->initInc;
             s2->finalInc = min(s2->maxInc,newseg->maxInc);
             newseg->initInc = s2->finalInc;
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



        //    diagnostics("af:s1->ID=%d, s1->length=%d,newseg->length=%d,newseg->id=%d,s2->length=%d,s2->angle=%d\n",s1->ID,(int)(s1->length*1000),(int)(newseg->length*1000),(int)(newseg->ID),(int)(s2->length*1000),(int)(s2->angle*1000*180/3.1415926));
          //     diagnostics("af:s1->end.x=%d, s1->end.y=%d,newseg->start.x=%d,newseg->start.y=%d,s2->start.x=%d,s2->start.y=%d,s2->end.x=%d,s2->end.y=%d\n",(int)(s1->end.tran.x*1000),(int)(s1->end.tran.y*1000),(int)(newseg->start.tran.x*1000),(int)(newseg->start.tran.y*1000),(int)(s2->start.tran.x*1000),(int)(s2->start.tran.y*1000),(int)(s2->end.tran.x*1000),(int)(s2->end.tran.y*1000));

             // s2->angle = 0.0;

                     if (-1 == sqLinkSegments_five(s2->prevSegment, s2, //hyb-2014/7/2
                             SQ_HIGH_LINKING_PRIORITY)) {
                         diagnostics("Error 10 in sqInsertCircle_five\n");
                         return -1;
                     }


        return 0;
}



static int zerofeedoverflag = -1;


int sqPreprocessLink(SEGMENTQUEUE * sq, SEGMENT * newseg) {

    /* this is the stuff that's needs to be done for both linear and
     circular motions. Only sqAddLine_five and sqAddCircle_five should call this
     function and it must be used after that sq->end+=1 .
     */

 //   SEGMENT *prevseg;
    SEGMENT *cursor;

   // static SEGMENT *lastSeg =  0;

   // SEGMENT *segend;
    SEGMENT *temp;

    int done;
   // int totnumber;
    int linkcrit, counter;

    double length;
    int linkedNumber;

    int backlingflag=0;
    //rtapi_print("Preprocess Segment %d\n", newseg->ID);

   // rtapi_print("");

    /* 2014-5-31 for test */
    int startclocks = 0, endclocks = 0;
    startclocks = rtapi_get_time();




            if (newseg->active == 1){
                diagnostics("Error 00 in sqPreprocessLink\n");
                return 0;
            }

            if (newseg->prevSegment->active == 1){
                diagnostics("Error 01 in sqPreprocessLink\n");
                return 0;
            }


         if(newseg->linkedToPrevSeg == 0) {
                if (-1 == sqLinkSegments_five(newseg->prevSegment, newseg, //hyb-2014/7/2
                        SQ_HIGH_LINKING_PRIORITY)) {
                    diagnostics("Error 0 in sqPreprocessLink\n");
                    return -1;
                }
         }

         if (newseg->linkedToPrevSeg == 0) return 0;

         cursor = newseg;
       //  temp = newseg;
    //     totnumber = 0;

            //先指向逻辑段段头
            while (cursor->linkedToPrevSeg == 1) {
          //       if (cursor == lastSeg) totnumber=1;
                cursor = cursor->prevSegment;
                if (cursor == 0 || cursor->active==1){
                    diagnostics("Error 10 in sqPreprocessLink\n");
                    return -1;
                }
            }



            done = 0;
            temp = cursor;
            length = cursor->length;
            linkedNumber = 0;
            counter = 0;



            while (!done) {
                counter++;
                if (counter > sq->size) {
                    /* we have browsed through the whole queue and can't
                     get out of the loop.... */
                    diagnostics(
                            "Error 2 Can't get out of the loop in sqPreprocessLink\n");
                    return -1;
                }

                if (temp ->active == 1)
                    return 0;

                if (temp == newseg)
                    return 0;

                if(temp->lengthFlag == 1&& (temp->nextSegment != 0)&&temp->nextSegment->lengthFlag==1){
                        temp = temp->nextSegment;
                        length += temp->length;
                        linkedNumber += 1;

                }else if((linkedNumber <= SQ_MAX_NUM_LINKED_SEGS) && (temp->nextSegment != 0)
                       && (fabs(temp->maxInc - temp->nextSegment->maxInc)  < 1e-6)
                       && (temp->finalInc   == min(temp->maxInc, temp->nextSegment->maxInc))
                       && cursor->initInc <= temp->finalInc + 1e-6) {
                                sqLengthFlagSet_five(temp, -1 ); // set cursor-temp length not long flag;
                                temp = temp->nextSegment;
                                length += temp->length;
                                linkedNumber += 1;

                } else  if (-1 == (linkcrit = sqLinkCriterion_five(sq, cursor, temp, length,  sq->maxFeedOverrideFactor))) {    //hyb-2014/7/5,第一次进来时，判断的是newseg前面的逻辑段的第一段够不够走ghf-2014/8/19
                    diagnostics(
                            "Error 3 in sqPreprocessLink: linkcrit = -1\n");
                    return -1;
                }  else if (linkcrit == SQ_LINKING_NEEDED) {
                    sqLengthFlagSet_five(temp, -1 ); // set cursor-temp length not long flag;
                    if (((cursor->initInc > temp->finalInc + 1e-6) && (cursor->prevSegment!=0)
                         && (cursor->prevSegment->active!=1))//保证连到一起的速度是增加的ghf-2014/12/16
                            || (temp->nextSegment == 0) ) {
                        backlingflag = 1; // need back link;
                        done = 1;
                    } else {
                        if (linkedNumber > SQ_MAX_NUM_LINKED_SEGS){
                            done = 1;
                        }else{
                            temp = temp->nextSegment;
                            length += temp->length;
                            linkedNumber += 1;
                        }
                    }
                } else {
                     sqLengthFlagSet_five(temp, -1 );
                        done = 1;
                    }

                 //end if
            } //end while



             if (temp == newseg) return 0;

            if ((temp != newseg) && (temp->nextSegment->linkedToPrevSeg)) {//断成两个逻辑段
                temp->nextSegment->linkedToPrevSeg = 0;
                temp->nextSegment->totLength = cursor->totLength - length;
                temp->nextSegment->numLinkedSegs = cursor->numLinkedSegs
                        - linkedNumber - 1;
                cursor->totLength = length;
                cursor->numLinkedSegs = linkedNumber;
            }

            if (-1 == sqBackwardLinkSegment_five(sq, temp->nextSegment, newseg, //hyb-2014/7/2
                    sq->maxFeedOverrideFactor)) {
                diagnostics(
                        "Error 8 in sqPreprocessLink()\n");
                return -1;
            }

            if (backlingflag == 1 && temp->nextSegment->linkedToPrevSeg != 1){
                if (-1 == sqBackwardLinkSegment_five(sq, cursor, temp, //hyb-2014/7/2
                        sq->maxFeedOverrideFactor)) {
                    diagnostics(
                            "Error 9 in sqPreprocessLink()\n");
                    return -1;
                }

                 sqLengthFlagSet_five(temp, -1 ); // set cursor-temp length not long flag;
            }


            endclocks = rtapi_get_time() - startclocks;
             if (endclocks >= 200000) {//0.2ms
                 rtapi_print("sqPreprocessLink: cursor->ID= %d;Pre segment time = %d,cursor->numLinkedSegs=%d\n", cursor->ID, endclocks,cursor->numLinkedSegs);
             }

             // newseg is long enough
         if (temp->nextSegment->linkedToPrevSeg != 1 && temp->nextSegment != newseg){
            return 1;
         }

             //   sqBackwardLinkSegment_five(sq, newseg, newseg,sq->maxFeedOverrideFactor);            //hyb-2014/7/2

    return 0;
}


static int sqPreprocessSegment_five(SEGMENTQUEUE * sq, SEGMENT * newseg, GMODE *G_mode) {

    /* this is the stuff that's needs to be done for both linear and
     circular motions. Only sqAddLine_five and sqAddCircle_five should call this
     function and it must be used after that sq->end+=1 .
     */

    double cornerInc;

    SEGMENT *prevseg;
    SEGMENT *cursor;
//    int circlelinkflag = 0;

    int sqprep_again = 1;
    int cout = 0;


    /* check if segment queue has been initialized and if newseg is valid */
    if (sq == 0 || sq->queue == 0 || newseg == 0) {
        diagnostics("Error 1 in sqPreprocessSegment_five()-- sq not init\n");
        return -1;
    }

   // sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
    /* if this is the first segment.... */
    if (((sq->end+sq->size-sq->start)%sq->size) == 1) {
        newseg->initInc = 0;
        newseg->start = sq->lastPoint;
        newseg->prevSegment = 0;
        //if (sq->paused != 1 && sq->stepping != 1)
        if (sq->paused != 1 && sq->stepping != 1 && sq->feedOverrideFactor != 0) //ghf-2014/10/25
        {
            sq->done = 0;//这句就是真正的启动runcycle的开关，ghf-2014/10/25
        }
        //ghf-2014/8/25,方便第一次启动时倍率为0,不能动，托倍率后，直接运行
        if(sq->feedOverrideFactor == 0) {
            sq->paused = 1;
            zerofeedoverflag = 1;//ghf-2014/8/26,无此句，暂停时拖到0,点退出，再点运行，拖到非0不动
        }
    } else {
        /* if not ... */
        prevseg = sq->queue + ((sq->end + sq->size - 2) % sq->size);
        //2 means this function only can be used after that the sq->end has been increased by 1 in function addline or addcircle.
        newseg->start = prevseg->end;
        prevseg->nextSegment = newseg;
        newseg->prevSegment = prevseg;

         cursor = prevseg;
         while (cursor->linkedToPrevSeg) cursor = cursor->prevSegment;
         cursor = cursor->prevSegment;

        if (prevseg->active == 1 || (cursor == 0 && sq->delayTime > 6) || (cursor !=0 && cursor->active == 1)) {
         // if (prevseg->active == 1) {
            /* we can't change anything of the active segment */
            //rtapi_print("2014--prevseg is active,we can't change anything!\n");
            newseg->initInc = prevseg->finalInc;//前面一段正在跑，新加的不做处理，保证速度连续，后续还会处理吗？ghf-2014/8/19
        } else {
            /* calculate the corner velocity for the corner between prevSegment
             and newseg */
            //ghf-2014/10/27,相邻线段是g0，g1时，计算拐角以g1的为准
            if(prevseg->gmodetype != newseg->gmodetype) {
                if (-1
                        == (cornerInc = sqGiveCornerVelocity_five(prevseg, newseg,
                                 sq->G_mode_1.maxAcc_nLine,sq) * sq->cycleTime)) {
                    diagnostics(
                            "Error 2 in sqPreprocessSegment_five()--sqgivecornerVel error!\n");
                    return -1;
                }
            }
            else {
                if (-1
                        == (cornerInc = sqGiveCornerVelocity_five(prevseg, newseg,
                                 G_mode->maxAcc_nLine,sq) * sq->cycleTime)) {
                    diagnostics(
                            "Error 2 in sqPreprocessSegment_five()--sqgivecornerVel error!\n");
                    return -1;
                }
            }

//****************************************************************************************
//prevseg段链接可能有误，需要重新链接
//比如，某段初始速度由拐点受限速度决定，为0.1，当它需要链接，
//且是最后一段时，它会和之前的段链接。然而，当有新段加入时，其末速由拐点受限速度重新计算，
//为0.2，则它应该和新段链接而不是和之前的段链接，因此链接错误。

            /* the corner velocity shouldn't exceed the maximum
             speeds of both segments */
            if ((cornerInc > prevseg->maxInc) || (cornerInc > newseg->maxInc))
                cornerInc = min(prevseg->maxInc, newseg->maxInc);

            newseg->initInc = cornerInc;
            prevseg->finalInc = cornerInc;

//            if (newseg->ID >196 && newseg->ID <205){
//             rtapi_print_msg(RTAPI_MSG_ERR," cor:as->ID=%d, as->cirlcelink=%d,as->linked=%d,as->initinc=%d,as->finalinc=%d,as->maxinc=%d,pre->maxInc=%d, as->angle=%d,as->amaxnorm=%d,\n",newseg->ID,newseg->type_circle_seg,newseg->linkedToPrevSeg,(int)(newseg->initInc*1000000),(int)(newseg->finalInc*1000000),(int)(newseg->maxInc*1000000),(int)(prevseg->maxInc*1000000),(int)(newseg->angle*1000000),(int)(newseg->amaxNorm*1000000));
//    }
            //如果seglinkflag是1,并且newseg和prevseg模式都是g1,则进行圆弧拟合
 /*           if(sq->seglinkflag == 1 && newseg->gmodetype == 1 && prevseg->gmodetype == 1){
               circlelinkflag = sqCircleFitting_five(prevseg, newseg, sq);
            }
*/
              //circlelinkflag为0表示圆弧拟合失败，在这个条件下，如果插入圆弧标志为1,则调用圆弧插入函数
          //  diagnostics("now get the filter parameter:%d, %d\n",sq->circle_insert_flag,(int)(sq->circle_tolerance_g1*1000));
/*            if (circlelinkflag == 0 && sq->circle_insert_flag == 1){
                  if(-1 == sqInsertCircle_five(sq,  prevseg, newseg)){
                       diagnostics(
                               "Error  in sqPreprocessSegment_five()--sqInsertCircle_five error!\n");
                   }
            }else{
                if (circlelinkflag == -1){
                    diagnostics(
                            "Error  in sqPreprocessSegment_five()--sqCircleFitting_five error!\n");
                }

            }
*/
           //圆弧插入后newseg会向后增加一段，调整之
           if (newseg->nextSegment !=0) newseg= newseg->nextSegment;

           prevseg = newseg->prevSegment;
            //prevseg指向头
           while(prevseg->linkedToPrevSeg) {
               prevseg = prevseg->prevSegment;
           }
           //调用预处理函数
           while (sqprep_again == 1){
                 if (-1 == (sqprep_again = sqPreprocessLink(sq, newseg))){
                     diagnostics(
                             "Error  in sqPreprocessSegment_five()--sqPreprocessLink error0!\n");
                 }
                 cout ++;
                 if (cout >5) break;
           }


//****************************************************************************************
        }            //end of prevseg is not active
    }            //end of newseg is not the first seg in sq



    return 0;
}


/* interface functions */
/* ------------------- */

int sqInitQueue_five(SEGMENTQUEUE * sq, SEGMENT * first, int size) {
    if (size <= 0 || sq == 0 || first == 0) {
        diagnostics("Error in sqInitQueue_five()\n");
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
    sq->n = 0;
    sq->maxFeedOverrideFactor = 1.2;
    sq->feedOverrideFactor = 1.0;
    sq->cycleTime = 0;
    //sq->maxAcc = 0;
    // sq->maxV = 0;
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
    sq->oldDist = 0;

//*****************************************************************************
    //add by heyuebang in 2015-12-13
    sq->tolerance = 0.025; //容许使用误差，小线段
   // sq->number = 0; //not used;
   // sq->T_constant = 0; //速度超前参数，没用

    //sq->queue[sq->size - 1].end = sq->lastPoint; //初始化时将最后一段的末点设为开始点

        sq->filter_const = 5; //初始定义5个数据滤波
        sq->is_or_not_filterV = 1; //滤波使能
    sqInitFilter(sq); //速度滤波参数

     sqInitPointFilter(sq);

     sq->G_mode_0.maxJerk = 200000.0;
      sq->G_mode_0.maxAcc_circle= 12000.0;
      sq->G_mode_0.maxAcc_nCircle= 12000.0;
     sq->G_mode_1.maxJerk = 100000.0;

   //  sq->seglinkflag = 0;  //小线段看成圆弧计算最大速度标志
      sq->tolerance = 0.025; //容许使用误差，小线段

     sq->corner_mix_flag = 0; //拐点速度调整（根据拐角大小调整放大缩小因子）

     sq->circle_insert_flag = 0; //插入圆弧标志
     sq->circle_tolerance_g1 = 0.01; //g1插入圆弧容许误差
     sq->circle_tolerance_g0 = 1.0;//g0插入圆弧容许误差
     //sq->dist_has_gone = 0.0;
    // sq->dec_velocity_factor = 1.0; //没用

    // sq->not_long_flag = 0;//小线段标志


     sq->omega_limit_flag = 0;//角速度限制标志

     sq->optimized_angle = PM_PI_2 + 0.001;

//*****************************************************************************
    return 0;
}


int sqSetPos_five(SEGMENTQUEUE * sq, EmcPose pos) {
    int i;
    if (sq == 0) {
        diagnostics("Error in sqSetPos_five()\n");
        return -1;
    }
    sq->initXYZ = pos;
    sq->lastPoint = sq->initXYZ;

    for ( i=0;i<SQ_POINT_FILTER_MAX;i++)
    {
         sq->FilteredPoint[i] = sq->lastPoint;
    }
    sq->Point_Filtered_LastPoint = sq->lastPoint;

    memset(sq->filter,0, SQ_FILTER_MAX_NUMBER * sizeof(double)); //滤波容器参数初始化
   // printf("now set lastpoint");
      sqInitFilter(sq); //速度滤波参数
    return 0;
}

int sqClearQueue_five(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqClearQueue_five(): sq == 0 \n");
        return -1;
    }
    sq->numSegments = 0;
    sq->start = sq->end = 0;
    sq->full = 0;
    sq->n = 0;
    sq->currentID = 0;

    sq->done = 1; /* the queue is empty, so by definition we're done */
    sq->done_filter=1;

    sq->paused = 0;
    sq->stepping = 0;
    sq->feedAdjusting = 0;
    sq->aborting = 0;

    sq->currentVel = 0;
    sq->oldDist = 0;
//    oldPos = sq->lastPoint;
//    oldVel.tran.x = 0;
//    oldVel.tran.y = 0;
//    oldVel.tran.z = 0;

    //  剩余距离清零
    sq->dynamicparameter[10] = 0;
    sq->dynamicparameter[11] = 0;
    sq->dynamicparameter[12] = 0;
    sq->dynamicparameter[13] = 0;


    sq->delayTime = 0; //启动延时
    //rtapi_print("in sqClearQueue_five,sq->numSegments=%d\n",sq->numSegments);
   sq->pausefinish = 0;

   sqInitFilter(sq); //速度滤波参数
    return 0;
}

//ghf-2014/7/4 未用
int sqClearoldpos_five(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqClearoldpos_five(): sq == 0 \n");
        return -1;
    }

    sq->numSegments = 0;
    sq->start = sq->end = 0;
    sq->full = 0;
    sq->n = 0;

    sq->done = 1; /* the queue is empty, so by definition we're done */
    sq->paused = 0;
    sq->stepping = 0;
    sq->feedAdjusting = 0;
    sq->aborting = 0;

    sq->oldDist = 0;
//    oldPos = sq->lastPoint;
//    oldVel.tran.x = 0;
//    oldVel.tran.y = 0;
//    oldVel.tran.z = 0;
    return 0;
}

int sqAddLine_five(SEGMENTQUEUE * sq, EmcPose end, int ID, unsigned char gmodetype,double *dynamicpar) //ghf-2014/10/27
        {
    double length, maxUVec;
    SEGMENT *newseg;
    EmcPose start;
    PmPose start_pose, end_pose;
//    static int idflag = 1;

    /********************************************************************************/
    GMODE G_mode;
    /* please add this parameter in function sqAddLine_five, such as
     sqAddLine_five(SEGMENTQUEUE * sq, EmcPose end, int ID,int G_mode)
     and delete the following two line.
     */
// G_number应该由sqAddLine_five()函数提供，0表示模式0(G00)，其他表示模式1(G01/G02/G03)；
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
        diagnostics("Error 1 in sqAddLine_five()\n");
        return -1;
    }

    /* check for full */
    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
    if (sq->numSegments == sq->size) {
        diagnostics(
                "Error 2 Panic!!!!, segmentqueue overflows!!!,sq->numSegments = %d,ID=%d\n",
                sq->numSegments, ID);
        return -1;
    }


//    if(idflag == 1) {
//        idflag = 0;
//        getidtonum();
//    }

    //rtapi_print_msg(RTAPI_MSG_ERR,"in addline,sq->numSegments=%d\n",sq->numSegments);
    //rtapi_print("in addline,ID=%d,end.tran.x = %d,end.tran.y = %d,end.tran.z = %d\n",ID,(int)(end.tran.x*1000),(int)(end.tran.y*1000),(int)(end.tran.z*1000));

    /* check if new motion has zero length */
    /********************************************************************************************************/
    if (sq->numSegments == 0)
        start = sq->lastPoint;
    else
        start = sq->queue[(sq->end + sq->size - 1) % sq->size].end;
    /********************************************************************************************************/


    length = sqGiveLength(start, end);

    //rtapi_print("in addline,ID=%d,length=%d/1000\n",ID,(int)(length*1000));


    if (length <= 0.01) {
        // if (length <= 0.01) {
        /* only set ID of last appended motion */
        sq->lastAppMotionID = ID;
        //rtapi_print("in addline,the current line too short\n");
        return 0;
    }

    /* let newseg point to the first empty place in the ring buffer... */
    newseg = sq->queue + sq->end;
    /* ...and update the ring buffer properties */
    sq->end = (sq->end + 1) % sq->size;
   // sq->numSegments++;


    /* fill segment parameter fields */
    newseg->ID = ID;

    newseg->type = 1;//ghf-2014/9/16

    newseg->length = length;
    /**************************************************************/
    //add by heyuebang in 2015-12-25
    newseg->givenLength = length;
    /**************************************************************/
    newseg->totLength = length;
    newseg->start = start;
    newseg->end = end;

    newseg->finalInc = 0;
    newseg->plInitInc = 0;
    newseg->plFinalInc = 0;
    newseg->cruiseInc = 0;
    newseg->planningDone = 0;
    newseg->active = 0;
    newseg->numLinkedSegs = 0;
    newseg->linkedToPrevSeg = 0;
    newseg->totNumPoints = 0;
    newseg->nextSegment = 0;
    newseg->type_circle_seg=0;//ghf-2014/11/27

    int jj=0;
    //前10个是上层传下来的参数值，需要根据实际情况返回
    for(;jj<10;jj++) {
        newseg->dynamicGetpar[jj] = dynamicpar[jj];
    }

      newseg->lengthFlag = 0;

    /* initialize line */
    EmcPose_to_pmpose(newseg->start, &start_pose);
    EmcPose_to_pmpose(newseg->end, &end_pose);
    pmLineInit(&newseg->line, start_pose, end_pose);

    /* set the maximum tangential acceleration for this line */
    maxUVec = max(fabs(newseg->line.uVec.x), fabs(newseg->line.uVec.y));
    maxUVec = max(fabs(newseg->line.uVec.z), maxUVec);
    // newseg->amaxTan = sq->maxAcc / maxUVec;

    /******************************************************************************/
//add by heyuebang in 2/17/2014
//根据模式不同，给newseg设定不同的最大速度，最大加速度，最大法向加速度（处理拐点受限用）
//最大加加速度（暂时没用）；
    if (G_number == 0) {
            newseg->maxInc = G_mode.maxV * sq->cycleTime;
            newseg->givemaxInc = newseg->maxInc;
            newseg->gmodetype = 0;
    }
    else {
        newseg->maxInc = min(sq->feed,G_mode.maxV) * sq->cycleTime;
        newseg->gmodetype = 1;
        newseg->givemaxInc = newseg->maxInc;
    }


    newseg->amaxTan = G_mode.maxAcc; //最大切向加速度，设置的最大加速度/单位向量的最大矢量ghf-2014/6/17
    newseg->amaxNorm = G_mode.maxAcc_nLine; //最大法向加速度
    newseg->jmaxTan = G_mode.maxJerk; //最大加加速度

    if((sq->segpara_i[1] == 1)&&(G_number == 0)){//ghf-2014/12/11
        newseg->maxInc = 500 * sq->segpara_d[0] * sq->cycleTime;
        newseg->amaxTan = 1200 ;
        newseg->amaxNorm = 1200 ;
        newseg->givemaxInc = newseg->maxInc;
    }

    /******************************************************************************/

    //rtapi_print("newseg->ID=%d,maxUVec = %d,newseg->amaxTan = %d\n",newseg->ID,(int) (maxUVec*1000),(int) (newseg->amaxTan*1000));
    //if((newseg->ID>=885)&&(newseg->ID<=888))
      //  rtapi_print("newseg->ID=%d,newseg->gmodetype =%d,G_mode.maxV=%d,newseg->maxInc = %d\n",newseg->ID,(int)newseg->gmodetype,(int)G_mode.maxV, (int) (newseg->maxInc*1000));

    if (-1 == sqPreprocessSegment_five(sq, newseg, &G_mode)) {
        diagnostics("Error 3 in sqAddLine_five()\n");
        return -1;
    }

    /* set last Appended Motion ID */
    sq->lastAppMotionID = ID;

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
    if (sq->numSegments >= sq->size - SQ_SAFETY_MARGIN) {
        sq->full = 1;
    }

    //rtapi_print_msg(RTAPI_MSG_ERR,"in addline,sq->numSegments=%d\n",sq->numSegments);

    return 0;
}

int sqAddCircle_five(SEGMENTQUEUE * sq, EmcPose end, PmCartesian center,
        PmCartesian normal, int turn, int ID, unsigned char gmodetype, double *dynamicpar) {
    SEGMENT *newseg;
    PmCircle circle;
    EmcPose start;
    PmCartesian helix;
    double absHelix;
    PmPose start_pose, end_pose;
    double temp;

    /********************************************************************************/
    GMODE G_mode;
   // double maxV;
    /* please add this parameter in function sqAddLine_five, such as
     sqAddLine_five(SEGMENTQUEUE * sq, EmcPose end, int ID,int G_mode)
     and delete the following two line.
     */
//根据模式不同，提取不同的参数，G_number参数应该由函数sqAddCircle_five（）提供；
    int G_number;  //0 is G0 mode,1 is G1 mode;
    G_number = gmodetype;

    //rtapi_print("end.x=%d/1000,end.y=%d/1000\n",(int)(end.tran.x*1000),(int)(end.tran.y*1000));
    //rtapi_print("center.x=%d/1000,center.y=%d/1000,turn=%d\n",(int)(center.x*1000),(int)(center.y*1000),turn);
    //rtapi_print("normal.x=%d/1000,normal.y=%d/1000,normal.z=%d/1000\n",(int)(normal.x*1000),(int)(normal.y*1000),(int)(normal.z*1000));

    if (G_number == 0)
        G_mode = sq->G_mode_0;
    else
        G_mode = sq->G_mode_1;

    /********************************************************************************/
    /* used to calculate the maximum tangential acceleration */
    // double rpow2, A, topIncPow2;
    /* check if segment queue has been initialized */
    if (sq == 0 || sq->queue == 0) {
        diagnostics("Error 1 in sqAddCircle_five()\n");
        return -1;
    }

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31

    /* check for full */
    if (sq->numSegments == sq->size) {
        diagnostics("Error 2 Panic!!!!, segmentqueue overflows!!!\n");
        return -1;
    }

/*************************************************************************************/
    if (sq->numSegments == 0)
        start = sq->lastPoint;
    else
        start = sq->queue[(sq->end + sq->size - 1) % sq->size].end;
/*************************************************************************************/

    EmcPose_to_pmpose(start, &start_pose);
    EmcPose_to_pmpose(end, &end_pose);
    pmCircleInit(&circle, start_pose, end_pose, center, normal, turn);

    //rtapi_print("----1111,paused = %d\n",sq->paused);


    if (turn < 0)
        turn = -1- turn;

    if((circle.angle - turn * PM_2_PI > -CIRCLE_FUZZ)&&(circle.angle -turn * PM_2_PI < CIRCLE_FUZZ)) {
        circle.angle += PM_PI * 2;
    }

    if (circle.angle == 0) {
        //only set ID of last appended motion
        sq->lastAppMotionID = ID;
        return 0;
    }

    /* Calculate the helix gradient in normal direction */
    pmCartScalDiv(circle.rHelix, circle.angle, &helix);
    pmCartMag(helix, &absHelix);

    /* let newseg point to the first empty place in the ring buffer... */
    newseg = sq->queue + sq->end;
    /* ...and update the ring buffer properties */
    sq->end = (sq->end + 1) % sq->size;
    //sq->numSegments++;
//    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
//    if (sq->numSegments >= sq->size - SQ_SAFETY_MARGIN) {
//        sq->full = 1;
//    }
    /* fill segment parameter fields */
    newseg->ID = ID;

    newseg->type = 2;//ghf-2014/9/16

    newseg->circle = circle;

    newseg->helixRadius = sqrt(
            circle.radius * circle.radius + absHelix * absHelix);
    newseg->length = circle.angle * newseg->helixRadius;

    newseg->totLength = newseg->length;
    /**************************************************************/
    //add by heyuebang in 2015-12-25
    newseg->givenLength = newseg->length;
    /**************************************************************/
    newseg->start = start;
    newseg->end = end;

    //rtapi_print("in addcircle,newseg->ID=%d,G_mode.maxV = %d,G_mode.maxAcc_circle = %d\n",newseg->ID,(int) (G_mode.maxV*1000),(int) (G_mode.maxAcc_circle*1000));

    // newseg->maxInc = min(sq->feed * sq->cycleTime, sqrt(sq->maxAcc *circle.radius) * sq->cycleTime);
//****************************************************************************************************
//changed by heyuebang in 2/17/2014
// 根据模式的不同，给newseg赋值最大速度
    if (G_number == 0) {
        newseg->maxInc =
        min(G_mode.maxV, sqrt(G_mode.maxAcc_circle * circle.radius))
                * sq->cycleTime;

       // maxV = newseg->maxInc / sq->cycleTime;
        if (absHelix != 0)
            newseg->maxInc = min(newseg->maxInc,
                    G_mode.maxV * sq->cycleTime / absHelix);
        newseg->gmodetype=0;

    } else {
        newseg->maxInc =  min(sq->feed, sqrt(G_mode.maxAcc_circle * circle.radius))  * sq->cycleTime;
   //     newseg->maxInc =  sqrt(G_mode.maxAcc_circle * circle.radius)  * sq->cycleTime;
      //  maxV = newseg->maxInc / sq->cycleTime;
//        if (absHelix != 0)
//            newseg->maxInc = min(newseg->maxInc,
//                    sq->feed * sq->cycleTime / absHelix);
               newseg->gmodetype = 1;
    }
    newseg->maxInc = min(newseg->maxInc, G_mode.maxV * sq->cycleTime);

    newseg->givemaxInc = newseg->maxInc;

    newseg->finalInc = 0;
    newseg->plInitInc = 0;
    newseg->plFinalInc = 0;
    newseg->cruiseInc = 0;
    newseg->planningDone = 0;
    newseg->active = 0;
    newseg->numLinkedSegs = 0;
    newseg->linkedToPrevSeg = 0;
    newseg->totNumPoints = 0;
    newseg->nextSegment = 0;

    newseg->type_circle_seg=0;//ghf-2014/11/27
    int kk=0;
    //前10个是上层传下来的参数值，需要根据实际情况返回
    for(kk=0;kk<10;kk++) {
        newseg->dynamicGetpar[kk] = dynamicpar[kk];
    }

        newseg->lengthFlag = 0;

    /* calculate the maximum tangential acceleration for this circle */
    /*  rpow2 = circle.radius * circle.radius;
     topIncPow2 = newseg->maxInc * newseg->maxInc;
     A = max(sq->maxAcc * sq->maxAcc * sq->ctPow2 * sq->ctPow2 * rpow2
     - topIncPow2 * topIncPow2, 3.0 / 4.0 * topIncPow2 * topIncPow2);

     newseg->amaxTan = sqrt(A / (rpow2 * (rpow2  + topIncPow2))) / sq->ctPow2;
     */
    //newseg->amaxTan = sq->maxAcc;
//***************************************************************************
// Changed by heyuebang in 1/23/2014;
// A conservative value is used to satisfy the acceleration and jerk limit.
//根据模式不同，给newseg赋值不同的线加速度amaxTan,法向加速度amaxNorm，加加速度jmaxTan；
    newseg->amaxTan = G_mode.maxAcc;

    /**************************************************************************/
    //add by heyuebang in 2015-12-25
    if (sq->omega_limit_flag){
        temp = max(G_mode.maxJerk,sqrt(newseg->amaxTan*0.2)*newseg->amaxTan);
        newseg->maxInc =min(newseg->maxInc,pow(temp* circle.radius* circle.radius,0.333333)* sq->cycleTime);
    }
    /**************************************************************************/
    newseg->amaxNorm = G_mode.maxAcc_nCircle;
    newseg->jmaxTan =  G_mode.maxJerk;

//    if (absHelix != 0) {
//        newseg->amaxTan = min(newseg->amaxTan,
//                G_mode.maxAcc * newseg->helixRadius / absHelix);
//    }

    //ghf-2014/11/6
    if((sq->segpara_i[1] == 1)&&(G_number == 0)){//ghf-2014/12/11
        newseg->maxInc = 500  * sq->segpara_d[0] * sq->cycleTime;
        newseg->amaxTan = 1200 ;
        newseg->amaxNorm = 1200 ;
        newseg->givemaxInc = newseg->maxInc;
    }

//***************************************************************************

    if (-1 == sqPreprocessSegment_five(sq, newseg, &G_mode)) {
        diagnostics("Error 3 in sqAddCircle_five()\n");
        return -1;
    }

    /*if (newseg->ID == 277){
     diagnostics("Error in AddCircle: newseg->ID =%d,newseg->angle =%d, turn =%d,newseg->length=%d\n",newseg->ID,(int)(circle.angle*1000000),turn,(int)(newseg->length*1000000));

     }*/

    /* set last Appended Motion ID */
    sq->lastAppMotionID = ID;

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
    if (sq->numSegments >= sq->size - SQ_SAFETY_MARGIN) {
        sq->full = 1;
    }
//  diagnostics("add circle!\n");
    return 0;
}

int sqGetPosition_five(SEGMENTQUEUE * sq, EmcPose * p) {
    //int ret;
    if ((sq == 0) || (p == 0)) {
        diagnostics("Error in sqGetPosition_five()\n");
        return -1;
    }

    sq->lastPoint.a = 0; //ghf-2014/9/26，屏蔽这里走全圆的会报di3轴跟随错误，
    /**********************************************************************/
    //add by heyuebang in 2015-12-15
    *p = sq->lastPoint;
     //如果使用规划速度滤波，则调整输出为滤波后输出
    if (sq->is_or_not_filterV == 1)
       {
        sq->lastPoint_aft_filter.a =0.0;
        *p = sq->lastPoint_aft_filter;
    }
    //如果使用单轴滤波，则调整输出为滤波后输出
   if( sq->point_filtered_flag == 1)
   {
       sq->Point_Filtered_LastPoint.a = 0.0;
       *p = sq->Point_Filtered_LastPoint;

   }

  // rtapi_print("X%f Y%f Z%f\n", p->tran.x,p->tran.y,p->tran.z);
   /**********************************************************************/
    //rtapi_print("sq->lastPoint.a = %d\n",(int)(sq->lastPoint.a)); //ghf-2014/9/16,这里应该一直为0才对阿？？
    return 0;
}
/*
static int sqGetGoalPosition_five(EmcPose * p) {
    if (p == 0) {
        diagnostics("Error in sqGetGoalPosition_five()\n");
        return -1;
    }

    *p = segEndPos;
    return 0;
}*/

/**********************************************************************/
//修改：heyuebang 2015-12-15
//增加均值滤波
int sqRunCycle_five(SEGMENTQUEUE * sq)
{
   // double deta_s;
     SEGMENT *as = sq->queue;

    PmPose last_point_pose;
    static int number=0;

    if (sq == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,"Error 1 in sqRunCycle_five(): Segmentqueue doesn't exist!\n");
        return -1;
    }

//    if(sq->done==1 && sq->paused==1)
//        diagnostics("Error 2 in sqrun, pausefinish==%d,sq->done_filter=%d\n",emcmotStatus->pausefinish,sq->done_filter);

    if(sq->done_filter == 1 &&sq->done==1){
       // rtapi_print_msg(RTAPI_MSG_ERR,"in sqRunCycle_five(): pausefinish=%d,done_filter=%d,sq->done=%d,pause=%d\n",emcmotStatus->pausefinish,sq->done_filter,sq->done,sq->paused);

        return 0;
    }

    //运行sqGeneratePosition_five函数（原sqRunCycle_five函数，获得滤波前的位置增量sq->deta_s）
    if (-1 == sqGeneratePosition_five(sq))
    {
        rtapi_print_msg(RTAPI_MSG_ERR,"Error 1 in sqRunCycle_five(): sqGeneratePosition_five(sq)!\n");
        return -1;
    }


    if (sq->done == 1){

        if(number < sq->filter_const + sq->point_filtered_const){
           // diagnostics("sq->done=%d,sq->done_filter=%d",sq->done,sq->done_filter);
          //  diagnostics(" run: sq->lastx=%f,sq->lastfilterx=%f,number=%d\n",sq->lastPoint.tran.x,sq->Point_Filtered_LastPoint.tran.x,number);
             number ++;
             sq->done_filter = 0;
        }else{
              sq->done_filter = 1;
        }
    }else{
        number = 0;
        sq->done_filter = 0;

    }


    if (sq->done_filter == 1 && sq->aborting == 1)
    {
         //emcmotStatus->pausefinish = 0;
        if (-1 == sqClearQueue(sq)) {
            diagnostics("Error 2 in sqAbort_five, sqrun\n");
            return -1;
        }
    }



    if (sq->done_filter == 1 && sq->paused != 0){
            sq->pausefinish = 1;
            diagnostics("Error 3 in sqrun, pausefinish==1\n");
    }


   // deta_s = sqMeanFilterV_five(sq);
    //获取当前段地址，注意，第一次运行该函数，as需要指向sq->queque最后一段，因为sq->dist_to_go（剩下要走的路径初值为0）表明该段走完。
    //所以sq->cursor_aft_filter初值指向最后一段的下标；
     if (sq->is_or_not_filterV == 1)
     {
    as = sq->queue + sq->cursor_aft_filter;  //第一次指向最后一段，可能没有初始化，但这句话没有意义，增加只是让程序不报警告
    //计算滤波后要走的增量，并计算该段剩下要走的长度
    sq->dist_to_go -= sqMeanFilterV(sq); //规划速度滤波
//    if(fabs(sq->deta_s - sq->currentVel )>1e-9){
//        diagnostics("error in sqRunCycle_five");
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
        rtapi_print_msg(RTAPI_MSG_ERR,"Error 2 Panic: as = NULL in sqRunCycle_five\n");
        return -1;
    }
    //计算新的位置点
    if (as->type == SQ_LINEAR) {
        pmLinePoint(&as->line, as->givenLength - sq->dist_to_go, &last_point_pose);
    } else if (as->type == SQ_CIRCULAR){
        pmCirclePoint(&as->circle, (as->givenLength - sq->dist_to_go) / as->helixRadius,
                &last_point_pose);
    }else{
        //第一次运行，而while没运行，则as指向最后一段，此时由于没有初始化，是错的，这里认为没有初始化时type=0;
        //需要确认？？？？？
         return 0;

    }

    pmpose_to_EmcPose(last_point_pose, &sq->lastPoint_aft_filter);//得到规划速度滤波后的结果
     }
    //需要考虑暂停，调整倍率时，运行剩下小线段运行问题，即滤波处理前将as段改变了。加入滤波器是不容许修改as段
  //如果使用了规划速度滤波，则单轴滤波使用规划速度滤波后结果，否则使用规划速度结果
   if (sq->is_or_not_filterV == 0)
    {
       sq->Point_Filtered_LastPoint = sqPointFilter(sq,sq->lastPoint);

     }else{
       sq->Point_Filtered_LastPoint = sqPointFilter(sq,sq->lastPoint_aft_filter);

     }


 /*   if (sq->Point_Filtered_LastPoint.tran.x != sq->lastPoint.tran.x)
    {
        diagnostics("sqGetPosition: x_error=%d,y_error=%d,z_error=%d\n",(int)((sq->lastPoint.tran.x - sq->Point_Filtered_LastPoint.tran.x)*1000000),(int)((sq->lastPoint.tran.y- sq->Point_Filtered_LastPoint.tran.y)*1000000),(int)((sq->lastPoint.tran.z-sq->Point_Filtered_LastPoint.tran.z)*1000000));
    }*/
   //printf("lastPoint.x=%f,lastpointfilter=%f,flagfiltetr=%d\n", sq->lastPoint.tran.x, sq->Point_Filtered_LastPoint.tran.x,sq->point_filtered_flag);

  // printf("X%f Y%f Z%f\n", sq->Point_Filtered_LastPoint.tran.x,sq->Point_Filtered_LastPoint.tran.y,sq->Point_Filtered_LastPoint.tran.z);

    return 0;

}

/**********************************************************************/

static int sqGeneratePosition_five(SEGMENTQUEUE *sq)
{
    SEGMENT *as; /* to use instead of sq->queue[sq->start],
     stands for Active Segment */
    int i;
    SEGMENT *cursor;
    double finalInc;
    //int done;
    int minNumSteps;
    //double angleToGo;
    double amaxTan;
    //int turn;
    //PmCartesian normal, center;
    PmPose last_point_pose;
  //  PmPose start_pose, end_pose;

    //int npow1, npow2, npow3;
    double npow1, npow2, npow3; //hebo-2014/7/8

    //my change
    double startInc;
    //int linkcrit,p;//hyb-2014/11/27

    static int mynumlink=0;

    //return -1;

    //if((sq->numSegments <= 100)&&(sq->numSegments > 0))
        //rtapi_print_msg(RTAPI_MSG_ERR,"in sqGeneratePosition_five,sq->numSegments=%d,sq->done=%d\n",sq->numSegments,sq->done);
    /****************************************/
    sq->deta_s = 0.0;  //初始化位置增量
    /****************************************/
    if (sq == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,"Error 1 in sqGeneratePosition_five(): Segmentqueue doesn't exist!\n");
        return -1;
    }

   // sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31

    if (sq->full == 1 && sq->numSegments < sq->size - SQ_SAFETY_MARGIN) {
        rtapi_print_msg(RTAPI_MSG_ERR,"sq->full == 1 although queue's not full!!!!\n");
    }


    //hong fix bug on 20160106, before that,sqAbort_five() cannot clear the queue,and retrun in here
    if (sq->done == 1) {
        /* do nothing */
        sq->delayTime = 0;
        return 0;
    }


    if (sq->done == 0 && sq->delayTime <10 ){
        sq->delayTime ++;
        return 0;
    }else{
        sq->delayTime = 11;
    }
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
    as = sq->queue + sq->start; /* much shorter ..... */

    if (as == 0) {
        rtapi_print_msg(RTAPI_MSG_ERR,"Error 2 Panic: as = NULL in sqRunCycle_five\n");
        return -1;
    }


    /****************************************/
  //  sq->deta_s += sq->deta_kesi; //将上次忽略的小线段加上，（可能出现的圆弧（角度<1e-6），剩下的小线段<1e-6）;
  //  sq->deta_kesi = 0.0; //清零，防止下次再累加
    /****************************************/

    /* if setFeedOverride in acceleration phase,and now  we accelerate to cruise phase . We need to adjust feed  12.17/2007*/
  //  if (sq->feedAdjusting == 1 && as->active == 1 && sq->n == as->m + 3) {
      if (sq->feedAdjusting == 1 && as->active == 1 && sq->n == as->m + 1) {
        sq->feedAdjusting = 0;


        startInc = as->cruiseInc;

        //add by hebo-2014/6/16

        amaxTan = sq->cursor->amaxTan;
        cursor = sq->cursor;
        while (cursor->nextSegment != 0 && cursor->linkedToPrevSeg == 1) {
            cursor = cursor->nextSegment;
            amaxTan = min(amaxTan, cursor->amaxTan);
        }

        if (cursor->nextSegment != 0
                && cursor->nextSegment->maxInc < cursor->maxInc)
            finalInc = min(cursor->finalInc,
                    cursor->nextSegment->maxInc * sq->feedOverrideFactor);
        else
            finalInc = min(cursor->finalInc,
                    cursor->maxInc * sq->feedOverrideFactor);

//        minNumSteps = ceil(
//                3 * fabs(finalInc - startInc) / (2 * amaxTan * sq->ctPow2));
//        minNumSteps = max(3, minNumSteps);
//        if ((minNumSteps + 1) * (startInc + finalInc) / 2
//                - startInc<
//                as->totLength-sq->dist-2*startInc-finalInc - (p_h+2) * max(startInc, finalInc)) {

            minNumSteps = ( 3 * fabs(finalInc - startInc) / (2 * amaxTan * sq->ctPow2));

            if ((minNumSteps + 1) * (startInc + finalInc) / 2.0  +(p_h+1+1)*max(startInc, finalInc) +(finalInc- startInc)/2.0<=
                  as->totLength-sq->dist){

            cursor = as;

        /**************************************************************/

            while (cursor != sq->cursor) {

                cursor = cursor->nextSegment;
                if (cursor == 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR,
                            "Error 3 Panic 1: cursor = NULL in sqSetFeedOverride_five\n");
                    return -1;
                }
                //sq->numSegments--;
            }



            cursor->prevSegment = 0;
            cursor->linkedToPrevSeg = 0;
            sq->start = sq->cursor - sq->queue;
            as = cursor;

            sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
            if (sq->numSegments < sq->size - SQ_SAFETY_MARGIN)
                sq->full = 0;
            if (sq->numSegments < 0) { //这里也会打印！ghf-2014/7/31
                rtapi_print_msg(RTAPI_MSG_ERR,
                        "Error 4 Panic: sq->numSegments <0  in sqSetFeedOverride_five\n");
                return -1;
            }

            as->planningDone = 0;
            as->plInitInc = startInc;
            as->initInc =  startInc;

// diagnostics("sqGet_pre: as->ID=%d,as->length=%d,sq->dist=%d,sq->offset=%d\n",as->ID,(int)(as->length*1000000),(int)(sq->dist*1000000),(int)(sq->offset*1000000));

            as->length = as->length - (sq->dist - sq->offset);
            as->totLength = as->length;

         //    diagnostics("sqGet_aft: as->ID=%d,as->length=%d,sq->dist=%d,sq->offset=%d\n",as->ID,(int)(as->length*1000000),(int)(sq->dist*1000000),(int)(sq->offset*1000000));

            as->numLinkedSegs = 0;

            as->active = 0;
            while (cursor->nextSegment != 0
                    && cursor->nextSegment->linkedToPrevSeg == 1) {
                cursor = cursor->nextSegment;

                cursor->active = 0; //ghf-2014/6/11

                as->numLinkedSegs++;
                as->totLength += cursor->length;
            }
        }
    }


    if (as->active == 0)
    /* we're about to start with a new segment */
    {
        if (as->linkedToPrevSeg)
            rtapi_print_msg(RTAPI_MSG_ERR,"in sqGeneratePosition_five,as->linkedToPrevSeg=%d\n",
                    as->linkedToPrevSeg);


        if (-1  == sqForceForwardLinkSegment_five(sq, as, as,sq->feedOverrideFactor)) { //hyb-2014/7/2
            rtapi_print_msg(RTAPI_MSG_ERR,"Error 5 in sqGeneratePosition_five add by hyb \n ");
            sq->done = 1; //ghf-2014/6/30
            return -1;
        }

       // diagnostics("sqGet_aft11: as->ID=%d,as->numLinkedSegs=%d,\n",as->ID,as->numLinkedSegs);


        if (-1 == sqPlanSegment_five(sq, as)) {
            rtapi_print_msg(RTAPI_MSG_ERR,"Error 6 in sqGeneratePosition_five");
            return -1;
        } else {
            /* mark the whole chain as active */
            cursor = as;
            for (i = 0; i <= as->numLinkedSegs; i++) {
                if (cursor == 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR,"Error 7 Panic: cursor 1 = NULL in sqGeneratePosition_five\n");
                    return -1;
                }
                cursor->active = 1;
                cursor = cursor->nextSegment;
            }

            sq->oldDist = 0;
            sq->dist = 0;

            /* reset base, cursor, offset and cumlength and n */
            sq->base = as->start;
            sq->cursor = as;
            sq->offset = 0;
            sq->cumLength = as->length;
            sq->n = 1;
            sq->currentID = as->ID;
            mynumlink=as->numLinkedSegs;
        }

    }
    /* else: planning has been done before and the parameters are correctly
     initialized, unless someone deliberately changed planningDone to 1.
     Let's trust our user and not check for that here. */

    /* depending of in what phase of the motion we are, determine the new
     distance */
//    if (sq->n == 1 || sq->n == 2) {
//        sq->dist += as->plInitInc;
//    } else if (sq->n <= as->m + 2)
    /* acceleration phase */
     if (sq->n <= as->m)
    {
      //  npow1 = sq->n - 2;
         npow1 = sq->n;
        npow2 = npow1 * npow1;
        npow3 = npow2 * npow1;
        sq->dist += as->a1 * npow3 * sq->ctPow3 + as->b1 * npow2 * sq->ctPow2
                + as->c1 * npow1 * sq->cycleTime + as->d1;
   // } else if (sq->n <= as->m + as->p + 2)
         } else if (sq->n <= as->m + as->p)
    /* cruising phase */
    {
        sq->dist += as->cruiseInc;
  //  } else if (sq->n <= as->m + as->p + as->q + 2)
    } else if (sq->n <= as->m + as->p + as->q)
    /* deceleration phase */
    {
//        npow1 = sq->n - as->m - as->p - 2;
          npow1 = sq->n - as->m - as->p;
        npow2 = npow1 * npow1;
        npow3 = npow2 * npow1;
        sq->dist += as->a3 * npow3 * sq->ctPow3 + as->b3 * npow2 * sq->ctPow2
                + as->c3 * npow1 * sq->cycleTime + as->d3;

//    } else if (sq->n == as->m + as->p + as->q + 3)
//    /* last step */
//    {
//        sq->dist += as->plFinalInc;
    } else
    /* we have a problem, because this should never happen */
    {
        rtapi_print("sq->n=%d,m=%d,p=%d,q=%d,Error 8 in sqGeneratePosition_five\n", sq->n,
                as->m, as->p, as->q);
        sq->done = 1; //ghf-2014/6/18
        return -1;
    }
    /*rtapi_print_msg(RTAPI_MSG_ERR,"sq->cursor->id=%d\n",sq->cursor->ID);
    rtapi_print_msg(RTAPI_MSG_ERR,"sq->dist=%d\n",(int)(sq->dist*1000000));
    rtapi_print_msg(RTAPI_MSG_ERR,"sq->cumLength=%d\n",(int)(sq->cumLength*1000000));
    if(fabs(sq->dist - sq->cumLength) < 0.000001)
        rtapi_print_msg(RTAPI_MSG_ERR,"1234\n");*/

    /* transform the dist scalar into a XYZ triplet */
    if (as->nextSegment != 0
            && (as->nextSegment->linkedToPrevSeg == 1 || sq->paused == 1))
            /* the active segment is the first segment of a chain */
            /* the sq->paused == 1 test is added to make sure that if a pause
             command is given just before the end of the segment, that the
             following segment is used to finish decelerating to zero */
            {
        while ((sq->dist > sq->cumLength) && (sq->cursor->nextSegment != 0)
                && ((sq->cursor->nextSegment->linkedToPrevSeg == 1)
                        || sq->paused == 1)) {
            sq->offset = sq->cumLength;
            sq->base = sq->cursor->end;
            sq->cursor = sq->cursor->nextSegment;
            sq->cumLength += sq->cursor->length;

        }
        /* set currentID */
        sq->currentID = sq->cursor->ID;           //next segment

        //rtapi_print_msg(RTAPI_MSG_ERR,"111sq->dist - sq->offset=%d,sq->cursor->type=%d\n",(int)((sq->dist - sq->offset)*1000),sq->cursor->type);

        if (sq->cursor->type == SQ_LINEAR) {
            pmLinePoint(&sq->cursor->line, sq->dist - sq->offset + sq->cursor->givenLength - sq->cursor->length,
                    &last_point_pose);
        } else {
            pmCirclePoint(&sq->cursor->circle,
                    (sq->dist - sq->offset + sq->cursor->givenLength - sq->cursor->length) / sq->cursor->helixRadius,
                    &last_point_pose);
        }

//        if (sq->cursor->givenLength != sq->cursor->length)
//           diagnostics("sqrun_aft0: sq->cursor->ID=%d,sq->cursor->givenL=%d,sq->cursor->length=%d,sq->cursor->dist=%d,sq->offset=%d,sq->cumLength=%d\n",as->ID,(int)(as->givenLength*1000000),(int)(as->length*1000000),(int)(sq->dist*1000000),(int)(sq->offset*1000000),(int)(sq->cumLength*1000000));

        pmpose_to_EmcPose(last_point_pose, &sq->lastPoint);
    }

    else
    /* the active segment has no other segments linked to it, which makes
     things much easier... */
    {
        if (sq->cursor->type == SQ_LINEAR) {
            pmLinePoint(&as->line, sq->dist+ as->givenLength - as->length, &last_point_pose);
        } else {
            pmCirclePoint(&as->circle, (sq->dist+ as->givenLength - as->length) / as->helixRadius,
                    &last_point_pose);
        }
        pmpose_to_EmcPose(last_point_pose, &sq->lastPoint);

//        if (as->givenLength != as->length)
//           diagnostics("sqrun_aft1: as->ID=%d,as->givenL=%d,as->length=%d,sq->dist=%d,sq->offset=%d,sq->cuml=%d\n",as->ID,(int)(as->givenLength*1000000),(int)(as->length*1000000),(int)(sq->dist*1000000),(int)(sq->offset*1000000),(int)(sq->cumLength*1000000));
    }

    segEndPos = sq->cursor->end;/*add on 06.10.19. to remember the end position*/
//diagnostics("s2->Id=%d,s2->angle=%d,s2->type_circle_seg=%d,s2->length=%d, s2->initInc=%d\n",sq->cursor->ID,(int)(sq->cursor->angle*180/PM_PI*1000),sq->cursor->type_circle_seg,(int)(sq->cursor->length*1000),(int)(sq->cursor->initInc*1000000));
    sq->n++;

    sq->currentVel = sq->dist - sq->oldDist;
    /****************************************/
    sq->deta_s += sq->currentVel ; //将规划的增量附加上
    /****************************************/
    sq->oldDist = sq->dist;

    if (sq->n > as->totNumPoints) {
        if (sq->aborting == 1) {
           sq->done = 1;
        }

        else if (sq->paused == 1) {

            sq->done = 1;
            finalInc = 0;

            /* remove all segments preceding the current segment */
            cursor = as;
            //rtapi_print("as->id=%d,sq->cursor=%d\n",as->ID,sq->cursor->ID);
            while (cursor != sq->cursor) { //segments before sq->cursor is done,and remove them.making cursor point to current segment

                cursor = cursor->nextSegment;
                if (cursor == 0) {
                    rtapi_print_msg(RTAPI_MSG_ERR,"Error 10 Panic: cursor 2 = NULL in sqRunCycle_five\n");
                    return -1;
                }
                //sq->numSegments--;
            }

            cursor->prevSegment = 0;
            cursor->linkedToPrevSeg = 0;
            sq->start = sq->cursor - sq->queue;
            as = cursor;

            sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
            if (sq->numSegments < sq->size - SQ_SAFETY_MARGIN)
                sq->full = 0;
            if (sq->numSegments < 0) { //这里也会打印！ghf-2014/7/31
                rtapi_print_msg(RTAPI_MSG_ERR,
                        "Error 11 Panic: sq->numSegments <0  in sqRunCycle_five\n");
                return -1;
            }

            as->planningDone = 0;
            as->plInitInc = finalInc;

            as->initInc = finalInc; //ghf-2014/6/21

            as->length = as->length -  (sq->dist - sq->offset);
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
            //rtapi_print_msg(RTAPI_MSG_ERR,"111as->ID=%d\n",as->ID);

       //     emcmotStatus->pausefinish = 1;

        } else if (sq->stepping == 1)
        /* we are at the end of the segment, but shouldn't go on
         with the next one */
        {
            //sq->numSegments--;
            sq->start = (sq->start + 1) % sq->size;
            sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;
            if (sq->numSegments < sq->size - SQ_SAFETY_MARGIN)
                sq->full = 0;
            if (sq->numSegments < 0) {
                rtapi_print_msg(RTAPI_MSG_ERR,"Error 13 Panic: sq->numSegments < 0  in sqRunCycle_five\n");
                return -1;
            }

            as = sq->queue + sq->start;
            as->plInitInc = 0;
            as->active = 0;
            //rtapi_print_msg(RTAPI_MSG_ERR,"333sq->stepping == 1\n");
            sq->done = 1;
        }

        else {
            /* end of segment reached */

            //ghf-204/8/4
            if(sq->paused == 2){
               // emcmotStatus->pausefinish = 1;
                sq->done = 1;//ghf-2014/8/5
            }


            //sq->numSegments -= 1 + as->numLinkedSegs;
                /****************************************************************/
            if(mynumlink!=as->numLinkedSegs){
               as->numLinkedSegs = mynumlink;
            }

            /****************************************************************/

            sq->start = (sq->start + as->numLinkedSegs + 1) % sq->size;
            sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
            if (sq->numSegments < sq->size - SQ_SAFETY_MARGIN)
                sq->full = 0; // we just removed some segments
            if (sq->numSegments < 0) {
                rtapi_print_msg(RTAPI_MSG_ERR,"Error 16 Panic: sq->numSegments <0  in sqRunCycle_five\n");
                return -1;
            }
       // }


            //rtapi_print_msg(RTAPI_MSG_ERR,"111as run end in sqRunCycle_five\n");
            //iii = 0;
        }
    }

    /* for debugging */
/*
     oldVel = newVel;
    newVel.tran.x = sq->lastPoint.tran.x - oldPos.tran.x;
    newVel.tran.y = sq->lastPoint.tran.y - oldPos.tran.y;
    newVel.tran.z = sq->lastPoint.tran.z - oldPos.tran.z;

    newAcc.tran.x = newVel.tran.x - oldVel.tran.x;
    newAcc.tran.y = newVel.tran.y - oldVel.tran.y;
    newAcc.tran.z = newVel.tran.z - oldVel.tran.z;
*/

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
        sq->dynamicparameter[18] = sq->cursor->givemaxInc / sq->cycleTime;  //ghf-2014/10/10
    }
    sq->dynamicparameter[19] = sq->currentVel / sq->cycleTime; //这样手动自动就统一到这个变量了

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

/*
    oldPos = sq->lastPoint;

*/

    return 0;
}


/* function to change the feed overide */
int sqSetFeedOverride_five(SEGMENTQUEUE * sq, double fo) {

    SEGMENT *as, *cursor; /* as = Active Segment */
    double startInc;  // finalInc, startAcc = 0;
    //double prevInc;
//  int npow1, npow2, npow3;
  //  double angleToGo;
   // int turn;
   // PmCartesian normal, center;
    //PmPose start_pose, end_pose;

    //*********************************************************
    //add by heyuebang in 06/11/2014

    double finalInc;

    int minNumSteps;

    double amaxTan;
    //*********************************************************

    if (sq == 0 || sq->queue == 0) {
        diagnostics("Error 1 in sqSetFeedOverride_five");
        return -1;
    }

    /* if fo is out of the valid range: clamp it */
    if (fo < 0)
        fo = 0;
    else if (fo > sq->maxFeedOverrideFactor)
        fo = sq->maxFeedOverrideFactor;

    if (fabs(sq->feedOverrideFactor - fo) < 0.0001)
        /* don't do anything, just return */
        return 0;


    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31

    if (sq->numSegments == 0) {    //队列为空，直接返回
        if ((sq->paused != 0) && (zerofeedoverflag == 1)) {    //当前是由运行态调整倍率导致的暂停ghf-2014/8/25
            if (sq->feedOverrideFactor == 0) {    //是由于倍率拖到0引起的暂停，而当前倍率不为0，执行resume
                /* If the previous feed override factor equals zero, then the
                 pause action was actually done by setting the feed
                 override to zero */
                if(sqIsDone_five(sq) != 1) return 0;
                sq->feedOverrideFactor = fo;
                zerofeedoverflag = 0;
                printf("Now in resume 1\n");
                if (-1 == sqResume_five(sq)) {
                    sq->feedOverrideFactor = 0;
                    zerofeedoverflag = 1;
                    diagnostics("Error 2 in sqSetFeedOverride_five\n");
                    return 0;
                }
            }
        }
        /* the queue is empty */
        sq->feedOverrideFactor = fo;
        return 0;
    }


    if (fo == 0) {    //倍率为0，设为暂停ghf-2014/6/6
        //ghf-2014/8/25
        if(sq->paused == 0) {
            zerofeedoverflag = 1;
        }
        else {
            zerofeedoverflag = 0;
            sq->feedOverrideFactor = fo;
            //rtapi_print_msg(RTAPI_MSG_ERR,"222\n");
            return 0;
        }

        /* then this is actually a pause action */
        if (-1 == sqPause_five(sq)) {
            diagnostics("Error in sqSetFeedOverride_five\n");
            return -1;
        } else {
            sq->feedOverrideFactor = 0;
            return 0;
        }
    }


    as = sq->queue + sq->start;

    //暂停有1和2有种状态，所以此处改为!=0 ghf-2014/8/25
    if ((sq->paused != 0) && (zerofeedoverflag == 1)) {    //当前是由运行态调整倍率导致的暂停ghf-2014/8/25
        if (sq->feedOverrideFactor == 0) {    //是由于倍率拖到0引起的暂停，而当前倍率不为0，执行resume
            /* If the previous feed override factor equals zero, then the
             pause action was actually done by setting the feed
             override to zero */
            if(sqIsDone_five(sq) != 1) return 0;
            sq->feedOverrideFactor = fo;
            zerofeedoverflag = 0;
            printf("Now in resume 2\n");
            if (-1 == sqResume_five(sq)) {
                sq->feedOverrideFactor = 0;
                zerofeedoverflag = 1;
                diagnostics("Error 2 in sqSetFeedOverride_five\n");
                return -1;
            } else
                return 0;
        } else {    //当前时暂停，倍率不为0，则什么都不做
            /* else: don't do anything else */
            sq->feedOverrideFactor = fo;
            return 0;
        }
    }
    sq->feedOverrideFactor = fo;
    if (as->active == 0) {    //当前队列头开始的逻辑段都还没有激活，也是直接返回，因为sqplan会考虑倍率因子
        /* the previous segment has just been finished. as still needs to
         be planned. So: don't do anything */
 //   } else if (sq->n < as->m + 2 || sq->feedAdjusting == 1) { //如果当前是激活状态，发生在加速阶段
   } else if (sq->n < as->m+1 || sq->feedAdjusting == 1) { //如果当前是激活状态，发生在加速阶段
    /* we are accelerating to (macInc * 'previous fo'). We need to adjust
     this phase in order get to the desired cruising feed */

        if (sq->feedAdjusting != 1 && as->p == 0
                && as->maxInc * fo > as->cruiseInc) //ghf-2014/6/2
        /* the the segment is too short to reach the current maxInc and it
         will be too short too to reach the new feed */
            return 0;

        sq->feedAdjusting = 1;

    } else if (sq->n < as->m + as->p+1) { //倍率调整发生在匀速阶段
    /* change the active segment such that it looks like we are starting
     with a new segment */

        //iii = 1; //add by ghf for debug

        startInc = as->cruiseInc;

        //sq->lastPoint = oldPos;//防止control中改变此值，由omega于精插补的原因导致的。ghf-2014/6/23

        //add by hebo-2014/6/16

        amaxTan = sq->cursor->amaxTan;
        cursor = sq->cursor;
        while (cursor->nextSegment != 0 && cursor->linkedToPrevSeg == 1) {
            cursor = cursor->nextSegment;
            amaxTan = min(amaxTan, cursor->amaxTan);
        }


        if(cursor->nextSegment != 0 && cursor->nextSegment->maxInc < cursor->maxInc )
        {
            finalInc = min(cursor->finalInc,cursor->nextSegment->maxInc * sq->feedOverrideFactor);
        }else{
            finalInc = min(cursor->finalInc,cursor->maxInc * sq->feedOverrideFactor);
        }

//        minNumSteps = ceil(
//                3 * fabs(finalInc - startInc) / (2 * amaxTan * sq->ctPow2));
//        minNumSteps = max(3, minNumSteps);
//        if ((minNumSteps + 1) * (startInc + finalInc) / 2
//                - startInc>
//                as->totLength-sq->dist-2*startInc-finalInc - (p_h+2) * max(startInc, finalInc)) {

            minNumSteps = ( 3 * fabs(finalInc - startInc) / (2 * amaxTan * sq->ctPow2));

            //
            if ((minNumSteps + 1) * (startInc + finalInc) / 2.0  +(p_h+1+1)*max(startInc, finalInc) +(finalInc- startInc)/2.0>
                  as->totLength-sq->dist){
            //diagnostics("yunsu,the current as sets is not long enough in sqSetFeedOverride_five\n");
            return 0;
        }

        /* delete all previous segments */
        cursor = as;
        while (cursor != sq->cursor) {
            cursor = cursor->nextSegment;
            if (cursor == 0) {
                diagnostics("Error 3 Panic 1: cursor = NULL in sqSetFeedOverride_five\n");
                return -1;
            }
            //sq->numSegments--;
        }



        cursor->prevSegment = 0;
        cursor->linkedToPrevSeg = 0;
        sq->start = sq->cursor - sq->queue;
        as = cursor;

        sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
        if (sq->numSegments < sq->size - SQ_SAFETY_MARGIN)
            sq->full = 0;
        if (sq->numSegments < 0) {
            diagnostics("Error 4 Panic: sq->numSegments <0  in sqSetFeedOverride_five\n");
            return -1;
        }

        as->planningDone = 0;
        as->plInitInc = startInc;
       /* as->start = sq->lastPoint;
        if (as->type == SQ_LINEAR) {
            as->length = sqGiveLength(as->start, as->end);
            as->totLength = as->length;
            EmcPose_to_pmpose(as->start, &start_pose);
            EmcPose_to_pmpose(as->end, &end_pose);
            pmLineInit(&as->line, start_pose, end_pose);
        } else {
            angleToGo = as->circle.angle
                    - (sq->dist - sq->offset) / as->helixRadius;

            if(angleToGo<1e-6){
                angleToGo = 0;
                as->start = as->end;
            }

            turn = floor(angleToGo / (2 * PM_PI));
            normal = as->circle.normal;
            center = as->circle.center;
            EmcPose_to_pmpose(as->start, &start_pose);
            EmcPose_to_pmpose(as->end, &end_pose);
            pmCircleInit(&as->circle, start_pose, end_pose, center, normal,
                    turn);
            as->length = as->circle.angle * as->helixRadius;
            as->totLength = as->length;
        }*/
        /**********************************************************/
        //add by heyuebang in 2015-12-25
        as->length = as->length - (sq->dist - sq->offset);
        as->totLength = as->length;
              /**********************************************************/
        as->active = 0; /* mark the first segment of the chain
         as not active */

        /* determine how many segments are linked to as */
        as->numLinkedSegs = 0;
        while (cursor->nextSegment != 0
                && cursor->nextSegment->linkedToPrevSeg == 1) {
            cursor = cursor->nextSegment;

            cursor->active = 0; //ghf-2014/6/11

            as->numLinkedSegs++;
            as->totLength += cursor->length;
        }

    } else { //减速阶段不处理倍率，等到下一段自行处理
        /* we are already decelerating, which is in most cases
         necessary.  Therefore, let's not interrupt this and finish
         the current segment first. The new segment will be planned
         with the new feedOverrideFactor. */
    }
    return 0;
}

/* function to pause the system (decelerate to zero velocity) */
int sqPause_five(SEGMENTQUEUE * sq) {
    SEGMENT *as,*cursor; /* as = Active Segment */
    double startInc; /* the incrediment at the time the pause command
     was given */
    double startAcc; /* the derivate of the inc at that time */
    //int npow1, npow2, npow3;
    double npow1, npow2, npow3; //hebo-2014/7/8
    double amaxTan;
    int i; //heyuebang 2014-07-11

    if (sq == 0 || sq->queue == 0) {
        diagnostics("Error 1 in sqPause_five\n");
        return -1;
    }

    if (sq->paused == 1 || sq->stepping == 1) {
        /* don't do anything, system is already paused or stepping will stop soon */
        diagnostics("--don't do anything, system is already paused,sq->done=%d\n",sq->done);
        return 0;
    }

    /* set paused flag */
    sq->paused = 1;
    //diagnostics("\nsq->paused=%d\n", (sq->paused));
    //rtapi_print("in sqPause_five,sq->numSegments=%d\n", sq->numSegments);

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31

    if (sq->numSegments == 0) {
        /* the queue is empty */
        sq->done = 1; /* propably redundant */
        sq->pausefinish = 1; //start the run, if 0 will run error;
        diagnostics("1sqPause_five12334\n");
        return 0;
    }

    as = sq->queue + sq->start; //as指向的是当前队列中的第一个逻辑段

    if (as->active == 0 && as->initInc == 0)
    /* then this is the very first segment and we have not started yet.
     So let's set the done flag and return */
    {
        sq->done = 1;
        //diagnostics("11111\n");
        sq->pausefinish = 1; //ghf-204/8/5
        return 0;
    } else if (as->active == 0 && sq->stepping == 1) {
        //diagnostics("22222\n");
        return 0;
        //no need to pause when stepping is done!
    }

    sq->feedAdjusting = 0; /* if we were adjusting the speed, ignore that too */

    amaxTan = sqGiveMinAmaxTan_five(as);

    /*rtapi_print(
            "in sqPause_five,before,as->id=%d,as->active=%d,sq->n=%d,as->m=%d,as->p=%d,as->q=%d\n",
            as->ID,as->active, sq->n, as->m, as->p, as->q);*/

    if (as->active == 0 || sq->n <= 1) { //mdi出错时跑进了这里ghf-2014/6/6，在初始阶段点击了暂停，那么要减速，并跳过初始阶段
        /****************************************************************/
        // add by heyuebang in 2014-07-11
        //用于初始化as逻辑段运行时需要的参数。
        if ( as->active == 0) {
            cursor = as;
            for (i = 0; i <= as->numLinkedSegs; i++) {
                if (cursor == 0) {
                    diagnostics("Panic: cursor  = NULL in sqPause_five\n");
                    return -1;
                }
                cursor->active = 1;
                cursor = cursor->nextSegment;
            }
            sq->oldDist = 0;
            sq->dist = 0;

            /* reset base, cursor, offset and cumlength and n */
            sq->base = as->start;
            sq->cursor = as;
            sq->offset = 0;
            sq->cumLength = as->length;
            sq->n = 1;
            sq->currentID = as->ID;
       }
     /****************************************************************/
        startInc = as->plInitInc;
        startAcc = 0;
        as->active = 1;
        as->planningDone = 1; /* the planning will be done in this function */

        /*rtapi_print("1startInc = %d,amaxTan = %d,sq->ctPow2=%d\n",
                (int) (startInc * 1000), (int) (amaxTan * 1000),
                (int) (sq->ctPow2 * 1000000));*/

        as->q = ceil(3 * startInc / (2 * amaxTan * sq->ctPow2));
        as->m = 0;
        as->p = 0;
        //as->totNumPoints = as->q + 3;
       // sq->n = 3; /* start allover, but skip the first initInc steps */
        as->totNumPoints = as->q;
        sq->n = 1; /* start allover, but skip the first initInc steps */
  //  } else if (sq->n <= as->m + 3) { //modify bu hebo 2014/8/6
          } else if (sq->n <= as->m + 1) { //modify bu hebo 2014/8/6
        /* recalculate the last two dist values */
    //    npow1 = (sq->n - 3)*sq->cycleTime; /* sq->n-2-1 */
            npow1 = (sq->n - 1)*sq->cycleTime; /* sq->n-1 */
        npow2 = npow1 * npow1;
        npow3 = npow2 * npow1;
        startInc = as->a1 * npow3  + as->b1 * npow2 + as->c1 * npow1 + as->d1;
        startAcc = (3.0*as->a1 * npow2  + 2.0*as->b1 * npow1 + as->c1)* sq->cycleTime;

        /*rtapi_print("2,startAcc=%d,startInc = %d,amaxTan = %d,sq->ctPow2=%d\n",
                (int) (startAcc * 1000),(int) (startInc * 1000), (int) (amaxTan * 1000),
                (int) (sq->ctPow2 * 1000000));*/

        npow1 = amaxTan *sq->ctPow2;
        as->q = ceil(3.0 * startInc / ((npow1-startAcc) + sqrt(npow1*npow1+npow1*startAcc )));

        as->m = 0;
        as->p = 0;
//        as->totNumPoints = as->q + 3;
//        sq->n = 3; /* start allover, but skip the first initInc steps */
        as->totNumPoints = as->q;
        sq->n = 1; /* start allover, but skip the first initInc steps */
  //  } else if (sq->n <= as->m + as->p + 3) {
          } else if (sq->n <= as->m + as->p + 1) {
        startInc = as->cruiseInc;
        startAcc = 0;
        /*rtapi_print("3startInc = %d,amaxTan = %d,sq->ctPow2=%d\n",
                (int) (startInc * 1000), (int) (amaxTan * 1000),
                (int) (sq->ctPow2 * 1000000));*/
        as->q = ceil(3 * startInc / (2 * amaxTan * sq->ctPow2));
        as->m = 0;
        as->p = 0;
//        as->totNumPoints = as->q + 3;
//        sq->n = 3; /* start allover, but skip the first initInc steps */
        as->totNumPoints = as->q;
        sq->n = 1; /* start allover, but skip the first initInc steps */
    } else {
        /* we are already decelerating, so it would be best to finish that
         first. After we're done with that, we can decelerate to zero */

        /* before messing the whole thing up, let's see if we are already
         decelerating to zero */
        if (as->plFinalInc == 0) {
            /* Then we'll reach the end of the segment with a zero velocity.
             This means that we don't have to do anything but waiting at the
             end of the segment. This is basically the same thing that
             happens at the end of a step-motion. This is exactly what
             we will tell the system: that we're stepping, so that all of
             the extra things needed for a pause within a segment are skipped.
             */
            /*rtapi_print("4,as->plFinalInc = %d\n",
                    (int) (as->plFinalInc * 1000000));*/
            //sq->paused = 0;
            //sq->stepping = 1; //ghf-2014/8/1
            sq->paused = 2; //ghf-2014/8/4
            return 0;
        } else {

            /* let's do a little trick: we copy the deceleration parameters
             of the current motion to the acceleration phase parameter
             fields in the current segment and calculate a new
             deceleration action from finalInc to 0. We'll decrease sq->n
             with a number such of steps, such that it will look like
             nothing has happened. sqRunCycle_five won't notice this, and will
             think it's (again) in phase 1. We know better.... */

            as->a1 = as->a3;
            as->b1 = as->b3;
            as->c1 = as->c3;
            as->d1 = as->d3;

            sq->n -= as->m + as->p;

            as->m = as->q;
            as->p = p_h; /* the cruising phase now becomes the
             usual final step of a segment */

            /* find the finalInc */
            startInc = as->plFinalInc;
            startAcc = 0;
            as->cruiseInc = startInc;

            as->q = ceil(3 * startInc / (2 * amaxTan * sq->ctPow2));
           // as->totNumPoints = as->m + as->q + as->p + 2;
            as->totNumPoints = as->m + as->q + as->p;
            /*rtapi_print("5,as->plFinalInc = %d\n",
                    (int) (as->plFinalInc * 1000000));*/

        }
    }
    as->b3 = -(3 * startInc + 2 * as->q * startAcc)
            / (as->q * as->q * sq->ctPow2);
    as->a3 = (2 * startInc + as->q * startAcc)
            / (as->q * as->q * as->q * sq->ctPow3);
    as->c3 = startAcc / sq->cycleTime;
    as->d3 = startInc;

    as->plFinalInc = 0;

    /*rtapi_print("in sqPause_five,after,sq->n=%d,as->m=%d,as->p=%d,as->q=%d\n", sq->n,
            as->m, as->p, as->q);*/

    return 0;
}

/* function to resume with a paused segmentqueue */
int sqResume_five(SEGMENTQUEUE * sq) {
    SEGMENT *as; /* as = Active Segment */
    SEGMENT *cursor;

    if (sq == 0 || sq->queue == 0) {
        diagnostics("Error 1 in sqResume_five\n");
        return -1;
    }

    diagnostics("get into resume,sq->done=%d,sq->numSegments=%d\n",sq->done,sq->numSegments);

    if (sq->done_filter != 1)

    /* we can't resume if the systems is not done yet with a step or pause
     action */
    {
        diagnostics("Can't resume if not done with pause or step action\n");
        /* this is not a critical error, so we'll just ignore the command */
        return -1;
    }

    if (sq->feedOverrideFactor == 0) {
        /* we can't resume if the feed override factor is zero. To resume
         the user should set a new non-zero value for this factor first.
         This will immediately result in a resume action. */
        diagnostics("Can't resume if feed override is zero\n");
        /* not a critical error, so ignore it */
        //ghf-2014/8/26
        zerofeedoverflag = 1;
        //rtapi_print_msg(RTAPI_MSG_ERR,"333\n");
        return -1;
    }

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
    if (sq->numSegments == 0) {
        sq->paused = 0;
        sq->done = 0; //add by ghf-2014/7/21,当前只有一行，且暂停发生在减速阶段，那么走完之后，sq->done=1,sq->numSegments=0,此时再addline后，num变1,进入runcycle后，出错
        sq->stepping = 0;//ghf-2014/8/1,不然进到runcycle后，又会把 sq->done 置为1.
        diagnostics("return,sq->done=%d\n",sq->done);
        sq->pausefinish = 0;
        sq->done_filter=0;
        return 0;
    }

    /* let's see if the length of the segment(chain) is large enough to
     accelerate from zero to finalInc */
    as = sq->queue + sq->start;
    //diagnostics("222as->ID=%d\n",as->ID);
    cursor = as;
    /* first find the last segment in the chain */ //这个while不是多次一举吗？ghf-2014/6/10
    while (cursor->nextSegment != 0 && cursor->nextSegment->linkedToPrevSeg == 1)
        cursor = cursor->nextSegment;

    /* keep linking successive segments to as until the chain is long
     enough */

    if (-1
            == sqForceForwardLinkSegment_five(sq, as, cursor,
                    sq->feedOverrideFactor)) { //hyb-2014/7/2
        diagnostics("Error 2 in sqResume_five \n ");
        return -1;
    }
    /*
     if (sq->paused == 1 && sq->stepping == 1)
     sq->stepping = 1;
     else
     sq->stepping = 0;
     */
    sq->stepping = 0; //resume no matter the status is stepping or pause
    sq->paused = 0;
    sq->done = 0;
    sq->done_filter = 0;
    sq->pausefinish = 0;

    diagnostics("get out resume,as->id=%d,as->linkedToPrevSeg=%d\n",as->ID,as->linkedToPrevSeg);

    return 0;
}

/* function to abort */
int sqAbort_five(SEGMENTQUEUE * sq) //ghf-2014/5/17,此函数出错时再跟5/28
{
    if (1 == sq->aborting)
        /* we are already aborting, so let's just ignore it */
        return 0;

    if (sq == 0) {
        diagnostics("Error 1 in sqAbort_five");
        return -1;
    }

    diagnostics("in sqAbort_five,sq->paused=%d,sq->done=%d\n",sq->paused,sq->done);
    //emcmotStatus->pausefinish = 0;//ghf-2014/8/5
    if (sq->done ==1 && sq->done_filter==1){
        //has done after filter
         sq->aborting = 1;
        if (-1 == sqClearQueue(sq)) {
            diagnostics("Error 2 in sqAbort_five");
            return -1;
        }
        return 0;
    }

    if (sq->paused == 1 || sq->done == 1) {
          sq->aborting = 1;

        rtapi_print("in abort,run sqClearQueue");
    } else {
       // emcmotStatus->pausefinish = 0;
        sq->aborting = 1; //程序还在运行中点击退出，进到这里，
        sqPause_five(sq);
        rtapi_print("in abort,run sqPause_five");
    }
    return 0;
}

/* function to do execute one motion from a stop and stop again */
int sqStep_five(SEGMENTQUEUE * sq) {
    SEGMENT *as; /* as = Active Segment */
    if (sq == 0) {
        diagnostics("Error in sqStep_five\n");
        return -1;
    }

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31

    if (sq->done != 1 || sq->numSegments == 0)
    /* step should only be used when system is paused and waiting... */
    {
        diagnosticsOff
        ("Stepping can only be done when system is paused and waiting\n");
        /* not a critical error, let's ignore it */
        return 0;;
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
    as->finalInc = 0;

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

int sqIsStepping_five(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqIsStepping_five\n");
        return -1;
    }
    return sq->stepping;
}

int sqIsDone_five(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqIsDone_five\n");
        return -1;
    }

    return sq->done_filter;
}

int sqIsPaused_five(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqIsPaused_five\n");
        return -1;
    }
    return sq->paused;
}
//ghf-2014/10/24,halmeter中要能监视done、paused、numSegments、aborting几个变量
int sqIsAborting_five(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqIsAborting_five\n");
        return -1;
    }
    return sq->aborting;
}



double sqGetMaxAcc_five(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqGetMaxAcc_five\n");
        return -1;
    }
    return sq->G_mode_1.maxAcc;
}

int sqGetDepth_five(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqGetDepth_five\n");
        return -1;
    }
    return sq->numSegments;
}

int sqIsQueueFull_five(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqIsQueueFull_five\n");
        return -1;
    }
    return sq->full;
}

int sqGetID_five(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqGetID_five\n");
        return -1;
    }
    return sq->currentID;
}

int sqSetFilterNumber_five(SEGMENTQUEUE *sq,int filter_number)
{
    if (sq == 0) {
        diagnostics("Error in sqSetFilterNumber_five\n");
        return -1;
    }
    if (filter_number < 1)
    {
        sq->filter_const = 1;
    }else if (filter_number >SQ_FILTER_MAX_NUMBER)
    {
        sq->filter_const = SQ_FILTER_MAX_NUMBER;
    }else
    {
        sq->filter_const = filter_number;
    }
    return 0;
}

int sqSetPointFilterNumber_five(SEGMENTQUEUE *sq,int filter_number)
{
    if (sq == 0) {
        diagnostics("Error in sqSetPointFilterNumber_five\n");
        return -1;
    }
    if (filter_number < 1)
    {
        sq->point_filtered_const = 1;
    }else if (filter_number >SQ_POINT_FILTER_MAX)
    {
       sq->point_filtered_const= SQ_POINT_FILTER_MAX;
    }else
    {
       sq->point_filtered_const = filter_number;
    }
    return 0;
}

