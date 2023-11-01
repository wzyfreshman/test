#ifndef COMPENSATION_H
#define COMPENSATION_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDoubleSpinBox>
#define MAXTOOL 52
#define MAXARRAY 100

namespace Ui {
class compensation;
}

class compensation : public QDialog
{
    Q_OBJECT
    
public:
    explicit compensation(QDialog *parent = 0,int array = 0,int setrow=1,int setcolumn=1
            ,int row = -1,int column = -1);
    ~compensation();

    int readComp();
    void inittable();
public slots:
    void savefile();
    
private:
    Ui::compensation *ui;
    QString compfile;
    int num;
    int toolnum;
    int arraynum;
    int tool[MAXARRAY][MAXTOOL];
    double xcomp[MAXARRAY][MAXTOOL];
    double ycomp[MAXARRAY][MAXTOOL];
    QTableWidgetItem *toolname[MAXTOOL];
    QDoubleSpinBox *toolxcomp[MAXTOOL];
    QDoubleSpinBox *toolycomp[MAXTOOL];
    int totalrow,totalcolumn,setrownum,setcolumnnum;
};

#endif // COMPENSATION_H
