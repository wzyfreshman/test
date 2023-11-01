#ifndef CD_ENRASERCOMP_H
#define CD_ENRASERCOMP_H

#include "cd_markbase.h"

class CCD_API CD_EnraserComp : public CD_MarkBase
{

public:
    CD_EnraserComp(CD_ImageScreen *parent);

    void initRectPosition();
    void setEnraseSize(int);
    int getEnraseSize();
    void setOperateMat(cv::Mat *bit,CD_ImageScreen*scr,double fx,double fy);

protected:
    void virtual paintEvent(QPaintEvent *);
    void virtual mousePressEvent(QMouseEvent *e);
    void virtual mouseReleaseEvent(QMouseEvent *e);
    void virtual mouseMoveEvent(QMouseEvent *e);

private:
    QPoint enrase_pos;
    int enrase_size;
    cv::Mat *operate_bit;
    CD_ImageScreen *dis_scr;
    double factorx,factory;
    bool active_enrase;

};

#endif // CD_ENRASERCOMP_H
