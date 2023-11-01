#include "cd_testsolve.h"
#include "cd_imageproc.h"

#include <list>
#include <utility>
#include <list>
#include <vector>
#include <map>
#include <stack>

#include <QtDebug>
#include <QTime>
#include <QRectF>
#include <QApplication>

#include <rs_debug.h>

#include "cd_imagescreen.h"
#include "cd_circlecomp.h"


//#define  DEBUG_DEBUG

using std::pair;
using std::make_pair;

static QList<int> test_contain;


CD_TestSolve::CD_TestSolve() {

    for(int i=0;i<500*500;i++) {
        test_contain.append(i);
    }

    pool1 = new int[5000*5000];
    pool2 = new int[5000*5000];;

}

#define TWOPOINTTOLINE(x1,y1,x2,y2) {a = (y2)-(y1); \
                                     b = (x1)-(x2); \
                          c = (y1)*((x2)-(x1)) - (x1)*((y2)-(y1));}

RS_Line calculateLine( std::list<std::pair<int ,int> > &p) {


    std::list< std::pair<int ,int> >::iterator it;
    RS_Line line;
    if(p.size()<2) return line;
    int size = p.size();

    bool needrotate = false;
    double x[size];
    double y[size];
    double tranx[size];
    double trany[size];

    int i = 0;
    for( it = p.begin(); it != p.end();  it++) {
        x[i] = it->first;
        y[i] = it->second;
        i++;
    }
    //for(int i = 0;i<size;i++) {
    //    x[i] = p.at(i).x;
    //    y[i] = p.at(i).y;
    //}
    RS_Vector tv1(x[0],y[0]);
    RS_Vector tv2(x[size-1],y[size-1]);
    double rotate = 0;
    double tmpangle = RS_Math::correctAngle( tv1.angleTo(tv2) );

    if(RS_Math::cmpDouble(tmpangle,M_PI_2,M_PI*10/180)
            || RS_Math::cmpDouble(tmpangle,M_PI_2*3,M_PI*10/180)) {
       rotate = -M_PI/4;
       needrotate = true;

    }

    for(int i = 0;i<size;i++) {
        tranx[i] = x[i]*cos(rotate) - y[i]*sin(rotate);
        trany[i] = x[i]*sin(rotate) + y[i]*cos(rotate);
    }

    float a,b,c;
    float t1=0, t2=0, t3=0, t4=0;
    for(int i=0; i<size; ++i) {
        t1 += tranx[i]*tranx[i];
        t2 += tranx[i];
        t3 += tranx[i]*trany[i];
        t4 += trany[i];
    }

    b = -1;
    a = (t3*size  - t2*t4) / (t1*size  - t2*t2);
    c = (t1*t4 - t2*t3) / (t1*size  - t2*t2);

    //The line is ax-y+c = 0;
    double startx,starty,endx,endy;
    double tstartx,tstarty,tendx,tendy;
    startx = tranx[0];
    starty = a*tranx[0] + c;
    endx = tranx[size-1];
    endy = a*tranx[size-1] + c;
    tstartx = startx;
    tstarty = starty;
    tendx = endx;
    tendy = endy;

    if(needrotate == true) {

        rotate = -rotate;
        tstartx = startx*cos(rotate) - starty*sin(rotate);
        tstarty = startx*sin(rotate) + starty*cos(rotate);
        tendx = endx*cos(rotate) - endy*sin(rotate);
        tendy = endx*sin(rotate) + endy*cos(rotate);
        TWOPOINTTOLINE(tstartx,tstarty,tendx,tendy)
    }

    //RS_Vector v(0,0);
    //double r = fabs( RS_Math::distanceFromPointToLine(v,a,b,c));
    //line.setR(r);

    RS_Vector v1(tstartx, tstarty);
    RS_Vector v2(tendx, tendy);
    //line.setThr(ang1);
    line.setStartpoint(v1);
    line.setEndpoint(v2);
    line.calculateRandThr();
    //line.setPoints(p);
    return line;

}

static int index_of_rect = 0;

inline static bool getSubArea(cv::Mat &mat,double x1, double y1, double angle, \
                              int wid, int hei,QList<QPoint> &points) {

    //QMap<int,QList<QPoint>* > pool;

    QMap<int,int> pool;
    QList<QList<QPoint> >points_temp;
    QList<QList<int> >points_type;

    y1 = -y1;
    int item;
    int imagex ,imagey;
    int get = 0;
    for(int w = 0;w<wid;w++) {

        QList<QPoint> temp;
        QList<int> type;
        for(int h = 0; h<hei; h++) {

            imagex = w*cos(angle) - h*sin(angle) + x1;
            imagey = -(w*sin(angle) + h*cos(angle) + y1);
            item= *mat.ptr<int>(imagey,imagex);
            if( item<2 ) continue;
            if( !pool.contains(item) ) {
                pool[item] = 0;
            }
            get = pool[item];
            get ++ ;
            pool[item] = get;
            temp.append(QPoint(imagex,imagey));
            type.append(item);
        }


        if( !temp.isEmpty())
            points_temp.append(temp);
        if( !type.isEmpty())
            points_type.append(type);

    }

    for(int i=0;i<points_temp.size();i++) {

        QList<QPoint> item = points_temp[i];
        QList<int> item_t = points_type[i];
        if(item.isEmpty()) continue;

        int max_pri = pool[  item_t[0] ];
        int mark = 0;

        for(int j=1;j<item.size();j++) {
            int pri = pool[ item_t[j] ];
            if( pri > max_pri)
                mark = j;
        }
        points.append(item[mark]);
    }

}

/**
  |-----------------------w-------------------------|
  h                                                              h
  |-----------------------w-------------------------|
  (x1,y1)
  angle is the rotate
  ---------------------------------------------------
*/

