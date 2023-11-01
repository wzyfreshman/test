#ifndef SEGMENT_FIVE_H
#define SEGMENT_FIVE_H

#include "segmentqueue.h"

/* Interface functions */


/* function to initialize the segment queue. Needs to be called after
creating an instantiation of the segmentqueue before it can be used */
extern int sqInitQueue_five(SEGMENTQUEUE *sq, SEGMENT *first, int size);

/* functions to set some parameters */

/* function to specify what the initial/current position is */
extern int sqSetPos_five(SEGMENTQUEUE *sq, EmcPose pos);


/* function to empty the segmentqueue */
extern int sqClearQueue_five(SEGMENTQUEUE *sq);

/* functiosn to add a motion to the queue */
extern int sqAddLine_five(SEGMENTQUEUE *sq, EmcPose end, int ID, unsigned char gmodetype, double *dynamicpar);//ghf-2014/3/22
extern int sqAddCircle_five(SEGMENTQUEUE *sq, EmcPose end,
 PmCartesian center, PmCartesian normal,
 int turn, int ID, unsigned char gmodetype, double *dynamicpar);

/* function to set the feed rate for the motions appended after this command */
extern int sqSetFeed_five(SEGMENTQUEUE *sq, double feed);

/* function to change the feed override factor */
extern int sqSetFeedOverride_five(SEGMENTQUEUE *sq, double factor);

/**************************************************************************/
//add by heyuebang in 2015-12-15
/* function to run a new cycle */
extern int sqRunCycle_five(SEGMENTQUEUE *sq); //该函数被修改为计算速度滤波结果
/**************************************************************************/

extern int sqGetPosition_five(SEGMENTQUEUE *sq, EmcPose *p);

extern int sqPause_five(SEGMENTQUEUE *sq);

extern int sqResume_five(SEGMENTQUEUE *sq);

extern int sqAbort_five(SEGMENTQUEUE *sq);

extern int sqStep_five(SEGMENTQUEUE *sq);

extern int sqIsDone_five(SEGMENTQUEUE *sq); /* after pause/abort to see if we're done*/
extern int sqIsPaused_five(SEGMENTQUEUE *sq);
extern int sqIsStepping_five(SEGMENTQUEUE *sq);
//extern double sqGetVel_five(SEGMENTQUEUE *sq);
//extern double sqGetDefaultVel_five(SEGMENTQUEUE *sq);
extern double sqGetMaxAcc_five(SEGMENTQUEUE *sq);
extern int sqGetDepth_five(SEGMENTQUEUE *sq);
extern int sqIsQueueFull_five(SEGMENTQUEUE *sq);
extern int sqGetID_five(SEGMENTQUEUE *sq); /* function the get the ID of the
                 active (sub)segment */

extern int sqClearoldpos_five(SEGMENTQUEUE * sq);
extern int sqIsAborting_five(SEGMENTQUEUE * sq);

extern int sqSetFilterNumber_five(SEGMENTQUEUE *sq,int filter_number);
extern int sqSetPointFilterNumber_five(SEGMENTQUEUE *sq,int filter_number);



#endif // SEGMENT_FIVE_H
