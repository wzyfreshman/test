#include "decrypt_dialog.h"
#include "ui_decrypt_dialog.h"

#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <unistd.h>
#include <time.h>

#include "extern.h"
#include "epp.h"

#include "math/hash.h"
#include "math/aes.h"
#include "math/udf.h"
#include "math/hexvalidator.h"

#include "emcstatus.hh"
#include "extern.h"
#include "mywidget.h"
#include "numstr_inputdialog.h"

#include "shcom.hh"
#include "emc.hh"

#include "../task/des.h"

Decrypt_Dialog::Decrypt_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Decrypt_Dialog)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Register").append("-V2"));

    connect(ui->Code1,SIGNAL(textChanged(QString)),this,SLOT(slotCode1(QString)));
    connect(ui->Code2,SIGNAL(textChanged(QString)),this,SLOT(slotCode2(QString)));
    connect(ui->Code3,SIGNAL(textChanged(QString)),this,SLOT(slotCode3(QString)));
    connect(ui->Code4,SIGNAL(textChanged(QString)),this,SLOT(slotCode4(QString)));

    connect(ui->Code1_2,SIGNAL(textChanged(QString)),this,SLOT(slotCode1_2(QString)));
    connect(ui->Code2_2,SIGNAL(textChanged(QString)),this,SLOT(slotCode2_2(QString)));
    connect(ui->Code3_2,SIGNAL(textChanged(QString)),this,SLOT(slotCode3_2(QString)));
    connect(ui->Code4_2,SIGNAL(textChanged(QString)),this,SLOT(slotCode4_2(QString)));

    //connect(ui->SysNum,SIGNAL(textChanged(QString)),this,SLOT(slotSys(QString)));
    connect(ui->sign,SIGNAL(clicked()),this,SLOT(slotSign()));

    ui->SysNum->setValidator(new Base36Validator());
    ui->Code1->setValidator(new HexValidator());
    ui->Code2->setValidator(new HexValidator());
    ui->Code3->setValidator(new HexValidator());
    ui->Code4->setValidator(new HexValidator());

    ui->Code1_2->setValidator(new HexValidator());
    ui->Code2_2->setValidator(new HexValidator());
    ui->Code3_2->setValidator(new HexValidator());
    ui->Code4_2->setValidator(new HexValidator());


    ui->SysNum->clear();
    ui->Code1->clear();
    ui->Code2->clear();
    ui->Code3->clear();
    ui->Code4->clear();
     ui->Code1->setFocus();

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    //read the sysnum
    ui->SysNum->setReadOnly(true);

    QString verstr;

    verstr.sprintf("  %02x%02x - %02x%02x - %02x%02x - %02x%02x",
                            emcStatus->motion.checkandsys[0],
                            emcStatus->motion.checkandsys[1],
                            emcStatus->motion.checkandsys[2],
              emcStatus->motion.checkandsys[3],
              emcStatus->motion.checkandsys[4],
              emcStatus->motion.checkandsys[5],
              emcStatus->motion.checkandsys[6],
              emcStatus->motion.checkandsys[7]);
    verstr = verstr.toUpper();

    ui->SysNum->setText(verstr);


    update();
    connect(ui->btncancel,SIGNAL(clicked()),this,SLOT(close()));

    //connect(ui->modify_num,SIGNAL(clicked()),this,SLOT(setSysString()));
    QTimer *flush_time = new QTimer(this);
    connect(flush_time,SIGNAL(timeout()),this,SLOT(update()));
    flush_time->start(5000);
}

Decrypt_Dialog::~Decrypt_Dialog()
{
    delete ui;
}