inline static bool getSubArea(CD_ImageScreen * sc,cv::Mat &mat,double x1, double y1, double *angle, \
                              int wid, int hei) {


    y1 = -y1;
    uchar *item;
    int imagex ,imagey;
    int exist_count = 0;
    bool line_exist;
    bool condition1 = false;
    bool condition2 = false;
#define sin(angle) (angle[0])
#define cos(angle) (angle[1])


#ifdef DEBUG_DEBUG
    //*****debug*****
    double xx[4],yy[4];
    xx[0] = 0*cos(angle) - 0*sin(angle) + x1;
    yy[0] = -(0*sin(angle) + 0*cos(angle) + y1);
    xx[1] = wid*cos(angle) - 0*sin(angle) + x1;
    yy[1] = -(wid*sin(angle) + 0*cos(angle) + y1);
    xx[2] = wid*cos(angle) - hei*sin(angle) + x1;
    yy[2] = -(wid*sin(angle) + hei*cos(angle) + y1);
    xx[3] = 0*cos(angle) - hei*sin(angle) + x1;
    yy[3] = -(0*sin(angle) + hei*cos(angle) + y1);

    RS_Line line_tmp(RS_Vector(xx[0],yy[0]),RS_Vector(xx[1],yy[1]));
    RS_Line line_tmp1(RS_Vector(xx[1],yy[1]),RS_Vector(xx[2],yy[2]));
    RS_Line line_tmp2(RS_Vector(xx[2],yy[2]),RS_Vector(xx[3],yy[3]));
    RS_Line line_tmp3(RS_Vector(xx[3],yy[3]),RS_Vector(xx[0],yy[0]));

    sc->drawLine(line_tmp);
    sc->drawLine(line_tmp1);
    sc->drawLine(line_tmp2);
    sc->drawLine(line_tmp3);

#endif

    //****end debug


    int h;
    int hei_t = hei;
    for(int w = 0;w<wid;w++) {

        line_exist = false;
        for(h = 0; h<hei_t; h++) {
            imagex = w*cos(angle) - h*sin(angle) + x1;
            imagey = -(w*sin(angle) + h*cos(angle) + y1);
            item= mat.ptr<uchar>(imagey,imagex);
            if( *item == 0xff )
               line_exist = true;
        }
        if(line_exist)
            exist_count ++;

    }

#undef sin(angle)
#undef cos(angle)

    if( exist_count > (wid * 0.9) ) {

        //qDebug()<<"exist_count = "<<exist_count<<wid;

        condition1 = true;
        //sim_line = calculateLine(hilight);
        //double diff = fabs(RS_Math::angleDiff(angle, sim_line.getAngle1()));
        //double angle_rect = (RS_Math::correctAngle(atan2(hei,wid)));
        //if( diff <= angle_rect ) {
            condition2 = true;
       // }

    }

    if(condition1 && condition2)
        return true;
    else
        return false;

}


