#ifndef IODIALOG_H
#define IODIALOG_H

#include <QDialog>
#include <QDialog>
#include <QString>
#include <QTableWidget>
#include <QSettings>
#include <QComboBox>
#include <QRadioButton>

#include "extern.h"
#include "emcstatus.hh"

#define IO_IN_MAX_NUM 120
#define IO_OUT_MAX_NUN 100

#define MODIFIED_NUM 500

namespace Ui {
class IODialog;
}

typedef struct IO_INFO
{
    QString info;
    QString signal;
    QString pin;
    QString polarity;
    double filterTime;
    int type;
}
io_info;

enum IO_TYPE{
    INPUT = 0,
    OUTPUT
};

typedef class AlarmIO{
public:
    int type;
    int polarity;
    bool exist;
    QString info;
} AlarmIO;

class IO_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit IO_Dialog(QWidget *parent = 0);
    ~IO_Dialog();
    friend class Table_Combobox;
    friend class Table_Polarity;

    void InitUi();
    void InitConnect();
    void init_io_data(int i,int type);
    void init_io_table(QTableWidget *widget, int maxnum);
    void manage_modify(QString name,QString value);
    void manage_por_modify(QString name,QString value);
    void setTableStatus(int type);
    void flushwarn();
    QTimer *getFlushTimer();

public slots:
    void table_text_change(int x,int y);
    void io_save();
    void io_cancel();
    void io_update();
    void io_init();

    void switch_to_operator(bool value);
    int switch_to_debug(bool value);
    void switch_to_develop(bool value);

    void setwarninfo(QModelIndex);
    void setwarnexist(bool);
    void setwarnpol(bool);
    void setwarntext(QString);
    void setwarntype(QTableWidgetItem*);
    void slotItemchange(QTableWidgetItem * item);
    void set_filtertime();



protected:
    bool checkModidfy();
    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);
    void reject();
    void allDataInit();
    void checkPins();
    bool getFilterData();
    bool readFilterData();
    bool saveFilter();
    bool checkFilter();

private:
    Ui::IODialog *ui;
    io_info input_io[IO_IN_MAX_NUM];
    io_info output_io[IO_OUT_MAX_NUN];

    QTableWidgetItem *signal_in[IO_IN_MAX_NUM];
    QTableWidgetItem *signal_out[IO_OUT_MAX_NUN];

    QTableWidgetItem *data_in[IO_IN_MAX_NUM];
    QTableWidgetItem *data_out[IO_OUT_MAX_NUN];

    QStringList chose_input_pin;     //input combox items
    QStringList chose_output_pin;     //output combox items
    QStringList input_io_pin;        //input pin list
    QStringList output_io_pin;       //output pin list
    QStringList input_pin_sort;      //input pin list after sort
    QStringList output_pin_sort;     //outpin pin list after sort
    QStringList iogroup;             //the total num of io
    QStringList in_row_info;            //the info of input according to the input_table
    QStringList out_row_info;
    QStringList input_io_polarity;
    QStringList output_io_polarity;
    QStringList modified_item[MODIFIED_NUM];
    QStringList modified_polarity[MODIFIED_NUM];



    int group_num ;
    int io_in_num  ;
    int io_out_num  ;
    int modified_count ;
    int modified_polarity_count  ;
    int code_choice ;
    int warnindex;
    bool recordpol[IO_IN_MAX_NUM];

    AlarmIO *alm_sig[DEFAULT_ALM_SIG];
    QTableWidgetItem *type[6];

    QTimer *flush_time;
    double filter_mini;
    double filter_max;
    double filter_set;

};

class Table_Combobox : public QDialog
{
    Q_OBJECT
public:
    Table_Combobox(int type, int x, int y, IO_Dialog *io);

    int row;    //record the row in the table
    int column; //record the column in the table

public slots:
    void combo_change(QString);
    void setcurrentindex(QString str);

protected:
    void wheelEvent(QWheelEvent *event);

signals:
    void valuechanged(int row,int column);

private:
    QStringList keylist,sortpin;
    QComboBox *comb;
    QString initstr;
    QPushButton *ensure_btn;

    IO_Dialog *externio;
    int type;

};

class Table_Polarity:public QDialog {
    Q_OBJECT
public:
    Table_Polarity(int type,int x,int y,IO_Dialog *io);
    int row,column;
    int check;
public slots:
    void valuechange(bool);
    void valuechange2(bool);
private:
    QRadioButton *btnpos,*btnneg;
    QWidget *w;
    QPushButton *ensure_btn;
    IO_Dialog *externio;
};

#endif // IO_DIALOG_H
