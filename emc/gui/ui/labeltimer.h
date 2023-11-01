#ifndef LABELTIMER_H
#define LABELTIMER_H
#include <QLabel>
#include <QTimer>
class labeltimer:public QLabel
{
    Q_OBJECT
public:
    explicit labeltimer(int delay,QString str,QWidget *parent = 0);
    void setstr(QString str,int ncolor);
    void setText1(QString str);
    void setPalette1(const QPalette &pa);
    void setStyleSheet1(QString Style);
    void hide1();
    void show1();
    QString text1();
    ~ labeltimer();
private:
    QTimer *timer;
    QFont ft;
    bool bhide;
    int ndelay;
signals:
    void timeout();
public slots:
    void hideMsg();
};

#endif // LABELTIMER_H
