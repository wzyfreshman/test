/*
  segmentqueue.h

  Headerfile for segmentqueue.c

  A segment is basically defined as a straight trajectory in space,
  characterized by a start point and an end point. Additional to this,
  a segment can be interpreted as a number of consequetive segments
  with similar properties (orientation and feed). These segments are
  said to be linked.

  The segment queue contains consequetive segments, which are stored
  in a double linked list.

  Using this datastructure the trajectory planning is performed. The basic
  principles for the planning are these:
  * The motion along each segment consists of 3 phases:
  1) adjusting velocity (in most cases acceleration)
  2) cruising
  3) adjusting velocity (in most cases deceleration)
  * When the velocity is adjusted, it is a 3rd order function of time.
  * The cruising speed is calculated such that the last calculated position
    on a segment is exactly the specified end point and that it is never larger
    than the specified feed value.

  The planning of a segment is done whenever needed, so that
  runCycle/getPosition always return a valid new position.


  Modification history:

  13-Aug-2003  FMP added #include "emcpos.h" for EmcPose
   8-Jun-1999  RSB added sqSetMaxFeedOverride()
   8-Jun-1999  RSB changed arguments of sqInitQueue, now also the start point
   of the queue has to be passed. Also took out the line for a statically
   linked in memory pool for the segmentqueue
   7-Jun-1999  RSB added currentVel
  18-May-1999  RSB added helixRadius and amaxTan to SEGMENT
  11-Mar-1999  RSB remove sqAppendMotion, added sqAddLine and sqAddCircle
   9-Mar-1999  RSB changed to PmPose
   5-Mar-1999  RSB removed switch for malloc/kmalloc, now taken care of in
   rtmalloc.h, that's included in segmentqueue.c
   3-Mar-1999  RSB added a flag in segmentqueue named aborting
  18-Feb-1999  FMP changed flag for malloc definition from __KERNEL__
  to rtlinux
  07-Dec-1998  RSB created
*/


#ifndef SEGMENTQUEUE_H
#define SEGMENTQUEUE_H

#include <rtapi_math.h>
/*  #include <stdio.h> */

#include "posemath.h"
/*  #include <linux/cons.h> */

#include "emcpos.h"		/* EmcPose */

#include <linux/version.h>
#include <linux/kernel.h>
#include <float.h>		/* for DBL_MAX */

#include "rtapi.h"		/* rtapi_print_msg */
#include "rtapi_string.h"       /* NULL */

#define SEGVERSION 1

#ifndef PI
#define PI 3.141592654
#endif

#define SQ_LINEAR 1
#define SQ_CIRCULAR 2

//#ifndef max
//#define max(A,B)  ( (A) > (B) ? (A) : (B))
//#endif

//#ifndef min
//#define min(A,B)  ( (A) < (B) ? (A) : (B))
//#endif

#define SQ_SAFETY_MARGIN 1000 //ghf-2014/6/17,因上下不同步，故把此值设大一点，大于上面每次加的段数

#define p_h 1
#define SQ_MAX_NUM_LINKED_SEGS 100  //ghf-2014/7/1

#define SQ_LOW_LINKING_PRIORITY 1
#define SQ_HIGH_LINKING_PRIORITY 2


#define SQ_LINKING_NEEDED 2
#define SQ_LINKING_NOT_NEEDED 1

#ifndef sqrt_2
#define sqrt_2 1.414213562
#endif

/* the next define can be used to turn off printing of individual messages */
#define diagnostics(fmt, args...) rtapi_print(fmt, ## args)
#define diagnosticsOff(fmt, args...) 	/* nothing: it's a placeholder */
//#define diagnosticsOff(fmt, args...) rtapi_print(fmt, ## args)	/* nothing: it's a placeholder */

/********************************************************************/
//add by heyuebang in 2015-12-15
#define SQ_FILTER_MAX_NUMBER 20   //1及其以上整数
#define SQ_POINT_FILTER_MAX 50  //1及其以上整数
/********************************************************************/

#define DYNAMIC_PAR_NUM 32

#define SEGPARANUM 16


typedef enum{
    four_orde_five_seg = 0, //4次5段式
    three_orde_seven_seg = 1, // 3次7段式
   three_orde_sin_seg = 2, //3次正弦函数
    four_orde_sin_seg = 3, //4次正弦函数
    three_orde_five_seg = 4 //3次5段式
}sq_control_type;

