#include "ccd_markeditor.h"
#include "ui_ccd_editwidget.h"
#include "ccd_mark.h"
#include "cd_application.h"
#include "ccd_cncmanager.h"
#include "camera.h"
#include "cd_identify.h"
#include "rs_debug.h"
#include "rs_doubleline.h"

#include "cd_crosscomp.h"

#include <QMessageBox>
#include <QDebug>

int CCD_MarkEditor::exe_ret = 0;

CCD_MarkEditor::CCD_MarkEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCD_MarkEditor) {

    ui->setupUi(this);
    connect(ui->Mode_Auto,SIGNAL(clicked()),this,SLOT(slotSetOperModeAuto()));
    connect(ui->Mode_Man,SIGNAL(clicked()),this,SLOT(slotSetOperModeManual()));

    connect(ui->Type_Angle,SIGNAL(clicked()),this,SLOT(slotSetFeatureAngle()));
    connect(ui->Type_Circle,SIGNAL(clicked()),this,SLOT(slotSetFeatureCircle()));
    connect(ui->Type_Cross,SIGNAL(clicked()),this,SLOT(slotSetFeatureCross()));
    connect(ui->Type_Relate,SIGNAL(clicked()),this,SLOT(slotSetFeatureRelate()));
    connect(ui->Type_Shape,SIGNAL(clicked()),this,SLOT(slotSetFeatureShape()));

    connect(ui->Btn_Identify,SIGNAL(clicked()),this,SLOT(slotTryIdentify()));
    connect(ui->Btn_Sure,SIGNAL(clicked()),this,SLOT(slotDetermineFeature()));
    connect(ui->Btn_GetTmp,SIGNAL(clicked()),this,SLOT(slotDoSelectTemplate()));
    connect(ui->Btn_Return,SIGNAL(clicked()),this,SLOT(close()));

    connect(ui->Template_Sure,SIGNAL(clicked()),this,SLOT(slotDetermineTemplate()));
    connect(ui->Template_Cancel,SIGNAL(clicked()),this,SLOT(slotForceReturnSelect()));

    setWindowTitle(tr("Mark-Parameter"));

    mainscreenplace = ui->Edit_View;
    cncmanager = new CCD_CncManager();

    shape_base_type = CD::FeatureRelate;

    match_par_dlg = new QDialog(this);
    if(match_par_dlg->layout() == NULL) {
        QVBoxLayout *lay2 = new QVBoxLayout(match_par_dlg);
        lay2->setContentsMargins(0,0,0,0);
        match_par_dlg->setLayout(lay2);
    }
    ui->Search_More_Frame->setParent(match_par_dlg);
    match_par_dlg->layout()->addWidget(ui->Search_More_Frame);
    match_par_dlg->adjustSize();
    connect(ui->Btn_MatchParSure,SIGNAL(pressed()),match_par_dlg,SLOT(close()));

    one_reference = true;

}

CCD_MarkEditor::~CCD_MarkEditor() {

    delete ui;

}

void CCD_MarkEditor::setCapture(CD_CaptureImage *c) {
    capture = c;
}

void CCD_MarkEditor::setCncManager(CCD_CncManager *m) {

    cncmanager = m;

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

    connect(ui->Btn_Xplus,SIGNAL(pressed()),cncmanager,SLOT(jogXplusStart()));
    connect(ui->Btn_Xplus,SIGNAL(released()),cncmanager,SLOT(jogXEnd()));
    connect(ui->Btn_Yplus,SIGNAL(pressed()),cncmanager,SLOT(jogYplusStart()));
    connect(ui->Btn_Yplus,SIGNAL(released()),cncmanager,SLOT(jogYEnd()));
    connect(ui->Btn_Zplus,SIGNAL(pressed()),cncmanager,SLOT(jogZplusStart()));
    connect(ui->Btn_Zplus,SIGNAL(released()),cncmanager,SLOT(jogZEnd()));


}

void CCD_MarkEditor::showInRealTime(int camera,cv::Mat &mat) {

    if(CD_CaptureImage::CameraInitok == false) return;
    if(mark->getCameraId() != camera) return;
    mark->getScreenTemplate()->showMatImage(mat);

}

void CCD_MarkEditor::showInRealTime(cv::Mat &mat) {

    //if(CD_CaptureImage::CameraInitok == false) return;
    mark->getScreenTemplate()->showMatImage(mat);

}

