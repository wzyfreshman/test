#include "mywidget.h"
#include "extern.h"

#include <QSlider>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QPainter>
#include <QtDebug>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include <QPainter>
#include <QColorDialog>

QString PassWord::OperCode;
QString PassWord::DebugCode;
QString PassWord::DeveCode;

void MyLabel::enterEvent(QMouseEvent *ev)
{

    setFrameStyle(QFrame::Raised);
    setFrameShadow(QFrame::Plain);
    QLabel::enterEvent(ev);
}

void MyLabel::mouseMoveEvent(QMouseEvent *)
{
    //this->setStyleSheet("background-color:red");
    setFrameStyle(QFrame::Raised);
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Plain);

}

void MyLabel::leaveEvent(QEvent *ev)
{

    setFrameStyle(QFrame::Sunken);
    setFrameShadow(QFrame::Plain);
    //setStyleSheet(0);
    QLabel::leaveEvent(ev);
    update();
}

void MyLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    emit clicked();

    if(ev->button() & Qt::LeftButton){
            emit leftClick(axis);
    } else if(ev->button() & Qt::RightButton){
    }

    QLabel::mouseReleaseEvent(ev);
}

MyLabel::~MyLabel()
{
}

MySlider::MySlider(CNC::SliderType ty, QWidget *parent) :
    QwtSlider(parent),shownum(parent)
{

    type = ty;
    doing  =false;
    shownum.hide();
    shownum.setWindowFlags(Qt::ToolTip|Qt::FramelessWindowHint);
    QFont font;
    font.setPixelSize(5);
    setFont(font);
    setOrientation( Qt::Horizontal );
    setScalePosition( QwtSlider::TrailingScale );
    setTrough( true );
    setGroove( true );
    setHandleSize( QSize( 40, 50 ) );
    setScale( 0, 100.0 );
    setTotalSteps( 100 );
    setSingleSteps( 1 );
    setPageSteps( 0 );
    setWrapping( false);
    setBorderWidth(1);
    setUpdateInterval(400);
    setSpacing(-5);
    QColor sc(Qt::yellow);
    shownum.setPalette(sc);

    connect(this,SIGNAL(valueChanged(double)), SLOT(setNum(double )));

}

MySlider::~MySlider()
{

}int bInit = 0;
void MySlider::drawHandle( QPainter *pa, const QRect &rc, int pos ) const
{
    pa->setBrush(Qt::darkBlue);            // 设置画刷
    pa->setPen(Qt::darkBlue);
    QRect myrc;
    myrc.setWidth(rc.y());
    myrc.setHeight(rc.x());
    if(bInit == 0)
    {
        myrc.setX(-30);
        bInit = 1;
    }
    else
        myrc.setX(rc.x());

    myrc.setY(rc.y());
    pa->drawRect(myrc);
    QwtSlider::drawHandle(pa,rc,pos);
}
void MySlider::mousePressEvent(QMouseEvent *ev) {

    doing = true;
    int step;

    if( ev->pos().x() > handleRect().x() + handleRect().width()) {

        if(type == CNC::SliderFeedOverride)
            step = _prefer->readEntry("DEFAULT/feedsliderstep","5",true).toInt();
        else
            step = _prefer->readEntry("DEFAULT/spindlesliderstep","5",true).toInt();
        qDebug()<<"shieeiee"<<value()+step<<" "<<step;
        setValue(value()+step);

    } else if(ev->pos().x() < handleRect().x()) {

        if(type == CNC::SliderFeedOverride)
            step = _prefer->readEntry("DEFAULT/feedsliderstep","5",true).toInt();
        else
            step = _prefer->readEntry("DEFAULT/spindlesliderstep","5",true).toInt();
        setValue(value()-step);

    }

    QPoint point(handleRect().x(),handleRect().y()-20);
    shownum.move(mapToGlobal(point));

    shownum.setNum(curvalue);
    shownum.adjustSize();
    shownum.show();

    QwtSlider::mousePressEvent(ev);

}

void MySlider::mouseMoveEvent(QMouseEvent *ev) {

    QPoint point(handleRect().x(),handleRect().y()-10);
    shownum.move(mapToGlobal(point));
    QwtSlider::mouseMoveEvent(ev);
}

void MySlider::mouseReleaseEvent(QMouseEvent *ev) {


    doing = false;
    //static double odv = -1;
    //if(odv != curvalue) {
        emit value_change();
    //}
    //odv = curvalue;

    shownum.move(mapToGlobal(ev->pos()));
    shownum.setNum(curvalue);
    shownum.adjustSize();
    shownum.hide();

    QwtSlider::mouseReleaseEvent(ev);
}