typedef enum
{
    XYZCA,
    XYZCB,
    CXYZA,
    CXYZB
}kinematicsMode;
typedef enum
{
    normal = 0, //5轴机床坐标系下规划
    rtcp,  //工件坐标系下规划
    rtcpForLathe//机械坐标下规划
}planMode;
typedef struct _Kinematics5Axis
{
    double pendulemLength;//摆长
    double WorkpieceCenter[3];//工件零点在机床坐标系的位置
    double RotationCenter[3];//旋转中心在机床坐标系下的初始位置，定零点用

    kinematicsMode mode; //安装模式
    planMode PlanMode; //规划模式
     double KnifeLength; //刀长

}Kinematics5Axis;

typedef struct _SegKinematics5Axis
{
    double KnifeLength; //刀长
    kinematicsMode mode; //安装模式
    planMode PlanMode; //规划模式
}SegKinematics5Axis;

/* Structure that contains all the information about a segment */
typedef struct _seg
{
    int ID;                 /* identification number */
    int type;               /* SQ_LINEAR or SQ_CIRCULAR */

    EmcPose start;   //一段的起点
    EmcPose end;  //一段的结束点

    PmLine line;   //直线
    PmCircle circle;  //圆弧

    double helixRadius;     /* 螺旋线高度造成的半径增加，计算螺旋线长度=角度*该参数 */
  /******************************************************************************/
//add by heyuebang 2015-12-25 修改定义，length可变，为一段中还没走完的线段长度，
//暂停，调倍率都要用到剩下没走完的长度来规划，如果暂停，则预处理函数也可能需要用到这个长度，
//之前方案是摧毁当前运行逻辑段，将其已走完的长度毁掉。
  double length;
  
  double givenLength; //给定长度，这个长度就是不会变。

/******************************************************************************/

  double totLength;      /* 线段链接后的总长度，指未走完的总长，即length参数的和 */
  double amaxTan;        /* 最大加速度 */
/******************************************************************************/
//add by heyuebang in 1/23/2014
 double jmaxTan;  /*最大加加速度*/
double amaxNorm; // 拐弯法向加速度，用于计算拐弯最大速度

/******************************************************************************/
  /* 4 次5段式专用*/
 double initInc; //5seg
 double maxInc;//5seg
 double finalInc;//5seg

  int lengthFlag; // not long enouth flag,0 is long enouth,  1 is not  long enouth; 5seg

  int m,p,q;              /* number steps needed for acceleration, cruising
                             and decelerating. */
  int totNumPoints;       /* total number of points on segment (=m+p+q+3) */

  double a1,b1,c1,d1;     /* coefficients for the 3rd polynomial for
                             the speed adjusting in phase 1 */
  double plInitInc;       /* planned init inc */
  double cruiseInc;       /* incrediment for phase 2 */
  double plFinalInc;      /* planned final inc */

  double a3,b3,c3,d3;     /* like the coefficients for phase 1, but then for
                             phase 3 */

   int type_circle_seg;  //1 is clockwise circle_seg,-1 is anticlockwise circle_seg,0 is not circle_seg;
     double givemaxInc;
/******************************************************************************/
    /* 3 次7段式专用*/
       double length_coefficient[9]; //用于九轴分配速度，可为负数，表示末点位移在始点前。
   double initVec; //初始位置增量--初始速度 7seg
   double maxVec; //最大位置增量--最大速度 7seg
   double finalVec;//末点位置增量--末点速度 7seg
     int lastSegFlag; //末段标志
     double plInitVec;       /* 规划后初始速度 */
     double plFinalVec;      /* 规划后末点速度 */
     double givemaxVec;  //给定最大位置增量，该变量不会被调整
/******************************************************************************/


  double angle; // 拐角;


  struct _seg *prevSegment; //前段
  struct _seg *nextSegment; //下一段

  /* some flags */
  int planningDone; //规划好标志
  int active;            /* 正在运行标志*/
  int linkedToPrevSeg;     /* 链接前一段标志 */

  int numLinkedSegs;      /* 总的链接数量，从0开始算，即不包含本身 */
  int gmodetype;//模式

  int tool;//ghf-2014/10/27

  int compside;//hong-2014/10/29

  double dynamiccomp;

    double dynamicGetpar[DYNAMIC_PAR_NUM]; //算法内存储的该线段的信息

    SegKinematics5Axis Kinematics5Axis;

} SEGMENT ;

