#include "ccd_mainwidget.h"
#include "ui_widget.h"
#include "edgedetector.h"
#include "cd_linefinder.h"
#include "cd_convert.h"
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <QPainter>

#include "cd_crosscomp.h"
#include "cd_anglemark.h"
#include "cd_interestrect.h"
#include "cd_featureangledata.h"
#include "cd_inisettings.h"
#include "cd_application.h"
#include "ccd_markeditor.h"
#include "ccd_cncmanager.h"
#include "cd_mainscreen.h"

#include "camera.h"

#include <QRect>
#include <stdio.h>
#include <QFileDialog>


using namespace cv;

CCD_MainWidget::CCD_MainWidget(QWidget *top,QWidget *parent,CD_CaptureImage *cap
                               ,const QString &conf,const QString &par,int num) :
    QWidget(parent),
    ui(new Ui::CCD_MainWidget)
{

    ui->setupUi(this);

    capture = cap;
    channel_num = num;

    defaultconfigfile = conf;
    local_configfile = par;

    loadCommonSettings();

    for(int i=0;i<4;i++)
        orgImage[i] = new cv::Mat();

    topwidget = top;

    application  = new CD_Application(4,par);
    application->setCapture(capture);

    QString str("~/meng.ppm");
    QFile file(str);

    if(file.exists())
        application->showInMainScreen(0,str);
    else {

#ifdef __USING_WINDOWS__

        QFile file(":runfile/pattern.ppm");
        QFile fileold("C:/Users/Public/pattern.ppm");
        fileold.remove();
        file.copy("C:/Users/Public/pattern.ppm");
        QString str("C:/Users/Public/pattern.ppm");
        application->showInMainScreen(0,str);

#else

        QFile file(":runfile/pattern.ppm");
        QFile fileold("/tmp/pattern.ppm");
        fileold.remove();
        file.copy("/tmp/pattern.ppm");
        QString str("/tmp/pattern.ppm");
        application->showInMainScreen(0,str);

#endif
    }

    application->setMainScreen(0,ui->MainFrame);

    slotLoadSetting();
    application->setMainWidget(this);
    QList<QWidget *> views;
    views.append(ui->TemplateView1);
    views.append(ui->TemplateView2);
    views.append(ui->TemplateView3);
    views.append(ui->TemplateView4);
    application->setAuxScreen(0,views);
    application->initScreen();

    initConnect();

    if(CD_CaptureImage::CameraInitok) {
        timerid = startTimer(120);
        if(capture->CameraThreadWorking() == false)
            capture->threadStart();

        ui->Btn_LoadImg->hide();

    } else {
        application->showInRealTime(0,*orgImage[0],false);
        //application->showInRealTime(1,orgImage[1],false);
        ui->Btn_LoadImg->show();
    }

    index_of_mark = 0;
    //ui->Btn_CCDPar2->hide();
    //ui->Btn_CommonPar2->hide();
    ui->Temp4->hide();

}

void CCD_MainWidget::initConnect() {

    connect(ui->SelectMark1,SIGNAL(clicked()),this,SLOT( slotChangeIdtifyIndex1()));
    connect(ui->IdentifyMark1,SIGNAL(clicked()),this,SLOT(slotIdentifyMark()));
    connect(ui->EditeMark1,SIGNAL(clicked()),this,SLOT(slotEditMark1()));
    connect(ui->SelectMark2,SIGNAL(clicked()),this,SLOT( slotChangeIdtifyIndex2()));
    connect(ui->EditeMark2,SIGNAL(clicked()),this,SLOT(slotEditMark2()));
    connect(ui->SelectMark3,SIGNAL(clicked()),this,SLOT( slotChangeIdtifyIndex3()));
    connect(ui->EditeMark3,SIGNAL(clicked()),this,SLOT(slotEditMark3()));
    connect(ui->SelectMark4,SIGNAL(clicked()),this,SLOT( slotChangeIdtifyIndex4()));
    connect(ui->EditeMark4,SIGNAL(clicked()),this,SLOT(slotEditMark4()));
    connect(ui->Btn_CCDPar,SIGNAL(clicked()),this,SLOT(actionEditSysParameter()));
    connect(ui->Btn_CommonPar,SIGNAL(clicked()),this,SLOT(slotEditCommonPar()));
    connect(ui->Btn_CCDPar2,SIGNAL(clicked()),this,SLOT(actionEditSysParameter2()));
    connect(ui->Btn_CommonPar2,SIGNAL(clicked()),this,SLOT(slotEditCommonPar2()));

    connect(ui->Btn_SimulateIdentify,SIGNAL(clicked()),this,SLOT(slotSimulateIdentify()));
    connect(ui->Btn_LocateM1,SIGNAL(clicked()),this,SLOT(slotGoMarkLocation1()));
    connect(ui->Btn_LocateM2,SIGNAL(clicked()),this,SLOT(slotGoMarkLocation2()));
    connect(ui->Btn_LocateM3,SIGNAL(clicked()),this,SLOT(slotGoMarkLocation3()));
    connect(ui->Btn_LocateM4,SIGNAL(clicked()),this,SLOT(slotGoMarkLocation4()));
}

