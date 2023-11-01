#include "homedialog.h"
#include "ui_homedialog.h"
#include "QTimer"
#include "emc.hh"
#include "emc_nml.hh"
#include <QtDebug>
#include "shcom.hh"
#include <QFileDialog>
#include <inifile.hh>
#include "extern.h"
#include "qshortcut.h"
#include "../../config.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QPropertyAnimation>
#include <QDesktopWidget>

HomeDialog::HomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeDialog)
{
    ui->setupUi(this);
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    starthomedirectly = false;
    start_home_all = false;
    showy2 = _ini->read("PAR_OPERATOR_SHOWY2" ,"VALUE","0").toInt();
    sendManual();
    this->setWindowTitle(tr("HomeDialog"));

    connect(ui->btn_home_all,SIGNAL(clicked()),this,SLOT(home_all()));
    connect(ui->btn_home_x,SIGNAL(clicked()),this,SLOT(home_x()));
    connect(ui->btn_home_y,SIGNAL(clicked()),this,SLOT(home_y()));
    connect(ui->btn_home_y_2,SIGNAL(clicked()),this,SLOT(home_y_2()));
    connect(ui->btn_home_z,SIGNAL(clicked()),this,SLOT(home_z()));
    connect(ui->btn_home_a,SIGNAL(clicked()),this,SLOT(home_a()));
    connect(ui->btn_home_b,SIGNAL(clicked()),this,SLOT(home_b()));
    connect(ui->btn_home_c,SIGNAL(clicked()),this,SLOT(home_c()));
 //   connect(ui->btn_abort,SIGNAL(clicked()),this,SLOT(home_quit()));
    connect(ui->btn_return,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->btn_directset,SIGNAL(clicked()),this,SLOT(home_direct()));

    connect(ui->btn_unhome_x,SIGNAL(clicked()),this,SLOT(unhome_x()));
    connect(ui->btn_unhome_y,SIGNAL(clicked()),this,SLOT(unhome_y()));
    connect(ui->btn_unhome_z,SIGNAL(clicked()),this,SLOT(unhome_z()));
    connect(ui->btn_unhome_a,SIGNAL(clicked()),this,SLOT(unhome_a()));
    connect(ui->btn_unhome_b,SIGNAL(clicked()),this,SLOT(unhome_b()));
    connect(ui->btn_unhome_c,SIGNAL(clicked()),this,SLOT(unhome_c()));

    flush_time = new QTimer(this);
    connect(flush_time,SIGNAL(timeout()),this,SLOT(home_update()));
    flush_time->start(100);



    if(_doublez < 6) {
        ui->btn_home_c->setHidden(true);
        ui->btn_unhome_c->setHidden(true);
    }
//    if(QApplication::desktop()->screenGeometry().height() < 850) {
        this->setMaximumSize(300,300);
    //    this->setMinimumSize(450,400);
  //  } else {
     //   this->setMinimumSize(600,440);
 //   }

//    ui->btn_directset->setHidden(true);
  //  ui->btn_home_all->setHidden(true);
//    ui->btn_abort->setHidden(true);

    ui->label_3->hide();
    ui->label_4->hide();
    ui->widget->hide();
    ui->label->setText(tr("AllHome"));
    ui->label_5->setText(tr("SingleHome"));
    ui->btn_home_x->setText("X");
    ui->btn_home_y->setText("Y");
    ui->btn_home_z->setText("Z");
    ui->btn_home_a->setText("A");
    ui->btn_home_b->setText("B");
    ui->btn_home_c->setText("C");
    if(par_get_var(3999)==1){
      ui->btn_home_b->hide();
      ui->btn_home_c->hide();
    }else if(par_get_var(3999)==4 || par_get_var(3999)==5 || par_get_var(3999)==7){
      ui->btn_home_a->hide();
      ui->btn_home_b->setHidden(par_get_var(3999)!=5);
      ui->btn_home_c->hide();
    }else if(par_get_var(3999)==6){
        ui->btn_home_z->hide();
        ui->btn_home_b->hide();
        ui->btn_home_c->hide();
    }

    if(SIMULATE == 1)
        home_init();

    if(par_get_var(3120) > 0)
        ui->btn_home_y_2->hide();

    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    QDesktopWidget *deskt = QApplication::desktop();
    this->move(deskt->width()/2 - this->width()/2,deskt->height()/2-this->height()/2);
}


