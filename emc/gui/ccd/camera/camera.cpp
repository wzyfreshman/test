#include <QDebug>
#include <QTime>


#ifdef __USING_WINDOWS__
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "camera.h"

#include "CameraApi.h"
#include "CameraDefine.h"
#include "CameraStatus.h"



static bool initok = false;
bool CD_CaptureImage::CameraInitok = false;
bool CD_CaptureImage::thread_working = false;
int CD_CaptureImage::CameraCount = 0;
int CD_CaptureImage::CameraResCount = 1;

bool CD_CaptureImage::camerastatus[10] = {false,false,false,false,\
                                          false,false,false,false,false,false};
cv::Mat** CD_CaptureImage:: image = NULL;
QMutex CD_CaptureImage::lock[10];
QMutex CD_CaptureImage::camera_lock;

using namespace std;

#define FILENAME        "/tmp/test"

static int                     iCameraCounts = 4;
static int                     iStatus=-1;


#define CAMERA_MAX_COUNT 10
unsigned char           * g_pRgbBuffer[CAMERA_MAX_COUNT];
static tSdkCameraDevInfo       tCameraEnumList[CAMERA_MAX_COUNT];
static int                     hCamera[CAMERA_MAX_COUNT];
static BYTE*			        pbyBuffer[CAMERA_MAX_COUNT];
static IplImage *iplImage[CAMERA_MAX_COUNT];

static tSdkCameraCapbility     tCapability;
static tSdkFrameHead           sFrameInfo;
static tSdkImageResolution     sImageSize;

static int __camera_gain[CAMERA_MAX_COUNT] = {0,0,0,0,0,0,0,0,0,0};
static int __camera_shutter[CAMERA_MAX_COUNT] = {0,0,0,0,0,0,0,0,0,0};
static int __camera_gamma[CAMERA_MAX_COUNT] = {0,0,0,0,0,0,0,0,0,0};
static int __camera_blacklevel[CAMERA_MAX_COUNT] = {0,0,0,0,0,0,0,0,0,0};
static int __camera_res_w[CAMERA_MAX_COUNT] = {0,0,0,0,0,0,0,0,0,0};
static int __camera_res_h[CAMERA_MAX_COUNT] = {0,0,0,0,0,0,0,0,0,0};

bool CD_CaptureImage::CameraSdkInitLocal()
{

    CameraSdkInit(1);
    CameraEnumerateDevice(tCameraEnumList,&iCameraCounts);

    //未使用的相机信息
    //CameraIdleStateDevice(tCameraEnumList,&iCameraCounts);


    qDebug()<<"++CameraIdleStateDevice___count="<<iCameraCounts;


    CameraCount = iCameraCounts;
    if(iCameraCounts<=0 || iCameraCounts >=CAMERA_MAX_COUNT ) {
            return false;
    }

    for(int i=0;i<iCameraCounts;i++) {

        iStatus = CameraInit(&tCameraEnumList[i],-1,-1,&hCamera[i]);

        //QString str = QString("camera_%1").arg(i);
        /*QString name1;
        char buf0[30] = "camera_0";
        char buf1[30] = "camera_1";
        name1 = tCameraEnumList[i].acFriendlyName;
        if(name1 == "camera_0")
            CameraSetFriendlyName(hCamera[i],buf1);
        else
            CameraSetFriendlyName(hCamera[i],buf0);
        */

        CameraGetCapability(hCamera[i],&tCapability);
        g_pRgbBuffer[i] = (unsigned char*)malloc(tCapability.sResolutionRange.iHeightMax*tCapability.sResolutionRange.iWidthMax*3);

        CameraPlay(hCamera[i]);
        //tCapability.iImageSizeDesc; // 预设分辨率的个数，即pImageSizeDesc数组的大小
        CameraResCount = tCapability.iImageSizeDesc;

        CameraSetImageResolution(hCamera[i],&tCapability.pImageSizeDesc[0]);
        qDebug()<<"tCapability.iImageSizeDesc"<<tCapability.iImageSizeDesc<<
                  tCapability.pImageSizeDesc[0].iWidth<<tCapability.pImageSizeDesc[0].iHeight;

        if(tCapability.sIspCapacity.bMonoSensor){
            CameraSetIspOutFormat(hCamera[i],CAMERA_MEDIA_TYPE_MONO8);
        }else{
            CameraSetIspOutFormat(hCamera[i],CAMERA_MEDIA_TYPE_RGB8);
        }

        CameraSetTriggerMode(hCamera[i],SOFT_TRIGGER);
        CameraSetTriggerCount(hCamera[i],1);
        CameraSetLutMode(hCamera[i],LUTMODE_PARAM_GEN);
        CameraSetAeState(hCamera[i],FALSE);

    }

    return true;

}

