#ifndef CAMERA_H
#define CAMERA_H


#include <QMutex>
#include <QWidget>
#include <QThread>

#include "ccd_setting.h"

namespace cv {
class Mat;
}

class CCD_API CD_CaptureImage:public QThread {


public:
    CD_CaptureImage(int force = -1);
    ~CD_CaptureImage();

    bool init();
    void run();
    void setCapture(bool _set);

    void threadStart();
    bool checkCameraStatus();
    bool doCapture();
    bool getCameraStatus(int index = 0);
    void getCaptureDataMat(cv::Mat *to,int i=0);
    bool getCaptureDataMatDirect(cv::Mat *to,int i=0);

    void setCameraMirror(int,bool,int i = 0);
    void setCameraResolution(int,int i = 0,int *retW = NULL,int *retH = NULL);
    void setCameraGain(int,int i = 0);
    void setCameraShutter(int,int i = 0);
    void setCameraGamma(int,int i = 0);
    void setCameraContrast(int,int i = 0);

    void setCameraName(char *buf1);

    int getCameraGain(int i = 0);
    int getCameraShutter(int i = 0);
    int getCameraGamma(int i = 0);
    int getCameraBlackLevelAll(int i = 0);
    int getCameraResolutionW(int i = 0);
    int getCameraResolutionH(int i = 0);

    int getCameraIndex(int i);


    static int getCameraCount();
    static int getCameraResCount();

    static bool CameraInitok ;
    static bool CameraSdkInitLocal();
    static bool CameraThreadWorking();
    static bool CameraSdkTerminate();

private:

    int force_camera_index;

    static bool thread_working;
    static bool camerastatus[10];
    static cv::Mat **image;
    static QMutex lock[10];
    static QMutex camera_lock;

    static int CameraCount;
    static int CameraResCount;

};

#endif // CAMERA_H
