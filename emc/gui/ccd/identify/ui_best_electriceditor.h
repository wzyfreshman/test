/********************************************************************************
** Form generated from reading UI file 'best_electriceditor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEST_ELECTRICEDITOR_H
#define UI_BEST_ELECTRICEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Best_ElectricEditor
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QFormLayout *formLayout;
    QLabel *label;
    QSpinBox *freq_sel;
    QLabel *label_2;
    QComboBox *vol_sel;
    QLabel *label_3;
    QLabel *label_4;
    QSpinBox *open_min;
    QComboBox *waves_sel;
    QLabel *label_5;
    QSpinBox *res_min;
    QSpinBox *cap_min;
    QSpinBox *open_gain;
    QSpinBox *res_gain;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QWidget *tab_2;
    QFormLayout *formLayout_2;
    QLabel *label_12;
    QSpinBox *open_dlt_max;
    QLabel *label_11;
    QLabel *label_9;
    QLabel *label_13;
    QSpinBox *cap_dlt_min;
    QSpinBox *res_liner_max;
    QSpinBox *res_liner_min;
    QSpinBox *res_dlt_max;
    QSpinBox *open_dlt_min;
    QSpinBox *cap_dlt_max;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_10;
    QLabel *label_16;
    QSpinBox *res_dlt_min;
    QLabel *label_17;
    QSpinBox *res_dlt_short;
    QLabel *label_18;
    QSpinBox *res_dlt_open;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *Btn_Cancel;
    QPushButton *Btn_Sure;

    void setupUi(QDialog *Best_ElectricEditor)
    {
        if (Best_ElectricEditor->objectName().isEmpty())
            Best_ElectricEditor->setObjectName(QString::fromUtf8("Best_ElectricEditor"));
        Best_ElectricEditor->resize(432, 458);
        verticalLayout = new QVBoxLayout(Best_ElectricEditor);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(Best_ElectricEditor);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        formLayout = new QFormLayout(tab);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        freq_sel = new QSpinBox(tab);
        freq_sel->setObjectName(QString::fromUtf8("freq_sel"));
        freq_sel->setMaximum(1000000000);

        formLayout->setWidget(0, QFormLayout::FieldRole, freq_sel);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        vol_sel = new QComboBox(tab);
        vol_sel->setObjectName(QString::fromUtf8("vol_sel"));
        vol_sel->setEditable(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, vol_sel);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        open_min = new QSpinBox(tab);
        open_min->setObjectName(QString::fromUtf8("open_min"));
        open_min->setMaximum(1000000000);

        formLayout->setWidget(3, QFormLayout::FieldRole, open_min);

        waves_sel = new QComboBox(tab);
        waves_sel->setObjectName(QString::fromUtf8("waves_sel"));
        waves_sel->setEditable(true);

        formLayout->setWidget(2, QFormLayout::FieldRole, waves_sel);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_5);

        res_min = new QSpinBox(tab);
        res_min->setObjectName(QString::fromUtf8("res_min"));
        res_min->setMaximum(1000000000);

        formLayout->setWidget(5, QFormLayout::FieldRole, res_min);

        cap_min = new QSpinBox(tab);
        cap_min->setObjectName(QString::fromUtf8("cap_min"));
        cap_min->setMaximum(1000000000);

        formLayout->setWidget(4, QFormLayout::FieldRole, cap_min);

        open_gain = new QSpinBox(tab);
        open_gain->setObjectName(QString::fromUtf8("open_gain"));
        open_gain->setMinimum(1);
        open_gain->setMaximum(16);

        formLayout->setWidget(6, QFormLayout::FieldRole, open_gain);

        res_gain = new QSpinBox(tab);
        res_gain->setObjectName(QString::fromUtf8("res_gain"));
        res_gain->setMinimum(1);
        res_gain->setMaximum(16);

        formLayout->setWidget(7, QFormLayout::FieldRole, res_gain);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_6);

        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_7);

        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_8);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        formLayout_2 = new QFormLayout(tab_2);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label_12 = new QLabel(tab_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_12);

        open_dlt_max = new QSpinBox(tab_2);
        open_dlt_max->setObjectName(QString::fromUtf8("open_dlt_max"));
        open_dlt_max->setMinimum(1);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, open_dlt_max);

        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_11);

        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_9);

        label_13 = new QLabel(tab_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_13);

        cap_dlt_min = new QSpinBox(tab_2);
        cap_dlt_min->setObjectName(QString::fromUtf8("cap_dlt_min"));
        cap_dlt_min->setMinimum(1);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, cap_dlt_min);

        res_liner_max = new QSpinBox(tab_2);
        res_liner_max->setObjectName(QString::fromUtf8("res_liner_max"));
        res_liner_max->setMinimum(1);

        formLayout_2->setWidget(4, QFormLayout::FieldRole, res_liner_max);

        res_liner_min = new QSpinBox(tab_2);
        res_liner_min->setObjectName(QString::fromUtf8("res_liner_min"));
        res_liner_min->setMinimum(1);

        formLayout_2->setWidget(5, QFormLayout::FieldRole, res_liner_min);

        res_dlt_max = new QSpinBox(tab_2);
        res_dlt_max->setObjectName(QString::fromUtf8("res_dlt_max"));
        res_dlt_max->setMinimum(1);
        res_dlt_max->setMaximum(1000000000);

        formLayout_2->setWidget(6, QFormLayout::FieldRole, res_dlt_max);

        open_dlt_min = new QSpinBox(tab_2);
        open_dlt_min->setObjectName(QString::fromUtf8("open_dlt_min"));
        open_dlt_min->setMinimum(1);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, open_dlt_min);

        cap_dlt_max = new QSpinBox(tab_2);
        cap_dlt_max->setObjectName(QString::fromUtf8("cap_dlt_max"));
        cap_dlt_max->setMinimum(1);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, cap_dlt_max);

        label_14 = new QLabel(tab_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_14);

        label_15 = new QLabel(tab_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_15);

        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, label_10);

        label_16 = new QLabel(tab_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        formLayout_2->setWidget(7, QFormLayout::LabelRole, label_16);

        res_dlt_min = new QSpinBox(tab_2);
        res_dlt_min->setObjectName(QString::fromUtf8("res_dlt_min"));
        res_dlt_min->setMinimum(1);
        res_dlt_min->setMaximum(1000000000);

        formLayout_2->setWidget(7, QFormLayout::FieldRole, res_dlt_min);

        label_17 = new QLabel(tab_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        formLayout_2->setWidget(8, QFormLayout::LabelRole, label_17);

        res_dlt_short = new QSpinBox(tab_2);
        res_dlt_short->setObjectName(QString::fromUtf8("res_dlt_short"));
        res_dlt_short->setMinimum(1);
        res_dlt_short->setMaximum(1000000000);

        formLayout_2->setWidget(8, QFormLayout::FieldRole, res_dlt_short);

        label_18 = new QLabel(tab_2);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        formLayout_2->setWidget(9, QFormLayout::LabelRole, label_18);

        res_dlt_open = new QSpinBox(tab_2);
        res_dlt_open->setObjectName(QString::fromUtf8("res_dlt_open"));
        res_dlt_open->setMinimum(1);
        res_dlt_open->setMaximum(1000000000);

        formLayout_2->setWidget(9, QFormLayout::FieldRole, res_dlt_open);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Btn_Cancel = new QPushButton(Best_ElectricEditor);
        Btn_Cancel->setObjectName(QString::fromUtf8("Btn_Cancel"));

        horizontalLayout->addWidget(Btn_Cancel);

        Btn_Sure = new QPushButton(Best_ElectricEditor);
        Btn_Sure->setObjectName(QString::fromUtf8("Btn_Sure"));

        horizontalLayout->addWidget(Btn_Sure);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Best_ElectricEditor);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Best_ElectricEditor);
    } // setupUi

    void retranslateUi(QDialog *Best_ElectricEditor)
    {
        Best_ElectricEditor->setWindowTitle(QApplication::translate("Best_ElectricEditor", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Best_ElectricEditor", "\351\242\221\347\216\207  HZ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Best_ElectricEditor", "\347\224\265\345\216\213  V", 0, QApplication::UnicodeUTF8));
        vol_sel->clear();
        vol_sel->insertItems(0, QStringList()
         << QApplication::translate("Best_ElectricEditor", "5.0V", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Best_ElectricEditor", "2.0V", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("Best_ElectricEditor", "\346\263\242\345\275\242", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Best_ElectricEditor", "\344\272\222\347\224\265\345\256\271\346\234\200\345\260\217\345\200\274", 0, QApplication::UnicodeUTF8));
        waves_sel->clear();
        waves_sel->insertItems(0, QStringList()
         << QApplication::translate("Best_ElectricEditor", "\346\226\271\346\263\242", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Best_ElectricEditor", "\344\270\211\350\247\222\346\263\242", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Best_ElectricEditor", "\346\255\243\345\274\246\346\263\242", 0, QApplication::UnicodeUTF8)
        );
        label_5->setText(QApplication::translate("Best_ElectricEditor", "\347\224\265\351\230\273\346\234\200\345\260\217\345\200\274", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Best_ElectricEditor", "\350\207\252\347\224\265\345\256\271\346\234\200\345\260\217\345\200\274", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Best_ElectricEditor", "\344\272\222\347\224\265\345\256\271\345\242\236\347\233\212", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Best_ElectricEditor", "\347\224\265\351\230\273\345\242\236\347\233\212", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Best_ElectricEditor", "\347\224\265\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Best_ElectricEditor", "\344\272\222\347\224\265\345\256\271\346\255\243\344\277\256\346\255\243\351\207\217  %", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Best_ElectricEditor", "\344\272\222\347\224\265\345\256\271\350\264\237\344\277\256\346\255\243\351\207\217  %", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Best_ElectricEditor", "\350\207\252\347\224\265\345\256\271\346\255\243\344\277\256\346\255\243\351\207\217 %", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Best_ElectricEditor", "\350\207\252\347\224\265\345\256\271\350\264\237\344\277\256\346\255\243\351\207\217 %", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Best_ElectricEditor", "\347\272\277\346\200\247\347\224\265\351\230\273\346\255\243\344\277\256\346\255\243\351\207\217 %", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("Best_ElectricEditor", "\347\272\277\346\200\247\347\224\265\351\230\273\350\264\237\344\277\256\346\255\243\351\207\217 %", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Best_ElectricEditor", "\347\224\265\351\230\273\347\272\277\346\200\247\344\270\212\351\231\220", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("Best_ElectricEditor", "\347\224\265\351\230\273\347\272\277\346\200\247\344\270\213\351\231\220", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("Best_ElectricEditor", "\347\224\265\351\230\273\347\237\255\350\267\257\344\270\212\351\231\220", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Best_ElectricEditor", "\347\224\265\351\230\273\345\274\200\350\267\257\344\270\213\351\231\220", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Best_ElectricEditor", "\350\257\257\345\267\256\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        Btn_Cancel->setText(QApplication::translate("Best_ElectricEditor", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Btn_Sure->setText(QApplication::translate("Best_ElectricEditor", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Best_ElectricEditor: public Ui_Best_ElectricEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEST_ELECTRICEDITOR_H
