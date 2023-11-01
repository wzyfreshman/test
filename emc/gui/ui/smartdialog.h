#ifndef SMARTDIALOG_H
#define SMARTDIALOG_H

#include <QDialog>

namespace Ui {
class Smartdialog;
}
class Rect_Bottom : public QObject
{
    Q_OBJECT
public:
    Rect_Bottom();
    int build();

    double total_deep;
    double level_deep;
    double tool_dia;
    double tool_space;
    double start_x;
    double start_y;
    double width;
    double height;
    double speed;
    double zspeed;
    int direction;
public slots:
    void set_total_deep(double);
    void set_level_deep(double);
    void set_tool_dia(double);
    void set_tool_space(double);
    void set_start_x(double);
    void set_start_y(double);
    void set_width(double);
    void set_height(double);
    void set_horizon();
    void set_vertical();
    void set_speed(double);
    void set_zspeed(double);

signals:
    void send_msg(int,QString str);

};

class Circle_Bottom : public QObject
{
    Q_OBJECT
public:
    Circle_Bottom();
    int build();

    double total_deep;
    double level_deep;
    double tool_dia;
    double tool_space;
    double center_x;
    double center_y;
    double radius;
    double speed;
    double zspeed;

public slots:
    void set_total_deep(double);
    void set_level_deep(double);
    void set_tool_dia(double);
    void set_tool_space(double);
    void set_center_x(double);
    void set_center_y(double);
    void set_radius(double);
    void set_speed(double);
    void set_zspeed(double);

signals:
    void send_msg(int,QString str);
};

class Rect_Frame : public QObject
{
    Q_OBJECT
public:
    Rect_Frame();
    int build();

    double total_deep;
    double level_deep;
    double tool_dia;
    double start_x;
    double start_y;
    double width;
    double height;
    double speed;
    double zspeed;
    int direction;
public slots:
    void set_total_deep(double);
    void set_level_deep(double);
    void set_tool_dia(double);
    void set_start_x(double);
    void set_start_y(double);
    void set_width(double);
    void set_height(double);
    void set_horizon();
    void set_vertical();
    void set_speed(double);
    void set_zspeed(double);

signals:
    void send_msg(int,QString str);
};

class Circle_Frame : public QObject
{
    Q_OBJECT
public:
    Circle_Frame();
    int build();

    double total_deep;
    double level_deep;
    double tool_dia;
    double center_x;
    double center_y;
    double radius;
    double speed;
    double zspeed;
    int direction;

public slots:
    void set_total_deep(double);
    void set_level_deep(double);
    void set_tool_dia(double);
    void set_center_x(double);
    void set_center_y(double);
    void set_radius(double);
    void set_horizon();
    void set_vertical();
    void set_speed(double);
    void set_zspeed(double);

signals:
    void send_msg(int,QString str);
};

class SmartControl : public QDialog
{
    Q_OBJECT

public:
    explicit SmartControl(QWidget *parent = 0);
    ~SmartControl();
public slots:
    void smart_execute();
    void setRectve();
    void setRectho();
    void setRectin();
    void setRectout();
    void setCirin();
    void setCirtout();
private:
    Ui::Smartdialog *ui;
    Rect_Bottom *rect;
    Circle_Bottom *circle_bottom;
    Rect_Frame *rectframe;
    Circle_Frame *circleframe;

    QMovie *moRectflact;
    QMovie *moRectframe;
    QMovie *moCirframe;

signals:
    void send_msg(int,QString str);

};




#endif // SMARTCONTROL_H