void MySlider::wheelEvent(QWheelEvent *e) {

    e->ignore();

}

void MySlider::keyReleaseEvent(QKeyEvent *key) {

    key->ignore();
    return;

}

void MySlider::timerEvent(QTimerEvent *ev) {
   //QwtSlider::timerEvent(ev);
    ev->ignore();
}

void MySlider::setMaximum(int set)
{

    setScale( 0, set );
    setTotalSteps( set );
}

void MySlider::setNum(double num) {

    curvalue = num;
    shownum.setNum(num);
    shownum.adjustSize();

}

PassModify::PassModify(const QString str, QDialog *parent):
    QDialog(parent)
{
    this->setWindowTitle(tr("Modify Code"));
    key = str;
    oldCode = new QLineEdit();
    newCode = new QLineEdit();
    finalCode = new QLineEdit();
    widgetold = new QWidget();
    widgetnew = new QWidget();
    widgetbtn = new QWidget();
    widgetagn = new QWidget();
    btnEnsure = new QPushButton(tr("Ensure"));
    btnCancel = new QPushButton(tr("Cancel"));
    oldshow = new QLabel(tr("old code:"));
    newshow = new QLabel(tr("new code:"));
    newshowagn = new QLabel(tr("new code again:"));

    oldCode->setEchoMode(QLineEdit::Password);
    newCode->setEchoMode(QLineEdit::Password);
    finalCode->setEchoMode(QLineEdit::Password);
    btnEnsure->setShortcut(QKeySequence("Enter"));
    QHBoxLayout *old_lay = new QHBoxLayout(widgetold);
    old_lay->addWidget(oldshow);
    old_lay->addWidget(oldCode);
    old_lay->setContentsMargins(0,0,0,0);
    QHBoxLayout *new_lay = new QHBoxLayout(widgetnew);
    new_lay->addWidget(newshow);
    new_lay->addWidget(finalCode);
    new_lay->setContentsMargins(0,0,0,0);
    QHBoxLayout *new_lay2 = new QHBoxLayout(widgetagn);
    new_lay2->addWidget(newshowagn);
    new_lay2->addWidget(newCode);
    new_lay2->setContentsMargins(0,0,0,0);
    QHBoxLayout *btn_lay = new QHBoxLayout(this->widgetbtn);
    btn_lay->addWidget(btnEnsure);
    btn_lay->addWidget(btnCancel);
    btn_lay->setContentsMargins(0,0,0,0);
    QVBoxLayout *total = new QVBoxLayout(this);
    total->addWidget(widgetold);
    total->addWidget(widgetnew);
    total->addWidget(widgetagn);
    total->addWidget(widgetbtn);
    total->setContentsMargins(0,0,0,0);
    codeOld = _prefer->read("DEFAULT",key,"cnext");

    connect(btnEnsure,SIGNAL(released()),this,SLOT(codeModify()));
    connect(btnCancel,SIGNAL(released()),this,SLOT(close()));
}

int PassModify::codeModify()
{
    if(oldCode->text().isEmpty() ||
            newCode->text().isEmpty() ||
            finalCode->text().isEmpty()) {
        QMessageBox::warning(this,tr("warning"),
        QString(tr("Input Code Cannot be NULL")),tr("Ok"));
        return -1;
    }
    if(0 != oldCode->text().compare(codeOld)) {
        QMessageBox::warning(this,tr("warning"),
        QString(tr("old code is not correct")),tr("Ok"));
        return -1;
    }
    if(0 != newCode->text().compare(finalCode->text())) {
        QMessageBox::warning(this,tr("warning"),
        QString(tr("two new code art not compative")),tr("Ok"));
        return -1;
    }

    QString str = QString("DEFAULT/%1").arg(key);
    _prefer->writeEntry(str,finalCode->text());
    QMessageBox::information(this,tr("tips"),
    QString(tr("modify code is ok")),tr("Ok"));
    codeOld = _prefer->read("DEFAULT",key);
    return 0;
}



