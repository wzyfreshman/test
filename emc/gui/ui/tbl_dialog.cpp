#include "tbl_dialog.h"
#include "ui_tbl_dialog.h"
#include "shcom.hh"
#include "extern.h"
#include "emcstatus.hh"
#include "tooltimes.h"
#include "linuxcnc.h"

#include <QMessageBox>
#include <QDebug>
#include <QCloseEvent>
#include <QTabBar>
#include <QStackedWidget>

Tbl_Dialog::Tbl_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tbl_Dialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Tool Table Editor"));
    lcnc = (linuxcnc*)parent;
    InitUI();
    InitConnect();
    ui->frame_3->hide();
    ui->ck_diffZ->hide();
    sendSetParameter(4109,0);
}

Tbl_Dialog::~Tbl_Dialog()
{
    releaseList();
    delete ui;
}
void Tbl_Dialog::getEmcStatues()
{
    if(1 == par_get_var(4110)) {
        ui->arrayframe->setHidden(false);
        ui->ck_array->setChecked(true);
    } else {
        ui->arrayframe->setHidden(true);
        ui->ck_array->setChecked(false);
    }
    if(1 == par_get_var(4130)) {
        ui->chk_oterarray->setChecked(true);
        ui->widgetwidth->setEnabled(true);
        ui->widgetlength->setEnabled(true);
        ui->widgetdisx->setEnabled(true);
        ui->widgetdisy->setEnabled(true);
    } else {
        ui->chk_oterarray->setChecked(false);

        ui->widgetwidth->setEnabled(false);
        ui->widgetlength->setEnabled(false);
        ui->widgetdisx->setEnabled(false);
        ui->widgetdisy->setEnabled(false);
    }
    if(par_get_var(4109) == 1){
        ui->ck_diffZ->setChecked(true);
        changeDiffSet(true);
    }else{
        ui->ck_diffZ->setChecked(false);
        changeDiffSet(false);
    }
}

void Tbl_Dialog::InitUI()
{
    ui->Delete->setHidden(true);
    ui->ReRead->setHidden(true);
    ui->AddTool->setHidden(true);
    ui->SaveFile->setHidden(true);
    toolmanager = new tooltimes(this,0,lcnc,0);
    QHBoxLayout *lay_toolman = new QHBoxLayout(ui->maintabPage2);
    lay_toolman->addWidget(toolmanager);
    lay_toolman->setContentsMargins(0,0,0,0);

    ui->tableWidget->verticalHeader()->setVisible(false);
    getEmcStatues();

    if(!_doublez)
        ui->ck_diffZ->hide();

}

void Tbl_Dialog::InitConnect()
{
    connect(ui->Delete,SIGNAL(clicked()),this,SLOT(slotRemoveTool()));
    connect(ui->ReRead,SIGNAL(clicked()),this,SLOT(slotLoadTool()));
    connect(ui->AddTool,SIGNAL(clicked()),this,SLOT(slotAddTool()));
    connect(ui->SaveFile,SIGNAL(clicked()),this,SLOT(slotSaveToFile()));
    connect(ui->Quit,SIGNAL(clicked()),this,SLOT(myquit()));

    connect(ui->SaveLoad,SIGNAL(clicked()),this,SLOT(slotSaveAndLoadDown()));
    connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),
            this,SLOT(slotItemchange(QTableWidgetItem*)));

    connect(ui->ck_diffZ , SIGNAL(clicked(bool)) , this ,SLOT(changeDiffSet(bool)));
    connect(ui->ck_array , SIGNAL(clicked(bool)) , this ,SLOT(usingArrayFile(bool)));
}

void Tbl_Dialog::changeDiffSet(bool checked)
{
    if(checked) //Z1+Z2
        sendSetParameter(4109,1);
    else
        sendSetParameter(4109,0);

    flushDisplay();
}

void Tbl_Dialog::usingArrayFile(bool flag)
{
    if(flag) {//using array file
        sendSetParameter(4110,1);
        ui->arrayframe->setHidden(false);
    } else {
        sendSetParameter(4110,0);
        ui->arrayframe->setHidden(true);
    }

    flushDisplay();
}

