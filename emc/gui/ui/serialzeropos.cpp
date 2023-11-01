#include "serialzeropos.h"
#include "ui_serialzeropos.h"
#include "extern.h"
#include "shcom.hh"
#include "unit_serialport.h"

#include <QDesktopWidget>
#include <QtDebug>
SerialzeroPos::SerialzeroPos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialzeroPos)
{
    ui->setupUi(this);
    initSerialpos();

    QDesktopWidget *deskt = QApplication::desktop();
    this->move(deskt->width()/2 - this->width()/2,deskt->height()/2-this->height()/2);
}

SerialzeroPos::~SerialzeroPos()
{
    delete ui;
}

//union data2floatx{
//    uint8 ch[4];
//    uint32 intvalue;
//    float value;
//};

void SerialzeroPos::initSerialpos()
{
    for(int i=0;i<AXIS_NUM;i++) {
        recordpos[i] = 0;
    }

//    data2floatx dtf;
//    dtf.ch[1] = 0x64;
//    dtf.ch[0] = 0x54;
//    dtf.ch[3] = 0x00;
//    dtf.ch[2] = 0x01;

//    printf("data=========================================== %d\n",dtf.intvalue);

    QString path = "/home/.config/serial_zero_pos";
    QString pos = " ";
    int axis_num = 0;

    QFile file(path);

    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        while( !stream.atEnd()){
            pos = stream.readLine();

            if(axis_num ==0)
                ui->show_datum_x_2->setText(QString::number(pos.toDouble(),'f',6));
            else if(axis_num ==1)
                ui->show_datum_y_2->setText(QString::number(pos.toDouble(),'f',6));
            else if(axis_num ==2)
                ui->show_datum_z_2->setText(QString::number(pos.toDouble(),'f',6));

            recordpos[axis_num] = pos.toDouble();
            axis_num++;
        }
    }
    file.close();


    serialpoart = new unit_serialport();
    connect(serialpoart,SIGNAL(sendMsg(int,QString)),this,SIGNAL(show_msg(int,QString)));

    int initp = serialpoart->initSerialport();
    if(initp != 0) {
        show_msg(2,"串口初始化失败");
        return;
    }
//    serialpoart->setting->show();
    int readpos = serialpoart->readZeroPos(AXIS_NUM);
    if(readpos == 0) {
        for(int i=0;i<AXIS_NUM;i++) {
            QString NAME = QString("PAR_AXIS_%1_OUTPUT_SCALE").arg(i);
            double scale = _ini->read(NAME,"VALUE","0.001").toDouble();
            NAME = QString("PAR_AXIS_%1_INPUT_SCALEDIR").arg(i);
            int dir = -1*_ini->read(NAME,"VALUE","0.001").toInt();
            nowpos[i] = scale*serialpoart->zeropos[i];
            qDebug()<<"Now nowpos : "<<nowpos[i]<<"recordpos:"<<recordpos[i]<<"zeropos:"<<serialpoart->zeropos[i];
            sendMultiFunction(71,i,dir * (nowpos[i] - recordpos[i]));
        }
    }


    sendMultiFunction(71,3,0);
    sendMultiFunction(71,4,0);
    sendMultiFunction(71,5,0);
    sendMultiFunction(71,6,0);
    sendMultiFunction(71,7,0);
    sendMultiFunction(71,8,0);
}

