#ifndef SEGMENT_SEVEN_H
#define SEGMENT_SEVEN_H

#include "segmentqueue.h"

/* Interface functions */

/* function to initialize the segment queue. Needs to be called after
 creating an instantiation of the segmentqueue before it can be used */
extern int sqInitQueue_seven(SEGMENTQUEUE *sq, SEGMENT *first, int size);
/* functions to set some parameters */




/* function to remove a whole segmentqueue from memory */
//extern int sqTrashQueue(SEGMENTQUEUE *sq);



/* Implemented commands */
/* -------------------- */

/* functiosn to add a motion to the queue */
extern int sqAddLine_seven(SEGMENTQUEUE *sq, EmcPose end, int ID, unsigned char gmodetype, double *dynamicpar);//ghf-2014/3/22
extern int sqAddCircle_seven(SEGMENTQUEUE *sq, EmcPose end,
                     PmCartesian center, PmCartesian normal,
                     int turn, int ID, unsigned char gmodetype, double *dynamicpar);

/* function to set the feed rate for the motions appended after this command */


/* function to change the feed override factor */
extern int sqSetFeedOverride_seven(SEGMENTQUEUE *sq, double factor);

/**************************************************************************/
//add by heyuebang in 2015-12-15
/* function to run a new cycle */
extern int sqRunCycle_seven(SEGMENTQUEUE *sq); //该函数被修改为计算速度滤波结果
/**************************************************************************/



/* function to pause the system (decelerate to zero velocity) */
extern int sqPause_seven(SEGMENTQUEUE *sq);

/* function to resume with a paused segmentqueue */
extern int sqResume_seven(SEGMENTQUEUE *sq);

/* function to abort */
extern int sqAbort_seven(SEGMENTQUEUE *sq);

/* function to do execute one motion from a stop and stop again */
extern int sqStep_seven(SEGMENTQUEUE *sq);


/* functions to get some status information */
/* ---------------------------------------- */




#endif // SEGMENT_FIVE_H
