#include "offsetdialog.h"
#include "ui_offsetdialog.h"
#include "QTimer"
#include "emc.hh"
#include "emc_nml.hh"
#include <QtDebug>
#include "shcom.hh"
#include <QFileDialog>
#include <inifile.hh>
#include <QTimer>
#include "extern.h"
#include "emcstatus.hh"

#include <QMessageBox>
#include <QEvent>
#include <QKeyEvent>
#include "mywidget.h"

OffsetDialog::OffsetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OffsetDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("OffsetDialog"));


    this->parentl = parent;
    connect(ui->btn_ensure,SIGNAL(clicked()),SLOT(set_offset()));
    connect(ui->piece_offset_x,SIGNAL(valueChanged(double)),SLOT(get_piece_offset_x(double)));
    connect(ui->piece_offset_y,SIGNAL(valueChanged(double)),SLOT(get_piece_offset_y(double)));
    connect(ui->piece_offset_z1,SIGNAL(valueChanged(double)),SLOT(get_piece_offset_z1(double)));
    connect(ui->piece_offset_z2,SIGNAL(valueChanged(double)),SLOT(get_piece_offset_z2(double)));
    connect(ui->public_offset_x,SIGNAL(valueChanged(double)),SLOT(get_public_offset_x(double)));
    connect(ui->public_offset_y,SIGNAL(valueChanged(double)),SLOT(get_public_offset_y(double)));
    connect(ui->public_offset_z1,SIGNAL(valueChanged(double)),SLOT(get_public_offset_z1(double)));
    connect(ui->public_offset_z2,SIGNAL(valueChanged(double)),SLOT(get_public_offset_z2(double)));

    connect(ui->btn_dec_1,SIGNAL(released()),this,SLOT(set_deepen_1()));
    connect(ui->btn_dec_01,SIGNAL(released()),this,SLOT(set_deepen_01()));
    connect(ui->btn_dec_001,SIGNAL(released()),this,SLOT(set_deepen_001()));
    connect(ui->btn_raise_1,SIGNAL(released()),this,SLOT(set_raise_1()));
    connect(ui->btn_raise_01,SIGNAL(released()),this,SLOT(set_raise_01()));
    connect(ui->btn_raise_001,SIGNAL(released()),this,SLOT(set_raise_001()));

    connect(ui->btn_record_x,SIGNAL(released()),this,SLOT(set_record_x()));
    connect(ui->btn_record_y,SIGNAL(released()),this,SLOT(set_record_y()));
    connect(ui->btn_set_x,SIGNAL(released()),this,SLOT(set_x()));
    connect(ui->btn_set_y,SIGNAL(released()),this,SLOT(set_y()));
    connect(ui->btn_set_xy,SIGNAL(released()),this,SLOT(set_xy()));


    connect(ui->btn_dichotomy_x,SIGNAL(released()),this,SLOT(set_dichotomy_x()));
    connect(ui->btn_dichotomy_y,SIGNAL(released()),this,SLOT(set_dichotomy_y()));

    connect(ui->btn_cancel,SIGNAL(released()),this,SLOT(close()));

    flush_time = new QTimer(this);
    connect(flush_time,SIGNAL(timeout()),this,SLOT(status_update()));

    ui->public_offset_x->installEventFilter(this);
    ui->public_offset_y->installEventFilter(this);
    ui->public_offset_z1->installEventFilter(this);
    ui->public_offset_z2->installEventFilter(this);
    ui->piece_offset_x->installEventFilter(this);
    ui->piece_offset_y->installEventFilter(this);
    ui->piece_offset_z1->installEventFilter(this);
    ui->piece_offset_z2->installEventFilter(this);

    ui->piece_coor_x->installEventFilter(this);
    ui->piece_coor_y->installEventFilter(this);
    ui->piece_coor_z1->installEventFilter(this);
    ui->piece_coor_z2->installEventFilter(this);

    if(!_doublez) {
        ui->public_offset_z2->hide();
        ui->piece_coor_z2->hide();
        ui->piece_offset_z2->hide();
        ui->label_p_z2->hide();
        ui->label_o_z2->hide();
        ui->label_c_z2->hide();

        ui->label_p_z1->setText("Z:");
        ui->label_o_z1->setText("Z:");
        ui->label_c_z1->setText("Z:");
    }


}

