#include <iostream>
#include <QDebug>

using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "CameraApi.h"
#include "CameraDefine.h"
#include "CameraStatus.h"




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


int main(int argc,char **argv)
{


    CameraSdkInit(1);
    CameraEnumerateDevice(tCameraEnumList,&iCameraCounts);

    qDebug()<<"+++++++++++iCameraCounts ~~~~~~~~ "<<iCameraCounts;

    if(iCameraCounts<=0 || iCameraCounts >=CAMERA_MAX_COUNT ) {
        return -1;
    }

    for(int i=0;i<iCameraCounts;i++) {

        iStatus = CameraInit(&tCameraEnumList[i],-1,-1,&hCamera[i]);

        QString name1;
        name1 = tCameraEnumList[i].acFriendlyName;
        qDebug()<<"current name = "<<name1;

        if(argc >=2 ) {
            QString str = argv[1];
            CameraSetFriendlyName(hCamera[i],str.toLocal8Bit().data());
            qDebug()<<"set new name"<<str;
        }

        CameraGetCapability(hCamera[i],&tCapability);
        g_pRgbBuffer[i] = (unsigned char*)malloc(tCapability.sResolutionRange.iHeightMax*tCapability.sResolutionRange.iWidthMax*3);

        CameraPlay(hCamera[i]);
        //tCapability.iImageSizeDesc; // 预设分辨率的个数，即pImageSizeDesc数组的大小

        CameraSetImageResolution(hCamera[i],&tCapability.pImageSizeDesc[0]);
        qDebug()<<"tCapability.iImageSizeDesc"<<tCapability.iImageSizeDesc<<
                  tCapability.pImageSizeDesc[0].iWidth<<tCapability.pImageSizeDesc[0].iHeight;

        if(tCapability.sIspCapacity.bMonoSensor){
            CameraSetIspOutFormat(hCamera[i],CAMERA_MEDIA_TYPE_MONO8);
        }else{
            CameraSetIspOutFormat(hCamera[i],CAMERA_MEDIA_TYPE_RGB8);
        }

        //CameraSetTriggerMode(hCamera,0);
        //CameraSetExposureTime(hCamera,50);
        //CameraSetAnalogGain(hCamera,100);
        CameraSetAeState(hCamera[i],FALSE);

    }

    return 0;
}