void CCD_MarkEditor::init(CCD_Mark *m, bool one_ref) {

    putImageScreenToWidget(m->getScreenFeature(),mainscreenplace);
    putImageScreenToWidget(m->getScreenTemplate(),mainscreenplace);

    if(m->getFeatureOperMode() == CD::OperAuto) {
        ui->Mode_Man->setChecked(false);
        ui->Mode_Auto->setChecked(true);
    } else {
        ui->Mode_Auto->setChecked(false);
        ui->Mode_Man->setChecked(true);
    }

    if(m->getFeatureType()  == CD::FeatureAngle) {
        ui->Type_Angle->setChecked(true);
    } else if(m->getFeatureType() == CD::FeatureCross) {
        ui->Type_Cross->setChecked(true);
    } else if(m->getFeatureType() == CD::FeatureCircle) {
        ui->Type_Circle->setChecked(true);
    } else if(m->getFeatureType() == CD::FeatureRelate) {
        ui->Type_Relate ->setChecked(true);
    }
    ui->Jog_speed->setValue(cncmanager->getJogSpeed());
    mark = m;

    slotUpdateFeatures();
    exe_ret = -1;

    slotDoSelectTemplate();

    if(mark->getEnableCameraPar()) {

        ui->CameraContrast->setValue(mark->getCameraContrast());
        ui->CameraExposeTime->setValue(mark->getCameraExposeTime());
        ui->CameraGain->setValue(mark->getCameraGain());
        ui->CameraGamma->setValue(mark->getCameraGamma());

        ui->Light1->setChecked(mark->getLightEnable1());
        ui->Light2->setChecked(mark->getLightEnable2());
        ui->Light3->setChecked(mark->getLightEnable3());

    } else {

        ui->Frame_CameraPar->hide();
        ui->Frame_Light->hide();

    }

    ui->Type_Shape->setChecked(mark->shape_mode);
    ui->CKB_Calcenter->setChecked(mark->shape_cal_center);
    ui->SB_SearchLen->setValue(mark->getTemplateImage()->getWidth()/3);
    index_of_shapes = -1;
    mark->search_shape_len = mark->getScreenTemplate()->getCurrentImage()->getWidth()/4;

    ui->SB_Sim_Threshold->setValue(mark->shape_sim_threshold);
    ui->SB_Wh_Threshold->setValue(mark->shape_wh_threshold);
    ui->SB_SearchLen->setValue(mark->search_shape_len);

    updateEditTemplateScreen();
    slotSetFeatureShape();
    mark->getScreenFeature()->resetCircles();
    mark->getScreenFeature()->resetLines();


    ui->DB_SimThreshold_SH->setValue(mark->getSimThresholdShape());

    ui->Canny_Threhold->setValue(mark->canny_threshold_low+5);
    ui->Thre_Show->setValue(mark->canny_threshold_low+5);

    ui->Type_Angle_2->hide();

    if(!mark->getShapeBaseMatchFlag()) {
        ui->Btn_Mode_Change->setText(QString::fromLocal8Bit("特征模式"));
        mark->shape_base_match_flag = false;
        ui->stackedWidget_Mode->setCurrentIndex(0);
    } else {
        slotSetOperModeAuto();
        ui->Mode_Auto->setChecked(true);
        mark->getCircleMark()->hide();
        mark->getCossMark()->hide();
        mark->getAngleMark()->hide();
        mark->getRelateMark()->show();
        shape_base_type = CD::FeatureRelate;
        ui->Btn_Mode_Change->setText(QString::fromLocal8Bit("形状模式"));
        mark->shape_base_match_flag = true;
        ui->stackedWidget_Mode->setCurrentIndex(1);
        ui->Type_Relate_2->setChecked(true);
    }

    ui->Search_Step->setValue(mark->match_par.search_angle_step);

    ui->Search_Enable1->setChecked(false);
    ui->Search_Enable2->setChecked(false);
    ui->Search_Enable3->setChecked(false);
    ui->Search_Enable4->setChecked(false);

    ui->Search_Start1->setValue(0.0);
    ui->Search_End1->setValue(0.0);
    ui->Search_Start2->setValue(0.0);
    ui->Search_End2->setValue(0.0);
    ui->Search_Start3->setValue(0.0);
    ui->Search_End3->setValue(0.0);
    ui->Search_Start4->setValue(0.0);
    ui->Search_End4->setValue(0.0);

    if( mark->match_par.search_angles_start.size() !=
            mark->match_par.search_angles_end.size() ) {

        QMessageBox::warning(this,QObject::tr("warning"),\
                             QObject::tr("mark.match_par is wrong"));

    } else {

        if( mark->match_par.search_angles_start.size() > 4) {
            while( mark->match_par.search_angles_start.size() > 4 ) {
                mark->match_par.search_angles_end.pop_back();
                mark->match_par.search_angles_start.pop_back();
            }
        }

        int size = mark->match_par.search_angles_start.size();
        if( size == 1 ) {

            ui->Search_Enable1->setChecked(true);

            ui->Search_Start1->setValue(mark->match_par.search_angles_start[0]);
            ui->Search_End1->setValue(mark->match_par.search_angles_end[0]);

        } else if( size == 2 ) {

            ui->Search_Enable1->setChecked(true);
            ui->Search_Enable2->setChecked(true);

            ui->Search_Start1->setValue(mark->match_par.search_angles_start[0]);
            ui->Search_End1->setValue(mark->match_par.search_angles_end[0]);
            ui->Search_Start2->setValue(mark->match_par.search_angles_start[1]);
            ui->Search_End2->setValue(mark->match_par.search_angles_end[1]);


        } else if( size == 3 ) {

            ui->Search_Enable1->setChecked(true);
            ui->Search_Enable2->setChecked(true);
            ui->Search_Enable3->setChecked(true);

            ui->Search_Start1->setValue(mark->match_par.search_angles_start[0]);
            ui->Search_End1->setValue(mark->match_par.search_angles_end[0]);
            ui->Search_Start2->setValue(mark->match_par.search_angles_start[1]);
            ui->Search_End2->setValue(mark->match_par.search_angles_end[1]);
            ui->Search_Start3->setValue(mark->match_par.search_angles_start[2]);
            ui->Search_End3->setValue(mark->match_par.search_angles_end[2]);

        } else if( size == 4 ) {

            ui->Search_Enable1->setChecked(true);
            ui->Search_Enable2->setChecked(true);
            ui->Search_Enable3->setChecked(true);
            ui->Search_Enable4->setChecked(true);

            ui->Search_Start1->setValue(mark->match_par.search_angles_start[0]);
            ui->Search_End1->setValue(mark->match_par.search_angles_end[0]);
            ui->Search_Start2->setValue(mark->match_par.search_angles_start[1]);
            ui->Search_End2->setValue(mark->match_par.search_angles_end[1]);
            ui->Search_Start3->setValue(mark->match_par.search_angles_start[2]);
            ui->Search_End3->setValue(mark->match_par.search_angles_end[2]);
            ui->Search_Start4->setValue(mark->match_par.search_angles_start[3]);
            ui->Search_End4->setValue(mark->match_par.search_angles_end[3]);

        }

    }

    ui->Search_PyNum->setValue(mark->match_par.pyramid_num);
    ui->Search_AngleNbh->setValue(mark->match_par.angle_search_width);
    ui->Search_RectNbh->setValue(mark->match_par.rect_search_width);

    ui->crossCenter->setChecked(true);

    ui->Search_Blur_Size->setValue(mark->match_par.median_blur_size);
    ui->Search_Enable_Filter->setChecked(mark->match_par.process_enable);
    ui->Search_Erosion_Size->setValue(mark->match_par.erosion_size);

    ui->Widget_Tools->hide();
    ui->Enraser_Size->setValue(mark->getEnraserMark()->getEnraseSize());
    ui->EditMore->hide();

    one_reference = one_ref;
    if( one_reference ) {
        mark->relate_location_x = mark->relate_location_x2;
        mark->relate_location_y = mark->relate_location_y2;
        ui->Btn_CalRefPoint2->hide();
    }
    drawReferencePoints();

}



void CCD_MarkEditor::slotSetAnglePos(CD::AnglePos) {

}

void CCD_MarkEditor::slotTestFeature() {

    slotUpdateFeatures();

}

void CCD_MarkEditor::slotTryIdentify() {

    if( mark->shape_mode ) {

        releaseListWidgetData();
        std::vector<cv::RotatedRect> ret;
        all_shapes_path.clear();
        mark->actionIdentifyShapes(*mark->getTemplateImage()->returnAsMat(),all_shapes_path,ret);
        all_shapes = ret;
        std::vector<cv::RotatedRect>::iterator it = ret.begin();
        int count = 0;
        for(;it != ret.end(); it++) {
            count ++;
            QListWidgetItem * item = new QListWidgetItem(QString("杞粨:%1").arg(count));
            ui->listWidget->addItem(item);
        }


    } else {

        if( mark->getFeatureType() == CD::FeatureNone) {
           QMessageBox::warning(this,QObject::tr("warning"),QObject::tr("please select a type"));
           return;
        }
        if(mark ->getFeatureOperMode() == CD::OperAuto)
            mark->actionTestFeature();
          else
            mark->actionTryIdentify();
        slotUpdateFeatures();

    }

}

