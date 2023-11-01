#include "multioffset.h"
#include "ui_multioffset.h"
#include "QTimer"
#include "emc.hh"
#include "emc_nml.hh"
#include <QtDebug>
#include "shcom.hh"
#include <QFileDialog>
#include <inifile.hh>
#include <QTimer>
#include <QTabWidget>
#include "extern.h"
#include "emcstatus.hh"
#include <unistd.h>


#include <QVBoxLayout>
#include <QHBoxLayout>
 #include <QGridLayout>
#include <QMessageBox>
#include <QEvent>
#include <QKeyEvent>
#include <QDesktopWidget>
#include "mywidget.h"

multioffset::multioffset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::multioffset)
{
    ui->setupUi(this);
    setWindowTitle(tr("multioffset"));

    this->parentl = parent;
    connect(ui->btn_ensure,SIGNAL(clicked()),SLOT(set_offset()));
    connect(ui->public_offset_x,SIGNAL(valueChanged(double)),SLOT(get_public_offset_x(double)));
    connect(ui->public_offset_y,SIGNAL(valueChanged(double)),SLOT(get_public_offset_y(double)));
    connect(ui->public_offset_z1,SIGNAL(valueChanged(double)),SLOT(get_public_offset_z1(double)));
    connect(ui->public_offset_z2,SIGNAL(valueChanged(double)),SLOT(get_public_offset_z2(double)));

    connect(ui->g54_x,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g54_y,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g54_z,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g54_a,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g55_x,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g55_y,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g55_z,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g55_a,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g56_x,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g56_y,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g56_z,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g56_a,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g57_x,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g57_y,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g57_z,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g57_a,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g58_x,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g58_y,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g58_z,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g58_a,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g59_x,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g59_y,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g59_z,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));
    connect(ui->g59_a,SIGNAL(valueChanged(double)),this,SLOT(updatemianshow()));

    /*************************************************/
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


    connect(ui->btn_dichotomy_x,SIGNAL(released()),this,SLOT(set_dichotomy_x()));
    connect(ui->btn_dichotomy_y,SIGNAL(released()),this,SLOT(set_dichotomy_y()));


    connect(ui->btn_cancel,SIGNAL(released()),this,SLOT(close()));

    connect(ui->set_g54,SIGNAL(toggled(bool)),this,SLOT(set_index_g54(bool)));
    connect(ui->set_g55,SIGNAL(toggled(bool)),this,SLOT(set_index_g55(bool)));
    connect(ui->set_g56,SIGNAL(toggled(bool)),this,SLOT(set_index_g56(bool)));
    connect(ui->set_g57,SIGNAL(toggled(bool)),this,SLOT(set_index_g57(bool)));
    connect(ui->set_g58,SIGNAL(toggled(bool)),this,SLOT(set_index_g58(bool)));
    connect(ui->set_g59,SIGNAL(toggled(bool)),this,SLOT(set_index_g59(bool)));

    flush_time = new QTimer(this);
    connect(flush_time,SIGNAL(timeout()),this,SLOT(status_update()));

    double height = 650;
    int size = 1;
    if(QApplication::desktop()->screenGeometry().height() < 850) {
        height = 530;
        size = 0;
    } else {
        size = 1;
    }

    //initValue();

    ui->public_offset_x->installEventFilter(this);
    ui->public_offset_y->installEventFilter(this);
    ui->public_offset_z1->installEventFilter(this);
    ui->public_offset_z2->installEventFilter(this);
    ui->piece_coor_x54->installEventFilter(this);
    ui->piece_coor_y54->installEventFilter(this);
    ui->piece_coor_z154->installEventFilter(this);
    ui->piece_coor_z254->installEventFilter(this);
    ui->g54_x->installEventFilter(this);
    ui->g54_y->installEventFilter(this);
    ui->g54_z->installEventFilter(this);
    ui->g55_x->installEventFilter(this);
    ui->g55_y->installEventFilter(this);
    ui->g55_z->installEventFilter(this);
    ui->g56_x->installEventFilter(this);
    ui->g56_y->installEventFilter(this);
    ui->g56_z->installEventFilter(this);
    ui->g57_x->installEventFilter(this);
    ui->g57_y->installEventFilter(this);
    ui->g57_z->installEventFilter(this);
    ui->g58_x->installEventFilter(this);
    ui->g58_y->installEventFilter(this);
    ui->g58_z->installEventFilter(this);
    ui->g59_x->installEventFilter(this);
    ui->g59_y->installEventFilter(this);
    ui->g59_z->installEventFilter(this);

    int usesinglez = 1;
    if(usesinglez) {
        ui->public_offset_z2->hide();
        ui->piece_coor_z254->hide();
        ui->label_p_z2->hide();
        ui->label_o_z2->hide();
        ui->label_c_z2->hide();
        ui->g54_a->hide();
        ui->g55_a->hide();
        ui->g56_a->hide();
        ui->g57_a->hide();
        ui->g58_a->hide();
        ui->g59_a->hide();
        ui->piece_coor_z254->hide();
        ui->piece_coor_z255->hide();
        ui->piece_coor_z256->hide();
        ui->piece_coor_z257->hide();
        ui->piece_coor_z258->hide();
        ui->piece_coor_z259->hide();


        ui->label_p_z1->setText("Z:");
        ui->label_o_z1->setText("Z:");
        ui->label_c_z1->setText("Z:");
        height -= 50;
    } else {

    }

    if(par_get_var(4061) ==0) {
        ui->piece_coor_x55->hide();
        ui->piece_coor_x56->hide();
        ui->piece_coor_x57->hide();
        ui->piece_coor_x58->hide();
        ui->piece_coor_x59->hide();
        ui->piece_coor_y55->hide();
        ui->piece_coor_y56->hide();
        ui->piece_coor_y57->hide();
        ui->piece_coor_y58->hide();
        ui->piece_coor_y59->hide();
        ui->piece_coor_z155->hide();
        ui->piece_coor_z156->hide();
        ui->piece_coor_z157->hide();
        ui->piece_coor_z158->hide();
        ui->piece_coor_z159->hide();
        ui->piece_coor_z255->hide();
        ui->piece_coor_z256->hide();
        ui->piece_coor_z257->hide();
        ui->piece_coor_z258->hide();
        ui->piece_coor_z259->hide();
        ui->set_g54->hide();
        ui->set_g55->hide();
        ui->set_g56->hide();
        ui->set_g57->hide();
        ui->set_g58->hide();
        ui->set_g59->hide();

        ui->g55_x->hide();
        ui->g56_x->hide();
        ui->g57_x->hide();
        ui->g58_x->hide();
        ui->g59_x->hide();
        ui->g55_y->hide();
        ui->g56_y->hide();
        ui->g57_y->hide();
        ui->g58_y->hide();
        ui->g59_y->hide();
        ui->g55_z->hide();
        ui->g56_z->hide();
        ui->g57_z->hide();
        ui->g58_z->hide();
        ui->g59_z->hide();
        ui->g55_a->hide();
        ui->g56_a->hide();
        ui->g57_a->hide();
        ui->g58_a->hide();
        ui->g59_a->hide();
        ui->widget_7->hide();
        ui->CHO_FAME->hide();

        ui->INCR_FRAME->hide();

        QVBoxLayout *incrlay = new QVBoxLayout(ui->INCR_FRAME);
        incrlay->addWidget(ui->groupBox_4);
        incrlay->setContentsMargins(2,1,2,1);
        QHBoxLayout *publiclay = new QHBoxLayout(ui->PUBLIC_FRAME);
        publiclay->addWidget(ui->groupBox);
        publiclay->addWidget(ui->groupBox_6);
//        publiclay->addWidget(ui->INCR_FRAME);
        publiclay->setContentsMargins(2,1,2,1);
        publiclay->setStretch(0,0);
        publiclay->setStretch(1,1);
        QHBoxLayout *piecelay = new QHBoxLayout(ui->PIECE_FRAME);
        piecelay->addWidget(ui->groupBox_2);
        piecelay->addWidget(ui->groupBox_5);
        piecelay->setContentsMargins(2,2,2,2);
        piecelay->setStretch(0,1);
        piecelay->setStretch(1,3);
        QHBoxLayout *coorlay = new QHBoxLayout(ui->COOR_FRAME);
        coorlay->addWidget(ui->groupBox_3);
//        coorlay->addWidget(ui->groupBox_6);
        coorlay->setContentsMargins(2,2,2,2);
        coorlay->setStretch(0,1);
        coorlay->setStretch(1,3);

        ui->groupBox->setMaximumWidth(130+size*50);
        ui->groupBox->setMinimumWidth(130+size*50);
        ui->groupBox_2->setMaximumWidth(130+size*50);
        ui->groupBox_2->setMinimumWidth(130+size*50);
        ui->groupBox_3->setMaximumWidth(130+size*50);
        ui->groupBox_3->setMinimumWidth(130+size*50);
        this->resize(450+size*100,height-85);
        if(size > 0)
            this->setStyleSheet("font: 13pt Sans Serif");
        else
            this->setStyleSheet("font: 9pt Sans Serif");
    } else {
        ui->INCR_FRAME->hide();
        ui->widget_7->hide();
        ui->CHO_FAME->hide();
        QVBoxLayout *incrlay = new QVBoxLayout(ui->INCR_FRAME);
        incrlay->addWidget(ui->groupBox_4);
        incrlay->addWidget(ui->groupBox_5);
        incrlay->setContentsMargins(2,2,2,2);
        incrlay->setSpacing(2);
        QHBoxLayout *publiclay = new QHBoxLayout(ui->PUBLIC_FRAME);
        publiclay->addWidget(ui->groupBox);
        publiclay->addWidget(ui->groupBox_6);
//        publiclay->addWidget(ui->INCR_FRAME);
        publiclay->setContentsMargins(2,2,2,2);
        publiclay->setStretch(0,0);
        publiclay->setStretch(1,1);
        QHBoxLayout *piecelay = new QHBoxLayout(ui->PIECE_FRAME);
        piecelay->addWidget(ui->groupBox_2);
        piecelay->setContentsMargins(2,2,2,2);
        QHBoxLayout *coorlay = new QHBoxLayout(ui->COOR_FRAME);
        coorlay->addWidget(ui->groupBox_3);
        coorlay->setContentsMargins(2,2,2,2);
        QHBoxLayout *chorlay = new QHBoxLayout(ui->CHO_FAME);
        chorlay->addWidget(ui->widget_7);
//        chorlay->addWidget(ui->groupBox_6);
        chorlay->setContentsMargins(2,2,2,2);
        chorlay->setStretch(0,0);
        chorlay->setStretch(1,1);
        this->resize(500+size*90,height);
        if(size > 0)
            this->setStyleSheet("font: 11pt Sans Serif");
        else
            this->setStyleSheet("font: 9pt Sans Serif");
    }
    QVBoxLayout *mainlay = new QVBoxLayout(this);
    mainlay->addWidget(ui->PUBLIC_FRAME);
    mainlay->addWidget(ui->PIECE_FRAME);
    mainlay->addWidget(ui->COOR_FRAME);
    mainlay->addWidget(ui->CHO_FAME);
    mainlay->addWidget(ui->frame);
    mainlay->setSpacing(1);
    mainlay->setContentsMargins(2,1,5,2);
    mainlay->setStretchFactor(ui->PUBLIC_FRAME,4);
    mainlay->setStretchFactor(ui->PIECE_FRAME,4);
    mainlay->setStretchFactor(ui->COOR_FRAME,4);
    mainlay->setStretchFactor(ui->CHO_FAME,2);
    mainlay->setStretchFactor(ui->frame,1);


    ui->COOR_FRAME->hide();
}

