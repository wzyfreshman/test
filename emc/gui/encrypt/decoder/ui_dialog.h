/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLineEdit *Code1;
    QLineEdit *VerNum;
    QPushButton *sign;
    QLineEdit *SysNum;
    QLabel *label;
    QLineEdit *Code2;
    QLineEdit *Code3;
    QLineEdit *Code4;
    QPushButton *read;
    QLabel *label_2;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(568, 349);
        Code1 = new QLineEdit(Dialog);
        Code1->setObjectName(QStringLiteral("Code1"));
        Code1->setGeometry(QRect(80, 80, 91, 31));
        Code1->setMaxLength(8);
        Code1->setAlignment(Qt::AlignCenter);
        VerNum = new QLineEdit(Dialog);
        VerNum->setObjectName(QStringLiteral("VerNum"));
        VerNum->setGeometry(QRect(180, 40, 91, 31));
        VerNum->setMaxLength(8);
        VerNum->setAlignment(Qt::AlignCenter);
        VerNum->setReadOnly(true);
        sign = new QPushButton(Dialog);
        sign->setObjectName(QStringLiteral("sign"));
        sign->setGeometry(QRect(80, 120, 131, 41));
        SysNum = new QLineEdit(Dialog);
        SysNum->setObjectName(QStringLiteral("SysNum"));
        SysNum->setGeometry(QRect(80, 40, 91, 31));
        SysNum->setInputMethodHints(Qt::ImhNone);
        SysNum->setMaxLength(8);
        SysNum->setAlignment(Qt::AlignCenter);
        SysNum->setReadOnly(true);
        label = new QLabel(Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 30, 66, 14));
        Code2 = new QLineEdit(Dialog);
        Code2->setObjectName(QStringLiteral("Code2"));
        Code2->setGeometry(QRect(180, 80, 91, 31));
        Code2->setMaxLength(8);
        Code2->setAlignment(Qt::AlignCenter);
        Code3 = new QLineEdit(Dialog);
        Code3->setObjectName(QStringLiteral("Code3"));
        Code3->setGeometry(QRect(290, 80, 91, 31));
        Code3->setMaxLength(8);
        Code3->setAlignment(Qt::AlignCenter);
        Code4 = new QLineEdit(Dialog);
        Code4->setObjectName(QStringLiteral("Code4"));
        Code4->setGeometry(QRect(400, 80, 121, 31));
        Code4->setMaxLength(8);
        Code4->setAlignment(Qt::AlignCenter);
        read = new QPushButton(Dialog);
        read->setObjectName(QStringLiteral("read"));
        read->setGeometry(QRect(220, 120, 131, 41));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(300, 50, 221, 16));
        textBrowser = new QTextBrowser(Dialog);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(80, 180, 361, 141));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", Q_NULLPTR));
        Code1->setText(QString());
        VerNum->setInputMask(QString());
        VerNum->setText(QString());
        sign->setText(QApplication::translate("Dialog", "Sure", Q_NULLPTR));
        SysNum->setInputMask(QString());
        SysNum->setText(QString());
        label->setText(QString());
        read->setText(QApplication::translate("Dialog", "Read", Q_NULLPTR));
        label_2->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
