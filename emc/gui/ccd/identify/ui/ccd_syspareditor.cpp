#include "ccd_syspareditor.h"
#include "ui_ccd_syspareditor.h"

#include "cd_imagescreen.h"
#include "cd_mainscreen.h"

#include "ccd_markeditor.h"
#include "ccd_mark.h"
#include "ccd_cncmanager.h"
#include "ccd_sysparameter.h"
#include "ccd_magviewdlg.h"
#include "camera.h"

#include "cd_fetchpiece.h"
#include "cd_imageproc.h"

#include <QMessageBox>
#include <QTime>

#include <math.h>

#ifdef __USING_LINUX__
#define _isnan std::isnan
#endif


CCD_SysParEditor::CCD_SysParEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCD_SysParEditor)
{
    ui->setupUi(this);

    initconnect();

    editsc = new CD_ImageScreen(0);
    selectsc = new CD_ImageScreen(0);
    fetchviewsc[0] = new CD_ImageScreen(0);
    fetchviewsc[1] = new CD_ImageScreen(0);
    fetchview_alone = new CD_ImageScreen(0);

    mark = new CCD_Mark(editsc,selectsc,-1);

    markeditor = new CCD_MarkEditor(this);

    mainscreenplace = ui->MainWidget;
    putImageScreenToWidget((CD_ImageScreen *)mainscreen,mainscreenplace);

    for(int i=0;i<10;i++) {
        editsc_for_offset[i] =  new CD_ImageScreen(0);
        selectsc_for_offset[i]  = new CD_ImageScreen(0);
        mark_for_offset[i]  = new CCD_Mark(editsc_for_offset[i],selectsc_for_offset[i],i);
    }

    cncmanager = new CCD_CncManager();

    fetchview[0] = new CD_FetchPiece((CD_ImageScreen *)mainscreen);
    fetchview[1] = new CD_FetchPiece((CD_ImageScreen *)mainscreen);
    fetchview_alone_sel = new CD_InterestRect((CD_ImageScreen *)mainscreen);

    fetchview[0]->hide();
    fetchview[1]->hide();
    fetchview_alone_sel->hide();
    mag_view = new CCD_MagViewDlg(this);
    mag_view->setView(fetchview_alone);

    putImageScreenToWidget(fetchviewsc[0],ui->Frame_Mag);
    putImageScreenToWidget(fetchviewsc[1],ui->Frame_Mag_2);


    QStringList sl;
    QList<QColor> cl;
    sl.append((""));
    cl.append(QColor(255,0,0));
    sl.append((""));
    cl.append(QColor(255,0,0));
    sl.append((""));
    cl.append(QColor(255,0,0));
    sl.append((""));
    cl.append(QColor(255,0,0));
    mainscreen->drawString(sl,cl);

    setWindowTitle(tr("CCD-Parameter"));
    enable_img_compensate = false;

    compensate_offsetx = 10;
    compensate_offsety = 10;

    in_img_compensate = false;

    ui->Fram_Choose->setParent(mainscreen);
    ui->Fram_Choose->setGeometry(10,10,151,281);

    for( int ii = 0; ii<15; ii++ )
        ui->CB_Resolution->addItem(QString("%1").arg(ii));

    ui->Btn_SaveMark->hide();
    ui->Show_Mag->hide();

}

void CCD_SysParEditor::readFromDisk(CD_IniSettings &set,const QString &grp) {

    mark->readFromDisk(set, grp,true);

}

void CCD_SysParEditor::saveToDisk(CD_IniSettings &set,const QString &grp) {

    mark->saveToDisk(set, grp,true);

}

void CCD_SysParEditor::initconnect() {

    connect(ui->Btn_Camera,SIGNAL(clicked()),this,SLOT(slotTurnToCamera()));
    connect(ui->Btn_Height,SIGNAL(clicked()),this,SLOT(slotTurnToCCDHeight()));
    connect(ui->Btn_Mag,SIGNAL(clicked()),this,SLOT(slotTurnToCCDMag()));
    connect(ui->Btn_Offset,SIGNAL(clicked()),this,SLOT(slotTurnToGlobalOffset()));

    connect(ui->Btn_GetMark,SIGNAL(clicked()),this,SLOT(slotGetMark()));
    connect(ui->Sld_Mag,SIGNAL(valueChanged(int)),this,SLOT(slotFetchImage(int)));

    connect(ui->Show_Mag,SIGNAL(clicked()),this,SLOT(slotShowMag()));
    connect(ui->Btn_ZHeight,SIGNAL(clicked()),this,SLOT(slotSetCameraHeight()));

    connect(ui->LinE_MagX,SIGNAL(valueChanged(double)),this,SLOT(slotSetMagX(double)));
    connect(ui->LinE_MagY,SIGNAL(valueChanged(double)),this,SLOT(slotSetMagY(double)));
    connect(ui->LinE_MagAngle,SIGNAL(valueChanged(double)),this,SLOT(slotSetOffsetAngle(double)));

    connect(ui->Btn_IdPos1,SIGNAL(clicked()),this,SLOT(slotIdPos1()));
    connect(ui->Btn_IdPos2,SIGNAL(clicked()),this,SLOT(slotIdPos2()));
    connect(ui->Btn_IdPos3,SIGNAL(clicked()),this,SLOT(slotIdPos3()));
    connect(ui->Btn_IdPos4,SIGNAL(clicked()),this,SLOT(slotIdPos4()));
    connect(ui->Btn_ComputeMag,SIGNAL(clicked()),this,SLOT(slotComputeMag()));
    connect(ui->Btn_Abort,SIGNAL(clicked()),this,SLOT(slotCncAbort()));
    connect(ui->Btn_SaveMark,SIGNAL(clicked()),this,SLOT(slotSaveMark()));

    connect(ui->LinE_OffsetX,SIGNAL(valueChanged(double)),this,SLOT(slotSetOffsetX(double)));
    connect(ui->LinE_OffsetY,SIGNAL(valueChanged(double)),this,SLOT(slotSetOffsetY(double)));
    //connect(ui->LinE_OffsetAngle,SIGNAL(valueChanged(double)),this,SLOT(slotSetOffsetAngle(double)));

    connect(ui->Btn_Identify,SIGNAL(clicked()),this,SLOT(slotAutoLearnIdentify()));

    connect(ui->Btn_IdMarkForOf,SIGNAL(clicked()),this,SLOT(slotIdMarkForOffset1()));
    connect(ui->Btn_IdMarkForOf_2,SIGNAL(clicked()),this,SLOT(slotIdMarkForOffset2()));
    connect(ui->Btn_IdMarkForOf_3,SIGNAL(clicked()),this,SLOT(slotIdMarkForOffset3()));
    connect(ui->Btn_IdMarkForOf_4,SIGNAL(clicked()),this,SLOT(slotIdMarkForOffset4()));
    connect(ui->Btn_IdMarkForOf_5,SIGNAL(clicked()),this,SLOT(slotIdMarkForOffset5()));
    connect(ui->Btn_IdMarkForOf_6,SIGNAL(clicked()),this,SLOT(slotIdMarkForOffset6()));

    connect(ui->Btn_GetMarkForOf,SIGNAL(clicked()),this,SLOT(slotGetMarkForOffset1()));
    //connect(ui->Btn_GetMarkForOf_2,SIGNAL(clicked()),this,SLOT(slotGetMarkForOffset2()));
    //connect(ui->Btn_GetMarkForOf_3,SIGNAL(clicked()),this,SLOT(slotGetMarkForOffset3()));
    //connect(ui->Btn_GetMarkForOf_4,SIGNAL(clicked()),this,SLOT(slotGetMarkForOffset4()));
    //connect(ui->Btn_GetMarkForOf_5,SIGNAL(clicked()),this,SLOT(slotGetMarkForOffset5()));
    //connect(ui->Btn_GetMarkForOf_6,SIGNAL(clicked()),this,SLOT(slotGetMarkForOffset6()));


    connect(ui->Btn_IdTranP1,SIGNAL(clicked()),this,SLOT(slotIdMarkForThrRad1()));
    connect(ui->Btn_IdTranP2,SIGNAL(clicked()),this,SLOT(slotIdMarkForThrRad2()));
    connect(ui->Btn_IdTranP3,SIGNAL(clicked()),this,SLOT(slotIdMarkForThrRad3()));
    connect(ui->Btn_IdTranP4,SIGNAL(clicked()),this,SLOT(slotIdMarkForThrRad4()));
    connect(ui->Btn_GetTranP1,SIGNAL(clicked()),this,SLOT(slotGetMarkForThrRad1()));
    connect(ui->Btn_GetTranP2,SIGNAL(clicked()),this,SLOT(slotGetMarkForThrRad2()));
    connect(ui->Btn_GetTranP3,SIGNAL(clicked()),this,SLOT(slotGetMarkForThrRad3()));
    connect(ui->Btn_GetTranP4,SIGNAL(clicked()),this,SLOT(slotGetMarkForThrRad4()));
    connect(ui->Btn_RadiusCompute,SIGNAL(clicked()),this,SLOT(slotThrRadiusCompute()));
    //connect(ui->Btn_Excute,SIGNAL(clicked()),this,SLOT(slotDoImgTran()));
    //connect(ui->Read_AffinePoints,SIGNAL(clicked()),this,SLOT(slotReadAffinePoints()));

    connect(ui->Offset_RecP1,SIGNAL(clicked()),this,SLOT(slotRecPositionForOffset1()));
    connect(ui->Offset_RecP2,SIGNAL(clicked()),this,SLOT(slotRecPositionForOffset2()));

    connect(ui->Btn_CalculateOffset,SIGNAL(clicked()),this,SLOT(slotCalculateOffset()));
    connect(ui->Btn_CalC1,SIGNAL(clicked()),this,SLOT(slotCalculateCenterPoint1()));
    connect(ui->Btn_CalC2,SIGNAL(clicked()),this,SLOT(slotCalculateCenterPoint2()));

    connect(ui->Btn_Sure,SIGNAL(clicked()),this,SLOT(slotSure()));
    connect(ui->Btn_Cancel,SIGNAL(clicked()),this,SLOT(slotCancel()));


    connect(ui->LinE_GlbOffsetH,SIGNAL(valueChanged(double)),this,SLOT(slotSetGlbOffsetH(double)));
    connect(ui->LinE_GlbOffsetV,SIGNAL(valueChanged(double)),this,SLOT(slotSetGlbOffsetV(double)));
    connect(ui->OffsetGlb_RecConnt,SIGNAL(clicked()),this,SLOT(slotRecPositionGlbOffset()));
    connect(ui->OffsetGlb_IdMark,SIGNAL(clicked()),this,SLOT(slotIdPositionGlbOffset()));
    connect(ui->OffsetGlb_GetMark,SIGNAL(clicked()),this,SLOT(slotGetGlbOffsetMark()));
    connect(ui->OffsetGlb_Compute,SIGNAL(clicked()),this,SLOT(slotCalculateGlbOffset()));

    //connect(ui->CKB_EnableAffine,SIGNAL(clicked(bool)),this,SLOT(slotEnableImgCompensate(bool)));
    //connect(ui->DSB_RectH,SIGNAL(valueChanged(double)),this,SLOT(slotSetRectHeight(double)));
    //connect(ui->DSB_RectW,SIGNAL(valueChanged(double)),this,SLOT(slotSetRectWidth(double)));
    //connect(ui->DSB_ImgMagW,SIGNAL(valueChanged(double)),this,SLOT(slotSetImgMagW(double)));
    //connect(ui->DSB_ImgMagH,SIGNAL(valueChanged(double)),this,SLOT(slotSetImgMagH(double)));

    connect(ui->DSB_ThrRadius,SIGNAL(valueChanged(double)),this,SLOT(slotSetThrRadius(double)));

    connect(ui->Ckb_All,SIGNAL(clicked(bool)),this,SLOT(slotCheckAll(bool)));

    connect(ui->CameraBlackLevel,SIGNAL(valueChanged(int)),this,SLOT(slotCamerBlackLevel(int)));
    connect(ui->CameraGain,SIGNAL(valueChanged(int)),this,SLOT(slotCameraGain(int)));
    connect(ui->CameraGamma,SIGNAL(valueChanged(int)),this,SLOT(slotCameraGamma(int)));
    connect(ui->CameraShutter,SIGNAL(valueChanged(int)),this,SLOT(slotCameraShutter(int)));
    connect(ui->Ckb_ReverseH,SIGNAL(clicked(bool)),this,SLOT(slotSetCameraMirrorH(bool)));
    connect(ui->Ckb_ReverseV,SIGNAL(clicked(bool)),this,SLOT(slotSetCameraMirrorV(bool)));

}