CD_CaptureImage::CD_CaptureImage(int force) {

    initok = CameraInitok;
    if( image== NULL) {
        image = new cv::Mat *[CAMERA_MAX_COUNT];
        for(int i=0;i<CAMERA_MAX_COUNT;i++) {
            image[i] = new cv::Mat();
            iplImage[i] = NULL;
        }
    }
    force_camera_index = force;
}


CD_CaptureImage::~CD_CaptureImage() {

    CameraSdkTerminate();

}


void CD_CaptureImage::CameraSdkTerminate() {

    for( int i = 0; i < iCameraCounts; i++ ) {
        CameraUnInit(hCamera[i]);
        free(g_pRgbBuffer[i]);
    }

}

bool CD_CaptureImage::init() {

    return true;
}

bool CD_CaptureImage::CameraThreadWorking() {

    return thread_working;

}

void CD_CaptureImage::setCapture(bool _set) {
    initok = _set;
}

bool CD_CaptureImage::checkCameraStatus() {
      return false;
}

void CD_CaptureImage::threadStart() {

    thread_working = true;
    this->start();

}

bool CD_CaptureImage::getCameraStatus(int ii) {

    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return false;
    return camerastatus[i];

}

bool CD_CaptureImage::doCapture() {


    for(int i=0;i<iCameraCounts;i++) {


        camera_lock.lock();
        CameraSoftTrigger(hCamera[i]);
        int rerr = CameraGetImageBuffer(hCamera[i],&sFrameInfo,&pbyBuffer[i],3000) ;
        if( rerr == CAMERA_STATUS_SUCCESS )  {

            lock[i].lock();
            CameraImageProcess(hCamera[i], pbyBuffer[i], g_pRgbBuffer[i],&sFrameInfo);

             if (iplImage[i])  {
                    cvReleaseImageHeader(&iplImage[i]);
             }
             iplImage[i] = cvCreateImageHeader(cvSize(sFrameInfo.iWidth,sFrameInfo.iHeight),IPL_DEPTH_8U,1);
              cvSetData(iplImage[i],g_pRgbBuffer[i],sFrameInfo.iWidth*1);//此处只是设置指针，无图像块数据拷贝，不需担心转换效率
              *image[i] = cv::Mat(iplImage[i]);
           CameraReleaseImageBuffer(hCamera[i],pbyBuffer[i]);

            camerastatus[i] = true;
            lock[i].unlock();
        }  else {
            camerastatus[i] = false;
        }
        camera_lock.unlock();

    }
    return true;

}

void CD_CaptureImage::run() {

    while(1) {

        while(initok) {
           doCapture();
           usleep(1000*10);
        }

    }
}

int CD_CaptureImage::getCameraCount() {
    return CameraCount;
}

int CD_CaptureImage::getCameraResCount() {
    return CameraResCount;
}

int CD_CaptureImage::getCameraIndex(int ii) {

//    for(int i=0;i<iCameraCounts;i++) {
//        QString name1 = tCameraEnumList[i].acFriendlyName;
//    qDebug()<<"getCameraIndex(int ii) "<<getCameraCount()<<force_camera_index<<"Name"<<name1;
//    }

    if(getCameraCount() == 1 && force_camera_index == -1) {
        if(ii == 0)
            return 0;
        else
            return -1;
    } else {

        QString getName;

        if(force_camera_index == -1)
            getName =  QString("camera_%1").arg(ii);
        else
            getName =  QString("camera_%1").arg(force_camera_index);

        int index=-1;
        for(int i=0;i<iCameraCounts;i++) {
            QString name1 = tCameraEnumList[i].acFriendlyName;
            if(name1 ==getName ) {
                index = i;
                break;
            }
        }
        return index;


    }

}

bool CD_CaptureImage::getCaptureDataMatDirect(cv::Mat *to,int ii) {

    if(ii<0||ii>=CAMERA_MAX_COUNT) return false;

    int i = getCameraIndex(ii);

    if( i == -1 ) return false;

    camera_lock.lock();

    for( int t=0; t < 20; t++ ) {

        CameraSoftTrigger(hCamera[i]);
        int rerr = CameraGetImageBuffer(hCamera[i],&sFrameInfo,&pbyBuffer[i],3000) ;
        if( rerr == CAMERA_STATUS_SUCCESS )  {
            lock[i].lock();

            CameraImageProcess(hCamera[i], pbyBuffer[i], g_pRgbBuffer[i],&sFrameInfo);

            if( sFrameInfo.iWidth != getCameraResolutionW(i) ||
                 sFrameInfo.iHeight != getCameraResolutionH(i)   )
                continue;

             if (iplImage[i])  {
                    cvReleaseImageHeader(&iplImage[i]);
             }
             iplImage[i] = cvCreateImageHeader(cvSize(sFrameInfo.iWidth,sFrameInfo.iHeight),IPL_DEPTH_8U,1);
              cvSetData(iplImage[i],g_pRgbBuffer[i],sFrameInfo.iWidth*1);//此处只是设置指针，无图像块数据拷贝，不需担心转换效率
              *image[i] = cv::Mat(iplImage[i]);
           CameraReleaseImageBuffer(hCamera[i],pbyBuffer[i]);
           image[i]->copyTo(*to);
           lock[i].unlock();
           camerastatus[i] = true;
           break;
        } else {
            camerastatus[i] = false;
        }

    }
    camera_lock.unlock();

    return camerastatus[i];

}