GETPassWord::GETPassWord(const QString str,QDialog *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("Code Input"));
    key = str;
    passok = false;
    password = _prefer->read("DEFAULT",key);
    codeinput = new QLineEdit();
    btnModify = new QPushButton(tr("Modify"));
    btnCancel = new QPushButton(tr("Cancel"));
    btnEnsure = new QPushButton(tr("Ensure"));
    codeText = new QLabel(tr("Input PassWord:"));
    widgetinput = new QWidget();
    widgetbtn = new QWidget();
    passmodify = NULL;
    QHBoxLayout *text_lay = new QHBoxLayout(widgetinput);
    text_lay->addWidget(codeText);
    text_lay->addWidget(codeinput);
    text_lay->setContentsMargins(0,0,0,0);
    QHBoxLayout *btn_lay = new QHBoxLayout(widgetbtn);
    btn_lay->addWidget(btnModify);
    btn_lay->addWidget(btnCancel);
    btn_lay->addWidget(btnEnsure);
    btn_lay->setContentsMargins(0,0,0,0);
    QVBoxLayout *total = new QVBoxLayout(this);
    total->addWidget(widgetinput);
    total->addWidget(widgetbtn);
    total->setContentsMargins(0,0,0,0);
    btnEnsure->setShortcut(QKeySequence("Enter"));
    btnModify->setFocusPolicy(Qt::NoFocus);
    btnCancel->setFocusPolicy(Qt::NoFocus);
    btnEnsure->setFocus();
    codeinput->setEchoMode(QLineEdit::Password);
    connect(btnEnsure,SIGNAL(released()),this,SLOT(ensurePassWord()));
    connect(btnCancel,SIGNAL(released()),this,SLOT(close()));
    connect(btnModify,SIGNAL(released()),this,SLOT(modifycode()));
}

int GETPassWord::ensurePassWord()
{
    if(codeText->text().isEmpty()) {
        passok = false;
        return -1;
    }
    password = _prefer->read("DEFAULT",key);
    if(0 == codeinput->text().compare(password)) {
        //QMessageBox::information(this,tr("tips"),
        //QString(tr("Input code is ok")),tr("Ok"));
        passok = true;
        this->close();
        return 0;
    } else {
        QMessageBox::warning(this,tr("warnning"),
        QString(tr("Input code error")),tr("Ok"));
        passok = false;
        return -1;
    }
}

int GETPassWord::modifycode()
{
    if(passmodify == NULL) {
        passmodify = new PassModify(QString("syslock"));
    }
    passmodify->exec();
    return 0;
}

PassWord::PassWord(QInputDialog *parent):
    QInputDialog(parent)
{

}

bool PassWord::getPassWord(QWidget *parent, QString str)
{
    bool ok;

    GETPassWord *pass = new GETPassWord(str,static_cast<QDialog *>(parent));
    pass->exec();
    ok = pass->passok;
    delete pass;
    return ok;

}

QString PassWord::getPassWord(QWidget * parent,
                                    const QString &title,
                                     const QString &label,
                                     const QString &text){
    bool ok;
    QString code = QInputDialog::getText(parent, title,
                                         label, QLineEdit::Password,
                                         text, &ok);
    if(ok && !code.isEmpty()) {
        return code;
    }
    return NULL;
}

void PassWord::initKey(){
    OperCode = _prefer->read("DEFAULT","operator","0");
    if(OperCode == "0")
        OperCode = "cnext";
    DebugCode = _prefer->read("DEFAULT","debug","0");
    if(DebugCode == "0")
        DebugCode = "cnext";
    DeveCode = _prefer->read("DEFAULT","develop","0");
    if(DeveCode == "0")
        DeveCode = "cnext2018";
}

bool PassWord::getCodecnc(QWidget *parent,int type)
{
    QString str = "";
    if(0 == type) str = tr("Input Operation Password");
    else if(1 == type) str = tr("Input Debug Password");
    else if(2 == type) str = tr("Input Developer Password");
    else if(-1 == type) str = tr("输入密码");

    int rev = 0;
    while(-1 == (rev=getCode(parent,type,str))) {
        str = tr("Input Password Again");
    }
    if(0 == rev)
        return false;
    if(1 == rev)
        return true;
    return false;
}

int PassWord::getCode(QWidget *parent, int type, QString str1)
{
    int rev = 0;
    bool ok = false;

    QString code = QInputDialog::getText(parent,tr("password"),
                                             str1, QLineEdit::Password,
                                             "", &ok);
    if(ok && !code.isEmpty()) {
        if(OPERAT == type) {
            if(0 == code.compare(PassWord::OperCode,Qt::CaseInsensitive))
                rev = 1;
            else
                rev = -1;
        }else if(DEBUG == type) {
            if(0 == code.compare(PassWord::DebugCode,Qt::CaseInsensitive))
                rev = 1;
            else
                rev = -1;
        }else if(DEVELOP == type) {
            if(0 == code.compare(PassWord::DeveCode,Qt::CaseInsensitive))
                rev = 1;
            else
                rev = -1;
        }else if(-1 == type) {
            if(0 == code.compare("4321",Qt::CaseInsensitive))
                rev = 1;
            else
                rev = -1;
        }  else {
            rev = -1;
        }
    } else
        rev = 0;

    return rev;
}


