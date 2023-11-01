#ifndef CD_CrossComp_H
#define CD_CrossComp_H

#include "cd.h"
#include "cd_rect.h"
#include "rs_line.h"
#include "cd_imagescreen.h"
#include "cd_markbase.h"
#include "cd_featurecross.h"

#include <QList>
#include <QPoint>
#include <QMap>
#include <QMouseEvent>
#include <iostream>
#include <QCursor>

class CCD_API CD_CrossComp : public CD_MarkBase {


public:

    CD_CrossComp(CD_ImageScreen *parent);
    CD::MarkType virtual getType() {
       return CD::CrossMark;
    }

    RS_Line getLineVertical1();
    RS_Line getLineHorizontal1();
    RS_Line getLineVertical2();
    RS_Line getLineHorizontal2();

    int getCrossWidth();
    int getCrossFat();
    virtual CD_Feature * getFeature();
    void virtual initRectPosition();
    void virtual initRectPosition(int w,int h);

    double getDistanceFat() const ;
    double getVerDisInside() const ;
    double getVerDisOutside() const ;
    double getVerDisWidth() const ;
    double getHorDisInside() const ;
    double getHorDisOutside() const ;
    double getHorDisWidth() const ;
    double getRotation() const ;
    double getPositionX() const;
    double getPositionY() const;
    int isPointInside(int x,int y);

protected:
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);

    void drawCross(QPainter &painter);
    void calculateCrossPoints();

    double rotate_angle;
    double postion_x;
    double position_y;

    double all_distance_fat;
    double hor_distance_inside;
    double hor_distance_outside;
    double hor_distance_width;
    double ver_distance_inside;
    double ver_distance_outside;
    double ver_distance_width;


private:

    int active_control;
    QVector<QLineF> draw_lines;
    QVector<QLineF> draw_lines_mid;
    QVector<QPointF> control_points;
    bool is_symmetric;

    CD_ImageScreen *parent;
    CD_FeatureCrossData feature;

};


#endif // CD_CrossComp_H