void CCD_MarkEditor::slotSetOperModeAuto() {

    mark->actionSetOperMode(CD::OperAuto);
    slotUpdateFeatures();
    ui->Frame_Mannul->hide();

}
void CCD_MarkEditor::slotSetOperModeManual() {

    mark->actionSetOperMode(CD::OperManual);
    slotUpdateFeatures();
    ui->Frame_Mannul->show();

}

void CCD_MarkEditor::slotSetFeatureAngle() {

    mark->actionSetFeatureType(CD::FeatureAngle);
    slotUpdateFeatures();
    slotSetFeatureShape();
}

void CCD_MarkEditor::slotSetFeatureCircle() {

    mark->actionSetFeatureType(CD::FeatureCircle);
    slotUpdateFeatures();
    slotSetFeatureShape();

}

void CCD_MarkEditor::slotSetFeatureRelate() {

    mark->actionSetFeatureType(CD::FeatureRelate);
    slotUpdateFeatures();
    slotSetFeatureShape();
}

void CCD_MarkEditor::slotSetFeatureShape() {

        if( ui->Type_Shape->isChecked() )
            mark->shape_mode = true;
        else
            mark->shape_mode = false;
        if(mark->shape_mode) {
            ui->Frame_Shape->show();
            ui->Frame_Mannul->hide();
        } else {
            ui->Frame_Shape->hide();
            if(mark->getFeatureOperMode() == CD::OperAuto)
                ui->Frame_Mannul->hide();
            else
                ui->Frame_Mannul->show();
        }

}

void CCD_MarkEditor::slotSetFeatureCross() {

    mark->actionSetFeatureType(CD::FeatureCross);
    slotUpdateFeatures();
    slotSetFeatureShape();

}

void CCD_MarkEditor::slotTrigleAnglePos() {


    if(mark->getFeatureOperMode() == CD::OperAuto)
        return;
    if(mark->getFeatureType() == CD::FeatureNone)
        return;

    CD::AnglePos pos = mark->getAnglePos();
    if(pos == CD::TopLeft) {
        mark->actionSetAnglePos(CD::TopRight);
    }
    else if(pos == CD::TopRight) {
        mark->actionSetAnglePos(CD::BottomRight);
    }
    else if(pos == CD::BottomRight) {
        mark->actionSetAnglePos(CD::BottomLeft);
    }
    else if(pos == CD::BottomLeft) {
        mark->actionSetAnglePos(CD::TopLeft);
    }

}

void CCD_MarkEditor::slotUpdateFeatures() {


    releaseFeatures();
    if(mark->getFeatureOperMode() == CD::OperAuto ||
            mark->getFeatureOperMode() == CD::OperManual) {
        QList<CD_Feature *> featurestmp;
        featurestmp = mark->getFeaturesCurrent();
        for(int i =0;i<featurestmp.size();i++)
            features.append(featurestmp.at(i)->clone());
    }
    displayFeature(features);
    slotFeatureClick(0);

}

void CCD_MarkEditor::contextMenuEvent(QContextMenuEvent *) {

    slotTrigleAnglePos();

}

void CCD_MarkEditor::releaseFeatures() {

    while(!features.isEmpty()) {
        CD_Feature *ft = features.first();
        if(ft!=NULL)
            delete ft;
        features.takeFirst();
    }

}

void CCD_MarkEditor::releaseListWidgetData() {

    QListWidgetItem *item;
    disconnect(ui->listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(slotFeatureClick()));
    do {
     item = ui->listWidget->takeItem(0);
     if(item != 0)
         delete item;
    } while(item);
    connect(ui->listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(slotFeatureClick()));

}

void CCD_MarkEditor::displayFeature(QList<CD_Feature *> &fts) {

    CD_Feature* feature;
    releaseListWidgetData();

    int countforangle = 0;
    int countforcross = 0;
    int countforcircle = 0;

    for(int i=0;i<fts.size();i++) {
        feature = fts.at(i);
        if(feature->getType() == CD::FeatureAngle) {

            countforangle ++;
            QListWidgetItem * item = new QListWidgetItem(QString("鐗瑰緛:%1").arg(countforangle));
            ui->listWidget->addItem(item);

        } else if(feature->getType() == CD::FeatureCross) {

            countforcross ++;
            QListWidgetItem * item = new QListWidgetItem(QString("鐗瑰緛:%1").arg(countforcross));
            ui->listWidget->addItem(item);

        } else if(feature->getType() == CD::FeatureCircle) {
            countforcircle ++;
            QListWidgetItem * item = new QListWidgetItem(QString("鐗瑰緛:%1").arg(countforcircle));
            ui->listWidget->addItem(item);
        }

    }

}

void CCD_MarkEditor::slotFeatureClick() {

    int ret=0;
    if( ui->listWidget->selectedItems().size() ) {
        QListWidgetItem *item = ui->listWidget->selectedItems().at(0);
        ret = ui->listWidget->row(item);
    }

    if( mark->shape_mode ) {
        if( ret >=0 && ret<all_shapes.size()) {
            this->slotFeatureClick(ret);
        }
        return;
    }

    if( ret >=0 && ret<features.size()) {
        this->slotFeatureClick(ret);
    }

}

