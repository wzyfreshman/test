#ifndef CD_ANGLECOMP_H
#define CD_ANGLECOMP_H

#include <QWidget>

#include "cd.h"

class CD_ImageScreen;

class CCD_API CD_AngleComp : public QWidget
{
    Q_OBJECT
public:
    explicit CD_AngleComp(CD_ImageScreen *parent = 0);

    void initRectPosition();
protected:
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);
    void virtual resizeEvent(QResizeEvent*e);

    void calculateDrawLines();
    void calculateControlPoint();

    void drawRect(QPainter &painter);


signals:

public slots:


private:
    double distance_fat;
    int active_control;

    double rotate_angle;
    double distance_width_in;
    double distance_width_out;
    double distance_height_in;
    double distance_height_out;
    double position_x;
    double position_y;

    QVector<QLineF> draw_lines;
    QVector<QPointF> control_points;
    CD_ImageScreen *parent;

};

#endif // CD_ANGLECOMP_H
