#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/types.h>
//#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <QTime>
#include <QDebug>

#include "des.h"
#include "hash.h"

extern "C" {
#include "lkt5103.h"
}
#include "epp.h"


void init_epp(){

}


int LKT4200_reset(unsigned char rstinfo[8]){

    int LenOfRes;
    unsigned char send_cmd[9];
    unsigned char localread[16];

    send_cmd[0] = 0x01;
    send_cmd[1] = 0;
    send_cmd[2] = 0;
    sendLKcmd(send_cmd,3,localread,&LenOfRes) ;

    for( int i=0; i<8; i++)
        rstinfo[i] = localread[i];

    return 0;

}



int readChipId(unsigned char *chipid) {

    int LenOfRes;
    unsigned char send_cmd[9];
    unsigned char localread[16];

    send_cmd[0] = 0x04;
    send_cmd[1] = 0;
    send_cmd[2] = 0;
    sendLKcmd(send_cmd,3,localread,&LenOfRes) ;

    for( int i=0; i<8; i++) {
        chipid[i] = localread[i];
    }



    return 0;

}

int readTime(unsigned char *time) {

    int LenOfRes;
    unsigned char send_cmd[9];
    unsigned char localread[16];

    send_cmd[0] = 0x09;
    send_cmd[1] = 0;
    send_cmd[2] = 0;
    sendLKcmd(send_cmd,3,localread,&LenOfRes) ;

    for( int i=0; i<8; i++)
        time[i] = localread[i];

    return 0;

}

void readEncryptInfo(unsigned short addrin,unsigned char *store,  int count) {



}

int writeEncryptInfo(unsigned char *info,unsigned short addrin,
                     int size,int num) {

    return 0;

}


int initEpp(unsigned char buf[8]) {

    return 0;
}

int maintest()
{
    return 0;

}



void writeStart() {

}

void writeStop() {

}

static int  write8Bytedown(unsigned char *addrin,unsigned char *data) {

    int i,ret = 0;
    int LenOfRes;
    unsigned char send_cmd[16];
    unsigned char ret_cmd[16];
    send_cmd[0] = 0x05;
    send_cmd[1] = addrin[0];
    send_cmd[2] = addrin[1];

    for(  i = 0; i < 8; i++ )
        send_cmd[3 + i] = data[i];
    ret = sendLKcmd(send_cmd,11,ret_cmd,&LenOfRes) ;

    return ret;
}

static void read8Byteup(unsigned char *addrin,unsigned char *data) {

     int LenOfRes;
     unsigned char send_cmd[9];
     send_cmd[0] = 0x06;
     send_cmd[1] = addrin[0];
     send_cmd[2] = addrin[1];
     sendLKcmd(send_cmd,3,data,&LenOfRes) ;

//    int j=0;
//    for(j=0;j<10;j++) {
//        printf("%.2X",data[j]);
//    }
//    printf(" read8Byteup ret %d\n",LenOfRes);

}

/**
* just support 4bytes each time
*/
static void forwriteEncrypt(unsigned short addrin,unsigned char *data,
                                              unsigned char *newaddr,unsigned char *newdata ) {

    unsigned char key[8] = {'W','R','I','T','E','M','C','U'};

    int i;
    unsigned char *orgdata;
    unsigned char randnum;
    unsigned char randpos;
    unsigned char crc16[2];
    unsigned char addr[2];
    unsigned char room[8];
    unsigned char out[8];
    unsigned char posflag ;
    unsigned char forcrccal[8];

    orgdata = data;
    addr[0] = addrin>>8;
    addr[1] = (unsigned char)addrin;

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    randnum = qrand()%255;
    randpos = qrand()%255;

    newaddr[0] = randnum;
    newaddr[1] = randpos;

    //**crc_calculate
    for(i=0;i<4;i++)
        forcrccal[i] = orgdata[i];
    forcrccal[4] = addr[0];
    forcrccal[5] = addr[1];
    forcrccal[6] = randnum;
    forcrccal[7] = randpos;
    calculateCrc16(forcrccal,crc16,8);

    //*process
    for(i=0;i<4;i++)
        orgdata[i] ^= randnum;
    addr[0] ^= randnum;
    addr[1] ^= randnum;
    crc16[0] ^= randnum;
    crc16[1] ^= randnum;

    //*place in position
    posflag = randpos%6;

    if( posflag == 0 ) {
       for(i=0;i<4;i++)
           room[i] = orgdata[i];
       room[4] = crc16[0];
       room[5] = crc16[1];
       room[6] = addr[0];
       room[7] = addr[1];
    } else if (posflag == 1 ) {
        for(i=0;i<4;i++)
            room[i] = orgdata[i];
        room[6] = crc16[0];
        room[7] = crc16[1];
        room[4] = addr[0];
        room[5] = addr[1];
    } else if (posflag == 2 ) {
        for(i=2;i<6;i++)
            room[i] = orgdata[i-2];
        room[0] = crc16[0];
        room[1] = crc16[1];
        room[6] = addr[0];
        room[7] = addr[1];
    } else if (posflag == 3 ) {
        for(i=4;i<8;i++)
            room[i] = orgdata[i-4];
        room[0] = crc16[0];
        room[1] = crc16[1];
        room[2] = addr[0];
        room[3] = addr[1];
    } else if (posflag == 4 ) {
        for(i=2;i<6;i++)
            room[i] = orgdata[i-2];
        room[6] = crc16[0];
        room[7] = crc16[1];
        room[0] = addr[0];
        room[1] = addr[1];
    } else  {
        for(i=4;i<8;i++)
            room[i] = orgdata[i-4];
        room[2] = crc16[0];
        room[3] = crc16[1];
        room[0] = addr[0];
        room[1] = addr[1];
    }

    des_context ctx;
    des_set_key(&ctx,key);
    des_encrypt(&ctx,room,out);

    for(i=0;i<8;i++)
        newdata[i] = out[i];


}

