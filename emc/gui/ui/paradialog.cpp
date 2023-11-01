#include "paradialog.h"
#include "ui_paradialog.h"
#include "emc.hh"
#include "emc_nml.hh"
#include "shcom.hh"
#include "homedialog.h"
#include "offsetdialog.h"
#include "extern.h"
#include "mywidget.h"

#include <QFile>
#include <QSettings>
#include <QLineEdit>
#include <QRegExp>
#include <QTextCodec>
#include <QMouseEvent>
#include <QFileDialog>
#include <QTime>
#include <inifile.hh>
#include <QtDebug>
#include <QTimer>
#include <QMessageBox>
#include <QApplication>
#include <QShortcut>
#include<QHeaderView>

#define TABLE_NUM 9
#define MODIFIED_NUM 1000
#define MAX_ROWS 500
static int group_num = 0;
static QStringList table[TABLE_NUM];
static QStringList ID[TABLE_NUM];
static QStringList modified_value[MODIFIED_NUM];
static QString detail[TABLE_NUM][MAX_ROWS];  //new for the detail info of the segment,show in the
                                             //table of the bottom
static QString oldval[TABLE_NUM][MAX_ROWS];
static int code_choice = 0;
static int modified_num=0;
static int valtype;  //0 is bool  1 is num 2 is 0123
QString groupname[9][8];

CEditdlg * edlg;

Par_Dialog::Par_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParaDialog)
{
    binitconnect=false;
    pTabDlg=new CMyTabWidget(this);
    pTabDlg->setMinimumHeight(200);
    pTabDlg->setTabPosition(QTabWidget::West);
    ui->setupUi(this);
    this->setWindowTitle(tr("ParameterConfig"));
    InitUi();
    InitConnect();

}
void Par_Dialog::InitUi()
{
    QVBoxLayout *pmainly=new QVBoxLayout();
    setLayout(pmainly);

    par_table_operate=new GroupTableWidget(this);
    par_table_feed=new GroupTableWidget(this);
    par_table_spindle=new GroupTableWidget(this);
    par_table_tool=new GroupTableWidget(this);
    par_table_parx=new GroupTableWidget(this);
    par_table_pary=new GroupTableWidget(this);
    par_table_parz=new GroupTableWidget(this);
    par_table_system=new GroupTableWidget(this);
    par_table_para=new GroupTableWidget(this);

    par_table_operate->setObjectName(QString::fromUtf8("tab_1"));
    par_table_feed->setObjectName(QString::fromUtf8("tab_2"));
    par_table_spindle->setObjectName(QString::fromUtf8("tab_3"));
    par_table_tool->setObjectName(QString::fromUtf8("tab_4"));
    par_table_parx->setObjectName(QString::fromUtf8("tab_5"));
    par_table_pary->setObjectName(QString::fromUtf8("tab_6"));
    par_table_parz->setObjectName(QString::fromUtf8("tab_7"));
    par_table_system->setObjectName(QString::fromUtf8("tab_a"));
    par_table_para->setObjectName(QString::fromUtf8("tab_8"));

    QString tbname;
    tbname.append(tr("Operation"));
    tbname.append("(P)");
    pTabDlg->addTab(par_table_operate,tbname);
    tbname="";
    tbname.append(tr("Feed"));
    tbname.append("(F)");
    pTabDlg->addTab(par_table_feed,tbname);
    tbname="";
    tbname.append(tr("Spindle"));
    tbname.append("(I)");
    pTabDlg->addTab(par_table_spindle,tbname);
    tbname="";
    tbname.append(tr("Handle"));
    tbname.append("(H)");
    pTabDlg->addTab(par_table_tool,tbname);
    tbname="";
    tbname.append(tr("Program"));
    tbname.append("(R)");
    pTabDlg->addTab(par_table_parx,tbname);
    tbname="";
    tbname.append(tr("Interface"));
    tbname.append("(V)");
    pTabDlg->addTab(par_table_pary,tbname);
    tbname="";
    tbname.append(tr("Tool"));
    tbname.append("(T)");
    pTabDlg->addTab(par_table_parz,tbname);
    tbname="";
    tbname.append(tr("Other"));
    tbname.append("(Q)");
    pTabDlg->addTab(par_table_system,tbname);
    tbname="";
    tbname.append(tr("All_System"));
    tbname.append("(S)");
    pTabDlg->addTab(par_table_para,tbname);

    //zhong 2016-4-20 for find para
    tbname="";
    tbname.append(tr("Find Results"));
//    tbname.append("(F)");
    find_table = new GroupTableWidget;
    find_table->setObjectName(QString::fromUtf8("Find Results"));
    find_table->setColumnCount(6);
    find_table->setHorizontalHeaderLabels(QStringList() <<tr("ID")<<tr("NAME")<<tr("INFO")
                                      <<tr("VALUE")<<tr("RANGE")<<tr("REBOOT"));
    find_table->setColumnWidth(0,70);
    find_table->setColumnWidth(1,0);
    find_table->setColumnWidth(2,270);
    find_table->setColumnWidth(3,100);
    find_table->setColumnWidth(4,110);
    find_table->setColumnWidth(5,90);
    pTabDlg->addTab(find_table,tbname);
    find_table->verticalHeader()->setVisible(true);
    popMenu = new QMenu(find_table);
    actGoto = new QAction(tr("goto"), this);
    popMenu->addAction(actGoto);
    find_table->setContextMenuPolicy(Qt::CustomContextMenu);
    find_table->setAlternatingRowColors(true);

    init_table(par_table_operate);
    init_table(par_table_feed);
    init_table(par_table_spindle);
    init_table(par_table_tool);
    init_table(par_table_pary);
    init_table(par_table_parx);
    init_table(par_table_parz);
    init_table(par_table_system);
    init_table(par_table_para);

    pmainly->addWidget(pTabDlg);
    pmainly->addWidget(ui->frame_2);
    pmainly->addWidget(ui->frame);

    modified_num = 0;
    needreboot = 0;
    reboottips = 0;
    for(int i=0;i<MODIFIED_NUM;i++)
     modified_value[i].clear();

    ui->ra_debug_code->setChecked(true);
    par_table_init();

    selecttab=0;
    selectrow=0;

    edlg = new CEditdlg(this);
}
void Par_Dialog::InitConnect()
{
    QShortcut *key=new QShortcut(QKeySequence(Qt::Key_Return),pTabDlg);
    connect(key,SIGNAL(activated()),this,SLOT(Key_Return()));
    QShortcut *key_O=new QShortcut(QKeySequence(Qt::Key_O),this);
    connect(key_O,SIGNAL(activated()),this,SLOT(Key_O()));
    QShortcut *key_M=new QShortcut(QKeySequence(Qt::Key_M),this);
    connect(key_M,SIGNAL(activated()),this,SLOT(Key_M()));
    QShortcut *key_D=new QShortcut(QKeySequence(Qt::Key_D),this);
    connect(key_D,SIGNAL(activated()),this,SLOT(Key_D()));

    QShortcut *Key_Operation=new QShortcut(QKeySequence(Qt::Key_P),this);
    connect(Key_Operation,SIGNAL(activated()),this,SLOT(Key_P()));
    QShortcut *Key_Feed=new QShortcut(QKeySequence(Qt::Key_F),this);
    connect(Key_Feed,SIGNAL(activated()),this,SLOT(Key_F()));
    QShortcut *Key_Spindle=new QShortcut(QKeySequence(Qt::Key_I),this);
    connect(Key_Spindle,SIGNAL(activated()),this,SLOT(Key_I()));
    QShortcut *Key_Handle=new QShortcut(QKeySequence(Qt::Key_H),this);
    connect(Key_Handle,SIGNAL(activated()),this,SLOT(Key_H()));
    QShortcut *Key_Program=new QShortcut(QKeySequence(Qt::Key_R),this);
    connect(Key_Program,SIGNAL(activated()),this,SLOT(Key_R()));
    QShortcut *Key_Interface=new QShortcut(QKeySequence(Qt::Key_V),this);
    connect(Key_Interface,SIGNAL(activated()),this,SLOT(Key_V()));
    QShortcut *Key_Tool=new QShortcut(QKeySequence(Qt::Key_T),this);
    connect(Key_Tool,SIGNAL(activated()),this,SLOT(Key_T()));
    QShortcut *Key_Other=new QShortcut(QKeySequence(Qt::Key_Q),this);
    connect(Key_Other,SIGNAL(activated()),this,SLOT(Key_Q()));
    QShortcut *Key_All_System=new QShortcut(QKeySequence(Qt::Key_S),this);
    connect(Key_All_System,SIGNAL(activated()),this,SLOT(Key_S()));

    connect(ui->ra_operator_code,SIGNAL(toggled(bool)),this,SLOT(switch_to_operator(bool)));
    connect(ui->ra_debug_code,SIGNAL(toggled(bool)),this,SLOT(switch_to_debug(bool)));
    connect(ui->ra_develop_code,SIGNAL(toggled(bool)),this,SLOT(switch_to_develop(bool)));
    connect(ui->btn_cancle,SIGNAL(released()),this,SLOT(par_cancel()));

    //data select
    connect(par_table_operate,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getcellselect(int,int)));
    connect(par_table_spindle,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getcellselect(int,int)));
    connect(par_table_feed,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getcellselect(int,int)));
    connect(par_table_tool,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getcellselect(int,int)));
    connect(par_table_parx,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getcellselect(int,int)));
    connect(par_table_pary,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getcellselect(int,int)));
    connect(par_table_parz,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getcellselect(int,int)));
    connect(par_table_para,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getcellselect(int,int)));
    connect(par_table_system,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getcellselect(int,int)));

    connect( par_table_operate,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(show_detail(int,int,int,int)));
    connect( par_table_spindle,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(show_detail(int,int,int,int)));
    connect( par_table_feed,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(show_detail(int,int,int,int)));
    connect( par_table_tool,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(show_detail(int,int,int,int)));
    connect( par_table_parx,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(show_detail(int,int,int,int)));
    connect( par_table_pary,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(show_detail(int,int,int,int)));
    connect( par_table_parz,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(show_detail(int,int,int,int)));
    connect( par_table_para,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(show_detail(int,int,int,int)));
    connect( par_table_system,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(show_detail(int,int,int,int)));


    connect(edlg,SIGNAL(setUserData(double,QString)),this,SLOT(getuserDataChanged(double,QString)));

    //for find
    connect(ui->btn_paraFind , SIGNAL(clicked()) ,this ,SLOT(ParaFind()));
    connect(find_table,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(getFindselect(int,int)));
    connect(find_table, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slot_MenuGoto(QPoint)));
    connect(actGoto, SIGNAL(triggered()), this, SLOT(slot_TblGoto()));

    connect(pTabDlg, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}
