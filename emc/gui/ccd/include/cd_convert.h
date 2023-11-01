
#ifndef MATTOQIMAGE_H
#define MATTOQIMAGE_H

#include <QtGui>
#include <QDebug>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "ccd_setting.h"

using namespace cv;
class CCD_API CD_Process {
public:
    static QImage matToQImage(const Mat&);
};


#endif
