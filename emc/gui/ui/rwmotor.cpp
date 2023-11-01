#include "rwmotor.h"
#include "ui_rwmotor.h"

#include <unistd.h>

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTableWidgetItem>
#include <QtDebug>
#include <QMessageBox>
#include "mywidget.h"
#include "extern.h"

RWMOTOR::RWMOTOR(QWidget *parent, int setaxis) :
    QWidget(parent),
    ui(new Ui::RWMOTOR)
{
    ui->setupUi(this);
    axis = setaxis;
    readfile();
    initTable();

}

RWMOTOR::~RWMOTOR()
{
    delete ui;
}

int RWMOTOR::readfile()
{
    motor_chose = _ini->read("PAR_EMCMOT_MOTOR_CHOSE" ,"VALUE","0").toInt();
    QString filepath = QString("/home/.config/%1/MotorPar").arg(motor_chose);
    QFile file(filepath);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return -1;

     QTextStream in(&file);
     int i = 0;
     parcount = 0;
     while (!in.atEnd()) {
         if(i < MAXPARNUM) {
             QString line = in.readLine();
             QStringList items = line.split(" ");
             if(items.count() >= 6) {
             data[i].index = items.at(0);
             data[i].subindex = items.at(1);
             data[i].datatype = items.at(2);
             data[i].detial = items.at(3);
             if(data[i].datatype.contains("int")) {
                data[i].min = items.at(4).toInt();
                data[i].max = items.at(5).toInt();
             } else {
                 data[i].min = items.at(4).toDouble();
                 data[i].max = items.at(5).toDouble();
             }
             parcount++;
             i++;
             }
         }
     }
     file.close();

}

void RWMOTOR::initTable()
{
    QString cmdret = "";
    get_system("sudo ethercat slaves",&cmdret);
    qDebug()<<cmdret;
    if(cmdret.contains("ERROR")) {
        QMessageBox::warning(this, tr("Info"),
                                             tr("Now Motor communication unwork,reset it"),
                                             tr("Ok"));
    }

    modifyList.clear();
    ui->partable->setRowCount(parcount);
    ui->partable->setColumnCount(5);
    ui->partable->clear();
    for(int i = 0;i<parcount;i++) {
        QTableWidgetItem *Index = new QTableWidgetItem();
        QString realid = "";
        realid.append(data[i].index);

        Index->setText(QString("P%1").arg(realid));
        ui->partable->setItem(i,0,Index);


        QTableWidgetItem *Subindex = new QTableWidgetItem();
        QString realin = "";
        int realsubindex = data[i].subindex.toInt(0,16);

        realin.sprintf("%.2d",realsubindex);
        Subindex->setText(QString("P%1").arg(realid));

        ui->partable->setItem(i,1,Subindex);

        QTableWidgetItem *Type = new QTableWidgetItem();
        Type->setText(QString("%1").arg(data[i].datatype));
        ui->partable->setItem(i,2,Type);


        QString cmd = "";
        if(data[i].datatype.toLower().compare("uint16") == 0 || \
                data[i].datatype.toLower().compare("uint32") == 0 || \
                data[i].datatype.toLower().compare("uint8") == 0) {
            cmd.append("sudo ethercat upload").append(QString(" -p%1 -t").arg(axis)).append(data[i].datatype.toLower()).\
                    append(" 0x").append(QString("%1").arg(data[i].index)).append(" ").append(QString("%1").arg(data[i].subindex));
        }
        QString ret = "";
        get_system(cmd,&ret);
        data[i].value = ret.split(" ").at(1);
        qDebug()<<cmd<< data[i].value;



        QTableWidgetItem *Value = new QTableWidgetItem();
        Value->setText(QString("%1").arg(data[i].value));
        Value->setFlags(Qt::ItemIsEnabled);
        ui->partable->setItem(i,3,Value);


        QTableWidgetItem *Detial = new QTableWidgetItem();
        Detial->setText(QString("%1").arg(data[i].detial));
        ui->partable->setItem(i,4,Detial);

    }
    qDebug()<<"Init table end";
    ui->partable->setHorizontalHeaderLabels(QStringList() <<tr("ID1")<<tr("ID")<<tr("TYPE")
                                      <<tr("VALUE")<<tr("DETAIL"));
    ui->partable->setColumnWidth(0,50);
    ui->partable->setColumnWidth(1,80);
    ui->partable->setColumnWidth(2,100);
    ui->partable->setColumnWidth(3,80);
    ui->partable->setColumnWidth(4,180);
    ui->partable->hideColumn(0);
    ui->partable->hideColumn(2);
    //隐藏首行
    ui->partable->setRowHidden(0,1);


}

