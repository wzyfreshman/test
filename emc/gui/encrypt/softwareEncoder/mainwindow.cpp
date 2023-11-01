#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QMessageBox>

#include "des.h"
#include "udf.h"
#include "epp.h"
#include "hash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <errno.h>
#include   <sys/socket.h>
#include   <netinet/in.h>
#include   <net/if.h>
#include  <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>        //for struct ifreq

#include "hexvalidator.h"
extern "C" {
#include "lkt5103.h"
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->Generate,SIGNAL(clicked()),this,SLOT(slotReadAll()));
    connect(ui->WriteDown,SIGNAL(clicked()),this,SLOT(slotWriteDown()));

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slotTest()));
    connect(ui->customers,SIGNAL(activated(int)),this,SLOT(slotCustomerIndex(int)));
    connect(ui->itemCode2,SIGNAL(textChanged(QString)),this,SLOT(slotEditItemCode(QString)));
    //ui->pushButton->hide();
//    if(initEpp(cpuid) == -1) {

//        QMessageBox::warning(this,"error","Epp Init Error",QMessageBox::Ok);

//    }
    customerindex = 0;
    QStringList customerlist;
    for(int i =0;i<__CUSTOMER_COUNT;i++)
        customerlist.append(__ALL_CUSTOMERS[i].name);
    ui->customers->addItems(customerlist);

    ui->itemCode1->setValidator(new HexValidator);
    ui->itemCode2->setValidator(new HexValidator);
    ui->itemCode3->setValidator(new HexValidator);

    unsigned char sysbuf[8];
    readRealData(0x0020+16*8,sysbuf,8);

    for( int i=0; i<8; i++)
        sysnum[i] = sysbuf[i];

    QString sptf;

//    sysnum[0] = __ALL_CUSTOMERS[0].itemprefix[0];
//    sysnum[1] = __ALL_CUSTOMERS[0].itemprefix[1];

    sptf.sprintf("%02x%02x",sysnum[0],
            sysnum[1]);
    ui->itemCode1->setText(sptf);


    sptf.sprintf("%02x%02x%02x%02x",sysnum[2],
            sysnum[3],sysnum[4],sysnum[5]);
    ui->itemCode2->setText(sptf);


    sysnum[6] = 0;
    sysnum[7] = 0;
    ui->itemCode3->setText("0000");

    QString code,tmp;
    int n;
    for(int i =0;i<8;i++) {
        n = sysnum[i];
        tmp = tmp.sprintf("%02x",n).toUpper();
        code+=tmp;
    }
    ui->lineEdit_Mac->setText(code);


    slotReadAll(false);

}

void MainWindow::slotEditItemCode(QString str) {

    if(str.size() !=8) return;
    QString local = str;
    for(int i = 0;i<local.size()/2;i++) {
        int tt;bool ok;
        QString strtmp;
        tt = i*2;
        strtmp = local.mid(tt,2);
        tt = strtmp.toInt(&ok,16);
        sysnum[i+2] = tt;
    }

    QString code,tmp;
    int n;
    for(int i =0;i<8;i++) {
        n = sysnum[i];
        tmp = tmp.sprintf("%02x",n).toUpper();
        code+=tmp;
    }
    ui->lineEdit_Mac->setText(code);

}

void MainWindow::slotCustomerIndex(int index) {
    customerindex = index;
    QString sptf;
    sptf.sprintf("%02x%02x",__ALL_CUSTOMERS[index].itemprefix[0],
            __ALL_CUSTOMERS[index].itemprefix[1]);
    ui->itemCode1->setText(sptf);
    sysnum[0] = __ALL_CUSTOMERS[index].itemprefix[0];
    sysnum[1] = __ALL_CUSTOMERS[index].itemprefix[1];

    printAllInfo();
    slotReadAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}


static unsigned char final[75+15];

