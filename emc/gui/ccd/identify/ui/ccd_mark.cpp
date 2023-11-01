#include <QTime>

#include "ccd_mark.h"
#include "cd_identify.h"
#include "rs_debug.h"
#include "cd_mainscreen.h"

CCD_Mark::CCD_Mark(CD_ImageScreen *opfc,CD_ImageScreen *optmp,int num)
    :CD_OneMark(opfc,optmp,num) {
    enable_camera_par = false;
    shape_sim_threshold = 0.96;
    shape_wh_threshold_store = 20;
}

CCD_Mark::~CCD_Mark() {}

void CCD_Mark::push() {
    CD_OneMark::push();
    locatexstore = locatex;
    locateystore = locatey;
    locatezstore = locatez;
    locateastore = locatea;

    camera_contrast_store = camera_contrast;
    camera_gain_store = camera_gain;
    camera_expose_time_store = camera_expose_time;
    camera_gamma_store = camera_gamma;

    enable_light1_store = enable_light1;
    enable_light2_store = enable_light2;
    enable_light3_store = enable_light3;

    shape_with_store = shape_with;
    shape_height_store = shape_height;
    shape_length_store = shape_length;
    shape_mode_store = shape_mode;

    memcpy(shape_hu_store,shape_hu,sizeof(shape_hu));
    rotate_rect_store = rotate_rect;

    shape_sim_threshold_store = shape_sim_threshold;
    shape_wh_threshold_store = shape_wh_threshold;

    shape_cal_center_store =  shape_cal_center;



}

void CCD_Mark::pop() {
    CD_OneMark::pop();
    locatex = locatexstore;
    locatey = locateystore;
    locatez = locatezstore;
    locatea = locateastore;

    camera_contrast = camera_contrast_store;
    camera_gain = camera_gain_store;
    camera_expose_time = camera_expose_time_store;
    camera_gamma = camera_gamma_store;

    enable_light1 = enable_light1_store;
    enable_light2 = enable_light2_store;
    enable_light3 = enable_light3_store;

    shape_with = shape_with_store;
    shape_height = shape_height_store;
    shape_length = shape_length_store;
    shape_mode = shape_mode_store;

    memcpy(shape_hu,shape_hu_store,sizeof(shape_hu));
    rotate_rect = rotate_rect_store;

    shape_sim_threshold = shape_sim_threshold_store;
    shape_wh_threshold = shape_wh_threshold_store;

    shape_cal_center =  shape_cal_center_store;

}

void CCD_Mark::saveToDisk( CD_IniSettings &set,const QString &name,bool load) {


    set.beginGroup(name);
    set.setValue("LocateX",locatex);
    set.setValue("LocateY",locatey);
    set.setValue("LocateZ",locatez);
    set.setValue("LocateA",locatea);

    set.setValue("CameraGain",camera_gain);
    set.setValue("CameraContrast",camera_contrast);
    set.setValue("CameraExposeTime",camera_expose_time);
    set.setValue("CameraGamma",camera_gamma);

    set.setValue("EnableLight1",enable_light1);
    set.setValue("EnableLight2",enable_light2);
    set.setValue("EnableLight3",enable_light3);

    set.setValue("IsSpecifyArea",is_specify_area);
    set.setValue("SpecifyArea",specify_area);

    for( int i=0; i<7; i++ ) {
        set.setValue( QString("ShapeHu_%1").arg(i),shape_hu[i] );
    }
    set.setValue("shape_sim_threshold",shape_sim_threshold);
    set.setValue("shape_wh_threshold",shape_wh_threshold);
    set.setValue("shape_with",shape_with);
    set.setValue("shape_height",shape_height);
    set.setValue("shape_mode",shape_mode);
    set.setValue("shape_cal_center",shape_cal_center);

    set.endGroup();
    if(load)
        CD_OneMark::saveToDisk(set,name);

}

