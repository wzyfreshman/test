#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QInputDialog>
#include <QPlainTextEdit>
#include <qwt_slider.h>
#include <QCheckBox>
#include <QSpinBox>
#include <QToolButton>
#include <QSlider>
#include<QVBoxLayout>
#include "actiontype.h"
#include "../usr_intf/emcstatus.hh"
#include"unistd.h"
#include<QPushButton>
class MyLabel : public QLabel
{
    Q_OBJECT

public:
    MyLabel(QString text)
    {
        axis = 0;
        setText(text);
        setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
        setMouseTracking(true);
    }
    ~MyLabel();

    void setAxis(AXIS_NAME _set) {
      axis = _set;
    }

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
    void enterEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void leaveEvent(QEvent *);
signals:
    void clicked();
    void leftClick(int);

private:
    int axis;

};


class CAutoMessageBox:public QDialog
{
    Q_OBJECT
public:
    CAutoMessageBox(QWidget*parent=0,QString title= "",QString text = "",int flag = 0);
    CAutoMessageBox(QWidget*parent=0,QStringList btnmsg=QStringList(), QString title= "",QString text = "");
    ~CAutoMessageBox(){}
    void setTText(QString );
    QLabel *labtext;
    QVBoxLayout *lay;
    QWidget *wid_btn;
    QTextEdit *text_edit;
public:
    int val;

protected:
    void resizeEvent(QResizeEvent*event);
    void changeEvent(QEvent *);
private:
    int m_width;
    int m_high;
    QPushButton *pBtnYes,*pBtnNo;
private slots:
    void btn_yes_slot(){
        val = 1;
        if("尝试重连"==pBtnYes->text()){

            system("halcmd setp ethercat.try_connect 0");
            usleep(200*1000);
            system("halcmd setp ethercat.try_connect 1");
        }
        accept();
    }
    void btn_no_slot(){
        val = 0;
        reject();
    }
    void clickindex(int i){
        val = i+1;
        accept();
    }
};


class MySlider : public QwtSlider
{
    Q_OBJECT
public:
    MySlider(CNC::SliderType ty,QWidget * parent = 0);
    ~MySlider();

    void setMaximum(int);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void timerEvent(QTimerEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void drawHandle( QPainter *, const QRect &, int pos ) const;

signals:
    void value_change();
public slots:
    void setNum(double);
private:

    QLabel shownum;
    double curvalue;

    CNC::SliderType type;

    QPoint recordPos;
    bool doing;
};

class PassModify : public QDialog
{
    Q_OBJECT
public:
    PassModify(const QString str,QDialog *parent = 0);
public slots:
    int codeModify();

private:
    QString codeOld,codeNew,key;
    QLineEdit *oldCode,*newCode,*finalCode;
    QPushButton *btnEnsure,*btnCancel;
    QWidget *widgetold,*widgetnew,*widgetagn,*widgetbtn;
    QLabel *oldshow,*newshow,*newshowagn;
};

class GETPassWord : public QDialog
{
    Q_OBJECT
public:
    GETPassWord(const QString str = 0,QDialog *parent=0);

    bool passok;

public slots:
    int ensurePassWord();
    int modifycode();

private:
    QString password,key;
    QLineEdit *codeinput;
    QPushButton *btnModify,*btnCancel,*btnEnsure;
    QLabel *codeText;
    QWidget *widgetinput,*widgetbtn;
    PassModify *passmodify;

};

class PassWord : public QInputDialog
{
    Q_OBJECT
public:
    enum CodeType{
        OPERAT = 0,
        DEBUG,
        DEVELOP
    };

    PassWord(QInputDialog *parent = 0);
    static bool getCodecnc(QWidget *parent,int type);
    static int getCode(QWidget *parent,int type, QString str1);
    static bool getPassWord(QWidget *parent,QString str);
    static QString getPassWord(QWidget *parent, const QString &title, const QString &label,
                                  const QString &text);
    static void initKey();

private:
    static QString OperCode;
    static QString DebugCode;
    static QString DeveCode;


};

class myFindWdiget : public QDialog
{
    Q_OBJECT
public:
    myFindWdiget(QDialog *parent = 0,QPlainTextEdit *plaintext = 0);

    QLineEdit *findtext;
public slots:
    void findStr();
    void backwardfunc(bool);
    void wholewordfunc(bool);
    void casesensitivefunc(bool);



private:
    QWidget *findwidget,*allwidget,*optionwidget;
    QPushButton *findnext;

    QPlainTextEdit *textedit;
    QCheckBox *wholeword,*casesensitive,*backward;

    QTextDocument::FindFlags findflag;

};

class myReplaceWdiget : public QDialog
{
    Q_OBJECT
public:
    myReplaceWdiget(QDialog *parent = 0,QPlainTextEdit *plaintext = 0);

    QLineEdit *replacetext,*findtext;

public slots:
    void replaceStr();
    void replaceAll();
    void jumpNext();
    void backwardfunc(bool);
    void wholewordfunc(bool);
    void casesensitivefunc(bool);



private:
    QWidget *replacewidget,*allwidget,*findwidget,*optionwidget;
    QPushButton *replacenext,*jumpnext,*replaceall;

    QPlainTextEdit *textedit;

    QCheckBox *wholeword,*casesensitive,*backward;

    QTextDocument::FindFlags findflag;
};

class myLock : public QObject
{
    Q_OBJECT
public :
    myLock(QObject *parent = 0);
    ~myLock();
    bool isLock();
    void setLock(int);

public slots:
    void timeout();

private:
    int locksec;
    QTimer *time;
};

class SetFeed : public QDialog
{
    Q_OBJECT
public:
    SetFeed(int def,int fast,QDialog *parent = 0 );
    int defvel,fastervel;

public slots:
    void ensureSetFeed();

private:

    QSpinBox *defaultfeed,*fasterfeed;
    QLabel *showdefault,*showfaster;
    QWidget *defaultset,*fasterset,*btnset;
    QPushButton *btn_ensure,*btn_cancel;

};

class MyInput : public QDialog {
    Q_OBJECT
public:
    MyInput(int type,QVariant def,QVariant min,QVariant max,QString title,QString label,QDialog *parent = 0 );
    QVariant val;
    QVariant defaultval;
    int inputtype;


public slots:
    void ensureSetVal();

private:

//    QSpinBox *intspinbox;
    QDoubleSpinBox *doublespinbox;
    QLabel *showranger,*showtips;
    QWidget *defaultset,*btnset;
    QPushButton *btn_ensure,*btn_cancel;
};

class ColorCombox : public QToolButton{
    Q_OBJECT

public:
    ColorCombox(QWidget *parent = 0);
    ~ColorCombox();



public slots:
    void setButtonIcon(QColor color);
    void OnShowColorBoard();         // 颜色板

private:

    QIcon createColorToolButtonIcon(QColor color);



signals:
    void sigColorChanged(QColor color);

private slots:

};


#endif // MYWIDGET_H