myFindWdiget::myFindWdiget(QDialog *parent, QPlainTextEdit *plaintext):
    QDialog(parent)
{

    this->setWindowTitle(tr("Find"));
    findflag = 0;
    textedit = plaintext;

    findwidget = new QWidget();
    findtext = new QLineEdit();
    findnext = new QPushButton(tr("findNext"));
    findnext->setShortcut(QKeySequence(tr("Return")));
    QHBoxLayout *find_lay = new QHBoxLayout(this->findwidget);
    find_lay->addWidget(findtext);
    find_lay->addWidget(findnext);
    find_lay->setContentsMargins(0,0,0,0);

    optionwidget = new QWidget();
    wholeword = new QCheckBox(tr("whole word"));
    casesensitive = new QCheckBox(tr("casesensitive"));
    backward = new QCheckBox(tr("backward"));
    QHBoxLayout *option_lay = new QHBoxLayout(this->optionwidget);
    option_lay->addWidget(backward);
    option_lay->addWidget(wholeword);
    option_lay->addWidget(casesensitive);
    option_lay->setContentsMargins(0,0,0,0);

    allwidget = new QWidget();
    QVBoxLayout *wholewidget_lay = new QVBoxLayout(this->allwidget);
    wholewidget_lay->addWidget(this->findwidget);
    wholewidget_lay->addWidget(this->optionwidget);
    wholewidget_lay->setContentsMargins(0,0,0,0);

    this->setLayout(wholewidget_lay);
    this->setModal(true);
    this->setWindowModality(Qt::WindowModal);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    connect(findnext,SIGNAL(released()),this,SLOT(findStr()));
    connect(backward,SIGNAL(toggled(bool)),this,SLOT(backwardfunc(bool)));
    connect(wholeword,SIGNAL(toggled(bool)),this,SLOT(wholewordfunc(bool)));
    connect(casesensitive,SIGNAL(toggled(bool)),this,SLOT(casesensitivefunc(bool)));
}

void myFindWdiget::backwardfunc(bool val) {
    if(val) {
        findflag = (findflag | QTextDocument::FindBackward);
    } else
        findflag = (findflag & ~(QTextDocument::FindBackward));
}

void myFindWdiget::wholewordfunc(bool val) {
    if(val) {
        findflag = (findflag | QTextDocument::FindWholeWords);
    } else
        findflag = (findflag & ~(QTextDocument::FindWholeWords));
}

void myFindWdiget::casesensitivefunc(bool val) {
    if(val) {
        findflag = (findflag | QTextDocument::FindCaseSensitively);
    } else
        findflag = (findflag & ~(QTextDocument::FindCaseSensitively));
}

void myFindWdiget::findStr()
{
    QString temp = findtext->text();
    if(!temp.isEmpty())
    if(!textedit->find(temp,findflag)) {
        QMessageBox::warning(this,tr("warning"),
                             QString(tr("Can not find %1")).arg(temp),tr("Ok"));
        QTextCursor cursor = textedit->textCursor();
        cursor.setPosition(0);
        textedit->setTextCursor(cursor);
    }
}


