#ifndef OFFSETDIALOG_H
#define OFFSETDIALOG_H

#include <QDialog>
#include "emcstatus.hh"

namespace Ui {
class OffsetDialog;
}

class OffsetDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OffsetDialog(QWidget *parent = 0);
    ~OffsetDialog();

    double public_x;
    double public_y;
    double public_z1;
    double public_z2;

    double piece_x;
    double piece_y;
    double piece_z1;
    double piece_z2;

    double real_offset_x;
    double real_offset_y;
    double real_offset_z1;
    double real_offset_z2;

    double record_x;
    double record_y;
    double dichotomy_x;
    double dichotomy_y;

    QTimer *getTimer();

    void initValue();
    void setFromCurrentPos(AXIS_NAME axis);

protected:
    bool checkModify();
    void reject();
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *, QEvent *);

public slots:
    void set_offset();

    void mdiState(QString);
    void mdiRefresh();
    void mdiExcuteRsult(bool);

    void status_update();

    void get_public_offset_x(double val);
    void get_public_offset_y(double val);
    void get_public_offset_z1(double val);
    void get_public_offset_z2(double val);
    void get_piece_offset_x(double val);
    void get_piece_offset_y(double val);
    void get_piece_offset_z1(double val);
    void get_piece_offset_z2(double val);

    void set_deepen_001();
    void set_deepen_01();
    void set_deepen_1();
    void set_raise_001();
    void set_raise_01();
    void set_raise_1();

    void set_record_x();
    void set_record_y();

    void set_x();
    void set_y();

    void set_dichotomy_x();
    void set_dichotomy_y();
    
private:
    Ui::OffsetDialog *ui;
    double org_piece_x;
    double org_piece_y;
    double org_piece_z1;
    double org_piece_z2;
    bool fromParent;

    QWidget *parentl;
    QTimer *flush_time;
    QList<QString> mdimsg;
    QList<int> mdiserials;
};

#endif // OFFSETDIALOG_H
