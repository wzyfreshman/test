#ifndef IO_OUTPUT_H
#define IO_OUTPUT_H

#include <QDialog>

#include <QString>
#include <QTableWidget>
#include <QSettings>
#include <QPushButton>
#include <QShortcut>

#define IO_OUT_MAX_NUM 100

namespace Ui {
class IO_output;
}

typedef struct IO_OUT_INFO
{
    QString name;
    QString info;
    QString signal;
    QString pin;
    QString polarity;
}
io_out_info;

class tableButtonout : public QPushButton
{
    Q_OBJECT
public:
    tableButtonout() {
        row = 0;
        col = 0;
        connect(this,SIGNAL(clicked()),this,SLOT(setClicked()));
    }
    int row,col;

public slots:
    void setClicked() {
            emit sendClicked(row,col);
    }
signals:
    void sendClicked(int,int);
};


class IO_output : public QDialog
{
    Q_OBJECT
    
public:
    explicit IO_output(QWidget *parent = 0);
    ~IO_output();

    void init_io_data(int i);
    void init_io_table(int maxnum);

protected:
    void reject();
    void resizeEvent(QResizeEvent *);
public slots:
    void io_update();
    void io_init();
    void auto_lube_time();
    void lube_connect_timeout();
    void getSimClick(int,int);
    void IOControl();
signals:
    void refresh_io();

private:
    Ui::IO_output *ui;
    QTimer *lube_connect_time;
    io_out_info output_io[IO_OUT_MAX_NUM];

    QTableWidgetItem *signal_out[IO_OUT_MAX_NUM];

    QTableWidgetItem *data_out[IO_OUT_MAX_NUM];
    int infopin[IO_OUT_MAX_NUM];
    bool iocontrol;
    QShortcut *shortcut;
};

#endif // IO_OUTPUT_H