//add by zhong 2016-4-20    for find para
void Par_Dialog::slot_TblGoto()
{
    int row = find_table->currentRow();
    int tabIndex = listfind.at(row).ntable;
    int tabrow = listfind.at(row).nrow;
    pTabDlg->setCurrentIndex(tabIndex);
    GroupTableWidget *tab = (GroupTableWidget *)pTabDlg->currentWidget();
    tab->selectRow(tabrow);
}

void Par_Dialog::slot_MenuGoto(QPoint pos)
{
    popMenu->exec(QCursor::pos());
}

void Par_Dialog::getFindResult(int paraIndex , QTableWidgetItem* findItem ,QString strKey)
{
    if(!checkSameRow(paraIndex ,findItem->row()))
    {
        findpara find;
        find.ntable = paraIndex;
        find.nrow = findItem->row();
        listfind.append(find);
        return;
    }
//    qDebug()<<"not add line#    table:"<<paraIndex <<"row:"<<findItem->row();
}
bool Par_Dialog::checkSameRow(int tabIndex ,int row)
{
    QList<findpara>::const_iterator ci;
    for(ci=listfind.constBegin(); ci!=listfind.constEnd(); ++ci)
    {
        findpara find_T = static_cast<findpara>(*ci);

        if(tabIndex == find_T.ntable)
        {
            if(find_T.nrow == row )
            {
                return true;
            }
        }

    }
    return false;
}
void Par_Dialog::showFindeData()
{
//    for(int i = 0;i<listfind.count();i++)
//    {
//        qDebug()<<"get line#    table:"<<listfind.at(i).ntable <<"row:"<<listfind.at(i).nrow ;
//    }

    for(int i= find_table->rowCount();i>=0;i--)
           find_table->removeRow(i);

    find_table->setRowCount(listfind.count());        //设置行数/

    int j=0;
    for(int i = 0;i<listfind.count();i++)
    {
        int tabIndex = listfind.at(i).ntable;
        int row = listfind.at(i).nrow;
        GroupTableWidget *Table = (GroupTableWidget *)pTabDlg->widget(tabIndex);

        if(!Table)
            return;

        for( j =0; j< Table->columnCount() ; j++)
        {
            QTableWidgetItem *item = Table->item(row,j);
            QString strSet = item->text();
            QTableWidgetItem *setitem = new QTableWidgetItem(strSet);
            find_table->setItem(i,j,setitem);
        }

    }
    QString strRes;
    if(listfind.isEmpty())
    {
         strRes=tr("Find Complete! Couldn't find any results");
         ui->lb_findresult->setStyleSheet("color:red;");
    }
    else
    {
        strRes=tr("Find Complete! query to %1 data").arg(listfind.count());
        ui->lb_findresult->setStyleSheet("color:black;");
    }
    ui->lb_findresult->setText(strRes);
    ui->lb_findresult->setAlignment(Qt::AlignRight);
    send_msg(0,strRes);
}

