#include "ccd_sysparcommoneditor.h"
#include "ui_ccd_sysparcommoneditor.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <math.h>

#include "rs_math.h"
#include "ccd_sysparameter.h"
#include "cd_parameter.h"
#include "ccd_cncmanager.h"

#ifdef __USING_LINUX__
#define _isnan std::isnan
#endif


CCD_SysParCommonEditor::CCD_SysParCommonEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCD_SysParCommonEditor) {

    ui->setupUi(this);
   // connect(ui->SB_CircleDistanceToler,SIGNAL(valueChanged(double)),this,SLOT(slotSetCircleDistanceToler(double)));
    //connect(ui->SB_SearchLen1,SIGNAL(valueChanged(int)),this,SLOT(slotSetCompensateSearchLen1(int)));
    //connect(ui->SB_SearchLen2,SIGNAL(valueChanged(int)),this,SLOT(slotSetCompensateSearchLen2(int)));
    connect(ui->SB_SearchLen3,SIGNAL(valueChanged(double)),this,SLOT(slotSetCompensateSearchLen3(double)));

    connect(ui->SB_SearchToler,SIGNAL(valueChanged(double)),this,SLOT(slotSetCompensateToler(double)));
    connect(ui->SB_AngleToler,SIGNAL(valueChanged(double)),this,SLOT(slotSetCompensateAngleToler(double)));
    //connect(ui->SB_PassDistance,SIGNAL(valueChanged(double)),this,SLOT(slotSetPassDistance(double)));

    //connect(ui->SB_SampleNum,SIGNAL(valueChanged(int)),this,SLOT(slotSetSampleNum(int)));
    //connect(ui->SB_SplineDegree,SIGNAL(valueChanged(int)),this,SLOT(slotSetSplineDegree(int)));
    //connect(ui->SB_SplineSeg,SIGNAL(valueChanged(int)),this,SLOT(slotSetSplineSeg(int)));
    //connect(ui->SB_ToolSize,SIGNAL(valueChanged(double)),this,SLOT(slotSetToolSize(double)));
    connect(ui->LinE_DelayId,SIGNAL(valueChanged(double)),this,SLOT(slotSetDelayTime(double)));
    //connect(ui->LinE_DistanceMerge,SIGNAL(valueChanged(double)),this,SLOT(slotSetDistanceMerge(double)));

    connect(ui->Btn_Sure,SIGNAL(clicked()),this,SLOT(slotSure()));
    connect(ui->Btn_Cancel,SIGNAL(clicked()),this,SLOT(slotCancel()));

    ui->LinE_ComputeDIff->setValue(0);
    ui->LinE_ComputeLen->setValue(0);

    offline_mode = false;

}

CCD_SysParCommonEditor::~CCD_SysParCommonEditor() {
    delete ui;
}

void CCD_SysParCommonEditor::init(CCD_SysParameter *p, CCD_CncManager *m) {

    ui->Ckb_Offline->hide();
    syspar = p;
    ui->LinE_DelayId->setValue(p->getimg_delaytime);

    if(p->marknum == 1) {

        ui->Btn_OneP->setChecked(true);

    } else if(p->marknum == 2) {
        ui->Btn_TwoP->setChecked(true);
        //ui->groupBox_Two->show();
    }
    else if(p->marknum == 3) {
        ui->Btn_ThreeP->setChecked(true);
        //ui->groupBox_Two->hide();
    }
    else {
        ui->Btn_FourP->setChecked(true);
        //ui->groupBox_Two->hide();
    }

    if( p->origin_pos == 0 )
        ui->Btn_Center->setChecked(true);
    else
        ui->Btn_LeftUp->setChecked(true);

    ui->SB_TwoPointsW->setValue(p->two_points_w);
    ui->SB_TwoPointsH->setValue(p->two_points_h);

    ui->SB_SearchToler->setValue(p->correct_offset_y);
    ui->SB_AngleToler->setValue((p->correct_max_a*180)/M_PI);
    ui->SB_SearchLen3->setValue(p->correct_offset_x);

    ui->LinE_RotateOffset->setValue(p->compute_rotate_offset);

    ui->Array_Distance_Hor->setValue(p->array_distance_hor);
    ui->Array_Distance_Ver->setValue(p->array_distance_ver);
    ui->Array_Enable->setChecked(p->enable_array_identify);
    ui->Array_With->setValue(p->array_with);
    ui->Array_Height->setValue(p->array_height);

    ui->SB_SampleNum->setValue(p->sample_num);

    if(p->enable_array_identify)
        ui->Array_Enable->setText(QString::fromUtf8("阵列有效"));
    else
        ui->Array_Enable->setText(QString::fromUtf8("阵列无效"));

    ui->ContrastValidValue->setValue(CD_Parameter::Instance()->getContrastValidValue());

    if( m != NULL)
        ui->CCD_Position->setValue(m->getZaxisNum());
    manager = m;

    p->push();


}

void CCD_SysParCommonEditor::slotSetDistanceMerge(double d) {

    syspar->two_points_w = d;

}

void CCD_SysParCommonEditor::slotSetToolSize(double d) {

    syspar->tool_size = d;

}

void CCD_SysParCommonEditor::slotSetDelayTime(double d) {

    syspar->getimg_delaytime = d;

}

void CCD_SysParCommonEditor::slotSetCircleDistanceToler(double d) {
        syspar->two_points_h = d;
}

void CCD_SysParCommonEditor::slotSetCompensateSearchLen1(int d){
    syspar->array_with = d;
}