myReplaceWdiget::myReplaceWdiget(QDialog *parent, QPlainTextEdit *plaintext):
    QDialog(parent)
{

    this->setWindowTitle(tr("Replace"));
    textedit = plaintext;

    replacewidget = new QWidget();
    replacetext = new QLineEdit();
    replacenext = new QPushButton(tr("replaceN"));
    replaceall = new QPushButton(tr("replaceall"));

    QHBoxLayout *replace_lay = new QHBoxLayout(this->replacewidget);
    replace_lay->addWidget(replacetext);
    replace_lay->addWidget(replacenext);
    replace_lay->setContentsMargins(0,0,0,0);


    findwidget = new QWidget();
    findtext = new QLineEdit();
    jumpnext = new QPushButton(tr("jumpNext"));
    jumpnext->setShortcut(QKeySequence(tr("Return")));
    QHBoxLayout *find_lay = new QHBoxLayout(this->findwidget);
    find_lay->addWidget(findtext);
    find_lay->addWidget(jumpnext);
    find_lay->setContentsMargins(0,0,0,0);

    optionwidget = new QWidget();
    wholeword = new QCheckBox(tr("whole word"));
    casesensitive = new QCheckBox(tr("casesensitive"));
    backward = new QCheckBox(tr("backward"));
    QHBoxLayout *option_lay = new QHBoxLayout(this->optionwidget);
    option_lay->addWidget(backward);
    option_lay->addWidget(wholeword);
    option_lay->addWidget(casesensitive);
    option_lay->addWidget(replaceall);
    option_lay->setContentsMargins(0,0,0,0);


    allwidget = new QWidget();
    QVBoxLayout *wholewidget_lay = new QVBoxLayout(this->allwidget);
    wholewidget_lay->addWidget(this->findwidget);
    wholewidget_lay->addWidget(this->replacewidget);
    wholewidget_lay->addWidget(this->optionwidget);
    wholewidget_lay->setContentsMargins(0,0,0,0);

    this->setLayout(wholewidget_lay);
    this->setModal(true);
    this->setWindowModality(Qt::WindowModal);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    connect(replacenext,SIGNAL(released()),this,SLOT(replaceStr()));
    connect(replaceall,SIGNAL(released()),this,SLOT(replaceAll()));
    connect(jumpnext,SIGNAL(released()),this,SLOT(jumpNext()));
    connect(backward,SIGNAL(toggled(bool)),this,SLOT(backwardfunc(bool)));
    connect(wholeword,SIGNAL(toggled(bool)),this,SLOT(wholewordfunc(bool)));
    connect(casesensitive,SIGNAL(toggled(bool)),this,SLOT(casesensitivefunc(bool)));
}

void myReplaceWdiget::backwardfunc(bool val) {
    if(val) {
        findflag = (findflag | QTextDocument::FindBackward);
    } else
        findflag = (findflag & ~(QTextDocument::FindBackward));
}

void myReplaceWdiget::wholewordfunc(bool val) {
    if(val) {
        findflag = (findflag | QTextDocument::FindWholeWords);
    } else
        findflag = (findflag & ~(QTextDocument::FindWholeWords));
}

void myReplaceWdiget::casesensitivefunc(bool val) {
    if(val) {
        findflag = (findflag | QTextDocument::FindCaseSensitively);
    } else
        findflag = (findflag & ~(QTextDocument::FindCaseSensitively));
}

void myReplaceWdiget::jumpNext()
{
    QString temp = findtext->text();
    if(!temp.isEmpty())
    if(!textedit->find(temp,findflag)) {
        QMessageBox::warning(this,tr("warning"),
                             QString(tr("Can not find %1")).arg(temp),tr("Ok"));
        QTextCursor cursor = textedit->textCursor();
        cursor.setPosition(0);
        textedit->setTextCursor(cursor);
    }
}

void myReplaceWdiget::replaceAll()
{
    if(findtext->text().isEmpty())
        return;
    QTextCursor cursor = textedit->textCursor();
    cursor.setPosition(0);
    textedit->setTextCursor(cursor);
    int count = 0;
    while(textedit->find(findtext->text(),findflag)) {
        if(!replacetext->text().isEmpty()) {
        textedit->textCursor().removeSelectedText();
        textedit->textCursor().insertText(replacetext->text());
        count++;
        }
    }

    cursor = textedit->textCursor();
    cursor.setPosition(0);
    textedit->setTextCursor(cursor);

    if(0 == count) {
        QMessageBox::warning(this,tr("warning"),
                             QString(tr("Can not find %1"))
                             .arg(findtext->text()),tr("Ok"));
    } else {
        QMessageBox::information(this,tr("tips"),QString(tr("Replace from %1 to %2 total %3 times"))
                                 .arg(findtext->text()).arg(replacetext->text()).arg(count));
    }

}

void myReplaceWdiget::replaceStr()
{
    QTextCursor textcursor = textedit->textCursor();
    if(0 == textcursor.selectedText().compare(findtext->text()) ||
            0 == textcursor.selectedText().compare(findtext->text().toUpper())){

        if(!replacetext->text().isEmpty()) {
        textedit->textCursor().removeSelectedText();
        textedit->textCursor().insertText(replacetext->text());
        }
    }
    if(!textedit->find(findtext->text(),findflag)) {
        QMessageBox::warning(this,tr("warning"),
                             QString(tr("Can not find %1"))
                             .arg(findtext->text()),tr("Ok"));
        QTextCursor cursor = textedit->textCursor();
        cursor.setPosition(0);
        textedit->setTextCursor(cursor);
        return;
    }
}

myLock::myLock(QObject *parent) :
    QObject(parent)
{
    locksec = 0;
    time = new QTimer();
    connect(time,SIGNAL(timeout()),this,SLOT(timeout()));
}