void multioffset::setFromCurrentPos(AXIS_NAME axis)
{

    initValue();
    if(axis == AXIS_X) {
        show_x[emcStatus->task.g5x_index-1]=get_abs_pos(AXIS_X);
    } else if(axis == AXIS_Y) {
        show_y[emcStatus->task.g5x_index-1]=get_abs_pos(AXIS_Y);
    } else if(axis == AXIS_Z1)
        show_z[emcStatus->task.g5x_index-1] = get_abs_pos(AXIS_Z1);
    else if(axis == AXIS_A)
        show_a[emcStatus->task.g5x_index-1] = get_abs_pos(AXIS_A);


    real_x[emcStatus->task.g5x_index-1] = public_x+show_x[emcStatus->task.g5x_index-1];
    real_y[emcStatus->task.g5x_index-1] = public_y+show_y[emcStatus->task.g5x_index-1];
    real_z[emcStatus->task.g5x_index-1] = public_z1+show_z[emcStatus->task.g5x_index-1];
    real_a[emcStatus->task.g5x_index-1] = public_z2+show_a[emcStatus->task.g5x_index-1];

    switch(emcStatus->task.g5x_index) {
    case 1:ui->g54_x->setValue(show_x[0]);ui->g54_y->setValue(show_y[0]);
        ui->g54_z->setValue(show_z[0]),ui->g54_a->setValue(show_a[0]);break;
    case 2:ui->g55_x->setValue(show_x[1]);ui->g55_y->setValue(show_y[1]);
        ui->g55_z->setValue(show_z[1]),ui->g55_a->setValue(show_a[1]);break;
    case 3:ui->g56_x->setValue(show_x[2]);ui->g56_y->setValue(show_y[2]);
        ui->g56_z->setValue(show_z[2]),ui->g56_a->setValue(show_a[2]);break;
    case 4:ui->g57_x->setValue(show_x[3]);ui->g57_y->setValue(show_y[3]);
        ui->g57_z->setValue(show_z[3]),ui->g57_a->setValue(show_a[3]);break;
    case 5:ui->g58_x->setValue(show_x[4]);ui->g58_y->setValue(show_y[4]);
        ui->g58_z->setValue(show_z[4]),ui->g58_a->setValue(show_a[4]);break;
    case 6:ui->g59_x->setValue(show_x[5]);ui->g59_y->setValue(show_y[5]);
        ui->g59_z->setValue(show_z[5]),ui->g59_a->setValue(show_a[5]);break;
    }

    set_offset();
    fromParent = true;
    return;

}

