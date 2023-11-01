/********************************************************************************
** Form generated from reading UI file 'best_testitem.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEST_TESTITEM_H
#define UI_BEST_TESTITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Best_TestItem
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *Item_Id;
    QLabel *open;
    QLabel *cap;
    QLabel *key;
    QLabel *resliner;
    QLabel *resinsulate;

    void setupUi(QWidget *Best_TestItem)
    {
        if (Best_TestItem->objectName().isEmpty())
            Best_TestItem->setObjectName(QString::fromUtf8("Best_TestItem"));
        Best_TestItem->resize(311, 312);
        Best_TestItem->setMinimumSize(QSize(80, 80));
        Best_TestItem->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,255);"));
        verticalLayout = new QVBoxLayout(Best_TestItem);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 0, 0, 0);
        Item_Id = new QLabel(Best_TestItem);
        Item_Id->setObjectName(QString::fromUtf8("Item_Id"));

        verticalLayout->addWidget(Item_Id);

        open = new QLabel(Best_TestItem);
        open->setObjectName(QString::fromUtf8("open"));

        verticalLayout->addWidget(open);

        cap = new QLabel(Best_TestItem);
        cap->setObjectName(QString::fromUtf8("cap"));

        verticalLayout->addWidget(cap);

        key = new QLabel(Best_TestItem);
        key->setObjectName(QString::fromUtf8("key"));

        verticalLayout->addWidget(key);

        resliner = new QLabel(Best_TestItem);
        resliner->setObjectName(QString::fromUtf8("resliner"));

        verticalLayout->addWidget(resliner);

        resinsulate = new QLabel(Best_TestItem);
        resinsulate->setObjectName(QString::fromUtf8("resinsulate"));

        verticalLayout->addWidget(resinsulate);


        retranslateUi(Best_TestItem);

        QMetaObject::connectSlotsByName(Best_TestItem);
    } // setupUi

    void retranslateUi(QWidget *Best_TestItem)
    {
        Best_TestItem->setWindowTitle(QApplication::translate("Best_TestItem", "Form", 0, QApplication::UnicodeUTF8));
        Item_Id->setText(QApplication::translate("Best_TestItem", "TextLabel", 0, QApplication::UnicodeUTF8));
        open->setText(QApplication::translate("Best_TestItem", "\344\272\222\347\224\265\345\256\271", 0, QApplication::UnicodeUTF8));
        cap->setText(QApplication::translate("Best_TestItem", "\350\207\252\347\224\265\345\256\271", 0, QApplication::UnicodeUTF8));
        key->setText(QApplication::translate("Best_TestItem", "\344\272\244\345\217\211\346\214\211\351\224\256", 0, QApplication::UnicodeUTF8));
        resliner->setText(QApplication::translate("Best_TestItem", "\347\272\277\346\200\247\347\224\265\351\230\273", 0, QApplication::UnicodeUTF8));
        resinsulate->setText(QApplication::translate("Best_TestItem", "\347\273\235\347\274\230\347\224\265\351\230\273", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Best_TestItem: public Ui_Best_TestItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEST_TESTITEM_H
