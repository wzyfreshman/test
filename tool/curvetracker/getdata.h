#ifndef GETDATA_H
#define GETDATA_H
#include "hal.h"

class info {
    hal_float_t *data1;
    hal_float_t *datatime;
};

class getData
{
public:
    getData();
private:
    int compID;
    info *data;
};

#endif // GETDATA_H
