#ifndef PICKZ_H
#define PICKZ_H

#include <QDialog>

namespace Ui {
class PickZ;
}

class PickZ : public QDialog
{
    Q_OBJECT
    
public:
    explicit PickZ(QWidget *parent = 0);
    ~PickZ();

public slots:
    void ensure();

private:
    Ui::PickZ *ui;
    int z1active,z2active,z3active,z4active;

protected:
    void showEvent(QShowEvent *);

};

#endif // PICKZ_H