void Tbl_Dialog::flushDisplay(){

    QStringList list;
    if(ui->ck_diffZ->isChecked()){
        recoveryZ2();
        list<<" "<<tr("TOOL")<<tr("POC")<<tr("NAME")
                 <<tr("DIAM")<<tr("Z1")<<tr("Z2")
                 <<tr("X")<<tr("X2")
                 <<tr("Y")<<tr("Y2");
        ui->tableWidget->setHorizontalHeaderLabels(list);
        ui->tableWidget->showColumn(6);

    }else{
        list<<" "<<tr("TOOL")<<tr("POC")<<tr("NAME")
                 <<tr("DIAM")<<tr("Z")<<tr("Z2")
                 <<tr("X")<<tr("X2")
                 <<tr("Y")<<tr("Y2");
        ui->tableWidget->setHorizontalHeaderLabels(list);
        ui->tableWidget->hideColumn(6);
    }

    if(ui->ck_array->isChecked()){
        ui->tableWidget->showColumn(8);
        ui->tableWidget->showColumn(10);
    }else{
        ui->tableWidget->hideColumn(8);
        ui->tableWidget->hideColumn(10);
    }

    ui->btn_setWidth->setText(QString::number(par_get_var(4131),'f',3));
    ui->btn_setHeight->setText(QString::number(par_get_var(4132),'f',3));
    ui->btn_setdisX->setText(QString::number(par_get_var(4133),'f',3));
    ui->btn_setdisY->setText(QString::number(par_get_var(4134),'f',3));
}

void Tbl_Dialog::setTblFile(QString &file) {

    filename = file;

}

void Tbl_Dialog::clear() {

    allitems.clear();
    showUpItem();
}

bool Tbl_Dialog::saveToFile() {

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    if(ui->tableWidget->rowCount() != allitems.size())
        return false;

    flushCache();

    QString final;
    QString tmp;
    Tbl_Item item;

    for(int i=0;i<allitems.size();i++) {

        item = allitems.at(i);
        tmp = QString("T%1 ").arg(item.t);
        final.append(tmp);
        tmp = QString("P%1 ").arg(item.p);
        final.append(tmp);
        tmp = QString("D%1 ").arg(item.d);
        final.append(tmp);
        tmp = QString("Z%1 ").arg(item.z);
        final.append(tmp);
        tmp = QString("A%1 ").arg(item.z2);
        final.append(tmp);

        tmp = QString("M%1 ").arg(item.x1);
        final.append(tmp);
        tmp = QString("N%1 ").arg(item.x2);
        final.append(tmp);
        tmp = QString("E%1 ").arg(item.y1);
        final.append(tmp);
        tmp = QString("F%1 ").arg(item.y2);
        final.append(tmp);


        final.append("\n");

    }

    file.write(final.toUtf8().data());
    file.close();

    return true;
}

void Tbl_Dialog::myquit() {

    this->close();

}


void Tbl_Dialog::closeEvent(QCloseEvent *cevt) {

    if(checkChange()) {

         int ret = QMessageBox::warning(this,tr("tips")
         ,tr("The value has been modified.\\nDo you realy want to exit?"),
                                tr("Ok"),tr("Cancel"));
         switch (ret) {
            case Ok:
                cevt->accept();
                break;
            case Cancel:
                cevt->ignore();
                break;
            default:
                break;
          }

    }


}

void Tbl_Dialog::reject() {

    this->close();

}

void Tbl_Dialog::slotSaveAndLoadDown() {

    if(ui->maintab->currentIndex() == 0) {

        saveTmpZ2();
        if(!saveToFile()) {

            QMessageBox::critical(this, tr("My Application"), tr("save and load faile!!"));

        } else {
            orgtoolnum = allitems.size();
            if(get_task_mode() == EMC_TASK_MODE_MANUAL )
                sendLoadToolTable(filename.toUtf8().data());
            else
                sendMultiFunction(99,0,0);
            QMessageBox::information(this, tr("My Application"), tr("save and load success"));
        }
    } else {
        toolmanager->on_pushButton_clicked();
    }

}

void Tbl_Dialog::slotItemchange(QTableWidgetItem * item) {

    if(item != NULL) {
        if(item->column() == 0 ||item->column() == 3) return;
        QString tmp = item->text();
        bool ok;

        tmp.toDouble(&ok);
        if(ok)
            item->setBackgroundColor(QColor(255,255,255));
         else
            item->setBackgroundColor(QColor(255,0,0));

    }
}

