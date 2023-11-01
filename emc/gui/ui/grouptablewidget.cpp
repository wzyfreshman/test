#include "grouptablewidget.h"
#include <QMessageBox>
#include <QPoint>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>

GroupTableWidget::GroupTableWidget(QWidget *parent) :
    QTableWidget(parent)
{

    defaultBkColor = QColor(255,255,255);   //默认背景色
    previousColorRow = -1;

//    tableWidget = new QTableWidget(this);
//    this->setRowCount(10);   //设置行数为10
//    this->setColumnCount(5); //设置列数为5
//    this->setGeometry(10,10,580,280);

    this->setItemDelegate(new NoFocusDelegate());   //NoFocus
    /*去掉每行的行号*/
    QHeaderView *headerView = this->verticalHeader();
    headerView->setHidden(true);

    this->setShowGrid(false);   //bu显示表格线

    this->setSelectionBehavior(QAbstractItemView::SelectRows);   //设置整行选择
    this->setMouseTracking(true);    //开启捕获鼠标功能
    this->setStyleSheet("selection-background-color:blue");  //设置选中行颜色
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);    //禁止编辑表格

    connect(this,SIGNAL(cellEntered(int,int)),this,SLOT(mycellEntered(int,int)));
}

void GroupTableWidget::mycellEntered(int row, int column)
{
    QTableWidgetItem *item = 0;

    //还原上一行的颜色
    item = this->item(previousColorRow, 0);
    if (item != 0)
    {
        this->setRowColor(previousColorRow, defaultBkColor);
    }

    //设置当前行的颜色
    item = this->item(row, column);
    if (item != 0 && !item->isSelected() && !item->text().isEmpty())
    {
        this->setRowColor(row, QColor(193,210,240));

    }

    //设置行的索引
    previousColorRow = row;
}

//设置某一行的颜色
void GroupTableWidget::setRowColor(int row, QColor color)
{
    for (int col=0; col<this->columnCount(); col++)
    {
        QTableWidgetItem *item = this->item(row, col);
        if(item !=0 && NULL !=item)
             item->setBackgroundColor(color);
    }
}
QLabel *GroupTableWidget::addgroup(QString gpname,int grouptype)
{
    QLabel *group=new QLabel;
    QHBoxLayout *hly=new QHBoxLayout;
    QLabel *headtitle=new QLabel;

    if(grouptype ==2)
    {
       QString strgpname="              ";
       strgpname.append(gpname);
       gpname=strgpname;
       headtitle->setText(gpname);
       headtitle->setStyleSheet("background-color:rgb(230,240,255)");
       hly->addWidget(headtitle);
    }else{
        QString strgpname="  ";
        strgpname.append(gpname);
        gpname=strgpname;
        QFrame *line = new QFrame(this);
        line->setObjectName(QString::fromUtf8("group"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        headtitle->setText(gpname);
        headtitle->setStyleSheet("font-weight:500");//font-size:18px;
        headtitle->setMinimumHeight(20);
        hly->addWidget(headtitle);
        hly->addWidget(line);

    }

    group->setLayout(hly);

    return group;

}

void GroupTableWidget::addnewgroup(QString gpname,int nrow)
{
        int nrowncount=this->rowCount();
        int ncolumncount=this->columnCount();
        this->insertRow(nrowncount);

        this->setRowHeight(nrowncount,40);
//        this->resizeRowsToContents();
        this->setSpan(nrowncount,0,1,ncolumncount);
        this->setCellWidget(nrowncount,0,addgroup(gpname,nrow));

}




