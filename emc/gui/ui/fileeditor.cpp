#include "fileeditor.h"
#include "ui_fileeditor.h"
#include "extern.h"
#include "codeeditor.h"
#include "linuxcnc.h"

#include <QFile>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QTextCodec>
#include <QTextBlock>
#include <QtDebug>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QFrame>
#include <QAction>
#include <QShortcut>
#include <QTextStream>


FileEditor::FileEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileEditor)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("File Editor"));
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlags(this->windowFlags() &
                         ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);
    text_browser = new MyEditor();
    closeflag = false;
    QVBoxLayout *text_show_lay = new QVBoxLayout(ui->show_text_widget);
    text_show_lay->addWidget(text_browser);
    text_show_lay->setContentsMargins(0,0,0,0);

    myfind = new myFindWdiget(0,(QPlainTextEdit *)text_browser);
    myreplace = new myReplaceWdiget(0,(QPlainTextEdit *)text_browser);

    ui->actionOpen->setVisible(false);
    ui->btn_open->setEnabled(false);
    text_browser->actionCut = new QAction(tr("Cut  Ctrl+X"),text_browser);
    connect(text_browser->actionCut,SIGNAL(triggered()),text_browser,SLOT(cut()));
    text_browser->actionCopy = new QAction(tr("Copy  Ctrl+C"),text_browser);
    connect(text_browser->actionCopy,SIGNAL(triggered()),text_browser,SLOT(copy()));
    text_browser->actionPaste = new QAction(tr("Paste  Ctrl+V"),text_browser);
    connect(text_browser->actionPaste,SIGNAL(triggered()),text_browser,SLOT(paste()));
    text_browser->actionRedo = ui->actionRedo;
    connect(text_browser,SIGNAL(redoAvailable(bool)),text_browser,SLOT(redoStatus(bool)));
    text_browser->actionUndo = ui->actionUndo;
    connect(text_browser,SIGNAL(undoAvailable(bool)),text_browser,SLOT(undoStatus(bool)));

    connect(text_browser,SIGNAL(undoAvailable(bool)),this,SLOT(undo_status(bool)));
    connect(text_browser,SIGNAL(redoAvailable(bool)),this,SLOT(redo_status(bool)));
    if(parent != 0)
        connect(this,SIGNAL(re_load(QString)),parent,SLOT(pro_open_slot(QString)));
    prt = parent;
    this->installEventFilter(this);

    statusShow = new QLabel();
    statusShow->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    statusShow->setFrameShape(QFrame::NoFrame);
    statusBar()->addWidget(statusShow,1);

    init_connet();
    ui->toolBar->hide();
    ui->btn_open->hide();

}

void FileEditor::displayText(QString &str) {

    ui->show_text_widget->show();
    ui->show_edit_btn->show();
    ui->tableWidget->hide();
    ui->show_info_btn->hide();

    ui->actionUndo->setVisible(false);
    ui->actionRedo->setVisible(false);
    ui->actionReplace->setVisible(false);
    ui->actionLoad->setVisible(false);
    ui->actionOpen->setVisible(false);

    ui->btn_undo->setEnabled(false);
    ui->btn_redo->setEnabled(false);
    ui->btn_replace->setEnabled(false);
    ui->btn_load->setEnabled(false);
    ui->btn_open->setEnabled(false);


    this->text_browser->setUnblockText(str);
    this->text_browser->setReadOnly(true);
    this->setWindowTitle(tr("Solfware Information"));

}

void FileEditor::reOpenFile(QString &_tmpfile,QString &_usrpath,bool editable) {


    if(editable) {
        ui->show_text_widget->show();
        ui->show_edit_btn->show();
        ui->tableWidget->hide();
        ui->show_info_btn->hide();
        ui->actionUndo->setEnabled(false);
        ui->actionRedo->setEnabled(false);
        ui->btn_undo->setEnabled(false);
        ui->btn_redo->setEnabled(false);
        tmpfile = _tmpfile;
        usrfile  = _usrpath;
        islocalfile = true;
        open_file(tmpfile);
        this->setWindowTitle(usrfile);
    } else {
        ui->actionUndo->setVisible(false);
        ui->actionRedo->setVisible(false);
        ui->actionReplace->setVisible(false);
        ui->actionLoad->setVisible(false);
        ui->actionOpen->setVisible(false);

        ui->btn_undo->setEnabled(false);
        ui->btn_redo->setEnabled(false);
        ui->btn_replace->setEnabled(false);
        ui->btn_load->setEnabled(false);
        ui->btn_open->setEnabled(false);

        tmpfile = _tmpfile;
        this->text_browser->setReadOnly(true);
        if(_tmpfile.contains("info")) {
            ui->show_text_widget->hide();
            ui->show_edit_btn->hide();
            ui->tableWidget->show();
            ui->show_info_btn->show();
            open_infoFile(_tmpfile);
            this->setWindowTitle(tr("cnextInfo"));
        } else if(_tmpfile.contains("key")) {
            ui->show_text_widget->show();
            ui->show_edit_btn->show();
            ui->tableWidget->hide();
            ui->show_info_btn->hide();
            open_file(_tmpfile);
            this->setWindowTitle(tr("shortCut"));
        }
    }
    closeflag = true;
}