typedef struct _GMode
{
    double maxV;  //最大速度，x,y,z合速度
    double maxAcc; //最大加速度,xyz合加速度
    double maxJerk; //最大加加速度
    double maxAcc_nLine;//直线法向最大速度，计算拐角用
    double maxAcc_circle; //圆弧最大加速度
    double maxAcc_nCircle;//圆弧最大法向加速度，计算圆弧最大速度用

    double maxAngleAcc_n_Axis[6]; //单轴最大法向加速度
    double maxAngleV_Axis[6]; //单轴最大速度
    //double maxAngleAcc[6]; //角动轴最大加速度，该值等于maxAcc/sq->axis_rate[i];请调axis_rate参数
} GMODE;




/* declaration of the segment queue */
typedef struct
{

  /* Definition of the ring buffer of SEGMENTs */
    SEGMENT *queue;  //定义队列
    int size;               /* 队列最大数量 */
    int numSegments;        /* 目前队列线段 数量*/
    int start, end;         /*开始段，结束 段 */
    int full;               /*队列满标志*/

/****************************************/
    //4次5段式专用
  int n;                    /*目前运行步数 */
 int pausefinish;
// int number;
 //int T_constant;
 /****************************************/

/****************************************/
  //3次7段式专用
  double axis_rate[9]; //各轴转动比率；
  int axis9_flag; // 是否是9轴系统,
   double limit_vmax; //根据线段长度确定的最大速度

   //贺跃帮 17.7.5
    double lastState[4]; // 0 dist 1, velocity,2 acceration,3 jerk
    double startState[4]; //一段运行开始位置，【位置，速度，加速度，加加速度】
   double timeForRun[7]; //7段时刻点；
   double timeLost; //已运行时间
   double incJerk;//加速区加加速度
   double decJerk; //减速区加加速度
   double CruiseStarEndPos[2];//匀速开始和结束点
   double vmaxForPlan; // 最大速度
   double remainderLength;
/****************************************/
  /* some parameters */
  double cycleTime;  //采样周期
  double ctPow2, ctPow3;  /* 采样周期的平方，3次方 */
  double maxFeedOverrideFactor;         /*最大调整倍率*/

  /*used to distinguish the segment numbered G0 or G1(G2,G3); 0 is G0, 1 is G1, G2, G3. Only two modes is considered*/

  GMODE G_mode_0;
  GMODE G_mode_1;

  //ghf-2014/10/13
  double angle1; //相邻两线段的夹角1,范围（90-180度）
  double angle2; //相邻两线段的夹角2,范围（90-angle1度）
  double coeff1; //夹角1系数限制，范围（0-1）
  double coeff2; //夹角2系数限制，范围（0-1）


  double tolerance; //以圆弧拟合计算最大速度时容许拟合误差,弓高误差
  int small_seg_cornerV_mode; // 0 表示由tolerance确认的小线段拐角速度由拐弯法向加速度确定，1表示由曲率确定，2表示由0,1模式最小值确定，3表示由0,1模式最大值确定。
  int circle_insert_flag;   //插入圆弧标志，1表示插入，0表示不插入圆弧
   double circle_tolerance_g0; // g0插入圆弧容许误差
   double circle_tolerance_g1;//g1 插入圆弧容许误差
  // int seglinkflag; // 小线段拟合圆弧标志，1表示拟合，0表示不拟合
   int corner_mix_flag; // 拐弯速度是否使用模糊因子调整

   int abGoAlongXYZ; //1为规划xyz，ab随动，0为规划xyzab合成速度
 // int number; //used for velocity filtering, Setting this number will smooth the Jerk, but also will decrease the corner velicoty proportionally. Unused at present.
 // double T_constant; // used for feed-forward compensation; Unused at present.

  int omega_limit_flag; // 圆弧角速度限制标志，1表示受限，0表示不受限

/******************************************************************************/
//贺跃帮 2015-12-14
//这组参数是为了给规划速度滤波使用
  //double dist_has_gone;
  double deta_s; //每步位置增量
  //double deta_kesi;//上步跳过的位置小量，记录圆弧和一个逻辑段最后可能剩下不足1e-6的长度时跳过的值
  double dist_to_go;//一段剩下没走完的长度
  int  cursor_aft_filter;//滤波后的当前段下标
  double filter[SQ_FILTER_MAX_NUMBER]; //滤波容器
  int insert_number; //当前插入滤波前数据位置
  int filter_const; //滤波常数，1 - SQ_FILTER_MAX_NUMBER
  EmcPose lastPoint_aft_filter; //用于记录滤波后的位置点
  int is_or_not_filterV; // 是否使用速度滤波标志，0表示不使用滤波，1表示使用滤波（事实上filter_const = 1 也等价于不使用滤波）

//这组参数是给三轴单独滤波使用
  int point_filtered_flag;  //滤波标志，1为使用滤波，0为不使用
  int point_filtered_const;  //滤波常数，1-SQ_POINT_FILTER_MAX,20对应误差理论上达到0.01以上，10对应0.05左右，5对应0.001左右,1与不使用滤波一致
    int point_number; // 当前插入滤波容器的下标
    EmcPose FilteredPoint[SQ_POINT_FILTER_MAX]; // 滤波容器
    EmcPose Point_Filtered_LastPoint; //滤波后的输出值

     double optimized_angle; //优化角度，只有大于这个角度才会计算拐角速度，否则拐角速度设为0;
     int done_filter;
/******************************************************************************/
  /* feed rate of the next motion(s) to append */
  double feed;//给定速度

  /* initial position */
  EmcPose initXYZ;//启动时的初始位置

  /* the so far travelled distance along the segment */
  double dist; //已运行距离
  double oldDist;
  /* last calculated point */
  EmcPose lastPoint;//最新位置

  /* feed overide factor, 1 is default */
  double feedOverrideFactor;  //倍率因子

  /* some flags */
  int done; /* 停止标志，指规划已停止，但如果有轨迹滤波，滤波可能还没停止*/
  int paused; /*设置暂停标志 */
  int stepping; /* 设置步进标志 */
  int feedAdjusting; //调速标志
  int aborting;  //终止标志

  /* for internal use .... */
  EmcPose base;
  double offset;
  double cumLength;
  SEGMENT *cursor;//正在运行段，轨迹滤波前的运行段

  /* ID of the segment on which the last calculated point lays */
  int currentID; //正在运行id，轨迹滤波前。

  /* ID of the last appended motion */
  int lastAppMotionID;//最新加入的ID


  double currentVel;   /*目前速度 */

  int delayTime; //启动滞后时间，用于run函数启动滞后于addline或addcircle函数，防止没有段时启动run函数造成错误。

  int cornerlimit; //圆弧相接点是否受到拐角限制。



  sq_control_type controlType;

  int plot_data; // 0 画图数据为sq规划数据，1 为精插补后数据，2为编码器返回数据

  int segpara_i[SEGPARANUM];
  double segpara_d[SEGPARANUM];
  double dynamicparameter[DYNAMIC_PAR_NUM]; //算法内存储的该线段的信息

 /********************************************************/
  Kinematics5Axis kinematics; //定义五轴参数和模式,插入线段用
  Kinematics5Axis realtimeKinematics;//实时运行时记录当前模式，状态，刀长等

  int version;              //算法版本定义
} SEGMENTQUEUE;

