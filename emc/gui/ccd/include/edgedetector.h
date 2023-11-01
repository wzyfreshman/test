#ifndef EDGEDETECTOR_H
#define EDGEDETECTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define PI 3.1415926

#include "ccd_setting.h"

using namespace cv;
class CCD_API EdgeDetector
{
private:
    Mat img;
    Mat sobel;
    int aperture;
    Mat sobelMagnitude;
    Mat sobelOrientation;
public:
    EdgeDetector():aperture(3){}
    //输入门限
    void setAperture(int a)
    {
        aperture = a;
    }

    //获取门限
    int getAperture() const
    {
        return aperture;
    }

    //计算Sobel结果
    void computeSobel(const Mat &image,Mat sobelX = cv::Mat(),Mat sobelY = cv::Mat())
    {
        Sobel(image,sobelX,CV_32F,1,0,aperture);
        Sobel(image,sobelY,CV_32F,0,1,aperture);
        cartToPolar(sobelX,sobelY,sobelMagnitude,sobelOrientation);
    }

    //获取幅度
    Mat getMagnitude()
    {
        return sobelMagnitude;
    }

    //获取方向
    Mat getOrientation()
    {
        return sobelOrientation;
    }

    Mat getBinaryMap(double Threhhold){

        Mat bgImage;
        threshold(sobelMagnitude,bgImage,Threhhold,255,THRESH_BINARY_INV);
        return bgImage;
    }

    //转化为CV_8U图像
    Mat getSobelImage()
    {
        Mat bgImage;
        double minval,maxval;
        minMaxLoc(sobelMagnitude,&minval,&maxval);
        sobelMagnitude.convertTo(bgImage,CV_8U,255/maxval);
        return bgImage;
    }
    //获取角度
    Mat getSobelOrientationImage()
    {
        Mat bgImage;
        sobelOrientation.convertTo(bgImage,CV_8U,90/PI);
        return bgImage;
    }
};


#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

//#pragma comment(lib, "cv.lib")
//#pragma comment(lib, "cxcore.lib")
//#pragma comment(lib, "highgui.lib")


//int Otsu(IplImage* src);
/*
int main11()
{
    IplImage* img = cvLoadImage("c:\\aSa.jpg",0);
    IplImage* dst = cvCreateImage(cvGetSize(img), 8, 1);
    int threshold = Otsu(img);

    cvThreshold(img, dst, threshold, 255, CV_THRESH_BINARY);


    cvNamedWindow( "img", 1 );
    cvShowImage("img", dst);


    cvWaitKey(-1);

    cvReleaseImage(&img);
    cvReleaseImage(&dst);

    cvDestroyWindow( "dst" );
    return 0;
}*/
/*
int Otsu(IplImage* src)
{
    int height=src->height;
    int width=src->width;
    long size = height * width;

    //histogram
    float histogram[256] = {0};
    for(int m=0; m < height; m++)
    {
        unsigned char* p=(unsigned char*)src->imageData + src->widthStep * m;
        for(int n = 0; n < width; n++)
        {
            histogram[int(*p++)]++;
        }
    }

    int threshold=0;
    long sum0 = 0, sum1 = 0; //存储前景的灰度总和和背景灰度总和
    long cnt0 = 0, cnt1 = 0; //前景的总个数和背景的总个数
    double w0 = 0, w1 = 0; //前景和背景所占整幅图像的比例
    double u0 = 0, u1 = 0;  //前景和背景的平均灰度
    double variance = 0; //最大类间方差
    int i, j;
    double u = 0;
    double maxVariance = 0;
    for(i = 1; i < 256; i++) //一次遍历每个像素
    {
        sum0 = 0;
        sum1 = 0;
        cnt0 = 0;
        cnt1 = 0;
        w0 = 0;
        w1 = 0;
        for(j = 0; j < i; j++)
        {
            cnt0 += histogram[j];
            sum0 += j * histogram[j];
        }

        u0 = (double)sum0 /  cnt0;
        w0 = (double)cnt0 / size;

        for(j = i ; j <= 255; j++)
        {
            cnt1 += histogram[j];
            sum1 += j * histogram[j];
        }

        u1 = (double)sum1 / cnt1;
        w1 = 1 - w0; // (double)cnt1 / size;

        u = u0 * w0 + u1 * w1; //图像的平均灰度
        printf("u = %f\n", u);
        //variance =  w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        variance =  w0 * w1 *  (u0 - u1) * (u0 - u1);
        if(variance > maxVariance)
        {
            maxVariance = variance;
            threshold = i;
        }
    }

    printf("threshold = %d\n", threshold);
    return threshold;
}

*/
#endif // EDGEDETECTOR_H