void CCD_MarkEditor::slotFeatureClick(int ret) {

    if( mark->shape_mode ) {

        if( ret<0 || ret>=all_shapes.size() ) return;

        mark->getScreenFeature()->resetLines();
        mark->getScreenFeature()->resetCircles();
        cv::RotatedRect rt = all_shapes[ret];
        cv::Point2f vertices[4];
        rt.points(vertices);
        for (int i = 0; i < 4; i++) {
            RS_Line line = RS_Line( RS_Vector(vertices[i].x , vertices[i].y ) ,\
                                                                    RS_Vector(vertices[(i+1)%4].x,vertices[(i+1)%4].y) );
            mark->getScreenFeature()->drawLine(line);
        }

        if(mark->isCalCircleCenterMode()) {

            qDebug()<<"in CalCircleCenterMode ttt";
            CD_Identify id;
            std::vector<cv::Point> &p = all_shapes_path[ret];
            QList<QPointF> tocal;
            std::vector<cv::Point>::iterator it;
            for( it=p.begin(); it!=p.end(); it++ ) {
                tocal.append(QPointF(it->x,it->y));
            }
            double rett[4];
            id.calculateArc(tocal,rett);
            RS_Circle circle;
            circle.setCenter(RS_Vector(rett[0],rett[1]));
            circle.setRadius(rett[2]);
            mark->getScreenFeature()->drawCircle(circle);

        }
        mark->getScreenFeature()->update();

        RS_Vector v1(vertices[0].x , -vertices[0].y);
        RS_Vector v2(vertices[1].x , -vertices[1].y);
        RS_Vector v3(vertices[2].x , -vertices[2].y);
        RS_Vector v4(vertices[3].x , -vertices[3].y);
        double d1 = v1.distanceTo(v2);
        double d2 = v2.distanceTo(v3);
        double with_final;
        double height_final;
        double angle_final = 0;
        if ( d1 > d2 ) {
            double angle_tmp = v1.angleTo(v2);
            if( angle_tmp >= 0 && angle_tmp<= M_PI)
                angle_final = angle_tmp;
            else
                angle_final = v2.angleTo(v1);
            with_final = d1;
            height_final = d2;
        }  else {
            double angle_tmp = v2.angleTo(v3);
            if( angle_tmp >= 0 && angle_tmp<= M_PI)
                angle_final = angle_tmp;
            else
                angle_final = v3.angleTo(v2);
            with_final = d2;
            height_final = d1;
        }

        RS_Line tmm(v1,v3);
        qDebug()<<"vv"<<v1.x<<v1.y;qDebug()<<"vv"<<v2.x<<v2.y;
        qDebug()<<"vv"<<v3.x<<v3.y;qDebug()<<"vv"<<v4.x<<v4.y;
        qDebug()<<angle_final<<tmm.getMiddlePoint().x<<tmm.getMiddlePoint().y<<rt.center.x<<rt.center.y;

        double hu[7];
        cv::Moments mts;
        cv::Mat sh(all_shapes_path[ret]);
        mts = cv::moments(sh, true);
        cv::HuMoments(mts,hu);
        qDebug("angle %f hu %f %f %f %f %f %f %f %f \n",rt.angle,hu[0],hu[1],hu[2],hu[3],\
                hu[4],hu[5],hu[6],hu[7]);

        double grativity_y = mts.m01/mts.m00;
        double grativity_x = mts.m10/mts.m00;
        qDebug("grativity=%f %f",grativity_x,grativity_y);

        memcpy(mark->shape_hu,hu,sizeof(hu));
        mark->rotate_rect = rt;
        index_of_shapes = ret;
        mark->shape_with = with_final;
        mark->shape_height = height_final;
        return;

    }

    if( ret >=0 && ret<features.size()) {
        CD_Feature *feature = features.at(ret);
        mark->getScreenFeature()->drawFeature(feature);

        if(feature->getType() == CD::FeatureAngle) {

            CD_FeatureAngleData *data = static_cast<CD_FeatureAngleData *>(feature);
            RS_Vector v = data->getCrossPoint();
            mark->getScreenFeature()->drawString(0,QColor(0,255,0),QString("X=%1  Y=%2").arg(v.x).arg(v.y));

        } else if(feature->getType() == CD::FeatureCross) {

            CD_FeatureCrossData *data = static_cast<CD_FeatureCrossData *>(feature);
            RS_Vector v = data->getCrossPoint();
            mark->getScreenFeature()->drawString(0,QColor(0,255,0),QString("X=%1 Y= %2").arg(v.x).arg(v.y));

        } else if(feature->getType() == CD::FeatureCircle) {

            CD_FeatureCircleData *data = (CD_FeatureCircleData *)feature;
            mark->getScreenFeature()->drawString(0,QColor(0,255,0),
                                                 QString("R= %1  X= %2 Y=%3").arg(data->getRadius()).arg(data->getX()).arg(data->getY()));

        }

    } else {
        mark->getScreenFeature()->drawFeature(NULL);
        mark->getScreenFeature()->drawString(0,QColor(0,255,0),QString(""));
    }
}

void CCD_MarkEditor::slotDetermineFeature() {


    QString ret;
    if(mark->getShapeBaseMatchFlag()) {

        handleMatchPar();
        pyramid_img[0].copyTo(mark->pyramid_img);
        pyramid_bit[0].copyTo(mark->pyramid_bit);
        mark->initProfile();
        //calculateRelatePoint();

    } else {
        ret = mark->determineFeature();
        if(mark->shape_mode) {
            if( index_of_shapes >=0 )
                ret.clear();
            else
                ret = QString(tr("Please Choose a template"));

        }
    }

    if(!ret.isEmpty()) {
        QMessageBox::warning(this,QObject::tr("warning"),ret);
        exe_ret = -1;
    } else {
        exe_ret = 0;
        this->close();
    }

}

void CCD_MarkEditor::slotDoSelectTemplate() {

    activeScreen(mainscreenplace,mark->getScreenTemplate());
    mark->actionSelectTemplate();
    ui->stackedWidget->setCurrentIndex(1);
    updateEditTemplateScreen();
    return;


}

void CCD_MarkEditor::slotDetermineTemplate() {

    QString ret;

    ret =  mark->determineTemplete();

    if(ret.isEmpty()) {

        ui->stackedWidget->setCurrentIndex(0);
        activeScreen(mainscreenplace,mark->getScreenFeature());

        mark->init();
        mark->setLocateX(this->cncmanager->getAbsPos(0));
        mark->setLocateY(this->cncmanager->getAbsPos(1));
        mark->setLocateZ(this->cncmanager->getAbsPos(2));
        mark->setLocateA(this->cncmanager->getAbsPos(3));
        updateEditFeatureScreen();

        if( mark->getShapeBaseMatchFlag() ) {
            mark->getCircleMark()->hide();
            mark->getCossMark()->hide();
            mark->getAngleMark()->hide();
            mark->getRelateMark()->show();
            shape_base_type = CD::FeatureRelate;
            ui->Type_Relate_2->setChecked(true);
        }

        calculateImageData();

    } else {
        QMessageBox::warning(this,QObject::tr("warning"),ret);
    }

}

void CCD_MarkEditor::slotForceReturnSelect() {


    mark->exitSelectTemplate();
    updateEditFeatureScreen();

    activeScreen(mainscreenplace,mark->getScreenFeature());
    mark->getScreenFeature()->showMatImage(*mark->getTemplateImage()->returnAsMat());
    ui->stackedWidget->setCurrentIndex(0);

//    mark->setLocateX(this->cncmanager->getAbsPos(0));
//    mark->setLocateY(this->cncmanager->getAbsPos(1));
//    mark->setLocateZ(this->cncmanager->getAbsPos(2));
//    mark->setLocateA(this->cncmanager->getAbsPos(3));

    calculateImageData();

    if( pyramid_bit.size() >0 ) {
      if( mark->pyramid_bit.rows == pyramid_bit[0].rows &&
          mark->pyramid_bit.cols == pyramid_bit[0].cols      ) {
          mark->pyramid_bit.copyTo(pyramid_bit[0]);
          mark->getScreenFeature()->showMatImage(pyramid_bit[0]);
      }
    }

    CD_RelateMark * mk = mark->getRelateMark();
    double x;
    double y;
    double fx = mark->getScreenFeature()->getFactorW();
    double fy = mark->getScreenFeature()->getFactorH();
    double w = mark->getScreenFeature()->getCurrentImage()->getWidth();
    double h = mark->getScreenFeature()->getCurrentImage()->getHeight();
    if( fx==0.0 || fy == 0.0 ) return;
    x = fx*(mark->relate_location_x + w/2.0);
    y = fy*(mark->relate_location_y + h/2.0);
    mk->setX(x);
    mk->setY(y);
    mk->update();



}

