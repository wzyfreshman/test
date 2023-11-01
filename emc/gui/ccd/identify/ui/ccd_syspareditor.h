#ifndef CCD_SYSPAREDITOR_H
#define CCD_SYSPAREDITOR_H

#include <QDialog>
#include <QPointF>

#include "ccd_marksface.h"
#include "cnc_listener.h"
#include "rs_vector.h"
#include "cd_inisettings.h"

#include <opencv2/core/core.hpp>

namespace Ui {
class CCD_SysParEditor;
}

class CCD_SysParameter;
class CCD_MarkEditor;
class CD_ImageScreen;
class CCD_CncManager;
class CCD_Mark;
class CD_FetchPiece;
class CD_Template;
class CD_InterestRect;
class CCD_MagViewDlg;
class CD_CaptureImage;

class CD_AutoLearnPoints {
public:
    CD_AutoLearnPoints(const QPointF &imagep,const QPointF &actp)
        :imagepoint(imagep),actualpoint(actp) {}
    QPointF imagepoint;
    QPointF actualpoint;
};

class CCD_SysParEditor : public QDialog,public CCD_MarksFace,public CNC_Listener
{
    Q_OBJECT
public:
    enum State {
        StateIdle,
        StateEnditingMark
    };

public:
    explicit CCD_SysParEditor(QWidget *parent = 0);
    ~CCD_SysParEditor();
    void init(CCD_SysParameter *,CD_CaptureImage *c);
    void initconnect();
    void saveToDisk(CD_IniSettings &set,const QString &grp);
    void readFromDisk(CD_IniSettings &set,const QString &grp);
    void showInRealTime(cv::Mat &mat);
    void setCncManager(CCD_CncManager *m,int id);
    void setCapture(CD_CaptureImage *c);

public slots:

    //**camera
    void slotCameraGain(int);
    void slotCameraShutter(int);
    void slotCameraGamma(int);
    void slotCamerBlackLevel(int);
    void slotSetCameraMirrorV(bool);
    void slotSetCameraMirrorH(bool);

    //**general
    void slotSure();
    void slotCancel();

    //*editor ui control
    void slotTurnToCamera();
    void slotTurnToCCDHeight();
    void slotTurnToCCDMag();
    void slotTurnToCCDOffset();
    void slotTurnToGlobalOffset();
    void slotTurnToImgCompensate();

     //****markedit need
    void slotGetMark(int index = -1);
    void slotAutoLearnIdentify();
    void activeMainScreen();

    //*camera height
    void slotSetCameraHeight();
    void slotReadCameraH();
    void slotFetchImage(int);
    void slotShowMag();
    void slotCheckAll(bool);

    //*camera mag
    void slotSetMagX(double);
    void slotSetMagY(double);
    void slotIdPos1();
    void slotIdPos2();
    void slotIdPos3();
    void slotIdPos4();
    void slotComputeMag();
    void slotStartAutoLearn();

    //*camera offset
    void slotSetOffsetX(double);
    void slotSetOffsetY(double);
    void slotSetOffsetAngle(double);
    void slotRecPositionForOffset1();
    void slotRecPositionForOffset2();

    void slotIdMarkForOffset(int index);

    void slotIdMarkForOffset1();
    void slotGetMarkForOffset1();

    void slotIdMarkForOffset2();
    void slotGetMarkForOffset2();

    void slotGetMarkForOffset3();
    void slotIdMarkForOffset3();

    void slotGetMarkForOffset4();
    void slotIdMarkForOffset4();

    void slotGetMarkForOffset5();
    void slotIdMarkForOffset5();

    void slotGetMarkForOffset6();
    void slotIdMarkForOffset6();


    void slotCalculateCenterPoint1();
    void slotCalculateCenterPoint2();
    void slotCalculateCenterPoint(RS_Vector &v);
    void slotCalculateOffset();

    //**abort
    void slotCncAbort();
    void slotSaveMark();