void multioffset::initValue()
{

    if(2 == emcStatus->motion.doublez.whichz_flag){
        ui->public_offset_z1->setEnabled(false);
        ui->public_offset_z2->setEnabled(false);
    }else if(1 == emcStatus->motion.doublez.whichz_flag){
        ui->public_offset_z1->setEnabled(true);
        ui->public_offset_z2->setEnabled(true);

    }else if(0 == emcStatus->motion.doublez.whichz_flag){
        ui->public_offset_z2->setEnabled(true);
        ui->public_offset_z1->setEnabled(true);

    }

    public_x = par_get_var(5101);
    ui->public_offset_x->setValue(public_x);
    public_y = par_get_var(5102);
    ui->public_offset_y->setValue(public_y);
    public_z1 = par_get_var(5103);
    ui->public_offset_z1->setValue(public_z1);
    public_z2 = par_get_var(5104);
    ui->public_offset_z2->setValue(public_z2);

    for(int i=0;i<6;i++) {
        real_x[i] = par_get_var(5221+i*20);
        real_y[i] = par_get_var(5222+i*20);
        if(par_get_var(4290) > 0) {
            real_z[i] = par_get_var(4801+i)+par_get_var(4303+5*(int)par_get_var(4288));
            real_a[i] = par_get_var(4811+i)+par_get_var(4703+5*(int)par_get_var(4289));
        } else {
            real_z[i] = par_get_var(5223+i*20);
            real_a[i] = par_get_var(5224+i*20);
        }

        show_x[i] = real_x[i] - public_x ;
        show_y[i] = real_y[i] - public_y ;
        show_z[i] = real_z[i] - public_z1 ;
        show_a[i] = real_a[i] - public_z2 ;
    }

    piece_x = real_offset_x - public_x ;
    piece_y = real_offset_y - public_y ;
    piece_z1 = real_offset_z1 - public_z1 ;
    piece_z2 = real_offset_z2 - public_z2 ;


    org_piece_x = piece_x;
    org_piece_y = piece_y;
    org_piece_z1 = piece_z1;
    org_piece_z2 = piece_z2;

    record_x = _prefer->read("RECORD_VALUE","record_x","0").toDouble();
    ui->show_record_x->setText(QString("%1").arg(record_x));
    record_y = _prefer->read("RECORD_VALUE","record_y","0").toDouble();
    ui->show_record_y->setText(QString("%1").arg(record_y));

 //   ui->piece_coor_x54->setText(QString::number(public_x+show_x[origin],'f',3));
//    ui->piece_coor_y54->setText(QString::number(public_y+show_y[origin],'f',3));
 //   ui->piece_coor_z154->setText(QString::number(public_z1+show_z[origin],'f',3));
 //   ui->piece_coor_z254->setText(QString::number(public_z2+show_a[origin],'f',3));

    fromParent = false;
    origin = emcStatus->task.g5x_index;
    ui->set_g54->setChecked(origin == 1);
    ui->set_g55->setChecked(origin == 2);
    ui->set_g56->setChecked(origin == 3);
    ui->set_g57->setChecked(origin == 4);
    ui->set_g58->setChecked(origin == 5);
    ui->set_g59->setChecked(origin == 6);

    ui->g54_x->setValue(real_x[0] - public_x);
    ui->g54_y->setValue(real_y[0] - public_y);
    ui->g54_z->setValue(real_z[0] - public_z1);
    ui->g54_a->setValue(real_a[0] - public_z2);
    ui->g55_x->setValue(real_x[1] - public_x);
    ui->g55_y->setValue(real_y[1] - public_y);
    ui->g55_z->setValue(real_z[1] - public_z1);
    ui->g55_a->setValue(real_a[1] - public_z2);
    ui->g56_x->setValue(real_x[2] - public_x);
    ui->g56_y->setValue(real_y[2] - public_y);
    ui->g56_z->setValue(real_z[2] - public_z1);
    ui->g56_a->setValue(real_a[2] - public_z2);
    ui->g57_x->setValue(real_x[3] - public_x);
    ui->g57_y->setValue(real_y[3] - public_y);
    ui->g57_z->setValue(real_z[3] - public_z1);
    ui->g57_a->setValue(real_a[3] - public_z2);
    ui->g58_x->setValue(real_x[4] - public_x);
    ui->g58_y->setValue(real_y[4] - public_y);
    ui->g58_z->setValue(real_z[4] - public_z1);
    ui->g58_a->setValue(real_a[4] - public_z2);
    ui->g59_x->setValue(real_x[5] - public_x);
    ui->g59_y->setValue(real_y[5] - public_y);
    ui->g59_z->setValue(real_z[5] - public_z1);
    ui->g59_a->setValue(real_a[5] - public_z2);


    if(1 == _ini->read("PAR_OPERATOR_SAFESETCOOR","VALUE","1").toInt()) {
        if(0 == get_task_z()) {
            ui->g54_a->setEnabled(false);
            ui->public_offset_z2->setEnabled(false);
            ui->g54_z->setEnabled(true);
            ui->public_offset_z1->setEnabled(true);

            ui->g55_a->setEnabled(false);
            ui->g56_a->setEnabled(false);
            ui->g57_a->setEnabled(false);
            ui->g58_a->setEnabled(false);
            ui->g59_a->setEnabled(false);

            ui->g55_z->setEnabled(true);
            ui->g56_z->setEnabled(true);
            ui->g57_z->setEnabled(true);
            ui->g58_z->setEnabled(true);
            ui->g59_z->setEnabled(true);
        } else if(1 == get_task_z()) {
            ui->public_offset_z1->setEnabled(false);
            ui->public_offset_z2->setEnabled(true);

            ui->g54_z->setEnabled(false);
            ui->g55_z->setEnabled(false);
            ui->g56_z->setEnabled(false);
            ui->g57_z->setEnabled(false);
            ui->g58_z->setEnabled(false);
            ui->g59_z->setEnabled(false);

            ui->g54_a->setEnabled(true);
            ui->g55_a->setEnabled(true);
            ui->g56_a->setEnabled(true);
            ui->g57_a->setEnabled(true);
            ui->g58_a->setEnabled(true);
            ui->g59_a->setEnabled(true);

        } else if(2 == get_task_z()) {
            ui->g54_z->setEnabled(true);
            ui->g55_z->setEnabled(true);
            ui->g56_z->setEnabled(true);
            ui->g57_z->setEnabled(true);
            ui->g58_z->setEnabled(true);
            ui->g59_z->setEnabled(true);
            ui->g54_a->setEnabled(true);
            ui->g55_a->setEnabled(true);
            ui->g56_a->setEnabled(true);
            ui->g57_a->setEnabled(true);
            ui->g58_a->setEnabled(true);
            ui->g59_a->setEnabled(true);

        }
    }

    if(par_get_var(5220) == 1) {
        set_index_g54(true);
    } else if(par_get_var(5220) == 2) {
        set_index_g55(true);
    } else if(par_get_var(5220) == 3) {
        set_index_g56(true);
    } else if(par_get_var(5220) == 4) {
        set_index_g57(true);
    } else if(par_get_var(5220) == 5) {
        set_index_g58(true);
    } else if(par_get_var(5220) == 6) {
        set_index_g59(true);
    }


}

