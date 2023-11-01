#include <linux/version.h>
#include <linux/kernel.h>

# include "segmentqueue.h"
#include "segment_five.h"
#include "segment_seven.h"


#include <string.h>

#include<stdio.h>

    #include <sys/ioctl.h>
    #include <linux/hdreg.h>
    #include <fcntl.h>
    #include <stdlib.h>
    #include<ctype.h>

static void initKinematics5AxisParameters(SEGMENTQUEUE *sq);//初始化

static int getdiskid (unsigned char *orghdid, size_t max)
{
    struct hd_driveid id;
    size_t j;
    int  fd;
     fd= open("/dev/sda", O_RDONLY|O_NONBLOCK);
   // fd= open("/dev/hda", O_RDONLY|O_NONBLOCK);
    if (fd < 0) {
        printf("read /dev/sda error");
        close(fd);
        return -1;
    } else {
        if(!ioctl(fd, HDIO_GET_IDENTITY, &id)) {
            for(j=0; j<max; j++) {
                orghdid[j] = id.serial_no[j];
            }
            printf("ok, id=%s\n",orghdid);
        }else{
            printf("Can't open ioctl\n");
        }


    }

    close(fd);
    return 0;
}

/*
#define getidtonum(void) do { \
    int a=0;\
    unsigned char xor=0x55,add=0;\
    unsigned short temp;\
    for(a=0;a<8;a++) {\
        xor ^= emcmotStatus->encrychipid[a];\
        add += emcmotStatus->encrychipid[a];\
    }\
    temp = (add << 8) + xor;\
    emcmotStatus->idstonum = temp;\
} while(0);
*/
//规划速度滤波器
//求均值，输入为sq->deta_s，返回滤波后结果。sq->filter_const决定了求和总数
//条件：sq->filter_const不能等于0,不能大于数组sq->filter最大个数
 double sqMeanFilterV(SEGMENTQUEUE * sq)
{
    int i;
    double temp = 0.0;
   sq->filter[sq->insert_number] = sq->deta_s;//将当前位置增量插入数组
   if(!sq->filter_const){
    diagnostics("Error1 in sqMeanFilterV(): sq->filter_const=0\n");
        sq->filter_const = 1; //不使用滤波
   }
   sq->insert_number = (sq->filter_const + sq->insert_number +1)%sq->filter_const;//调整下次插入下标
   for (i=0;i<sq->filter_const;i++)
   {
       temp += sq->filter[i];
   }
   return temp/sq->filter_const;
}

//初始化规划速度滤波器
void sqInitFilter(SEGMENTQUEUE *sq)
{
    SEGMENT *as;
    sq->insert_number = 1; //
//    sq->filter_const = 5; //初始定义5个数据滤波
//    sq->is_or_not_filterV = 1; //滤波使能
    memset(sq->filter,0, SQ_FILTER_MAX_NUMBER * sizeof(double)); //滤波容器参数初始化
    sq->deta_s = 0.0;  //初始化位置增量
    sq->dist_to_go = 0.0;//初始化剩下需要走的长度
    sq->cursor_aft_filter = sq->size - 1;//初始化当前段的下标，第一次指向最后一段
    as = sq->queue + sq->cursor_aft_filter;//获取最后一段的指针
    as->type = 0;//将此段的类型设为0
    sq->lastPoint_aft_filter = sq->lastPoint; //用于记录滤波后的位置点

}

//初始化单轴滤波器
void sqInitPointFilter(SEGMENTQUEUE *sq)
{
    int i;
    sq->point_filtered_flag = 0; //滤波标志
    sq->point_filtered_const = 10; //最大滤波数据点数
    sq->point_number = 0;//插入点为第一个点
  //滤波容器清零
    for (i=0;i<SQ_POINT_FILTER_MAX;i++)
    {
         sq->FilteredPoint[i] = sq->lastPoint;
    }
}


//单轴均值滤波
 EmcPose sqPointFilter(SEGMENTQUEUE *sq,EmcPose lastPoint)
{
    EmcPose temp;
    int i;
    if (!sq->point_filtered_const){
        diagnostics("Error1 in sqPointFilter(): sq->point_filtered_const=0\n");
        sq->point_filtered_const = 1;
    }
   //将输入位置点插入滤波容器
//    sq->FilteredPoint[sq->point_number].tran = lastPoint.tran;
//    sq->FilteredPoint[sq->point_number].a = lastPoint.a;
//    sq->FilteredPoint[sq->point_number].b = lastPoint.b;
//    sq->FilteredPoint[sq->point_number].c = lastPoint.c;
    sq->FilteredPoint[sq->point_number] = lastPoint;
    //计算下次需要插入的位置下标
    sq->point_number = (sq->point_number + 1 + sq->point_filtered_const) % sq->point_filtered_const;

   //将滤波容器中所有数据相加并将结果/容器最大数，等到平均只
    temp.tran.x = 0.0;
    temp.tran.y= 0.0;
    temp.tran.z = 0.0;
    temp.a =0.0;
    temp.b = 0.0;
    temp.c = 0.0;
    temp.u = 0.0;temp.v = 0.0;temp.w = 0.0;
    for (i=0;i<sq->point_filtered_const;i++)
    {
        temp.tran.x += sq->FilteredPoint[i].tran.x;
        temp.tran.y += sq->FilteredPoint[i].tran.y;
        temp.tran.z += sq->FilteredPoint[i].tran.z;
        temp.a += sq->FilteredPoint[i].a;
          temp.b += sq->FilteredPoint[i].b;
            temp.c += sq->FilteredPoint[i].c;
            temp.u += sq->FilteredPoint[i].u;
              temp.v += sq->FilteredPoint[i].v;
                temp.w += sq->FilteredPoint[i].w;
    }
    temp.tran.x /= sq->point_filtered_const;
     temp.tran.y /= sq->point_filtered_const;
      temp.tran.z /= sq->point_filtered_const;
       temp.a /= sq->point_filtered_const;
       temp.b /= sq->point_filtered_const;
       temp.c /= sq->point_filtered_const;
       temp.u /= sq->point_filtered_const;
       temp.v /= sq->point_filtered_const;
       temp.w /= sq->point_filtered_const;

       return temp;
}

