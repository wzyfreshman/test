#include "io_output.h"
#include "ui_io_output.h"
#include <QFile>
#include <QtAlgorithms>
#include <QtDebug>
#include <QTextCodec>
#include <QTimer>
#include <QPushButton>
#include <QShortcut>
#include <QtDebug>

#include "shcom.hh"
#include "emc.hh"
#include "emc_nml.hh"
#include "rs274ngc.hh"
#include "rs274ngc_return.hh"
#include "interp_internal.hh"	// interpreter private definitions
#include "interp_queue.hh"
#include "rs274ngc_interp.hh"

#include "extern.h"
#include "emcstatus.hh"
#include "mywidget.h"

#define MODIFIED_NUM 500

static QStringList output_io_pin;        //output pin list
static QStringList output_pin_sort;      //output pin list after sort
static QStringList iogroup;             //the total num of io
static QStringList output_io_polarity;

static int group_num = 0;
static int io_in_num = 0;

IO_output::IO_output(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IO_output)
{
    ui->setupUi(this);

    iocontrol = false;

    ui->table_io_output->setColumnWidth(0,150);
    ui->table_io_output->setColumnWidth(1,50);
    ui->table_io_output->setColumnWidth(2,150);
    ui->table_io_output->setColumnWidth(3,50);
    ui->table_io_output->setColumnWidth(4,50);
    ui->table_io_output->setColumnWidth(5,80);
    //ui->table_io_output->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    //ui->table_io_output->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->table_io_output->hideColumn(5);

    io_init();

    QStringList header;
    header.clear();
    header.append(tr("Signal"));header.append(tr("Pin"));
    header.append(tr("Name"));header.append(tr("Polarity"));
    header.append(tr("Value"));header.append(tr(""));
    ui->table_io_output->setHorizontalHeaderLabels(header);



    connect(this,SIGNAL(refresh_io()),this,SLOT(io_init()));

    QTimer *flush_time = new QTimer(this);
    connect(flush_time,SIGNAL(timeout()),this,SLOT(io_update()));
    flush_time->start(100);

    QTimer *lube_time = new QTimer(this);
    connect(lube_time,SIGNAL(timeout()),this,SLOT(auto_lube_time()));
    if(_ini->read("PAR_OPERATOR_LUBEAUTOSTART","VALUE").toInt() == 1)
        lube_time->start(_ini->read("PAR_OPERATOR_LUBESTARTTIME","VALUE").toInt()*1000);

    lube_connect_time = new QTimer(this);
    connect(lube_connect_time,SIGNAL(timeout()),this,SLOT(lube_connect_timeout()));

    shortcut = new QShortcut(QKeySequence("Ctrl+Shift+M"),this);
    connect(shortcut,SIGNAL(activated()),this,SLOT(IOControl()));

}

void IO_output::io_init()
{
    group_num = 0;
    QString line;
    io_in_num = 0;


//    init_io_status();

    iogroup.clear();
    output_io_pin.clear();
    output_pin_sort.clear();
    output_io_polarity.clear();
    ui->table_io_output->clear();
    for(int i=ui->table_io_output->rowCount();i>0;i--)
    ui->table_io_output->removeRow(i);

    //read the ini file and get the num of the io
    QFile file(_ini_filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        if(_debug_flag)
            qDebug()<<"cannot open ini file in output_io INFO";
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
        if(_ini->read(iogroup.at(i),"TYPE").toInt() == 1&&
                _ini->read(iogroup.at(i),"SHOW").toInt() == 1) {  //add IO info display or not. janmi 2015-8-29
            output_io[io_in_num].signal = iogroup.at(i);
            init_io_data(i);
        }
    }


    //sort the io according to the pins
    output_pin_sort = output_io_pin;

    //4z cancel the pin sort on 2016-10-07
    qSort(output_pin_sort.begin(),output_pin_sort.end());

 //   qDebug()<<output_pin_sort<<io_in_num;
    init_io_table(io_in_num);
}


void IO_output::init_io_data(int i)
{

    output_io[io_in_num].name = _ini->read(iogroup.at(i),"SIG");
    output_io[io_in_num].info = _ini->read(iogroup.at(i),"INFO");
    output_io[io_in_num].pin.sprintf("%02d",_ini->read(iogroup.at(i),"PIN").toInt());
    output_io[io_in_num].polarity = _ini->read(iogroup.at(i),"POLARITY");
    output_io_pin.append(output_io[io_in_num].pin);
    io_in_num++;

}

void IO_output::reject()
{
}

void IO_output::resizeEvent(QResizeEvent *)
{
    int w = ui->table_io_output->width();
    w -= 275;
    w/=2;
    if(w<100)
        w=100;
    ui->table_io_output->setColumnWidth(0,w);
    ui->table_io_output->setColumnWidth(1,50);
    ui->table_io_output->setColumnWidth(2,w);
    ui->table_io_output->setColumnWidth(3,50);
    ui->table_io_output->setColumnWidth(4,50);
    ui->table_io_output->setColumnWidth(5,80);
}

