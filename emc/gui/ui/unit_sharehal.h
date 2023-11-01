#ifndef UNIT_SHAREHAL_H
#define UNIT_SHAREHAL_H

#include <QObject>


#include "config.h"
#include "rtapi.h"
#include "hal.h"
#include "hal_priv.h"

#include <QStringList>
#include <QObject>
#define MAXNUM 10000

static int comp_temp_id;
void exitHal(void);
class unit_sharehal : public QObject
{
public:
    unit_sharehal();
    void initHalData();
    void getHalData();
    double data_value(int type, void *valptr);
    double setdata_value(int type,void *valptr,double data);

public slots:
    double flushData(QString _name, int _type);
    int writeData(QString _name, int _type, double data);

private:
    int comp_id;
    QStringList list[3];
    hal_pin_t *Pin[MAXNUM];
    hal_sig_t *Sig[MAXNUM];
    hal_param_t *Par[MAXNUM];
};


#endif // UNIT_SHAREHAL_H