void myLock::timeout()
{
    locksec = 0;
}

void myLock::setLock(int val)
{
    locksec = val;
    time->start(locksec);
}

bool myLock::isLock()
{
    if(0 == locksec)
        return false;
    else
        return true;
}

myLock::~myLock()
{
    delete time;
}

SetFeed::SetFeed (int def,int fast,QDialog *parent)
{
    defvel = def;
    fastervel = fast;
    this->setWindowTitle(tr("SetFeed"));

    defaultfeed = new QSpinBox(parent);
    defaultfeed->setMaximum(100000);
    defaultfeed->setButtonSymbols(QAbstractSpinBox::NoButtons);
    showdefault = new QLabel(tr("Default Feed:"));
    showdefault->setMaximumWidth(90);
    defaultset = new QWidget(parent);
    QHBoxLayout *defaultlay = new QHBoxLayout(defaultset);
    defaultlay->addWidget(showdefault);
    defaultlay->addWidget(defaultfeed);
    defaultlay->setContentsMargins(0,0,0,0);

    fasterfeed = new QSpinBox(parent);
    fasterfeed->setMaximum(100000);
    fasterfeed->setButtonSymbols(QAbstractSpinBox::NoButtons);
    showfaster = new QLabel(tr("High Feed:"));
    showfaster->setMaximumWidth(90);
    fasterset = new QWidget(parent);
    QHBoxLayout *fastlay = new QHBoxLayout(fasterset);
    fastlay->addWidget(showfaster);
    fastlay->addWidget(fasterfeed);
    fastlay->setContentsMargins(0,0,0,0);

    btn_ensure = new QPushButton(tr("Ensure"));
    btn_cancel = new QPushButton(tr("Cancel"));
    btnset = new QWidget(parent);
    QHBoxLayout *btnlay = new QHBoxLayout(btnset);
    btnlay->addWidget(btn_ensure);
    btnlay->addWidget(btn_cancel);
    btnlay->setContentsMargins(0,0,0,0);

    QVBoxLayout *totallay = new QVBoxLayout(this);
    totallay->addWidget(defaultset);
    totallay->addWidget(fasterset);
    totallay->addWidget(btnset);
    totallay->setContentsMargins(5,5,5,5);

    defaultfeed->setValue(defvel);
    fasterfeed->setValue(fastervel);
    btn_cancel->setFocusPolicy(Qt::NoFocus);
    defaultfeed->setFocus();

    connect(btn_ensure,SIGNAL(clicked()),this,SLOT(ensureSetFeed()));
    connect(btn_cancel,SIGNAL(clicked()),this,SLOT(close()));
}


void SetFeed::ensureSetFeed()
{
    if(defaultfeed->value() > fasterfeed->value()) {
        QMessageBox::warning(this,tr("warning"),
        QString(tr("defaultfeed higher than fasterfeed")),tr("Ok"));
        return;
    }
    defvel = defaultfeed->value();
    fastervel = fasterfeed->value();
    this->close();
}

MyInput::MyInput(int type, QVariant def, QVariant min, QVariant max, QString title, QString label, QDialog *parent) {
    QVariant maxval,minval;
    defaultval = def;
    maxval = QVariant(max);
    minval = QVariant(min);
    inputtype = type;
    this->setWindowTitle(title);
    showranger = new QLabel(0);

    doublespinbox = new QDoubleSpinBox(parent);
    if(0 == type) {
        val = min.toInt() - 1;
        doublespinbox->setMaximum(maxval.toInt());
        doublespinbox->setMinimum(minval.toInt());
        doublespinbox->setDecimals(0);
        doublespinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doublespinbox->setValue(def.toInt());
        showranger->setText(QString(tr("Input Range:%1 - %2")).arg(minval.toInt()).arg(maxval.toInt()));
    } else if(1 == type) {
        val = min.toDouble() - 1;
        doublespinbox->setMaximum(maxval.toDouble());
        doublespinbox->setMinimum(minval.toDouble());
        doublespinbox->setDecimals(3);
        doublespinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doublespinbox->setValue(def.toDouble());
        showranger->setText(QString(tr("Input Range:%1 ~ %2")).arg(minval.toDouble()).arg(maxval.toDouble()));
    }else if(2 == type) {
        val = min.toDouble() - 1;
        doublespinbox->setMaximum(maxval.toDouble());
        doublespinbox->setMinimum(minval.toDouble());
        doublespinbox->setDecimals(2);
        doublespinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doublespinbox->setValue(def.toDouble());
        showranger->setText(QString(tr("Input Range:%1 ~ %2")).arg(minval.toDouble()).arg(maxval.toDouble()));
    }
        showtips = new QLabel(tr("Set Value:"));
        showtips->setMaximumWidth(60);
        defaultset = new QWidget(parent);
        QHBoxLayout *defaultlay = new QHBoxLayout(defaultset);
        defaultlay->addWidget(showtips);
        defaultlay->addWidget(doublespinbox);
        defaultlay->setContentsMargins(0,0,0,0);

        btn_ensure = new QPushButton(tr("Ensure"));
        btn_cancel = new QPushButton(tr("Cancel"));
        btnset = new QWidget(parent);
        QHBoxLayout *btnlay = new QHBoxLayout(btnset);
        btnlay->addWidget(btn_ensure);
        btnlay->addWidget(btn_cancel);
        btnlay->setContentsMargins(0,0,0,0);

        QVBoxLayout *totallay = new QVBoxLayout(this);
        totallay->addWidget(showranger);
        totallay->addWidget(defaultset);
        totallay->addWidget(btnset);
        totallay->setContentsMargins(5,5,5,5);

        btn_cancel->setFocusPolicy(Qt::NoFocus);
        doublespinbox->setFocus();

        connect(btn_ensure,SIGNAL(clicked()),this,SLOT(ensureSetVal()));
        connect(btn_cancel,SIGNAL(clicked()),this,SLOT(close()));
}

