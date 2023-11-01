#ifndef CD_RECT_H
#define CD_RECT_H

#include <QRect>

#include "rs_line.h"

class CCD_API CD_Rect
{

public:

    CD_Rect(){}

    void calculate(double x1,double y1,double width,double height,RS_Vector &point,double r);
    void calculate(QRect &rect,RS_Vector &point,double r);

    void operator = (CD_Rect &_rect);

    RS_Line *Line1();
    RS_Line *Line2();
    RS_Line *Line3();
    RS_Line *Line4();

private:
    RS_Line line1;
    RS_Line line2;
    RS_Line line3;
    RS_Line line4;

};

#endif // CD_RECT_H
