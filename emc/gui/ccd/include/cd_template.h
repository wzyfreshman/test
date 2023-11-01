#ifndef CD_TEMPLATE_H
#define CD_TEMPLATE_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ccd_setting.h"

class CCD_API CD_Template
{
public:
    CD_Template(){}
    CD_Template(cv::Mat &_mat);


    CD_Template(CD_Template &cp) {
         cp.image.copyTo(image);
         this->locateinparentx = cp.locateinparentx;
         this->locateinparenty = cp.locateinparenty;

    }

    void operator =(cv::Mat &mat) {
        mat.copyTo(image);

    }
    void operator =(CD_Template &cp) {
        cp.image.copyTo(image);
        this->locateinparentx = cp.locateinparentx;
        this->locateinparenty = cp.locateinparenty;
    }

    int getWidth();
    int getHeight();
    int getLocateInParentX();
    int getLocateInParentY();

    bool isValid(){
        if(image.rows && image.cols)
            return true;
        else
            return false;
    }

    void setLocateInParentX(int _set);
    void setLocateInParentY(int _set);

    cv::Mat* returnAsMat();
private:
    cv::Mat image;
    int locateinparentx;
    int locateinparenty;
};


class CCD_API CD_Image:public CD_Template {

public:
    CD_Image():CD_Template(){}
    CD_Image(cv::Mat& _mat)
        :CD_Template(_mat){}

};


#endif // CD_TEMPLATE_H