void CCD_MainWidget::initScreen() {

    //application->setTemplateView(0,ui->TemplateView1);
    //application->setTemplateView(1,ui->TemplateView2);
    //application->setTemplateView(2,ui->TemplateView3);
    //application->setTemplateView(3,ui->TemplateView4);


}

CCD_MainWidget::~CCD_MainWidget()
{
    slotSaveSetting();
    delete ui;
    delete application;

}

void CCD_MainWidget:: slotChangeIdtifyIndex1() {
    //application-> actionChangeIdentifyIndex(0);
    index_of_mark = 0;
}

void CCD_MainWidget:: slotChangeIdtifyIndex2() {
    //application-> actionChangeIdentifyIndex(1);
    index_of_mark = 1;
}

void CCD_MainWidget:: slotChangeIdtifyIndex3() {
    //application-> actionChangeIdentifyIndex(2);
    index_of_mark = 2;
}

void CCD_MainWidget:: slotChangeIdtifyIndex4() {
    //application-> actionChangeIdentifyIndex(3);
    index_of_mark = 3;
}

void CCD_MainWidget::slotIdentifyMark() {
    application->updateWhichY();
    application->actionIdentify(0,index_of_mark);
}

void CCD_MainWidget::slotEditMark1() {
    application->actionEditTemplate(0,0);

}

void CCD_MainWidget::slotEditMark2() {
    application->actionEditTemplate(0,1);
}

void CCD_MainWidget::slotEditMark3() {
    application->actionEditTemplate(0,2);
}

void CCD_MainWidget::slotEditMark4() {
    application->actionEditTemplate(0,3);
}

void CCD_MainWidget::slotGoMarkLocation1() {
    application->gotoLocation(0);
}

void CCD_MainWidget::slotGoMarkLocation2() {
    application->gotoLocation(1);
}

void CCD_MainWidget::slotGoMarkLocation3() {
    application->gotoLocation(2);
}

void CCD_MainWidget::slotGoMarkLocation4() {
    application->gotoLocation(3);
}

void CCD_MainWidget::actionEditSysParameter() {
    application->actionEditSysParameter(0);
}

void CCD_MainWidget::actionEditSysParameter2() {
    application->actionEditSysParameter(1);
}

void CCD_MainWidget::timerEvent(QTimerEvent *) {
    timerGetImg();
}

void CCD_MainWidget::slotEditCommonPar() {

    application->actionEditeCommonPar(0);
    CD_IniSettings local_set(defaultconfigfile);
    QString str = "GLOABEL_COMMONSETIING";
    local_set.beginGroup(str);
    local_set.setValue("cncmanager_z_num",cncmanager->getZaxisNum());
    local_set.endGroup();

}

void CCD_MainWidget::slotEditCommonPar2() {

    application->actionEditeCommonPar(1);

}

