#include "touch_checklink.h"
#include "ui_touch_checklink.h"
#include "emcstatus.hh"
#include "shcom.hh"
#include "extern.h"

#include <QDesktopWidget>
touch_Checklink::touch_Checklink(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::touch_Checklink)
{
    ui->setupUi(this);
}

touch_Checklink::~touch_Checklink()
{
    delete ui;
}

void touch_Checklink::updateUI()
{
    if(emcStatus->motion.traj.linktoEth > 0)
    {
        this->close();
    }
}

void touch_Checklink::on_btn_ignore_clicked()
{
    this->close();
}

void touch_Checklink::showEvent(QShowEvent *event)
{
    QDesktopWidget *screenResolution = QApplication::desktop();
    int center_y = screenResolution->height()/2 - this->height()/2;
    int center_x = screenResolution->width()/2 - this->width()/2;
    this->move(center_x,center_y);
//    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    QWidget::showEvent(event);
}


