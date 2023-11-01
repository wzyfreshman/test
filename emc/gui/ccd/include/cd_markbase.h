#ifndef CD_MARKBASE_H
#define CD_MARKBASE_H

#include <QWidget>

#include "cd.h"
#include "cd_imagescreen.h"

class CCD_API CD_MarkBase : public QWidget
{
    Q_OBJECT

public:
    explicit CD_MarkBase(CD_ImageScreen *parent = 0);

    CD::MarkType virtual getType() {
       return CD::BaseMark;
    }


    void virtual initRectPosition(){}
protected:
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);
    void virtual resizeEvent(QResizeEvent *);
    void virtual drawCrossCenter(QPainter &painter);

    double distanceToPoint(double x1,double y1,double x2,double y2);
    QPoint center;
    double rotate;

    int storeposx;
    int storeposy;
    QPoint storepos;

    QString settingpointname;
private:
    bool centeron;
    double storerotate;
    QPoint dragpoint;
    double rotateangle; //for calculate


signals:
    
public slots:
    
};

#endif // CD_MARKBASE_H
