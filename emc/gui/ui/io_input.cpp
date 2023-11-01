#include "io_input.h"
#include "ui_io_input.h"
#include <QFile>
#include <QtAlgorithms>
#include <QtDebug>
#include <QTextCodec>
#include <QTimer>

#include "shcom.hh"
#include "emc.hh"
#include "emc_nml.hh"
#include <QtDebug>
#include "rs274ngc.hh"
#include "rs274ngc_return.hh"
#include "interp_internal.hh"	// interpreter private definitions
#include "interp_queue.hh"
#include "rs274ngc_interp.hh"

#include "extern.h"
#include "mywidget.h"

#define MODIFIED_NUM 500

static QStringList input_io_pin;        //input pin list
static QStringList input_pin_sort;      //input pin list after sort
static QStringList iogroup;             //the total num of io
static QStringList input_io_polarity;

static int group_num = 0;
static int io_in_num = 0;

IO_input::IO_input(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IO_input)
{
    ui->setupUi(this);
    io_init();
    iocontrol = false;
    //ui->table_io_input->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    //ui->table_io_input->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    QStringList header ;
    header.clear();
    header.append(tr("Signal"));header.append(tr("Pin"));
    header.append(tr("Name"));header.append(tr("Polarity"));
    header.append(tr("Value"));header.append(tr(""));
    ui->table_io_input->setHorizontalHeaderLabels(header);

    ui->table_io_input->hideColumn(5);
    connect(this,SIGNAL(refresh_io()),this,SLOT(io_init()));

    QTimer *flush_time = new QTimer(this);
    connect(flush_time,SIGNAL(timeout()),this,SLOT(io_update()));
    flush_time->start(100);

    shortcut = new QShortcut(QKeySequence("Ctrl+Shift+M"),this);
    connect(shortcut,SIGNAL(activated()),this,SLOT(IOControl()));
}

void IO_input::io_init()
{
    group_num = 0;
    QString line;
    io_in_num = 0;

//    init_io_status();
    iogroup.clear();
    input_io_pin.clear();
    input_pin_sort.clear();
    input_io_polarity.clear();
    ui->table_io_input->clear();
    for(int i=ui->table_io_input->rowCount();i>0;i--)
    ui->table_io_input->removeRow(i);

    //read the ini file and get the num of the io
    QFile file(_ini_filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        if(_debug_flag)
            qDebug()<<"cannot open ini file in input_io INFO";
        return;
    }

    while (!file.atEnd()) {
        line = file.readLine();
        if(line.contains('[') &&
                line.contains(']') &&
                line.contains("IO_") &&
                !line.contains("PAR_") &&
                !line.contains('#')){
            iogroup.append(line.remove(QRegExp("[\\[\\]\\n]")));
            group_num++;
        }
    }
    file.close();


    for(int i=0;i<group_num;i++)
    {
        if(_ini->read(iogroup.at(i),"TYPE").toInt() != 1 &&
                _ini->read(iogroup.at(i),"SHOW").toInt() == 1) {                  //add show or not . janmi2015-08-29
            input_io[io_in_num].signal = iogroup.at(i);
            init_io_data(i);
        }
    }

    //sort the io according to the pins
    input_pin_sort = input_io_pin;
    //4z cancel the pin sort on 2016-10-07
    qSort(input_pin_sort.begin(),input_pin_sort.end());

 //   qDebug()<<input_pin_sort<<io_in_num;
    init_io_table(io_in_num);


}


void IO_input::init_io_data(int i)
{

    input_io[io_in_num].name = _ini->read(iogroup.at(i),"SIG");
    input_io[io_in_num].info = _ini->read(iogroup.at(i),"INFO");
    input_io[io_in_num].pin.sprintf("%02d",_ini->read(iogroup.at(i),"PIN").toInt());
    if(_ini->read(iogroup.at(i),"POLARITY").toInt() != 1)
        input_io[io_in_num].polarity = "0";
    else
        input_io[io_in_num].polarity = "1";
    input_io_pin.append(input_io[io_in_num].pin);
    io_in_num++;

}

void IO_input::reject()
{
}

void IO_input::resizeEvent(QResizeEvent *)
{
    int w = ui->table_io_input->width();
    w -= 275;
    w/=2;
    if(w<80)
        w=80;

    ui->table_io_input->setColumnWidth(0,w);
    ui->table_io_input->setColumnWidth(1,50);
    ui->table_io_input->setColumnWidth(2,w);
    ui->table_io_input->setColumnWidth(3,50);
    ui->table_io_input->setColumnWidth(4,50);
    ui->table_io_input->setColumnWidth(5,80);

}

