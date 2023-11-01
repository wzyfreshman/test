#ifndef CD_MATCHTEMPLATE_H
#define CD_MATCHTEMPLATE_H

#include "cd_imageproc.h"
#include <QThread>

#define MAX_PRYMID 8
#define CALCULATE_THREAD_NUM 4

struct Gradient {
    double locate_x;
    double locate_y;
    double sub_locate_x;
    double sub_locate_y;
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

class CCD_API MatchTemplateParameter {

public:
    double search_angle_step;
    double search_angle_start;
    double search_angle_end;
    int pyramid_num ;
    int angle_search_width ;
    int rect_search_width ;
    double s_min ;
    double s_g;

    std::vector<double> search_angles_start;
    std::vector<double> search_angles_end;
    std::vector<int> search_angles_seperate;

    //imag process parameter
    int median_blur_size,erosion_size;
    bool process_enable;

};

class CCD_API MatchResult {

public:
    bool valid;
    double sim;
    double real_pos_x;
    double real_pos_y;
    double real_pos_angle;
    std::vector<Gradient> grades;

};

class CCD_API RS_MathAcc {

public:

    static int atan2XNum()  {return atan2_x_num;}
    static int atan2YNum()  {return atan2_y_num;}
    static double *atan2PoolX() {return pool_x;}
    static double *atan2PoolY() {return pool_y;}
    static RS_MathAcc* instance() {
        if(unique == NULL) {
            unique = new RS_MathAcc();
        }
        return unique;
    }

    void initAtan2Pool();
    static bool loadOk() {return load_status;}
    void lockMemery();

private:
    RS_MathAcc();
    static int atan2_y_num;
    static int atan2_x_num;
    static double *pool_x;
    static double *pool_y;
    static RS_MathAcc* unique;
    static bool load_status;



};



class CCD_API CD_CalculateThread:public QThread {

public:
    CD_CalculateThread();
private:
    void run();
    void upAroundCalculate();
    void stepGradeCalculate();
public:

    int work_index;

    std::vector<TemplateProfile> *pmd_tmp_profile;
    MatchTemplateParameter *par;
    Gradient *grad_top;
    int top_search_start ;
    int top_search_end ;
    int top_img_index;
    int img_w ;
    int img_h ;
    int max_pos_angle;
    int max_pos_x;
    int max_pos_y;
    double max_grad_sum;

    //work2
    double *template_ang_x[ MAX_PRYMID ];
    double *template_ang_y[ MAX_PRYMID ];
    int rect_search_width;


    //work3
    unsigned char *in;
    int width;
    int height_start;
    int height_end;
    int widthStep;
    double *angx;
    double *angy;
    double *poolx;
    double *pooly;


private:
    int *pool1;
    int *pool2;

};

class CCD_API CD_MatchTemplate : public CD_ImageProc {


public:
    static CD_MatchTemplate* instance() {
        if(unique == NULL) {
            unique = new CD_MatchTemplate();
        }
        return unique;
    }
    void matchTemplate(cv::Mat &Mat, std::vector<TemplateProfile> *mod,MatchResult *ret,\
                       MatchTemplateParameter *par,int angles_index = -1);
    std::vector<TemplateProfile> * generateModel(cv::Mat &Img,MatchTemplateParameter *par);
    std::vector<TemplateProfile> * generateModel(std::vector<cv::Mat> &Imgs,\
                                                 std::vector<cv::Mat> &Bits,\
                                                 MatchTemplateParameter *par);
    std::vector<TemplateProfile> * generateModel(cv::Mat &Img,\
                                                 cv::Mat &Bit,\
                                                 MatchTemplateParameter *par);
    void setCannyThreshold(int L,int H);

    void tranMat(cv::Mat & mat,unsigned char *d);
    void tranMat(unsigned char *s,int w,int h,cv::Mat & mat);
    unsigned char inline *  calculateImgPyramid(unsigned char *orig, \
                                                unsigned char *store, \
                                                int w, int h, MatchTemplateParameter *par = NULL);
    unsigned char  *  calculateImgPyramidOut(unsigned char *orig,\
                                                unsigned char *store,\
                                                int w,int h);
    unsigned char** pmdImgPool() {return pmd_img_pool;}

private:
    CD_MatchTemplate();

    void inline preProcessImag(cv::Mat &image, MatchTemplateParameter *par);
    void inline fastSobel(unsigned char *in, int width, int height, int widthStep,
                   unsigned char *edg, double *ang);
    void inline fastSobelNoEdg(unsigned char *in, int width, int height, int widthStep,
                        double *angx,double *angy,double * poolx,double *pooly);

    static CD_MatchTemplate* unique;
    MatchTemplateParameter parameter;

private:
    int *pool1;
    int *pool2;
    double *pmd_pool_x[MAX_PRYMID];
    double *pmd_pool_y[MAX_PRYMID];
    unsigned char*pmd_img_pool[MAX_PRYMID];
    CD_CalculateThread *calculate_thd[CALCULATE_THREAD_NUM];

};

#endif // CD_MATCHTEMPLATE_H