void CD_TestSolve::searchCross(CD_ImageScreen * sc,cv::Mat &mat,cv::Mat &label,\
                               double *parameters,bool symmetric) {

        double global_angle = 0.0;
        int imgw = mat.cols;
        int imgh = mat.rows;
        int search_limit = 0;
        int search_width = 0;
        int search_height = 0;
        double max_size = 0;
        double all_fat = parameters[0];
        double hor_inside = parameters[1];
        double hor_outside = parameters[2];
        double hor_with = parameters[3];
        double ver_inside = parameters[4];
        double ver_outside = parameters[5];
        double ver_with = parameters[6];
        double rotate = parameters[7];
        double rotate_step = (1*M_PI)/180.0;
        std::vector<std::vector<double> > store_imgx;
        std::vector<std::vector<double> > store_imgy;
        std::vector<std::vector<int> > store_flag;
        std::vector<int> store_flag_size;

        if( symmetric )
            max_size = hor_outside;
        else
            max_size = hor_outside>ver_outside ? hor_outside:ver_outside;

        max_size *= 2;
        max_size *= 1;
        if ( max_size >= imgw || max_size >= imgh) return;

        search_width = imgw - max_size;
        search_height = imgh - max_size;

        search_limit = max_size/2;

        if( imgw <= search_limit || imgh <= search_limit ) return;


        qDebug()<<"max_size"<<max_size<<"imgsize"<<imgw<<imgh<<"search_size"<<search_width<<search_height;

        store_imgx.clear();
        store_imgy.clear();
        store_flag.clear();
        store_flag_size.clear();

        for( double local_angle = rotate; local_angle<=rotate;  local_angle+=rotate_step ) {

            int exist_flag[8];
            double x1,y1,imagex[8],imagey[8];
            bool exist = false;
            bool find_one = false;
            int init_px,init_py;

            memset(exist_flag,0x00,sizeof(exist_flag));

            //global_angle = ((double)local_angle*M_PI)/180.0;
            global_angle = local_angle;

            find_one = false;

            //find the first point
            x1 =  -ver_with;
            y1 =   ver_outside ;
            init_px = x1*cos(global_angle) - y1*sin(global_angle) + search_limit;
            init_py = -(x1*sin(global_angle) + y1*cos(global_angle) - search_limit);

            //now make the top-left point as a reference and calculate another points
            x1 =  -ver_with - all_fat ;
            y1 =   ver_outside ;
            imagex[0] = x1*cos(global_angle) - y1*sin(global_angle) + search_limit;
            imagey[0] = -(x1*sin(global_angle) + y1*cos(global_angle) - search_limit);

            x1 =   ver_with  -  all_fat ;
            y1 =   ver_outside ;
            imagex[1] = x1*cos(global_angle) - y1*sin(global_angle) + search_limit;
            imagey[1] = -(x1*sin(global_angle) + y1*cos(global_angle) - search_limit);

            x1 =   -ver_with  -  all_fat ;
            y1 =   -ver_inside ;
            imagex[2] = x1*cos(global_angle) - y1*sin(global_angle) + search_limit;
            imagey[2] = -(x1*sin(global_angle) + y1*cos(global_angle) - search_limit);

            x1 =   ver_with  -  all_fat ;
            y1 =   -ver_inside ;
            imagex[3] = x1*cos(global_angle) - y1*sin(global_angle) + search_limit;
            imagey[3] = -(x1*sin(global_angle) + y1*cos(global_angle) - search_limit);

            x1 =  -hor_outside;
            y1 =   hor_with  -  all_fat ;
            imagex[4] = x1*cos(global_angle) - y1*sin(global_angle) + search_limit;
            imagey[4] = -(x1*sin(global_angle) + y1*cos(global_angle) - search_limit);

            x1 =  hor_inside;
            y1 =  hor_with  -  all_fat ;
            imagex[5] = x1*cos(global_angle) - y1*sin(global_angle) + search_limit;
            imagey[5] = -(x1*sin(global_angle) + y1*cos(global_angle) - search_limit);

            x1 =   -hor_outside;
            y1 =   -hor_with  -  all_fat ;
            imagex[6] = x1*cos(global_angle) - y1*sin(global_angle) + search_limit;
            imagey[6] = -(x1*sin(global_angle) + y1*cos(global_angle) - search_limit);

            x1 =   hor_inside;
            y1 =   -hor_with  -  all_fat ;
            imagex[7] = x1*cos(global_angle) - y1*sin(global_angle) + search_limit;
            imagey[7] = -(x1*sin(global_angle) + y1*cos(global_angle) - search_limit);

            double search_angle_ver[2];
            double search_angle_hor[2];
            search_angle_ver[0] = sin(global_angle-M_PI_2);
            search_angle_ver[1] = cos(global_angle-M_PI_2);
            search_angle_hor[0] = sin(global_angle);
            search_angle_hor[1] = cos(global_angle);

            //now we start search
            for( int i = 0; i < (search_width); i++ ) {
                for( int j = 0; j < (search_height); j++ ) {

                    if( *mat.ptr<uchar>( init_py+j,init_px+i ) < 255 ) continue; //uselesse points
                    sc->resetLines();

                    //********************vertical**************************
                    index_of_rect = 0;
                    exist = getSubArea(sc,mat,imagex[0] + i,imagey[0] +j,search_angle_ver,ver_outside-ver_inside,2*all_fat);
                    if(exist) exist_flag[0] = 1;
                    index_of_rect ++;
                    exist = getSubArea(sc,mat,imagex[1] + i,imagey[1] +j,search_angle_ver,ver_outside-ver_inside,2*all_fat);
                    if(exist) exist_flag[1] = 1;
                    index_of_rect ++;
                    exist = getSubArea(sc,mat,imagex[2] + i,imagey[2] +j,search_angle_ver,ver_outside-ver_inside,2*all_fat);
                    if(exist) exist_flag[2] = 1;
                    index_of_rect ++;
                    exist = getSubArea(sc,mat,imagex[3] + i,imagey[3] +j,search_angle_ver,ver_outside-ver_inside,2*all_fat);
                    if(exist) exist_flag[3] = 1;
                    //********************horizol**************************
                    index_of_rect ++;
                    exist = getSubArea(sc,mat,imagex[4] + i,imagey[4] +j,search_angle_hor,hor_outside-hor_inside,2*all_fat);
                    if(exist) exist_flag[4] = 1;
                    index_of_rect ++;
                    exist = getSubArea(sc,mat,imagex[5] + i,imagey[5] +j,search_angle_hor,hor_outside-hor_inside,2*all_fat);
                    if(exist) exist_flag[5] = 1;
                    index_of_rect ++;
                    exist = getSubArea(sc,mat,imagex[6] + i,imagey[6] +j,search_angle_hor,hor_outside-hor_inside,2*all_fat);
                    if(exist) exist_flag[6] = 1;
                    index_of_rect ++;
                    exist = getSubArea(sc,mat,imagex[7] + i,imagey[7] +j,search_angle_hor,hor_outside-hor_inside,2*all_fat);
                    if(exist) exist_flag[7] = 1;
                    //***************************************************

                    int count1 = 0,count2 = 0;
                    for(int k=0;k<4;k++)
                        if( exist_flag[k] )
                            count1 ++;
                    for(int k=4;k<8;k++)
                        if( exist_flag[k] )
                            count2 ++;

                    if( count1>= 3 && count2 >=3 ) {

//                        qDebug()<<"in angle"<<local_angle;
//                        qDebug()<<exist_flag[0]<<exist_flag[1]<<exist_flag[2]<<exist_flag[3] \
//                                <<exist_flag[4]<<exist_flag[5]<<exist_flag[6]<<exist_flag[7];

                        std::vector<double> xx,yy;
                        std::vector<int> ff;
                        for(int kk=0;kk<8;kk++) {
                            xx.push_back(imagex[kk] + i);
                            yy.push_back(imagey[kk] + j);
                            ff.push_back(exist_flag[kk]);
                        }
                        store_imgx.push_back(xx);
                        store_imgy.push_back(yy);
                        store_flag.push_back(ff);
                        store_flag_size.push_back(count1 + count2);
                        //calculateCrossCenter(sc,label,parameters,exist_flag,global_angle,xx,yy,true);
                        if( (count1 + count2) == 8 ) {
                            find_one = true;
                            break;
                        }

                    }

                    memset(exist_flag,0x00,sizeof(exist_flag));

#ifdef DEBUG_DEBUG
                    QApplication::processEvents();
                    sc->update();
                    usleep(1000*300);
#endif

                }//for____oneangle__slider_in_one_position

                if(find_one)  break;
            }//for____oneangle__slider_in_diff_position


            if(find_one)  break;
        }//for____diffrent_angle


        //now find the best one
        if(store_flag_size.size()) {
            int max = *store_flag_size.begin();
            int mark = 0;
            int i=0;
            std::vector<int>::iterator it;
            for(   it=store_flag_size.begin(); \
                     it!=store_flag_size.end();it++,i++) {
                    if(*it > max)
                        mark = i;
            }

            int flg[8];
            double xx[8],yy[8];
            for(int kk=0;kk<8;kk++) {
                flg[kk] = store_flag[mark][kk];
                xx[kk] = store_imgx[mark][kk];
                yy[kk] = store_imgy[mark][kk];
            }
            calculateCrossCenter(sc,label,parameters,flg,global_angle,xx,yy,true);

        }


}


void CD_TestSolve::calculateCrossCenter(CD_ImageScreen * sc,cv::Mat &mat, \
                                        double *parameters,int *flag, \
                                        double angle,double * imagex,double *imagey,bool ) {


    double global_angle = angle;
    double all_fat = parameters[0];
    double hor_inside = parameters[1];
    double hor_outside = parameters[2];
    //double hor_with = parameters[3];
    double ver_inside = parameters[4];
    double ver_outside = parameters[5];
    //double ver_with = parameters[6];
    //double  imagex,imagey;


    QList<QPoint> v1,v2,h1,h2;
    //********************vertical**************************
    if(flag[0])
        getSubArea(mat,imagex[0],imagey[0],global_angle-M_PI_2,ver_outside-ver_inside,2*all_fat,v1);
    if(flag[1])
        getSubArea(mat,imagex[1],imagey[1],global_angle-M_PI_2,ver_outside-ver_inside,2*all_fat,v2);
    if(flag[2])
        getSubArea(mat,imagex[2],imagey[2],global_angle-M_PI_2,ver_outside-ver_inside,2*all_fat,v1);
    if(flag[3])
        getSubArea(mat,imagex[3],imagey[3],global_angle-M_PI_2,ver_outside-ver_inside,2*all_fat,v2);
    //*********************horizol***************************
    if(flag[4])
        getSubArea(mat,imagex[4],imagey[4],global_angle,hor_outside-hor_inside,2*all_fat,h1);
    if(flag[5])
        getSubArea(mat,imagex[5],imagey[5],global_angle,hor_outside-hor_inside,2*all_fat,h1);
    if(flag[6])
        getSubArea(mat,imagex[6],imagey[6],global_angle,hor_outside-hor_inside,2*all_fat,h2);
    if(flag[7])
        getSubArea(mat,imagex[7],imagey[7],global_angle,hor_outside-hor_inside,2*all_fat,h2);

    CD_ImageProc proc;
    RS_Line l1,l2,l3,l4;
    l1 = proc.calculateLine(v1);
    l2 = proc.calculateLine(v2);
    l3 = proc.calculateLine(h1);
    l4 = proc.calculateLine(h2);

    sc->drawLine(l1);
    sc->drawLine(l2);
    sc->drawLine(l3);
    sc->drawLine(l4);

}

