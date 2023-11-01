#ifndef CD_FEATURE_H
#define CD_FEATURE_H

#include "cd.h"
#include "cd_parameter.h"
#include "cd_inisettings.h"

class CCD_API CD_Feature
{
public:
    CD_Feature();
    ~CD_Feature(){}

    virtual CD::FeatureType getType() {
        return CD::FeatureNone;
    }
    virtual CD_Feature* clone(){ return NULL;}
    virtual void setParameter(CD_Parameter &p);
    virtual CD_Parameter getParameter();
    virtual void DbugPrint();

    virtual void saveToDisk( CD_IniSettings &set);
    virtual void readFromDisk( CD_IniSettings &set);

    bool isValid();

protected:
    bool valid;
    CD_Parameter par;

};

#endif // CD_FEATURE_H