void MyInput::ensureSetVal() {
    val = doublespinbox->value();
    this->close();
}


ColorCombox::ColorCombox(QWidget *parent)
    : QToolButton(parent)
{
    setPopupMode(QToolButton::InstantPopup);
    setAutoFillBackground(true);
    connect(this,SIGNAL(clicked()),this,SLOT(OnShowColorBoard()));
}

ColorCombox::~ColorCombox()
{

}


#include"qbuttongroup.h"

CAutoMessageBox::CAutoMessageBox(QWidget *parent,QString title,QString text,int flag)
{
    m_width = 300;
    m_high = 300;
    this->setWindowTitle(title);
    setFixedWidth(m_width);
    setMinimumHeight(m_high/2);
    setMaximumHeight(m_high);
    val = 0;


    labtext = new QLabel(text);
    labtext->setWordWrap(true);

    labtext->setAlignment(Qt::AlignCenter);
    labtext->setWordWrap(true);



    pBtnYes = new QPushButton(tr("确定"));
    pBtnNo = new QPushButton(tr("取消"));



    connect(pBtnYes,SIGNAL(clicked()),SLOT(btn_yes_slot()));
    connect(pBtnNo,SIGNAL(clicked()),SLOT(btn_no_slot()));

    wid_btn = new QWidget(0);
    QHBoxLayout *laybtn = new QHBoxLayout(wid_btn);
    laybtn->addWidget(pBtnYes);
    laybtn->addWidget(pBtnNo);
    laybtn->setSpacing(20);


    pBtnYes->setFixedSize(120,50);
    pBtnNo->setFixedSize(120,50);

    lay = new QVBoxLayout(this);
    lay->addWidget(labtext);
    lay->addWidget(wid_btn);

    text_edit = new QTextEdit(this);
    text_edit->hide();


    this->setStyleSheet("QWidget{background-color: rgb(222, 220, 210); font: 15pt \"Ubuntu\";} \
                        QPushButton{\
                        color: rgb(0, 0, 0);\
                        background-color: qlineargradient(spread:pad, x1:1, y1:0.778, x2:0, y2:0.205, stop:0 rgba(180, 180, 168, 255), stop:1 rgba(243, 243, 243, 255));\
                        border: 2px ;\
                        border-style: outset;\
                        border-radius: 2px;\
                        border-color: rgb(113, 113, 113);\
                        padding:2px; }\
                        QPushButton::checked{ \
                        background-color: rgb(40, 100, 155);\
                        color: rgb(255, 255, 255);\
                        border: 2px ;\
                        border-style: outset;\
                        border-radius: 2px;\
                        border-color: rgb(0, 100, 150);}\
                    ");

    pBtnYes->setShortcut(QKeySequence(Qt::Key_Return));
    pBtnYes->setAutoDefault(true);
//    if(flag==0)
//        this->exec();
}

