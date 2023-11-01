#include "toolchanger.h"
#include "ui_toolchanger.h"
#include "extern.h"
#include "emcstatus.hh"
#include "shcom.hh"
#include "mywidget.h"
#include "extern.h"
#include<QInputDialog>
#include <QMessageBox>
#include <QtDebug>
#include <QTimer>
#include <QDir>
#include <QTest>

ToolChanger::ToolChanger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolChanger)
{
    ui->setupUi(this);
    toolnumber = _ini->read("PAR_RS274_TOOLNUMBER","VALUE","0").toInt();
    tableType = _ini->readEntry("PAR_RS274_TOOLTABLE_TYPE/VALUE",0).toInt();
    sendSetParameter(3811,toolnumber);
    if(_doublez > 0)
        toolnumber = toolnumber;
    for(int i=0;i<toolnumber;i++) {
        ui->toolchose->addItem(QString("%1").arg(i+1));
    }

    ui->toolchose->setCurrentIndex(0);
    connect(ui->btn_changetotool,SIGNAL(released()),this,SLOT(change_to_chosetool()));
    connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(setSpindleIO()));
    connect(ui->pushButton_4,SIGNAL(released()),this,SLOT(setToolChangerCover()));

    QTimer *flush_time = new QTimer(this);
    connect(flush_time,SIGNAL(timeout()),this,SLOT(ToolUpdate()));
    flush_time->start(20);

    ui->pushButton_2->hide();
    if(tableType > 0) {
        ui->move_b->hide();
        ui->move_c->hide();
        ui->btn_resetB->hide();
        ui->btn_resetC->hide();
        if(tableType == 2) {
            ui->btn_tool1->hide();
            ui->btn_tool2->hide();
            ui->btn_tool3->hide();
            ui->btn_tool4->hide();
            ui->btn_tool5->hide();
            ui->btn_tool6->hide();
            ui->btn_tool7->hide();
            ui->btn_tool8->hide();
        } if(tableType == 3) {
            ui->btn_tool2->setText("刀盘正转");
            ui->btn_tool3->setText("刀盘反转");
            ui->btn_tool1->setText("刀盘回零");
            ui->btn_tool4->setText("刀套垂直");
            ui->btn_tool5->setText("刀套水平");
            ui->btn_tool6->setText("ATC机械手");
            ui->btn_tool7->setText("机械手单步");
            ui->move_x->hide();
        }if(tableType == 4) {
            ui->btn_tool1->setText("推刀");
            ui->move_x->hide();
        }
    }

    ui->move_x->hide();
}


void ToolChanger::change_to_chosetool() {
    if(par_get_var(4127) > 0) {
        QMessageBox::information(this, "提示","换刀功能已经关闭","确定");
         return;
    }



    int ret = QMessageBox::information(this, tr("提示"),tr("确定更换到刀具 %1").arg(ui->toolchose->currentIndex()+1),tr("确定"),tr("取消"));
    if(ret == Ok) {
//        if((get_task_z() == 2) && (fabs(get_abs_pos(2) - get_abs_pos(3) - (emcStatus->task.g5x_offset.tran.z - emcStatus->task.g5x_offset.a)) > 0.001 ||
//                fabs((emcStatus->task.g5x_offset.tran.z - emcStatus->task.g5x_offset.a) - emcStatus->motion.doublez.zdiff) > 0.001)) {
//            sendDoubleZcmd(2,1,par_get_var(5203+par_get_var(5220)*20)-emcStatus->task.g5x_offset.a);
//            toolchanging = 1;
//        } else {
            changetoTool();
//        }

    } else if (ret == Cancel){
        return;
    } else {
        if(!PassWord::getCodecnc(this,1)) {
            return;
        }
        sendSetParameter(4300,ui->toolchose->currentIndex()+1);
        sendSetParameter(4301,ui->toolchose->currentIndex()+1);


        //双Z轴模式设置当前的刀具号
        if(get_task_z() == 0) {
            sendSetParameter(4288,ui->toolchose->currentIndex()+1);
            sendSetParameter(4283,ui->toolchose->currentIndex()+1);
        } else if(get_task_z() == 1) {
            sendSetParameter(4289,ui->toolchose->currentIndex()+1);
            sendSetParameter(4284,ui->toolchose->currentIndex()+1);
        } else if(get_task_z() == 2) {
            sendSetParameter(4288,ui->toolchose->currentIndex()+1);
            sendSetParameter(4283,ui->toolchose->currentIndex()+1);
            sendSetParameter(4289,ui->toolchose->currentIndex()+1);
            sendSetParameter(4284,ui->toolchose->currentIndex()+1);
        }
        return;
    }

}