void SerialzeroPos::on_btn_home_x_2_clicked()
{
    QString path = "/home/.config/serial_zero_pos";
    QFile file(path);


    int readpos = serialpoart->readZeroPos(AXIS_NUM);
    if(readpos == 0) {
        for(int i=0;i<AXIS_NUM;i++) {
            QString NAME = QString("PAR_AXIS_%1_OUTPUT_SCALE").arg(i);
            double scale = _ini->read(NAME,"VALUE","0.001").toDouble();
            nowpos[i] = scale*serialpoart->zeropos[i];
        }
    } else {
        show_msg(2,"读取零点失败，不能设置零点");
        return;
    }
    recordpos[0] = nowpos[0];
    sendMultiFunction(71,0,0);
    ui->show_datum_x_2->setText(QString::number(recordpos[0],'f',6));
    qDebug()<<"Now X set zero : "<<nowpos[0]<<"recordpos:"<<recordpos[0]<<"zeropos:"<<serialpoart->zeropos[0];

    if(file.open(QIODevice::ReadWrite)){
        QTextStream stream(&file);
        for(int i=0;i<AXIS_NUM;i++) {
            stream<<QString::number(recordpos[i],'f',6)<<"\n";
        }
    }
    file.close();
    show_msg(1,QString("更新X零点为:%1").arg(QString::number(recordpos[0],'f',6)));
}

void SerialzeroPos::on_btn_home_y_2_clicked()
{
    QString path = "/home/.config/serial_zero_pos";
    QFile file(path);


    int readpos = serialpoart->readZeroPos(AXIS_NUM);
    if(readpos == 0) {
        for(int i=0;i<AXIS_NUM;i++) {
            QString NAME = QString("PAR_AXIS_%1_OUTPUT_SCALE").arg(i);
            double scale = _ini->read(NAME,"VALUE","0.001").toDouble();
            nowpos[i] = scale*serialpoart->zeropos[i];
        }
    } else {
        show_msg(2,"读取零点失败，不能设置零点");
        return;
    }
    recordpos[1] = nowpos[1];
    sendMultiFunction(71,1,0);
    ui->show_datum_y_2->setText(QString::number(recordpos[1],'f',6));
    qDebug()<<"Now Y set zero : "<<nowpos[1]<<"recordpos:"<<recordpos[1]<<"zeropos:"<<serialpoart->zeropos[1];

    if(file.open(QIODevice::ReadWrite)){
        QTextStream stream(&file);
        for(int i=0;i<AXIS_NUM;i++) {
            stream<<QString::number(recordpos[i],'f',6)<<"\n";
        }
    }
    file.close();
    show_msg(1,QString("更新Y零点为:%1").arg(QString::number(recordpos[1],'f',6)));
}

void SerialzeroPos::on_btn_home_z_2_clicked()
{
    QString path = "/home/.config/serial_zero_pos";
    QFile file(path);


    int readpos = serialpoart->readZeroPos(AXIS_NUM);
    if(readpos == 0) {
        for(int i=0;i<AXIS_NUM;i++) {
            QString NAME = QString("PAR_AXIS_%1_OUTPUT_SCALE").arg(i);
            double scale = _ini->read(NAME,"VALUE","0.001").toDouble();
            nowpos[i] = scale*serialpoart->zeropos[i];
        }
    } else {
        show_msg(2,"读取零点失败，不能设置零点");
        return;
    }
    recordpos[2] = nowpos[2];
    sendMultiFunction(71,2,0);
    ui->show_datum_z_2->setText(QString::number(recordpos[2],'f',6));

    qDebug()<<"Now Z set zero : "<<nowpos[2]<<"recordpos:"<<recordpos[2]<<"zeropos:"<<serialpoart->zeropos[2];
    if(file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        for(int i=0;i<AXIS_NUM;i++) {
            stream<<QString::number(recordpos[i],'f',6)<<"\n";
        }
    }
    file.close();
    show_msg(1,QString("更新Z零点为:%1").arg(QString::number(recordpos[2],'f',6)));
}

void SerialzeroPos::on_btn_abort_clicked()
{
    serialpoart->setting->close();
    this->close();
}

void SerialzeroPos::on_btn_485cfg_clicked()
{
    serialpoart->setting->show();
}

void SerialzeroPos::on_btn_directSet_clicked()
{
    for(int i=0;i<9;i++) {
        sendMultiFunction(71,i,0);
    }
    show_msg(1,QString("直接设定零点"));
}
