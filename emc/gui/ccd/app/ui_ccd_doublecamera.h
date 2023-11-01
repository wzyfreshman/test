/********************************************************************************
** Form generated from reading UI file 'ccd_doublecamera.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_DOUBLECAMERA_H
#define UI_CCD_DOUBLECAMERA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CCD_DoubleCamera
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *Btn_IdDestMark2;
    QPushButton *Btn_IdDestMark1;
    QHBoxLayout *horizontalLayout_2;
    QFrame *MainFrame1;
    QFrame *MainFrame2;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *Brs_Log;
    QGridLayout *gridLayout;
    QPushButton *Btn_RecInitPos;
    QPushButton *Btn_Cancel;
    QPushButton *Btn_Sure;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *CCD_DoubleCamera)
    {
        if (CCD_DoubleCamera->objectName().isEmpty())
            CCD_DoubleCamera->setObjectName(QStringLiteral("CCD_DoubleCamera"));
        CCD_DoubleCamera->resize(885, 430);
        CCD_DoubleCamera->setMaximumSize(QSize(1280, 1024));
        verticalLayout = new QVBoxLayout(CCD_DoubleCamera);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        Btn_IdDestMark2 = new QPushButton(CCD_DoubleCamera);
        Btn_IdDestMark2->setObjectName(QStringLiteral("Btn_IdDestMark2"));

        gridLayout_2->addWidget(Btn_IdDestMark2, 0, 1, 1, 1);

        Btn_IdDestMark1 = new QPushButton(CCD_DoubleCamera);
        Btn_IdDestMark1->setObjectName(QStringLiteral("Btn_IdDestMark1"));

        gridLayout_2->addWidget(Btn_IdDestMark1, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        MainFrame1 = new QFrame(CCD_DoubleCamera);
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

        MainFrame2 = new QFrame(CCD_DoubleCamera);
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
        Brs_Log = new QPlainTextEdit(CCD_DoubleCamera);
        Brs_Log->setObjectName(QStringLiteral("Brs_Log"));
        Brs_Log->setMinimumSize(QSize(432, 0));
        Brs_Log->setMaximumSize(QSize(432, 16777215));
        Brs_Log->setReadOnly(true);

        horizontalLayout->addWidget(Brs_Log);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        Btn_RecInitPos = new QPushButton(CCD_DoubleCamera);
        Btn_RecInitPos->setObjectName(QStringLiteral("Btn_RecInitPos"));

        gridLayout->addWidget(Btn_RecInitPos, 0, 2, 1, 1);

        Btn_Cancel = new QPushButton(CCD_DoubleCamera);
        Btn_Cancel->setObjectName(QStringLiteral("Btn_Cancel"));

        gridLayout->addWidget(Btn_Cancel, 1, 1, 1, 1);

        Btn_Sure = new QPushButton(CCD_DoubleCamera);
        Btn_Sure->setObjectName(QStringLiteral("Btn_Sure"));

        gridLayout->addWidget(Btn_Sure, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CCD_DoubleCamera);

        QMetaObject::connectSlotsByName(CCD_DoubleCamera);
    } // setupUi

    void retranslateUi(QDialog *CCD_DoubleCamera)
    {
        CCD_DoubleCamera->setWindowTitle(QApplication::translate("CCD_DoubleCamera", "Dialog", Q_NULLPTR));
        Btn_IdDestMark2->setText(QApplication::translate("CCD_DoubleCamera", "\350\257\206\345\210\2532", Q_NULLPTR));
        Btn_IdDestMark1->setText(QApplication::translate("CCD_DoubleCamera", "\350\257\206\345\210\2531", Q_NULLPTR));
        Btn_RecInitPos->setText(QApplication::translate("CCD_DoubleCamera", "\350\256\276\347\275\256\345\237\272\345\207\206\345\200\274", Q_NULLPTR));
        Btn_Cancel->setText(QApplication::translate("CCD_DoubleCamera", "\345\217\226\346\266\210", Q_NULLPTR));
        Btn_Sure->setText(QApplication::translate("CCD_DoubleCamera", "\344\277\235\345\255\230\345\271\266\351\200\200\345\207\272", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_DoubleCamera: public Ui_CCD_DoubleCamera {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_DOUBLECAMERA_H
