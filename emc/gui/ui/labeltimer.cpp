#include "labeltimer.h"
labeltimer::labeltimer(int delay,QString str,QWidget *parent) :
    QLabel(parent)
{
    QPalette palette;;
    palette.setColor(QPalette::WindowText,QColor(0,0,0));
    this->setText(str);
    this->setPalette(palette);
    setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    this->setStyleSheet("background-color:red");
    ndelay=delay;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(hideMsg()));
    bhide=false;
}
labeltimer::~labeltimer()
{
    if (NULL != timer)
    {
       delete timer;
       timer = NULL;
    }
}

void labeltimer::setstr(QString str,int ncolor)
{
    if(bhide){
        timer->start(ndelay);
    }else {
        timer->stop();
        timer->start(ndelay);
    }
    this->setText(str);
    ft.setPointSize(15);
    this->setFont(ft);
    switch (ncolor) {
    case 1:
        this->setStyleSheet("background-color:blue");
        break;
    case 2:
        this->setStyleSheet("background-color:red");
        break;
    default:
        this->setStyleSheet("background-color:blue");
        break;
    }

}
void labeltimer::hideMsg()
{
    bhide=true;
    this->setText("");
    this->setStyleSheet(0);
//    this->setStyleSheet("background-color:gray");
}

QString labeltimer::text1()
{
    QString str;
    str=this->text();
    return str;
}
void labeltimer::setText1(QString str)
{
    if(bhide){
        timer->start(ndelay);
    }else {
        timer->stop();
        timer->start(ndelay);
    }
    ft.setPointSize(15);
    this->setFont(ft);
    this->setText(str);
}

void labeltimer::setPalette1(const QPalette &pa)
{
        this->setPalette(pa);
}
void labeltimer::setStyleSheet1(QString Style)
{
        this->setStyleSheet(Style);
}
void labeltimer::hide1()
{
    this->hide();
}
void labeltimer::show1()
{
    this->show();
}
