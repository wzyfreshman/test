#ifndef SERIALZEROPOS_H
#define SERIALZEROPOS_H
#define AXIS_NUM 3

#include <QWidget>

namespace Ui {
class SerialzeroPos;
}

class unit_serialport;

class SerialzeroPos : public QWidget
{
    Q_OBJECT
    
public:
    explicit SerialzeroPos(QWidget *parent = 0);
    ~SerialzeroPos();

    void initSerialpos();
    
signals:
    void show_msg(int,QString);

private slots:
    void on_btn_home_x_2_clicked();

    void on_btn_home_y_2_clicked();

    void on_btn_home_z_2_clicked();



    void on_btn_abort_clicked();

    void on_btn_485cfg_clicked();

    void on_btn_directSet_clicked();

private:
    Ui::SerialzeroPos *ui;

    double recordpos[AXIS_NUM];
    double nowpos[AXIS_NUM];
    unit_serialport *serialpoart;
};

#endif // SERIALZEROPOS_H
