#include "compensation.h"
#include "ui_compensation.h"
#include "array.h"

#include <QFile>
#include <QtDebug>
#include <QString>

compensation::compensation(QDialog *parent, int array, int setrow, int setcolumn, int row, int column) :
    QDialog(parent),
    ui(new Ui::compensation)
{
    ui->setupUi(this);

    totalrow = row;
    totalcolumn = column;
    setrownum = setrow;
    setcolumnnum = setcolumn;
    for(int j = 0;j<MAXARRAY;j++) {
        for(int i=0;i<MAXTOOL;i++) {
            tool[j][i] = 0;
            xcomp[j][i] = 0;
            ycomp[j][i] = 0;
        }
    }
    for(int i=0;i<MAXTOOL;i++) {
        toolname[i] = NULL;
        toolxcomp[i] = NULL;
        toolycomp[i] = NULL;
    }
    arraynum = 0;
    compfile = ARRAY_COMPENSATION_FILE;
    num = array;
    readComp();
    inittable();

    connect(ui->btn_ensure,SIGNAL(clicked()),this,SLOT(savefile()));
    connect(ui->btn_cancel,SIGNAL(clicked()),this,SLOT(close()));
}

void compensation::savefile() {
    QFile file(compfile);
    if(!file.open(QIODevice::WriteOnly))
        return;
    qDebug()<<"write";
    QTextStream out(&file);
    if(-1 == totalrow && -1 == totalcolumn) {
        for(int j=0;j<arraynum;j++) {
            if(j == num) {
                out<<QString("[%1]\n").arg(j);
                for(int i=0;i<toolnum;i++) {
                    out<<toolname[i]->text()<<" "<<toolxcomp[i]->value()
                      <<" "<<toolycomp[i]->value()<<"\n";
                }
            } else {
                out<<QString("[%1]\n").arg(j);
                for(int i=0;i<toolnum;i++) {
                    out<<tool[j][i]<<" "<<xcomp[j][i]<<" "<<ycomp[j][i]<<"\n";
                }
            }
        }
    } else {
         for(int j=0;j<arraynum;j++) {
             if(totalrow != -1) {
                 if(j/setcolumnnum == totalrow) {
                     out<<QString("[%1]\n").arg(j);
                     for(int i=0;i<toolnum;i++) {
                         out<<toolname[i]->text()<<" "<<toolxcomp[i]->value()
                           <<" "<<toolycomp[i]->value()<<"\n";
                     }
                 } else {
                     out<<QString("[%1]\n").arg(j);
                     for(int i=0;i<toolnum;i++) {
                         out<<tool[j][i]<<" "<<xcomp[j][i]<<" "<<ycomp[j][i]<<"\n";
                     }
                 }
             } else if(totalcolumn != -1) {
                 if(j%setcolumnnum == totalcolumn) {
                     out<<QString("[%1]\n").arg(j);
                     for(int i=0;i<toolnum;i++) {
                         out<<toolname[i]->text()<<" "<<toolxcomp[i]->value()
                           <<" "<<toolycomp[i]->value()<<"\n";
                     }
                 } else {
                     out<<QString("[%1]\n").arg(j);
                     for(int i=0;i<toolnum;i++) {
                         out<<tool[j][i]<<" "<<xcomp[j][i]<<" "<<ycomp[j][i]<<"\n";
                     }
                 }
             }
         }
    }
    file.close();
}

void compensation::inittable() {
    //qDebug()<<"int toottable"<<num;
    this->setWindowTitle(tr("Array:%1").arg(num));
    ui->comptable->setRowCount(toolnum);
    for(int i=0;i<toolnum;i++) {
        toolname[i] = new QTableWidgetItem();
        toolname[i]->setText(QString("%1").arg(tool[num][i]));
        toolname[i]->setFlags(Qt::ItemIsEnabled);

        toolxcomp[i] = new QDoubleSpinBox();
        toolxcomp[i]->setMinimum(-1000);
        toolxcomp[i]->setMaximum(1000);
        toolxcomp[i]->setDecimals(3);
        toolxcomp[i]->setButtonSymbols(QAbstractSpinBox::NoButtons);
        toolxcomp[i]->setValue(xcomp[num][i]);

        toolycomp[i] = new QDoubleSpinBox();
        toolycomp[i]->setMinimum(-1000);
        toolycomp[i]->setMaximum(1000);
        toolycomp[i]->setDecimals(3);
        toolycomp[i]->setButtonSymbols(QAbstractSpinBox::NoButtons);
        toolycomp[i]->setValue(ycomp[num][i]);

         ui->comptable->setItem(i,0,toolname[i]);
         ui->comptable->setCellWidget(i,1,toolxcomp[i]);
         ui->comptable->setCellWidget(i,2,toolycomp[i]);
    }
}

 int compensation::readComp() {
     QFile file(compfile);
     int array = 0,toolcount = 0;
     if(!file.open(QIODevice::ReadOnly))
         return -1;
     while(!file.atEnd()) {
         QByteArray line = file.readLine();
         if(line.contains('[') && line.contains(']')) {
             QString tool = line;
             QStringList list1;
             list1 = tool.split('[');
             tool = list1.at(1).split(']').at(0);
             array = tool.toInt();
             toolcount = 0;
             arraynum++;
         } else {
                    QString toolxx = line;
                    QStringList info;
                    info = toolxx.split(" ",QString::SkipEmptyParts);
                    tool[array][toolcount] = info.at(0).toInt();
                    QString neg = "";
                    if(info.at(1).contains('-')) {
                        neg = info.at(1);
                        neg.remove(QChar('-'));
                        xcomp[array][toolcount] = -1*neg.toDouble();
                    } else
                        xcomp[array][toolcount] = info.at(1).toDouble();
                    if(info.at(2).contains('-')) {
                        neg = info.at(2);
                        neg.remove(QChar('-'));
                        ycomp[array][toolcount] = -1*neg.toDouble();
                    } else
                        ycomp[array][toolcount] = info.at(2).toDouble();
                    toolcount++;
         }
         if(array == num)
             toolnum = toolcount;
     }
     toolcount = 0;
     file.close();

     return 0;
 }

compensation::~compensation()
{
    delete ui;
}