void MyEditor::redoStatus(bool s)
{
    actionRedo->setEnabled(s);
}

void MyEditor::undoStatus(bool s)
{
    actionUndo->setEnabled(s);
}

FileEditor::~FileEditor()
{
    delete ui;
}

void FileEditor::undo_status(bool s)
{
    ui->actionUndo->setEnabled(s);
    ui->btn_undo->setEnabled(s);
}

void FileEditor::redo_status(bool s)
{
    ui->actionRedo->setEnabled(s);
    ui->btn_redo->setEnabled(s);
}


void FileEditor::open_file(QString path)
{
    QTextCodec *code = QTextCodec::codecForName("UTF8");
    text_browser->clear();

    QFile file(path);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        printf("Cannot open file %s\n",path.toLatin1().data());
        return;
    }

    QByteArray text ;
    text = file.read(READ_MAX_LEN);
    file.close();
    QString str = code->toUnicode(text);

    text_browser->setUnblockText((str));


}

void FileEditor::open_infoFile(QString path){

//    ui->tableWidget->clear();

    for(int i=ui->tableWidget->rowCount()-1;i>0;i--) {
        for(int j=ui->tableWidget->colorCount()-1;j>0;j--) {
          delete ui->tableWidget->takeItem(i, j);
        }
    }
    for(int i=ui->tableWidget->rowCount();i>0;i--)
        ui->tableWidget->removeRow(0);

    QString lines;
    QStringList mylist;
    int type;
    int row = 0;

    QFile file(path);

    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        while( !stream.atEnd()){
            lines = stream.readLine();
            mylist = lines.split("  ");


            if(3 == mylist.size()){

                ui->tableWidget->insertRow(row);

                type = mylist.at(0).toInt();

                QTableWidgetItem *typeTips = new QTableWidgetItem();
                QTableWidgetItem *typeItem = new QTableWidgetItem();
                if(0 == type){
                    typeItem->setIcon(icon_info);
                    typeTips->setText("0");
                }else if(1 == type){
                    typeItem->setIcon(icon_warning);
                    typeTips->setText("1");
                }else if(2 == type){
                    typeItem->setIcon(icon_error);
                    typeTips->setText("2");
                }else{
                    qDebug()<<"Open encn_Info error";
                }

                QTableWidgetItem *newItem = new QTableWidgetItem();
                newItem->setText(mylist.at(2));

                QTableWidgetItem *time = new QTableWidgetItem();
                time->setText(mylist.at(1));

                newItem->setFlags(Qt::ItemIsEnabled);
                typeItem->setFlags(Qt::ItemIsEnabled);
                time->setFlags(Qt::ItemIsEnabled);
                typeTips->setFlags(Qt::ItemIsEnabled);

                ui->tableWidget->setItem(row,0,typeItem);
                ui->tableWidget->setItem(row,1,newItem);
                ui->tableWidget->setItem(row,2,time);
                ui->tableWidget->setItem(row,3,typeTips);

                row++;

            }

        }
        file.close();
    }

    init_info_display();

}

bool FileEditor::save_file()
{
    if(text_browser->toPlainText().isEmpty())
        return false;
    if(islocalfile) {

        if( usrfile.isEmpty() && tmpfile.isEmpty() ) {//when edit a empty file

            QString filepath = QFileDialog::getSaveFileName(this, tr("Save File"),
                                        "/home/ncfile/",
                                        tr("NCFile (*.ngc)"));
            if(filepath.isEmpty()) return false;
            tmpfile = filepath;
            QFile file2(filepath);
            if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
                file2.close();
                return false;
            }
            file2.write(text_browser->toPlainText().toUtf8().data());
            file2.flush();
            file2.close();
            return true;
        }

        QFile file(tmpfile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.close();
            return false;
        }
        file.write(text_browser->toPlainText().toUtf8().data());
        file.flush();
        file.close();

        QFile file2(usrfile);
        if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file2.close();
            return false;
        }
        file2.write(text_browser->toPlainText().toUtf8().data());
        file2.flush();
        file2.close();


    } else {

        QFile file(extfile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.close();
            return false;
        }
        file.write(text_browser->toPlainText().toUtf8().data());
        file.close();

    }

    return true;
}

