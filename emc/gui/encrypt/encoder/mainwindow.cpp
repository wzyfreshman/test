#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTime>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileInfo>

#include "../../encrypt/math/des.h"
#include "aes.h"
#include "hash.h"
#include "udf.h"
#include "hexvalidator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    connect(ui->Info ,SIGNAL(textChanged(QString)),this,SLOT(slotInfo(QString)));
    connect(ui->SysNum ,SIGNAL(textChanged(QString)),this,SLOT(slotSys(QString)));
    //connect(ui->VerNum ,SIGNAL(textChanged(QString)),this,SLOT(slotVer(QString)));
    connect(ui->info_day ,SIGNAL(textChanged(QString)),this,SLOT(slotInfoday(QString)));
    connect(ui->Info_hour ,SIGNAL(textChanged(QString)),this,SLOT(slotInfohour(QString)));
    connect(ui->Change_Mode,SIGNAL(activated(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));

    ui->SysNum->setValidator(new HexValidator);
    //ui->VerNum->setValidator(new QIntValidator());

    QIntValidator * infol = new QIntValidator();
    infol->setRange(1,8760);
    ui->Info->setValidator(infol);

    QIntValidator * dayl = new QIntValidator();
    dayl->setRange(1,365);
    ui->info_day->setValidator(dayl);

    QIntValidator * hourl = new QIntValidator();
    hourl->setRange(1,24);
    ui->Info_hour->setValidator(hourl);

    connect(ui->generate,SIGNAL(clicked()),this,SLOT(slotGenerateCode()));
    connect(ui->foreverCheck,SIGNAL(stateChanged(int)),this,SLOT(slotForeverCheck(int)));
    connect(ui->Change_Mode,SIGNAL(activated(int)),this,SLOT(slotChangeMode(int)));
    connect(ui->GetDate,SIGNAL(dateChanged(QDate)),this,SLOT(slotInfoDate(QDate)));
    ui->customer->setText(__CURRENT_CUSTOMER.name);

    QDate maxD(2050,1,1);
    QDate minD = QDate::currentDate();
    ui->GetDate->setMaximumDate(maxD);
    ui->GetDate->setMinimumDate(minD);
    ui->GetDate->setDate(QDate::currentDate());

    day = 0;
    hour = 0;
    mode = 0;

    ui->Change_Mode->addItem(QString::fromUtf8("按小时"));
    ui->Change_Mode->addItem(QString::fromUtf8("按日期"));
    ui->GetDate->setEnabled(false);

}

void MainWindow::setExcutePath(const QString &str) {

    QFileInfo info(str);
    excutepath =  info.absolutePath();

}

void MainWindow::slotChangeMode(int m) {
    qDebug()<<"mode"<<m;
    mode = m;
    if(mode == 0) {

        ui->GetDate->setEnabled(false);
        ui->Info->setEnabled(true);
        ui->info_day->setEnabled(true);
        ui->Info_hour->setEnabled(true);
    } else {

        ui->GetDate->setEnabled(true);
        ui->Info->setEnabled(false);
        ui->info_day->setEnabled(false);
        ui->Info_hour->setEnabled(false);

    }


}

void MainWindow::slotForeverCheck(int ) {

    if(ui->foreverCheck->isChecked()) {
       ui->stackedWidget->setEnabled(false);
    } else {
       ui->stackedWidget->setEnabled(true);
    }

}

void MainWindow::slotInputCode(QString str) {
    userinput = str;
}

void MainWindow::slotVer(QString v) {

    ver_string = v;
    if(ver_string.size()>3) return;
    bool ok;
    unsigned int tt = ver_string.toInt(&ok,10);
    intToChar2(tt,sys_ver+6);


}

void MainWindow::slotSys(QString v) {

    sys_string = v;
    if(sys_string.size() != 16) return;

    for(int i = 0;i<sys_string.size()/2;i++) {
        int tt;bool ok;
        QString strtmp;
        tt = i*2;
        strtmp = sys_string.mid(tt,2);
        tt = strtmp.toInt(&ok,16);
        sys_ver[i] = tt;
    }

}

void MainWindow::slotInfoday(QString v) {

    //if(v.size()>3) return;
    day = v.toInt();
    QString tmp;
    int d = day*hour;
    tmp = QString::number(d);
    ui->Info->setText(tmp);
}

void MainWindow::slotInfohour(QString v) {

    //if(v.size()>3) return;
    hour = v.toInt();
    QString tmp;
    int d = day*hour;
    tmp = QString::number(d);
    ui->Info->setText(tmp);
}


