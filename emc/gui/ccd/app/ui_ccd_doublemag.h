/********************************************************************************
** Form generated from reading UI file 'ccd_doublemag.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_DOUBLEMAG_H
#define UI_CCD_DOUBLEMAG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CCD_DoubleMag
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *Obj_TargetOffsetX;
    QDoubleSpinBox *Obj_TargetOffsetY;
    QPushButton *Btn_SaveInfo;
    QDoubleSpinBox *Auto_PreMoveY;
    QLabel *label_11;
    QLabel *label_4;
    QPushButton *Btn_AutoStart;
    QPushButton *Btn_P1;
    QDoubleSpinBox *Obj_Length;
    QLabel *label;
    QPushButton *Btn_Test;
    QComboBox *Auto_PlatformType;
    QDoubleSpinBox *Auto_PreMoveX;
    QDoubleSpinBox *Auto_IdentifyDelay;
    QPushButton *Btn_P2;
    QLabel *label_3;
    QLabel *label_12;
    QDoubleSpinBox *Auto_PreMoveA;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_2;
    QPushButton *Btn_P4;
    QPushButton *Btn_P3;
    QLabel *label_13;
    QLabel *label_14;
    QHBoxLayout *horizontalLayout_2;
    QFrame *MainFrame1;
    QFrame *MainFrame2;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *Brs_Log;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QDoubleSpinBox *LinE_MagY1;
    QLabel *label_9;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QDoubleSpinBox *LinE_OffsetAngle1;
    QDoubleSpinBox *LinE_MagX1;
    QDoubleSpinBox *LinE_MagY2;
    QDoubleSpinBox *LinE_MagX2;
    QDoubleSpinBox *LinE_OffsetAngle2;
    QLabel *label_10;
    QPushButton *Btn_CalAffineMat;
    QPushButton *Btn_Init;
    QPushButton *Btn_Sure;
    QPushButton *Btn_Cancel;

    void setupUi(QDialog *CCD_DoubleMag)
    {
        if (CCD_DoubleMag->objectName().isEmpty())
            CCD_DoubleMag->setObjectName(QStringLiteral("CCD_DoubleMag"));
        CCD_DoubleMag->resize(890, 501);
        CCD_DoubleMag->setMaximumSize(QSize(1280, 1024));
        verticalLayout = new QVBoxLayout(CCD_DoubleMag);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        Obj_TargetOffsetX = new QDoubleSpinBox(CCD_DoubleMag);
        Obj_TargetOffsetX->setObjectName(QStringLiteral("Obj_TargetOffsetX"));
        Obj_TargetOffsetX->setDecimals(3);
        Obj_TargetOffsetX->setMinimum(-100);
        Obj_TargetOffsetX->setMaximum(100);
        Obj_TargetOffsetX->setValue(0);

        gridLayout_3->addWidget(Obj_TargetOffsetX, 0, 6, 1, 1);

        Obj_TargetOffsetY = new QDoubleSpinBox(CCD_DoubleMag);
        Obj_TargetOffsetY->setObjectName(QStringLiteral("Obj_TargetOffsetY"));
        Obj_TargetOffsetY->setDecimals(3);
        Obj_TargetOffsetY->setMinimum(-100);
        Obj_TargetOffsetY->setMaximum(100);
        Obj_TargetOffsetY->setValue(0);

        gridLayout_3->addWidget(Obj_TargetOffsetY, 1, 6, 1, 1);

        Btn_SaveInfo = new QPushButton(CCD_DoubleMag);
        Btn_SaveInfo->setObjectName(QStringLiteral("Btn_SaveInfo"));

        gridLayout_3->addWidget(Btn_SaveInfo, 0, 14, 1, 1);

        Auto_PreMoveY = new QDoubleSpinBox(CCD_DoubleMag);
        Auto_PreMoveY->setObjectName(QStringLiteral("Auto_PreMoveY"));
        Auto_PreMoveY->setDecimals(3);
        Auto_PreMoveY->setMinimum(-5000);
        Auto_PreMoveY->setMaximum(5000);
        Auto_PreMoveY->setValue(0.5);

        gridLayout_3->addWidget(Auto_PreMoveY, 0, 13, 1, 1);

        label_11 = new QLabel(CCD_DoubleMag);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_11, 1, 8, 1, 1);

        label_4 = new QLabel(CCD_DoubleMag);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_4, 0, 3, 1, 1);

        Btn_AutoStart = new QPushButton(CCD_DoubleMag);
        Btn_AutoStart->setObjectName(QStringLiteral("Btn_AutoStart"));

        gridLayout_3->addWidget(Btn_AutoStart, 1, 14, 1, 1);

        Btn_P1 = new QPushButton(CCD_DoubleMag);
        Btn_P1->setObjectName(QStringLiteral("Btn_P1"));

        gridLayout_3->addWidget(Btn_P1, 0, 1, 1, 1);

        Obj_Length = new QDoubleSpinBox(CCD_DoubleMag);
        Obj_Length->setObjectName(QStringLiteral("Obj_Length"));
        Obj_Length->setDecimals(3);
        Obj_Length->setMinimum(-5000);
        Obj_Length->setMaximum(5000);
        Obj_Length->setValue(3000);

        gridLayout_3->addWidget(Obj_Length, 0, 4, 1, 1);

        label = new QLabel(CCD_DoubleMag);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label, 0, 10, 1, 1);

        Btn_Test = new QPushButton(CCD_DoubleMag);
        Btn_Test->setObjectName(QStringLiteral("Btn_Test"));

        gridLayout_3->addWidget(Btn_Test, 1, 4, 1, 1);

        Auto_PlatformType = new QComboBox(CCD_DoubleMag);
        Auto_PlatformType->setObjectName(QStringLiteral("Auto_PlatformType"));
        Auto_PlatformType->setDuplicatesEnabled(false);

        gridLayout_3->addWidget(Auto_PlatformType, 1, 9, 1, 1);

        Auto_PreMoveX = new QDoubleSpinBox(CCD_DoubleMag);
        Auto_PreMoveX->setObjectName(QStringLiteral("Auto_PreMoveX"));
        Auto_PreMoveX->setDecimals(3);
        Auto_PreMoveX->setMinimum(-5000);
        Auto_PreMoveX->setMaximum(5000);
        Auto_PreMoveX->setValue(0.5);

        gridLayout_3->addWidget(Auto_PreMoveX, 0, 11, 1, 1);

        Auto_IdentifyDelay = new QDoubleSpinBox(CCD_DoubleMag);
        Auto_IdentifyDelay->setObjectName(QStringLiteral("Auto_IdentifyDelay"));
        Auto_IdentifyDelay->setDecimals(3);
        Auto_IdentifyDelay->setMinimum(-5000);
        Auto_IdentifyDelay->setMaximum(5000);
        Auto_IdentifyDelay->setValue(3000);

        gridLayout_3->addWidget(Auto_IdentifyDelay, 0, 9, 1, 1);

        Btn_P2 = new QPushButton(CCD_DoubleMag);
        Btn_P2->setObjectName(QStringLiteral("Btn_P2"));

        gridLayout_3->addWidget(Btn_P2, 0, 2, 1, 1);

        label_3 = new QLabel(CCD_DoubleMag);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_3, 0, 8, 1, 1);

        label_12 = new QLabel(CCD_DoubleMag);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_12, 0, 12, 1, 1);

        Auto_PreMoveA = new QDoubleSpinBox(CCD_DoubleMag);
        Auto_PreMoveA->setObjectName(QStringLiteral("Auto_PreMoveA"));
        Auto_PreMoveA->setDecimals(3);
        Auto_PreMoveA->setMinimum(-5000);
        Auto_PreMoveA->setMaximum(5000);
        Auto_PreMoveA->setValue(0.5);

        gridLayout_3->addWidget(Auto_PreMoveA, 1, 11, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 0, 7, 1, 1);

        label_2 = new QLabel(CCD_DoubleMag);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_2, 1, 10, 1, 1);

        Btn_P4 = new QPushButton(CCD_DoubleMag);
        Btn_P4->setObjectName(QStringLiteral("Btn_P4"));

        gridLayout_3->addWidget(Btn_P4, 1, 1, 1, 1);

        Btn_P3 = new QPushButton(CCD_DoubleMag);
        Btn_P3->setObjectName(QStringLiteral("Btn_P3"));

        gridLayout_3->addWidget(Btn_P3, 1, 2, 1, 1);

        label_13 = new QLabel(CCD_DoubleMag);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_3->addWidget(label_13, 0, 5, 1, 1);

        label_14 = new QLabel(CCD_DoubleMag);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_3->addWidget(label_14, 1, 5, 1, 1);


        verticalLayout->addLayout(gridLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        MainFrame1 = new QFrame(CCD_DoubleMag);
        MainFrame1->setObjectName(QStringLiteral("MainFrame1"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(10);
        sizePolicy.setVerticalStretch(8);
        sizePolicy.setHeightForWidth(MainFrame1->sizePolicy().hasHeightForWidth());
        MainFrame1->setSizePolicy(sizePolicy);
        MainFrame1->setMinimumSize(QSize(432, 324));
        MainFrame1->setMaximumSize(QSize(432, 324));
        MainFrame1->setFrameShape(QFrame::StyledPanel);
        MainFrame1->setFrameShadow(QFrame::Raised);

        horizontalLayout_2->addWidget(MainFrame1);

        MainFrame2 = new QFrame(CCD_DoubleMag);
        MainFrame2->setObjectName(QStringLiteral("MainFrame2"));
        sizePolicy.setHeightForWidth(MainFrame2->sizePolicy().hasHeightForWidth());
        MainFrame2->setSizePolicy(sizePolicy);
        MainFrame2->setMinimumSize(QSize(432, 324));
        MainFrame2->setMaximumSize(QSize(432, 324));
        MainFrame2->setFrameShape(QFrame::StyledPanel);
        MainFrame2->setFrameShadow(QFrame::Raised);

        horizontalLayout_2->addWidget(MainFrame2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Brs_Log = new QPlainTextEdit(CCD_DoubleMag);
        Brs_Log->setObjectName(QStringLiteral("Brs_Log"));
        Brs_Log->setMinimumSize(QSize(432, 0));
        Brs_Log->setMaximumSize(QSize(432, 16777215));
        Brs_Log->setReadOnly(true);

        horizontalLayout->addWidget(Brs_Log);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_5 = new QLabel(CCD_DoubleMag);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        LinE_MagY1 = new QDoubleSpinBox(CCD_DoubleMag);
        LinE_MagY1->setObjectName(QStringLiteral("LinE_MagY1"));
        LinE_MagY1->setDecimals(3);
        LinE_MagY1->setMinimum(-5000);
        LinE_MagY1->setMaximum(5000);

        gridLayout->addWidget(LinE_MagY1, 4, 1, 1, 1);

        label_9 = new QLabel(CCD_DoubleMag);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_9, 4, 2, 1, 1);

        label_6 = new QLabel(CCD_DoubleMag);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        label_7 = new QLabel(CCD_DoubleMag);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        label_8 = new QLabel(CCD_DoubleMag);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_8, 0, 2, 1, 1);

        LinE_OffsetAngle1 = new QDoubleSpinBox(CCD_DoubleMag);
        LinE_OffsetAngle1->setObjectName(QStringLiteral("LinE_OffsetAngle1"));
        LinE_OffsetAngle1->setDecimals(4);
        LinE_OffsetAngle1->setMinimum(-500);
        LinE_OffsetAngle1->setMaximum(500);

        gridLayout->addWidget(LinE_OffsetAngle1, 5, 1, 1, 1);

        LinE_MagX1 = new QDoubleSpinBox(CCD_DoubleMag);
        LinE_MagX1->setObjectName(QStringLiteral("LinE_MagX1"));
        LinE_MagX1->setDecimals(3);
        LinE_MagX1->setMinimum(-5000);
        LinE_MagX1->setMaximum(5000);

        gridLayout->addWidget(LinE_MagX1, 0, 1, 1, 1);

        LinE_MagY2 = new QDoubleSpinBox(CCD_DoubleMag);
        LinE_MagY2->setObjectName(QStringLiteral("LinE_MagY2"));
        LinE_MagY2->setDecimals(3);
        LinE_MagY2->setMinimum(-5000);
        LinE_MagY2->setMaximum(5000);

        gridLayout->addWidget(LinE_MagY2, 4, 3, 1, 1);

        LinE_MagX2 = new QDoubleSpinBox(CCD_DoubleMag);
        LinE_MagX2->setObjectName(QStringLiteral("LinE_MagX2"));
        LinE_MagX2->setDecimals(3);
        LinE_MagX2->setMinimum(-5000);
        LinE_MagX2->setMaximum(5000);

        gridLayout->addWidget(LinE_MagX2, 0, 3, 1, 1);

        LinE_OffsetAngle2 = new QDoubleSpinBox(CCD_DoubleMag);
        LinE_OffsetAngle2->setObjectName(QStringLiteral("LinE_OffsetAngle2"));
        LinE_OffsetAngle2->setDecimals(4);
        LinE_OffsetAngle2->setMinimum(-500);
        LinE_OffsetAngle2->setMaximum(500);

        gridLayout->addWidget(LinE_OffsetAngle2, 5, 3, 1, 1);

        label_10 = new QLabel(CCD_DoubleMag);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_10, 5, 2, 1, 1);

        Btn_CalAffineMat = new QPushButton(CCD_DoubleMag);
        Btn_CalAffineMat->setObjectName(QStringLiteral("Btn_CalAffineMat"));

        gridLayout->addWidget(Btn_CalAffineMat, 4, 6, 1, 1);

        Btn_Init = new QPushButton(CCD_DoubleMag);
        Btn_Init->setObjectName(QStringLiteral("Btn_Init"));

        gridLayout->addWidget(Btn_Init, 4, 7, 1, 1);

        Btn_Sure = new QPushButton(CCD_DoubleMag);
        Btn_Sure->setObjectName(QStringLiteral("Btn_Sure"));

        gridLayout->addWidget(Btn_Sure, 5, 7, 1, 1);

        Btn_Cancel = new QPushButton(CCD_DoubleMag);
        Btn_Cancel->setObjectName(QStringLiteral("Btn_Cancel"));

        gridLayout->addWidget(Btn_Cancel, 5, 6, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CCD_DoubleMag);

        Auto_PlatformType->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(CCD_DoubleMag);
    } // setupUi

    void retranslateUi(QDialog *CCD_DoubleMag)
    {
        CCD_DoubleMag->setWindowTitle(QApplication::translate("CCD_DoubleMag", "Dialog", Q_NULLPTR));
        Btn_SaveInfo->setText(QApplication::translate("CCD_DoubleMag", "\344\277\235\345\255\230\344\277\241\346\201\257", Q_NULLPTR));
        label_11->setText(QApplication::translate("CCD_DoubleMag", "\345\271\263\345\217\260", Q_NULLPTR));
        label_4->setText(QApplication::translate("CCD_DoubleMag", "\351\225\277\345\272\246", Q_NULLPTR));
        Btn_AutoStart->setText(QApplication::translate("CCD_DoubleMag", "\350\207\252\345\212\250\345\255\246\344\271\240", Q_NULLPTR));
        Btn_P1->setText(QApplication::translate("CCD_DoubleMag", "\350\257\206\345\210\2531", Q_NULLPTR));
        label->setText(QApplication::translate("CCD_DoubleMag", "\345\210\235\345\247\213X", Q_NULLPTR));
        Btn_Test->setText(QApplication::translate("CCD_DoubleMag", "\346\265\213\350\257\225", Q_NULLPTR));
        Btn_P2->setText(QApplication::translate("CCD_DoubleMag", "\350\257\206\345\210\2532", Q_NULLPTR));
        label_3->setText(QApplication::translate("CCD_DoubleMag", "\345\273\266\346\227\266", Q_NULLPTR));
        label_12->setText(QApplication::translate("CCD_DoubleMag", "\345\210\235\345\247\213Y", Q_NULLPTR));
        label_2->setText(QApplication::translate("CCD_DoubleMag", "\345\210\235\345\247\213A", Q_NULLPTR));
        Btn_P4->setText(QApplication::translate("CCD_DoubleMag", "\350\257\206\345\210\2534", Q_NULLPTR));
        Btn_P3->setText(QApplication::translate("CCD_DoubleMag", "\350\257\206\345\210\2533", Q_NULLPTR));
        label_13->setText(QApplication::translate("CCD_DoubleMag", "\350\264\264\345\220\210\345\201\217\347\247\273X:", Q_NULLPTR));
        label_14->setText(QApplication::translate("CCD_DoubleMag", "\350\264\264\345\220\210\345\201\217\347\247\273Y:", Q_NULLPTR));
        label_5->setText(QApplication::translate("CCD_DoubleMag", "X", Q_NULLPTR));
        label_9->setText(QApplication::translate("CCD_DoubleMag", "Y", Q_NULLPTR));
        label_6->setText(QApplication::translate("CCD_DoubleMag", "Y", Q_NULLPTR));
        label_7->setText(QApplication::translate("CCD_DoubleMag", "A", Q_NULLPTR));
        label_8->setText(QApplication::translate("CCD_DoubleMag", "X", Q_NULLPTR));
        label_10->setText(QApplication::translate("CCD_DoubleMag", "A", Q_NULLPTR));
        Btn_CalAffineMat->setText(QApplication::translate("CCD_DoubleMag", "\350\256\241\347\256\227", Q_NULLPTR));
        Btn_Init->setText(QApplication::translate("CCD_DoubleMag", "\350\256\276\347\275\256\345\237\272\345\207\206\345\200\274", Q_NULLPTR));
        Btn_Sure->setText(QApplication::translate("CCD_DoubleMag", "\344\277\235\345\255\230\345\271\266\351\200\200\345\207\272", Q_NULLPTR));
        Btn_Cancel->setText(QApplication::translate("CCD_DoubleMag", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_DoubleMag: public Ui_CCD_DoubleMag {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_DOUBLEMAG_H
