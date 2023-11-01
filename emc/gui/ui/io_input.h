#ifndef IO_INPUT_H
#define IO_INPUT_H

#include <QDialog>

#include <QString>
#include <QTableWidget>
#include <QSettings>
#include <QShortcut>
#include <QPushButton>

#define IO_IN_MAX_NUM 120

namespace Ui {
class IO_input;
}

class tableButton : public QPushButton
{
    Q_OBJECT
public:
    tableButton() {
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

typedef struct IO_IN_INFO
{
    QString name;
    QString info;
    QString signal;
    QString pin;
    QString polarity;
}
io_in_info;

class IO_input : public QDialog
{
    Q_OBJECT
    
public:
    explicit IO_input(QWidget *parent = 0);
    ~IO_input();

    void init_io_data(int i);
    void init_io_table(int maxnum);

protected:
    void reject();
    void resizeEvent(QResizeEvent *);
public slots:
    void io_update();
    void io_init();
    void getSimClick(int,int);
    void IOControl();

signals:
    void refresh_io();

private:
    Ui::IO_input *ui;
    io_in_info input_io[IO_IN_MAX_NUM];

    QTableWidgetItem *signal_in[IO_IN_MAX_NUM];

    QTableWidgetItem *data_in[IO_IN_MAX_NUM];
    int infopin[IO_IN_MAX_NUM];
    bool iocontrol;
    QShortcut *shortcut;

};

#endif // IO_INPUT_H