int sqInitQueue(SEGMENTQUEUE * sq, SEGMENT * first, int size) {
    if (size <= 0 || sq == 0 || first == 0) {
        diagnostics("Error in sqInitQueue()\n");
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
    sq->maxFeedOverrideFactor = 1.2; /* this has to be set at start-up using
     sqSetMaxFeedOverride() */
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
       sq->oldDist = 0;

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
      sq->small_seg_cornerV_mode = 0;
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
    printf("sqinitqueue:laststate[0]=%f,laststate[1]=%f,laststate[2]=%f\n",sq->lastState[0],sq->lastState[1],sq->lastState[2]);
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
      //  2*M_PI/360*R 假设R为10
        sq->axis_rate[0] = 1.0;
        sq->axis_rate[1] = 1.0;
        sq->axis_rate[2] = 1.0;
        sq->axis_rate[3] =1;
        sq->axis_rate[4] = 1;
        sq->axis_rate[5] = 1;
        sq->axis_rate[6] = 0.0;
        sq->axis_rate[7] = 0.0;
        sq->axis_rate[8] = 0.0;

         sq->axis9_flag = 2; //0表示3轴，1表示4轴，2表示5轴
         sq->limit_vmax = DBL_MAX;

         sq->abGoAlongXYZ = 0;

         sq->plot_data = 0;

           initKinematics5AxisParameters(sq);//初始化五轴参数
    return 0;
}

int sqSetMaxAcc(SEGMENTQUEUE * sq, double amax) {
    if (sq == 0 || amax <= 0) {
        diagnostics("Error in SetMaxAcc!!!\n");
        return -1;
    }
    sq->G_mode_1.maxAcc = amax;
    return 0;
}

int sqSetVmax(SEGMENTQUEUE * sq, double vmax) {
    if (sq == 0 || vmax <= 0) {
        diagnostics("Error in SetVmax!!!\n");
        return -1;
    }
    sq->G_mode_1.maxV = vmax;
    return 0;
}

int sqSetCycleTime(SEGMENTQUEUE * sq, double secs) {
    if (sq == 0 || secs == 0) {
        //diagnostics("Cycletime is zero!!!\n");
        return -1;
    }
    sq->cycleTime = secs;
    sq->ctPow2 = secs * secs;
    sq->ctPow3 = sq->ctPow2 * secs;
    return 0;
}

int sqSetMaxFeedOverride(SEGMENTQUEUE * sq, double mfo) {
    if (sq == 0 || mfo <= 0) {
        diagnostics("Error in sqSetMaxFeedOverride()\n");
        return -1;
    }
    sq->maxFeedOverrideFactor = mfo;
    //diagnostics("maxFeedOverrideFactor set to: %e\n", mfo);
    return 0;
}

int sqSetPos(SEGMENTQUEUE * sq, EmcPose pos) {
    int i;
    if (sq == 0) {
        diagnostics("Error in sqSetPos()\n");
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

int sqClearQueue(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqClearQueue(): sq == 0 \n");
        return -1;
    }
    sq->numSegments = 0;
    sq->start = sq->end = 0;
    sq->full = 0;
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
    //rtapi_print("in sqClearQueue,sq->numSegments=%d\n",sq->numSegments);
    sq->pausefinish = 0;

   sqInitFilter(sq); //速度滤波参数

   sq->startState[0] = 0.0;sq->startState[1] = 0.0;sq->startState[2] = 0.0;sq->startState[3] = 0.0;

   sq->timeLost = 0.0;
   sq->startState[0] = 0.0;
   sq->startState[1] = 0.0;
sq->startState[2] = 0.0;
sq->startState[3] = 0.0;
memcpy(sq->lastState,sq->startState,4*sizeof(double));
sq->timeLost = 0.0;
sq->remainderLength = 0.0;
sq->timeForRun[0]=0.0;sq->timeForRun[1]=0.0;sq->timeForRun[2]=0.0;sq->timeForRun[3]=0.0;sq->timeForRun[4]=0.0;sq->timeForRun[5]=0.0;sq->timeForRun[6]=0.0;
sq->CruiseStarEndPos[0] = 0.0; sq->CruiseStarEndPos[1] = 0.0;
    return 0;
}

/* function to set the feed rate for the motions appended after this command */
int sqSetFeed(SEGMENTQUEUE * sq, double feed) {

    if (sq == 0) {
        diagnostics("Error in sqSetFeed: null pointer\n");
        return -1;
    }

    if (feed <= 0.0) {
        /* tp.c ignores this and returns an error that's never checked */
        diagnosticsOff("Error in sqSetFeed: non-positive feed %f, using old feed %f\n", feed,sq->feed);
        feed = sq->feed;
    }

        sq->feed = feed;
    return 0;
}

#include "./emc/kinematics/kinematics.h"
int sqAddLine(SEGMENTQUEUE * sq, EmcPose end, int ID, unsigned char gmodetype,double *dynamicpar) //ghf-2014/10/27
{
    int flag;
    double getpos[9];
    reCaculatePos(&end,getpos);
    end.tran.x = getpos[0];
    end.tran.y = getpos[1];
    end.tran.z = getpos[2];
    end.a = getpos[3];
    end.b = getpos[4];
    end.c = getpos[5];
    switch (sq->controlType)
    {
    case four_orde_five_seg:
        flag = sqAddLine_five(sq,end,ID,gmodetype,dynamicpar);
         break;
    case three_orde_seven_seg:
       flag = sqAddLine_seven(sq,end,ID,gmodetype,dynamicpar);
        break;
    default:
        flag =-1;
        break;
    }

    return flag;
}

int sqAddCircle(SEGMENTQUEUE * sq, EmcPose end, PmCartesian center,
        PmCartesian normal, int turn, int ID, unsigned char gmodetype, double *dynamicpar) {
  int flag;
    switch (sq->controlType)
    {
    case four_orde_five_seg:
       flag = sqAddCircle_five(sq,end,center,normal,turn,ID,gmodetype,dynamicpar);
         break;
    case three_orde_seven_seg:
       flag =  sqAddCircle_seven(sq,end,center,normal,turn,ID,gmodetype,dynamicpar);
        break;
    default:
        flag = -1;
        break;
    }

    return flag;
}

int sqGetPosition(SEGMENTQUEUE * sq, EmcPose * p) {
    if ((sq == 0) || (p == 0)) {
        diagnostics("Error in sqGetPosition_five()\n");
        return -1;
    }

   // sq->lastPoint.a = 0; //三轴使用
    /**********************************************************************/
    //add by heyuebang in 2015-12-15
    *p = sq->lastPoint;
     //如果使用规划速度滤波，则调整输出为滤波后输出
    if (sq->is_or_not_filterV == 1)
       {
     //   sq->lastPoint_aft_filter.a =0.0;
        *p = sq->lastPoint_aft_filter;
    }
    //如果使用单轴滤波，则调整输出为滤波后输出
   if( sq->point_filtered_flag == 1)
   {
      // sq->Point_Filtered_LastPoint.a = 0.0;
       *p = sq->Point_Filtered_LastPoint;

   }

   //rtapi_print("X%.3f Y%f Z%.3f A:%.3f B:%.3f C%.3f\n", p->tran.x,p->tran.y,p->tran.z,p->a,p->b,p->c);
   /**********************************************************************/
    //rtapi_print("sq->lastPoint.a = %d\n",(int)(sq->lastPoint.a)); //ghf-2014/9/16,这里应该一直为0才对阿？？
    return 0;
}


/**********************************************************************/
//修改：heyuebang 2015-12-15
//增加均值滤波
int sqRunCycle(SEGMENTQUEUE * sq)
{
   int flag;
    switch (sq->controlType)
    {
    case four_orde_five_seg:
          flag = sqRunCycle_five(sq);
         break;
    case three_orde_seven_seg:
         flag = sqRunCycle_seven(sq);
        break;
    default:
        flag = -1;
        break;
    }

    return flag;

}

/* function to change the feed overide */
int sqSetFeedOverride(SEGMENTQUEUE * sq, double fo) {
   int flag;
    switch (sq->controlType)
    {
    case four_orde_five_seg:
        flag = sqSetFeedOverride_five(sq,fo);
         break;
    case three_orde_seven_seg:
        flag = sqSetFeedOverride_seven(sq,fo);
        break;
    default:
        flag = -1;
        break;
    }

    return flag;
}

/* function to pause the system (decelerate to zero velocity) */
int sqPause(SEGMENTQUEUE * sq) {
   int flag;
    switch (sq->controlType)
    {
    case four_orde_five_seg:
          flag = sqPause_five(sq);
         break;
    case three_orde_seven_seg:
         flag = sqPause_seven(sq);
        break;
    default:
        flag = -1;
        break;
    }

    return flag;
}

/* function to resume with a paused segmentqueue */
int sqResume(SEGMENTQUEUE * sq) {
   int flag;
    switch (sq->controlType)
    {
    case four_orde_five_seg:
          flag=sqResume_five(sq);
         break;
    case three_orde_seven_seg:
         flag=sqResume_seven(sq);
        break;
    default:
        flag = -1;
        break;
    }

    return flag;
}

/* function to abort */
int sqAbort(SEGMENTQUEUE * sq) //ghf-2014/5/17,此函数出错时再跟5/28
{
   int flag;
    switch (sq->controlType)
    {
    case four_orde_five_seg:
          flag=sqAbort_five(sq);
         break;
    case three_orde_seven_seg:
         flag=sqAbort_seven(sq);
        break;
    default:
        flag = -1;
        break;
    }

    return flag;
}

/* function to do execute one motion from a stop and stop again */
int sqStep(SEGMENTQUEUE * sq) {
   int flag;
    switch (sq->controlType)
    {
    case four_orde_five_seg:
           flag=sqStep_five(sq);
         break;
    case three_orde_seven_seg:
         flag=sqStep_seven(sq);
        break;
    default:
        flag=-1;
        break;
    }
    return flag;

}

int sqIsStepping(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqIsStepping\n");
        return -1;
    }
    return sq->stepping;
}

int sqIsDone(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqIsDone\n");
        return -1;
    }

    return sq->done_filter;
}