void MainWindow::slotTest() {


    unsigned char ver[4] = {0x7d,0x52,0xbf,0xa1};
    unsigned char sys[4]= {0x00,0x00,0x00,0x01};
    unsigned char code[18] = {0x55,0xb3,0xd3,0x81,0x1e,0x5e,0xe4,0x9d};

    unsigned char test_code[18] = {0xe9,0xb3,0xd3,0x81,0x1e,0x5e,0xe4,0x9d};
    unsigned char readdata[8];

    writeRealData(0x0020,test_code,8);
    readRealData(0x0020,readdata,8);

    QString str;
    str.sprintf("%02x %02x %02x %02x %02x %02x %02x %02x",readdata[0],
            readdata[1],readdata[2],readdata[3],readdata[4],readdata[5],readdata[6],readdata[7]);
    qDebug()<<str;
    return;

    unsigned char down[30];
    memset(down,0x00,sizeof(down));

    for(int i=0;i<8;i++)
        down[i] = code[i];
    for(int i=0;i<4;i++)
        down[8+i] = ver[i];
    for(int i=0;i<4;i++)
        down[12+i] = sys[i];

    if(writeEncryptInfo(down,0x0080,16,2) == -1) {
        QMessageBox::information(this,"info","write fail -1!!");
        return;
    }

    usleep(1000*100);

    unsigned char time[8];
    readTime(time);

    qDebug()<<"read time";
    qDebug()<<time[0]<<time[1]<<time[2]<<time[3];
    qDebug()<<time[4]<<time[5]<<time[6]<<time[7];

    if(time[0] == 0xff &&time[1] == 0xff
      &&time[2] == 0xff &&time[3] == 0xff) {
         QMessageBox::information(this,"info","write fail -2!!");
    } else
         QMessageBox::information(this,"info","write success!!");


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

void MainWindow::slotWriteDown() {

    unsigned char time[8];
    unsigned char down[16];

    if(ui->itemCode2->text().size() != 8) {
        QMessageBox::information(this,"info","please input the item code");
        return;
    }

    if(writeRealData(0x0020,final,88)  < 0 )  {
        QMessageBox::information(this,"info","write fail!! errorcode -1");
        return;
    }
    //*generate forevertime
    unsigned char random;
    unsigned char out[8];
    unsigned char infotmp[4];
    unsigned char encryinfo[8];
    unsigned char sys[4] = {0x01,0x02,0x03,0xef};
    unsigned char ver[4] = {0x01,0x02,0x08,0xef};
    random = qrand()%255;
    setVerNum(ver);
    setSysNum(sys);
    for(int i = 0;i<4;i++)
        infotmp[i] = 0xff;
    setInfoNum(infotmp);
    infoEncryPt(encryinfo,random);
    des_context ctx;
    des_set_key(&ctx,__ALL_CUSTOMERS[customerindex].id);
    des_encrypt(&ctx,encryinfo,out);
   for(int i=0;i<8;i++)
       down[i] = out[i];
   for(int i=0;i<4;i++)
       down[i+8] = ver[i];
   for(int i=0;i<4;i++)
       down[i+12] = sys[i];
  //***end generate foevertime

    usleep(1000*50);

    if(writeRealData(0x0020+8*15,down,16) < 0 )  {
        QMessageBox::warning(this,tr("warning"),tr("write fail errorcode -2!!"));
        //return;
    }
    readTime(time);
    if(time[2] == 0xff &&time[3] == 0xff
      &&time[4] == 0xff &&time[5] == 0xff)  {
        QMessageBox::warning(this,tr("warning"),tr("write fail errorcode -3!!"));
        //return;
    }
    writeRealData(0x0020+8*16,sysnum,8);

    unsigned char timedata[8];
    //clear mode
    timedata[0] = 0;
    timedata[1] = 0;
    timedata[2] = 0;
    timedata[3] = 0;
    writeRealData(0x0020+8*17,timedata,8);

    //flush time
    struct tm tm;
    readTimeFun(tm);
    intToChar2(tm.tm_year,timedata);
    timedata[2] = tm.tm_mon;
    timedata[3] = tm.tm_mday;
    timedata[4] = tm.tm_hour;
    timedata[5] = tm.tm_min;
    writeRealData(0x0020+8*12,timedata,8);

    //writed time
    readTimeFun(tm);
    intToChar2(tm.tm_year,timedata);
    timedata[2] = tm.tm_mon;
    timedata[3] = tm.tm_mday;
    timedata[4] = tm.tm_hour;
    timedata[5] = tm.tm_min;
    writeRealData(0x0020+8*13,timedata,8);

    //end date
    readTimeFun(tm);
    intToChar2(tm.tm_year,timedata);
    timedata[2] = tm.tm_mon;
    timedata[3] = tm.tm_mday;
    timedata[4] = 12;
    timedata[5] = 0;
    writeRealData(0x0020+8*11,timedata,8);

    timedata[0]  = 0;
    timedata[1]  = 1;
    timedata[2]  = 1;
    timedata[3]  = 1;
    writeRealData(0x0020+8*14,timedata,8);


    timedata[0]  = 0xff;
    timedata[1]  = 0xff;
    timedata[2]  = 12;
    timedata[3]  = 01;
    writeRealData(0x0020+8*19,timedata,4);

    QMessageBox::information(this,tr("info"),tr("write success!!"));


}

void MainWindow::slotReadAll(bool tip) {

    //QString str,tmp;
    if(readAllInfo()) {

        QString code;
        QString tmp;
        int n;

        for(int i =0;i<32;i++) {
            n = final[i];
            tmp = tmp.sprintf("%02x",n).toUpper();
            code+=tmp;
        }
        ui->lineEditH->setText(code);

        code.clear();
        for(int i =32;i<64;i++) {

            n = final[i];
            tmp = tmp.sprintf("%02x",n).toUpper();
            code+=tmp;
        }
        ui->lineEditL->setText(code);


        code.clear();
        for(int i =0;i<8;i++) {
            n = chipid[i];
            tmp = tmp.sprintf("%02x",n).toUpper();
            code+=tmp;
        }
        ui->lineEdit_ChIP->setText(code);

        code.clear();
        for(int i =0;i<8;i++) {
            n = hdid[i];
            tmp = tmp.sprintf("%02x",n).toUpper();
            code+=tmp;
        }
        ui->lineEdit_HD->setText(code);


        code.clear();
        for(int i =0;i<8;i++) {
            n = cpuid[i];
            tmp = tmp.sprintf("%02x",n).toUpper();
            code+=tmp;
        }
        ui->lineEdit_FPGA->setText(code);


        code.clear();
        for(int i =0;i<8;i++) {
            n = sysnum[i];
            tmp = tmp.sprintf("%02x",n).toUpper();
            code+=tmp;
        }
        ui->lineEdit_Mac->setText(code);

        if( tip )
            QMessageBox::information(this,"info","generate success!!");

    } else {

        if( tip )
            QMessageBox::warning(this,"warning", tr("Read hardware infomation fail!!"));

    }

}

bool MainWindow::readAllInfo() {

    unsigned char allhd[20];
    unsigned char myxor=0x55,myadd=0;

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    printf("Now I get IN here\n");

    struct hd_driveid id;
    int  fd = open("/dev/sda", O_RDONLY|O_NONBLOCK);

    if (fd < 0) {
        qDebug()<<"read"<<"/dev/sda"<<"error!!";
        return false;
    } else {

        if(!ioctl(fd, HDIO_GET_IDENTITY, &id)) {
            for(int i=0;i<20;i++) {
                allhd[i] = id.serial_no[i];
                }
        } else {
            return false;
        }

    }

    calculateCrc64(allhd,hdid,20);
    LKT4200_reset(cpuid);
    readChipId(chipid);
    //readRealData(0x0020+18*8,chipid,8);  //no need
    printAllInfo();
    generate();

    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    for(int i = 40; i<64; i++)
        final[i] = qrand()%255;

    for(int i = 0; i<8; i++)
        final[64+i] = hdid[i];

    for(int i = 0; i<8; i++)  //customer id
        final[72+i] = __ALL_CUSTOMERS[customerindex].id[i];

    //intToChar4(180,&final[80]);
    for(int i = 0; i<4; i++)  //left time
        final[80+i] = 0xFF;

    for(int i = 0; i<8; i++) {
        myxor ^= chipid[i];
        myadd += chipid[i];
    }
    //ghf-2014/9/15
    final[84] = myxor;
    final[85] = myadd;

    for(int i = 6; i<8; i++)
        final[80+i] = 0xFF;


    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    for(int i = 40;i<64;i++)
        final[i] = qrand()%255;


    return true;


}



void MainWindow::generate() {


    unsigned char cpu[8];
    unsigned char hd[8];
    unsigned char chip[8];
    unsigned char key[16];



    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    for(int i = 0;i<16;i++)
        key[i] = qrand()%255;


    des3_context ctx;
    des3_set_2keys(&ctx,key,&key[8]);
    des3_encrypt(&ctx,hdid,hd);
    des3_encrypt(&ctx,cpuid,cpu);
    des3_encrypt(&ctx,chipid,chip);

    for(int i = 0;i<8;i++) {
        hd[i] ^= cpu[i];
        chip[i] ^= cpu[i];
    }


    for(int i = 0;i<8;i++) {

        final[5*i] = cpu[i];
        final[5*i+1] = hd[i];
        final[5*i+2] = chip[i];
        final[5*i+3] = key[i*2];
        final[5*i+4] = key[i*2+1];

    }


    decrypt();


}

void MainWindow::printAllInfo() {

    printf("cpuid  ");
    for(int i = 0;i<8;i++) {
        //cpuid[i] = qrand()%255;
        printf("%x ",cpuid[i]);
    }
    printf("\n");


    printf("sysnum  ");
    for(int i = 0;i<8;i++) {
        //cpuid[i] = qrand()%255;
        printf("%x ",sysnum[i]);
    }
    printf("\n");


    printf("hdid  ");
    for(int i = 9;i<9+8;i++) {
        //hdid[i-9] = qrand()%255;

        printf("%x ",hdid[i-9]);
    }
    printf("\n");


    printf("chipid  ");
    for(int i = 21;i<21+8;i++) {
        //chipid[i-21] = qrand()%255;
        printf("%x ",chipid[i-21]);
    }
    printf("\n");

}

void MainWindow::decrypt() {


    unsigned char cpu[8];
    unsigned char hd[8];
    unsigned char chip[8];
    unsigned char key[16];


    for(int i = 0;i<8;i++) {

        cpu[i] = final[5*i];
        hd[i]= final[5*i+1];
        chip[i] = final[5*i+2];
        key[i*2] = final[5*i+3];
        key[i*2+1] = final[5*i+4];

    }

    for(int i = 0;i<8;i++) {
        hd[i] ^= cpu[i];
        chip[i] ^= cpu[i];
    }


    des3_context ctx;
    des3_set_2keys(&ctx,key,&key[8]);
    des3_decrypt(&ctx,hd,hdid);
    des3_decrypt(&ctx,cpu,cpuid);
    des3_decrypt(&ctx,chip,chipid);



}

void MainWindow::timerEvent(QTimerEvent *) {

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    for(int i = 0;i<64;i++)
        final[i] = qrand()%255;

    static int count = 0;
    static int errorcount0 = 0;
    static int errorcount1 = 0;

    int ret = writeEncryptInfo(final,0x0020,64,8);

    if(ret == -1) {
        errorcount0 ++;
    } else if(ret == -2) {
        errorcount1 ++;
    }

    count ++;

    QString str;
    str.sprintf("All read/write times = %d  sys-error-times = %d  mcuself-error-times = %d",count,errorcount0,errorcount1);
    ui->label->setText(str);

}
