#ifndef SET_FEED_H
#define SET_FEED_H

#include <QDialog>
#include "mywidget.h"
#include <QHBoxLayout>

namespace Ui {
class set_feed;
}

class set_feed : public QDialog
{
    Q_OBJECT
    
public:
    explicit set_feed(QWidget *parent = 0);
    ~set_feed();

signals:
    void show_msg(int,QString);
    
private slots:
    void on_btn_close_clicked();

    void set_a_feed(int id);
    void set_b_feed(int id);
    void set_c_feed(int id);

    void set_a_fast();
    void set_b_fast();
    void set_c_fast();

    void set_turn_delay();


private:
    Ui::set_feed *ui;

    QList<QFrame *> set_a_z;
    MyLabel *lab_set_a_feed[6];

    QList<QFrame *> set_b_z;
    MyLabel *lab_set_b_feed[6];

    QList<QFrame *> set_c_z;
    MyLabel *lab_set_c_feed[6];


    MyLabel *lab_set_fast_distance_a;
    MyLabel *lab_set_fast_distance_b;
    MyLabel *lab_set_fast_distance_c;

    MyLabel *lab_turn_delay;

//    QFrame *set_a_z;
    int index_c;



};





#endif // SET_FEED_H
