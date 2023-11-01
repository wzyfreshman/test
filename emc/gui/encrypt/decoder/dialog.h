#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void setSysString(QString);
    void setVerString(QString);

public slots:
    void slotCode1(QString);
    void slotCode2(QString);
    void slotCode3(QString);
    void slotCode4(QString);

    void slotVer(QString);
    void slotSys(QString);
    void slotSign();
    void slotRead();
    void testFun();
protected:
    void timerEvent(QTimerEvent *);
private:


    QString sys_string;
    QString ver_string;
    unsigned char sys[4];
    unsigned char ver[4];
    unsigned char code[8];

    Ui::Dialog *ui;
    QString code1;
    QString code2;
    QString code3;
    QString code4;
    QString vercode;

};

#endif // DIALOG_H