void OffsetDialog::setFromCurrentPos(AXIS_NAME axis)
{

    initValue();
    if(axis == AXIS_X)
        piece_x=get_abs_pos(AXIS_X);
    else if(axis == AXIS_Y)
        piece_y=get_abs_pos(AXIS_Y);
    else if(axis == AXIS_Z1)
        piece_z1 = get_abs_pos(AXIS_Z1);
    else if(axis == AXIS_A)
        piece_z2 = get_abs_pos(AXIS_A);


    real_offset_x = public_x+piece_x;
    real_offset_y = public_y+piece_y;
    real_offset_z1 = public_z1+piece_z1;
    real_offset_z2 = public_z2+piece_z2;

    set_offset();
    fromParent = true;
    return;

}

void OffsetDialog::initValue()
{
    if(2 == emcStatus->motion.doublez.whichz_flag){
        ui->public_offset_z1->setEnabled(false);
        ui->public_offset_z2->setEnabled(false);
        ui->piece_offset_z1->setEnabled(false);
        ui->piece_offset_z2->setEnabled(false);
    }else if(1 == emcStatus->motion.doublez.whichz_flag){
        ui->public_offset_z1->setEnabled(true);
        ui->piece_offset_z1->setEnabled(true);

        ui->public_offset_z2->setEnabled(true);
        ui->piece_offset_z2->setEnabled(true);

    }else if(0 == emcStatus->motion.doublez.whichz_flag){
        ui->public_offset_z2->setEnabled(true);
        ui->piece_offset_z2->setEnabled(true);

        ui->public_offset_z1->setEnabled(true);
        ui->piece_offset_z1->setEnabled(true);
    }

    public_x = _prefer->read("PUBLIC_OFFSET","offset_x","0").toDouble();
    ui->public_offset_x->setValue(public_x);
    public_y = _prefer->read("PUBLIC_OFFSET","offset_y","0").toDouble();
    ui->public_offset_y->setValue(public_y);
    public_z1 = _prefer->read("PUBLIC_OFFSET","offset_z1","0").toDouble();
    ui->public_offset_z1->setValue(public_z1);
    public_z2 = _prefer->read("PUBLIC_OFFSET","offset_z2","0").toDouble();
    ui->public_offset_z2->setValue(public_z2);

    if(_doublez) {
        if(0 == emcStatus->motion.doublez.whichz_flag ||
                2 == emcStatus->motion.doublez.whichz_flag ) {
            real_offset_x = par_get_var(5341);
            real_offset_y = par_get_var(5342);
        } else if(1 == emcStatus->motion.doublez.whichz_flag)
        {
            real_offset_x = par_get_var(5361);
            real_offset_y = par_get_var(5362);
        }
        real_offset_z1 = par_get_var(5343);
        real_offset_z2 = par_get_var(5363);
    } else {
        real_offset_x = par_get_var(5221);
        real_offset_y = par_get_var(5222);
        real_offset_z1 = par_get_var(5223);
    }

    piece_x = real_offset_x - public_x ;
    piece_y = real_offset_y - public_y ;
    piece_z1 = real_offset_z1 - public_z1 ;
    piece_z2 = real_offset_z2 - public_z2 ;


    org_piece_x = piece_x;
    org_piece_y = piece_y;
    org_piece_z1 = piece_z1;
    org_piece_z2 = piece_z2;

    ui->piece_offset_x->setValue(piece_x);
    ui->piece_offset_y->setValue(piece_y); 
    ui->piece_offset_z1->setValue(piece_z1);
    ui->piece_offset_z2->setValue(piece_z2);

    record_x = _prefer->read("RECORD_VALUE","record_x","0").toDouble();
    ui->show_record_x->setText(QString("%1").arg(record_x));
    record_y = _prefer->read("RECORD_VALUE","record_y","0").toDouble();
    ui->show_record_y->setText(QString("%1").arg(record_y));

    ui->piece_coor_x->setText(QString::number(public_x+piece_x,'f',3));
    ui->piece_coor_y->setText(QString::number(public_y+piece_y,'f',3));
    ui->piece_coor_z1->setText(QString::number(public_z1+piece_z1,'f',3));
    ui->piece_coor_z2->setText(QString::number(public_z2+piece_z2,'f',3));

    fromParent = false;

}