int sqIsPaused(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqIsPaused\n");
        return -1;
    }
    return sq->paused;
}
//ghf-2014/10/24,halmeter中要能监视done、paused、numSegments、aborting几个变量
int sqIsAborting(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqIsAborting\n");
        return -1;
    }
    return sq->aborting;
}

//未用
double sqGetMaxAcc(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqGetMaxAcc\n");
        return -1;
    }
    return sq->G_mode_1.maxAcc;
}

int sqGetDepth(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqGetDepth\n");
        return -1;
    }
    return sq->numSegments;
}

int sqIsQueueFull(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqIsQueueFull\n");
        return -1;
    }
    return sq->full;
}

int sqGetID(SEGMENTQUEUE * sq) {
    if (sq == 0) {
        diagnostics("Error in sqGetID\n");
        return -1;
    }
    return sq->currentID;
}

int sqSetCornerLimit(SEGMENTQUEUE * sq,int limit) {
    if (sq == 0) {
        diagnostics("Error in sqSetCornerLimit\n");
        return -1;
    }

    sq->cornerlimit = limit;
    return 0;
}
int sqSetFilterNumber(SEGMENTQUEUE *sq,int filter_number)
{
    if (filter_number <= 1)
    {
        sq->filter_const = 1;
        sq->is_or_not_filterV = 0;
    }else if (filter_number >SQ_FILTER_MAX_NUMBER)
    {
        sq->filter_const = SQ_FILTER_MAX_NUMBER;
        sq->is_or_not_filterV = 1;
    }else
    {
        sq->filter_const = filter_number;
        sq->is_or_not_filterV = 1;
    }

    return 0;
}

int sqSetPointFilterNumber(SEGMENTQUEUE *sq,int filter_number)
{
    if (sq == 0) {
        diagnostics("Error in sqSetFilterNumber\n");
        return -1;
    }
    if (filter_number <= 1)
    {
        sq->point_filtered_const = 1;
        sq->point_filtered_flag = 0;
    }else if (filter_number >SQ_POINT_FILTER_MAX)
    {
       sq->point_filtered_const= SQ_POINT_FILTER_MAX;
        sq->point_filtered_flag = 1;
    }else
    {
       sq->point_filtered_const = filter_number;
        sq->point_filtered_flag = 1;
    }

    return 0;
}

int sqSetTolerance(SEGMENTQUEUE *sq,double tolerance)
{
    if (sq == 0) {
        diagnostics("Error in sqSetTolerance\n");
        return -1;
    }
    sq->tolerance = tolerance;
    return 0;
}

int sqSetCircleInsertFlag(SEGMENTQUEUE *sq,int circle_insert_flag)
{
    if (sq == 0) {
        diagnostics("Error in sqSetCircleInsertFlag\n");
        return -1;
    }
    sq->circle_insert_flag = circle_insert_flag;
    return 0;
}

int sqSetCircleToleranceG0(SEGMENTQUEUE *sq,double circle_tolerance_g0)
{
    if (sq == 0) {
        diagnostics("Error in sqSetCircleToleranceG0\n");
        return -1;
    }
    sq->circle_tolerance_g0 = circle_tolerance_g0;
    return 0;
}

int sqSetCircleToleranceG1(SEGMENTQUEUE *sq,double circle_tolerance_g1)
{
    if (sq == 0) {
        diagnostics("Error in sqSetCircleToleranceG1\n");
        return -1;
    }
    sq->circle_tolerance_g1 = circle_tolerance_g1;
    return 0;
}
//optimized_angle单位度
int sqSetOptimizedAngle(SEGMENTQUEUE *sq,double optimized_angle)
{
    if (sq == 0) {
        diagnostics("Error in sqSetCircleToleranceG1\n");
        return -1;
    }
    sq->optimized_angle = optimized_angle*PM_PI/180 + 0.001;
    return 0;
}

 int sqSetOmegaLimit(SEGMENTQUEUE *sq,int omega_limit_flag)
 {
     if (sq == 0) {
         diagnostics("Error in sqSetOmegaLimit\n");
         return -1;
     }
     sq->omega_limit_flag = omega_limit_flag;
     return 0;
 }

  int sqSetaxis9_flag(SEGMENTQUEUE *sq,int axis9_flag)
  {
      if (sq == 0) {
          diagnostics("Error in sqSetaxis9_flag\n");
          return -1;
      }
      sq->axis9_flag = axis9_flag;
      return 0;
  }

int sqSetControlType(SEGMENTQUEUE *sq,sq_control_type controlType)
{
    if (sq == 0) {
        diagnostics("Error in sqSetControlType\n");
        return -1;
    }
    sq->controlType = controlType;
     diagnostics("sqSetControlType： using control mode %d\n",controlType);
    return 0;
}

int sqSetSmallSegMode(SEGMENTQUEUE *sq, int mode)
{
    if (sq == 0) {
        diagnostics("Error in sqSetSmallSegMode\n");
        return -1;
    }
    if (mode>3) {
        diagnostics("Error in sqSetSmallSegMode, mode must small than 4\n");
        return -1;
    }

    sq->small_seg_cornerV_mode = mode;

    return 0;
}
//设置模式0 和1 最大速度，最大加速度，最大加加速度，最大直线法向加速度，最大圆弧法向加速度
//未用，command文件中直接赋值了。
int sqSetGMode(SEGMENTQUEUE *sq,GMODE gmode0,GMODE gmode1){
    if (sq == 0) {
        diagnostics("Error in sqSetGMode\n");
        return -1;
    }
   sq->G_mode_0 = gmode0;
    sq->G_mode_1 = gmode1;

    return 0;
}

int sqSetPlotData(SEGMENTQUEUE *sq, int method)
{
    if (sq == 0) {
        diagnostics("Error in sqSetPlotData\n");
        return -1;
    }
    sq->plot_data = method;

    return 0;
}
int sqGetPlotData(SEGMENTQUEUE *sq)
{
    if (sq == 0) {
        diagnostics("Error in sqGetPlotData\n");
        return -1;
    }

    return sq->plot_data;
}

