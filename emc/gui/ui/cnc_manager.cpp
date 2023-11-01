#include "cnc_manager.h"
#include "shcom.hh"
#include "extern.h"
#include "emcstatus.hh"

#include <QApplication>
#include <QDebug>

CNC_Manager::CNC_Manager() {

}

double CNC_Manager::getAbsPos(int axis) {

    if( axis == 2 ) {
        if( z_axis_num == 1)
            return get_abs_pos(2);
        else
            return get_abs_pos(3);
    } else {
        return get_abs_pos(axis);
    }

}
double CNC_Manager::getOffsetPos(int axis) {

    if( axis == 2 ) {
        if( z_axis_num == 1)
            return get_offset_pos(2,0);
        else
            return get_offset_pos(3,0);
    } else {
        return get_offset_pos(axis,0);
    }

}

int CNC_Manager::jogStop(int axis) {
    sendJogStop(axis);
    return 0;
}

int CNC_Manager::jogCont(int axis, double speed) {
    sendJogCont(axis,speed);
    return 0;
}

int CNC_Manager::jogIncr(int axis, double speed, double incr) {
    sendJogIncr(axis,speed,incr);
    return 0;
}

void CNC_Manager::changeM71IO(int i, int flg) {

    sendSetExternIO(i,flg);

}

void CNC_Manager::changeM64IO(int i, int flg) {

    sendSetDIO(i,flg);

}


void CNC_Manager::excuteCmd(const QString &s) {
    cmd_system(s.toLocal8Bit().data());
}

void CNC_Manager::m91Respond(int res) {
    sendWaitSignal(res); 
}


double CNC_Manager::getParameter(int var) {

    double ret = par_get_var(var);
    return ret;

}

void CNC_Manager::setParameter(int var, double value) {
    sendSetParameter(var,value);
}

void CNC_Manager::abort() {

    sendAbort();

}

int CNC_Manager::excuteFile(const QString &file,int mode) {

    if(1 == emcStatus->task.interpState)
    {


        if(0 != wait_for_state(RCS_DONE))
            return -1;
        if(0 != wait_for_mode(EMC_TASK_MODE_MANUAL))
            return -1;
        if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
            ensure_mode(EMC_TASK_MODE_MANUAL);
            sendResetcmd();
            printf("now I get error abort\n");
            return -1;
        }
        sendProgramOpen(file.toLocal8Bit().data());
        sendProgramRun(mode,0);

    } else return -1;

    return 0;

}


void CNC_Manager::jogXplusStart() {
     CCD_CncManager::jogXplusStart();
}

void CNC_Manager::jogYplusStart() {
    CCD_CncManager::jogYplusStart();
}

void CNC_Manager::jogZplusStart() {
    CCD_CncManager::jogZplusStart();
}

void CNC_Manager::jogAplusStart() {
    CCD_CncManager::jogAplusStart();
}


void CNC_Manager::jogXminusStart() {
    CCD_CncManager::jogXminusStart();
}

void CNC_Manager::jogYminusStart() {
    CCD_CncManager::jogYminusStart();
}

void CNC_Manager::jogZminusStart() {
    CCD_CncManager::jogZminusStart();
}

void CNC_Manager::jogAminusStart() {
    CCD_CncManager::jogAminusStart();
}

void CNC_Manager::jogXEnd() {
    CCD_CncManager::jogXEnd();
}

void CNC_Manager::jogYEnd() {
    CCD_CncManager::jogYEnd();
}

void CNC_Manager::jogZEnd() {
    CCD_CncManager::jogZEnd();
}

void CNC_Manager::jogAEnd() {
    CCD_CncManager::jogAEnd();
}


void CNC_Manager::jogIncr1() {
    CCD_CncManager::jogIncr1();
}

void CNC_Manager::jogIncr01() {
    CCD_CncManager::jogIncr01();
}

void CNC_Manager::jogIncr001() {
    CCD_CncManager::jogIncr001();
}

void CNC_Manager::jogIncr0001() {
    CCD_CncManager::jogIncr0001();
}

void CNC_Manager::jogContinue() {
    CCD_CncManager::jogContinue();
}

void CNC_Manager::jogHandle() {
    CCD_CncManager::jogHandle();
}

void CNC_Manager::jogSpeed(int s) {
    CCD_CncManager::jogSpeed(s);
}

int CNC_Manager::sendSetLightcmdUp(int i) {
    return sendSetlightcmd(i);
}

CCD_CncManager::CNC_STATUS_TYPE CNC_Manager::getCncStatus() {

    if(get_task_mode() == EMC_TASK_MODE_AUTO)
        return CCD_CncManager::RUNNING;

    if(get_task_mode() == EMC_TASK_MODE_MANUAL) {
        if(get_actual_feed()>0)
            return CCD_CncManager::RUNNING;
        else
            return CCD_CncManager::IDLE;
    }

}