void CCD_MarkEditor::reject() {

    this->close();

}

void CCD_MarkEditor::closeEvent(QCloseEvent *e) {

    e->accept();
    return;
    if(exe_ret == -1) {

        int ret =  QMessageBox::warning(this, tr("Info"),
                    tr("currently has not generate any feature yet,\ndo you really want to exist?"),
                                        QMessageBox::Ok,QMessageBox::Cancel);

        if(ret == QMessageBox::Ok)
            e->accept();
        else
            e->ignore();



    }
}

void CCD_MarkEditor::on_CameraGain_valueChanged(int value) {
    if(!mark->getEnableCameraPar()) return;
    mark->camera_gain = value;
    capture->setCameraGain(value,mark->getCameraId());
    ui->SB_CameraGain->setValue(value);
}

void CCD_MarkEditor::on_CameraExposeTime_valueChanged(int value) {
    if(!mark->getEnableCameraPar()) return;
    mark->camera_expose_time = value;
    capture->setCameraShutter(value,mark->getCameraId());
    ui->SB_ExposeTime->setValue(value);
}

void CCD_MarkEditor::on_CameraContrast_valueChanged(int value) {
    if(!mark->getEnableCameraPar()) return;
    mark->camera_contrast = value;
    capture->setCameraContrast(value,mark->getCameraId());
    ui->SB_CameraContra->setValue(value);
}

void CCD_MarkEditor::on_CameraGamma_valueChanged(int value)
{

    if(!mark->getEnableCameraPar()) return;
    mark->camera_gamma= value;
    capture->setCameraGamma(value,mark->getCameraId());
    ui->SB_CameraGamma->setValue(value);

}

void CCD_MarkEditor::on_Light1_clicked(bool ) {

}

void CCD_MarkEditor::on_Light2_clicked(bool ) {

}

void CCD_MarkEditor::on_Light3_clicked(bool ) {

}

void CCD_MarkEditor::on_Light1_toggled(bool )
{
    //if(!mark->getEnableCameraPar()) return;
    //mark->enable_light1 = checked;
    //if(checked)
     //   cncmanager->changeM71IO(9,1);
    //else
    //    cncmanager->changeM71IO(9,0);
}

void CCD_MarkEditor::on_Light2_toggled(bool )
{
    //if(!mark->getEnableCameraPar()) return;
    //mark->enable_light2 = checked;
    //if(checked)
    //    cncmanager->changeM71IO(10,1);
    //else
    //    cncmanager->changeM71IO(10,0);
}

void CCD_MarkEditor::on_Light3_toggled(bool )
{

    //if(!mark->getEnableCameraPar()) return;
    //mark->enable_light3 = checked;
    //if(checked) {
    //    cncmanager->changeM71IO(12,1);
    //    cncmanager->changeM71IO(13,1);
    //} else {
    //    cncmanager->changeM71IO(12,0);
    //    cncmanager->changeM71IO(13,0);
    //}

}

void CCD_MarkEditor::on_LightPar_Save_clicked()
{

    mark->camera_contrast_store = mark->camera_contrast;
    mark->camera_gain_store = mark->camera_gain;
    mark->camera_expose_time_store = mark->camera_expose_time;
    mark->camera_gamma_store = mark->camera_gamma;

}

void CCD_MarkEditor::on_SB_SearchLen_valueChanged(int arg1)
{
    mark->search_shape_len = arg1;
}

void CCD_MarkEditor::on_SB_Sim_Threshold_valueChanged(double arg1)
{
    qDebug()<<"on_SB_Sim_Threshold_valueChanged="<<arg1<<mark;
    mark->shape_sim_threshold = arg1;
}



void CCD_MarkEditor::updateEditTemplateScreen() {

    int imgw = capture->getCameraResolutionW();
    int imgh =  capture->getCameraResolutionH();
    int setw,seth;
    if( ( imgw==0 || imgh == 0) ) {
        imgw = mark->getScreenTemplate()->getCurrentImage()->getWidth();
        imgh = mark->getScreenTemplate()->getCurrentImage()->getHeight();
    }
    if( ( imgw==0 || imgh == 0) ) {
        ui->Edit_View->setMinimumSize(648,486);
        ui->Edit_View->setMaximumSize(648,486);
        setw = 648;
        seth = 486;
    } else {
        double f = (float)imgh/(float)imgw;
        double set = 648.0*f;
        ui->Edit_View->setMinimumSize(648,set);
        ui->Edit_View->setMaximumSize(648,set);
        setw = 648;
        seth = set;
    }
    adjustSize();

    mark->getCossMark()->initRectPosition(setw,seth);


}

void CCD_MarkEditor::updateEditFeatureScreen() {

    int imgw = mark->getTemplateImage()->getWidth();
    int imgh = mark->getTemplateImage()->getHeight();

    qDebug()<<"updateEditFeatureScreen"<<imgw<<imgh;
    int setw,seth;

    if( imgw==0 || imgh == 0 ) {
        ui->Edit_View->setMinimumSize(648,486);
        ui->Edit_View->setMaximumSize(648,486);
    } else {

        if(imgh <= imgw) {
            double f = (float)imgh/(float)imgw;
            double set = 648.0*f;
            ui->Edit_View->setMinimumSize(648,set);
            ui->Edit_View->setMaximumSize(648,set);
            setw = 648;
            seth = set;

        } else {
            double f = (float)imgw/(float)imgh;
            double set = 600.0*f;
            ui->Edit_View->setMinimumSize(set,600);
            ui->Edit_View->setMaximumSize(set,600);

            setw = set;
            seth = 600;

        }

    }
    mark->getCossMark()->initRectPosition(setw,seth);

    adjustSize();
}


void CCD_MarkEditor::on_SB_Fat_valueChanged(int arg1)
{
    mark->actionSetFat(arg1);
}

void CCD_MarkEditor::on_CKB_Calcenter_clicked(bool checked)
{
    mark->shape_cal_center = checked;
}

void CCD_MarkEditor::on_SB_Wh_Threshold_valueChanged(double arg1)
{
    qDebug()<<"on_SB_Wh_Threshold_valueChanged= double"<<arg1<<mark;
    mark->shape_wh_threshold = arg1;
}



void CCD_MarkEditor::on_DB_SimThreshold_SH_valueChanged(double arg1)
{
    mark->setSimThresholdShape(arg1);
}

void CCD_MarkEditor::on_Canny_Threhold_valueChanged(int value)
{
    mark->setCannyThr(value-5,value+5);
    ui->Thre_Show->setValue(value);
}

