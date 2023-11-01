#ifndef CD_CROSSMARK_H
#define CD_CROSSMARK_H

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


class CCD_API CD_CrossMark : public CD_MarkBase
{

public:

    CD_CrossMark(CD_ImageScreen *parent);
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


protected:
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);

    void virtual calculatespecPoint();
    void virtual drawCross();

    void virtual calculatespecPointUpLeft();
    void virtual calculatespecPointUpRight();
    void virtual calculatespecPointDownLeft();
    void virtual calculatespecPointDownRight();

    void virtual drawCrossUpLeft(QPainter &painter);
    void virtual drawCrossDownLeft(QPainter &painter);
    void virtual drawCrossUpRight(QPainter &painter);
    void virtual drawCrossDownRight(QPainter &painter);


    RS_Line lineul;
    RS_Line lineur;
    RS_Line linedl;
    RS_Line linedr;
    RS_Line lineru;
    RS_Line linerd;
    RS_Line linelu;
    RS_Line lineld;


    QMap< QString, QPoint> specpoint;

    int storedistancebetweenrect;
    int storecrosslength;
    int storerectfat;

    int rectfat;// default rect width
    int distancebetweenrect;//十字宽度
    int crosslength;//十字宽度

    QRect storeGeometry;
    QPoint storePoint;

    int ul[4],ur[4],dl[4],dr[4],lu[4],ld[4],ru[4],rd[4];


    QRect uprectL;
    QRect uprectR;

    QRect leftrectU;
    QRect leftrectD;

    QRect rightrectU;
    QRect rightrectD;

    QRect downrectL;
    QRect downrectR;

    CD_Rect upL;
    CD_Rect upR;

    CD_Rect leftU;
    CD_Rect leftD;

    CD_Rect rightU;
    CD_Rect rightD;

    CD_Rect downL;
    CD_Rect downR;

private:
    CD_ImageScreen *parent;
    CD_FeatureCrossData feature;


};

#endif // CD_CROSSMARK_H