void CD_TestSolve::icvprCcaBySeedFill(const cv::Mat& _binImg, \
                                      cv::Mat& _lableImg,std::vector<std::vector<std::pair<int ,int> > > &roads) {

    if (_binImg.empty() ||
        _binImg.type() != CV_8UC1)
    {
        return ;
    }

    _lableImg.release() ;
    _binImg.convertTo(_lableImg, CV_32SC1) ;

    int label = 1 ;  // start by 2

    int rows = _binImg.rows - 1 ;
    int cols = _binImg.cols - 1 ;

    for (int i = 0; i < _binImg.rows; i++)
    {
       *_lableImg.ptr<int>(i,0) = 0 ;
       *_lableImg.ptr<int>(i,_binImg.cols-1) = 0 ;
    }
    for (int i = 0; i < _binImg.cols; i++)
    {
        *_lableImg.ptr<int>(0,i) = 0 ;
        *_lableImg.ptr<int>(_binImg.rows-1,i) = 0 ;
    }

    for (int i = 1; i < rows-1; i++)
    {
        int* data= _lableImg.ptr<int>(i) ;
        for (int j = 1; j < cols-1; j++)
        {
            if (data[j] == 1)
            {
                std::stack<std::pair<int,int> > neighborPixels ;
                neighborPixels.push(std::pair<int,int>(i,j)) ;     // pixel position: <i,j>
                ++label ;  // begin with a new label

                std::vector<std::pair<int, int > > one_route;
                one_route.push_back(std::pair<int,int>(i,j));

                while (!neighborPixels.empty())
                {
                    // get the top pixel on the stack and label it with the same label
                    std::pair<int,int> curPixel = neighborPixels.top() ;
                    int curX = curPixel.first ;
                    int curY = curPixel.second ;
                    _lableImg.at<int>(curX, curY) = label ;

                    // pop the top pixel
                    neighborPixels.pop() ;

                    // push the 4-neighbors (foreground pixels)
                    if (_lableImg.at<int>(curX, curY-1) == 1)
                    {// left pixel
                        neighborPixels.push(std::pair<int,int>(curX, curY-1)) ;
                        one_route.push_back(std::pair<int,int>(curX, curY-1));
                    }
                    if (_lableImg.at<int>(curX, curY+1) == 1)
                    {// right pixel
                        neighborPixels.push(std::pair<int,int>(curX, curY+1)) ;
                        one_route.push_back(std::pair<int,int>(curX, curY+1)) ;
                    }
                    if (_lableImg.at<int>(curX-1, curY) == 1)
                    {// up pixel
                        neighborPixels.push(std::pair<int,int>(curX-1, curY)) ;
                        one_route.push_back(std::pair<int,int>(curX-1, curY)) ;
                    }
                    if (_lableImg.at<int>(curX+1, curY) == 1)
                    {// down pixel
                        neighborPixels.push(std::pair<int,int>(curX+1, curY)) ;
                        one_route.push_back(std::pair<int,int>(curX+1, curY)) ;
                    }

                    if (_lableImg.at<int>(curX-1, curY-1) == 1)
                    {// down pixel
                        neighborPixels.push(std::pair<int,int>(curX-1, curY-1)) ;
                        one_route.push_back(std::pair<int,int>(curX-1, curY-1)) ;
                    }
                    if (_lableImg.at<int>(curX+1, curY-1) == 1)
                    {// down pixel
                        neighborPixels.push(std::pair<int,int>(curX+1, curY-1)) ;
                        one_route.push_back(std::pair<int,int>(curX+1, curY-1)) ;
                    }
                    if (_lableImg.at<int>(curX-1, curY+1) == 1)
                    {// down pixel
                        neighborPixels.push(std::pair<int,int>(curX-1, curY+1)) ;
                        one_route.push_back(std::pair<int,int>(curX-1, curY+1)) ;
                    }
                    if (_lableImg.at<int>(curX+1, curY+1) == 1)
                    {// down pixel
                        neighborPixels.push(std::pair<int,int>(curX+1, curY+1)) ;
                        one_route.push_back(std::pair<int,int>(curX+1, curY+1)) ;
                    }

                }

                roads.push_back(one_route);

            }
        }
    }

    qDebug()<<"icvprCcaBySeedFill::label___count"<<label;
}


cv::Scalar CD_TestSolve::icvprGetRandomColor()
{
    uchar r = 255 * (rand()/(1.0 + RAND_MAX));
    uchar g = 255 * (rand()/(1.0 + RAND_MAX));
    uchar b = 255 * (rand()/(1.0 + RAND_MAX));
    return cv::Scalar(b,g,r) ;
}


void CD_TestSolve::icvprLabelColor(const cv::Mat& _labelImg, cv::Mat& _colorLabelImg)
{
    if (_labelImg.empty() ||
        _labelImg.type() != CV_32SC1)
    {
        return ;
    }

    std::map<int, cv::Scalar> colors ;

    int rows = _labelImg.rows ;
    int cols = _labelImg.cols ;

    _colorLabelImg.release() ;
    _colorLabelImg.create(rows, cols, CV_8UC3) ;
    _colorLabelImg = cv::Scalar::all(0) ;

    for (int i = 0; i < rows; i++)
    {
        const int* data_src = (int*)_labelImg.ptr<int>(i) ;
        uchar* data_dst = _colorLabelImg.ptr<uchar>(i) ;
        for (int j = 0; j < cols; j++)
        {
            int pixelValue = data_src[j] ;
            if (pixelValue > 1)
            {
                if (colors.count(pixelValue) <= 0)
                {
                    colors[pixelValue] = icvprGetRandomColor() ;
                }
                cv::Scalar color = colors[pixelValue] ;
                *data_dst++   = color[0] ;
                *data_dst++ = color[1] ;
                *data_dst++ = color[2] ;
            }
            else
            {
                data_dst++ ;
                data_dst++ ;
                data_dst++ ;
            }
        }
    }
}


