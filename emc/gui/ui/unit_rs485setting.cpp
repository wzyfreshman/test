#include "unit_rs485setting.h"
#include "ui_unit_rs485setting.h"
#include <QMessageBox>
#include "ini_settings.h"
#include "extern.h"
#include "shcom.hh"
#include "emcstatus.hh"
#include <termio.h>

unit_RS485Setting::unit_RS485Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::unit_RS485Setting)
{
    ui->setupUi(this);
    baudrate = _prefer->read("RS485","baud","1",true).toInt();
    QStringList baudname = {"4800","9600","19200","38400","57600","115200"};
    ui->Baul->addItems(baudname);
    ui->Baul->setCurrentIndex(baudrate);
    baudrate = baudArray[baudrate];

    QString devName = _prefer->read("RS485","devname","/dev/ttyUSB0",true);
    ui->DevName->setText(devName);
    devname = ui->DevName->text();

    bool convertok;
    QString getAddr = "";
    getAddr = _prefer->read("RS485","electricaddr","0x0702",true);
    ui->electric_addr->setText(getAddr);
    zeroposAddr = getAddr.toInt(&convertok,16);

    getAddr = _prefer->read("RS485","speedaddr","0x0303",true);
    ui->speed_addr->setText(getAddr);
    speedAddr = getAddr.toInt(&convertok,16);

    getAddr = _prefer->read("RS485","alarmaddr","0x0311",true);
    ui->alarm_addr->setText(getAddr);
    alarmAddr = getAddr.toInt(&convertok,16);

    ui->chk_usealm->setChecked(par_get_var(3093) > 0);

    ui->label_5->hide();
    ui->speed_addr->hide();
    ui->chk_usealm->hide();
    ui->alarm_addr->hide();
    ui->label_3->hide();
}

unit_RS485Setting::~unit_RS485Setting()
{
    delete ui;
}

void unit_RS485Setting::on_btn_close_clicked()
{
//    system("sudo killall florence &");
    this->close();
}

void unit_RS485Setting::on_btn_save_clicked()
{
    _prefer->writeEntry("RS485/devname",ui->DevName->text());
    _prefer->writeEntry("RS485/baud",QString::number(ui->Baul->currentIndex()));
    _prefer->writeEntry("RS485/electricaddr",ui->electric_addr->text());
    _prefer->writeEntry("RS485/speedaddr",ui->speed_addr->text());
    _prefer->writeEntry("RS485/alarmaddr",ui->alarm_addr->text());

    bool convertok;
    baudrate = ui->Baul->currentIndex();
    baudrate = baudArray[baudrate];
    zeroposAddr = ui->electric_addr->text().toInt(&convertok,16);
    speedAddr = ui->speed_addr->text().toInt(&convertok,16);
    devname = ui->DevName->text();
    alarmAddr = ui->alarm_addr->text().toInt(&convertok,16);

    QMessageBox::information(this,"提示","配置已保存",QMessageBox::Ok);
//    system("sudo killall florence &");
    this->close();
}

void unit_RS485Setting::showEvent(QShowEvent *)
{
//    system("su - cnext -c florence &");
}

void unit_RS485Setting::on_chk_usealm_clicked()
{
    if(par_get_var(3093) > 0)
        sendSetParameter(3093,0);
    else
         sendSetParameter(3093,1);
}
