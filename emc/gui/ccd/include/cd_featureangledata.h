#ifndef CD_FEATUREANGLEDATA_H
#define CD_FEATUREANGLEDATA_H

#include "cd_imagescreen.h"
#include "cd_feature.h"
#include "cd_parameter.h"

class CD_AngleMark;

class CCD_API CD_FeatureAngleData : public CD_Feature
{

    friend class CD_FeatureAngle;
    friend class CD_OperateFeature;
public:
    CD_FeatureAngleData()
        :CD_Feature() {

         rh = 0.0;
         thh = 0.0;
         rv = 0.0;
         thv = 0.0;

    }

    CD_FeatureAngleData(double r1,double th1,double r2,double th2)
        :CD_Feature() {

         rh = r1;
         thh = th1;
         rv = r2;
         thv = th2;

    }

    virtual CD_Feature * clone() {
        return new CD_FeatureAngleData(*this);
    }


    virtual void saveToDisk( CD_IniSettings &set);
    virtual void readFromDisk( CD_IniSettings &set);

    RS_Vector getCrossPoint();

    double getR1() const {
        return rh;
    }
    double getR2() const {
        return rv;
    }
    double getThe1() const {
        return thh;
    }
    double getThe2() const {
        return thv;
    }

    virtual CD::FeatureType getType() {
        return CD::FeatureAngle;
    }
    virtual void DbugPrint() {

        qDebug()<<"rh"<<rh<<"thh"<<thh
               <<"rv"<<rv<<"thv"<<thv;
        CD_Feature::DbugPrint();

    }

private:
    double rh,thh;
    double rv,thv;   
    CD::AnglePos pos;

};

class CCD_API CD_FeatureAngle
{
public:
    CD_FeatureAngle(CD_ImageScreen *screen,CD_AngleMark *mark);

    void trigle(CD::AnglePos pos = CD::TopLeft);

    CD_FeatureAngleData getData() {
        return data;
    }
private:

    void determineParameter();

    CD_FeatureAngleData data;
    CD_ImageScreen *screen;
    CD_AngleMark *mark;


};

#endif // CD_FEATUREANGLEDATA_H
