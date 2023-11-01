#ifndef CD_MATCHTEMPLATEGPU_H
#define CD_MATCHTEMPLATEGPU_H

#include "cd_imageproc.h"
#include "cd_matchtemplate.h"

#include <QThread>
#include <tchar.h>
#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

class CCD_API CD_CalculateThreadGpu:public QThread {

public:
    CD_CalculateThreadGpu();
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


class CCD_API CD_MatchTemplateGpu : public CD_ImageProc {


public:
    static CD_MatchTemplateGpu* instance() {
        if(unique == NULL) {
            unique = new CD_MatchTemplateGpu();
        }
        return unique;
    }
    void matchTemplate(cv::Mat &Mat, std::vector<TemplateProfile> *mod,MatchResult *ret,\
                       MatchTemplateParameter *par);
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
    unsigned char inline *  calculateImgPyramid(unsigned char *orig,\
                                                unsigned char *store,\
                                                int w,int h);
    unsigned char  *  calculateImgPyramidOut(unsigned char *orig,\
                                                unsigned char *store,\
                                                int w,int h);
    unsigned char** pmdImgPool() {return pmd_img_pool;}

private:
    CD_MatchTemplateGpu();
    ~CD_MatchTemplateGpu();


    void inline fastSobel(unsigned char *in, int width, int height, int widthStep,
                   unsigned char *edg, double *ang);
    void inline fastSobelNoEdg(unsigned char *in, int width, int height, int widthStep,
                        double *angx,double *angy,double * poolx,double *pooly);

    static CD_MatchTemplateGpu* unique;
    MatchTemplateParameter parameter;

    cl_int ConvertToString(const char *pFileName, std::string &str);

private:
    int *pool1;
    int *pool2;
    double *pmd_pool_x[MAX_PRYMID];
    double *pmd_pool_y[MAX_PRYMID];
    unsigned char*pmd_img_pool[MAX_PRYMID];



    cl_int			iStatus;				// 函数返回状态
    cl_uint			uiNumPlatforms;				// 平台个数
    cl_platform_id	Platform;				// 选择的平台
    size_t			uiSize;				// 平台版本名字字节数
    cl_int			iErr;				// 返回参数
    char			*pName;				// 平台版本名
    cl_uint			uiNumDevices;				// 设备数量
    cl_device_id	*pDevices ;				// 设备
    cl_context		Context;				// 设备环境
    cl_command_queue	CommandQueue;			// 命令队列
    const char		*pFileName;	// cl文件名
    string			strSource;				// 用于存储cl文件中的代码
    const char		*pSource;							// 代码字符串指针
    size_t			uiArrSourceSize[5];			// 代码字符串长度
    cl_program		Program;				// 程序对象
    const char		*pInput;		// 输入字符串
    size_t			uiStrlength;	// 输入字符串长度
    char			*pOutput;				// 输出字符串
    cl_mem			memInutBuffer;				// 输入内存对象
    cl_mem			memOutputBuffer;				// 输出内存对象
    cl_kernel		Kernel;				// 内核对象
    size_t			uiGlobal_Work_Size[1];		// 用于设定内核分布


};

#endif // CD_MATCHTEMPLATE_H
