#include "segmentqueue.h"

#include<stdio.h>
#include <math.h>

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>

using namespace  space_seg;


#define max(A,B)  ( (A) > (B) ? (A) : (B))
#define min(A,B)  ( (A) < (B) ? (A) : (B))


static void GetOneStepPVAG(const double X[3], const double time[3], double j, double t,
                    double y[4]);

static void VelocityPlan(double vStart, double vEnd, double a,  double amax,
                  double jmax, double t[3], double *j);

SQ_FollowX::SQ_FollowX()
{

    m_sq = new SEGMENTQUEUE();
    SEGMENT *first =  new SEGMENT[SQ_MAX_Y_AXIS * DEFAULT_TC_QUEUE_SIZE];
    sqInitQueue((SEGMENT (*)[DEFAULT_TC_QUEUE_SIZE])first,DEFAULT_TC_QUEUE_SIZE,0);

}

int SQ_FollowX::sqYInitQueue(YSEGQUEUE *sq, SEGMENT *first,int size)
{

    int i=0;
    SEGMENT *seg;
    if( size <= 0 || sq == 0 || first == 0 ) {
        diagnostics("Error in sqYInitQueue()\n");
        return -1;
    }

    sq->queue = first;
    sq->size = size;

    for (i = 0;i<sq->size;i++)
    {
        seg = sq->queue +i;
        seg->nextSegment = 0;
        seg->prevSegment = 0;
    }

    sq->start = 0;
    sq->end = 0;

    sq->full = 0;
    sq->numSegments = 0;

    return 0;

}

int SQ_FollowX::sqYClearQueue(YSEGQUEUE *sq)
{

    int i = 0;
    SEGMENT *segment;
    if (sq == 0 ) {
        diagnostics("Error in sqYClearQueue()\n");
        return -1;
    }

    for (i=0;i<sq->size;i++)
    {
        segment = sq->queue+i;
        segment->nextSegment = 0;
        segment->prevSegment = 0;
    }
    sq->start = 0;
    sq->end = 0;
    sq->full = 0;
    sq->numSegments = 0;

    return 0;

}

//int SQ_FollowX::sqInitQueue(SEGMENT first[SQ_MAX_Y_AXIS][DEFAULT_TC_QUEUE_SIZE], int size, int ) {

int SQ_FollowX::sqInitQueue(SEGMENT first[SQ_MAX_Y_AXIS][DEFAULT_TC_QUEUE_SIZE], int size, int ) {


   SEGMENTQUEUE *sq = m_sq;
   if ( size <= 0 || sq == 0 || first == 0 ) {
         diagnostics("Error in sqInitQueue()\n");
         return -1;
    }

    //axismask = 0;
    int i=0;
    SEGMENT *first_0;
    YSEGQUEUE *queue;

    for (i=0;i<SQ_MAX_Y_AXIS;i++)
    {
       queue = &(sq->yqueue[i]);
       first_0 = &(first[i][0]);

       if( first_0 == 0)
       {
           diagnostics("Error in sqInitQueue():  first_0 = 0\n");
           return -1;
       }

       if(queue == 0 )
       {
           diagnostics("Error in sqInitQueue(): queue = 0\n");
           return -1;
       }

        sqYInitQueue(queue,first_0,size);
    }

     sq->initXYZ.tran.x = 0;
     sq->initXYZ.tran.y = 0;
     sq->initXYZ.tran.z = 0;
     sq->lastPoint = sq->initXYZ;

     sq->maxFeedOverrideFactor = 1.2; /* this has to be set at start-up using
      sqSetMaxFeedOverride() */
     sq->feedOverrideFactor = 1.0;
     sq->cycleTime = 0;
     sq->currentID = 0;

     sq->done = 1; /* the queue is empty, so by definition we're done */

     sq->paused = 0;

     sq->feedAdjusting = 0;
     sq->aborting = 1;

      sq->G_mode_0.maxJerk = 20000.0;
      sq->G_mode_0.maxAcc_circle= 3000.0;
      sq->G_mode_0.maxAcc_nCircle= 3000.0;
      sq->G_mode_1.maxJerk = 10000.0;


          sq->plot_data = 0;

          sq->plan_time[0]=0.0;
          sq->plan_time[1]=0.0;
          sq->plan_time[2]=0.0;
          sq->start_states[0]=sq->lastPoint.tran.x;
          sq->start_states[1] = 0.0;
          sq->start_states[2] = 0.0;
          sq->start_states[3] = 0.0;

          sq->last_states[0] = 0.0;
          sq->last_states[1] = 0.0;
          sq->last_states[2] =0.0;
          sq->last_states[3] =0.0;

          sq->last_time = 0.0;
          sq->plat_jerk =0.0;

          sq->follow_x_number = 0;

          for(i=0;i<SQ_MAX_X_START_POINT;i++)
          {
              sq->follow_x_trigger[i] = DBL_MAX;
          }

          for(i=0;i<SQ_MAX_Y_AXIS;i++)
          {

              sq->interval_value = 50.0;
              sq->follow_x_offset[i] = 50.0;
              sq->distance_into_step1[i] = 20;
              sq->distance_into_step2[i] = 10;

          }

     return 0;
 }