void Par_Dialog::ParaFind()
{
    listfind.clear();
    findType = 1;
    strFindKey =ui->txt_findkey->text().trimmed();

    if(!strFindKey.isEmpty())
    {
//        qDebug()<<"strFindKey:"<<strFindKey;
        int findType = pTabDlg->count() - 1;
        bool matchType = ui->ck_match->isChecked();

        for(int i=0;i<findType;i++)
        {
            QTableWidget *Table = (QTableWidget *)pTabDlg->widget(i);
            QList<QTableWidgetItem*> itemList;
            if(!matchType)
                itemList = Table->findItems(strFindKey, Qt::MatchContains);
            else
                itemList = Table->findItems(strFindKey, Qt::MatchExactly);

             if(!itemList.isEmpty())
             {
                 for(int m=0;m<itemList.count();m++)
                 {
                     getFindResult(i , itemList.at(m) ,strFindKey);
                 }

             }
        }
    }
    else
    {
        QString strRes;
        strRes=tr("Enter the keyword you want to search for");
        ui->lb_findresult->setStyleSheet("color:red;");
        ui->lb_findresult->setText(strRes);
        for(int i= find_table->rowCount();i>=0;i--)
               find_table->removeRow(i);
        return;
    }

    showFindeData();
    pTabDlg->setCurrentWidget(find_table);
}
void Par_Dialog::getFindselect(int row,int column)
{
    selectTabIndex = listfind.at(row).ntable;
    selectTabRow = listfind.at(row).nrow;
    findRow = row;
    getrowdata(selectTabRow,selectTabIndex);
}
void Par_Dialog::tabChanged(int index)
{
    ui->lb_findresult->setText("");
}

//---------end------------------
void Par_Dialog::init_table(GroupTableWidget *widget) //janmi.0
{

//    widget->setRowCount(100);   //设置行数为10


    widget->setColumnCount(6); //设置列数为6

    widget->setHorizontalHeaderLabels(QStringList() <<tr("ID")<<tr("NAME")<<tr("INFO")
                                      <<tr("VALUE")<<tr("RANGE")<<tr("REBOOT"));
    widget->setColumnWidth(0,70);
    widget->setColumnWidth(1,0);
    widget->setColumnWidth(2,270);
    widget->setColumnWidth(3,100);
    widget->setColumnWidth(4,110);
    widget->setColumnWidth(5,90);

}

void Par_Dialog::insertrow(GroupTableWidget *widget,_key tmpdata,bool bX60)
{
       widget->insertRow(widget->rowCount());
       QTableWidgetItem *id = new QTableWidgetItem();
       id->setText(_ini->read(tmpdata.section,"ID"));
       QString strid = _ini->read(tmpdata.section,"ID");


       QTableWidgetItem *group = new QTableWidgetItem();
       group->setText(tmpdata.section);
       QTableWidgetItem *info = new QTableWidgetItem();
       info->setText(_ini->read(tmpdata.section,"INFO"));

       QString par_value = "";

       QTableWidgetItem *pvalue = new QTableWidgetItem();
       par_value = _ini->read(tmpdata.section,"VALUE");
       if(bX60)
       {
           int convert_value = 0;
           if(par_value.toDouble() > 0)
               convert_value = (int)(par_value.toDouble() * 60+0.5);
           else if(par_value.toDouble() < 0)
               convert_value = (int)(par_value.toDouble() * 60-0.5);
           par_value = QString::number(convert_value,10);
       }

       QString range_type;
       range_type = _ini->read(tmpdata.section,"RANGE");
       if(range_type=="0_1"){
           if(0 == par_value.toInt())
               pvalue->setText(tr("No"));
           else if(1 == par_value.toInt())
               pvalue->setText(tr("Yes"));
           else
               pvalue->setText(par_value);
       }else if(range_type=="-1_1"){
           if(1 == par_value.toInt())
               pvalue->setText(tr("positive"));
           else if(-1 == par_value.toInt())
               pvalue->setText(tr("negative"));
           else
               pvalue->setText(par_value);
       }else{
            pvalue->setText(par_value);
       }

//       pvalue->setText(par_value);

       QTableWidgetItem *range = new QTableWidgetItem();
       range->setText(_ini->read(tmpdata.section,"RANGE").replace("_","-"));
       QTableWidgetItem *reboot = new QTableWidgetItem();
       if(0 == _ini->read(tmpdata.section,"REBOOT").toInt())
           reboot->setText(tr("Validate Immediately"));
       else
           reboot->setText(tr("Validate Reboot"));

       widget->setItem(widget->rowCount()-1,0,id);
       widget->setItem(widget->rowCount()-1,1,group);
       widget->setItem(widget->rowCount()-1,2,info);
       widget->setItem(widget->rowCount()-1,3,pvalue);
       widget->setItem(widget->rowCount()-1,4,range);
       widget->setItem(widget->rowCount()-1,5,reboot);
}