CCD_SysParEditor::~CCD_SysParEditor() {

    delete ui;
    delete mark;

    for(int i=0;i<10;i++) {
        delete mark_for_offset[i];
    }

}

void CCD_SysParEditor::showInRealTime(cv::Mat &mat) {

    if(!isVisible()) return;
    if(CD_CaptureImage::CameraInitok == false) return;

    if(state ==  StateIdle) {

        showInMainScreen(mat);
        cv::Mat *m = fetchview[0]->doCalculateMatImage()->returnAsMat();
        if(m != NULL)
            fetchviewsc[0]->showMatImage(*m);
        m = fetchview[1]->doCalculateMatImage()->returnAsMat();
        if(m != NULL)
            fetchviewsc[1]->showMatImage(*m);

//        m = fetchview_alone_sel->doCalculateMatImage()->returnAsMat();
//        if(m != NULL)
//           fetchview_alone->showMatImage(*m);

    } else if(state == StateEnditingMark) {
        markeditor->showInRealTime(mat);
    }

}

void CCD_SysParEditor::slotStartAutoLearn() {


}

void CCD_SysParEditor::m91Notify(QString n) {

    if(!isVisible()) return;

    if(n.startsWith("PE-L")) {
        bool ok;
        int num = n.mid(4).toDouble(&ok);
        if(ok ) {
            if(num>=0 && num< 10) {
               cncmanager->setParameter(2000,cncmanager->getAbsPos(0));
               cncmanager->setParameter(2001,cncmanager->getAbsPos(1));
               cncmanager->setParameter(2002,cncmanager->getAbsPos(2));
               cncmanager->setParameter(2003,mark_for_offset[num]->getLocateA());
               cncmanager->m91Respond(0);
            } else {
                cncmanager->m91Respond(-1);
            }
        } else {
            cncmanager->m91Respond(-1);
        }

    } else if(n.startsWith("PE-GETCOUNT")) {
           cncmanager->setParameter(2005,6);
           double sleeptime = syspar->getimg_delaytime/1000.0;
           cncmanager->setParameter(2006,sleeptime);
           cncmanager->m91Respond(0);
       }  else if(n.startsWith("PE-I")) {

        //usleep(syspar->getimg_delaytime*1000);
        int num = n.mid(4).toDouble();
        CD_Template tmp;
        if(num<0 || num>10) {
            mainscreen->drawString(1,QColor(255,0,0),tr("num<0 or num>4"));
            cncmanager->m91Respond(-1);
            return;
        }

        cv::Mat mat;
        doImgCompensate(*mainscreen->getCurrentImageMat(),mat);
        RS_Vector resultP(false);
        if(num>=0 &&num<=5)
             resultP= mark_for_offset[0]->actionIdentify(mat,mainscreen,&tmp);

        if(resultP.valid) {

            double centx = (mainscreen->getCurrentImageMat()->cols)/2.0;
            double centy = (mainscreen->getCurrentImageMat()->rows)/2.0;
            double newx = resultP.x - centx;
            double newy =  -(resultP.y - centy);

            if(syspar->camera_magx == 0. ||syspar->camera_magy == 0.) {
                mainscreen->drawString(1,QColor(255,0,0),tr("错误：放大倍率为零 !"));
                cncmanager->m91Respond(-1);
            }  else {

                if(num <=5 && num>=0)
                    offset_pos[num] = RS_Vector(newx,newy);

                cncmanager->m91Respond(0);
                mainscreen->drawString(num,QColor(0,255,0),QString("x=%1 y=%2").arg(newx).arg(newy));
            }

        } else {
            mainscreen->drawString(1,QColor(255,0,0),tr("识别失败"));
            cncmanager->m91Respond(-1);
        }
        mainscreen->update();

    } else if(n.startsWith("PE-C")) {


        if(index_of_offset_center_point == 1) {
            offset_length1 = cncmanager->getAbsPos(2);
            slotCalculateCenterPoint(offset_center_pos1);
        } else {
            offset_length2 = cncmanager->getAbsPos(2);
            slotCalculateCenterPoint(offset_center_pos2);
        }

        cncmanager->m91Respond(0);
    }


}

void CCD_SysParEditor::setCapture(CD_CaptureImage *c) {
    markeditor->setCapture(c);
}