void Tbl_Dialog::removeTool() {


    for(int i=0;i<ui->tableWidget->rowCount();) {


        QTableWidgetItem *item = ui->tableWidget->item(i,0);
        if(item != NULL) {
            if(item->checkState() & Qt::Checked) {
                //qDebug("is selected!! %d",i);
                for(int j=0;j<ui->tableWidget->columnCount();j++) {
                   QTableWidgetItem *item = ui->tableWidget->item(i,j);
                   if(item != NULL)
                     delete item;
                }
                ui->tableWidget->removeRow(i);
                if(i<allitems.size())
                    allitems.removeAt(i);
            } else {
                i++;
            }
        }

    }


}

void Tbl_Dialog::slotLoadTool() {

    if(checkChange()) {

         int ret =QMessageBox::warning(this,tr("tips"),
         tr("The value has been modified.\\nDo you realy want to reload?"),
         tr("Ok"),tr("Cancel"));

         switch (ret) {
            case Ok:
                loadTool();
                break;
            case Cancel:
                break;
            default:
                break;
          }

    } else {
         loadTool();
    }

}
void Tbl_Dialog::slotAddTool() {

    addTool();

}
void Tbl_Dialog::slotSaveToFile() {

    if(!saveToFile()) {
        QMessageBox::critical(this, tr("My Application"), tr("save tbl_file faile!!"));
    } else {
        orgtoolnum = allitems.size();
        QMessageBox::information(this, tr("My Application"), tr("save tbl_file success"));
    }

}

void Tbl_Dialog::slotRemoveTool() {

    removeTool();
}



bool Tbl_Dialog::checkChange() {

    if(ui->tableWidget->rowCount() != allitems.size())
        return true;
    if( ui->tableWidget->rowCount() != orgtoolnum )
        return true;

    QTableWidgetItem *item  = NULL;
    for(int i=0;i<ui->tableWidget->rowCount();i++) {

       item = ui->tableWidget->item(i,1);
       if(item != NULL) {
           if(allitems.at(i).t != item->text().toDouble())
               return true;
       }
       item = ui->tableWidget->item(i,2);
       if(item != NULL) {
           if(allitems.at(i).p != item->text().toDouble())
               return true;
       }
       item = ui->tableWidget->item(i,4);
       if(item != NULL) {
           if(allitems.at(i).d != item->text().toDouble())
               return true;
       }
       if(ui->ck_diffZ->isChecked())
       {
           item = ui->tableWidget->item(i,5);
           if(item != NULL) {
               if(allitems.at(i).z != item->text().toDouble())
                   return true;
            }

           item = ui->tableWidget->item(i,6);
           if(item != NULL) {
               if(allitems.at(i).z2 != item->text().toDouble())
                   return true;
            }
       }else
       {
           item = ui->tableWidget->item(i,5);
            if(item != NULL) {
              if(allitems.at(i).z != item->text().toDouble())
                  return true;
               }
       }


       item = ui->tableWidget->item(i,7);
       if(item != NULL) {
           if(allitems.at(i).x1 != item->text().toDouble())
               return true;
       }
       item = ui->tableWidget->item(i,8);
       if(item != NULL) {
           if(allitems.at(i).x2 != item->text().toDouble())
               return true;
       }
       item = ui->tableWidget->item(i,9);
       if(item != NULL) {
           if(allitems.at(i).y1 != item->text().toDouble())
               return true;
       }
       item = ui->tableWidget->item(i,10);
       if(item != NULL) {
           if(allitems.at(i).y2 != item->text().toDouble())
               return true;
        }

    }

    return false;



}
void Tbl_Dialog::recoveryZ2()
{
    if(tmpZ2_vec.isEmpty())
    {
        if(filename.isEmpty())
            return;
        QString strtmp = filename;
        QString filenameZ2 = strtmp.replace(".tbl","_z2.tbl");
//        qDebug()<<"read file from:"<<filenameZ2;
        QFile file(filenameZ2);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            saveTmpZ2();
        }
        else
        {
            QTextStream stream( &file );
            QString line;
            while ( !stream.atEnd() ) {
                line = stream.readLine();
                tmpZ2_vec.append(line.toDouble());
            }
        }

    }

    if(!tmpZ2_vec.isEmpty())
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return ;
        }
        QString final;
        QTextStream stream( &file );
        QString line;
        QString findA = "A";
        QString find2 = "M";
        int iFind1 = 0;
        int iFind2 = 0;
        int lines = 0;
        while ( !stream.atEnd() ) {
            line = stream.readLine();

            iFind1 = line.indexOf(findA);
            iFind2 = line.indexOf(find2);
            if(iFind1 != 0 && iFind2 != 0 && iFind2 > iFind1)
            {
                QString strReplace =QString("A%1 ").arg(tmpZ2_vec.at(lines));
                line.replace(iFind1 , (iFind2 - iFind1) ,strReplace);
                line.append("\n");
                final.append(line);
            }
            else
            {

            }
            lines++;

        }
        file.close();

        QFile out(filename);
        if (!out.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return ;
        }
        out.write(final.toUtf8().data());
        out.close();
    }
}