int SQ_FollowX::sqSetMaxAcc(double amax) {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0 || amax <= 0) {
        diagnostics("Error in SetMaxAcc!!!\n");
        return -1;
    }
    sq->G_mode_1.maxAcc = amax;
    return 0;

}

int SQ_FollowX::sqSetVmax(double vmax) {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0 || vmax <= 0) {
        diagnostics("Error in SetVmax!!!\n");
        return -1;
    }
    sq->G_mode_1.maxV = vmax;
    return 0;

}

int SQ_FollowX::sqSetCycleTime(double secs) {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0 || secs == 0) {
        //diagnostics("Cycletime is zero!!!\n");
        return -1;
    }
    sq->cycleTime = secs;
    sq->ctPow2 = secs * secs;
    sq->ctPow3 = sq->ctPow2 * secs;
    return 0;
}

int SQ_FollowX::sqSetMaxFeedOverride(double mfo) {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0 || mfo <= 0) {
        diagnostics("Error in sqSetMaxFeedOverride()\n");
        return -1;
    }
    sq->maxFeedOverrideFactor = mfo;
    //diagnostics("maxFeedOverrideFactor set to: %e\n", mfo);
    return 0;

}

int SQ_FollowX::sqSetPosition(EmcPose &pos) {

    int i;
    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqSetPos()\n");
        return -1;
    }

    if( sq->aborting == 0 ) {
        diagnostics("system is still running\n");
        return -1;
    }

    sq->initXYZ = pos;
    sq->lastPoint = sq->initXYZ;

    sq->start_states[0] = sq->lastPoint.tran.x;
    sq->start_states[1] = 0.0;
    sq->start_states[2] = 0.0;
    sq->start_states[3] = 0.0;

    sq->last_states[0] = sq->lastPoint.tran.x;
    sq->last_states[1] = 0.0;
    sq->last_states[2] =0.0;
    sq->last_states[3] =0.0;

    //触发点也要清零
    sq->follow_x_number = 0;
    for( i = 0; i < SQ_MAX_X_START_POINT; i++ ) {
        sq->follow_x_trigger[i] = DBL_MAX;
    }


    //初始化所有y轴
    EmcPose posy;
    posy.tran.x = pos.tran.x; posy.tran.y = 0; posy.tran.z = 0.0;
    posy.a = 0.0; posy.b = 0.0; posy.c = 0.0;
    posy.u = 0.0; posy.v = 0.0; posy.w = 0.0;

     i = 0;
    //第0通道使用y数据
    if( i < SQ_MAX_Y_AXIS )  {

        posy.tran.y = pos.tran.y;
        sqSetYpose(posy,0);
        i++;

    }

    //第1通道使用z数据
    if(i < SQ_MAX_Y_AXIS) {

        posy.tran.y = 0.0;
        posy.tran.y = pos.tran.z;
        sqSetYpose(posy,1);
        i++;

    }

    //第2通道使用a数据
    if(i < SQ_MAX_Y_AXIS)
    {
        posy.tran.z = 0.0;
         posy.tran.y = pos.a;
        sqSetYpose(posy,2);
        i++;
    }

    //第3通道使用b数据
    if(i < SQ_MAX_Y_AXIS)
    {
        posy.a = 0.0;
         posy.tran.y = pos.b;
        sqSetYpose(posy,3);
        i++;
    }

    //第4通道使用c数据
    if(i < SQ_MAX_Y_AXIS)
    {
        posy.b = 0.0;
         posy.tran.y = pos.c;
        sqSetYpose(posy,4);
        i++;
    }

    //第5通道使用u数据
    if(i < SQ_MAX_Y_AXIS)
    {
        posy.c = 0.0;
         posy.tran.y = pos.u;
        sqSetYpose(posy,5);
        i++;
    }
    //第6通道使用v数据
    if(i < SQ_MAX_Y_AXIS)
    {
        posy.u = 0.0;
         posy.tran.y = pos.v;
        sqSetYpose(posy,6);
        i++;
    }
    //第7通道使用w数据
    if(i < SQ_MAX_Y_AXIS)
    {
        posy.v = 0.0;
         posy.tran.y = pos.w;
        sqSetYpose(posy,7);
        i++;
    }

    return 0;
}


