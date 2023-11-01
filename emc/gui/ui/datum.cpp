#include "datum.h"
#include "ui_datum.h"
#include "emc.hh"
#include "emc_nml.hh"
#include <QtDebug>
#include "shcom.hh"
#include <QFileDialog>
#include <inifile.hh>
#include "extern.h"
#include "qshortcut.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QPropertyAnimation>
#include <QDesktopWidget>

Datum::Datum(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Datum)
{
    ui->setupUi(this);

    if(_doublez < 6){
        ui->frame_6->hide();
        ui->btn_home_z4->hide();
        ui->btn_unhome_z4->hide();
        ui->show_abs_x_6->hide();
    }


    connect(ui->btn_home_x,SIGNAL(clicked()),this,SLOT(home_x()));
    connect(ui->btn_home_y,SIGNAL(clicked()),this,SLOT(home_y()));
    connect(ui->btn_home_z,SIGNAL(clicked()),this,SLOT(home_z()));
    connect(ui->btn_home_z2,SIGNAL(clicked()),this,SLOT(home_z2()));
    connect(ui->btn_home_z3,SIGNAL(clicked()),this,SLOT(home_z3()));
    connect(ui->btn_home_z4,SIGNAL(clicked()),this,SLOT(home_z4()));

    connect(ui->btn_unhome_x,SIGNAL(clicked()),this,SLOT(unhome_x()));
    connect(ui->btn_unhome_y,SIGNAL(clicked()),this,SLOT(unhome_y()));
    connect(ui->btn_unhome_z,SIGNAL(clicked()),this,SLOT(unhome_z()));
    connect(ui->btn_unhome_z2,SIGNAL(clicked()),this,SLOT(unhome_z2()));
    connect(ui->btn_unhome_z3,SIGNAL(clicked()),this,SLOT(unhome_z3()));
    connect(ui->btn_unhome_z4,SIGNAL(clicked()),this,SLOT(unhome_z4()));

    connect(ui->btn_export,SIGNAL(released()),this,SLOT(Export()));
    connect(ui->btn_import,SIGNAL(released()),this,SLOT(Import()));

    home_init();

    //四轴
    if((int)par_get_var(3999) == 1) {
        ui->btn_home_z3->hide();
        ui->frame_5->hide();
        ui->btn_unhome_z3->hide();
        ui->show_abs_x_5->hide();

        ui->btn_home_z4->hide();
        ui->frame_6->hide();
        ui->btn_unhome_z4->hide();
        ui->show_abs_x_6->hide();

    } else if((int)par_get_var(3999) == 2) {
        //AB五轴
        ui->btn_home_z4->hide();
        ui->frame_6->hide();
        ui->btn_unhome_z4->hide();
        ui->show_abs_x_6->hide();
    } else if((int)par_get_var(3999) == 3) {
        //AC五轴
        ui->btn_home_z3->hide();
        ui->frame_5->hide();
        ui->btn_unhome_z3->hide();
        ui->show_abs_x_5->hide();
    } //3轴
    if((int)par_get_var(3999) == 4 || (int)par_get_var(3999) == 5 || (int)par_get_var(3999) == 7) {
        int tooltype = _ini->read("PAR_RS274_TOOLTABLE_TYPE","VALUE","1").toInt();
        if(tooltype != 2){
        ui->btn_home_z3->hide();
        ui->frame_5->hide();
        ui->btn_unhome_z3->hide();
        ui->show_abs_x_5->hide();
        }

        ui->btn_home_z4->hide();
        ui->frame_6->hide();
        ui->btn_unhome_z4->hide();
        ui->show_abs_x_6->hide();

        ui->btn_home_z2->hide();
        ui->frame_4->hide();
        ui->btn_unhome_z2->hide();
        ui->show_abs_x_4->hide();
    }//XYA轴
    if((int)par_get_var(3999) == 6 ) {
        ui->btn_home_z3->hide();
        ui->frame_5->hide();
        ui->btn_unhome_z3->hide();
        ui->show_abs_x_5->hide();

        ui->btn_home_z4->hide();
        ui->frame_6->hide();
        ui->btn_unhome_z4->hide();
        ui->show_abs_x_6->hide();

        ui->btn_home_z->hide();
        ui->frame_3->hide();
        ui->btn_unhome_z->hide();
        ui->show_abs_x_3->hide();
    }
}