bool OffsetDialog::checkModify()
{

    double tmp;
    tmp = _prefer->read("PUBLIC_OFFSET","offset_x","0").toDouble();
    if(tmp != ui->public_offset_x->value())
        return true;
    tmp = _prefer->read("PUBLIC_OFFSET","offset_y","0").toDouble();
    if(tmp != ui->public_offset_y->value())
        return true;
    tmp = _prefer->read("PUBLIC_OFFSET","offset_z1","0").toDouble();
    if(tmp != ui->public_offset_z1->value())
        return true;
    tmp = _prefer->read("PUBLIC_OFFSET","offset_z2","0").toDouble();
    if(tmp != ui->public_offset_z2->value())
        return true;

    if(fabs(org_piece_x - ui->piece_offset_x->value()) >= 0.001) {

        return true;
    }


    if(fabs(org_piece_y - ui->piece_offset_y->value()) >= 0.001) {

        return true;
    }

    if(fabs(org_piece_z1 - ui->piece_offset_z1->value()) >= 0.001) {

        return true;
    }

    if(fabs(org_piece_z2 - ui->piece_offset_z2->value()) >= 0.001) {

        return true;
    }

    return false;
}

void OffsetDialog::reject()
{
    this->close();

}

void OffsetDialog::closeEvent(QCloseEvent *evt)
{
    if(checkModify()) {
        int ret = QMessageBox::warning(this,tr("tips"),
        tr("The value has been modified.\\nDo you realy want to exit?")
        ,tr("Ok"),tr("Cancel"));
        switch (ret) {
        case Ok:
            evt->accept();
            break;
        case Cancel:
            evt->ignore();
            break;
        default:
            break;
        }
    } else {
        evt->accept();
    }

}

bool OffsetDialog::eventFilter(QObject *obj, QEvent *evt)
{

    if(obj==ui->public_offset_x || obj==ui->public_offset_y ||
       obj==ui->public_offset_z1 || obj==ui->public_offset_z2 ||
       obj==ui->piece_offset_x || obj==ui->piece_offset_y ||
       obj==ui->piece_offset_z1 || obj==ui->piece_offset_z2) {

        if(evt->type() == QEvent::MouseButtonPress ||
           evt->type() == QEvent::MouseButtonRelease ||
           evt->type() == QEvent::MouseButtonDblClick ||
                evt->type() == QEvent::Wheel) {

                 return true;
        }

    }

    if(obj==ui->piece_coor_x || obj==ui->piece_coor_y ||
       obj==ui->piece_coor_z1 || obj==ui->piece_coor_z2) {

        if(evt->type() == QEvent::MouseButtonPress ||
           evt->type() == QEvent::MouseButtonRelease ||
           evt->type() == QEvent::MouseButtonDblClick ||
                evt->type() == QEvent::Wheel ||
                evt->type() == QEvent::KeyPress ||
                evt->type() == QEvent::KeyRelease) {

                 return true;
        }

    }

    return QDialog::eventFilter(obj,evt);

}

QTimer *OffsetDialog::getTimer()
{
    return flush_time;
}

