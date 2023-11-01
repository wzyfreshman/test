#ifndef NUMSTR_INPUTDIALOG_H
#define NUMSTR_INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class NumStr_InputDialog;
}

class NumStr_InputDialog : public QDialog
{
    Q_OBJECT
public:
    static QString inputtext;

public:
    explicit NumStr_InputDialog(QWidget *parent = 0);
    ~NumStr_InputDialog();
public slots:
    void setStrings(QString);
private:
    Ui::NumStr_InputDialog *ui;
};

#endif // NUMSTR_INPUTDIALOG_H
