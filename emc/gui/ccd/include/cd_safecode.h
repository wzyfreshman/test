#ifndef CD_SAFECODE_H
#define CD_SAFECODE_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <QDebug>

#include "ccd_setting.h"

class CCD_API CD_SafeCode {

public:
    static void init(int &data);
    static void chk(int &value,int fun,char *msg);
    static void excute(int len, unsigned char* send_buf, int *ret,unsigned char *ret_buf);
    static void TRANSLATE2(double o_x,double o_y,double t,\
                             double x0,double y0,double &d_x,double &d_y);
    static void TRANSLATE1(double o_x,double o_y,double t,\
                           double x0,double y0,double &d_x,double &d_y);
    static void close();

};


#endif // CD_SAFECODE_H
