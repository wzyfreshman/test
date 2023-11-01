#ifndef CD_CIRCLECOMP_H
#define CD_CIRCLECOMP_H

#include <QWidget>

#include "rs_circle.h"
#include "cd_featurecircledata.h"

class CD_ImageScreen;
class CD_Feature;

class CCD_API CD_CircleComp : public QWidget,public RS_Circle
{
    Q_OBJECT
public:
    explicit CD_CircleComp(CD_ImageScreen *parent = 0);

    void init();
    void setFat(int fat);
    void initRectPosition();

    double getFatImg();
    double getRadiusImg();
    CD_Feature* getFeature();
    int isPointInside(int touch_x, int touch_y);

protected:
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);
    void virtual resizeEvent(QResizeEvent*e);

    void calculateControlPoint();
signals:

public slots:
private:
    double distance_fat;
    double start_angle;
    double span_angle;
    int active_control;
    QVector<QLineF> draw_lines;
    QVector<QPointF> control_points;
    CD_ImageScreen *parent;
    CD_FeatureCircleData feature;

};

#endif // CD_CIRCLECOMP_H
