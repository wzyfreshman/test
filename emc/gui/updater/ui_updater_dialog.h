/********************************************************************************
** Form generated from reading UI file 'updater_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATER_DIALOG_H
#define UI_UPDATER_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_Updater_Dialog
{
public:
    QTextBrowser *logger;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *Updater_Dialog)
    {
        if (Updater_Dialog->objectName().isEmpty())
            Updater_Dialog->setObjectName(QStringLiteral("Updater_Dialog"));
        Updater_Dialog->resize(443, 329);
        Updater_Dialog->setMinimumSize(QSize(443, 329));
        Updater_Dialog->setMaximumSize(QSize(443, 329));
        logger = new QTextBrowser(Updater_Dialog);
        logger->setObjectName(QStringLiteral("logger"));
        logger->setGeometry(QRect(10, 30, 421, 261));
        label = new QLabel(Updater_Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 411, 16));
        label_2 = new QLabel(Updater_Dialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(280, 300, 151, 16));

        retranslateUi(Updater_Dialog);

        QMetaObject::connectSlotsByName(Updater_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Updater_Dialog)
    {
        Updater_Dialog->setWindowTitle(QApplication::translate("Updater_Dialog", "Updater_Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("Updater_Dialog", "\350\275\257\344\273\266\345\215\207\347\272\247", Q_NULLPTR));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Updater_Dialog: public Ui_Updater_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATER_DIALOG_H