bool Tbl_Dialog::saveTmpZ2()
{

    QTableWidgetItem *item;
    double dZ2 =0.0;
    tmpZ2_vec.clear();
    for(int i=0;i<ui->tableWidget->rowCount();i++) {
        item = ui->tableWidget->item(i,6);
        if(item != NULL)
        {
            dZ2= item->text().toDouble();
            tmpZ2_vec.append(dZ2);
        }
    }
    QString strtmp = filename;
    QString filenameZ2 = strtmp.replace(".tbl","_z2.tbl");
//    qDebug()<<"save file :"<<filenameZ2;
    QFile file(filenameZ2);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return false;
    }
    QString final;
    for(int i = 0 ;i < tmpZ2_vec.count() ;i++)
    {
        final.append(QString("%1").arg(tmpZ2_vec.at(i)));
        final.append("\n");
    }
    file.write(final.toUtf8().data());
    file.close();

    return true;
}

void Tbl_Dialog::flushCache() {

    QTableWidgetItem *item;
    int saveType = 0;
    if(ui->ck_diffZ->isChecked())
        saveType = 1 ;
    else
        saveType = 0 ;

    for(int i=0;i<ui->tableWidget->rowCount();i++) {
       Tbl_Item tmmp = allitems.at(i);
       item = ui->tableWidget->item(i,1);
       if(item != NULL)
            (tmmp.t) = item->text().toDouble();

       item = ui->tableWidget->item(i,2);
       if(item != NULL)
            (tmmp.p) = item->text().toDouble();

       item = ui->tableWidget->item(i,4);
       if(item != NULL)
            (tmmp.d) = item->text().toDouble();

       if(saveType == 1)
       {
           item = ui->tableWidget->item(i,5);
           if(item != NULL)
                (tmmp.z) = item->text().toDouble();

           item = ui->tableWidget->item(i,6);
           if(item != NULL)
                (tmmp.z2) = item->text().toDouble();
       }
       else
       {
               item = ui->tableWidget->item(i,5);
               if(item != NULL)
                    (tmmp.z) = item->text().toDouble();

               if(item != NULL)
                    (tmmp.z2) = item->text().toDouble();
       }

       item = ui->tableWidget->item(i,7);
       if(item != NULL)
            (tmmp.x1) = item->text().toDouble();

       item = ui->tableWidget->item(i,8);
       if(item != NULL)
            (tmmp.x2) = item->text().toDouble();

       item = ui->tableWidget->item(i,9);
       if(item != NULL)
            (tmmp.y1) = item->text().toDouble();

       item = ui->tableWidget->item(i,10);
       if(item != NULL)
            (tmmp.y2) = item->text().toDouble();

       allitems.replace(i,tmmp);
    }

}