int Par_Dialog::get_change_text(QTableWidget *widget,int x,int y,int type,QString *str){
    int rev = 0;
    if(widget == NULL)
        return -1;
    if(0 == type){
        Par_LineEdit *temp_line1 = (Par_LineEdit*)widget->cellWidget(x,y);
            *str = temp_line1->text();
    }else if(1 == type){
        Par_SpinBox *temp_line2 = (Par_SpinBox*)widget->cellWidget(x,y);
        *str = QString::number(temp_line2->value(),10);
    }else if(2 == type){
        Par_DoubleSpinBox *temp_line = (Par_DoubleSpinBox*)widget->cellWidget(x,y);
        *str = QString::number(temp_line->value(),'f',3);
    }else
        rev = -1;
    QTableWidgetItem *reboot = (QTableWidgetItem*)widget->item(x,5);
    if(reboot->text().contains(tr("Validate Reboot"))) {
    }

    return rev;
}
void Par_Dialog::parse(QString filename, KEYLIST *keys)
{

    QString curLine;
    QString section;
    bool havesection;
    bool havevalues;  //for bgroup2nd
    int paratype=-1;  //1 groupname  2 para
    int tabelno=0;

    struct _key tmpKey;

    int sum = 0;
    havesection=false;
    havevalues=false;
    QFile *ini = new QFile(filename);
    if (!ini->open(QFile::ReadOnly)){
        qDebug()<<"Open file error!";
        return ;
    }

    while(!ini->atEnd()){

        curLine = QString (ini->readLine());

        if(QString::compare(curLine, "\n", Qt::CaseInsensitive) == 0)
        {
            if(havevalues && paratype==2)
            {
                keys->append(tmpKey);

                havevalues=false;
                paratype =-1;
            }
            continue;
        }


        //section,such as "[FPGA3]"
        if ((curLine.contains('[') && curLine.contains(']') && curLine.contains("PAR_"))||
                (curLine.contains('[') && curLine.contains(']') && curLine.contains("GROUP_")))
        {
            if(havevalues && paratype==2)
            {
                keys->append(tmpKey);
//                qDebug()<<"get "<<sum<<"ge   :"<<keys->value(sum).section<<keys->value(sum).ntable<<keys->value(sum).ngroup;
                sum++;
                havevalues=false;
            }
            havesection=true;
            section = curLine.remove(QRegExp("[\\[\\]\\n]"));
            if(section.indexOf("PAR_TABGROUPNAME") == 0)
            {
                paratype=1;
            }
            else
            {
                paratype=2;
            }
            continue;
        }

        //Key,values
        if (curLine.contains('=') && paratype==2)
        {

            havevalues=true;
            if(havesection)
            {
                tmpKey.section = section;
                tmpKey.bgroup2nd=true;
                havesection=false;
            }

            QStringList sections = curLine.split('=');
            QString strkey= sections.at(0).trimmed();
            QString strvalues= sections.at(1).trimmed();

//            qDebug()<<"Current strkey:"<<strkey;
//            qDebug()<<"Current strvalues :"<<strvalues;
            if(QString::compare(strkey, "TABLE", Qt::CaseInsensitive) == 0)
            {
                tmpKey.ntable=strvalues.toInt();
                 continue;
            }

            if(QString::compare(strkey, "GROUP", Qt::CaseInsensitive) == 0)
            {
                tmpKey.ngroup=strvalues.toInt();
                 continue;
            }

            if(QString::compare(strkey, "GROUP2ND", Qt::CaseInsensitive) == 0)
            {
                tmpKey.ngroup2nd=strvalues.toInt();
                 continue;
            }

            if(QString::compare(strkey, "ID", Qt::CaseInsensitive) == 0)
            {
                tmpKey.bgroup2nd=false;
                 continue;
            }

            if(QString::compare(strkey, "X60", Qt::CaseInsensitive) == 0)
            {
                tmpKey.X60=strvalues.toInt();
                 continue;
            }


        }
        else if(curLine.contains('=') &&paratype==1)
        {
            QStringList sections = curLine.split('=');
            QString strkey= sections.at(0).trimmed();
            QString strvalues= sections.at(1).trimmed();

                if(QString::compare(strkey, "TABLE", Qt::CaseInsensitive) == 0)
                {
                    tabelno=strvalues.toInt();
                     continue;
                }

                if(QString::compare(strkey, "NAME", Qt::CaseInsensitive) == 0)
                {
                    QStringList strname = strvalues.split(',');
                    for(int i=0;i<strname.count();i++)
                    {
                        if(i == 0)
                        {
                            groupname[tabelno][i]=strname.at(i).mid(1);
                        }
                        else if(i == strname.count()-1)
                        {
                            groupname[tabelno][i]=strname.at(i).mid(1);
                        }
                        else
                        {
                            groupname[tabelno][i]=strname.at(i);
                        }

                    }
                     continue;
                }
        }

    }
    ini->close();

    return ;
}
void Par_Dialog::init_table_data(GroupTableWidget *widget,_key tmpdata,int ntable,int groupno,int groupno2,KEYLIST &keys)
{



    QString strtable=tmpdata.section;
    bool bX60=false;
//    qDebug()<<"now section: "<<strtable<< " i is:"<<"table num is:"<<widget->rowCount();
//    qDebug()<<"now X60 is"<<tmpdata.X60;
    if((0 == _ini->read(tmpdata.section,"SHOW").toInt() &&
                    (2 == code_choice || 1 == code_choice || 0 == code_choice)) ||
                   (1 == _ini->read(tmpdata.section,"SHOW").toInt() && (1 == code_choice || 2 == code_choice)) ||
                   (2 == _ini->read(tmpdata.section,"SHOW").toInt() && 2 == code_choice))
    {

        if(groupno2==0|groupno2==1)
        {
            if(groupno!=currentgrouplast)
            {
                widget->addnewgroup(groupname[ntable][groupno],0);
            }
            currentgrouplast=groupno;
        }

       // else
       // {

            if(groupno2!=currentgroup2last)
            {
                for(int i=0;i<keys.count();i++)
                {
                    if(keys[i].ntable ==ntable && keys[i].ngroup == groupno){
                       if(keys[i].ngroup2nd ==groupno2&& keys[i].bgroup2nd ==true){
                           QString strinfo= _ini->read(keys[i].section,"INFO");
                           widget->addnewgroup(strinfo,2);
                           break;
                    }
                  }
                }
                currentgroup2last=groupno2;
            }
       // }
       // qDebug()<<"section is:"<<tmpdata.section;
        QString strdetail=_ini->read(tmpdata.section,"DETAIL");
        //qDebug()<<"insert row is:"<<widget->rowCount()<<"str is:"<<strdetail;

                    if(tmpdata.X60 == 1) bX60=true;
                    insertrow(widget,tmpdata,bX60);
                    detail[ntable][ widget->rowCount()] =strdetail;

    }
}

void Par_Dialog::get_data(GroupTableWidget *widget,int ntable,KEYLIST &keys)
{
    _key tmpdata;
    int maxgroup=0;
    int minigroup=0;
    int maxgroup2=0;
    int minigroup2=0;

    int ncount = keys.count();
    int groupno=-1;
    int groupno2=-1;

    currentgrouplast=-1;
    currentgroup2last=-1;

    QList<_key>::const_iterator ci;
    for(ci=keys.constBegin(); ci!=keys.constEnd(); ++ci)
    {
        _key dd=(_key)*ci;

        if(ntable ==dd.ntable)
        {
            if(minigroup>dd.ngroup) minigroup=dd.ngroup;
            if(maxgroup<dd.ngroup) maxgroup=dd.ngroup;
        }
    }



    for(groupno=minigroup;groupno<=maxgroup;groupno++)
    {

          //set big groupno
        for(int i=0;i<ncount;i++)
        {
            if(keys[i].ntable ==ntable && keys[i].ngroup == groupno && keys[i].ngroup2nd == 0 && keys[i].bgroup2nd ==false)
            {

                tmpdata.section=keys[i].section;
                tmpdata.ntable=keys[i].ntable;
                tmpdata.ngroup=keys[i].ngroup;
                tmpdata.ngroup2nd=keys[i].ngroup2nd;
                tmpdata.bgroup2nd=keys[i].bgroup2nd;
                tmpdata.X60=keys[i].X60;
                //qDebug()<<"big tmpdata.section:"<<tmpdata.section;
                groupno2=0;


                init_table_data(widget,tmpdata,ntable,groupno,groupno2,keys);
            }
            else if(keys[i].ntable ==ntable && keys[i].ngroup == groupno && keys[i].bgroup2nd ==true)
            {
                if(minigroup2>keys[i].ngroup2nd) minigroup2=keys[i].ngroup2nd;
                if(maxgroup2<keys[i].ngroup2nd) maxgroup2=keys[i].ngroup2nd;
            }

        }

        for(groupno2=minigroup2;groupno2<=maxgroup2;groupno2++)
        {

            for(int i=0;i<ncount;i++)
            {
                //set 2nd groupno


                if(keys[i].ntable ==ntable && keys[i].ngroup == groupno && keys[i].ngroup2nd == groupno2 && keys[i].ngroup2nd != 0 && keys[i].bgroup2nd ==false)
                {
//                    qDebug()<<"2nd  tmpdata.section:"<<tmpdata.section;
                    tmpdata.section=keys[i].section;
                    tmpdata.ntable=keys[i].ntable;
                    tmpdata.ngroup=keys[i].ngroup;
                    tmpdata.ngroup2nd=keys[i].ngroup2nd;
                    tmpdata.bgroup2nd=keys[i].bgroup2nd;
                    tmpdata.X60=keys[i].X60;

                    init_table_data(widget,tmpdata,ntable,groupno,groupno2,keys);
                }
            }

        }

    }

}


