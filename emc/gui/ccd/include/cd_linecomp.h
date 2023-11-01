#ifndef CD_LINECOMP_H
#define CD_LINECOMP_H

#include <QWidget>
#include "rs_line.h"

class CD_ImageScreen;

class CCD_API CD_LineComp : public QWidget,public RS_Line
{
    Q_OBJECT
public:
    explicit CD_LineComp(CD_ImageScreen *p);

    void init();
protected:
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);
    void virtual resizeEvent(QResizeEvent*e);

    void calculateDrawLines();
public slots:


private:
    double distance_fat;
    int active_control;
    QVector<QLineF> draw_lines;
    QVector<QPointF> control_points;
    CD_ImageScreen *parent;

};

#endif // CD_LINECOMP_H