//index 0 :g0_usefeedrate g G0是否受倍率控制
//index 1 :g0enableflag g G0速度恒定，m54控制
//index 0 :g0_usefeedrate g G0是否受倍率控制
//index 1 :g0enableflag g G0速度恒定，m54控制

int sqSetParaInt(SEGMENTQUEUE *sq,int index,int val) {
    if(SEGPARANUM > 16)
        return -1;
    sq->segpara_i[index] = val;
    return 0;
}

//index 0 :g0enableflag g G0速度恒定，m54G0系数
//index 1 :旋转轴比率
int sqSetParaDouble(SEGMENTQUEUE *sq,int index,double val) {
    if(SEGPARANUM > 16)
        return -1;
    sq->segpara_d[index] = val;

    if(index == 1) {
        sq->axis_rate[3] = sq->segpara_d[1];
        sq->axis_rate[4] = sq->segpara_d[1];
        sq->axis_rate[5] = sq->segpara_d[1];
        sq->axis_rate[6] = 0.0;
        sq->axis_rate[7] = 0.0;
        sq->axis_rate[8] = 0.0;
    }
    return 0;
}

int sqSyncPar(SEGMENTQUEUE *sq,double *getval) {
    int i=0;

    //获取暂停完毕的信号，算法内需要不断刷新的信息值可以在这里赋值来传到motion
    sq->dynamicparameter[20] = sq->pausefinish; //这个参数很关键。
    for(i=0;i<DYNAMIC_PAR_NUM;i++) {
        getval[i] =  sq->dynamicparameter[i];
    }
    return 0;
}





/***************************************************/
/*五轴运动学正解和逆解，提供设置参数，变模式，变刀长功能*/
void setKinematicsMode(SEGMENTQUEUE *sq, kinematicsMode mode)//设置五轴安装模式，初始化时设置即可，默认为xyzcb
{
    if(sq == 0){
    diagnostics("Error in setKinematicsMode\n");
    return;
    }
    sq->kinematics.mode = mode;
}

void setPlanMode(SEGMENTQUEUE *sq, planMode mode)//设置规划模式，rtcp为工件坐标系下规划， normal为5轴机床坐标系下规划，变模式时调用此函数即可，初始模式默认为normal
{
    if(sq == 0){
    diagnostics("Error in setPlanMode\n");
    return;
    }
    sq->kinematics.PlanMode = mode; //只更新新模式
}
//设置刀长，变刀长时调用此函数设置即可，默认为0
//注意 这里length是指刀长在旋转坐标系下旋转角为零时的长度（有正负之分），这里只考虑刀初始指向z轴，正方形为正，反方向为负。
 void setKnifeLength(SEGMENTQUEUE *sq, double length)
 {
     if(sq == 0){
         diagnostics("Error in setKnifeLength\n");
         return;
     }
     sq->kinematics.KnifeLength = length;

     //将轴率改为刀长，方便角度与长度统一规划
//     sq->axis_rate[3] = length*PM_PI/180;
//     sq->axis_rate[4] = sq->axis_rate[3];
//     sq->axis_rate[5] = sq->axis_rate[3];
 }
//设置摆长，该参数不能变，初始化时设置即可或启动前设置，默认为0
//注意 这里length是指摆长在旋转坐标系下旋转角为零时的长度（有正负之分），这里只考虑摆初始指向z轴，正方形为正，反方向为负。
void setPendulemLenth(SEGMENTQUEUE *sq, double length)
{
    if(sq == 0){
        diagnostics("Error in setPendulemLenth\n");
        return;
    }
    sq->kinematics.pendulemLength = length;
    sq->realtimeKinematics.pendulemLength = length;
}

static void initKinematics5AxisParameters(SEGMENTQUEUE *sq)//初始化
{
    if(sq == 0){
    diagnostics("Error in initKinematics5AxisParameters\n");
    return;
    }

    sq->kinematics.mode = CXYZA;
    sq->kinematics.KnifeLength = 0.0;

    sq->kinematics.PlanMode = normal;

    sq->kinematics.pendulemLength = 0.0;

    sq->kinematics.RotationCenter[0]  = 0.0; sq->kinematics.RotationCenter[1]  = 0.0; sq->kinematics.RotationCenter[2]  = 0.0;
    sq->kinematics.WorkpieceCenter[0] = 0.0; sq->kinematics.WorkpieceCenter[1] = 0.0; sq->kinematics.WorkpieceCenter[2] = 0.0;

}

//以下函数描述了求解刀长+摆长。利用同一个刀尖位置，不同旋转角度求解
static double CXYZAsolvePendulemAndKnife(EmcPose point1,EmcPose point2)
{
    double angle = M_PI/180;
    double sinphi = sin(point1.a*angle);
    double cosphi = cos(point1.a*angle);
    double sinpsi = sin(point2.a*angle);
    double cospsi = cos(point2.a*angle);
    double y0 = -sinphi +sinpsi;
    double y1 = cosphi - cospsi;
    double y01 = y0*y0+y1*y1;

    double y2 = - point1.tran.y + point2.tran.y;
    double z2 = - point1.tran.z + point2.tran.z;

    if(fabs(y01) < 0.1)
    {
        diagnostics("CXYZAsolvePendulemAndKnife:error, y01 is too small\n");
        return 0.0;
    }

    double length = (y0*y2 + y1*z2) / y01;
    return length;
}
double solvePendulemAndKnife(SEGMENTQUEUE *sq, EmcPose point1,EmcPose point2)
{

    if(sq == 0){
        diagnostics("Error in solvePendulemAndKnife\n");
        return 0.0;
    }

    double length = 0.0;

    switch(sq->kinematics.mode)
    {
    case XYZCA:

        break;
    case XYZCB:
        break;
    case CXYZA:
         length = CXYZAsolvePendulemAndKnife(point1,point2);
        break;
    default:
        break;
    }

    return length;
}
//以下函数描述了定零点，即机床坐标系下零点人为固定，将刀尖或摆移动到机床坐标系零点，确定xyz零点，旋转ABC零点由其他工艺定
/*摆长参与定零
 *ABC轴已定零，才能使用该函数确定摆旋转中心位置
 *point指5轴读数
 */
// [point.x;point.y;point.z] + RcRa*[0;0;L]+RoCenter = [point.x;point.y;0];最好是phi，psi取为0
static void XYZCAsetRotationCenterWithPendulem(SEGMENTQUEUE *sq, EmcPose point)
{
    double angle = M_PI/180;
    double sinphi = sin(point.a*angle);
    double cosphi = cos(point.a*angle);
    double sinpsi = sin(point.c*angle);
    double cospsi = cos(point.c*angle);
    sq->kinematics.RotationCenter[0] = 0 - sq->kinematics.pendulemLength*sinphi*sinpsi;
    sq->kinematics.RotationCenter[1] =0+ sq->kinematics.pendulemLength *cospsi*sinphi;
    sq->kinematics.RotationCenter[2] = -point.tran.z - sq->kinematics.pendulemLength*cosphi;

    sq->realtimeKinematics.RotationCenter[0] =sq->kinematics.RotationCenter[0];
     sq->realtimeKinematics.RotationCenter[1] =sq->kinematics.RotationCenter[1];
      sq->realtimeKinematics.RotationCenter[2] =sq->kinematics.RotationCenter[2];
}

