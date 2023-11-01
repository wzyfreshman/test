#ifndef CD_OPERATEFEATURE_H
#define CD_OPERATEFEATURE_H

#include "cd_imagescreen.h"
#include "cd_anglemark.h"
#include "cd_crosscomp.h"
#include "cd_testfeature.h"
#include "cd_circlemark.h"
#include "cd_relatemark.h"
#include "cd_circlecomp.h"
#include "cd_anglecomp.h"
#include "cd_enrasercomp.h"

class CCD_API CD_OperateFeature
{

public:
    CD_OperateFeature(CD_ImageScreen *screen);

    void copy(CD_OperateFeature &);

    void init(CD_Template *tmp);
    void showUp(CD::FeatureType);

    bool tryIdentify();

    void speculateTestParameter();
    void testFeature();

    void setMarkType(CD::FeatureType);
    void setMode(CD::OperMode _mode);
    void setAnglePos(CD::AnglePos);
    void setFat(int fat);
    bool doFinishEditFeature();

    CD::FeatureType getCurrentFeatureType();
    CD::OperMode getCurrentMode();
    QList<CD_Feature *> getFeatures();
    QList<CD_Feature *> getFeaturesManual();
    CD::AnglePos getAnglepos();
    CD_ImageScreen *getScreen();

    CD_Feature *getManualFinalFeature(); //this is modified from the mark by try identify
    CD_Feature *getManualFeature();  //this is from the mark directly

private:
    //*when use manual mode ,do this to get the final feature
    //CD_Feature *flushFeatureInManual(QList<RS_Entity*> &lines);
    //CD_Feature *flushFeatureInManual(CD_Feature *manual,const QList<CD_Feature*> &features);
    void releaseFinalFeature();

    CD_ImageScreen *screen;
    CD_AngleMark angle;
    CD_AngleComp angle_comp;
    CD_CrossComp cross;
    CD_CircleComp circle;
    CD_EnraserComp enraser;
    CD_RelateMark relate;

    CD_Feature *finalfeature;

    CD::FeatureType currentmark;
    CD_TestFeature testfeature;
    CD_TestFeature testfeatureformanual;

    CD::OperMode mode;
    CD_Parameter speculatepar;

    CD_FeatureCrossData finalfeaturecross;
    CD_FeatureAngleData finalfeatureangle;
    CD_FeatureCircleData finalfeaturecircle;
    friend class CD_OneMark;

};

#endif // CD_OPERATEFEATURE_H