Datum::~Datum()
{
    delete ui;
}

void Datum::home_init(){
    QString path = "/home/.config/zero_pos";
    QString pos = " ";
    int axis_num = 0;

    QFile file(path);

    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        while( !stream.atEnd()){
            pos = stream.readLine();
//            if(axis_num < _doublez && fabs(pos.toDouble()) < 0.00001)
//                sendUnHome(axis_num,1);

            if(axis_num ==0)
                ui->show_datum_x->setText(QString::number(pos.toDouble(),'f',6));
            else if(axis_num ==1)
                ui->show_datum_y->setText(QString::number(pos.toDouble(),'f',6));
            else if(axis_num ==2)
                ui->show_datum_z->setText(QString::number(pos.toDouble(),'f',6));
            else if(axis_num ==3)
                ui->show_datum_z2->setText(QString::number(pos.toDouble(),'f',6));
            else if(axis_num ==4)
                ui->show_datum_z3->setText(QString::number(pos.toDouble(),'f',6));
            else if(axis_num ==5)
                ui->show_datum_z4->setText(QString::number(pos.toDouble(),'f',6));

            axis_num++;
        }
    }
    file.close();

}


double Datum::get_datum(int axis){
    QString path = "/home/.config/zero_pos";
    QString pos = " ";
    double ret = 0;
    int line = 0;

    QFile file(path);

    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        while( !stream.atEnd()){
            pos = stream.readLine();

            if(line == axis)
                ret = pos.toDouble();

            line++;
        }
    }else{
        emit(send_msg(0,tr("File open fail!")));
    }

    file.close();

    return ret;

}

void Datum::unhome_x(){
    sendUnHome(0,1);
}

void Datum::unhome_y(){
    sendUnHome(1,1);
}

void Datum::unhome_z(){
    sendUnHome(2,1);
}

void Datum::unhome_z2(){
    sendUnHome(3,1);
}

void Datum::unhome_z3(){
    sendUnHome(4,1);
}

void Datum::unhome_z4(){
    sendUnHome(5,1);
}

void Datum::home_x()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();
    sendUnHome(0);
    sendHome(0,1);
    ui->show_datum_x->setText(QString::number(get_datum(0),'f',6));
    emit(send_msg(0,tr("Set current coordination as X zero pos")));

}

void Datum::home_y()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();
    sendUnHome(1);
    sendHome(1,1);

    sendUnHome(7);
    sendHome(7,1);
    ui->show_datum_y->setText(QString::number(get_datum(1),'f',6));
    emit(send_msg(0,tr("Set current coordination as Y zero pos")));
}

void Datum::home_z()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();

    if(_doublez) {
        sendUnHome(2);
        sendHome(2,1);
    } else {
        sendHome(2,1);
        sendHome(3,1);
    }

    ui->show_datum_z->setText(QString::number(get_datum(2),'f',6));

    emit(send_msg(0,tr("Set current coordination as Z zero pos")));
}

void Datum::home_z2()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();
    sendUnHome(3);
    sendHome(3,1);
    ui->show_datum_z2->setText(QString::number(get_datum(3),'f',6));
    emit(send_msg(0,tr("Set current coordination as Z2 zero pos")));

}

void Datum::home_z3()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();
    sendUnHome(4);
    sendHome(4,1);
    ui->show_datum_z3->setText(QString::number(get_datum(4),'f',6));
    emit(send_msg(0,tr("Set current coordination as Z3 zero pos")));

}

void Datum::home_z4()
{
    if(EMC_TASK_MODE_MANUAL != emcStatus->task.mode)
        sendManual();
    sendUnHome(5);
    sendHome(5,1);
    ui->show_datum_z4->setText(QString::number(get_datum(5),'f',6));
    emit(send_msg(0,tr("Set current coordination as Z4 zero pos")));

}