void CCD_MarkEditor::on_Btn_Mode_Change_clicked()
{

    mark->getCircleMark()->hide();
    mark->getCossMark()->hide();
    mark->getAngleMark()->hide();
    mark->getRelateMark()->hide();

    slotSetOperModeAuto();
    ui->Mode_Auto->setChecked(true);

    if( mark->getShapeBaseMatchFlag() ) {
        ui->Btn_Mode_Change->setText(QString::fromLocal8Bit("特征模式"));
        mark->shape_base_match_flag = false;
        ui->stackedWidget_Mode->setCurrentIndex(0);
    } else {
        ui->Btn_Mode_Change->setText(QString::fromLocal8Bit("形状模式"));
        mark->shape_base_match_flag = true;
        ui->stackedWidget_Mode->setCurrentIndex(1);

        mark->getCircleMark()->hide();
        mark->getCossMark()->hide();
        mark->getAngleMark()->hide();
        mark->getRelateMark()->show();
        shape_base_type = CD::FeatureRelate;
        ui->Type_Relate_2->setChecked(true);
    }



}

void CCD_MarkEditor::on_Type_Relate_2_toggled(bool checked)
{
    if(checked) {
        mark->getRelateMark()->show();
        shape_base_type = CD::FeatureRelate;
    } else {
        mark->getRelateMark()->hide();
    }

}

void CCD_MarkEditor::on_Type_Circle_2_toggled(bool checked)
{
    if(checked) {
        mark->getCircleMark()->show();
        shape_base_type = CD::FeatureCircle;
    } else {
        mark->getCircleMark()->hide();
    }
}

void CCD_MarkEditor::on_Type_Angle_2_toggled(bool checked)
{
    if(checked) {
        mark->getAngleMark()->show();
        shape_base_type = CD::FeatureAngle;
    } else {
        mark->getAngleMark()->hide();
    }
}

void CCD_MarkEditor::on_Type_Cross_2_toggled(bool checked)
{
    if(checked) {
        mark->getCossMark()->show();
        shape_base_type = CD::FeatureCross;
    } else {
        mark->getCossMark()->hide();
    }

}

inline static bool getSubArea(cv::Mat &mat,CD_ImageScreen *mm,double x1, double y1, double angle, \
                              int wid, int hei,QList<QPoint> &ret ) {


    uchar *item;
    int imagex ,imagey;
    int hei_t = hei;
    for(int w = 0;  w<wid;  w++) {
        for(int h = 0; h<hei_t; h++) {

            imagex = w*cos(angle) - h*sin(angle) + x1;
            imagey = (w*sin(angle) + h*cos(angle) + y1);
            item= mat.ptr<uchar>(imagey,imagex);
            //qDebug()<<"imagexy"<<imagex<<imagey;
            mm->drawPoints(imagex,imagey);
            if( *item == 0xff ) {
                ret.append(QPoint(imagex,imagey));
            }

        }
    }

}


void CCD_MarkEditor::drawReferencePoints() {


    cv::Mat mat = *mark->getScreenFeature()->getCurrentImageMat();
    CD_ImageScreen *m_sc = mark->getScreenFeature();
    double w = mark->getScreenFeature()->getCurrentImage()->getWidth();
    double h = mark->getScreenFeature()->getCurrentImage()->getHeight();
    int wid = mat.cols;
    wid/=8;
    QList<RS_Line> list;
    RS_Vector v1,v2;
    RS_Vector resultvector;

    resultvector.x = mark->relate_location_x + w/2.0;
    resultvector.y = mark->relate_location_y + h/2.0;

    v1.x = resultvector.x - wid;
    v1.y = resultvector.y;
    v2.x = resultvector.x + wid;
    v2.y = resultvector.y;
    list.append(RS_Line(v1,v2));
    v1.x = resultvector.x;
    v1.y = resultvector.y-wid;
    v2.x = resultvector.x;
    v2.y = resultvector.y+wid;
    list.append(RS_Line(v1,v2));

    resultvector.x = mark->relate_location_x2 + w/2.0;
    resultvector.y = mark->relate_location_y2 + h/2.0;

    v1.x = resultvector.x - wid;
    v1.y = resultvector.y;
    v2.x = resultvector.x + wid;
    v2.y = resultvector.y;
    list.append(RS_Line(v1,v2));
    v1.x = resultvector.x;
    v1.y = resultvector.y-wid;
    v2.x = resultvector.x;
    v2.y = resultvector.y+wid;
    list.append(RS_Line(v1,v2));

    m_sc->drawLines(list);
    m_sc->update();

}

