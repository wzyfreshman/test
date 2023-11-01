#ifndef CCD_MAGVIEWDLG_H
#define CCD_MAGVIEWDLG_H

#include <QDialog>

class CD_ImageScreen;

namespace Ui {
class CCD_MagViewDlg;
}

class CCD_MagViewDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CCD_MagViewDlg(QWidget *parent = 0);
    ~CCD_MagViewDlg();
    void setView(CD_ImageScreen *);

private:
    Ui::CCD_MagViewDlg *ui;
};

#endif // CCD_MAGVIEWDLG_H