void HomeDialog::home_init(){
    QString path = "/home/.config/zero_pos";
    QString pos = " ";
    int axis_num = 0;


    QFile file(path);

    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        while( !stream.atEnd()){
            pos = stream.readLine();
//            if(axis_num < 6 && fabs(pos.toDouble()) < 0.00001)
//                sendUnHome(axis_num,1);

            axis_num++;
        }
    }
    file.close();

}

void HomeDialog::unhome_x(){
    sendUnHome(0,1);
}

void HomeDialog::unhome_y(){
    sendUnHome(1,1);
}

void HomeDialog::unhome_z(){
    sendUnHome(2,1);
}

void HomeDialog::unhome_a(){
    sendUnHome(3,1);
}

void HomeDialog::unhome_b(){
    sendUnHome(4,1);
}

void HomeDialog::unhome_c(){
    sendUnHome(5,1);
}

void HomeDialog::home_update()
{

    static bool old_home_flagx = !emcStatus->motion.axis[0].homed;
    if(old_home_flagx != emcStatus->motion.axis[0].homed
            && emcStatus->motion.axis[0].homed && par_get_var(4070) == 0){
        emit(send_msg(0,tr("homeX posfb %1").arg(emcStatus->motion.switch_posfb[0])));//ghf-2014/6/24
        if(fabs(emcStatus->motion.switch_posfb[0]) < 0.05 &&
                _ini->read("PAR_AXIS_0_HOME_USE_INDEX" ,"VALUE","0").toInt() > 0 &&
                !starthomedirectly) {
            sendUnHome(0);
            send_msg(1,tr("axis X Cannot find the coder index ,please check the signal"));
        }
    }
    old_home_flagx = emcStatus->motion.axis[0].homed;

    static bool old_home_flagy = !emcStatus->motion.axis[1].homed;
    if(old_home_flagy != emcStatus->motion.axis[1].homed
            && emcStatus->motion.axis[1].homed && par_get_var(4070) == 0){
        emit(send_msg(0,tr("homeY posfb %1").arg(emcStatus->motion.switch_posfb[1])));
        if(fabs(emcStatus->motion.switch_posfb[1]) < 0.05 &&
                _ini->read("PAR_AXIS_1_HOME_USE_INDEX" ,"VALUE","0").toInt() > 0 &&
                !starthomedirectly) {
            sendUnHome(1);
            send_msg(1,tr("axis Y Cannot find the coder index ,please check the signal"));
        }
    }
    old_home_flagy = emcStatus->motion.axis[1].homed;


    static bool old_home_flagz1 = !emcStatus->motion.axis[2].homed;
    if(old_home_flagz1 != emcStatus->motion.axis[2].homed
            && emcStatus->motion.axis[2].homed && par_get_var(4070) == 0){
        emit(send_msg(0,tr("homeZ1 posfb %1").arg(emcStatus->motion.switch_posfb[2])));
        if(fabs(emcStatus->motion.switch_posfb[2]) < 0.05 &&
                _ini->read("PAR_AXIS_2_HOME_USE_INDEX" ,"VALUE","0").toInt() > 0 &&
                !starthomedirectly) {
            sendUnHome(2);
            send_msg(1,tr("axis Z1 Cannot find the coder index ,please check the signal"));
        }
    }
    old_home_flagz1 = emcStatus->motion.axis[2].homed;



    /*
    static bool old_home_flagz2 = !emcStatus->motion.axis[3].homed;
    if(old_home_flagz2 != emcStatus->motion.axis[3].homed
            && emcStatus->motion.axis[3].homed && par_get_var(4070) == 0){
        emit(send_msg(0,tr("homeZ2 posfb %1").arg(emcStatus->motion.switch_posfb[3])));
        if(fabs(emcStatus->motion.switch_posfb[3]) < 0.05 &&
                _ini->read("PAR_AXIS_3_HOME_USE_INDEX" ,"VALUE","0").toInt() > 0 &&
                !starthomedirectly) {
            sendUnHome(3);
            send_msg(1,tr("axis Z2 Cannot find the coder index ,please check the signal"));
        }
    }
    old_home_flagz2 = emcStatus->motion.axis[3].homed;



    static bool old_home_flagz3 = !emcStatus->motion.axis[4].homed;
    if(old_home_flagz3 != emcStatus->motion.axis[4].homed
            && emcStatus->motion.axis[4].homed&& par_get_var(4070) == 0){
        emit(send_msg(0,tr("homeZ3 posfb %1").arg(emcStatus->motion.switch_posfb[4])));
    }
    old_home_flagz3 = emcStatus->motion.axis[4].homed;

    static bool old_home_flagz4 = !emcStatus->motion.axis[5].homed;
    if(old_home_flagz4 != emcStatus->motion.axis[5].homed
            && emcStatus->motion.axis[5].homed && _doublez > 5 && par_get_var(4070) == 0){
        emit(send_msg(0,tr("homeZ4 posfb %1").arg(emcStatus->motion.switch_posfb[5])));
    }
    old_home_flagz4 = emcStatus->motion.axis[5].homed;

    */

    if(_doublez > 6) {

        static bool old_home_flagy2 = !emcStatus->motion.axis[6].homed;
        if(old_home_flagy2 != emcStatus->motion.axis[6].homed
                && emcStatus->motion.axis[6].homed && par_get_var(4070) == 0){
            emit(send_msg(0,tr("homeY2 posfb %1").arg(emcStatus->motion.switch_posfb[6])));
            if(fabs(emcStatus->motion.switch_posfb[6]) < 0.05 &&
                    _ini->read("PAR_AXIS_6_HOME_USE_INDEX" ,"VALUE","0").toInt() > 0 &&
                    !starthomedirectly) {
                sendUnHome(6);
                send_msg(1,tr("axis Y Cannot find the coder index ,please check the signal"));
            }
        }
        old_home_flagy2 = emcStatus->motion.axis[6].homed;

    }



    if(emcStatus->motion.axis[0].homing ||
            emcStatus->motion.axis[1].homing ||
            emcStatus->motion.axis[2].homing ||
            emcStatus->motion.axis[3].homing ||
            emcStatus->motion.axis[4].homing ||
            (emcStatus->motion.axis[5].homing && _doublez > 5)||
            (emcStatus->motion.axis[6].homing && _doublez > 6)) {
        this->homing = true;
    } else {
        this->homing = false;
    }

    if(emcStatus->motion.axis[0].homed &&
            emcStatus->motion.axis[1].homed &&
            emcStatus->motion.axis[2].homed &&
            emcStatus->motion.axis[3].homed &&
            emcStatus->motion.axis[4].homed &&  (_doublez <= 5 ||
            (emcStatus->motion.axis[5].homed && _doublez > 5))) {
        this->homed = true;
    } else {
        this->homed = false;
    }

    if(homed ==true && start_home_all) {
        start_home_all = false;
        this->close();
    }

}

