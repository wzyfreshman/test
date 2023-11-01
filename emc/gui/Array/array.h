#ifndef ARRAY_H
#define ARRAY_H

#define MAXTOOL 52
#define MAXARRAYNUM 100
#define ARRAY_COMPENSATION_FILE "/home/hong/compensation"
#define PREFER_FILE "/home/.config/ecnc_preference"

#include <QWidget>

#include "node.h"
#include "ini_settings.h"

namespace Ui {
class Array;
}

class Array : public QWidget
{
    Q_OBJECT
    
public:
    explicit Array(QWidget *parent = 0,double prowidth=10,double proheight=10);
    ~Array();

    void caculate();
    int getfile(int row, int column);
    
public slots:
    void reflushtext();
    void cleartext();
    void update();
    void buildarray();
    void setTotalRowComp();
    void setTotalColumnComp();
    void setItemSize(double width,double heigth);
    void ItemGetInfo(QString text);
    void arraySlotBuild();
    void setTotalCompx();
    void setTotalCompy();

signals:
    void loadArrayfile(QString);

protected:
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject *, QEvent *);
    void dragEnterEvent(QDragEnterEvent *);

private:
    Ui::Array *ui;
    Node *node[MAXARRAYNUM];
    QString iteminfo[MAXARRAYNUM];
    int rownum,columnum;
    double itemwidth,itemheigth;
    double distancex,distancey;
    QGraphicsScene *scene;
    QString filepath;
    Ini_Settings *_preferce;
    bool startverify;
};

#endif // ARRAY_H
