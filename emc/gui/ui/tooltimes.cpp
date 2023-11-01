#include "tooltimes.h"
#include "ui_tooltimes.h"
#include "extern.h"
#include <QShowEvent>
#include <QTimer>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QtDebug>
#include <QModelIndex>
#include <QFile>
#include <QTextStream>

#include "emcstatus.hh"
#include "shcom.hh"
#include "mywidget.h"

tooltimes::tooltimes(QWidget *parent, int current1, linuxcnc *lcnc, int current2) :
    QDialog(parent),
    ui(new Ui::tooltimes)
{
    ui->setupUi(this);
    cnc = lcnc;
    this->setWindowTitle(tr("Tool Length Manager"));
    toolworkpiece[0] = _prefer->read("DEFAULT","tool_1_savepiece","0",true).toInt();
    toolworkpiece[1] = _prefer->read("DEFAULT","tool_2_savepiece","0",true).toInt();
    ui->tool1current->setValue(toolworkpiece[0]);
    ui->tool2current->setValue(toolworkpiece[1]);

    toolmaxnum[0] = _prefer->read("DEFAULT","tool_1_piece","0",true).toInt();
    toolmaxnum[1] = _prefer->read("DEFAULT","tool_2_piece","0",true).toInt();
    ui->tool1num->setValue(toolmaxnum[0]);
    ui->tool2num->setValue(toolmaxnum[1]);



    init_tbl();

    QTimer *flushtimer = new QTimer(this);
    connect(flushtimer,SIGNAL(timeout()),this,SLOT(update()));
    flushtimer->start(100);

    this->setMaximumHeight(160);
    this->setMinimumHeight(160);
    this->resize(this->width(),160);

    if(par_get_var(4102) > 0) {
        ui->usetbl->setChecked(true);        
        ui->toolManageTbl->setEnabled(true);
    } else {
        ui->usetbl->setChecked(false);
        ui->toolManageTbl->setEnabled(false);

    }

    ui->frame->setHidden(true);
    this->setMaximumHeight(500);
    this->setMinimumHeight(500);
    this->resize(this->width(),500);

    if(!_doublez) {
        ui->label_2->setHidden(true);
        ui->label_4->setHidden(true);
        ui->tool2current->setHidden(true);
        ui->tool2num->setHidden(true);
        toolworkpiece[1] = 0;
        toolmaxnum[1] = 0;
    }
}

tooltimes::~tooltimes()
{
    delete ui;
}

