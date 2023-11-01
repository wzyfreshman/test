#include "set_feed.h"
#include "ui_set_feed.h"

#include "emcstatus.hh"
#include "ini_settings.h"
#include "extern.h"
#include "shcom.hh"

#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

set_feed::set_feed(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::set_feed)
{
    ui->setupUi(this);
    this->setStyleSheet("font: 12pt Sans Serif");

    QVBoxLayout *lay_set_z = new QVBoxLayout(ui->widget_c);
    for( int i=0;i<6;i++) {

        QFrame *item = new QFrame();
        set_c_z.append(item);
        lay_set_z->addWidget(item);
        lay_set_z->setContentsMargins(0,0,0,0);
        set_c_z[i]->setStyleSheet("background-color: rgb(255, 255, 255)");

    }
    QVBoxLayout *lay_set_a = new QVBoxLayout(ui->widget_a);
    for( int i=0;i<6;i++) {

        QFrame *item = new QFrame();
        set_a_z.append(item);
        lay_set_a->addWidget(item);
        lay_set_a->setContentsMargins(0,0,0,0);
        set_a_z[i]->setStyleSheet("background-color: rgb(255, 255, 255)");

    }
    QVBoxLayout *lay_set_b = new QVBoxLayout(ui->widget_b);
    for( int i=0;i<6;i++) {

        QFrame *item = new QFrame();
        set_b_z.append(item);
        lay_set_b->addWidget(item);
        lay_set_b->setContentsMargins(0,0,0,0);
        set_b_z[i]->setStyleSheet("background-color: rgb(255, 255, 255)");

    }

    for( int i=0;i<6;i++) {
        lab_set_c_feed[i] = new MyLabel(0);
        lab_set_c_feed[i]->setAxis((AXIS_NAME)i);
        QHBoxLayout *lab_set_c_feed_lay = new QHBoxLayout(set_c_z[i]);
        lab_set_c_feed_lay->addWidget(lab_set_c_feed[i]);
        lab_set_c_feed_lay->setContentsMargins(0,0,0,0);
        lab_set_c_feed[i]->setText(QString::number(par_get_var(3650+i),'f',2));
        lab_set_c_feed[i]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        connect(lab_set_c_feed[i],SIGNAL(leftClick(int)),this,SLOT(set_c_feed(int)));
    }

    for( int i=0;i<6;i++) {
        lab_set_a_feed[i] = new MyLabel(0);
        lab_set_a_feed[i]->setAxis((AXIS_NAME)i);
        QHBoxLayout *lab_set_a_feed_lay = new QHBoxLayout(set_a_z[i]);
        lab_set_a_feed_lay->addWidget(lab_set_a_feed[i]);
        lab_set_a_feed_lay->setContentsMargins(0,0,0,0);
        lab_set_a_feed[i]->setText(QString::number(par_get_var(3630+i),'f',2));
        lab_set_a_feed[i]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        connect(lab_set_a_feed[i],SIGNAL(leftClick(int)),this,SLOT(set_a_feed(int)));
    }

    for( int i=0;i<6;i++) {
        lab_set_b_feed[i] = new MyLabel(0);
        lab_set_b_feed[i]->setAxis((AXIS_NAME)i);
        QHBoxLayout *lab_set_b_feed_lay = new QHBoxLayout(set_b_z[i]);
        lab_set_b_feed_lay->addWidget(lab_set_b_feed[i]);
        lab_set_b_feed_lay->setContentsMargins(0,0,0,0);
        lab_set_b_feed[i]->setText(QString::number(par_get_var(3640+i),'f',2));
        lab_set_b_feed[i]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        connect(lab_set_b_feed[i],SIGNAL(leftClick(int)),this,SLOT(set_b_feed(int)));
    }


    lab_set_fast_distance_a = new MyLabel(0);
    QHBoxLayout *lab_set_fast_distance_a_lay = new QHBoxLayout(ui->set_fast_dis_a);
    lab_set_fast_distance_a_lay->addWidget(lab_set_fast_distance_a);
    lab_set_fast_distance_a_lay->setContentsMargins(0,0,0,0);
    lab_set_fast_distance_a->setText(QString::number(par_get_var(3608),'f',2));
    lab_set_fast_distance_a->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_set_fast_distance_a,SIGNAL(clicked()),this,SLOT(set_a_fast()));

    lab_set_fast_distance_b = new MyLabel(0);
    QHBoxLayout *lab_set_fast_distance_b_lay = new QHBoxLayout(ui->set_fast_dis_b);
    lab_set_fast_distance_b_lay->addWidget(lab_set_fast_distance_b);
    lab_set_fast_distance_b_lay->setContentsMargins(0,0,0,0);
    lab_set_fast_distance_b->setText(QString::number(par_get_var(3609),'f',2));
    lab_set_fast_distance_b->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_set_fast_distance_b,SIGNAL(clicked()),this,SLOT(set_b_fast()));

    lab_set_fast_distance_c = new MyLabel(0);
    QHBoxLayout *lab_set_fast_distance_c_lay = new QHBoxLayout(ui->set_fast_dis_c);
    lab_set_fast_distance_c_lay->addWidget(lab_set_fast_distance_c);
    lab_set_fast_distance_c_lay->setContentsMargins(0,0,0,0);
    lab_set_fast_distance_c->setText(QString::number(par_get_var(3610),'f',2));
    lab_set_fast_distance_c->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_set_fast_distance_c,SIGNAL(clicked()),this,SLOT(set_c_fast()));


    if(par_get_var(3624)==0.00)
        sendSetParameter(3624,5.00);

    lab_turn_delay = new MyLabel(0);
    QHBoxLayout *lab_turn_delay_lay = new QHBoxLayout(ui->set_delay);
    lab_turn_delay_lay->addWidget(lab_turn_delay);
    lab_turn_delay_lay->setContentsMargins(0,0,0,0);
    lab_turn_delay->setText(QString::number(par_get_var(3624),'f',2));
    lab_turn_delay->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    connect(lab_turn_delay,SIGNAL(clicked()),this,SLOT(set_turn_delay()));


    if(par_get_var(3999)!=5)
        ui->widget_delay->hide();


}

