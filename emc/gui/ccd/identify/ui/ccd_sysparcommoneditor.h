#ifndef CCD_SYSPARCOMMONEDITOR_H
#define CCD_SYSPARCOMMONEDITOR_H

#include <QDialog>


class CCD_SysParameter;
class CCD_CncManager;

namespace Ui {
class CCD_SysParCommonEditor;
}

class CCD_SysParCommonEditor : public QDialog
{
    Q_OBJECT

public:
    explicit CCD_SysParCommonEditor(QWidget *parent = 0);
    ~CCD_SysParCommonEditor();
    void init(CCD_SysParameter *p,CCD_CncManager *m);

public slots:
    void slotSure();
    void slotCancel();
    void slotSetDelayTime(double);
    void slotSetDistanceMerge(double);
    void slotSetToolSize(double);
    void slotSetCircleDistanceToler(double);
    void slotSetCompensateSearchLen1(int);
    void slotSetCompensateSearchLen2(int);
    void slotSetCompensateSearchLen3(double);
    void slotSetCompensateToler(double);
    void slotSetCompensateAngleToler(double);

private slots:
    void on_Btn_TwoP_clicked(bool checked);
    void on_Btn_ThreeP_clicked(bool checked);
    void on_Btn_FourP_clicked(bool checked);
    void on_Btn_LeftUp_clicked(bool checked);
    void on_Btn_Center_clicked(bool checked);
    void on_SB_TwoPointsW_valueChanged(double arg1);
    void on_SB_TwoPointsH_valueChanged(double arg1);

    void on_LinE_ComputeLen_valueChanged(double arg1);
    void on_LinE_ComputeDIff_valueChanged(double arg1);
    void on_Btn_ComputeRotate_clicked();
    void on_LinE_RotateOffset_valueChanged(double arg1);

    void on_Array_With_valueChanged(int arg1);
    void on_Array_Height_valueChanged(int arg1);
    void on_Array_Distance_Hor_valueChanged(double arg1);
    void on_Array_Distance_Ver_valueChanged(double arg1);
    void on_Array_Enable_clicked(bool checked);
    void on_SB_SampleNum_valueChanged(int arg1);
    void on_Ckb_Offline_clicked(bool checked);

    void on_Btn_OneP_clicked(bool checked);

private:
    void closeEvent(QCloseEvent *);
    void reject();
    void keyPressEvent(QKeyEvent *e);

    CCD_SysParameter *syspar;
    Ui::CCD_SysParCommonEditor *ui;
    CCD_CncManager *manager;

    double compute_len;
    double compute_diff;

public:
    bool offline_mode;


};

#endif // CCD_SYSPARCOMMONEDITOR_H
