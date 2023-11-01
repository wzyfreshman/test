#ifndef CD_ANGLEMARK_H
#define CD_ANGLEMARK_H

#include <QPoint>
#include <QMap>


#include "cd_markbase.h"
#include "cd_crossmark.h"
#include "cd_featureangledata.h"
#include "cd.h"
#include "rs_line.h"


class CCD_API CD_AngleMark : public CD_CrossMark
{

public:


public:
    CD_AngleMark(CD_ImageScreen *parent,CD::AnglePos type);
    CD::MarkType virtual getType() {
       return CD::AngleMark;
    }

    virtual CD_Feature *getFeature();
    void virtual initRectPosition();
    void setAnglePos(CD::AnglePos type);
protected:
    void virtual paintEvent(QPaintEvent *);

private:
    CD::AnglePos angletype;
    CD_ImageScreen *parent;
    CD_FeatureAngleData feature;
    friend class CD_OperateFeature;

};

#endif // CD_ANGLEMARK_H
