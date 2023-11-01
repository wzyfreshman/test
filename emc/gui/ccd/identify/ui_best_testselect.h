/********************************************************************************
** Form generated from reading UI file 'best_testselect.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEST_TESTSELECT_H
#define UI_BEST_TESTSELECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Best_TestSelect
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QCheckBox *res_liner_rcd;
    QCheckBox *cap_rcd;
    QCheckBox *open_rcd;
    QCheckBox *cap_select;
    QCheckBox *res_liner_select;
    QCheckBox *open_select;
    QCheckBox *res_insulate_select;
    QCheckBox *key_selet;
    QCheckBox *res_insulate_rcd;
    QCheckBox *key_rcd;
    QCheckBox *auto_recheck;
    QCheckBox *auto_mark;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *Btn_Cancel;
    QPushButton *Btn_Sure;

    void setupUi(QDialog *Best_TestSelect)
    {
        if (Best_TestSelect->objectName().isEmpty())
            Best_TestSelect->setObjectName(QString::fromUtf8("Best_TestSelect"));
        Best_TestSelect->resize(307, 320);
        verticalLayout = new QVBoxLayout(Best_TestSelect);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        res_liner_rcd = new QCheckBox(Best_TestSelect);
        res_liner_rcd->setObjectName(QString::fromUtf8("res_liner_rcd"));

        gridLayout->addWidget(res_liner_rcd, 0, 1, 1, 1);

        cap_rcd = new QCheckBox(Best_TestSelect);
        cap_rcd->setObjectName(QString::fromUtf8("cap_rcd"));

        gridLayout->addWidget(cap_rcd, 3, 1, 1, 1);

        open_rcd = new QCheckBox(Best_TestSelect);
        open_rcd->setObjectName(QString::fromUtf8("open_rcd"));

        gridLayout->addWidget(open_rcd, 2, 1, 1, 1);

        cap_select = new QCheckBox(Best_TestSelect);
        cap_select->setObjectName(QString::fromUtf8("cap_select"));

        gridLayout->addWidget(cap_select, 3, 0, 1, 1);

        res_liner_select = new QCheckBox(Best_TestSelect);
        res_liner_select->setObjectName(QString::fromUtf8("res_liner_select"));

        gridLayout->addWidget(res_liner_select, 0, 0, 1, 1);

        open_select = new QCheckBox(Best_TestSelect);
        open_select->setObjectName(QString::fromUtf8("open_select"));

        gridLayout->addWidget(open_select, 2, 0, 1, 1);

        res_insulate_select = new QCheckBox(Best_TestSelect);
        res_insulate_select->setObjectName(QString::fromUtf8("res_insulate_select"));

        gridLayout->addWidget(res_insulate_select, 1, 0, 1, 1);

        key_selet = new QCheckBox(Best_TestSelect);
        key_selet->setObjectName(QString::fromUtf8("key_selet"));

        gridLayout->addWidget(key_selet, 4, 0, 1, 1);

        res_insulate_rcd = new QCheckBox(Best_TestSelect);
        res_insulate_rcd->setObjectName(QString::fromUtf8("res_insulate_rcd"));

        gridLayout->addWidget(res_insulate_rcd, 1, 1, 1, 1);

        key_rcd = new QCheckBox(Best_TestSelect);
        key_rcd->setObjectName(QString::fromUtf8("key_rcd"));

        gridLayout->addWidget(key_rcd, 4, 1, 1, 1);

        auto_recheck = new QCheckBox(Best_TestSelect);
        auto_recheck->setObjectName(QString::fromUtf8("auto_recheck"));

        gridLayout->addWidget(auto_recheck, 5, 0, 1, 1);

        auto_mark = new QCheckBox(Best_TestSelect);
        auto_mark->setObjectName(QString::fromUtf8("auto_mark"));

        gridLayout->addWidget(auto_mark, 5, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Btn_Cancel = new QPushButton(Best_TestSelect);
        Btn_Cancel->setObjectName(QString::fromUtf8("Btn_Cancel"));

        horizontalLayout->addWidget(Btn_Cancel);

        Btn_Sure = new QPushButton(Best_TestSelect);
        Btn_Sure->setObjectName(QString::fromUtf8("Btn_Sure"));

        horizontalLayout->addWidget(Btn_Sure);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Best_TestSelect);

        QMetaObject::connectSlotsByName(Best_TestSelect);
    } // setupUi

    void retranslateUi(QDialog *Best_TestSelect)
    {
        Best_TestSelect->setWindowTitle(QApplication::translate("Best_TestSelect", "Dialog", 0, QApplication::UnicodeUTF8));
        res_liner_rcd->setText(QApplication::translate("Best_TestSelect", "\347\272\277\346\200\247\347\224\265\351\230\273\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        cap_rcd->setText(QApplication::translate("Best_TestSelect", "\350\207\252\347\224\265\345\256\271\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        open_rcd->setText(QApplication::translate("Best_TestSelect", "\344\272\222\347\224\265\345\256\271\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        cap_select->setText(QApplication::translate("Best_TestSelect", "\350\207\252\347\224\265\345\256\271\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        res_liner_select->setText(QApplication::translate("Best_TestSelect", "\347\272\277\346\200\247\347\224\265\351\230\273", 0, QApplication::UnicodeUTF8));
        open_select->setText(QApplication::translate("Best_TestSelect", "\344\272\222\347\224\265\345\256\271\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        res_insulate_select->setText(QApplication::translate("Best_TestSelect", "\347\273\235\347\274\230\347\224\265\351\230\273", 0, QApplication::UnicodeUTF8));
        key_selet->setText(QApplication::translate("Best_TestSelect", "\346\214\211\351\224\256\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        res_insulate_rcd->setText(QApplication::translate("Best_TestSelect", "\347\273\235\347\274\230\347\224\265\351\230\273\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        key_rcd->setText(QApplication::translate("Best_TestSelect", "\346\214\211\351\224\256\350\256\260\345\275\225", 0, QApplication::UnicodeUTF8));
        auto_recheck->setText(QApplication::translate("Best_TestSelect", "\350\207\252\345\212\250\345\244\215\346\265\213", 0, QApplication::UnicodeUTF8));
        auto_mark->setText(QApplication::translate("Best_TestSelect", "\350\207\252\345\212\250\346\211\223\346\240\207", 0, QApplication::UnicodeUTF8));
        Btn_Cancel->setText(QApplication::translate("Best_TestSelect", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Btn_Sure->setText(QApplication::translate("Best_TestSelect", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Best_TestSelect: public Ui_Best_TestSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEST_TESTSELECT_H
