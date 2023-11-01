#include "ccd_magviewdlg.h"
#include "ui_ccd_magviewdlg.h"

CCD_MagViewDlg::CCD_MagViewDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCD_MagViewDlg)
{
    ui->setupUi(this);
}

CCD_MagViewDlg::~CCD_MagViewDlg()
{
    delete ui;
}

void CCD_MagViewDlg::setView(CD_ImageScreen *w) {

    ui->frame->layout()->addWidget((QWidget *)w);

}