void HomeDialog::reject()
{
    this->close();
}

void HomeDialog::closeEvent(QCloseEvent *evt)
{
    evt->accept();
}

void HomeDialog::showEvent(QShowEvent *ev)
{
    homedirectly = _ini->read("PAR_OPERATOR_DIRECTLYHOME" ,"VALUE","0").toInt();
    if(!homedirectly) {
        ui->btn_directset->setEnabled(false);
    } else {
        ui->btn_directset->setEnabled(true);
    }
    ev->ignore();
}

HomeDialog::~HomeDialog()
{
    delete flush_time;
    delete ui;
}

void HomeDialog::home_all()
{
    //if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
    //    return;
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();

    sendUnHome(-1);
    sendHome(-1,1);
    start_home_all = true;
    starthomedirectly = false;

    qDebug("now start to HOMEALL !!!!!!!!!!!!");

}

void HomeDialog::home_x()
{
    if((0 == emcStatus->motion.axis[2].homed ||
            0 == emcStatus->motion.axis[3].homed ||
            0 == emcStatus->motion.axis[4].homed ||
            (_doublez > 5 && 0 == emcStatus->motion.axis[5].homed))
            && par_get_var(4070) == 0 ) {
        int ret=QMessageBox::information(this,tr("warn"),
                                         tr("AXIS Z is unhomed,ensure to continue?"),
                                         QMessageBox::Ok,QMessageBox::Cancel);
        if(ret==QMessageBox::Ok){
        }else if(QMessageBox::Cancel){
            return;
        }
    }
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();
    sendUnHome(0);
    sendHome(0,1);
    starthomedirectly = false;
    if(par_get_var(4070) == 0)
        emit(send_msg(0,tr("Home axis X")));
    else
        emit(send_msg(0,tr("Set current coordination as X zero pos")));

}

