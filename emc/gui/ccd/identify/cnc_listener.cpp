#include <QDebug>

#include "cnc_listener.h"

CNC_Listener::CNC_Listener() {

}

void CNC_Listener::m91Notify(QString n) {
    qDebug()<<"CNC_Listener::m91Notify"<<n;
}

void CNC_Listener::ioStatusChanged(int i,int v){
    qDebug()<<"CNC_Listener::ioStatusChanged"<<i<<v;
}