/***************************************************/
/*五轴运动学正解和逆解，提供设置参数，变模式，变刀长功能*/
extern void setKinematicsMode(SEGMENTQUEUE *sq, kinematicsMode mode);//设置五轴安装模式，初始化时设置即可，默认为xyzcb
extern void setPlanMode(SEGMENTQUEUE *sq, planMode mode);//设置规划模式，rtcp为工件坐标系下规划， normal为5轴机床坐标系下规划，变模式时调用此函数即可，初始模式默认为normal
extern void setKnifeLength(SEGMENTQUEUE *sq, double length);//设置刀长，变刀长时调用此函数设置即可，默认为0
extern void setPendulemLenth(SEGMENTQUEUE *sq, double length);//设置摆长，该参数不能变，初始化时设置即可或启动前设置，默认为0

extern double solvePendulemAndKnife(SEGMENTQUEUE *sq, EmcPose point1,EmcPose point2);

extern void setRotationCenter(SEGMENTQUEUE *sq, double x,double y, double z);//直接设定

extern void setCenterOfWorkpiece(SEGMENTQUEUE *sq, double x, double y, double z);//直接设定

//以下函数描述了定零点，即机床坐标系下零点人为固定，将刀尖或摆移动到机床坐标系零点，确定xyz零点，旋转ABC零点由其他工艺定
extern void setRotationCenterWithPendulem(SEGMENTQUEUE *sq, EmcPose point);//摆长参与定零
extern void setRotationCenterWithKnife(SEGMENTQUEUE *sq, EmcPose point);//刀尖参与定零
//以下函数描述了定工件零点，即机床坐标系下零点人为固定，将刀尖或摆移动到工件坐标系零点，确定xyz零点，旋转ABC零点由其他工艺定
extern void setCenterOfWorkpieceWithPendulem(SEGMENTQUEUE *sq, EmcPose point);//设置工件坐标系下零点在机床坐标系下的坐标，启动前设置,，默认为0,摆长参与
extern void setCenterOfWorkpieceWithKnife(SEGMENTQUEUE *sq, EmcPose point);//设置工件坐标系下零点在机床坐标系下的坐标，启动前设置,，默认为0，刀尖参与

