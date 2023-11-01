#include "unit_serialport.h"
#include <stdio.h>   /* included for printf() */
#include <fcntl.h>   /* included for O_RDWR, O_NOCTTY and O_NDELAY */
#include <termios.h> /* included for functions and defines used in set_interface_attribs */
#include <unistd.h>  /* included for function write() */
#include <string.h>  /* included for memset */
#include <pthread.h>

#include <QtDebug>
#include <QThread>
#include <QDateTime>
#include <QString>

void *readport(void *parent) {
    unit_serialport *p = (unit_serialport *)parent;
    p->readData();
}


int unit_serialport::initSerialport()
{

    printf("opening device %s %d\n", setting->devname.toLocal8Bit().data(),setting->baudrate);
    if ( (fd = open(setting->devname.toLocal8Bit().data(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) < 0 )
    {
        printf("Open Serial Port failed.\n");
        emit sendMsg(3,"打开串口失败");
        return -1;
    }

    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
    {
      printf("error using tcgetattr\n");
      return -1;
    }

   cfsetospeed (&tty, setting->baudrate);
   cfsetispeed (&tty, setting->baudrate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    tty.c_iflag = IGNBRK;         // ignore break signal
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= 0;        //奇偶校验
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

//    tty.c_cflag &= ~HUPCL;
//    tty.c_iflag &= ~INPCK;
//    tty.c_iflag |= IGNBRK;
//    tty.c_iflag &= ~ICRNL;
//    tty.c_iflag &= ~IXON;
//    tty.c_lflag &= ~IEXTEN;
//    tty.c_lflag &= ~ECHOK;
//    tty.c_lflag &= ~ECHOCTL;
//    tty.c_lflag &= ~ECHOKE;
//    tty.c_oflag &= ~ONLCR;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
      printf("error from tcsetattr\n");
      emit sendMsg(3,"初始化串口失败");

      return -1;
    }
    return 0;
}

void unit_serialport::readData() {
    int errorcout = 0;
    memset(readBuffer,0,sizeof(readBuffer));
    do{
        usleep(50*1000);
        int readBytes = read(fd, readBuffer, receivesize);
        if (readBytes > 0) {
//            printf("Now I read in read data  xxx..%d....\n",readBytes);
            printf("Read %d:\n", readBytes);
            int i=0;
            for (i=0; i<receivesize; i++) {
                printf(" 0x%x", readBuffer[i]);
            }
            receiveok = 1;


            int readBytenum = read(fd, readBufferclr, receivesize);
            printf("\n readBytes=%d \n",readBytenum);
            break;
        } else if (readBytes == 0) {
            errorcout++;
            if(errorcout > 10) {
                receiveok = -1;
                break;
            }
        } else if (readBytes < 0) {
            printf("read data error  %d\n",readBytes);
            receiveok = -1;
            break;
        }
    } while (1);

    void *st;
    pthread_exit(st);
}

int unit_serialport::closePort()
{
    if(fd)
        close(fd);
}

int unit_serialport::SendModbus(int slaveno, int type, uint16 addr, int data)
{
    int ret = 0;
    uint8 cmd[8];
    memset(cmd,0,8);
    datach ch;
    ch.number = slaveno;
    cmd[0] = ch.ch;
    if(type == 0)
        cmd[1] = 0x03;
    else if(type == 1)
        cmd[1] = 0x06;

    cmd[3] = addr & 0xFF;
    cmd[2] = (addr>>8) & 0xFF;

    data2ch todata;
    todata.number = data;
    cmd[4] = todata.ch[1];
    cmd[5] = todata.ch[0];

    uint16 result = CRC16_Check(cmd,6);
    cmd[7] = result & 0xFF;
    cmd[6] = (result>>8) & 0xFF;

//    cmd[0] = 0x24;
//    cmd[1] = 0x01;
//    cmd[2] = 0x00;
//    cmd[3] = 0x11;
//    cmd[4] = 0x00;
//    cmd[5] = 0xc3;
//    cmd[6] = 0x0a;
//    cmd[7] = 0x94;

    for(int i=0;i<8;i++) {
        printf("%02X ",cmd[i]);
    }
//    printf("\n");

    receiveok = 0;
    ret = sendCMD(cmd,10);
    return ret;
}

int unit_serialport::sendCMD(unsigned char *MSG,int _receivesize)
{
    int res;
    pthread_t read_thread;
    void *status;

    receivesize = _receivesize;
    res = writeData(MSG);
    if (res != 0)
    {
        perror("Write data failed!");
        return -1;
    }
    res = pthread_create(&read_thread, NULL, readport, this);
    if (res != 0)
    {
        perror("Thread creation failed!");
        return -1;
    }

    pthread_join(read_thread,&status);
//    close(fd);
    return 0;
}

int unit_serialport::writeData(unsigned char *cmd)
{
    int writelen = sizeof(cmd);
    int i=0;
    char writeBuffer[128];
    memset(writeBuffer,0,sizeof(writeBuffer));
    for(i=0;i<writelen;i++) {
        writeBuffer[i] = cmd[i];
//        printf("%02x",writeBuffer[i]);
    }
    int writebytes = write(fd, writeBuffer, writelen);
    if(writebytes != writelen) {
        printf("error send bytes:%s \n",writeBuffer);
        return -1;
    }

    tcdrain(fd);

    return 0;
}


uint16 unit_serialport::CRC16_Check(uint8 *Pushdata,uint8 length)
{
  uint16 Reg_CRC=0xffff;
  uint8 Temp_reg=0x00;
  uint8 i,j;
 for( i = 0; i<length; i ++)
   {
   Reg_CRC^= *Pushdata++;
   for (j = 0; j<8; j++)
   {
   if (Reg_CRC & 0x0001)

   Reg_CRC=Reg_CRC>>1^0xA001;
else
Reg_CRC >>=1;
   }
   }
Temp_reg=Reg_CRC>>8;
return (Reg_CRC<<8|Temp_reg);
}

void unit_serialport::cycleExec()
{
}

int unit_serialport::readZeroPos(int axisnum)
{

    static int waitcount = 0;
    for(int i=0;i<axisnum;i++) {
        for(int j = 0;j<10;j++) {

            int ret = SendModbus(i+1,0,setting->zeroposAddr,2);       //新的


            while(receiveok == 0) {
                waitcount++;
                usleep(1000*100);
                if(waitcount > 5) {
                    emit sendMsg(3,"485通讯失败,获取零点失败");
                    waitcount = 0;
                    return -1;
                }
            };
            waitcount = 0;
            if(ret != 0 || receiveok == -1) {
                emit sendMsg(3,"485通讯失败,获取零点失败");
                receiveok = 0;
                return -1;
            }



            uint16 result = CRC16_Check(readBuffer,7);
            unsigned char crc2 = result & 0xFF;
            unsigned char crc1 = (result>>8) & 0xFF;

            if(readBuffer[0] == i+1 && readBuffer[1] == 0x03 && readBuffer[2] == 0x04 && crc1 == readBuffer[7] && crc2 == readBuffer[8]) {


                data2float zerop;
                zerop.ch[1] = readBuffer[3];
                zerop.ch[0] = readBuffer[4];
                zerop.ch[3] = readBuffer[5];
                zerop.ch[2] = readBuffer[6];
                printf(" CRC:%02X %02X %02X %02X read %02X %02X %02X %02X  %f  %d \n",crc1,crc2,readBuffer[7],readBuffer[8], readBuffer[3], readBuffer[4],readBuffer[5],readBuffer[6],zerop.value,zerop.intvalue);
                if(zerop.ch[0] == 0xff && zerop.ch[1] == 0xff)
                    return -1;
                if(zeropos[i] == zerop.intvalue) {
                     j = 10;
                }
                zeropos[i] = zerop.intvalue;
            }

        }
    }
    return 0;
}

void unit_serialport::setbrushDir(int dir)
{
    qDebug()<<"send brush dir"<<dir;
    if(dir > 0)
        dir = 16;

//    int ret = SendModbus(1,1,0x2005,dir);         测试先屏蔽四方的
//    int ret2 = SendModbus(2,1,0x2005,dir);
//    if(ret != 0 || ret2 != 0) {
//        qDebug()<<"Set brush direction failed!!";
//    }
    return;
}

void unit_serialport::checkAlarm()
{
//    if(par_get_var(3093) == 0) {
//        cnc->rightbrushalarm = 0;
//        cnc->leftbrushalarm = 0;
//        return;
//    }

//    int ret = SendModbus(1,0,cnc->alarmAddr,1);
//    if(ret != 0)
//        return;
//    data2ch torqueRvalue;
//    torqueRvalue.ch[1] = readBuffer[3];
//    torqueRvalue.ch[0] = readBuffer[4];
//    if(torqueRvalue.ch[0] == 0xff && torqueRvalue.ch[1] == 0xff)
//        return;

//    qDebug()<<"Left:";
//    for(int i=0;i<8;i++) {
//        qDebug()<<QString::number(this->readBuffer[i],16);
//    }

//    if((torqueRvalue.ch[1] & (0x01 << 1)) > 0 ) {
//        cnc->leftbrushalarm = 0;
//    } else {
//        cnc->leftbrushalarm = 1;
//    }


//    ret = SendModbus(2,0,cnc->alarmAddr,1);
//    if(ret != 0)
//        return;
//    torqueRvalue.ch[1] = readBuffer[3];
//    torqueRvalue.ch[0] = readBuffer[4];
//    if(torqueRvalue.ch[0] == 0xff && torqueRvalue.ch[1] == 0xff)
//        return;
////    qDebug()<<"Right:";
////    for(int i=0;i<8;i++) {
////        qDebug()<<QString::number(this->readBuffer[i],16);
////    }
//    if((torqueRvalue.ch[1] & (0x01 << 1)) > 0 ) {
//        cnc->rightbrushalarm = 0;
//    } else {
//        cnc->rightbrushalarm = 1;
//    }
}
