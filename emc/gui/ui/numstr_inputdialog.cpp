#include "numstr_inputdialog.h"
#include "ui_numstr_inputdialog.h"
#include "math/hexvalidator.h"


QString NumStr_InputDialog::inputtext = "";

NumStr_InputDialog::NumStr_InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumStr_InputDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new Base36Validator());
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(setStrings(QString)));
    inputtext = "";
}

NumStr_InputDialog::~NumStr_InputDialog()
{
    delete ui;
}

void NumStr_InputDialog::setStrings(QString s) {
    inputtext = s;
}