//init the table
void IO_output::init_io_table(int maxnum)
{
    ui->table_io_output->setRowCount(maxnum);
    QStringList rowmanage; rowmanage.clear();
    int rowaux[maxnum];
    for(int i=0;i<maxnum;i++) {
        rowaux[i] = 0;
        infopin[i] = 0;
    }

    for(int i=0;i<maxnum;i++){
            int row = output_pin_sort.indexOf(output_io[i].pin);
            if(!rowmanage.contains(QString::number(row))) {
                rowmanage.append(QString::number(row));
            } else {
                rowaux[row] += 1;
                row += rowaux[row];
            }

            signal_out[i] = new QTableWidgetItem();
            signal_out[i]->setText(output_io[i].signal);
            signal_out[i]->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *info_1 = new QTableWidgetItem();
            info_1->setText(output_io[i].info);
            info_1->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *info_pin = new QTableWidgetItem();
            info_pin->setText(output_io[i].pin);
            info_pin->setFlags(Qt::ItemIsEnabled);
            infopin[row] = output_io[i].pin.toInt();

            QTableWidgetItem *info_name = new QTableWidgetItem();
            info_name->setText(output_io[i].name);
            info_name->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *info_por = new QTableWidgetItem();
            info_por->setText(output_io[i].polarity);
            info_por->setFlags(Qt::ItemIsEnabled);

            tableButtonout *info_btn = new tableButtonout();
            info_btn->setCheckable(true);
            info_btn->row = row;
            info_btn->col = 5;
            connect(info_btn,SIGNAL(sendClicked(int,int)),
                    this,SLOT(getSimClick(int,int)));
            info_btn->setStyleSheet(
    "QPushButton{border-image: url(:/new/prefix1/images/OFF.png);}QPushButton:checked{border-image: url(:/new/prefix1/images/ON.png);}");

            data_out[i] = new QTableWidgetItem();
            data_out[i]->setFlags(Qt::ItemIsEnabled);


            ui->table_io_output->setItem(row,0,info_name);
            ui->table_io_output->setItem(row,1,info_pin);
            ui->table_io_output->setItem(row,3,info_por);
            ui->table_io_output->setItem(row,2,info_1);
            ui->table_io_output->setItem(row,4,data_out[i]);
            ui->table_io_output->setCellWidget(row,5,info_btn);

            //QTableWidgetItem *tmp = new QTableWidgetItem();
            //tmp->setFlags(Qt::ItemIsEnabled);
            //ui->table_io_output->setItem(row,5,tmp);

    }

}

void IO_output::getSimClick(int row,int col) {
    tableButtonout *btn =
            (tableButtonout *)ui->table_io_output->cellWidget(row,col);
    QString temp;
    if(btn->isChecked()) {
        QString cmd = QString("halcmd setp conv-io.1.insim-%1 1")
                .arg(infopin[row]);
        get_system(cmd,&temp);
    } else {
        QString cmd = QString("halcmd setp conv-io.1.insim-%1 0")
                .arg(infopin[row]);
        get_system(cmd,&temp);
    }
}

void IO_output::io_update()
{
    init_io_status();
    static int lube_flag = -1;
    if(1 == get_lube_status() && lube_flag != get_lube_status()){
       int lubetime=_ini->read("PAR_OPERATOR_LUBEWORKTIME","VALUE").toInt();
        lube_connect_time->start(lubetime*1000);
        sendSetParameter(4019,lubetime);
    }
    lube_flag = get_lube_status();
    for(int i=0;i<io_in_num;i++)
    {
        if(_io_map.value(signal_out[i]->text()) > 0)
            data_out[i]->setIcon(QIcon(":/new/prefix1/images/signal.png"));
        else
            data_out[i]->setIcon(QIcon(":/new/prefix1/images/blanklight.png"));

    }
}

void IO_output::auto_lube_time()
{
    if((get_pro_running() && _ini->read("PAR_OPERATOR_LUBEONRUNNING","VALUE").toInt() == 1) ||
            _ini->read("PAR_OPERATOR_LUBEONRUNNING","VALUE").toInt() == 0){
        if(0 == get_lube_status()){
            sendLubeOn();
        }
    }
}

void IO_output::lube_connect_timeout()
{
    if(1 == get_lube_status()){
        sendLubeOff();
        lube_connect_time->stop();      //修复润滑油bug
    }
}

IO_output::~IO_output()
{
    delete ui;
}

void IO_output::IOControl()
{
    QString temp = "";
    if(!iocontrol) {
        if(!PassWord::getCodecnc(this,1)) {
            return;
        }
        iocontrol = true;
        ui->table_io_output->showColumn(5);
        for(int i=0;i<io_in_num;i++) {
            if(_io_map.value(signal_out[i]->text()) > 0) {
                tableButtonout *btn =
                        (tableButtonout *)ui->table_io_output->cellWidget(i,5);
                btn->setChecked(true);
                QString cmd = QString("halcmd setp conv-io.1.insim-%1 1")
                        .arg(i);
                get_system(cmd,&temp);
            } else {
                tableButtonout *btn =
                        (tableButtonout *)ui->table_io_output->cellWidget(i,5);
                btn->setChecked(false);
                QString cmd = QString("halcmd setp conv-io.1.insim-%1 0")
                        .arg(i);
                get_system(cmd,&temp);
            }
        }
        QString cmd = QString("halcmd setp conv-io.1.simflagout 1");
        get_system(cmd,&temp);
    } else {
        ui->table_io_output->hideColumn(5);
        iocontrol = false;
        for(int i=0;i<io_in_num;i++) {
            if(_io_map.value(signal_out[i]->text()) > 0) {
                tableButtonout *btn =
                        (tableButtonout *)ui->table_io_output->cellWidget(i,5);
                btn->setChecked(true);
                QString cmd = QString("halcmd setp conv-io.1.insim-%1 1")
                        .arg(i);
                get_system(cmd,&temp);
            } else {
                tableButtonout *btn =
                        (tableButtonout *)ui->table_io_output->cellWidget(i,5);
                btn->setChecked(false);
                QString cmd = QString("halcmd setp conv-io.1.insim-%1 0")
                        .arg(i);
                get_system(cmd,&temp);
            }
        }
        QString cmd = QString("halcmd setp conv-io.1.simflagout 0");
        get_system(cmd,&temp);
    }
}
