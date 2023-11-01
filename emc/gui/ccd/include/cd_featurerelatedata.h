#ifndef CD_FEATURERELATEDATA_H
#define CD_FEATURERELATEDATA_H

#include "cd_imagescreen.h"
#include "cd_feature.h"
#include "cd_parameter.h"


class CD_RelateMark;

class CCD_API CD_FeatureRelateData : public CD_Feature
{

    friend class CD_FeatureRelate;
    friend class CD_OperateFeature;

public:
    CD_FeatureRelateData();
    CD_FeatureRelateData(double ofx,double ofy)
        :offsetx(ofx),offsety(ofy) {}

    virtual CD_Feature * clone() {
        return new CD_FeatureRelateData(*this);
    }
    virtual CD::FeatureType getType() {
        return CD::FeatureRelate;
    }
    virtual void saveToDisk( CD_IniSettings &set);
    virtual void readFromDisk( CD_IniSettings &set);
    double getOffsetX() const {return offsetx;}
    double getOffsetY() const {return offsety;}

private:
    double offsetx,offsety;

};


class CCD_API CD_FeatureRelate
{
public:
    CD_FeatureRelate(CD_ImageScreen *screen,CD_RelateMark *mark);

    void trigle();

    CD_FeatureRelateData getData() const {
        return data;
    }

private:

    void determineParameter();

    CD_FeatureRelateData data;
    CD_ImageScreen *screen;
    CD_RelateMark *mark;

};



#endif // CD_FEATURERELATEDATA_H
