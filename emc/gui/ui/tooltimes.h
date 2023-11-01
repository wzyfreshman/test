#ifndef TOOLTIMES_H
#define TOOLTIMES_H

#include <QDialog>
#include <QShowEvent>

#include "linuxcnc.h"

#define MAXROWS 50
#define MAXCOLS 10

namespace Ui {
class tooltimes;
}

class tooltimes : public QDialog
{
    Q_OBJECT

public:
    explicit tooltimes(QWidget *parent = 0,int current1 =0 ,linuxcnc *lcnc=0,int current2 = 0);
    ~tooltimes();

    int toolcurrent[2];
    int toolmaxnum[2];
    int toolworkpiece[2];

    void init_tbl();
    void updatetbl(int index,int clearz=0);

public slots:
    void setvalue(int x,int y);
    void setheader(int index);
    void on_pushButton_clicked();

protected:
     void showEvent(QShowEvent *);
     void reject();

private slots:


    void on_pushButton_2_clicked();

    void update();



    void on_usetbl_clicked(bool checked);
signals:
    void send_msg(int,QString str);

private:
    Ui::tooltimes *ui;
    linuxcnc *cnc;

    QStringList setpnum[MAXCOLS];
    QString tblData[MAXROWS][MAXCOLS];
    QStringList headlist;
    QTableWidgetItem *dataItem[MAXROWS][MAXCOLS];

};



#endif // TOOLTIMES_H