CAutoMessageBox::CAutoMessageBox(QWidget *parent,QStringList msg,QString title,QString text)
{
    m_width = 600;
    m_high = 300;
    this->setWindowTitle(title);
    setFixedWidth(m_width);
    setMinimumHeight(m_high/2);
    setMaximumHeight(m_high);
    val = 0;


    labtext = new QLabel(text);
    labtext->setWordWrap(true);

    labtext->setAlignment(Qt::AlignCenter);
    labtext->setWordWrap(true);

    QButtonGroup *bg = new QButtonGroup();

    for(int i=0;i<msg.size();++i){
        QPushButton *btn  = new QPushButton();
        btn->setText(msg.at(i));
        bg->addButton(btn,i);
    }


    pBtnNo = new QPushButton(tr("no"));



    connect(bg,SIGNAL(buttonClicked(int)),SLOT(clickindex(int)));
    connect(pBtnNo,SIGNAL(clicked()),SLOT(btn_no_slot()));

    wid_btn = new QWidget(0);
    QHBoxLayout *laybtn = new QHBoxLayout(wid_btn);

    for(int i=0;i<bg->buttons().size();++i){
        laybtn->addWidget(bg->button(i));
        bg->button(i)->setFixedSize(140,50);
//        qDebug()<<"ssss----"<<bg->button(i)->text();
    }

    laybtn->addWidget(pBtnNo);
    laybtn->setSpacing(20);


    pBtnNo->setFixedSize(120,50);

    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->addWidget(labtext);
    lay->addWidget(wid_btn);




    this->setStyleSheet("QWidget{background-color: rgb(222, 220, 210); font: 15pt \"Ubuntu\";} \
                        QPushButton{\
                        color: rgb(0, 0, 0);\
                        background-color: qlineargradient(spread:pad, x1:1, y1:0.778, x2:0, y2:0.205, stop:0 rgba(180, 180, 168, 255), stop:1 rgba(243, 243, 243, 255));\
                        border: 2px ;\
                        border-style: outset;\
                        border-radius: 2px;\
                        border-color: rgb(113, 113, 113);\
                        padding:2px; }\
                        QPushButton::checked{ \
                        background-color: rgb(40, 100, 155);\
                        color: rgb(255, 255, 255);\
                        border: 2px ;\
                        border-style: outset;\
                        border-radius: 2px;\
                        border-color: rgb(0, 100, 150);}\
                    ");



}

void CAutoMessageBox::setTText(QString s)
{

    text_edit->show();
    text_edit->setText("");
    text_edit->append(s);
    labtext->setText("");
    setFixedWidth(300);
    setMinimumHeight(500);
    setMaximumHeight(500);
    labtext->setAlignment(Qt::AlignLeft);


    text_edit->setReadOnly(true);

    lay->removeWidget(labtext);
    lay->removeWidget(wid_btn);

    lay->addWidget(text_edit);
    lay->addWidget(wid_btn);

    lay->setStretchFactor(text_edit,8);
    lay->setStretchFactor(wid_btn,1);

    pBtnYes->setFixedSize(220,30);
    pBtnNo->setFixedSize(220,30);

    pBtnYes->setText("尝试重连");
    pBtnNo->setText("取消");
    pBtnYes->hide();
    this->setWindowTitle("从站信息");
    this->setStyleSheet("QWidget{background-color: rgb(222, 220, 210); font: 11pt \"Ubuntu\";} \
                        QPushButton{\
                        color: rgb(0, 0, 0);\
                        background-color: qlineargradient(spread:pad, x1:1, y1:0.778, x2:0, y2:0.205, stop:0 rgba(180, 180, 168, 255), stop:1 rgba(243, 243, 243, 255));\
                        border: 2px ;\
                        border-style: outset;\
                        border-radius: 2px;\
                        border-color: rgb(113, 113, 113);\
                        padding:2px; }\
                        QPushButton::checked{ \
                        background-color: rgb(40, 100, 155);\
                        color: rgb(255, 255, 255);\
                        border: 2px ;\
                        border-style: outset;\
                        border-radius: 2px;\
                        border-color: rgb(0, 100, 150);}\
                    ");

}
void CAutoMessageBox::resizeEvent(QResizeEvent *event)
{

}
void CAutoMessageBox::changeEvent(QEvent *e)
{
    if ((e->type() == QEvent::WindowStateChange) && (isMinimized() || isMaximized()))
    {
        this->showNormal();
        e->ignore();
    }
}








void ColorCombox::setButtonIcon(QColor color)
{
    setIcon(createColorToolButtonIcon(color));
}

QIcon ColorCombox::createColorToolButtonIcon( QColor color)
{
    QPixmap pixmap(16, 14);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 16, 16), color);
    return QIcon(pixmap);
}


void ColorCombox::OnShowColorBoard()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    if (!color.isValid())
        return;

    setButtonIcon(color);
    emit sigColorChanged(color);
}
