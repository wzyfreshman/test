#ifndef HOMEDIALOG_H
#define HOMEDIALOG_H

#include <QDialog>

namespace Ui {
class HomeDialog;
}

class HomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HomeDialog(QWidget *parent = 0);
    ~HomeDialog();

public slots:
    void home_all();
    void home_x();
    void home_y();
    void home_y_2();
    void home_z();
    void home_a();
    void home_b();
    void home_c();
    void home_quit();
    void home_direct();
    void home_update();
    void home_init();
    void unhome_x();
    void unhome_y();
    void unhome_z();
    void unhome_a();
    void unhome_b();
    void unhome_c();

protected:
    void reject();
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);
signals:
    void send_msg(int,QString str);

private slots:
 //   void on_btn_home_u_clicked();
    void on_btn_stop_pressed();

private:
    bool start_home_all;
    bool homing;
    bool homed;
    bool homedirectly;
    int showy2;
    bool starthomedirectly;
    QTimer *flush_time;
    Ui::HomeDialog *ui;
};

#endif // HOMEDIALOG_H




