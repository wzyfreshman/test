#ifndef CCD_EDITWIDGET_H
#define CCD_EDITWIDGET_H

#include <QDialog>

#include "cd.h"
#include "ccd_marksface.h"
#include "cv.h"

class CCD_Mark;
class CD_Feature;
class CCD_MarksFace;
class CCD_CncManager;
class CD_CaptureImage;
class MatchTemplateParameter;

namespace Ui {
class CCD_MarkEditor;
}

class  CCD_MarkEditor : public QDialog ,public CCD_MarksFace
{
    Q_OBJECT
    
public:

    explicit CCD_MarkEditor(QWidget *parent = 0);
    ~CCD_MarkEditor();

    void init(CCD_Mark *m,bool one_ref = true);
    void setCncManager(CCD_CncManager *m);
    void setCapture(CD_CaptureImage *c);
    void showInRealTime(int camera,cv::Mat &mat);
    void showInRealTime(cv::Mat &mat);

public slots:
    void slotDoSelectTemplate();

    void slotSetAnglePos(CD::AnglePos);

    void slotTestFeature();
    void slotTryIdentify();

    void slotSetOperModeAuto();
    void slotSetOperModeManual();

    void slotSetFeatureAngle();
    void slotSetFeatureCircle();
    void slotSetFeatureRelate();
    void slotSetFeatureCross();
    void slotSetFeatureShape();
    void slotTrigleAnglePos();
    void slotUpdateFeatures();
    void slotFeatureClick();
    void slotFeatureClick(int);
    void slotDetermineTemplate();
    void slotForceReturnSelect();
    void slotDetermineFeature();

    void contextMenuEvent(QContextMenuEvent *);
    void releaseFeatures();
    void displayFeature(QList<CD_Feature *> &fts);
    void releaseListWidgetData();

private:
    void closeEvent(QCloseEvent *);
    void reject();
    void updateEditTemplateScreen();
    void updateEditFeatureScreen();
    void calculateRelatePoint(int num);
    void drawReferencePoints();
    void calculateImageData();
    void handleMatchPar();

    Ui::CCD_MarkEditor *ui;
    CCD_Mark *mark;
    CCD_CncManager *cncmanager;
    CD_CaptureImage *capture;
    QList<CD_Feature *> features;

    std::vector<cv::RotatedRect> all_shapes;
    std::vector<std::vector<cv::Point> >all_shapes_path;
    int index_of_shapes;

    CD::FeatureType shape_base_type;
    cv::Mat shape_mat;

    QDialog *match_par_dlg;

    std::vector<cv::Mat> pyramid_img;
    std::vector<cv::Mat> pyramid_bit;

    int pyramid_index_img;

    bool one_reference;

public:
    static int exe_ret;


private slots:
    void on_CameraGain_valueChanged(int value);
    void on_CameraExposeTime_valueChanged(int value);
    void on_CameraContrast_valueChanged(int value);
    void on_CameraGamma_valueChanged(int value);

    void on_Light1_clicked(bool checked);
    void on_Light2_clicked(bool checked);
    void on_Light3_clicked(bool checked);

    void on_Light1_toggled(bool checked);
    void on_Light2_toggled(bool checked);
    void on_Light3_toggled(bool checked);
    void on_LightPar_Save_clicked();

    void on_SB_SearchLen_valueChanged(int arg1);
    void on_SB_Sim_Threshold_valueChanged(double arg1);

    void on_SB_Fat_valueChanged(int arg1);
    void on_CKB_Calcenter_clicked(bool checked);
    void on_SB_Wh_Threshold_valueChanged(double arg1);

    void on_Btn_GetShape_clicked();
    void on_Btn_ReturnOriImg_clicked();
    void on_DB_SimThreshold_SH_valueChanged(double arg1);
    void on_Canny_Threhold_valueChanged(int value);
    void on_Btn_Mode_Change_clicked();
    void on_Type_Relate_2_toggled(bool checked);
    void on_Type_Circle_2_toggled(bool checked);
    void on_Type_Angle_2_toggled(bool checked);
    void on_Type_Cross_2_toggled(bool checked);

    void on_Btn_CalRefPoint_clicked();
    void on_Btn_CalRefPoint2_clicked();
    void on_Search_Step_valueChanged(double arg1);
    void on_Search_PyNum_valueChanged(int arg1);
    void on_Search_AngleNbh_valueChanged(int arg1);
    void on_Search_RectNbh_valueChanged(int arg1);

    void on_Btn_MorePar_clicked();
    void on_EditMore_clicked(bool checked);

    void on_Img_Last_clicked();
    void on_Img_Next_clicked();
    void on_Btn_Enraser_clicked(bool checked);
    void on_Enraser_Size_valueChanged(int value);


    void on_SB_CameraContra_valueChanged(int arg1);
    void on_SB_ExposeTime_valueChanged(int arg1);
    void on_SB_CameraGain_valueChanged(int arg1);
    void on_SB_CameraGamma_valueChanged(int arg1);

    void on_Search_Enable_Filter_clicked(bool checked);
    void on_Search_Blur_Size_valueChanged(int arg1);
    void on_Search_Erosion_Size_valueChanged(int arg1);


    void on_MarkPos_Save_clicked();
};

#endif // CCD_EDITWIDGET_H