void CCD_Mark::readFromDisk( CD_IniSettings &set,const QString &name,bool load) {

    set.beginGroup(name);
    locatex = set.value("LocateX",0.0).toDouble();
    locatey = set.value("LocateY",0.0).toDouble();
    locatez = set.value("LocateZ",0.0).toDouble();
    locatea = set.value("LocateA",0.0).toDouble();

    camera_gain = set.value("CameraGain",1).toInt();
    camera_contrast = set.value("CameraContrast",100).toInt();
    camera_expose_time = set.value("CameraExposeTime",100).toInt();
    camera_gamma = set.value("CameraGamma",100).toInt();

    enable_light1 = set.value("EnableLight1",true).toBool();
    enable_light2 = set.value("EnableLight2",true).toBool();
    enable_light3 = set.value("EnableLight3",true).toBool();

    is_specify_area = set.value("IsSpecifyArea",false).toBool();
    specify_area = set.value("SpecifyArea",QRect()).toRect();

    shape_sim_threshold =  set.value("shape_sim_threshold",0.96).toDouble();
    shape_wh_threshold=  set.value("shape_wh_threshold",15).toInt();
    shape_with =  set.value("shape_with",100).toDouble();
    shape_height =  set.value("shape_height",50).toDouble();
    shape_mode =  set.value("shape_mode",false).toBool();
    shape_cal_center =  set.value("shape_cal_center",false).toBool();

    for( int i=0; i<7; i++ ) {
        shape_hu[i] = set.value( QString("ShapeHu_%1").arg(i), 0.0).toDouble();
    }

    set.endGroup();
    if(load)
        CD_OneMark::readFromDisk(set,name);
}

void CCD_Mark::setLocateX(double x) {
    locatex = x;
}

void CCD_Mark::setLocateY(double y) {
    locatey = y;
}

void CCD_Mark::setLocateZ(double z) {
    locatez = z;
}

void CCD_Mark::setLocateA(double a) {
    locatea = a;
}


double CCD_Mark::getLocateX() {
    return locatex;
}

double CCD_Mark::getLocateY() {
    return locatey;
}

double CCD_Mark::getLocateZ() {
    return locatez;
}

double CCD_Mark::getLocateA() {
    return locatea;
}


cv::Mat *CCD_Mark::getEdges() {

    return &edges;

}

int CCD_Mark::getCameraGamma() {
    return camera_gamma;
}

int CCD_Mark::getCameraContrast() {
    return camera_contrast;
}

int CCD_Mark::getCameraGain() {
    return camera_gain;
}

int CCD_Mark::getCameraExposeTime() {
    return camera_expose_time;
}

bool CCD_Mark::getLightEnable1() {return enable_light1;}
bool CCD_Mark::getLightEnable2() {return enable_light2;}
bool CCD_Mark::getLightEnable3() {return enable_light3;}

void CCD_Mark::setEnableCameraPar(bool b) {
    enable_camera_par = b;
}

bool CCD_Mark::getEnableCameraPar() {
    return enable_camera_par;
}

void CCD_Mark::clearEdges() {
    points.clear();
}

RS_Vector CCD_Mark::actionIdentify(cv::Mat &mat,CD_MainScreen *m, CD_Template *temp) {

        return CD_OneMark::actionIdentify(mat,m,temp);

}

void CCD_Mark::detectEdges(cv::Mat *mat) {

    CD_Identify id;
    qDebug()<<"start"<<QTime::currentTime().msec();

    QList< QList<QPoint> > tmp;
    if( isSpecifyAreaEnable() ) {

        cv::Rect rectCut(specify_area.x(), specify_area.y(),
                         specify_area.width(), specify_area.height());
        cv::Mat mt = (*mat)(rectCut);
        cv::Mat tmp_mt ;
        mt.copyTo(tmp_mt);
        id.detectAllPoint(tmp_mt,tmp);

        qDebug()<<"CCD_Mark::detectEdges"<<specify_area<<tmp.size();

        QList<QPoint> localpoints;
        for(int i=0;i<tmp.size();i++) {
            localpoints.clear();
            for(int j=0;j<tmp[i].size();j++) {
                QPoint pp(   tmp[i][j].x() + specify_area.x()  ,
                                     tmp[i][j].y() + specify_area.y()
                                 );
                localpoints.append(pp);
            }
            points.append(localpoints);
        }

    } else {
        id.detectAllPoint(*mat,points);
    }
    qDebug()<<"end"<<QTime::currentTime().msec();


}