int SQ_FollowX::sqSetSegStartYpose(EmcPose pos, int channel) {


    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqSetYpose()\n");
        return -1;
    }
    if(channel<0 || channel > SQ_MAX_Y_AXIS)
    {
        diagnostics("Error in sqSetYpose(): channel is out of range\n");
        return -1;
    }
    YSEGQUEUE *seg;
    seg = &(sq->yqueue[channel]);
    seg->segStartPoint = pos;
    return 0;


}

//初始化Y通道位置
int SQ_FollowX::sqSetYpose(EmcPose pos, int channel)
{

     SEGMENTQUEUE *sq = m_sq;
     if (sq == 0) {
         diagnostics("Error in sqSetYpose()\n");
         return -1;
     }

     if(channel<0 || channel > SQ_MAX_Y_AXIS)
     {
         diagnostics("Error in sqSetYpose(): channel is out of range\n");
         return -1;
     }

     YSEGQUEUE *seg;
     seg = &(sq->yqueue[channel]);
     seg->lastPoint = pos;
     seg->initXYZ = pos;
     return 0;

}

int SQ_FollowX::sqClearQueue() {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqClearQueue(): sq == 0 \n");
        return -1;
    }

  int i=0;

  YSEGQUEUE *queue;
  for (i=0;i<SQ_MAX_Y_AXIS;i++)
  {
      queue = &(sq->yqueue[i]);
      if(queue == 0 )
      {
          diagnostics("Error in sqClearQueue(): queue = 0\n");
          return -1;
      }
       sqYClearQueue(queue);
  }


    sq->currentID = 0;

    sq->done = 1; /* the queue is empty, so by definition we're done */

    sq->paused = 0;

    sq->feedAdjusting = 0;
    sq->aborting = 0;

    sq->currentVel = 0;

    //  剩余距离清零
    sq->dynamicparameter[10] = 0;
    sq->dynamicparameter[11] = 0;
    sq->dynamicparameter[12] = 0;
    sq->dynamicparameter[13] = 0;


    sq->pausefinish = 0;

    return 0;
}