    //****
    void slotSetGlbOffsetH(double);
    void slotSetGlbOffsetV(double);
    void slotRecPositionGlbOffset();
    void slotIdPositionGlbOffset();
    void slotCalculateGlbOffset();
    void slotGetGlbOffsetMark();

    ///image compensate

    //void slotEnableImgCompensate(bool);
    //void slotImgTranMatCompute();
    //void slotDoImgTran();
    //void slotReadAffinePoints();
    //void slotSetRectWidth(double);
    //void slotSetRectHeight(double);

    ///two_points_w
    void slotSetThrRadius(double);
    void slotThrRadiusCompute();

    void slotGetMarkForThrRad1();
    void slotGetMarkForThrRad2();
    void slotGetMarkForThrRad3();
    void slotGetMarkForThrRad4();
    void slotIdMarkForThrRad1();
    void slotIdMarkForThrRad2();
    void slotIdMarkForThrRad3();
    void slotIdMarkForThrRad4();

    ///log to the widget
    void log(const QString &str);

protected:
    void closeEvent(QCloseEvent *);
    void reject();
    void timerEvent(QTimerEvent *);
    void drawIdResult(CD_MainScreen *m,CD_Template *t,RS_Vector *Result);
    void doImgCompensate(cv::Mat &F,cv::Mat&T);
    RS_Vector calculateTranCenter(RS_Vector &v1,RS_Vector &v2,double angle);
    RS_Vector calculateTranCenter(RS_Vector &v1,RS_Vector &v2,
                                  RS_Vector &v3,RS_Vector &v4,
                                  double angle1,double angle2,double angle3);
    double angleThreePoint(const RS_Vector &p1,
                                           const RS_Vector &p2,
                                           const RS_Vector &p3);
    double distanceToAngle(double d);

private slots:
    void on_CB_Resolution_activated(int index);
    void on_LinE_CntPosX_valueChanged(double arg1);
    void on_LinE_CntPosY_valueChanged(double arg1);
    void on_SB_ZHeight_valueChanged(double arg1);

    void on_Btn_SetCameraName_clicked();

private:
    void m91Notify(QString n);

private:
    Ui::CCD_SysParEditor *ui;

    CD_ImageScreen *editsc;
    CD_ImageScreen *selectsc;
    QWidget *editplace;
    QWidget *selectplace;
    CCD_Mark *mark;

    CD_ImageScreen *editsc_for_offset[10];
    CD_ImageScreen *selectsc_for_offset[10];
    CCD_Mark *mark_for_offset[10];

    CCD_MarkEditor *markeditor;
    CD_FetchPiece *fetchview[2];
    CD_ImageScreen *fetchviewsc[2];
    CD_ImageScreen *fetchview_alone;
    CD_InterestRect *fetchview_alone_sel;
    CCD_MagViewDlg *mag_view;

    CCD_CncManager *cncmanager;
    CD_CaptureImage *capture;
    CCD_SysParameter *syspar;

    State state;
    int timerid;
    QList<CD_AutoLearnPoints> autolearnp;

    //******for glboffset
    RS_Vector glboffset_poscnt;
    RS_Vector glboffset_posimg;

    //******for offset
    RS_Vector offset_pos[6];
    int index_of_offset_center_point;
    double offset_length1;
    double offset_length2;
    RS_Vector offset_center_pos1;
    RS_Vector offset_center_pos2;
    QList<RS_Vector> vcts;

    //******for mag
    RS_Vector mag_img_pos1;
    RS_Vector mag_img_pos2;
    RS_Vector mag_img_pos3;
    RS_Vector mag_img_pos4;
    RS_Vector mag_real_pos1;
    RS_Vector mag_real_pos2;
    RS_Vector mag_real_pos3;
    RS_Vector mag_real_pos4;

    //****for compensate  //now for radius detect
    double compensate_anglex;
    double compensate_angley;
    double compensate_offsetx;
    double compensate_offsety;
    bool enable_img_compensate;

    RS_Vector imgtran_pos[5];
    cv::Mat org_Img;
    cv::Mat tran_mat;
    bool in_img_compensate;


    int id;

};

#endif // CCD_SYSPAREDITOR_H