void HomeDialog::home_y()
{
    if((0 == emcStatus->motion.axis[2].homed ||
            0 == emcStatus->motion.axis[3].homed ||
            0 == emcStatus->motion.axis[4].homed ||
            (_doublez > 5 && 0 == emcStatus->motion.axis[5].homed))
            && par_get_var(4070) == 0 ) {

        int ret=QMessageBox::information(this,tr("warn"),
                                         tr("AXIS Z is unhomed,ensure to continue?"),
                                         QMessageBox::Ok,QMessageBox::Cancel);
        if(ret==QMessageBox::Ok){
        }else if(QMessageBox::Cancel){
            return;
        }
    }
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();

    sendUnHome(1);
    sendHome(1,1);

    starthomedirectly = false;
    if(par_get_var(4070) == 0)
        emit(send_msg(0,tr("Home axis Y")));
    else
        emit(send_msg(0,tr("Set current coordination as Y zero pos")));
}


void HomeDialog::home_y_2()
{
    if((0 == emcStatus->motion.axis[2].homed ||
            0 == emcStatus->motion.axis[3].homed ||
            0 == emcStatus->motion.axis[4].homed ||
            (_doublez > 5 && 0 == emcStatus->motion.axis[5].homed))
            && par_get_var(4070) == 0 ) {

        int ret=QMessageBox::information(this,tr("warn"),
                                         tr("AXIS Z is unhomed,ensure to continue?"),
                                         QMessageBox::Ok,QMessageBox::Cancel);
        if(ret==QMessageBox::Ok){
        }else if(QMessageBox::Cancel){
            return;
        }
    }
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();


    sendUnHome(6);
    sendHome(6,1);

    starthomedirectly = false;
    if(par_get_var(4070) == 0)
        emit(send_msg(0,tr("Home axis Y2")));
    else
        emit(send_msg(0,tr("Set current coordination as Y zero pos")));
}


void HomeDialog::home_z()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();

    sendUnHome(2);
    sendHome(2,1);

    sendUnHome(3);
    sendHome(3,1);

    sendUnHome(4);
    sendHome(4,1);

    sendUnHome(5);
    sendHome(5,1);

//    if(_doublez) {
//        sendUnHome(2);
//        sendHome(2,1);
//    } else {
//        sendHome(2,1);
//        sendHome(3,1);
//    }
    starthomedirectly = false;
    if(par_get_var(4070) == 0)
        emit(send_msg(0,tr("Home axis Z")));
    else
        emit(send_msg(0,tr("Set current coordination as Z zero pos")));
}

void HomeDialog::home_a()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();
    sendUnHome(3);
    sendHome(3,1);
    starthomedirectly = false;
    if(par_get_var(4070) == 0)
        emit(send_msg(0,tr("Home axis Z2")));
    else
        emit(send_msg(0,tr("Set current coordination as Z2 zero pos")));
}

void HomeDialog::home_b()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();
    sendUnHome(4);
    sendHome(4,1);
    if(par_get_var(4070) == 0)
        emit(send_msg(0,tr("B轴回零")));
    else
        emit(send_msg(0,tr("设置B轴零点")));
}

void HomeDialog::home_c()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();
    sendUnHome(5);
    sendHome(5,1);
    if(par_get_var(4070) == 0)
        emit(send_msg(0,tr("Home axis Z4")));
    else
        emit(send_msg(0,tr("Set current coordination as Z4 zero pos")));
}
void HomeDialog::home_direct()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();

    sendHome(-1,0);
    start_home_all = true;
    starthomedirectly = true;
}


void HomeDialog::home_quit()
{
    sendAbort();
}

/*
void HomeDialog::on_btn_home_u_clicked()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();
    sendUnHome(6);
    sendHome(6,1);
    emit(send_msg(0,tr("Home axis Y2")));
}
*/

void HomeDialog::on_btn_stop_pressed()
{
    sendAbort();
}