void CD_TestSolve::searchCircle(CD_ImageScreen *sc, cv::Mat &mat, cv::Mat &,\
                                RS_Circle &circle) {


    int fat = 10;
    int imgw = mat.cols;
    int imgh = mat.rows;
    int search_limit = 0;
    int search_width = 0;
    int search_height = 0;
    double max_size = 0;
    double circle_r = circle.getRadius();

    max_size = circle.getRadius()*2;

    if ( max_size >= imgw || max_size >= imgh) return;

    search_width = imgw - max_size;
    search_height = imgh - max_size;
    search_limit = max_size/2;

    if( imgw <= search_limit || imgh <= search_limit ) return;

    int init_px,init_py;
    init_px = search_limit;
    init_py = search_limit;

    double ang_cos[720];
    double ang_sin[720];
    double init_v = 0.0;
    for(int i=0;i<360;i++) {
        ang_cos[i] = cos(init_v);
        ang_sin[i] = sin(init_v);
        init_v += ( (1/180.0)*M_PI );
    }

    //now we start search
    for( int i = 0; i < (search_width); i++ ) {
        for( int j = 0; j < (search_height); j++ ) {

            //if( *mat.ptr<uchar>( init_py+j,init_px+i ) < 255 ) continue; //uselesse points
            #ifdef DEBUG_DEBUG
            RS_Circle cc;
            cc.setCenter(RS_Vector(init_px + i,init_py + j));
            cc.setRadius(circle_r);
            sc->resetCircles();
            sc->drawCircle(cc);
            #endif

            int imgx,imgy;
            int exist_count = 0;
            double d = circle_r - fat;
            //qDebug()<<"angss"<<s<<ang_cos[s]<<ang_sin[s];

            for(int s=0;s<360;s++) {


                  for( d = circle_r - fat;d < (circle_r + fat); d+= 1) {
                      imgx = init_px + i + ang_cos[s]*d;
                      imgy = (init_py + j )- ang_sin[s]*d;

#ifdef DEBUG_DEBUG
                      cc.setCenter(RS_Vector(imgx,imgy));
                      cc.setRadius(2);
                      sc->drawCircle(cc);
#endif

                      if(  *mat.ptr<uchar>(imgy,imgx ) == 255 ) {
                          exist_count ++;
                          break;
                      }
                  }

            }

#ifdef DEBUG_DEBUG
            QApplication::processEvents();
            sc->update();
            usleep(1000*30);
#endif

//            if(exist_count)
//             qDebug()<<"fine one circle __++++"<<exist_count<<i<<j;
            if( exist_count >= 300 ) {
//                RS_Circle cc;
//                cc.setCenter(RS_Vector(imgx,imgy));
//                cc.setRadius(circle_r);
//                sc->drawCircle(cc);
//                qDebug()<<"fine one circle __++++"<<exist_count<<i<<j;
                goto donee;
            }




        }
    }

donee:
    qDebug()<<"done";

}

void CD_TestSolve::tranMat(cv::Mat &mat, unsigned char *imgdata) {

    int www = mat.cols;
    for(int i=0;i<mat.rows;i++)
        for(int j=0;j<mat.cols;j++) {
            imgdata[i*www +j] = *mat.ptr<uchar>(i,j);
        }

}

void CD_TestSolve::tranMat(unsigned char *s, int w, int h, cv::Mat &mat) {


    mat.create(h,w,CV_8UC1);
    int www = w;
    for(int i=0;i<mat.rows;i++)
        for(int j=0;j<mat.cols;j++)
            *mat.ptr<uchar>(i,j) = s[i*www +j];

}


/**
 *在二项式系数基础之上计算梯度方向
*/
void CD_TestSolve::calculateOri(double *k,double *x,double *y) {
    *x = 2*k[1] + k[3] + k[4];
    *y = 2*k[2] + k[3] + k[5];
//    RS_Vector v(xx,yy);
//    *ret = v.angle();
}

/**
*这个函数要求是在已经建立好了梯度图像的基础之上
* 得到了拟合系数之后
*/
void CD_TestSolve::calculateSubpiex(double *k, double *x, double *y) {

    double K = (2*k[2-1] + k[3-1] + k[5-1])/(2*k[1-1] + k[3-1] + k[4-1]);
    double xx = (2*k[2-1]*K*K - 2*k[2-1]*K + k[3-1]*K - k[5-1]*K - k[4-1] - k[3-1])/ \
            (2*( k[1-1] + k[2-1]*K*K + k[3-1]*K ));
    *x = xx;
    *y = K*xx - K +1;
}


void CD_TestSolve::calculateCoefficient(unsigned char *Mat3_3,int wid_step,double *K) {

    static double A[6][6] = {
        {0.5,   0,     0,   -1,  0,   0.166667},
        {0,      0.5,  0,   0 , -1,  0.166667},
        {0,      0,     0.25 , -0.25,   -0.25,   0.25},
        {-1,     0,    -0.25,  2.416667,  0.25,  -0.75},
        {0,     -1,    -0.25,  0.25,  2.416667,   -0.75},
        {0.166667, 0.166667, 0.25, -0.75, -0.75,  0.805556}
    };

    double H[6];
    double g[3][3];
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            g[i][j] = Mat3_3[j*wid_step+i];

    H[0] = g[1][0] + g[1][1] + g[1][2] + 4*( g[2][0] + g[2][1] + g[2][2] );
    H[1] = g[0][1] + g[1][1] + g[2][1] + 4*( g[0][2] + g[1][2] + g[2][2] );
    H[2] = g[1][1] + 2*( g[2][1] + g[1][2] ) + 4*( g[2][2] );
    H[3] = g[1][0] + g[1][1] + g[1][2] + 2*( g[2][0] + g[2][1] + g[2][2] );
    H[4] = g[0][1] + g[1][1] + g[2][1] + 2*( g[0][2] + g[1][2] + g[2][2] );
    H[5] = g[0][0] + g[1][0] + g[2][0] + g[0][1] + g[1][1] + g[2][1] + g[0][2] + g[1][2] + g[2][2];

    double count = 0;
    for( int i = 0; i<6; i++ ) {
        count = 0;
        for( int j = 0; j<6; j++ ) {
            count += (A[i][j] * H[j]);
        }
        K[i] = count;
    }

}

