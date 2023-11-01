#include "iodialog.h"
#include "ui_iodialog.h"
#include "extern.h"
#include "shcom.hh"
#include "emc.hh"
#include "emc_nml.hh"
#include "rs274ngc.hh"
#include "rs274ngc_return.hh"
#include "interp_internal.hh"	// interpreter private definitions
#include "interp_queue.hh"
#include "rs274ngc_interp.hh"
#include "mywidget.h"

#include <QComboBox>
#include <QFile>
#include <QtAlgorithms>
#include <QtDebug>
#include <QTextCodec>
#include <QTimer>
#include <QtDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTableWidget>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>

IO_Dialog::IO_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IODialog)
{
    ui->setupUi(this); 

    this->setWindowTitle(tr("IOConfig"));

    //QTextCodec *code = QTextCodec::codecForName("utf-8");
    //QTextCodec::setCodecForTr(code );
    //QTextCodec::setCodecForLocale(code );
    //QTextCodec::setCodecForCStrings(code );

    InitUi();
    InitConnect();
}

QTimer *IO_Dialog::getFlushTimer()
{
    return flush_time;
}
void IO_Dialog::InitUi()
{
    filter_mini = 0;
    filter_max = 10;
    //ui->table_io_input->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    //ui->table_io_input->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->table_io_input->setColumnWidth(0,130);
    ui->table_io_input->setColumnWidth(1,200);
    ui->table_io_input->setColumnWidth(2,50);
    ui->table_io_input->setColumnWidth(3,60);
    ui->table_io_input->setColumnWidth(4,60);
    ui->table_io_input->setColumnWidth(5,70);
    ui->table_io_input->setColumnWidth(6,90);

    //ui->table_io_output->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    //ui->table_io_output->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->table_io_output->setColumnWidth(0,130);
    ui->table_io_output->setColumnWidth(1,200);
    ui->table_io_output->setColumnWidth(2,80);
    ui->table_io_output->setColumnWidth(3,80);
    ui->table_io_output->setColumnWidth(4,80);
    ui->table_io_output->setColumnWidth(5,80);


    code_choice = 0;
     group_num = 0;
     io_in_num = 0;
     io_out_num = 0;
     modified_count = 0;
     modified_polarity_count = 0;
     warnindex = 0;

     allDataInit();
     io_init();

     ui->ra_debug_code->setChecked(true);

     flush_time = new QTimer(this);
     connect(flush_time,SIGNAL(timeout()),this,SLOT(io_update()));
     flush_time->start(100);
     ui->table_io_output->hideColumn(3);

     for(int i=0;i<DEFAULT_ALM_SIG;i++) {
         alm_sig[i]  = NULL;
     }
     QStringList itemlist;
     for(int i=0;i<DEFAULT_ALM_SIG;i++) {
         QString temp;
         temp = QString("WARNING_%1").arg(i);
         alm_sig[i] = new AlarmIO();
         alm_sig[i]->info = _alarm->read(temp,"info","0");
         alm_sig[i]->polarity = _alarm->read(temp,"polarity","0").toInt();
         alm_sig[i]->exist = _alarm->read(temp,"exist","0").toInt();
         alm_sig[i]->type = strtol(_alarm->read(temp,"type","0").toLatin1().data(),NULL,16);
         itemlist.append(temp.remove("WARNING_").append(":").append(_real_alm_info[i]));
     }
     type[0] = new QTableWidgetItem(tr("forbid_run"));
     type[1] = new QTableWidgetItem(tr("Stop_and_Redlight"));
     type[2] = new QTableWidgetItem(tr("forbid_spindle"));
     type[3] = new QTableWidgetItem(tr("getAlarm_only_showinfo"));
     type[4] = new QTableWidgetItem(tr("run"));
     type[5] = new QTableWidgetItem(tr("abort_or_pause"));

     this->flushwarn();

     ui->warntypetable->setItem(0,0,type[0]);
     ui->warntypetable->setItem(1,0,type[1]);
     ui->warntypetable->setItem(2,0,type[2]);
     ui->warntypetable->setItem(3,0,type[3]);
     ui->warntypetable->setItem(4,0,type[4]);
     ui->warntypetable->setItem(5,0,type[5]);

     ui->warnlistview->addItems(itemlist);
//     readFilterData();
}
void IO_Dialog::InitConnect()
{
    connect(ui->table_io_input,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(table_text_change(int,int)));
    connect(ui->table_io_output,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(table_text_change(int,int)));

   connect(ui->btn_save,SIGNAL(clicked()),this,SLOT(io_save()));
   connect(ui->btn_cancel,SIGNAL(clicked()),this,SLOT(io_cancel()));
   connect(ui->ra_operator_code,SIGNAL(toggled(bool)),this,SLOT(switch_to_operator(bool)));
   connect(ui->ra_debug_code,SIGNAL(toggled(bool)),this,SLOT(switch_to_debug(bool)));
   connect(ui->ra_develop_code,SIGNAL(toggled(bool)),this,SLOT(switch_to_develop(bool)));

   connect(ui->warnlistview,SIGNAL(clicked(QModelIndex)),this,SLOT(setwarninfo(QModelIndex)));
   connect(ui->effenable,SIGNAL(toggled(bool)),this,SLOT(setwarnexist(bool)));
   connect(ui->porpos,SIGNAL(toggled(bool)),this,SLOT(setwarnpol(bool)));
   connect(ui->warninfo,SIGNAL(textChanged(QString)),this,SLOT(setwarntext(QString)));
   connect(ui->warntypetable,SIGNAL(itemChanged(QTableWidgetItem*)),
           this,SLOT(setwarntype(QTableWidgetItem*)));

   connect(ui->table_io_input,SIGNAL(itemChanged(QTableWidgetItem*)),
           this,SLOT(slotItemchange(QTableWidgetItem*)));
}
bool IO_Dialog::checkFilter()
{
    int row = ui->table_io_input->rowCount();
    QTableWidgetItem *item = NULL;
    for (int i = 0; i< row ;i++)
    {
        item = ui->table_io_input->item(i,5);
        QString tmp =  item->text();
        bool ok;
        tmp.toDouble(&ok);
        if(!ok)
            return false;
     }
    return true;
}

