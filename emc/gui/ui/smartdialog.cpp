#include "smartdialog.h"
#include "ui_smartdialog.h"
#include"extern.h"
#include "shcom.hh"
#include "emcstatus.hh"

#include <QSettings>
#include <QFile>
#include <QtDebug>
#include <QTextStream>
#include <QMessageBox>
#include <QMovie>
#include <QFileDialog>





SmartControl::SmartControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Smartdialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("SmartControl"));
    /*******rect bottom*********/
    rect = new Rect_Bottom();
    ui->rect_total_deep->setValue(rect->total_deep);
    ui->rect_level_deep->setValue(rect->level_deep);
    ui->rect_tool_dia->setValue(rect->tool_dia);
    ui->rect_tool_space->setValue(rect->tool_space);
    ui->rect_width->setValue(rect->width);
    ui->rect_height->setValue(rect->height);
    ui->rect_start_coorx->setValue(rect->start_x);
    ui->rect_start_coory->setValue(rect->start_y);
    ui->rect_speed->setValue(rect->speed);
    ui->rect_zspeed->setValue(rect->zspeed);
    0 == rect->direction ? ui->rect_horizon->setChecked(true) :
                           ui->rect_vertical->setChecked(true);
    connect(ui->rect_total_deep,SIGNAL(valueChanged(double)),rect,SLOT(set_total_deep(double)));
    connect(ui->rect_level_deep,SIGNAL(valueChanged(double)),rect,SLOT(set_level_deep(double)));
    connect(ui->rect_tool_dia,SIGNAL(valueChanged(double)),rect,SLOT(set_tool_dia(double)));
    connect(ui->rect_tool_space,SIGNAL(valueChanged(double)),rect,SLOT(set_tool_space(double)));
    connect(ui->rect_width,SIGNAL(valueChanged(double)),rect,SLOT(set_width(double)));
    connect(ui->rect_height,SIGNAL(valueChanged(double)),rect,SLOT(set_height(double)));
    connect(ui->rect_start_coorx,SIGNAL(valueChanged(double)),rect,SLOT(set_start_x(double)));
    connect(ui->rect_start_coory,SIGNAL(valueChanged(double)),rect,SLOT(set_start_y(double)));
    connect(ui->rect_speed,SIGNAL(valueChanged(double)),rect,SLOT(set_speed(double)));
    connect(ui->rect_zspeed,SIGNAL(valueChanged(double)),rect,SLOT(set_zspeed(double)));
    connect(ui->rect_horizon,SIGNAL(clicked(bool)),rect,SLOT(set_horizon()));
    connect(ui->rect_vertical,SIGNAL(clicked(bool)),rect,SLOT(set_vertical()));
    connect(ui->rect_horizon,SIGNAL(clicked(bool)),this,SLOT(setRectho()));
    connect(ui->rect_vertical,SIGNAL(clicked(bool)),this,SLOT(setRectve()));
    connect(rect,SIGNAL(send_msg(int,QString)),this,SIGNAL(send_msg(int,QString)));
    /**************CIRCLE BOTTOM***************************/
    circle_bottom = new Circle_Bottom();
    ui->cir_bottom_total_deep->setValue(circle_bottom->total_deep);
    ui->cir_bottom_level_deep->setValue(circle_bottom->level_deep);
    ui->cir_bottom_tool_dia->setValue(circle_bottom->tool_dia);
    ui->cir_bottom_dia_space->setValue(circle_bottom->tool_space);
    ui->circle_bottom_radius->setValue(circle_bottom->radius);
    ui->circle_bottom_centerx->setValue(circle_bottom->center_x);
    ui->circle_bottom_centery->setValue(circle_bottom->center_y);
    ui->cir_bottom_speed->setValue(circle_bottom->speed);
    ui->cir_bottom_zspeed->setValue(circle_bottom->zspeed);
    connect(ui->cir_bottom_total_deep,SIGNAL(valueChanged(double)),circle_bottom,SLOT(set_total_deep(double)));
    connect(ui->cir_bottom_level_deep,SIGNAL(valueChanged(double)),circle_bottom,SLOT(set_level_deep(double)));
    connect(ui->cir_bottom_tool_dia,SIGNAL(valueChanged(double)),circle_bottom,SLOT(set_tool_dia(double)));
    connect(ui->cir_bottom_dia_space,SIGNAL(valueChanged(double)),circle_bottom,SLOT(set_tool_space(double)));
    connect(ui->circle_bottom_radius,SIGNAL(valueChanged(double)),circle_bottom,SLOT(set_radius(double)));
    connect(ui->circle_bottom_centerx,SIGNAL(valueChanged(double)),circle_bottom,SLOT(set_center_x(double)));
    connect(ui->circle_bottom_centery,SIGNAL(valueChanged(double)),circle_bottom,SLOT(set_center_y(double)));
    connect(ui->cir_bottom_speed,SIGNAL(valueChanged(double)),circle_bottom,SLOT(set_speed(double)));
    connect(ui->cir_bottom_zspeed,SIGNAL(valueChanged(double)),circle_bottom,SLOT(set_zspeed(double)));
    connect(circle_bottom,SIGNAL(send_msg(int,QString)),this,SIGNAL(send_msg(int,QString)));
    /*******RECT FRAME*********/
    rectframe = new Rect_Frame();
    ui->rect_frame_total_deep->setValue(rectframe->total_deep);
    ui->rect_frame_level_deep->setValue(rectframe->level_deep);
    ui->rect_frame_tool_dia->setValue(rectframe->tool_dia);
    ui->rect_frame_width->setValue(rectframe->width);
    ui->rect_frame_height->setValue(rectframe->height);
    ui->rect_frame_startx->setValue(rectframe->start_x);
    ui->rect_frame_starty->setValue(rectframe->start_y);
    ui->rect_frame_speed->setValue(rectframe->speed);
    ui->rect_frame_zspeed->setValue(rectframe->zspeed);
    0 == rectframe->direction ? ui->rect_frame_horizon->setChecked(true) :
                           ui->rect_frame_vertical->setChecked(true);
    connect(ui->rect_frame_total_deep,SIGNAL(valueChanged(double)),rectframe,SLOT(set_total_deep(double)));
    connect(ui->rect_frame_level_deep,SIGNAL(valueChanged(double)),rectframe,SLOT(set_level_deep(double)));
    connect(ui->rect_frame_tool_dia,SIGNAL(valueChanged(double)),rectframe,SLOT(set_tool_dia(double)));
    connect(ui->rect_frame_width,SIGNAL(valueChanged(double)),rectframe,SLOT(set_width(double)));
    connect(ui->rect_frame_height,SIGNAL(valueChanged(double)),rectframe,SLOT(set_height(double)));
    connect(ui->rect_frame_startx,SIGNAL(valueChanged(double)),rectframe,SLOT(set_start_x(double)));
    connect(ui->rect_frame_starty,SIGNAL(valueChanged(double)),rectframe,SLOT(set_start_y(double)));
    connect(ui->rect_frame_speed,SIGNAL(valueChanged(double)),rectframe,SLOT(set_speed(double)));
    connect(ui->rect_frame_zspeed,SIGNAL(valueChanged(double)),rectframe,SLOT(set_zspeed(double)));
    connect(ui->rect_frame_horizon,SIGNAL(clicked(bool)),rectframe,SLOT(set_horizon()));
    connect(ui->rect_frame_vertical,SIGNAL(clicked(bool)),rectframe,SLOT(set_vertical()));
    connect(ui->rect_frame_horizon,SIGNAL(clicked(bool)),this,SLOT(setRectin()));
    connect(ui->rect_frame_vertical,SIGNAL(clicked(bool)),this,SLOT(setRectout()));
    connect(rectframe,SIGNAL(send_msg(int,QString)),this,SIGNAL(send_msg(int,QString)));
    /*******RECT FRAME*********/
    circleframe = new Circle_Frame();
    ui->cir_frame_total_deep->setValue(circleframe->total_deep);
    ui->cir_frame_level_deep->setValue(circleframe->level_deep);
    ui->cir_frame_dia->setValue(circleframe->tool_dia);
    ui->circle_frame_radius->setValue(circleframe->radius);
    ui->circle_frame_centerx->setValue(circleframe->center_x);
    ui->circle_frame_centery->setValue(circleframe->center_y);
    ui->cir_frame_speed->setValue(circleframe->speed);
    ui->cir_frame_zspeed->setValue(circleframe->zspeed);
    0 == circleframe->direction ? ui->circle_frame_horizon->setChecked(true) :
                           ui->circle_frame_vertical->setChecked(true);
    connect(ui->cir_frame_total_deep,SIGNAL(valueChanged(double)),circleframe,SLOT(set_total_deep(double)));
    connect(ui->cir_frame_level_deep,SIGNAL(valueChanged(double)),circleframe,SLOT(set_level_deep(double)));
    connect(ui->cir_frame_dia,SIGNAL(valueChanged(double)),circleframe,SLOT(set_tool_dia(double)));
    connect(ui->circle_frame_radius,SIGNAL(valueChanged(double)),circleframe,SLOT(set_radius(double)));
    connect(ui->circle_frame_centerx,SIGNAL(valueChanged(double)),circleframe,SLOT(set_center_x(double)));
    connect(ui->circle_frame_centery,SIGNAL(valueChanged(double)),circleframe,SLOT(set_center_y(double)));
    connect(ui->cir_frame_speed,SIGNAL(valueChanged(double)),circleframe,SLOT(set_speed(double)));
    connect(ui->cir_frame_zspeed,SIGNAL(valueChanged(double)),circleframe,SLOT(set_zspeed(double)));
    connect(ui->circle_frame_horizon,SIGNAL(clicked(bool)),circleframe,SLOT(set_horizon()));
    connect(ui->circle_frame_vertical,SIGNAL(clicked(bool)),circleframe,SLOT(set_vertical()));
    connect(ui->circle_frame_horizon,SIGNAL(clicked(bool)),this,SLOT(setCirin()));
    connect(ui->circle_frame_vertical,SIGNAL(clicked(bool)),this,SLOT(setCirtout()));
    connect(circleframe,SIGNAL(send_msg(int,QString)),this,SIGNAL(send_msg(int,QString)));

    connect(ui->btn_execute,SIGNAL(released()),this,SLOT(smart_execute()));
    connect(ui->btn_cancel,SIGNAL(released()),this,SLOT(close()));
    moRectflact = NULL;
    moRectframe = NULL;
    moCirframe = NULL;
    if(0 == rect->direction)
        setRectho();
    else
        setRectve();
    if(0 == rectframe->direction)
        setRectin();
    else
        setRectout();
    if(0 == circleframe->direction)
        setCirin();
    else
        setCirtout();
}

