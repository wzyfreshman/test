#ifndef PARADIALOG_H
#define PARADIALOG_H

#include <QDialog>

#include <QTableWidget>
#include <QSettings>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "grouptablewidget.h"
#include "mytabbar.h"
#include <QRadioButton>
#include<QVBoxLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QMenu>

//#include "ini_settingsex.h"

struct _key {
    QString section;
    int ntable;
    int ngroup;
    int ngroup2nd;
    bool bgroup2nd;
    int X60;
};
typedef QList<struct _key> KEYLIST;

typedef struct tag_findpara {
    int ntable;
    int nrow;
}findpara;

namespace Ui {
class ParaDialog;
}

enum PARTABLE {
    TABLE_OPERATION = 0,
    TABLE_FEED,
    TABLE_SPINDLE,
    TABLE_TOOL,
    TABLE_PARX,
    TABLE_PARY,
    TABLE_PARZ,
    TABLE_PARA,
    TABLE_SYSTEM
};

class Par_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Par_Dialog(QWidget *parent = 0);
    ~Par_Dialog();
    void init_table(GroupTableWidget *widget);
    int get_change_text(QTableWidget *widget,int x, int y, int type, QString *str);
    void par_table_init();
    void manage_verify(QString name,QString value, QString old,QString ID);
    int modify_check(int i,QString real_value);
    void parse(QString filename, KEYLIST *keys);
    void init_table_data(GroupTableWidget *widget,_key tmpdata,int ntable,int groupno,int groupno2,KEYLIST &keys);
    void insertrow(GroupTableWidget *widget,_key tmpdata,bool bX60);
    void get_data(GroupTableWidget *widget,int ntable,KEYLIST &keys);

public slots:
    void table_change(int x,int y,int type);
    void par_cancel();
    void show_detail(int,int);
    void show_detail(int,int,int,int);
    void switch_to_operator(bool value);
    int switch_to_debug(bool value);
    void switch_to_develop(bool value);
    void getcellselect(int,int);
    void getuserDataChanged(double,QString);
    void Key_Return();
    void Key_O();
    void Key_M();
    void Key_D();
    void Key_P();
    void Key_F();
    void Key_I();
    void Key_H();
    void Key_R();
    void Key_V();
    void Key_T();
    void Key_Q();
    void Key_S();

    void getFindselect(int row,int column);
    void ParaFind();
    void slot_MenuGoto(QPoint pos);
    void slot_TblGoto();
    void tabChanged(int);


protected:
//    void keyPressEvent(QKeyEvent *e);
    void showeditdlg();
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);

    void getrowdata(int row,int tabIndex);
    void reject();

private:
        bool binitconnect;
        KEYLIST keys;
        int currentgrouplast;
        int currentgroup2last;

        void InitConnect();
        void InitUi();
        void getFindResult(int paraIndex , QTableWidgetItem* findItem ,QString strKey);
        bool checkSameRow(int tabIndex ,int row);
        void showFindeData();



signals:
    void send_msg(int,QString str);

private:
    Ui::ParaDialog *ui;
    int needreboot;
    int reboottips;

    GroupTableWidget *par_table_operate;
    GroupTableWidget *par_table_feed;
    GroupTableWidget *par_table_spindle;
    GroupTableWidget *par_table_tool;
    GroupTableWidget *par_table_parx;
    GroupTableWidget *par_table_pary;
    GroupTableWidget *par_table_parz;
    GroupTableWidget *par_table_system;
    GroupTableWidget *par_table_para;

    GroupTableWidget *find_table;

    CMyTabWidget *pTabDlg;
    int selecttab;
    int selectrow;

    QList<findpara> listfind;
    int selectTabIndex;
    int selectTabRow;
    int findRow;

    int findType;
    QString strFindKey;
    QMenu *popMenu;
    QAction *actGoto;

};


class CEditdlg:public QDialog
{
    Q_OBJECT
public:
    CEditdlg(QWidget *parent = NULL);
public:
    void setdetail(QString str);
    void setvalue(int nvalue);
    void seteditWidget(QString str);
    void seteditvalue(QString strnum);
    void setparainfo(QString str);
    void setischeck(int ischeck);
private:
    QPushButton *btn_sure;
    QPushButton *btn_cancel;
    QPushButton *btn_copy;
    QPushButton *btn_paste;

    QRadioButton *yes_btn;
    QRadioButton *no_btn;
    //---------------------guding---------------------
    //-----------Layout----------------------------------
    QHBoxLayout *lay_yseno_btn;  //yesno
    QHBoxLayout *lay_fun_btn;   //four btn
    QVBoxLayout *mainly;        //main Layout
    //-----------edit-------
    QTextBrowser *txt_browser;  //show info
    QLineEdit *stredit;
private:
    int bcheck;
    double copydata;
    double msetvalue;
    QString msetstrvalue;
    double valmini;
    double valmax;
signals:
    void setUserData(double,QString);
public slots:
    void par_sure();
    void par_cancel();
    void par_copy();
    void par_paste();

};
class Par_txtEdit : public QDoubleSpinBox
{
    Q_OBJECT
public:
    Par_txtEdit();
    ~Par_txtEdit();
public:
    void settxttype(double valmini ,double valmax,int valtype);
protected:
    void focusInEvent(QFocusEvent *ev);
private:
    double oldvalue;
    double minivalue;
    double maxvalue;
    QString strrx;
private slots:
    void line_change(QString str);

};

class Par_LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    Par_LineEdit(int,int);

    int row;
    int type;
    int column;

public slots:
    void line_change();
    void line_change1(QString);

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void focusInEvent(QFocusEvent *ev);

signals:
//     void valuechange(QString str);
    void valuechange(int row,int column,int type);
    void valueactive(int,int);
};

class Par_SpinBox : public QSpinBox
{
    Q_OBJECT

public:
    Par_SpinBox(int x, int y,int min,int max)
    {
        this->setRange(min,max);
        this->setButtonSymbols(QAbstractSpinBox::NoButtons);
        row = x;
        column = y;
        type = 1;
    }

    int row;
    int column;
    int type;

public slots:
    void line_change();

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *event);
    void focusInEvent(QFocusEvent *event);

signals:
    void valuechange(int row,int column,int type);
    void valueactive(int,int);
};

class Par_DoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    Par_DoubleSpinBox(int x, int y,double min,double max)
    {
        this->setRange(min,max);
        this->setButtonSymbols(QAbstractSpinBox::NoButtons);
        row = x;
        column = y;
        type = 2;
    }

    int row;
    int column;
    int type;

public slots:
    void line_change();

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *event);
    void focusInEvent(QFocusEvent *event);

signals:
    void valuechange(int row,int column,int type);
    void valueactive(int,int);
};
#endif // PAR_DIALOG_H