void CCD_SysParEditor::setCncManager(CCD_CncManager *m,int id) {


    cncmanager = m;
    cncmanager->addListener("PE",this);
    markeditor->setCncManager(cncmanager);

    connect(ui->Btn_Cont,SIGNAL(clicked()),cncmanager,SLOT(jogContinue()));
    connect(ui->Btn_Handle,SIGNAL(clicked()),cncmanager,SLOT(jogHandle()));
    connect(ui->Btn_0001,SIGNAL(clicked()),cncmanager,SLOT(jogIncr0001()));
    connect(ui->Btn_001,SIGNAL(clicked()),cncmanager,SLOT(jogIncr001()));
    connect(ui->Btn_01,SIGNAL(clicked()),cncmanager,SLOT(jogIncr01()));
    connect(ui->Btn_1,SIGNAL(clicked()),cncmanager,SLOT(jogIncr1()));
    connect(ui->Jog_speed,SIGNAL(valueChanged(int)),cncmanager,SLOT(jogSpeed(int)));

    connect(ui->Btn_Xminus,SIGNAL(pressed()),cncmanager,SLOT(jogXminusStart()));
    connect(ui->Btn_Xminus,SIGNAL(released()),cncmanager,SLOT(jogXEnd()));

    connect(ui->Btn_Yminus,SIGNAL(pressed()),cncmanager,SLOT(jogYminusStart()));
    connect(ui->Btn_Yminus,SIGNAL(released()),cncmanager,SLOT(jogYEnd()));

    connect(ui->Btn_Zminus,SIGNAL(pressed()),cncmanager,SLOT(jogZminusStart()));
    connect(ui->Btn_Zminus,SIGNAL(released()),cncmanager,SLOT(jogZEnd()));
    connect(ui->Btn_Aminus,SIGNAL(pressed()),cncmanager,SLOT(jogAminusStart()));
    connect(ui->Btn_Aminus,SIGNAL(released()),cncmanager,SLOT(jogAEnd()));

    connect(ui->Btn_Xplus,SIGNAL(pressed()),cncmanager,SLOT(jogXplusStart()));
    connect(ui->Btn_Xplus,SIGNAL(released()),cncmanager,SLOT(jogXEnd()));
    connect(ui->Btn_Yplus,SIGNAL(pressed()),cncmanager,SLOT(jogYplusStart()));
    connect(ui->Btn_Yplus,SIGNAL(released()),cncmanager,SLOT(jogYEnd()));
    connect(ui->Btn_Zplus,SIGNAL(pressed()),cncmanager,SLOT(jogZplusStart()));
    connect(ui->Btn_Zplus,SIGNAL(released()),cncmanager,SLOT(jogZEnd()));
    connect(ui->Btn_Aplus,SIGNAL(pressed()),cncmanager,SLOT(jogAplusStart()));
    connect(ui->Btn_Aplus,SIGNAL(released()),cncmanager,SLOT(jogAEnd()));



}

void CCD_SysParEditor::init(CCD_SysParameter *p,CD_CaptureImage *c) {

    state = StateIdle;
    capture = c;

    ui->Btn_Camera->setChecked(true);
    ui->Btn_Offset->setChecked(false);
    ui->Btn_Height->setChecked(false);
    ui->Btn_Mag->setChecked(false);
    ui->stackedWidget->setCurrentIndex(0);

    syspar = p;
    ui->LinE_MagX->setValue(p->camera_magx);
    ui->LinE_MagY->setValue(p->camera_magy);

    ui->LinE_OffsetX->setValue(p->offset_h);
    ui->LinE_OffsetY->setValue(p->offset_v);
    ui->LinE_OffsetAngle->setValue(p->offset_angle);

    ui->LinE_MagAngle->setValue(p->offset_angle);


    ui->Jog_speed->setValue(cncmanager->getJogSpeed());

    ui->LinE_GlbOffsetH->setValue(p->global_offset_h);
    ui->LinE_GlbOffsetV->setValue(p->global_offset_v);

    ui->CameraBlackLevel->setValue(capture->getCameraBlackLevelAll());
    ui->CameraGain->setValue(capture->getCameraGain());
    ui->CameraShutter->setValue(capture->getCameraShutter());
    ui->CameraGamma->setValue(capture->getCameraGamma());

    ui->DSB_ThrRadius->setValue(p->two_points_w);

    ui->CB_Resolution->setCurrentIndex(p->img_resolution);
    ui->SB_ZHeight->setValue(p->camera_height);
    p->push();
    timerid = startTimer(50);

    ui->Fram_Choose->hide();
}

void CCD_SysParEditor::activeMainScreen() {
    activeScreen(mainscreenplace,(CD_ImageScreen *)mainscreen);
}

void CCD_SysParEditor::slotAutoLearnIdentify() {

    CD_Template tmp;

    cv::Mat mat;
    doImgCompensate(*mainscreen->getCurrentImageMat(),mat);

    RS_Vector resultP;
    if(mark->isShapeMode() && !mark->getShapeBaseMatchFlag()) {

        double angle;
        cv::RotatedRect rect;
        RS_Vector PPP;
        resultP= mark->actionIdentifyShapes(mat,mainscreen,&PPP,&angle,&rect,&tmp,false);

    } else {
        resultP = mark->actionIdentify(mat,mainscreen,&tmp);
    }

    mainscreen->update();


}

void CCD_SysParEditor::slotGetMark(int index) {

    if( index>10 ) return;

    CCD_Mark *localmark;
    if(index == -1) {
        localmark = this->mark;
    } else {
        localmark = mark_for_offset[index];
    }

    state = StateEnditingMark;
    //activeEditeScreen(index);
    localmark->actionEditFeature(true);
    localmark->getScreenFeature()->catchOneImage((CD_ImageScreen *)mainscreen);
    localmark->getScreenTemplate()->catchOneImage((CD_ImageScreen *)mainscreen);
    localmark->push();
    markeditor->init(localmark);
    markeditor->exec();

    int rr = markeditor->exe_ret;
    if( rr == -1 )
         localmark->pop();

    qDebug()<<"shape_sim_threshold = "<<localmark->getSimThresh()
              <<localmark->getWhThresh();

    localmark->exitEditeFeature();
    activeMainScreen();
    state = StateIdle;

}

void CCD_SysParEditor::slotTurnToCamera() {
    ui->stackedWidget->setCurrentIndex(0);
    fetchview[0]->hide();
    fetchview[1]->hide();
    fetchview_alone_sel->hide();
    ui->Fram_Choose->hide();
}

void CCD_SysParEditor::slotTurnToCCDHeight() {
    ui->stackedWidget->setCurrentIndex(1);
    fetchview[0]->show();
    fetchview[1]->show();
    fetchview_alone_sel->show();
    ui->Fram_Choose->hide();
}

void CCD_SysParEditor::slotTurnToCCDMag() {
     ui->stackedWidget->setCurrentIndex(2);
     fetchview[0]->hide();
     fetchview[1]->hide();
     fetchview_alone_sel->hide();
     ui->Fram_Choose->hide();
}

void CCD_SysParEditor::slotTurnToCCDOffset() {
     ui->stackedWidget->setCurrentIndex(3);
     fetchview[0]->hide();
     fetchview[1]->hide();
     fetchview_alone_sel->hide();
     ui->Fram_Choose->show();
}

void CCD_SysParEditor::slotTurnToGlobalOffset() {
     ui->stackedWidget->setCurrentIndex(4);
     fetchview[0]->hide();
     fetchview[1]->hide();
     fetchview_alone_sel->hide();
     ui->Fram_Choose->hide();
}

void CCD_SysParEditor::slotTurnToImgCompensate() {
    ui->stackedWidget->setCurrentIndex(5);
    fetchview[0]->hide();
    fetchview[1]->hide();
    fetchview_alone_sel->hide();
    ui->Fram_Choose->hide();
}

void CCD_SysParEditor::reject() {
    this->close();
}

void CCD_SysParEditor::closeEvent(QCloseEvent *e) {

    if(syspar->checkChange()) {

        int ret = QMessageBox::warning(this,tr("tips")
        ,tr("参数已经修改，确定放弃保存返回？"),
                               tr("确定"),tr("取消"));

        switch (ret) {
           case 0:
               syspar->pop();
               killTimer(timerid);
               e->accept();
               break;
           case 1:
               e->ignore();
               break;
           default:
               break;
         }
    } else {
        killTimer(timerid);
    }
}

void CCD_SysParEditor::slotCameraGain(int g) {
    syspar->camergain = g;
    capture->setCameraGain(g,syspar->getId());
    ui->SB_CameraGain->setValue(g);
}


void CCD_SysParEditor::slotCameraShutter(int s) {
    syspar->shutter = s;
    capture->setCameraShutter(s,syspar->getId());
    ui->spinBox->setValue(s);
}
void CCD_SysParEditor::slotCameraGamma(int s) {
    //syspar->shutter = s;
    capture->setCameraGamma(s);
    ui->SB_CameraGamma->setValue(s);

}
void CCD_SysParEditor::slotCamerBlackLevel(int b){
    syspar->blacklevel = b;
    capture->setCameraContrast(b,syspar->getId());
    ui->SB_CameraContra->setValue(b);
}
void CCD_SysParEditor::slotSetCameraMirrorV(bool b){
    syspar->mirrorV = b;
    capture->setCameraMirror(1,b,syspar->getId());
}
void CCD_SysParEditor::slotSetCameraMirrorH(bool b){
    syspar->mirrorH = b;
    capture->setCameraMirror(0,b,syspar->getId());
}


void CCD_SysParEditor::slotSure() {
    syspar->push();
    this->close();
}

void CCD_SysParEditor::slotCancel() {

    this->close();

}

void CCD_SysParEditor::slotCheckAll(bool b) {
    if( !b) return;
    ui->Ckb_C1->setChecked(true);
    ui->Ckb_C2->setChecked(true);
    ui->Ckb_C3->setChecked(true);
    ui->Ckb_C4->setChecked(true);
    ui->Ckb_C5->setChecked(true);
    ui->Ckb_C6->setChecked(true);
    ui->Ckb_C7->setChecked(true);
    ui->Ckb_C8->setChecked(true);
    ui->Ckb_C9->setChecked(true);
    ui->Ckb_C10->setChecked(true);
    ui->Ckb_C11->setChecked(true);
    ui->Ckb_C12->setChecked(true);
    ui->Ckb_C13->setChecked(true);
    ui->Ckb_C14->setChecked(true);
    ui->Ckb_C15->setChecked(true);

}