static void CXYZAsetRotationCenterWithPendulem(SEGMENTQUEUE *sq, EmcPose point)
{
    double angle = M_PI/180;
    double sinphi = sin(point.a*angle);
    double cosphi = cos(point.a*angle);
//    double sinpsi = sin(point.c*angle);
//    double cospsi = cos(point.c*angle);
    sq->kinematics.RotationCenter[0] = -point.tran.x ;
    sq->kinematics.RotationCenter[1] =-point.tran.y+ sq->kinematics.pendulemLength *sinphi;
    sq->kinematics.RotationCenter[2] = -point.tran.z - sq->kinematics.pendulemLength*cosphi;

    sq->realtimeKinematics.RotationCenter[0] =sq->kinematics.RotationCenter[0];
     sq->realtimeKinematics.RotationCenter[1] =sq->kinematics.RotationCenter[1];
      sq->realtimeKinematics.RotationCenter[2] =sq->kinematics.RotationCenter[2];
}

 void setRotationCenterWithPendulem(SEGMENTQUEUE *sq, EmcPose point)
 {
     if(sq == 0){
     diagnostics("Error in setRotationCenterWithPendulem\n");
     }

     switch(sq->kinematics.mode)
     {
     case XYZCA:
         XYZCAsetRotationCenterWithPendulem(sq,  point);
         break;
     case XYZCB:
         break;
     case CXYZA:
             CXYZAsetRotationCenterWithPendulem(sq,  point);
             break;
     default:
         break;
     }

 }

 //以下函数描述了定零点，即机床坐标系下零点人为固定，将刀尖或摆移动到机床坐标系零点，确定xyz零点，旋转ABC零点由其他工艺定
 /*刀长与摆长参与定零
  *ABC轴已定零，才能使用该函数确定摆旋转中心位置
  *point指5轴读数
  */
 static void XYZCAsetRotationCenterWithKnife(SEGMENTQUEUE *sq, EmcPose point)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
     double sinpsi = sin(point.c*angle);
     double cospsi = cos(point.c*angle);
     double length = sq->kinematics.pendulemLength + sq->kinematics.KnifeLength;
     sq->kinematics.RotationCenter[0] = 0 - length*sinphi*sinpsi;
     sq->kinematics.RotationCenter[1] =0+ length *cospsi*sinphi;
     sq->kinematics.RotationCenter[2] = - point.tran.z - length*cosphi;

     sq->realtimeKinematics.RotationCenter[0] =sq->kinematics.RotationCenter[0];
      sq->realtimeKinematics.RotationCenter[1] =sq->kinematics.RotationCenter[1];
       sq->realtimeKinematics.RotationCenter[2] =sq->kinematics.RotationCenter[2];
 }

 static void CXYZAsetRotationCenterWithKnife(SEGMENTQUEUE *sq, EmcPose point)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
//     double sinpsi = sin(point.c*angle);
//     double cospsi = cos(point.c*angle);
     double length = sq->kinematics.pendulemLength + sq->kinematics.KnifeLength;
     sq->kinematics.RotationCenter[0] = - point.tran.x ;
     sq->kinematics.RotationCenter[1] =- point.tran.y+ length *sinphi;
     sq->kinematics.RotationCenter[2] = - point.tran.z - length*cosphi;

     sq->realtimeKinematics.RotationCenter[0] =sq->kinematics.RotationCenter[0];
      sq->realtimeKinematics.RotationCenter[1] =sq->kinematics.RotationCenter[1];
       sq->realtimeKinematics.RotationCenter[2] =sq->kinematics.RotationCenter[2];
 }

 void setRotationCenterWithKnife(SEGMENTQUEUE *sq, EmcPose point)//刀尖参与定零
 {
     if(sq == 0){
     diagnostics("Error in setRotationCenterWithKnife\n");
     return;
     }

     switch(sq->kinematics.mode)
     {
     case XYZCA:
         XYZCAsetRotationCenterWithKnife(sq,  point);
         break;
     case XYZCB:
         break;
     case CXYZA:
              CXYZAsetRotationCenterWithKnife(sq,  point);
              break;
     default:
         break;
     }

 }

 void setRotationCenter(SEGMENTQUEUE *sq, double x,double y, double z)//直接设定
  {
     sq->kinematics.RotationCenter[0] = x ;
     sq->kinematics.RotationCenter[1] =y;
     sq->kinematics.RotationCenter[2] = z;

     sq->realtimeKinematics.RotationCenter[0] =sq->kinematics.RotationCenter[0];
      sq->realtimeKinematics.RotationCenter[1] =sq->kinematics.RotationCenter[1];
       sq->realtimeKinematics.RotationCenter[2] =sq->kinematics.RotationCenter[2];
  }