void OffsetDialog::status_update()
{
    static double public_x_old = 0.0,piece_x_old = 0.0;
    static double public_y_old = 0.0,piece_y_old = 0.0;
    static double public_z1_old = 0.0,piece_z1_old = 0.0;
    static double public_z2_old = 0.0,piece_z2_old = 0.0;

    if(public_x_old != public_x || public_y_old != public_y
       || public_z1_old != public_z1 || public_z2_old != public_z2 ||
        piece_x_old != piece_x || piece_y_old != piece_y
       || piece_z1_old != piece_z1 || piece_z2_old != piece_z2){

        fromParent = false;

        real_offset_x = public_x+piece_x;
        real_offset_y = public_y+piece_y;
        real_offset_z1 = public_z1+piece_z1;
        real_offset_z2 = public_z2+piece_z2;
        ui->piece_coor_x->setText(QString::number(real_offset_x,'f',3));
        ui->piece_coor_y->setText(QString::number(real_offset_y,'f',3));
        ui->piece_coor_z1->setText(QString::number(real_offset_z1,'f',3));
        ui->piece_coor_z2->setText(QString::number(real_offset_z2,'f',3));
        public_x_old = public_x;public_y_old = public_y;
        public_z1_old = public_z1;public_z2_old = public_z2;
        piece_x_old = piece_x;piece_y_old = piece_y;
        piece_z1_old = piece_z1;piece_z2_old = piece_z2;


    }



}

void OffsetDialog::mdiExcuteRsult(bool ok)
{
    if(ok) {
        _prefer->writeEntry("PUBLIC_OFFSET/offset_x",QString::number(public_x,'f',3));
        _prefer->writeEntry("PUBLIC_OFFSET/offset_y",QString::number(public_y,'f',3));
        _prefer->writeEntry("PUBLIC_OFFSET/offset_z1",QString::number(public_z1,'f',3));
        _prefer->writeEntry("PUBLIC_OFFSET/offset_z2",QString::number(public_z2,'f',3));
        _prefer->writeEntry("PIECE_OFFSET/offset_x",QString::number(piece_x,'f',3));
        _prefer->writeEntry("PIECE_OFFSET/offset_y",QString::number(piece_y,'f',3));
        _prefer->writeEntry("PIECE_OFFSET/offset_z1",QString::number(piece_z1,'f',3));
        _prefer->writeEntry("PIECE_OFFSET/offset_z2",QString::number(piece_z2,'f',3));
        org_piece_x = piece_x;
        org_piece_y = piece_y;
        org_piece_z1 = piece_z1;
        org_piece_z2 = piece_z2;
        if(fromParent);
          //QMessageBox::information(parentl,tr("information"),
        //tr("set offset complete"));
        else
            QMessageBox::information(this,tr("information"),
                                     tr("set offset complete"),tr("Ok"));

    } else {
        initValue();
        if(fromParent)
            QMessageBox::warning(parentl,tr("warning"),
                                 tr("set offset fail!!"),tr("Ok"));
        else
            QMessageBox::warning(this,tr("warning"),
                                 tr("set offset fail!!"),tr("Ok"));
    }
}

void OffsetDialog::mdiState(QString msg)
{

    if(mdimsg.size() >=1000)
        mdimsg.takeLast();
    mdimsg.prepend(msg);

    mdiRefresh();

}

void OffsetDialog::mdiRefresh()
{

    QString tmp,read;
    int check = 0,allreadyget = 0;
    for(int i=0;i<mdiserials.size();i++) {
        int ser = mdiserials.at(i);
        tmp = QString("[%1]").arg(ser);
        for(int j=0;j<mdimsg.size();j++) {
            read = mdimsg.at(j);
            if(read.contains(tmp)) {
                if(read.contains("MDI_ERROR")) {
                    check = -1;
                    break;
                } else {
                    check = 0;
                }
                allreadyget ++;
            }
        }
        if (check == -1) {
            allreadyget = mdiserials.size();
            break;
        }
    }
    if(mdiserials.size() >0 && allreadyget == mdiserials.size()) {
        mdiserials.clear();
        mdimsg.clear();
        if(check == -1)
            mdiExcuteRsult(false);
        else
            mdiExcuteRsult(true);
    }

}

