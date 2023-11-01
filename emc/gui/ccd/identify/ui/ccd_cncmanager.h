#ifndef CCD_CNCMANAGER_H
#define CCD_CNCMANAGER_H

#include <QString>
#include <QObject>
#include <QHash>

class CNC_Listener;

class CCD_CncManager :public QObject
{

    Q_OBJECT

public:
    enum CNC_STATUS_TYPE {
        IDLE = 0,
        RUNNING = 1
    };

protected:
    enum JOG_TYPE {
        JOG_CONTINUE = 0,
        JOG_HANDLE,
        JOG_INCREMENT
    };

public:
    CCD_CncManager();

    void addListener(const QString &,CNC_Listener *);
    virtual double getAbsPos(int axis);
    virtual double getOffsetPos(int axis);
    int getJogSpeed();
    void setZaxisNum( int z ) {z_axis_num = z;}
    int getZaxisNum() {return z_axis_num;}

public slots:
    virtual int  excuteFile(const QString &file,int mode);
    virtual void excuteCmd(const QString &s);
    virtual void changeM71IO(int,int);
    virtual void changeM64IO(int,int);
    virtual void ioStatusChanged(int,int);
    virtual void m91Notify(QString n);
    virtual void m91Respond(int res);
    virtual void setParameter(int var,double value);
    virtual double getParameter(int var);
    virtual void abort();
    virtual int getfloodstatus();

    virtual int jogStop(int axis);
    virtual int jogCont(int axis, double speed);
    virtual int jogIncr(int axis, double speed, double incr);
    virtual void jogXplusStart();
    virtual void jogYplusStart();
    virtual void jogZplusStart();
    virtual void jogAplusStart();
    virtual void jogXminusStart();
    virtual void jogYminusStart();
    virtual void jogZminusStart();
    virtual void jogAminusStart();
    virtual void jogXEnd();
    virtual void jogYEnd();
    virtual void jogZEnd();
    virtual void jogAEnd();
    virtual void jogIncr1();
    virtual void jogIncr01();
    virtual void jogIncr001();
    virtual void jogIncr0001();
    virtual void jogContinue();
    virtual void jogSpeed(int);
    virtual void jogHandle();

    virtual void updateStatus(CNC_STATUS_TYPE s);
    virtual int sendSetLightcmdUp(int) {return 0;}
    virtual CNC_STATUS_TYPE getCncStatus();

protected:
    int z_axis_num;

private:

    QHash<QString, CNC_Listener*> listeners;
    int jogspeed;

    double jogincr;
    JOG_TYPE mode;
    CNC_STATUS_TYPE cnc_status;

};

#endif // CCD_CNCMANAGER_H
