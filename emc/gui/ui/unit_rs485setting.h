#ifndef UNIT_RS485SETTING_H
#define UNIT_RS485SETTING_H

#include <QDialog>
#include <termios.h>


namespace Ui {
class unit_RS485Setting;
}

class CNCControl;
class unit_RS485Setting : public QDialog
{
    Q_OBJECT
    
public:
    explicit unit_RS485Setting(QWidget *parent = 0);
    ~unit_RS485Setting();

    int zeroposAddr;           //零点地址
    int speedAddr;              //速度地址
    int baudrate;       //使用的波特率
    QString devname;    //485的端口
    int alarmAddr;      //毛刷报警的地址

private slots:
    void on_btn_close_clicked();

    void on_btn_save_clicked();

    void on_chk_usealm_clicked();

protected:
    void showEvent(QShowEvent *);

private:
    Ui::unit_RS485Setting *ui;


    int baudArray[6] =  {B4800,B9600,B19200,B38400,B57600,B115200};
};

#endif // UNIT_RS485SETTING_H
