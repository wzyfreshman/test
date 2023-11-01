#include <QDebug>
#include <QStringList>

#include "ccd_cncmanager.h"
#include "cnc_listener.h"

CCD_CncManager::CCD_CncManager() {

    z_axis_num = 1;
    jogspeed = 500;
    jogincr = 0;
    mode = JOG_CONTINUE;
    cnc_status = IDLE;

}

void CCD_CncManager::addListener(const QString &s, CNC_Listener *l) {

    listeners.insert(s,l);

}

double CCD_CncManager::getAbsPos(int axis) {

    qDebug()<<"CCD_CncManager::getAbsPos"<<axis;
    return 100.0;

}

int CCD_CncManager::getJogSpeed() {

    qDebug()<<"CCD_CncManager::getJogSpeed";
    return jogspeed;

}

double CCD_CncManager::getOffsetPos(int axis) {

    qDebug()<<"CCD_CncManager::getOffsetPos"<<axis;
    return 50.0;

}

int CCD_CncManager::excuteFile(const QString &f,int ) {

    qDebug()<<"CCD_CncManager::excuteFile"<<f;
    return 0;

}

int CCD_CncManager::jogStop(int axis) {
    qDebug()<<"CCD_CncManager::jogStop"<<axis;
    return -1;
}

int CCD_CncManager::jogCont(int axis, double speed) {
    qDebug()<<"CCD_CncManager::jogCont"<<axis<<speed;
    return -1;
}

int CCD_CncManager::jogIncr(int axis, double speed, double incr) {
    qDebug()<<"CCD_CncManager::jogIncr"<<axis<<speed<<incr;
    return -1;
}

void CCD_CncManager::ioStatusChanged(int num, int value) {

    CNC_Listener* ll;
    QHashIterator<QString, CNC_Listener*> i(listeners);
    while (i.hasNext()) {
        i.next();
        ll = i.value();
        if(ll != NULL)
            ll->ioStatusChanged(num,value);
    }

}

void CCD_CncManager::changeM71IO(int, int) {}
void CCD_CncManager::changeM64IO(int, int) {}
void CCD_CncManager::excuteCmd(const QString &) {}

void CCD_CncManager::m91Notify(QString n) {

    QStringList sl= n.split("-");
    if(sl.size() <= 0) return;
    CNC_Listener *listener = listeners.value(sl.first());
    if(listener!=NULL)
        listener->m91Notify(n);

}

void CCD_CncManager::m91Respond(int ) {}

void CCD_CncManager::setParameter(int , double ) {}

double CCD_CncManager::getParameter(int) {return 0;}

int CCD_CncManager::getfloodstatus(){return 0;}

void CCD_CncManager::abort() {}

void CCD_CncManager::jogXplusStart() {

    if(mode == JOG_CONTINUE)
        jogCont(0,jogspeed);
    else if(mode == JOG_INCREMENT)
        jogIncr(0,jogspeed,jogincr);

}

void CCD_CncManager::jogYplusStart() {
    if(mode == JOG_CONTINUE)
        jogCont(1,jogspeed);
    else if(mode == JOG_INCREMENT)
        jogIncr(1,jogspeed,jogincr);
}

void CCD_CncManager::jogZplusStart() {

    if( z_axis_num == 1 ) {

        if(mode == JOG_CONTINUE)
            jogCont(2,jogspeed);
        else if(mode == JOG_INCREMENT)
            jogIncr(2,jogspeed,jogincr);

    } else {

        if(mode == JOG_CONTINUE)
            jogCont(3,jogspeed);
        else if(mode == JOG_INCREMENT)
            jogIncr(3,jogspeed,jogincr);

    }

}

void CCD_CncManager::jogAplusStart() {
    if(mode == JOG_CONTINUE)
        jogCont(3,jogspeed);
    else if(mode == JOG_INCREMENT)
        jogIncr(3,jogspeed,jogincr);
}

void CCD_CncManager::jogXminusStart() {
    if(mode == JOG_CONTINUE)
        jogCont(0,-jogspeed);
    else if(mode == JOG_INCREMENT)
        jogIncr(0,jogspeed,-jogincr);
}

void CCD_CncManager::jogYminusStart() {
    if(mode == JOG_CONTINUE)
        jogCont(1,-jogspeed);
    else if(mode == JOG_INCREMENT)
        jogIncr(1,jogspeed,-jogincr);
}

void CCD_CncManager::jogZminusStart() {

    if( z_axis_num == 1 ) {

        if(mode == JOG_CONTINUE)
            jogCont(2,-jogspeed);
        else if(mode == JOG_INCREMENT)
            jogIncr(2,jogspeed,-jogincr);

    } else {

        if(mode == JOG_CONTINUE)
            jogCont(3,-jogspeed);
        else if(mode == JOG_INCREMENT)
            jogIncr(3,jogspeed,-jogincr);

    }

}

void CCD_CncManager::jogAminusStart() {
    if(mode == JOG_CONTINUE)
        jogCont(3,-jogspeed);
    else if(mode == JOG_INCREMENT)
        jogIncr(3,jogspeed,-jogincr);
}


void CCD_CncManager::jogXEnd() {
    if(mode == JOG_INCREMENT) return;
    jogStop(0);
}

void CCD_CncManager::jogYEnd() {
    if(mode == JOG_INCREMENT) return;
    jogStop(1);
}

void CCD_CncManager::jogZEnd() {
    if(mode == JOG_INCREMENT) return;
    if( z_axis_num == 1)
        jogStop(2);
    else
        jogStop(3);


}

void CCD_CncManager::jogAEnd() {
    if(mode == JOG_INCREMENT) return;
    jogStop(3);
}


void CCD_CncManager::jogIncr1() {
    jogincr = 1;
    mode = JOG_INCREMENT;
    excuteCmd("halcmd sets jog_enable 0");
}

void CCD_CncManager::jogIncr01() {
    jogincr = 0.1;
    mode = JOG_INCREMENT;
    excuteCmd("halcmd sets jog_enable 0");
}

void CCD_CncManager::jogIncr001() {
    jogincr = 0.01;
    mode = JOG_INCREMENT;
    excuteCmd("halcmd sets jog_enable 0");
}

void CCD_CncManager::jogIncr0001() {
    jogincr = 0.001;
    mode = JOG_INCREMENT;
    excuteCmd("halcmd sets jog_enable 0");
}

void CCD_CncManager::jogContinue() {
    mode = JOG_CONTINUE;
    excuteCmd("halcmd sets jog_enable 0");
}

void CCD_CncManager::jogHandle() {
    mode = JOG_HANDLE;
    excuteCmd("halcmd sets jog_enable 1");
}

void CCD_CncManager::jogSpeed(int s) {
     jogspeed = s;
}

void CCD_CncManager::updateStatus(CNC_STATUS_TYPE s) {

    cnc_status = s;

}

CCD_CncManager::CNC_STATUS_TYPE CCD_CncManager::getCncStatus() {
    return cnc_status;
}
