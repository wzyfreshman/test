#include "smartrundialog.h"
#include "ui_smartrundialog.h"

#include <QtDebug>
#include <QFileDialog>
#include <inifile.hh>
#include <QMessageBox>

#include "emc.hh"
#include "emc_nml.hh"
#include "shcom.hh"
#include "extern.h"
#include "emcstatus.hh"

SmartRunDialog::SmartRunDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SmartRunDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("SmartStart"));

    startEnable = false;
    endEnable = false;

    if(1 == par_get_var(4037)) {
        ui->fastrun->setChecked(true);
    } else
        ui->fastrun->setChecked(false);

    connect(ui->btn_cancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->btn_runline,SIGNAL(clicked()),this,SLOT(runfile()));
    connect(ui->rad_pro_head,SIGNAL(clicked(bool)),this,SLOT(setFilehead(bool)));
    connect(ui->rad_startline,SIGNAL(clicked(bool)),this,SLOT(setStartline(bool)));
    connect(ui->rad_pro_end,SIGNAL(clicked(bool)),this,SLOT(setFileend(bool)));
    connect(ui->rad_endline,SIGNAL(clicked(bool)),this,SLOT(setEndLine(bool)));
    connect(ui->fastrun,SIGNAL(clicked(bool)),this,SLOT(setFastrun(bool)));

    ui->safeheight->setValue(par_get_var(4540));
    ui->fastrun->hide();
    sendSetParameter(4037,1);   //默认启用快速模式
    ui->safeheight->hide();
    ui->label->hide();
    ui->label_2->hide();
}

void SmartRunDialog::setFastrun(bool val)
{
    if(val) {
        sendSetParameter(4037,1);
    } else
        sendSetParameter(4037,0);
}

void SmartRunDialog::runfile()
{
    runfromline(ui->set_startline->value(),ui->set_endline->value());
}

void SmartRunDialog::runfromline(int line,int endline)
{
    if(filepath == NULL) {
        qDebug()<<"can not run with a null filepath";
        return ;
    }
    if(line >= endline && endEnable && startEnable) {
        QMessageBox::warning(this,tr("tips"),
                                 tr("endline must be greater than startline!"),
                             tr("Ok"));
        return;
    }
    if(1 == emcStatus->task.interpState)
    {
        if(0 != wait_for_state(RCS_DONE))
            return ;
        if(0 != wait_for_mode(EMC_TASK_MODE_MANUAL))
            return ;
        if(0 != ensure_mode(EMC_TASK_MODE_AUTO))
            return;
        sendProgramOpen(realpath.toLocal8Bit().data());
        //qDebug()<<"Start Program Run from line" <<line;
        sendProgramRun(startEnable?line:0,endEnable?endline:0);
        this->close();
    }
}

void SmartRunDialog::setfilename(QString file)
{
    filepath = file;
    QStringList temp_file;
    QString now_file;
    temp_file = filepath.split("/");
    now_file = temp_file.at(temp_file.count()-1);
    realpath.clear();
    realpath.append("/home/.tmp/").append(now_file);
}

void SmartRunDialog::setFilehead(bool val)
{
    if(val) {
        ui->set_startline->setEnabled(false);
        startEnable = false;
    }
}

void SmartRunDialog::setFileend(bool val)
{
    if(val) {
       ui->set_endline->setEnabled(false);
       endEnable = false;
    }
}

void SmartRunDialog::setStartline(bool val)
{
    if(val) {
        ui->set_startline->setEnabled(true);
        startEnable = true;
    }
}

void SmartRunDialog::setEndLine(bool val)
{
    if(val) {
        ui->set_endline->setEnabled(true);
        endEnable = true;
    }
}

void SmartRunDialog::showEvent(QShowEvent *ev)
{
    ui->set_startline->setRange(0,totalline);
    ui->set_endline->setRange(0,totalline);
    startEnable = false;
    endEnable = false;
    ui->rad_pro_head->setChecked(true);
    ui->rad_pro_end->setChecked(true);
    setFilehead(true);
    setFileend(true);
    ui->rad_startline->setText(
                QString(tr("Specify line(total:%1):")).arg(totalline));
    ui->rad_endline->setText(
                QString(tr("Specify line(total:%1):")).arg(totalline));
    ui->set_startline->setValue(get_pro_interpline());
    ui->set_endline->setValue(0);
    QDialog::showEvent(ev);

}

void SmartRunDialog::settotlalines(int line)
{
    totalline = line;
}

SmartRunDialog::~SmartRunDialog()
{
    delete ui;
}

void SmartRunDialog::on_safeheight_valueChanged(double arg1)
{
    sendSetParameter(4540,arg1);
}
