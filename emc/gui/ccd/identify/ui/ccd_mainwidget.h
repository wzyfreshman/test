#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListWidgetItem>

#include "cd.h"
#include "cnc_listener.h"

class CD_MarkBase;
class CD_CaptureImage;
class CD_Feature;
class CD_Application;
class CCD_Mark;
class CCD_MarkEditor;
class CCD_CncManager;
class CD_ImageScreen;
class CD_IniSettings;

namespace Ui {
class CCD_MainWidget;
}
namespace cv {
class Mat;
}

class CCD_MainWidget : public QWidget,public CNC_Listener
{
    Q_OBJECT
    
public:
    explicit CCD_MainWidget(QWidget *top,QWidget *parent,CD_CaptureImage *cap,\
                            const QString &conf,const QString &par,int num = 0);
    ~CCD_MainWidget();

    void initScreen();
    void initConnect();
    void setDefaultConfigFile(const QString &file);
    void setCncManager(CCD_CncManager *m);
    void setExcuteFiles(const QString &sim,const QString &locate);

    virtual QWidget *getTopWidget();

    void displayResultAngle(double);
    void displayResultOffset(double x,double y);
    void displayCCDOffset(double x,double y);
    void displayIdentifyMark(int index);
    void doLog(const QString &log);
    void setWhitchTempalte(int i);

public slots:

    void slotChangeIdtifyIndex1();
    void slotChangeIdtifyIndex2();
    void slotChangeIdtifyIndex3();
    void slotChangeIdtifyIndex4();
    void slotIdentifyMark();
    void slotEditMark1();
    void slotEditMark2();
    void slotEditMark3();
    void slotEditMark4();
    void slotGoMarkLocation1();
    void slotGoMarkLocation2();
    void slotGoMarkLocation3();
    void slotGoMarkLocation4();
    void actionEditSysParameter();
    void actionEditSysParameter2();

    void slotEditCommonPar();
    void slotEditCommonPar2();

    void slotLoadSetting(QString file = "");
    void slotSaveSetting(QString file = "", bool savemark = false);
    void saveCommonSettings(CD_IniSettings *set = NULL);
    void loadCommonSettings(CD_IniSettings *set = NULL);

    void slotSimulateIdentify();
    void slotUpdateTemDis();


protected:
    virtual void simulateIdentify();
    void timerGetImg();
    CCD_CncManager *cncmanager;

private slots:
    void on_Btn_SavePattern_clicked();
    void on_Btn_LoadPattern_clicked();
    void on_Btn_SaveImg_clicked();
    void on_Btn_LoadImg_clicked();
    void on_Btn_FirstTemp_clicked(bool checked);
    void on_Btn_SecondTemp_clicked(bool checked);


signals:
    void show_msg(int,QString);
private:

    void resizeEvent(QResizeEvent *);
    void timerEvent(QTimerEvent *);

    Ui::CCD_MainWidget *ui;

    CD_MarkBase *selectrect;
    CD_Application *application;
    CD_CaptureImage *capture;
    QString defaultconfigfile;
    QString local_configfile;

    int camergain;
    int shutter;
    int blacklevel;
    int timerid;
    int index_of_mark;

    QWidget *topwidget;

    int channel_num;
    cv::Mat* orgImage[4];

};

#endif // WIDGET_H
