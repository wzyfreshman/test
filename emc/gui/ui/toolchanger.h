#ifndef TOOLCHANGER_H
#define TOOLCHANGER_H

#include <QWidget>

namespace Ui {
class ToolChanger;
}

class ToolChanger : public QWidget
{
    Q_OBJECT
    
public:
    explicit ToolChanger(QWidget *parent = 0);
    ~ToolChanger();
    
public slots:
    void change_to_chosetool();
    void setSpindleIO();
    void setToolChangerCover();
    void ToolUpdate();

    void changetoTool();

signals:
    void send_msg(int,QString str);


private slots:

    void on_move_c_clicked();

    void on_btn_tool1_clicked();

    void on_btn_tool2_clicked();

    void on_btn_tool3_clicked();

    void on_btn_tool4_clicked();

    void on_btn_tool5_clicked();

    void on_btn_tool6_clicked();

    void on_btn_tool7_clicked();

    void on_btn_tool8_clicked();

    void on_pushButton_2_clicked();

    void on_move_b_clicked();

    void on_spindle2_on_clicked();

    void on_btn_resetB_clicked();

    void on_btn_resetC_clicked();

    void on_move_x_clicked();

    void on_btn_spindlelocation_clicked();

    void on_OffsetAngle_valueChanged(double arg1);


    void on_setangle_clicked(bool checked);

    void on_btn_probe_clicked();

    void on_chk_debug_clicked();

private:
    Ui::ToolChanger *ui;

    int toolnumber;
    int tableType;

    int moveATC = 0;
    int unlockpar = 0;
};

#endif // TOOLCHANGER_H