void CCD_SysParCommonEditor::slotSetCompensateSearchLen2(int d){
    syspar->array_height = d;
}

void CCD_SysParCommonEditor::slotSetCompensateSearchLen3(double d){
    syspar->correct_offset_x = d;
}


void CCD_SysParCommonEditor::slotSetCompensateToler(double d){
    syspar->correct_offset_y = d;
}

void CCD_SysParCommonEditor::slotSetCompensateAngleToler(double d){
    syspar->correct_max_a = (d*M_PI)/180.0;
}


void CCD_SysParCommonEditor::slotSure() {
    syspar->push();
    CD_Parameter::Instance()->contrast_valid_value =
                ui->ContrastValidValue->value();
    manager->setZaxisNum(ui->CCD_Position->value());

    this->close();
}

void CCD_SysParCommonEditor::slotCancel() {

    this->close();

}

void CCD_SysParCommonEditor::reject() {
    this->close();
}

void CCD_SysParCommonEditor::closeEvent(QCloseEvent *e) {

    if(syspar->checkChange()) {

        int ret = QMessageBox::warning(this,tr("tips")
        ,tr("参数已经修改，确定放弃保存返回？"),
                               tr("确定"),tr("取消"));
        switch (ret) {
           case 0:
               syspar->pop();
               e->accept();
               break;
           case 1:
               e->ignore();
               break;
           default:
               break;
         }
    }

}


void CCD_SysParCommonEditor::on_Btn_TwoP_clicked(bool checked)
{
    if(!checked) return;
        syspar->marknum = 2;
    //ui->groupBox_Two->show();

}

void CCD_SysParCommonEditor::on_Btn_ThreeP_clicked(bool checked)
{
    if(!checked) return;
        syspar->marknum = 3;
    //ui->groupBox_Two->hide();


}

void CCD_SysParCommonEditor::on_Btn_FourP_clicked(bool checked)
{
    if(!checked) return;
        syspar->marknum = 4;
}

void CCD_SysParCommonEditor::on_Btn_LeftUp_clicked(bool checked)
{
    if(!checked) return;
    syspar->origin_pos = 1;//left_up
}

void CCD_SysParCommonEditor::on_Btn_Center_clicked(bool checked)
{
    if(!checked) return;
    syspar->origin_pos = 0;//left_up
}

void CCD_SysParCommonEditor::on_SB_TwoPointsW_valueChanged(double arg1)
{
    syspar->two_points_w = arg1;
}

void CCD_SysParCommonEditor::on_SB_TwoPointsH_valueChanged(double arg1)
{
    syspar->two_points_h = arg1;
}

void CCD_SysParCommonEditor::on_LinE_ComputeLen_valueChanged(double arg1)
{
    compute_len = arg1;
}

void CCD_SysParCommonEditor::on_LinE_ComputeDIff_valueChanged(double arg1)
{
    compute_diff = arg1;
}

void CCD_SysParCommonEditor::on_Btn_ComputeRotate_clicked()
{

    if( fabs(compute_len - 0.0)< 0.00001 || fabs(compute_diff - 0.0)< 0.00001 ) {
        ui->LinE_RotateOffset->setValue(0.0);
        return;
    }
    float jd = (asin(compute_diff/compute_len) *180.0)/M_PI;
    if( _isnan(jd) ) {
        ui->LinE_RotateOffset->setValue(0.0);
        return;
    }
    ui->LinE_RotateOffset->setValue( (asin(compute_diff/compute_len) *180.0)/M_PI );

}

void CCD_SysParCommonEditor::on_LinE_RotateOffset_valueChanged(double arg1)
{
    syspar->compute_rotate_offset =arg1;
}

void CCD_SysParCommonEditor::on_Array_With_valueChanged(int arg1)
{
    syspar->array_with = arg1;
}

void CCD_SysParCommonEditor::on_Array_Height_valueChanged(int arg1)
{
    syspar->array_height = arg1;
}

void CCD_SysParCommonEditor::on_Array_Distance_Hor_valueChanged(double arg1)
{
    syspar->array_distance_hor = arg1;
}

void CCD_SysParCommonEditor::on_Array_Distance_Ver_valueChanged(double arg1)
{
    syspar->array_distance_ver = arg1;
}

void CCD_SysParCommonEditor::on_Array_Enable_clicked(bool checked)
{
    syspar->enable_array_identify = checked;
}

void CCD_SysParCommonEditor::on_SB_SampleNum_valueChanged(int arg1)
{
    syspar->sample_num = arg1;
}

void CCD_SysParCommonEditor::on_Ckb_Offline_clicked(bool checked)
{


    if(checked) {
        int ret = QMessageBox::warning(this,tr("tips")
        ,tr("打开这个将不确认相机链接直接识别，确定打开?"),
                               tr("确定"),tr("取消"));
        switch (ret) {
           case 0:
               offline_mode = checked;
               break;
           case 1:
               ui->Ckb_Offline->setChecked(false);
               break;
           default:
               break;
         }

    } else {
        offline_mode = checked;
    }


}

void CCD_SysParCommonEditor::keyPressEvent(QKeyEvent *e) {

    if (e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier) && e->key() == Qt::Key_D) {
       ui->Ckb_Offline->show();
    }
}

void CCD_SysParCommonEditor::on_Btn_OneP_clicked(bool checked)
{
    if(!checked) return;
        syspar->marknum = 1;
}
