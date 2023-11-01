#include "datum_parameter.h"
#include "ui_datum_parameter.h"

#include "emcstatus.hh"
#include "ini_settings.h"
#include "extern.h"
#include "shcom.hh"
#include "datum_explain.h"

#include <QInputDialog>
#include <QMessageBox>

Datum_parameter::Datum_parameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Datum_parameter)
{
    ui->setupUi(this);

        this->setStyleSheet("font: 12pt Sans Serif");

    Locking_flag = true;

    explain = new datum_Explain(this);


    lab_record_x = new MyLabel(0);
    QVBoxLayout *lab_record_x_lay = new QVBoxLayout(ui->lab_recordx);
    lab_record_x_lay->addWidget(lab_record_x);
    lab_record_x_lay->setContentsMargins(0,0,0,0);
    lab_record_x->setText(QString::number(par_get_var(3601),'f',2));
    lab_record_x->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_record_x,SIGNAL(clicked()),this,SLOT(set_record_x()));

    lab_record_y = new MyLabel(0);
    QVBoxLayout *lab_record_y_lay = new QVBoxLayout(ui->lab_recordy);
    lab_record_y_lay->addWidget(lab_record_y);
    lab_record_y_lay->setContentsMargins(0,0,0,0);
    lab_record_y->setText(QString::number(par_get_var(3602),'f',2));
    lab_record_y->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_record_y,SIGNAL(clicked()),this,SLOT(set_record_y()));

    lab_record_z = new MyLabel(0);
    QVBoxLayout *lab_record_z_lay = new QVBoxLayout(ui->lab_recordz);
    lab_record_z_lay->addWidget(lab_record_z);
    lab_record_z_lay->setContentsMargins(0,0,0,0);
    lab_record_z->setText(QString::number(par_get_var(3603),'f',2));
    lab_record_z->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_record_z,SIGNAL(clicked()),this,SLOT(set_record_z()));

    lab_record_y_a = new MyLabel(0);
    QVBoxLayout *lab_record_y_a_lay = new QVBoxLayout(ui->lab_recordy_a);
    lab_record_y_a_lay->addWidget(lab_record_y_a);
    lab_record_y_a_lay->setContentsMargins(0,0,0,0);
    lab_record_y_a->setText(QString::number(par_get_var(3604),'f',2));
    lab_record_y_a->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_record_y_a,SIGNAL(clicked()),this,SLOT(set_record_y_a()));

    lab_record_z_a = new MyLabel(0);
    QVBoxLayout *lab_record_z_a_lay = new QVBoxLayout(ui->lab_recordz_a);
    lab_record_z_a_lay->addWidget(lab_record_z_a);
    lab_record_z_a_lay->setContentsMargins(0,0,0,0);
    lab_record_z_a->setText(QString::number(par_get_var(3605),'f',2));
    lab_record_z_a->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_record_z_a,SIGNAL(clicked()),this,SLOT(set_record_z_a()));

    lab_record_y_c = new MyLabel(0);
    QVBoxLayout *lab_record_y_c_lay = new QVBoxLayout(ui->lab_recordy_c);
    lab_record_y_c_lay->addWidget(lab_record_y_c);
    lab_record_y_c_lay->setContentsMargins(0,0,0,0);
    lab_record_y_c->setText(QString::number(par_get_var(3606),'f',2));
    lab_record_y_c->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_record_y_c,SIGNAL(clicked()),this,SLOT(set_record_y_c()));

    lab_record_z_c = new MyLabel(0);
    QVBoxLayout *lab_record_z_c_lay = new QVBoxLayout(ui->lab_recordz_c);
    lab_record_z_c_lay->addWidget(lab_record_z_c);
    lab_record_z_c_lay->setContentsMargins(0,0,0,0);
    lab_record_z_c->setText(QString::number(par_get_var(3607),'f',2));
    lab_record_z_c->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_record_z_c,SIGNAL(clicked()),this,SLOT(set_record_z_c()));

    lab_Material_L = new MyLabel(0);
    QVBoxLayout *lab_Material_L_lay = new QVBoxLayout(ui->set_Material_l);
    lab_Material_L_lay->addWidget(lab_Material_L);
    lab_Material_L_lay->setContentsMargins(0,0,0,0);
    lab_Material_L->setText(QString::number(par_get_var(3612),'f',2));
    lab_Material_L->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_Material_L,SIGNAL(clicked()),this,SLOT(set_Material_L()));

    lab_Material_W = new MyLabel(0);
    QVBoxLayout *lab_Material_W_lay = new QVBoxLayout(ui->set_Material_w);
    lab_Material_W_lay->addWidget(lab_Material_W);
    lab_Material_W_lay->setContentsMargins(0,0,0,0);
    lab_Material_W->setText(QString::number(par_get_var(3613),'f',2));
    lab_Material_W->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_Material_W,SIGNAL(clicked()),this,SLOT(set_Material_W()));

    lab_Material_H = new MyLabel(0);
    QVBoxLayout *lab_Material_H_lay = new QVBoxLayout(ui->set_Material_h);
    lab_Material_H_lay->addWidget(lab_Material_H);
    lab_Material_H_lay->setContentsMargins(0,0,0,0);
    lab_Material_H->setText(QString::number(par_get_var(3614),'f',2));
    lab_Material_H->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_Material_H,SIGNAL(clicked()),this,SLOT(set_Material_H()));


    ui->btn_set_a->hide();
    ui->btn_set_b->hide();
    ui->btn_set_c->hide();

}

