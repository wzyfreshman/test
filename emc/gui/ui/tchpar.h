#ifndef TCHPAR_H
#define TCHPAR_H

#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include "mywidget.h"

#include <QTableWidget>
#define MAXTOOL 38

namespace Ui {
class TCHPar;
}

class ToolPar {
   public:
    double x;
    double y;
    double z1;
    double z2;
    double b;
    double c;
};

class TCHPar : public QWidget
{
    Q_OBJECT
    
public:
    explicit TCHPar(QWidget *parent = 0);
    ~TCHPar();

    void initData();
    void initTool();

    ToolPar tooldata[MAXTOOL];
    QTableWidgetItem *data_X[MAXTOOL];
    QTableWidgetItem *data_Y[MAXTOOL];
    QTableWidgetItem *data_Z1[MAXTOOL];
    QTableWidgetItem *data_Z2[MAXTOOL];
    QTableWidgetItem *data_B[MAXTOOL];
    QTableWidgetItem *data_C[MAXTOOL];
    int setToolnum;
    int tableType;

public slots:
    void tableclick(int x,int y);

    void setToolnow(int);
    void setToolLen(int);

signals:
    void showMsg(int,QString);

private slots:

    void on_chk_ignoretool_toggled(bool checked);

    void on_chk_ignorespindle_toggled(bool checked);

    void on_chk_ignorecover_toggled(bool checked);

    void on_chk_usetbl_toggled(bool checked);

    void updateui();

    void on_chk_modify_toggled(bool checked);

    void on_chk_autoprobe_toggled(bool checked);


    void on_changedelay_valueChanged(double arg1);

    void on_safetblpos_valueChanged(double arg1);

    void on_allowdebug_toggled(bool checked);

    void initToolLengthTbl();

    void on_checkBox_toggled(bool checked);

    void set_safeYpos();

    void on_tableToolLen_cellDoubleClicked(int row, int column);


    void on_chk_runprobe_clicked();

private:
    Ui::TCHPar *ui;

    MyLabel *show_tnum_z1,*show_tnum_z2,*show_toollen;

    QTableWidgetItem *tool1len[MAXTOOL],*toolpocket[MAXTOOL],*toolno[MAXTOOL];

    QMap<int, int> pocket;

    double safeypos;
    MyLabel *lab_safeypos;
};



class SetValue : public QDialog
{
    Q_OBJECT
public:
    SetValue(double defaultval,int x,int y,QDialog *parent = 0 );
    double defvel;
    int row ,column;

public slots:
    void ensureSetValue();
    void getPos();

private:

    QDoubleSpinBox *defaulval;
    QLabel *showdefault;
    QWidget *defaultset,*btnset;
    QPushButton *btn_ensure,*btn_cancel,*btn_getpos;


};

#endif // TCHPAR_H