/**
* this part is the template_match algorithm
* this fun test ok
*/
void CD_TestSolve::calculateCoefficient(const cv::Mat &Mat3_3,double *K) {

    static double A[6][6] = {
        {0.5,   0,     0,   -1,  0,   0.166667},
        {0,      0.5,  0,   0 , -1,  0.166667},
        {0,      0,     0.25 , -0.25,   -0.25,   0.25},
        {-1,     0,    -0.25,  2.416667,  0.25,  -0.75},
        {0,     -1,    -0.25,  0.25,  2.416667,   -0.75},
        {0.166667, 0.166667, 0.25, -0.75, -0.75,  0.805556}
    };

    double H[6];
    double g[3][3];
    g[0][0] = *Mat3_3.ptr<uchar>(0,0);
    g[0][1] = *Mat3_3.ptr<uchar>(1,0);
    g[0][2] = *Mat3_3.ptr<uchar>(2,0);
    g[1][0] = *Mat3_3.ptr<uchar>(0,1);
    g[1][1] = *Mat3_3.ptr<uchar>(1,1);
    g[1][2] = *Mat3_3.ptr<uchar>(2,1);
    g[2][0] = *Mat3_3.ptr<uchar>(0,2);
    g[2][1] = *Mat3_3.ptr<uchar>(1,2);
    g[2][2] = *Mat3_3.ptr<uchar>(2,2);
    H[0] = g[1][0] + g[1][1] + g[1][2] + 4*( g[2][0] + g[2][1] + g[2][2] );
    H[1] = g[0][1] + g[1][1] + g[2][1] + 4*( g[0][2] + g[1][2] + g[2][2] );
    H[2] = g[1][1] + 2*( g[2][1] + g[1][2] ) + 4*( g[2][2] );
    H[3] = g[1][0] + g[1][1] + g[1][2] + 2*( g[2][0] + g[2][1] + g[2][2] );
    H[4] = g[0][1] + g[1][1] + g[2][1] + 2*( g[0][2] + g[1][2] + g[2][2] );
    H[5] = g[0][0] + g[1][0] + g[2][0] + g[0][1] + g[1][1] + g[2][1] + g[0][2] + g[1][2] + g[2][2];

    double count = 0;
    for( int i = 0; i<6; i++ ) {
        count = 0;
        for( int j = 0; j<6; j++ ) {
            count += (A[i][j] * H[j]);
        }
        K[i] = count;
    }

}


/**
*计算图像的梯度（sobel算子）
*/
void CD_TestSolve::fastSobel(unsigned char *in, int width, int height,unsigned char *edg ) {

         int i,j,temp,widthStep;

         unsigned char *inPtr = NULL;
         unsigned char *inPtr1 = NULL;
         unsigned char *inPtr2 = NULL;
         unsigned char *inPtr3 = NULL;
         unsigned char *inPtr4 = NULL;
         unsigned char *inPtr5 = NULL;
         unsigned char *inPtr6 = NULL;

         int *pEdgeX = pool1;
         int *pEdgeY = pool2;
         int *pEdgeXPtr = NULL;
         int *pEdgeYPtr = NULL;

         unsigned char *angPtr = NULL;
         unsigned char *edgPtr = NULL;

         int widH = 1;
         int widV = 1;
         widV *= width;
         widthStep = width;
         for (i=1;i<height-1;i++)
         {
                   pEdgeXPtr = pEdgeX + i*width + 1;
                   inPtr = in + i * widthStep + 1;
                   inPtr1 = inPtr + widH - widV;
                   inPtr2 = inPtr + widH;
                   inPtr3 = inPtr + widH + widV;
                   inPtr4 = inPtr - widH - widV;
                   inPtr5 = inPtr - widH;
                   inPtr6 = inPtr - widH + widV;
                   for (j=1;j<width-1;j++, pEdgeXPtr++)
                   {
                            *pEdgeXPtr
                                     = (*inPtr1++ * 1 + (int)*inPtr2++ * 2 + *inPtr3++ * 1)
                                     - (*inPtr4++ * 1 + (int)*inPtr5++ * 2 + *inPtr6++ * 1);
                   }
         }

         for (i=1;i<height-1;i++)
         {
                   pEdgeYPtr = pEdgeY + i*width + 1;
                   inPtr = in + i * widthStep + 1;
                   inPtr1 = inPtr + widV - widH;
                   inPtr2 = inPtr + widV;
                   inPtr3 = inPtr + widV + widH;
                   inPtr4 = inPtr - widV - widH;
                   inPtr5 = inPtr - widV;
                   inPtr6 = inPtr - widV + widH;
                   for (j=1;j<width-1;j++, pEdgeYPtr++)
                   {
                            *pEdgeYPtr
                                     = (*inPtr1++ * 1 + (int)*inPtr2++ * 2 + *inPtr3++ * 1)
                                     - (*inPtr4++ * 1 + (int)*inPtr5++ * 2 + *inPtr6++ * 1);
                   }
         }

         for (i=1;i<height-1;i++)
         {
                   pEdgeXPtr = pEdgeX + i*width + 1;
                   pEdgeYPtr = pEdgeY + i*width + 1;
                   //angPtr = ang + i * widthStep + 1;
                   edgPtr = edg + i * widthStep + 1;
                   for (j=1; j<width-1; j++, pEdgeYPtr++, pEdgeXPtr++, angPtr++, edgPtr++)
                   {
                            //*angPtr = atan2((double)*pEdgeYPtr,(double)*pEdgeXPtr)*180/3.141592654;
                            temp = sqrt((float)*pEdgeXPtr**pEdgeXPtr + (float)*pEdgeYPtr**pEdgeYPtr)/2;
                            *edgPtr = (255<temp ? 255:temp);
                   }
         }

}