void CCD_MarkEditor::calculateRelatePoint(int num) {

    if( shape_base_type == CD::FeatureRelate ) {

        CD_RelateMark * mk = mark->getRelateMark();
        double x = mk->getX();
        double y = mk->getY();
        double fx = mark->getScreenFeature()->getFactorW();
        double fy = mark->getScreenFeature()->getFactorH();
        double w = mark->getScreenFeature()->getCurrentImage()->getWidth();
        double h = mark->getScreenFeature()->getCurrentImage()->getHeight();
        if( fx==0.0 || fy == 0.0 ) return;

        if( num == 0) {

            mark->relate_location_x = x/fx - w/2.0;
            mark->relate_location_y = y/fy - h/2.0;

        } else {
            mark->relate_location_x2 = x/fx - w/2.0;
            mark->relate_location_y2 = y/fy - h/2.0;

        }

        drawReferencePoints();


    } else if( shape_base_type == CD::FeatureCross ) {

        CD_CrossComp *cs = mark->getCossMark();
        cv::Mat mat = *mark->getScreenFeature()->getCurrentImageMat();
        double w = mark->getScreenFeature()->getCurrentImage()->getWidth();
        double h = mark->getScreenFeature()->getCurrentImage()->getHeight();
        double fx = mark->getScreenFeature()->getFactorW();
        double fy = mark->getScreenFeature()->getFactorH();

        QList<QPoint> points[8];

        for( int i=0; i<mat.rows; i++ ) {
            for( int j=0; j<mat.cols; j++ ) {
                if( *mat.ptr<uchar>(i,j) == 0xff ) {

                    double dis_x = fx*j;
                    double dis_y = fy*i;
                    int in = cs->isPointInside(dis_x,dis_y);
                    if(in != -1)
                        points[in - 1].append(QPoint(j,i));
                }

            }
        }

        CD_ImageScreen *m_sc = mark->getScreenFeature();
        CD_ImageProc proc;
        RS_Line l1,l2,l3,l4;
        m_sc->resetCircles();
        m_sc->resetLines();

        l1 = proc.calculateLine(points[0] + points[1]);
        l2 = proc.calculateLine(points[2] + points[3]);
        l3 = proc.calculateLine(points[4] + points[5]);
        l4 = proc.calculateLine(points[6] + points[7]);

        l1.calculateLineAbcFromTwoPoints();
        l2.calculateLineAbcFromTwoPoints();
        l3.calculateLineAbcFromTwoPoints();
        l4.calculateLineAbcFromTwoPoints();
        RS_Vector resultvector;


        if( ui->crossCenter->isChecked() ) {


            RS_DoubleLine dbl1(l1,l2);
            RS_DoubleLine dbl2(l3,l4);

            RS_Line mid1 = dbl1.getMidleLine();
            RS_Line mid2 = dbl2.getMidleLine();

            resultvector = RS_Math::crossPointBtwTwoLines(mid1,mid2);


        } else if( ui->topLeft->isChecked() ) {

            resultvector = RS_Math::crossPointBtwTwoLines(l1,l4);

        } else if( ui->topRight->isChecked() ) {

            resultvector = RS_Math::crossPointBtwTwoLines(l2,l4);

        } else if( ui->bottomLeft->isChecked() ) {

            resultvector = RS_Math::crossPointBtwTwoLines(l1,l3);

        } else if( ui->bottomRight->isChecked() ) {

            resultvector = RS_Math::crossPointBtwTwoLines(l2,l3);

        }


        if( num == 0 ) {
            mark->relate_location_x = resultvector.x - w/2.0;
            mark->relate_location_y = resultvector.y - h/2.0;
        } else {
            mark->relate_location_x2 = resultvector.x - w/2.0;
            mark->relate_location_y2 = resultvector.y - h/2.0;
        }

        drawReferencePoints();


    } else if( shape_base_type == CD::FeatureAngle ) {



    } else if( shape_base_type == CD::FeatureCircle ) {

        CD_CircleComp *cl = mark->getCircleMark();
        cv::Mat mat = *mark->getScreenFeature()->getCurrentImageMat();
        double w = mark->getScreenFeature()->getCurrentImage()->getWidth();
        double h = mark->getScreenFeature()->getCurrentImage()->getHeight();
        double fx = mark->getScreenFeature()->getFactorW();
        double fy = mark->getScreenFeature()->getFactorH();
        CD_ImageScreen *m_sc = mark->getScreenFeature();
        CD_ImageProc proc;
        QList<RS_Line> list;
        QList<QPoint> points;
        for( int i=0; i<mat.rows; i++ ) {
            for( int j=0; j<mat.cols; j++ ) {
                if( *mat.ptr<uchar>(i,j) == 0xff ) {
                    double dis_x = fx*j;
                    double dis_y = fy*i;
                    int in = cl->isPointInside(dis_x,dis_y);
                    if(in == 1)
                        points.append(QPoint(j,i));
                }

            }
        }

        double rett[3];
        proc.calculateArc(points,rett);

        RS_Vector resultvector(rett[0],rett[1]);
        RS_Circle cicle(rett[0],rett[1],rett[2]);
        RS_Vector v1,v2;

        int wid = mat.cols;
        wid/=8;

        m_sc->resetCircles();
        m_sc->resetLines();
        m_sc->drawCircle(cicle);


        if( num == 0 ) {
            mark->relate_location_x = resultvector.x - w/2.0;
            mark->relate_location_y = resultvector.y - h/2.0;
        } else {
            mark->relate_location_x2 = resultvector.x - w/2.0;
            mark->relate_location_y2 = resultvector.y - h/2.0;
        }

        drawReferencePoints();


    }

}

void CCD_MarkEditor::on_Btn_CalRefPoint_clicked()
{
    calculateRelatePoint(0);
    if( one_reference ) {
       calculateRelatePoint(1);
    }
}

void CCD_MarkEditor::on_Search_Step_valueChanged(double arg1)
{
    mark->match_par.search_angle_step = arg1;
}

void CCD_MarkEditor::on_Search_PyNum_valueChanged(int arg1)
{
    mark->match_par.pyramid_num = arg1;
}

void CCD_MarkEditor::on_Search_AngleNbh_valueChanged(int arg1)
{
    mark->match_par.angle_search_width = arg1;
}

void CCD_MarkEditor::on_Search_RectNbh_valueChanged(int arg1)
{
    mark->match_par.rect_search_width = arg1;
}

void CCD_MarkEditor::on_Btn_MorePar_clicked()
{
    if(match_par_dlg->isHidden())
        match_par_dlg->exec();
    else
        match_par_dlg->hide();
}

void CCD_MarkEditor::calculateImageData() {

    if( mark == NULL ) return;

    CD_MatchTemplate *match = CD_MatchTemplate::instance();
    MatchTemplateParameter par = mark->match_par;
    cv::Mat Img = *mark->getTemplateImage()->returnAsMat();

    //pyramid callculation
    unsigned char **pmd_img_pool = match->pmdImgPool();
    int i,last_w,last_h;
    int pyramid_num = par.pyramid_num;
    int tmp_img_w = Img.cols;
    int tmp_img_h = Img.rows;


    pyramid_num++;
    std::vector<int>pmd_tmp_width(pyramid_num);
    std::vector<int>pmd_tmp_height(pyramid_num);
    std::vector<unsigned char *>pmd_tmp_data(pyramid_num);

    unsigned char *tmpOriginData;
    unsigned char *last_img_data;

    tmpOriginData = pmd_img_pool[0];
    match->tranMat(Img,tmpOriginData);
    last_w = tmp_img_w;
    last_h = tmp_img_h;
    last_img_data = tmpOriginData;
    for( i = 1; i < pyramid_num; i ++ ) {
        pmd_tmp_width[i] = last_w/2;
        pmd_tmp_height[i] = last_h/2;
        pmd_tmp_data[i] = match->calculateImgPyramidOut(last_img_data,pmd_img_pool[i],last_w,last_h);
        last_w = pmd_tmp_width[i];
        last_h = pmd_tmp_height[i];
        last_img_data = pmd_tmp_data[i];
    }
    pmd_tmp_width[0] = Img.cols;
    pmd_tmp_height[0] = Img.rows;
    pmd_tmp_data[0] = tmpOriginData;

    pyramid_img.clear();
    pyramid_img.resize(pyramid_num);
    pyramid_bit.clear();
    pyramid_bit.resize(pyramid_num);

    for( i = 0; i < pyramid_num; i ++ ) {
        match->tranMat(pmd_tmp_data[i],pmd_tmp_width[i],pmd_tmp_height[i],pyramid_img[i]);
        cv::Mat M0(pyramid_img[i].rows,pyramid_img[i].cols,CV_8UC1,cv::Scalar(0));
        M0.copyTo(pyramid_bit[i]);

    }

    pyramid_index_img = 1;
    ui->Img_IndexLabel->setText(QString("1/%1").arg(pyramid_num));
    mark->getScreenFeature()->showMatImage(pyramid_img[0]);

}

void CCD_MarkEditor::on_Img_Last_clicked()
{

    if( pyramid_index_img > 1 ) {

        pyramid_index_img -- ;
        ui->Img_IndexLabel->setText(QString("%1/%2").\
                                    arg(pyramid_index_img).\
                                    arg(mark->match_par.pyramid_num + 1));


        if( pyramid_index_img == 1 )
            mark->getScreenFeature()->showMatImage(pyramid_img[pyramid_index_img-1]);
        else
            mark->getScreenFeature()->showMatImage(pyramid_bit[pyramid_index_img-1]);

    }

}

