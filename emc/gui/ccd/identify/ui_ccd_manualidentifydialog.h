/********************************************************************************
** Form generated from reading UI file 'ccd_manualidentifydialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_MANUALIDENTIFYDIALOG_H
#define UI_CCD_MANUALIDENTIFYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCD_ManualIdentifyDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CCD_ManualIdentifyDialog)
    {
        if (CCD_ManualIdentifyDialog->objectName().isEmpty())
            CCD_ManualIdentifyDialog->setObjectName(QStringLiteral("CCD_ManualIdentifyDialog"));
        CCD_ManualIdentifyDialog->resize(658, 529);
        verticalLayout = new QVBoxLayout(CCD_ManualIdentifyDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget = new QWidget(CCD_ManualIdentifyDialog);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(640, 480));

        verticalLayout->addWidget(widget);

        buttonBox = new QDialogButtonBox(CCD_ManualIdentifyDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(CCD_ManualIdentifyDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CCD_ManualIdentifyDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CCD_ManualIdentifyDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CCD_ManualIdentifyDialog);
    } // setupUi

    void retranslateUi(QDialog *CCD_ManualIdentifyDialog)
    {
        CCD_ManualIdentifyDialog->setWindowTitle(QApplication::translate("CCD_ManualIdentifyDialog", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_ManualIdentifyDialog: public Ui_CCD_ManualIdentifyDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_MANUALIDENTIFYDIALOG_H