void FileEditor::on_actionUndo_triggered()
{
    text_browser->undo();
}


void FileEditor::on_actionRedo_triggered()
{
    text_browser->redo();
}

void FileEditor::on_actionOpen_triggered()
{
    QString filepath = getfileName(tr("Open File"),_ncdir,tr("Files(*.*)"),this);
    if(filepath.isEmpty())
        return;
    if(filepath == usrfile) return;
    islocalfile = false;
    extfile = filepath;
    open_file(filepath);

}

void FileEditor::on_actionSave_triggered()
{
    QString file_temp = "";
    file_temp = QFileDialog::getSaveFileName(this,
               tr("Open File"),"/home/cnext/NC",tr("Files(*.*)"));
    if(file_temp == NULL)
        return;
    write_data_to_file(file_temp,
                       text_browser->toPlainText());
}

void FileEditor::on_actionLoad_triggered()
{

    if(!save_file()) {
        QMessageBox::warning(this, tr("WARNING"),tr("Save File Fail !!"));
        return;
    }


    if(islocalfile) {

        if( usrfile.isEmpty() && !tmpfile.isEmpty() ) {
            emit re_load(tmpfile);
        } else {
            linuxcnc * pp = static_cast<linuxcnc *>(prt);
            QString strtmp = text_browser->toPlainText();
            pp->updateNcView(strtmp);
        }

    } else {

       emit re_load(extfile);
    }
    closeflag = false;
    this->close();

}

void MyEditor::contextMenuEvent(QContextMenuEvent *)
{
    QCursor cur = this->cursor();
    static QMenu *menu = new QMenu();
    menu->clear();
    actionCut->setEnabled(this->textCursor().hasSelection());
    actionCopy->setEnabled(this->textCursor().hasSelection());
    menu->addAction(actionRedo);
    menu->addAction(actionUndo);
    menu->addAction(actionCopy);
    menu->addAction(actionCut);
    menu->addAction(actionPaste);
    menu->exec(cur.pos());
}

void FileEditor::on_actionFind_triggered()
{

    myfind->show();
    myreplace->hide();

    if(!myreplace->findtext->text().isEmpty())
        myfind->findtext->setText(myreplace->findtext->text());

    if(text_browser->textCursor().hasSelection())
        myfind->findtext->setText(text_browser->textCursor().selectedText());

}

void FileEditor::on_actionReplace_triggered()
{
    myreplace->show();
    myfind->hide();
    if(!myfind->findtext->text().isEmpty())
        myreplace->findtext->setText(myfind->findtext->text());

    if(text_browser->textCursor().hasSelection())
        myreplace->findtext->setText(text_browser->textCursor().selectedText());
}

void FileEditor::closeEvent(QCloseEvent *e)
{

    if(!tmpfile.contains("info") &&
            !tmpfile.contains("key") && closeflag) {
    int ret = QMessageBox::warning(this,tr("Warnning"),
            tr("Your changes will not save if U exit from here\n Still exist?")
                         ,tr("Ok"),tr("Cancel"));
    if(ret == Cancel)
        e->ignore();
    else if(ret == Ok)
        e->accept();
    }

    myreplace->close();
    myfind->close();

}

bool FileEditor::eventFilter(QObject *obj, QEvent *event)
{

    static bool oldflag = !text_browser->loading;
    if(oldflag != text_browser->loading) {
        if(text_browser->loading) {
            statusShow->setText(tr("now is loading file,please wait..."));
            statusShow->setStyleSheet("background-color:yellow");
        } else {
            statusShow->setText("");
            statusShow->setStyleSheet("");
        }
        oldflag = text_browser->loading;
    }

    if(event->type() == QEvent::KeyPress) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
         if(keyEvent->key() == Qt::Key_Escape) {
             this->close();
             return true;
         } else {
            return QMainWindow::eventFilter(obj,event);
         }
    } else {
        return QMainWindow::eventFilter(obj,event);
    }
    return QMainWindow::eventFilter(obj,event);
}

MyEditor::MyEditor()
{
}



void FileEditor::on_actionJump_triggered()
{
    bool ok = false;
    int line = QInputDialog::getInt(this,tr("Jump"),
                                         tr("Jump to line:"), 0,0,
                                         10000000,1, &ok);
    if(ok && line > 0) {
        QTextCursor cur = text_browser->textCursor();
        int position = text_browser->document()->findBlockByLineNumber(line-1).position();
        cur.setPosition(position,QTextCursor::MoveAnchor);
        text_browser->setTextCursor(cur);
    }
}