void Tbl_Dialog::releaseItems() {

    for(int i=0;i<ui->tableWidget->rowCount();i++) {
       for(int j=0;j<ui->tableWidget->columnCount();j++) {
          QTableWidgetItem *item = ui->tableWidget->item(i,j);
          if(item != NULL)
            delete item;
       }
    }
}
void Tbl_Dialog::readToolName()
{
    releaseList();
//    int ToolLenNo = 0;
//    int ToolDiaNo = 0;
//    int LastToolLenNo = 0;
//    int LastToolDiaNo = 0;
    QString toolFlag = "TOOL";
    QString diaFlag = "DIA";
//    QString lastName;
    QStringList lines;
    QString realfile  = lcnc->getGCfile();
    QFile file(realfile);
//    QString str;
    int ntool =0;
    int ndia = 0;
    bool lastIsToolInfo =false;
    if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
           QTextStream stream( &file );
           QString line;
           while ( !stream.atEnd() ) {
               line = stream.readLine();
               lines += line;
               line= line.toUpper().trimmed();
//               line.replace(QRegExp("[\\s]+"), " ");
               line = line.simplified();
               lastIsToolInfo = false;  //off name
               if(lastIsToolInfo) //get like （精修）
               {
                   bool isNum = false ;
                   for(int j = 0; j < line.length(); j++)
                   {
                       if(line[j] >= '0' && line[j] <= '9')
                           isNum = true;
                   }

                   if(!isNum)
                   {
                       int nlast = toolnameList.count();
                       if(nlast<=0)
                           return;
                       Tool_Name *item=NULL;
                       item = toolnameList.at(nlast - 1);
                       item->remark.append(line);
                   }

                   lastIsToolInfo = false ;

               }

               if(line.contains(toolFlag) && line.contains(diaFlag))
               {
                   line.replace(" ","");
                   QString tmp;
                   int j = 0;
                   int pos = 0;
                   pos = line.indexOf(toolFlag) + toolFlag.length();
                   for(j = pos; j < line.length(); j++)
                   {
                       if(line[j] >= '0' && line[j] <= '9')
                            tmp.append(line[j]);
                       else
                       {
                           if(!tmp.isEmpty())
                                break;
                       }

                   }
                   if(!tmp.isEmpty())
                        ntool = tmp.toInt();

                   pos = line.indexOf(diaFlag);
                   pos = line.indexOf(diaFlag,pos + diaFlag.length()) + diaFlag.length() +1;
                   tmp="";
                   int startpos = 0;
                   int pointpos = 0;
                   int endpos = 0;
                   int numpos = 0;
                   for(j = pos; j < line.length(); j++)
                   {
                       if(line[j] == '-')
                       {
                           startpos = j;
                       }
                       else if(line[j] == '.')
                       {
                           pointpos = j;
                       }
                       else if(line[j] >= '0' && line[j] <= '9')
                       {
                            if(numpos == 0)
                                numpos = j;
                       }
                       else
                       {
                           if(startpos > 0 && numpos != 0)
                           {
                               if(j + 1>= line.length())
                               {
                                   if(line[j +1 ] < '0' && line[j +1] > '9')
                                   { }
                                   else
                                   {
                                       endpos = j;
                                       break;
                                   }
                               }
                           }
                       }
                   }
                   if(pointpos != 0)
                   {
                       if(pointpos <  numpos) //LIKE .6 (0.6)
                       {
                           tmp= line.mid(startpos + 1,endpos - startpos - 1);
                           tmp = "0"+tmp;
                       }
                       else if(pointpos >  numpos && pointpos < endpos && pointpos == endpos -1)//LIKE 6.(6.0)
                       {
                            tmp= line.mid(startpos +1,endpos - startpos -2);
                       }
                       else //LIKE 1.6
                       {
                           tmp= line.mid(startpos + 1,endpos - startpos -1);
                       }

                   }
                   else
                   {
                        tmp= line.mid(startpos + 1,endpos - startpos -1);
                   }

                  Tool_Name *item = new Tool_Name;
                  item->remark = tr("dia:") +/*QString::number(ndia)*/tmp ;
                  item->tool_no = ntool;
                  toolnameList.append(item);
                  lastIsToolInfo = true;
                  sendSetParameter(4600+item->tool_no,tmp.toDouble());
               }
           }

       }
    file.close();
//    qDebug()<<"file.close()";

}
void Tbl_Dialog::releaseList()
{
    for(int i = 0; i<toolnameList.count() ;i++)
    {
        if(toolnameList.at(i) != NULL)
        {
            delete toolnameList.at(i);
        }
    }
    toolnameList.clear();
}

bool Tbl_Dialog::getToolItem(int toolno ,Tool_Name *&item)
{
    for(int i = 0 ;i < toolnameList.count() ; i++)
    {
        if(toolno == toolnameList.at(i)->tool_no)
        {
            item = toolnameList.at(i);
            return true;
        }
    }
    return false;
}