void CCD_SysParEditor::slotShowMag() {

//    cv::Mat *m = fetchview_alone_sel->doCalculateMatImage()->returnAsMat();
//    if(m != NULL)
//       fetchview_alone->showMatImage(*m);
//    mag_view->exec();

}

void CCD_SysParEditor::slotFetchImage(int s) {

    fetchview[0]->setFactor(s);
    fetchview[1]->setFactor(s);

}

void CCD_SysParEditor::slotReadCameraH() {

    qDebug()<<"CCD_SysParEditor::slotReadCameraH"<<cncmanager->getAbsPos(3);
    //ui->LinE_Zheight->setValue(cncmanager->getAbsPos(2));

}


void CCD_SysParEditor::slotSetCameraHeight() {
    ui->SB_ZHeight->setValue(cncmanager->getAbsPos(2));
}


void CCD_SysParEditor::slotSetMagX(double set) {
    syspar->camera_magx = set;
}

void CCD_SysParEditor::slotSetMagY(double set) {
    syspar->camera_magy = set;
}

void CCD_SysParEditor::slotIdPos1() {

    CD_Template tmp;
    cv::Mat mat;
    doImgCompensate(*mainscreen->getCurrentImageMat(),mat);

    RS_Vector resultP;
    if(mark->isShapeMode() && !mark->getShapeBaseMatchFlag()) {

        double angle;
        cv::RotatedRect rect;
        RS_Vector PPP;
        resultP= mark->actionIdentifyShapes(mat,mainscreen,&PPP,&angle,&rect,&tmp,false);

    } else {
        resultP = mark->actionIdentify(mat,mainscreen,&tmp);
    }

    if(resultP.valid) {

        double centx = (mainscreen->getCurrentImageMat()->cols)/2.0;
        double centy = (mainscreen->getCurrentImageMat()->rows)/2.0;
        double newx = resultP.x - centx;
        double newy = -(resultP.y - centy);

        mag_real_pos1 = RS_Vector(cncmanager->getAbsPos(0), \
                                  cncmanager->getAbsPos(1));
        mag_img_pos1 = RS_Vector(newx,newy);
        mainscreen->drawString(0,QColor(0,255,0),QString("%1 %2 | %3 %4").arg(cncmanager->getAbsPos(0)).arg(cncmanager->getAbsPos(1)).arg(newx).arg(newy));

    }
    mainscreen->update();
}

void CCD_SysParEditor::slotIdPos2() {

    CD_Template tmp;
    cv::Mat mat;
    doImgCompensate(*mainscreen->getCurrentImageMat(),mat);

    RS_Vector resultP;
    if(mark->isShapeMode() && !mark->getShapeBaseMatchFlag()) {

        double angle;
        cv::RotatedRect rect;
        RS_Vector PPP;
        resultP= mark->actionIdentifyShapes(mat,mainscreen,&PPP,&angle,&rect,&tmp,false);

    } else {
        resultP = mark->actionIdentify(mat,mainscreen,&tmp);
    }


    if(resultP.valid) {

        double centx = (mainscreen->getCurrentImageMat()->cols)/2.0;
        double centy = (mainscreen->getCurrentImageMat()->rows)/2.0;
        double newx = resultP.x - centx;
        double newy = -(resultP.y - centy);

        mag_real_pos2 = RS_Vector(cncmanager->getAbsPos(0), \
                                  cncmanager->getAbsPos(1));
        mag_img_pos2 = RS_Vector(newx,newy);
        mainscreen->drawString(1,QColor(0,255,0),QString("%1 %2 | %3 %4").arg(cncmanager->getAbsPos(0))
                                                    .arg(cncmanager->getAbsPos(1)).arg(newx).arg(newy));


    }
    mainscreen->update();
}

void CCD_SysParEditor::slotIdPos3() {

    CD_Template tmp;
    cv::Mat mat;
    doImgCompensate(*mainscreen->getCurrentImageMat(),mat);

    RS_Vector resultP;
    if(mark->isShapeMode() && !mark->getShapeBaseMatchFlag()) {

        double angle;
        cv::RotatedRect rect;
        RS_Vector PPP;
        resultP= mark->actionIdentifyShapes(mat,mainscreen,&PPP,&angle,&rect,&tmp,false);

    } else {
        resultP = mark->actionIdentify(mat,mainscreen,&tmp);
    }


    if(resultP.valid) {

        double centx = (mainscreen->getCurrentImageMat()->cols)/2.0;
        double centy = (mainscreen->getCurrentImageMat()->rows)/2.0;
        double newx = resultP.x - centx;
        double newy =  -(resultP.y - centy);
        mag_real_pos3 = RS_Vector(cncmanager->getAbsPos(0), \
                                  cncmanager->getAbsPos(1));
        mag_img_pos3 = RS_Vector(newx,newy);
        mainscreen->drawString(2,QColor(0,255,0),QString("%1 %2 | %3 %4").arg(cncmanager->getAbsPos(0))
                                                    .arg(cncmanager->getAbsPos(1)).arg(newx).arg(newy));


    }
    mainscreen->update();
}

void CCD_SysParEditor::slotIdPos4() {

    CD_Template tmp;
    cv::Mat mat;
    doImgCompensate(*mainscreen->getCurrentImageMat(),mat);

    RS_Vector resultP;
    if(mark->isShapeMode() && !mark->getShapeBaseMatchFlag()) {

        double angle;
        cv::RotatedRect rect;
        RS_Vector PPP;
        resultP= mark->actionIdentifyShapes(mat,mainscreen,&PPP,&angle,&rect,&tmp,false);

    } else {
        resultP = mark->actionIdentify(mat,mainscreen,&tmp);
    }


    if(resultP.valid) {
        double centx = (mainscreen->getCurrentImageMat()->cols)/2.0;
        double centy = (mainscreen->getCurrentImageMat()->rows)/2.0;
        double newx = resultP.x - centx;
        double newy = -(resultP.y - centy);
        mag_real_pos4 = RS_Vector(cncmanager->getAbsPos(0), \
                                  cncmanager->getAbsPos(1));
        mag_img_pos4 = RS_Vector(newx,newy);
        mainscreen->drawString(3,QColor(0,255,0),QString("%1 %2 | %3 %4").arg(cncmanager->getAbsPos(0))
                                                    .arg(cncmanager->getAbsPos(1)).arg(newx).arg(newy));
    }
    mainscreen->update();
}

void CCD_SysParEditor::slotComputeMag() {


    double magy = 0;
    double v1 = mag_img_pos2.distanceTo(mag_img_pos1);
    double v2 = mag_real_pos2.distanceTo(mag_real_pos1);

    if(v2 == 0.0 ) return;
    magy += (v1 / v2);
    this->log(QString("M1 %1").arg(v1 / v2));
    v1 = mag_img_pos3.distanceTo(mag_img_pos2);
    v2 = mag_real_pos3.distanceTo(mag_real_pos2);

    if(v2 == 0.0 ) return;
    magy += (v1 / v2);
    this->log(QString("M2 %1").arg(v1 / v2));
    v1 = mag_img_pos4.distanceTo(mag_img_pos3);
    v2 = mag_real_pos4.distanceTo(mag_real_pos3);

    if(v2 == 0.0 ) return;
    magy += (v1 / v2);
    this->log(QString("M3 %1").arg(v1 / v2));
    v1 = mag_img_pos4.distanceTo(mag_img_pos1);
    v2 = mag_real_pos4.distanceTo(mag_real_pos1);

    if(v2 == 0.0 ) return;
    magy += (v1 / v2);
    this->log(QString("M4 %1").arg(v1 / v2));
    ui->LinE_MagX->setValue(magy/4.0);
    ui->LinE_MagY->setValue(magy/4.0);

    double sum_h[2];
    double final[2];

    double h1 = mag_real_pos2.angleTo(mag_real_pos1);
    double h2 = mag_img_pos1.angleTo(mag_img_pos2);
    sum_h[0] = RS_Math::correctAngle(RS_Math::angleDiff(h2,h1));



    double h3 = mag_real_pos3.angleTo(mag_real_pos4);
    double h4 = mag_img_pos4.angleTo(mag_img_pos3);
    sum_h[1]  = RS_Math::correctAngle(RS_Math::angleDiff(h4,h3));

    final[0] = RS_Math::angleAvg(sum_h,2);

    this->log(QString("h1 h2 %1 %2").arg(h1).arg(h2));
    this->log(QString("h3 h4 %1 %2").arg(h1).arg(h2));
    this->log(QString("sum1 sum2 final1 %1 %2 %3").arg(sum_h[0]).arg(sum_h[1]).arg(final[0]));



    double sum_v[2];
    v1 = mag_real_pos2.angleTo(mag_real_pos3);
    v2 = mag_img_pos3.angleTo(mag_img_pos2);
    sum_v[0] = RS_Math::correctAngle(RS_Math::angleDiff(v2,v1));

    double v3 = mag_real_pos1.angleTo(mag_real_pos4);
    double v4 = mag_img_pos4.angleTo(mag_img_pos1);
    sum_v[1]  = RS_Math::correctAngle(RS_Math::angleDiff(v4,v3));


    final[1] = RS_Math::angleAvg(sum_v,2);


    this->log(QString("v1 v2 %1 %2").arg(v1).arg(v2));
    this->log(QString("v3 v4 %1 %2").arg(v1).arg(v2));
    this->log(QString("sum1 sum2 final1 %1 %2 %3").arg(sum_v[0]).arg(sum_v[1]).arg(final[1]));

    //final[1] = RS_Math::correctAngle(final[1] - M_PI_2);

    this->log(QString("A1 A2 %1 %2").arg(final[0]).arg(final[1]));

    ui->LinE_MagAngle->setValue(RS_Math::angleAvg(final,2));



}

