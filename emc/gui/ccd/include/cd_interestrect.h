#ifndef CD_INTERESTRECT_H
#define CD_INTERESTRECT_H

#include "cd_markbase.h"

#include <QList>
#include <QPoint>
#include <QMap>
#include "rs_line.h"
#include <QMouseEvent>
#include <iostream>
#include <QCursor>
#include <cd_template.h>

class CCD_API CD_InterestRect : public CD_MarkBase
{
public:
    CD_InterestRect(CD_ImageScreen *parent);
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);
    void virtual calculatespecPoint();
    void virtual drawInterestRect();
    void virtual initRectPosition();
    void virtual initRectPosition(int w,int h);
    void virtual resizeEvent(QResizeEvent *);
    virtual CD_Template *  doCalculateMatImage();
    CD::MarkType virtual getType() {
       return CD::InterestMark;
    }

    QRect getInterestRect() {
       return interestrect;
    }
    void setFactor(int f){factor = f;}

protected:
    QRect interestrect;
    QMap< QString, QPoint> specpoint;

    int storewidth;
    int storeheight;

    CD_ImageScreen *parent;

    CD_Template image;
    CD_ImageScreen *screen;
    int factor;

};

#endif // CD_INTERESTRECT_H
