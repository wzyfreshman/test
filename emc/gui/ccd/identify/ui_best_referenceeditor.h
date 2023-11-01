/********************************************************************************
** Form generated from reading UI file 'best_referenceeditor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEST_REFERENCEEDITOR_H
#define UI_BEST_REFERENCEEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Best_ReferenceEditor
{
public:
    QVBoxLayout *verticalLayout_6;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QTableWidget *reslinertable;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *ResLinerRead;
    QPushButton *ResLinerAvg;
    QSpacerItem *horizontalSpacer_2;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_5;
    QTableWidget *resinsulatetable;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *ResInsulateRead;
    QPushButton *ResInsulateAvg;
    QSpacerItem *horizontalSpacer_4;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *opentable;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *OpenRead;
    QPushButton *OpenAvg;
    QSpacerItem *horizontalSpacer_6;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *captable;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *CapRead;
    QPushButton *CapAvg;
    QSpacerItem *horizontalSpacer_8;
    QWidget *tab_5;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *keytable;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *KeyRead;
    QPushButton *KeyAvg;
    QSpacerItem *horizontalSpacer_10;
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *Btn_Cancel;
    QPushButton *Btn_Sure;
    QPushButton *Btn_CCD;
    QPushButton *Btn_Go;
    QDoubleSpinBox *Value_Z;
    QSpinBox *Piece_Index;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *Btn_SetZvalue;
    QPushButton *Btn_Reset;
    QPushButton *Btn_OpenHandle;

    void setupUi(QDialog *Best_ReferenceEditor)
    {
        if (Best_ReferenceEditor->objectName().isEmpty())
            Best_ReferenceEditor->setObjectName(QString::fromUtf8("Best_ReferenceEditor"));
        Best_ReferenceEditor->resize(1013, 748);
        verticalLayout_6 = new QVBoxLayout(Best_ReferenceEditor);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        tabWidget = new QTabWidget(Best_ReferenceEditor);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        reslinertable = new QTableWidget(tab);
        reslinertable->setObjectName(QString::fromUtf8("reslinertable"));

        verticalLayout->addWidget(reslinertable);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ResLinerRead = new QPushButton(tab);
        ResLinerRead->setObjectName(QString::fromUtf8("ResLinerRead"));

        horizontalLayout->addWidget(ResLinerRead);

        ResLinerAvg = new QPushButton(tab);
        ResLinerAvg->setObjectName(QString::fromUtf8("ResLinerAvg"));

        horizontalLayout->addWidget(ResLinerAvg);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_5 = new QVBoxLayout(tab_2);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        resinsulatetable = new QTableWidget(tab_2);
        resinsulatetable->setObjectName(QString::fromUtf8("resinsulatetable"));

        verticalLayout_5->addWidget(resinsulatetable);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        ResInsulateRead = new QPushButton(tab_2);
        ResInsulateRead->setObjectName(QString::fromUtf8("ResInsulateRead"));

        horizontalLayout_2->addWidget(ResInsulateRead);

        ResInsulateAvg = new QPushButton(tab_2);
        ResInsulateAvg->setObjectName(QString::fromUtf8("ResInsulateAvg"));

        horizontalLayout_2->addWidget(ResInsulateAvg);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout_5->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        opentable = new QTableWidget(tab_3);
        opentable->setObjectName(QString::fromUtf8("opentable"));

        verticalLayout_4->addWidget(opentable);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        OpenRead = new QPushButton(tab_3);
        OpenRead->setObjectName(QString::fromUtf8("OpenRead"));

        horizontalLayout_3->addWidget(OpenRead);

        OpenAvg = new QPushButton(tab_3);
        OpenAvg->setObjectName(QString::fromUtf8("OpenAvg"));

        horizontalLayout_3->addWidget(OpenAvg);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        verticalLayout_4->addLayout(horizontalLayout_3);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_3 = new QVBoxLayout(tab_4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        captable = new QTableWidget(tab_4);
        captable->setObjectName(QString::fromUtf8("captable"));

        verticalLayout_3->addWidget(captable);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        CapRead = new QPushButton(tab_4);
        CapRead->setObjectName(QString::fromUtf8("CapRead"));

        horizontalLayout_4->addWidget(CapRead);

        CapAvg = new QPushButton(tab_4);
        CapAvg->setObjectName(QString::fromUtf8("CapAvg"));

        horizontalLayout_4->addWidget(CapAvg);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);


        verticalLayout_3->addLayout(horizontalLayout_4);

        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        verticalLayout_2 = new QVBoxLayout(tab_5);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        keytable = new QTableWidget(tab_5);
        keytable->setObjectName(QString::fromUtf8("keytable"));

        verticalLayout_2->addWidget(keytable);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_9);

        KeyRead = new QPushButton(tab_5);
        KeyRead->setObjectName(QString::fromUtf8("KeyRead"));

        horizontalLayout_5->addWidget(KeyRead);

        KeyAvg = new QPushButton(tab_5);
        KeyAvg->setObjectName(QString::fromUtf8("KeyAvg"));

        horizontalLayout_5->addWidget(KeyAvg);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_10);


        verticalLayout_2->addLayout(horizontalLayout_5);

        tabWidget->addTab(tab_5, QString());

        verticalLayout_6->addWidget(tabWidget);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(Best_ReferenceEditor);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        Btn_Cancel = new QPushButton(Best_ReferenceEditor);
        Btn_Cancel->setObjectName(QString::fromUtf8("Btn_Cancel"));

        gridLayout->addWidget(Btn_Cancel, 2, 5, 1, 1);

        Btn_Sure = new QPushButton(Best_ReferenceEditor);
        Btn_Sure->setObjectName(QString::fromUtf8("Btn_Sure"));

        gridLayout->addWidget(Btn_Sure, 2, 6, 1, 1);

        Btn_CCD = new QPushButton(Best_ReferenceEditor);
        Btn_CCD->setObjectName(QString::fromUtf8("Btn_CCD"));

        gridLayout->addWidget(Btn_CCD, 1, 0, 1, 1);

        Btn_Go = new QPushButton(Best_ReferenceEditor);
        Btn_Go->setObjectName(QString::fromUtf8("Btn_Go"));

        gridLayout->addWidget(Btn_Go, 1, 2, 1, 1);

        Value_Z = new QDoubleSpinBox(Best_ReferenceEditor);
        Value_Z->setObjectName(QString::fromUtf8("Value_Z"));
        Value_Z->setReadOnly(true);
        Value_Z->setDecimals(3);
        Value_Z->setMinimum(-1e+07);
        Value_Z->setMaximum(1e+07);

        gridLayout->addWidget(Value_Z, 0, 1, 1, 1);

        Piece_Index = new QSpinBox(Best_ReferenceEditor);
        Piece_Index->setObjectName(QString::fromUtf8("Piece_Index"));

        gridLayout->addWidget(Piece_Index, 0, 2, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_11, 1, 4, 1, 1);

        Btn_SetZvalue = new QPushButton(Best_ReferenceEditor);
        Btn_SetZvalue->setObjectName(QString::fromUtf8("Btn_SetZvalue"));

        gridLayout->addWidget(Btn_SetZvalue, 1, 1, 1, 1);

        Btn_Reset = new QPushButton(Best_ReferenceEditor);
        Btn_Reset->setObjectName(QString::fromUtf8("Btn_Reset"));

        gridLayout->addWidget(Btn_Reset, 0, 3, 1, 1);

        Btn_OpenHandle = new QPushButton(Best_ReferenceEditor);
        Btn_OpenHandle->setObjectName(QString::fromUtf8("Btn_OpenHandle"));

        gridLayout->addWidget(Btn_OpenHandle, 1, 3, 1, 1);


        verticalLayout_6->addLayout(gridLayout);


        retranslateUi(Best_ReferenceEditor);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Best_ReferenceEditor);
    } // setupUi

    void retranslateUi(QDialog *Best_ReferenceEditor)
    {
        Best_ReferenceEditor->setWindowTitle(QApplication::translate("Best_ReferenceEditor", "Dialog", 0, QApplication::UnicodeUTF8));
        ResLinerRead->setText(QApplication::translate("Best_ReferenceEditor", "\350\257\273\345\217\226", 0, QApplication::UnicodeUTF8));
        ResLinerAvg->setText(QApplication::translate("Best_ReferenceEditor", "\345\271\263\345\235\207", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Best_ReferenceEditor", "\347\272\277\346\200\247\347\224\265\351\230\273", 0, QApplication::UnicodeUTF8));
        ResInsulateRead->setText(QApplication::translate("Best_ReferenceEditor", "\350\257\273\345\217\226", 0, QApplication::UnicodeUTF8));
        ResInsulateAvg->setText(QApplication::translate("Best_ReferenceEditor", "\345\271\263\345\235\207", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Best_ReferenceEditor", "\347\273\235\347\274\230\347\224\265\351\230\273", 0, QApplication::UnicodeUTF8));
        OpenRead->setText(QApplication::translate("Best_ReferenceEditor", "\350\257\273\345\217\226", 0, QApplication::UnicodeUTF8));
        OpenAvg->setText(QApplication::translate("Best_ReferenceEditor", "\345\271\263\345\235\207", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Best_ReferenceEditor", "\344\272\222\347\224\265\345\256\271", 0, QApplication::UnicodeUTF8));
        CapRead->setText(QApplication::translate("Best_ReferenceEditor", "\350\257\273\345\217\226", 0, QApplication::UnicodeUTF8));
        CapAvg->setText(QApplication::translate("Best_ReferenceEditor", "\345\271\263\345\235\207", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("Best_ReferenceEditor", "\350\207\252\347\224\265\345\256\271", 0, QApplication::UnicodeUTF8));
        KeyRead->setText(QApplication::translate("Best_ReferenceEditor", "\350\257\273\345\217\226", 0, QApplication::UnicodeUTF8));
        KeyAvg->setText(QApplication::translate("Best_ReferenceEditor", "\345\271\263\345\235\207", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("Best_ReferenceEditor", "\344\272\244\345\217\211\346\214\211\351\224\256", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Best_ReferenceEditor", "Z\350\275\264\345\235\220\346\240\207:", 0, QApplication::UnicodeUTF8));
        Btn_Cancel->setText(QApplication::translate("Best_ReferenceEditor", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Btn_Sure->setText(QApplication::translate("Best_ReferenceEditor", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        Btn_CCD->setText(QApplication::translate("Best_ReferenceEditor", "CCD\345\256\232\344\275\215", 0, QApplication::UnicodeUTF8));
        Btn_Go->setText(QApplication::translate("Best_ReferenceEditor", "Go", 0, QApplication::UnicodeUTF8));
        Btn_SetZvalue->setText(QApplication::translate("Best_ReferenceEditor", "\350\256\276\345\256\232Z\350\275\264\345\235\220\346\240\207", 0, QApplication::UnicodeUTF8));
        Btn_Reset->setText(QApplication::translate("Best_ReferenceEditor", "\347\255\211\345\276\205\345\244\215\344\275\215", 0, QApplication::UnicodeUTF8));
        Btn_OpenHandle->setText(QApplication::translate("Best_ReferenceEditor", "\346\211\223\345\274\200\346\211\213\350\275\256", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Best_ReferenceEditor: public Ui_Best_ReferenceEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEST_REFERENCEEDITOR_H