void Decrypt_Dialog::update() {

    ui->showNowtime->setText(_time_now);

    if(0 == emcStatus->motion.timetype) {
        ui->showTime->setText(tr("LeftTime:"));
        ui->showLefttime->setStyleSheet("color:black");
        if(get_time_left() > 3600)
            ui->showLefttime->setText(QString(tr("%1 hours %2 minutes"))
                                      .arg(get_time_left()/3600)
                                      .arg((get_time_left()%3600)/60));
        else if(get_time_left() < 3600 && get_time_left() > 60)
            ui->showLefttime->setText(QString(tr("%1 minutes")).arg(get_time_left()/60));
        else if(get_time_left() >= 0 && get_time_left() <= 60){
            ui->showLefttime->setText(QString(tr("time over")));
        }else if(get_time_left() < 0) {
            ui->showLefttime->setText(QString(tr("permanent valid")));
        }
    } else {
        ui->showTime->setText(tr("EndDate:"));
        QString timeshow = QString("%1-%2-%3  %4:%5:00").arg(emcStatus->motion.Enddate[0])
                .arg(emcStatus->motion.Enddate[1]).arg(emcStatus->motion.Enddate[2])
                .arg(emcStatus->motion.Enddate[3]).arg(emcStatus->motion.Enddate[4]);
        ui->showLefttime->setText(timeshow);
        ui->showLefttime->setStyleSheet("color:black");
    }




    if( authorize_timeout()) {
            ui->SysManuState->setText(tr("Matchine is Locked.Please input the unlock code"));
    } else {
        if(emcStatus->motion.MachineLockdate[0] == 0xffff) {
            ui->SysManuState->setText(tr("Matchine is unLocked"));
        } else {
            QString timeshow = QString("%1-%2-%3  %4:00:00").arg(emcStatus->motion.MachineLockdate[0])
                    .arg(emcStatus->motion.MachineLockdate[1]).arg(emcStatus->motion.MachineLockdate[2])
                    .arg(emcStatus->motion.MachineLockdate[3]);
            ui->SysManuState->setText(timeshow);
        }

    }

    //set background-color red
    static int count = 0;
    if(count < 10){
        if(register_timeout())
            ui->showLefttime->setStyleSheet("background-color:red;color:yellow");
        else
            ui->showLefttime->setStyleSheet("color:black");


         if( authorize_timeout() )
             ui->SysManuState->setStyleSheet("background-color:red;color:yellow");
         else
             ui->SysManuState->setStyleSheet("color:black");
    }


}

void Decrypt_Dialog::setSysString() {

    if(PassWord::getPassWord(this,"syslock")) {

        bool ok;
        NumStr_InputDialog dlg(this);
        int ret = dlg.exec();
        if(ret) ok = true;
        else ok =false;

        QString text = NumStr_InputDialog::inputtext;

         if (ok && !text.isEmpty()) {
             if(text.size() > 20) {
                 QMessageBox::warning(this,tr("warnning"),
                 QString(tr("sysnum too large")),tr("Ok"));
                 return;
             }
             ui->SysNum->setText(text);
         }
    }

}

void Decrypt_Dialog::setVerString(QString str) {

    this->slotVer(str);

}

void Decrypt_Dialog::slotCode1(QString sre) {

    code1 = sre;
    int n;bool ok;
    n = code1.toInt(&ok,16);

    code[0] = n>>8;
    code[1] = n;

    if(sre.size()==4 )
        ui->Code2->setFocus();

}

void Decrypt_Dialog::slotCode2(QString sre) {
    code2 = sre;
    int n;bool ok;
    n = code2.toInt(&ok,16);

    code[2] = n>>8;
    code[3] = n;

    if(sre.size()==4 )
        ui->Code3->setFocus();
}

void Decrypt_Dialog::slotCode3(QString sre) {
    code3 = sre;
    int n;bool ok;
    n = code3.toInt(&ok,16);

    code[4] = n>>8;
    code[5] = n;

    if(sre.size()==4 )
        ui->Code4->setFocus();
}

void Decrypt_Dialog::slotCode4(QString sre) {

    code4 = sre;
    int n;bool ok;
    n = code4.toInt(&ok,16);

    code[6] = n>>8;
    code[7] = n;
}



void Decrypt_Dialog::slotCode1_2(QString sre) {

    code1_2 = sre;
    int n;bool ok;
    n = code1_2.toInt(&ok,16);

    code_2[0] = n>>8;
    code_2[1] = n;

    if(sre.size()==4 )
        ui->Code2_2->setFocus();

}

void Decrypt_Dialog::slotCode2_2(QString sre) {
    code2_2 = sre;
    int n;bool ok;
    n = code2_2.toInt(&ok,16);

    code_2[2] = n>>8;
    code_2[3] = n;

    if(sre.size()==4 )
        ui->Code3_2->setFocus();
}

void Decrypt_Dialog::slotCode3_2(QString sre) {
    code3_2 = sre;
    int n;bool ok;
    n = code3_2.toInt(&ok,16);

    code_2[4] = n>>8;
    code_2[5] = n;

    if(sre.size()==4 )
        ui->Code4_2->setFocus();
}

void Decrypt_Dialog::slotCode4_2(QString sre) {

    code4_2 = sre;
    int n;bool ok;
    n = code4_2.toInt(&ok,16);

    code_2[6] = n>>8;
    code_2[7] = n;
}



