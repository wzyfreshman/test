#include "gl_property.h"
#include "ui_gl_property.h"
#include "extern.h"
#include"mywidget.h"
#include <QAction>
#include <QGridLayout>
#include <QMenu>
#include <QPainter>
#include <QColorDialog>
#include<QtDebug>

gl_property::gl_property(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gl_property)
{
    ui->setupUi(this);
//    this->setWindowTitle(tr("Graphic_properties"));


    Color_RealTime = new ColorCombox(ui->widget_realtime);
    Color_Feed = new ColorCombox(ui->widget_G1);
    Color_Feedarc = new ColorCombox(ui->widget_G2);
    Color_BackGround = new ColorCombox(ui->widget_backGround);
    Color_Origin = new ColorCombox(ui->widget_origin);
    Color_Selected = new ColorCombox(ui->widget_selected);
    Color_Traverse = new ColorCombox(ui->widget_G0);
    Color_limit = new ColorCombox(ui->widget_limit);


    gl_init();


    connect(Color_RealTime,SIGNAL(sigColorChanged(QColor)),
            this,SLOT(slotSetRealTimeColor(QColor)));
    connect(Color_Origin,SIGNAL(sigColorChanged(QColor)),
            this,SLOT(slotSetOriginColor(QColor)));
    connect(Color_Feed,SIGNAL(sigColorChanged(QColor)),
            this,SLOT(slotSetFeedColor(QColor)));
    connect(Color_Feedarc,SIGNAL(sigColorChanged(QColor)),
            this,SLOT(slotSetArcFeedColor(QColor)));
    connect(Color_Selected,SIGNAL(sigColorChanged(QColor)),
            this,SLOT(slotSetSelectedColor(QColor)));
    connect(Color_BackGround,SIGNAL(sigColorChanged(QColor)),
            this,SLOT(slotSetBackGroundColor(QColor)));
    connect(Color_Traverse,SIGNAL(sigColorChanged(QColor)),
            this,SLOT(slotSetTraverseColor(QColor)));
    connect(Color_limit,SIGNAL(sigColorChanged(QColor)),
            this,SLOT(slotSetLimitColor(QColor)));

}

gl_property::~gl_property()
{
    delete ui;
}


void gl_property::slotSetRealTimeColor(QColor RealTime)
{
    reRealTime =  RealTime;
}

void gl_property::slotSetFeedColor(QColor Feed)
{
     reFeed = Feed;
}

void gl_property::slotSetArcFeedColor(QColor ArcFeed)
{
    reArcFeed = ArcFeed;
}

void gl_property::slotSetOriginColor(QColor Origin)
{
    reOrigin = Origin;
}

void gl_property::slotSetSelectedColor(QColor Selected)
{
    reSelected = Selected;
}

void gl_property::slotSetTraverseColor(QColor Traverse)
{
    reTraverse = Traverse;
}

void gl_property::slotSetBackGroundColor(QColor BackGround)
{
    reBackGround = BackGround;
}

void gl_property::slotSetLimitColor(QColor Limit)
{
    reLimit = Limit;
}

