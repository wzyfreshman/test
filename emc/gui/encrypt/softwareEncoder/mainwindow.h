#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void generate();
    void decrypt();
    void printAllInfo();
    void slotReadAll(bool tip = true);
    void slotWriteDown();
    void slotTest();
    void slotCustomerIndex(int);

    void slotEditItemCode(QString str);

private:
    void timerEvent(QTimerEvent *);
    bool readAllInfo();

    Ui::MainWindow *ui;

    int customerindex;
    unsigned char cpuid[8];
    unsigned char hdid[8];
    unsigned char chipid[8];

    unsigned char sysnum[8];


};

#endif // MAINWINDOW_H
