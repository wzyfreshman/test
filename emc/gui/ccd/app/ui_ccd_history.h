/********************************************************************************
** Form generated from reading UI file 'ccd_history.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_HISTORY_H
#define UI_CCD_HISTORY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CCD_History
{
public:
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QFrame *MainFrame1;
    QFrame *MainFrame2;
    QFrame *MainFrame3;
    QFrame *MainFrame4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *imgType;
    QPushButton *Btn_clear;
    QPushButton *pushButton;
    QPushButton *Btn_resetview;
    QPushButton *Btn_saveImg;
    QSpinBox *Img_index;
    QTextBrowser *textBrowser;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *CCD_History)
    {
        if (CCD_History->objectName().isEmpty())
            CCD_History->setObjectName(QStringLiteral("CCD_History"));
        CCD_History->resize(923, 640);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(10);
        sizePolicy.setVerticalStretch(8);
        sizePolicy.setHeightForWidth(CCD_History->sizePolicy().hasHeightForWidth());
        CCD_History->setSizePolicy(sizePolicy);
        CCD_History->setMinimumSize(QSize(923, 640));
        horizontalLayout_2 = new QHBoxLayout(CCD_History);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        MainFrame1 = new QFrame(CCD_History);
        MainFrame1->setObjectName(QStringLiteral("MainFrame1"));
        MainFrame1->setMinimumSize(QSize(360, 300));
        MainFrame1->setMaximumSize(QSize(360, 300));
        MainFrame1->setFrameShape(QFrame::StyledPanel);
        MainFrame1->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(MainFrame1, 0, 0, 1, 1);

        MainFrame2 = new QFrame(CCD_History);
        MainFrame2->setObjectName(QStringLiteral("MainFrame2"));
        MainFrame2->setMinimumSize(QSize(360, 300));
        MainFrame2->setMaximumSize(QSize(360, 300));
        MainFrame2->setFrameShape(QFrame::StyledPanel);
        MainFrame2->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(MainFrame2, 0, 1, 1, 1);

        MainFrame3 = new QFrame(CCD_History);
        MainFrame3->setObjectName(QStringLiteral("MainFrame3"));
        MainFrame3->setMinimumSize(QSize(360, 300));
        MainFrame3->setMaximumSize(QSize(360, 300));
        MainFrame3->setFrameShape(QFrame::StyledPanel);
        MainFrame3->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(MainFrame3, 1, 0, 1, 1);

        MainFrame4 = new QFrame(CCD_History);
        MainFrame4->setObjectName(QStringLiteral("MainFrame4"));
        MainFrame4->setMinimumSize(QSize(360, 300));
        MainFrame4->setMaximumSize(QSize(360, 300));
        MainFrame4->setFrameShape(QFrame::StyledPanel);
        MainFrame4->setFrameShadow(QFrame::Raised);

        gridLayout->addWidget(MainFrame4, 1, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(CCD_History);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        imgType = new QComboBox(CCD_History);
        imgType->setObjectName(QStringLiteral("imgType"));

        horizontalLayout->addWidget(imgType);


        verticalLayout->addLayout(horizontalLayout);

        Btn_clear = new QPushButton(CCD_History);
        Btn_clear->setObjectName(QStringLiteral("Btn_clear"));

        verticalLayout->addWidget(Btn_clear);

        pushButton = new QPushButton(CCD_History);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        Btn_resetview = new QPushButton(CCD_History);
        Btn_resetview->setObjectName(QStringLiteral("Btn_resetview"));

        verticalLayout->addWidget(Btn_resetview);

        Btn_saveImg = new QPushButton(CCD_History);
        Btn_saveImg->setObjectName(QStringLiteral("Btn_saveImg"));

        verticalLayout->addWidget(Btn_saveImg);

        Img_index = new QSpinBox(CCD_History);
        Img_index->setObjectName(QStringLiteral("Img_index"));
        Img_index->setMaximum(3);

        verticalLayout->addWidget(Img_index);

        textBrowser = new QTextBrowser(CCD_History);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(CCD_History);

        QMetaObject::connectSlotsByName(CCD_History);
    } // setupUi

    void retranslateUi(QDialog *CCD_History)
    {
        CCD_History->setWindowTitle(QApplication::translate("CCD_History", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("CCD_History", "\345\216\206\345\217\262\346\227\266\351\227\264", Q_NULLPTR));
        Btn_clear->setText(QApplication::translate("CCD_History", "\346\270\205\351\231\244\345\216\206\345\217\262", Q_NULLPTR));
        pushButton->setText(QApplication::translate("CCD_History", "\346\270\205\351\231\244\346\227\245\345\277\227", Q_NULLPTR));
        Btn_resetview->setText(QApplication::translate("CCD_History", "\346\201\242\345\244\215\350\247\206\345\233\276", Q_NULLPTR));
        Btn_saveImg->setText(QApplication::translate("CCD_History", "\344\277\235\345\255\230\345\233\276\347\211\207", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_History: public Ui_CCD_History {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_HISTORY_H
