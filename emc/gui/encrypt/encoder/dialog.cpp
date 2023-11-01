#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>
#include <QDebug>
#include <QTime>

#include "des.h"
#include "hash.h"
#include "aes.h"
#include "udf.h"
#include "hexvalidator.h"

#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/io.h>
#include <sys/time.h>
#include "epp.h"

static int readTimeFun() {

    int fd, retval;
    struct rtc_time rtc_tm;
    int ret;
    fd = open("/dev/rtc", O_RDONLY);
    if (fd == -1) {
        perror("/dev/rtc");
        return -1;
    }

    /* Read the RTC time/date */
    retval = ioctl(fd, RTC_RD_TIME , &rtc_tm);
    if (retval == -1) {
        perror("ioctl");
        return -1;
    }
    close(fd);

    qDebug( "RTC date/time: %d/%d/%d %02d:%02d:%02d\n", rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900, rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

    return 0;

}

static int year = 2014;
static int mouth =  12;
static int day = 15;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    qDebug()<<readTimeFun();
    unsigned char fpgaversion=0;

    int ret;
    ret = iopl(3);
    if(ret)
    {
        qDebug("iopl set error=%i\n",ret);

    }



    connect(ui->sign,SIGNAL(clicked()),this,SLOT(slotSign()));
    connect(ui->read,SIGNAL(clicked()),this,SLOT(slotRead()));
    connect(ui->Code1,SIGNAL(textChanged(QString)),this,SLOT(slotCode1(QString)));
    connect(ui->Code2,SIGNAL(textChanged(QString)),this,SLOT(slotCode2(QString)));
    connect(ui->Code3,SIGNAL(textChanged(QString)),this,SLOT(slotCode3(QString)));

   //startTimer(1);

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::setSysString(QString str) {

    this->slotSys(str);

}

void Dialog::setVerString(QString str) {

    this->slotVer(str);

}

void Dialog::slotCode1(QString sre) {

    year = sre.toInt();
    code1 = sre;
    int n;bool ok;
    n = code1.toInt(&ok,16);

    code[0] = n>>8;
    code[1] = n;

}

void Dialog::slotCode2(QString sre) {
    mouth = sre.toInt();
    code2 = sre;
    int n;bool ok;
    n = code2.toInt(&ok,16);

    code[2] = n>>8;
    code[3] = n;
}

void Dialog::slotCode3(QString sre) {

    day = sre.toInt();
    code3 = sre;
    int n;bool ok;
    n = code3.toInt(&ok,16);

    code[4] = n>>8;
    code[5] = n;
}

void Dialog::slotCode4(QString sre) {

    code4 = sre;
    int n;bool ok;
    n = code4.toInt(&ok,16);

    code[6] = n>>8;
    code[7] = n;
}

void Dialog::slotVer(QString v) {
    ver_string = v;
    if(ver_string.size()>8) return;
    for(int i = 0;i<ver_string.size()/2;i++) {
        int tt;bool ok;QString strtmp;
        tt = i*2;
        strtmp = ver_string.mid(tt,2);
        tt = strtmp.toInt(&ok,16);
        ver[i] = tt;
    }
}

void Dialog::slotSys(QString v) {
    sys_string = v;
    if(sys_string.size()>8) return;
    for(int i = 0;i<sys_string.size()/2;i++) {
        int tt;bool ok;QString strtmp;
        tt = i*2;
        strtmp = sys_string.mid(tt,2);
        tt = strtmp.toInt(&ok,16);
        sys[i] = tt;
    }
}


void Dialog::slotRead() {

    unsigned char tmp[16];
    unsigned char writee[8] = {1,2,3,4,89,6,7,8};
    writeRealData(0x0020,writee,8);
    usleep(1000*50);

    readRealData(0x0020+8*17,tmp,4);
    ui->textBrowser->append(QString("mode=%1 left-flag=%2 ").arg(tmp[0]).arg(tmp[1]));

    readRealData(0x0020+8*10,tmp,8);
    int leftt = charToInt4(tmp);
    ui->textBrowser->append(QString("lefttime=%1").arg(leftt));

    readRealData(0x0020+8*11,tmp,8);
    int year = charToInt2(tmp);
    ui->textBrowser->append(QString("end date=%1 %2 %3").arg(year).arg(tmp[2]).arg(tmp[3]));

    readRealData(0x0020+8*12,tmp,8);
    year = charToInt2(tmp);
    ui->textBrowser->append(QString("fresh date=%1 %2 %3 : %5 %6").
                            arg(year).arg(tmp[2]).arg(tmp[3]).arg(tmp[4]).arg(tmp[5]));

    readRealData(0x0020+8*13,tmp,8);
    year = charToInt2(tmp);
    ui->textBrowser->append(QString("writed date=%1 %2 %3 : %5 %6").
                            arg(year).arg(tmp[2]).arg(tmp[3]).arg(tmp[4]).arg(tmp[5]));

    readRealData(0x0020+8*14,tmp,8);
    leftt = charToInt4(tmp);
    ui->textBrowser->append(QString("maxlefttime=%1").arg(leftt));



}

void Dialog::slotSign() {

     unsigned char org[8];

     qDebug()<<"ye++++"<<year<<mouth<<day;
     intToChar2(year,org);
     intToChar2(year,org+4);
     org[2] = mouth;
     org[3] = day;
     org[6] = mouth;
     org[7] = day;

     for(int i=0;i<8;i++)
         qDebug()<<"org"<<org[i];

     qDebug()<<"result~~!!"<<writeEncryptInfo(org,0x0020+88,8,1);

}

void Dialog::timerEvent(QTimerEvent *) {

    testFun();
}


int readyear = 2014;
int readmouth = 12;
int readday = 15;
int hour = 12;
int minute = 1;

static void addDay() {

    if(readday<=0 || readday>31) return;
    if(readyear<=0) return;
    if(readmouth<=0 || readmouth>12) return;

    if(readday <= 27 && readday>0) {
          readday++;
    } else if( readmouth ==2 ) {
            if( ((readyear%4 ==0) && (readyear%100 != 0)) || (readyear%400 == 0) ) {
                 if(readday == 28)
                    readday++;
                 else {
                     readday = 1;
                     if(readmouth<12)
                         readmouth ++;
                     else {
                         readyear++;
                         readmouth = 1;
                     }
                 }

        } else {
                readday = 1;
                if(readmouth<12)
                    readmouth ++;
                else {
                    readyear++;
                    readmouth = 1;
                }

            }
    } else if( ((readmouth<=7)&&(readmouth%2 == 0) )||
                         ((readmouth>=8)&&(readmouth%2 == 1) ) ) {
            if(readday < 30) {
                readday++;
            } else {
                readday = 1;
                if(readmouth<12)
                    readmouth ++;
                else {
                    readyear++;
                    readmouth = 1;
                }
            }

    } else if( ((readmouth<=7)&&(readmouth%2 == 1)) ||
                      ((readmouth>=8)&&(readmouth%2 == 0) ) ) {
            if(readday < 31) {
                readday++;
            } else {
                readday = 1;
                if(readmouth<12)
                    readmouth ++;
                else {
                    readyear++;
                    readmouth = 1;
                }
            }

    }


}

/**
*return 0---means normal mode  1---means just add hour  2---means add hour and day
* return -1---means error
*/
static int addOneMinute() {

    if( hour<0 || hour >=24 ) return -1;
    if( minute<0 || minute >=60 ) return -1;
    int ret = 0;
    if( minute>=0 && minute <= 58) {
        minute ++;
    } else {
        minute = 0;
        if(hour == 23) {
            ret = 2;
            hour = 0;
        } else {
            hour++;
            ret = 1;
        }
    }
    return ret;

}

static int addFiveMinute() {

    int i ;
    int ret = 0;
    for(i = 0; i<5; i++)
        ret += addOneMinute();
    if(ret>2) ret = -1;
    return ret;

}

void Dialog::testFun() {

   int ret =  addFiveMinute();
   if(ret == 2)
       addDay();

   QString tmmp = QString("%1 %2 %3 : %4 %5").arg(readyear).arg(readmouth)
           .arg(readday).arg(hour).arg(minute);
   ui->label_2->setText(tmmp);


}
