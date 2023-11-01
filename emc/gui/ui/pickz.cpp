#include "pickz.h"
#include "ui_pickz.h"

#include "shcom.hh"
#include "extern.h"

#include <unistd.h>
#include <QtDebug>

PickZ::PickZ(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PickZ)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Select the Active Z axis"));
    connect(ui->btn_ensure,SIGNAL(released()),this,SLOT(ensure()));

    if(_doublez - 5 == 0) {
        ui->Z4Active->setVisible(false);
    }
}

void PickZ::ensure() {

    _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",ui->Z1Active->isChecked());
    _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",ui->Z2Active->isChecked());
    _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",ui->Z3Active->isChecked());
    _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",ui->Z4Active->isChecked());

    int val = 0;


    if(ui->Z1Active->isChecked())
        val += 4;

    if(ui->Z2Active->isChecked())
        val += 8;

    if(ui->Z3Active->isChecked())
        val += 16;

    if(ui->Z4Active->isChecked())
        val += 32;

//    usleep(100*1000);
//    sendParaFlush();
//    usleep(50*1000);
    if(0 != ensure_mode(EMC_TASK_MODE_MDI))
        return;
    QString cmd = QString("M90P%1").arg(val);
//    qDebug()<<"1221212212==="<<cmd;
    sendMdiCmd(cmd.toLatin1().data(),1);


//     ensure_mode(EMC_TASK_MODE_AUTO);
//    QString goOrigin = _subDir;
//    goOrigin.append("goZero.ngc");
//    QFile file(goOrigin);
//         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//             return;
//     QTextStream out(&file);
//     out << "M90P"<< val << "\n";
//     file.close();

//     int count = 1;
//     sendProgramOpen(goOrigin.toLocal8Bit().data());
//     while(0 != strcmp(goOrigin.toLocal8Bit().data(),emcStatus->task.file)) {
//         sendProgramOpen(goOrigin.toLocal8Bit().data());
//         count++;
//         usleep(1000*40);
//         updateStatus();
//         if(count > 5) {
//             ensure_mode(EMC_TASK_MODE_MANUAL);
//              this->close();
//             return ;
//         }
//     }
//    sendProgramRun(-1,0);
    //show_msg(0,tr("Switch to Z PICK"));

    this->close();
}

void PickZ::showEvent(QShowEvent *) {
    qDebug()<<"now which z"<<emcStatus->motion.set_which_z;
    if((emcStatus->motion.set_which_z & (0x01 << 2)) > 0) {
        z1active = 1;
        _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",1);
    } else {
        z1active = 0;
        _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",0);
    }
    if((emcStatus->motion.set_which_z & (0x01 << 3)) > 0) {
        z2active = 1;
        _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",1);
    } else {
        z2active = 0;
        _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",0);
    }
    if((emcStatus->motion.set_which_z & (0x01 << 4)) > 0) {
        z3active = 1;
        _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",1);
    } else {
        z3active = 0;
        _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",0);
    }
    if((emcStatus->motion.set_which_z & (0x01 << 5)) > 0) {
        z4active = 1;
        _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",1);
    } else {
        z4active = 0;
        _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",0);
    }

    z1active > 0?ui->Z1Active->setChecked(true):ui->Z1Active->setChecked(false);
    z2active > 0?ui->Z2Active->setChecked(true):ui->Z2Active->setChecked(false);
    z3active > 0?ui->Z3Active->setChecked(true):ui->Z3Active->setChecked(false);
    z4active > 0?ui->Z4Active->setChecked(true):ui->Z4Active->setChecked(false);
}

PickZ::~PickZ()
{
    delete ui;
}
