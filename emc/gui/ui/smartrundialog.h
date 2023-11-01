#ifndef SMARTRUNDIALOG_H
#define SMARTRUNDIALOG_H

#include <QDialog>

namespace Ui {
class SmartRunDialog;
}

class SmartRunDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SmartRunDialog(QWidget *parent = 0);
    ~SmartRunDialog();

public slots:
    void runfromline(int line, int endline);
    void setfilename(QString);
    void settotlalines(int);
    void runfile();
    void setFilehead(bool val);
    void setFileend(bool val);
    void setStartline(bool val);
    void setEndLine(bool val);
    void setFastrun(bool);

protected:
    void showEvent(QShowEvent *);
    
private slots:
    void on_safeheight_valueChanged(double arg1);

private:
    Ui::SmartRunDialog *ui;
    QString filepath;
    QString realpath;
    bool startEnable;
    bool endEnable;
    int totalline;
};

#endif // SMARTRUNDIALOG_H
