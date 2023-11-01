#ifndef CD_ARCCOMP_H
#define CD_ARCCOMP_H

#include <QWidget>
#include "rs_circle.h"

class CD_ImageScreen;

class CCD_API CD_ArcComp : public QWidget,public RS_Circle
{
    Q_OBJECT
public:
    explicit CD_ArcComp(CD_ImageScreen *parent = 0);
    void init();
protected:
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);
    void virtual resizeEvent(QResizeEvent*e);

    void calculateDrawLines();
signals:

public slots:

private:
    double start_angle;
    double end_angle;
    double distance_fat;
    int active_control;
    QVector<QLineF> draw_lines;
    QVector<QPointF> control_points;
    CD_ImageScreen *parent;

};

#endif // CD_ARCCOMP_H
