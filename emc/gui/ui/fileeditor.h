#ifndef FILEEDITOR_H
#define FILEEDITOR_H

#include <QMainWindow>
#include <QUndoStack>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>

#include "codeeditor.h"
#include "mywidget.h"
namespace Ui {
class FileEditor;
}


class MyEditor : public CodeEditor
{
    Q_OBJECT

public:
    MyEditor();
    void contextMenuEvent(QContextMenuEvent *);

    QAction * actionCut;
    QAction * actionCopy;
    QAction * actionPaste;
    QAction * actionRedo;
    QAction * actionUndo;
public slots:
    void redoStatus(bool s);
    void undoStatus(bool s);
};

class FileEditor : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit FileEditor( QWidget *parent = 0);
    ~FileEditor();
    void open_file(QString path);
    void open_infoFile(QString path);
    bool save_file();

    void reOpenFile(QString &_tmp,QString &, bool editable);
    void displayText(QString &str);

public slots:
    void undo_status(bool);
    void redo_status(bool);

protected:
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *, QEvent *);
    void resizeEvent(QResizeEvent *);

private slots:
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();

    void on_actionJump_triggered();

signals:
    void re_load(QString str);

private slots:
    void  init_connet();
    void  init_info_display();



private:
    Ui::FileEditor *ui;
    QUndoStack *undo;

    QString usrfile;
    QString tmpfile;

    QString extfile;
    QWidget *prt;
    bool islocalfile ;
    MyEditor *text_browser;

    QLabel *statusShow;

    myReplaceWdiget *myreplace;
    myFindWdiget *myfind;

    bool closeflag;

    QIcon icon_error;
    QIcon icon_warning;
    QIcon icon_info;


};



#endif // FILEEDITOR_H
