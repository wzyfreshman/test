#include "info_tabledialog.h"
#include "ui_info_tabledialog.h"
#include "extern.h"

#include <QtDebug>
#include <QMenu>

Info_TableDIalog::Info_TableDIalog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info_TableDIalog)
{
    ui->setupUi(this);

    act_clear = new QAction(tr("clean"),this);
    connect(act_clear,SIGNAL(triggered()),this,SLOT(clear_table()));
    //resize the table,auto changeLine
    ui->info_table->setWordWrap(true);
    //ui->info_table->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
   // ui->info_table->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

    QStringList header ;
    header.clear();
    header.append(tr("Type"));header.append(tr("Info"));
    header.append(tr("Time"));
    ui->info_table->setHorizontalHeaderLabels(header);

    icon_error =  QIcon(":/std/std_error.gif");
    icon_warning =  QIcon(":/std/std_warning.gif");
    icon_info =  QIcon(":/std/std_info.gif");

    ui->btn_system_info->setChecked(true);
    ui->btn_warning_info->setChecked(true);
    ui->btn_alarm_info->setChecked(true);

    connect(ui->btn_system_info,SIGNAL(clicked()),this,SLOT(classify_display()));
    connect(ui->btn_warning_info,SIGNAL(clicked()),this,SLOT(classify_display()));
    connect(ui->btn_alarm_info,SIGNAL(clicked()),this,SLOT(classify_display()));
    connect(ui->btn_clear,SIGNAL(clicked()),this,SLOT(clear_table()));

}

void Info_TableDIalog::insert_info_line(int type,QString info)
{

    ui->info_table->insertRow(0);

    QTableWidgetItem *typeTips = new QTableWidgetItem();
    typeTips->setText(QString::number(type,10));

    QTableWidgetItem *typeItem = new QTableWidgetItem();
    if(type == 2) {
       typeItem->setIcon(icon_error);
       typeTips->setText("2");
    } else if(type == 1) {
       typeItem->setIcon(icon_warning);
       typeTips->setText("1");
    } else {
       typeItem->setIcon(icon_info);
       typeTips->setText("0");
    }
    typeItem->setFlags(Qt::ItemIsEnabled);
    QTableWidgetItem *newItem = new QTableWidgetItem();
    newItem->setText(info);
    newItem->setFlags(Qt::ItemIsEnabled);


    QTableWidgetItem *time = new QTableWidgetItem();
    time->setText(_time_now);
    time->setFlags(Qt::ItemIsEnabled);
    ui->info_table->setItem(0,0,typeItem);
    ui->info_table->setItem(0,1,newItem);
    ui->info_table->setItem(0,2,time);
    ui->info_table->setItem(0,3,typeTips);

    classify_display();
}

void Info_TableDIalog::reject()
{

}

void Info_TableDIalog::resizeEvent(QResizeEvent *)
{

    int w = width()-66-180;
    if(w<200)
        w = 200;
    ui->info_table->setColumnWidth(0,50);
    ui->info_table->setColumnWidth(1,w);
    ui->info_table->setColumnWidth(2,170);
    ui->info_table->setColumnWidth(3,0);

}

void Info_TableDIalog::contextMenuEvent(QContextMenuEvent *)
{
//    QCursor cur = this->cursor();
//    QMenu *menu = new QMenu();
//    menu->addAction(act_clear);
//    menu->exec(cur.pos());
}

QTableWidget *Info_TableDIalog::getTableWidget()
{
    return ui->info_table;
}

void Info_TableDIalog::clear_table()
{
//    ui->info_table->clear();

    for(int i=ui->info_table->rowCount()-1;i>0;i--) {
        for(int j=ui->info_table->colorCount()-1;j>0;j--) {
          delete ui->info_table->takeItem(i, j);
        }
    }
    for(int i=ui->info_table->rowCount();i>0;i--)
        ui->info_table->removeRow(0);

}

void Info_TableDIalog::classify_display(){

    int num ;
    for(int i=ui->info_table->rowCount()-1;i>=0;i--){
        num = ui->info_table->item(i,3)->text().toInt();
        if(0 == num){
            if(ui->btn_system_info->isChecked())
                ui->info_table->setRowHidden(i,false);
            else
                ui->info_table->setRowHidden(i,true);
        }else if(1 == num){
            if(ui->btn_warning_info->isChecked())
                ui->info_table->setRowHidden(i,false);
            else
                ui->info_table->setRowHidden(i,true);
        }else if(2 == num){
            if(ui->btn_alarm_info->isChecked())
                ui->info_table->setRowHidden(i,false);
            else
                ui->info_table->setRowHidden(i,true);
        }
    }

//    ui->info_table->hideRow(0);




}


Info_TableDIalog::~Info_TableDIalog()
{
    delete ui;
}
