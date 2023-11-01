/********************************************************************************
** Form generated from reading UI file 'ccd_editwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_EDITWIDGET_H
#define UI_CCD_EDITWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCD_MarkEditor
{
public:
    QHBoxLayout *horizontalLayout_3;
    QWidget *Edit_View;
    QTableView *tableView;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_13;
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
    QPushButton *Btn_EditBitImg;
    QPushButton *Btn_ReturnOriImg;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_20;
    QSlider *Enraser_Size;
    QSpinBox *Enraser_Show;
    QPushButton *Btn_Enraser;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *EditMore;
    QWidget *Widget_Tools;
    QGridLayout *gridLayout_5;
    QPushButton *Img_Next;
    QPushButton *Img_Last;
    QLabel *Img_IndexLabel;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_9;
    QRadioButton *Type_Cross_2;
    QRadioButton *Type_Angle_2;
    QRadioButton *Type_Circle_2;
    QRadioButton *Type_Relate_2;
    QPushButton *Btn_CalRefPoint;
    QPushButton *Btn_CalRefPoint2;
    QFrame *frame_9;
    QGridLayout *gridLayout_6;
    QCheckBox *topLeft;
    QCheckBox *bottomLeft;
    QCheckBox *topRight;
    QCheckBox *bottomRight;
    QCheckBox *crossCenter;
    QFrame *frame_7;
    QFormLayout *formLayout_3;
    QLabel *label_14;
    QDoubleSpinBox *DB_SimThreshold_SH;
    QPushButton *Btn_MorePar;
    QFrame *Search_More_Frame;
    QGridLayout *gridLayout_7;
    QSpinBox *Search_Blur_Size;
    QLabel *label_15;
    QSpinBox *Search_PyNum;
    QDoubleSpinBox *Search_Start1;
    QDoubleSpinBox *Search_End1;
    QLabel *label_17;
    QLabel *label_19;
    QLabel *label_16;
    QSpinBox *Search_AngleNbh;
    QSpinBox *Search_RectNbh;
    QLabel *label_18;
    QDoubleSpinBox *Search_Start3;
    QDoubleSpinBox *Search_Start2;
    QDoubleSpinBox *Search_Start4;
    QCheckBox *Search_Enable1;
    QDoubleSpinBox *Search_End3;
    QCheckBox *Search_Enable3;
    QCheckBox *Search_Enable2;
    QDoubleSpinBox *Search_End2;
    QDoubleSpinBox *Search_End4;
    QCheckBox *Search_Enable4;
    QLabel *label_13;
    QDoubleSpinBox *Search_Step;
    QSpinBox *Search_Erosion_Size;
    QLabel *label_21;
    QLabel *label_22;
    QPushButton *Btn_MatchParSure;
    QCheckBox *Search_Enable_Filter;
    QCheckBox *Gray_Asisst_Enable;
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
    QSpinBox *SB_ExposeTime;
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
    QFrame *frame_10;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_23;
    QSpinBox *exposeCount;
    QLabel *label_24;
    QDoubleSpinBox *exposeValue;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QFrame *frame_5;
    QGridLayout *gridLayout_3;
    QPushButton *AreaSelPar_Save;
    QPushButton *Template_Cancel;
    QPushButton *LightPar_Save;
    QPushButton *MarkPosSaved;
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
            CCD_MarkEditor->setObjectName(QStringLiteral("CCD_MarkEditor"));
        CCD_MarkEditor->resize(1077, 855);
        horizontalLayout_3 = new QHBoxLayout(CCD_MarkEditor);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        Edit_View = new QWidget(CCD_MarkEditor);
        Edit_View->setObjectName(QStringLiteral("Edit_View"));
        Edit_View->setMinimumSize(QSize(648, 486));
        Edit_View->setMaximumSize(QSize(648, 486));
        tableView = new QTableView(Edit_View);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(300, 70, 256, 192));
        widget = new QWidget(Edit_View);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(260, 330, 405, 44));
        horizontalLayout_13 = new QHBoxLayout(widget);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));

        horizontalLayout_3->addWidget(Edit_View);

        stackedWidget = new QStackedWidget(CCD_MarkEditor);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        verticalLayout_4 = new QVBoxLayout(page);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        stackedWidget_Mode = new QStackedWidget(page);
        stackedWidget_Mode->setObjectName(QStringLiteral("stackedWidget_Mode"));
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        verticalLayout_8 = new QVBoxLayout(page_3);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(page_3);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setSpacing(13);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 9, 9, 9);
        widget_2 = new QWidget(frame);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, 2, 2, 2);
        Mode_Auto = new QRadioButton(widget_2);
        Mode_Auto->setObjectName(QStringLiteral("Mode_Auto"));

        verticalLayout_2->addWidget(Mode_Auto);

        Mode_Man = new QRadioButton(widget_2);
        Mode_Man->setObjectName(QStringLiteral("Mode_Man"));

        verticalLayout_2->addWidget(Mode_Man);


        horizontalLayout_2->addWidget(widget_2);

        Btn_Identify = new QPushButton(frame);
        Btn_Identify->setObjectName(QStringLiteral("Btn_Identify"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Btn_Identify->sizePolicy().hasHeightForWidth());
        Btn_Identify->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(Btn_Identify);


        verticalLayout_8->addWidget(frame);

        frame_3 = new QFrame(page_3);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_3);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, -1, -1, -1);
        widget_3 = new QWidget(frame_3);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        verticalLayout = new QVBoxLayout(widget_3);
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(9, 2, 2, 2);
        Type_Cross = new QRadioButton(widget_3);
        Type_Cross->setObjectName(QStringLiteral("Type_Cross"));

        verticalLayout->addWidget(Type_Cross);

        Type_Angle = new QRadioButton(widget_3);
        Type_Angle->setObjectName(QStringLiteral("Type_Angle"));

        verticalLayout->addWidget(Type_Angle);

        Type_Circle = new QRadioButton(widget_3);
        Type_Circle->setObjectName(QStringLiteral("Type_Circle"));

        verticalLayout->addWidget(Type_Circle);

        Type_Relate = new QRadioButton(widget_3);
        Type_Relate->setObjectName(QStringLiteral("Type_Relate"));

        verticalLayout->addWidget(Type_Relate);

        Type_Shape = new QRadioButton(widget_3);
        Type_Shape->setObjectName(QStringLiteral("Type_Shape"));

        verticalLayout->addWidget(Type_Shape);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout->addWidget(widget_3);

        listWidget = new QListWidget(frame_3);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        horizontalLayout->addWidget(listWidget);


        verticalLayout_8->addWidget(frame_3);

        Frame_Mannul = new QFrame(page_3);
        Frame_Mannul->setObjectName(QStringLiteral("Frame_Mannul"));
        Frame_Mannul->setFrameShape(QFrame::StyledPanel);
        Frame_Mannul->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(Frame_Mannul);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_10 = new QLabel(Frame_Mannul);
        label_10->setObjectName(QStringLiteral("label_10"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_10);

        SB_Fat = new QSpinBox(Frame_Mannul);
        SB_Fat->setObjectName(QStringLiteral("SB_Fat"));
        SB_Fat->setMinimum(5);
        SB_Fat->setMaximum(30);

        formLayout->setWidget(0, QFormLayout::FieldRole, SB_Fat);


        verticalLayout_8->addWidget(Frame_Mannul);

        Frame_Shape = new QFrame(page_3);
        Frame_Shape->setObjectName(QStringLiteral("Frame_Shape"));
        Frame_Shape->setFrameShape(QFrame::StyledPanel);
        Frame_Shape->setFrameShadow(QFrame::Raised);
        formLayout_2 = new QFormLayout(Frame_Shape);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_7 = new QLabel(Frame_Shape);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_7);

        SB_SearchLen = new QSpinBox(Frame_Shape);
        SB_SearchLen->setObjectName(QStringLiteral("SB_SearchLen"));
        SB_SearchLen->setMinimum(5);
        SB_SearchLen->setMaximum(1000);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, SB_SearchLen);

        label_8 = new QLabel(Frame_Shape);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_8);

        label_9 = new QLabel(Frame_Shape);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_9);

        SB_Sim_Threshold = new QDoubleSpinBox(Frame_Shape);
        SB_Sim_Threshold->setObjectName(QStringLiteral("SB_Sim_Threshold"));
        SB_Sim_Threshold->setMaximum(1);
        SB_Sim_Threshold->setSingleStep(0.01);
        SB_Sim_Threshold->setValue(0.96);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, SB_Sim_Threshold);

        CKB_Calcenter = new QCheckBox(Frame_Shape);
        CKB_Calcenter->setObjectName(QStringLiteral("CKB_Calcenter"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, CKB_Calcenter);

        SB_Wh_Threshold = new QDoubleSpinBox(Frame_Shape);
        SB_Wh_Threshold->setObjectName(QStringLiteral("SB_Wh_Threshold"));
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
        page_4->setObjectName(QStringLiteral("page_4"));
        verticalLayout_10 = new QVBoxLayout(page_4);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        frame_8 = new QFrame(page_4);
        frame_8->setObjectName(QStringLiteral("frame_8"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        verticalLayout_11 = new QVBoxLayout(frame_8);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_12 = new QLabel(frame_8);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_7->addWidget(label_12);

        Canny_Threhold = new QSlider(frame_8);
        Canny_Threhold->setObjectName(QStringLiteral("Canny_Threhold"));
        Canny_Threhold->setMinimum(10);
        Canny_Threhold->setMaximum(250);
        Canny_Threhold->setValue(120);
        Canny_Threhold->setOrientation(Qt::Horizontal);

        horizontalLayout_7->addWidget(Canny_Threhold);

        Thre_Show = new QSpinBox(frame_8);
        Thre_Show->setObjectName(QStringLiteral("Thre_Show"));
        Thre_Show->setReadOnly(true);
        Thre_Show->setMinimum(1);
        Thre_Show->setMaximum(255);

        horizontalLayout_7->addWidget(Thre_Show);


        verticalLayout_11->addLayout(horizontalLayout_7);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        Btn_GetShape = new QPushButton(frame_8);
        Btn_GetShape->setObjectName(QStringLiteral("Btn_GetShape"));

        horizontalLayout_10->addWidget(Btn_GetShape);

        Btn_EditBitImg = new QPushButton(frame_8);
        Btn_EditBitImg->setObjectName(QStringLiteral("Btn_EditBitImg"));

        horizontalLayout_10->addWidget(Btn_EditBitImg);

        Btn_ReturnOriImg = new QPushButton(frame_8);
        Btn_ReturnOriImg->setObjectName(QStringLiteral("Btn_ReturnOriImg"));

        horizontalLayout_10->addWidget(Btn_ReturnOriImg);


        verticalLayout_11->addLayout(horizontalLayout_10);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_20 = new QLabel(frame_8);
        label_20->setObjectName(QStringLiteral("label_20"));

        horizontalLayout_12->addWidget(label_20);

        Enraser_Size = new QSlider(frame_8);
        Enraser_Size->setObjectName(QStringLiteral("Enraser_Size"));
        Enraser_Size->setMinimum(10);
        Enraser_Size->setMaximum(50);
        Enraser_Size->setValue(50);
        Enraser_Size->setOrientation(Qt::Horizontal);

        horizontalLayout_12->addWidget(Enraser_Size);

        Enraser_Show = new QSpinBox(frame_8);
        Enraser_Show->setObjectName(QStringLiteral("Enraser_Show"));
        Enraser_Show->setReadOnly(true);
        Enraser_Show->setMinimum(1);
        Enraser_Show->setMaximum(255);

        horizontalLayout_12->addWidget(Enraser_Show);


        verticalLayout_11->addLayout(horizontalLayout_12);

        Btn_Enraser = new QPushButton(frame_8);
        Btn_Enraser->setObjectName(QStringLiteral("Btn_Enraser"));
        Btn_Enraser->setStyleSheet(QLatin1String("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_Enraser->setCheckable(true);

        verticalLayout_11->addWidget(Btn_Enraser);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        EditMore = new QPushButton(frame_8);
        EditMore->setObjectName(QStringLiteral("EditMore"));
        EditMore->setCheckable(false);

        horizontalLayout_11->addWidget(EditMore);


        verticalLayout_11->addLayout(horizontalLayout_11);

        Widget_Tools = new QWidget(frame_8);
        Widget_Tools->setObjectName(QStringLiteral("Widget_Tools"));
        gridLayout_5 = new QGridLayout(Widget_Tools);
        gridLayout_5->setSpacing(0);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        Img_Next = new QPushButton(Widget_Tools);
        Img_Next->setObjectName(QStringLiteral("Img_Next"));

        gridLayout_5->addWidget(Img_Next, 3, 3, 1, 1);

        Img_Last = new QPushButton(Widget_Tools);
        Img_Last->setObjectName(QStringLiteral("Img_Last"));

        gridLayout_5->addWidget(Img_Last, 3, 1, 1, 1);

        Img_IndexLabel = new QLabel(Widget_Tools);
        Img_IndexLabel->setObjectName(QStringLiteral("Img_IndexLabel"));
        Img_IndexLabel->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(Img_IndexLabel, 3, 2, 1, 1);


        verticalLayout_11->addWidget(Widget_Tools);


        verticalLayout_10->addWidget(frame_8);

        frame_6 = new QFrame(page_4);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, -1, -1, -1);
        widget_4 = new QWidget(frame_6);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        verticalLayout_9 = new QVBoxLayout(widget_4);
        verticalLayout_9->setSpacing(2);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(9, 2, 2, 2);
        Type_Cross_2 = new QRadioButton(widget_4);
        Type_Cross_2->setObjectName(QStringLiteral("Type_Cross_2"));

        verticalLayout_9->addWidget(Type_Cross_2);

        Type_Angle_2 = new QRadioButton(widget_4);
        Type_Angle_2->setObjectName(QStringLiteral("Type_Angle_2"));

        verticalLayout_9->addWidget(Type_Angle_2);

        Type_Circle_2 = new QRadioButton(widget_4);
        Type_Circle_2->setObjectName(QStringLiteral("Type_Circle_2"));

        verticalLayout_9->addWidget(Type_Circle_2);

        Type_Relate_2 = new QRadioButton(widget_4);
        Type_Relate_2->setObjectName(QStringLiteral("Type_Relate_2"));

        verticalLayout_9->addWidget(Type_Relate_2);


        horizontalLayout_5->addWidget(widget_4);

        Btn_CalRefPoint = new QPushButton(frame_6);
        Btn_CalRefPoint->setObjectName(QStringLiteral("Btn_CalRefPoint"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Btn_CalRefPoint->sizePolicy().hasHeightForWidth());
        Btn_CalRefPoint->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(Btn_CalRefPoint);

        Btn_CalRefPoint2 = new QPushButton(frame_6);
        Btn_CalRefPoint2->setObjectName(QStringLiteral("Btn_CalRefPoint2"));
        sizePolicy1.setHeightForWidth(Btn_CalRefPoint2->sizePolicy().hasHeightForWidth());
        Btn_CalRefPoint2->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(Btn_CalRefPoint2);


        verticalLayout_10->addWidget(frame_6);

        frame_9 = new QFrame(page_4);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        gridLayout_6 = new QGridLayout(frame_9);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        topLeft = new QCheckBox(frame_9);
        topLeft->setObjectName(QStringLiteral("topLeft"));
        topLeft->setAutoExclusive(true);

        gridLayout_6->addWidget(topLeft, 0, 0, 1, 1);

        bottomLeft = new QCheckBox(frame_9);
        bottomLeft->setObjectName(QStringLiteral("bottomLeft"));
        bottomLeft->setAutoExclusive(true);

        gridLayout_6->addWidget(bottomLeft, 1, 0, 1, 1);

        topRight = new QCheckBox(frame_9);
        topRight->setObjectName(QStringLiteral("topRight"));
        topRight->setAutoExclusive(true);

        gridLayout_6->addWidget(topRight, 0, 1, 1, 1);

        bottomRight = new QCheckBox(frame_9);
        bottomRight->setObjectName(QStringLiteral("bottomRight"));
        bottomRight->setAutoExclusive(true);

        gridLayout_6->addWidget(bottomRight, 1, 1, 1, 1);

        crossCenter = new QCheckBox(frame_9);
        crossCenter->setObjectName(QStringLiteral("crossCenter"));
        crossCenter->setAutoExclusive(true);

        gridLayout_6->addWidget(crossCenter, 0, 2, 1, 1);


        verticalLayout_10->addWidget(frame_9);

        frame_7 = new QFrame(page_4);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        formLayout_3 = new QFormLayout(frame_7);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_14 = new QLabel(frame_7);
        label_14->setObjectName(QStringLiteral("label_14"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_14);

        DB_SimThreshold_SH = new QDoubleSpinBox(frame_7);
        DB_SimThreshold_SH->setObjectName(QStringLiteral("DB_SimThreshold_SH"));
        DB_SimThreshold_SH->setMinimum(-0.2);
        DB_SimThreshold_SH->setMaximum(1);
        DB_SimThreshold_SH->setSingleStep(0.05);
        DB_SimThreshold_SH->setValue(0.45);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, DB_SimThreshold_SH);

        Btn_MorePar = new QPushButton(frame_7);
        Btn_MorePar->setObjectName(QStringLiteral("Btn_MorePar"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, Btn_MorePar);


        verticalLayout_10->addWidget(frame_7);

        stackedWidget_Mode->addWidget(page_4);

        verticalLayout_4->addWidget(stackedWidget_Mode);

        Search_More_Frame = new QFrame(page);
        Search_More_Frame->setObjectName(QStringLiteral("Search_More_Frame"));
        Search_More_Frame->setFrameShape(QFrame::StyledPanel);
        Search_More_Frame->setFrameShadow(QFrame::Raised);
        gridLayout_7 = new QGridLayout(Search_More_Frame);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        Search_Blur_Size = new QSpinBox(Search_More_Frame);
        Search_Blur_Size->setObjectName(QStringLiteral("Search_Blur_Size"));
        Search_Blur_Size->setMinimum(10);
        Search_Blur_Size->setMaximum(100000);

        gridLayout_7->addWidget(Search_Blur_Size, 8, 1, 1, 1);

        label_15 = new QLabel(Search_More_Frame);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_15, 2, 0, 1, 1);

        Search_PyNum = new QSpinBox(Search_More_Frame);
        Search_PyNum->setObjectName(QStringLiteral("Search_PyNum"));
        Search_PyNum->setMinimum(1);
        Search_PyNum->setMaximum(6);

        gridLayout_7->addWidget(Search_PyNum, 4, 1, 1, 1);

        Search_Start1 = new QDoubleSpinBox(Search_More_Frame);
        Search_Start1->setObjectName(QStringLiteral("Search_Start1"));
        Search_Start1->setMinimum(-360);
        Search_Start1->setMaximum(360);
        Search_Start1->setSingleStep(1);
        Search_Start1->setValue(-10);

        gridLayout_7->addWidget(Search_Start1, 2, 1, 1, 1);

        Search_End1 = new QDoubleSpinBox(Search_More_Frame);
        Search_End1->setObjectName(QStringLiteral("Search_End1"));
        Search_End1->setMinimum(-360);
        Search_End1->setMaximum(360);
        Search_End1->setSingleStep(1);
        Search_End1->setValue(10);

        gridLayout_7->addWidget(Search_End1, 3, 1, 1, 1);

        label_17 = new QLabel(Search_More_Frame);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_17, 4, 0, 1, 1);

        label_19 = new QLabel(Search_More_Frame);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_19, 6, 0, 1, 1);

        label_16 = new QLabel(Search_More_Frame);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_16, 3, 0, 1, 1);

        Search_AngleNbh = new QSpinBox(Search_More_Frame);
        Search_AngleNbh->setObjectName(QStringLiteral("Search_AngleNbh"));
        Search_AngleNbh->setMinimum(2);

        gridLayout_7->addWidget(Search_AngleNbh, 4, 3, 1, 1);

        Search_RectNbh = new QSpinBox(Search_More_Frame);
        Search_RectNbh->setObjectName(QStringLiteral("Search_RectNbh"));
        Search_RectNbh->setMinimum(2);

        gridLayout_7->addWidget(Search_RectNbh, 6, 1, 1, 1);

        label_18 = new QLabel(Search_More_Frame);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_18, 4, 2, 1, 1);

        Search_Start3 = new QDoubleSpinBox(Search_More_Frame);
        Search_Start3->setObjectName(QStringLiteral("Search_Start3"));
        Search_Start3->setMinimum(-360);
        Search_Start3->setMaximum(360);
        Search_Start3->setSingleStep(1);
        Search_Start3->setValue(-10);

        gridLayout_7->addWidget(Search_Start3, 2, 3, 1, 1);

        Search_Start2 = new QDoubleSpinBox(Search_More_Frame);
        Search_Start2->setObjectName(QStringLiteral("Search_Start2"));
        Search_Start2->setMinimum(-360);
        Search_Start2->setMaximum(360);
        Search_Start2->setSingleStep(1);
        Search_Start2->setValue(-10);

        gridLayout_7->addWidget(Search_Start2, 2, 2, 1, 1);

        Search_Start4 = new QDoubleSpinBox(Search_More_Frame);
        Search_Start4->setObjectName(QStringLiteral("Search_Start4"));
        Search_Start4->setMinimum(-360);
        Search_Start4->setMaximum(360);
        Search_Start4->setSingleStep(1);
        Search_Start4->setValue(-10);

        gridLayout_7->addWidget(Search_Start4, 2, 4, 1, 1);

        Search_Enable1 = new QCheckBox(Search_More_Frame);
        Search_Enable1->setObjectName(QStringLiteral("Search_Enable1"));

        gridLayout_7->addWidget(Search_Enable1, 1, 1, 1, 1);

        Search_End3 = new QDoubleSpinBox(Search_More_Frame);
        Search_End3->setObjectName(QStringLiteral("Search_End3"));
        Search_End3->setMinimum(-360);
        Search_End3->setMaximum(360);
        Search_End3->setSingleStep(1);
        Search_End3->setValue(10);

        gridLayout_7->addWidget(Search_End3, 3, 3, 1, 1);

        Search_Enable3 = new QCheckBox(Search_More_Frame);
        Search_Enable3->setObjectName(QStringLiteral("Search_Enable3"));

        gridLayout_7->addWidget(Search_Enable3, 1, 3, 1, 1);

        Search_Enable2 = new QCheckBox(Search_More_Frame);
        Search_Enable2->setObjectName(QStringLiteral("Search_Enable2"));

        gridLayout_7->addWidget(Search_Enable2, 1, 2, 1, 1);

        Search_End2 = new QDoubleSpinBox(Search_More_Frame);
        Search_End2->setObjectName(QStringLiteral("Search_End2"));
        Search_End2->setMinimum(-360);
        Search_End2->setMaximum(360);
        Search_End2->setSingleStep(1);
        Search_End2->setValue(10);

        gridLayout_7->addWidget(Search_End2, 3, 2, 1, 1);

        Search_End4 = new QDoubleSpinBox(Search_More_Frame);
        Search_End4->setObjectName(QStringLiteral("Search_End4"));
        Search_End4->setMinimum(-360);
        Search_End4->setMaximum(360);
        Search_End4->setSingleStep(1);
        Search_End4->setValue(10);

        gridLayout_7->addWidget(Search_End4, 3, 4, 1, 1);

        Search_Enable4 = new QCheckBox(Search_More_Frame);
        Search_Enable4->setObjectName(QStringLiteral("Search_Enable4"));

        gridLayout_7->addWidget(Search_Enable4, 1, 4, 1, 1);

        label_13 = new QLabel(Search_More_Frame);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_13, 6, 2, 1, 1);

        Search_Step = new QDoubleSpinBox(Search_More_Frame);
        Search_Step->setObjectName(QStringLiteral("Search_Step"));
        Search_Step->setMinimum(0.1);
        Search_Step->setMaximum(10);
        Search_Step->setSingleStep(0.1);

        gridLayout_7->addWidget(Search_Step, 6, 3, 1, 1);

        Search_Erosion_Size = new QSpinBox(Search_More_Frame);
        Search_Erosion_Size->setObjectName(QStringLiteral("Search_Erosion_Size"));
        Search_Erosion_Size->setMinimum(1);
        Search_Erosion_Size->setMaximum(6);

        gridLayout_7->addWidget(Search_Erosion_Size, 8, 3, 1, 1);

        label_21 = new QLabel(Search_More_Frame);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_21, 8, 0, 1, 1);

        label_22 = new QLabel(Search_More_Frame);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_22, 8, 2, 1, 1);

        Btn_MatchParSure = new QPushButton(Search_More_Frame);
        Btn_MatchParSure->setObjectName(QStringLiteral("Btn_MatchParSure"));

        gridLayout_7->addWidget(Btn_MatchParSure, 8, 4, 1, 1);

        Search_Enable_Filter = new QCheckBox(Search_More_Frame);
        Search_Enable_Filter->setObjectName(QStringLiteral("Search_Enable_Filter"));

        gridLayout_7->addWidget(Search_Enable_Filter, 7, 1, 1, 1);

        Gray_Asisst_Enable = new QCheckBox(Search_More_Frame);
        Gray_Asisst_Enable->setObjectName(QStringLiteral("Gray_Asisst_Enable"));

        gridLayout_7->addWidget(Gray_Asisst_Enable, 4, 4, 1, 1);


        verticalLayout_4->addWidget(Search_More_Frame);

        frame_4 = new QFrame(page);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frame_4);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        Btn_Sure = new QPushButton(frame_4);
        Btn_Sure->setObjectName(QStringLiteral("Btn_Sure"));

        gridLayout_4->addWidget(Btn_Sure, 1, 0, 1, 1);

        Btn_Return = new QPushButton(frame_4);
        Btn_Return->setObjectName(QStringLiteral("Btn_Return"));

        gridLayout_4->addWidget(Btn_Return, 1, 1, 1, 1);

        Btn_GetTmp = new QPushButton(frame_4);
        Btn_GetTmp->setObjectName(QStringLiteral("Btn_GetTmp"));

        gridLayout_4->addWidget(Btn_GetTmp, 0, 0, 1, 1);

        Btn_Mode_Change = new QPushButton(frame_4);
        Btn_Mode_Change->setObjectName(QStringLiteral("Btn_Mode_Change"));

        gridLayout_4->addWidget(Btn_Mode_Change, 0, 1, 1, 1);


        verticalLayout_4->addWidget(frame_4);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        verticalLayout_3 = new QVBoxLayout(page_2);
        verticalLayout_3->setSpacing(8);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        Frame_Light = new QFrame(page_2);
        Frame_Light->setObjectName(QStringLiteral("Frame_Light"));
        Frame_Light->setFrameShape(QFrame::StyledPanel);
        Frame_Light->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(Frame_Light);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        Light1 = new QCheckBox(Frame_Light);
        Light1->setObjectName(QStringLiteral("Light1"));

        horizontalLayout_9->addWidget(Light1);

        Light2 = new QCheckBox(Frame_Light);
        Light2->setObjectName(QStringLiteral("Light2"));

        horizontalLayout_9->addWidget(Light2);

        Light3 = new QCheckBox(Frame_Light);
        Light3->setObjectName(QStringLiteral("Light3"));

        horizontalLayout_9->addWidget(Light3);


        verticalLayout_6->addLayout(horizontalLayout_9);


        verticalLayout_3->addWidget(Frame_Light);

        Frame_CameraPar = new QFrame(page_2);
        Frame_CameraPar->setObjectName(QStringLiteral("Frame_CameraPar"));
        Frame_CameraPar->setFrameShape(QFrame::StyledPanel);
        Frame_CameraPar->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(Frame_CameraPar);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        SB_CameraContra = new QSpinBox(Frame_CameraPar);
        SB_CameraContra->setObjectName(QStringLiteral("SB_CameraContra"));
        SB_CameraContra->setReadOnly(false);
        SB_CameraContra->setMinimum(1);
        SB_CameraContra->setMaximum(1000);

        gridLayout->addWidget(SB_CameraContra, 4, 2, 1, 1);

        SB_ExposeTime = new QSpinBox(Frame_CameraPar);
        SB_ExposeTime->setObjectName(QStringLiteral("SB_ExposeTime"));
        SB_ExposeTime->setReadOnly(false);
        SB_ExposeTime->setMinimum(1);
        SB_ExposeTime->setMaximum(800);

        gridLayout->addWidget(SB_ExposeTime, 3, 2, 1, 1);

        label_6 = new QLabel(Frame_CameraPar);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 3, 3, 1, 1);

        CameraGain = new QSlider(Frame_CameraPar);
        CameraGain->setObjectName(QStringLiteral("CameraGain"));
        CameraGain->setMinimum(1);
        CameraGain->setMaximum(500);
        CameraGain->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(CameraGain, 0, 1, 1, 1);

        label_5 = new QLabel(Frame_CameraPar);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        label_4 = new QLabel(Frame_CameraPar);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        SB_CameraGain = new QSpinBox(Frame_CameraPar);
        SB_CameraGain->setObjectName(QStringLiteral("SB_CameraGain"));
        SB_CameraGain->setReadOnly(false);
        SB_CameraGain->setMinimum(1);
        SB_CameraGain->setMaximum(500);

        gridLayout->addWidget(SB_CameraGain, 0, 2, 1, 1);

        label_3 = new QLabel(Frame_CameraPar);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        CameraContrast = new QSlider(Frame_CameraPar);
        CameraContrast->setObjectName(QStringLiteral("CameraContrast"));
        CameraContrast->setMinimum(1);
        CameraContrast->setMaximum(1000);
        CameraContrast->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(CameraContrast, 4, 1, 1, 1);

        CameraExposeTime = new QSlider(Frame_CameraPar);
        CameraExposeTime->setObjectName(QStringLiteral("CameraExposeTime"));
        CameraExposeTime->setMinimum(1);
        CameraExposeTime->setMaximum(800);
        CameraExposeTime->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(CameraExposeTime, 3, 1, 1, 1);

        CameraGamma = new QSlider(Frame_CameraPar);
        CameraGamma->setObjectName(QStringLiteral("CameraGamma"));
        CameraGamma->setMinimum(1);
        CameraGamma->setMaximum(1000);
        CameraGamma->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(CameraGamma, 5, 1, 1, 1);

        label_11 = new QLabel(Frame_CameraPar);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_11, 5, 0, 1, 1);

        SB_CameraGamma = new QSpinBox(Frame_CameraPar);
        SB_CameraGamma->setObjectName(QStringLiteral("SB_CameraGamma"));
        SB_CameraGamma->setReadOnly(false);
        SB_CameraGamma->setMinimum(1);
        SB_CameraGamma->setMaximum(1000);

        gridLayout->addWidget(SB_CameraGamma, 5, 2, 1, 1);


        verticalLayout_5->addLayout(gridLayout);


        verticalLayout_3->addWidget(Frame_CameraPar);

        frame_10 = new QFrame(page_2);
        frame_10->setObjectName(QStringLiteral("frame_10"));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        horizontalLayout_14 = new QHBoxLayout(frame_10);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_23 = new QLabel(frame_10);
        label_23->setObjectName(QStringLiteral("label_23"));

        horizontalLayout_14->addWidget(label_23);

        exposeCount = new QSpinBox(frame_10);
        exposeCount->setObjectName(QStringLiteral("exposeCount"));
        exposeCount->setMaximum(10);
        exposeCount->setValue(1);

        horizontalLayout_14->addWidget(exposeCount);

        label_24 = new QLabel(frame_10);
        label_24->setObjectName(QStringLiteral("label_24"));

        horizontalLayout_14->addWidget(label_24);

        exposeValue = new QDoubleSpinBox(frame_10);
        exposeValue->setObjectName(QStringLiteral("exposeValue"));
        exposeValue->setDecimals(1);
        exposeValue->setMaximum(50);
        exposeValue->setValue(2);

        horizontalLayout_14->addWidget(exposeValue);


        verticalLayout_3->addWidget(frame_10);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        label = new QLabel(page_2);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_3->addWidget(label);

        frame_5 = new QFrame(page_2);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_5);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(4, -1, -1, -1);
        AreaSelPar_Save = new QPushButton(frame_5);
        AreaSelPar_Save->setObjectName(QStringLiteral("AreaSelPar_Save"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(AreaSelPar_Save->sizePolicy().hasHeightForWidth());
        AreaSelPar_Save->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(AreaSelPar_Save, 0, 2, 1, 1);

        Template_Cancel = new QPushButton(frame_5);
        Template_Cancel->setObjectName(QStringLiteral("Template_Cancel"));
        sizePolicy2.setHeightForWidth(Template_Cancel->sizePolicy().hasHeightForWidth());
        Template_Cancel->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(Template_Cancel, 0, 0, 1, 1);

        LightPar_Save = new QPushButton(frame_5);
        LightPar_Save->setObjectName(QStringLiteral("LightPar_Save"));
        sizePolicy2.setHeightForWidth(LightPar_Save->sizePolicy().hasHeightForWidth());
        LightPar_Save->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(LightPar_Save, 0, 1, 1, 1);

        MarkPosSaved = new QPushButton(frame_5);
        MarkPosSaved->setObjectName(QStringLiteral("MarkPosSaved"));

        gridLayout_3->addWidget(MarkPosSaved, 1, 1, 1, 1);

        Template_Sure = new QPushButton(frame_5);
        Template_Sure->setObjectName(QStringLiteral("Template_Sure"));
        sizePolicy2.setHeightForWidth(Template_Sure->sizePolicy().hasHeightForWidth());
        Template_Sure->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(Template_Sure, 1, 2, 1, 1);


        verticalLayout_3->addWidget(frame_5);

        frame_2 = new QFrame(page_2);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy3);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_2);
        horizontalLayout_4->setSpacing(1);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(4, 4, 0, 4);
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_2);

        Jog_speed = new QSlider(frame_2);
        Jog_speed->setObjectName(QStringLiteral("Jog_speed"));
        Jog_speed->setMaximum(1500);
        Jog_speed->setOrientation(Qt::Horizontal);

        verticalLayout_7->addWidget(Jog_speed);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        Btn_Cont = new QPushButton(frame_2);
        Btn_Cont->setObjectName(QStringLiteral("Btn_Cont"));
        Btn_Cont->setStyleSheet(QLatin1String("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_Cont->setCheckable(true);
        Btn_Cont->setChecked(true);
        Btn_Cont->setAutoExclusive(true);

        horizontalLayout_6->addWidget(Btn_Cont);

        Btn_Handle = new QPushButton(frame_2);
        Btn_Handle->setObjectName(QStringLiteral("Btn_Handle"));
        Btn_Handle->setStyleSheet(QLatin1String("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_Handle->setCheckable(true);
        Btn_Handle->setAutoExclusive(true);

        horizontalLayout_6->addWidget(Btn_Handle);

        Btn_1 = new QPushButton(frame_2);
        Btn_1->setObjectName(QStringLiteral("Btn_1"));
        Btn_1->setMinimumSize(QSize(0, 0));
        Btn_1->setStyleSheet(QLatin1String("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_1->setCheckable(true);
        Btn_1->setAutoExclusive(true);

        horizontalLayout_6->addWidget(Btn_1);


        verticalLayout_7->addLayout(horizontalLayout_6);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        Btn_001 = new QPushButton(frame_2);
        Btn_001->setObjectName(QStringLiteral("Btn_001"));
        Btn_001->setMinimumSize(QSize(0, 0));
        Btn_001->setStyleSheet(QLatin1String("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_001->setCheckable(true);
        Btn_001->setAutoExclusive(true);

        horizontalLayout_8->addWidget(Btn_001);

        Btn_01 = new QPushButton(frame_2);
        Btn_01->setObjectName(QStringLiteral("Btn_01"));
        Btn_01->setMinimumSize(QSize(0, 0));
        Btn_01->setStyleSheet(QLatin1String("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_01->setCheckable(true);
        Btn_01->setAutoExclusive(true);

        horizontalLayout_8->addWidget(Btn_01);

        Btn_0001 = new QPushButton(frame_2);
        Btn_0001->setObjectName(QStringLiteral("Btn_0001"));
        Btn_0001->setMinimumSize(QSize(0, 0));
        Btn_0001->setStyleSheet(QLatin1String("QPushButton:checked{\n"
"	background-color:rgb(109, 255, 42)\n"
"}"));
        Btn_0001->setCheckable(true);
        Btn_0001->setAutoExclusive(true);

        horizontalLayout_8->addWidget(Btn_0001);


        verticalLayout_7->addLayout(horizontalLayout_8);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        Btn_Zplus = new QPushButton(frame_2);
        Btn_Zplus->setObjectName(QStringLiteral("Btn_Zplus"));

        gridLayout_2->addWidget(Btn_Zplus, 0, 0, 1, 1);

        Btn_Yplus = new QPushButton(frame_2);
        Btn_Yplus->setObjectName(QStringLiteral("Btn_Yplus"));

        gridLayout_2->addWidget(Btn_Yplus, 0, 1, 1, 1);

        pushButton_10 = new QPushButton(frame_2);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));

        gridLayout_2->addWidget(pushButton_10, 2, 1, 1, 1);

        Btn_Xminus = new QPushButton(frame_2);
        Btn_Xminus->setObjectName(QStringLiteral("Btn_Xminus"));

        gridLayout_2->addWidget(Btn_Xminus, 2, 0, 1, 1);

        Btn_Zminus = new QPushButton(frame_2);
        Btn_Zminus->setObjectName(QStringLiteral("Btn_Zminus"));

        gridLayout_2->addWidget(Btn_Zminus, 3, 0, 1, 1);

        Btn_Yminus = new QPushButton(frame_2);
        Btn_Yminus->setObjectName(QStringLiteral("Btn_Yminus"));

        gridLayout_2->addWidget(Btn_Yminus, 3, 1, 1, 1);

        Btn_Xplus = new QPushButton(frame_2);
        Btn_Xplus->setObjectName(QStringLiteral("Btn_Xplus"));

        gridLayout_2->addWidget(Btn_Xplus, 2, 2, 1, 1);

        pushButton_12 = new QPushButton(frame_2);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));

        gridLayout_2->addWidget(pushButton_12, 0, 2, 1, 1);

        pushButton_16 = new QPushButton(frame_2);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));

        gridLayout_2->addWidget(pushButton_16, 3, 2, 1, 1);


        verticalLayout_7->addLayout(gridLayout_2);


        horizontalLayout_4->addLayout(verticalLayout_7);


        verticalLayout_3->addWidget(frame_2);

        stackedWidget->addWidget(page_2);

        horizontalLayout_3->addWidget(stackedWidget);


        retranslateUi(CCD_MarkEditor);

        stackedWidget->setCurrentIndex(1);
        stackedWidget_Mode->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(CCD_MarkEditor);
    } // setupUi

    void retranslateUi(QDialog *CCD_MarkEditor)
    {
        CCD_MarkEditor->setWindowTitle(QApplication::translate("CCD_MarkEditor", "Dialog", Q_NULLPTR));
        Mode_Auto->setText(QApplication::translate("CCD_MarkEditor", "\350\207\252\345\212\250  ", Q_NULLPTR));
        Mode_Man->setText(QApplication::translate("CCD_MarkEditor", "\346\211\213\345\212\250  ", Q_NULLPTR));
        Btn_Identify->setText(QApplication::translate("CCD_MarkEditor", "\350\257\206\345\210\253", Q_NULLPTR));
        Type_Cross->setText(QApplication::translate("CCD_MarkEditor", "\345\215\201\345\255\227", Q_NULLPTR));
        Type_Angle->setText(QApplication::translate("CCD_MarkEditor", "\347\233\264\350\247\222", Q_NULLPTR));
        Type_Circle->setText(QApplication::translate("CCD_MarkEditor", "\345\234\206\345\275\242", Q_NULLPTR));
        Type_Relate->setText(QApplication::translate("CCD_MarkEditor", "\345\217\202\350\200\203\347\202\271", Q_NULLPTR));
        Type_Shape->setText(QApplication::translate("CCD_MarkEditor", "\350\275\256\345\273\223", Q_NULLPTR));
        label_10->setText(QApplication::translate("CCD_MarkEditor", "\350\275\256\345\273\223\345\256\275\345\272\246", Q_NULLPTR));
        label_7->setText(QApplication::translate("CCD_MarkEditor", "\350\275\256\345\273\223\346\220\234\347\264\242\350\267\235\347\246\273", Q_NULLPTR));
        label_8->setText(QApplication::translate("CCD_MarkEditor", "\351\225\277\345\256\275\350\257\257\345\267\256\345\205\201\350\256\270", Q_NULLPTR));
        label_9->setText(QApplication::translate("CCD_MarkEditor", "\347\233\270\344\274\274\345\272\246", Q_NULLPTR));
        CKB_Calcenter->setText(QApplication::translate("CCD_MarkEditor", "\350\256\241\347\256\227\345\234\206\345\277\203", Q_NULLPTR));
        label_12->setText(QApplication::translate("CCD_MarkEditor", "\351\230\210\345\200\274", Q_NULLPTR));
        Btn_GetShape->setText(QApplication::translate("CCD_MarkEditor", "\350\216\267\345\217\226\345\275\242\347\212\266", Q_NULLPTR));
        Btn_EditBitImg->setText(QApplication::translate("CCD_MarkEditor", "\347\274\226\350\276\221\345\275\242\347\212\266", Q_NULLPTR));
        Btn_ReturnOriImg->setText(QApplication::translate("CCD_MarkEditor", "\345\216\237\345\233\276", Q_NULLPTR));
        label_20->setText(QApplication::translate("CCD_MarkEditor", "\346\251\241\347\232\256\346\223\246\345\244\247\345\260\217", Q_NULLPTR));
        Btn_Enraser->setText(QApplication::translate("CCD_MarkEditor", "\346\251\241\347\232\256\346\223\246", Q_NULLPTR));
        EditMore->setText(QApplication::translate("CCD_MarkEditor", "\347\241\256\345\256\232\345\275\242\347\212\266", Q_NULLPTR));
        Img_Next->setText(QApplication::translate("CCD_MarkEditor", ">>>", Q_NULLPTR));
        Img_Last->setText(QApplication::translate("CCD_MarkEditor", "<<<", Q_NULLPTR));
        Img_IndexLabel->setText(QApplication::translate("CCD_MarkEditor", "1/4", Q_NULLPTR));
        Type_Cross_2->setText(QApplication::translate("CCD_MarkEditor", "\344\272\244\347\202\271", Q_NULLPTR));
        Type_Angle_2->setText(QApplication::translate("CCD_MarkEditor", "\347\233\264\350\247\222", Q_NULLPTR));
        Type_Circle_2->setText(QApplication::translate("CCD_MarkEditor", "\345\234\206\345\275\242", Q_NULLPTR));
        Type_Relate_2->setText(QApplication::translate("CCD_MarkEditor", "\346\214\207\345\256\232", Q_NULLPTR));
        Btn_CalRefPoint->setText(QApplication::translate("CCD_MarkEditor", "\350\256\241\347\256\227\345\217\202\350\200\203\347\202\2711", Q_NULLPTR));
        Btn_CalRefPoint2->setText(QApplication::translate("CCD_MarkEditor", "\350\256\241\347\256\227\345\217\202\350\200\203\347\202\2712", Q_NULLPTR));
        topLeft->setText(QApplication::translate("CCD_MarkEditor", "\345\267\246\344\270\212", Q_NULLPTR));
        bottomLeft->setText(QApplication::translate("CCD_MarkEditor", "\345\267\246\344\270\213", Q_NULLPTR));
        topRight->setText(QApplication::translate("CCD_MarkEditor", "\345\217\263\344\270\212", Q_NULLPTR));
        bottomRight->setText(QApplication::translate("CCD_MarkEditor", "\345\217\263\344\270\213", Q_NULLPTR));
        crossCenter->setText(QApplication::translate("CCD_MarkEditor", "\345\215\201\345\255\227", Q_NULLPTR));
        label_14->setText(QApplication::translate("CCD_MarkEditor", "\347\233\270\344\274\274\345\272\246", Q_NULLPTR));
        Btn_MorePar->setText(QApplication::translate("CCD_MarkEditor", "\351\253\230\347\272\247\345\217\202\346\225\260", Q_NULLPTR));
        label_15->setText(QApplication::translate("CCD_MarkEditor", "\350\265\267\345\247\213\350\247\222\345\272\246", Q_NULLPTR));
        label_17->setText(QApplication::translate("CCD_MarkEditor", "\346\220\234\347\264\242\345\261\202\346\225\260", Q_NULLPTR));
        label_19->setText(QApplication::translate("CCD_MarkEditor", "\345\271\263\347\247\273\345\217\202\346\225\260", Q_NULLPTR));
        label_16->setText(QApplication::translate("CCD_MarkEditor", "\347\273\223\346\235\237\350\247\222\345\272\246", Q_NULLPTR));
        label_18->setText(QApplication::translate("CCD_MarkEditor", "\350\247\222\345\272\246\345\217\202\346\225\260", Q_NULLPTR));
        Search_Enable1->setText(QApplication::translate("CCD_MarkEditor", "\350\214\203\345\233\2641", Q_NULLPTR));
        Search_Enable3->setText(QApplication::translate("CCD_MarkEditor", "\350\214\203\345\233\2643", Q_NULLPTR));
        Search_Enable2->setText(QApplication::translate("CCD_MarkEditor", "\350\214\203\345\233\2642", Q_NULLPTR));
        Search_Enable4->setText(QApplication::translate("CCD_MarkEditor", "\350\214\203\345\233\2644", Q_NULLPTR));
        label_13->setText(QApplication::translate("CCD_MarkEditor", "\350\247\222\345\272\246\346\255\245\351\225\277", Q_NULLPTR));
        label_21->setText(QApplication::translate("CCD_MarkEditor", "\346\234\200\345\260\217\351\227\264\350\267\235", Q_NULLPTR));
        label_22->setText(QApplication::translate("CCD_MarkEditor", "\347\233\256\346\240\207\344\270\252\346\225\260", Q_NULLPTR));
        Btn_MatchParSure->setText(QApplication::translate("CCD_MarkEditor", "\347\241\256\345\256\232", Q_NULLPTR));
        Search_Enable_Filter->setText(QApplication::translate("CCD_MarkEditor", "\344\275\277\350\203\275\346\273\244\346\263\242", Q_NULLPTR));
        Gray_Asisst_Enable->setText(QApplication::translate("CCD_MarkEditor", "\347\201\260\345\272\246\350\276\205\345\212\251", Q_NULLPTR));
        Btn_Sure->setText(QApplication::translate("CCD_MarkEditor", "\347\241\256\345\256\232\347\211\271\345\276\201", Q_NULLPTR));
        Btn_Return->setText(QApplication::translate("CCD_MarkEditor", "\346\224\276\345\274\203\350\277\224\345\233\236", Q_NULLPTR));
        Btn_GetTmp->setText(QApplication::translate("CCD_MarkEditor", "\351\207\215\346\226\260\350\216\267\345\217\226\346\250\241\346\235\277", Q_NULLPTR));
        Btn_Mode_Change->setText(QApplication::translate("CCD_MarkEditor", "\347\201\260\345\272\246\346\250\241\345\274\217", Q_NULLPTR));
        Light1->setText(QApplication::translate("CCD_MarkEditor", "\345\205\211\346\272\2201", Q_NULLPTR));
        Light2->setText(QApplication::translate("CCD_MarkEditor", "\345\205\211\346\272\2202", Q_NULLPTR));
        Light3->setText(QApplication::translate("CCD_MarkEditor", "\345\205\211\346\272\2203", Q_NULLPTR));
        label_6->setText(QApplication::translate("CCD_MarkEditor", "ms", Q_NULLPTR));
        label_5->setText(QApplication::translate("CCD_MarkEditor", "\345\257\271\346\257\224\345\272\246", Q_NULLPTR));
        label_4->setText(QApplication::translate("CCD_MarkEditor", "\346\233\235\345\205\211\346\227\266\351\227\264", Q_NULLPTR));
        label_3->setText(QApplication::translate("CCD_MarkEditor", "\345\242\236\347\233\212", Q_NULLPTR));
        label_11->setText(QApplication::translate("CCD_MarkEditor", "\344\274\275\351\251\254\345\200\274", Q_NULLPTR));
        label_23->setText(QApplication::translate("CCD_MarkEditor", "\350\207\252\345\212\250\346\233\235\345\205\211\346\254\241\346\225\260", Q_NULLPTR));
        label_24->setText(QApplication::translate("CCD_MarkEditor", "\350\207\252\345\212\250\346\233\235\345\205\211\346\227\266\351\227\264", Q_NULLPTR));
        label->setText(QApplication::translate("CCD_MarkEditor", "       \350\257\267\346\213\226\345\212\250\350\223\235\350\211\262\347\232\204\346\241\206\346\235\245\351\200\211\346\213\251\346\250\241\346\235\277\357\274\214\n"
"\345\246\202\346\227\240\351\234\200\346\233\264\346\215\242\346\250\241\346\235\277\357\274\214\347\233\264\346\216\245\347\202\271\345\207\273\344\275\277\347\224\250\n"
"\344\270\212\344\270\200\346\254\241\350\277\233\345\205\245\347\274\226\350\276\221\347\211\271\345\276\201\347\225\214\351\235\242", Q_NULLPTR));
        AreaSelPar_Save->setText(QApplication::translate("CCD_MarkEditor", "\344\277\235\345\255\230\346\220\234\347\264\242\350\214\203\345\233\264", Q_NULLPTR));
        Template_Cancel->setText(QApplication::translate("CCD_MarkEditor", "\344\275\277\347\224\250\344\270\212\344\270\200\346\254\241", Q_NULLPTR));
        LightPar_Save->setText(QApplication::translate("CCD_MarkEditor", "\344\277\235\345\255\230\345\205\211\347\205\247", Q_NULLPTR));
        MarkPosSaved->setText(QApplication::translate("CCD_MarkEditor", "\344\277\235\345\255\230\346\213\215\347\205\247\344\275\215", Q_NULLPTR));
        Template_Sure->setText(QApplication::translate("CCD_MarkEditor", "\347\241\256\345\256\232\346\250\241\346\235\277", Q_NULLPTR));
        label_2->setText(QApplication::translate("CCD_MarkEditor", "  \350\277\236\347\273\255\351\200\237\345\272\246", Q_NULLPTR));
        Btn_Cont->setText(QApplication::translate("CCD_MarkEditor", "\350\277\236\347\273\255", Q_NULLPTR));
        Btn_Handle->setText(QApplication::translate("CCD_MarkEditor", "\346\211\213\350\275\256", Q_NULLPTR));
        Btn_1->setText(QApplication::translate("CCD_MarkEditor", "1mm", Q_NULLPTR));
        Btn_001->setText(QApplication::translate("CCD_MarkEditor", "0.01mm", Q_NULLPTR));
        Btn_01->setText(QApplication::translate("CCD_MarkEditor", "0.1mm", Q_NULLPTR));
        Btn_0001->setText(QApplication::translate("CCD_MarkEditor", "0.001mm", Q_NULLPTR));
        Btn_Zplus->setText(QApplication::translate("CCD_MarkEditor", "Z+", Q_NULLPTR));
        Btn_Yplus->setText(QApplication::translate("CCD_MarkEditor", "Y+", Q_NULLPTR));
        pushButton_10->setText(QString());
        Btn_Xminus->setText(QApplication::translate("CCD_MarkEditor", "X-", Q_NULLPTR));
        Btn_Zminus->setText(QApplication::translate("CCD_MarkEditor", "Z-", Q_NULLPTR));
        Btn_Yminus->setText(QApplication::translate("CCD_MarkEditor", "Y-", Q_NULLPTR));
        Btn_Xplus->setText(QApplication::translate("CCD_MarkEditor", "X+", Q_NULLPTR));
        pushButton_12->setText(QString());
        pushButton_16->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CCD_MarkEditor: public Ui_CCD_MarkEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_EDITWIDGET_H