void CCD_MainWidget::saveCommonSettings(CD_IniSettings *set) {


    if( set == NULL) {

        CD_IniSettings local_set(defaultconfigfile);
        QString str = "GLOABEL_COMMONSETIING";
        local_set.beginGroup(str);
        CD_Parameter::commonSettingSaveToDisk(local_set);
        local_set.endGroup();

    } else {

        QString str = "GLOABEL_COMMONSETIING";
        set->beginGroup(str);
        CD_Parameter::commonSettingSaveToDisk(*set);
        set->endGroup();

    }


}


void CCD_MainWidget::loadCommonSettings(CD_IniSettings *set) {


    if( set == NULL) {

        CD_IniSettings local_set(defaultconfigfile);
        QString str = "GLOABEL_COMMONSETIING";
        local_set.beginGroup(str);
        CD_Parameter::commonSettingReadFromDisk(local_set);
        local_set.endGroup();

    } else {

        QString str = "GLOABEL_COMMONSETIING";
        set->beginGroup(str);
        CD_Parameter::commonSettingReadFromDisk(*set);
        set->endGroup();

    }

    RS_MathAcc::instance()->initAtan2Pool();



}

void CCD_MainWidget::slotSaveSetting(QString file, bool savemark) {

    if(file.isEmpty()) {

        CD_IniSettings local_set(defaultconfigfile);
        CD_IniSettings mark_set(local_configfile);
        saveCommonSettings();
        if(savemark)
            application->saveToDisk(local_set,mark_set,true,true);
        else
            application->saveToDisk(local_set,mark_set,true,false);

    } else {

        CD_IniSettings mark_set(file);
        //CD_IniSettings local_set(defaultconfigfile);//unused
        saveCommonSettings(&mark_set);
        application->saveToDisk(mark_set,mark_set,true);

    }

}

void CCD_MainWidget::slotLoadSetting(QString file) {

    if(file.isEmpty()) {

        CD_IniSettings local_set(defaultconfigfile);
        CD_IniSettings mark_set(local_configfile);
        loadCommonSettings();
        application->readFromDisk(local_set,mark_set,true);

    } else {

        CD_IniSettings mark_set(file);
        //CD_IniSettings local_set(defaultconfigfile);//unused
        loadCommonSettings(&mark_set);
        application->readFromDisk(mark_set,mark_set,true);

    }

}


QWidget *CCD_MainWidget::getTopWidget() {

    return topwidget;
}

void CCD_MainWidget::setDefaultConfigFile(const QString &file) {

    defaultconfigfile = file;

}

void CCD_MainWidget::setCncManager(CCD_CncManager *m) {
    cncmanager = m;
    cncmanager->addListener("MW",this);

    CD_IniSettings local_set(defaultconfigfile);
    QString str = "GLOABEL_COMMONSETIING";
    local_set.beginGroup(str);
    int zz = local_set.value("cncmanager_z_num",1).toInt();
    cncmanager->setZaxisNum( zz );
    local_set.endGroup();

    application->setCncManager(m);

}


void CCD_MainWidget::setExcuteFiles(const QString &sim,const QString &locate) {

    application->setExcuteFiles(sim,locate);

}


void CCD_MainWidget::resizeEvent(QResizeEvent *e) {


    QWidget::resizeEvent(e);

}

void CCD_MainWidget::slotUpdateTemDis() {


    application->actionUpdateDisplayTemp();

}

void CCD_MainWidget::slotSimulateIdentify() {
    application->simulateIdentify();
}

void CCD_MainWidget::simulateIdentify() {
    qDebug()<<"virtual simulateidentify";
}

void CCD_MainWidget::doLog(const QString &log) {

    //ui->log_label->setText(log);
    QString tmp = log;
    tmp.prepend(QString("CH%1: ").arg(channel_num+1));
    emit show_msg(0,tmp);

}

void CCD_MainWidget::displayIdentifyMark(int index) {

    if(index==0)
        ui->SelectMark1->setChecked(true);
    else if(index == 1)
        ui->SelectMark2->setChecked(true);
    else if(index == 2)
        ui->SelectMark3->setChecked(true);
    else if(index == 3)
        ui->SelectMark4->setChecked(true);

    if(index>=0 && index<4)
        index_of_mark = index;

}

