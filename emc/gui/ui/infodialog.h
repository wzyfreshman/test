#ifndef INFODIALOG_H
#define INFODIALOG_H

#define SYSTEMINFO "/home/.cnc/gui/version"

#include "fileeditor.h"

#include <QDialog>

namespace Ui {
class Infodialog;
}

class Infodialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Infodialog(QWidget *parent = 0);
    ~Infodialog();

public slots:
    void showMoreSftInfo();
    void setSysString();
    void slotSys(QString v);

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::Infodialog *ui;
    FileEditor *logview;
    QString version;
    QString sys_string;
};

#endif // INFODIALOG_H