bool Tbl_Dialog::usedTool(int toolno ,QString &str)
{
    for(int i = 0 ;i < toolnameList.count() ; i++)
    {
        if(toolno == toolnameList.at(i)->tool_no)
        {
            str = toolnameList.at(i)->remark;
            break;
        }
    }
    return true;
}

void Tbl_Dialog::showUpItem() {

    checkFile();
    releaseItems();
    ui->tableWidget->clear();

    QStringList list;

    ui->tableWidget->setRowCount(allitems.size());
    ui->tableWidget->setColumnCount(11);
    list<<" "<<tr("TOOL")<<tr("POC")<<tr("NAME")
             <<tr("DIAM")<<tr("Z1")<<tr("Z2")
             <<tr("X1")<<tr("X2")
             <<tr("Y1")<<tr("Y2");
    ui->tableWidget->setHorizontalHeaderLabels(list);

    ui->tableWidget->setColumnWidth(0,20);
    ui->tableWidget->setColumnWidth(1,60);
    ui->tableWidget->setColumnWidth(2,50);
    ui->tableWidget->setColumnWidth(3,100);
    ui->tableWidget->setColumnWidth(4,100);
    ui->tableWidget->setColumnWidth(5,100);
    ui->tableWidget->setColumnWidth(6,100);
    ui->tableWidget->setColumnWidth(7,100);
    ui->tableWidget->setColumnWidth(8,100);
    ui->tableWidget->setColumnWidth(9,100);
    ui->tableWidget->setColumnWidth(10,100);
    ui->tableWidget->hideColumn(1);
    ui->tableWidget->hideColumn(0);


    readToolName();

    for(int i=0;i<allitems.size();i++) {


        QTableWidgetItem *select = new QTableWidgetItem();
        QTableWidgetItem *t = new QTableWidgetItem();
        QTableWidgetItem *p = new QTableWidgetItem();
        QTableWidgetItem *name = new QTableWidgetItem();
        QTableWidgetItem *d = new QTableWidgetItem();
        QTableWidgetItem *z = new QTableWidgetItem();
        QTableWidgetItem *z2 = new QTableWidgetItem();

        QTableWidgetItem *x1 = new QTableWidgetItem();
        QTableWidgetItem *x2 = new QTableWidgetItem();
        QTableWidgetItem *y1 = new QTableWidgetItem();
        QTableWidgetItem *y2 = new QTableWidgetItem();


        select->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        select->setCheckState(Qt::Unchecked);
        t->setText(QString::number(allitems.at(i).t));
        p->setText(QString::number(allitems.at(i).p));

        QString strname;
        if(usedTool(allitems.at(i).p ,strname))
            name->setText(strname);
        else
            name->setText("");

        d->setText(QString::number(allitems.at(i).d));
        z->setText(QString::number(allitems.at(i).z));
        z2->setText(QString::number(allitems.at(i).z2));


        x1->setText(QString::number(allitems.at(i).x1));
        x2->setText(QString::number(allitems.at(i).x2));
        y1->setText(QString::number(allitems.at(i).y1));
        y2->setText(QString::number(allitems.at(i).y2));

        ui->tableWidget->setItem(i,0,select);
        ui->tableWidget->setItem(i,1,t);
        ui->tableWidget->setItem(i,2,p);
        ui->tableWidget->setItem(i,3,name);
        ui->tableWidget->setItem(i,4,d);
        ui->tableWidget->setItem(i,5,z);
        ui->tableWidget->setItem(i,6,z2);

        ui->tableWidget->setItem(i,7,x1);
        ui->tableWidget->setItem(i,8,x2);
        ui->tableWidget->setItem(i,9,y1);
        ui->tableWidget->setItem(i,10,y2);

    }

    int i;
    int col = 2;    //EditDisable
    for(i = 0; i < ui->tableWidget->rowCount(); i++){
        QTableWidgetItem *item = ui->tableWidget->item(i, col);
        if(item) {
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        }

        item = ui->tableWidget->item(i, col+1);
        if(item) {
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        }

    }



}