/* function to set the feed rate for the motions appended after this command */
int SQ_FollowX::sqSetFeed(double feed) {

    SEGMENTQUEUE *sq = m_sq;
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

int SQ_FollowX::sqAddLine(EmcPose end, int ID,\
              unsigned char ,double *,int channel)
{

    SEGMENTQUEUE *sqq = m_sq;
    qDebug("Add line %.3f %.3f %.3f",end.tran.x,end.tran.y,end.tran.z);

    SEGMENT *newseg, *prevseg;
    EmcPose start;
    YSEGQUEUE *sq;

    /* check if segment queue has been initialized */
    if (sqq == 0 ) {
      diagnostics("Error 1 in sqAddLine()\n");
      return -1;
    }

    if(channel<0 || channel > SQ_MAX_Y_AXIS) {
      diagnostics("Error in sqAddLine(): channel is out of range\n");
      return -1;
    }

    sq = &(sqq->yqueue[channel]);
    if (sq == 0 ) {
      diagnostics("Error 1 in sqAddLine():sq is 0\n");
      return -1;
    }

    sq->numSegments = (sq->end+sq->size-sq->start)%sq->size;//ghf-2015/1/31
    if (sq->numSegments == sq->size - 1) {
      diagnostics(
              "Error 2 Panic!!!!, segmentqueue overflows!!!,sq->numSegments = %d,ID=%d\n",
              sq->numSegments, ID);
      return -1;
    }

    //  sq->lastAppMotionID = ID;


    if (sq->numSegments >= sq->size - SQ_SAFETY_MARGIN) {
        sq->full = 1;
    }

    /********************************************************************************************************/
    if (sq->numSegments == 0) {
        start = sq->segStartPoint;
    } else {
        start = sq->queue[(sq->end + sq->size - 1) % sq->size].end;
    }

    /********************************************************************************************************/
    if(end.tran.x - start.tran.x < 0.0) {
      //太小，忽略
       diagnostics("Error  in sqAddLine(): ID=%d, x must be increased\n",ID);
       return -1;
    }

    if(end.tran.x - start.tran.x < 1e-6) {
      //太小，忽略
       diagnostics("Warning 1 in sqAddLine(): ID=%d\n",ID);
       return 0;
    }

    newseg = sq->queue + sq->end;
    /* ...and update the ring buffer properties */
    sq->end = (sq->end + 1) % sq->size;
    sq->numSegments++;
    /* fill segment parameter fields */
    newseg->ID = ID;

    //newseg->type = sqGetid(emcmotStatus->restartagain,emcmotStatus->idstonum);//ghf-2014/9/16 通过fpga设置了密钥
    newseg->type =SQ_LINEAR;

    newseg->start = start;
    newseg->end = end;



    if (sq->numSegments == 1) {
    //   newseg->start = sq->lastPoint;
      newseg->prevSegment = 0;

    } else {
      /* 有两段以上*/
      prevseg = sq->queue + ((sq->end + sq->size - 2) % sq->size);
      //2 means this function only can be used after that the sq->end has been increased by 1 in function addline or addcircle.
    //   newseg->start = prevseg->end;
      prevseg->nextSegment = newseg;
      newseg->prevSegment = prevseg;
    }

    return 0;
}

int SQ_FollowX::sqGetPosition(EmcPose * p) {

    SEGMENTQUEUE *sq = m_sq;
    if ((sq == 0) || (p == 0)) {
        diagnostics("Error in sqGetPosition()\n");
        return -1;
    }

    p->tran.x = sq->lastPoint.tran.x;
    int i=0;
    if(i<SQ_MAX_Y_AXIS)
    {
        p->tran.y = sq->yqueue[i].lastPoint.tran.y;
        i++;
    }
    if(i<SQ_MAX_Y_AXIS)
    {
        p->tran.z = sq->yqueue[i].lastPoint.tran.y;
        i++;
    }
    if(i<SQ_MAX_Y_AXIS)
    {
        p->a= sq->yqueue[i].lastPoint.tran.y;
        i++;
    }
    if(i<SQ_MAX_Y_AXIS)
    {
        p->b= sq->yqueue[i].lastPoint.tran.y;
        i++;
    }
    if(i<SQ_MAX_Y_AXIS)
    {
        p->c= sq->yqueue[i].lastPoint.tran.y;
        i++;
    }
    if(i<SQ_MAX_Y_AXIS)
    {
        p->u= sq->yqueue[i].lastPoint.tran.y;
        i++;
    }
    if(i<SQ_MAX_Y_AXIS)
    {
        p->v= sq->yqueue[i].lastPoint.tran.y;
        i++;
    }
    if(i<SQ_MAX_Y_AXIS)
    {
        p->w= sq->yqueue[i].lastPoint.tran.y;
        i++;
    }

    //printf("lastPoint.x=%f,lastPoint.y=%f\n",p->tran.x,p->tran.y);
    return 0;
}


/**********************************************************************/
//搜索y
 int SQ_FollowX::sqGetYfromX(int channel)
 {

     SEGMENTQUEUE *sq = m_sq;
     if (sq == 0) {
         diagnostics("Error in sqGetYformX()\n");
         return -1;
     }
     if(channel<0 || channel > SQ_MAX_Y_AXIS)
     {
         diagnostics("Error in sqGetYformX(): channel is out of range\n");
         return -1;
     }

     YSEGQUEUE *sqy;
     sqy = &(sq->yqueue[channel]);

     //找到x在follow_x_trigger中位置
     double x_with_offset = sq->lastPoint.tran.x - sq->follow_x_offset[channel];
     int i = 0;


     double zero_x_pos = DBL_MAX;
     double premove_x_pos = DBL_MAX;

     while( i < sq->follow_x_number && i < SQ_MAX_X_START_POINT )
     {

         if( x_with_offset > sq->follow_x_trigger[i] )
         {
             zero_x_pos = sq->follow_x_trigger[i];
         } else {
             premove_x_pos = sq->follow_x_trigger[i];
             break;
         }
         i++;

     }

     //搜索x在sqy队列中的位置
     zero_x_pos = x_with_offset - zero_x_pos;

     bool continue_plane = true;
     if( premove_x_pos != DBL_MAX ) {

        double ref_value = -(sq->distance_into_step1[channel] + sq->distance_into_step2[channel]);
        premove_x_pos = x_with_offset - premove_x_pos;

        if( channel == 0 ) {
            //printf("premove_x_pos = %.3f\n",premove_x_pos);
        }

        if( premove_x_pos >= ref_value ) {

            continue_plane = false;
            SEGMENT * cursor;
            if( sqy->numSegments == 0 ) {
                return 0;
            }
            cursor = sqy->queue;

            if( premove_x_pos < ( -sq->distance_into_step2[channel] )  ) {

                //起始阶段，进入坐标阶段

                //double den = cursor->start.tran.x - sq->temp_pos[channel].tran.x;
                double den = -sq->distance_into_step2[channel] - sq->temp_pos[channel].tran.x;
                if ( den <= 1e-9 )
                {
                    diagnostics("Error in sqGetYformX(): den is too small\n");
                    return -1;
                }
                double k = (premove_x_pos - sq->temp_pos[channel].tran.x) /den;
                sqy->lastPoint.tran.x = sq->lastPoint.tran.x;
                sqy->lastPoint.tran.y = sq->temp_pos[channel].tran.y + k*(cursor->start.tran.y - sq->temp_pos[channel].tran.y);

                if( channel == 0 ) {
                    //printf("-----clamp-------yyyyy = %.3f  %.3f  \n",sqy->lastPoint.tran.x,sqy->lastPoint.tran.y);
                }

            } else if( premove_x_pos >= ( -sq->distance_into_step2[channel] ) && premove_x_pos < cursor->start.tran.x ) {

                //直线阶段
                sqy->lastPoint.tran.x = sq->lastPoint.tran.x;
                sqy->lastPoint.tran.y = cursor->start.tran.y;
                if( channel == 0 ) {
                    //printf("-----line-------yyyyy = %.3f  %.3f  \n",sqy->lastPoint.tran.x,sqy->lastPoint.tran.y);
                }

            } else {

                //开始规划路径阶段
                if( channel == 0 ) {
                    //printf("-----start-------yyyyy = %.3f  %.3f  \n",sqy->lastPoint.tran.x,sqy->lastPoint.tran.y);
                }
                continue_plane = true;
                zero_x_pos = premove_x_pos;

            }

        } else {
            sq->temp_pos[channel].tran.x = premove_x_pos;
            sq->temp_pos[channel].tran.y = sqy->lastPoint.tran.y;
        }

     }

     if( continue_plane ) {

         i = 0;
         SEGMENT * cursor;
         if (sqy->numSegments == 0) return 0;
         cursor = sqy->queue;

         while( i < sqy->numSegments )
         {

             cursor = sqy->queue+i;
             i++;
             if(cursor->end.tran.x > zero_x_pos)
             {
                 //可能在这一段
                 if(cursor->start.tran.x <= zero_x_pos)
                 {
                     //找到x所在位置，计算其他轴坐标
                     double den = cursor->end.tran.x - cursor->start.tran.x;
                     if ( den <= 1e-9 )
                     {
                         diagnostics("Error in sqGetYformX(): den is too small\n");
                         return -1;
                     }
                     double k = (zero_x_pos - cursor->start.tran.x) /den;
                     sqy->lastPoint.tran.x = sq->lastPoint.tran.x;
                     sqy->lastPoint.tran.y = cursor->start.tran.y + k*(cursor->end.tran.y - cursor->start.tran.y);
                     sqy->lastPoint.tran.z = cursor->start.tran.z +  k*(cursor->end.tran.z - cursor->start.tran.z) ;
                     sqy->lastPoint.a = cursor->start.a +  k*(cursor->end.a - cursor->start.a) ;
                     sqy->lastPoint.b = cursor->start.b +  k*(cursor->end.b - cursor->start.b) ;
                     sqy->lastPoint.c = cursor->start.c +  k*(cursor->end.c - cursor->start.c) ;
                     sqy->lastPoint.u = cursor->start.u +  k*(cursor->end.u - cursor->start.u) ;
                     sqy->lastPoint.v = cursor->start.v +  k*(cursor->end.v - cursor->start.v) ;
                     sqy->lastPoint.w = cursor->start.w +  k*(cursor->end.w - cursor->start.w) ;

                     //printf("sqGetYfromX %.3f %.3f\n",sqy->lastPoint.tran.x,sqy->lastPoint.tran.y);

                 } else {
                     //不在这一段，zero_x_pos太小，什么也不做
                     return 0;
                 }
             } else {
                 //不在这一段，搜索下一段
             }

         }

     }

     return 0;

 }

 int SQ_FollowX::sqSetNewTriggerFromPlanData()
 {

    SEGMENTQUEUE *sq = m_sq;
     if(sq->follow_x_number<0)
     {
         diagnostics("error in sqSetNewTriggerFromPlanData: follow_x_number <0;\n");
         return -1;
     }else if(sq->follow_x_number == 0)
     {
         sq->follow_x_trigger[sq->follow_x_number] = sq->lastPoint.tran.x;
         sq->follow_x_number++;
     }else if(sq->follow_x_number == SQ_MAX_X_START_POINT)
     {
         //存储空间不够，去掉第一个
         if(sq->lastPoint.tran.x - sq->follow_x_trigger[sq->follow_x_number-1] > sq->interval_value)
         {
             int i=0;
             for(i=0;i<SQ_MAX_X_START_POINT-1;i++)
             {
                 sq->follow_x_trigger[i] = sq->follow_x_trigger[i+1];
             }
                 sq->follow_x_trigger[sq->follow_x_number-1] = sq->lastPoint.tran.x;
         }

     }else
     {
         if(sq->lastPoint.tran.x - sq->follow_x_trigger[sq->follow_x_number-1] > sq->interval_value)
         {
                 sq->follow_x_trigger[sq->follow_x_number] = sq->lastPoint.tran.x;
                 sq->follow_x_number++;
         }
     }
    printf("sqSetNewTriggerFromPlanData %.3f\n",sq->lastPoint.tran.x);

     return 0;
 }

int SQ_FollowX::sqSetNewTriggerFromSensorData(double x0)
{

    SEGMENTQUEUE *sq = m_sq;
    if(sq->follow_x_number<0)
    {
        diagnostics("error in sqSetNewTriggerFromPlanData: follow_x_number <0;\n");
        return -1;
    }else if(sq->follow_x_number == 0)
    {
        sq->follow_x_trigger[sq->follow_x_number] = x0;
        sq->follow_x_number++;
    }else if(sq->follow_x_number == SQ_MAX_X_START_POINT)
    {
        //存储空间不够，去掉第一个
        if(sq->lastPoint.tran.x - sq->follow_x_trigger[sq->follow_x_number-1] > sq->interval_value)
        {
            int i=0;
            for(i=0;i<SQ_MAX_X_START_POINT-1;i++)
            {
                sq->follow_x_trigger[i] = sq->follow_x_trigger[i+1];
            }
                sq->follow_x_trigger[sq->follow_x_number-1] = x0;
        }

    }else
    {
        if(sq->lastPoint.tran.x - sq->follow_x_trigger[sq->follow_x_number-1] > sq->interval_value)
        {
                sq->follow_x_trigger[sq->follow_x_number] = x0;
                sq->follow_x_number++;
        }
    }
    return 0;
}

int SQ_FollowX::sqRunCycle()
{

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        printf("Error 1 in sqRunCycle(): Segmentqueue doesn't exist!\n");
        return -1;
    }

    //diagnostics("000000 now in sqruncycle,done=%d\n",sq->done);

    //规划已停止
    if (sq->done == 1) {
        /* do nothing */
        return 0;
    }

    //diagnostics("111 111 now run in sqruncycle\n");

    sq->last_time += sq->cycleTime;

    double temp[3];
    temp[0] = sq->start_states[0]; temp[1] = sq->start_states[1]; temp[2] = sq->start_states[2];
   GetOneStepPVAG(temp,sq->plan_time,sq->plat_jerk,sq->last_time,sq->last_states);

   sq->lastPoint.tran.x = sq->last_states[0];
   //获取其他y通道位置
   int i=0;
   for (i=0;i<SQ_MAX_Y_AXIS;i++)
   {
      sqGetYfromX(i);
   }

   sq->currentVel = sq->last_states[1]; //实时速度计算

   //其他轴计算
   if(sq->aborting == 1)
   {

       if(sq->last_time > sq->plan_time[2])
       {
           sq->done = 1;
            /*
           if (-1 == sqClearQueue()) {
               diagnostics("Error 2 in sqAbort, sqrun\n");
               return -1;
           }*/
       }

   } else if( sq->paused == 1 ) {

        if( sq->last_time > sq->plan_time[2] ) {
              sq->done = 1;
              sq->pausefinish = 1;
        }

   } else {
       //do nothing
   }

   //在程序运行是获取动态变化的信息值
   sq->dynamicparameter[0] = 0.0;
   sq->dynamicparameter[1] = 0.0;
   sq->dynamicparameter[2] = 0.0;
   sq->dynamicparameter[3] = 0.0;
   sq->dynamicparameter[4] = 0.0;
   sq->dynamicparameter[5] =0.0;
   sq->dynamicparameter[6] = 0.0;
   sq->dynamicparameter[7] = 0.0;
   sq->dynamicparameter[8] = 0.0;
   sq->dynamicparameter[9] = 0.0;
   sq->dynamicparameter[10] = 0.0;

   sq->dynamicparameter[18] = sq->last_states[1] ;  //ghf-2014/10/10
   sq->dynamicparameter[19] = sq->last_states[1]; //这样手动自动就统一到这个变量了

   sq->dynamicparameter[10] = 0.0;
   sq->dynamicparameter[11] =0.0;
   sq->dynamicparameter[12] = 0.0;
   sq->dynamicparameter[13] = 0.0;

   sq->dynamicparameter[14] = 1;
   sq->dynamicparameter[15] =1.0;
   sq->dynamicparameter[16] = 0.0;
   sq->dynamicparameter[17] =0.0;

    return 0;

}

