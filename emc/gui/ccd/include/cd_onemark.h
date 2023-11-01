#ifndef CD_ONEMARK_H
#define CD_ONEMARK_H

#include "cd_operatefeature.h"
#include "cd_operatetemplate.h"
#include "cd_inisettings.h"
#include "cd_matchtemplate.h"

class CD_MainScreen;

class CCD_API CD_OneMark
{

public:
    enum State {
        StateIdle,
        StateSelectTemp,
        StateEditeFeature
    };

    CD_OneMark(CD_ImageScreen *opfc,CD_ImageScreen *optmp,int num);
    ~CD_OneMark();
    virtual void saveToDisk( CD_IniSettings &set,const QString &name);
    virtual void readFromDisk( CD_IniSettings &set,const QString &name);

    bool actionSelectTemplate();
    bool actionEditFeature(bool init = true);
    virtual RS_Vector actionIdentify(cv::Mat&, CD_MainScreen *m,
                                     CD_Template *temp = NULL,int angles_index = -1);
    virtual MatchResult getIdentifyResult();

    void init();

    void reset();
    bool exitEditeFeature();
    bool exitSelectTemplate();

    bool actionSetFeatureType(CD::FeatureType);
    void actionSetOperMode(CD::OperMode);
    void actionSetAnglePos(CD::AnglePos);
    void actionSetFat(int);

    void actionTestFeature();
    void actionTryIdentify();

    bool isTemplateValid();
    CD_ImageScreen *getScreenTemplate();
    CD_ImageScreen *getScreenFeature();


    QString determineTemplete();
    CD_Template* getTemplateImage();

    QString determineFeature();
    CD_Feature* getFinalFeature();


    State getState();
    CD::OperMode getFeatureOperMode();
    CD::FeatureType getFeatureType();
    CD::AnglePos getAnglePos();
    QList<CD_Feature *> getFeatures();
    QList<CD_Feature *> getFeaturesCurrent();
    RS_Vector getIdentifyVector();
    int getSelfMarkNum();

    bool isFirstRead();
    void resettFirstRead();

    virtual void push();
    virtual void pop();

    virtual void setSpecifyAreaEnable(bool);
    virtual bool isSpecifyAreaEnable();
    virtual void setSpecifyArea(const QRect &rec);
    virtual QRect getSpecifyArea();

    CD_CrossComp* getCossMark();
    CD_CircleComp*getCircleMark();
    CD_AngleComp* getAngleMark();
    CD_RelateMark* getRelateMark();
    CD_EnraserComp* getEnraserMark();

    void initProfile();

    MatchResult matchTemplate(cv::Mat &mat);
    void setShapeBaseMatchEnable(bool b);
    void setCannyThr(int l,int h);
    void setSimThresholdShape(double s);
    double getSimThresholdShape();
    bool getShapeBaseMatchFlag();

    void getMarkMaxRect(int *w,int *h);
    void getRelateLocation(double *x,double *y,int num = 0);

private:

    CD_Template templateimage;
    CD_Template templateimagestore;

    CD_Feature *determinedfeature;
    CD_Feature *determinedfeaturestore;

    CD_OperateFeature opfeature; 
    CD_OperateTemplate optemplate;
    CD_OperateFeature opfeaturestore;
    CD_OperateTemplate optemplatestore;

    bool optmplatevisable;
    bool firstread;
    State state;

    bool optmplatevisablestore;
    bool firstreadstore;
    State statestore;

    int marknum;
    CD_ImageScreen *featurescreen;
    CD_ImageScreen *templatescreen;

    RS_Vector resultvector;
    MatchResult result;

    bool shape_base_match_flag;
    bool shape_base_match_flag_store;
    double sim_threshold;
    double sim_threshold_store;
    double relate_location_x;
    double relate_location_x_store;
    double relate_location_y;
    double relate_location_y_store;

    double relate_location_x2;
    double relate_location_x2_store;
    double relate_location_y2;
    double relate_location_y2_store;

    int canny_threshold_low;
    int canny_threshold_hight;
    int canny_threshold_low_store;
    int canny_threshold_hight_store;

    std::vector<TemplateProfile> *profile;
    int profile_array_num;

    MatchTemplateParameter match_par;
    MatchTemplateParameter match_par_store;

    cv::Mat pyramid_img;
    cv::Mat pyramid_bit;

    cv::Mat pyramid_img_store;
    cv::Mat pyramid_bit_store;

protected:
    bool is_specify_area;
    QRect specify_area;

    friend class CCD_MarkEditor;

};

#endif // CD_ONEMARK_H
