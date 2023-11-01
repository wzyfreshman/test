#ifndef CD_APPLICATION_H
#define CD_APPLICATION_H

#include <QThread>

#include "ccd_mark.h"
#include "cd_fetchpiece.h"
#include "ccd_marksface.h"
#include "cnc_listener.h"

class CCD_MainWidget;
class CCD_MarkEditor;
class CCD_SysParameter;
class CCD_SysParEditor;
class CCD_CncManager;
class CD_CaptureImage;
class CCD_SysParCommonEditor;

#define CCD_CAMERA_COUNT 2

class CD_AppState
{

public:
    enum State {
        StateIdle,
        StateEditFeature,
        StateEditeSysParameter

    };

    CD::MarkPosition pos;
    State state;
    int index;
};


class CD_Application :public QObject,public CCD_MarksFace,public CNC_Listener
{

    Q_OBJECT

public:
    CD_Application(int marknum,const QString &confi);
    ~CD_Application();

    void showInRealTime(int camera,cv::Mat &mat,bool status = false);
    void initScreen();

    ///init widget setup
    void setMainScreen(int camera,QWidget *_place);
    void setAuxScreen(int camera,QList<QWidget *> &list);
    void showInMainScreen(int camera,cv::Mat &mat);
    void showInMainScreen(int camera, QString &filename);
    void showInAuxScreen(int camera,int index,cv::Mat *mat);
    void saveMainScreenImage(int i,const QString &file);

    void saveToDisk( CD_IniSettings &local_set,CD_IniSettings &mark_set,bool local = false,bool mark = true);
    void readFromDisk( CD_IniSettings &local_set,CD_IniSettings &mark_set,bool local = false);
    void localSaveToDisk();
    void localReadFromDisk();

    void setCncManager(CCD_CncManager *m);
    void setCapture(CD_CaptureImage *c);
    void setExcuteFiles(const QString &sim,const QString &locate);

    void m91Notify(QString n);
    void setPosCondition(int camera,int index);
    void updateArrayParameter();
    void updateWhichY();
    void setWhichTemplate(int i);

    //application custom
    void simulateIdentify();
    RS_Vector transFormPoints(RS_Vector &v,RS_Vector &real,
                         CCD_SysParameter *par);

    bool calculate();
    void calculateOffsetMove(RS_Vector &R1,RS_Vector &R2,
                             RS_Vector &P1,RS_Vector &P2);
    bool calculateFromThreePoints(RS_Vector &P1,RS_Vector &P2,
                                  RS_Vector &P3);

    void gotoLocation(int index);

    void initAllMarkProfile();

public slots:

    void actionUpdateDisplayTemp();
    void actionEditTemplate(CCD_Mark *mark);
    void actionEditTemplate(int camera,int index);
    void actionEditSysParameter(int index = 0);
    void actionEditeCommonPar(int index = 0);
    void doImgCompensate(cv::Mat &mat,cv::Mat &To);
    RS_Vector actionIdentify(int camera,int index);
    void slotClearScreen();
    void back();

private:


    void syncCommonParameter(int main = 0);
    void setMainWidget(CCD_MainWidget *wdgt);
    void clearMainScreenCoordinate();

    CCD_MarkEditor *markeditor;
    CD_CaptureImage *capture;
    QList<CCD_Mark *> marks[CCD_CAMERA_COUNT];
    CCD_SysParameter *sysparameter[CCD_CAMERA_COUNT];
    CCD_SysParEditor *sysparametereditor[CCD_CAMERA_COUNT];
    CD_MainScreen  *mainscreens[CCD_CAMERA_COUNT];
    QWidget * mainscreensplace[CCD_CAMERA_COUNT];
    QList<CD_ImageScreen *> auxscreens[CCD_CAMERA_COUNT];
    QList<QWidget *> auxscreensplace[CCD_CAMERA_COUNT];

    int camergain[CCD_CAMERA_COUNT];
    int shutter[CCD_CAMERA_COUNT];
    int blacklevel[CCD_CAMERA_COUNT];
    bool mirrorV[CCD_CAMERA_COUNT];
    bool mirrorH[CCD_CAMERA_COUNT];
    int imgresolution[CCD_CAMERA_COUNT];
    int edit_camera_par_index;

    CCD_SysParCommonEditor *syscommonpareditor;


    CD_AppState appstate;
    CCD_CncManager *cncmanager;
    CCD_MainWidget *mainwidget;

    bool run_without_camera;

    //******application--data******
    int location_num;
    RS_Vector id_img_pos[4];
    RS_Vector id_real_pos[2];


    //**array count
    int array_count;
    int fail_count;
    bool one_piece_mode;
    int one_piece_num;
    QList<int> fail_piece;
    QList<int> fail_index;

    QList<QPointF> array_locations[4];

    int which_y;
    int which_y_set;

    QString local_conf_file;

    QString excute_file_simulation;
    QString excute_file_location;

    friend class CCD_MainWidget;

};

#endif // CD_APPLICATION_H
