/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCD_MainWidget
{
public:
    QHBoxLayout *horizontalLayout_13;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QVBoxLayout *verticalLayout_16;
    QPushButton *EditeMark1;
    QPushButton *Btn_LocateM1;
    QFrame *TemplateView1;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QVBoxLayout *verticalLayout_13;
    QPushButton *EditeMark2;
    QPushButton *Btn_LocateM2;
    QFrame *TemplateView2;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QVBoxLayout *verticalLayout_14;
    QPushButton *EditeMark3;
    QPushButton *Btn_LocateM3;
    QFrame *TemplateView3;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QVBoxLayout *verticalLayout_15;
    QPushButton *EditeMark4;
    QPushButton *Btn_LocateM4;
    QFrame *TemplateView4;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_18;
    QFrame *MainFrame;
    QLabel *log_label;
    QVBoxLayout *verticalLayout_19;
    QFrame *frame_8;
    QGridLayout *gridLayout_3;
    QPushButton *SelectMark4;
    QPushButton *SelectMark3;
    QPushButton *SelectMark2;
    QPushButton *SelectMark1;
    QPushButton *IdentifyMark1;
    QPushButton *Btn_CCDPar;
    QPushButton *Btn_CommonPar;
    QPushButton *Btn_CommonPar2;
    QPushButton *Btn_CCDPar2;
    QCheckBox *RunWithoutCamera;
    QFrame *frame_2;
    QFormLayout *formLayout;
    QLabel *DisOffset11;
    QLabel *DisOffset;
    QLabel *DisOffset0;
    QLabel *DisAngle;
    QFrame *frame_10;
    QGridLayout *gridLayout_2;
    QPushButton *Btn_SimulateIdentify;
    QPushButton *Btn_LoadPattern;
    QPushButton *Btn_SavePattern;
    QPushButton *Btn_SaveImg;

    void setupUi(QWidget *CCD_MainWidget)
    {
        if (CCD_MainWidget->objectName().isEmpty())
            CCD_MainWidget->setObjectName(QString::fromUtf8("CCD_MainWidget"));
        CCD_MainWidget->resize(686, 446);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(10);
        sizePolicy.setVerticalStretch(8);
        sizePolicy.setHeightForWidth(CCD_MainWidget->sizePolicy().hasHeightForWidth());
        CCD_MainWidget->setSizePolicy(sizePolicy);
        horizontalLayout_13 = new QHBoxLayout(CCD_MainWidget);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(4, 6, 4, 6);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(4);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        frame = new QFrame(CCD_MainWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(9, 4, -1, 4);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label->setWordWrap(true);

        horizontalLayout->addWidget(label);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        EditeMark1 = new QPushButton(frame);
        EditeMark1->setObjectName(QString::fromUtf8("EditeMark1"));
        QFont font;
        font.setPointSize(10);
        EditeMark1->setFont(font);

        verticalLayout_16->addWidget(EditeMark1);

        Btn_LocateM1 = new QPushButton(frame);
        Btn_LocateM1->setObjectName(QString::fromUtf8("Btn_LocateM1"));
        Btn_LocateM1->setFont(font);

        verticalLayout_16->addWidget(Btn_LocateM1);


        horizontalLayout->addLayout(verticalLayout_16);

        TemplateView1 = new QFrame(frame);
        TemplateView1->setObjectName(QString::fromUtf8("TemplateView1"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(12);
        sizePolicy1.setVerticalStretch(10);
        sizePolicy1.setHeightForWidth(TemplateView1->sizePolicy().hasHeightForWidth());
        TemplateView1->setSizePolicy(sizePolicy1);
        TemplateView1->setMinimumSize(QSize(34, 27));
        TemplateView1->setMaximumSize(QSize(100, 80));
        TemplateView1->setFrameShape(QFrame::Box);
        TemplateView1->setFrameShadow(QFrame::Raised);

        horizontalLayout->addWidget(TemplateView1);


        horizontalLayout_8->addWidget(frame);

        frame_4 = new QFrame(CCD_MainWidget);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_4);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(9, 4, -1, 4);
        label_3 = new QLabel(frame_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_3->setWordWrap(true);

        horizontalLayout_3->addWidget(label_3);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        EditeMark2 = new QPushButton(frame_4);
        EditeMark2->setObjectName(QString::fromUtf8("EditeMark2"));

        verticalLayout_13->addWidget(EditeMark2);

        Btn_LocateM2 = new QPushButton(frame_4);
        Btn_LocateM2->setObjectName(QString::fromUtf8("Btn_LocateM2"));

        verticalLayout_13->addWidget(Btn_LocateM2);


        horizontalLayout_3->addLayout(verticalLayout_13);

        TemplateView2 = new QFrame(frame_4);
        TemplateView2->setObjectName(QString::fromUtf8("TemplateView2"));
        sizePolicy1.setHeightForWidth(TemplateView2->sizePolicy().hasHeightForWidth());
        TemplateView2->setSizePolicy(sizePolicy1);
        TemplateView2->setMinimumSize(QSize(34, 27));
        TemplateView2->setMaximumSize(QSize(100, 80));
        TemplateView2->setFrameShape(QFrame::Box);
        TemplateView2->setFrameShadow(QFrame::Raised);

        horizontalLayout_3->addWidget(TemplateView2);


        horizontalLayout_8->addWidget(frame_4);

        frame_5 = new QFrame(CCD_MainWidget);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_5);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(9, 4, -1, 4);
        label_4 = new QLabel(frame_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4->setWordWrap(true);

        horizontalLayout_5->addWidget(label_4);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        EditeMark3 = new QPushButton(frame_5);
        EditeMark3->setObjectName(QString::fromUtf8("EditeMark3"));

        verticalLayout_14->addWidget(EditeMark3);

        Btn_LocateM3 = new QPushButton(frame_5);
        Btn_LocateM3->setObjectName(QString::fromUtf8("Btn_LocateM3"));

        verticalLayout_14->addWidget(Btn_LocateM3);


        horizontalLayout_5->addLayout(verticalLayout_14);

        TemplateView3 = new QFrame(frame_5);
        TemplateView3->setObjectName(QString::fromUtf8("TemplateView3"));
        sizePolicy1.setHeightForWidth(TemplateView3->sizePolicy().hasHeightForWidth());
        TemplateView3->setSizePolicy(sizePolicy1);
        TemplateView3->setMinimumSize(QSize(34, 27));
        TemplateView3->setMaximumSize(QSize(100, 80));
        TemplateView3->setFrameShape(QFrame::Box);
        TemplateView3->setFrameShadow(QFrame::Raised);

        horizontalLayout_5->addWidget(TemplateView3);


        horizontalLayout_8->addWidget(frame_5);

        frame_6 = new QFrame(CCD_MainWidget);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(frame_6);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(9, 4, -1, 4);
        label_5 = new QLabel(frame_6);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5->setWordWrap(true);

        horizontalLayout_6->addWidget(label_5);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        EditeMark4 = new QPushButton(frame_6);
        EditeMark4->setObjectName(QString::fromUtf8("EditeMark4"));

        verticalLayout_15->addWidget(EditeMark4);

        Btn_LocateM4 = new QPushButton(frame_6);
        Btn_LocateM4->setObjectName(QString::fromUtf8("Btn_LocateM4"));

        verticalLayout_15->addWidget(Btn_LocateM4);


        horizontalLayout_6->addLayout(verticalLayout_15);

        TemplateView4 = new QFrame(frame_6);
        TemplateView4->setObjectName(QString::fromUtf8("TemplateView4"));
        sizePolicy1.setHeightForWidth(TemplateView4->sizePolicy().hasHeightForWidth());
        TemplateView4->setSizePolicy(sizePolicy1);
        TemplateView4->setMinimumSize(QSize(34, 27));
        TemplateView4->setMaximumSize(QSize(100, 80));
        TemplateView4->setFrameShape(QFrame::Box);
        TemplateView4->setFrameShadow(QFrame::Raised);

        horizontalLayout_6->addWidget(TemplateView4);


        horizontalLayout_8->addWidget(frame_6);


        verticalLayout_3->addLayout(horizontalLayout_8);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        MainFrame = new QFrame(CCD_MainWidget);
        MainFrame->setObjectName(QString::fromUtf8("MainFrame"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(10);
        sizePolicy2.setVerticalStretch(8);
        sizePolicy2.setHeightForWidth(MainFrame->sizePolicy().hasHeightForWidth());
        MainFrame->setSizePolicy(sizePolicy2);
        MainFrame->setMinimumSize(QSize(387, 310));
        MainFrame->setMaximumSize(QSize(387, 310));
        MainFrame->setFrameShape(QFrame::StyledPanel);
        MainFrame->setFrameShadow(QFrame::Raised);

        verticalLayout_18->addWidget(MainFrame);

        log_label = new QLabel(CCD_MainWidget);
        log_label->setObjectName(QString::fromUtf8("log_label"));

        verticalLayout_18->addWidget(log_label);


        horizontalLayout_4->addLayout(verticalLayout_18);

        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setSpacing(0);
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        frame_8 = new QFrame(CCD_MainWidget);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_8);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(6, 6, 6, 6);
        SelectMark4 = new QPushButton(frame_8);
        SelectMark4->setObjectName(QString::fromUtf8("SelectMark4"));
        SelectMark4->setStyleSheet(QString::fromUtf8("QPushButton:checked{\n"
"	background-color: rgb(255, 139, 7);\n"
"}"));
        SelectMark4->setCheckable(true);
        SelectMark4->setChecked(false);
        SelectMark4->setAutoExclusive(true);

        gridLayout_3->addWidget(SelectMark4, 1, 0, 1, 1);

        SelectMark3 = new QPushButton(frame_8);
        SelectMark3->setObjectName(QString::fromUtf8("SelectMark3"));
        SelectMark3->setStyleSheet(QString::fromUtf8("QPushButton:checked{\n"
"	background-color: rgb(255, 139, 7);\n"
"}"));
        SelectMark3->setCheckable(true);
        SelectMark3->setAutoExclusive(true);

        gridLayout_3->addWidget(SelectMark3, 1, 1, 1, 1);

        SelectMark2 = new QPushButton(frame_8);
        SelectMark2->setObjectName(QString::fromUtf8("SelectMark2"));
        SelectMark2->setStyleSheet(QString::fromUtf8("QPushButton:checked{\n"
"	background-color: rgb(255, 139, 7);\n"
"}"));
        SelectMark2->setCheckable(true);
        SelectMark2->setAutoExclusive(true);

        gridLayout_3->addWidget(SelectMark2, 0, 1, 1, 1);

        SelectMark1 = new QPushButton(frame_8);
        SelectMark1->setObjectName(QString::fromUtf8("SelectMark1"));
        SelectMark1->setStyleSheet(QString::fromUtf8("QPushButton:checked{\n"
"	background-color: rgb(255, 139, 7);\n"
"}"));
        SelectMark1->setCheckable(true);
        SelectMark1->setChecked(true);
        SelectMark1->setAutoExclusive(true);

        gridLayout_3->addWidget(SelectMark1, 0, 0, 1, 1);

        IdentifyMark1 = new QPushButton(frame_8);
        IdentifyMark1->setObjectName(QString::fromUtf8("IdentifyMark1"));
        IdentifyMark1->setMinimumSize(QSize(0, 0));

        gridLayout_3->addWidget(IdentifyMark1, 2, 0, 1, 1);

        Btn_CCDPar = new QPushButton(frame_8);
        Btn_CCDPar->setObjectName(QString::fromUtf8("Btn_CCDPar"));

        gridLayout_3->addWidget(Btn_CCDPar, 3, 1, 1, 1);

        Btn_CommonPar = new QPushButton(frame_8);
        Btn_CommonPar->setObjectName(QString::fromUtf8("Btn_CommonPar"));

        gridLayout_3->addWidget(Btn_CommonPar, 3, 0, 1, 1);

        Btn_CommonPar2 = new QPushButton(frame_8);
        Btn_CommonPar2->setObjectName(QString::fromUtf8("Btn_CommonPar2"));

        gridLayout_3->addWidget(Btn_CommonPar2, 4, 0, 1, 1);

        Btn_CCDPar2 = new QPushButton(frame_8);
        Btn_CCDPar2->setObjectName(QString::fromUtf8("Btn_CCDPar2"));

        gridLayout_3->addWidget(Btn_CCDPar2, 4, 1, 1, 1);

        RunWithoutCamera = new QCheckBox(frame_8);
        RunWithoutCamera->setObjectName(QString::fromUtf8("RunWithoutCamera"));

        gridLayout_3->addWidget(RunWithoutCamera, 2, 1, 1, 1);


        verticalLayout_19->addWidget(frame_8);

        frame_2 = new QFrame(CCD_MainWidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(frame_2);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        DisOffset11 = new QLabel(frame_2);
        DisOffset11->setObjectName(QString::fromUtf8("DisOffset11"));

        formLayout->setWidget(0, QFormLayout::LabelRole, DisOffset11);

        DisOffset = new QLabel(frame_2);
        DisOffset->setObjectName(QString::fromUtf8("DisOffset"));

        formLayout->setWidget(0, QFormLayout::FieldRole, DisOffset);

        DisOffset0 = new QLabel(frame_2);
        DisOffset0->setObjectName(QString::fromUtf8("DisOffset0"));

        formLayout->setWidget(1, QFormLayout::LabelRole, DisOffset0);

        DisAngle = new QLabel(frame_2);
        DisAngle->setObjectName(QString::fromUtf8("DisAngle"));

        formLayout->setWidget(1, QFormLayout::FieldRole, DisAngle);


        verticalLayout_19->addWidget(frame_2);

        frame_10 = new QFrame(CCD_MainWidget);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_10);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(9, 9, 9, 9);
        Btn_SimulateIdentify = new QPushButton(frame_10);
        Btn_SimulateIdentify->setObjectName(QString::fromUtf8("Btn_SimulateIdentify"));

        gridLayout_2->addWidget(Btn_SimulateIdentify, 0, 1, 1, 1);

        Btn_LoadPattern = new QPushButton(frame_10);
        Btn_LoadPattern->setObjectName(QString::fromUtf8("Btn_LoadPattern"));

        gridLayout_2->addWidget(Btn_LoadPattern, 1, 0, 1, 1);

        Btn_SavePattern = new QPushButton(frame_10);
        Btn_SavePattern->setObjectName(QString::fromUtf8("Btn_SavePattern"));

        gridLayout_2->addWidget(Btn_SavePattern, 1, 1, 1, 1);

        Btn_SaveImg = new QPushButton(frame_10);
        Btn_SaveImg->setObjectName(QString::fromUtf8("Btn_SaveImg"));

        gridLayout_2->addWidget(Btn_SaveImg, 0, 0, 1, 1);


        verticalLayout_19->addWidget(frame_10);


        horizontalLayout_4->addLayout(verticalLayout_19);


        verticalLayout_3->addLayout(horizontalLayout_4);


        horizontalLayout_13->addLayout(verticalLayout_3);


        retranslateUi(CCD_MainWidget);

        QMetaObject::connectSlotsByName(CCD_MainWidget);
    } // setupUi

    void retranslateUi(QWidget *CCD_MainWidget)
    {
        CCD_MainWidget->setWindowTitle(QApplication::translate("CCD_MainWidget", "Widget", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CCD_MainWidget", "\346\250\241\n"
"\346\235\277\n"
"1", 0, QApplication::UnicodeUTF8));
        EditeMark1->setText(QApplication::translate("CCD_MainWidget", "\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        Btn_LocateM1->setText(QApplication::translate("CCD_MainWidget", "\346\265\213\351\207\217\347\202\271", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CCD_MainWidget", "\346\250\241\n"
"\346\235\277\n"
"2", 0, QApplication::UnicodeUTF8));
        EditeMark2->setText(QApplication::translate("CCD_MainWidget", "\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        Btn_LocateM2->setText(QApplication::translate("CCD_MainWidget", "\346\265\213\351\207\217\347\202\271", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CCD_MainWidget", "\346\250\241\n"
"\346\235\277\n"
"3", 0, QApplication::UnicodeUTF8));
        EditeMark3->setText(QApplication::translate("CCD_MainWidget", "\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        Btn_LocateM3->setText(QApplication::translate("CCD_MainWidget", "\346\265\213\351\207\217\347\202\271", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CCD_MainWidget", "\346\250\241\n"
"\346\235\277\n"
"4", 0, QApplication::UnicodeUTF8));
        EditeMark4->setText(QApplication::translate("CCD_MainWidget", "\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        Btn_LocateM4->setText(QApplication::translate("CCD_MainWidget", "\346\265\213\351\207\217\347\202\271", 0, QApplication::UnicodeUTF8));
        log_label->setText(QString());
        SelectMark4->setText(QApplication::translate("CCD_MainWidget", "\346\250\241\346\235\2774", 0, QApplication::UnicodeUTF8));
        SelectMark3->setText(QApplication::translate("CCD_MainWidget", "\346\250\241\346\235\2773", 0, QApplication::UnicodeUTF8));
        SelectMark2->setText(QApplication::translate("CCD_MainWidget", "\346\250\241\346\235\2772", 0, QApplication::UnicodeUTF8));
        SelectMark1->setText(QApplication::translate("CCD_MainWidget", "\346\250\241\346\235\2771", 0, QApplication::UnicodeUTF8));
        IdentifyMark1->setText(QApplication::translate("CCD_MainWidget", "\350\257\206\345\210\253", 0, QApplication::UnicodeUTF8));
        Btn_CCDPar->setText(QApplication::translate("CCD_MainWidget", "CCD\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        Btn_CommonPar->setText(QApplication::translate("CCD_MainWidget", "\345\212\240\345\267\245\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        Btn_CommonPar2->setText(QApplication::translate("CCD_MainWidget", "\345\212\240\345\267\245\345\217\202\346\225\260-Y2", 0, QApplication::UnicodeUTF8));
        Btn_CCDPar2->setText(QApplication::translate("CCD_MainWidget", "CCD\345\217\202\346\225\260-Y2", 0, QApplication::UnicodeUTF8));
        RunWithoutCamera->setText(QApplication::translate("CCD_MainWidget", "RunOffline", 0, QApplication::UnicodeUTF8));
        DisOffset11->setText(QApplication::translate("CCD_MainWidget", "\345\201\217\347\247\273: ", 0, QApplication::UnicodeUTF8));
        DisOffset->setText(QApplication::translate("CCD_MainWidget", "x 0.000   y 0.000", 0, QApplication::UnicodeUTF8));
        DisOffset0->setText(QApplication::translate("CCD_MainWidget", "\350\247\222\345\272\246: ", 0, QApplication::UnicodeUTF8));
        DisAngle->setText(QApplication::translate("CCD_MainWidget", "0.000 ", 0, QApplication::UnicodeUTF8));
        Btn_SimulateIdentify->setText(QApplication::translate("CCD_MainWidget", "\350\207\252\345\212\250\350\257\206\345\210\253", 0, QApplication::UnicodeUTF8));
        Btn_LoadPattern->setText(QApplication::translate("CCD_MainWidget", "\345\212\240\350\275\275", 0, QApplication::UnicodeUTF8));
        Btn_SavePattern->setText(QApplication::translate("CCD_MainWidget", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
        Btn_SaveImg->setText(QApplication::translate("CCD_MainWidget", "\344\277\235\345\255\230\345\233\276\345\203\217", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CCD_MainWidget: public Ui_CCD_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
