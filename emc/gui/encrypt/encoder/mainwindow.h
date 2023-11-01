#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDate>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setExcutePath(const QString &str);

public slots:

    void slotInputCode(QString);
    void slotGenerateCode();
    void slotVer(QString);
    void slotSys(QString);
    void slotInfo(QString);
    void slotInfoday(QString);
    void slotInfohour(QString);
    void slotForeverCheck(int);
    void slotChangeMode(int);
    void slotInfoDate(QDate date);

private:
    Ui::MainWindow *ui;
    QString userinput;

    QString sys_string;
    QString ver_string;
    QString info_string;
    unsigned char sys[4];
    unsigned char ver[4];
    unsigned char sys_ver[8];
    unsigned char info[4];
    unsigned char info_date[4];
    int day;
    int hour;

    int mode;

    QString excutepath;

};

#endif // MAINWINDOW_H