void Decrypt_Dialog::slotVer(QString v) {
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

void Decrypt_Dialog::slotSys(QString v) {
/*
    sys_string = v;
    if(sys_string.size() != 6) return;
    QFile fp(SAVE_SYSNUM);
    fp.open(QIODevice::WriteOnly);
    fp.write(sys_string.toLocal8Bit());
    fp.close();

    char buf[32];
    int i;
    for( i = 0;i<sys_string.size();i++) {
        buf[i] = sys_string.at(i).toLatin1();
    }
    buf[i] = '\0';

    unsigned int tmp = base36ToDecimal(buf);
    intToChar4(tmp,sys);

    qDebug()<<"sysnum=="<<tmp;
*/
}



static int readTimeFun(struct tm &rtc_tm) {
    time_t now;
    struct tm *t;
    time(&now);
    t = localtime(&now);
    rtc_tm = *t;
    rtc_tm.tm_mon +=1;			/* Month.	[0-11] */
    rtc_tm.tm_year +=1900;			/* Year	- 1900.  */
    qDebug( "RTC date/time: %d/%d/%d %02d:%02d:%02d\n", rtc_tm.tm_mday,
            rtc_tm.tm_mon , rtc_tm.tm_year ,
            rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
    return 0;
}

#define MINUTE 60	// 1 分钟的秒数。
#define HOUR (60*MINUTE)	// 1 小时的秒数。
#define DAY (24*HOUR)	// 1 天的秒数。
#define YEAR (365*DAY)	// 1 年的秒数。
/* interestingly, we assume leap-years */
/* 有趣的是我们考虑进了闰年 */
// 下面以年为界限，定义了每个月开始时的秒数时间数组。
static int month[12] = {
  0,
  DAY * (31),
  DAY * (31 + 29),
  DAY * (31 + 29 + 31),
  DAY * (31 + 29 + 31 + 30),
  DAY * (31 + 29 + 31 + 30 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
  DAY * (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
};
// 该函数计算从1970 年1 月1 日0 时起到开机当日经过的秒数，作为开机时间。
static long kernel_mktime (struct tm *tm)
{
  long res;
  int year;
  year = tm->tm_year - 70;	// 从70 年到现在经过的年数(2 位表示方式)，
  // 因此会有2000 年问题。
  /* magic offsets (y+1) needed to get leapyears right. */
  /* 为了获得正确的闰年数，这里需要这样一个魔幻偏值(y+1) */
  res = YEAR * year + DAY * ((year + 1) / 4);	// 这些年经过的秒数时间 + 每个闰年时多1 天
  res += month[tm->tm_mon];	// 的秒数时间，在加上当年到当月时的秒数。
  /* and (y+2) here. If it wasn't a leap-year, we have to adjust */
  /* 以及(y+2)。如果(y+2)不是闰年，那么我们就必须进行调整(减去一天的秒数时间)。 */
  if (tm->tm_mon > 1 && ((year + 2) % 4))
    res -= DAY;
  res += DAY * (tm->tm_mday - 1);	// 再加上本月过去的天数的秒数时间。
  res += HOUR * tm->tm_hour;	// 再加上当天过去的小时数的秒数时间。
  res += MINUTE * tm->tm_min;	// 再加上1 小时内过去的分钟数的秒数时间。
  res += tm->tm_sec;	// 再加上1 分钟内已过的秒数。
  return res;	// 即等于从1970 年以来经过的秒数时间。
}

long mydifftime(struct tm *tm1,struct tm *tm2){
    long res;
    res=kernel_mktime(tm1) - kernel_mktime(tm2) ;
    return res;
}


void Decrypt_Dialog:: slotSign() {

    int do_what = -1;

    if(code1.size() == 4 && code2.size() ==4 &&
            code3.size() == 4 &&code4.size() ==4) {
        do_what = 1;
    } else if(code1_2.size() == 4 && code2_2.size() ==4 &&
              code3_2.size() == 4 &&code4_2.size() ==4) {
        do_what = 2;
    }

    if( do_what == -1 ) {
        QMessageBox::warning(this,tr("warning"), tr("Format is not right!!"));
        emit(send_msg(2,tr("Format is not right!!")));
        return;
    }


    static bool once = false;
    if(!once) {
        once = true;
//        cmd_system("killall milltask");
//        cmd_system("halcmd stop");
//        cmd_system("halcmd unload all");

            usleep(1000*50);

    }

    unsigned char buf[8];

    if(initEpp(buf) == -1) {
        QMessageBox::warning(this,"error",tr("write fail errorcode -1!!"),QMessageBox::Ok);
        emit(send_msg(2,tr("write fail errorcode -1!!")));
    }

    if(do_what == 1) {

        unsigned char down[16];
        memset(down,0x00,sizeof(down));

        QString codeget = "";
        for(int i=0;i<8;i++) {
            down[i] = code[i];
        }
        codeget.sprintf("%02x%02x%02x%02x%02x%02x%02x%02x",
                        down[0],down[1],down[2],down[3],down[4],down[5],down[6],down[7]);

        emit(send_msg(3,QString("code:%1").arg(codeget)));
        emit(send_msg(3,QString("sysnum:%1").arg(ui->SysNum->text())));

        if(writeRealData(0x0020+8*15,down,8) == -1) {
            QMessageBox::warning(this,tr("warning"),tr("write fail errorcode -2!!"));
            emit(send_msg(2,tr("write fail errorcode -2!!")));
            return;
        }

        usleep(1000*100);

        unsigned char time[8];
        readTime(time);


        if(time[0] == 0xff &&time[1] == 0xff
          &&time[2] == 0xff &&time[3] == 0xff) {
            QMessageBox::warning(this,tr("warning"),tr("write fail errorcode -3!!"));
            return;
        } else {
            QMessageBox::information(this,tr("info"),tr("write success!!"));
            emit(send_msg(0,tr("write success!!")));
        }

        struct tm currenttime,endtime;
        readTimeFun(currenttime);
        currenttime.tm_year -= 1900;
        currenttime.tm_mon -= 1;

        unsigned char tmp[8];
        readRealData(0x0020+8*11,tmp,8);
        endtime.tm_year = charToInt2(tmp) - 1900;
        endtime.tm_mon = tmp[2] - 1;
        endtime.tm_mday = tmp[3];
        endtime.tm_hour = 12;
        endtime.tm_min = 0;
        endtime.tm_sec = 0;

        if( (kernel_mktime(&endtime)-kernel_mktime(&currenttime)) >300 ) {
            long hours = (kernel_mktime(&endtime)-kernel_mktime(&currenttime))/300;
            tmp[0] = hours>>24;
            tmp[1] = (hours&0x00ffffff)>>16;
            tmp[2] = (hours&0x0000ffff)>>8;
            tmp[3] = (hours&0x000000ff)>>0;
            writeRealData(0x0020+8*14,tmp,4);

            tmp[0] = 0;
            tmp[1] = 0;
            tmp[2] = 0;
            tmp[3] = 0;
            writeRealData(0x0020+8*17+1,tmp,4);

            readTimeFun(currenttime);
            intToChar2(currenttime.tm_year,tmp);
            tmp[2] = currenttime.tm_mon;
            tmp[3] = currenttime.tm_mday ;
            tmp[4] = currenttime.tm_hour ;
            tmp[5] = currenttime.tm_min ;
            tmp[6] = currenttime.tm_sec ;
            writeRealData(0x0020+8*12,tmp,8);

        } else {

            tmp[0] = 1;
            tmp[1] = 0;
            tmp[2] = 0;
            tmp[3] = 0;
            writeRealData(0x0020+8*17+1,tmp,4);

        }


    } else if( do_what == 2) {


        unsigned char buf1[8];
        unsigned char buf2[2];
        unsigned char key_buf[8];

        for(int i=0; i<8; i++)
            key_buf[i] = emcStatus->motion.checkandsys[i];
        key_buf[6] = 4;
        key_buf[7] = 4;

        des_context ctx;//用于解密用的密钥结构体
        des_set_key(&ctx,key_buf);
        des_decrypt(&ctx,code_2,buf1);
        calculateCrc16(buf1,buf2,6);

        if( buf1[6] == buf2[0] &&  buf1[7] == buf2[1]  ) {
            writeRealData(0x0020+8*19,code_2,8);
            QMessageBox::information(this,tr("info"),tr("write success!!"));
            emit(send_msg(0,tr("write success!!")));
        } else {
            QMessageBox::warning(this,tr("warning"),tr("write fail errorcode -3!!"));
        }


    }



}

void Decrypt_Dialog::init() {

    ui->SysManuFrame->hide();
    ui->SysManuStateF->hide();
    adjustSize();

}

void Decrypt_Dialog::on_Btn_More_clicked()
{


    if(ui->SysManuFrame->isHidden()) {
        ui->SysManuFrame->show();
        ui->SysManuStateF->show();
    } else {
        ui->SysManuFrame->hide();
        ui->SysManuStateF->hide();
    }
    adjustSize();

}

void Decrypt_Dialog::showEvent(QShowEvent *){

    if( authorize_timeout() ) {
        ui->SysManuFrame->show();
        ui->SysManuStateF->show();

        if(get_time_left() >= 0 && get_time_left() <= 60){

        }else
            ui->Code1_2->setFocus();
    }

}

