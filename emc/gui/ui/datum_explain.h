#ifndef DATUM_EXPLAIN_H
#define DATUM_EXPLAIN_H

#include <QDialog>

namespace Ui {
class datum_Explain;
}

class datum_Explain : public QDialog
{
    Q_OBJECT
    
public:
    explicit datum_Explain(QWidget *parent = 0);
    ~datum_Explain();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::datum_Explain *ui;
};

#endif // DATUM_EXPLAIN_H
