#ifndef CD_TESTSOLVE_H
#define CD_TESTSOLVE_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QList>
#include <QPoint>

#include "rs_circle.h"

class CD_ImageScreen;
class CD_CircleComp;

class CD_TestSolve
{
public:
    CD_TestSolve();


    void searchCircle(CD_ImageScreen * sc,cv::Mat &mat,cv::Mat &label,RS_Circle &circle);

    void searchCross(CD_ImageScreen * sc,cv::Mat &mat,cv::Mat &label, double *parameters,bool symmetric);
    void calculateCrossCenter(CD_ImageScreen * sc,cv::Mat &mat, double *parameters,int *flag, \
                              double angle,double * imagex,double *imagey,bool symmetric);

    void findRoads(cv::Mat &mat,QList<QPoint> &road);
    void icvprLabelColor(const cv::Mat& _labelImg, cv::Mat& _colorLabelImg);
    cv::Scalar icvprGetRandomColor();
    void icvprCcaBySeedFill(const cv::Mat& _binImg, cv::Mat& _lableImg,\
                            std::vector<std::vector<std::pair<int ,int> > > &roads);

    void tranMat(cv::Mat & mat,unsigned char *d);
    void tranMat(unsigned char *s,int w,int h,cv::Mat & mat);





    //***计算点的曲面拟合系数****
    void calculateCoefficient(const cv::Mat &Mat3_3,double *K);
    void calculateCoefficient(unsigned char *Mat3_3,int wid_step,double *K);
    //***计算亚像素位置***
    void calculateSubpiex(double *K,double *x,double *y);
    //***计算梯度方向****
    void calculateOri(double *K,double *x,double *y);
    //***计算图像的梯度(sobel算子)
    void fastSobel(unsigned char *in, int width, int height,unsigned char *edg);
    //***计算图像金字塔
    unsigned char * calculateImgPyramid(unsigned char *orig,int w,int h);


    void matchTemplate( cv::Mat &Mat, cv::Mat &tmp);



private:
    int *pool1;
    int *pool2;

};

#endif // CD_TESTSOLVE_H