bool multioffset::checkModify()
{

    double tmp;
    tmp = par_get_var(5101);
    if(tmp != ui->public_offset_x->value())
        return true;
    tmp = par_get_var(5102);
    if(tmp != ui->public_offset_y->value())
        return true;
    tmp = par_get_var(5103);
    if(tmp != ui->public_offset_z1->value())
        return true;
    tmp = par_get_var(5104);
    if(tmp != ui->public_offset_z2->value())
        return true;

    if(fabs(par_get_var(5221) - ui->g54_x->value()-public_x) >= 0.001)
        return true;
    if(fabs(par_get_var(5222) - ui->g54_y->value()-public_y) >= 0.001)
        return true;
    if(par_get_var(4290) > 0) {
    if(fabs(par_get_var(4801)+par_get_var(4303+5*(int)par_get_var(4288)) - ui->g54_z->value()-public_z1) >= 0.001)
        return true;
    if(fabs(par_get_var(4811)+par_get_var(4303+5*(int)par_get_var(4289)) - ui->g54_a->value()-public_z2) >= 0.001)
        return true;
    } else {
        if(fabs(par_get_var(5223) - ui->g54_z->value()-public_z1) >= 0.001)
            return true;
        if(fabs(par_get_var(5224) - ui->g54_a->value()-public_z2) >= 0.001)
            return true;
    }
    if(par_get_var(4061) > 0) {
    if(fabs(par_get_var(5241) - ui->g55_x->value()-public_x) >= 0.001)
        return true;
    if(fabs(par_get_var(5242) - ui->g55_y->value()-public_y) >= 0.001)
        return true;
    if(par_get_var(4290) > 0) {
    if(fabs(par_get_var(4802)+par_get_var(4303+5*(int)par_get_var(4288)) - ui->g55_z->value()-public_z1) >= 0.001)
        return true;
    if(fabs(par_get_var(4812)+par_get_var(4303+5*(int)par_get_var(4289))  - ui->g55_a->value()-public_z2) >= 0.001)
        return true;
    } else {
        if(fabs(par_get_var(5243) - ui->g55_z->value()-public_z1) >= 0.001)
            return true;
        if(fabs(par_get_var(5244) - ui->g55_a->value()-public_z2) >= 0.001)
            return true;
    }
    if(fabs(par_get_var(5261) - ui->g56_x->value()-public_x) >= 0.001)
        return true;
    if(fabs(par_get_var(5262) - ui->g56_y->value()-public_y) >= 0.001)
        return true;
    if(par_get_var(4290) > 0) {
    if(fabs(par_get_var(4803)+par_get_var(4303+5*(int)par_get_var(4288)) - ui->g56_z->value()-public_z1) >= 0.001)
        return true;
    if(fabs(par_get_var(4813)+par_get_var(4303+5*(int)par_get_var(4289))  - ui->g56_a->value()-public_z2) >= 0.001)
        return true;
    } else {
        if(fabs(par_get_var(5263) - ui->g56_z->value()-public_z1) >= 0.001)
            return true;
        if(fabs(par_get_var(5264) - ui->g56_a->value()-public_z2) >= 0.001)
            return true;
    }
    if(fabs(par_get_var(5281) - ui->g57_x->value()-public_x) >= 0.001)
        return true;
    if(fabs(par_get_var(5282) - ui->g57_y->value()-public_y) >= 0.001)
        return true;
    if(par_get_var(4290) > 0) {
    if(fabs(par_get_var(4804)+par_get_var(4303+5*(int)par_get_var(4288)) - ui->g57_z->value()-public_z1) >= 0.001)
        return true;
    if(fabs(par_get_var(4814)+par_get_var(4303+5*(int)par_get_var(4289))  - ui->g57_a->value()-public_z2) >= 0.001)
        return true;
    } else {
        if(fabs(par_get_var(5283) - ui->g57_z->value()-public_z1) >= 0.001)
            return true;
        if(fabs(par_get_var(5284) - ui->g57_a->value()-public_z2) >= 0.001)
            return true;
    }
    if(fabs(par_get_var(5301) - ui->g58_x->value()-public_x) >= 0.001)
        return true;
    if(fabs(par_get_var(5302) - ui->g58_y->value()-public_y) >= 0.001)
        return true;
    if(par_get_var(4290) > 0) {
        if(fabs(par_get_var(4805)+par_get_var(4303+5*(int)par_get_var(4288)) - ui->g58_z->value()-public_z1) >= 0.001)
            return true;
        if(fabs(par_get_var(4815)+par_get_var(4303+5*(int)par_get_var(4289))  - ui->g58_a->value()-public_z2) >= 0.001)
            return true;
    } else {
        if(fabs(par_get_var(5303) - ui->g58_z->value()-public_z1) >= 0.001)
            return true;
        if(fabs(par_get_var(5304) - ui->g58_a->value()-public_z2) >= 0.001)
            return true;
    }
    if(fabs(par_get_var(5321) - ui->g59_x->value()-public_x) >= 0.001)
        return true;
    if(fabs(par_get_var(5322) - ui->g59_y->value()-public_y) >= 0.001)
        return true;
    if(par_get_var(4290) > 0) {
    if(fabs(par_get_var(4806)+par_get_var(4303+5*(int)par_get_var(4288)) - ui->g59_z->value()-public_z1) >= 0.001)
        return true;
    if(fabs(par_get_var(4816)+par_get_var(4303+5*(int)par_get_var(4289))  - ui->g59_a->value()-public_z2) >= 0.001)
        return true;
    }
    } else {
        if(fabs(par_get_var(5323) - ui->g59_z->value()-public_z1) >= 0.001)
            return true;
        if(fabs(par_get_var(5324) - ui->g59_a->value()-public_z2) >= 0.001)
            return true;
    }

    return false;
}

void multioffset::reject()
{
    this->close();

}

void multioffset::closeEvent(QCloseEvent *evt)
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

