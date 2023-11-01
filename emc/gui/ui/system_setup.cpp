#include "system_setup.h"
#include "ui_system_setup.h"

#include<QFile>
#include<QMessageBox>
#include<QDebug>

System_setup::System_setup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::System_setup)
{
    ui->setupUi(this);
//    this->setWindowTitle(tr("SystemTool"));

    fail_flag = 0;
    rundir = "/tmp/systemrun.sh";

    ui->show_progress->setReadOnly(true);
    ui->show_progress->setStyleSheet("font: 8pt Sans Serif");


    sh_proc = new QProcess();
    connect(sh_proc,SIGNAL(readyReadStandardOutput()),this,SLOT(TerminalLog()));
    connect(sh_proc,SIGNAL(readyReadStandardError()),this,SLOT(TerminalLog()));
    connect(sh_proc,SIGNAL(readyRead()),this,SLOT(TerminalLog()));
    connect(sh_proc,SIGNAL(finished(int)),this,SLOT(ExcuteFinish(int)));



}

System_setup::~System_setup()
{
    delete ui;
}


void System_setup::TerminalLog() {

//    ui->show_progress->append(QString::fromLocal8Bit(sh_proc->readAllStandardOutput().data()));
//    ui->show_progress->append(QString::fromLocal8Bit(sh_proc->readAllStandardError().data()));

    QString str = QString::fromLocal8Bit(sh_proc->readAllStandardOutput().data());
    if(!str.isEmpty())
        ui->show_progress->insertPlainText(str);
    str = QString::fromLocal8Bit(sh_proc->readAllStandardError().data());
    if(!str.isEmpty()){
        ui->show_progress->insertPlainText(str);
        fail_flag = 1;
    }

    QTextCursor text_cursor(ui->show_progress->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->show_progress->setTextCursor(text_cursor);

}

void System_setup::ExcuteFinish(int) {

    QString str;
    if(1 == fail_flag)
        str = tr("Operation failed");
    else
        str = tr("Operation Succeed");

    ui->show_progress->insertPlainText(QString("%1\n").arg(str));

    QTextCursor text_cursor(ui->show_progress->textCursor());
    text_cursor.movePosition(QTextCursor::End);
    ui->show_progress->setTextCursor(text_cursor);

    fail_flag = 0;
    ui->show_btn->setEnabled(true);



}

void System_setup::on_btn_mastercam_bk_clicked()
{
    QStringList args;
    args.append("1");
    runsh(rundir,args);

}

void System_setup::on_btn_winxp_restore_clicked()
{
    int ret=QMessageBox::information(this,tr("info"),
                                     tr("Reinstall the system, all user data will be lost!"),
                                     QMessageBox::Ok,QMessageBox::Cancel);
    if(ret==QMessageBox::Cancel)
        return;

    QStringList args;
    args.append("2");
    runsh(rundir,args);

}

void System_setup::runsh(const QString &name,QStringList &arg)
{
    QFile file(":/system/systemrun.sh");
    QFile fileold("/tmp/systemrun.sh");
    fileold.remove();
    file.copy("/tmp/systemrun.sh");
    system("sudo chmod 777 /tmp/systemrun.sh");
    sh_proc->start(name,arg);

    ui->show_btn->setEnabled(false);
}

void System_setup::on_btn_qipmsg_clicked()
{
    QStringList args;
    args.append("3");
    runsh(rundir,args);

}