void OffsetDialog::set_offset()
{

    static myLock locker;
    if(locker.isLock()) {
        return;
     }else
        locker.setLock(400);

    if(get_mdi_running())
        return;

    QString cmd_coor;


    if(_doublez) {
        if(0 == emcStatus->motion.doublez.whichz_flag || 2 == emcStatus->motion.doublez.whichz_flag
                || 0 == _zmode) {
            mdiserials.clear();
            cmd_coor.append("G10L2P7").append("X").append(QString::number(real_offset_x,'f',3))
                    .append("Y").append(QString::number(real_offset_y,'f',3))
                    .append("Z").append(QString::number(real_offset_z1,'f',3));
            sendMdi();
            int retSeri = -1;
            sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
            mdiserials.append(retSeri);

            cmd_coor.clear();
            cmd_coor.append("G10L2P9").append("X").append(QString::number(real_offset_x,'f',3))
                    .append("Y").append(QString::number(real_offset_y,'f',3))
                    .append("Z").append(QString::number(real_offset_z1,'f',3));
            sendMdi();
            sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
            mdiserials.append(retSeri);

            if(0 == _zmode) {
                cmd_coor.clear();
                cmd_coor.append("G10L2P8").append("X").append(QString::number(real_offset_x,'f',3))
                        .append("Y").append(QString::number(real_offset_y,'f',3))
                        .append("Z").append(QString::number(real_offset_z2,'f',3));
                sendMdi();
                int retSeri = -1;
                sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
                mdiserials.append(retSeri);
            } else {
                cmd_coor.clear();
                cmd_coor.append("G10L2P8").append("z").append(QString::number(real_offset_z2,'f',3));
                sendMdi();
                sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
                mdiserials.append(retSeri);
            }
        } else if(1 == emcStatus->motion.doublez.whichz_flag) {
            cmd_coor.clear();
            cmd_coor.append("G10L2P8").append("X").append(QString::number(real_offset_x,'f',3))
                    .append("Y").append(QString::number(real_offset_y,'f',3))
                    .append("Z").append(QString::number(real_offset_z2,'f',3));
            sendMdi();
            int retSeri = -1;
            sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
            mdiserials.append(retSeri);

            cmd_coor.clear();
            cmd_coor.append("G10L2P7").append("Z").append(QString::number(real_offset_z1,'f',3));
            sendMdi();
            sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
            mdiserials.append(retSeri);

        }
    } else {
        mdiserials.clear();
        cmd_coor.append("G10L2P1").append("X").append(QString::number(real_offset_x,'f',3))
                .append("Y").append(QString::number(real_offset_y,'f',3))
                .append("Z").append(QString::number(real_offset_z1,'f',3));
        sendMdi();
        int retSeri = -1;
        sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
        mdiserials.append(retSeri);

    }

}

void OffsetDialog::get_piece_offset_x(double val)
{
    piece_x = val;
}
void OffsetDialog::get_piece_offset_y(double val)
{
    piece_y = val;
}
void OffsetDialog::get_piece_offset_z1(double val)
{
    piece_z1 = val;
}
void OffsetDialog::get_piece_offset_z2(double val)
{
    piece_z2 = val;
}
void OffsetDialog::get_public_offset_x(double val)
{
    public_x = val;
}
void OffsetDialog::get_public_offset_y(double val)
{
    public_y = val;
}
void OffsetDialog::get_public_offset_z1(double val)
{
    public_z1 = val;
}
void OffsetDialog::get_public_offset_z2(double val)
{
    public_z2 = val;
}

