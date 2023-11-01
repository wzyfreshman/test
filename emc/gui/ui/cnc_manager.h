#ifndef CNC_MANAGER_H
#define CNC_MANAGER_H

#include "ccd_cncmanager.h"
#include <QObject>


class CNC_Listener;

class CNC_Manager : public CCD_CncManager {

    Q_OBJECT

public:
    CNC_Manager();
    double getAbsPos(int axis);
    double getOffsetPos(int axis);

    int  excuteFile(const QString &file,int mode);
    void excuteCmd(const QString &s);
    void changeM71IO(int,int);
    void changeM64IO(int,int);
    void m91Respond(int res);
    void setParameter(int var, double value);
    double getParameter(int);
    void abort();

public slots:

    void jogXplusStart();
    void jogYplusStart();
    void jogZplusStart();
    void jogAplusStart();

    void jogXminusStart();
    void jogYminusStart();
    void jogZminusStart();
    void jogAminusStart();
    void jogXEnd();
    void jogYEnd();
    void jogZEnd();

    void jogAEnd();

    void jogIncr1();
    void jogIncr01();
    void jogIncr001();
    void jogIncr0001();
    void jogContinue();
    void jogSpeed(int);
    void jogHandle();
    virtual int sendSetLightcmdUp(int);
    virtual CNC_STATUS_TYPE getCncStatus();

private:

    virtual int jogStop(int axis);
    virtual int jogCont(int axis, double speed);
    virtual int jogIncr(int axis, double speed, double incr);



};

#endif // CNC_MANAGER_H
