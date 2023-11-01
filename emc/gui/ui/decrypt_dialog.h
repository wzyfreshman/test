#ifndef DECRYPTDIALOG_H
#define DECRYPTDIALOG_H

#include <QDialog>

namespace Ui {
class Decrypt_Dialog;
}

class Decrypt_Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Decrypt_Dialog(QWidget *parent = 0);
    ~Decrypt_Dialog();


    void setVerString(QString);
    void init();

public slots:
    void slotCode1(QString);
    void slotCode2(QString);
    void slotCode3(QString);
    void slotCode4(QString);

    void slotCode1_2(QString);
    void slotCode2_2(QString);
    void slotCode3_2(QString);
    void slotCode4_2(QString);

    void slotVer(QString);
    void slotSys(QString);
    void slotSign();
    void update();

    void setSysString();

signals:
    void send_msg(int,QString);

private slots:
    void on_Btn_More_clicked();

private:



    QString sys_string;
    QString ver_string;
    unsigned char sys[4];
    unsigned char ver[4];
    unsigned char code[8];

    unsigned char code_2[8];

    Ui::Decrypt_Dialog *ui;
    QString code1;
    QString code2;
    QString code3;
    QString code4;

    QString code1_2;
    QString code2_2;
    QString code3_2;
    QString code4_2;

    QString vercode;

protected:
    void showEvent(QShowEvent *);

};

#endif // DIALOG_H