/**
*计算图像金字塔
*/
unsigned char * CD_TestSolve::calculateImgPyramid(unsigned char *orig,int w,int h) {


    int i,j;
    int level1_w = w/2;
    int level1_h = h/2;
    unsigned char *data_level1 = new unsigned char[level1_h*level1_w];
    for(  i = 0;  i < level1_h;  i++  ) {
        for(  j = 0;  j < level1_w;  j++  )  {
          data_level1[i*level1_w + j] = (orig[ (i*2)*w + (j*2) ] +  orig[ (i*2)*w + (j*2+1) ] + \
                  orig[ (i*2+1)*w + (j*2) ] +  orig[ (i*2+1)*w + (j*2+1) ] )/4;
        }
    }
    return data_level1;

}


struct Gradient {
    double locate_x;
    double locate_y;
    double x;
    double y;
    double tha;
};

struct TemplateProfile {
    double angle;
    double max_x;
    double max_y;
    std::vector<Gradient>* grad;
};

void CD_TestSolve::matchTemplate( cv::Mat &Mat, cv::Mat &tmmp) {


    //***计算图像金字塔****
    int i,j,last_w,last_h;
    int pyramid_num = 2;
    unsigned char *imgOriginData;
    unsigned char *tmpOriginData;
    int ori_img_w = Mat.cols;
    int ori_img_h = Mat.rows;
    int tmp_img_w = tmmp.cols;
    int tmp_img_h = tmmp.rows;
    int pmd_img_width[pyramid_num];
    int pmd_img_height[pyramid_num];
    int pmd_tmp_width[pyramid_num];
    int pmd_tmp_height[pyramid_num];
    unsigned char *pmd_img_data[pyramid_num];
    unsigned char *pmd_tmp_data[pyramid_num];
    unsigned char *last_img_data;

    imgOriginData =  new unsigned char[Mat.rows*Mat.cols];
    tranMat(Mat,imgOriginData);
    last_w = ori_img_w;
    last_h = ori_img_h;
    last_img_data = imgOriginData;
    for( i = 0; i < pyramid_num; i ++ ) {
        pmd_img_width[i] = last_w/2;
        pmd_img_height[i] = last_h/2;
        pmd_img_data[i] = calculateImgPyramid(last_img_data,last_w,last_h);
        last_w = pmd_img_width[i];
        last_h = pmd_img_height[i];
        last_img_data = pmd_img_data[i];
    }

    tmpOriginData =  new unsigned char[tmp_img_w*tmp_img_h];
    tranMat(tmmp,tmpOriginData);
    last_w = tmp_img_w;
    last_h = tmp_img_h;
    last_img_data = tmpOriginData;
    for( i = 0; i < pyramid_num; i ++ ) {
        pmd_tmp_width[i] = last_w/2;
        pmd_tmp_height[i] = last_h/2;
        pmd_tmp_data[i] = calculateImgPyramid(last_img_data,last_w,last_h);
        last_w = pmd_tmp_width[i];
        last_h = pmd_tmp_height[i];
        last_img_data = pmd_tmp_data[i];
    }




    //***计算顶层梯度方向***注意长宽的宽度都减2
    int top_img_index = pyramid_num -1;
    double KK[6];
    int w = pmd_img_width[top_img_index];
    int h = pmd_img_height[top_img_index];
    double x,y;
    double mod_value;
    Gradient grad_top[h][w];

    cv::Mat mm,mm2;
    tranMat(pmd_tmp_data[top_img_index],pmd_tmp_width[top_img_index],\
            pmd_tmp_height[top_img_index],mm);
    RS_DEBUG->printImage(mm,"tmp");
    tranMat(pmd_img_data[top_img_index],pmd_img_width[top_img_index],\
            pmd_img_height[top_img_index],mm2);
    CD_ImageScreen * sc = RS_DEBUG->printImage(mm2,"img");


    for( i = 0; i < h -2; i++ ) {
        for( j = 0; j < w -2; j++ ) {
                calculateCoefficient(pmd_img_data[top_img_index] + i*w + j,w,KK);
                x = 2*KK[1] + KK[3] + KK[4];
                y = 2*KK[2] + KK[3] + KK[5];
                mod_value = sqrt( x*x + y*y );
                if(mod_value == 0) {
                    grad_top[i+1][j+1].x = 0;
                    grad_top[i+1][j+1].y = 0;
                    continue;
                }
                grad_top[i+1][j+1].x = x/mod_value;
                grad_top[i+1][j+1].y = -y/mod_value;

                if(i%2==0&&j%2==0)
                sc->drawLine(j+1,i+1,grad_top[i+1][j+1].x+j+1,grad_top[i+1][j+1].y+i+1);

                RS_Vector vv(grad_top[i+1][j+1].x,grad_top[i+1][j+1].y);
                printf("%.3f ",vv.angle());

        }
        printf("\n");
    }

    sc->update();

    //***计算模板关键点的梯度***offline
    CD_ImageProc proc;
    cv::Mat tmp_bit_map[pyramid_num];
    std::vector<TemplateProfile> pmd_tmp_profile[pyramid_num];
    Gradient grad;
    TemplateProfile profile;

    int t,s,k;
    for( i = 0; i < pyramid_num; i ++ ) {

        cv::Mat mm;
        std::vector<Gradient> *pmd_tmp_grad = new std::vector<Gradient>;
        tranMat(pmd_tmp_data[i],pmd_tmp_width[i],pmd_tmp_height[i],mm);
        proc.canny(mm,tmp_bit_map[i]);
        //RS_DEBUG->printImage(tmp_bit_map[i],"aftercany");

        w = tmp_bit_map[i].cols;
        h = tmp_bit_map[i].rows;
        for( t = 0; t < h; t++ ) {
            for( s = 0; s < w; s++ ) {
                if( *tmp_bit_map[i].ptr<uchar>(t,s) == 0xff ) {

                    calculateCoefficient(pmd_tmp_data[i] + t*w + s,w,KK);
                    x = 2*KK[1] + KK[3] + KK[4];
                    y = 2*KK[2] + KK[3] + KK[5];
                    mod_value = sqrt( x*x + y*y );
                    if(mod_value == 0) {
                        grad.x = 0;
                        grad.y = 0;
                    } else {
                        grad.x = x/mod_value;
                        grad.y = -y/mod_value;
                    }
                    grad.locate_x = s - w/2.0;
                    grad.locate_y = -t + h/2.0;
                    pmd_tmp_grad->push_back(grad);

                }
            }
        }
        profile.angle = 0.0;
        profile.max_x = w/2+2;
        profile.max_y = h/2 +2;
        profile.grad = pmd_tmp_grad;
        pmd_tmp_profile[i].push_back(profile);  //the profile that angle=0.0

        qDebug()<<"pmd_tmp_grad size = "<<pmd_tmp_grad->size();

        //now we calculate all the angles profile,the step is 0.5
        double angle_count = 0;
        double new_x,new_x1;
        double new_y,new_y1;
        double max_new_x,min_new_x;
        double max_new_y,min_new_y;
        double cos_v,sin_v;
        for( k = 1;k < 720; k++) {
            angle_count += (0.5*M_PI)/180.0;
            std::vector<Gradient> *grades = new std::vector<Gradient>;
            std::vector<Gradient>::iterator it;
            max_new_x = 0;
            min_new_x = 0;
            max_new_y = 0;
            min_new_y = 0;
            for( it = pmd_tmp_grad->begin(); it != pmd_tmp_grad->end(); it++) {

                x = (it->locate_x);
                y = (it->locate_y);
                cos_v = cos(angle_count);
                sin_v = sin(angle_count);
                new_x = (x )*cos_v - (y)*sin_v;
                new_y = (x )*sin_v + (y)*cos_v;
                max_new_x = max_new_x>new_x?max_new_x:new_x;
                max_new_y = max_new_y>new_y?max_new_y:new_y;
                min_new_x = min_new_x<new_x?min_new_x:new_x;
                min_new_y = min_new_y<new_y?min_new_y:new_y;
                grad.locate_x = new_x;
                grad.locate_y = new_y;

                x = x+it->x;
                y = y+it->y;
                new_x1 = (x )*cos_v - (y)*sin_v;
                new_y1 = (x )*sin_v + (y)*cos_v;

                x = new_x1 - new_x;
                y = new_y1 - new_y;

                mod_value = sqrt( x*x + y*y );
                if(mod_value == 0) {
                    grad.x = 0;
                    grad.y = 0;
                } else {
                    grad.x = x/mod_value;
                    grad.y = y/mod_value;
                }
                grades->push_back(grad);
            }

            max_new_x =  fabs(max_new_x);
            max_new_y =  fabs(max_new_y);
            min_new_x = fabs(min_new_x);
            min_new_y = fabs(min_new_y);
            profile.max_x = max_new_x>min_new_x ? max_new_x:min_new_x;
            profile.max_y = max_new_y>min_new_y ? max_new_y:min_new_y;
            profile.max_x +=2;
            profile.max_y +=2;
            profile.angle = angle_count;
            profile.grad = grades;
            pmd_tmp_profile[i].push_back(profile);
        }

    }
    //**the work up there can be done offline
    //**let's print the imormation

    for( i = 0; i < pyramid_num; i ++ ) {
        qDebug()<<"pmd_tmp_profile--size = "<<pmd_tmp_profile[i].size();
    }


    ///***
    ///

    //*************在顶层暴力穷举搜索.....
    qDebug()<<"starting searching....."<<QTime::currentTime(). second()<<QTime::currentTime().msec();

    double max_grad_sum = -1;
    double grad_sum = 0;
    int max_pos_angle = 0;
    int max_pos_x;
    int max_pos_y;

    for( i = 0; i < pmd_tmp_profile[top_img_index].size(); i++) {

        int ww = pmd_tmp_profile[top_img_index][i].max_x;
        int hh = pmd_tmp_profile[top_img_index][i].max_y;
        int img_w = pmd_img_width[top_img_index];
        int img_h = pmd_img_height[top_img_index];
        int search_w = img_w - ww*2;
        int search_h = img_h - hh*2;
        int locateh,locatew;
        if( search_w <= 0 || search_h <= 0 ) continue;

        std::vector<Gradient> *grades = pmd_tmp_profile[top_img_index][i].grad;
        std::vector<Gradient>::iterator it;

        for( t = hh; t < hh+search_h; t++ ) {
            for( s = ww; s < ww+search_w; s++ ) {

                grad_sum = 0;
                for( it = grades->begin(); it != grades->end(); it++ ) {

                    locateh = it->locate_y;
                    locatew = it->locate_x;
                    grad_sum += ( (grad_top[ locateh+ t ] [ locatew + s ].x) * (it->x) +\
                            (grad_top[ locateh+ t] [ locatew +  s].y) * (it->y) );

                }
                grad_sum /= grades->size();
                //max_grad_sum = max_grad_sum>grad_sum ? max_grad_sum:grad_sum;
                if( max_grad_sum < grad_sum ) {
                    max_pos_angle = i;
                    max_pos_x = s;
                    max_pos_y = t;
                    max_grad_sum = grad_sum;
                }

            }
        }

    }


    ///debug___print
    ///


    double mmx = pmd_tmp_profile[top_img_index][max_pos_angle].max_x;
    double mmy = pmd_tmp_profile[top_img_index][max_pos_angle].max_y;
    double pos_xx[4];
    double pos_yy[4];
    pos_xx[0] = max_pos_x - mmx;
    pos_yy[0] = max_pos_y + mmy;
    pos_xx[1] = max_pos_x + mmx;
    pos_yy[1] = max_pos_y + mmy;
    pos_xx[2] = max_pos_x + mmx;
    pos_yy[2] = max_pos_y - mmy;
    pos_xx[3] = max_pos_x - mmx;
    pos_yy[3] = max_pos_y - mmy;

    qDebug()<<"img__width = "<<pmd_img_width[top_img_index]<<"  "<<mmx;
    qDebug()<<"img__height = "<<pmd_img_height[top_img_index]<<"  "<<mmy;

    RS_Line line1(RS_Vector(pos_xx[0],pos_yy[0]),RS_Vector(pos_xx[1],pos_yy[1]));
    RS_Line line2(RS_Vector(pos_xx[1],pos_yy[1]),RS_Vector(pos_xx[2],pos_yy[2]));
    RS_Line line3(RS_Vector(pos_xx[2],pos_yy[2]),RS_Vector(pos_xx[3],pos_yy[3]));
    RS_Line line4(RS_Vector(pos_xx[3],pos_yy[3]),RS_Vector(pos_xx[0],pos_yy[0]));

    sc->drawLine(line1);
    sc->drawLine(line2);
    sc->drawLine(line3);
    sc->drawLine(line4);
    sc->update();
    ///
    ///

    qDebug()<<"end searching....."<<QTime::currentTime(). second()<<QTime::currentTime().msec()<<max_grad_sum<<"  "<<max_pos_angle<<max_pos_x<<max_pos_y;





}
