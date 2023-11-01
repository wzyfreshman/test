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
    ui(new Ui::Dialog) {

    ui->setupUi(this);

    qDebug()<<readTimeFun();
    unsigned char fpgaversion=0;

    int ret;
    ret = iopl(3);
    if(ret)
    {
        qDebug("iopl set error=%i\n",ret);

    }

    connect(ui->read,SIGNAL(clicked()),this,SLOT(slotRead()));

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

static int readTimeFun(struct rtc_time &rtc_tm) {

    int fd, retval;
    //int ret;
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
    rtc_tm.tm_mon  += 1;
    rtc_tm.tm_year  += 1900;
    qDebug( "RTC date/time: %d/%d/%d %02d:%02d:%02d\n", rtc_tm.tm_mday,
            rtc_tm.tm_mon , rtc_tm.tm_year ,
            rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
    return 0;

}



static int cmpDate(unsigned int date1[],unsigned int date2[]) {

    if(date1[0] == date2[0] &&
            date1[1] == date2[1] &&
            date1[2] == date2[2]  ) {
        return 0;
    } else if( ( date1[0] >date2[0] ) ||
               ( date1[0] == date2[0] && date1[1] > date2[1] ) ||
               ( date1[0] == date2[0] && date1[1] == date2[1] && date1[2] > date2[2] ) ) {
        return 1;
    } else return -1;

}

static int cmpTime(unsigned int time1[],unsigned int time2[]) {

   int hour = time1[0] - time2[0];
   int minute = time1[1] - time2[1];
   return hour*60 + minute;

}

static int calculateHoursFromDate(unsigned int date1[],
                                  unsigned int date2[]) {

    int year = date1[0] - date2[0];
    int month = date1[1] - date2[1];
    int day = date1[2] - date2[2];
    int hour = cmpTime(date1+3,date2+3);
    return (365*24*year)+(month*30*24)+(day*24)+hour;

}

void Dialog::slotRead() {

    unsigned char tmp[16];
    unsigned int enddate[3];
    unsigned int freshdate[5];
    unsigned int writedate[5];
    unsigned int currentdate[5];

   readRealData(0x0020+8*17,tmp,4);
   ui->textBrowser->append(QString("tmp[0]: %1").arg(tmp[0]));
   if( tmp[0] == 0) {
        ui->textBrowser->append(QString::fromUtf8("当前计时模式为:按小时"));
        readRealData(0x0020+8*10,tmp,8);
        int leftt = charToInt4(tmp);
        ui->textBrowser->append(QString::fromUtf8("还剩余小时数: %1").arg(leftt));
   } else if(tmp[1] ==1 && tmp[2] == 1) {
       ui->textBrowser->append(QString::fromUtf8("当前计时模式为:按日期"));
       ui->textBrowser->append(QString::fromUtf8("已经没有剩余时间了"));
   } else {

       ui->textBrowser->append(QString::fromUtf8("当前计时模式为:按日期"));
       readRealData(0x0020+8*11,tmp,8);
       enddate[0] = charToInt2(tmp);
       enddate[1]  = tmp[2];
       enddate[2]  = tmp[3];
       ui->textBrowser->append(QString::fromUtf8("截止日期: %1 %2 %3").arg(enddate[0]).arg(tmp[2]).arg(tmp[3]));


       readRealData(0x0020+8*12,tmp,8);
       year = charToInt2(tmp);
       freshdate[0] = charToInt2(tmp);
       freshdate[1] = tmp[2];
       freshdate[2] = tmp[3];
       freshdate[3] = tmp[4];
       freshdate[4] = tmp[5];
       ui->textBrowser->append(QString::fromUtf8("刷新日期=%1 %2 %3:    %5 %6").
                               arg(year).arg(tmp[2]).arg(tmp[3]).arg(tmp[4]).arg(tmp[5]));


       readRealData(0x0020+8*13,tmp,8);
       writedate[0]  = charToInt2(tmp);
       writedate[1]  = tmp[2];
       writedate[2]  = tmp[3];
      ui->textBrowser->append(QString::fromUtf8("写入日期=%1 %2 %3 :   %5 %6").
                               arg(writedate[0] ).arg(tmp[2]).arg(tmp[3]).arg(tmp[4]).arg(tmp[5]));


      struct rtc_time rtc_tm;
      readTimeFun(rtc_tm);
      currentdate[0] = rtc_tm.tm_year;
      currentdate[1] = rtc_tm.tm_mon;
      currentdate[2] = rtc_tm.tm_mday;
      currentdate[3] = rtc_tm.tm_hour;
      currentdate[4] = rtc_tm.tm_min;

      ui->textBrowser->append(QString::fromUtf8("当前日期=%1 %2 %3 :   %5 %6").
                               arg(currentdate[0]).arg(currentdate[1]).arg(currentdate[2]).
                              arg(currentdate[3]).arg(currentdate[4]));

       //readRealData(0x0020+8*14,tmp,8);
       //leftt = charToInt4(tmp);
       //ui->textBrowser->append(QString("maxlefttime=%1").arg(leftt));


   }

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

}
