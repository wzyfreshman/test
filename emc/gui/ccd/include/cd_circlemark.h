#ifndef CD_CIRCLEMARK_H
#define CD_CIRCLEMARK_H

#include "cd_markbase.h"
#include "cd_featurecircledata.h"

class CCD_API CD_CircleMark : public CD_MarkBase {

    enum ControlType {
        ControlNone,
        ControlRadius,
        ControlPosition
    };

public:
    CD_CircleMark(CD_ImageScreen *parent);
    CD::MarkType virtual getType() {
       return CD::CircleMark;
    }

    void virtual initRectPosition();
    double getX() const {return currentX;}
    double getY() const {return currentY;}
    double getRadius() const {return currentR;}
    double getSearchFat() const {return search_fat;}

    virtual CD_Feature * getFeature();

protected:
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);

private:
    void updateControlPoint();

    CD_ImageScreen *parent;
    double currentR;
    double currentX;
    double currentY;
    double search_fat;

    double ctl_radius_x;
    double ctl_radius_y;

    ControlType control_type;
    int control_init_x;
    int control_init_y;

private:

    CD_FeatureCircleData feature;

};

#endif // CD_CIRCLEMARK_H
