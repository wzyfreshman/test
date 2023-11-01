#include "infodialog.h"
#include "ui_infodialog.h"
#include "emcstatus.hh"
#include "codeeditor.h"
#include "extern.h"
#include "numstr_inputdialog.h"
//#include "../../.git/LatestHash.h"
#include "../../config.h"

#include <QtDebug>
#include <QSettings>
#include <QFile>
#include <QTextCodec>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDate>

Infodialog::Infodialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Infodialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("System Info"));

    QString  str = tr("CNEXT-");
    version = str;

    version.append("A809");
    if(0 ==  SIMULATE){
        version.append("-A");
    }  else {
        version.append("-S");
    }

    ui->show_hardware->setText(tr("%1-%2").
                    arg(versiondata->IOboardVersion).arg(get_chip_ver()));
    ui->show_customid->setText(get_customid());
    ui->show_boardid->setText(get_boardid());

    connect(ui->SftVersion_More,SIGNAL(clicked()),this,SLOT(showMoreSftInfo()));

    ui->SftVersion_More->setHidden(true);


    QFile fs(SAVE_SYSNUM);
    fs.open(QIODevice::ReadOnly);
    sys_string = QString(fs.read(20));
    fs.close();
    ui->SysNum->setText(sys_string);
    this->slotSys(sys_string);
    connect(ui->modify_num,SIGNAL(clicked()),this,SLOT(setSysString()));

    logview = new FileEditor(0);

    QTextCodec *code = QTextCodec::codecForName("utf-8");
    QString file_text;
    QString Versioninfo = "";
    QFile file(":log/log.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file_text = code->toUnicode(file.readAll());
        file.seek(0);
        do{
            Versioninfo = file.readLine();
        } while(Versioninfo.length() == 0);
        file.close();
        logview->displayText(file_text);
        Versioninfo.remove("\n");
    }
    logview->hide();
    this->installEventFilter(this);

    version.append("-S").append(QString::number(versiondata->segVersion)).append("H").append(QString::number(versiondata->ethercatVersion));
    version.append("-").append(Versioninfo);
    ui->show_version->setText(version);
}


bool Infodialog::eventFilter(QObject *obj, QEvent *e) {

    if((e->type() == QEvent::KeyPress ||
        e->type() == QEvent::KeyRelease)
            && (this->isActiveWindow()) ) {
        QKeyEvent *event = static_cast<QKeyEvent*>(e);
        if (event->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier) && event->key() == Qt::Key_M)
            {
                ui->SftVersion_More->setHidden(false);
                return true;
            }
    }
    return QWidget::eventFilter(obj,e);
}


Infodialog::~Infodialog()
{
    delete ui;
}

void Infodialog::showMoreSftInfo() {

    this->close();
    logview->show();
}

void Infodialog::setSysString() {

    if(PassWord::getPassWord(this,"syslock")) {

        bool ok;
        NumStr_InputDialog dlg(this);
        int ret = dlg.exec();
        if(ret) ok = true;
        else ok =false;

        QString text = NumStr_InputDialog::inputtext;

         if (ok && !text.isEmpty()) {
             if(text.size() > 20 ) {
                 QMessageBox::warning(this,tr("warnning"),
                 QString(tr("sysnum too large")),tr("Ok"));
                 return;
             }
             this->slotSys(text);
             ui->SysNum->setText(text);
         }
    }

}

void Infodialog::slotSys(QString v) {
    sys_string = v;
    QFile fp(SAVE_SYSNUM);
    fp.open(QIODevice::WriteOnly);
    fp.write(sys_string.toLocal8Bit());
    fp.close();
}
