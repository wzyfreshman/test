#ifndef CD_IMAGESCREEN_H
#define CD_IMAGESCREEN_H

#include <QLabel>
#include <QHash>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QMouseEvent>

#include "cd_template.h"
#include "rs_line.h"
#include "rs_circle.h"

class CD_Feature;

class CCD_API CD_ImageScreen : public QWidget//QLabel
{
public:
    CD_ImageScreen(QWidget *parent);

    virtual void showMatImage(cv::Mat &mat);
    virtual void showMatImage(const QString &filename);
    virtual void saveMatImage(const QString &filename);
    virtual int getWidth();
    virtual int getHeight();
    virtual void catchOneImage(CD_ImageScreen *catched);

    virtual void drawFeature(CD_Feature *feature);
    virtual void drawLine(RS_Line &line);
    virtual void drawPoints(const RS_Vector &p);
    virtual void drawPoints(double x1,double y1);
    virtual void drawLine(double x1,double y1,double x2,double y2);
    virtual void drawLines(QList<RS_Line> &);
    virtual void drawCircle(RS_Circle &line);
    virtual void drawCircles(QList<RS_Circle> &);
    virtual void drawLineWithNum(RS_Line &line,int);
    virtual void drawLinesWithNum(QList<RS_Line> &,QList<int> &);

    virtual void drawEntity(RS_Entity* e);
    virtual void drawEntitys(QList<RS_Entity*> &es);
    virtual void drawRect(const QRectF &);
    virtual void drawString(const QStringList &s,const QList<QColor>&c);
    virtual void drawString(int index,const QColor &,const QString&);
    virtual void drawMidString(const QString&,const QColor &);

    virtual void resetPoints();
    virtual void resetLines();
    virtual void resetCircles();
    virtual void resetEntity();
    virtual void resetFeature();
    virtual void resetRects();
    virtual void copyTo(CD_ImageScreen *dst);

    CD_Feature *getDisplayFrature();

    double getFactorW() {
       return factorx;
    }
    double getFactorH() {
       return factory;
    }

    cv::Mat *getCurrentImageMat() {
        return orgdata.returnAsMat();
    }
    CD_Image *getCurrentImage() {
        return &orgdata;
    }
    QList<RS_Line> lines;
    QList<RS_Circle> circles;
    QList<QPointF> points;
protected:
    virtual void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    virtual void drawLine(QPainter &painter,double rho,double theta);
    virtual void drawLine(QPainter &painter,int x1,int y1,int x2,int y2);
    virtual void drawNum(QPainter &painter,int x1,int y1,int n);
    virtual void drawCircle(QPainter &painter,double x1,double y1,double r);


private:
    double factorx;
    double factory;

    CD_Feature *feature;
    CD_Image orgdata;
    QImage img;
    //QList<RS_Line> lines;
    QList<RS_Entity*> entities;
    QList<QRectF> rects;

    QStringList stringdisplay;
    QList<QColor> stringcolors;
    QString stringcenter;
    QColor colorcenter;

    QList<RS_Line> lines_with_num;
    QList<int> lines_nums;
public:
    QColor feature_color_angle;
    QColor feature_color_cross;
    QColor feature_color_circle;

};

#endif // CD_IMAGESCREEN_H