void Par_Dialog::par_table_init(){
    for(int i=0;i<TABLE_NUM;i++) {
        table[i].clear();
        ID[i].clear();
    }
    for(int i= par_table_operate->rowCount();i>=0;i--)
           par_table_operate->removeRow(i);
      for(int i= par_table_feed->rowCount();i>=0;i--)
           par_table_feed->removeRow(i);
      for(int i= par_table_parx->rowCount();i>=0;i--)
           par_table_parx->removeRow(i);
      for(int i= par_table_pary->rowCount();i>=0;i--)
           par_table_pary->removeRow(i);
      for(int i= par_table_parz->rowCount();i>=0;i--)
           par_table_parz->removeRow(i);
      for(int i= par_table_para->rowCount();i>=0;i--)
           par_table_para->removeRow(i);
      for(int i= par_table_spindle->rowCount();i>=0;i--)
           par_table_spindle->removeRow(i);
      for(int i= par_table_system->rowCount();i>=0;i--)
           par_table_system->removeRow(i);
      for(int i= par_table_tool->rowCount();i>=0;i--)
           par_table_tool->removeRow(i);

      par_table_operate->clearContents();

//      KEYLIST keys;

      keys.clear();
      parse(_ini_filepath,&keys);
      get_data(par_table_operate,0,keys);
      get_data(par_table_feed,1,keys);
      get_data(par_table_spindle,2,keys);
      get_data(par_table_tool,3,keys);
      get_data(par_table_parx,4,keys);
      get_data(par_table_pary,5,keys);
      get_data(par_table_parz,6,keys);
      get_data(par_table_system,7,keys);
      get_data(par_table_para,8,keys);
}

void Par_Dialog::switch_to_operator(bool value) {
    if(!value){
        return;
    }

    binitconnect=true;
    if(1 == code_choice) {
        code_choice = 0;
        par_table_init();
        code_choice = 1;
        return;
    }

    if(2 == code_choice) {
        code_choice = 0;
        par_table_init();
        code_choice = 2;
        return;
    }

    if(0 == code_choice)
        return;
    code_choice = 0;
    par_table_init();

}

int Par_Dialog::switch_to_debug(bool value) {
    if(!value){
        return -1;
    }

    if(1 == code_choice) {
        par_table_init();
        return 0;
    }

    if(2 == code_choice) {
        code_choice = 1;
        par_table_init();
        code_choice = 2;
        return 0;
    }

    if(!PassWord::getCodecnc(this,1)) {
        switch(code_choice){
        case 0:ui->ra_operator_code->setChecked(true);break;
        case 1:ui->ra_debug_code->setChecked(true);break;
        case 2:ui->ra_develop_code->setChecked(true);break;
        default:break;
        }
         return -1;
    }

    code_choice = 1;
    par_table_init();
    return 0;
}