void CCD_SysParEditor::slotSetOffsetX(double set) {
    syspar->offset_h = set;
}

void CCD_SysParEditor::slotSetOffsetY(double set) {
    syspar->offset_v = set;
}

void CCD_SysParEditor::slotSetOffsetAngle(double set) {
    syspar->offset_angle = set;
}

void CCD_SysParEditor::slotRecPositionForOffset1() {

    index_of_offset_center_point = 1;
    QFile file(":runfile/autolearn.ngc");
    QFile fileold("/tmp/autolearn.ngc");
    fileold.remove();
    file.copy("/tmp/autolearn.ngc");
    cncmanager->excuteFile("/tmp/autolearn.ngc",-1);

}

void CCD_SysParEditor::slotRecPositionForOffset2() {

    index_of_offset_center_point = 2;
    QFile file(":runfile/autolearn.ngc");
    QFile fileold("/tmp/autolearn.ngc");
    fileold.remove();
    file.copy("/tmp/autolearn.ngc");
    cncmanager->excuteFile("/tmp/autolearn.ngc",-1);

}


static void getMatrixOfCal(double x1,double y1,double x2,double y2, \
                           double angle) {

    //qDebug()<<x1<<y1<<x2<<y2<<angle;
    double S1,S2;
    double K[2][6];
    S1 = cos(angle) - 1 + (sin(angle)*sin(angle)/(cos(angle) -1));
    S2 = sin(angle)/(cos(angle) -1);
    K[0][0] = x2*cos(angle) - x1 + x2*S2*sin(angle);
    K[0][1] = y2*cos(angle) - y1 + y2*S2*sin(angle);
    K[0][2] = x2*sin(angle) + x1*S2 - x2*S2*cos(angle);
    K[0][3] = y2*sin(angle) + y1*S2 - y2*S2*cos(angle);
    K[0][4] = -S1;
    K[0][5] = 0.0;

    K[1][0] = -x2*sin(angle) + x2*S2*cos(angle) - x1*S2;
    K[1][1] = -y2*sin(angle) + y2*S2*cos(angle) - y1*S2;
    K[1][2] = x2*cos(angle) - x1 + x2*S2*sin(angle);
    K[1][3] = y2 * cos(angle) - y1 + y2*S2*sin(angle);
    K[1][4] = 0.0;
    K[1][5] = -S1;

    for(int i = 0;i<2;i++) {
        for(int j = 0;j<6; j++) {
            printf("%f ,",K[i][j]);
        }
        printf("\n");
    }
}

RS_Vector CCD_SysParEditor::calculateTranCenter(RS_Vector &v1, RS_Vector &v2,
                                                RS_Vector &v3,RS_Vector &v4,
                                                double angle1, double angle2,double angle3) {


    double x1,y1,x2,y2;

    x1 = v1.x;y1 = v1.y;   x2 = v2.x;y2 = v2.y;
    getMatrixOfCal(x1,y1,x2,y2,angle1);


    x1 = v2.x;y1 = v2.y;   x2 = v3.x;y2 = v3.y;
    getMatrixOfCal(x1,y1,x2,y2,angle2 );


    x1 = v3.x;y1 = v3.y;   x2 = v4.x;y2 = v4.y;
    getMatrixOfCal(x1,y1,x2,y2,angle3);




/*
    double S1 = cos(angle1) - 1 + (sin(angle1)*sin(angle1)/(cos(angle1) -1));
    double S2 = sin(angle1)/(cos(angle1) -1);
    double _S1 = cos(angle2) - 1 + (sin(angle2)*sin(angle2)/(cos(angle2) -1));
    double _S2 = sin(angle2)/(cos(angle2) -1);

    double A1 = v2.x * cos(angle1) - v1.x;
    double B1 = v2.y * sin(angle1);
    double C1 = v2.y * cos(angle1) - v1.y;
    double D1 = -v2.x * sin(angle1);

    double _A1 = v4.x * cos(angle2) - v3.x;
    double _B1 = v4.y * sin(angle2);
    double _C1 = v4.y * cos(angle2) - v3.y;
    double _D1 = -v4.x * sin(angle2);

    double FX1 = A1 - D1*S2;
    double FY1 = B1 - C1*S2;
    double FX2 = D1 +A1*S2;
    double FY2 = C1 + B1*S2;

    double _FX1 = _A1 - _D1*_S2;
    double _FY1 = _B1 - _C1*_S2;
    double _FX2 = _D1 + _A1*_S2;
    double _FY2 = _C1 + _B1*_S2;

    double fy = 1/syspar->camera_magy;

    using namespace boost::numeric::ublas;
    const int n = 3;
    permutation_matrix<double> P(n);
    matrix<double> m(n,n);
    vector<double> x(n);
    vector<double> v(n);

    v(0) = fy*(B1 - C1*S2);
    v(1) = fy*(C1 + B1*S2);
    v(2) = fy*(_B1 - _C1*_S2);

    m(0, 0) = S1;
    m(0, 1) = 0;
    m(0, 2) = -FX1;

    m(1, 0) = 0;
    m(1, 1) = S1;
    m(1, 2) = -FX2;

    m(2, 0) = _S1;
    m(2, 1) = 0;
    m(2, 2) = - _FX1;

    try {
        lu_factorize(m,P);
        x = v;
        lu_substitute(m,P,x);
        std::cout << "解向量: " << x << std::endl;
    } catch(...) {
        std::cout << "奇异矩阵(无解或无穷解)." << std::endl;
    }
    return RS_Vector();
*/
    return RS_Vector();
}

RS_Vector CCD_SysParEditor::calculateTranCenter(RS_Vector &v1, RS_Vector &v2, double angle1) {

    if(syspar->camera_magx == 0 || syspar->camera_magy == 0 )
        return RS_Vector();

    double Fx = 1/syspar->camera_magx;
    double Fy = 1/syspar->camera_magy;
    double S1 = cos(angle1) - 1 + (sin(angle1)*sin(angle1)/(cos(angle1) -1));
    double S2 = sin(angle1)/(cos(angle1) -1);
    double A1 = v2.x * cos(angle1) - v1.x;
    double B1 = v2.y * sin(angle1);
    double C1 = v2.y * cos(angle1) - v1.y;
    double D1 = -v2.x * sin(angle1);


    double x0 =  (Fx*(A1 - D1*S2) + Fy*(B1 - C1*S2))/S1;
    double y0 =  (Fx*(D1+A1*S2) + Fy*(C1+B1*S2)) / S1;
    return RS_Vector(x0,y0);

/*
    double x1 = v1.x / syspar->camera_magx;
    double y1 = v1.y / syspar->camera_magy;
    double x2 = v2.x / syspar->camera_magx;
    double y2 = v2.y / syspar->camera_magy;

    double a = x2*cos(angle) - x1 + y2*sin(angle);
    double b = y2*cos(angle) - y1 - x2*sin(angle);

    double c = (sin(angle)*sin(angle)/(cos(angle) -1));
    double d = sin(angle)/(cos(angle) -1);

    double x0 = (a - b*d) / ( cos(angle) - 1 + c );
    double y0 = (b + a*d) / ( cos(angle) - 1 + c );

    return RS_Vector(x0,y0);
*/



}