//以下函数描述了定工件零点，即机床坐标系下零点人为固定，将刀尖或摆移动到工件坐标系零点，确定xyz零点，旋转ABC零点由其他工艺定
 /***********************************************************
  *point 指5轴读值
  * 公式：P = point + rotationCenter + RcRa*[0;0;L];
  */
 static void  XYZCAsetCenterOfWorkpieceWithPendulem(SEGMENTQUEUE *sq, EmcPose point)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
     double sinpsi = sin(point.c*angle);
     double cospsi = cos(point.c*angle);
     sq->kinematics.WorkpieceCenter[0] = point.tran.x + sq->kinematics.pendulemLength *sinphi*sinpsi +sq->kinematics.RotationCenter[0];
     sq->kinematics.WorkpieceCenter[1] = point.tran.y -  sq->kinematics.pendulemLength  *cospsi*sinphi + sq->kinematics.RotationCenter[1];
     sq->kinematics.WorkpieceCenter[2] = point.tran.z +  sq->kinematics.pendulemLength *cosphi + sq->kinematics.RotationCenter[2];

     sq->realtimeKinematics.WorkpieceCenter[0] =sq->kinematics.WorkpieceCenter[0];
      sq->realtimeKinematics.WorkpieceCenter[1] =sq->kinematics.WorkpieceCenter[1];
       sq->realtimeKinematics.WorkpieceCenter[2] =sq->kinematics.WorkpieceCenter[2];
 }

 static void  CXYZAsetCenterOfWorkpieceWithPendulem(SEGMENTQUEUE *sq, EmcPose point)
 {

     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
//     double sinpsi = sin(point.c*angle);
//     double cospsi = cos(point.c*angle);
     sq->kinematics.WorkpieceCenter[0] = point.tran.x  +sq->kinematics.RotationCenter[0];
     sq->kinematics.WorkpieceCenter[1] = point.tran.y -  sq->kinematics.pendulemLength  *sinphi + sq->kinematics.RotationCenter[1];
     sq->kinematics.WorkpieceCenter[2] = point.tran.z +  sq->kinematics.pendulemLength *cosphi + sq->kinematics.RotationCenter[2];

     sq->realtimeKinematics.WorkpieceCenter[0] =sq->kinematics.WorkpieceCenter[0];
      sq->realtimeKinematics.WorkpieceCenter[1] =sq->kinematics.WorkpieceCenter[1];
       sq->realtimeKinematics.WorkpieceCenter[2] =sq->kinematics.WorkpieceCenter[2];
 }

 void setCenterOfWorkpieceWithPendulem(SEGMENTQUEUE *sq, EmcPose point)//设置工件坐标系下零点在机床坐标系下的坐标，启动前设置,，默认为0,摆长参与
{
     if(sq == 0){
     diagnostics("Error in setCenterOfWorkpieceWithPendulem\n");
     }

     switch(sq->kinematics.mode)
     {
     case XYZCA:
         XYZCAsetCenterOfWorkpieceWithPendulem(sq,  point);
         break;
     case XYZCB:
         break;
     case CXYZA:
             CXYZAsetCenterOfWorkpieceWithPendulem(sq,  point);
             break;
     default:
         break;
     }
 }

 static void XYZCAsetCenterOfWorkpieceWithKnife(SEGMENTQUEUE *sq, EmcPose point)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
     double sinpsi = sin(point.c*angle);
     double cospsi = cos(point.c*angle);
        double length = sq->kinematics.pendulemLength + sq->kinematics.KnifeLength;
     sq->kinematics.WorkpieceCenter[0] = point.tran.x + length *sinphi*sinpsi +sq->kinematics.RotationCenter[0];
     sq->kinematics.WorkpieceCenter[1] = point.tran.y -  length *cospsi*sinphi + sq->kinematics.RotationCenter[1];
     sq->kinematics.WorkpieceCenter[2] = point.tran.z +  length *cosphi + sq->kinematics.RotationCenter[2];

     sq->realtimeKinematics.WorkpieceCenter[0] =sq->kinematics.WorkpieceCenter[0];
      sq->realtimeKinematics.WorkpieceCenter[1] =sq->kinematics.WorkpieceCenter[1];
       sq->realtimeKinematics.WorkpieceCenter[2] =sq->kinematics.WorkpieceCenter[2];

 }

 static void CXYZAsetCenterOfWorkpieceWithKnife(SEGMENTQUEUE *sq, EmcPose point)
  {
      double angle = M_PI/180;
      double sinphi = sin(point.a*angle);
      double cosphi = cos(point.a*angle);
 //     double sinpsi = sin(point.c*angle);
 //     double cospsi = cos(point.c*angle);
         double length = sq->kinematics.pendulemLength + sq->kinematics.KnifeLength;
      sq->kinematics.WorkpieceCenter[0] = point.tran.x +sq->kinematics.RotationCenter[0];
      sq->kinematics.WorkpieceCenter[1] = point.tran.y -  length *sinphi + sq->kinematics.RotationCenter[1];
      sq->kinematics.WorkpieceCenter[2] = point.tran.z +  length *cosphi + sq->kinematics.RotationCenter[2];

      sq->realtimeKinematics.WorkpieceCenter[0] =sq->kinematics.WorkpieceCenter[0];
       sq->realtimeKinematics.WorkpieceCenter[1] =sq->kinematics.WorkpieceCenter[1];
        sq->realtimeKinematics.WorkpieceCenter[2] =sq->kinematics.WorkpieceCenter[2];

  }


 void setCenterOfWorkpieceWithKnife(SEGMENTQUEUE *sq, EmcPose point)//设置工件坐标系下零点在机床坐标系下的坐标，启动前设置,，默认为0，刀尖参与
 {
     if(sq == 0){
     diagnostics("Error in setCenterOfWorkpieceWithKnife\n");
     }

     switch(sq->kinematics.mode)
     {
     case XYZCA:
         XYZCAsetCenterOfWorkpieceWithKnife(sq,  point);
         break;
     case XYZCB:
         break;
     case CXYZA:
             CXYZAsetCenterOfWorkpieceWithKnife(sq,  point);
             break;
     default:
         break;
     }
 }
 void setCenterOfWorkpiece(SEGMENTQUEUE *sq, double x, double y, double z)//直接设定
{
     sq->kinematics.WorkpieceCenter[0] = x;
     sq->kinematics.WorkpieceCenter[1] = y;
     sq->kinematics.WorkpieceCenter[2] = z;

     sq->realtimeKinematics.WorkpieceCenter[0] =sq->kinematics.WorkpieceCenter[0];
      sq->realtimeKinematics.WorkpieceCenter[1] =sq->kinematics.WorkpieceCenter[1];
       sq->realtimeKinematics.WorkpieceCenter[2] =sq->kinematics.WorkpieceCenter[2];
 }
//以下三个函数描述了从UG数据到工件坐标系下刀路数据的转化
 static EmcPose XYZCAUGnoPendulem2Workpiece(SEGMENTQUEUE *sq, EmcPose point)
 {
     return point;
 }
 EmcPose UGnoPendulem2Workpiece(SEGMENTQUEUE *sq, EmcPose point)//UG出图没有考虑摆长转化到工件坐标系（没考虑摆长即为工件坐标系）
{
     if(sq == 0){
     diagnostics("Error in UGnoPendulem2Workpiece\n");
     return point;
     }
   EmcPose out;
     switch(sq->kinematics.mode)
     {
     case XYZCA:
         out = XYZCAUGnoPendulem2Workpiece( sq, point);
         break;
     case XYZCB:
         break;
     default:
         break;
     }

     return out;
 }
//只考虑了UGxyz为旋转中心坐标，没有考虑对零问题。
 static EmcPose XYZCAUGwithPendulem2Workpiece(SEGMENTQUEUE *sq, EmcPose point)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
     double sinpsi = sin(point.c*angle);
     double cospsi = cos(point.c*angle);
     EmcPose out;
     out.tran.x = point.tran.x + sq->kinematics.pendulemLength*sinphi*sinpsi;
     out.tran.y = point.tran.y - sq->kinematics.pendulemLength*cospsi*sinphi;
     out.tran.z = point.tran.z + sq->kinematics.pendulemLength*cosphi;
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }
 EmcPose UGwithPendulem2Workpiece(SEGMENTQUEUE *sq, EmcPose point)//UG考虑了摆长转化到工件坐标系
 {
     if(sq == 0){
     diagnostics("Error in UGwithPendulem2Workpiece\n");
     return point;
     }
   EmcPose out;
     switch(sq->kinematics.mode)
     {
     case XYZCA:
         out = XYZCAUGwithPendulem2Workpiece(sq,  point);
         break;
     case XYZCB:
         break;
     default:
         break;
     }

     return out;
 }

 static EmcPose XYZCAUGwithKnife2Workpiece(SEGMENTQUEUE *sq, EmcPose point)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
     double sinpsi = sin(point.c*angle);
     double cospsi = cos(point.c*angle);
     EmcPose out;
     double length =  sq->kinematics.pendulemLength + sq->kinematics.KnifeLength;
     out.tran.x = point.tran.x + length*sinphi*sinpsi;
     out.tran.y = point.tran.y - length*cospsi*sinphi;
     out.tran.z = point.tran.z + length*cosphi;
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 static EmcPose CXYZAUG2Workpiece(SEGMENTQUEUE *sq, EmcPose point)
 {
     if(sq == 0){
     diagnostics("Error in CXYZAUG2Workpiece\n");
     return point;
     }

     return point;
 }

 EmcPose UGwithKnife2Workpiece(SEGMENTQUEUE *sq, EmcPose point)//UG考虑了刀长转化到工件坐标系
 {
     if(sq == 0){
     diagnostics("Error in UGwithKnife2Workpiece\n");
     return point;
     }
   EmcPose out;
     switch(sq->kinematics.mode)
     {
     case XYZCA:
         out = XYZCAUGwithKnife2Workpiece(sq,  point);
         break;
     case XYZCB:
         break;
     case CXYZA:
             out = CXYZAUG2Workpiece( sq, point);
             break;
     default:
         break;
     }

     return out;
 }