void CCD_MainWidget::displayCCDOffset(double x, double y) {

    ui->DisOffset->setText(QString("X %1   Y %2").arg(x).arg(y));
}

void CCD_MainWidget::displayResultAngle(double a) {

     ui->DisAngle->setText(QString("%1").arg(a));
}

void CCD_MainWidget::displayResultOffset(double x, double y) {

     ui->DisOffset->setText(QString("X:%1 Y:%2").arg(x).arg(y));
}

void CCD_MainWidget::timerGetImg() {

    if(CD_CaptureImage::CameraInitok == false) return;

    //qDebug()<<"timer get img0"<<QTime::currentTime().second()<<QTime::currentTime().msec();
    if(capture->getCameraStatus(0)) {
        capture->getCaptureDataMat(orgImage[0],0);
        application->showInRealTime(0,*orgImage[0],true);
    } else {
        application->showInRealTime(0,*orgImage[0],false);
    }

    /*
    if(capture->getCameraStatus(1)) {
        capture->getCaptureDataMat(&orgImage[1],1);
        application->showInRealTime(1,orgImage[1],true);
    } else {
        application->showInRealTime(1,orgImage[1],false);
    }*/

    //qDebug()<<"timer get img1"<<QTime::currentTime().second()<<QTime::currentTime().msec();

    static CCD_CncManager::CNC_STATUS_TYPE old_status = CCD_CncManager::IDLE;
    if(cncmanager->getCncStatus() != old_status ) {
        bool enable = true;
        if(cncmanager->getCncStatus() == CCD_CncManager::IDLE)
            enable = true;
        else
            enable = false;

        this->setEnabled(enable);
    }
    old_status = cncmanager->getCncStatus();

}

void CCD_MainWidget::on_Btn_SavePattern_clicked() {

    QDir dir("~/CCD/");
    if( dir.exists() == false )
        dir.mkdir("~/CCD/");

    QString file = QFileDialog::getSaveFileName(this,"ccd","~/CCD/","*.ccd");
    if(!file.isEmpty()) {
        if(!file.endsWith(".ccd"))
            file.append(".ccd");
        slotSaveSetting(file);
    }

}

void CCD_MainWidget::on_Btn_LoadPattern_clicked() {

    QDir dir("~/CCD/");
    if( dir.exists() == false )
        dir.mkdir("~/CCD/");

    QString file = QFileDialog::getOpenFileName(this,"ccd","~/CCD/","*.ccd");
    QFile fs(file);
    if(!file.isEmpty() && fs.exists()) {
        slotLoadSetting(file);
    }

}

void CCD_MainWidget::on_Btn_SaveImg_clicked()
{

    QDir dir("~/CCD/");
    if( dir.exists() == false )
        dir.mkdir("~/CCD/");

    QString file = QFileDialog::getSaveFileName(this,"ppm","~/CCD/","*.ppm");
    if(!file.isEmpty()) {
        if(!file.endsWith(".ppm"))
            file.append(".ppm");
        application->mainscreens[0]->saveMatImage(file);
    }

}


void CCD_MainWidget::on_Btn_LoadImg_clicked()
{

    QDir dir("~/CCD/");
    if( dir.exists() == false )
        dir.mkdir("~/CCD/");

    QString file = QFileDialog::getOpenFileName(this,"ppm","~/CCD/","*");
    QFile fs(file);
    if(!file.isEmpty() && fs.exists()) {
        application->showInMainScreen(0,file);
    }
}

void CCD_MainWidget::on_Btn_FirstTemp_clicked(bool checked)
{
    application->setWhichTemplate(0);
    application->actionUpdateDisplayTemp();
}

void CCD_MainWidget::on_Btn_SecondTemp_clicked(bool checked)
{
    application->setWhichTemplate(1);
    application->actionUpdateDisplayTemp();
}

void CCD_MainWidget::setWhitchTempalte(int i) {

    if( i == 0) {
        ui->Btn_FirstTemp->setChecked(true);
        on_Btn_FirstTemp_clicked(true);
    } else {
        ui->Btn_SecondTemp->setChecked(true);
        on_Btn_SecondTemp_clicked(true);
    }

}