Datum_parameter::~Datum_parameter()
{
    delete ui;
}

void Datum_parameter::updateUI()
{

      ui->checkBox->setChecked(Locking_flag);

      if(Locking_flag){
          ui->record_a->setEnabled(false);
          ui->record_b->setEnabled(false);
          ui->record_c->setEnabled(false);
      }else{
          ui->record_a->setEnabled(true);
          ui->record_b->setEnabled(true);
          ui->record_c->setEnabled(true);
      }


      if(emcStatus->task.state == EMC_TASK_STATE_ON
              && !_forbid_run && !_forbid_spindle && !_get_alm) {
            ui->btn_set_a->setEnabled(true);
            ui->btn_set_b->setEnabled(true);
            ui->btn_set_c->setEnabled(true);
            ui->btn_setall->setEnabled(true);
      }else{
          ui->btn_set_a->setEnabled(false);
          ui->btn_set_b->setEnabled(false);
          ui->btn_set_c->setEnabled(false);
          ui->btn_setall->setEnabled(false);
      }


}

void Datum_parameter::on_checkBox_clicked()
{
    if(Locking_flag){
        QString password = "cnext";
        QString input = "";
        QString tips = tr("Please input password:");
        bool ok;
        while(input != password){
             input = QInputDialog::getText(this,tr("Input Password"),tips,
                                           QLineEdit::Password,"",&ok);
             tips = tr("Password is wrong, input again:");
             if(!ok){
                 return;
             }
        }
        Locking_flag = false;
    }else
        Locking_flag = true;
}


void Datum_parameter::on_btn_recordX_clicked()
{
    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot record the datum while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the datum from the current position"),
    tr("Record current mechanical coordinates as datum.Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        show_msg(1,tr("record the X datum of T(0): %1-->%2").arg(par_get_var(3601)).arg(get_abs_pos(0)));
        sendSetParameter(3601,get_abs_pos(0));
        lab_record_x->setText(QString::number(get_abs_pos(0),'f',2));
        break;
    case Cancel:

        break;
    default:
        break;
    }
}

void Datum_parameter::set_record_x()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3601),(QVariant)par_get_var(4052),(QVariant)par_get_var(4051),tr("set recordx"),tr("set recordx"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= par_get_var(4052) && val <= par_get_var(4051)){
        show_msg(1,tr("modify the X datum of T(0): %1-->%2").arg(par_get_var(3601)).arg(val));
        lab_record_x->setText(QString::number(val,'f',2));
        sendSetParameter(3601,val);
    }
}

void Datum_parameter::set_record_y()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3602),(QVariant)par_get_var(4054),(QVariant)par_get_var(4053),tr("set recordy"),tr("set recordy"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= par_get_var(4054) && val <= par_get_var(4053)){
        show_msg(1,tr("modify the Y datum of T(0): %1-->%2").arg(par_get_var(3602)).arg(val));
        lab_record_y->setText(QString::number(val,'f',2));
        sendSetParameter(3602,val);
    }
}

void Datum_parameter::set_record_z()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3603),(QVariant)par_get_var(4056),(QVariant)par_get_var(4055),tr("set recordz"),tr("set recordz"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= par_get_var(4056) && val <= par_get_var(4055)){
        show_msg(1,tr("modify the Z datum of T(0) : %1-->%2").arg(par_get_var(3603)).arg(val));
        lab_record_z->setText(QString::number(val,'f',2));
        sendSetParameter(3603,val);
    }
}


void Datum_parameter::on_btn_recordY_clicked()
{
    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot record the datum while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the datum from the current position"),
    tr("Record current mechanical coordinates as datum.Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        show_msg(1,tr("record the Y datum of T(0) : %1-->%2").arg(par_get_var(3602)).arg(get_abs_pos(1)));
        sendSetParameter(3602,get_abs_pos(1));
        lab_record_y->setText(QString::number(get_abs_pos(1),'f',2));
        break;
    case Cancel:

        break;
    default:
        break;
    }
}

