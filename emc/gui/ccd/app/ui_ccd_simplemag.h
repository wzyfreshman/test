/********************************************************************************
** Form generated from reading UI file 'ccd_simplemag.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_SIMPLEMAG_H
#define UI_CCD_SIMPLEMAG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCD_SimpleMag
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *Auto_IdentifyDelay;
    QPushButton *Btn_AutoStart;
    QPushButton *Btn_P2;
    QComboBox *Auto_PlatformType;
    QLabel *label_3;
    QPushButton *Btn_Test;
    QPushButton *Btn_P1;
    QPushButton *Btn_SaveInfo;
    QPushButton *Btn_getAbs;
    QLabel *label_11;
    QPushButton *Btn_getMark;
    QPushButton *Btn_P3;
    QPushButton *Btn_P4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QFrame *MainFrame1;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *Brs_Log;
    QGridLayout *gridLayout;
    QDoubleSpinBox *LinE_MagY1;
    QDoubleSpinBox *LinE_OffsetAngle1;
    QDoubleSpinBox *LinE_MagX1;
    QPushButton *Btn_CalAffineMat;
    QPushButton *Btn_Sure;
    QPushButton *Btn_Init;
    QPushButton *Btn_Cancel;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_6;
    QDoubleSpinBox *LinE_OffsetAngle2;
    QDoubleSpinBox *LinE_MagY2;
    QDoubleSpinBox *LinE_MagX2;

    void setupUi(QWidget *CCD_SimpleMag)
    {
        if (CCD_SimpleMag->objectName().isEmpty())
            CCD_SimpleMag->setObjectName(QStringLiteral("CCD_SimpleMag"));
        CCD_SimpleMag->resize(891, 522);
        QFont font;
        font.setPointSize(9);
        CCD_SimpleMag->setFont(font);
        verticalLayout = new QVBoxLayout(CCD_SimpleMag);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        Auto_IdentifyDelay = new QDoubleSpinBox(CCD_SimpleMag);
        Auto_IdentifyDelay->setObjectName(QStringLiteral("Auto_IdentifyDelay"));
        Auto_IdentifyDelay->setDecimals(3);
        Auto_IdentifyDelay->setMinimum(-5000);
        Auto_IdentifyDelay->setMaximum(5000);
        Auto_IdentifyDelay->setValue(3000);

        gridLayout_3->addWidget(Auto_IdentifyDelay, 2, 7, 1, 1);

        Btn_AutoStart = new QPushButton(CCD_SimpleMag);
        Btn_AutoStart->setObjectName(QStringLiteral("Btn_AutoStart"));

        gridLayout_3->addWidget(Btn_AutoStart, 5, 8, 1, 1);

        Btn_P2 = new QPushButton(CCD_SimpleMag);
        Btn_P2->setObjectName(QStringLiteral("Btn_P2"));

        gridLayout_3->addWidget(Btn_P2, 2, 2, 1, 1);

        Auto_PlatformType = new QComboBox(CCD_SimpleMag);
        Auto_PlatformType->setObjectName(QStringLiteral("Auto_PlatformType"));
        Auto_PlatformType->setDuplicatesEnabled(false);

        gridLayout_3->addWidget(Auto_PlatformType, 5, 7, 1, 1);

        label_3 = new QLabel(CCD_SimpleMag);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_3, 2, 6, 1, 1);

        Btn_Test = new QPushButton(CCD_SimpleMag);
        Btn_Test->setObjectName(QStringLiteral("Btn_Test"));

        gridLayout_3->addWidget(Btn_Test, 2, 5, 1, 1);

        Btn_P1 = new QPushButton(CCD_SimpleMag);
        Btn_P1->setObjectName(QStringLiteral("Btn_P1"));

        gridLayout_3->addWidget(Btn_P1, 2, 1, 1, 1);

        Btn_SaveInfo = new QPushButton(CCD_SimpleMag);
        Btn_SaveInfo->setObjectName(QStringLiteral("Btn_SaveInfo"));

        gridLayout_3->addWidget(Btn_SaveInfo, 2, 8, 1, 1);

        Btn_getAbs = new QPushButton(CCD_SimpleMag);
        Btn_getAbs->setObjectName(QStringLiteral("Btn_getAbs"));

        gridLayout_3->addWidget(Btn_getAbs, 5, 5, 1, 1);

        label_11 = new QLabel(CCD_SimpleMag);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_11, 5, 6, 1, 1);

        Btn_getMark = new QPushButton(CCD_SimpleMag);
        Btn_getMark->setObjectName(QStringLiteral("Btn_getMark"));

        gridLayout_3->addWidget(Btn_getMark, 2, 4, 1, 1);

        Btn_P3 = new QPushButton(CCD_SimpleMag);
        Btn_P3->setObjectName(QStringLiteral("Btn_P3"));

        gridLayout_3->addWidget(Btn_P3, 5, 1, 1, 1);

        Btn_P4 = new QPushButton(CCD_SimpleMag);
        Btn_P4->setObjectName(QStringLiteral("Btn_P4"));

        gridLayout_3->addWidget(Btn_P4, 5, 2, 1, 1);


        verticalLayout->addLayout(gridLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(178, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        MainFrame1 = new QFrame(CCD_SimpleMag);
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

        horizontalSpacer_2 = new QSpacerItem(178, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Brs_Log = new QPlainTextEdit(CCD_SimpleMag);
        Brs_Log->setObjectName(QStringLiteral("Brs_Log"));
        Brs_Log->setMinimumSize(QSize(432, 0));
        Brs_Log->setMaximumSize(QSize(432, 16777215));
        Brs_Log->setReadOnly(true);

        horizontalLayout->addWidget(Brs_Log);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        LinE_MagY1 = new QDoubleSpinBox(CCD_SimpleMag);
        LinE_MagY1->setObjectName(QStringLiteral("LinE_MagY1"));
        LinE_MagY1->setDecimals(3);
        LinE_MagY1->setMinimum(-5000);
        LinE_MagY1->setMaximum(5000);

        gridLayout->addWidget(LinE_MagY1, 4, 1, 1, 1);

        LinE_OffsetAngle1 = new QDoubleSpinBox(CCD_SimpleMag);
        LinE_OffsetAngle1->setObjectName(QStringLiteral("LinE_OffsetAngle1"));
        LinE_OffsetAngle1->setDecimals(4);
        LinE_OffsetAngle1->setMinimum(-500);
        LinE_OffsetAngle1->setMaximum(500);

        gridLayout->addWidget(LinE_OffsetAngle1, 5, 1, 1, 1);

        LinE_MagX1 = new QDoubleSpinBox(CCD_SimpleMag);
        LinE_MagX1->setObjectName(QStringLiteral("LinE_MagX1"));
        LinE_MagX1->setDecimals(3);
        LinE_MagX1->setMinimum(-5000);
        LinE_MagX1->setMaximum(5000);

        gridLayout->addWidget(LinE_MagX1, 0, 1, 1, 1);

        Btn_CalAffineMat = new QPushButton(CCD_SimpleMag);
        Btn_CalAffineMat->setObjectName(QStringLiteral("Btn_CalAffineMat"));

        gridLayout->addWidget(Btn_CalAffineMat, 4, 5, 1, 1);

        Btn_Sure = new QPushButton(CCD_SimpleMag);
        Btn_Sure->setObjectName(QStringLiteral("Btn_Sure"));

        gridLayout->addWidget(Btn_Sure, 5, 6, 1, 1);

        Btn_Init = new QPushButton(CCD_SimpleMag);
        Btn_Init->setObjectName(QStringLiteral("Btn_Init"));

        gridLayout->addWidget(Btn_Init, 4, 6, 1, 1);

        Btn_Cancel = new QPushButton(CCD_SimpleMag);
        Btn_Cancel->setObjectName(QStringLiteral("Btn_Cancel"));

        gridLayout->addWidget(Btn_Cancel, 5, 5, 1, 1);

        label_5 = new QLabel(CCD_SimpleMag);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        label_7 = new QLabel(CCD_SimpleMag);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        label_6 = new QLabel(CCD_SimpleMag);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        LinE_OffsetAngle2 = new QDoubleSpinBox(CCD_SimpleMag);
        LinE_OffsetAngle2->setObjectName(QStringLiteral("LinE_OffsetAngle2"));
        LinE_OffsetAngle2->setDecimals(4);
        LinE_OffsetAngle2->setMinimum(-500);
        LinE_OffsetAngle2->setMaximum(500);

        gridLayout->addWidget(LinE_OffsetAngle2, 5, 2, 1, 1);

        LinE_MagY2 = new QDoubleSpinBox(CCD_SimpleMag);
        LinE_MagY2->setObjectName(QStringLiteral("LinE_MagY2"));
        LinE_MagY2->setDecimals(3);
        LinE_MagY2->setMinimum(-5000);
        LinE_MagY2->setMaximum(5000);

        gridLayout->addWidget(LinE_MagY2, 4, 2, 1, 1);

        LinE_MagX2 = new QDoubleSpinBox(CCD_SimpleMag);
        LinE_MagX2->setObjectName(QStringLiteral("LinE_MagX2"));
        LinE_MagX2->setDecimals(3);
        LinE_MagX2->setMinimum(-5000);
        LinE_MagX2->setMaximum(5000);

        gridLayout->addWidget(LinE_MagX2, 0, 2, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CCD_SimpleMag);

        Auto_PlatformType->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(CCD_SimpleMag);
    } // setupUi

    void retranslateUi(QWidget *CCD_SimpleMag)
    {
        CCD_SimpleMag->setWindowTitle(QApplication::translate("CCD_SimpleMag", "Form", Q_NULLPTR));
        Btn_AutoStart->setText(QApplication::translate("CCD_SimpleMag", "\350\207\252\345\212\250\345\255\246\344\271\240", Q_NULLPTR));
        Btn_P2->setText(QApplication::translate("CCD_SimpleMag", "\350\257\206\345\210\2532", Q_NULLPTR));
        label_3->setText(QApplication::translate("CCD_SimpleMag", "\345\273\266\346\227\266", Q_NULLPTR));
        Btn_Test->setText(QApplication::translate("CCD_SimpleMag", "\346\265\213\350\257\225", Q_NULLPTR));
        Btn_P1->setText(QApplication::translate("CCD_SimpleMag", "\350\257\206\345\210\2531", Q_NULLPTR));
        Btn_SaveInfo->setText(QApplication::translate("CCD_SimpleMag", "\344\277\235\345\255\230\344\277\241\346\201\257", Q_NULLPTR));
        Btn_getAbs->setText(QApplication::translate("CCD_SimpleMag", "\350\216\267\345\217\226\345\235\220\346\240\207", Q_NULLPTR));
        label_11->setText(QApplication::translate("CCD_SimpleMag", "\345\271\263\345\217\260", Q_NULLPTR));
        Btn_getMark->setText(QApplication::translate("CCD_SimpleMag", "\350\216\267\345\217\226\345\217\202\350\200\203", Q_NULLPTR));
        Btn_P3->setText(QApplication::translate("CCD_SimpleMag", "\350\257\206\345\210\2534", Q_NULLPTR));
        Btn_P4->setText(QApplication::translate("CCD_SimpleMag", "\350\257\206\345\210\2533", Q_NULLPTR));
        Btn_CalAffineMat->setText(QApplication::translate("CCD_SimpleMag", "\350\256\241\347\256\227", Q_NULLPTR));
        Btn_Sure->setText(QApplication::translate("CCD_SimpleMag", "\344\277\235\345\255\230\345\271\266\351\200\200\345\207\272", Q_NULLPTR));
        Btn_Init->setText(QApplication::translate("CCD_SimpleMag", "\350\256\276\347\275\256\345\237\272\345\207\206\345\200\274", Q_NULLPTR));
        Btn_Cancel->setText(QApplication::translate("CCD_SimpleMag", "\345\217\226\346\266\210", Q_NULLPTR));
        label_5->setText(QApplication::translate("CCD_SimpleMag", "X", Q_NULLPTR));
        label_7->setText(QApplication::translate("CCD_SimpleMag", "A", Q_NULLPTR));
        label_6->setText(QApplication::translate("CCD_SimpleMag", "Y", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_SimpleMag: public Ui_CCD_SimpleMag {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_SIMPLEMAG_H
