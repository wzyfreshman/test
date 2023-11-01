#ifndef CD_IMAGEPROC_H
#define CD_IMAGEPROC_H

#include <QList>
#include <QPoint>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "rs_line.h"
#include "cd_template.h"
#include "rs_circle.h"

class CD_Parameter;

class CCD_API CD_CirclePar {
public:
    double x;
    double y;
    double r;
    int indexofseq;
    int startofseq;
    int endofseq;
};

class CCD_API CD_ImageProc
{
public:
    CD_ImageProc();
    virtual void findAllLinescontinue(cv::Mat &image,
                              QList<RS_Line> &,
                              CD_Parameter* p=NULL);

    virtual void canny(cv::Mat &image,cv::Mat &out);

    virtual void findAllLines(QList< QList<RS_Vector> > points,
                              QList<RS_Line> &,
                              CD_Parameter* p=NULL);

    virtual void findAllLines(cv::Mat &image,
                              QList<RS_Line> &,
                              CD_Parameter* p=NULL);

    virtual void findAllCircles(cv::Mat &image,
                              QList<RS_Circle> &,
                              CD_Parameter* p=NULL);
    virtual void  findAllLinesdilate(cv::Mat &image,
                                    QList<RS_Line> &alldetectlines,
                                    CD_Parameter *p);
    virtual void detectAllPoint(cv::Mat &image,CD_Parameter *p);
    virtual void detectAllPoint(cv::Mat &image,QList< QList<QPoint> >& points,CD_Parameter *p = NULL);
    virtual double findmatch(cv::Mat &s,cv::Mat &t,CD_Template &d);
    virtual double findmatch(cv::Mat &s,CD_Template &t,CD_Template &d);
    virtual int getMostlyValue(const cv::Mat& image);
    virtual cv::Mat getHistogram(const cv::Mat& image);
    virtual cv::Mat getsubMat(cv::Mat &org,const cv::Point &p1,const cv::Point &p2);
    virtual cv::Mat getsubMat(cv::Mat &org,int x,int y,int w,int h);

    void ThresholdOtsu(cv::Mat &src, cv::Mat &dst);
    void thinImage( cv::Mat &src,int maxIterations = -1 );
    void scanPointImage(cv::Mat &src,QList< QList<QPoint> > &points);
    RS_Line calculateLine(const QList<QPoint> &p);
    RS_Line calculateLine(const QList<RS_Vector> &p);
    QList<QPoint> getLinesFromPoints(const QList<QPoint> &points,int length,int fat);
    QList<QPoint> getLinesFromPoints(const QList<RS_Vector> &points,int length,int fat);
    void mergeLines(const QList<RS_Line> &lines,QList<RS_Line> &dist,int mergefat);
    void testFun();
    void calculateArc(const QList<QPoint> &p,double reslt[4]);
    void calculateArc(const QList<QPointF> &p,double reslt[4]);

    void affineImg(cv::Mat &src,cv::Mat &dst,double thrx,double thry,double offx,double offy);

    double calculateSimHu1(double *h1,double *h2);
    double calculateSimHu2(double *h1,double *h2);

    void smoothCurves( std::vector<cv::Point> points, \
                      std::vector<cv::Point> &ret);

protected:
    QList< QList<QPoint> > points;
    int canny_thresholdl,canny_thresholdh;
private:
    int histSize[1];
    float hrangee[2];
    const float* ranges[1];
    int channels[1];

};

#endif // CD_IMAGEPROC_H