RS_Vector CCD_Mark::actionIdentifyShapes(cv::Mat &ori, CD_MainScreen *m_sc,\
                                    RS_Vector *center,  double *ang,cv::RotatedRect *draw_rect,\
                                   CD_Template *tttt,bool offset_temp) {


    RS_Vector ret_sh(false);
    m_sc->resetPoints();
    m_sc->resetRects();
    m_sc->resetLines();
    m_sc->resetCircles();
    //using template
    CD_Identify idty;
    CD_Template temp;
    CD_Template *tp;
    if(tttt == NULL)
        tp = &temp;
    else
        tp = tttt;

    double relatefactor = CD_Parameter::Instance()->getMatchFactor();
    double findrelatfactor = idty.findmatch(ori,*(getTemplateImage()),*tp);

    qDebug()<<"temlatematch factor"<<findrelatfactor<<"threshold = "<<relatefactor;

    if(findrelatfactor<relatefactor) {
        qDebug()<<"templatematch fail!";
        return ret_sh;
    }

    cv::Mat search_mat = *tp->returnAsMat();

    std::vector<std::vector<cv::Point> >path;
    std::vector<cv::RotatedRect> rect;
    int search_shape_len_bk = search_shape_len;
    search_shape_len = 20;
    actionIdentifyShapes(search_mat,path,rect);

    //**first*** width and height must be right
    std::vector<cv::RotatedRect>::iterator it1;
    std::vector<int> wh_ok;
    std::vector<double> shape_angles;

    int index = 0;
    for( it1 = rect.begin(); it1 != rect.end(); it1++,index++ ) {

        cv::Point2f vertices[4];
        it1->points(vertices);
        RS_Vector v1(vertices[0].x , -vertices[0].y);
        RS_Vector v2(vertices[1].x , -vertices[1].y);
        RS_Vector v3(vertices[2].x , -vertices[2].y);
        double d1 = v1.distanceTo(v2);
        double d2 = v2.distanceTo(v3);
        double with_final;
        double height_final;
        double angle_final;
        if ( d1 > d2 ) {
            double angle_tmp = v1.angleTo(v2);
            if( angle_tmp >= 0 && angle_tmp<= M_PI)
                angle_final = angle_tmp;
            else
                angle_final = v2.angleTo(v1);
            with_final = d1;
            height_final = d2;
        }  else {
            double angle_tmp = v2.angleTo(v3);
            if( angle_tmp >= 0 && angle_tmp<= M_PI)
                angle_final = angle_tmp;
            else
                angle_final = v3.angleTo(v2);
            with_final = d2;
            height_final = d1;
        }

        shape_angles.push_back(angle_final);
        if ( fabs( with_final - shape_with) < shape_wh_threshold &&
                fabs( height_final - shape_height) < shape_wh_threshold ) {
            wh_ok.push_back(index);
        }

        //qDebug()<<"wh__match__"<<with_final<<height_final<<shape_wh_threshold<<this;
    }
    //**second** hu mount must be right
    if( wh_ok.size() == 0 ) {
        qDebug()<<"wh__match__fail"<<shape_with<<shape_height<<shape_wh_threshold<<shape_sim_threshold;
        return ret_sh;
    }

    std::vector<int> hu_ok;
    std::vector<int>::iterator it_in;
    CD_ImageProc proc;
    std::vector<double> hus;

    for( it_in = wh_ok.begin(); it_in != wh_ok.end(); it_in++ ) {

        double hu[7];
        cv::Moments mts;
        cv::Mat sh(path[*it_in]);
        mts = cv::moments(sh, true);
        cv::HuMoments(mts,hu);

        qDebug(" hu-ori %f %f %f %f %f %f %f ",shape_hu[0],shape_hu[1],shape_hu[2],shape_hu[3],\
               shape_hu[4],shape_hu[5],shape_hu[6]);
        qDebug(" hu-now %f %f %f %f %f %f %f  ",hu[0],hu[1],hu[2],hu[3],\
                hu[4],hu[5],hu[6]);

        double sim = proc.calculateSimHu2(hu,shape_hu);
        qDebug()<<"sim ="<<sim;
        if(sim >= shape_sim_threshold) {
            hu_ok.push_back(*it_in);
            hus.push_back(sim);
        }
    }

    if(hu_ok.size() == 0) {
        qDebug()<<"hu__match__fail!! "<<shape_with<<shape_height;
        return ret_sh;
    }

    int final_index = -1;
    double most_sim_value = -1;
    index = 0;
    for( it_in = hu_ok.begin(); it_in != hu_ok.end(); it_in++,index++) {
        if( hus[index] > most_sim_value) {
            most_sim_value = hus[index];
            final_index = hu_ok[index];
        }
    }


    if(final_index<0) return ret_sh;

    cv::RotatedRect rt = rect[final_index];
    RS_Vector vvv(rt.center.x,rt.center.y);
    vvv.x = tp->getLocateInParentX()+vvv.x;
    vvv.y = tp->getLocateInParentY()+vvv.y;

    *center = vvv;
    *ang = shape_angles[final_index];
    *draw_rect = rt;


    RS_Vector center_temp;
    //smooth curves
    std::vector<cv::Point> rett;
    proc.smoothCurves(path[final_index],rett);
    qDebug()<<"success center--no-smooth"<<vvv.x<<vvv.y<<shape_angles[final_index];

    cv::RotatedRect rRect = cv::minAreaRect(cv::Mat(rett));
    cv::Point2f vertices_sm[4];
    rRect.points(vertices_sm);
    RS_Vector v1_sm(vertices_sm[0].x , -vertices_sm[0].y);
    RS_Vector v2_sm(vertices_sm[1].x , -vertices_sm[1].y);
    RS_Vector v3_sm(vertices_sm[2].x , -vertices_sm[2].y);
    {
        double d1_sm = v1_sm.distanceTo(v2_sm);
        double d2_sm = v2_sm.distanceTo(v3_sm);
        double angle_final;
        if ( d1_sm > d2_sm ) {
            double angle_tmp = v1_sm.angleTo(v2_sm);
            if( angle_tmp >= 0 && angle_tmp<= M_PI)
                angle_final = angle_tmp;
            else
                angle_final = v2_sm.angleTo(v1_sm);
        }  else {
            double angle_tmp = v2_sm.angleTo(v3_sm);
            if( angle_tmp >= 0 && angle_tmp<= M_PI)
                angle_final = angle_tmp;
            else
                angle_final = v3_sm.angleTo(v2_sm);
        }


        if( isCalCircleCenterMode() ) {

            qDebug()<<"in CalCircleCenterMode";
            CD_Identify id;
            std::vector<cv::Point> &p = path[final_index];
            QList<QPointF> tocal;
            std::vector<cv::Point>::iterator it;
            for( it=p.begin(); it!=p.end(); it++ ) {
                tocal.append(QPointF(it->x,it->y));
            }
            double rett[4];
            id.calculateArc(tocal,rett);

            if(offset_temp) {
                *center = RS_Vector(tp->getLocateInParentX() + rett[0],\
                                    tp->getLocateInParentY() + rett[1]);

            } else {
                *center = RS_Vector(rett[0], rett[1]);
            }

            *ang = 0;
            *draw_rect = rRect;
            circle_radius = rett[2];
            center_temp = RS_Vector(tp->getLocateInParentX() + rett[0],\
                    tp->getLocateInParentY() + rett[1]);

        } else {
            if(offset_temp) {
             *center = RS_Vector(tp->getLocateInParentX() + rRect.center.x,\
                                 tp->getLocateInParentY() + rRect.center.y);
            } else {
                *center = RS_Vector( rRect.center.x,rRect.center.y);
            }
         *ang = angle_final;
         *draw_rect = rRect;

            center_temp = RS_Vector(tp->getLocateInParentX() + rRect.center.x,\
                                    tp->getLocateInParentY() + rRect.center.y);
        }
      qDebug()<<"success center--after-smooth"<<center->x<<center->y<<\
                angle_final;

   }

    if( CD_Parameter::Instance()->isDebugMode() ) {
        cv::Mat matt(getTemplateImage()->getHeight(),\
                     getTemplateImage()->getWidth(),CV_8UC1,cv::Scalar(255));
        std::vector<cv::Point> ::iterator itc;
        for( itc=rett.begin(); itc != rett.end();itc++) {
            int x = itc->x;
            int y = itc->y;
            if(  x>=0 && x<matt.cols && \
                  y>=0 && y<matt.rows) {
                  *matt.ptr<uchar>(y,x) = 0;
            }
        }
        RS_DEBUG->printImage(matt,"shitsssss");
    }



    //****draw*****
    int locatx = tp->getLocateInParentX();
    int locaty = tp->getLocateInParentY();
    int width  = tp->getWidth();
    int height = tp->getHeight();

    ret_sh.valid = true;
    ret_sh.x = center_temp.x;
    ret_sh.y = center_temp.y;

    if(isCalCircleCenterMode() ) {
        RS_Circle circle;
        circle.setCenter(center_temp);
        circle.setRadius(circle_radius);
        m_sc->drawCircle(circle);
    }

    QRectF recf(locatx,locaty,width,height);
    QList<RS_Line> list;
    RS_Vector v1,v2;

    v1.x = center_temp.x - 70;
    v1.y = center_temp.y;
    v2.x = center_temp.x + 70;
    v2.y = center_temp.y;
    list.append(RS_Line(v1,v2));
    v1.x = center_temp.x;
    v1.y = center_temp.y-70;
    v2.x = center_temp.x;
    v2.y = center_temp.y+70;
    list.append(RS_Line(v1,v2));

    m_sc->drawRect(recf);
    m_sc->drawLines(list);

    search_shape_len = search_shape_len_bk;

    return ret_sh;

}