bool multioffset::eventFilter(QObject *obj, QEvent *evt)
{

    if(obj==ui->public_offset_x || obj==ui->public_offset_y ||
       obj==ui->public_offset_z1 || obj==ui->public_offset_z2 ||
 //      obj==ui->piece_offset_x || obj==ui->piece_offset_y ||
 //      obj==ui->piece_offset_z1 || obj==ui->piece_offset_z2 ||
//      obj==ui->public_x || obj==ui->public_y ||
//       obj==ui->public_z  ||
            obj == ui->g54_x || obj == ui->g54_y || obj == ui->g54_z ||
            obj == ui->g55_x || obj == ui->g55_y || obj == ui->g55_z ||
            obj == ui->g56_x || obj == ui->g56_y || obj == ui->g56_z ||
            obj == ui->g57_x || obj == ui->g57_y || obj == ui->g57_z ||
            obj == ui->g58_x || obj == ui->g58_y || obj == ui->g58_z ||
            obj == ui->g59_x || obj == ui->g59_y || obj == ui->g59_z) {

        if(evt->type() == QEvent::MouseButtonPress ||
           evt->type() == QEvent::MouseButtonRelease ||
           evt->type() == QEvent::MouseButtonDblClick ||
                evt->type() == QEvent::Wheel) {

                 return true;
        }

    }

    if(obj==ui->piece_coor_x54 || obj==ui->piece_coor_y54 ||
       obj==ui->piece_coor_z154 || obj==ui->piece_coor_z254) {

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

QTimer *multioffset::getTimer()
{
    return flush_time;
}

void multioffset::status_update()
{
    static double public_x_old = 0.0;
    static double public_y_old = 0.0;
    static double public_z1_old = 0.0;
    static double public_z2_old = 0.0;
    static double public_54x_old = 0.0,public_54z1_old = 0.0;
    static double public_54y_old = 0.0,public_54z2_old = 0.0;
    static double public_55x_old = 0.0,public_55z1_old = 0.0;
    static double public_55y_old = 0.0,public_55z2_old = 0.0;
    static double public_56x_old = 0.0,public_56z1_old = 0.0;
    static double public_56y_old = 0.0,public_56z2_old = 0.0;
    static double public_57x_old = 0.0,public_57z1_old = 0.0;
    static double public_57y_old = 0.0,public_57z2_old = 0.0;
    static double public_58x_old = 0.0,public_58z1_old = 0.0;
    static double public_58y_old = 0.0,public_58z2_old = 0.0;
    static double public_59x_old = 0.0,public_59z1_old = 0.0;
    static double public_59y_old = 0.0,public_59z2_old = 0.0;

    if(public_x_old != public_x || public_y_old != public_y
       || public_z1_old != public_z1 || public_z2_old != public_z2 ||
            public_54x_old != show_x[0]  || public_54y_old != show_y[0] ||
            public_54z1_old != show_z[0] || public_54z2_old != show_a[0] ||
            public_55x_old != show_x[1]  || public_55y_old != show_y[1] ||
            public_55z1_old != show_z[1] || public_55z2_old != show_a[1] ||
            public_56x_old != show_x[2]  || public_56y_old != show_y[2] ||
            public_56z1_old != show_z[2] || public_56z2_old != show_a[2] ||
            public_57x_old != show_x[3]  || public_57y_old != show_y[3] ||
            public_57z1_old != show_z[3] || public_57z2_old != show_a[3] ||
            public_58x_old != show_x[4]  || public_58y_old != show_y[4] ||
            public_58z1_old != show_z[4] || public_58z2_old != show_a[4] ||
            public_59x_old != show_x[5]  || public_59y_old != show_y[5] ||
            public_59z1_old != show_z[5] || public_59z2_old != show_a[5] ) {

        real_x[0] = public_x+show_x[0];
        real_y[0] = public_y+show_y[0];
        real_z[0] = public_z1+show_z[0];
        real_a[0] = public_z2+show_a[0];
        ui->piece_coor_x54->setText(QString::number(real_x[0],'f',3));
        ui->piece_coor_y54->setText(QString::number(real_y[0],'f',3));
        ui->piece_coor_z154->setText(QString::number(real_z[0],'f',3));
        ui->piece_coor_z254->setText(QString::number(real_a[0],'f',3));
        real_x[1] = public_x+show_x[1];
        real_y[1] = public_y+show_y[1];
        real_z[1] = public_z1+show_z[1];
        real_a[1] = public_z2+show_a[1];
        ui->piece_coor_x55->setText(QString::number(real_x[1],'f',3));
        ui->piece_coor_y55->setText(QString::number(real_y[1],'f',3));
        ui->piece_coor_z155->setText(QString::number(real_z[1],'f',3));
        ui->piece_coor_z255->setText(QString::number(real_a[1],'f',3));
        real_x[2] = public_x+show_x[2];
        real_y[2] = public_y+show_y[2];
        real_z[2] = public_z1+show_z[2];
        real_a[2] = public_z2+show_a[2];
        ui->piece_coor_x56->setText(QString::number(real_x[2],'f',3));
        ui->piece_coor_y56->setText(QString::number(real_y[2],'f',3));
        ui->piece_coor_z156->setText(QString::number(real_z[2],'f',3));
        ui->piece_coor_z256->setText(QString::number(real_a[2],'f',3));
        real_x[3] = public_x+show_x[3];
        real_y[3] = public_y+show_y[3];
        real_z[3] = public_z1+show_z[3];
        real_a[3] = public_z2+show_a[3];
        ui->piece_coor_x57->setText(QString::number(real_x[3],'f',3));
        ui->piece_coor_y57->setText(QString::number(real_y[3],'f',3));
        ui->piece_coor_z157->setText(QString::number(real_z[3],'f',3));
        ui->piece_coor_z257->setText(QString::number(real_a[3],'f',3));
        real_x[4] = public_x+show_x[4];
        real_y[4] = public_y+show_y[4];
        real_z[4] = public_z1+show_z[4];
        real_a[4] = public_z2+show_a[4];
        ui->piece_coor_x58->setText(QString::number(real_x[4],'f',3));
        ui->piece_coor_y58->setText(QString::number(real_y[4],'f',3));
        ui->piece_coor_z158->setText(QString::number(real_z[4],'f',3));
        ui->piece_coor_z258->setText(QString::number(real_a[4],'f',3));
        real_x[5] = public_x+show_x[5];
        real_y[5] = public_y+show_y[5];
        real_z[5] = public_z1+show_z[5];
        real_a[5] = public_z2+show_a[5];
        ui->piece_coor_x59->setText(QString::number(real_x[5],'f',3));
        ui->piece_coor_y59->setText(QString::number(real_y[5],'f',3));
        ui->piece_coor_z159->setText(QString::number(real_z[5],'f',3));
        ui->piece_coor_z259->setText(QString::number(real_a[5],'f',3));
/*
        switch((int)par_get_var(5220)) {
        case 1:ui->g54_x->setValue(real_x[0]);ui->g54_y->setValue(real_y[0]);
            ui->g54_z->setValue(real_z[0]);ui->g54_a->setValue(real_a[0]);break;
        case 2:ui->g55_x->setValue(real_x[1]);ui->g55_y->setValue(real_y[1]);
            ui->g55_z->setValue(real_z[1]);ui->g55_a->setValue(real_a[1]);break;
        case 3:ui->g56_x->setValue(real_x[2]);ui->g56_y->setValue(real_y[2]);
            ui->g56_z->setValue(real_z[2]);ui->g56_a->setValue(real_a[2]);break;
        case 4:ui->g57_x->setValue(real_x[3]);ui->g57_y->setValue(real_y[3]);
            ui->g57_z->setValue(real_z[3]);ui->g57_a->setValue(real_a[3]);break;
        case 5:ui->g58_x->setValue(real_x[4]);ui->g58_y->setValue(real_y[4]);
            ui->g58_z->setValue(real_z[4]);ui->g58_a->setValue(real_a[4]);break;
        case 6:ui->g59_x->setValue(real_x[5]);ui->g59_y->setValue(real_y[5]);
            ui->g59_z->setValue(real_z[5]);ui->g59_a->setValue(real_a[5]);break;
        }
*/


        public_x_old = public_x;public_y_old = public_y;
        public_z1_old = public_z1;public_z2_old = public_z2;
        public_54x_old = show_x[0];public_54y_old = show_y[0];
        public_54z1_old = show_z[0];public_54z2_old = show_a[0];
        public_55x_old = show_x[1];public_55y_old = show_y[1];
        public_55z1_old = show_z[1];public_55z2_old = show_a[1];
        public_56x_old = show_x[2];public_56y_old = show_y[2];
        public_56z1_old = show_z[2];public_56z2_old = show_a[2];
        public_57x_old = show_x[3];public_57y_old = show_y[3];
        public_57z1_old = show_z[3];public_57z2_old = show_a[3];
        public_58x_old = show_x[4];public_58y_old = show_y[4];
        public_58z1_old = show_z[4];public_58z2_old = show_a[4];
        public_59x_old = show_x[5];public_59y_old = show_y[5];
        public_59z1_old = show_z[5];public_59z2_old = show_a[5];
    }

    ui->system_now->setText(_coor_system.value(emcStatus->task.g5x_index));
}

void multioffset::mdiExcuteRsult(bool ok)
{
    qDebug()<<"in ok mdi";
    if(ok) {
        qDebug()<<"in ok mdi";
        sendSetParameter(5101,public_x);
        sendSetParameter(5102,public_y);
        sendSetParameter(5103,public_z1);
        sendSetParameter(5104,public_z2);
        _prefer->writeEntry("PIECE_OFFSET/offset_x",QString::number(piece_x,'f',3));
        _prefer->writeEntry("PIECE_OFFSET/offset_y",QString::number(piece_y,'f',3));
        _prefer->writeEntry("PIECE_OFFSET/offset_z1",QString::number(piece_z1,'f',3));
        _prefer->writeEntry("PIECE_OFFSET/offset_z2",QString::number(piece_z2,'f',3));
        org_piece_x = piece_x;
        org_piece_y = piece_y;
        org_piece_z1 = piece_z1;
        org_piece_z2 = piece_z2;
        if(fromParent) {
            fromParent = false;
        }
          //QMessageBox::information(parentl,tr("information"),
        //tr("set offset complete"));
        else
            QMessageBox::information(this,tr("information"),
                                     tr("set offset complete"),tr("Ok"));

    } else {
        initValue();
        if(fromParent) {
            QMessageBox::warning(parentl,tr("warning"),
                                 tr("set offset fail!!"),tr("Ok"));
            fromParent = false;
        } else
            QMessageBox::warning(this,tr("warning"),
                                 tr("set offset fail!!"),tr("Ok"));
    }
}

void multioffset::mdiState(QString msg)
{

    if(mdimsg.size() >=1000)
        mdimsg.takeLast();
    mdimsg.prepend(msg);

    mdiRefresh();

}

void multioffset::mdiRefresh()
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

void multioffset::set_offset()
{

    static myLock locker;
    if(locker.isLock()) {
        return;
     }else
        locker.setLock(400);

    if(get_mdi_running())
        return;

        if(fabs(par_get_var(5221)-(ui->g54_x->value()+public_x))>0.001 ||
                fabs(par_get_var(5222)-(ui->g54_y->value()+public_y))>0.001||
                fabs(par_get_var(5223)-(ui->g54_z->value()+public_z1))>0.001 ||
                fabs(par_get_var(5224)-(ui->g54_a->value()+public_z2))>0.001)
            set_g54();
        if(par_get_var(4061) == 0)  // single coordinate system do not use other coordinate
            return;
        if(fabs(par_get_var(5241)-(ui->g55_x->value()+public_x))>0.001 ||
                fabs(par_get_var(5242)-(ui->g55_y->value()+public_y))>0.001||
                fabs(par_get_var(5243)-(ui->g55_z->value()+public_z1))>0.001 ||
                fabs(par_get_var(5244)-(ui->g55_a->value()+public_z2))>0.001)
            set_g55();
        if(fabs(par_get_var(5261) - (ui->g56_x->value()+public_x))>0.001 ||
                fabs(par_get_var(5262) - (ui->g56_y->value()+public_y))>0.001||
                fabs(par_get_var(5263) - (ui->g56_z->value()+public_z1))>0.001 ||
                fabs(par_get_var(5264) -(ui->g56_a->value()+public_z2))>0.001)
            set_g56();
        if(fabs(par_get_var(5281) - (ui->g57_x->value()+public_x))>0.001 ||
                fabs(par_get_var(5282)- (ui->g57_y->value()+public_y))>0.001||
                fabs(par_get_var(5283)-(ui->g57_z->value()+public_z1))>0.001 ||
                fabs(par_get_var(5284) - (ui->g57_a->value()+public_z2))>0.001)
            set_g57();
        if(fabs(par_get_var(5301) - (ui->g58_x->value()+public_x))>0.001 ||
                fabs(par_get_var(5302)- (ui->g58_y->value()+public_y))>0.001||
                fabs(par_get_var(5303) - (ui->g58_z->value()+public_z1))>0.001 ||
                fabs(par_get_var(5304)- (ui->g58_a->value()+public_z2))>0.001)
            set_g58();
        if(fabs(par_get_var(5321) - (ui->g59_x->value()+public_x))>0.001 ||
                fabs(par_get_var(5322) - (ui->g59_y->value()+public_y))>0.001||
                fabs(par_get_var(5323) - (ui->g59_z->value()+public_z1))>0.001 ||
                fabs(par_get_var(5324) - (ui->g59_a->value()+public_z2))>0.001)
            set_g59();
}

void multioffset::get_piece_offset_x(double val)
{
    piece_x = val;
}
void multioffset::get_piece_offset_y(double val)
{
    piece_y = val;
}
void multioffset::get_piece_offset_z1(double val)
{
    piece_z1 = val;
}
void multioffset::get_piece_offset_z2(double val)
{
    piece_z2 = val;
}
void multioffset::get_public_offset_x(double val)
{
    if(public_x != val) {
        public_x = val;
//        ui->public_offset_x->setValue(public_x);
    }

}
void multioffset::get_public_offset_y(double val)
{
    if(public_y != val) {
        public_y = val;
//        ui->public_offset_y->setValue(public_y);
    }
}
void multioffset::get_public_offset_z1(double val)
{
    if(public_z1 != val) {
        public_z1 = val;
//        ui->public_offset_z1->setValue(public_z1);
    }
}
void multioffset::get_public_offset_z2(double val)
{
    public_z2 = val;
}

void multioffset::set_deepen_001(){
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
void multioffset::set_deepen_01(){
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
void multioffset::set_deepen_1(){
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
void multioffset::set_raise_001(){
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
void multioffset::set_raise_01(){
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
void multioffset::set_raise_1(){
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

void multioffset::set_x()
{
        if(1 == par_get_var(5220)) {
            ui->g54_x->setValue(get_abs_pos(AXIS_X));
        } else if (2 == par_get_var(5220)) {
            ui->g55_x->setValue(get_abs_pos(AXIS_X));
        } else if (3 == par_get_var(5220)) {
            ui->g56_x->setValue(get_abs_pos(AXIS_X));
        } else if(4 == par_get_var(5220)) {
            ui->g57_x->setValue(get_abs_pos(AXIS_X));
        } else if(5 == par_get_var(5220)) {
            ui->g58_x->setValue(get_abs_pos(AXIS_X));
        } else if(6 == par_get_var(5220)) {
            ui->g59_x->setValue(get_abs_pos(AXIS_X));
        }
}

void multioffset::set_y()
{
    if(1 == par_get_var(5220)) {
        ui->g54_y->setValue(get_abs_pos(AXIS_Y));
    } else if (2 == par_get_var(5220)) {
        ui->g55_y->setValue(get_abs_pos(AXIS_Y));
    } else if (3 == par_get_var(5220)) {
        ui->g56_y->setValue(get_abs_pos(AXIS_Y));
    } else if(4 == par_get_var(5220)) {
        ui->g57_y->setValue(get_abs_pos(AXIS_Y));
    } else if(5 == par_get_var(5220)) {
        ui->g58_y->setValue(get_abs_pos(AXIS_Y));
    } else if(6 == par_get_var(5220)) {
        ui->g59_y->setValue(get_abs_pos(AXIS_Y));
    }
}

void multioffset::set_xy()
{

  //ui->piece_offset_x->setValue( get_abs_pos(AXIS_X));
  //ui->piece_offset_y->setValue( get_abs_pos(AXIS_Y));

}

void multioffset::set_record_x()
{
    QString temp = "";
    record_x = get_abs_pos(AXIS_X);
    temp = QString::number(record_x,'f',3);
    ui->show_record_x->setText(QString("%1").arg(record_x));
    _prefer->writeEntry("RECORD_VALUE/record_x",temp);
    ui->show_record_x->setStyleSheet("background-color: rgb(255, 255, 127);");
}

void multioffset::set_record_y()
{
    QString temp = "";
    record_y = get_abs_pos(AXIS_Y);
    temp = QString::number(record_y,'f',3);
    ui->show_record_y->setText(QString("%1").arg(record_y));
    _prefer->writeEntry("RECORD_VALUE/record_y",temp);
    ui->show_record_y->setStyleSheet("background-color: rgb(255, 255, 127);");
}

void multioffset::set_dichotomy_x()
{
    piece_x = (get_abs_pos(AXIS_X) + record_x)/2.0;
    if(1 == par_get_var(5220)) {
        ui->g54_x->setValue(piece_x);
    } else if (2 == par_get_var(5220)) {
        ui->g55_x->setValue(piece_x);
    } else if (3 == par_get_var(5220)) {
        ui->g56_x->setValue(piece_x);
    } else if(4 == par_get_var(5220)) {
        ui->g57_x->setValue(piece_x);
    } else if(5 == par_get_var(5220)) {
        ui->g58_x->setValue(piece_x);
    } else if(6 == par_get_var(5220)) {
        ui->g59_x->setValue(piece_x);
    }
    ui->show_record_x->setStyleSheet("background-color: rgb(0, 170, 127);");
}

void multioffset::set_dichotomy_y()
{
    piece_y = (get_abs_pos(AXIS_Y) + record_y)/2.0;
    if(1 == par_get_var(5220)) {
        ui->g54_y->setValue(piece_y);
    } else if (2 == par_get_var(5220)) {
        ui->g55_y->setValue(piece_y);
    } else if (3 == par_get_var(5220)) {
        ui->g56_y->setValue(piece_y);
    } else if(4 == par_get_var(5220)) {
        ui->g57_y->setValue(piece_y);
    } else if(5 == par_get_var(5220)) {
        ui->g58_y->setValue(piece_y);
    } else if(6 == par_get_var(5220)) {
        ui->g59_y->setValue(piece_y);
    }
    ui->show_record_y->setStyleSheet("background-color: rgb(0, 170, 127);");
}

void multioffset::updatemianshow() {
//    if(1 == par_get_var(5220)) {
        show_x[0] = ui->g54_x->value();
        show_y[0] = ui->g54_y->value();
        show_z[0] = ui->g54_z->value();
        show_a[0] = ui->g54_a->value();
//    } else if (2 == par_get_var(5220)) {
        show_x[1] = ui->g55_x->value();
        show_y[1] = ui->g55_y->value();
        show_z[1] = ui->g55_z->value();
        show_a[1] = ui->g55_a->value();
//    } else if (3 == par_get_var(5220)) {
        show_x[2] = ui->g56_x->value();
        show_y[2] = ui->g56_y->value();
        show_z[2] = ui->g56_z->value();
        show_a[2] = ui->g56_a->value();
//    } else if(4 == par_get_var(5220)) {
        show_x[3] = ui->g57_x->value();
        show_y[3] = ui->g57_y->value();
        show_z[3] = ui->g57_z->value();
        show_a[3] = ui->g57_a->value();
//    } else if(5 == par_get_var(5220)) {
        show_x[4] = ui->g58_x->value();
        show_y[4] = ui->g58_y->value();
        show_z[4] = ui->g58_z->value();
        show_a[4] = ui->g58_a->value();
//    } else if(6 == par_get_var(5220)) {
        show_x[5] = ui->g59_x->value();
        show_y[5] = ui->g59_y->value();
        show_z[5] = ui->g59_z->value();
        show_a[5] = ui->g59_a->value();
//    }
    piece_x = show_x[origin];
    piece_y = show_y[origin];
    piece_z1 = show_z[origin];
    piece_z2 = show_a[origin];
}

void multioffset::set_g54() {
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G10L2P1").append("X").append(QString::number(public_x+ui->g54_x->value(),'f',3))
            .append("Y").append(QString::number(public_y+ui->g54_y->value(),'f',3))
            .append("Z").append(QString::number(public_z1+ui->g54_z->value(),'f',3))
            .append("A").append(QString::number(public_z2+ui->g54_a->value(),'f',3));
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);

    emit send_msg(1,QString(tr("Now Set G54 :X %1 Y: %2 Z %3")).arg(public_x+ui->g54_x->value()).arg(public_y+ui->g54_y->value())
                  .arg(public_z1+ui->g54_z->value()));
}
void multioffset::set_g55() {
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G10L2P2").append("X").append(QString::number(public_x+ui->g55_x->value(),'f',3))
            .append("Y").append(QString::number(public_y+ui->g55_y->value(),'f',3))
            .append("Z").append(QString::number(public_z1+ui->g55_z->value(),'f',3))
                                .append("A").append(QString::number(public_z2+ui->g55_a->value(),'f',3));
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);
    emit send_msg(1,QString(tr("Now Set G55 :X %1 Y: %2 Z %3")).arg(public_x+ui->g55_x->value()).arg(public_y+ui->g55_y->value())
                  .arg(public_z1+ui->g55_z->value()));
}
void multioffset::set_g56() {
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G10L2P3").append("X").append(QString::number(public_x+ui->g56_x->value(),'f',3))
            .append("Y").append(QString::number(public_y+ui->g56_y->value(),'f',3))
            .append("Z").append(QString::number(public_z1+ui->g56_z->value(),'f',3))
                                .append("A").append(QString::number(public_z2+ui->g56_a->value(),'f',3));
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);
    emit send_msg(1,QString(tr("Now Set G56 :X %1 Y: %2 Z %3")).arg(public_x+ui->g56_x->value()).arg(public_y+ui->g56_y->value())
                  .arg(public_z1+ui->g56_z->value()));
}
void multioffset::set_g57() {
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G10L2P4").append("X").append(QString::number(public_x+ui->g57_x->value(),'f',3))
            .append("Y").append(QString::number(public_y+ui->g57_y->value(),'f',3))
            .append("Z").append(QString::number(public_z1+ui->g57_z->value(),'f',3))
                                .append("A").append(QString::number(public_z2+ui->g57_a->value(),'f',3));
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);
    emit send_msg(1,QString(tr("Now Set G57 :X %1 Y: %2 Z %3")).arg(public_x+ui->g57_x->value()).arg(public_y+ui->g57_y->value())
                  .arg(public_z1+ui->g57_z->value()));
}
void multioffset::set_g58() {
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G10L2P5").append("X").append(QString::number(public_x+ui->g58_x->value(),'f',3))
            .append("Y").append(QString::number(public_y+ui->g58_y->value(),'f',3))
            .append("Z").append(QString::number(public_z1+ui->g58_z->value(),'f',3))
                                .append("A").append(QString::number(public_z2+ui->g58_a->value(),'f',3));
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);
    emit send_msg(1,QString(tr("Now Set G58 :X %1 Y: %2 Z %3")).arg(public_x+ui->g58_x->value()).arg(public_y+ui->g58_y->value())
                  .arg(public_z1+ui->g58_z->value()));
}
void multioffset::set_g59() {
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G10L2P6").append("X").append(QString::number(public_x+ui->g59_x->value(),'f',3))
            .append("Y").append(QString::number(public_y+ui->g59_y->value(),'f',3))
            .append("Z").append(QString::number(public_z1+ui->g59_z->value(),'f',3))
                                .append("A").append(QString::number(public_z2+ui->g59_a->value(),'f',3));
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);
    emit send_msg(1,QString(tr("Now Set G59 :X %1 Y: %2 Z %3")).arg(public_x+ui->g59_x->value()).arg(public_y+ui->g59_y->value())
                  .arg(public_z1+ui->g59_z->value()));
}

void multioffset::set_index_g54(bool flag) {
    if(!flag) {ui->group54->setStyleSheet(""); return;}
    origin = 1;
    fromParent = true;
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G54");
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);
    //initValue();
    fromParent = true;
    ui->group54->setStyleSheet("background-color: rgb(241, 255, 194);border-color: rgb(241, 255, 194);");

    if(par_get_var(4290) == 1) {
        ui->g54_z->setValue(par_get_var(4801)+par_get_var(4303+5*(int)par_get_var(4288))-public_z1);
        ui->g54_a->setValue(par_get_var(4811)+par_get_var(4703+5*(int)par_get_var(4289))-public_z2);
    }

}
void multioffset::set_index_g55(bool flag) {
    if(!flag)  {ui->group55->setStyleSheet(""); return;}
    origin = 2;
    fromParent = true;
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G55");
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);
    ui->group55->setStyleSheet("background-color: rgb(241, 255, 194);border-color: rgb(241, 255, 194);");
    //initValue();
    fromParent = true;

    if(par_get_var(4290) == 1) {
        ui->g55_z->setValue(par_get_var(4802)+par_get_var(4303+5*(int)par_get_var(4288))-public_z1);
        ui->g55_a->setValue(par_get_var(4812)+par_get_var(4703+5*(int)par_get_var(4289))-public_z2);
    }
}
void multioffset::set_index_g56(bool flag) {
    if(!flag)  {ui->group56->setStyleSheet(""); return;}
    origin = 3;
    fromParent = true;
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G56");
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);
    //initValue();
    fromParent = true;
    ui->group56->setStyleSheet("background-color: rgb(241, 255, 194);border-color: rgb(241, 255, 194);");

    if(par_get_var(4290) == 1) {
        ui->g56_z->setValue(par_get_var(4803)+par_get_var(4303+5*(int)par_get_var(4288))-public_z1);
        ui->g56_a->setValue(par_get_var(4813)+par_get_var(4703+5*(int)par_get_var(4289))-public_z2);
    }
}
void multioffset::set_index_g57(bool flag) {
    if(!flag)  {ui->group57->setStyleSheet(""); return;}
    origin = 4;
    fromParent = true;
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G57");
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);

    //initValue();
    fromParent = true;
    ui->group57->setStyleSheet("background-color: rgb(241, 255, 194);border-color: rgb(241, 255, 194);");

    if(par_get_var(4290) == 1) {
        ui->g57_z->setValue(par_get_var(4804)+par_get_var(4303+5*(int)par_get_var(4288))-public_z1);
        ui->g57_a->setValue(par_get_var(4814)+par_get_var(4703+5*(int)par_get_var(4289))-public_z2);
    }
}
void multioffset::set_index_g58(bool flag) {
    if(!flag)  {ui->group58->setStyleSheet(""); return;}
    origin = 5;
    fromParent = true;
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G58");
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);

    //initValue();
    fromParent = true;
    ui->group58->setStyleSheet("background-color: rgb(241, 255, 194);border-color: rgb(241, 255, 194);");

    if(par_get_var(4290) == 1) {
        ui->g58_z->setValue(par_get_var(4805)+par_get_var(4303+5*(int)par_get_var(4288))-public_z1);
        ui->g58_a->setValue(par_get_var(4815)+par_get_var(4703+5*(int)par_get_var(4289))-public_z2);
    }
}
void multioffset::set_index_g59(bool flag) {
    if(!flag)  {ui->group59->setStyleSheet(""); return;}
    origin = 6;
    fromParent = true;
    QString cmd_coor;
    mdiserials.clear();
    cmd_coor.append("G59");
    sendMdi();
    int retSeri = -1;
    sendMdiCmd(cmd_coor.toLatin1().data(),&retSeri,1);
    mdiserials.append(retSeri);


   // initValue();
    fromParent = true;
    ui->group59->setStyleSheet("background-color: rgb(241, 255, 194);border-color: rgb(241, 255, 194);");

    if(par_get_var(4290) == 1) {
        ui->g59_z->setValue(par_get_var(4806)+par_get_var(4303+5*(int)par_get_var(4288))-public_z1);
        ui->g59_a->setValue(par_get_var(4816)+par_get_var(4703+5*(int)par_get_var(4289))-public_z2);
    }
}


multioffset::~multioffset()
{
    delete ui;
}
