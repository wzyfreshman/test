#include "unit_sharehal.h"
#include <QtDebug>

#include <stdio.h>
#include "extern.h"



unit_sharehal::unit_sharehal()
{
    initHalData();
}

void unit_sharehal::initHalData()
{

    QString modulename = "ReadHal";
    comp_id = hal_init(modulename.toLocal8Bit().data());

    int errorid = rtapi_shmem_new(22222,comp_id,sizeof(version_struct));
    rtapi_shmem_getptr(errorid,(void **)&versiondata);

    comp_temp_id = comp_id;
    if (comp_id < 0) {
        return ;
    }
    hal_ready(comp_id);
    atexit(exitHal);
    getHalData();


}

void unit_sharehal::getHalData()
{
    int next, row;
    hal_pin_t *pin;
    hal_sig_t *sig;
    hal_param_t *param;
    char *name;

    list[0].clear();
    list[1].clear();
    list[2].clear();
    rtapi_mutex_get(&(hal_data->mutex));
    next = hal_data->pin_list_ptr;
    row = 0;
    while (next != 0) {
        pin = (hal_pin_t *)SHMPTR(next);
        name = pin->name;
        list[0].append(QString("%1").arg(name));
        next = pin->next_ptr;
        Pin[row] = pin;
        row++;
    }

    next = hal_data->sig_list_ptr;
    row = 0;
    while (next != 0) {
        sig = (hal_sig_t *)SHMPTR(next);
        name = sig->name;
        list[1].append(QString("%1").arg(name));
        next = sig->next_ptr;
        Sig[row] = sig;
        row++;
    }

    next = hal_data->param_list_ptr;
    row = 0;
    while (next != 0) {
        param = (hal_param_t *)SHMPTR(next);
        name = param->name;
        list[2].append(QString("%1").arg(name));
        next = param->next_ptr;
        Par[row] = param;
        row++;
    }

    rtapi_mutex_give(&(hal_data->mutex));
}

double unit_sharehal::flushData(QString _name,int _type)
{
    double val = 0;
    hal_sig_t *sig;

    rtapi_mutex_get(&(hal_data->mutex));

    if(_type == 0) {
        for(int i=0;i<list[0].count();i++) {
            if(_name.compare(QString("%1").arg(Pin[i]->name)) == 0) {
                if (Pin[i]->signal == 0) {
                    val = data_value(Pin[i]->type, &(Pin[i]->dummysig));
                    break;
                } else  {
                    sig = (hal_sig_t *)SHMPTR(Pin[i]->signal);
                    val = data_value(Pin[i]->type, SHMPTR(sig->data_ptr));
                    break;
                }
            }
        }
    } else if(_type == 1) {
        for(int i=0;i<list[1].count();i++) {
            if(_name.compare(QString("%1").arg(Sig[i]->name)) == 0) {
                val = data_value(Sig[i]->type, SHMPTR(Sig[i]->data_ptr));
                break;
            }
        }
    } else if(_type ==2) {
        for(int i=0;i<list[2].count();i++) {
            if(_name.compare(QString("%1").arg(Par[i]->name)) == 0) {
                val = data_value(Par[i]->type, SHMPTR(Par[i]->data_ptr));
                break;
            }
        }
    }
    rtapi_mutex_give(&(hal_data->mutex));
//    qDebug()<<"Get pinname"<<_name<<val;
    return val;
}

int unit_sharehal::writeData(QString _name, int _type,double data)
{
    double val = 0;
    hal_sig_t *sig;

    rtapi_mutex_get(&(hal_data->mutex));

    if(_type == 0) {
        for(int i=0;i<list[0].count();i++) {
            if(_name.compare(QString("%1").arg(Pin[i]->name)) == 0) {
                if (Pin[i]->signal == 0) {
                    val = setdata_value(Pin[i]->type, &(Pin[i]->dummysig),data);
//                    qDebug()<<"Now setnosig "<<_name<<data;
                    break;
                } else  {
                    sig = (hal_sig_t *)SHMPTR(Pin[i]->signal);
                    val = setdata_value(Pin[i]->type, SHMPTR(sig->data_ptr),data);
//                    qDebug()<<"Now setp "<<_name<<data;
                    break;
                }
            }
        }
    } else if(_type == 1) {
        for(int i=0;i<list[1].count();i++) {
            if(_name.compare(QString("%1").arg(Sig[i]->name)) == 0) {
                val = setdata_value(Sig[i]->type, SHMPTR(Sig[i]->data_ptr),data);
                break;
            }
        }
    } else if(_type ==2) {
        for(int i=0;i<list[2].count();i++) {
            if(_name.compare(QString("%1").arg(Par[i]->name)) == 0) {
                val = setdata_value(Par[i]->type, SHMPTR(Par[i]->data_ptr),data);
                break;
            }
        }
    }
    rtapi_mutex_give(&(hal_data->mutex));
    return val;
}


double unit_sharehal::data_value(int type, void *valptr)
{
     double value;
     int tempval=0;

    switch (type) {
    case HAL_BIT:
    if (*((char *) valptr) == 0)
        value = 0;
    else
        value = 1;
    break;
    case HAL_FLOAT:
    value =  (double)*((hal_float_t *) valptr);
    break;
    case HAL_S32:
    tempval = (long)*((hal_s32_t *) valptr);
    value  = (double)tempval;
    break;
    case HAL_U32:
    tempval = (unsigned long)*((hal_u32_t *) valptr);
    value  = (double)tempval;
    break;
    default:
    value = 0;
    }
    return value;
}

double unit_sharehal::setdata_value(int type, void *valptr, double data)
{

   switch (type) {
   case HAL_BIT:
       if(data == 0)
           *((char *) valptr) = 0;
       else
           *((char *) valptr) = 1;
   break;
   case HAL_FLOAT:
        *((hal_float_t *) valptr) = data;
   break;
   case HAL_S32:
       *((hal_s32_t *) valptr) = (long)data;
   break;
   case HAL_U32:
       *((hal_u32_t *) valptr) = (unsigned long)data;
   break;
   default:
       break;
   }
   return 0;
}

void exitHal()
{
    hal_exit(comp_temp_id);
}