//以下三个函数描述了从UG数据到机床xyz坐标系下刀路数据的转化

 EmcPose UGnoPendulem2Lathe(SEGMENTQUEUE *sq, EmcPose point)//UG没有考虑摆长转化到机床坐标系（xyz）
 {
       if(sq == 0){
         diagnostics("Error in UGnoPendulem2Lathe\n");
         return point;
     }
   EmcPose out;
      out = UGnoPendulem2Workpiece(sq,point);//先转工件坐标，再转机床坐标
     return Workpiece2Lathe(sq,out);
 }
 EmcPose UGwithPendulem2Lathe(SEGMENTQUEUE *sq, EmcPose point)//UG考虑摆长转化到机床坐标系
{
     if(sq == 0){
       diagnostics("Error in UGwithPendulem2Lathe\n");
       return point;
   }
 EmcPose out;
    out = UGwithPendulem2Workpiece(sq,point);//先转工件坐标，再转机床坐标
   return Workpiece2Lathe(sq,out);
 }
 EmcPose UGwithKnife2Lathe(SEGMENTQUEUE *sq, EmcPose point)//UG考虑刀长转化到机床坐标系
 {
     if(sq == 0){
       diagnostics("Error in UGwithKnife2Lathe\n");
       return point;
   }
 EmcPose out;
    out = UGwithKnife2Workpiece(sq,point);//先转工件坐标，再转机床坐标
   return Workpiece2Lathe(sq,out);
 }

//以下两个函数描述机床坐标系下刀路到5轴电机数据的相互转化
 static EmcPose XYZCALathe2Axis(SEGMENTQUEUE *sq, EmcPose point,double knifeLength)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
     double sinpsi = sin(point.c*angle);
     double cospsi = cos(point.c*angle);
     EmcPose out;
     double length =  sq->kinematics.pendulemLength + knifeLength;

     out.tran.x = point.tran.x - sq->kinematics.RotationCenter[0] - length*sinphi*sinpsi;
     out.tran.y = point.tran.y - sq->kinematics.RotationCenter[1] +  length*cospsi*sinphi;
     out.tran.z = point.tran.z - sq->kinematics.RotationCenter[2] - length*cosphi;
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 static EmcPose CXYZALathe2Axis(SEGMENTQUEUE *sq, EmcPose point,double knifeLength)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
//     double sinpsi = sin(point.c*angle);
//     double cospsi = cos(point.c*angle);
     EmcPose out;
     double length =  sq->kinematics.pendulemLength + knifeLength;

     out.tran.x = point.tran.x - sq->kinematics.RotationCenter[0];
     out.tran.y = point.tran.y - sq->kinematics.RotationCenter[1] +  length*sinphi;
     out.tran.z = point.tran.z - sq->kinematics.RotationCenter[2] - length*cosphi;
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 EmcPose Lathe2Axis(SEGMENTQUEUE *sq, EmcPose point,double knifeLength)//机床坐标系刀路数据转化到5轴数据
{
     if(sq == 0){
     diagnostics("Error in Lathe2Axis\n");
     return point;
     }
   EmcPose out;
     switch(sq->kinematics.mode)
     {
     case XYZCA:
         out = XYZCALathe2Axis(sq,  point,knifeLength);
         break;
     case XYZCB:
         break;
     case CXYZA:
             out = CXYZALathe2Axis(sq,  point,knifeLength);
             break;
     default:
         break;
     }

     return out;
 }

 static EmcPose XYZCAAxis2Lathe(SEGMENTQUEUE *sq, EmcPose point,double knifeLength)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
     double sinpsi = sin(point.c*angle);
     double cospsi = cos(point.c*angle);
     EmcPose out;
     double length =  sq->kinematics.pendulemLength + knifeLength;
     out.tran.x = point.tran.x + sq->kinematics.RotationCenter[0] + length*sinphi*sinpsi;
     out.tran.y = point.tran.y + sq->kinematics.RotationCenter[1] - length*cospsi*sinphi;
     out.tran.z = point.tran.z + sq->kinematics.RotationCenter[2] + length*cosphi;
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 static EmcPose CXYZAAxis2Lathe(SEGMENTQUEUE *sq, EmcPose point,double knifeLength)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
//     double sinpsi = sin(point.c*angle);
//     double cospsi = cos(point.c*angle);
     EmcPose out;
     double length =  sq->kinematics.pendulemLength + knifeLength;
     out.tran.x = point.tran.x + sq->kinematics.RotationCenter[0] ;
     out.tran.y = point.tran.y + sq->kinematics.RotationCenter[1] - length*sinphi;
     out.tran.z = point.tran.z + sq->kinematics.RotationCenter[2] + length*cosphi;
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 EmcPose Axis2Lathe(SEGMENTQUEUE *sq, EmcPose point,double knifeLength)//5轴数据转化到机床坐标系下刀路数据
 {
     if(sq == 0){
     diagnostics("Error in Axis2Lathe\n");
     return point;
     }
   EmcPose out = point;
     switch(sq->kinematics.mode)
     {
     case XYZCA:
         out = XYZCAAxis2Lathe(sq,  point,knifeLength);
         break;
     case XYZCB:
         break;
     case CXYZA:
         out = CXYZAAxis2Lathe(sq,  point,knifeLength);
         break;
     default:
         break;
     }

     return out;
 }