void gl_property::gl_init()
{

    setcolor_flag=0;

    QString name;
    QString tmp;
    QString dft;
    QStringList cls;


    //OpenGL/ColorTool

    name = "OpenGL/ColorTraverse";
    if(dft.isEmpty())
       tmp = _prefer->readEntry(name,"1.0|0.0|0.0|1",true);
    else
       tmp = _prefer->readEntry(name,dft,true);
    cls = tmp.split("|");
    reTraverse.setRedF(cls.at(0).toFloat());
    reTraverse.setGreenF(cls.at(1).toFloat());
    reTraverse.setBlueF(cls.at(2).toFloat());
    reTraverse.setAlphaF(cls.at(3).toFloat());


    name = "OpenGL/ColorRealTimePath";
    if(dft.isEmpty())
       tmp = _prefer->readEntry(name,"1.0|0.0|0.0|1",true);
    else
       tmp = _prefer->readEntry(name,dft,true);
    cls = tmp.split("|");
    reRealTime.setRedF(cls.at(0).toFloat());
    reRealTime.setGreenF(cls.at(1).toFloat());
    reRealTime.setBlueF(cls.at(2).toFloat());
    reRealTime.setAlphaF(cls.at(3).toFloat());


    name = "OpenGL/ColorFeed";
    if(dft.isEmpty())
       tmp = _prefer->readEntry(name,"1.0|0.0|0.0|1",true);
    else
       tmp = _prefer->readEntry(name,dft,true);
    cls = tmp.split("|");
    reFeed.setRedF(cls.at(0).toFloat());
    reFeed.setGreenF(cls.at(1).toFloat());
    reFeed.setBlueF(cls.at(2).toFloat());
    reFeed.setAlphaF(cls.at(3).toFloat());

    name = "OpenGL/ColorArcFeed";
    if(dft.isEmpty())
       tmp = _prefer->readEntry(name,"1.0|0.0|0.0|1",true);
    else
       tmp = _prefer->readEntry(name,dft,true);
    cls = tmp.split("|");
    reArcFeed.setRedF(cls.at(0).toFloat());
    reArcFeed.setGreenF(cls.at(1).toFloat());
    reArcFeed.setBlueF(cls.at(2).toFloat());
    reArcFeed.setAlphaF(cls.at(3).toFloat());

    name = "OpenGL/ColorSelected";
    if(dft.isEmpty())
       tmp = _prefer->readEntry(name,"1.0|0.0|0.0|1",true);
    else
       tmp = _prefer->readEntry(name,dft,true);
    cls = tmp.split("|");
    reSelected.setRedF(cls.at(0).toFloat());
    reSelected.setGreenF(cls.at(1).toFloat());
    reSelected.setBlueF(cls.at(2).toFloat());
    reSelected.setAlphaF(cls.at(3).toFloat());

    name = "OpenGL/ColorOrigin";
    if(dft.isEmpty())
       tmp = _prefer->readEntry(name,"1.0|0.0|0.0|1",true);
    else
       tmp = _prefer->readEntry(name,dft,true);
    cls = tmp.split("|");
    reOrigin.setRedF(cls.at(0).toFloat());
    reOrigin.setGreenF(cls.at(1).toFloat());
    reOrigin.setBlueF(cls.at(2).toFloat());
    reOrigin.setAlphaF(cls.at(3).toFloat());

    name = "OpenGL/ColorBackGround";
    if(dft.isEmpty())
       tmp = _prefer->readEntry(name,"1.0|0.0|0.0|1",true);
    else
       tmp = _prefer->readEntry(name,dft,true);
    cls = tmp.split("|");
    reBackGround.setRedF(cls.at(0).toFloat());
    reBackGround.setGreenF(cls.at(1).toFloat());
    reBackGround.setBlueF(cls.at(2).toFloat());
    reBackGround.setAlphaF(cls.at(3).toFloat());

    name = "OpenGL/ColorLimit";
    if(dft.isEmpty())
       tmp = _prefer->readEntry(name,"1.0|0.0|0.0|1",true);
    else
       tmp = _prefer->readEntry(name,dft,true);
    cls = tmp.split("|");
    reLimit.setRedF(cls.at(0).toFloat());
    reLimit.setGreenF(cls.at(1).toFloat());
    reLimit.setBlueF(cls.at(2).toFloat());
    reLimit.setAlphaF(cls.at(3).toFloat());

    //update button color
    Color_RealTime->setButtonIcon(reRealTime);
    Color_Traverse->setButtonIcon(reTraverse);
    Color_Feed->setButtonIcon(reFeed);
    Color_Feedarc->setButtonIcon(reArcFeed);
    Color_Origin->setButtonIcon(reOrigin);
    Color_Selected->setButtonIcon(reSelected);
    Color_BackGround->setButtonIcon(reBackGround);
    Color_limit->setButtonIcon(reLimit);

}



void gl_property::on_sure_btn_clicked()
{
    setcolor_flag=1;
    this->close();
}

void gl_property::on_cancle_btn_clicked()
{
    setcolor_flag=0;
    this->close();
}

void gl_property::showEvent(QShowEvent *)
{
    gl_init();
    ui->sure_btn->setFocus();
}


void gl_property::on_reset_btn_clicked()
{
    QString name;
    QString value;

    name = "OpenGL/ColorRealTimePath";
    value.sprintf("%f|%f|%f|%f",1.0,1.0,0.0,1.0);
    _prefer->writeEntry(name,value);

    name = "OpenGL/ColorSelected";
    value.sprintf("%f|%f|%f|%f",1.0,0.0,0.0,1.0);
    _prefer->writeEntry(name,value);

    name = "OpenGL/ColorTraverse";
    value.sprintf("%f|%f|%f|%f",0.5,0.5,0.5,1.0);
    _prefer->writeEntry(name,value);

    name = "OpenGL/ColorOrigin";
    value.sprintf("%f|%f|%f|%f",0.3,0.8,0.8,1.0);
    _prefer->writeEntry(name,value);

    name = "OpenGL/ColorArcFeed";
    value.sprintf("%f|%f|%f|%f",0.0,1.0,0.6,1.0);
    _prefer->writeEntry(name,value);

    name = "OpenGL/ColorFeed";
    value.sprintf("%f|%f|%f|%f",0.0,1.0,1.0,1.0);
    _prefer->writeEntry(name,value);

    name = "OpenGL/ColorBackGround";
    value.sprintf("%f|%f|%f|%f",0.0,0.0,0.4,1.0);
    _prefer->writeEntry(name,value);

    name = "OpenGL/ColorLimit";
    value.sprintf("%f|%f|%f|%f",0.0,0.5,0.5,1.0);
    _prefer->writeEntry(name,value);

    gl_init();

}