void CCD_Mark::actionIdentifyShapes(cv::Mat &origin2, std::vector<std::vector<cv::Point> >&path,\
                                    std::vector<cv::RotatedRect> &ret) {


    QList<QList<QPoint> >conts;
    QList<int > mark_indexes;
    CD_ImageProc proc;

    //search_shape_len = origin2.cols/3;
    //qDebug()<<"search_shape_len"<<search_shape_len;
    proc.detectAllPoint(origin2,conts);


    for(int i=0; i<conts.size(); i++) {
        if( conts[i].isEmpty() ) continue;
        QPoint start = conts[i].first();
        QPoint end = conts[i].last();
        QLineF len = QLineF(start,end);
        if( len.length() <=10 && conts[i].size()>=search_shape_len )
            mark_indexes.append(i);
    }

    for(int k=0;k<mark_indexes.size();k++) {
        int mark_index = mark_indexes[k];
        std::vector<cv::Point> one_circle;
        if( mark_index<conts.size() ) {
                for(int i=0;i<conts[mark_index].size();i++) {
                    one_circle.push_back(cv::Point(conts[mark_index][i].x(),conts[mark_index][i].y()));
                }
                cv::RotatedRect rRect = cv::minAreaRect(cv::Mat(one_circle));
                ret.push_back(rRect);
                path.push_back(one_circle);
        }
    }
    qDebug()<<"CCD_Mark::actionIdentifyShapes"<<ret.size();

//    qDebug()<<rRect.angle<<rRect.center.x<<rRect.center.y;
//    Point2f vertices[4];
//    rRect.points(vertices);
//    for (int i = 0; i < 4; i++)
//        cv::line(toDraw, vertices[i], vertices[(i+1)%4], Scalar(255));


}