void Par_Dialog::switch_to_develop(bool value) {
    if(!value){
        return;
    }
    if(2 == code_choice) {
        par_table_init();
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
    par_table_init();
}

void Par_Dialog::manage_verify(QString name,QString value,
                               QString old,QString ID){
    int exist = 0;
    for(int i=0;i<MODIFIED_NUM;i++){
        if(modified_value[i].isEmpty())
            break;
        if(modified_value[i].contains(name)){
            modified_value[i].replace(1,value);
            exist = 1;
        }
    }
    if(1 != exist){

        modified_value[modified_num].append(name);
        modified_value[modified_num].append(value);
        modified_value[modified_num].append(old);
        modified_value[modified_num].append(ID);
        modified_num++;
    }

}

void Par_Dialog::table_change(int x, int y,int type){
    int currentindex = 0;

}


int Par_Dialog::modify_check(int i,QString real_value) {
 }

void Par_Dialog::par_cancel()
{
    this->close();
}

void Par_Dialog::Key_Return()
{
    qDebug()<<"showeditdlg";
    showeditdlg();
}
void Par_Dialog::Key_O()
{
    ui->ra_operator_code->setChecked(true);
//    switch_to_operator(true);
}
void Par_Dialog::Key_M()
{
    ui->ra_debug_code->setChecked(true);
//    switch_to_debug(true);
}
void Par_Dialog::Key_D()
{
    ui->ra_develop_code->setChecked(true);
//    switch_to_develop(true);
}

void Par_Dialog::Key_P()
{
    pTabDlg->setCurrentIndex(0);
}

void Par_Dialog::Key_F()
{
    pTabDlg->setCurrentIndex(1);
}

void Par_Dialog::Key_I()
{
    pTabDlg->setCurrentIndex(2);
}

void Par_Dialog::Key_H()
{
    pTabDlg->setCurrentIndex(3);
}

void Par_Dialog::Key_R()
{
    pTabDlg->setCurrentIndex(4);
}

void Par_Dialog::Key_V()
{
    pTabDlg->setCurrentIndex(5);
}

void Par_Dialog::Key_T()
{
    pTabDlg->setCurrentIndex(6);
}

void Par_Dialog::Key_Q()
{
    pTabDlg->setCurrentIndex(7);
}

void Par_Dialog::Key_S()
{
    pTabDlg->setCurrentIndex(8);
}




void Par_Dialog::showeditdlg()
{
    int selectTabIndex = pTabDlg->currentIndex();
//    qDebug()<<"selectTabIndex=="<<selectTabIndex;
    getrowdata(selectrow,selectTabIndex);
}
//modify by zhong
void Par_Dialog::getcellselect(int row,int column)
{
    int selectTabIndex = pTabDlg->currentIndex();
    getrowdata(row,selectTabIndex);
}
void Par_Dialog::getuserDataChanged(double nvalue,QString str)
{
    QTableWidgetItem * itemstr = new QTableWidgetItem;
    QString strsection;
    QString strsectionname;
    QString strid;
    QString strold;
    QString real_value;
    QString show_msg_str;
    int currentindex = 0;
    bool reSearchData = false;

    if(!str.isEmpty())
    {
        valtype=8;
    }


    real_value = QString::number(nvalue);
    show_msg_str = real_value;

//    qDebug()<<"now the valtype is =="<<valtype;

    if(0 == valtype){

        if(1 == nvalue)
            show_msg_str = tr("Yes");
        else
            show_msg_str = tr("No");

    }else if(4 == valtype){

        if(1 == nvalue)
            show_msg_str = tr("positive");
        else
            show_msg_str = tr("negative");

    }else if(8 == valtype){
        real_value=str;
        show_msg_str = real_value;
    }else{
    }


    if( pTabDlg->currentWidget()->objectName().contains("tab_1"))
        currentindex = TABLE_OPERATION;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_2"))
        currentindex = TABLE_FEED;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_3"))
        currentindex = TABLE_SPINDLE;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_4"))
        currentindex = TABLE_TOOL;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_5"))
        currentindex = TABLE_PARX;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_6"))
        currentindex = TABLE_PARY;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_7"))
        currentindex = TABLE_PARZ;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_8"))
        currentindex = TABLE_PARA;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_a"))
        currentindex = TABLE_SYSTEM;

    //modify by zhong for find
    if(pTabDlg->currentWidget()->objectName().contains("Find"))
    {
        QString tblObj = pTabDlg->widget(selectTabIndex)->objectName();

        if( tblObj.contains("tab_1"))
            currentindex = TABLE_OPERATION;
        else if( tblObj.contains("tab_2"))
            currentindex = TABLE_FEED;
        else if( tblObj.contains("tab_3"))
            currentindex = TABLE_SPINDLE;
        else if( tblObj.contains("tab_4"))
            currentindex = TABLE_TOOL;
        else if( tblObj.contains("tab_5"))
            currentindex = TABLE_PARX;
        else if( tblObj.contains("tab_6"))
            currentindex = TABLE_PARY;
        else if( tblObj.contains("tab_7"))
            currentindex = TABLE_PARZ;
        else if( tblObj.contains("tab_8"))
            currentindex = TABLE_PARA;
        else if( tblObj.contains("tab_a"))
            currentindex = TABLE_SYSTEM;

        selectrow = selectTabRow;
        reSearchData = true;
    }

    switch (currentindex)
    {
        case TABLE_OPERATION:{
            itemstr=par_table_operate->item(selectrow,1);
            if(!itemstr)
                return;
            strsection=itemstr->text();
            strsectionname=strsection;
            itemstr=par_table_operate->item(selectrow,3);
            strold=itemstr->text();
            itemstr=par_table_operate->item(selectrow,0);
            strid=itemstr->text();
            if(8 == valtype)
                par_table_operate->item(selectrow,3)->setText(str);
            else
                par_table_operate->item(selectrow,3)->setText(show_msg_str);
        }break;

        case TABLE_FEED:{
            itemstr=par_table_feed->item(selectrow,1);
            if(!itemstr)
                return;
            strsection=itemstr->text();
            strsectionname=strsection;
            itemstr=par_table_feed->item(selectrow,3);
            strold=itemstr->text();
            itemstr=par_table_feed->item(selectrow,0);
            strid=itemstr->text();
            if(8 == valtype)
                par_table_feed->item(selectrow,3)->setText(str);
            else
                par_table_feed->item(selectrow,3)->setText(show_msg_str);
        }break;

        case TABLE_SPINDLE:{
            itemstr=par_table_spindle->item(selectrow,1);
            if(!itemstr)
                return;
            strsection=itemstr->text();
            strsectionname=strsection;
            itemstr=par_table_spindle->item(selectrow,3);
            strold=itemstr->text();
            itemstr=par_table_spindle->item(selectrow,0);
            strid=itemstr->text();
            if(8 == valtype)
                par_table_spindle->item(selectrow,3)->setText(str);
            else
                par_table_spindle->item(selectrow,3)->setText(show_msg_str);
        }break;

        case TABLE_TOOL:{
            itemstr=par_table_tool->item(selectrow,1);
            if(!itemstr)
                return;
            strsection=itemstr->text();
            strsectionname=strsection;
            itemstr=par_table_tool->item(selectrow,3);
            strold=itemstr->text();
            itemstr=par_table_tool->item(selectrow,0);
            strid=itemstr->text();
            if(8 == valtype)
                par_table_tool->item(selectrow,3)->setText(str);
            else
                par_table_tool->item(selectrow,3)->setText(show_msg_str);
        }break;

        case TABLE_PARX:{
            itemstr=par_table_parx->item(selectrow,1);
            if(!itemstr)
                return;
            strsection=itemstr->text();
            strsectionname=strsection;
            itemstr=par_table_parx->item(selectrow,3);
            strold=itemstr->text();
            itemstr=par_table_parx->item(selectrow,0);
            strid=itemstr->text();
            if(8 == valtype)
                par_table_parx->item(selectrow,3)->setText(str);
            else
                par_table_parx->item(selectrow,3)->setText(show_msg_str);
        }break;

        case TABLE_PARY:{
            itemstr=par_table_pary->item(selectrow,1);
            if(!itemstr)
                return;
            strsection=itemstr->text();
            strsectionname=strsection;
            itemstr=par_table_pary->item(selectrow,3);
            strold=itemstr->text();
            itemstr=par_table_pary->item(selectrow,0);
            strid=itemstr->text();
            if(8 == valtype)
                par_table_pary->item(selectrow,3)->setText(str);
            else
                par_table_pary->item(selectrow,3)->setText(show_msg_str);
        }break;

        case TABLE_PARZ:{
            itemstr=par_table_parz->item(selectrow,1);
            if(!itemstr)
                return;
            strsection=itemstr->text();
            strsectionname=strsection;
            itemstr=par_table_parz->item(selectrow,3);
            strold=itemstr->text();
            itemstr=par_table_parz->item(selectrow,0);
            strid=itemstr->text();
            if(8 == valtype)
                par_table_parz->item(selectrow,3)->setText(str);
            else
                par_table_parz->item(selectrow,3)->setText(show_msg_str);
        }break;

        case TABLE_PARA:{
            itemstr=par_table_para->item(selectrow,1);
            if(!itemstr)
                return;
            strsection=itemstr->text();
            strsectionname=strsection;
            itemstr=par_table_para->item(selectrow,3);
            strold=itemstr->text();
            itemstr=par_table_para->item(selectrow,0);
            strid=itemstr->text();
            if(8 == valtype){
                par_table_para->item(selectrow,3)->setText(str);
            }else{
                par_table_para->item(selectrow,3)->setText(show_msg_str);
            }
            }break;

        case TABLE_SYSTEM:{
            itemstr=par_table_system->item(selectrow,1);
            if(!itemstr)
                return;
            strsection=itemstr->text();
            strsectionname=strsection;
            itemstr=par_table_system->item(selectrow,3);
            strold=itemstr->text();
            itemstr=par_table_system->item(selectrow,0);
            strid=itemstr->text();

            if(8 == valtype){
                par_table_system->item(selectrow,3)->setText(str);
            }else{
                par_table_system->item(selectrow,3)->setText(show_msg_str);
            }


        }break;

        default:
        break;
    }



    strsection.append('/').append("VALUE");

    if((1 == valtype || 5 == valtype) && 1 == _ini->read(strsectionname,"X60").toInt())
        real_value = QString::number(nvalue/60.00,'f',4);

    //modify by zhong 2016-4-20
    if(_ini->writeEntry(strsection,real_value))
    {
        send_msg(0,QString(tr("parameter %1: %2 ==> %3")).arg(strid,strold,show_msg_str));
        //add by zhong  For find flash new data
        if(reSearchData)
        {
            QTableWidgetItem * itemstr = new QTableWidgetItem;
            itemstr = find_table->item(findRow,3);
            if(itemstr)
                itemstr->setText(real_value);
        }
    }
    else
        send_msg(0,QString(tr("Failed ! parameter %1: %2 ==> %3  is not save")).arg(strid,strold,real_value));

    double old_para_value = strold.toDouble();
    if((old_para_value != nvalue)&&1 ==needreboot){
        reboottips =1;
    }



}
//midify by zhong
void Par_Dialog::getrowdata(int row,int tabIndex)
{

    GroupTableWidget * temptab;
    QTableWidgetItem * itemID = new QTableWidgetItem;
    switch (tabIndex) {
    case 0:
        temptab=par_table_operate;
        break;
    case 1:
        temptab=par_table_feed;
        break;
    case 2:
        temptab=par_table_spindle;
        break;
    case 3:
        temptab=par_table_tool;
        break;
    case 4:
        temptab=par_table_parx;
        break;
    case 5:
        temptab=par_table_pary;
        break;
    case 6:
        temptab=par_table_parz;
        break;
    case 7:
        temptab=par_table_system;
        break;
    case 8:
        temptab=par_table_para;
        break;
    case 9:
        temptab=find_table;
        break;
    default:
        break;
    }
     itemID=temptab->item(row,0);
    if(itemID== 0 || itemID==NULL) return;
    QTableWidgetItem * itemstr = new QTableWidgetItem;

    itemstr=temptab->item(row,2);
    QString parainfo=tr("Parameter name:");
    parainfo.append(itemstr->text());
    parainfo.append("\r\n");

    itemstr=temptab->item(row,4);
    QString range_temp = itemstr->text();
    edlg->seteditWidget(range_temp);

    parainfo.append(tr("Parameter range:"));
    parainfo.append(range_temp);
    parainfo.append("\r\n");

    parainfo.append(tr("Onset time:"));
    itemstr=temptab->item(row,5);
    QString time_temp = itemstr->text();
    parainfo.append(time_temp);
    parainfo.append("\r\n");

    QString rebootinfo = tr("Validate Reboot");  //reboot tips
    if(rebootinfo==time_temp)
        needreboot = 1;
    else
       needreboot = 0;

    edlg->setparainfo(parainfo);

    itemstr=temptab->item(row,3);
    QString valuestr=itemstr->text();
    edlg->seteditvalue(valuestr);

    if(EMC_TASK_MODE_AUTO!=get_task_mode())
        edlg->exec();
    else{
        QMessageBox::information(this,tr("tips"),tr("The system is runing!"),tr("Ok"));
        }
}

void Par_Dialog::show_detail(int row,int column,int prow,int pcolumn)
{
//    qDebug()<<"current row is:"<<row;
    int currentindex = 0;
    if( pTabDlg->currentWidget()->objectName().contains("tab_1"))
        currentindex = TABLE_OPERATION;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_2"))
        currentindex = TABLE_FEED;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_3"))
        currentindex = TABLE_SPINDLE;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_4"))
        currentindex = TABLE_TOOL;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_5"))
        currentindex = TABLE_PARX;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_6"))
        currentindex = TABLE_PARY;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_7"))
        currentindex = TABLE_PARZ;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_a"))
        currentindex = TABLE_PARA;
    else if( pTabDlg->currentWidget()->objectName().contains("tab_8"))
        currentindex = TABLE_SYSTEM;

    column = 0;
    ui->show_details->clear();

    GroupTableWidget * temptab;
    QTableWidgetItem * itemID = new QTableWidgetItem;
    switch (currentindex) {
    case 0:
        temptab=par_table_operate;
        break;
    case 1:
        temptab=par_table_feed;
        break;
    case 2:
        temptab=par_table_spindle;
        break;
    case 3:
        temptab=par_table_tool;
        break;
    case 4:
        temptab=par_table_parx;
        break;
    case 5:
        temptab=par_table_pary;
        break;
    case 6:
        temptab=par_table_parz;
        break;
    case 7:
        temptab=par_table_system;
        break;
    case 8:
        temptab=par_table_para;
        break;
    default:
        break;
    }
   itemID=temptab->item(row,0);
   if(!itemID)
   {
       return;
   }
   if(itemID== 0 || itemID==NULL) return;

    ui->show_details->setText(detail[currentindex][row+1]);
    selecttab=currentindex;
    selectrow=row;
}

void Par_Dialog::show_detail(int row,int column)
{
    QString  tabname=pTabDlg->currentWidget()->objectName();
        int currentindex = 0;
        if( pTabDlg->currentWidget()->objectName().contains("tab_1"))
            currentindex = TABLE_OPERATION;
        else if( pTabDlg->currentWidget()->objectName().contains("tab_2"))
            currentindex = TABLE_FEED;
        else if( pTabDlg->currentWidget()->objectName().contains("tab_3"))
            currentindex = TABLE_SPINDLE;
        else if( pTabDlg->currentWidget()->objectName().contains("tab_4"))
            currentindex = TABLE_TOOL;
        else if( pTabDlg->currentWidget()->objectName().contains("tab_5"))
            currentindex = TABLE_PARX;
        else if( pTabDlg->currentWidget()->objectName().contains("tab_6"))
            currentindex = TABLE_PARY;
        else if( pTabDlg->currentWidget()->objectName().contains("tab_7"))
            currentindex = TABLE_PARZ;
        else if( pTabDlg->currentWidget()->objectName().contains("tab_a"))
            currentindex = TABLE_PARA;
        else if( pTabDlg->currentWidget()->objectName().contains("tab_8"))
            currentindex = TABLE_SYSTEM;

        column = 0;
        ui->show_details->clear();
        ui->show_details->setText(detail[currentindex][row+1]);
}

void Par_Dialog::reject(){
    this->close();
}

void Par_Dialog::closeEvent(QCloseEvent *ev)
{
    if(reboottips>0){
        QMessageBox::information(this,tr("tips"),tr("The para is changed ,need reboot!"),tr("Ok"));
    }
    reboottips = 0;
}

void Par_Dialog::showEvent(QShowEvent *)
{
    par_table_init();/****janmi20150810*****/
}


Par_Dialog::~Par_Dialog()
{
    delete ui;
}
Par_txtEdit::Par_txtEdit()
{
    oldvalue = 0;
    minivalue = 0;
    maxvalue = 0;
}
Par_txtEdit::~Par_txtEdit()
{
}
void Par_txtEdit::settxttype(double valmini ,double valmax,int valtype)
{
    this->setButtonSymbols(QAbstractSpinBox::NoButtons);
    //valtype:0是bool，1是常浮点，2是整型，3是字符串，4是方向设置，5是脉冲当量
    if(valtype==1)
    {
        this->setRange(valmini,valmax);
        this->setDecimals(3);
    }
    else  if(valtype==2)
    {
        this->setRange(valmini,valmax);
        this->setDecimals(0);
    }else  if(valtype==5)
    {
        this->setRange(valmini,valmax);
        this->setDecimals(12);
    }
}

void Par_txtEdit::focusInEvent(QFocusEvent *ev)
{
    this->setCursor(Qt::IBeamCursor);
    QDoubleSpinBox::focusInEvent(ev);

}
void Par_txtEdit::line_change(QString str)
{
//    QRegExp rx(strrx);
//    bool match = rx.exactMatch(str);
//    qDebug()<<"exactMatch is"<<match;
//    qDebug()<<"str is"<<str;
}


Par_LineEdit::Par_LineEdit(int x, int y)
{
    row = x;
    column = y;
    type = 0;
}

void Par_LineEdit::line_change()
{
    emit valuechange(row,column,type);
}
void Par_LineEdit::line_change1(QString str)
{
    double nowvalue=this->text().toDouble();
}

void Par_LineEdit::focusInEvent(QFocusEvent *ev)
{
    emit valueactive(row,column);
    this->setCursor(Qt::IBeamCursor);

    QLineEdit::focusInEvent(ev);

}


void Par_LineEdit::mouseReleaseEvent(QMouseEvent *ev)
{
    emit valueactive(row,column);
    if(ev->buttons()){}
}

void Par_SpinBox::line_change()
{
    emit valuechange(row,column,type);
}


void Par_SpinBox::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->buttons()){}
}