static void forwriteDecrypt(unsigned char randnum,
                           unsigned char randpos,unsigned char *out)  {

    unsigned char key[8] = {'W','R','I','T','E','M','C','U'};
    int i;
    unsigned char orgdata[4];
    //unsigned char randnum;
    //unsigned char randpos;
    unsigned char crc16[2];
    unsigned char crc16_ver[2];
    unsigned char addr[2] = {0x22,0x23};

    unsigned char room[8];
    //unsigned char out[8];
    unsigned char posflag;

    unsigned char forcrccal[8];

    qDebug()<<"decrypt out"<<out[0]<<out[1]<<out[2]<<out[3]<<out[4]<<out[5]<<out[6]<<out[7];

    des_context ctx;
    des_set_key(&ctx,key);
    des_decrypt(&ctx,out,room);
    qDebug()<<"decrypt room"<<room[0]<<room[1]<<room[2]<<room[3]<<room[4]<<room[5]<<room[6]<<room[7];
    //*place in position
    posflag = randpos%6;

    if( posflag == 0 ) {
       for(i=0;i<4;i++)
           orgdata[i] = room[i];
      crc16[0] = room[4] ;
      crc16[1] =  room[5] ;
      addr[0]  = room[6] ;
      addr[1]  = room[7] ;
    } else if (posflag == 1 ) {
        for(i=0;i<4;i++)
              orgdata[i] = room[i];
        crc16[0] = room[6] ;
        crc16[1] = room[7] ;
        addr[0] = room[4]  ;
        addr[1] = room[5] ;
    } else if (posflag == 2 ) {
        for(i=2;i<6;i++)
            orgdata[i-2] = room[i];
         crc16[0] = room[0];
         crc16[1] = room[1];
         addr[0] = room[6];
         addr[1] = room[7];
    } else if (posflag == 3 ) {
        for(i=4;i<8;i++)
            orgdata[i-4] = room[i];
         crc16[0] = room[0] ;
         crc16[1] = room[1] ;
         addr[0] = room[2];
         addr[1] = room[3];
    } else if (posflag == 4 ) {
        for(i=2;i<6;i++)
            orgdata[i-2] = room[i];
        crc16[0] = room[6] ;
        crc16[1] = room[7] ;
        addr[0] = room[0] ;
        addr[1] = room[1] ;
    } else  {
        for(i=4;i<8;i++)
            orgdata[i-4] = room[i];
        crc16[0] = room[2] ;
        crc16[1] = room[3] ;
        addr[0] = room[0] ;
        addr[1] = room[1]  ;
    }

    //*process
    for(i=0;i<4;i++)
        orgdata[i] ^= randnum;
    addr[0] ^= randnum;
    addr[1] ^= randnum;
    crc16[0] ^= randnum;
    crc16[1] ^= randnum;

    //**crc_calculate
    for(i=0;i<4;i++)
        forcrccal[i] = orgdata[i];
    forcrccal[4] = addr[0];
    
    forcrccal[5] = addr[1];
    forcrccal[6] = randnum;
    forcrccal[7] = randpos;
    calculateCrc16(forcrccal,crc16_ver,8);

    printf("addr %d %d\n",addr[0],addr[1]);
    printf("data %d %d %d %d\n",orgdata[0],orgdata[1],orgdata[2],orgdata[3]);

}


/**
* just some code that should in the mcu
*/
static void forreadEncypt() {

    unsigned char key[8] = {'G','E','T','C','N','E','X','T'};
    unsigned char orgdata[4] = {0x01,0x02,0x03,0x04};
    unsigned char rand[4];
    unsigned char room[8];
    unsigned char out[8];

    int i;
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    for(i=0;i<4;i++)
        rand[i] = qrand()%255;
    for(i=0;i<4;i++)
        orgdata[i] ^=  rand[0];
    for(i=0;i<4;i++) {
        room[i*2] = orgdata[i];
        room[i*2+1] = rand[i];
    }
    des_context ctx;
    des_set_key(&ctx,key);
    des_encrypt(&ctx,room,out);

}

