#ifndef UNIT_SERIALPORT_H
#define UNIT_SERIALPORT_H

#include "unit_rs485setting.h"
#include  <QObject>

typedef unsigned char   uint8;         /* Unsigned  8 bit quantity     */
typedef signed   char   int8;          /* Signed    8 bit quantity     */
typedef unsigned short  uint16;        /* Unsigned  16 bit quantity    */
typedef signed   short  int16;         /* Signed    16 bit quantity    */
typedef unsigned int    uint32;        /* Unsigned  32 bit quantity    */
typedef signed   int    int32;         /* Signed    32 bit quantity    */

union datach{
    int number;
    uint8 ch;
};
union data2ch{
    signed int number;
    uint8 ch[2];
    uint16 value;
};
union data2float{
    uint8 ch[4];
    uint32 intvalue;
    float value;
};

class unit_serialport : public QObject
{
    Q_OBJECT

public:

    unit_serialport(){
        setting = new unit_RS485Setting();
    }
    unsigned char cmddata[8];
    unsigned char readBuffer[128];
    unsigned char readBufferclr[128];
    int receivesize;
    int leftTorque = 0;
    int rightTorque = 0;
    int zeropos[3];
    unit_RS485Setting *setting;

    ~unit_serialport(){}
    int initSerialport();

    int sendCMD(unsigned char *cmd,int _receivesize);
    int writeData(unsigned char *cmd);
    void readData();
    int closePort();

    int SendModbus(int slaveno, int type, uint16 addr, int data);
    uint16 CRC16_Check(uint8 *Pushdata,uint8 length);

public slots:
    void cycleExec();
    int readZeroPos(int axisnum);
    void setbrushDir(int dir);
    void checkAlarm();

signals:
    void sendMsg(int,QString);

private:
    int fd;
    int receiveok;
};

#endif // UNIT_SERIALPORT_H
