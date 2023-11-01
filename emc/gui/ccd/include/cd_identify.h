#ifndef CD_IDENTIFY_H
#define CD_IDENTIFY_H

#include "cd_imageproc.h"
#include "cd_featurecross.h"
#include "cd_template.h"
#include "cd_featureangledata.h"
#include "rs_vector.h"

class CD_FeatureCircleData;

class CCD_API CD_Identify: public CD_ImageProc
{
public:

    CD_Identify();
    CD_Identify(CD_Image *_image,CD_Template *_temp,CD_Feature *_feature)
        :feature(_feature),temp(_temp),image(_image) {}

    void setImage(CD_Image *_image);
    void setTemplate(CD_Template *_temp);
    void setFeature(CD_Feature *_feature);

    RS_Vector startIdentify(bool *ok);
    RS_Vector identifyInTemp(CD_Template *_temp,CD_Feature *_feature,
                             QList<RS_Entity*> *dst = NULL);

    RS_Vector identifyAngle(QList<RS_Line> &lines,
                            CD_FeatureAngleData *_feature,
                            QList<RS_Entity*> *dst = NULL);
    RS_Vector identifyCross(QList<RS_Line> &lines,
                            CD_FeatureCrossData *_feature,
                            QList<RS_Entity*> *dst = NULL);
    RS_Vector identifyCircle(QList<RS_Circle> &lines,
                            CD_FeatureCircleData *_feature,
                            QList<RS_Entity*> *dst = NULL);

    RS_Vector judgeAngle(QList<RS_Entity*> *lines,CD_FeatureAngleData *par);
    RS_Vector judgeCross(QList<RS_Entity*> *lines,CD_FeatureCrossData *par);

    CD_Feature *feature;
    CD_Template *temp;
    CD_Image *image;
    CD_Image tmpimage;

};

#endif // CD_IDENTIFY_H