static void sqPlan(SEGMENTQUEUE *sq,double target_velocity)
{

    printf("void sqPlan---\n");
    //规划初始点记录为当前点
    sq->start_states[0] = sq->last_states[0];
    sq->start_states[1] = sq->last_states[1];
    sq->start_states[2] = sq->last_states[2];
    sq->start_states[3] = sq->last_states[3];
    //规划时间归为0
    sq->last_time = 0.0;
    VelocityPlan(sq->start_states[1],target_velocity,sq->start_states[2],sq->G_mode_1.maxAcc,sq->G_mode_1.maxJerk,sq->plan_time,&sq->plat_jerk);

}
/* function to change the feed overide */
int SQ_FollowX::sqSetFeedOverride(double fo) {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        printf("Error 1 in sqSetFeedOverride(): Segmentqueue doesn't exist!\n");
        return -1;
    }

    if (fo < 0.01)
        fo = 0.0;
    else if (fo > sq->maxFeedOverrideFactor)
        fo = sq->maxFeedOverrideFactor;

    if(fabs(fo - sq->feedOverrideFactor) >0.01)
    {
        sq->feedOverrideFactor = fo;
        //重新规划
        sqPlan(sq,sq->feedOverrideFactor * min(sq->G_mode_1.maxV,sq->feed));
    }
    return 0;

}