void CCD_SysParEditor::slotCalculateCenterPoint1() {

    if( !ui->Ckb_C1->isChecked() &&
             !ui->Ckb_C2->isChecked() &&
         !ui->Ckb_C3->isChecked() &&
             !ui->Ckb_C4->isChecked() &&
             !ui->Ckb_C5->isChecked() &&
             !ui->Ckb_C6->isChecked() &&
            !ui->Ckb_C7->isChecked() &&
                        !ui->Ckb_C8->isChecked() &&
                    !ui->Ckb_C9->isChecked() &&
                        !ui->Ckb_C10->isChecked() &&
                        !ui->Ckb_C11->isChecked() &&
                        !ui->Ckb_C12->isChecked() &&
            !ui->Ckb_C13->isChecked() &&
                        !ui->Ckb_C14->isChecked() &&
                    !ui->Ckb_C15->isChecked() ) {

         QMessageBox::warning(this,tr("ERROR"),tr("Please Choose one array at least"),QMessageBox::Ok);


    }


    QList<RS_Vector> local_vcts;
    if(vcts.size() != 15) return;

    if(ui->Ckb_C1->isChecked())
        local_vcts.append(vcts[0]);
    if(ui->Ckb_C2->isChecked())
        local_vcts.append(vcts[1]);
    if(ui->Ckb_C3->isChecked())
        local_vcts.append(vcts[2]);
    if(ui->Ckb_C4->isChecked())
        local_vcts.append(vcts[3]);
    if(ui->Ckb_C5->isChecked())
        local_vcts.append(vcts[4]);
    if(ui->Ckb_C6->isChecked())
        local_vcts.append(vcts[5]);

    if(ui->Ckb_C7->isChecked())
        local_vcts.append(vcts[6]);
    if(ui->Ckb_C8->isChecked())
        local_vcts.append(vcts[7]);
    if(ui->Ckb_C9->isChecked())
        local_vcts.append(vcts[8]);
    if(ui->Ckb_C10->isChecked())
        local_vcts.append(vcts[9]);
    if(ui->Ckb_C11->isChecked())
        local_vcts.append(vcts[10]);
    if(ui->Ckb_C12->isChecked())
        local_vcts.append(vcts[11]);
    if(ui->Ckb_C13->isChecked())
        local_vcts.append(vcts[12]);
    if(ui->Ckb_C14->isChecked())
        local_vcts.append(vcts[13]);
    if(ui->Ckb_C15->isChecked())
        local_vcts.append(vcts[14]);

    RS_Vector center;
    if(local_vcts.size()) {
        for(int i=0;i<local_vcts.size();i++)
            center += local_vcts[i];
        center /= local_vcts.size();
    }
    offset_center_pos1 = center;
    log("---------------------------");
    QString logstr;
    logstr.sprintf("center %03f %03f",offset_center_pos1.x,offset_center_pos1.y);
    log(logstr);

}

void CCD_SysParEditor::slotCalculateCenterPoint2() {

    if( !ui->Ckb_C1->isChecked() &&
             !ui->Ckb_C2->isChecked() &&
         !ui->Ckb_C3->isChecked() &&
             !ui->Ckb_C4->isChecked() &&
             !ui->Ckb_C5->isChecked() &&
             !ui->Ckb_C6->isChecked() &&
            !ui->Ckb_C7->isChecked() &&
                        !ui->Ckb_C8->isChecked() &&
                    !ui->Ckb_C9->isChecked() &&
                        !ui->Ckb_C10->isChecked() &&
                        !ui->Ckb_C11->isChecked() &&
                        !ui->Ckb_C12->isChecked() &&
            !ui->Ckb_C13->isChecked() &&
                        !ui->Ckb_C14->isChecked() &&
                    !ui->Ckb_C15->isChecked() ) {
         QMessageBox::warning(this,tr("ERROR"),tr("Please Choose one array at least"),QMessageBox::Ok);
    }

    QList<RS_Vector> local_vcts;
    if(vcts.size() != 15) return;

    if(ui->Ckb_C1->isChecked())
        local_vcts.append(vcts[0]);
    if(ui->Ckb_C2->isChecked())
        local_vcts.append(vcts[1]);
    if(ui->Ckb_C3->isChecked())
        local_vcts.append(vcts[2]);
    if(ui->Ckb_C4->isChecked())
        local_vcts.append(vcts[3]);
    if(ui->Ckb_C5->isChecked())
        local_vcts.append(vcts[4]);
    if(ui->Ckb_C6->isChecked())
        local_vcts.append(vcts[5]);


    if(ui->Ckb_C7->isChecked())
        local_vcts.append(vcts[6]);
    if(ui->Ckb_C8->isChecked())
        local_vcts.append(vcts[7]);
    if(ui->Ckb_C9->isChecked())
        local_vcts.append(vcts[8]);
    if(ui->Ckb_C10->isChecked())
        local_vcts.append(vcts[9]);
    if(ui->Ckb_C11->isChecked())
        local_vcts.append(vcts[10]);
    if(ui->Ckb_C12->isChecked())
        local_vcts.append(vcts[11]);
    if(ui->Ckb_C13->isChecked())
        local_vcts.append(vcts[12]);
    if(ui->Ckb_C14->isChecked())
        local_vcts.append(vcts[13]);
    if(ui->Ckb_C15->isChecked())
        local_vcts.append(vcts[14]);

    RS_Vector center;
    if(local_vcts.size()) {
        for(int i=0;i<local_vcts.size();i++)
            center += local_vcts[i];
        center /= local_vcts.size();
    }
    offset_center_pos2 = center;
    log("---------------------------");
    QString logstr;
    logstr.sprintf("center2 %03f %03f",offset_center_pos2.x,offset_center_pos2.y);
    log(logstr);

}

void CCD_SysParEditor::slotCalculateCenterPoint(RS_Vector &v) {

    QString logstr;
    RS_Vector v1,center;
    log("---------------------------");
    vcts.clear();

    double angle;
    int count = 0;

    for(int i = 0;i<6;i++) {
        for(int j = i+1;j<6;j++) {
            count ++;
            angle = distanceToAngle( mark_for_offset[j]->getLocateA() - \
                                     mark_for_offset[i]->getLocateA() );

            v1 = calculateTranCenter(offset_pos[i],offset_pos[j],angle/*(angle*M_PI)/180.0*/);
            logstr.sprintf("c%d %03f %03f",count,v1.x,v1.y);
            log(logstr);
            vcts.append(v1);
        }
    }

    if(vcts.size()) {
        for(int i=0;i<vcts.size();i++)
            center += vcts[i];
        center /= vcts.size();
    }
    v = center;

/*
    double angle1 = mark_for_offset[1]->getLocateX() - mark_for_offset[0]->getLocateX();
    if(revert) angle1 = -angle1;
    double angle2 = mark_for_offset[2]->getLocateX() - mark_for_offset[1]->getLocateX();
    if(revert) angle2 = -angle2;
    double angle3 = mark_for_offset[3]->getLocateX() - mark_for_offset[2]->getLocateX();
    if(revert) angle3 = -angle3;

    RS_Vector testp1(-25*sqrt(2.0),50-25*sqrt(2));
    RS_Vector testp2(0,0);
    RS_Vector testp3(25*sqrt(2.0),50-25*sqrt(2));
    RS_Vector testp4(50,50);


    calculateTranCenter(testp1,testp2,testp3,testp4,
                        (45*M_PI)/180.0,(45*M_PI)/180.0,(45*M_PI)/180.0);
**/

}

void CCD_SysParEditor::slotSaveMark() {

}

void CCD_SysParEditor::slotCncAbort() {

    cncmanager->abort();
//    RS_Vector testp1(-25*sqrt(2.0),50-25*sqrt(2));
//    RS_Vector testp2(0,0);
//    RS_Vector testp3(25*sqrt(2.0),50-25*sqrt(2));
//    RS_Vector testp4(50,50);

//    calculateTranCenter(testp1,testp2,testp3,testp4,
//                        (45*M_PI)/180.0,(45*M_PI)/180.0,(45*M_PI)/180.0);

}

void CCD_SysParEditor::slotCalculateOffset() {

    double angle = offset_center_pos2.angleTo(offset_center_pos1);
    qDebug()<<"         angle = "<<angle<<angle - M_PI_2;
    angle -= M_PI_2;
    double x0 = offset_center_pos1.x  + offset_length1*sin(angle);
    double y0 = offset_center_pos1.y  -  offset_length1*cos(angle);

    ui->LinE_OffsetAngle->setValue(RS_Math::correctAngle(angle));
    ui->LinE_OffsetX->setValue(x0);
    ui->LinE_OffsetY->setValue(y0);

    if(syspar->camera_magx != 0.0)
    qDebug()<<"   real_length"<<offset_length2-offset_length1
           <<(offset_center_pos1.distanceTo(offset_center_pos2));

}

void CCD_SysParEditor::slotGetMarkForOffset1() {
    slotGetMark(0);
}

void CCD_SysParEditor::slotGetMarkForOffset2() {
    slotGetMark(1);
}

void CCD_SysParEditor::slotGetMarkForOffset3() {
    slotGetMark(2);
}

void CCD_SysParEditor::slotGetMarkForOffset4() {
    slotGetMark(3);
}

void CCD_SysParEditor::slotGetMarkForOffset5() {
    slotGetMark(4);
}

void CCD_SysParEditor::slotGetMarkForOffset6() {
    slotGetMark(5);
}


void CCD_SysParEditor::slotGetMarkForThrRad1() {
    slotGetMark(6);
}

void CCD_SysParEditor::slotGetMarkForThrRad2() {
    slotGetMark(7);
}

void CCD_SysParEditor::slotGetMarkForThrRad3() {
    slotGetMark(8);
}

void CCD_SysParEditor::slotGetMarkForThrRad4() {
    slotGetMark(9);
}


void CCD_SysParEditor::slotIdMarkForThrRad1() {
    in_img_compensate = true;
    slotIdMarkForOffset(6);
}
void CCD_SysParEditor::slotIdMarkForThrRad2() {
    in_img_compensate = true;
    slotIdMarkForOffset(7);
}
void CCD_SysParEditor::slotIdMarkForThrRad3() {
    in_img_compensate = true;
    slotIdMarkForOffset(8);
}
void CCD_SysParEditor::slotIdMarkForThrRad4() {
    in_img_compensate = true;
    slotIdMarkForOffset(9);
}

