/********************************************************************************
** Form generated from reading UI file 'ccd_magviewdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_MAGVIEWDLG_H
#define UI_CCD_MAGVIEWDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CCD_MagViewDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QFrame *line;

    void setupUi(QDialog *CCD_MagViewDlg)
    {
        if (CCD_MagViewDlg->objectName().isEmpty())
            CCD_MagViewDlg->setObjectName(QStringLiteral("CCD_MagViewDlg"));
        CCD_MagViewDlg->resize(658, 530);
        verticalLayout_2 = new QVBoxLayout(CCD_MagViewDlg);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        frame = new QFrame(CCD_MagViewDlg);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMinimumSize(QSize(640, 512));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        line = new QFrame(frame);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);


        verticalLayout_2->addWidget(frame);


        retranslateUi(CCD_MagViewDlg);

        QMetaObject::connectSlotsByName(CCD_MagViewDlg);
    } // setupUi

    void retranslateUi(QDialog *CCD_MagViewDlg)
    {
        CCD_MagViewDlg->setWindowTitle(QApplication::translate("CCD_MagViewDlg", "MagView", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_MagViewDlg: public Ui_CCD_MagViewDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_MAGVIEWDLG_H