/* function to pause the system (decelerate to zero velocity) */
int SQ_FollowX::sqPause() {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error 1 in sqPause\n");
        return -1;
    }

    if (sq->paused == 1 || sq->aborting ==1) {
        /* don't do anything, system is already paused or stepping will stop soon */
        diagnostics("--don't do anything, system is already paused,sq->done=%d\n",sq->done);
        return 0;
    }

    /* set paused flag */
    sq->paused = 1;
    //diagnostics("\nsq->paused=%d\n", (sq->paused));


  //重新规划
    sqPlan(sq,0.0);

    return 0;
}

/* function to resume with a paused segmentqueue */
int SQ_FollowX::sqResume() {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0 ) {
        diagnostics("Error 1 in sqResume\n");
        return -1;
    }

    diagnostics("get into resume,sq->done=%d\n",sq->done);

    if (sq->done != 1)

    /* we can't resume if the systems is not done yet with a step or pause
     action */
    {
        diagnostics("Can't resume if not done with pause or step action\n");
        /* this is not a critical error, so we'll just ignore the command */
        return 0;
    }

    sq->aborting = 0;
    sq->paused = 0;
    sq->done = 0;
    sq->pausefinish = 0;

    if(sq->feedOverrideFactor < 0.01)
          diagnostics("Resume: feedovverrideFactor is zero\n");

    if(sq->G_mode_1.maxV < 1e-6)
        diagnostics("Resume: maxVelocity is zero\n");
    //重新规划
    sqPlan(sq,sq->feedOverrideFactor * min(sq->G_mode_1.maxV,sq->feed));

    return 0;
}