void Par_SpinBox::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

void Par_SpinBox::focusInEvent(QFocusEvent *ev)
{
    this->setCursor(Qt::IBeamCursor);
    emit valueactive(row,column);
    QSpinBox::focusInEvent(ev);
}

void Par_DoubleSpinBox::line_change()
{
    emit valuechange(row,column,type);
}

void Par_DoubleSpinBox::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

void Par_DoubleSpinBox::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->buttons()){}
}
void Par_DoubleSpinBox::focusInEvent(QFocusEvent *ev)
{
    emit valueactive(row,column);
    QDoubleSpinBox::focusInEvent(ev);
}
static Par_txtEdit *text_edit;        //edit

CEditdlg::CEditdlg(QWidget *parent):
    QDialog(parent)
{

    bcheck=false;
    msetvalue=0;
    valmini=0;
    valmax=0;
    valtype=0;
    resize(400,220);
    mainly = new QVBoxLayout(this);
    setLayout(mainly);
    copydata=0;
    this->setWindowTitle(tr("Para_set"));
    //TXT
    txt_browser = new QTextBrowser(this);
    //edit
    stredit= new QLineEdit;
    text_edit = new Par_txtEdit();
    text_edit->setMaximumHeight(30);
//    connect(text_edit,SIGNAL(textChanged(QString)),text_edit,SLOT(line_change(QString)));

    //yes no
    yes_btn = new QRadioButton(this);
    no_btn = new QRadioButton(this);
    yes_btn->setText(tr("yes"));
    no_btn->setText(tr("no"));

    lay_yseno_btn = new QHBoxLayout();
    lay_yseno_btn->addWidget(yes_btn,0,Qt::AlignCenter);
    lay_yseno_btn->addWidget(no_btn,0,Qt::AlignCenter);

    //four btn
    btn_sure = new QPushButton(this);
    btn_cancel = new QPushButton(this);
    btn_copy =new  QPushButton(this);
    btn_paste = new QPushButton(this);
    btn_sure->setMaximumWidth(100);
    btn_cancel->setMaximumWidth(100);
    btn_sure->setText(tr("sure"));
    btn_cancel->setText(tr("cancel"));
    btn_copy->setMaximumWidth(100);
    btn_paste->setMaximumWidth(100);
    btn_copy->setText(tr("copy"));
    btn_paste->setText(tr("paste"));

    lay_fun_btn = new QHBoxLayout();
    lay_fun_btn->addWidget(btn_sure);
    lay_fun_btn->addWidget(btn_cancel);
    lay_fun_btn->addWidget(btn_copy);
    lay_fun_btn->addWidget(btn_paste);


    mainly->addWidget(txt_browser);
    mainly->addWidget(text_edit);
    mainly->addWidget(stredit);
    mainly->addLayout(lay_yseno_btn);
    mainly->addLayout(lay_fun_btn);

    mainly->setSpacing(20);


    connect(btn_sure,SIGNAL(clicked()),this,SLOT(par_sure()));
    connect(btn_cancel,SIGNAL(clicked()),this,SLOT(par_cancel()));
    connect(btn_copy,SIGNAL(clicked()),this,SLOT(par_copy()));
    connect(btn_paste,SIGNAL(clicked()),this,SLOT(par_paste()));
}
void CEditdlg::seteditWidget(QString str)
{
    QStringList range_value;
       if(str.contains("-") && !str.contains("~") && !str.contains("-1-1") )
       {
            range_value = str.split("-");
            valtype=0;

       }
       else if(str.contains("~")&& !str.contains("0~1.0"))
       {
            range_value = str.split("~");
            valtype=1;
       }
       else if(str.contains("-1-1"))
       {
            valtype=4;
            text_edit->settxttype(0,0,0);
            setischeck(valtype);
            return;
       }else if(str.contains("0~1.0"))
       {
            range_value = str.split("~");
            valtype=5;
       }else{
       valtype=3;
       text_edit->settxttype(0,0,0);
       setischeck(valtype);
       return;
       }

       valmini=range_value.at(0).toDouble();
       valmax=range_value.at(1).toDouble();
       if(valtype == 0 && valmax>1)  valtype=2;

        text_edit->settxttype(valmini,valmax,valtype);
        setischeck(valtype);

}

