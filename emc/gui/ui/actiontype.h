#ifndef ACTIONTYPE_H
#define ACTIONTYPE_H

class CNC {

public:

   enum ActionType {

       //***OpenGL modal***//
       ActionViewX,
       ActionViewY,
       ActionViewZ,
       ActionViewZ2,
       ActionViewP ,
       ActionPathClear,
       ActionModelReload,
       ActionZoomIn ,
       ActionZoomOut,
       ActionLoadAbort,


       ActionSaveCoor1,
       ActionSaveCoor2,
       ActionSaveCoor3,
       ActionSaveCoor4,
       ActionSaveCoor5,
       ActionSaveCoor6,
       ActionSaveCoor7,
       ActionSaveCoor8,
       ActionSaveCoor9,
       ActionSaveCoor10,
       ActionLoadCoor1,
       ActionLoadCoor2,
       ActionLoadCoor3,
       ActionLoadCoor4,
       ActionLoadCoor5,
       ActionLoadCoor6,
       ActionLoadCoor7,
       ActionLoadCoor8,
       ActionLoadCoor9,
       ActionLoadCoor10,

       ActionTrigleManual,
       ActionTrigleMdi,
       ActionTrigleProbe,

       ActionTrigleCoor,
       ActionTrigleModel,
       ActionTrigleInfo,
       ActionTrigleIoIn,
       ActionTrigleIoOut,
       ActionTrigleArray,

       ActionFocusOnFeedSlider,
       ActionFocusOnSpindleSlider,

       ActionFeed10,
       ActionFeed20,
       ActionFeed30,
       ActionFeed40,
       ActionFeed50,
       ActionFeed60,
       ActionFeed70,
       ActionFeed80,
       ActionFeed90,
       ActionFeed100,

       ActionSpindlePlus,
       ActionSpindleDec


   };


   enum SliderType {
       SliderFeedOverride,
       SliderSpindleOverride
   };

};


#endif // ACTIONTYPE_H