void tooltimes::update() {
    static int old_work_piece = get_task_work_piece();
        if(old_work_piece != get_task_work_piece()) {
//            qDebug()<<"in the work change +++++++++++++++++++++janmi++++++++++++++++++++++";


            if(par_get_var(4102) > 0) {
                if(cnc->work_piece == 0)
                    updatetbl(0,1);             //reset the tblfile while I clear the work-piece
                else {
                    for(int i=0;i<MAXCOLS;i++) {
                        //qDebug()<<cnc->work_piece <<tblData[0][i].toInt() <<i;
                        if(get_task_z() == 0 ||get_task_z() == 2 ) {
                            if(toolworkpiece[0]+1 == tblData[0][i].toInt()) {
                                updatetbl(i,0);
                            }
                        } else if(get_task_z() == 1) {
                            if(toolworkpiece[1]+1 == tblData[0][i].toInt()) {
                                updatetbl(i,0);
                            }
                        }
                    }
                }
            }

            if(2 == get_task_z() && (_prefer->read("DEFAULT","tool_1_piece","0",true).toInt()>0?((toolworkpiece[0]+1) % _prefer->read("DEFAULT","tool_1_piece","0",true).toInt() == 0 ):false)&&(_prefer->read("DEFAULT","tool_2_piece","0",true).toInt()>0? ((toolworkpiece[1]+1) % _prefer->read("DEFAULT","tool_2_piece","0",true).toInt() == 0):false)) {
                int repeat = 0;
                if(0 == cnc->externabortfunc) {
                    if(get_autorepeat())
                        repeat = 1;
                    cnc->pro_abort();
                } else {
                    if(get_task_mode() == EMC_TASK_MODE_AUTO)
                        cnc->pro_pause();
                }
                int ret =  QMessageBox::warning(this, tr("Info"),
                            tr("Now tool should be changed,Press Ok to change tool"),
                                                 tr("Ok"),tr("Cancel"));
                if(ret == Cancel) {
                    toolworkpiece[0]++ ;
                    toolworkpiece[1]++ ;
                    if(repeat) {
                        sendAutoRepeat(true);
                    }
                    cnc->pro_run();
                }
                if(ret == Ok) {
                    toolworkpiece[0] = 0;
                    toolworkpiece[1] = 0;
                    cnc->pro_abort();
                }

                qDebug()<<"in the externabortfunc +++++++++++++++++++++janmi++++++++++++++++++++++";

            } else {
                if(1 != get_task_z()) {
                    toolworkpiece[0]++ ;
                    if(_prefer->read("DEFAULT","tool_1_piece","0",true).toInt() > 0) {
                    _prefer->writeEntry("DEFAULT/tool_1_savepiece",QString::number(toolworkpiece[0],10));
                    if(toolworkpiece[0] % _prefer->read("DEFAULT","tool_1_piece","0",true).toInt() == 0) {
                        int repeat = 0;
                        if(0 == cnc->externabortfunc) {
                            if(get_autorepeat())
                                repeat = 1;
                            cnc->pro_abort();

                        } else {
                            if(get_task_mode() == EMC_TASK_MODE_AUTO)
                                cnc->pro_pause();
                        }
                        int ret =  QMessageBox::warning(this, tr("Info"),
                                    tr("Now tool1 should be changed,Press Ok to change tool"),
                                                         tr("Ok"),tr("Cancel"));
                        if(ret == Cancel) {
                            if(repeat) {
                                sendAutoRepeat(true);
                            }
                            cnc->pro_run();
                        }
                        if(ret == Ok) {
                            toolworkpiece[0] = 0;
                            cnc->pro_abort();
                        }
                    }
                    }
                }
                if(0 != get_task_z()) {
                    toolworkpiece[1]++ ;
                    if(_prefer->read("DEFAULT","tool_2_piece","0",true).toInt() > 0) {
                    _prefer->writeEntry("DEFAULT/tool_2_savepiece",QString::number(toolworkpiece[1],10));
                    if(toolworkpiece[1] % _prefer->read("DEFAULT","tool_2_piece","0",true).toInt() == 0) {
                        int repeat = 0;
                        if(0 == cnc->externabortfunc) {
                            if(get_autorepeat())
                                repeat = 1;
                            cnc->pro_abort();
                        } else {
                            if(get_task_mode() == EMC_TASK_MODE_AUTO)
                                cnc->pro_pause();
                        }
                        int ret =  QMessageBox::warning(this, tr("Info"),
                                    tr("Now tool2 should be changed,Press Ok to change tool"),
                                                         tr("Ok"),tr("Cancel"));
                        if(ret == Cancel) {
                            if(repeat) {
                                sendAutoRepeat(true);
                            }
                            cnc->pro_run();
                        }
                        if(ret == Ok) {
                            toolworkpiece[1] =0;
                            cnc->pro_abort();
                        }
                    }
                    }
                }
            }
            ui->tool1current->setValue(toolworkpiece[0]);
            ui->tool2current->setValue(toolworkpiece[1]);

            _prefer->writeEntry("DEFAULT/tool_1_savepiece",QString::number(toolworkpiece[0],10));
            _prefer->writeEntry("DEFAULT/tool_2_savepiece",QString::number(toolworkpiece[1],10));


        }
        old_work_piece= get_task_work_piece();

        static int oldworkpiece1 = -1,oldworkpiece2 = -1;
        if(oldworkpiece1 != emcStatus->motion.mot_extern_in[0]) {
                if(emcStatus->motion.mot_extern_in[0] > 0 && toolmaxnum[0] > 0) {
                        send_msg(0,tr("Now clear the workstation1 pieces"));
                    toolworkpiece[0] = 0;
                     ui->tool1current->setValue(0);
                }
                oldworkpiece1 = emcStatus->motion.mot_extern_in[0];
            }

            if(oldworkpiece2 != emcStatus->motion.mot_extern_in[1]) {
                if(emcStatus->motion.mot_extern_in[1] > 0 && toolmaxnum[1] > 0) {
                        send_msg(0,tr("Now clear the workstation2 pieces"));
                    toolworkpiece[1] = 0;
                    ui->tool2current->setValue(0);
                }
                oldworkpiece2 = emcStatus->motion.mot_extern_in[1];
             }

}