void SmartControl::smart_execute()
{
    int ret = 0;
    if(0 == ui->tabWidget->currentIndex()) {
        ret = rect->build();
//        emit(send_msg(0,tr("execute the rect bottom program\n")));
    }
    if(1 == ui->tabWidget->currentIndex()) {
        ret = circle_bottom->build();
//        emit(send_msg(0,tr("execute the circle bottom program\n")));
    }
    if(2 == ui->tabWidget->currentIndex()) {
        ret = rectframe->build();
//        emit(send_msg(0,tr("execute the rect frame program\n")));
    }
    if(3 == ui->tabWidget->currentIndex()) {
        ret = circleframe->build();
//        emit(send_msg(0,tr("execute the circle frame program\n")));
    }
    if(ret < 0) {
        this->close();
        return;
    }
//    ensure_mode(EMC_TASK_MODE_AUTO);
//    sendAbort();
//    if(0 != ensure_mode(EMC_TASK_MODE_AUTO))
//        return;

//    QString temp = _subDir;
//    temp.append("smartControl.ngc");
//    sendProgramOpen(temp.toLatin1().data());
//    sendProgramRun(-2,0);

//    this->close();


    QString temp = _subDir;
    temp.append("smartControl.ngc");
    QString file_temp = "";
    file_temp = QFileDialog::getSaveFileName(this,
                                             tr("Save File"),"/home/cnext/NC/smartControl.NC",tr("Files(*.NC)"));
    if(file_temp == NULL)
        return;

    QString file_text = "";
    file_text = read_data_from_file(temp);
    write_data_to_file(file_temp,file_text);
            emit(send_msg(0,tr("Generate  program %1").arg(file_temp)));

}

