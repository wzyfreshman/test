#ifndef DATUM_PARAMETER_H
#define DATUM_PARAMETER_H

#include <QDialog>
#include "mywidget.h"
#include "datum_explain.h"

namespace Ui {
class Datum_parameter;
}

class Datum_parameter : public QDialog
{
    Q_OBJECT
    
public:
    explicit Datum_parameter(QWidget *parent = 0);
    ~Datum_parameter();

    void updateUI();

signals:
    void show_msg(int,QString);
    
private slots:

    void on_checkBox_clicked();

    void on_btn_recordX_clicked();
    void on_btn_recordY_clicked();
    void on_btn_recordZ_clicked();

    void set_record_x();
    void set_record_y();
    void set_record_z();

    void on_btn_recordY_a_clicked();
    void on_btn_recordZ_a_clicked();

    void set_record_y_a();
    void set_record_z_a();

    void on_btn_recordY_c_clicked();
    void on_btn_recordZ_c_clicked();

    void set_record_y_c();
    void set_record_z_c();

    void set_Material_L();
    void set_Material_W();
    void set_Material_H();


    void on_btn_close_clicked();

    void on_btn_set_a_clicked();
    void on_btn_set_b_clicked();
    void on_btn_set_c_clicked();

    void set_a();
    void set_b();
    void set_c();

    void set_offset_y_a();
    void set_offset_z_a();
    void set_offset_y_c();
    void set_offset_z_c();


    void on_btn_setall_clicked();

    void on_btn_Explain_clicked();

private:
    Ui::Datum_parameter *ui;

    datum_Explain *explain;

    bool Locking_flag;

    MyLabel *lab_record_x;
    MyLabel *lab_record_y;
    MyLabel *lab_record_z;

    MyLabel *lab_record_y_a;
    MyLabel *lab_record_z_a;

    MyLabel *lab_record_y_c;
    MyLabel *lab_record_z_c;

    MyLabel *lab_Material_L;
    MyLabel *lab_Material_W;
    MyLabel *lab_Material_H;


};

#endif // DATUM_PARAMETER_H