static void forreadDecypt(unsigned char in[],unsigned char out[]) {

    unsigned char key[8] = {'G','E','T','C','N','E','X','T'};
    unsigned char orgdata[4];
    unsigned char rand[4];
    unsigned char room[8];
    int i;

    des_context ctx;
    des_set_key(&ctx,key);
    des_decrypt(&ctx,in,room);

    //***

//    for( i=0;i<8;i++)
//        room[i] = in[i] ;

    /**/
    for(i=0;i<4;i++) {
       orgdata[i] = room[i*2] ;
       rand[i] =  room[i*2+1] ;
    }
    for(i=0;i<4;i++)
        orgdata[i] ^=  rand[i];

    for(i=0;i<4;i++)
        out[i]  =  orgdata[i];

}


/**
*should be 4*n bytes
*/

int writeRealData(unsigned short addrin,unsigned char *data,int size) {


        unsigned char tran[8];
        unsigned char newaddr[2];
        int index = 0,i,count;
        unsigned short addrorg = addrin;
        count = size/4;
        writeStart();
        for(i = 0;i<count;i++) {
            forwriteEncrypt(addrorg,data+index,newaddr,tran);
            index +=4;
            addrorg +=4;
            int ret =  write8Bytedown(newaddr,tran);

            if(ret == -1) return -1;

        }
        writeStop();
        return 0;

}

void readRealData(unsigned short addrin,unsigned char *data,int size) {

    unsigned char tran[16];
    unsigned char newaddr[2];
    int index = 0,i,count;
    unsigned short addrorg = addrin;

    count = size/4;
    for(i = 0;i<count;i++) {
        newaddr[0] = addrorg>>8;
        newaddr[1] = (unsigned char)addrorg;
        read8Byteup(newaddr,tran);
        forreadDecypt(tran,data+index);
        index +=4;
        addrorg +=4;
    }

}

int LK_ReadAddr(unsigned char addr,\
                unsigned char *ResBuf,\
                int *LenOfRes) {
    int ret,fd;
    int LenOfAtr;
    unsigned char ATR[0x40];
    int i=0,reslen,usr_len;
    unsigned char RSB[300];
    unsigned char usr_cmd[300];
    usr_cmd[0]=0x80;
    usr_cmd[1]=0x08;
    usr_cmd[2]=0x00;
    usr_cmd[3]=0x00;
    usr_cmd[4]=0x03;
    usr_cmd[5]=0x06;
    usr_cmd[6]=0x00;
    usr_cmd[7]=addr;

    usr_len = 8;
    printf("len=%d\n",usr_len);
    int j=0;
    for(j=0;j<8;j++) {
        printf("%.2X",usr_cmd[j]);
    }
    printf("END\n");

    ret = LKSC_Open("/dev/sg1",&fd);
    if(ret < 0) {
        printf("Cannot opne the LKSC dev\n");
        return -1;
    }

    ret = LKSC_Reset(fd,ATR,&LenOfAtr);

    ret = LKSC_SendAPDU(fd,usr_cmd,usr_len,RSB, &reslen);
    *LenOfRes = reslen;
    for (i=0;i<reslen;i++) {
        ResBuf[i] = RSB[i];
    }


    LKSC_Close(fd);
    return ret;
}

int sendLKcmd(unsigned char *CMD,\
              int LenOfCmd,\
              unsigned char *ResBuf,\
              int *LenOfRes) {
    int ret = 0,fd;
    int LenOfAtr = 0;
    unsigned char ATR[0x40];
    int i=0,j=0,reslen,usr_len;
    unsigned char RSB[300];
    unsigned char usr_cmd[300];
    char dev_name[300];
    usr_cmd[0]=0x80;
    usr_cmd[1]=0x08;
    usr_cmd[2]=0x00;
    usr_cmd[3]=0x00;
    usr_cmd[4]=LenOfCmd;

    for (i=0;i<LenOfCmd;i++) {
        usr_cmd[5+i] = CMD[i];
    }
    usr_len = LenOfCmd+5;


    i = 0;
    while(LenOfAtr != 17) {
        memset(dev_name,0,sizeof(dev_name));
        sprintf(dev_name,"/dev/sg%d",i);
        ret = LKSC_Open(dev_name,&fd);
        if(ret == 0)
            ret = LKSC_Reset(fd,ATR,&LenOfAtr);
   //     printf("Now I get lkt %s len:%d ret:%d\n",dev_name,LenOfAtr,ret);
        i++;

        if(LenOfAtr != 17)
            LKSC_Close(fd);
        if(i>3) {
   //         i=0;
    //        j++;
  //          if(j > 10) {
                printf("Cannot read the LKT Chip,please check the dog or try to reboot\n");
                return -1;
 //           }
        }
    }

    if(ATR[0] != 0X3B && ATR[1] != 0X6D && ATR[2] != 0X00 && ATR[3] != 0X00 ) {
        return -1;
    }

    if(LenOfAtr == 17) {
        ret = LKSC_SendAPDU(fd,usr_cmd,usr_len,RSB, &reslen);
        if(ret != 0)
            return -1;
        *LenOfRes = reslen;
        for (i=0;i<reslen;i++) {
            ResBuf[i] = RSB[i];
        }
    } else {
        printf("Cannot read the LKSC dev\n");
        return -1;
    }


    LKSC_Close(fd);
    return ret;
}