void SmartControl::setRectho()
{
    if(moRectflact != NULL)
        delete moRectflact;
    moRectflact = new QMovie();
    moRectflact->setFileName(":/new/prefix1/images/rectrow.gif");
    ui->rectframe->setMovie(moRectflact);
    moRectflact->start();
}
void SmartControl::setRectve()
{
    if(moRectflact != NULL)
        delete moRectflact;
    moRectflact = new QMovie();
    moRectflact->setFileName(":/new/prefix1/images/rectcolumn.gif");
    ui->rectframe->setMovie(moRectflact);
    moRectflact->start();
}
void SmartControl::setRectin()
{

    if(moRectframe != NULL)
        delete moRectframe;
    moRectframe = new QMovie();
    moRectframe->setFileName(":/new/prefix1/images/rectinside.gif");
    ui->rect_pic->setMovie(moRectframe);
    moRectframe->start();
}
void SmartControl::setRectout()
{
    if(moRectframe != NULL)
        delete moRectframe;
    moRectframe = new QMovie();
    moRectframe->setFileName(":/new/prefix1/images/rectoutside.gif");
    ui->rect_pic->setMovie(moRectframe);
    moRectframe->start();
}
void SmartControl::setCirin()
{
    if(moCirframe != NULL)
        delete moCirframe;
    moCirframe = new QMovie();
    moCirframe->setFileName(":/new/prefix1/images/circleinside.gif");
    ui->circle_pic->setMovie(moCirframe);
    moCirframe->start();
}
void SmartControl::setCirtout()
{
    if(moCirframe != NULL)
        delete moCirframe;
    moCirframe = new QMovie();
    moCirframe->setFileName(":/new/prefix1/images/circleoutside.gif");
    ui->circle_pic->setMovie(moCirframe);
    moCirframe->start();
}
SmartControl::~SmartControl()
{
    delete ui;
}

Rect_Bottom::Rect_Bottom()
{
    QSettings *key = new QSettings(PREFER_FILE,QSettings::IniFormat);
    key->beginGroup("SMART_RECT");
    total_deep = key->value("total_deep").toDouble();
    level_deep = key->value("level_deep").toDouble();
    tool_dia = key->value("tool_dia").toDouble();
    tool_space = key->value("tool_space").toDouble();
    width = key->value("width").toDouble();
    height = key->value("height").toDouble();
    start_x = key->value("start_x").toDouble();
    start_y = key->value("start_y").toDouble();
    direction = key->value("direction").toDouble();
    speed = key->value("speed").toDouble();
    zspeed = key->value("zspeed").toDouble();
    key->endGroup();
    delete key;


}