void Tbl_Dialog::addTool() {

    Tbl_Item item;
    item.valid = true;
    allitems.append(item);
    int i = ui->tableWidget->rowCount();

    QTableWidgetItem *select = new QTableWidgetItem();
    QTableWidgetItem *t = new QTableWidgetItem();
    QTableWidgetItem *p = new QTableWidgetItem();
    QTableWidgetItem *d = new QTableWidgetItem();
    QTableWidgetItem *z = new QTableWidgetItem();
    QTableWidgetItem *z2 = new QTableWidgetItem();

    QTableWidgetItem *x1 = new QTableWidgetItem();
    QTableWidgetItem *x2 = new QTableWidgetItem();
    QTableWidgetItem *y1 = new QTableWidgetItem();
    QTableWidgetItem *y2 = new QTableWidgetItem();


    select->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    select->setCheckState(Qt::Unchecked);

    t->setText(QString::number(allitems.at(i).t));
    p->setText(QString::number(allitems.at(i).p));
    d->setText(QString::number(allitems.at(i).d));
    z->setText(QString::number(allitems.at(i).z));
    z2->setText(QString::number(allitems.at(i).z2));

    x1->setText(QString::number(allitems.at(i).x1));
    x2->setText(QString::number(allitems.at(i).x2));
    y1->setText(QString::number(allitems.at(i).y1));
    y2->setText(QString::number(allitems.at(i).y2));


    ui->tableWidget->insertRow(i);
    ui->tableWidget->setItem(i,0,select);
    ui->tableWidget->setItem(i,1,t);
    ui->tableWidget->setItem(i,2,p);
    ui->tableWidget->setItem(i,3,p);
    ui->tableWidget->setItem(i,4,d);
    ui->tableWidget->setItem(i,5,z);
    ui->tableWidget->setItem(i,6,z2);

    ui->tableWidget->setItem(i,7,x1);
    ui->tableWidget->setItem(i,8,x2);
    ui->tableWidget->setItem(i,9,y1);
    ui->tableWidget->setItem(i,10,y2);




}
void Tbl_Dialog::checkFile()
{
    QFile file(filename);
    bool modify;
    QString final;
    if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
           QTextStream stream( &file );
           QString line;
           while ( !stream.atEnd() ) {
               line = stream.readLine();
               if(!line.contains("A"))
               {
                   line.replace("M","A0 M");
                   modify =true;
               }
               final.append(line);
               final.append("\n");
           }
    }
    file.close();
    if(modify){

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug()<<"open and load faile!!";
            return;
        }
        file.write(final.toUtf8().data());
        file.close();
    }

}
void Tbl_Dialog::cleartool() {
    for(int i=0;i<ui->tableWidget->rowCount();i++) {
       for(int j=4;j<ui->tableWidget->columnCount();j++) {
          QTableWidgetItem *item = ui->tableWidget->item(i,j);
          if(item != NULL)
              item->setText("0");
       }
    }

    if(!saveToFile()) {
        qDebug()<<"save and load faile!!";
    }else {
        orgtoolnum = allitems.size();
        sendLoadToolTable(filename.toUtf8().data());
    }

}

bool Tbl_Dialog::loadTool() {

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    allitems.clear();
    QTextStream instream(&file);
    QString allstr,read;

    allstr = instream.read(1024*512);
    QTextDocument doc(allstr);
    QTextCursor modify(&doc);
    QStringList tmplst;

    for(modify.movePosition(QTextCursor::Start);
        !modify.atEnd();
        modify.movePosition(QTextCursor::Down)) {
        modify.select(QTextCursor::LineUnderCursor);
        read=modify.selectedText();
        //qDebug("read = %s",read.toUtf8().data());

        tmplst = read.split(" ",QString::SkipEmptyParts);

        QString tmp;
        int count = 0;
        Tbl_Item item;
        double tmpd = 0;
        bool ok;
        for(int i = 0;i<tmplst.size();i++) {
            tmp = tmplst.at(i);

            if(tmp.startsWith("T") ) {

                tmp.remove(0,1);
                tmpd = tmp.toDouble(&ok);

                if(ok == true) {
                   item.t = tmpd;
                   count ++;
                }

            } else if(tmp.startsWith("P")) {

                tmp.remove(0,1);
                tmpd = tmp.toDouble(&ok);
                if(ok == true) {
                   item.p = tmpd;
                   count ++;
                }

            }  else if(tmp.startsWith("Z")) {

                tmp.remove(0,1);
                tmpd = tmp.toDouble(&ok);
                if(ok == true) {
                   item.z = tmpd;
                }
            }  else if(tmp.startsWith("A")) {

                tmp.remove(0,1);
                tmpd = tmp.toDouble(&ok);
                if(ok == true) {
                   item.z2 = tmpd;
                }

            }  else if(tmp.startsWith("D")) {

                tmp.remove(0,1);
                tmpd = tmp.toDouble(&ok);
                if(ok == true) {
                   item.d = tmpd;

                }

            }  else if(tmp.startsWith("M")) {

                tmp.remove(0,1);
                tmpd = tmp.toDouble(&ok);
                if(ok == true) {
                   item.x1 = tmpd;

                }

            }  else if(tmp.startsWith("N")) {

                tmp.remove(0,1);
                tmpd = tmp.toDouble(&ok);
                if(ok == true) {
                   item.x2 = tmpd;

                }

            }  else if(tmp.startsWith("E")) {

                tmp.remove(0,1);
                tmpd = tmp.toDouble(&ok);
                if(ok == true) {
                   item.y1 = tmpd;

                }

            }  else if(tmp.startsWith("F")) {

                tmp.remove(0,1);
                tmpd = tmp.toDouble(&ok);
                if(ok == true) {
                   item.y2 = tmpd;

                }

            }

        }

        if(count == 2 ) {
            item.valid = true;
            allitems.append(item);
        }


    }

    showUpItem();
    orgtoolnum = allitems.size();
    return true;
}