set_feed::~set_feed()
{
    delete ui;
}

void set_feed::on_btn_close_clicked()
{
    this->close();
}

void set_feed::set_a_feed(int id)
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3630+id),(QVariant)-10000,(QVariant)0,tr("Set A(-90) Zfeed"),tr("Set A(-90) Zfeed"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= -10000 && val <= 0){
        show_msg(1,tr("Set A(-90) Zfeed for the time %1 :%2->%3").arg(id+1).arg(par_get_var(3630+id)).arg(val));
        lab_set_a_feed[id]->setText(QString::number(val,'f',2));
        sendSetParameter(3630+id,val);
    }
}

void set_feed::set_b_feed(int id)
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3640+id),(QVariant)-10000,(QVariant)0,tr("Set B(0) Zfeed"),tr("Set B(0) Zfeed"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= -10000 && val <= 0){
        show_msg(1,tr("Set B(0) Zfeed for the time %1 :%2->%3").arg(id+1).arg(par_get_var(3640+id)).arg(val));
        lab_set_b_feed[id]->setText(QString::number(val,'f',2));
        sendSetParameter(3640+id,val);
    }
}

void set_feed::set_c_feed(int id)
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3650+id),(QVariant)-10000,(QVariant)0,tr("Set C(90) Zfeed"),tr("Set C(90) Zfeed"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= -10000 && val <= 0){
        show_msg(1,tr("Set C(90) Zfeed for the time %1 :%2->%3").arg(id+1).arg(par_get_var(3650+id)).arg(val));
        lab_set_c_feed[id]->setText(QString::number(val,'f',2));
        sendSetParameter(3650+id,val);
    }
}

void set_feed::set_a_fast()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3608),(QVariant)0,(QVariant)1000,tr("Set fast safe"),tr("Set fast safe"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= 0 && val <= 1000){
        show_msg(1,tr("Set A(-90) fast safe distance :%1->%2").arg(par_get_var(3608)).arg(val));
        lab_set_fast_distance_a->setText(QString::number(val,'f',2));
        sendSetParameter(3608,val);
    }
}

void set_feed::set_b_fast()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3609),(QVariant)0,(QVariant)1000,tr("Set fast safe"),tr("Set fast safe"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= 0 && val <= 1000){
        show_msg(1,tr("Set B(0) fast safe distance :%1->%2").arg(par_get_var(3609)).arg(val));
        lab_set_fast_distance_b->setText(QString::number(val,'f',2));
        sendSetParameter(3609,val);
    }
}

void set_feed::set_c_fast()
{
    if(get_pro_running())
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3610),(QVariant)0,(QVariant)1000,tr("Set fast safe"),tr("Set fast safe"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= 0 && val <= 1000){
        show_msg(1,tr("Set C(90) fast safe distance :%1->%2").arg(par_get_var(3610)).arg(val));
        lab_set_fast_distance_c->setText(QString::number(val,'f',2));
        sendSetParameter(3610,val);
    }
}

void set_feed::set_turn_delay()
{
    if(get_pro_running())
        return;

    if(!PassWord::getCodecnc(this,1))
        return;

    double val = 0;
    MyInput input(2,(QVariant)par_get_var(3624),(QVariant)0,(QVariant)100,tr("翻转延时"),tr("翻转延时"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= 0 && val <= 100){
        show_msg(1,tr("翻转延时 :%1->%2").arg(par_get_var(3624)).arg(val));
        lab_turn_delay->setText(QString::number(val,'f',2));
        sendSetParameter(3624,val);
    }
}