//以下三个函数描述了从UG数据到工件坐标系下刀路数据的转化
extern EmcPose UGnoPendulem2Workpiece(SEGMENTQUEUE *sq, EmcPose point);//UG出图没有考虑摆长转化到工件坐标系（没考虑摆长即为工件坐标系）
extern EmcPose UGwithPendulem2Workpiece(SEGMENTQUEUE *sq, EmcPose point);//UG考虑了摆长转化到工件坐标系
extern EmcPose UGwithKnife2Workpiece(SEGMENTQUEUE *sq, EmcPose point);//UG考虑了刀长转化到工件坐标系

//以下三个函数描述了从UG数据到机床xyz坐标系下刀路数据的转化
extern EmcPose UGnoPendulem2Lathe(SEGMENTQUEUE *sq, EmcPose point);//UG没有考虑摆长转化到机床坐标系（xyz）
extern EmcPose UGwithPendulem2Lathe(SEGMENTQUEUE *sq, EmcPose point);//UG考虑摆长转化到机床坐标系
extern EmcPose UGwithKnife2Lathe(SEGMENTQUEUE *sq, EmcPose point);//UG考虑刀长转化到机床坐标系

//以下两个函数描述机床坐标系下刀路到5轴电机数据的相互转化
extern EmcPose Lathe2Axis(SEGMENTQUEUE *sq, EmcPose point,double knifeLength);//机床坐标系刀路数据转化到5轴数据
extern EmcPose Axis2Lathe(SEGMENTQUEUE *sq, EmcPose point,double knifeLength);//5轴数据转化到机床坐标系下刀路数据

//以下两个函数描述机床坐标下刀路到工件坐标的相互转化
extern EmcPose Lathe2Workpiece(SEGMENTQUEUE *sq, EmcPose point); //机床坐标系下刀路转化到工件坐标系下，画图可用
extern EmcPose Workpiece2Lathe(SEGMENTQUEUE *sq, EmcPose point); //工件坐标到机床坐标的转化

//以下函数实现刀长变化刀路不变
extern EmcPose varyKnifeLength(SEGMENTQUEUE *sq,EmcPose point, double newKnifeLength,double oldKnifeLength);

//以下函数实现运动学正解和逆解,求5轴坐标到工件坐标的转换。
extern EmcPose KinematicsForwardFor5axisRun(SEGMENTQUEUE *sq,EmcPose point,double segmentKnifeLength);
extern EmcPose KinematicsInverseFor5axisRun(SEGMENTQUEUE *sq, EmcPose point, double segmentKnifeLength);
extern EmcPose KinematicsForwardFor5axisPlan(SEGMENTQUEUE *sq,EmcPose point,double segmentKnifeLength);
extern EmcPose KinematicsInverseFor5axisPlan(SEGMENTQUEUE *sq, EmcPose point, double segmentKnifeLength);
/***************************************************/
extern int setsqAbGoAlongXYZ(SEGMENTQUEUE *sq,int flag);//设置规划轴方法

/* function to initialize the segment queue. Needs to be called after
creating an instantiation of the segmentqueue before it can be used */
extern int sqInitQueue(SEGMENTQUEUE *sq, SEGMENT *first, int size);
extern double sqMeanFilterV(SEGMENTQUEUE * sq);
 extern EmcPose sqPointFilter(SEGMENTQUEUE *sq,EmcPose lastPoint);
 extern void sqInitPointFilter(SEGMENTQUEUE *sq);
  extern void sqInitFilter(SEGMENTQUEUE *sq);

/* functions to set some parameters */
extern int sqSetMaxAcc(SEGMENTQUEUE *sq, double amax);
extern int sqSetVmax(SEGMENTQUEUE *sq, double vmax);
extern int sqSetCycleTime(SEGMENTQUEUE *sq, double secs);
extern int sqSetMaxFeedOverride(SEGMENTQUEUE *sq, double mfo);