void Datum_parameter::on_btn_recordZ_clicked()
{
    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot record the datum while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the datum from the current position"),
    tr("Record current mechanical coordinates as datum.Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        show_msg(1,tr("record the Z datum of T(0) : %1-->%2").arg(par_get_var(3603)).arg(get_abs_pos(2)));
        sendSetParameter(3603,get_abs_pos(2));
        lab_record_z->setText(QString::number(get_abs_pos(2),'f',2));
        break;
    case Cancel:

        break;
    default:
        break;
    }
}

void Datum_parameter::on_btn_recordY_a_clicked()
{
    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot set the offset while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the offset from T(0)-->R(-90)"),
    tr("Set the offset from T(0)-->R(-90).Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        this->set_offset_y_a();
        break;
    case Cancel:

        break;
    default:
        break;
    }
}

void Datum_parameter::on_btn_recordZ_a_clicked()
{
    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot set the offset while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the offset from T(0)-->R(-90)"),
    tr("Set the offset from T(0)-->R(-90).Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        this->set_offset_z_a();
        break;
    case Cancel:

        break;
    default:
        break;
    }
}

void Datum_parameter::set_record_y_a()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3604),(QVariant)-1000,(QVariant)1000,tr("set offset"),tr("set offset"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= -1000 && val <= 1000){
        show_msg(1,tr("modify the Y offset of T(0)-->R(-90) : %1-->%2").arg(par_get_var(3604)).arg(val));
        lab_record_y_a->setText(QString::number(val,'f',2));
        sendSetParameter(3604,val);
    }
}

void Datum_parameter::set_record_z_a()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3605),(QVariant)-500,(QVariant)500,tr("set offset"),tr("set offset"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= -500 && val <= 500){
        show_msg(1,tr("modify the Z offset of T(0)-->R(-90) : %1-->%2").arg(par_get_var(3605)).arg(val));
        lab_record_z_a->setText(QString::number(val,'f',2));
        sendSetParameter(3605,val);
    }
}

void Datum_parameter::on_btn_recordY_c_clicked()
{
    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot set the offset while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the offset from T(0)-->L(90)"),
    tr("Set the offset from T(0)-->L(90).Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        this->set_offset_y_c();
        break;
    case Cancel:

        break;
    default:
        break;
    }
}

void Datum_parameter::on_btn_recordZ_c_clicked()
{
    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot set the offset while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the offset from T(0)-->L(90)"),
    tr("Set the offset from T(0)-->L(90).Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        this->set_offset_z_c();
        break;
    case Cancel:

        break;
    default:
        break;
    }
}

void Datum_parameter::set_record_y_c()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3606),(QVariant)-1000,(QVariant)1000,tr("set offset"),tr("set offset"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= -1000 && val <= 1000){
        show_msg(1,tr("modify the Y offset of T(0)-->L(90) : %1-->%2").arg(par_get_var(3606)).arg(val));
        lab_record_y_c->setText(QString::number(val,'f',2));
        sendSetParameter(3606,val);
    }
}

void Datum_parameter::set_record_z_c()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3607),(QVariant)-500,(QVariant)500,tr("set offset"),tr("set offset"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= -500 && val <= 500){
        show_msg(1,tr("modify the Z offset of T(0)-->L(90) : %1-->%2").arg(par_get_var(3607)).arg(val));
        lab_record_z_c->setText(QString::number(val,'f',2));
        sendSetParameter(3607,val);
    }
}

void Datum_parameter::set_Material_L()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3612),(QVariant)0,(QVariant)par_get_var(4051),tr("set Length"),tr("set Length"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= 0 && val <= par_get_var(4051)){
        show_msg(1,tr("modify the Length of Material: %1-->%2").arg(par_get_var(3612)).arg(val));
        lab_Material_L->setText(QString::number(val,'f',2));
        sendSetParameter(3612,val);
    }
}

void Datum_parameter::set_Material_W()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3613),(QVariant)0,(QVariant)1000,tr("set width "),tr("set width "),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= 0 && val <= 1000){
        show_msg(1,tr("modify the width of Material: %1-->%2").arg(par_get_var(3613)).arg(val));
        lab_Material_W->setText(QString::number(val,'f',2));
        sendSetParameter(3613,val);
    }
}

void Datum_parameter::set_Material_H()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3614),(QVariant)0,(QVariant)500,tr("set height"),tr("set height"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= 0 && val <= 500){
        show_msg(1,tr("modify the height of Material: %1-->%2").arg(par_get_var(3614)).arg(val));
        lab_Material_H->setText(QString::number(val,'f',2));
        sendSetParameter(3614,val);
    }
}


void Datum_parameter::on_btn_close_clicked()
{
    this->close();
}

void Datum_parameter::on_btn_set_a_clicked()
{
    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot set the offset while not home!"));
        return;
    }


    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the offset"),
    tr("Set the offset of R(-90).Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        this->set_a();
        break;
    case Cancel:

        break;
    default:
        break;
    }
}