void Tbl_Dialog::on_btn_page1_clicked()
{
        ui->btn_page1->setChecked(true);
        ui->btn_page2->setChecked(false);
        ui->maintab->setCurrentIndex(0);

}

void Tbl_Dialog::on_btn_page2_clicked()
{
        ui->btn_page2->setChecked(true);
        ui->btn_page1->setChecked(false);
        ui->maintab->setCurrentIndex(1);
}

void Tbl_Dialog::on_btn_setWidth_clicked()
{
    MyInput input(1,(QVariant)par_get_var(4131),(QVariant)-999999,(QVariant)99999,tr("set piece width"),tr("piece width"),0);
    input.exec();
    double val = input.val.toDouble();
    if(val >= -99999 && val <= 99999 && fabs(val - par_get_var(4131)) > 0.0001){
        ui->btn_setWidth->setText(QString::number(val,'f',3));
        sendSetParameter(4131,val);
    }
}

void Tbl_Dialog::on_btn_setHeight_clicked()
{
    MyInput input(1,(QVariant)par_get_var(4132),(QVariant)-999999,(QVariant)99999,tr("set piece height"),tr("piece height"),0);
    input.exec();
    double val = input.val.toDouble();
    if(val >= -99999 && val <= 99999 && fabs(val - par_get_var(4132)) > 0.0001){
        ui->btn_setHeight->setText(QString::number(val,'f',3));
        sendSetParameter(4132,val);
    }
}

void Tbl_Dialog::on_btn_setdisX_clicked()
{
    MyInput input(1,(QVariant)par_get_var(4133),(QVariant)-999999,(QVariant)99999,tr("set X distance"),tr("X distance"),0);
    input.exec();
    double val = input.val.toDouble();
    if(val >= -99999 && val <= 99999 && fabs(val - par_get_var(4133)) > 0.0001){
        ui->btn_setdisX->setText(QString::number(val,'f',3));
        sendSetParameter(4133,val);
    }
}

void Tbl_Dialog::on_btn_setdisY_clicked()
{
    MyInput input(1,(QVariant)par_get_var(4134),(QVariant)-999999,(QVariant)99999,tr("set Y distance"),tr("Y distance"),0);
    input.exec();
    double val = input.val.toDouble();
    if(val >= -99999 && val <= 99999 && fabs(val - par_get_var(4134)) > 0.0001){
        ui->btn_setdisY->setText(QString::number(val,'f',3));
        sendSetParameter(4134,val);
    }
}

void Tbl_Dialog::on_chk_oterarray_toggled(bool checked)
{
    if(checked) {
        sendSetParameter(4130,1);
        ui->widgetwidth->setEnabled(true);
        ui->widgetlength->setEnabled(true);
        ui->widgetdisx->setEnabled(true);
        ui->widgetdisy->setEnabled(true);
    } else {
        sendSetParameter(4130,0);
        ui->widgetwidth->setEnabled(false);
        ui->widgetlength->setEnabled(false);
        ui->widgetdisx->setEnabled(false);
        ui->widgetdisy->setEnabled(false);
    }
}
