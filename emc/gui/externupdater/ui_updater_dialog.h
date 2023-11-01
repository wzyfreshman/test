/********************************************************************************
** Form generated from reading UI file 'updater_dialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATER_DIALOG_H
#define UI_UPDATER_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Updater_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLineEdit *pathinfo;
    QPushButton *btn_open;
    QTextBrowser *logger;
    QLabel *label_2;

    void setupUi(QDialog *Updater_Dialog)
    {
        if (Updater_Dialog->objectName().isEmpty())
            Updater_Dialog->setObjectName(QString::fromUtf8("Updater_Dialog"));
        Updater_Dialog->resize(443, 329);
        Updater_Dialog->setMinimumSize(QSize(443, 329));
        Updater_Dialog->setMaximumSize(QSize(443, 329));
        verticalLayout = new QVBoxLayout(Updater_Dialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(Updater_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        frame = new QFrame(Updater_Dialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 0, 2, 0);
        pathinfo = new QLineEdit(frame);
        pathinfo->setObjectName(QString::fromUtf8("pathinfo"));
        pathinfo->setReadOnly(true);

        horizontalLayout->addWidget(pathinfo);

        btn_open = new QPushButton(frame);
        btn_open->setObjectName(QString::fromUtf8("btn_open"));

        horizontalLayout->addWidget(btn_open);


        verticalLayout->addWidget(frame);

        logger = new QTextBrowser(Updater_Dialog);
        logger->setObjectName(QString::fromUtf8("logger"));

        verticalLayout->addWidget(logger);

        label_2 = new QLabel(Updater_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);


        retranslateUi(Updater_Dialog);

        QMetaObject::connectSlotsByName(Updater_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Updater_Dialog)
    {
        Updater_Dialog->setWindowTitle(QApplication::translate("Updater_Dialog", "Updater_Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Updater_Dialog", "CNEXT\350\275\257\344\273\266\345\215\207\347\272\247", 0, QApplication::UnicodeUTF8));
        btn_open->setText(QApplication::translate("Updater_Dialog", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Updater_Dialog: public Ui_Updater_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATER_DIALOG_H
