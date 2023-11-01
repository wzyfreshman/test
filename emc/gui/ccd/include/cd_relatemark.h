#ifndef CD_RELATEMARK_H
#define CD_RELATEMARK_H

#include "cd_markbase.h"
#include "cd_featurerelatedata.h"

class CCD_API CD_RelateMark : public CD_MarkBase
{

    enum ControlType {
        ControlNone,
        ControlCenter
    };

public:
    CD_RelateMark(CD_ImageScreen *parent);
    CD::MarkType virtual getType() {
       return CD::RelateMark;
    }

    void virtual initRectPosition();
    virtual CD_Feature * getFeature();

    double getX() const {return currentX;}
    double getY() const {return currentY;}
    void setX(double s) {currentX = s;}
    void setY(double s) {currentY = s;}

protected:
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);
private:

    void updateControlPoint();

    CD_ImageScreen *parent;
    double currentX;
    double currentY;

    double ctl_center_x;
    double ctl_center_y;

    ControlType control_type;
    int control_init_x;
    int control_init_y;

    CD_FeatureRelateData feature;

};

#endif // CD_RELATEMARK_H