void RWMOTOR::on_partable_doubleClicked(const QModelIndex &index)
{
    qDebug()<<index.row()<<index.column();
    if(index.column() != 3)
        return;
    QString getRet;
    if(data[index.row()].datatype.contains("int")) {
        MyInput input(0,(QVariant)data[index.row()].value.toInt(),(QVariant)data[index.row()].min,\
                (QVariant)data[index.row()].max,data[index.row()].detial,tr(""),0);
        input.exec();
        getRet = input.val.toString();
        if(getRet.toInt() > data[index.row()].max.toInt() || \
                getRet.toInt() < data[index.row()].min.toInt() ||\
                getRet.toInt() == data[index.row()].value.toInt()) {
            qDebug()<<"Out of range";
            return;
        }
    } else {
        MyInput input2(1,(QVariant)data[index.row()].value.toInt(),(QVariant)data[index.row()].min,\
                (QVariant)data[index.row()].max,data[index.row()].detial,tr(""),0);
        input2.exec();
        getRet = input2.val.toString();
        if(getRet.toDouble() > data[index.row()].max.toDouble() || \
                getRet.toDouble() < data[index.row()].min.toDouble() ||\
                getRet.toDouble() == data[index.row()].value.toDouble()) {
            qDebug()<<"Out of range";
            return;
        }
    }
    qDebug()<<getRet;
    for(int i=0;i<modifyList.count();i++) {
        if(modifyList.at(i).toInt() == index.row()) {
            ui->partable->item(index.row(),index.column())->setText(getRet);
            data[index.row()].newvalue = getRet;
            return;
        }
    }
    ui->partable->item(index.row(),index.column())->setText(getRet);
    modifyList.append(QString("%1").arg(index.row()));
    data[index.row()].newvalue = getRet;
    return;
}

void RWMOTOR::ensure_clicked()
{
    qDebug()<<modifyList;
    if(modifyList.count() == 0)
        return;
    QString cmd = "";
    QString ret = "";

    QString cmdret = "";
    get_system("sudo ethercat slaves",&cmdret);
    qDebug()<<cmdret;
    if(cmdret.contains("ERROR")) {
        QMessageBox::warning(this, tr("Info"),
                                             tr("Now Motor communication unwork,reset it"),
                                             tr("Ok"));
        return ;
    }

    system("sudo ethercat states SAFEOP");
    sleep(1);
    for(int i=0;i<modifyList.count();i++) {
            cmd = "ethercat download";
            cmd.append(QString(" -p%1 -t").arg(axis));
            cmd.append(data[modifyList.at(i).toInt()].datatype).\
                    append(QString(" 0x%1 ").arg(data[modifyList.at(i).toInt()].index))\
                    .append(QString(" %1 ").arg(data[modifyList.at(i).toInt()].subindex))\
                    .append(data[modifyList.at(i).toInt()].newvalue);
            system(cmd.toLatin1());
            //get_system(cmd,&ret);
            qDebug()<<cmd<<ret;

    }
    //保存参数，重启生效
    cmd = "ethercat download";
    cmd.append(QString(" -p%1 -t").arg(axis));
    cmd.append(data[0].datatype).\
            append(QString(" 0x%1 ").arg(data[0].index))\
            .append(data[0].subindex)\
            .append(QString(" %1 ").arg(data[0].max.toInt()));
    system(cmd.toLatin1());
    qDebug()<<cmd;
}


MULTIMOTOR::MULTIMOTOR(QWidget *parent)
{
    QWidget *mainframe;
    QWidget *btnframe;
    QPushButton *ensurebtn,*abortbtn;
    for(int i = 0;i<AXISNUM;i++) {
            axis[i] = NULL;
    }

    btn_axis = new QPushButton("设置轴");
    spaxisnum = new QSpinBox();
    choseaxis = new QWidget();
    QHBoxLayout *btnaxislay = new QHBoxLayout(choseaxis);
    btnaxislay->addWidget(spaxisnum);
    btnaxislay->addWidget(btn_axis);
    btnaxislay->setContentsMargins(2,2,2,2);

    ensurebtn = new QPushButton(tr("Ensure"));
    abortbtn = new QPushButton(tr("Abort"));
    btnframe = new QWidget();
    QHBoxLayout *btnlay = new QHBoxLayout(btnframe);
    btnlay->addWidget(ensurebtn);
    btnlay->addWidget(abortbtn);
    btnlay->setContentsMargins(2,2,2,2);

    tab = new QTabWidget();
    tab->setMinimumWidth(400);
    tab->setMinimumHeight(450);

    mainframe = new QWidget(this);
    QVBoxLayout *mainlay = new QVBoxLayout(mainframe);
    mainlay->addWidget(choseaxis);
    mainlay->addWidget(tab);
    mainlay->addWidget(btnframe);
    mainlay->setContentsMargins(2,2,2,2);

    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->addWidget(mainframe);
    lay->setContentsMargins(0,0,0,0);

    connect(ensurebtn,SIGNAL(clicked()),this,SLOT(ensure()));
    connect(abortbtn,SIGNAL(clicked()),this,SLOT(quit()));
    connect(btn_axis,SIGNAL(clicked()),this,SLOT(initAxisData()));
}

void MULTIMOTOR::ensure()
{
    int currentaxis = spaxisnum->value();
//    for(int i = 0;i<AXISNUM;i++) {
        axis[currentaxis]->ensure_clicked();
//    }
}

void MULTIMOTOR::quit()
{
    this->close();
}

void MULTIMOTOR::initAxisData()
{
        int currentaxis = spaxisnum->value();
        if(axis[currentaxis] == NULL)
            axis[currentaxis] = new RWMOTOR(this,currentaxis);
        tab->removeTab(0);
        tab->insertTab(0,axis[currentaxis],QString(tr("MT%1")).arg(currentaxis+1));

}