//init the table
void IO_input::init_io_table(int maxnum)
{
    ui->table_io_input->setRowCount(maxnum);
    QStringList rowmanage; rowmanage.clear();
    int rowaux[maxnum];
    for(int i=0;i<maxnum;i++) {
        rowaux[i] = 0;
        infopin[i] = 0;
    }

    for(int i=0;i<maxnum;i++){
            int row = input_pin_sort.indexOf(input_io[i].pin);
            if(!rowmanage.contains(QString::number(row))) {
                rowmanage.append(QString::number(row));
            } else {
                rowaux[row] += 1;
                row += rowaux[row];
            }


            signal_in[i] = new QTableWidgetItem();
            signal_in[i]->setText(input_io[i].signal);
            signal_in[i]->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *info_1 = new QTableWidgetItem();
            info_1->setText(input_io[i].info);
            info_1->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *info_pin = new QTableWidgetItem();
            info_pin->setText(input_io[i].pin);
            info_pin->setFlags(Qt::ItemIsEnabled);
            infopin[row] = input_io[i].pin.toInt();

            QTableWidgetItem *info_name = new QTableWidgetItem();
            info_name->setText(input_io[i].name);
            info_name->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *info_por = new QTableWidgetItem();
            info_por->setText(input_io[i].polarity);
            info_por->setFlags(Qt::ItemIsEnabled);

            tableButton *info_btn = new tableButton();
            info_btn->setCheckable(true);
            info_btn->row = row;
            info_btn->col = 5;
            connect(info_btn,SIGNAL(sendClicked(int,int)),
                    this,SLOT(getSimClick(int,int)));
            info_btn->setStyleSheet(
    "QPushButton{border-image: url(:/new/prefix1/images/OFF.png);}QPushButton:checked{border-image: url(:/new/prefix1/images/ON.png);}");


            data_in[i] = new QTableWidgetItem();
            data_in[i]->setFlags(Qt::ItemIsEnabled);

            ui->table_io_input->setItem(row,0,info_name);
            ui->table_io_input->setItem(row,1,info_pin);
            ui->table_io_input->setItem(row,3,info_por);
            ui->table_io_input->setItem(row,2,info_1);
            ui->table_io_input->setItem(row,4,data_in[i]);
            ui->table_io_input->setCellWidget(row,5,info_btn);
    }

}

void IO_input::io_update()
{
    init_io_status();
    for(int i=0;i<io_in_num;i++)
    {

        if(_io_map.value(signal_in[i]->text()) > 0)
            data_in[i]->setIcon(    QIcon(":/new/prefix1/images/redsignal.png"));
        else
            data_in[i]->setIcon(QIcon(":/new/prefix1/images/blanklight.png"));

    }
}

IO_input::~IO_input()
{
    delete ui;
}

void IO_input::getSimClick(int row,int col) {
    tableButton *btn =
            (tableButton *)ui->table_io_input->cellWidget(row,col);
    QString temp;
    if(btn->isChecked()) {
        QString cmd = QString("halcmd setp conv-io.0.insim-%1 1")
                .arg(infopin[row]);
        get_system(cmd,&temp);
    } else {
        QString cmd = QString("halcmd setp conv-io.0.insim-%1 0")
                .arg(infopin[row]);
        get_system(cmd,&temp);
    }
}

void IO_input::IOControl()
{
    QString temp = "";
    if(!iocontrol) {
        if(!PassWord::getCodecnc(this,1)) {
            return;
        }
        iocontrol = true;
        ui->table_io_input->showColumn(5);
        for(int i=0;i<io_in_num;i++) {
            if(_io_map.value(signal_in[i]->text()) > 0) {
                tableButton *btn =
                        (tableButton *)ui->table_io_input->cellWidget(i,5);
                btn->setChecked(true);
                QString cmd = QString("halcmd setp conv-io.0.insim-%1 1")
                        .arg(i);
                get_system(cmd,&temp);
            } else {
                tableButton *btn =
                        (tableButton *)ui->table_io_input->cellWidget(i,5);
                btn->setChecked(false);
                QString cmd = QString("halcmd setp conv-io.0.insim-%1 0")
                        .arg(i);
                get_system(cmd,&temp);
            }
        }
        QString cmd = QString("halcmd setp conv-io.0.simflagin 1");
        get_system(cmd,&temp);
    } else {
        for(int i=0;i<io_in_num;i++) {
            if(_io_map.value(signal_in[i]->text()) > 0) {
                tableButton *btn =
                        (tableButton *)ui->table_io_input->cellWidget(i,5);
                btn->setChecked(true);
                QString cmd = QString("halcmd setp conv-io.0.insim-%1 1")
                        .arg(i);
                get_system(cmd,&temp);
            } else {
                tableButton *btn =
                        (tableButton *)ui->table_io_input->cellWidget(i,5);
                btn->setChecked(false);
                QString cmd = QString("halcmd setp conv-io.0.insim-%1 0")
                        .arg(i);
                get_system(cmd,&temp);
            }
        }
        QString cmd = QString("halcmd setp conv-io.0.simflagin 0");
        get_system(cmd,&temp);
        ui->table_io_input->hideColumn(5);
        iocontrol = false;
    }
}
