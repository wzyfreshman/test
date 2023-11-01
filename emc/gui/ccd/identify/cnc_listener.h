#ifndef CNC_LISTENER_H
#define CNC_LISTENER_H

#include <QString>

class CNC_Listener
{
public:
    CNC_Listener();
    virtual void m91Notify(QString n);
    virtual void ioStatusChanged(int,int);
};

#endif // CNC_LISTENER_H
