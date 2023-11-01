#ifndef CD_FEATURECROSS_H
#define CD_FEATURECROSS_H

#include "cd_imagescreen.h"
#include "cd_feature.h"
#include "rs_line.h"

class CD_CrossComp;

class CCD_API CD_FeatureCrossData: public CD_Feature {

    friend class CD_FeatureCross;
    friend class CD_OperateFeature;
public:
    CD_FeatureCrossData():CD_Feature() {

        valid =false;
        crosswidthH = 0;
        distanceH = 0;
        angleH = 0;

        crosswidthV = 0;
        distanceV = 0;
        angleV = 0;

    }
    void init(double verR1 ,double verR2,
    double horR1,double horR2,
    double verThr1,double verThr2,
    double horThr1,double horThr2);

    CD_FeatureCrossData(RS_Line &vl1,RS_Line &vl2,RS_Line &hl1,
                        RS_Line &hl2);

    virtual CD::FeatureType getType() {
        return CD::FeatureCross;
    }
    virtual CD_Feature * clone() {
        return new CD_FeatureCrossData(*this);
    }

    virtual void saveToDisk( CD_IniSettings &set);
    virtual void readFromDisk( CD_IniSettings &set);

    double getFeatureDistanceH(){return distanceH;}
    double getFeatureAngleH(){return angleH;}
    double getCrossWidthH() {return crosswidthH;}

    double getFeatureDistanceV(){return distanceV;}
    double getFeatureAngleV(){return angleV;}
    double getCrossWidthV() {return crosswidthV;}

    double getVerR1() {return verR1;}
    double getVerR2() {return verR2;}
    double getHorR1() {return horR1;}
    double getHorR2() {return horR2;}

    double getVerThr1() {return verThr1;}
    double getVerThr2() {return verThr2;}
    double getHorThr1() {return horThr1;}
    double getHorThr2() {return horThr2;}

    void debugPrint();
    virtual void DbugPrint() {


        qDebug()<<"verR1"<<verR1<<"verThr1"<<verThr1
               <<"verR2"<<verR2<<"verThr2"<<verThr2
               <<"horR1"<<horR1<<"horThr1"<<horThr1
               <<"horR2"<<horR2<<"horThr2"<<horThr2;
        CD_Feature::DbugPrint();

    }

    //double getCrossWidth();
    RS_Vector getCrossPoint();

private:

    double crosswidthH;
    double distanceH;
    double angleH;

    double crosswidthV;
    double distanceV;
    double angleV;

    double verR1,verR2;
    double horR1,horR2;
    double verThr1,verThr2;
    double horThr1,horThr2;


};

class CCD_API CD_FeatureCross
{
public:
    CD_FeatureCross(CD_ImageScreen *screen,CD_CrossComp *mark);

    void trigle();
    CD_FeatureCrossData getData() {
        return data;
    }
private:
    void determineParameter();
    CD_FeatureCrossData data;
    CD_ImageScreen *screen;
    CD_CrossComp *mark;


};

#endif // CD_FEATURECROSS_H