void MainWindow::slotInfo(QString v) {
    info_string = v;
    if(info_string.size()>8) return;


    unsigned int h = info_string.toInt();

    h *= 12;
    intToChar4(h,info);

    info[0] &= 0x3f;

}

void MainWindow::slotInfoDate(QDate date) {

    int year = date.year();
    int month = date.month();
    int day = date.day();

    intToChar2(year,info_date);
    info_date[2] = month;
    info_date[3] = day;

    info_date[0] &= 0x3f;
    info_date[0] |= 0x40;

}


void MainWindow::slotGenerateCode() {

    if( !ui->foreverCheck->isChecked()  && mode == 0) {
        if(  sys_string.size() != 16 ||
                info_string.size() <= 0) {
            QMessageBox::warning(this,"warning", QString::fromLocal8Bit("格式不正确!!"));
            return;
        }

    } else {

        if( sys_string.size() != 16 ) {
            QMessageBox::warning(this,"warning", QString::fromLocal8Bit("格式不正确!!"));
            return;
        }

    }

    for(int i = 0;  i<4;  i++) {
        sys[i] = sys_ver[i+4];
        ver[i] = sys_ver[i];

    }

    QString code,sptf;
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    unsigned char random;
    random = qrand()%255;

    unsigned char out[8];
    setVerNum(ver);
    setSysNum(sys);
    if(ui->foreverCheck->isChecked()) {
        unsigned char infotmp[4];
        for(int i = 0;i<4;i++)
            infotmp[i] = 0xff;
        setInfoNum(infotmp);
    } else {
        if(mode == 0) {
            setInfoNum(info);
        }  else {
            setInfoNum(info_date);
        }
    }

    unsigned char encryinfo[8];
    infoEncryPt(encryinfo,random);

    des_context ctx;
    des_set_key(&ctx,__CURRENT_CUSTOMER.id);
    des_encrypt(&ctx,encryinfo,out);

    QString tmp;
    int n;

    QString forWrite;

    code.clear();
    n = out[0];
    tmp = sptf.sprintf("%02x",n).toUpper();
    code+=tmp;
    n = out[1];
    tmp = sptf.sprintf("%02x",n).toUpper();
    code+=tmp;
    ui->Key1->setText(code);
    forWrite +=code;

    code.clear();
    n = out[2];
    tmp = sptf.sprintf("%02x",n).toUpper();
    code+=tmp;
    n = out[3];
    tmp = sptf.sprintf("%02x",n).toUpper();
    code+=tmp;
    ui->Key2->setText(code);
    forWrite +=code;

    code.clear();
    n = out[4];
    tmp = sptf.sprintf("%02x",n).toUpper();
    code+=tmp;
    n = out[5];
    tmp = sptf.sprintf("%02x",n).toUpper();
    code+=tmp;
    ui->Key3->setText(code);
    forWrite +=code;

    code.clear();
    n = out[6];
    tmp = sptf.sprintf("%02x",n).toUpper();
    code+=tmp;
    n = out[7];
    tmp = sptf.sprintf("%02x",n).toUpper();
    code+=tmp;
    ui->Key4->setText(code);
    forWrite +=code;

   QString filename = QString("%1/%2").arg(excutepath).arg(("record.txt"));
   QFile file(filename);
   if(!file.open(QFile::Append|QFile::WriteOnly|QFile::Text)) {
       QMessageBox::warning(this,"warning", QString::fromLocal8Bit("当前没有权限读写文件，此次记录无效!!"));
   } else {

        QString data;
       if(mode == 0 && !ui->foreverCheck->isChecked()) {
           file.write("Hour  ");
           data = info_string;
           for(int i=data.size();i<10;i++)
                data.append(" ");
       } else if(mode == 1 && !ui->foreverCheck->isChecked())  {
           file.write("Date  ");
           data = ui->GetDate->date().toString("yyyy-MM-dd");
       } else {
           file.write("None  ");
           data = "Forever   ";
       }
        file.write(data.toLocal8Bit().data());
        file.write("  ");
        file.write(sys_string.toLocal8Bit().data());
        file.write("  ");
        file.write(ver_string.toLocal8Bit().data());
        file.write("  ");
        file.write(forWrite.toLocal8Bit().data());
        file.write("       ");
        file.write(QDate::currentDate().toString("yyyy-MM-dd:").toLocal8Bit().data());
        file.write(QTime::currentTime().toString().toLocal8Bit().data());
        file.write("\n");
        file.flush();
   }
       file.close();

}

MainWindow::~MainWindow() {

    delete ui;
}
