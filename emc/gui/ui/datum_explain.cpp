#include "datum_explain.h"
#include "ui_datum_explain.h"

datum_Explain::datum_Explain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::datum_Explain)
{
    ui->setupUi(this);
}

datum_Explain::~datum_Explain()
{
    delete ui;
}

void datum_Explain::on_pushButton_clicked()
{
    this->close();
}