void CD_CaptureImage::getCaptureDataMat(cv::Mat *to,int ii) {

    if(ii<0||ii>=CAMERA_MAX_COUNT) return;

    int index = getCameraIndex(ii);
    if(!camerastatus[index]) return;

    if(index == -1) return;

    //camera_lock.lock();
    lock[index].lock();
    image[index]->copyTo(*to);
    lock[index].unlock();
    //camera_lock.unlock();

}

void CD_CaptureImage::setCameraMirror(int dir, bool enable,int ii) {

    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return;
    CameraSetMirror(hCamera[i],dir,enable);

}

void CD_CaptureImage::setCameraResolution(int r, int ii,int *retW ,int *retH) {

    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return;
    if( r<0 || r>=tCapability.iImageSizeDesc ) return;
    CameraSetImageResolution(hCamera[i],&tCapability.pImageSizeDesc[r]);

    if(retW != NULL)
        *retW = tCapability.pImageSizeDesc[r].iWidth;
    if(retH != NULL)
        *retH = tCapability.pImageSizeDesc[r].iHeight;

    __camera_res_w[i] = tCapability.pImageSizeDesc[r].iWidth;
    __camera_res_h[i] = tCapability.pImageSizeDesc[r].iHeight;

}

void CD_CaptureImage::setCameraGain(int gain,int ii) {
    //qDebug()<<"setCameraGain"<<gain<<ii;
    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return;
    CameraSetAnalogGain(hCamera[i],gain);
    //qDebug()<<"setCameraGain"<<gain<<ii<<i;
    __camera_gain[i] = gain;

}


void CD_CaptureImage::setCameraShutter(int ms,int ii) {
    //qDebug()<<"setCameraShutter"<<ms<<ii;
    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return;
    double set = ms*1000;
    CameraSetExposureTime(hCamera[i],set);
    //qDebug()<<"setCameraGain"<<ms<<ii<<i;
    __camera_shutter[i] = ms;
    //qDebug()<<camera_shutter[i]<<i;
}

void CD_CaptureImage::setCameraGamma(int ms,int ii) {

    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return;
    qDebug()<<"CameraSetGamma"<<ms<<ii<<i;
    CameraSetGamma(hCamera[i],ms);
    __camera_gamma[i] = ms;

}


void CD_CaptureImage::setCameraContrast(int l,int ii) {
    //qDebug()<<"setCameraContrast"<<l<<ii;
    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return;
     int ret = CameraSetContrast(hCamera[i],l);//dui bi du
     int mode;
     CameraGetLutMode(hCamera[i],&mode);
     int con;
     CameraGetContrast(hCamera[i],&con);
     qDebug()<<"CameraSetContrast"<<l<<ii<<i<<"ret = "<<ret<<"mode="<<mode<<"con="<<con;


    __camera_blacklevel[i] = l;
}

int CD_CaptureImage::getCameraGain(int ii ) {
    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return 0;
    return __camera_gain[i];
}
int CD_CaptureImage::getCameraShutter(int ii ) {
    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return 0;
    return __camera_shutter[i];
}

int CD_CaptureImage::getCameraGamma(int ii ) {
    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return 0;
    return __camera_gamma[i];
}

int CD_CaptureImage::getCameraBlackLevelAll(int ii ) {
    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return 0;
    return __camera_blacklevel[i];
}

int CD_CaptureImage::getCameraResolutionW(int ii ) {
    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return 0;
    int ret = tCapability.sResolutionRange.iWidthMax;
    return ret;

}
int CD_CaptureImage::getCameraResolutionH(int ii ) {
    int i = getCameraIndex(ii);
    if(i<0||i>=CAMERA_MAX_COUNT) return 0;
    int ret = tCapability.sResolutionRange.iHeightMax;
    return ret;
}


void CD_CaptureImage::setCameraName(char *buf1) {

    qDebug()<<"CD_CaptureImage::setCameraName"<<buf1;
    CameraSetFriendlyName(hCamera[0],buf1);

}