void OffsetDialog::set_deepen_001(){
    if(0 == emcStatus->motion.doublez.whichz_flag){
        public_z1 -= 0.01;
        ui->public_offset_z1->setValue(public_z1);
    }else if(1 == emcStatus->motion.doublez.whichz_flag){
        public_z2 -= 0.01;
        ui->public_offset_z2->setValue(public_z2);
    }else if(2 == emcStatus->motion.doublez.whichz_flag){
        public_z2 -= 0.01;
        public_z1 -= 0.01;
        ui->public_offset_z2->setValue(public_z2);
        ui->public_offset_z1->setValue(public_z1);
    }
}
void OffsetDialog::set_deepen_01(){
    if(0 == emcStatus->motion.doublez.whichz_flag){
        public_z1 -= 0.1;
        ui->public_offset_z1->setValue(public_z1);
    }else if(1 == emcStatus->motion.doublez.whichz_flag){
        public_z2 -= 0.1;
        ui->public_offset_z2->setValue(public_z2);
    }else if(2 == emcStatus->motion.doublez.whichz_flag){
        public_z2 -= 0.1;
        public_z1 -= 0.1;
        ui->public_offset_z2->setValue(public_z2);
        ui->public_offset_z1->setValue(public_z1);
    }
}
void OffsetDialog::set_deepen_1(){
    if(0 == emcStatus->motion.doublez.whichz_flag){
        public_z1 -= 1;
        ui->public_offset_z1->setValue(public_z1);
    }else if(1 == emcStatus->motion.doublez.whichz_flag){
        public_z2 -= 1;
        ui->public_offset_z2->setValue(public_z2);
    }else if(2 == emcStatus->motion.doublez.whichz_flag){
        public_z2 -= 1;
        public_z1 -= 1;
        ui->public_offset_z2->setValue(public_z2);
        ui->public_offset_z1->setValue(public_z1);
    }
}
void OffsetDialog::set_raise_001(){
    if(0 == emcStatus->motion.doublez.whichz_flag){
        public_z1 += 0.01;
        ui->public_offset_z1->setValue(public_z1);
    }else if(1 == emcStatus->motion.doublez.whichz_flag){
        public_z2 += 0.01;
        ui->public_offset_z2->setValue(public_z2);
    }else if(2 == emcStatus->motion.doublez.whichz_flag){
        public_z2 += 0.01;
        public_z1 += 0.01;
        ui->public_offset_z2->setValue(public_z2);
        ui->public_offset_z1->setValue(public_z1);
    }
}
void OffsetDialog::set_raise_01(){
    if(0 == emcStatus->motion.doublez.whichz_flag){
        public_z1 += 0.1;
        ui->public_offset_z1->setValue(public_z1);
    }else if(1 == emcStatus->motion.doublez.whichz_flag){
        public_z2 += 0.1;
        ui->public_offset_z2->setValue(public_z2);
    }else if(2 == emcStatus->motion.doublez.whichz_flag){
        public_z2 += 0.1;
        public_z1 += 0.1;
        ui->public_offset_z2->setValue(public_z2);
        ui->public_offset_z1->setValue(public_z1);
    }
}
void OffsetDialog::set_raise_1(){
    if(0 == emcStatus->motion.doublez.whichz_flag){
        public_z1 += 1;
        ui->public_offset_z1->setValue(public_z1);
    }else if(1 == emcStatus->motion.doublez.whichz_flag){
        public_z2 += 1;
        ui->public_offset_z2->setValue(public_z2);
    }else if(2 == emcStatus->motion.doublez.whichz_flag){
        public_z2 += 1;
        public_z1 += 1;
        ui->public_offset_z2->setValue(public_z2);
        ui->public_offset_z1->setValue(public_z1);
    }
}

void OffsetDialog::set_x()
{

  ui->piece_offset_x->setValue( get_abs_pos(AXIS_X));

}

void OffsetDialog::set_y()
{

  ui->piece_offset_y->setValue( get_abs_pos(AXIS_Y));

}

void OffsetDialog::set_record_x()
{
    QString temp = "";
    record_x = get_abs_pos(AXIS_X);
    temp = QString::number(record_x,'f',3);
    ui->show_record_x->setText(QString("%1").arg(record_x));
    _prefer->writeEntry("RECORD_VALUE/record_x",temp);
}

void OffsetDialog::set_record_y()
{
    QString temp = "";
    record_y = get_abs_pos(AXIS_Y);
    temp = QString::number(record_y,'f',3);
    ui->show_record_y->setText(QString("%1").arg(record_y));
    _prefer->writeEntry("RECORD_VALUE/record_y",temp);
}

void OffsetDialog::set_dichotomy_x()
{
    piece_x = (get_abs_pos(AXIS_X) + record_x)/2.0;
    ui->piece_offset_x->setValue(piece_x);
}

void OffsetDialog::set_dichotomy_y()
{
    piece_y = (get_abs_pos(AXIS_Y) + record_y)/2.0;
    ui->piece_offset_y->setValue(piece_y);
}

OffsetDialog::~OffsetDialog()
{
    delete ui;
}