void ToolChanger::setSpindleIO() {

    if(emcStatus->motion.synch_do[3] > 0) {
        sendSetDIO(3,0);
    } else
        sendSetDIO(3,1);
}
void ToolChanger::setToolChangerCover() {

    if(tableType != 2 &&  tableType != 3) {
        //刀库门打开安全位判断
        if( emcStatus->motion.synch_di[13] > 0 || emcStatus->motion.synch_di[14] > 0 ) {
                QMessageBox::information(this,tr("提示"),tr("主轴有刀不能打开刀库"),QMessageBox::Ok);
                    return;
        }
    }

    if(tableType == 2) {
        if((get_abs_pos(AXIS_Z1) < -1e-3 && get_task_z() == 0)) {
            QMessageBox::information(this,tr("提示"),tr("Z轴不在零点不能开关刀库门"),QMessageBox::Ok);
                return;
        }
    }

    QString filepath = "/home/.config/subNgc/openToolDoor.ngc";

    if(emcStatus->motion.mot_extern_out[1] > 0) {
        filepath = "/home/.config/subNgc/closeToolDoor.ngc";
        if(tableType != 3) {
            sendSetExternIO(1,0);
            sendSetExternIO(0,1);
        }
    } else {
        filepath = "/home/.config/subNgc/openToolDoor.ngc";
        if(tableType != 3) {
            sendSetExternIO(1,1);
            sendSetExternIO(0,0);
        }
    }
    QFile file(filepath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
      return;

    file.close();
    ensure_mode(EMC_TASK_MODE_AUTO);
    sendProgramOpen(filepath.toLocal8Bit().data());
    sendProgramRun(-1,0);
}

ToolChanger::~ToolChanger()
{
    delete ui;
}


void ToolChanger::ToolUpdate() {

    ui->OffsetAngle->setEnabled(unlockpar > 0);

    ui->btn_tool2->setEnabled(par_get_var(4281) > 0);
    ui->btn_tool3->setEnabled(par_get_var(4281) > 0);

    ui->chk_debug->setChecked(par_get_var(4281) > 0);

    if(emcStatus->motion.mot_extern_out[1] > 0) {
        if(!ui->pushButton_4->isChecked())
            ui->pushButton_4->setChecked(true);
    } else {
        if(ui->pushButton_4->isChecked())
            ui->pushButton_4->setChecked(false);
    }

    if(emcStatus->motion.mot_extern_out[2] > 0) {
        if(!ui->btn_tool1->isChecked())
            ui->btn_tool1->setChecked(true);

    } else {
        if(ui->btn_tool1->isChecked())
            ui->btn_tool1->setChecked(false);
    }

    if(emcStatus->motion.mot_extern_out[3] > 0) {
        if(!ui->btn_tool2->isChecked())
            ui->btn_tool2->setChecked(true);
    } else {
        if(ui->btn_tool2->isChecked())
            ui->btn_tool2->setChecked(false);
    }

    if(emcStatus->motion.mot_extern_out[4] > 0) {
        if(!ui->btn_tool3->isChecked())
            ui->btn_tool3->setChecked(true);
    } else {
        if(ui->btn_tool3->isChecked())
            ui->btn_tool3->setChecked(false);
    }

    if(emcStatus->motion.mot_extern_out[5] > 0) {
        if(!ui->btn_tool4->isChecked())
            ui->btn_tool4->setChecked(true);
    } else {
        if(ui->btn_tool4->isChecked())
            ui->btn_tool4->setChecked(false);
    }

    if(emcStatus->motion.mot_extern_out[6] > 0) {
        if(!ui->btn_tool5->isChecked())
            ui->btn_tool5->setChecked(true);
    } else {
        if(ui->btn_tool5->isChecked())
            ui->btn_tool5->setChecked(false);
    }

    if(emcStatus->motion.mot_extern_out[7] > 0) {
        if(!ui->btn_tool6->isChecked())
            ui->btn_tool6->setChecked(true);
    } else {
        if(ui->btn_tool6->isChecked())
            ui->btn_tool6->setChecked(false);
    }

    if(emcStatus->motion.mot_extern_out[8] > 0) {
        if(!ui->btn_tool7->isChecked())
            ui->btn_tool7->setChecked(true);
    } else {
        if(ui->btn_tool7->isChecked())
            ui->btn_tool7->setChecked(false);
    }

    if(emcStatus->motion.mot_extern_out[9] > 0) {
        if(!ui->btn_tool8->isChecked())
            ui->btn_tool8->setChecked(true);
    } else {
        if(ui->btn_tool8->isChecked())
            ui->btn_tool8->setChecked(false);
    }

    if(emcStatus->motion.synch_do[3] > 0) {
        if(!ui->pushButton_3->isChecked())
            ui->pushButton_3->setChecked(true);
    } else {
        if(ui->pushButton_3->isChecked())
            ui->pushButton_3->setChecked(false);
    }

    if(emcStatus->motion.synch_do[4] > 0) {
        if(!ui->spindle2_on->isChecked())
            ui->spindle2_on->setChecked(true);
    } else {
        if(ui->spindle2_on->isChecked())
            ui->spindle2_on->setChecked(false);
    }

    if(par_get_var(3815) != ui->OffsetAngle->value())
        ui->OffsetAngle->setValue(par_get_var(3815));

    if(moveATC > 0) {
            moveATC = 0;
            sendSetExternIO(7,0);
    }

    static int recordturnpos = 0;
    if(recordturnpos != emcStatus->motion.synch_di[25]) {
        if(recordturnpos == 1 && emcStatus->motion.synch_di[25] == 0) {
            sendSetExternIO(3,0);
        }
        recordturnpos = emcStatus->motion.synch_di[25];
    }

    static int recordturnneg = 0;
    if(recordturnneg != emcStatus->motion.synch_di[24]) {
        if(recordturnneg == 1 && emcStatus->motion.synch_di[24] == 0) {
            sendSetExternIO(4,0);
        }
        recordturnneg = emcStatus->motion.synch_di[24];
    }
}

void ToolChanger::changetoTool()
{

    sendSetParameter(4294,1);
    QString cmd = QString("T%1M6").arg(ui->toolchose->currentIndex()+1);
    QString filepath = "/home/.config/subNgc/ManualToolchange.ngc";
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

    QTextStream out(&file);
    out << cmd;
    file.close();
    ensure_mode(EMC_TASK_MODE_AUTO);
    sendProgramOpen(filepath.toLocal8Bit().data());
    sendProgramRun(-1,0);
    emit send_msg(1,QString(tr("当前切换到刀具%1")).arg(ui->toolchose->currentIndex()+1));
}

void ToolChanger::on_move_c_clicked()
{


    int toolnum = ui->toolchose->currentIndex()+1;
    double targetpos = par_get_var(4702+(toolnum)*5);
    if(toolnum > 19) {
        targetpos = par_get_var(4702+(toolnum)*5 + 20);
    }
    double incrementpos = targetpos - get_abs_pos(AXIS_B);
    sendJogIncr(5,3000,incrementpos);
}

void ToolChanger::on_btn_tool1_clicked()
{
    if(tableType != 3) {
        if(emcStatus->motion.mot_extern_out[2] > 0) {
            sendSetExternIO(2,0);
        } else
            sendSetExternIO(2,1);
    } else {
        send_msg(0,tr("刀盘回零"));
        if(EMC_TASK_MODE_AUTO==get_task_mode())
            return;
        if(0!=ensure_mode(EMC_TASK_MODE_AUTO)){
            ensure_mode(EMC_TASK_MODE_MANUAL);
            return;
        }
        QString goOrigin="/home/.config/subNgc/zeroTooldisk.ngc";
        sendProgramOpen(goOrigin.toLocal8Bit().data());
        sendProgramRun(-1,0);
    }

}

void ToolChanger::on_btn_tool2_clicked()
{
    sendSetAnalogyIO(1,0); //  设置选刀数为0一直输出
    if(emcStatus->motion.mot_extern_out[3] > 0) {
        sendSetExternIO(3,0);
        send_msg(0,tr("取消刀盘正转"));
    } else {
        if(emcStatus->motion.mot_extern_out[5] > 0 || emcStatus->motion.synch_di[27] == 0) {
            QMessageBox::information(this,tr("提示"),tr("刀套必须水平"),QMessageBox::Ok);
            return;
        }
        sendSetExternIO(4,0);
        QTest::qSleep(200);
        sendSetExternIO(3,1);

        send_msg(0,tr("刀盘正转"));
    }
}

void ToolChanger::on_btn_tool3_clicked()
{
    sendSetAnalogyIO(1,0); //  设置选刀数为0一直输出
    if(emcStatus->motion.mot_extern_out[4] > 0) {
        sendSetExternIO(4,0);
        send_msg(0,tr("取消刀盘反转"));
    } else {
        if(emcStatus->motion.mot_extern_out[5] > 0 || emcStatus->motion.synch_di[27] == 0) {
            QMessageBox::information(this,tr("提示"),tr("刀套必须水平"),QMessageBox::Ok);
            return;
        }
        sendSetExternIO(3,0);
        QTest::qSleep(200);
        sendSetExternIO(4,1);
        send_msg(0,tr("刀盘反转"));
    }
}

void ToolChanger::on_btn_tool4_clicked()
{
    if(emcStatus->motion.mot_extern_out[5] > 0) {
        sendSetExternIO(5,0);
    } else {
        if(emcStatus->motion.mot_extern_out[4] > 0 || emcStatus->motion.mot_extern_out[3] > 0) {
            QMessageBox::information(this,tr("提示"),tr("刀盘转动过程中不能打刀"),QMessageBox::Ok);
            return;
        }
        sendSetExternIO(6,0);
        sendSetExternIO(5,1);
        send_msg(0,tr("刀套垂直"));
    }
}

void ToolChanger::on_btn_tool5_clicked()
{
    if(emcStatus->motion.mot_extern_out[6] > 0) {
        sendSetExternIO(6,0);
    } else{
        sendSetExternIO(5,0);
        sendSetExternIO(6,1);
        send_msg(0,tr("刀套水平"));
    }
}

void ToolChanger::on_btn_tool6_clicked()
{
    if(get_abs_pos(AXIS_Z1) < -1e-2) {
        QMessageBox::information(this,tr("提示"),tr("Z轴不在零点不能旋转机械手"),QMessageBox::Ok);
            return;
    }
    if(emcStatus->motion.mot_extern_out[7] > 0) {
        sendSetExternIO(7,0);
    } else {
        sendSetExternIO(7,1);
        moveATC = 1;
        send_msg(0,tr("ATC机械手"));
    }
}

void ToolChanger::on_btn_tool7_clicked()
{
    if(EMC_TASK_MODE_AUTO==get_task_mode())
        return;
    if(0!=ensure_mode(EMC_TASK_MODE_AUTO)){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString goOrigin="/home/.config/subNgc/stepMoveATC.ngc";
    sendProgramOpen(goOrigin.toLocal8Bit().data());
    sendProgramRun(-1,0);
    send_msg(0,tr("ATC点动"));
}

void ToolChanger::on_btn_tool8_clicked()
{
    if(emcStatus->motion.mot_extern_out[9] > 0) {
        sendSetExternIO(9,0);
    } else
        sendSetExternIO(9,1);
}

void ToolChanger::on_pushButton_2_clicked()
{
    //刀库门打开安全位判断
    if((fabs(get_abs_pos(AXIS_X) - par_get_var(4024)) > 0.001 ||
            fabs(get_abs_pos(AXIS_Y) - par_get_var(4025)) > 0.001  ||
            fabs(get_abs_pos(AXIS_Z1) - par_get_var(4026)) > 0.001 ||
            !_all_homed) && par_get_var(4281) == 0 ) {
            QMessageBox::information(this,tr("提示"),
                                             tr("请先回到固定点"),QMessageBox::Ok);
                return;
    }

        sendSetExternIO(1,0);
        sendSetExternIO(0,1);

}

void ToolChanger::on_move_b_clicked()
{
    if(get_abs_pos(AXIS_Z1) < -1e-3) {
        QMessageBox::information(this,tr("提示"),  tr("移动前Z轴先回零点"),QMessageBox::Ok);
            return;
    }

    int toolnum = ui->toolchose->currentIndex()+1;
    double targetpos = par_get_var(4701+(toolnum)*5);
    if(toolnum > 19) {
        targetpos = par_get_var(4701+(toolnum)*5 + 20);
    }
    double incrementpos = targetpos - get_abs_pos(AXIS_A);
    sendJogIncr(4,3000,incrementpos);
}

void ToolChanger::on_spindle2_on_clicked()
{
    if(emcStatus->motion.synch_do[4] > 0) {
        sendSetDIO(4,0);
    } else
        sendSetDIO(4,1);
}

void ToolChanger::on_btn_resetB_clicked()
{
    if(get_abs_pos(AXIS_Z1) < -1e-3) {
        QMessageBox::information(this,tr("提示"),  tr("移动前Z轴先回零点"),QMessageBox::Ok);
            return;
    }

    double targetpos = 0;
    double incrementpos = targetpos - get_abs_pos(AXIS_A);
    sendJogIncr(4,3000,incrementpos);
}

void ToolChanger::on_btn_resetC_clicked()
{
    if(get_abs_pos(AXIS_Z1) < -1e-3) {
        QMessageBox::information(this,tr("提示"),  tr("移动前Z轴先回零点"),QMessageBox::Ok);
            return;
    }

    double targetpos = 0;
    double incrementpos = targetpos - get_abs_pos(AXIS_B);
    sendJogIncr(5,3000,incrementpos);
}

void ToolChanger::on_move_x_clicked()
{
    if(get_abs_pos(AXIS_Z1) < -1e-3) {
        QMessageBox::information(this,tr("提示"),  tr("移动前Z轴先回零点"),QMessageBox::Ok);
            return;
    }


    int toolnum = ui->toolchose->currentIndex()+1;
    double targetpos = par_get_var(4300+(toolnum)*5);
    double incrementpos = targetpos - get_abs_pos(AXIS_X);
    sendJogIncr(0,3000,incrementpos);
}

void ToolChanger::on_btn_spindlelocation_clicked()
{
    if(EMC_TASK_MODE_AUTO==get_task_mode())
        return;
    if(0!=ensure_mode(EMC_TASK_MODE_AUTO)){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString goOrigin="/home/.config/subNgc/spindleLocation.ngc";
    sendProgramOpen(goOrigin.toLocal8Bit().data());
    sendProgramRun(-1,0);
    send_msg(0,tr("ATC点动"));
}

void ToolChanger::on_OffsetAngle_valueChanged(double arg1)
{
    sendSetParameter(3815,arg1);
}


void ToolChanger::on_setangle_clicked(bool checked)
{
    if(checked) {

        if(!PassWord::getCodecnc(this,-1)) {
            ui->setangle->setChecked(false);
            ui->groupBox_3->setChecked(false);
            return;
        }
        unlockpar = 1;
    } else {
        unlockpar = 0;
    }
}

void ToolChanger::on_btn_probe_clicked()
{
    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString secondprobefile = _subDir;
    secondprobefile.append("secondProbe.ngc");
    sendProgramOpen(secondprobefile.toLocal8Bit().data());
    sendProgramRun(-1,0);
    send_msg(0,tr("开始测量刀长"));
}

void ToolChanger::on_chk_debug_clicked()
{
    if(par_get_var(4281) > 0) {
        sendSetParameter(4281,0);
    } else {
        if(!PassWord::getCodecnc(this,1)) {
            return;
        }
        sendSetParameter(4281,1);
    }
}