void tooltimes::init_tbl() {
    ui->toolManageTbl->verticalHeader()->setVisible(false);
    ui->toolManageTbl->horizontalHeader()->setVisible(false);
    ui->toolManageTbl->clear();
    ui->toolManageTbl->setRowCount(MAXROWS);
    ui->toolManageTbl->setColumnCount(MAXCOLS);
    QString str = "",str1="";


    QTableWidgetItem *headerNo = new QTableWidgetItem(tr("T\\Piece"));
    ui->toolManageTbl->setItem(0,0,headerNo);
    ui->toolManageTbl->setColumnWidth(0,90);
    for(int j=1;j<MAXCOLS;j++) {
        ui->toolManageTbl->setColumnWidth(j,60);
        str.sprintf("piecelevel%d",j);
        QString temp = _prefer->read("PIECEMANAGER",str,"0",true);
        headlist.append(temp);
        tblData[0][j] = temp;
        dataItem[0][j] = new QTableWidgetItem(tblData[0][j]);
        ui->toolManageTbl->setItem(0,j,dataItem[0][j]);
        dataItem[0][j]->setFlags(Qt::ItemIsEnabled);
    }
    for(int j = 0;j<MAXCOLS;j++) {
        for(int i=1;i<MAXROWS;i++) {
            if(j != 0) {
                str.sprintf("compval%d",j);
                str1.sprintf("TOOLMANAGER%d",i);
                tblData[i][j] = _prefer->read(str1,str,"0",true);
            } else {
                tblData[i][j] = QString::number(i,10);
            }
            dataItem[i][j] = new QTableWidgetItem(tblData[i][j]);
            ui->toolManageTbl->setItem(i,j,dataItem[i][j]);
            dataItem[i][j]->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        }
    }

    connect(ui->toolManageTbl,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(setvalue(int,int)));
    //connect(ui->toolManageTbl->horizontalHeader(),SIGNAL(sectionDoubleClicked(int)),this,SLOT(setheader(int)));
}

void tooltimes::setvalue(int x,int y) {
    if(y==0)
        return;
    if(x == 0) {
        MyInput input(0,(QVariant)tblData[x][y].toInt(),(QVariant)0,(QVariant)10000,tr("Input piece num"),tr("Set piece"),0);
        input.exec();
        int val = input.val.toInt();
        if(val >= 0 && val <= 10000){
            tblData[x][y] = QString::number(val,10);
            QString str = "";
            str.sprintf("PIECEMANAGER/piecelevel%d",y);
            _prefer->writeEntry(str,QString::number(val,10));
            ui->toolManageTbl->item(x,y)->setText(tblData[x][y]);
        }
    } else {
        MyInput input(1,(QVariant)tblData[x][y],(QVariant)-100,(QVariant)100,tr("Input  comp value"),tr("Set piece"),0);
        input.exec();
        double val = input.val.toDouble();
        if(val >= -100 && val <= 100){
            tblData[x][y] = QString::number(val,'f',3);
            QString str = "";
            str.sprintf("TOOLMANAGER%d/compval%d",x,y);
            _prefer->writeEntry(str,tblData[x][y]);
            ui->toolManageTbl->item(x,y)->setText(tblData[x][y]);
        }
    }
    qDebug()<<"Now Set X Y"<<x<<y<<tblData[x][y];
}
void tooltimes::setheader(int index){
    MyInput input(0,(QVariant)headlist.at(index).toInt(),(QVariant)0,(QVariant)10000,tr("Input piece num"),tr("Set piece"),0);
    input.exec();
    int val = input.val.toInt();
    if(val >= 0 && val <= 10000){
        headlist.replace(index,QString::number(val,10));
        ui->toolManageTbl->setHorizontalHeaderLabels(headlist);
        QString str = "";
        str.sprintf("PIECEMANAGER/piecelevel%d",index);
        _prefer->writeEntry(str,QString::number(val,10));
    }
    qDebug()<<"Now Set header"<<index<<headlist;
}

