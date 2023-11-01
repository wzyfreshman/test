#ifndef LINEFINDER_H
#define LINEFINDER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

#include "cd_parameter.h"

#define PI 3.1415926

using namespace cv;

class CCD_API LineFinder
{

public:
    LineFinder():deltaRho(1),deltaTheta(PI/180),minVote(10),minLength(0.),maxGap(0.){}

    void setAccResolution(double dRho,double dTheta);
    void setMinVote(int minv);
    void setLineLengthAndGap(double length,double gap);
    std::vector<Vec4i> findLines(Mat &binary,CD_Parameter *p =NULL);
    void findLinesMin2(const CvMat* img,CD_Parameter *p =NULL);

    std::vector<Vec2f> returnUnlimitLine();
    void drawDetectedLines(Mat &image,Scalar color = Scalar(255,0,0));
    void icvHoughLinesStandard( const CvMat* img, Mat &dst,float rho, float theta,
                                int threshold/* , CvSeq *lines, int linesMax */);

private:

    Mat img;
    std::vector<Vec4i> lines;
    std::vector<Vec2f> infinitlines;
    double deltaRho;
    double deltaTheta;
    int minVote;
    double minLength;
    double maxGap;

};


#endif // LINEFINDER_H
