/********************************************************************************
** Form generated from reading UI file 'ccd_sysparcommoneditor.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_SYSPARCOMMONEDITOR_H
#define UI_CCD_SYSPARCOMMONEDITOR_H

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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCD_SysParCommonEditor
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *AA;
    QGridLayout *gridLayout_2;
    QLabel *label_17;
    QDoubleSpinBox *Com_AY0;
    QDoubleSpinBox *Com_AQ1;
    QFrame *frame;
    QLabel *label_20;
    QDoubleSpinBox *Com_AY3;
    QDoubleSpinBox *Com_AQ3;
    QDoubleSpinBox *Com_AQ2;
    QDoubleSpinBox *Com_AX3;
    QLabel *label_19;
    QDoubleSpinBox *Com_AY2;
    QLabel *label_16;
    QDoubleSpinBox *Com_AX2;
    QDoubleSpinBox *Com_AQ0;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_21;
    QDoubleSpinBox *Com_AX0;
    QLabel *label_18;
    QLabel *label_22;
    QLabel *label_32;
    QDoubleSpinBox *Com_AY1;
    QDoubleSpinBox *Com_AX1;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout_2;
    QLabel *label_14;
    QDoubleSpinBox *LinE_DelayId;
    QLabel *label_6;
    QDoubleSpinBox *SB_SearchLen3;
    QLabel *label_5;
    QDoubleSpinBox *SB_AngleToler;
    QDoubleSpinBox *SB_SearchToler;
    QLabel *label_4;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *SB_Offset_X;
    QDoubleSpinBox *SB_Offset_Y;
    QDoubleSpinBox *SB_Offset_A;
    QLabel *label_7;
    QSpinBox *ContrastValidValue;
    QWidget *ParTab;
    QVBoxLayout *verticalLayout_3;
    QFrame *line;
    QWidget *tab_2;
    QGridLayout *gridLayout;
    QDoubleSpinBox *Com_BX2;
    QDoubleSpinBox *Com_BY6;
    QDoubleSpinBox *Com_BQ1;
    QLabel *label_8;
    QDoubleSpinBox *Com_BX1;
    QLabel *label_9;
    QDoubleSpinBox *Com_BY0;
    QDoubleSpinBox *Com_BY4;
    QLabel *label_23;
    QDoubleSpinBox *Com_BX5;
    QDoubleSpinBox *Com_BY5;
    QDoubleSpinBox *Com_BY2;
    QDoubleSpinBox *Com_BQ3;
    QDoubleSpinBox *Com_BX0;
    QDoubleSpinBox *Com_BQ4;
    QDoubleSpinBox *Com_BQ7;
    QLabel *label_25;
    QLabel *label_10;
    QDoubleSpinBox *Com_BX4;
    QDoubleSpinBox *Com_BQ0;
    QLabel *label_26;
    QDoubleSpinBox *Com_BQ2;
    QDoubleSpinBox *Com_BQ5;
    QDoubleSpinBox *Com_BY7;
    QDoubleSpinBox *Com_BX3;
    QDoubleSpinBox *Com_BY3;
    QDoubleSpinBox *Com_BX6;
    QLabel *label_29;
    QDoubleSpinBox *Com_BQ6;
    QDoubleSpinBox *Com_BX7;
    QLabel *label_28;
    QLabel *label_15;
    QLabel *label_24;
    QDoubleSpinBox *Com_BY1;
    QLabel *label_27;
    QLabel *label_13;
    QLabel *label_31;
    QLabel *label_12;
    QLabel *label_30;
    QLabel *label_11;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QCheckBox *OffLine;
    QPushButton *Btn_Sure;
    QPushButton *Btn_Cancel;

    void setupUi(QDialog *CCD_SysParCommonEditor)
    {
        if (CCD_SysParCommonEditor->objectName().isEmpty())
            CCD_SysParCommonEditor->setObjectName(QStringLiteral("CCD_SysParCommonEditor"));
        CCD_SysParCommonEditor->resize(526, 375);
        verticalLayout = new QVBoxLayout(CCD_SysParCommonEditor);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(CCD_SysParCommonEditor);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        AA = new QWidget();
        AA->setObjectName(QStringLiteral("AA"));
        gridLayout_2 = new QGridLayout(AA);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_17 = new QLabel(AA);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_17, 1, 0, 1, 1);

        Com_AY0 = new QDoubleSpinBox(AA);
        Com_AY0->setObjectName(QStringLiteral("Com_AY0"));
        Com_AY0->setDecimals(3);
        Com_AY0->setMinimum(-10000);
        Com_AY0->setMaximum(10000);
        Com_AY0->setSingleStep(0.01);

        gridLayout_2->addWidget(Com_AY0, 1, 1, 1, 1);

        Com_AQ1 = new QDoubleSpinBox(AA);
        Com_AQ1->setObjectName(QStringLiteral("Com_AQ1"));
        Com_AQ1->setDecimals(3);
        Com_AQ1->setMinimum(-1000);
        Com_AQ1->setMaximum(1000);
        Com_AQ1->setSingleStep(0.1);
        Com_AQ1->setValue(0);

        gridLayout_2->addWidget(Com_AQ1, 4, 1, 1, 1);

        frame = new QFrame(AA);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_20 = new QLabel(frame);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(110, 0, 92, 12));
        Com_AY3 = new QDoubleSpinBox(frame);
        Com_AY3->setObjectName(QStringLiteral("Com_AY3"));
        Com_AY3->setGeometry(QRect(110, 44, 92, 20));
        Com_AY3->setDecimals(3);
        Com_AY3->setMinimum(-10);
        Com_AY3->setMaximum(10);
        Com_AY3->setSingleStep(0.01);
        Com_AQ3 = new QDoubleSpinBox(frame);
        Com_AQ3->setObjectName(QStringLiteral("Com_AQ3"));
        Com_AQ3->setGeometry(QRect(110, 70, 92, 20));
        Com_AQ3->setDecimals(3);
        Com_AQ3->setMinimum(-10);
        Com_AQ3->setMaximum(10);
        Com_AQ3->setSingleStep(0.01);
        Com_AQ2 = new QDoubleSpinBox(frame);
        Com_AQ2->setObjectName(QStringLiteral("Com_AQ2"));
        Com_AQ2->setGeometry(QRect(12, 70, 92, 20));
        Com_AQ2->setDecimals(3);
        Com_AQ2->setMinimum(-10);
        Com_AQ2->setMaximum(10);
        Com_AQ2->setSingleStep(0.01);
        Com_AX3 = new QDoubleSpinBox(frame);
        Com_AX3->setObjectName(QStringLiteral("Com_AX3"));
        Com_AX3->setGeometry(QRect(110, 18, 92, 20));
        Com_AX3->setDecimals(3);
        Com_AX3->setMinimum(-10);
        Com_AX3->setMaximum(10);
        Com_AX3->setSingleStep(0.1);
        Com_AX3->setValue(0);
        label_19 = new QLabel(frame);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(12, 0, 92, 12));
        Com_AY2 = new QDoubleSpinBox(frame);
        Com_AY2->setObjectName(QStringLiteral("Com_AY2"));
        Com_AY2->setGeometry(QRect(12, 44, 92, 20));
        Com_AY2->setDecimals(3);
        Com_AY2->setMinimum(-10);
        Com_AY2->setMaximum(10);
        Com_AY2->setSingleStep(0.1);
        Com_AY2->setValue(0);
        label_16 = new QLabel(frame);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(-86, 0, 92, 12));
        Com_AX2 = new QDoubleSpinBox(frame);
        Com_AX2->setObjectName(QStringLiteral("Com_AX2"));
        Com_AX2->setGeometry(QRect(12, 18, 92, 20));
        Com_AX2->setDecimals(3);
        Com_AX2->setMinimum(-10);
        Com_AX2->setMaximum(10);
        Com_AX2->setSingleStep(0.01);
        Com_AQ0 = new QDoubleSpinBox(frame);
        Com_AQ0->setObjectName(QStringLiteral("Com_AQ0"));
        Com_AQ0->setGeometry(QRect(10, 90, 158, 26));
        Com_AQ0->setDecimals(3);
        Com_AQ0->setMinimum(-10);
        Com_AQ0->setMaximum(10);
        Com_AQ0->setSingleStep(0.1);
        Com_AQ0->setValue(0);

        gridLayout_2->addWidget(frame, 5, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 5, 1, 1, 1);

        label_21 = new QLabel(AA);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_21, 0, 0, 1, 1);

        Com_AX0 = new QDoubleSpinBox(AA);
        Com_AX0->setObjectName(QStringLiteral("Com_AX0"));
        Com_AX0->setDecimals(3);
        Com_AX0->setMinimum(-10000);
        Com_AX0->setMaximum(10000);
        Com_AX0->setSingleStep(0.01);

        gridLayout_2->addWidget(Com_AX0, 0, 1, 1, 1);

        label_18 = new QLabel(AA);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_18, 2, 0, 1, 1);

        label_22 = new QLabel(AA);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_22, 3, 0, 1, 1);

        label_32 = new QLabel(AA);
        label_32->setObjectName(QStringLiteral("label_32"));
        label_32->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_32, 4, 0, 1, 1);

        Com_AY1 = new QDoubleSpinBox(AA);
        Com_AY1->setObjectName(QStringLiteral("Com_AY1"));
        Com_AY1->setDecimals(3);
        Com_AY1->setMinimum(-1000);
        Com_AY1->setMaximum(1000);
        Com_AY1->setSingleStep(0.01);

        gridLayout_2->addWidget(Com_AY1, 3, 1, 1, 1);

        Com_AX1 = new QDoubleSpinBox(AA);
        Com_AX1->setObjectName(QStringLiteral("Com_AX1"));
        Com_AX1->setDecimals(3);
        Com_AX1->setMinimum(-1000);
        Com_AX1->setMaximum(1000);
        Com_AX1->setSingleStep(0.01);

        gridLayout_2->addWidget(Com_AX1, 2, 1, 1, 1);

        tabWidget->addTab(AA, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_14 = new QLabel(tab);
        label_14->setObjectName(QStringLiteral("label_14"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_14);

        LinE_DelayId = new QDoubleSpinBox(tab);
        LinE_DelayId->setObjectName(QStringLiteral("LinE_DelayId"));
        LinE_DelayId->setDecimals(3);
        LinE_DelayId->setMinimum(10);
        LinE_DelayId->setMaximum(10000);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, LinE_DelayId);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_6);

        SB_SearchLen3 = new QDoubleSpinBox(tab);
        SB_SearchLen3->setObjectName(QStringLiteral("SB_SearchLen3"));
        SB_SearchLen3->setDecimals(3);
        SB_SearchLen3->setMinimum(1);
        SB_SearchLen3->setMaximum(100);
        SB_SearchLen3->setSingleStep(1);
        SB_SearchLen3->setValue(10);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, SB_SearchLen3);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_5);

        SB_AngleToler = new QDoubleSpinBox(tab);
        SB_AngleToler->setObjectName(QStringLiteral("SB_AngleToler"));
        SB_AngleToler->setDecimals(2);
        SB_AngleToler->setMinimum(1);
        SB_AngleToler->setMaximum(50);
        SB_AngleToler->setSingleStep(0.01);
        SB_AngleToler->setValue(5);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, SB_AngleToler);

        SB_SearchToler = new QDoubleSpinBox(tab);
        SB_SearchToler->setObjectName(QStringLiteral("SB_SearchToler"));
        SB_SearchToler->setDecimals(2);
        SB_SearchToler->setMinimum(1);
        SB_SearchToler->setMaximum(100);
        SB_SearchToler->setSingleStep(1);
        SB_SearchToler->setValue(10);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, SB_SearchToler);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_4);

        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, label_3);

        SB_Offset_X = new QDoubleSpinBox(tab);
        SB_Offset_X->setObjectName(QStringLiteral("SB_Offset_X"));
        SB_Offset_X->setDecimals(3);
        SB_Offset_X->setMinimum(-10);
        SB_Offset_X->setMaximum(10);
        SB_Offset_X->setSingleStep(0.01);

        formLayout_2->setWidget(4, QFormLayout::FieldRole, SB_Offset_X);

        SB_Offset_Y = new QDoubleSpinBox(tab);
        SB_Offset_Y->setObjectName(QStringLiteral("SB_Offset_Y"));
        SB_Offset_Y->setDecimals(3);
        SB_Offset_Y->setMinimum(-10);
        SB_Offset_Y->setMaximum(10);
        SB_Offset_Y->setSingleStep(0.01);

        formLayout_2->setWidget(5, QFormLayout::FieldRole, SB_Offset_Y);

        SB_Offset_A = new QDoubleSpinBox(tab);
        SB_Offset_A->setObjectName(QStringLiteral("SB_Offset_A"));
        SB_Offset_A->setDecimals(3);
        SB_Offset_A->setMinimum(-10);
        SB_Offset_A->setMaximum(10);
        SB_Offset_A->setSingleStep(0.1);
        SB_Offset_A->setValue(0);

        formLayout_2->setWidget(6, QFormLayout::FieldRole, SB_Offset_A);

        label_7 = new QLabel(tab);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout_2->setWidget(7, QFormLayout::LabelRole, label_7);

        ContrastValidValue = new QSpinBox(tab);
        ContrastValidValue->setObjectName(QStringLiteral("ContrastValidValue"));
        ContrastValidValue->setMinimum(0);
        ContrastValidValue->setMaximum(255);
        ContrastValidValue->setValue(81);

        formLayout_2->setWidget(7, QFormLayout::FieldRole, ContrastValidValue);


        verticalLayout_2->addLayout(formLayout_2);

        tabWidget->addTab(tab, QString());
        ParTab = new QWidget();
        ParTab->setObjectName(QStringLiteral("ParTab"));
        verticalLayout_3 = new QVBoxLayout(ParTab);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        line = new QFrame(ParTab);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        tabWidget->addTab(ParTab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout = new QGridLayout(tab_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        Com_BX2 = new QDoubleSpinBox(tab_2);
        Com_BX2->setObjectName(QStringLiteral("Com_BX2"));
        Com_BX2->setDecimals(3);
        Com_BX2->setMinimum(-50);
        Com_BX2->setMaximum(50);
        Com_BX2->setSingleStep(0.01);

        gridLayout->addWidget(Com_BX2, 4, 5, 1, 1);

        Com_BY6 = new QDoubleSpinBox(tab_2);
        Com_BY6->setObjectName(QStringLiteral("Com_BY6"));
        Com_BY6->setDecimals(3);

        gridLayout->addWidget(Com_BY6, 10, 5, 1, 1);

        Com_BQ1 = new QDoubleSpinBox(tab_2);
        Com_BQ1->setObjectName(QStringLiteral("Com_BQ1"));
        Com_BQ1->setDecimals(3);
        Com_BQ1->setMinimum(-10);
        Com_BQ1->setMaximum(10);
        Com_BQ1->setSingleStep(0.1);
        Com_BQ1->setValue(0);

        gridLayout->addWidget(Com_BQ1, 7, 3, 1, 1);

        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_8, 4, 0, 1, 1);

        Com_BX1 = new QDoubleSpinBox(tab_2);
        Com_BX1->setObjectName(QStringLiteral("Com_BX1"));
        Com_BX1->setDecimals(3);
        Com_BX1->setMinimum(-10000);
        Com_BX1->setMaximum(10000);
        Com_BX1->setSingleStep(0.01);

        gridLayout->addWidget(Com_BX1, 4, 3, 1, 1);

        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_9, 5, 0, 1, 1);

        Com_BY0 = new QDoubleSpinBox(tab_2);
        Com_BY0->setObjectName(QStringLiteral("Com_BY0"));
        Com_BY0->setDecimals(3);
        Com_BY0->setMinimum(-1000);
        Com_BY0->setMaximum(1000);
        Com_BY0->setSingleStep(0.01);

        gridLayout->addWidget(Com_BY0, 5, 2, 1, 1);

        Com_BY4 = new QDoubleSpinBox(tab_2);
        Com_BY4->setObjectName(QStringLiteral("Com_BY4"));
        Com_BY4->setDecimals(3);

        gridLayout->addWidget(Com_BY4, 10, 2, 1, 1);

        label_23 = new QLabel(tab_2);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout->addWidget(label_23, 8, 2, 1, 1);

        Com_BX5 = new QDoubleSpinBox(tab_2);
        Com_BX5->setObjectName(QStringLiteral("Com_BX5"));
        Com_BX5->setDecimals(3);

        gridLayout->addWidget(Com_BX5, 9, 3, 1, 1);

        Com_BY5 = new QDoubleSpinBox(tab_2);
        Com_BY5->setObjectName(QStringLiteral("Com_BY5"));
        Com_BY5->setDecimals(3);

        gridLayout->addWidget(Com_BY5, 10, 3, 1, 1);

        Com_BY2 = new QDoubleSpinBox(tab_2);
        Com_BY2->setObjectName(QStringLiteral("Com_BY2"));
        Com_BY2->setDecimals(3);
        Com_BY2->setMinimum(-50);
        Com_BY2->setMaximum(50);
        Com_BY2->setSingleStep(0.01);
        Com_BY2->setValue(0);

        gridLayout->addWidget(Com_BY2, 5, 5, 1, 1);

        Com_BQ3 = new QDoubleSpinBox(tab_2);
        Com_BQ3->setObjectName(QStringLiteral("Com_BQ3"));
        Com_BQ3->setDecimals(3);
        Com_BQ3->setMinimum(-50);
        Com_BQ3->setMaximum(50);
        Com_BQ3->setSingleStep(0.01);

        gridLayout->addWidget(Com_BQ3, 7, 6, 1, 1);

        Com_BX0 = new QDoubleSpinBox(tab_2);
        Com_BX0->setObjectName(QStringLiteral("Com_BX0"));
        Com_BX0->setDecimals(3);
        Com_BX0->setMinimum(-1000);
        Com_BX0->setMaximum(1000);
        Com_BX0->setSingleStep(0.01);

        gridLayout->addWidget(Com_BX0, 4, 2, 1, 1);

        Com_BQ4 = new QDoubleSpinBox(tab_2);
        Com_BQ4->setObjectName(QStringLiteral("Com_BQ4"));
        Com_BQ4->setDecimals(3);

        gridLayout->addWidget(Com_BQ4, 11, 2, 1, 1);

        Com_BQ7 = new QDoubleSpinBox(tab_2);
        Com_BQ7->setObjectName(QStringLiteral("Com_BQ7"));
        Com_BQ7->setDecimals(3);

        gridLayout->addWidget(Com_BQ7, 11, 6, 1, 1);

        label_25 = new QLabel(tab_2);
        label_25->setObjectName(QStringLiteral("label_25"));

        gridLayout->addWidget(label_25, 8, 5, 1, 1);

        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_10, 7, 0, 1, 1);

        Com_BX4 = new QDoubleSpinBox(tab_2);
        Com_BX4->setObjectName(QStringLiteral("Com_BX4"));
        Com_BX4->setDecimals(3);

        gridLayout->addWidget(Com_BX4, 9, 2, 1, 1);

        Com_BQ0 = new QDoubleSpinBox(tab_2);
        Com_BQ0->setObjectName(QStringLiteral("Com_BQ0"));
        Com_BQ0->setDecimals(3);
        Com_BQ0->setMinimum(-10);
        Com_BQ0->setMaximum(10);
        Com_BQ0->setSingleStep(0.1);
        Com_BQ0->setValue(0);

        gridLayout->addWidget(Com_BQ0, 7, 2, 1, 1);

        label_26 = new QLabel(tab_2);
        label_26->setObjectName(QStringLiteral("label_26"));

        gridLayout->addWidget(label_26, 8, 6, 1, 1);

        Com_BQ2 = new QDoubleSpinBox(tab_2);
        Com_BQ2->setObjectName(QStringLiteral("Com_BQ2"));
        Com_BQ2->setDecimals(3);
        Com_BQ2->setMinimum(-50);
        Com_BQ2->setMaximum(50);
        Com_BQ2->setSingleStep(0.01);

        gridLayout->addWidget(Com_BQ2, 7, 5, 1, 1);

        Com_BQ5 = new QDoubleSpinBox(tab_2);
        Com_BQ5->setObjectName(QStringLiteral("Com_BQ5"));
        Com_BQ5->setDecimals(3);

        gridLayout->addWidget(Com_BQ5, 11, 3, 1, 1);

        Com_BY7 = new QDoubleSpinBox(tab_2);
        Com_BY7->setObjectName(QStringLiteral("Com_BY7"));
        Com_BY7->setDecimals(3);

        gridLayout->addWidget(Com_BY7, 10, 6, 1, 1);

        Com_BX3 = new QDoubleSpinBox(tab_2);
        Com_BX3->setObjectName(QStringLiteral("Com_BX3"));
        Com_BX3->setDecimals(3);
        Com_BX3->setMinimum(-50);
        Com_BX3->setMaximum(50);
        Com_BX3->setSingleStep(0.01);
        Com_BX3->setValue(0);

        gridLayout->addWidget(Com_BX3, 4, 6, 1, 1);

        Com_BY3 = new QDoubleSpinBox(tab_2);
        Com_BY3->setObjectName(QStringLiteral("Com_BY3"));
        Com_BY3->setDecimals(3);
        Com_BY3->setMinimum(-50);
        Com_BY3->setMaximum(50);
        Com_BY3->setSingleStep(0.01);

        gridLayout->addWidget(Com_BY3, 5, 6, 1, 1);

        Com_BX6 = new QDoubleSpinBox(tab_2);
        Com_BX6->setObjectName(QStringLiteral("Com_BX6"));
        Com_BX6->setDecimals(3);

        gridLayout->addWidget(Com_BX6, 9, 5, 1, 1);

        label_29 = new QLabel(tab_2);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_29, 11, 0, 1, 1);

        Com_BQ6 = new QDoubleSpinBox(tab_2);
        Com_BQ6->setObjectName(QStringLiteral("Com_BQ6"));
        Com_BQ6->setDecimals(3);

        gridLayout->addWidget(Com_BQ6, 11, 5, 1, 1);

        Com_BX7 = new QDoubleSpinBox(tab_2);
        Com_BX7->setObjectName(QStringLiteral("Com_BX7"));
        Com_BX7->setDecimals(3);

        gridLayout->addWidget(Com_BX7, 9, 6, 1, 1);

        label_28 = new QLabel(tab_2);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_28, 10, 0, 1, 1);

        label_15 = new QLabel(tab_2);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout->addWidget(label_15, 0, 6, 1, 1);

        label_24 = new QLabel(tab_2);
        label_24->setObjectName(QStringLiteral("label_24"));

        gridLayout->addWidget(label_24, 8, 3, 1, 1);

        Com_BY1 = new QDoubleSpinBox(tab_2);
        Com_BY1->setObjectName(QStringLiteral("Com_BY1"));
        Com_BY1->setDecimals(3);
        Com_BY1->setMinimum(-1000);
        Com_BY1->setMaximum(1000);
        Com_BY1->setSingleStep(0.01);

        gridLayout->addWidget(Com_BY1, 5, 3, 1, 1);

        label_27 = new QLabel(tab_2);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_27, 9, 0, 1, 1);

        label_13 = new QLabel(tab_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout->addWidget(label_13, 0, 5, 1, 1);

        label_31 = new QLabel(tab_2);
        label_31->setObjectName(QStringLiteral("label_31"));

        gridLayout->addWidget(label_31, 2, 5, 1, 1);

        label_12 = new QLabel(tab_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout->addWidget(label_12, 2, 3, 1, 1);

        label_30 = new QLabel(tab_2);
        label_30->setObjectName(QStringLiteral("label_30"));

        gridLayout->addWidget(label_30, 2, 6, 1, 1);

        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout->addWidget(label_11, 2, 2, 1, 1);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        OffLine = new QCheckBox(CCD_SysParCommonEditor);
        OffLine->setObjectName(QStringLiteral("OffLine"));

        horizontalLayout->addWidget(OffLine);

        Btn_Sure = new QPushButton(CCD_SysParCommonEditor);
        Btn_Sure->setObjectName(QStringLiteral("Btn_Sure"));

        horizontalLayout->addWidget(Btn_Sure);

        Btn_Cancel = new QPushButton(CCD_SysParCommonEditor);
        Btn_Cancel->setObjectName(QStringLiteral("Btn_Cancel"));

        horizontalLayout->addWidget(Btn_Cancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CCD_SysParCommonEditor);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CCD_SysParCommonEditor);
    } // setupUi

    void retranslateUi(QDialog *CCD_SysParCommonEditor)
    {
        CCD_SysParCommonEditor->setWindowTitle(QApplication::translate("CCD_SysParCommonEditor", "Dialog", Q_NULLPTR));
        label_17->setText(QApplication::translate("CCD_SysParCommonEditor", "Y\346\226\271\345\220\221\351\225\277\345\272\246", Q_NULLPTR));
        label_20->setText(QApplication::translate("CCD_SysParCommonEditor", "4\345\267\245\344\275\215", Q_NULLPTR));
        label_19->setText(QApplication::translate("CCD_SysParCommonEditor", "3\345\267\245\344\275\215", Q_NULLPTR));
        label_16->setText(QApplication::translate("CCD_SysParCommonEditor", "2\345\267\245\344\275\215", Q_NULLPTR));
        label_21->setText(QApplication::translate("CCD_SysParCommonEditor", "X\346\226\271\345\220\221\351\225\277\345\272\246", Q_NULLPTR));
        label_18->setText(QApplication::translate("CCD_SysParCommonEditor", "X\350\241\245\345\201\277", Q_NULLPTR));
        label_22->setText(QApplication::translate("CCD_SysParCommonEditor", "Y\350\241\245\345\201\277", Q_NULLPTR));
        label_32->setText(QApplication::translate("CCD_SysParCommonEditor", "A\350\241\245\345\201\277", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(AA), QApplication::translate("CCD_SysParCommonEditor", "\345\256\232\344\275\215\345\217\202\346\225\260", Q_NULLPTR));
        label_14->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\273\266\346\227\266\350\257\206\345\210\253", Q_NULLPTR));
        label_6->setText(QApplication::translate("CCD_SysParCommonEditor", "\346\240\241\346\255\243X\347\247\273\345\212\250\346\234\200\345\244\247\345\200\274", Q_NULLPTR));
        label_5->setText(QApplication::translate("CCD_SysParCommonEditor", "\346\240\241\346\255\243A\347\247\273\345\212\250\346\234\200\345\244\247\345\200\274", Q_NULLPTR));
        label_4->setText(QApplication::translate("CCD_SysParCommonEditor", "\346\240\241\346\255\243Y\347\247\273\345\212\250\346\234\200\345\244\247\345\200\274", Q_NULLPTR));
        label->setText(QApplication::translate("CCD_SysParCommonEditor", "\344\272\247\345\223\201\345\205\254\345\267\256\346\212\245\350\255\246\345\200\274", Q_NULLPTR));
        label_2->setText(QString());
        label_3->setText(QString());
        label_7->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\257\271\346\257\224\345\272\246\346\234\211\346\225\210\345\200\274", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("CCD_SysParCommonEditor", "\345\205\266\344\273\226", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(ParTab), QApplication::translate("CCD_SysParCommonEditor", "PAR", Q_NULLPTR));
        label_8->setText(QApplication::translate("CCD_SysParCommonEditor", "X\350\275\264", Q_NULLPTR));
        label_9->setText(QApplication::translate("CCD_SysParCommonEditor", "Y\350\275\264", Q_NULLPTR));
        label_23->setText(QApplication::translate("CCD_SysParCommonEditor", "\344\270\212\346\226\231\346\234\200\345\244\247\345\201\217\345\267\256", Q_NULLPTR));
        label_25->setText(QString());
        label_10->setText(QString());
        label_26->setText(QString());
        label_29->setText(QApplication::translate("CCD_SysParCommonEditor", "\346\227\213\350\275\254\350\275\264", Q_NULLPTR));
        label_28->setText(QApplication::translate("CCD_SysParCommonEditor", "Y\350\275\264", Q_NULLPTR));
        label_15->setText(QString());
        label_24->setText(QApplication::translate("CCD_SysParCommonEditor", "\344\270\213\346\226\231\346\234\200\345\244\247\345\201\217\345\267\256", Q_NULLPTR));
        label_27->setText(QApplication::translate("CCD_SysParCommonEditor", "X\350\275\264", Q_NULLPTR));
        label_13->setText(QString());
        label_31->setText(QApplication::translate("CCD_SysParCommonEditor", "A\345\217\226\346\226\231\345\201\217\347\247\273", Q_NULLPTR));
        label_12->setText(QApplication::translate("CCD_SysParCommonEditor", "B\344\270\255\345\277\203\345\201\217\350\267\235", Q_NULLPTR));
        label_30->setText(QApplication::translate("CCD_SysParCommonEditor", "B\345\217\226\346\226\231\345\201\217\347\247\273", Q_NULLPTR));
        label_11->setText(QApplication::translate("CCD_SysParCommonEditor", "A\344\270\255\345\277\203\345\201\217\350\267\235", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("CCD_SysParCommonEditor", "\350\264\264\345\220\210\350\241\245\345\201\277", Q_NULLPTR));
        OffLine->setText(QApplication::translate("CCD_SysParCommonEditor", "\347\246\273\347\272\277", Q_NULLPTR));
        Btn_Sure->setText(QApplication::translate("CCD_SysParCommonEditor", "\347\241\256\345\256\232", Q_NULLPTR));
        Btn_Cancel->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_SysParCommonEditor: public Ui_CCD_SysParCommonEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_SYSPARCOMMONEDITOR_H