void CEditdlg::seteditvalue(QString strnum)
{
    if(valtype == 0)
    {
//        if(1 == strnum.toInt())
//            yes_btn->setChecked(true);
//        else
//            no_btn->setChecked(true);

        QString str = tr("Yes");
        if(str == strnum)
            yes_btn->setChecked(true);
        else
            no_btn->setChecked(true);
    }
    else if(valtype == 4)
    {
//        if(1 == strnum.toInt())
//            yes_btn->setChecked(true);
//        else
//            no_btn->setChecked(true);

        QString str = tr("positive");
        if(str == strnum)
            yes_btn->setChecked(true);
        else
            no_btn->setChecked(true);
    }
    else if(valtype == 3)
    {
        stredit->setText(strnum);
    }
    else
    {
        text_edit->setValue(strnum.toDouble());
    }

}
void CEditdlg::setparainfo(QString str)
{
    txt_browser->clear();
    txt_browser->setText(str);
}

void CEditdlg::setischeck(int ischeck)
{
    if(0 == ischeck)
    {
        text_edit->hide();
        btn_copy->hide();
        btn_paste->hide();

        yes_btn->show();
        yes_btn->setText(tr("Yes_btn"));
        no_btn->show();
        no_btn->setText(tr("No_btn"));
        stredit->hide();
    }
    else if(1 == ischeck || 2 == ischeck || 5 == ischeck )
    {
        text_edit->show();
        btn_copy->show();
        btn_paste->show();
        text_edit->setFocus();

        yes_btn->hide();
        no_btn->hide();
        stredit->hide();
    }
    else if(3 == ischeck)   //str
    {
        stredit->show();

        yes_btn->hide();
        no_btn->hide();
        text_edit->hide();
    }
    else if(4 == ischeck)
    {
        text_edit->hide();
        btn_copy->hide();
        btn_paste->hide();

        yes_btn->show();
        yes_btn->setText(tr("positive"));
        no_btn->show();
        no_btn->setText(tr("negative"));
        stredit->hide();
    }
    bcheck=ischeck;
}
void CEditdlg::setdetail(QString str)
{
    txt_browser->setText((str));
}
void CEditdlg::setvalue(int nvalue)
{

}
void CEditdlg::par_sure()
{

    msetstrvalue="";
    if(0 ==bcheck)
    {
        if(yes_btn->isChecked()) msetvalue=1;
        else if(no_btn->isChecked()) msetvalue=0;
    }
    else if(4 ==bcheck)
    {
        if(yes_btn->isChecked()) msetvalue=1;
        else if(no_btn->isChecked()) msetvalue=-1;
    }
    else if(1 ==bcheck || 2 == bcheck|| 5 == bcheck)
    {
        msetvalue=text_edit->text().toDouble();
    }
    else if(3 ==bcheck)
    {
        msetstrvalue=stredit->text();
    }


    emit setUserData(msetvalue,msetstrvalue);
    this->close();
}
void CEditdlg::par_cancel()
{
    this->close();
}
void CEditdlg::par_copy()
{
    copydata=text_edit->text().toDouble();
}
void CEditdlg::par_paste()
{
    if(valmini<=copydata && copydata<=valmax)
//        text_edit->setText(QString::number(copydata));
        text_edit->setValue(copydata);

}