void tooltimes::on_pushButton_clicked()
{
    _prefer->writeEntry("DEFAULT/tool_1_piece",QString::number(ui->tool1num->value(),10));
    _prefer->writeEntry("DEFAULT/tool_1_savepiece",QString::number(ui->tool1current->value(),10));
    toolworkpiece[0] = ui->tool1current->value();
    _prefer->writeEntry("DEFAULT/tool_2_piece",QString::number(ui->tool2num->value(),10));
    _prefer->writeEntry("DEFAULT/tool_2_savepiece",QString::number(ui->tool2current->value(),10));
    toolworkpiece[1] = ui->tool2current->value();

    toolmaxnum[0] = ui->tool1num->value();
    toolmaxnum[1] = ui->tool2num->value();

    int ret =  QMessageBox::information(this, tr("Info"),
                tr("Tool manager parameter save success!"),
                                     tr("Ok"));
}

void tooltimes::on_pushButton_2_clicked()
{
    this->close();
}

void tooltimes::showEvent(QShowEvent *) {
    ui->tool1current->setValue(toolworkpiece[0]);
    ui->tool2current->setValue(toolworkpiece[1]);
    toolmaxnum[0] = _prefer->read("DEFAULT","tool_1_piece","0",true).toInt();
    toolmaxnum[1] = _prefer->read("DEFAULT","tool_2_piece","0",true).toInt();
    ui->tool1num->setValue(toolmaxnum[0] );
    ui->tool2num->setValue(toolmaxnum[1] );
}

void tooltimes::on_usetbl_clicked(bool checked)
{
    if(checked) {
        sendSetParameter(4102,1);
        ui->toolManageTbl->setEnabled(true);
    } else {
        sendSetParameter(4102,0);
        ui->toolManageTbl->setEnabled(false);

    }
}

void tooltimes::updatetbl(int index, int clearz) {
    QString nn = _ini->readEntry("PAR_EMCIO_TOOL_TABLE/VALUE","tool.tbl");
    QFileInfo info(nn);
    QString final = info.absoluteDir().absolutePath();
    final = QString("%1/%2").arg(final).arg(nn);
    QFile file(final);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;
    QStringList textlist;
     while (!file.atEnd()) {
         textlist.append(file.readLine());
     }
     file.close();

     QFile file2(final);
     if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))
              return;

     QTextStream out(&file2);

     for(int i=0;i<textlist.size();i++) {
         QStringList list = textlist.at(i).split(" ");
         if(clearz == 0) {
            list.replace(3,QString("Z%1").arg(tblData[i+1][index]));
            list.replace(4,QString("A%1").arg(tblData[i+1][index]));
         } else {
             list.replace(3,QString("Z%1").arg(0));
             list.replace(4,QString("A%1").arg(0));
         }
         for(int j=0;j<list.count()-1;j++) {
             out<<list.at(j)<<" ";
         }
         out<<list.at(list.count()-1);
     }
     file2.close();
     if(clearz == 0) {
        sendMultiFunction(99,0,0);
        send_msg(0,tr("Now Change the ToolLength as the Table Set"));
     } else {
         sendLoadToolTable(final.toUtf8().data());
         send_msg(0,tr("Now Clear the ToolLength offset"));
     }
}

void tooltimes::reject(){

}