void Datum_parameter::on_btn_set_b_clicked()
{

    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot set the offset while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the offset"),
    tr("Set the offset of T(0).Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        this->set_b();
        break;
    case Cancel:

        break;
    default:
        break;
    }

}

void Datum_parameter::on_btn_set_c_clicked()
{
    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot set the offset while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the offset"),
    tr("Set the offset of L(90).Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        this->set_c();
      break;
    case Cancel:

        break;
    default:
        break;
    }

}

void Datum_parameter::set_a()
{
    QString cmd_coor;
    double xoffset_a = par_get_var(5101)+par_get_var(3601);
    double yoffset_a = par_get_var(5102)+par_get_var(3604)+par_get_var(3602)+par_get_var(3614);
    double zoffset_a = par_get_var(5103)+par_get_var(3605)+par_get_var(3603)+par_get_var(3613);
    cmd_coor.append("G10L2P1").append("X").append(QString::number(xoffset_a,'f',3))
            .append("Y").append(QString::number(yoffset_a,'f',3))
            .append("Z").append(QString::number(zoffset_a,'f',3));
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    show_msg(1,tr("Set the offset of R(-90): X: %1. Y: %2 . Z: %3 .").arg(xoffset_a).arg(yoffset_a).arg(zoffset_a));
}

void Datum_parameter::set_b()
{
    QString cmd_coor;
    double xoffset_b = par_get_var(5101)+par_get_var(3601);
    double yoffset_b = par_get_var(5102)+par_get_var(3602);
    double zoffset_b = par_get_var(5103)+par_get_var(3603)+par_get_var(3614);
    cmd_coor.append("G10L2P2").append("X").append(QString::number(xoffset_b,'f',3))
            .append("Y").append(QString::number(yoffset_b,'f',3))
            .append("Z").append(QString::number(zoffset_b,'f',3));
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    show_msg(1,tr("Set the offset of T(0): X: %1. Y: %2 . Z: %3 .").arg(xoffset_b).arg(yoffset_b).arg(zoffset_b));
}

void Datum_parameter::set_c()
{
    QString cmd_coor;
    double xoffset_c = par_get_var(5101)+par_get_var(3601);
    double yoffset_c = par_get_var(5102)+par_get_var(3606)+par_get_var(3602);
    double zoffset_c = par_get_var(5103)+par_get_var(3607)+par_get_var(3603);
    cmd_coor.append("G10L2P3").append("X").append(QString::number(xoffset_c,'f',3))
            .append("Y").append(QString::number(yoffset_c,'f',3))
            .append("Z").append(QString::number(zoffset_c,'f',3));
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    show_msg(1,tr("Set the offset of L(90): X: %1. Y: %2 . Z: %3 .").arg(xoffset_c).arg(yoffset_c).arg(zoffset_c));
}

void Datum_parameter::set_offset_y_a()
{
    double offset_y = get_abs_pos(1)-par_get_var(3602);
    show_msg(1,tr("record the Y datum of R(-90) : %1-->%2").arg(par_get_var(3604)).arg(offset_y));
    sendSetParameter(3604,offset_y);
    lab_record_y_a->setText(QString::number(offset_y,'f',2));
}

void Datum_parameter::set_offset_z_a()
{
    double offset_z = get_abs_pos(2)-par_get_var(3603);
    show_msg(1,tr("record the Z datum of R(-90) : %1-->%2").arg(par_get_var(3605)).arg(offset_z));
    sendSetParameter(3605,offset_z);
    lab_record_z_a->setText(QString::number(offset_z,'f',2));
}

void Datum_parameter::set_offset_y_c()
{
    double offset_y = get_abs_pos(1)-par_get_var(3602);
    show_msg(1,tr("record the Y datum of L(90) : %1-->%2").arg(par_get_var(3606)).arg(offset_y));
    sendSetParameter(3606,offset_y);
    lab_record_y_c->setText(QString::number(offset_y,'f',2));
}

void Datum_parameter::set_offset_z_c()
{
    double offset_z = get_abs_pos(2)-par_get_var(3603);
    show_msg(1,tr("record the Z datum of L(90) : %1-->%2").arg(par_get_var(3607)).arg(offset_z));
    sendSetParameter(3607,offset_z);
    lab_record_z_c->setText(QString::number(offset_z,'f',2));
}

void Datum_parameter::on_btn_setall_clicked()
{

    if(get_pro_running())
        return;

    if(!get_task_homed(0) || !get_task_homed(1) || !get_task_homed(2)) {
        show_msg(1,tr("Cannot set the offset while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the offset"),
    tr("Set the offset of All axis.Do you realy want to set it?"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        this->set_a();
        this->set_b();
        this->set_c();
      break;
    case Cancel:

        break;
    default:
        break;
    }

}

void Datum_parameter::on_btn_Explain_clicked()
{
    explain->show();
}
