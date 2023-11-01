#ifndef DATUM_H
#define DATUM_H

#include <QDialog>

namespace Ui {
class Datum;
}

class Datum : public QDialog
{
    Q_OBJECT

public:
    explicit Datum(QWidget *parent = 0);
    ~Datum();

private:
    Ui::Datum *ui;

private slots:
    void home_x();
    void home_y();
    void home_z();
    void home_z2();
    void home_z3();
    void home_z4();

    void unhome_x();
    void unhome_y();
    void unhome_z();
    void unhome_z2();
    void unhome_z3();
    void unhome_z4();

    void Export();
    void Import();

    double get_datum(int axis);
    void home_init();

signals:
    void send_msg(int,QString str);

};

#endif // DATUM_H
