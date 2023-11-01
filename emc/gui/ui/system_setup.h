#ifndef SYSTEM_SETUP_H
#define SYSTEM_SETUP_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class System_setup;
}

class System_setup : public QDialog
{
    Q_OBJECT

public:
    explicit System_setup(QWidget *parent = 0);
    ~System_setup();

private:
    Ui::System_setup *ui;
    QProcess *sh_proc ;
    QString rundir;
    int fail_flag;

private slots:
    void runsh(const QString &name,QStringList &arg);
    void TerminalLog();
    void ExcuteFinish(int);

    void on_btn_mastercam_bk_clicked();
    void on_btn_winxp_restore_clicked();

    void on_btn_qipmsg_clicked();
};

#endif // SYSTEM_SETUP_H