//以下两个函数描述机床坐标下刀路到工件坐标的相互转化
 static EmcPose XYZCALathe2Workpiece(SEGMENTQUEUE *sq, EmcPose point)
 {
     EmcPose out;
     out.tran.x = point.tran.x - sq->kinematics.WorkpieceCenter[0];
     out.tran.y = point.tran.y  - sq->kinematics.WorkpieceCenter[1];
     out.tran.z = point.tran.z  - sq->kinematics.WorkpieceCenter[2];
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 static EmcPose CXYZALathe2Workpiece(SEGMENTQUEUE *sq, EmcPose point)
 {
     EmcPose out,out0;
     out0.tran.x = point.tran.x - sq->kinematics.WorkpieceCenter[0];
     out0.tran.y = point.tran.y  - sq->kinematics.WorkpieceCenter[1];
     out0.tran.z = point.tran.z  - sq->kinematics.WorkpieceCenter[2];

     double angle = M_PI/180;
//     double sinphi = sin(point.a*angle);
//     double cosphi = cos(point.a*angle);
     double sinpsi = sin(point.c*angle);
     double cospsi = cos(point.c*angle);

     out.tran.x = cospsi*out0.tran.x + sinpsi*out0.tran.y;
     out.tran.y = -sinpsi*out0.tran.x + cospsi*out0.tran.y;
     out.tran.z = out0.tran.z;
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 EmcPose Lathe2Workpiece(SEGMENTQUEUE *sq, EmcPose point) //机床坐标系下刀路转化到工件坐标系下，画图可用
 {
     if(sq == 0){
     diagnostics("Error in Lathe2Workpiece\n");
     return point;
     }
   EmcPose out = point;
     switch(sq->kinematics.mode)
     {
     case XYZCA:
         out = XYZCALathe2Workpiece(sq,  point);
         break;
     case XYZCB:
         break;
     case CXYZA:
         out = CXYZALathe2Workpiece(sq,  point);
         break;
     default:
         break;
     }

     return out;
 }

 static EmcPose XYZCAWorkpiece2Lathe(SEGMENTQUEUE *sq, EmcPose point)
 {
     EmcPose out;
     out.tran.x = point.tran.x + sq->kinematics.WorkpieceCenter[0];
     out.tran.y = point.tran.y  + sq->kinematics.WorkpieceCenter[1];
     out.tran.z = point.tran.z  + sq->kinematics.WorkpieceCenter[2];
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 static EmcPose CXYZAWorkpiece2Lathe(SEGMENTQUEUE *sq, EmcPose point)
 {
     EmcPose out;

     double angle = M_PI/180;
//     double sinphi = sin(point.a*angle);
//     double cosphi = cos(point.a*angle);
     double sinpsi = sin(point.c*angle);
     double cospsi = cos(point.c*angle);

     out.tran.x = cospsi*point.tran.x - sinpsi*point.tran.y + sq->kinematics.WorkpieceCenter[0];
     out.tran.y = sinpsi*point.tran.x + cospsi*point.tran.y  + sq->kinematics.WorkpieceCenter[1];
     out.tran.z = point.tran.z  + sq->kinematics.WorkpieceCenter[2];
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 EmcPose Workpiece2Lathe(SEGMENTQUEUE *sq, EmcPose point) //工件坐标到机床坐标的转化
 {
     if(sq == 0){
     diagnostics("Error in Workpiece2Lathe\n");
     return point;
     }
   EmcPose out = point;
     switch(sq->kinematics.mode)
     {
     case XYZCA:
         out = XYZCAWorkpiece2Lathe(sq,  point);
         break;
     case XYZCB:
         break;
     case CXYZA:
         out = CXYZAWorkpiece2Lathe(sq,  point);
         break;
     default:
         break;
     }

     return out;
 }

 //以下函数实现刀长变化刀路不变;point 为变刀长前的5轴数据，输出为变刀长后的5轴数据，保证前后刀尖点路径不变,注意：该函数输入point如果是机床坐标，结果就是机床坐标，如果输入是5轴数据，输出就是5轴数据
 static EmcPose  XYZCAvaryKnifeLength(EmcPose point, double newKnifeLength,double oldKnifeLength)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
     double sinpsi = sin(point.c*angle);
     double cospsi = cos(point.c*angle);
     EmcPose out;
     double length =  oldKnifeLength -  newKnifeLength;
     out.tran.x = point.tran.x + length*sinphi*sinpsi;
     out.tran.y = point.tran.y - length*cospsi*sinphi;
     out.tran.z = point.tran.z  + length*cosphi;
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 static EmcPose  CXYZAvaryKnifeLength(EmcPose point, double newKnifeLength,double oldKnifeLength)
 {
     double angle = M_PI/180;
     double sinphi = sin(point.a*angle);
     double cosphi = cos(point.a*angle);
//     double sinpsi = sin(point.c*angle);
//     double cospsi = cos(point.c*angle);
     EmcPose out;
     double length =  oldKnifeLength -  newKnifeLength;
     out.tran.x = point.tran.x;
     out.tran.y = point.tran.y - length*sinphi;
     out.tran.z = point.tran.z  + length*cosphi;
     out.a = point.a;out.b = point.b; out.c = point.c; out.u = point.u; out.v = point.v; out.w = point.w;
     return out;
 }

 EmcPose varyKnifeLength(SEGMENTQUEUE *sq,EmcPose point, double newKnifeLength,double oldKnifeLength)
 {
     if(sq == 0){
     diagnostics("Error in varyKnifeLength\n");
     return point;
     }
   EmcPose out = point;
     switch(sq->kinematics.mode)
     {
     case XYZCA:
         out = XYZCAvaryKnifeLength(point,newKnifeLength,oldKnifeLength);
         break;
     case XYZCB:
         break;
     case  CXYZA:
             out = CXYZAvaryKnifeLength( point,newKnifeLength,oldKnifeLength);
             break;
     default:
         break;
     }

     return out;
 }

 //以下函数实现运动学正解和逆解
 EmcPose KinematicsForwardFor5axisRun(SEGMENTQUEUE *sq, EmcPose point, double segmentKnifeLength)
{
     if(sq == 0){
         diagnostics("Error in KinematicsForwardFor5axis\n");
         return point;
     }

     EmcPose  point1;
     switch(sq->realtimeKinematics.PlanMode)
     {
     case normal:
         point1 = point;
         break;
     case rtcp: //工件坐标系下规划模式
         point1 = Axis2Lathe(sq,point,segmentKnifeLength);//获取机械坐标下的刀尖位置
         EmcPose point2 = Lathe2Workpiece(sq,point);
         point1 = point2;
         break;
     case rtcpForLathe://机械坐标下规划
            point1 = Axis2Lathe(sq,point,segmentKnifeLength);//获取机械坐标下的刀尖位置
         break;
     default:
         point1 = point;
         break;
     }

     return point1;
 }
 EmcPose KinematicsInverseFor5axisRun(SEGMENTQUEUE *sq,EmcPose point, double segmentKnifeLength)
 {
     if(sq == 0){
         diagnostics("Error in KinematicsInverseFor5axis\n");
         return point;
     }

     EmcPose  point1;
     EmcPose point2;
     switch(sq->realtimeKinematics.PlanMode)
     {
     case normal:
         point1 = point;
         break;
     case rtcp: //工件坐标系下规划模式
          point2 = Workpiece2Lathe(sq,point);
         point1 = Lathe2Axis(sq,point2,segmentKnifeLength);//由机械坐标计算五轴位置
         break;
     case rtcpForLathe://机械坐标下规划
               point1 = Lathe2Axis(sq,point2,segmentKnifeLength);//由机械坐标计算五轴位置
         break;
     default:
         point1 = point;
         break;
     }

     return point1;

 }
 //以下函数实现运动学正解和逆解
 EmcPose KinematicsForwardFor5axisPlan(SEGMENTQUEUE *sq, EmcPose point, double segmentKnifeLength)
{
     if(sq == 0){
         diagnostics("Error in KinematicsForwardFor5axis\n");
         return point;
     }

     EmcPose  point1;
     switch(sq->kinematics.PlanMode)
     {
     case normal:
         point1 = point;
         break;
     case rtcp: //工件坐标系下规划模式
         point1 = Axis2Lathe(sq,point,segmentKnifeLength);//获取机械坐标下的刀尖位置
         EmcPose point2 = Lathe2Workpiece(sq,point);
         point1 = point2;
         break;
     case rtcpForLathe://机械坐标下规划
            point1 = Axis2Lathe(sq,point,segmentKnifeLength);//获取机械坐标下的刀尖位置
         break;
     default:
         point1 = point;
         break;
     }

     return point1;
 }
 EmcPose KinematicsInverseFor5axisPlan(SEGMENTQUEUE *sq,EmcPose point, double segmentKnifeLength)
 {
     if(sq == 0){
         diagnostics("Error in KinematicsInverseFor5axis\n");
         return point;
     }

     EmcPose  point1,point2;
     switch(sq->kinematics.PlanMode)
     {
     case normal:
         point1 = point;
         break;
     case rtcp: //工件坐标系下规划模式
          point2 = Workpiece2Lathe(sq,point);
         point1 = Lathe2Axis(sq,point2,segmentKnifeLength);//由机械坐标计算五轴位置
         break;
     case rtcpForLathe://机械坐标下规划
               point1 = Lathe2Axis(sq,point2,segmentKnifeLength);//由机械坐标计算五轴位置
         break;
     default:
         point1 = point;
         break;
     }

     return point1;

 }

 //设置多轴规划是采用xyz规划，其他轴跟随，还是采用总的合成速度规划，1表示跟随，0表示合成
 int setsqAbGoAlongXYZ(SEGMENTQUEUE *sq,int flag)//设置规划轴方法
 {
     if(sq == 0){
         diagnostics("Error in setsqAbGoAlongXYZ\n");
         return -1;
     }

     if(flag !=0) flag = 1;
     sq->abGoAlongXYZ = flag;
     return 0;
 }
