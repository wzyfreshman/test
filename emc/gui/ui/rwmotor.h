#ifndef RWMOTOR_H
#define RWMOTOR_H

#define MAXPARNUM 200
#define AXISNUM 200

#include <QDialog>
#include <QModelIndex>
#include <QTabWidget>
#include <QSpinBox>

namespace Ui {
class RWMOTOR;
}
class MOTDATA {
  public:
    QString index;
    QString subindex;
    QString datatype;
    QString detial;
    QString value;
    QString newvalue;

    QVariant min;
    QVariant max;
};

class RWMOTOR : public QWidget
{
    Q_OBJECT

public:
    explicit RWMOTOR(QWidget *parent = 0,int setaxis = 0);
    ~RWMOTOR();

    int readfile();
    void initTable();
    void ensure_clicked();

private slots:
    void on_partable_doubleClicked(const QModelIndex &index);


private:
    Ui::RWMOTOR *ui;

    MOTDATA data[MAXPARNUM];
    int parcount;
    QStringList modifyList;
    int axis;
    int motor_chose;
};


class MULTIMOTOR : public QDialog{
    Q_OBJECT

public:
    explicit MULTIMOTOR(QWidget *parent = 0);

public slots:
    void ensure();
    void quit();
    void initAxisData();

private:
    RWMOTOR *axis[AXISNUM];
    QWidget *mainframe,*choseaxis;
    QTabWidget *tab;
    QWidget *btnframe;
    QPushButton *ensurebtn,*abortbtn;
    QPushButton *btn_axis;
    QSpinBox *spaxisnum;

};

#endif // RWMOTOR_H
