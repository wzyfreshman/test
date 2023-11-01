#ifndef CCD_MARK_H
#define CCD_MARK_H

#include "cd_onemark.h"

class CD_MainScreen;

class CCD_Mark :public CD_OneMark {

public:
    CCD_Mark(CD_ImageScreen *opfc,CD_ImageScreen *optmp,int num);
    ~CCD_Mark();

    void setLocateX(double x);
    void setLocateY(double y);
    void setLocateZ(double z);
    void setLocateA(double z);

    double getLocateX();
    double getLocateY();
    double getLocateZ();
    double getLocateA();

    int getCameraGain();
    int getCameraExposeTime();
    int getCameraContrast();
    int getCameraGamma();
    bool getEnableCameraPar();
    void setEnableCameraPar(bool);

    bool getLightEnable1();
    bool getLightEnable2();
    bool getLightEnable3();

    cv::Mat *getEdges();
    void clearEdges();
    void detectEdges(cv::Mat *);
    virtual RS_Vector actionIdentify(cv::Mat&,CD_MainScreen  *m,CD_Template *temp = NULL);
    virtual RS_Vector actionIdentifyShapes(cv::Mat&,CD_MainScreen *m_sc,\
                                           RS_Vector*center,double*angle,cv::RotatedRect *rect,
                                     CD_Template *tttt = NULL,bool offset_temp = true);
    virtual void actionIdentifyShapes(cv::Mat&, std::vector<std::vector<cv::Point> >&,\
                                      std::vector<cv::RotatedRect>&ret);

    virtual void push();
    virtual void pop();
    virtual void saveToDisk( CD_IniSettings &set,const QString &name,bool loadtemp = false);
    virtual void readFromDisk( CD_IniSettings &set,const QString &name,bool loadtemp = false);
    int getCameraId() {return cameraid;}
    void setCameraId(int id) {cameraid = id;}
    bool isShapeMode() {return shape_mode;}
    bool isCalCircleCenterMode(){return shape_cal_center;}

    double getWhThresh() {return shape_wh_threshold;}
    double getSimThresh() {return shape_sim_threshold;}

    double circle_radius;
private:
    double locatex;
    double locatey;
    double locatez;
    double locatea;

    double locatexstore;
    double locateystore;
    double locatezstore;
    double locateastore;

    QList< QList<QPoint> > points;
    QList< QList<RS_Vector> > transformed_points;
    cv::Mat edges;

    int cameraid;
    int camera_gain;
    int camera_expose_time;
    int camera_contrast;
    int camera_gamma;
    int camera_gain_store;
    int camera_expose_time_store;
    int camera_contrast_store;
    int camera_gamma_store;
    bool enable_camera_par;

    bool enable_light1;
    bool enable_light2;
    bool enable_light3;

    bool enable_light1_store;
    bool enable_light2_store;
    bool enable_light3_store;


    bool shape_mode;
    double shape_with;
    double shape_height;
    double shape_length;


    double shape_with_store;
    double shape_height_store;
    double shape_length_store;
    bool shape_mode_store;

    int search_shape_len;

    cv::RotatedRect rotate_rect;
    cv::RotatedRect rotate_rect_store;
    double shape_hu[7];
    double shape_hu_store[7];


    double shape_sim_threshold;
    double shape_sim_threshold_store;
    double shape_wh_threshold;
    double shape_wh_threshold_store;

    bool shape_cal_center;
    bool shape_cal_center_store;

    friend class CD_Application;
    friend class CCD_MarkEditor;

};

#endif // CCD_MARK_H
