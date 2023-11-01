#ifndef CD_FEATURECIRCLEDATA_H
#define CD_FEATURECIRCLEDATA_H

#include "cd_imagescreen.h"
#include "cd_feature.h"
#include "cd_parameter.h"

class CD_CircleComp;

class CCD_API CD_FeatureCircleData : public CD_Feature
{

    friend class CD_FeatureCircle;
    friend class CD_OperateFeature;

public:
    CD_FeatureCircleData();
    CD_FeatureCircleData(double fx,double fy,double fr)
        :x(fx),y(fy),radius(fr){}

    virtual CD_Feature * clone() {
        return new CD_FeatureCircleData(*this);
    }
    virtual CD::FeatureType getType() {
        return CD::FeatureCircle;
    }
    virtual void saveToDisk( CD_IniSettings &set);
    virtual void readFromDisk( CD_IniSettings &set);
    double getX() const {return x;}
    double getY() const {return y;}
    double getRadius() const {return radius;}
private:
    double x,y;
    double radius;
};


class CCD_API CD_FeatureCircle
{
public:
    CD_FeatureCircle(CD_ImageScreen *screen,CD_CircleComp *mark);

    void trigle();

    CD_FeatureCircleData getData() {
        return data;
    }
private:

    void determineParameter();

    CD_FeatureCircleData data;
    CD_ImageScreen *screen;
    CD_CircleComp *mark;


};

#endif // CD_FEATURECIRCLEDATA_H