/* function to abort */
int SQ_FollowX::sqAbort() //ghf-2014/5/17,此函数出错时再跟5/28
{

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error 1 in sqAbort\n");
        return -1;
    }

    if (1 == sq->aborting)
        /* we are already aborting, so let's just ignore it */
        return 0;

    diagnostics("in sqAbort,sq->paused=%d,sq->done=%d\n",sq->paused,sq->done);
    if (sq->done ==1 ) {
         sq->aborting = 1;
         /*
        if (-1 == sqClearQueue()) {
            diagnostics("Error 2 in sqAbort\n");
            return -1;
        }*/
        return 0;
    }

    if (sq->paused != 0 || sq->done == 1) {
          sq->aborting = 1;

        printf("in abort,run sqClearQueue\n");
    } else {
       // emcmotStatus->pausefinish = 0;
        sqPause();
        sq->paused = 0; //取消暂停标志
        sq->aborting = 1; //程序还在运行中点击退出，进到这里，
        printf("in abort,run sqPause\n");
    }

    return 0;
}

/* function to do execute one motion from a stop and stop again */
int SQ_FollowX::sqStep() {


    return 0;
}



int SQ_FollowX::sqIsDone() {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqIsDone\n");
        return -1;
    }

    return sq->done;
}


int SQ_FollowX::sqIsPaused() {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqIsPaused\n");
        return -1;
    }
    return sq->paused;
}
//ghf-2014/10/24,halmeter中要能监视done、paused、numSegments、aborting几个变量
int SQ_FollowX::sqIsAborting() {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqIsAborting\n");
        return -1;
    }
    return sq->aborting;
}


