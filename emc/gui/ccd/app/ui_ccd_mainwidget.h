/********************************************************************************
** Form generated from reading UI file 'ccd_mainwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_MAINWIDGET_H
#define UI_CCD_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCD_MainWidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_11;
    QFrame *MainFrame1;
    QFrame *MainFrame2;
    QFrame *MainFrame4;
    QVBoxLayout *verticalLayout_5;
    QFrame *MainFrame3;
    QVBoxLayout *verticalLayout_9;
    QWidget *widget;
    QVBoxLayout *verticalLayout_10;
    QWidget *WidgetHide;
    QGridLayout *gridLayout_2;
    QFrame *frame_6;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_15;
    QPushButton *MarkDownLoadB;
    QPushButton *IdDownLoadB;
    QFrame *ViewDownB;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_13;
    QPushButton *MarkDownLoadA;
    QPushButton *IdDownLoadA;
    QFrame *ViewDownA;
    QFrame *frame_9;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_11;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_18;
    QPushButton *MarkTop2;
    QPushButton *IdTop2;
    QFrame *ViewTop2;
    QPushButton *Btn_UpLoadRefA;
    QPushButton *Btn_UpLoadRefB;
    QPushButton *Btn_DownLoadRefA;
    QPushButton *Btn_DownLoadRefB;
    QFrame *frame_5;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_14;
    QPushButton *MarkTop1;
    QPushButton *IdTop1;
    QFrame *ViewTop1;
    QPushButton *Btn_CCDPar3;
    QGridLayout *gridLayout;
    QCheckBox *Head_B;
    QLabel *label_7;
    QPushButton *Btn_Connect_cfg;
    QSpinBox *SourceIndex;
    QSpinBox *SourceSubIndex;
    QCheckBox *Head_A;
    QSpinBox *DstSubIndex;
    QPushButton *pushButton;
    QLabel *label_4;
    QSpinBox *DstIndex;
    QLabel *label_5;
    QLabel *label_9;
    QLabel *label_12;
    QPushButton *btn_runtest;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QFrame *frame_2;
    QGridLayout *gridLayout_4;
    QPushButton *Btn_CCDPar2;
    QFrame *frame_10;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_19;
    QPushButton *MarkUpLoadB;
    QPushButton *IdUpLoadB;
    QFrame *ViewUpB;
    QPushButton *Btn_CCDPar1;
    QFrame *frame1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_16;
    QPushButton *MarkUpLoadA;
    QPushButton *IdUpLoadA;
    QFrame *ViewUpA;
    QPushButton *Btn_CalSim;
    QPushButton *SetIdPosition2;
    QPushButton *SetIdPosition1;
    QPushButton *StartIdentify;
    QPlainTextEdit *Dis_Log;
    QFrame *frame_7;
    QGridLayout *gridLayout_5;
    QSpacerItem *verticalSpacer;
    QPushButton *Btn_History;
    QPushButton *Btn_ClearLog;
    QPushButton *Btn_SaveImg;
    QCheckBox *Btn_EnableHistory;
    QSpinBox *camera_index;
    QPushButton *Btn_CommonPar;
    QCheckBox *Ck_RealTimeImg;
    QPushButton *Btn_SavePattern;
    QPushButton *Btn_LoadPattern;
    QPushButton *Btn_LoadImg;

    void setupUi(QWidget *CCD_MainWidget)
    {
        if (CCD_MainWidget->objectName().isEmpty())
            CCD_MainWidget->setObjectName(QStringLiteral("CCD_MainWidget"));
        CCD_MainWidget->resize(969, 714);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(10);
        sizePolicy.setVerticalStretch(8);
        sizePolicy.setHeightForWidth(CCD_MainWidget->sizePolicy().hasHeightForWidth());
        CCD_MainWidget->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(CCD_MainWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        MainFrame1 = new QFrame(CCD_MainWidget);
        MainFrame1->setObjectName(QStringLiteral("MainFrame1"));
        MainFrame1->setStyleSheet(QStringLiteral(""));
        MainFrame1->setFrameShape(QFrame::StyledPanel);
        MainFrame1->setFrameShadow(QFrame::Raised);

        verticalLayout_11->addWidget(MainFrame1);

        MainFrame2 = new QFrame(CCD_MainWidget);
        MainFrame2->setObjectName(QStringLiteral("MainFrame2"));
        MainFrame2->setStyleSheet(QStringLiteral(""));
        MainFrame2->setFrameShape(QFrame::StyledPanel);
        MainFrame2->setFrameShadow(QFrame::Raised);

        verticalLayout_11->addWidget(MainFrame2);

        MainFrame4 = new QFrame(CCD_MainWidget);
        MainFrame4->setObjectName(QStringLiteral("MainFrame4"));
        MainFrame4->setFrameShape(QFrame::StyledPanel);
        MainFrame4->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(MainFrame4);
        verticalLayout_5->setSpacing(2);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);

        verticalLayout_11->addWidget(MainFrame4);

        MainFrame3 = new QFrame(CCD_MainWidget);
        MainFrame3->setObjectName(QStringLiteral("MainFrame3"));
        MainFrame3->setFrameShape(QFrame::StyledPanel);
        MainFrame3->setFrameShadow(QFrame::Raised);
        verticalLayout_9 = new QVBoxLayout(MainFrame3);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        widget = new QWidget(MainFrame3);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout_10 = new QVBoxLayout(widget);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        WidgetHide = new QWidget(widget);
        WidgetHide->setObjectName(QStringLiteral("WidgetHide"));
        gridLayout_2 = new QGridLayout(WidgetHide);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        frame_6 = new QFrame(WidgetHide);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        verticalLayout_8 = new QVBoxLayout(frame_6);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, -1, -1, -1);
        label_3 = new QLabel(frame_6);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_8->addWidget(label_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        MarkDownLoadB = new QPushButton(frame_6);
        MarkDownLoadB->setObjectName(QStringLiteral("MarkDownLoadB"));

        verticalLayout_15->addWidget(MarkDownLoadB);

        IdDownLoadB = new QPushButton(frame_6);
        IdDownLoadB->setObjectName(QStringLiteral("IdDownLoadB"));

        verticalLayout_15->addWidget(IdDownLoadB);


        horizontalLayout_6->addLayout(verticalLayout_15);

        ViewDownB = new QFrame(frame_6);
        ViewDownB->setObjectName(QStringLiteral("ViewDownB"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(12);
        sizePolicy1.setVerticalStretch(10);
        sizePolicy1.setHeightForWidth(ViewDownB->sizePolicy().hasHeightForWidth());
        ViewDownB->setSizePolicy(sizePolicy1);
        ViewDownB->setMinimumSize(QSize(34, 27));
        ViewDownB->setMaximumSize(QSize(100, 80));
        ViewDownB->setFrameShape(QFrame::Box);
        ViewDownB->setFrameShadow(QFrame::Raised);

        horizontalLayout_6->addWidget(ViewDownB);


        verticalLayout_8->addLayout(horizontalLayout_6);


        gridLayout_2->addWidget(frame_6, 3, 1, 1, 1);

        frame_4 = new QFrame(WidgetHide);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, -1, -1, -1);
        label_6 = new QLabel(frame_4);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_4->addWidget(label_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        MarkDownLoadA = new QPushButton(frame_4);
        MarkDownLoadA->setObjectName(QStringLiteral("MarkDownLoadA"));

        verticalLayout_13->addWidget(MarkDownLoadA);

        IdDownLoadA = new QPushButton(frame_4);
        IdDownLoadA->setObjectName(QStringLiteral("IdDownLoadA"));

        verticalLayout_13->addWidget(IdDownLoadA);


        horizontalLayout_7->addLayout(verticalLayout_13);

        ViewDownA = new QFrame(frame_4);
        ViewDownA->setObjectName(QStringLiteral("ViewDownA"));
        sizePolicy1.setHeightForWidth(ViewDownA->sizePolicy().hasHeightForWidth());
        ViewDownA->setSizePolicy(sizePolicy1);
        ViewDownA->setMinimumSize(QSize(34, 27));
        ViewDownA->setMaximumSize(QSize(100, 80));
        ViewDownA->setFrameShape(QFrame::Box);
        ViewDownA->setFrameShadow(QFrame::Raised);

        horizontalLayout_7->addWidget(ViewDownA);


        verticalLayout_4->addLayout(horizontalLayout_7);


        gridLayout_2->addWidget(frame_4, 2, 1, 1, 1);

        frame_9 = new QFrame(WidgetHide);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        verticalLayout_7 = new QVBoxLayout(frame_9);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, -1, -1, -1);
        label_11 = new QLabel(frame_9);
        label_11->setObjectName(QStringLiteral("label_11"));

        verticalLayout_7->addWidget(label_11);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        MarkTop2 = new QPushButton(frame_9);
        MarkTop2->setObjectName(QStringLiteral("MarkTop2"));

        verticalLayout_18->addWidget(MarkTop2);

        IdTop2 = new QPushButton(frame_9);
        IdTop2->setObjectName(QStringLiteral("IdTop2"));

        verticalLayout_18->addWidget(IdTop2);


        horizontalLayout_5->addLayout(verticalLayout_18);

        ViewTop2 = new QFrame(frame_9);
        ViewTop2->setObjectName(QStringLiteral("ViewTop2"));
        sizePolicy1.setHeightForWidth(ViewTop2->sizePolicy().hasHeightForWidth());
        ViewTop2->setSizePolicy(sizePolicy1);
        ViewTop2->setMinimumSize(QSize(34, 27));
        ViewTop2->setMaximumSize(QSize(100, 80));
        ViewTop2->setFrameShape(QFrame::Box);
        ViewTop2->setFrameShadow(QFrame::Raised);

        horizontalLayout_5->addWidget(ViewTop2);


        verticalLayout_7->addLayout(horizontalLayout_5);


        gridLayout_2->addWidget(frame_9, 4, 1, 1, 1);

        Btn_UpLoadRefA = new QPushButton(WidgetHide);
        Btn_UpLoadRefA->setObjectName(QStringLiteral("Btn_UpLoadRefA"));

        gridLayout_2->addWidget(Btn_UpLoadRefA, 0, 0, 1, 1);

        Btn_UpLoadRefB = new QPushButton(WidgetHide);
        Btn_UpLoadRefB->setObjectName(QStringLiteral("Btn_UpLoadRefB"));

        gridLayout_2->addWidget(Btn_UpLoadRefB, 4, 0, 1, 1);

        Btn_DownLoadRefA = new QPushButton(WidgetHide);
        Btn_DownLoadRefA->setObjectName(QStringLiteral("Btn_DownLoadRefA"));

        gridLayout_2->addWidget(Btn_DownLoadRefA, 0, 1, 1, 1);

        Btn_DownLoadRefB = new QPushButton(WidgetHide);
        Btn_DownLoadRefB->setObjectName(QStringLiteral("Btn_DownLoadRefB"));

        gridLayout_2->addWidget(Btn_DownLoadRefB, 1, 1, 1, 1);

        frame_5 = new QFrame(WidgetHide);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(frame_5);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, -1, -1, -1);
        label_10 = new QLabel(frame_5);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout_6->addWidget(label_10);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        MarkTop1 = new QPushButton(frame_5);
        MarkTop1->setObjectName(QStringLiteral("MarkTop1"));

        verticalLayout_14->addWidget(MarkTop1);

        IdTop1 = new QPushButton(frame_5);
        IdTop1->setObjectName(QStringLiteral("IdTop1"));

        verticalLayout_14->addWidget(IdTop1);


        horizontalLayout_3->addLayout(verticalLayout_14);

        ViewTop1 = new QFrame(frame_5);
        ViewTop1->setObjectName(QStringLiteral("ViewTop1"));
        sizePolicy1.setHeightForWidth(ViewTop1->sizePolicy().hasHeightForWidth());
        ViewTop1->setSizePolicy(sizePolicy1);
        ViewTop1->setMinimumSize(QSize(34, 27));
        ViewTop1->setMaximumSize(QSize(100, 80));
        ViewTop1->setFrameShape(QFrame::Box);
        ViewTop1->setFrameShadow(QFrame::Raised);

        horizontalLayout_3->addWidget(ViewTop1);


        verticalLayout_6->addLayout(horizontalLayout_3);


        gridLayout_2->addWidget(frame_5, 2, 0, 1, 1);

        Btn_CCDPar3 = new QPushButton(WidgetHide);
        Btn_CCDPar3->setObjectName(QStringLiteral("Btn_CCDPar3"));

        gridLayout_2->addWidget(Btn_CCDPar3, 1, 0, 1, 1);


        verticalLayout_10->addWidget(WidgetHide);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        Head_B = new QCheckBox(widget);
        Head_B->setObjectName(QStringLiteral("Head_B"));
        Head_B->setAutoExclusive(true);

        gridLayout->addWidget(Head_B, 1, 0, 1, 1);

        label_7 = new QLabel(widget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        Btn_Connect_cfg = new QPushButton(widget);
        Btn_Connect_cfg->setObjectName(QStringLiteral("Btn_Connect_cfg"));

        gridLayout->addWidget(Btn_Connect_cfg, 5, 0, 1, 1);

        SourceIndex = new QSpinBox(widget);
        SourceIndex->setObjectName(QStringLiteral("SourceIndex"));
        SourceIndex->setMaximum(3);

        gridLayout->addWidget(SourceIndex, 2, 1, 1, 1);

        SourceSubIndex = new QSpinBox(widget);
        SourceSubIndex->setObjectName(QStringLiteral("SourceSubIndex"));

        gridLayout->addWidget(SourceSubIndex, 2, 3, 1, 1);

        Head_A = new QCheckBox(widget);
        Head_A->setObjectName(QStringLiteral("Head_A"));
        Head_A->setChecked(true);
        Head_A->setAutoExclusive(true);

        gridLayout->addWidget(Head_A, 0, 0, 1, 1);

        DstSubIndex = new QSpinBox(widget);
        DstSubIndex->setObjectName(QStringLiteral("DstSubIndex"));

        gridLayout->addWidget(DstSubIndex, 3, 3, 1, 1);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 4, 0, 1, 1);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        DstIndex = new QSpinBox(widget);
        DstIndex->setObjectName(QStringLiteral("DstIndex"));

        gridLayout->addWidget(DstIndex, 3, 1, 1, 1);

        label_5 = new QLabel(widget);
        label_5->setObjectName(QStringLiteral("label_5"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 2, 4, 1, 1);

        label_9 = new QLabel(widget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_9, 3, 4, 1, 1);

        label_12 = new QLabel(widget);
        label_12->setObjectName(QStringLiteral("label_12"));
        sizePolicy2.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy2);
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_12, 2, 2, 1, 1);

        btn_runtest = new QPushButton(widget);
        btn_runtest->setObjectName(QStringLiteral("btn_runtest"));

        gridLayout->addWidget(btn_runtest, 5, 3, 1, 1);


        verticalLayout_10->addLayout(gridLayout);


        verticalLayout_9->addWidget(widget);


        verticalLayout_11->addWidget(MainFrame3);


        horizontalLayout_2->addLayout(verticalLayout_11);

        frame = new QFrame(CCD_MainWidget);
        frame->setObjectName(QStringLiteral("frame"));
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frame_2);
        gridLayout_4->setSpacing(2);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(2, 2, 2, 2);
        Btn_CCDPar2 = new QPushButton(frame_2);
        Btn_CCDPar2->setObjectName(QStringLiteral("Btn_CCDPar2"));

        gridLayout_4->addWidget(Btn_CCDPar2, 5, 3, 1, 1);

        frame_10 = new QFrame(frame_2);
        frame_10->setObjectName(QStringLiteral("frame_10"));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_10);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, -1, -1, -1);
        label = new QLabel(frame_10);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_3->addWidget(label);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        MarkUpLoadB = new QPushButton(frame_10);
        MarkUpLoadB->setObjectName(QStringLiteral("MarkUpLoadB"));

        verticalLayout_19->addWidget(MarkUpLoadB);

        IdUpLoadB = new QPushButton(frame_10);
        IdUpLoadB->setObjectName(QStringLiteral("IdUpLoadB"));

        verticalLayout_19->addWidget(IdUpLoadB);


        horizontalLayout_4->addLayout(verticalLayout_19);

        ViewUpB = new QFrame(frame_10);
        ViewUpB->setObjectName(QStringLiteral("ViewUpB"));
        sizePolicy1.setHeightForWidth(ViewUpB->sizePolicy().hasHeightForWidth());
        ViewUpB->setSizePolicy(sizePolicy1);
        ViewUpB->setMinimumSize(QSize(34, 27));
        ViewUpB->setMaximumSize(QSize(100, 80));
        ViewUpB->setFrameShape(QFrame::Box);
        ViewUpB->setFrameShadow(QFrame::Raised);

        horizontalLayout_4->addWidget(ViewUpB);


        verticalLayout_3->addLayout(horizontalLayout_4);


        gridLayout_4->addWidget(frame_10, 0, 3, 1, 1);

        Btn_CCDPar1 = new QPushButton(frame_2);
        Btn_CCDPar1->setObjectName(QStringLiteral("Btn_CCDPar1"));

        gridLayout_4->addWidget(Btn_CCDPar1, 5, 1, 1, 1);

        frame1 = new QFrame(frame_2);
        frame1->setObjectName(QStringLiteral("frame1"));
        frame1->setFrameShape(QFrame::StyledPanel);
        frame1->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, -1, -1, -1);
        label_2 = new QLabel(frame1);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        MarkUpLoadA = new QPushButton(frame1);
        MarkUpLoadA->setObjectName(QStringLiteral("MarkUpLoadA"));
        QFont font;
        font.setPointSize(10);
        MarkUpLoadA->setFont(font);

        verticalLayout_16->addWidget(MarkUpLoadA);

        IdUpLoadA = new QPushButton(frame1);
        IdUpLoadA->setObjectName(QStringLiteral("IdUpLoadA"));
        IdUpLoadA->setFont(font);

        verticalLayout_16->addWidget(IdUpLoadA);


        horizontalLayout->addLayout(verticalLayout_16);

        ViewUpA = new QFrame(frame1);
        ViewUpA->setObjectName(QStringLiteral("ViewUpA"));
        sizePolicy1.setHeightForWidth(ViewUpA->sizePolicy().hasHeightForWidth());
        ViewUpA->setSizePolicy(sizePolicy1);
        ViewUpA->setMinimumSize(QSize(34, 27));
        ViewUpA->setMaximumSize(QSize(100, 80));
        ViewUpA->setFrameShape(QFrame::Box);
        ViewUpA->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(ViewUpA);


        verticalLayout_2->addLayout(horizontalLayout);


        gridLayout_4->addWidget(frame1, 0, 1, 1, 1);

        Btn_CalSim = new QPushButton(frame_2);
        Btn_CalSim->setObjectName(QStringLiteral("Btn_CalSim"));

        gridLayout_4->addWidget(Btn_CalSim, 7, 3, 1, 1);

        SetIdPosition2 = new QPushButton(frame_2);
        SetIdPosition2->setObjectName(QStringLiteral("SetIdPosition2"));

        gridLayout_4->addWidget(SetIdPosition2, 6, 3, 1, 1);

        SetIdPosition1 = new QPushButton(frame_2);
        SetIdPosition1->setObjectName(QStringLiteral("SetIdPosition1"));

        gridLayout_4->addWidget(SetIdPosition1, 6, 1, 1, 1);

        StartIdentify = new QPushButton(frame_2);
        StartIdentify->setObjectName(QStringLiteral("StartIdentify"));

        gridLayout_4->addWidget(StartIdentify, 7, 1, 1, 1);


        verticalLayout->addWidget(frame_2);

        Dis_Log = new QPlainTextEdit(frame);
        Dis_Log->setObjectName(QStringLiteral("Dis_Log"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(Dis_Log->sizePolicy().hasHeightForWidth());
        Dis_Log->setSizePolicy(sizePolicy3);

        verticalLayout->addWidget(Dis_Log);

        frame_7 = new QFrame(frame);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(frame_7);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 9, 0, 1, 1);

        Btn_History = new QPushButton(frame_7);
        Btn_History->setObjectName(QStringLiteral("Btn_History"));

        gridLayout_5->addWidget(Btn_History, 2, 4, 1, 1);

        Btn_ClearLog = new QPushButton(frame_7);
        Btn_ClearLog->setObjectName(QStringLiteral("Btn_ClearLog"));

        gridLayout_5->addWidget(Btn_ClearLog, 3, 0, 1, 1);

        Btn_SaveImg = new QPushButton(frame_7);
        Btn_SaveImg->setObjectName(QStringLiteral("Btn_SaveImg"));

        gridLayout_5->addWidget(Btn_SaveImg, 7, 0, 1, 1);

        Btn_EnableHistory = new QCheckBox(frame_7);
        Btn_EnableHistory->setObjectName(QStringLiteral("Btn_EnableHistory"));

        gridLayout_5->addWidget(Btn_EnableHistory, 2, 1, 1, 1);

        camera_index = new QSpinBox(frame_7);
        camera_index->setObjectName(QStringLiteral("camera_index"));

        gridLayout_5->addWidget(camera_index, 3, 4, 1, 1);

        Btn_CommonPar = new QPushButton(frame_7);
        Btn_CommonPar->setObjectName(QStringLiteral("Btn_CommonPar"));

        gridLayout_5->addWidget(Btn_CommonPar, 2, 0, 1, 1);

        Ck_RealTimeImg = new QCheckBox(frame_7);
        Ck_RealTimeImg->setObjectName(QStringLiteral("Ck_RealTimeImg"));

        gridLayout_5->addWidget(Ck_RealTimeImg, 3, 1, 1, 1);

        Btn_SavePattern = new QPushButton(frame_7);
        Btn_SavePattern->setObjectName(QStringLiteral("Btn_SavePattern"));

        gridLayout_5->addWidget(Btn_SavePattern, 8, 1, 1, 1);

        Btn_LoadPattern = new QPushButton(frame_7);
        Btn_LoadPattern->setObjectName(QStringLiteral("Btn_LoadPattern"));

        gridLayout_5->addWidget(Btn_LoadPattern, 8, 0, 1, 1);

        Btn_LoadImg = new QPushButton(frame_7);
        Btn_LoadImg->setObjectName(QStringLiteral("Btn_LoadImg"));

        gridLayout_5->addWidget(Btn_LoadImg, 8, 4, 1, 1);


        verticalLayout->addWidget(frame_7);


        horizontalLayout_2->addWidget(frame);

        horizontalLayout_2->setStretch(0, 4);
        horizontalLayout_2->setStretch(1, 1);

        retranslateUi(CCD_MainWidget);

        QMetaObject::connectSlotsByName(CCD_MainWidget);
    } // setupUi

    void retranslateUi(QWidget *CCD_MainWidget)
    {
        CCD_MainWidget->setWindowTitle(QApplication::translate("CCD_MainWidget", "Widget", Q_NULLPTR));
        label_3->setText(QApplication::translate("CCD_MainWidget", "\344\270\213\346\226\231B\346\250\241\346\235\277", Q_NULLPTR));
        MarkDownLoadB->setText(QApplication::translate("CCD_MainWidget", "\347\274\226\350\276\221", Q_NULLPTR));
        IdDownLoadB->setText(QApplication::translate("CCD_MainWidget", "\350\257\206\345\210\253", Q_NULLPTR));
        label_6->setText(QApplication::translate("CCD_MainWidget", "\344\270\213\346\226\231A\346\250\241\346\235\277", Q_NULLPTR));
        MarkDownLoadA->setText(QApplication::translate("CCD_MainWidget", "\347\274\226\350\276\221", Q_NULLPTR));
        IdDownLoadA->setText(QApplication::translate("CCD_MainWidget", "\350\257\206\345\210\253", Q_NULLPTR));
        label_11->setText(QApplication::translate("CCD_MainWidget", "\345\234\206\347\233\230\345\256\232\344\275\215", Q_NULLPTR));
        MarkTop2->setText(QApplication::translate("CCD_MainWidget", "\347\274\226\350\276\221", Q_NULLPTR));
        IdTop2->setText(QApplication::translate("CCD_MainWidget", "\350\257\206\345\210\253", Q_NULLPTR));
        Btn_UpLoadRefA->setText(QApplication::translate("CCD_MainWidget", "\344\270\212\346\226\231A\345\237\272\345\207\206\350\256\276\347\275\256", Q_NULLPTR));
        Btn_UpLoadRefB->setText(QApplication::translate("CCD_MainWidget", "\344\270\212\346\226\231B\345\237\272\345\207\206\350\256\276\347\275\256", Q_NULLPTR));
        Btn_DownLoadRefA->setText(QApplication::translate("CCD_MainWidget", "\344\270\213\346\226\231A\345\237\272\345\207\206\350\256\276\347\275\256", Q_NULLPTR));
        Btn_DownLoadRefB->setText(QApplication::translate("CCD_MainWidget", "\344\270\213\346\226\231B\345\237\272\345\207\206\350\256\276\347\275\256", Q_NULLPTR));
        label_10->setText(QApplication::translate("CCD_MainWidget", "\345\234\206\347\233\230\344\270\255\345\277\203", Q_NULLPTR));
        MarkTop1->setText(QApplication::translate("CCD_MainWidget", "\347\274\226\350\276\221", Q_NULLPTR));
        IdTop1->setText(QApplication::translate("CCD_MainWidget", "\350\257\206\345\210\253", Q_NULLPTR));
        Btn_CCDPar3->setText(QApplication::translate("CCD_MainWidget", "\344\270\212\347\233\270\346\234\272\346\240\207\345\256\232", Q_NULLPTR));
        Head_B->setText(QApplication::translate("CCD_MainWidget", "B\345\244\264", Q_NULLPTR));
        label_7->setText(QApplication::translate("CCD_MainWidget", "\346\224\276\346\226\231", Q_NULLPTR));
        Btn_Connect_cfg->setText(QApplication::translate("CCD_MainWidget", "\345\217\226\346\226\231\345\210\260\346\213\215\347\205\247\344\275\215", Q_NULLPTR));
        Head_A->setText(QApplication::translate("CCD_MainWidget", "A\345\244\264", Q_NULLPTR));
        pushButton->setText(QApplication::translate("CCD_MainWidget", "\345\217\226\346\226\231\345\210\260\347\233\256\346\240\207\344\275\215", Q_NULLPTR));
        label_4->setText(QApplication::translate("CCD_MainWidget", "\345\217\226\346\226\231", Q_NULLPTR));
        label_5->setText(QApplication::translate("CCD_MainWidget", "\347\211\207", Q_NULLPTR));
        label_9->setText(QApplication::translate("CCD_MainWidget", "\347\211\207", Q_NULLPTR));
        label_12->setText(QApplication::translate("CCD_MainWidget", "\347\233\230", Q_NULLPTR));
        btn_runtest->setText(QApplication::translate("CCD_MainWidget", "\346\265\213\350\257\225", Q_NULLPTR));
        Btn_CCDPar2->setText(QApplication::translate("CCD_MainWidget", "\346\265\213\351\207\217\347\233\270\346\234\272\346\240\207\345\256\232", Q_NULLPTR));
        label->setText(QApplication::translate("CCD_MainWidget", "\345\256\232\344\275\215\346\250\241\346\235\2772", Q_NULLPTR));
        MarkUpLoadB->setText(QApplication::translate("CCD_MainWidget", "\347\274\226\350\276\221", Q_NULLPTR));
        IdUpLoadB->setText(QApplication::translate("CCD_MainWidget", "\350\257\206\345\210\253", Q_NULLPTR));
        Btn_CCDPar1->setText(QApplication::translate("CCD_MainWidget", "\345\256\232\344\275\215\347\233\270\346\234\272\346\240\207\345\256\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("CCD_MainWidget", "\345\256\232\344\275\215\346\250\241\346\235\2771", Q_NULLPTR));
        MarkUpLoadA->setText(QApplication::translate("CCD_MainWidget", "\347\274\226\350\276\221", Q_NULLPTR));
        IdUpLoadA->setText(QApplication::translate("CCD_MainWidget", "\350\257\206\345\210\253", Q_NULLPTR));
        Btn_CalSim->setText(QApplication::translate("CCD_MainWidget", "\346\250\241\346\213\237\350\256\241\347\256\227", Q_NULLPTR));
        SetIdPosition2->setText(QApplication::translate("CCD_MainWidget", "\347\247\273\345\212\250\345\210\260\346\213\215\347\205\247\344\275\2152", Q_NULLPTR));
        SetIdPosition1->setText(QApplication::translate("CCD_MainWidget", "\347\247\273\345\212\250\345\210\260\346\213\215\347\205\247\344\275\2151", Q_NULLPTR));
        StartIdentify->setText(QApplication::translate("CCD_MainWidget", "\345\274\200\345\247\213\350\257\206\345\210\253", Q_NULLPTR));
        Btn_History->setText(QApplication::translate("CCD_MainWidget", "\345\216\206\345\217\262\350\256\260\345\275\225", Q_NULLPTR));
        Btn_ClearLog->setText(QApplication::translate("CCD_MainWidget", "\346\270\205\351\231\244\346\227\245\345\277\227", Q_NULLPTR));
        Btn_SaveImg->setText(QApplication::translate("CCD_MainWidget", "\344\277\235\345\255\230\345\233\276\345\203\217", Q_NULLPTR));
        Btn_EnableHistory->setText(QApplication::translate("CCD_MainWidget", "\350\256\260\345\275\225\345\216\206\345\217\262", Q_NULLPTR));
        Btn_CommonPar->setText(QApplication::translate("CCD_MainWidget", "\350\241\245\345\201\277\345\217\202\346\225\260", Q_NULLPTR));
        Ck_RealTimeImg->setText(QApplication::translate("CCD_MainWidget", "\345\256\236\346\227\266\345\233\276\345\203\217", Q_NULLPTR));
        Btn_SavePattern->setText(QApplication::translate("CCD_MainWidget", "\344\277\235\345\255\230", Q_NULLPTR));
        Btn_LoadPattern->setText(QApplication::translate("CCD_MainWidget", "\345\212\240\350\275\275", Q_NULLPTR));
        Btn_LoadImg->setText(QApplication::translate("CCD_MainWidget", "\345\212\240\350\275\275\345\233\276\347\211\207", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_MainWidget: public Ui_CCD_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_MAINWIDGET_H
