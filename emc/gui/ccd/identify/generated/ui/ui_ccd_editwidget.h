/********************************************************************************
** Form generated from reading UI file 'ccd_editwidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_EDITWIDGET_H
#define UI_CCD_EDITWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCD_MarkEditor
{
public:
    QHBoxLayout *horizontalLayout_3;
    QWidget *Edit_View;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_4;
    QStackedWidget *stackedWidget_Mode;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_8;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *Mode_Auto;
    QRadioButton *Mode_Man;
    QPushButton *Btn_Identify;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout;
    QRadioButton *Type_Cross;
    QRadioButton *Type_Angle;
    QRadioButton *Type_Circle;
    QRadioButton *Type_Relate;
    QRadioButton *Type_Shape;
    QSpacerItem *verticalSpacer_3;
    QListWidget *listWidget;
    QFrame *Frame_Mannul;
    QFormLayout *formLayout;
    QLabel *label_10;
    QSpinBox *SB_Fat;
    QFrame *Frame_Shape;
    QFormLayout *formLayout_2;
    QLabel *label_7;
    QSpinBox *SB_SearchLen;
    QLabel *label_8;
    QLabel *label_9;
    QDoubleSpinBox *SB_Sim_Threshold;
    QCheckBox *CKB_Calcenter;
    QDoubleSpinBox *SB_Wh_Threshold;
    QSpacerItem *verticalSpacer_2;
    QWidget *page_4;
    QVBoxLayout *verticalLayout_10;
    QFrame *frame_8;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_12;
    QSlider *Canny_Threhold;
    QSpinBox *Thre_Show;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *Btn_GetShape;
    QPushButton *Btn_ReturnOriImg;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_9;
    QRadioButton *Type_Cross_2;
    QRadioButton *Type_Angle_2;
    QRadioButton *Type_Circle_2;
    QRadioButton *Type_Relate_2;
    QPushButton *Btn_CalRefPoint;
    QFrame *frame_7;
    QFormLayout *formLayout_3;
    QLabel *label_14;
    QDoubleSpinBox *DB_SimThreshold_SH;
    QPushButton *Btn_MorePar;
    QFrame *Search_More_Frame;
    QFormLayout *formLayout_4;
    QLabel *label_13;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QDoubleSpinBox *Search_Step;
    QDoubleSpinBox *Search_Start;
    QDoubleSpinBox *Search_End;
    QSpinBox *Search_PyNum;
    QSpinBox *Search_AngleNbh;
    QSpinBox *Search_RectNbh;
    QFrame *frame_4;
    QGridLayout *gridLayout_4;
    QPushButton *Btn_Sure;
    QPushButton *Btn_Return;
    QPushButton *Btn_GetTmp;
    QPushButton *Btn_Mode_Change;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_3;
    QFrame *Frame_Light;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_9;
    QCheckBox *Light1;
    QCheckBox *Light2;
    QCheckBox *Light3;
    QFrame *Frame_CameraPar;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout;
    QSpinBox *SB_CameraContra;
    QSpinBox *spinBox;
    QLabel *label_6;
    QSlider *CameraGain;
    QLabel *label_5;
    QLabel *label_4;
    QSpinBox *SB_CameraGain;
    QLabel *label_3;
    QSlider *CameraContrast;
    QSlider *CameraExposeTime;
    QSlider *CameraGamma;
    QLabel *label_11;
    QSpinBox *SB_CameraGamma;
    QLabel *label;
    QFrame *frame_5;
    QGridLayout *gridLayout_3;
    QPushButton *Template_Cancel;
    QPushButton *LightPar_Save;
    QPushButton *Template_Sure;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_2;
    QSlider *Jog_speed;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *Btn_Cont;
    QPushButton *Btn_Handle;
    QPushButton *Btn_1;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *Btn_001;
    QPushButton *Btn_01;
    QPushButton *Btn_0001;
    QGridLayout *gridLayout_2;
    QPushButton *Btn_Zplus;
    QPushButton *Btn_Yplus;
    QPushButton *pushButton_10;
    QPushButton *Btn_Xminus;
    QPushButton *Btn_Zminus;
    QPushButton *Btn_Yminus;
    QPushButton *Btn_Xplus;
    QPushButton *pushButton_12;
    QPushButton *pushButton_16;

    void setupUi(QDialog *CCD_MarkEditor)
    {
        if (CCD_MarkEditor->objectName().isEmpty())
            CCD_MarkEditor->setObjectName(QString::fromUtf8("CCD_MarkEditor"));
        CCD_MarkEditor->resize(939, 760);
        horizontalLayout_3 = new QHBoxLayout(CCD_MarkEditor);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        Edit_View = new QWidget(CCD_MarkEditor);
        Edit_View->setObjectName(QString::fromUtf8("Edit_View"));
        Edit_View->setMinimumSize(QSize(648, 486));
        Edit_View->setMaximumSize(QSize(648, 486));

        horizontalLayout_3->addWidget(Edit_View);

        stackedWidget = new QStackedWidget(CCD_MarkEditor);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_4 = new QVBoxLayout(page);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        stackedWidget_Mode = new QStackedWidget(page);
        stackedWidget_Mode->setObjectName(QString::fromUtf8("stackedWidget_Mode"));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        verticalLayout_8 = new QVBoxLayout(page_3);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(page_3);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setSpacing(13);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 9, 9, 9);
        widget_2 = new QWidget(frame);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 2, 2, 2);
        Mode_Auto = new QRadioButton(widget_2);
        Mode_Auto->setObjectName(QString::fromUtf8("Mode_Auto"));

        verticalLayout_2->addWidget(Mode_Auto);

        Mode_Man = new QRadioButton(widget_2);
        Mode_Man->setObjectName(QString::fromUtf8("Mode_Man"));

        verticalLayout_2->addWidget(Mode_Man);


        horizontalLayout_2->addWidget(widget_2);

        Btn_Identify = new QPushButton(frame);
        Btn_Identify->setObjectName(QString::fromUtf8("Btn_Identify"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Btn_Identify->sizePolicy().hasHeightForWidth());
        Btn_Identify->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(Btn_Identify);


        verticalLayout_8->addWidget(frame);

        frame_3 = new QFrame(page_3);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, -1, -1, -1);
        widget_3 = new QWidget(frame_3);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        verticalLayout = new QVBoxLayout(widget_3);
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(9, 2, 2, 2);
        Type_Cross = new QRadioButton(widget_3);
        Type_Cross->setObjectName(QString::fromUtf8("Type_Cross"));

        verticalLayout->addWidget(Type_Cross);

        Type_Angle = new QRadioButton(widget_3);
        Type_Angle->setObjectName(QString::fromUtf8("Type_Angle"));

        verticalLayout->addWidget(Type_Angle);

        Type_Circle = new QRadioButton(widget_3);
        Type_Circle->setObjectName(QString::fromUtf8("Type_Circle"));

        verticalLayout->addWidget(Type_Circle);

        Type_Relate = new QRadioButton(widget_3);
        Type_Relate->setObjectName(QString::fromUtf8("Type_Relate"));

        verticalLayout->addWidget(Type_Relate);

        Type_Shape = new QRadioButton(widget_3);
        Type_Shape->setObjectName(QString::fromUtf8("Type_Shape"));

        verticalLayout->addWidget(Type_Shape);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout->addWidget(widget_3);

        listWidget = new QListWidget(frame_3);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout->addWidget(listWidget);


        verticalLayout_8->addWidget(frame_3);

        Frame_Mannul = new QFrame(page_3);
        Frame_Mannul->setObjectName(QString::fromUtf8("Frame_Mannul"));
        Frame_Mannul->setFrameShape(QFrame::StyledPanel);
        Frame_Mannul->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(Frame_Mannul);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_10 = new QLabel(Frame_Mannul);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_10);

        SB_Fat = new QSpinBox(Frame_Mannul);
        SB_Fat->setObjectName(QString::fromUtf8("SB_Fat"));
        SB_Fat->setMinimum(5);
        SB_Fat->setMaximum(30);

        formLayout->setWidget(0, QFormLayout::FieldRole, SB_Fat);


        verticalLayout_8->addWidget(Frame_Mannul);

        Frame_Shape = new QFrame(page_3);
        Frame_Shape->setObjectName(QString::fromUtf8("Frame_Shape"));
        Frame_Shape->setFrameShape(QFrame::StyledPanel);
        Frame_Shape->setFrameShadow(QFrame::Raised);
        formLayout_2 = new QFormLayout(Frame_Shape);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label_7 = new QLabel(Frame_Shape);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_7);

        SB_SearchLen = new QSpinBox(Frame_Shape);
        SB_SearchLen->setObjectName(QString::fromUtf8("SB_SearchLen"));
        SB_SearchLen->setMinimum(5);
        SB_SearchLen->setMaximum(1000);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, SB_SearchLen);

        label_8 = new QLabel(Frame_Shape);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_8);

        label_9 = new QLabel(Frame_Shape);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_9);

        SB_Sim_Threshold = new QDoubleSpinBox(Frame_Shape);
        SB_Sim_Threshold->setObjectName(QString::fromUtf8("SB_Sim_Threshold"));
        SB_Sim_Threshold->setMaximum(1);
        SB_Sim_Threshold->setSingleStep(0.01);
        SB_Sim_Threshold->setValue(0.96);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, SB_Sim_Threshold);

        CKB_Calcenter = new QCheckBox(Frame_Shape);
        CKB_Calcenter->setObjectName(QString::fromUtf8("CKB_Calcenter"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, CKB_Calcenter);

        SB_Wh_Threshold = new QDoubleSpinBox(Frame_Shape);
        SB_Wh_Threshold->setObjectName(QString::fromUtf8("SB_Wh_Threshold"));
        SB_Wh_Threshold->setMinimum(10);
        SB_Wh_Threshold->setMaximum(100);
        SB_Wh_Threshold->setSingleStep(1);
        SB_Wh_Threshold->setValue(15);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, SB_Wh_Threshold);


        verticalLayout_8->addWidget(Frame_Shape);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_2);

        stackedWidget_Mode->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QString::fromUtf8("page_4"));
        verticalLayout_10 = new QVBoxLayout(page_4);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        frame_8 = new QFrame(page_4);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        verticalLayout_11 = new QVBoxLayout(frame_8);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_12 = new QLabel(frame_8);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_7->addWidget(label_12);

        Canny_Threhold = new QSlider(frame_8);
        Canny_Threhold->setObjectName(QString::fromUtf8("Canny_Threhold"));
        Canny_Threhold->setMinimum(10);
        Canny_Threhold->setMaximum(250);
        Canny_Threhold->setValue(120);
        Canny_Threhold->setOrientation(Qt::Horizontal);

        horizontalLayout_7->addWidget(Canny_Threhold);

        Thre_Show = new QSpinBox(frame_8);
        Thre_Show->setObjectName(QString::fromUtf8("Thre_Show"));
        Thre_Show->setReadOnly(true);
        Thre_Show->setMinimum(1);
        Thre_Show->setMaximum(255);

        horizontalLayout_7->addWidget(Thre_Show);


        verticalLayout_11->addLayout(horizontalLayout_7);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        Btn_GetShape = new QPushButton(frame_8);
        Btn_GetShape->setObjectName(QString::fromUtf8("Btn_GetShape"));

        horizontalLayout_10->addWidget(Btn_GetShape);

        Btn_ReturnOriImg = new QPushButton(frame_8);
        Btn_ReturnOriImg->setObjectName(QString::fromUtf8("Btn_ReturnOriImg"));

        horizontalLayout_10->addWidget(Btn_ReturnOriImg);


        verticalLayout_11->addLayout(horizontalLayout_10);


        verticalLayout_10->addWidget(frame_8);

        frame_6 = new QFrame(page_4);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, -1, -1, -1);
        widget_4 = new QWidget(frame_6);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        verticalLayout_9 = new QVBoxLayout(widget_4);
        verticalLayout_9->setSpacing(2);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(9, 2, 2, 2);
        Type_Cross_2 = new QRadioButton(widget_4);
        Type_Cross_2->setObjectName(QString::fromUtf8("Type_Cross_2"));

        verticalLayout_9->addWidget(Type_Cross_2);

        Type_Angle_2 = new QRadioButton(widget_4);
        Type_Angle_2->setObjectName(QString::fromUtf8("Type_Angle_2"));

        verticalLayout_9->addWidget(Type_Angle_2);

        Type_Circle_2 = new QRadioButton(widget_4);
        Type_Circle_2->setObjectName(QString::fromUtf8("Type_Circle_2"));

        verticalLayout_9->addWidget(Type_Circle_2);

        Type_Relate_2 = new QRadioButton(widget_4);
        Type_Relate_2->setObjectName(QString::fromUtf8("Type_Relate_2"));

        verticalLayout_9->addWidget(Type_Relate_2);


        horizontalLayout_5->addWidget(widget_4);

        Btn_CalRefPoint = new QPushButton(frame_6);
        Btn_CalRefPoint->setObjectName(QString::fromUtf8("Btn_CalRefPoint"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Btn_CalRefPoint->sizePolicy().hasHeightForWidth());
        Btn_CalRefPoint->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(Btn_CalRefPoint);


        verticalLayout_10->addWidget(frame_6);

        frame_7 = new QFrame(page_4);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        formLayout_3 = new QFormLayout(frame_7);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_14 = new QLabel(frame_7);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_14);

        DB_SimThreshold_SH = new QDoubleSpinBox(frame_7);
        DB_SimThreshold_SH->setObjectName(QString::fromUtf8("DB_SimThreshold_SH"));
        DB_SimThreshold_SH->setMinimum(0.1);
        DB_SimThreshold_SH->setMaximum(1);
        DB_SimThreshold_SH->setSingleStep(0.1);
        DB_SimThreshold_SH->setValue(0.84);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, DB_SimThreshold_SH);

        Btn_MorePar = new QPushButton(frame_7);
        Btn_MorePar->setObjectName(QString::fromUtf8("Btn_MorePar"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, Btn_MorePar);


        verticalLayout_10->addWidget(frame_7);

        stackedWidget_Mode->addWidget(page_4);

        verticalLayout_4->addWidget(stackedWidget_Mode);

        Search_More_Frame = new QFrame(page);
        Search_More_Frame->setObjectName(QString::fromUtf8("Search_More_Frame"));
        Search_More_Frame->setFrameShape(QFrame::StyledPanel);
        Search_More_Frame->setFrameShadow(QFrame::Raised);
        formLayout_4 = new QFormLayout(Search_More_Frame);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        label_13 = new QLabel(Search_More_Frame);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_13);

        label_15 = new QLabel(Search_More_Frame);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_15);

        label_16 = new QLabel(Search_More_Frame);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, label_16);

        label_17 = new QLabel(Search_More_Frame);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, label_17);

        label_18 = new QLabel(Search_More_Frame);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        formLayout_4->setWidget(4, QFormLayout::LabelRole, label_18);

        label_19 = new QLabel(Search_More_Frame);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        formLayout_4->setWidget(5, QFormLayout::LabelRole, label_19);

        Search_Step = new QDoubleSpinBox(Search_More_Frame);
        Search_Step->setObjectName(QString::fromUtf8("Search_Step"));
        Search_Step->setMinimum(0.2);
        Search_Step->setMaximum(10);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, Search_Step);

        Search_Start = new QDoubleSpinBox(Search_More_Frame);
        Search_Start->setObjectName(QString::fromUtf8("Search_Start"));
        Search_Start->setMinimum(-360);
        Search_Start->setMaximum(360);
        Search_Start->setSingleStep(1);
        Search_Start->setValue(-10);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, Search_Start);

        Search_End = new QDoubleSpinBox(Search_More_Frame);
        Search_End->setObjectName(QString::fromUtf8("Search_End"));
        Search_End->setMinimum(-360);
        Search_End->setMaximum(360);
        Search_End->setSingleStep(1);
        Search_End->setValue(10);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, Search_End);

        Search_PyNum = new QSpinBox(Search_More_Frame);
        Search_PyNum->setObjectName(QString::fromUtf8("Search_PyNum"));
        Search_PyNum->setMinimum(1);
        Search_PyNum->setMaximum(6);

        formLayout_4->setWidget(3, QFormLayout::FieldRole, Search_PyNum);

        Search_AngleNbh = new QSpinBox(Search_More_Frame);
        Search_AngleNbh->setObjectName(QString::fromUtf8("Search_AngleNbh"));
        Search_AngleNbh->setMinimum(2);

        formLayout_4->setWidget(4, QFormLayout::FieldRole, Search_AngleNbh);

        Search_RectNbh = new QSpinBox(Search_More_Frame);
        Search_RectNbh->setObjectName(QString::fromUtf8("Search_RectNbh"));
        Search_RectNbh->setMinimum(2);

        formLayout_4->setWidget(5, QFormLayout::FieldRole, Search_RectNbh);


        verticalLayout_4->addWidget(Search_More_Frame);

        frame_4 = new QFrame(page);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frame_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        Btn_Sure = new QPushButton(frame_4);
        Btn_Sure->setObjectName(QString::fromUtf8("Btn_Sure"));

        gridLayout_4->addWidget(Btn_Sure, 1, 0, 1, 1);

        Btn_Return = new QPushButton(frame_4);
        Btn_Return->setObjectName(QString::fromUtf8("Btn_Return"));

        gridLayout_4->addWidget(Btn_Return, 1, 1, 1, 1);

        Btn_GetTmp = new QPushButton(frame_4);
        Btn_GetTmp->setObjectName(QString::fromUtf8("Btn_GetTmp"));

        gridLayout_4->addWidget(Btn_GetTmp, 0, 0, 1, 1);

        Btn_Mode_Change = new QPushButton(frame_4);
        Btn_Mode_Change->setObjectName(QString::fromUtf8("Btn_Mode_Change"));

        gridLayout_4->addWidget(Btn_Mode_Change, 0, 1, 1, 1);


        verticalLayout_4->addWidget(frame_4);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_3 = new QVBoxLayout(page_2);
        verticalLayout_3->setSpacing(8);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        Frame_Light = new QFrame(page_2);
        Frame_Light->setObjectName(QString::fromUtf8("Frame_Light"));
        Frame_Light->setFrameShape(QFrame::StyledPanel);
        Frame_Light->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(Frame_Light);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        Light1 = new QCheckBox(Frame_Light);
        Light1->setObjectName(QString::fromUtf8("Light1"));

        horizontalLayout_9->addWidget(Light1);

        Light2 = new QCheckBox(Frame_Light);
        Light2->setObjectName(QString::fromUtf8("Light2"));

        horizontalLayout_9->addWidget(Light2);

        Light3 = new QCheckBox(Frame_Light);
        Light3->setObjectName(QString::fromUtf8("Light3"));

        horizontalLayout_9->addWidget(Light3);


        verticalLayout_6->addLayout(horizontalLayout_9);


        verticalLayout_3->addWidget(Frame_Light);

        Frame_CameraPar = new QFrame(page_2);
        Frame_CameraPar->setObjectName(QString::fromUtf8("Frame_CameraPar"));
        Frame_CameraPar->setFrameShape(QFrame::StyledPanel);
        Frame_CameraPar->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(Frame_CameraPar);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        SB_CameraContra = new QSpinBox(Frame_CameraPar);
        SB_CameraContra->setObjectName(QString::fromUtf8("SB_CameraContra"));
        SB_CameraContra->setReadOnly(true);
        SB_CameraContra->setMinimum(1);
        SB_CameraContra->setMaximum(1000);

        gridLayout->addWidget(SB_CameraContra, 4, 2, 1, 1);

        spinBox = new QSpinBox(Frame_CameraPar);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setReadOnly(true);
        spinBox->setMinimum(1);
        spinBox->setMaximum(800);

        gridLayout->addWidget(spinBox, 3, 2, 1, 1);

        label_6 = new QLabel(Frame_CameraPar);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 3, 3, 1, 1);

        CameraGain = new QSlider(Frame_CameraPar);
        CameraGain->setObjectName(QString::fromUtf8("CameraGain"));
        CameraGain->setMinimum(1);
        CameraGain->setMaximum(500);
        CameraGain->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(CameraGain, 0, 1, 1, 1);

        label_5 = new QLabel(Frame_CameraPar);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        label_4 = new QLabel(Frame_CameraPar);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        SB_CameraGain = new QSpinBox(Frame_CameraPar);
        SB_CameraGain->setObjectName(QString::fromUtf8("SB_CameraGain"));
        SB_CameraGain->setReadOnly(true);
        SB_CameraGain->setMinimum(1);
        SB_CameraGain->setMaximum(500);

        gridLayout->addWidget(SB_CameraGain, 0, 2, 1, 1);

        label_3 = new QLabel(Frame_CameraPar);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        CameraContrast = new QSlider(Frame_CameraPar);
        CameraContrast->setObjectName(QString::fromUtf8("CameraContrast"));
        CameraContrast->setMinimum(1);
        CameraContrast->setMaximum(1000);
        CameraContrast->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(CameraContrast, 4, 1, 1, 1);

        CameraExposeTime = new QSlider(Frame_CameraPar);
        CameraExposeTime->setObjectName(QString::fromUtf8("CameraExposeTime"));
        CameraExposeTime->setMinimum(1);
        CameraExposeTime->setMaximum(800);
        CameraExposeTime->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(CameraExposeTime, 3, 1, 1, 1);

        CameraGamma = new QSlider(Frame_CameraPar);
        CameraGamma->setObjectName(QString::fromUtf8("CameraGamma"));
        CameraGamma->setMinimum(1);
        CameraGamma->setMaximum(1000);
        CameraGamma->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(CameraGamma, 5, 1, 1, 1);

        label_11 = new QLabel(Frame_CameraPar);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_11, 5, 0, 1, 1);

        SB_CameraGamma = new QSpinBox(Frame_CameraPar);
        SB_CameraGamma->setObjectName(QString::fromUtf8("SB_CameraGamma"));
        SB_CameraGamma->setReadOnly(true);
        SB_CameraGamma->setMinimum(1);
        SB_CameraGamma->setMaximum(1000);

        gridLayout->addWidget(SB_CameraGamma, 5, 2, 1, 1);


        verticalLayout_5->addLayout(gridLayout);


        verticalLayout_3->addWidget(Frame_CameraPar);

        label = new QLabel(page_2);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_3->addWidget(label);

        frame_5 = new QFrame(page_2);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_5);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(4, -1, -1, -1);
        Template_Cancel = new QPushButton(frame_5);
        Template_Cancel->setObjectName(QString::fromUtf8("Template_Cancel"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Template_Cancel->sizePolicy().hasHeightForWidth());
        Template_Cancel->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(Template_Cancel, 0, 0, 1, 1);

        LightPar_Save = new QPushButton(frame_5);
        LightPar_Save->setObjectName(QString::fromUtf8("LightPar_Save"));
        sizePolicy2.setHeightForWidth(LightPar_Save->sizePolicy().hasHeightForWidth());
        LightPar_Save->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(LightPar_Save, 0, 1, 1, 1);

        Template_Sure = new QPushButton(frame_5);
        Template_Sure->setObjectName(QString::fromUtf8("Template_Sure"));
        sizePolicy2.setHeightForWidth(Template_Sure->sizePolicy().hasHeightForWidth());
        Template_Sure->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(Template_Sure, 0, 2, 1, 1);


        verticalLayout_3->addWidget(frame_5);

        frame_2 = new QFrame(page_2);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy3);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_2);
        horizontalLayout_4->setSpacing(1);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(4, 4, 0, 4);
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_2);

        Jog_speed = new QSlider(frame_2);
        Jog_speed->setObjectName(QString::fromUtf8("Jog_speed"));
        Jog_speed->setMaximum(1500);
        Jog_speed->setOrientation(Qt::Horizontal);

        verticalLayout_7->addWidget(Jog_speed);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        Btn_Cont = new QPushButton(frame_2);
        Btn_Cont->setObjectName(QString::fromUtf8("Btn_Cont"));
        Btn_Cont->setStyleSheet(QString::fromUtf8("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_Cont->setCheckable(true);
        Btn_Cont->setChecked(true);
        Btn_Cont->setAutoExclusive(true);

        horizontalLayout_6->addWidget(Btn_Cont);

        Btn_Handle = new QPushButton(frame_2);
        Btn_Handle->setObjectName(QString::fromUtf8("Btn_Handle"));
        Btn_Handle->setStyleSheet(QString::fromUtf8("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_Handle->setCheckable(true);
        Btn_Handle->setAutoExclusive(true);

        horizontalLayout_6->addWidget(Btn_Handle);

        Btn_1 = new QPushButton(frame_2);
        Btn_1->setObjectName(QString::fromUtf8("Btn_1"));
        Btn_1->setMinimumSize(QSize(0, 0));
        Btn_1->setStyleSheet(QString::fromUtf8("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_1->setCheckable(true);
        Btn_1->setAutoExclusive(true);

        horizontalLayout_6->addWidget(Btn_1);


        verticalLayout_7->addLayout(horizontalLayout_6);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        Btn_001 = new QPushButton(frame_2);
        Btn_001->setObjectName(QString::fromUtf8("Btn_001"));
        Btn_001->setMinimumSize(QSize(0, 0));
        Btn_001->setStyleSheet(QString::fromUtf8("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_001->setCheckable(true);
        Btn_001->setAutoExclusive(true);

        horizontalLayout_8->addWidget(Btn_001);

        Btn_01 = new QPushButton(frame_2);
        Btn_01->setObjectName(QString::fromUtf8("Btn_01"));
        Btn_01->setMinimumSize(QSize(0, 0));
        Btn_01->setStyleSheet(QString::fromUtf8("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_01->setCheckable(true);
        Btn_01->setAutoExclusive(true);

        horizontalLayout_8->addWidget(Btn_01);

        Btn_0001 = new QPushButton(frame_2);
        Btn_0001->setObjectName(QString::fromUtf8("Btn_0001"));
        Btn_0001->setMinimumSize(QSize(0, 0));
        Btn_0001->setStyleSheet(QString::fromUtf8("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_0001->setCheckable(true);
        Btn_0001->setAutoExclusive(true);

        horizontalLayout_8->addWidget(Btn_0001);


        verticalLayout_7->addLayout(horizontalLayout_8);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        Btn_Zplus = new QPushButton(frame_2);
        Btn_Zplus->setObjectName(QString::fromUtf8("Btn_Zplus"));

        gridLayout_2->addWidget(Btn_Zplus, 0, 0, 1, 1);

        Btn_Yplus = new QPushButton(frame_2);
        Btn_Yplus->setObjectName(QString::fromUtf8("Btn_Yplus"));

        gridLayout_2->addWidget(Btn_Yplus, 0, 1, 1, 1);

        pushButton_10 = new QPushButton(frame_2);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));

        gridLayout_2->addWidget(pushButton_10, 2, 1, 1, 1);

        Btn_Xminus = new QPushButton(frame_2);
        Btn_Xminus->setObjectName(QString::fromUtf8("Btn_Xminus"));

        gridLayout_2->addWidget(Btn_Xminus, 2, 0, 1, 1);

        Btn_Zminus = new QPushButton(frame_2);
        Btn_Zminus->setObjectName(QString::fromUtf8("Btn_Zminus"));

        gridLayout_2->addWidget(Btn_Zminus, 3, 0, 1, 1);

        Btn_Yminus = new QPushButton(frame_2);
        Btn_Yminus->setObjectName(QString::fromUtf8("Btn_Yminus"));

        gridLayout_2->addWidget(Btn_Yminus, 3, 1, 1, 1);

        Btn_Xplus = new QPushButton(frame_2);
        Btn_Xplus->setObjectName(QString::fromUtf8("Btn_Xplus"));

        gridLayout_2->addWidget(Btn_Xplus, 2, 2, 1, 1);

        pushButton_12 = new QPushButton(frame_2);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));

        gridLayout_2->addWidget(pushButton_12, 0, 2, 1, 1);

        pushButton_16 = new QPushButton(frame_2);
        pushButton_16->setObjectName(QString::fromUtf8("pushButton_16"));

        gridLayout_2->addWidget(pushButton_16, 3, 2, 1, 1);


        verticalLayout_7->addLayout(gridLayout_2);


        horizontalLayout_4->addLayout(verticalLayout_7);


        verticalLayout_3->addWidget(frame_2);

        stackedWidget->addWidget(page_2);

        horizontalLayout_3->addWidget(stackedWidget);


        retranslateUi(CCD_MarkEditor);

        stackedWidget->setCurrentIndex(0);
        stackedWidget_Mode->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(CCD_MarkEditor);
    } // setupUi

    void retranslateUi(QDialog *CCD_MarkEditor)
    {
        CCD_MarkEditor->setWindowTitle(QApplication::translate("CCD_MarkEditor", "Dialog", 0, QApplication::UnicodeUTF8));
        Mode_Auto->setText(QApplication::translate("CCD_MarkEditor", "\350\207\252\345\212\250  ", 0, QApplication::UnicodeUTF8));
        Mode_Man->setText(QApplication::translate("CCD_MarkEditor", "\346\211\213\345\212\250  ", 0, QApplication::UnicodeUTF8));
        Btn_Identify->setText(QApplication::translate("CCD_MarkEditor", "\350\257\206\345\210\253", 0, QApplication::UnicodeUTF8));
        Type_Cross->setText(QApplication::translate("CCD_MarkEditor", "\345\215\201\345\255\227", 0, QApplication::UnicodeUTF8));
        Type_Angle->setText(QApplication::translate("CCD_MarkEditor", "\347\233\264\350\247\222", 0, QApplication::UnicodeUTF8));
        Type_Circle->setText(QApplication::translate("CCD_MarkEditor", "\345\234\206\345\275\242", 0, QApplication::UnicodeUTF8));
        Type_Relate->setText(QApplication::translate("CCD_MarkEditor", "\345\217\202\350\200\203\347\202\271", 0, QApplication::UnicodeUTF8));
        Type_Shape->setText(QApplication::translate("CCD_MarkEditor", "\350\275\256\345\273\223", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CCD_MarkEditor", "\350\275\256\345\273\223\345\256\275\345\272\246", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CCD_MarkEditor", "\350\275\256\345\273\223\346\220\234\347\264\242\350\267\235\347\246\273", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CCD_MarkEditor", "\351\225\277\345\256\275\350\257\257\345\267\256\345\205\201\350\256\270", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CCD_MarkEditor", "\347\233\270\344\274\274\345\272\246", 0, QApplication::UnicodeUTF8));
        CKB_Calcenter->setText(QApplication::translate("CCD_MarkEditor", "\350\256\241\347\256\227\345\234\206\345\277\203", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("CCD_MarkEditor", "\351\230\210\345\200\274", 0, QApplication::UnicodeUTF8));
        Btn_GetShape->setText(QApplication::translate("CCD_MarkEditor", "\350\216\267\345\217\226\345\275\242\347\212\266", 0, QApplication::UnicodeUTF8));
        Btn_ReturnOriImg->setText(QApplication::translate("CCD_MarkEditor", "\350\277\224\345\233\236\345\216\237\345\233\276", 0, QApplication::UnicodeUTF8));
        Type_Cross_2->setText(QApplication::translate("CCD_MarkEditor", "\345\215\201\345\255\227", 0, QApplication::UnicodeUTF8));
        Type_Angle_2->setText(QApplication::translate("CCD_MarkEditor", "\347\233\264\350\247\222", 0, QApplication::UnicodeUTF8));
        Type_Circle_2->setText(QApplication::translate("CCD_MarkEditor", "\345\234\206\345\275\242", 0, QApplication::UnicodeUTF8));
        Type_Relate_2->setText(QApplication::translate("CCD_MarkEditor", "\346\214\207\345\256\232", 0, QApplication::UnicodeUTF8));
        Btn_CalRefPoint->setText(QApplication::translate("CCD_MarkEditor", "\350\256\241\347\256\227\345\217\202\350\200\203\347\202\271", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("CCD_MarkEditor", "\347\233\270\344\274\274\345\272\246", 0, QApplication::UnicodeUTF8));
        Btn_MorePar->setText(QApplication::translate("CCD_MarkEditor", "\351\253\230\347\272\247\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("CCD_MarkEditor", "\350\247\222\345\272\246\346\255\245\351\225\277", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("CCD_MarkEditor", "\350\265\267\345\247\213\350\247\222\345\272\246", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("CCD_MarkEditor", "\347\273\223\346\235\237\350\247\222\345\272\246", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("CCD_MarkEditor", "\346\220\234\347\264\242\345\217\202\346\225\2601", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("CCD_MarkEditor", "\346\220\234\347\264\242\345\217\202\346\225\2602", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("CCD_MarkEditor", "\346\220\234\347\264\242\345\217\202\346\225\2603", 0, QApplication::UnicodeUTF8));
        Btn_Sure->setText(QApplication::translate("CCD_MarkEditor", "\347\241\256\345\256\232\347\211\271\345\276\201", 0, QApplication::UnicodeUTF8));
        Btn_Return->setText(QApplication::translate("CCD_MarkEditor", "\346\224\276\345\274\203\350\277\224\345\233\236", 0, QApplication::UnicodeUTF8));
        Btn_GetTmp->setText(QApplication::translate("CCD_MarkEditor", "\351\207\215\346\226\260\350\216\267\345\217\226\346\250\241\346\235\277", 0, QApplication::UnicodeUTF8));
        Btn_Mode_Change->setText(QApplication::translate("CCD_MarkEditor", "\347\201\260\345\272\246\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        Light1->setText(QApplication::translate("CCD_MarkEditor", "\345\205\211\346\272\2201", 0, QApplication::UnicodeUTF8));
        Light2->setText(QApplication::translate("CCD_MarkEditor", "\345\205\211\346\272\2202", 0, QApplication::UnicodeUTF8));
        Light3->setText(QApplication::translate("CCD_MarkEditor", "\345\205\211\346\272\2203", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CCD_MarkEditor", "ms", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CCD_MarkEditor", "\345\257\271\346\257\224\345\272\246", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CCD_MarkEditor", "\346\233\235\345\205\211\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CCD_MarkEditor", "\345\242\236\347\233\212", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("CCD_MarkEditor", "\344\274\275\351\251\254\345\200\274", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CCD_MarkEditor", "       \350\257\267\346\213\226\345\212\250\350\223\235\350\211\262\347\232\204\346\241\206\346\235\245\351\200\211\346\213\251\346\250\241\346\235\277\357\274\214\n"
"\345\246\202\346\227\240\351\234\200\346\233\264\346\215\242\346\250\241\346\235\277\357\274\214\347\233\264\346\216\245\347\202\271\345\207\273\344\275\277\347\224\250\n"
"\344\270\212\344\270\200\346\254\241\350\277\233\345\205\245\347\274\226\350\276\221\347\211\271\345\276\201\347\225\214\351\235\242", 0, QApplication::UnicodeUTF8));
        Template_Cancel->setText(QApplication::translate("CCD_MarkEditor", "\344\275\277\347\224\250\344\270\212\344\270\200\346\254\241", 0, QApplication::UnicodeUTF8));
        LightPar_Save->setText(QApplication::translate("CCD_MarkEditor", "\344\277\235\345\255\230\345\205\211\347\205\247", 0, QApplication::UnicodeUTF8));
        Template_Sure->setText(QApplication::translate("CCD_MarkEditor", "\347\241\256\345\256\232\346\250\241\346\235\277", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CCD_MarkEditor", "  \350\277\236\347\273\255\351\200\237\345\272\246", 0, QApplication::UnicodeUTF8));
        Btn_Cont->setText(QApplication::translate("CCD_MarkEditor", "\350\277\236\347\273\255", 0, QApplication::UnicodeUTF8));
        Btn_Handle->setText(QApplication::translate("CCD_MarkEditor", "\346\211\213\350\275\256", 0, QApplication::UnicodeUTF8));
        Btn_1->setText(QApplication::translate("CCD_MarkEditor", "1mm", 0, QApplication::UnicodeUTF8));
        Btn_001->setText(QApplication::translate("CCD_MarkEditor", "0.01mm", 0, QApplication::UnicodeUTF8));
        Btn_01->setText(QApplication::translate("CCD_MarkEditor", "0.1mm", 0, QApplication::UnicodeUTF8));
        Btn_0001->setText(QApplication::translate("CCD_MarkEditor", "0.001mm", 0, QApplication::UnicodeUTF8));
        Btn_Zplus->setText(QApplication::translate("CCD_MarkEditor", "Z+", 0, QApplication::UnicodeUTF8));
        Btn_Yplus->setText(QApplication::translate("CCD_MarkEditor", "Y+", 0, QApplication::UnicodeUTF8));
        pushButton_10->setText(QString());
        Btn_Xminus->setText(QApplication::translate("CCD_MarkEditor", "X-", 0, QApplication::UnicodeUTF8));
        Btn_Zminus->setText(QApplication::translate("CCD_MarkEditor", "Z-", 0, QApplication::UnicodeUTF8));
        Btn_Yminus->setText(QApplication::translate("CCD_MarkEditor", "Y-", 0, QApplication::UnicodeUTF8));
        Btn_Xplus->setText(QApplication::translate("CCD_MarkEditor", "X+", 0, QApplication::UnicodeUTF8));
        pushButton_12->setText(QString());
        pushButton_16->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CCD_MarkEditor: public Ui_CCD_MarkEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_EDITWIDGET_H
