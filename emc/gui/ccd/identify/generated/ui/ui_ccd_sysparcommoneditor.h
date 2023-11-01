/********************************************************************************
** Form generated from reading UI file 'ccd_sysparcommoneditor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_SYSPARCOMMONEDITOR_H
#define UI_CCD_SYSPARCOMMONEDITOR_H

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
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CCD_SysParCommonEditor
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *ID_Choose;
    QGridLayout *gridLayout_3;
    QLabel *label_17;
    QRadioButton *Btn_TwoP;
    QRadioButton *Btn_ThreeP;
    QRadioButton *Btn_FourP;
    QFrame *GB_Two;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_13;
    QFormLayout *formLayout_5;
    QLabel *label_15;
    QDoubleSpinBox *SB_TwoPointsW;
    QLabel *label_16;
    QDoubleSpinBox *SB_TwoPointsH;
    QFrame *WD_Ref;
    QGridLayout *gridLayout;
    QRadioButton *Btn_LeftUp;
    QLabel *label_12;
    QRadioButton *Btn_Center;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout_2;
    QLabel *label_6;
    QDoubleSpinBox *SB_SearchLen3;
    QLabel *label_4;
    QDoubleSpinBox *SB_SearchToler;
    QLabel *label_5;
    QDoubleSpinBox *SB_AngleToler;
    QDoubleSpinBox *LinE_DelayId;
    QLabel *label_14;
    QFrame *frame;
    QFormLayout *formLayout_3;
    QLabel *label_2;
    QDoubleSpinBox *LinE_ComputeLen;
    QDoubleSpinBox *LinE_ComputeDIff;
    QLabel *label_3;
    QPushButton *Btn_ComputeRotate;
    QDoubleSpinBox *LinE_RotateOffset;
    QLabel *label;
    QFrame *frame_2;
    QGridLayout *gridLayout_2;
    QLabel *label_11;
    QFormLayout *formLayout_4;
    QSpinBox *Array_With;
    QSpinBox *Array_Height;
    QLabel *label_7;
    QLabel *label_8;
    QSpinBox *SB_SampleNum;
    QLabel *label_18;
    QCheckBox *Array_Enable;
    QFormLayout *formLayout_6;
    QLabel *label_10;
    QDoubleSpinBox *Array_Distance_Hor;
    QLabel *label_9;
    QDoubleSpinBox *Array_Distance_Ver;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QCheckBox *Ckb_Offline;
    QPushButton *Btn_Sure;
    QPushButton *Btn_Cancel;

    void setupUi(QDialog *CCD_SysParCommonEditor)
    {
        if (CCD_SysParCommonEditor->objectName().isEmpty())
            CCD_SysParCommonEditor->setObjectName(QString::fromUtf8("CCD_SysParCommonEditor"));
        CCD_SysParCommonEditor->resize(410, 668);
        verticalLayout = new QVBoxLayout(CCD_SysParCommonEditor);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ID_Choose = new QFrame(CCD_SysParCommonEditor);
        ID_Choose->setObjectName(QString::fromUtf8("ID_Choose"));
        ID_Choose->setFrameShape(QFrame::StyledPanel);
        ID_Choose->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(ID_Choose);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_17 = new QLabel(ID_Choose);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_3->addWidget(label_17, 0, 0, 1, 1);

        Btn_TwoP = new QRadioButton(ID_Choose);
        Btn_TwoP->setObjectName(QString::fromUtf8("Btn_TwoP"));

        gridLayout_3->addWidget(Btn_TwoP, 2, 0, 1, 1);

        Btn_ThreeP = new QRadioButton(ID_Choose);
        Btn_ThreeP->setObjectName(QString::fromUtf8("Btn_ThreeP"));

        gridLayout_3->addWidget(Btn_ThreeP, 2, 1, 1, 1);

        Btn_FourP = new QRadioButton(ID_Choose);
        Btn_FourP->setObjectName(QString::fromUtf8("Btn_FourP"));

        gridLayout_3->addWidget(Btn_FourP, 2, 2, 1, 1);


        verticalLayout->addWidget(ID_Choose);

        GB_Two = new QFrame(CCD_SysParCommonEditor);
        GB_Two->setObjectName(QString::fromUtf8("GB_Two"));
        GB_Two->setFrameShape(QFrame::StyledPanel);
        GB_Two->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(GB_Two);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 0, -1, 0);
        label_13 = new QLabel(GB_Two);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        verticalLayout_3->addWidget(label_13);

        formLayout_5 = new QFormLayout();
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        label_15 = new QLabel(GB_Two);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, label_15);

        SB_TwoPointsW = new QDoubleSpinBox(GB_Two);
        SB_TwoPointsW->setObjectName(QString::fromUtf8("SB_TwoPointsW"));
        SB_TwoPointsW->setDecimals(3);
        SB_TwoPointsW->setMinimum(-5000);
        SB_TwoPointsW->setMaximum(5000);
        SB_TwoPointsW->setSingleStep(1);
        SB_TwoPointsW->setValue(10);

        formLayout_5->setWidget(0, QFormLayout::FieldRole, SB_TwoPointsW);

        label_16 = new QLabel(GB_Two);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        formLayout_5->setWidget(1, QFormLayout::LabelRole, label_16);

        SB_TwoPointsH = new QDoubleSpinBox(GB_Two);
        SB_TwoPointsH->setObjectName(QString::fromUtf8("SB_TwoPointsH"));
        SB_TwoPointsH->setDecimals(3);
        SB_TwoPointsH->setMinimum(-5001);
        SB_TwoPointsH->setMaximum(5000);
        SB_TwoPointsH->setSingleStep(1);
        SB_TwoPointsH->setValue(10);

        formLayout_5->setWidget(1, QFormLayout::FieldRole, SB_TwoPointsH);


        verticalLayout_3->addLayout(formLayout_5);


        verticalLayout->addWidget(GB_Two);

        WD_Ref = new QFrame(CCD_SysParCommonEditor);
        WD_Ref->setObjectName(QString::fromUtf8("WD_Ref"));
        WD_Ref->setFrameShape(QFrame::StyledPanel);
        WD_Ref->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(WD_Ref);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(-1, 0, -1, 0);
        Btn_LeftUp = new QRadioButton(WD_Ref);
        Btn_LeftUp->setObjectName(QString::fromUtf8("Btn_LeftUp"));

        gridLayout->addWidget(Btn_LeftUp, 1, 0, 1, 1);

        label_12 = new QLabel(WD_Ref);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 0, 0, 1, 1);

        Btn_Center = new QRadioButton(WD_Ref);
        Btn_Center->setObjectName(QString::fromUtf8("Btn_Center"));

        gridLayout->addWidget(Btn_Center, 1, 1, 1, 1);


        verticalLayout->addWidget(WD_Ref);

        frame_4 = new QFrame(CCD_SysParCommonEditor);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_6 = new QLabel(frame_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_6);

        SB_SearchLen3 = new QDoubleSpinBox(frame_4);
        SB_SearchLen3->setObjectName(QString::fromUtf8("SB_SearchLen3"));
        SB_SearchLen3->setDecimals(3);
        SB_SearchLen3->setMinimum(-1000);
        SB_SearchLen3->setMaximum(1000);
        SB_SearchLen3->setSingleStep(1);
        SB_SearchLen3->setValue(10);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, SB_SearchLen3);

        label_4 = new QLabel(frame_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_4);

        SB_SearchToler = new QDoubleSpinBox(frame_4);
        SB_SearchToler->setObjectName(QString::fromUtf8("SB_SearchToler"));
        SB_SearchToler->setDecimals(3);
        SB_SearchToler->setMinimum(-1000);
        SB_SearchToler->setMaximum(1000);
        SB_SearchToler->setSingleStep(1);
        SB_SearchToler->setValue(10);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, SB_SearchToler);

        label_5 = new QLabel(frame_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_5);

        SB_AngleToler = new QDoubleSpinBox(frame_4);
        SB_AngleToler->setObjectName(QString::fromUtf8("SB_AngleToler"));
        SB_AngleToler->setDecimals(2);
        SB_AngleToler->setMinimum(1);
        SB_AngleToler->setMaximum(120);
        SB_AngleToler->setSingleStep(0.01);
        SB_AngleToler->setValue(5);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, SB_AngleToler);

        LinE_DelayId = new QDoubleSpinBox(frame_4);
        LinE_DelayId->setObjectName(QString::fromUtf8("LinE_DelayId"));
        LinE_DelayId->setDecimals(3);
        LinE_DelayId->setMinimum(10);
        LinE_DelayId->setMaximum(10000);

        formLayout_2->setWidget(4, QFormLayout::FieldRole, LinE_DelayId);

        label_14 = new QLabel(frame_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_14);


        verticalLayout_2->addLayout(formLayout_2);


        verticalLayout->addWidget(frame_4);

        frame = new QFrame(CCD_SysParCommonEditor);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        formLayout_3 = new QFormLayout(frame);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setVerticalSpacing(2);
        formLayout_3->setContentsMargins(-1, 0, -1, 0);
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_2);

        LinE_ComputeLen = new QDoubleSpinBox(frame);
        LinE_ComputeLen->setObjectName(QString::fromUtf8("LinE_ComputeLen"));
        LinE_ComputeLen->setDecimals(3);
        LinE_ComputeLen->setMinimum(-1000);
        LinE_ComputeLen->setMaximum(1000);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, LinE_ComputeLen);

        LinE_ComputeDIff = new QDoubleSpinBox(frame);
        LinE_ComputeDIff->setObjectName(QString::fromUtf8("LinE_ComputeDIff"));
        LinE_ComputeDIff->setDecimals(3);
        LinE_ComputeDIff->setMinimum(-1000);
        LinE_ComputeDIff->setMaximum(1000);
        LinE_ComputeDIff->setValue(0);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, LinE_ComputeDIff);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_3);

        Btn_ComputeRotate = new QPushButton(frame);
        Btn_ComputeRotate->setObjectName(QString::fromUtf8("Btn_ComputeRotate"));

        formLayout_3->setWidget(3, QFormLayout::FieldRole, Btn_ComputeRotate);

        LinE_RotateOffset = new QDoubleSpinBox(frame);
        LinE_RotateOffset->setObjectName(QString::fromUtf8("LinE_RotateOffset"));
        LinE_RotateOffset->setDecimals(6);
        LinE_RotateOffset->setMinimum(-1000);
        LinE_RotateOffset->setMaximum(1000);

        formLayout_3->setWidget(5, QFormLayout::FieldRole, LinE_RotateOffset);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_3->setWidget(5, QFormLayout::LabelRole, label);


        verticalLayout->addWidget(frame);

        frame_2 = new QFrame(CCD_SysParCommonEditor);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(-1, 0, -1, 0);
        label_11 = new QLabel(frame_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 0, 0, 1, 1);

        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        Array_With = new QSpinBox(frame_2);
        Array_With->setObjectName(QString::fromUtf8("Array_With"));
        Array_With->setReadOnly(true);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, Array_With);

        Array_Height = new QSpinBox(frame_2);
        Array_Height->setObjectName(QString::fromUtf8("Array_Height"));
        Array_Height->setReadOnly(true);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, Array_Height);

        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_7);

        label_8 = new QLabel(frame_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_8);

        SB_SampleNum = new QSpinBox(frame_2);
        SB_SampleNum->setObjectName(QString::fromUtf8("SB_SampleNum"));
        SB_SampleNum->setMinimum(0);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, SB_SampleNum);

        label_18 = new QLabel(frame_2);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, label_18);


        gridLayout_2->addLayout(formLayout_4, 2, 0, 1, 1);

        Array_Enable = new QCheckBox(frame_2);
        Array_Enable->setObjectName(QString::fromUtf8("Array_Enable"));
        Array_Enable->setEnabled(false);
        Array_Enable->setCheckable(true);

        gridLayout_2->addWidget(Array_Enable, 0, 1, 1, 1);

        formLayout_6 = new QFormLayout();
        formLayout_6->setObjectName(QString::fromUtf8("formLayout_6"));
        label_10 = new QLabel(frame_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, label_10);

        Array_Distance_Hor = new QDoubleSpinBox(frame_2);
        Array_Distance_Hor->setObjectName(QString::fromUtf8("Array_Distance_Hor"));
        Array_Distance_Hor->setReadOnly(true);
        Array_Distance_Hor->setDecimals(3);
        Array_Distance_Hor->setMinimum(-1000);
        Array_Distance_Hor->setMaximum(1000);

        formLayout_6->setWidget(0, QFormLayout::FieldRole, Array_Distance_Hor);

        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout_6->setWidget(1, QFormLayout::LabelRole, label_9);

        Array_Distance_Ver = new QDoubleSpinBox(frame_2);
        Array_Distance_Ver->setObjectName(QString::fromUtf8("Array_Distance_Ver"));
        Array_Distance_Ver->setReadOnly(true);
        Array_Distance_Ver->setDecimals(3);
        Array_Distance_Ver->setMinimum(-1000);
        Array_Distance_Ver->setMaximum(1000);

        formLayout_6->setWidget(1, QFormLayout::FieldRole, Array_Distance_Ver);


        gridLayout_2->addLayout(formLayout_6, 2, 1, 1, 1);


        verticalLayout->addWidget(frame_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Ckb_Offline = new QCheckBox(CCD_SysParCommonEditor);
        Ckb_Offline->setObjectName(QString::fromUtf8("Ckb_Offline"));

        horizontalLayout->addWidget(Ckb_Offline);

        Btn_Sure = new QPushButton(CCD_SysParCommonEditor);
        Btn_Sure->setObjectName(QString::fromUtf8("Btn_Sure"));

        horizontalLayout->addWidget(Btn_Sure);

        Btn_Cancel = new QPushButton(CCD_SysParCommonEditor);
        Btn_Cancel->setObjectName(QString::fromUtf8("Btn_Cancel"));

        horizontalLayout->addWidget(Btn_Cancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CCD_SysParCommonEditor);

        QMetaObject::connectSlotsByName(CCD_SysParCommonEditor);
    } // setupUi

    void retranslateUi(QDialog *CCD_SysParCommonEditor)
    {
        CCD_SysParCommonEditor->setWindowTitle(QApplication::translate("CCD_SysParCommonEditor", "Dialog", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\256\232\344\275\215\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        Btn_TwoP->setText(QApplication::translate("CCD_SysParCommonEditor", "\344\270\244\347\202\271\345\256\232\344\275\215", 0, QApplication::UnicodeUTF8));
        Btn_ThreeP->setText(QApplication::translate("CCD_SysParCommonEditor", "\344\270\211\347\202\271\345\256\232\344\275\215", 0, QApplication::UnicodeUTF8));
        Btn_FourP->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\233\233\347\202\271\345\256\232\344\275\215", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("CCD_SysParCommonEditor", "\344\270\244\347\202\271\345\256\232\344\275\215\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\256\275\345\272\246", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("CCD_SysParCommonEditor", "\351\253\230\345\272\246", 0, QApplication::UnicodeUTF8));
        Btn_LeftUp->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\267\246\344\270\212\347\202\271", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\216\237\347\202\271\345\217\202\350\200\203\347\202\271", 0, QApplication::UnicodeUTF8));
        Btn_Center->setText(QApplication::translate("CCD_SysParCommonEditor", "\344\270\255\347\202\271", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CCD_SysParCommonEditor", "\346\227\213\350\275\254\344\270\255\345\277\203\345\201\217\347\247\273X", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CCD_SysParCommonEditor", "\346\227\213\350\275\254\344\270\255\345\277\203\345\201\217\347\247\273Y", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CCD_SysParCommonEditor", "\346\227\213\350\275\254\346\234\200\345\244\247\345\200\274", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\273\266\346\227\266\350\257\206\345\210\253\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CCD_SysParCommonEditor", "\351\225\277\345\272\246", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\201\217\345\267\256", 0, QApplication::UnicodeUTF8));
        Btn_ComputeRotate->setText(QApplication::translate("CCD_SysParCommonEditor", "\350\256\241\347\256\227", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CCD_SysParCommonEditor", "\346\227\213\350\275\254\350\241\245\346\255\243", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("CCD_SysParCommonEditor", "\351\230\265\345\210\227\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\210\227\346\225\260", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CCD_SysParCommonEditor", "\350\241\214\346\225\260", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\244\261\350\264\245\346\254\241\346\225\260\346\217\220\351\206\222", 0, QApplication::UnicodeUTF8));
        Array_Enable->setText(QApplication::translate("CCD_SysParCommonEditor", "\351\230\265\345\210\227\346\234\211\346\225\210", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\210\227\351\227\264\351\232\224", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("CCD_SysParCommonEditor", "\350\241\214\351\227\264\351\232\224", 0, QApplication::UnicodeUTF8));
        Ckb_Offline->setText(QApplication::translate("CCD_SysParCommonEditor", "\347\246\273\347\272\277", 0, QApplication::UnicodeUTF8));
        Btn_Sure->setText(QApplication::translate("CCD_SysParCommonEditor", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        Btn_Cancel->setText(QApplication::translate("CCD_SysParCommonEditor", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CCD_SysParCommonEditor: public Ui_CCD_SysParCommonEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_SYSPARCOMMONEDITOR_H