/* function to specify what the initial/current position is */
extern int sqSetPos(SEGMENTQUEUE *sq, EmcPose pos);

/* function to remove a whole segmentqueue from memory */
//extern int sqTrashQueue(SEGMENTQUEUE *sq);

/* function to empty the segmentqueue */
extern int sqClearQueue(SEGMENTQUEUE *sq);

/* functiosn to add a motion to the queue */
extern int sqAddLine(SEGMENTQUEUE *sq, EmcPose end, int ID, unsigned char gmodetype, double *dynamicpar);//ghf-2014/3/22
extern int sqAddCircle(SEGMENTQUEUE *sq, EmcPose end,
 PmCartesian center, PmCartesian normal,
 int turn, int ID, unsigned char gmodetype, double *dynamicpar);

/* function to set the feed rate for the motions appended after this command */
extern int sqSetFeed(SEGMENTQUEUE *sq, double feed);

/* function to change the feed override factor */
extern int sqSetFeedOverride(SEGMENTQUEUE *sq, double factor);

/**************************************************************************/
//add by heyuebang in 2015-12-15
/* function to run a new cycle */
extern int sqRunCycle(SEGMENTQUEUE *sq); //该函数被修改为计算速度滤波结果
/**************************************************************************/

extern int sqGetPosition(SEGMENTQUEUE *sq, EmcPose *p);

extern int sqPause(SEGMENTQUEUE *sq);

extern int sqResume(SEGMENTQUEUE *sq);

extern int sqAbort(SEGMENTQUEUE *sq);

extern int sqStep(SEGMENTQUEUE *sq);

extern int sqIsDone(SEGMENTQUEUE *sq); /* after pause/abort to see if we're done*/
extern int sqIsPaused(SEGMENTQUEUE *sq);
extern int sqIsStepping(SEGMENTQUEUE *sq);
//extern double sqGetVel(SEGMENTQUEUE *sq);
//extern double sqGetDefaultVel(SEGMENTQUEUE *sq);
extern double sqGetMaxAcc(SEGMENTQUEUE *sq);
extern int sqGetDepth(SEGMENTQUEUE *sq);
extern int sqIsQueueFull(SEGMENTQUEUE *sq);
extern int sqGetID(SEGMENTQUEUE *sq); /* function the get the ID of the
                 active (sub)segment */

//extern int sqClearoldpos(SEGMENTQUEUE * sq);
extern int sqIsAborting(SEGMENTQUEUE * sq);

extern int sqSetCornerLimit(SEGMENTQUEUE * sq,int limit); //拐角是否限速
extern int sqSetFilterNumber(SEGMENTQUEUE *sq,int filter_number);
extern int sqSetPointFilterNumber(SEGMENTQUEUE *sq,int filter_number);

/***************************************************************************/
extern int sqSetTolerance(SEGMENTQUEUE *sq,double tolerance);
extern int sqSetCircleInsertFlag(SEGMENTQUEUE *sq,int circle_insert_flag);
extern int sqSetCircleToleranceG0(SEGMENTQUEUE *sq,double circle_tolerance_g0);
extern int sqSetCircleToleranceG1(SEGMENTQUEUE *sq,double circle_tolerance_g1);
extern int sqSetOptimizedAngle(SEGMENTQUEUE *sq,double optimized_angle);
extern int sqSetOmegaLimit(SEGMENTQUEUE *sq,int omega_limit_flag);
extern int sqSetaxis9_flag(SEGMENTQUEUE *sq,int axis9_flag);
extern int sqSetControlType(SEGMENTQUEUE *sq,sq_control_type controlType);
extern int sqSetSmallSegMode(SEGMENTQUEUE *sq, int mode);
//设置模式0 和1 最大速度，最大加速度，最大加加速度，最大直线法向加速度，最大圆弧法向加速度
extern int sqSetGMode(SEGMENTQUEUE *sq,GMODE gmode0,GMODE gmode1);
/***************************************************************************/

extern int sqSetPlotData(SEGMENTQUEUE *sq, int method);
extern int sqGetPlotData(SEGMENTQUEUE *sq);


extern int sqSetParaInt(SEGMENTQUEUE *sq,int index,int val);
extern int sqSetParaDouble(SEGMENTQUEUE *sq,int index,double val);
extern int sqSyncPar(SEGMENTQUEUE *sq,double *getval) ;

#endif
