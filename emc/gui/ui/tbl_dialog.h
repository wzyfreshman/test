#ifndef TBL_DIALOG_H
#define TBL_DIALOG_H

#include <QDialog>
#include "ini_settings.h"

class linuxcnc;
class tooltimes;
class QTableWidgetItem;

namespace Ui {
class Tbl_Dialog;
}
class Tool_Name {
public:
    Tool_Name(){
        tool_no = 0;
        remark = "";
    }
    int tool_no;
    QString remark;
};

class Tbl_Item {

public:
    Tbl_Item(){
        t = 0;
        p = 0;
        z = 0;
        z2 = 0;
        d = 0;

         x1 = 0;
         x2 = 0;
         y1 = 0;
         y2 = 0;

        valid = false;
    }
    Tbl_Item(double _t,double _p,
             double _z,double _z2,double _d,
             double _x1,double _x2,
             double _y1,double _y2){
        t = _t;
        p = _p;
        z = _z;
        z2 = _z2;
        d = _d;

        x1 = _x1;
        x2 = _x2;
        y1 = _y1;
        y2 = _y2;

        valid = false;
    }
    double t;
    double p;
    double z;
    double z2;
    double d;

    double x1;
    double x2;
    double y1;
    double y2;

    bool valid;

};

class Tbl_Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Tbl_Dialog(QWidget *parent = 0);
    ~Tbl_Dialog();
    void setTblFile(QString &file);
    bool loadTool();
    void cleartool();
    void readToolName();
    void getEmcStatues();
    void checkFile();
    void recoveryZ2();
    bool saveTmpZ2();

    tooltimes *toolmanager;

private:
    void addTool();
    bool saveToFile();
    void removeTool();
    void releaseList();

    void InitConnect();
    void InitUI();



protected:
    void closeEvent(QCloseEvent *);
    void reject();
public slots:
    void slotItemchange(QTableWidgetItem *item);
    void slotLoadTool();
    void slotAddTool();
    void slotSaveToFile();
    void slotRemoveTool();
    void myquit();
    void slotSaveAndLoadDown();
    void changeDiffSet(bool checked);
    void usingArrayFile(bool flag);
    void flushDisplay();

private slots:
    void on_btn_page1_clicked();

    void on_btn_page2_clicked();

    void on_btn_setWidth_clicked();

    void on_btn_setHeight_clicked();

    void on_btn_setdisX_clicked();

    void on_btn_setdisY_clicked();

    void on_chk_oterarray_toggled(bool checked);

private:
    void clear();
    void showUpItem();
    void releaseItems();
    void flushCache();
    bool checkChange();
    bool usedTool(int toolno ,QString &str);


    bool getToolItem(int toolno ,Tool_Name *&item);

    //QList< QList<QTableWidgetItem *> >tableitems;
    Ui::Tbl_Dialog *ui;

    QList<Tbl_Item> allitems;
    QList<Tool_Name *> toolnameList;
    QString filename;
    QVector<double> tmpZ2_vec;

    int orgtoolnum;
    linuxcnc *lcnc;


};

#endif // TBL_DIALOG_H