void CCD_SysParEditor::slotIdMarkForOffset1() {
    in_img_compensate = false;
    slotIdMarkForOffset(0);
    mark_for_offset[0]->setLocateA(cncmanager->getAbsPos(3));
}
void CCD_SysParEditor::slotIdMarkForOffset2() {
    in_img_compensate = false;
    slotIdMarkForOffset(0);
   mark_for_offset[1]->setLocateA(cncmanager->getAbsPos(3));
   //qDebug()<<"mark_for_offset[1]"<<cncmanager->getAbsPos(3);

}
void CCD_SysParEditor::slotIdMarkForOffset3() {
    in_img_compensate = false;
    slotIdMarkForOffset(0);
    mark_for_offset[2]->setLocateA(cncmanager->getAbsPos(3));
    //qDebug()<<"mark_for_offset[2]"<<cncmanager->getAbsPos(3);
}
void CCD_SysParEditor::slotIdMarkForOffset4() {
    in_img_compensate = false;
    slotIdMarkForOffset(0);
    mark_for_offset[3]->setLocateA(cncmanager->getAbsPos(3));
}

void CCD_SysParEditor::slotIdMarkForOffset5() {
    in_img_compensate = false;
    slotIdMarkForOffset(0);
    mark_for_offset[4]->setLocateA(cncmanager->getAbsPos(3));
}
void CCD_SysParEditor::slotIdMarkForOffset6() {
    in_img_compensate = false;
    slotIdMarkForOffset(0);
    mark_for_offset[5]->setLocateA(cncmanager->getAbsPos(3));
}


void CCD_SysParEditor::slotIdMarkForOffset(int index)  {

    if(index>10) return;

    CD_Template tmp;
    CCD_Mark *localmark = mark_for_offset[index];

    cv::Mat mat;
    if(!in_img_compensate)
        doImgCompensate(*mainscreen->getCurrentImageMat(),mat);
    else
        mat = *mainscreen->getCurrentImageMat();

    RS_Vector resultP = localmark->actionIdentify(mat,mainscreen,&tmp);

    if(resultP.valid) {

        double centx = (mainscreen->getCurrentImageMat()->cols)/2.0;
        double centy = (mainscreen->getCurrentImageMat()->rows)/2.0;
        double newx = resultP.x - centx;
        double newy =  -(resultP.y - centy);

        double realpx = 0.0;
        double realpy = 0.0;

        if(in_img_compensate)
            imgtran_pos[index-6] = resultP;

        if(syspar->camera_magx == 0. ||syspar->camera_magy == 0.) {
            mainscreen->drawString(1,QColor(255,0,0),tr("错误：放大倍率为零 !"));
        }  else {
            realpx = newx/syspar->camera_magx;
            realpy = newy/syspar->camera_magy;
            //offset_pos2 = RS_Vector(realpx,realpy);
            mainscreen->drawString(1,QColor(0,255,0),QString("x= %1 y= %2").arg(realpx).arg(realpy));
        }

    } else {
        mainscreen->drawString(1,QColor(255,0,0),tr("识别失败"));
    }
    mainscreen->update();

}

void CCD_SysParEditor::slotSetGlbOffsetH(double d) {
    syspar->global_offset_h = d;
}

void CCD_SysParEditor::slotSetGlbOffsetV(double d) {
    syspar->global_offset_v = d;
}

void CCD_SysParEditor::slotRecPositionGlbOffset() {

    //glboffset_poscnt = RS_Vector(cncmanager->getAbsPos(0),
    //                             cncmanager->getAbsPos(1));

    ui->LinE_CntPosX->setValue(cncmanager->getAbsPos(0));
    ui->LinE_CntPosY->setValue(cncmanager->getAbsPos(1));


}

void CCD_SysParEditor::slotGetGlbOffsetMark() {
    slotGetMark(-1);
}


void CCD_SysParEditor::slotIdPositionGlbOffset() {

    CD_Template tmp;
    cv::Mat mat;
    doImgCompensate(*mainscreen->getCurrentImageMat(),mat);

    RS_Vector resultP;
    if(mark->isShapeMode() && !mark->getShapeBaseMatchFlag()) {

        double angle;
        cv::RotatedRect rect;
        RS_Vector PPP;
        resultP= mark->actionIdentifyShapes(mat,mainscreen,&PPP,&angle,&rect,&tmp,false);

    } else {
        resultP = mark->actionIdentify(mat,mainscreen,&tmp);
    }

    double centx = (mainscreen->getCurrentImageMat()->cols)/2.0;
    double centy = (mainscreen->getCurrentImageMat()->rows)/2.0;

    double newx = resultP.x - centx;
    double newy = -(resultP.y - centy);

     newx = 0;
     newy = 0;

    resultP.valid = true;

    if( resultP.valid ) {
        glboffset_posimg = RS_Vector(newx,newy);
        mainscreen->drawString(0,QColor(0,255,0),QString("%1 %2 ").arg(newx).arg(newy));
    } else {
        mainscreen->drawString(0,QColor(0,255,0),"Fail~");
    }
    mainscreen->update();
}

void CCD_SysParEditor::slotCalculateGlbOffset() {


    glboffset_posimg = RS_Vector(0.0, 0.0);

    qDebug()<<"glboffset_posimg"<<glboffset_posimg.x<<glboffset_posimg.y;
    qDebug()<<"glboffset_poscnt"<<glboffset_poscnt.x<<glboffset_poscnt.y;

    if(syspar->camera_magx == 0 || syspar->camera_magy == 0) {
        mainscreen->drawString(0,QColor(255,0,0),"camera_magx and camera_magy is zero");
        return ;
    }

    double x0 = glboffset_posimg.x/syspar->camera_magx;
    double y0 = glboffset_posimg.y/syspar->camera_magy;
    double t = syspar->offset_angle;

    //X' = x*cost + y*sint - x0*cost -y0*sint
    //Y' = y*cost - x*sint + x0*sint - y0*cost

    double X = - x0*cos(t) -y0*sin(t);
    double Y = x0*sin(t) - y0*cos(t);

    log(QString("XY= %1 %2").arg(X).arg(Y));

    double curX = cncmanager->getAbsPos(0);
    double curY = cncmanager->getAbsPos(1);

    curX -= X;
    curY -= Y;

    log(QString("CurXY= %1 %2").arg(curX).arg(curY));

    double par_off_x = glboffset_poscnt.x - curX;
    double par_off_y = glboffset_poscnt.y - curY;

    ui->LinE_GlbOffsetH->setValue(par_off_x);
    ui->LinE_GlbOffsetV->setValue(par_off_y);


}



void CCD_SysParEditor::timerEvent(QTimerEvent *) {

    static CCD_CncManager::CNC_STATUS_TYPE old_status = CCD_CncManager::IDLE;
    if(cncmanager->getCncStatus() != old_status ) {
        bool enable = true;
        if(cncmanager->getCncStatus() == CCD_CncManager::IDLE)
            enable = true;
        else
            enable = false;
        ui->stackedWidget->setEnabled(enable);
        /*
        ui->Btn_1->setEnabled(enable);
        ui->Btn_01->setEnabled(enable);
        ui->Btn_001->setEnabled(enable);
        ui->Btn_0001->setEnabled(enable);
        ui->Btn_Xminus->setEnabled(enable);
        ui->Btn_Yminus->setEnabled(enable);
        ui->Btn_Zminus->setEnabled(enable);
        ui->Btn_Yplus->setEnabled(enable);
        ui->Btn_Xplus->setEnabled(enable);
        ui->Btn_Zplus->setEnabled(enable);
        ui->Btn_Handle->setEnabled(enable);
        ui->Btn_Cont->setEnabled(enable);
        */
        //ui->Btn_Cancel->setEnabled(enable);
        //ui->Btn_Sure->setEnabled(enable);
    }
    old_status = cncmanager->getCncStatus();

}

void CCD_SysParEditor::doImgCompensate(cv::Mat &mat, cv::Mat &To) {

    To = mat;
    return;
    if( syspar->compensate_img_enable ) {
            double local[3][3];
            for(int i=0;i<3;i++) {
                for(int j=0;j<3;j++) {
                    local[i][j] = syspar->compensate_mat[i][j];
                }
            }
            qDebug()<<"------do warpPerspective------------";
            cv::Mat C = (cv::Mat_<double>(3,3) << local[0][0], local[0][1], local[0][2],
                    local[1][0], local[1][1], local[1][2],
                    local[2][0], local[2][1], local[2][2]);
            cv::warpPerspective(mat, To, C, mat.size(), cv::INTER_LINEAR,
                                cv::BORDER_CONSTANT,
                                cv::Scalar(255));
            qDebug()<<"------------end cal warpPerspective"<<QTime::currentTime().second()<<QTime::currentTime().msec();
    } else {
        To = mat;
    }

}

