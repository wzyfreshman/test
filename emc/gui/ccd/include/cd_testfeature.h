#ifndef CD_TESTFEATURE_H
#define CD_TESTFEATURE_H

#include "cd_featurecross.h"
#include "cd_template.h"
#include "cd_parameter.h"
#include "rs_vector.h"
#include "rs_line.h"
#include "cd_imageproc.h"

class CCD_API CD_TestFeature : public CD_ImageProc
{
public:
    CD_TestFeature();
    CD_TestFeature(CD_Image *_image)
        :image(_image) {}
    ~CD_TestFeature();
    void operator = (CD_TestFeature& cp);

    void init();
    void setImage(CD_Image *_image);
    void setType(CD::FeatureType);
    void setAnglePos(CD::AnglePos);
    void startTryAll(CD_Parameter* p=NULL);
    void generateSolution();

    void findCrossFeatures(QList<RS_Line> &alldetectlines,
                           CD_Parameter &par,QList<CD_Feature *>  &finalfeatures);

    bool isAllreadyGetEntity();

    bool checkAngleType(const RS_Line &line1,const RS_Line &line2,CD_Parameter* p=NULL);
    bool checkCrossType(const RS_Line &linev1,const RS_Line &linev2
                        ,const RS_Line &lineh1,const RS_Line &lineh2,CD_Parameter* p=NULL);

    QList<CD_Feature *> getFeatures();
    cv::Mat findresult;

private:
    void releaseAutoFeature();

    QList<CD_Feature *>  finalfeatures;
    QList<RS_Line> alldetectlines;
    QList<RS_Circle> alldetectcircles;
    CD::FeatureType featuretype;
    CD::AnglePos anglepos;
    CD_Image *image;

    bool alreadygetentities;
    bool alreadygenerateangle;
    bool alreadygeneratecross;
    bool alreadygeneratecircle;

    CD_Parameter par;

};

#endif // CD_TESTFEATURE_H