void Rect_Bottom::set_total_deep(double val){
    total_deep = val;
}
void Rect_Bottom::set_level_deep(double val){
    level_deep = val;
}
void Rect_Bottom::set_tool_dia(double val){
    tool_dia = val;
}
void Rect_Bottom::set_tool_space(double val){
    tool_space = val;
}
void Rect_Bottom::set_width(double val){
    width = val;
}
void Rect_Bottom::set_height(double val){
    height = val;
}
void Rect_Bottom::set_start_x(double val){
    start_x = val;
}
void Rect_Bottom::set_start_y(double val){
    start_y = val;
}
void Rect_Bottom::set_speed(double val){
    speed = val;
}
void Rect_Bottom::set_zspeed(double val){
    zspeed = val;
}
void Rect_Bottom::set_horizon(){
    direction = 0;
}
void Rect_Bottom::set_vertical(){
    direction = 1;
}
int Rect_Bottom::build()
{
    int ret = 0;
    double startx = 0,starty = 0;
    double xlen = 0,ylen = 0;
    double zpos = 0;
    double offsetz = emcStatus->task.g5x_offset.tran.z;

    QString smartfile = _subDir;
    QFile file(smartfile.append("smartControl.ngc"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return -1;
    QTextStream out(&file);
    out<<"%"<<"\n"<<"O001 if [#4034 EQ 1]"<<"\n"
      <<"m8"<<"\n"<<"O001 endif"<<"\n";
    if(0 == get_actual_spindle_vel()) {
        if(par_get_var(4084) == 0)
            out<<"m3"<<"\n";
        else
            out<<"m3"<<"\n";
    }

 //   if((offsetz + par_get_var(4030)) < 0)
  //      out<<"g0z"<<par_get_var(4030)<<"\n";
   // else
       out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
    out<<"g"<<53+par_get_var(5220)<<"g0x0y0\n";
    startx = start_x + tool_dia/2;
    starty = start_y + tool_dia/2;
    xlen = width - tool_dia;
    ylen = height - tool_dia;

    if(0 == level_deep && 0 == total_deep) {
        goto in_loop;
    }else if (0 == level_deep && 0 != total_deep) {
        emit(send_msg(2,tr("level_deep cannot be zero!!\n")));
        ret = -1;
        return ret;
    }
/*    if(tool_space > tool_dia) {
        emit(send_msg(2,tr("tool_space cannot greater than radius")));
        ret = -1;
        return ret;
    }
*/
    for(zpos = level_deep;zpos < total_deep + level_deep;) {
in_loop:
        if(zpos > total_deep)
            zpos = total_deep;

        if((offsetz + par_get_var(4030)) < 0)
            out<<"g0z"<<par_get_var(4030)<<"\n";
        else
           out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
        out<<"g1x"<<startx<<"y"<<starty<<"f"<<speed<<"\n";
        out<<"g1z0"<<"f"<<zspeed<<"\n";
        out<<"g1z"<<-zpos<<"f"<<zspeed<<"\n";
        out<<"f"<<speed<<"\n";
        out<<"x"<<startx<<"\n";
        if(0 == direction) {
            int face = 1;

            for(double space_all = tool_space;space_all < ylen+tool_space;
                space_all += tool_space) {

                out<<"g91x"<<face* xlen<<"\n";
                if(space_all < ylen)
                out<<"g90y"<<starty + space_all<<"\n";
                else {
                    out<<"g90y"<<starty+ylen<<"\n";
                    if(ylen != space_all-tool_space)
                    out<<"g91x"<<-face* xlen<<"\n"<<"g90\n";
                }

                face = -face;
                //qDebug()<<space_all;
            }
        } else {
            int face = 1;
            for(double space_all = tool_space;space_all < xlen+tool_space;
                space_all += tool_space){
                out<<"g91y"<<face* ylen<<"\n";
                if(space_all < xlen)
                out<<"g90x"<<startx + space_all<<"\n";
                else {
                    out<<"g90x"<<startx+xlen<<"\n";
                    if(xlen != space_all-tool_space)
                    out<<"g91y"<<-face* ylen<<"\n"<<"g90\n";
                }
                face = -face;
            }
        }
        if((offsetz + par_get_var(4030)) < 0)
            out<<"g0z"<<par_get_var(4030)<<"\n";
        else
            out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
        out<<"g"<<53+par_get_var(5220)<<"g0x"<<startx<<"y"<<starty<<"\n";
        out<<"g1z0"<<"f"<<zspeed<<"\n";
        out<<"g1z"<<-zpos<<"f"<<zspeed<<"\n";
        out<<"f"<<speed<<"\n";
        out<<"y"<<starty+ylen<<"\n";
        out<<"x"<<startx+xlen<<"\n";
        out<<"y"<<starty<<"\n";
        out<<"x"<<startx<<"\n";
        zpos += level_deep;
    }
    if((offsetz + par_get_var(4030)) < 0)
        out<<"g0z"<<par_get_var(4030)<<"\n";
    else
       out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
    out<<"O004 if [#4035 EQ 1]\n"
      <<"m9\n"<<"O004 endif\n"
      <<"O003 if [#4033 EQ 1]\n"
      <<"m5\n"<<"O003 endif\n"
      <<"O005 if [#4022 EQ 1]\n"
      <<"G53G90G01Z0F[#4023]\n"
      <<"G53G90G01X[#4024]Y[#4025]F[#4023]\n"
      <<"G53G90G01Z[#4026]F[#4023]\n"
      <<"O005 else if [#4022 EQ 2]\n"
      <<"G53G90G01Z0F[#4023]\n"
      <<"G01X[#4027]Y[#4028]F[#4023]\n"
      <<"O005 endif\n""%"<<"\n";
    file.close();
    _prefer->writeEntry("SMART_RECT/total_deep",QString::number(total_deep,'f',3));
    _prefer->writeEntry("SMART_RECT/level_deep",QString::number(level_deep,'f',3));
    _prefer->writeEntry("SMART_RECT/tool_dia",QString::number(tool_dia,'f',3));
    _prefer->writeEntry("SMART_RECT/tool_space",QString::number(tool_space,'f',3));
    _prefer->writeEntry("SMART_RECT/start_x",QString::number(start_x,'f',3));
    _prefer->writeEntry("SMART_RECT/start_y",QString::number(start_y,'f',3));
    _prefer->writeEntry("SMART_RECT/width",QString::number(width,'f',3));
    _prefer->writeEntry("SMART_RECT/height",QString::number(height,'f',3));
    _prefer->writeEntry("SMART_RECT/speed",QString::number(speed,'f',3));
    _prefer->writeEntry("SMART_RECT/zspeed",QString::number(zspeed,'f',3));
    _prefer->writeEntry("SMART_RECT/direction",QString::number(direction,'f',3));
    return ret;
}

Circle_Bottom::Circle_Bottom()
{
    QSettings *key = new QSettings(PREFER_FILE,QSettings::IniFormat);
    key->beginGroup("SMART_CIRCLE_BOTTOM");
    total_deep = key->value("total_deep").toDouble();
    level_deep = key->value("level_deep").toDouble();
    tool_dia = key->value("tool_dia").toDouble();
    tool_space = key->value("tool_space").toDouble();
    center_x = key->value("center_x").toDouble();
    center_y = key->value("center_y").toDouble();
    radius = key->value("radius").toDouble();
    speed = key->value("speed").toDouble();
    zspeed = key->value("zspeed").toDouble();
    key->endGroup();
    delete key;
}

void Circle_Bottom::set_total_deep(double val){
    total_deep = val;
}
void Circle_Bottom::set_level_deep(double val){
    level_deep = val;
}
void Circle_Bottom::set_tool_dia(double val){
    tool_dia = val;
}
void Circle_Bottom::set_tool_space(double val){
    tool_space = val;
}
void Circle_Bottom::set_center_x(double val){
    center_x = val;
}
void Circle_Bottom::set_center_y(double val){
    center_y = val;
}
void Circle_Bottom::set_radius(double val){
    radius = val;
}
void Circle_Bottom::set_speed(double val){
    speed = val;
}
void Circle_Bottom::set_zspeed(double val){
    zspeed = val;
}
int Circle_Bottom::build(){
    int ret = 0;
    double centerx = 0,centery = 0;
    double zpos = 0;
    double maxlen = 0;
    double offsetz = emcStatus->task.g5x_offset.tran.z;

    QString smartfile = _subDir;
    QFile file(smartfile.append("smartControl.ngc"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return -1;
    QTextStream out(&file);
    out<<"%"<<"\n"<<"O001 if [#4034 EQ 1]"<<"\n"
      <<"m8"<<"\n"<<"O001 endif"<<"\n";
    if(0 == get_actual_spindle_vel()) {
        if(par_get_var(4084) == 0)
            out<<"m3"<<"\n";
        else
            out<<"m3"<<"\n";
    }
 //   if((offsetz + par_get_var(4030)) < 0)
 //       out<<"g0z"<<par_get_var(4030)<<"\n";
  //  else
       out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
    out<<"g"<<53+par_get_var(5220)<<"g0x0y0\n";
    centerx = center_x;
    centery = center_y;
    maxlen = radius - tool_dia/2;

    if(0 == level_deep && 0 == total_deep) {
        goto in_loop;
    }else if (0 == level_deep && 0 != total_deep) {
        emit(send_msg(2,tr("level_deep cannot be zero!!\n")));
        ret = -1;
        return ret;
    }
/*
    if(tool_space > tool_dia) {
        emit(send_msg(2,tr("tool_space cannot greater than radius")));
        ret = -1;
        return ret;
    }
*/
    for(zpos = level_deep;zpos < total_deep + level_deep;) {
in_loop:
        if(zpos > total_deep)
            zpos = total_deep;

        if((offsetz + par_get_var(4030)) < 0)
            out<<"g0z"<<par_get_var(4030)<<"\n";
        else
            out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
        out<<"g1x"<<centerx<<"y"<<centery<<"f"<<speed<<"\n";
        out<<"g1z0"<<"f"<<zspeed<<"\n";
        out<<"g1z"<<-zpos<<"f"<<zspeed<<"\n";
        out<<"f"<<speed<<"\n";
        out<<"x"<<centerx<<"\n";
        for(double len = tool_space;len < maxlen + tool_space;){
            if(len > maxlen)
                len = maxlen;
            out<<"g1x"<<centerx+len<<"\n";
            out<<"g17g2i"<<-len<<"\n";
            len+=tool_space;
        }
        if((offsetz + par_get_var(4030)) < 0)
            out<<"g0z"<<par_get_var(4030)<<"\n";
        else
            out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
        out<<"g"<<53+par_get_var(5220)<<"g0x"<<centerx<<"y"<<centery<<"\n";
        zpos += level_deep;
    }

    if((offsetz + par_get_var(4030)) < 0)
        out<<"g0z"<<par_get_var(4030)<<"\n";
    else
       out<<"g0g53z"<<par_get_var(4055)-1<<"\n";

    out<<"O004 if [#4035 EQ 1]\n"
      <<"m9\n"<<"O004 endif\n"
      <<"O003 if [#4033 EQ 1]\n"
      <<"m5\n"<<"O003 endif\n"
      <<"O005 if [#4022 EQ 1]\n"
      <<"G53G90G01Z0F[#4023]\n"
      <<"G53G90G01X[#4024]Y[#4025]F[#4023]\n"
      <<"G53G90G01Z[#4026]F[#4023]\n"
      <<"O005 else if [#4022 EQ 2]\n"
      <<"G53G90G01Z0F[#4023]\n"
      <<"G01X[#4027]Y[#4028]F[#4023]\n"
      <<"O005 endif\n""%"<<"\n";

    _prefer->writeEntry("SMART_CIRCLE_BOTTOM/total_deep",QString::number(total_deep,'f',3));
    _prefer->writeEntry("SMART_CIRCLE_BOTTOM/level_deep",QString::number(level_deep,'f',3));
    _prefer->writeEntry("SMART_CIRCLE_BOTTOM/tool_dia",QString::number(tool_dia,'f',3));
    _prefer->writeEntry("SMART_CIRCLE_BOTTOM/tool_space",QString::number(tool_space,'f',3));
    _prefer->writeEntry("SMART_CIRCLE_BOTTOM/center_x",QString::number(center_x,'f',3));
    _prefer->writeEntry("SMART_CIRCLE_BOTTOM/center_y",QString::number(center_y,'f',3));
    _prefer->writeEntry("SMART_CIRCLE_BOTTOM/radius",QString::number(radius,'f',3));
    _prefer->writeEntry("SMART_CIRCLE_BOTTOM/speed",QString::number(speed,'f',3));
    _prefer->writeEntry("SMART_CIRCLE_BOTTOM/zspeed",QString::number(zspeed,'f',3));
    return ret;
}

/*****************Rect_Frame*****************/
Rect_Frame::Rect_Frame()
{
    QSettings *key = new QSettings(PREFER_FILE,QSettings::IniFormat);
    key->beginGroup("SMART_FRAME_RECT");
    total_deep = key->value("total_deep").toDouble();
    level_deep = key->value("level_deep").toDouble();
    tool_dia = key->value("tool_dia").toDouble();
    width = key->value("width").toDouble();
    height = key->value("height").toDouble();
    start_x = key->value("start_x").toDouble();
    start_y = key->value("start_y").toDouble();
    direction = key->value("direction").toDouble();
    speed = key->value("speed").toDouble();
    zspeed = key->value("zspeed").toDouble();
    key->endGroup();
    delete key;
}

void Rect_Frame::set_total_deep(double val){
    total_deep = val;
}
void Rect_Frame::set_level_deep(double val){
    level_deep = val;
}
void Rect_Frame::set_tool_dia(double val){
    tool_dia = val;
}
void Rect_Frame::set_width(double val){
    width = val;
}
void Rect_Frame::set_height(double val){
    height = val;
}
void Rect_Frame::set_start_x(double val){
    start_x = val;
}
void Rect_Frame::set_start_y(double val){
    start_y = val;
}
void Rect_Frame::set_speed(double val){
    speed = val;
}
void Rect_Frame::set_zspeed(double val){
    zspeed = val;
}
void Rect_Frame::set_horizon(){
    direction = 0;
}
void Rect_Frame::set_vertical(){
    direction = 1;
}
int Rect_Frame::build(){
    int ret = 0;
    double startx = 0,starty = 0;
    double xlen = 0,ylen = 0;
    double zpos = 0;
    double offsetz = emcStatus->task.g5x_offset.tran.z;

    QString smartfile = _subDir;
    QFile file(smartfile.append("smartControl.ngc"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return -1;
    QTextStream out(&file);
    out<<"%"<<"\n"<<"\n"<<"O001 if [#4034 EQ 1]"<<"\n"
      <<"m8"<<"\n"<<"O001 endif"<<"\n";
    if(0 == get_actual_spindle_vel()) {
        if(par_get_var(4084) == 0)
            out<<"m3"<<"\n";
        else
            out<<"m3"<<"\n";
    }

 //   if((offsetz + par_get_var(4030)) < 0)
 //       out<<"g0z"<<par_get_var(4030)<<"\n";
 //   else
        out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
    out<<"g"<<53+par_get_var(5220)<<"g0x0y0\n";
    if(0 == direction){
        startx = start_x + tool_dia/2;
        starty = start_y + tool_dia/2;
        xlen = width - tool_dia;
        ylen = height - tool_dia;
    }else if(1 == direction){
        startx = start_x - tool_dia/2;
        starty = start_y - tool_dia/2;
        xlen = width + tool_dia;
        ylen = height + tool_dia;
    }
    if(0 == level_deep && 0 == total_deep) {
        goto in_loop;
    }else if (0 == level_deep && 0 != total_deep) {
        emit(send_msg(2,tr("level_deep cannot be zero!!")));
        ret = -1;
        return ret;
    }
    for(zpos = level_deep;zpos < total_deep + level_deep;){
in_loop:
        if(zpos > total_deep)
            zpos = total_deep;


        if((offsetz + par_get_var(4030)) < 0)
            out<<"g0z"<<par_get_var(4030)<<"\n";
        else
            out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
        out<<"g1x"<<startx<<"y"<<starty<<"f"<<speed<<"\n";
        out<<"g1z0"<<"f"<<zspeed<<"\n";
        out<<"g1z"<<-zpos<<"f"<<zspeed<<"\n";
        out<<"f"<<speed<<"\n";
        out<<"x"<<startx<<"\n";
        out<<"x"<<startx + xlen<<"\n";
        out<<"y"<<starty + ylen<<"\n";
        out<<"x"<<startx<<"\n";
        out<<"y"<<starty<<"\n";
        zpos += level_deep;
    }
    if((offsetz + par_get_var(4030)) < 0)
        out<<"g0z"<<par_get_var(4030)<<"\n";
    else
       out<<"g0g53z"<<par_get_var(4055)-1<<"\n";

    out<<"O004 if [#4035 EQ 1]\n"
      <<"m9\n"<<"O004 endif\n"
      <<"O003 if [#4033 EQ 1]\n"
      <<"m5\n"<<"O003 endif\n"
      <<"O005 if [#4022 EQ 1]\n"
      <<"G53G90G01Z0F[#4023]\n"
      <<"G53G90G01X[#4024]Y[#4025]F[#4023]\n"
      <<"G53G90G01Z[#4026]F[#4023]\n"
      <<"O005 else if [#4022 EQ 2]\n"
      <<"G53G90G01Z0F[#4023]\n"
      <<"G01X[#4027]Y[#4028]F[#4023]\n"
      <<"O005 endif\n""%"<<"\n";
    file.close();
    _prefer->writeEntry("SMART_FRAME_RECT/total_deep",QString::number(total_deep,'f',3));
    _prefer->writeEntry("SMART_FRAME_RECT/level_deep",QString::number(level_deep,'f',3));
    _prefer->writeEntry("SMART_FRAME_RECT/tool_dia",QString::number(tool_dia,'f',3));
    _prefer->writeEntry("SMART_FRAME_RECT/start_x",QString::number(start_x,'f',3));
    _prefer->writeEntry("SMART_FRAME_RECT/start_y",QString::number(start_y,'f',3));
    _prefer->writeEntry("SMART_FRAME_RECT/width",QString::number(width,'f',3));
    _prefer->writeEntry("SMART_FRAME_RECT/height",QString::number(height,'f',3));
    _prefer->writeEntry("SMART_FRAME_RECT/speed",QString::number(speed,'f',3));
    _prefer->writeEntry("SMART_FRAME_RECT/zspeed",QString::number(zspeed,'f',3));
    _prefer->writeEntry("SMART_FRAME_RECT/direction",QString::number(direction,'f',3));
    return ret;
}

/**********************Circle Frame************************/
Circle_Frame::Circle_Frame()
{
    QSettings *key = new QSettings(PREFER_FILE,QSettings::IniFormat);
    key->beginGroup("SMART_FRAME_CIRCLE");
    total_deep = key->value("total_deep").toDouble();
    level_deep = key->value("level_deep").toDouble();
    tool_dia = key->value("tool_dia").toDouble();
    center_x = key->value("center_x").toDouble();
    center_y = key->value("center_y").toDouble();
    radius = key->value("radius").toDouble();
    speed = key->value("speed").toDouble();
    direction = key->value("direction").toInt();
    zspeed = key->value("zspeed").toDouble();
    key->endGroup();
    delete key;
}

void Circle_Frame::set_total_deep(double val){
    total_deep = val;
}
void Circle_Frame::set_level_deep(double val){
    level_deep = val;
}
void Circle_Frame::set_tool_dia(double val){
    tool_dia = val;
}
void Circle_Frame::set_center_x(double val){
    center_x = val;
}
void Circle_Frame::set_center_y(double val){
    center_y = val;
}
void Circle_Frame::set_radius(double val){
    radius = val;
}
void Circle_Frame::set_speed(double val){
    speed = val;
}
void Circle_Frame::set_zspeed(double val){
    zspeed = val;
}
void Circle_Frame::set_horizon() {
    direction = 0;
}
void Circle_Frame::set_vertical() {
    direction = 1;
}
int Circle_Frame::build() {
    int ret = 0;
    double startx = 0,starty = 0;
    double len = 0;
    double zpos = 0;
    double offsetz = 0;
    QString smartfile = _subDir;
    QFile file(smartfile.append("smartControl.ngc"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return -1;
    QTextStream out(&file);
    out<<"%"<<"\n"<<"\n"<<"O001 if [#4034 EQ 1]"<<"\n"
      <<"m8"<<"\n"<<"O001 endif"<<"\n";
    if(0 == get_actual_spindle_vel()) {
        if(par_get_var(4084) == 0)
            out<<"m3"<<"\n";
        else
            out<<"m3"<<"\n";
    }

    startx = center_x;
    starty = center_y;
    if(0 == direction){
        len = radius - tool_dia/2;
    }else if(1 == direction){
        len = radius + tool_dia/2;
    }
 //   if((offsetz + par_get_var(4030)) < 0)
 //       out<<"g0z"<<par_get_var(4030)<<"\n";
 //   else
       out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
    out<<"g"<<53+par_get_var(5220)<<"g0x0y0\n";
    out<<"g1x"<<startx<<"y"<<starty<<"f"<<speed<<"\n";
    if(0 == level_deep && 0 == total_deep) {
        goto in_loop;
    }else if (0 == level_deep && 0 != total_deep) {
        emit(send_msg(2,tr("level_deep cannot be zero!!")));
        ret = -1;
        return ret;
    }
    for(zpos = level_deep;zpos < total_deep + level_deep;) {
in_loop:
        if(zpos > total_deep)
            zpos = total_deep;

        out<<"g1x"<<startx+len<<"f"<<speed<<"\n";
        out<<"g1z0"<<"f"<<zspeed<<"\n";
        out<<"g1z"<<-zpos<<"f"<<zspeed<<"\n";
        out<<"f"<<speed<<"\n";
        out<<"g17g2i"<<-len<<"\n";
        double offsetz = emcStatus->task.g5x_offset.tran.z;

        if((offsetz + par_get_var(4030)) < 0)
            out<<"g0z"<<par_get_var(4030)<<"\n";
        else
           out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
        zpos += level_deep;
    }
    if((offsetz + par_get_var(4030)) < 0)
        out<<"g0z"<<par_get_var(4030)<<"\n";
    else
       out<<"g0g53z"<<par_get_var(4055)-1<<"\n";
    out<<"O004 if [#4035 EQ 1]\n"
      <<"m9\n"<<"O004 endif\n"
      <<"O003 if [#4033 EQ 1]\n"
      <<"m5\n"<<"O003 endif\n"
      <<"O005 if [#4022 EQ 1]\n"
      <<"G53G90G01Z0F[#4023]\n"
      <<"G53G90G01X[#4024]Y[#4025]F[#4023]\n"
      <<"G53G90G01Z[#4026]F[#4023]\n"
      <<"O005 else if [#4022 EQ 2]\n"
      <<"G53G90G01Z0F[#4023]\n"
      <<"G01X[#4027]Y[#4028]F[#4023]\n"
      <<"O005 endif\n"<<"%"<<"\n";
    file.close();
    _prefer->writeEntry("SMART_FRAME_CIRCLE/total_deep",QString::number(total_deep,'f',3));
    _prefer->writeEntry("SMART_FRAME_CIRCLE/level_deep",QString::number(level_deep,'f',3));
    _prefer->writeEntry("SMART_FRAME_CIRCLE/tool_dia",QString::number(tool_dia,'f',3));
    _prefer->writeEntry("SMART_FRAME_CIRCLE/center_x",QString::number(center_x,'f',3));
    _prefer->writeEntry("SMART_FRAME_CIRCLE/center_y",QString::number(center_y,'f',3));
    _prefer->writeEntry("SMART_FRAME_CIRCLE/radius",QString::number(radius,'f',3));
    _prefer->writeEntry("SMART_FRAME_CIRCLE/speed",QString::number(speed,'f',3));
    _prefer->writeEntry("SMART_FRAME_CIRCLE/zspeed",QString::number(zspeed,'f',3));
    _prefer->writeEntry("SMART_FRAME_CIRCLE/direction",QString::number(direction,'f',3));
    return ret;


}