void CCD_SysParEditor::drawIdResult(CD_MainScreen *m, CD_Template *t, RS_Vector *Result) {

    CD_Template *tmp = t;
    CD_MainScreen *mainscreen = m;
    RS_Vector resultP  = *Result;
    int locatx = tmp->getLocateInParentX();
    int locaty = tmp->getLocateInParentY();
    int width = tmp->getWidth();
    int height = tmp->getHeight();
    QRectF recf(locatx,locaty,width,height);
    QList<RS_Line> list;
    RS_Vector v1,v2;
    mainscreen->resetRects();
    mainscreen->resetLines();
    if(resultP.valid) {

        v1.x = tmp->getLocateInParentX()+resultP.x - 70;
        v1.y = tmp->getLocateInParentY() +resultP.y;
        v2.x = tmp->getLocateInParentX()+resultP.x + 70;
        v2.y = tmp->getLocateInParentY() +resultP.y;
        list.append(RS_Line(v1,v2));
        v1.x = tmp->getLocateInParentX()+resultP.x;
        v1.y = tmp->getLocateInParentY() +resultP.y-70;
        v2.x = tmp->getLocateInParentX()+resultP.x;
        v2.y = tmp->getLocateInParentY() +resultP.y+70;
        list.append(RS_Line(v1,v2));
        resultP.x = tmp->getLocateInParentX()+resultP.x;
        resultP.y = tmp->getLocateInParentY()+resultP.y;
        mainscreen->drawRect(recf);
        mainscreen->drawLines(list);
    }

}


/*
void CCD_SysParEditor::slotEnableImgCompensate(bool b) {
    syspar->compensate_img_enable = b;
}

void CCD_SysParEditor::slotImgTranMatCompute() {


    double distancex = imgtran_pos[0].distanceTo(imgtran_pos[1]);
    double distancey = ((distancex*compensate_offsety)/compensate_offsetx);

    RS_Vector v1 ;
    v1.x =imgtran_pos[0].x;
    v1.y =-imgtran_pos[0].y;
    RS_Vector v2 ;
    v2.x =imgtran_pos[1].x;
    v2.y =-imgtran_pos[1].y;

    double t = v1.angleTo(v2);

    cv::Point2f src[4]; //源图像中的点
    cv::Point2f dst[4]; //目标图像中的点
    for(int i=0;i<4;i++) {
        src[i].x = imgtran_pos[i].x;
        src[i].y = imgtran_pos[i].y;
    }


    dst[0].x = imgtran_pos[0].x;
    dst[0].y = imgtran_pos[0].y;
    dst[1].x = imgtran_pos[0].x+distancex;
    dst[1].y = imgtran_pos[0].y;

    dst[2].x = imgtran_pos[0].x+distancex;
    dst[2].y = imgtran_pos[0].y+distancey;;
    dst[3].x = imgtran_pos[0].x;
    dst[3].y = imgtran_pos[0].y+distancey;

    //变换矩阵

    tran_mat = cv::getPerspectiveTransform(src,dst);
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            syspar->compensate_mat[i][j] =  tran_mat.at<double>(i,j);
       }
    }

    ///fang she bian huan
    //cv::Point2f srcTri[3];
    //cv::Point2f dstTri[3];
    //for(int i=0;i<3;i++) {
    //    srcTri[i].x = imgtran_pos[i].x;
    //    srcTri[i].y = imgtran_pos[i].y;
    //}
    //dstTri[0].x = imgtran_pos[0].x;
    //dstTri[0].y = imgtran_pos[0].y;
    //dstTri[1].x = imgtran_pos[0].x+distancex;
    //dstTri[1].y = imgtran_pos[0].y;
    //dstTri[2].x = imgtran_pos[0].x+distancex;
    //dstTri[2].y = imgtran_pos[0].y+distancey;;
    //tran_mat = cv::getAffineTransform( srcTri, dstTri );
    //for(int i=0;i<2;i++) {
    //    for(int j=0;j<3;j++) {
    //        syspar->compensate_mat[i][j] =  tran_mat.at<double>(i,j);
    //    }
    //}


}

void CCD_SysParEditor::slotReadAffinePoints() {

    log("---------------------------");
    QString str;
    str.sprintf("p1= %03f %03f",imgtran_pos[0].x,imgtran_pos[0].y);
    log(str);
    str.sprintf("p2= %03f %03f",imgtran_pos[1].x,imgtran_pos[1].y);
    log(str);
    str.sprintf("p3= %03f %03f",imgtran_pos[2].x,imgtran_pos[2].y);
    log(str);
    str.sprintf("p4= %03f %03f",imgtran_pos[3].x,imgtran_pos[3].y);
    log(str);

}

void CCD_SysParEditor::slotDoImgTran() {

    if(!syspar->compensate_img_enable) return;
    cv::Mat mat_affine;
    cv::Mat mat ;
    mainscreen->getCurrentImageMat()->copyTo(mat); ;
    doImgCompensate(mat,mat_affine);
    showInMainScreen(mat_affine);


}

void CCD_SysParEditor::slotSetRectWidth(double d) {
    compensate_offsetx  = d;

}

void CCD_SysParEditor::slotSetRectHeight(double d) {
    compensate_offsety  = d;

}

*/


void CCD_SysParEditor::log(const QString &str) {
    ui->Brs_Log->appendPlainText(str);
}

void CCD_SysParEditor::slotSetThrRadius(double d) {
    syspar->two_points_w = d;
}

double CCD_SysParEditor::distanceToAngle(double d) {

    qDebug()<<"CCD_SysParEditor::distanceToAngle"<<d<<d/syspar->two_points_w;
    return d/syspar->two_points_w;

}

double CCD_SysParEditor::angleThreePoint(const RS_Vector &p1,
                                       const RS_Vector &p2,
                                       const RS_Vector &p3) {

    RS_Vector v1 = p2 - p1;
    RS_Vector v2 = p3 - p1;
    double mag_v1 = v1.magnitude();
    double mag_v2 = v2.magnitude();
    double ab = v1.x *v2.x + v1.y*v2.y;
    double angle = acos( ab/(mag_v1*mag_v2) );

    if( _isnan(angle) )
        return 0.0;
    else
        return angle;

}


void CCD_SysParEditor::slotThrRadiusCompute() {

    RS_Vector c1,c2,c3,c4,center;
    RS_Circle circle(0,0,0);
    circle.createFrom3P(imgtran_pos[0],imgtran_pos[1],imgtran_pos[2]);
    c1 = circle.getCenter();
    circle.createFrom3P(imgtran_pos[0],imgtran_pos[1],imgtran_pos[3]);
    c2 = circle.getCenter();
    circle.createFrom3P(imgtran_pos[0],imgtran_pos[2],imgtran_pos[3]);
    c3 = circle.getCenter();
    circle.createFrom3P(imgtran_pos[1],imgtran_pos[2],imgtran_pos[3]);
    c4 = circle.getCenter();

    QString str;
    str.sprintf("c1 %.3f %.3f",c1.x,c1.y);
    log(str);
    str.sprintf("c2 %.3f %.3f",c2.x,c2.y);
    log(str);
    str.sprintf("c3 %.3f %.3f",c3.x,c3.y);
    log(str);
    str.sprintf("c4 %.3f %.3f",c4.x,c4.y);
    log(str);
    center = (c1+c2+c3+c4)/4.0;
    str.sprintf("center %.3f %.3f",center.x,center.y);
    log(str);

    double angle1,angle2,angle3;
    double radius1,radius2,radius3;
    angle1 = angleThreePoint(center,imgtran_pos[0],imgtran_pos[1]);
    angle2 = angleThreePoint(center,imgtran_pos[1],imgtran_pos[2]);
    angle3 = angleThreePoint(center,imgtran_pos[2],imgtran_pos[3]);

    double distance1 = fabs(mark_for_offset[6]->getLocateA() - mark_for_offset[7]->getLocateA());
    double distance2 = fabs(mark_for_offset[7]->getLocateA() - mark_for_offset[8]->getLocateA());
    double distance3 = fabs(mark_for_offset[8]->getLocateA() - mark_for_offset[9]->getLocateA());

    radius1 = distance1/angle1;
    radius2 = distance2/angle2;
    radius3 = distance3/angle3;

    str.sprintf("radius1 %.3f \nradius2 %.3f \nradius3 %.3f ",radius1,radius2,radius3);
    log(str);

    ui->DSB_ThrRadius->setValue( (radius1+radius2+radius3) / 3.0 );

}

void CCD_SysParEditor::on_CB_Resolution_activated(int index) {

    syspar->img_resolution = index;
    int w,h;
    capture->setCameraResolution(index,0,&w,&h);
    log(QString("%1 %2").arg(w).arg(h));

}

void CCD_SysParEditor::on_LinE_CntPosX_valueChanged(double arg1)
{
    glboffset_poscnt.x = arg1;
}

void CCD_SysParEditor::on_LinE_CntPosY_valueChanged(double arg1)
{
    glboffset_poscnt.y = arg1;
}

void CCD_SysParEditor::on_SB_ZHeight_valueChanged(double arg1)
{
    syspar->camera_height = arg1;
}

void CCD_SysParEditor::on_Btn_SetCameraName_clicked()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Change The Name Is Dangerous"));
    msgBox.setInformativeText(tr("Sure To Modify?"));
    msgBox.setStandardButtons(QMessageBox::Ok |QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Ok:
          capture->setCameraName(ui->LE_CameraName->text().toUtf8().data());
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }

}