void CCD_MarkEditor::on_Img_Next_clicked()
{
    if( pyramid_index_img < ( mark->match_par.pyramid_num + 1 ) ) {

        pyramid_index_img ++ ;
        ui->Img_IndexLabel->setText(QString("%1/%2").\
                                    arg(pyramid_index_img).\
                                    arg(mark->match_par.pyramid_num + 1));

        if( pyramid_index_img == 1 )
            mark->getScreenFeature()->showMatImage(pyramid_img[pyramid_index_img-1]);
        else
            mark->getScreenFeature()->showMatImage(pyramid_bit[pyramid_index_img-1]);

    }

}

void CCD_MarkEditor::on_Btn_GetShape_clicked()
{
    if( pyramid_index_img != 1 ) return;

    CD_MatchTemplate * m = CD_MatchTemplate::instance();
    int i = pyramid_index_img-1;
    m->canny(pyramid_img[i],pyramid_bit[i]);
    mark->getScreenFeature()->showMatImage(pyramid_bit[i]);



}

void CCD_MarkEditor::on_Btn_ReturnOriImg_clicked()
{

    if( pyramid_index_img != 1 ) return;

    int i = pyramid_index_img-1;
    mark->getScreenFeature()->showMatImage(pyramid_img[i]);


}

void CCD_MarkEditor::on_EditMore_clicked(bool)
{

    int t,s,i,div_v;
    int new_t,new_s;
    for( i = 1; i < mark->match_par.pyramid_num+1; i++) {

        div_v = 2;
        for( int k = 1; k < i; k++)
            div_v *= 2;

        for( t = 0; t < pyramid_bit[0].rows; t++ ) {
            for( s = 0; s < pyramid_bit[0].cols; s++ ) {

               if( *pyramid_bit[0].ptr<uchar>(t,s) == 0xff ) {

                   new_t = t/div_v;
                   new_s = s/div_v;
                   if( new_t < pyramid_bit[i].rows && new_s < pyramid_bit[i].cols )
                       *pyramid_bit[i].ptr<uchar>(new_t,new_s) = 0xff;

               }
            }
       }
    }
    ui->Btn_Enraser->setChecked(false);
    on_Btn_Enraser_clicked(false);
}

void CCD_MarkEditor::on_Btn_Enraser_clicked(bool checked)
{

    if(checked) {

        double fx = mark->getScreenFeature()->getFactorW();
        double fy = mark->getScreenFeature()->getFactorH();
        mark->getEnraserMark()->resize(ui->Edit_View->size());
        mark->getEnraserMark()->setOperateMat(&pyramid_bit[0],mark->getScreenFeature(),fx,fy);
        mark->getAngleMark()->hide();
        mark->getCossMark()->hide();
        mark->getRelateMark()->hide();
        mark->getCircleMark()->hide();
        mark->getEnraserMark()->show();
    } else {

        if(ui->Type_Angle_2->isChecked())
            mark->getAngleMark()->show();
        if(ui->Type_Circle_2->isChecked())
            mark->getCircleMark()->show();
        if(ui->Type_Cross_2->isChecked())
            mark->getCossMark()->show();
        if(ui->Type_Relate_2->isChecked())
            mark->getRelateMark()->show();

        mark->getEnraserMark()->hide();
    }


}

void CCD_MarkEditor::on_Enraser_Size_valueChanged(int value)
{

    mark->getEnraserMark()->setEnraseSize(value);
    mark->getEnraserMark()->update();
    ui->Enraser_Show->setValue(value);

}

void CCD_MarkEditor::handleMatchPar() {

    mark->match_par.search_angles_start.clear();
    mark->match_par.search_angles_end.clear();

    if( ui->Search_Enable1->isChecked() ) {
        mark->match_par.search_angles_start.push_back(ui->Search_Start1->value());
        mark->match_par.search_angles_end.push_back(ui->Search_End1->value());
    }
    if( ui->Search_Enable2->isChecked() ) {
        mark->match_par.search_angles_start.push_back(ui->Search_Start2->value());
        mark->match_par.search_angles_end.push_back(ui->Search_End2->value());
    }
    if( ui->Search_Enable3->isChecked() ) {
        mark->match_par.search_angles_start.push_back(ui->Search_Start3->value());
        mark->match_par.search_angles_end.push_back(ui->Search_End3->value());
    }
    if( ui->Search_Enable4->isChecked() ) {
        mark->match_par.search_angles_start.push_back(ui->Search_Start4->value());
        mark->match_par.search_angles_end.push_back(ui->Search_End4->value());
    }


}

void CCD_MarkEditor::on_SB_CameraContra_valueChanged(int value)
{
    if(!mark->getEnableCameraPar()) return;
    mark->camera_contrast = value;
    capture->setCameraContrast(value,mark->getCameraId());
    ui->CameraContrast->setValue(value);


}

void CCD_MarkEditor::on_SB_ExposeTime_valueChanged(int value)
{
    if(!mark->getEnableCameraPar()) return;
    mark->camera_expose_time = value;
    capture->setCameraShutter(value,mark->getCameraId());
    ui->CameraExposeTime->setValue(value);

}

void CCD_MarkEditor::on_SB_CameraGain_valueChanged(int value)
{
    if(!mark->getEnableCameraPar()) return;
    mark->camera_gain = value;
    capture->setCameraGain(value,mark->getCameraId());
    ui->CameraGain->setValue(value);

}

void CCD_MarkEditor::on_SB_CameraGamma_valueChanged(int value)
{

    if(!mark->getEnableCameraPar()) return;
    mark->camera_gamma = value;
    capture->setCameraGamma(value,mark->getCameraId());
    ui->CameraGamma->setValue(value);

}

void CCD_MarkEditor::on_Btn_CalRefPoint2_clicked()
{
    calculateRelatePoint(1);
}

void CCD_MarkEditor::on_Search_Enable_Filter_clicked(bool checked)
{
    mark->match_par.process_enable = checked;
}

void CCD_MarkEditor::on_Search_Blur_Size_valueChanged(int arg1)
{
    mark->match_par.median_blur_size = arg1;
}

void CCD_MarkEditor::on_Search_Erosion_Size_valueChanged(int arg1)
{
    mark->match_par.erosion_size = arg1;
}

void CCD_MarkEditor::on_MarkPos_Save_clicked()
{
    int ret =  QMessageBox::warning(this, tr("Info"),
                    tr("确定更新拍照位置?"),
                                    QMessageBox::Ok,QMessageBox::Cancel);

    if(ret == QMessageBox::Ok) {

        mark->pop();
        mark->setLocateX(this->cncmanager->getAbsPos(0));
        mark->setLocateY(this->cncmanager->getAbsPos(1));
        mark->setLocateZ(this->cncmanager->getAbsPos(2));
        mark->setLocateA(this->cncmanager->getAbsPos(3));
        exe_ret = 0;
        this->close();

    } else {


    }

}