void IO_Dialog::slotItemchange(QTableWidgetItem * item) {

    if(item != NULL) {
        if(item->column() != 5) return;

        QString tmp = item->text();
        bool ok;
        tmp.toDouble(&ok);
        if(ok)
            item->setBackgroundColor(QColor(255,255,255));
         else
            item->setBackgroundColor(QColor(255,0,0));
    }
}
void IO_Dialog::allDataInit()
{

    for(int i=0;i<IO_IN_MAX_NUM;i++)
        signal_in[i] = NULL;
    for(int i=0;i<IO_OUT_MAX_NUN;i++)
        signal_out[i] = NULL;
    for(int i=0;i<IO_IN_MAX_NUM;i++)
        data_in[i] = NULL;
    for(int i=0;i<IO_OUT_MAX_NUN;i++)
        data_out[i] = NULL;

}

void IO_Dialog::checkPins()
{

    QString line;
    QFile file(_ini_filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    while (!file.atEnd()) {
        line = file.readLine();
        if(line.contains('[') &&
                line.contains(']') &&
                line.contains("IO_") &&
                !line.contains("PAR_") &&
                !line.contains('#')){

            while(!line.isEmpty()) {
                if(!line.endsWith("]"))
                    line.chop(1);
                else
                    break;
            }
            iogroup.append(line.remove(QRegExp("[\\[\\]\\n]")));
            group_num++;
        }
    }
    file.close();

    //QList<int> input_pins;
    //QList<int> output_pins;
    for(int i=0;i<iogroup.size();i++)
    {
        if(1 == _ini->read(iogroup.at(i),"SHOW").toInt())
        {
            int iotype = _ini->read(iogroup.at(i),"TYPE").toInt();
            if(iotype != 1)
                iotype = INPUT;
            else
                iotype = OUTPUT;
            switch(iotype)
            {
            case INPUT:
                _ini->read(iogroup.at(i),"PIN");
                break;
            case OUTPUT:
                break;
            default: break;
            }
        }
    }


}

void IO_Dialog::io_init()
{
    group_num = 0;
    io_in_num = 0;
    io_out_num = 0;

    QString line;
    init_io_status();

    iogroup.clear();

    input_io_pin.clear();
    input_pin_sort.clear();

    output_io_pin.clear();
    output_pin_sort.clear();

    chose_input_pin.clear();
    chose_output_pin.clear();

    out_row_info.clear();
    in_row_info.clear();


    for(int i=ui->table_io_input->rowCount();i>=0;i--)
        ui->table_io_input->removeRow(i);
    for(int i=ui->table_io_output->rowCount();i>=0;i--)
        ui->table_io_output->removeRow(i);

    for(int i=0;i<IO_IN_MAX_NUM;i++){
        chose_input_pin.append(QString("%1").arg(i,2,10,QChar('0')));
        input_io_polarity.append("0");
        recordpol[i] = 0;
    }
    for(int i=0;i<IO_OUT_MAX_NUN;i++){
        chose_output_pin.append(QString("%1").arg(i,2,10,QChar('0')));
    }


    QFile file(_ini_filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    while (!file.atEnd()) {
        line = file.readLine();
        if(line.contains('[') &&
                line.contains(']') &&
                line.contains("IO_") &&
                !line.contains("PAR_") &&
                !line.contains('#')){

            while(!line.isEmpty()) {
                if(!line.endsWith("]"))
                    line.chop(1);
                else
                    break;
            }
            iogroup.append(line.remove(QRegExp("[\\[\\]\\n]")));
            group_num++;
//            qDebug()<<"out+++"<<iogroup.at(i);
        }
    }
    file.close();


    for(int i=0;i<iogroup.size();i++)
    {
        if(1 == _ini->read(iogroup.at(i),"SHOW").toInt())
        {
//            qDebug()<<"out--"<<iogroup.at(i);
            int iotype = _ini->read(iogroup.at(i),"TYPE").toInt();
            if(iotype != 1)
                iotype = INPUT;
            else
                iotype = OUTPUT;
            switch(iotype)
            {
            case INPUT: input_io[io_in_num].signal = iogroup.at(i);
                init_io_data(i,INPUT);
                break;
            case OUTPUT: output_io[io_out_num].signal = iogroup.at(i);
                init_io_data(i,OUTPUT); break;
            default: break;
            }
        }
    }

    input_pin_sort = input_io_pin;
    qSort(input_pin_sort.begin(),input_pin_sort.end());

    output_pin_sort = output_io_pin;
    qSort(output_pin_sort.begin(),output_pin_sort.end());

    init_io_table(ui->table_io_input,io_in_num);
    init_io_table(ui->table_io_output,io_out_num);

    setTableStatus(code_choice);
}
bool IO_Dialog::saveFilter()
{
    getFilterData();

    QString ioFile = "/home/.config/io.hal";
    QString strsection;
    strsection = "HAL";
    QString line;
    QString flag = "times-filtertimebuf";
    QFile file(ioFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QString outStr;
    while (!file.atEnd()) {
        line = file.readLine();

        if(line.contains(flag) )
        {
                int pos = line.indexOf(flag) + flag.length();
                int pin = line.mid(pos+1,2).toInt();
                QString strpart0 = line.mid(0,pos + 4);
                QString strok = strpart0 + QString::number(input_io[pin].filterTime) + " \n";
                outStr.append(strok);
        }
        else
        {
            outStr.append(line);
        }

    }
    file.close();


    QFile out(ioFile);
    if (!out.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return 0;
    }
    out.write(outStr.toUtf8().data());
    out.close();
    return true;
}
bool IO_Dialog::getFilterData()
{
    int row = ui->table_io_input->rowCount();
    QTableWidgetItem *item = NULL;
    int pin = 0;
    double filterTime = 0;

    for (int i = 0; i< row ;i++)
    {
        item = ui->table_io_input->item(i,2);
        pin = item->text().toInt();

//        item = ui->table_io_input->item(i,5);
        MyLabel * lab_get_filter = (MyLabel *) ui->table_io_input->cellWidget(i,5);
        filterTime = lab_get_filter->text().toDouble();
        input_io[pin].filterTime = filterTime;
    }
    return true;
}

bool IO_Dialog::readFilterData()
{
    QString ioFile = "/home/.config/io.hal";
    QString strsection;
    strsection = "HAL";
    QString line;
    QString flag = "times-filtertimebuf";
    int pos;
    for(int i = 0; i<IO_IN_MAX_NUM ;i++)
    {
        input_io[i].filterTime = 0;
    }
    QFile file(ioFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;
    while (!file.atEnd()) {
        line = file.readLine();
        if(line.contains(flag) )
        {
            QString tmp;
            int pin=0;
            double time =0;
            line = line.trimmed();
            pos = line.indexOf(flag) + flag.length();
            for(int j = pos; j < line.length(); j++)
            {
                if(line[j] >= '0' && line[j] <= '9')
                {
                    tmp.append(line[j]);
                }
                else if(line[j] == ' ')
                {
                    pin = tmp.toInt();
                    tmp.clear();
                }
            }
            if(!tmp.isEmpty())
                time = tmp.toDouble();

            input_io[pin].filterTime = time;
        }
    }
    file.close();

    return 1;

}

void IO_Dialog::io_update()
{

    init_io_status();

    for(int i=0;i<io_in_num;i++)
    {

        data_in[i]->setText(recordpol[i]==0?QString::number(_io_map.value(
              QString("IO_%1").arg(ui->table_io_input->item(i,0)->text()))):QString::number(!_io_map.value(
                                                                              QString("IO_%1").arg(ui->table_io_input->item(i,0)->text())),10));

    }
    for(int i=0;i<io_out_num;i++)
    {
        data_out[i]->setText(QString::number(_io_map.value(
              QString("IO_%1").arg(signal_out[i]->text())),10));
    }

    static int oldcode = -1;
    if(oldcode != code_choice) {
    if(0 == code_choice) {
        ui->tabWidget->removeTab(2);
    } else if(0 < code_choice) {
        ui->tabWidget->addTab(ui->tab_3,tr("Warnning"));
        if(1 == code_choice) {
            ui->porfame->setEnabled(false);
            ui->warntypetable->setEnabled(false);
            ui->effframe->setEnabled(false);
        } else if (2 == code_choice) {
            ui->effframe->setEnabled(true);
            if(1 == alm_sig[warnindex]->exist) {
                ui->porfame->setEnabled(true);
                ui->warntypetable->setEnabled(true);
            }
        }
    }
    oldcode = code_choice;
    }

}

void IO_Dialog::manage_modify(QString name,QString value)
{
    int exist = 0;
    for(int i=0;i<MODIFIED_NUM;i++){
        if(modified_item[i].isEmpty())
            break;
        if(modified_item[i].contains(name)){
            modified_item[i].replace(1,value);
            exist = 1;
        }
    }
    if(1 != exist){
        modified_item[modified_count].append(name);
        modified_item[modified_count].append(value);
        modified_count++;
    }
}

void IO_Dialog::manage_por_modify(QString name,QString value)
{
    int exist = 0;
    for(int i=0;i<MODIFIED_NUM;i++){
        if(modified_polarity[i].isEmpty())
            break;
        if(modified_polarity[i].contains(name)){
            modified_polarity[i].replace(1,value);
            exist = 1;
        }
    }
    if(1 != exist) {
        modified_polarity[modified_polarity_count].append(name);
        modified_polarity[modified_polarity_count].append(value);
        modified_polarity_count++;
    }
}

void IO_Dialog::table_text_change(int x, int y)
{

    switch(ui->tabWidget->currentIndex())
    {
    case INPUT: if(2 == y){
            Table_Combobox combox(INPUT,x,y,this);
            combox.exec();

        } else if(3 == y){
            Table_Polarity pol(INPUT,x,y,this);
            pol.exec();
        }   break;
    case OUTPUT:if(2 == y){
            Table_Combobox combox(OUTPUT,x,y,this);
            combox.exec();
        }else if(3 == y){
/*            QComboBox *temp1_comb = (QComboBox*)ui->table_io_output->cellWidget(x,y);
            manage_por_modify(out_row_info.at(x),temp1_comb->currentText());
            if(temp1_comb->currentText() != output_io_polarity.at(x)){
                output_io_polarity.replace(x,temp1_comb->currentText());
                _polarity_change[out_row_info.at(x)] = !_polarity_change.value(out_row_info.at(x));
            }
        }
 */       }   break;
    }


}
void IO_Dialog::set_filtertime()
{
    int row = ui->table_io_input->currentRow();
    MyLabel * lab_set_filter = (MyLabel *)ui->table_io_input->cellWidget(row,5);
    QString str = lab_set_filter->text();

    int val = 0;
    MyInput input(1,(QVariant)str,(QVariant)filter_mini,(QVariant)filter_max,tr("Set filter time"),tr("filter time"),0);
    input.exec();
    val = input.val.toDouble();
    if(val >= filter_mini && val <= filter_max){
        lab_set_filter->setText(QString::number(val));
    }
}
//init the table
void IO_Dialog::init_io_table(QTableWidget *widget, int maxnum)
{
    if(widget == ui->table_io_input)
    {
        readFilterData();
        widget->setRowCount(maxnum);
        QStringList rowmanage; rowmanage.clear();
        int rowaux[maxnum];
        for(int i=0;i<maxnum;i++)
            rowaux[i] = 0;

        for(int i=0;i<maxnum;i++)
        {
            int num = input_pin_sort.indexOf(input_io[i].pin);
            if(!rowmanage.contains(QString::number(num))) {
                rowmanage.append(QString::number(num));
            } else {
                rowaux[num] += 1;
                num += rowaux[num];
            }

            signal_in[i] = new QTableWidgetItem();
            QString str = input_io[i].signal;
            signal_in[i]->setText(str.remove("IO_"));
            signal_in[i]->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *info = new QTableWidgetItem();
            info->setText(input_io[i].info);
            info->setFlags(Qt::ItemIsEnabled);

            data_in[i] = new QTableWidgetItem();
            data_in[i]->setText(QString::number(_io_map[input_io[i].signal],10));
            data_in[i]->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *io_num = new QTableWidgetItem();
            io_num->setText(QString("%1").arg(input_io[i].pin));
            io_num->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *io_polarity = new QTableWidgetItem();
            io_polarity->setText(QString("%1").arg(input_io[i].polarity));
            io_polarity->setFlags(Qt::ItemIsEnabled);
            _polarity_change[input_io[i].signal]=io_polarity->text().toInt();

            MyLabel *lab_set_filter = new MyLabel(0);
            lab_set_filter->setText(QString("%1").arg(input_io[i].filterTime));
            connect(lab_set_filter,SIGNAL(clicked()),this,SLOT(set_filtertime()));


            QTableWidgetItem *tip = new QTableWidgetItem();
            tip->setText(tr("Need Reboot"));
            tip->setFlags(Qt::ItemIsEnabled);

            widget->setItem(num,0,signal_in[i]);
            widget->setItem(num,1,info);
            widget->setItem(num,2,io_num);
            widget->setItem(num,3,io_polarity);
            widget->setCellWidget(num,5,lab_set_filter);
            widget->setItem(num,6,tip);
        }
        for(int i=0;i<maxnum;i++)
        {

            widget->setItem(i,4,data_in[i]);
            QTableWidgetItem *item = widget->item(i,0);
            if(item!=NULL)
                in_row_info.append(QString("IO_").append(widget->item(i,0)->text()));

        }

    } else if (widget == ui->table_io_output) {
        widget->setRowCount(maxnum);
        for(int i=0;i<maxnum;i++)
        {
            int num = output_pin_sort.indexOf(output_io[i].pin);

            signal_out[i] = new QTableWidgetItem();
            signal_out[i]->setText(output_io[i].signal.remove("IO_"));
            signal_out[i]->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *info = new QTableWidgetItem();
            info->setText(output_io[i].info);
            info->setFlags(Qt::ItemIsEnabled);

            data_out[i] = new QTableWidgetItem();
            data_out[i]->setText(QString::number(_io_map[output_io[i].signal],10));
            data_out[i]->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *io_num = new QTableWidgetItem();
            io_num->setText(QString("%1").arg(output_io[i].pin));
            io_num->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *io_polarity = new QTableWidgetItem();
            io_polarity->setText(QString("%1").arg(output_io[i].polarity));
            io_polarity->setFlags(Qt::ItemIsEnabled);

            QTableWidgetItem *tip = new QTableWidgetItem();
            tip->setText(tr("Need Reboot"));
            tip->setFlags(Qt::ItemIsEnabled);

            widget->setItem(num,0,signal_out[i]);
            widget->setItem(num,1,info);
            widget->setItem(num,4,data_out[i]);
            widget->setItem(num,2,io_num);
            widget->setItem(num,3,io_polarity);
            widget->setItem(num,5,tip);
        }

        for(int i=0;i<maxnum;i++)
        {
            QTableWidgetItem *item = widget->item(i,0);
            if(item!=NULL)
                out_row_info.append(QString("IO_").append(widget->item(i,0)->text()));
        }

    }

}

//this function is used to init the io struct data
void IO_Dialog::init_io_data(int i,int type)
{
    if(INPUT == type)
    {
        if(io_in_num >= IO_IN_MAX_NUM) return;
        input_io[io_in_num].info = _ini->read(iogroup.at(i),"INFO");
        input_io[io_in_num].pin.sprintf("%02d",_ini->read(iogroup.at(i),"PIN").toInt());
        input_io[io_in_num].polarity = _ini->read(iogroup.at(i),"POLARITY");
        input_io[io_in_num].type = _ini->read(iogroup.at(i),"TYPE").toInt();
        input_io_pin.append(input_io[io_in_num].pin);
        io_in_num++;
    }else if (OUTPUT == type)
    {
        if(io_out_num >= IO_OUT_MAX_NUN) return;
        output_io[io_out_num].info = _ini->read(iogroup.at(i),"INFO");
        output_io[io_out_num].polarity = _ini->read(iogroup.at(i),"POLARITY");
        output_io[io_out_num].pin.sprintf("%02d",_ini->read(iogroup.at(i),"PIN").toInt());
        output_io_pin.append(output_io[io_out_num].pin);
        output_io[io_in_num].type = _ini->read(iogroup.at(i),"TYPE").toInt();
        io_out_num++;
    }
}

void IO_Dialog::io_save()
{
    for(int i=0;i<modified_count;i++) {
        qDebug()<<modified_item[i];
        QString str = "";
        char temp[100];
        str.append(modified_item[i].at(0)).append('/').append("PIN");
        sprintf(temp,"%d",modified_item[i].at(1).toInt());
        _ini->writeEntry(str,QString(temp));
    }
    for(int i=0;i<modified_polarity_count;i++){
        QString str = "";
        str.append(modified_polarity[i].at(0)).append('/').append("POLARITY");
        _ini->writeEntry(str,modified_polarity[i].at(1));
    }

    for(int i=0;i<modified_count;i++)
        modified_item[i].clear();
    modified_count = 0;
    for(int i=0;i<modified_polarity_count;i++)
        modified_polarity[i].clear();
    modified_polarity_count = 0;

    QMessageBox::information(this, tr("Information"), tr("save success"));

    for(int i=0;i<DEFAULT_ALM_SIG;i++) {
        QString temp;
        temp = QString("WARNING_%1").arg(i);
        QString info = temp,pola = temp,exist=temp,type=temp;
        QString to16 = "";
        to16.sprintf("0x%02X",alm_sig[i]->type);
        _alarm->writeEntry(info.append("/info"),QString(alm_sig[i]->info));
        _alarm->writeEntry(pola.append("/polarity"),alm_sig[i]->polarity);
        _alarm->writeEntry(exist.append("/exist"),alm_sig[i]->exist);
        _alarm->writeEntry(type.append("/type"),to16);
    }

    if(!saveFilter())   //add by zhong for save filter time
        QMessageBox::information(this, tr("Information"), tr("save filter time failed!"));
}

void IO_Dialog::io_cancel()
{
    this->close();
}

void IO_Dialog::switch_to_operator(bool value){
    if(!value){
        return;
    }
    if(0 == code_choice)
        return;

    if(1 == code_choice) {
        code_choice = 0;
        setTableStatus(code_choice);
        code_choice = 1;
        return;
    }

    if(2 == code_choice) {
        code_choice = 0;
        setTableStatus(code_choice);
        code_choice = 2;
        return;
    }

    code_choice = 0;
    setTableStatus(code_choice);

}

int IO_Dialog::switch_to_debug(bool value){
    if(!value){
        return -1;
    }

    if(2 == code_choice) {
        code_choice = 1;
        setTableStatus(code_choice);
        code_choice = 2;
        return 0;
    }

    if(1 == code_choice) {
        setTableStatus(code_choice);
        return 0;
    }
    if(!PassWord::getCodecnc(this,1)){
        switch(code_choice){
        case 0:ui->ra_operator_code->setChecked(true);break;
        case 1:ui->ra_debug_code->setChecked(true);break;
        case 2:ui->ra_develop_code->setChecked(true);break;
        default:break;
        }
         return -1;
    }

    code_choice = 1;
    setTableStatus(code_choice);
    return 0;
}



void IO_Dialog::switch_to_develop(bool value){
    if(!value){
        return;
    }
    if(2 == code_choice) {
        setTableStatus(code_choice);
        return;
    }
    if(!PassWord::getCodecnc(this,2)){
        switch(code_choice){
        case 0:ui->ra_operator_code->setChecked(true);break;
        case 1:ui->ra_debug_code->setChecked(true);break;
        case 2:ui->ra_develop_code->setChecked(true);break;
        default:break;
        }
        return;
    }
    code_choice = 2;
    setTableStatus(code_choice);
}

void IO_Dialog::setTableStatus(int type)
{
    //io_init();
    QTableWidgetItem *item;

    if(type == 0) {
        for(int i = 0;i<ui->table_io_input->rowCount();i++) {
         item = ui->table_io_input->item(i,2);
         item->setFlags(Qt::NoItemFlags);
         item = ui->table_io_input->item(i,3);
         item->setFlags(Qt::NoItemFlags);
        }
        for(int i = 0;i<ui->table_io_output->rowCount();i++) {
         item = ui->table_io_output->item(i,2);
         item->setFlags(Qt::NoItemFlags);
        }
    } else if(type == 1) {
        for(int i = 0;i<ui->table_io_input->rowCount();i++) {
         item = ui->table_io_input->item(i,2);
         item->setFlags(Qt::NoItemFlags);
         item = ui->table_io_input->item(i,3);
         item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        }
        for(int i = 0;i<ui->table_io_output->rowCount();i++) {
         item = ui->table_io_output->item(i,2);
         item->setFlags(Qt::NoItemFlags);
        }

    } else {
        for(int i = 0;i<ui->table_io_input->rowCount();i++) {
         item = ui->table_io_input->item(i,2);
         item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
         item = ui->table_io_input->item(i,3);
         item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        }
        for(int i = 0;i<ui->table_io_output->rowCount();i++) {
         item = ui->table_io_output->item(i,2);
         item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        }
    }
}

void IO_Dialog::setwarnexist(bool val) {
    if(val) {
        alm_sig[warnindex]->exist = 1;
        ui->infoframe->setEnabled(true);
        ui->porfame->setEnabled(true);
        ui->warntypetable->setEnabled(true);
    } else {
        alm_sig[warnindex]->exist = 0;
        ui->infoframe->setEnabled(false);
        ui->porfame->setEnabled(false);
        ui->warntypetable->setEnabled(false);
    }
}
void IO_Dialog::setwarnpol(bool val) {
    if(val) {
        alm_sig[warnindex]->polarity = 1;
    } else
        alm_sig[warnindex]->polarity = 0;
}
void IO_Dialog::setwarntext(QString str) {
    alm_sig[warnindex]->info = str;
}

void IO_Dialog::setwarntype(QTableWidgetItem* item) {
    if(item->checkState() == Qt::Checked) {

        if(item == type[0]) {
            alm_sig[warnindex]->type |= 0x01;
        } else if(item == type[1]) {
            alm_sig[warnindex]->type |= (0x01 << 1) ;
        } else if(item == type[2]) {
            alm_sig[warnindex]->type |= (0x01 << 2) ;
        }  else if(item == type[3]) {
            alm_sig[warnindex]->type |= (0x01 << 3) ;
        }  else if(item == type[4]) {
            alm_sig[warnindex]->type |= (0x01 << 7) ;
        } else if(item == type[5]) {
            alm_sig[warnindex]->type |= (0x01 << 6) ;
        }
    } else {
        if(item == type[0]) {
            alm_sig[warnindex]->type &= ~0x01;
        } else if(item == type[1]) {
            alm_sig[warnindex]->type &= ~(0x01 << 1) ;
        } else if(item == type[2]) {
            alm_sig[warnindex]->type &= ~(0x01 << 2) ;
        }  else if(item == type[3]) {
            alm_sig[warnindex]->type &= ~(0x01 << 3) ;
        }  else if(item == type[4]) {
            alm_sig[warnindex]->type &= ~(0x01 << 7) ;
        } else if(item == type[5]) {
            alm_sig[warnindex]->type &= ~(0x01 << 6) ;
        }
    }

}

void IO_Dialog::setwarninfo(QModelIndex ev) {
    warnindex = ev.row();
    flushwarn();
}

void IO_Dialog::flushwarn() {
    ui->warninfo->setText(alm_sig[warnindex]->info);
    if(0 == alm_sig[warnindex]->polarity) {
        ui->porneg->setChecked(true);
        ui->porpos->setChecked(false);
    } else {
        ui->porneg->setChecked(false);
        ui->porpos->setChecked(true);
    }
    if(0 == alm_sig[warnindex]->exist) {
        ui->effdisable->setChecked(true);
        ui->effenable->setChecked(false);
        ui->infoframe->setEnabled(false);
        ui->porfame->setEnabled(false);
        ui->warntypetable->setEnabled(false);
    } else {
        ui->effdisable->setChecked(false);
        ui->effenable->setChecked(true);
        ui->infoframe->setEnabled(true);

        if (2 == code_choice) {
            ui->porfame->setEnabled(true);
            ui->warntypetable->setEnabled(true);
        } else if (1 == code_choice) {
            ui->porfame->setEnabled(false);
            ui->warntypetable->setEnabled(false);
        }
    }

    ui->warntypetable->setRowCount(6);
    ui->warntypetable->setColumnCount(1);
    ui->warntypetable->setColumnWidth(0,400);

    if((alm_sig[warnindex]->type & 0x0001) > 0)
        type[0]->setCheckState(Qt::Checked);
    else
        type[0]->setCheckState(Qt::Unchecked);

    if((alm_sig[warnindex]->type & (0x0001 << 1)) > 0)
        type[1]->setCheckState(Qt::Checked);
    else
        type[1]->setCheckState(Qt::Unchecked);

    if((alm_sig[warnindex]->type & (0x0001 << 2)) > 0)
        type[2]->setCheckState(Qt::Checked);
    else
        type[2]->setCheckState(Qt::Unchecked);


    if((alm_sig[warnindex]->type & (0x0001 << 3)) > 0)
        type[3]->setCheckState(Qt::Checked);
    else
        type[3]->setCheckState(Qt::Unchecked);


    if((alm_sig[warnindex]->type & (0x0001<< 7)) > 0)
        type[4]->setCheckState(Qt::Checked);
    else
        type[4]->setCheckState(Qt::Unchecked);


    if((alm_sig[warnindex]->type & (0x0001 << 6)) > 0)
        type[5]->setCheckState(Qt::Checked);
    else
        type[5]->setCheckState(Qt::Unchecked);


}

IO_Dialog::~IO_Dialog()
{
    delete ui;
}

void IO_Dialog::reject()
{
    this->close();
}

bool IO_Dialog::checkModidfy()
{
    if(modified_count || modified_polarity_count)
        return true;
    else
        return false;
}

void IO_Dialog::showEvent(QShowEvent *)
{

    group_num = 0;
    io_in_num = 0;
    io_out_num = 0;
    modified_count = 0;
    modified_polarity_count = 0;
    for(int i=0;i<modified_count;i++)
        modified_item[i].clear();
    for(int i=0;i<modified_polarity_count;i++)
        modified_polarity[i].clear();
    allDataInit();
    io_init();/****janmi20150810*****/
}

void IO_Dialog::closeEvent(QCloseEvent *cevt)
{
    if(checkModidfy()) {
        int ret = QMessageBox::warning(this,tr("warning"),
        tr("The value has been modified.\\nDo you realy want to exit?"),
        tr("Ok"),tr("Cancel"));
        switch (ret) {
           case Ok:
               cevt->accept();
               for(int i=0;i<modified_count;i++)
                   modified_item[i].clear();
               for(int i=0;i<modified_polarity_count;i++)
                   modified_polarity[i].clear();
               break;
           case Cancel:
               cevt->ignore();
               break;
           default:
               break;
         }

    }
}

Table_Combobox::Table_Combobox(int type1,int x,int y,IO_Dialog *io)
{
    externio = io;
    row = x;
    column = y;
    type = type1;
    QWidget *w = new QWidget(this);
    QWidget *frame1 = new QWidget(this);
    QWidget *frame2 = new QWidget(this);
    QHBoxLayout *hly=new QHBoxLayout(frame1);
    QHBoxLayout *hly2=new QHBoxLayout(frame2);
    QLabel *lab = new QLabel("                          ");
    QLabel *headtitle;
    if(type == INPUT) {
        headtitle=new QLabel(externio->ui->table_io_input->item(row,1)->text());
    } else {
        headtitle=new QLabel(externio->ui->table_io_output->item(row,1)->text());
    }
    ensure_btn = new QPushButton(tr("Ensure"));
    comb = new QComboBox();
    comb->setMinimumSize(100,25);
    comb->setMaximumSize(100,25);
    ensure_btn->setMinimumSize(80,25);
    ensure_btn->setMaximumSize(80,25);
    hly->addWidget(headtitle);
    hly->addWidget(comb);
    hly->setContentsMargins(2,2,2,2);
    hly2->addWidget(lab);
    hly2->addWidget(ensure_btn);
    hly2->setContentsMargins(2,2,2,2);
    w->setWindowTitle(tr("now chose pin"));
    QVBoxLayout *vly=new QVBoxLayout(w);
    vly->addWidget(frame1);
    vly->addWidget(frame2);
    w->setMinimumSize(280,80);
    QTableWidgetItem *item;
    if(type == INPUT) {
        keylist = externio->chose_input_pin;
        sortpin = externio->input_pin_sort;
        item = (QTableWidgetItem*)externio->ui->table_io_input->item(row,column);
    } else {
        keylist = externio->chose_output_pin;
        sortpin = externio->output_pin_sort;
        item = (QTableWidgetItem*)externio->ui->table_io_output->item(row,column);
    }
    comb->addItems(keylist);
    comb->setCurrentIndex(keylist.indexOf(item->text()));
    initstr = item->text();
    connect(comb,SIGNAL(currentIndexChanged(QString)),this,SLOT(combo_change(QString)));
    connect(ensure_btn,SIGNAL(clicked()),this,SLOT(close()));
     this->setStyleSheet("QComboBox:!editable { background-color: rgb(255, 255, 255); \
                         color: rgb(270, 100, 0);}");

}

void Table_Combobox::setcurrentindex(QString str) {
        comb->setCurrentIndex(keylist.indexOf(str));

}

void Table_Combobox::combo_change(QString str)
{
    if(str == initstr)
        return;

    QStringList sort,ioinfo;
    if(type == INPUT) {
        sort = externio->input_pin_sort;
        ioinfo = externio->in_row_info;
    }else {
        sort = externio->output_pin_sort;
        ioinfo = externio->out_row_info;
    }
    if(sort.contains(str)) {
        int ret =  QMessageBox::warning(this, tr("modify_tip"),
                                         QString(tr("the pin %1 is already exist,make sure u need to change?"))
                                        .arg(comb->currentText()),
                                         tr("Ok"),tr("cancel")
                                         );
        if(ret == Cancel){
            comb->setCurrentIndex(keylist.indexOf(initstr));
            return;
        }
        QTableWidgetItem *item1;
        if(type == INPUT) {
            item1 = (QTableWidgetItem*)externio->ui->table_io_input->item(
                        sort.indexOf(comb->currentText()),column);
        }else {
            item1 = (QTableWidgetItem*)externio->ui->table_io_output->item(
                        sort.indexOf(comb->currentText()),column);
            ioinfo = externio->out_row_info;
        }

        item1->setText(sort.at(row));
        externio->manage_modify(ioinfo.at(sort.indexOf(comb->currentText())),
                      sort.at(row));

        QString temp = sort.at(sort.indexOf(comb->currentText()));
        sort.replace(sort.indexOf(comb->currentText()),sort.at(row));
        sort.replace(row,temp);


    }else{
        if(type == INPUT) {
            sort = externio->input_pin_sort;
        }else {
            sort = externio->output_pin_sort;
        }
        sort.replace(row,comb->currentText());
    }
    QTableWidgetItem *itemnow;
    if(type == INPUT) {
        itemnow= (QTableWidgetItem*)externio->ui->table_io_input->item(row,column);
        externio->input_pin_sort = sort;
    } else {
        itemnow= (QTableWidgetItem*)externio->ui->table_io_output->item(row,column);
        externio->output_pin_sort = sort;
    }
    itemnow->setText(comb->currentText());
    externio->manage_modify(ioinfo.at(row),comb->currentText());
    initstr = comb->currentText();

}

void Table_Combobox::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

Table_Polarity::Table_Polarity(int type, int x, int y, IO_Dialog *io) {

    row = x;
    column = y;
    externio = io;
    w = new QWidget(this);
    QWidget *frame1 = new QWidget(this);
    QWidget *frame3 = new QWidget(this);
    QHBoxLayout *hly=new QHBoxLayout(frame1);
    QHBoxLayout *hly2=new QHBoxLayout(frame3);
    btnpos = new QRadioButton(tr("Pos"));
    btnneg = new QRadioButton(tr("Neg"));
    hly->addWidget(btnpos);
    hly->addWidget(btnneg);
    hly->setContentsMargins(2,2,2,2);
    QLabel *lab = new QLabel(externio->ui->table_io_input->item(row,1)->text());
    QVBoxLayout *vly=new QVBoxLayout(w);
    QLabel *lab2 = new QLabel("                 ");
    ensure_btn = new QPushButton(tr("Ensure"));
    hly2->addWidget(lab2);
    hly2->addWidget(ensure_btn);
    hly2->setContentsMargins(2,2,2,2);
    vly->addWidget(lab);
    vly->addWidget(frame1);
    vly->addWidget(frame3);
    vly->setContentsMargins(2,2,2,2);
    frame1->setMinimumSize(240,25);
    lab->setMinimumSize(240,25);
    frame3->setMinimumSize(240,25);
    w->setMinimumSize(255,120);

    QTableWidgetItem *item = (QTableWidgetItem*)externio->ui->table_io_input->item(row,column);
    if(item->text().toInt() == 0) {
        btnneg->setChecked(true);
        check = 0;
    }else{
        btnpos->setChecked(true);
        check = 1;
    }

    connect(btnpos,SIGNAL(clicked(bool)),this,SLOT(valuechange(bool)));
    connect(btnneg,SIGNAL(clicked(bool)),this,SLOT(valuechange2(bool)));
    connect(ensure_btn,SIGNAL(clicked()),this,SLOT(close()));
}


void Table_Polarity::valuechange(bool val) {

    if(val) {
        if(check == 0) {
        externio->manage_por_modify(externio->in_row_info.at(row),"1");
        QTableWidgetItem *item = (QTableWidgetItem*)externio->ui->table_io_input->item(row,column);
        item->setText("1");
        externio->input_io_polarity.replace(row,item->text());
        _polarity_change[externio->in_row_info.at(row)] = 1;
        externio->recordpol[row] = !externio->recordpol[row];
            check = 1;
        }

    }
}

void Table_Polarity::valuechange2(bool val) {

    if(val) {
        if(check == 1) {
        externio->manage_por_modify(externio->in_row_info.at(row),"0");
        QTableWidgetItem *item = (QTableWidgetItem*)externio->ui->table_io_input->item(row,column);
        item->setText("0");
        externio->input_io_polarity.replace(row,item->text());
        _polarity_change[externio->in_row_info.at(row)] = 0;
        externio->recordpol[row] = !externio->recordpol[row];
        check = 0;
        }
    }
}