int SQ_FollowX::sqGetDepth() {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqGetDepth\n");
        return -1;
    }
    return 0;
}

int SQ_FollowX::sqIsQueueFull() {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqIsQueueFull\n");
        return -1;
    }
    return 0;
}

int SQ_FollowX::sqGetID() {

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqGetID\n");
        return -1;
    }
    return sq->currentID;

}

//设置模式0 和1 最大速度，最大加速度，最大加加速度，最大直线法向加速度，最大圆弧法向加速度
//未用，command文件中直接赋值了。
int SQ_FollowX::sqSetGMode(GMODE gmode0,GMODE gmode1){

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqSetGMode\n");
        return -1;
    }
   sq->G_mode_0 = gmode0;
   sq->G_mode_1 = gmode1;

    return 0;
}

int SQ_FollowX::sqSetPlotData(int method)
{

    SEGMENTQUEUE *sq = m_sq;
    if (sq == 0) {
        diagnostics("Error in sqSetPlotData\n");
        return -1;
    }
    sq->plot_data = method;

    return 0;
}
int SQ_FollowX::sqGetPlotData()
{

    SEGMENTQUEUE *sq = m_sq;
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

int SQ_FollowX::sqSetParaInt(int index,int val) {

    if(SEGPARANUM > 16)
        return -1;
    SEGMENTQUEUE *sq = m_sq;
    sq->segpara_i[index] = val;
    return 0;

}

//index 0 :g0enableflag g G0速度恒定，m54G0系数
int SQ_FollowX::sqSetParaDouble(int index,double val) {

    if(SEGPARANUM > 16)
        return -1;
    SEGMENTQUEUE *sq = m_sq;
    sq->segpara_d[index] = val;
    return 0;

}

int SQ_FollowX::sqSyncPar(double *getval) {

    SEGMENTQUEUE *sq = m_sq;
    int i=0;
    //获取暂停完毕的信号，算法内需要不断刷新的信息值可以在这里赋值来传到motion
    sq->dynamicparameter[20] = sq->pausefinish;
    for(i=0;i<DYNAMIC_PAR_NUM;i++) {
        getval[i] =  sq->dynamicparameter[i];
    }
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
//   if (t > time[2]) {
//     t = time[2];
//   }

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
           if (temp <0.0)  t1 = a / jmax;
           else t1 = a / jmax + sqrt(temp) / jmax;
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
         if (temp <0.0)       t1 = -a / jmax;
       else t1 = -a / jmax + sqrt(temp) / jmax;

       t2 = t1;
     } else if (jv <= amax * amax - b) {
       *j = -jmax;
         temp = b+jv;
         if(temp <0.0)  t1 = a / jmax;
      else  t1 = a / jmax + sqrt(b + jv) / jmax;
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


 int SQ_FollowX::sqLoadSegmentFromFile(const char *fileName, int channel)
 {

     QFile file(fileName);
     if( file.exists() ) {

         if( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
            qDebug() << "Cannot open file for reading: "
                      << qPrintable(file.errorString());
            return -1;
         }

         sqClearQueue();

         QStringList list;
         list.clear();
         QTextStream in(&file);
         int i = 0;
         bool first_point = true;

         while(!in.atEnd())
         {
             QString fileLine = in.readLine();
             list = fileLine.split(" ", QString::SkipEmptyParts);

             int item_size = list.size();
             if( item_size >= 2 ) {

                 QString strX;
                 QString strY;

                 for( int k = 0; k < item_size; k++ ) {

                     if( list[k].startsWith("X") )
                         strX = list[k].mid(1);
                     if( list[k].startsWith("Y") )
                         strY = list[k].mid(1);

                 }

                 if( !strX.isEmpty() && !strY.isEmpty() ) {

                     EmcPose pos;
                     pos.tran.x = strX.toDouble();
                     pos.tran.y = strY.toDouble();

                     if( first_point )  {

                         first_point = false;
                         sqSetSegStartYpose(pos,0);
                         sqSetSegStartYpose(pos,1);

                     } else {

                         sqAddLine(pos,0,0,0,0);
                         sqAddLine(pos,0,0,0,1);

                     }
                 }

             }
             i++;

         }
         file.close();

     }

 }