void Datum::Export(){
    QString absfilepath;
    QDir dir("/home/cnc-abs/");
    int rett;
    int ret;
    if(!dir.exists())
        rett = system("mkdir /home/cnc-abs/");

    QString desktop = "/home/cnext/Desktop/cncPos.abs";
    QDir Desktop(desktop);
    if(!Desktop.exists())
        desktop = tr("/home/cnext/Desktop/cncPos.abs");


    absfilepath = QFileDialog::getSaveFileName(this,tr("Save File"),desktop,tr("Files(*.abs)"));

    if(absfilepath == NULL)
        return;

    rett = system("cp /home/.config/zero_pos /home/cnc-abs/ -rf");

    rett = system("sudo tar -zcvf /home/cnc-abs.tar -C /home/ cnc-abs");



    QString cmd = QString("sudo zip -qjP CNEXT2018 /home/cncPos.zip /home/cnc-abs.tar");
    ret = system(cmd.toLocal8Bit().data());
    if(ret == -1) {
        QMessageBox::warning(this,tr("warning"),
               tr("Some error occurred,Error code -502"),tr("Ok"));
    }


    rett = system("sudo rm /home/cnc-abs.tar -rf");


    cmd = QString("mv /home/cncPos.zip %1").arg(absfilepath);
    qDebug()<<"path ==="<<absfilepath;
    ret = system(cmd.toLocal8Bit().data());
    if(ret == -1) {
        QMessageBox::warning(this,tr("warning"),
               tr("Some error occurred,Error code -503"),tr("Ok"));
    }


//    cmd = QString("sudo chmod 777 %1").arg(absfilepath);
//    ret = system(cmd.toLocal8Bit().data());
//    if(ret == -1) {
//        QMessageBox::warning(this,tr("warning"),
//               tr("Some error occurred,Error code -504"),tr("Ok"));
//    }




    emit(send_msg(0,tr("Export Successful!")));
    QMessageBox::information(this,tr("info"),tr("Export Successful!"),
                                 QMessageBox::Ok);
}

void Datum::Import(){

    QString absfilepath;
    QDir dir("/home/.tmp/");
    int rett;
    int ret;
    if(!dir.exists())
        rett = system("mkdir /home/.tmp/");


    QString desktop = "/home/cnext/Desktop";
    QDir Desktop(desktop);
    if(!Desktop.exists())
        desktop = tr("/home/cnext/Desktop");

    //updatefilepath = QFileDialog::getOpenFileName(this,
     //    tr("Open Update File"), desktop, tr("Update Files (*.cnc)"));

    absfilepath = getfileName(tr("Open Update File"),desktop,tr("Datum Files (*.abs)"),this);

    if(absfilepath.isEmpty())
        return;

    int rets=QMessageBox::information(this,tr("info"),
                                     tr("Import ABScoor,ensure continue?"),
                                     QMessageBox::Ok,QMessageBox::Cancel);
    if(rets==QMessageBox::Ok){

    }else if(QMessageBox::Cancel){
        return;
    }

    QDir dirt("/home/.tmp/abs");
    if(!dirt.exists())
        ret = system("mkdir /home/.tmp/abs");
    if(ret == -1) {
        QMessageBox::warning(this,tr("warning"),
               tr("Some error occurred,Error code -401"),tr("Ok"));
    }

    ret = system("rm -rf /home/.tmp/abs/*");
    if(ret == -1) {
        QMessageBox::warning(this,tr("warning"),
               tr("Some error occurred,Error code -402"),tr("Ok"));
    }


    QString cmd;
    cmd = QString("unzip -P CNEXT2018 '%1' -d /home/.tmp/abs").arg(absfilepath);
    ret = system(cmd.toLocal8Bit().data());
    if(ret == -1) {
        QMessageBox::warning(this,tr("warning"),
               tr("Some error occurred,Error code -403"),tr("Ok"));
    }


    cmd = QString("tar -xmf /home/.tmp/abs/cnc-abs.tar -C /home/.tmp/abs");
    ret = system(cmd.toLocal8Bit().data());
    if(ret == -1) {
        QMessageBox::warning(this,tr("warning"),
               tr("Some error occurred,Error code -404"),tr("Ok"));
    }

    QFile file_test("/home/.tmp/abs/cnc-abs/zero_pos");
    if( !file_test.exists() ) {
        QMessageBox::warning(this,tr("warning"),
               tr("File corruption!"),tr("OK"));
        rett = system("rm /home/.tmp/abs/* -rf");
        return;
    }

    rett = system("rm /home/.config/zero_pos -rf");

    rett = system("cp /home/.tmp/abs/cnc-abs/zero_pos /home/.config/zero_pos -rf");


    emit(send_msg(1,tr("Import Successful! Validate Reboot!")));
//    QMessageBox::information(this,tr("info"),tr("Validate Reboot"),
//                                 QMessageBox::Ok);

}

