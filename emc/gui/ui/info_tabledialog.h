#ifndef INFO_TABLEDIALOG_H
#define INFO_TABLEDIALOG_H

#include <QDialog>
#include <QIcon>

class QTableWidget;

namespace Ui {
class Info_TableDIalog;
}

class Info_TableDIalog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Info_TableDIalog(QWidget *parent = 0);
    ~Info_TableDIalog();
    
    void contextMenuEvent(QContextMenuEvent *);
    QTableWidget * getTableWidget();

protected:
    void reject();
    void resizeEvent(QResizeEvent *);

public slots:
    void insert_info_line(int,QString);
    void clear_table();
    void classify_display();

private:
    Ui::Info_TableDIalog *ui;
    QAction* act_clear;

    QIcon icon_error;
    QIcon icon_warning;
    QIcon icon_info;

};

#endif // INFO_TABLEDIALOG_H