void FileEditor::init_connet()
{
    connect(ui->btn_open,SIGNAL(released()),this,SLOT(on_actionOpen_triggered()));
    connect(ui->btn_save,SIGNAL(released()),this,SLOT(on_actionSave_triggered()));
    connect(ui->btn_undo,SIGNAL(released()),this,SLOT(on_actionUndo_triggered()));
    connect(ui->btn_redo,SIGNAL(released()),this,SLOT(on_actionRedo_triggered()));
    connect(ui->btn_load,SIGNAL(released()),this,SLOT(on_actionLoad_triggered()));
    connect(ui->btn_find,SIGNAL(released()),this,SLOT(on_actionFind_triggered()));
    connect(ui->btn_replace,SIGNAL(released()),this,SLOT(on_actionReplace_triggered()));
    connect(ui->btn_jump,SIGNAL(released()),this,SLOT(on_actionJump_triggered()));
    connect(ui->btn_cancel,SIGNAL(released()),this,SLOT(close()));

    connect(text_browser->actionUndo,SIGNAL(triggered()),this,SLOT(on_actionUndo_triggered()));
    connect(text_browser->actionRedo,SIGNAL(triggered()),this,SLOT(on_actionRedo_triggered()));

//    QShortcut *key_f1=new QShortcut(QKeySequence(Qt::Key_F1),this);
//    connect(key_f1,SIGNAL(activated()),this,SLOT(on_actionSave_triggered()));
//    QShortcut *key_f2=new QShortcut(QKeySequence(Qt::Key_F2),this);
//    connect(key_f2,SIGNAL(activated()),this,SLOT(on_actionUndo_triggered()));
//    QShortcut *key_f3=new QShortcut(QKeySequence(Qt::Key_F3),this);
//    connect(key_f3,SIGNAL(activated()),this,SLOT(on_actionRedo_triggered()));
//    QShortcut *key_f4=new QShortcut(QKeySequence(Qt::Key_F4),this);
//    connect(key_f4,SIGNAL(activated()),this,SLOT(on_actionFind_triggered()));
//    QShortcut *key_f5=new QShortcut(QKeySequence(Qt::Key_F5),this);
//    connect(key_f5,SIGNAL(activated()),this,SLOT(on_actionReplace_triggered()));
//    QShortcut *key_f6=new QShortcut(QKeySequence(Qt::Key_F6),this);
//    connect(key_f6,SIGNAL(activated()),this,SLOT(on_actionJump_triggered()));

    ui->btn_save->setShortcut(Qt::Key_F1);
    ui->btn_undo->setShortcut(Qt::Key_F2);
    ui->btn_redo->setShortcut(Qt::Key_F3);
    ui->btn_find->setShortcut(Qt::Key_F4);
    ui->btn_replace->setShortcut(Qt::Key_F5);
    ui->btn_jump->setShortcut(Qt::Key_F6);

    icon_error =  QIcon(":/std/std_error.gif");
    icon_warning =  QIcon(":/std/std_warning.gif");
    icon_info =  QIcon(":/std/std_info.gif");
    QStringList header ;
    header.clear();
    header.append(tr("Type"));header.append(tr("Info"));
    header.append(tr("Time"));
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->btn_info->setChecked(true);
    ui->btn_err->setChecked(true);
    ui->btn_warn->setChecked(true);
    connect(ui->btn_info,SIGNAL(clicked()),this,SLOT(init_info_display()));
    connect(ui->btn_err,SIGNAL(clicked()),this,SLOT(init_info_display()));
    connect(ui->btn_warn,SIGNAL(clicked()),this,SLOT(init_info_display()));


}


void FileEditor::resizeEvent(QResizeEvent *)
{

    int w = width()-66-190;
    if(w<200)
        w = 200;
    ui->tableWidget->setColumnWidth(0,25);
    ui->tableWidget->setColumnWidth(1,w);
    ui->tableWidget->setColumnWidth(2,170);
    ui->tableWidget->setColumnWidth(3,0);

}

void FileEditor::init_info_display(){
    int num ;
    for(int i=ui->tableWidget->rowCount()-1;i>=0;i--){
        num = ui->tableWidget->item(i,3)->text().toInt();
        if(0 == num){
            if(ui->btn_info->isChecked())
                ui->tableWidget->setRowHidden(i,false);
            else
                ui->tableWidget->setRowHidden(i,true);
        }else if(1 == num){
            if(ui->btn_warn->isChecked())
                ui->tableWidget->setRowHidden(i,false);
            else
                ui->tableWidget->setRowHidden(i,true);
        }else if(2 == num){
            if(ui->btn_err->isChecked())
                ui->tableWidget->setRowHidden(i,false);
            else
                ui->tableWidget->setRowHidden(i,true);
        }
    }

}
