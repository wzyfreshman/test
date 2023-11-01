/********************************************************************************
** Form generated from reading UI file 'best_producteditor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEST_PRODUCTEDITOR_H
#define UI_BEST_PRODUCTEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Best_ProductEditor
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QLineEdit *Product_Name;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QSpinBox *Y_Num;
    QLabel *label_3;
    QLabel *label_6;
    QSpinBox *F_Num;
    QSpinBox *X_Num;
    QLabel *label_4;
    QSpinBox *XL_Num;
    QSpinBox *K_Num;
    QLabel *label_2;
    QLabel *label_7;
    QSpinBox *G_Num;
    QLabel *label_8;
    QLabel *label_9;
    QCheckBox *Ck_AutoAdd;
    QSpinBox *YL_Num;
    QSpinBox *PIC_Num;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *Btn_Cancel1;
    QPushButton *Btn_Next;
    QWidget *page_2;
    QGridLayout *gridLayout_2;
    QTableWidget *Table_G;
    QTableWidget *Table_X;
    QTableWidget *Table_Y;
    QTableWidget *Table_K;
    QTableWidget *Table_F;
    QWidget *widget;
    QPushButton *Btn_Up;
    QPushButton *Btn_Cancel2;
    QPushButton *Btn_Sure;

    void setupUi(QDialog *Best_ProductEditor)
    {
        if (Best_ProductEditor->objectName().isEmpty())
            Best_ProductEditor->setObjectName(QString::fromUtf8("Best_ProductEditor"));
        Best_ProductEditor->resize(837, 580);
        verticalLayout = new QVBoxLayout(Best_ProductEditor);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stackedWidget = new QStackedWidget(Best_ProductEditor);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_2 = new QVBoxLayout(page);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label);

        Product_Name = new QLineEdit(page);
        Product_Name->setObjectName(QString::fromUtf8("Product_Name"));

        horizontalLayout_2->addWidget(Product_Name);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_5 = new QLabel(page);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        Y_Num = new QSpinBox(page);
        Y_Num->setObjectName(QString::fromUtf8("Y_Num"));
        Y_Num->setMaximum(255);

        gridLayout->addWidget(Y_Num, 2, 1, 1, 1);

        label_3 = new QLabel(page);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_6 = new QLabel(page);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 1, 2, 1, 1);

        F_Num = new QSpinBox(page);
        F_Num->setObjectName(QString::fromUtf8("F_Num"));
        F_Num->setMaximum(255);

        gridLayout->addWidget(F_Num, 4, 1, 1, 1);

        X_Num = new QSpinBox(page);
        X_Num->setObjectName(QString::fromUtf8("X_Num"));
        X_Num->setMaximum(255);

        gridLayout->addWidget(X_Num, 1, 1, 1, 1);

        label_4 = new QLabel(page);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        XL_Num = new QSpinBox(page);
        XL_Num->setObjectName(QString::fromUtf8("XL_Num"));
        XL_Num->setMaximum(255);

        gridLayout->addWidget(XL_Num, 1, 3, 1, 1);

        K_Num = new QSpinBox(page);
        K_Num->setObjectName(QString::fromUtf8("K_Num"));
        K_Num->setMaximum(255);

        gridLayout->addWidget(K_Num, 3, 1, 1, 1);

        label_2 = new QLabel(page);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_7 = new QLabel(page);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        G_Num = new QSpinBox(page);
        G_Num->setObjectName(QString::fromUtf8("G_Num"));
        G_Num->setMaximum(255);

        gridLayout->addWidget(G_Num, 5, 1, 1, 1);

        label_8 = new QLabel(page);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_8, 2, 2, 1, 1);

        label_9 = new QLabel(page);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_9, 3, 2, 1, 1);

        Ck_AutoAdd = new QCheckBox(page);
        Ck_AutoAdd->setObjectName(QString::fromUtf8("Ck_AutoAdd"));

        gridLayout->addWidget(Ck_AutoAdd, 4, 3, 1, 1);

        YL_Num = new QSpinBox(page);
        YL_Num->setObjectName(QString::fromUtf8("YL_Num"));
        YL_Num->setMaximum(255);

        gridLayout->addWidget(YL_Num, 2, 3, 1, 1);

        PIC_Num = new QSpinBox(page);
        PIC_Num->setObjectName(QString::fromUtf8("PIC_Num"));
        PIC_Num->setMaximum(255);

        gridLayout->addWidget(PIC_Num, 3, 3, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Btn_Cancel1 = new QPushButton(page);
        Btn_Cancel1->setObjectName(QString::fromUtf8("Btn_Cancel1"));

        horizontalLayout->addWidget(Btn_Cancel1);

        Btn_Next = new QPushButton(page);
        Btn_Next->setObjectName(QString::fromUtf8("Btn_Next"));

        horizontalLayout->addWidget(Btn_Next);


        verticalLayout_2->addLayout(horizontalLayout);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        gridLayout_2 = new QGridLayout(page_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        Table_G = new QTableWidget(page_2);
        if (Table_G->columnCount() < 2)
            Table_G->setColumnCount(2);
        if (Table_G->rowCount() < 2)
            Table_G->setRowCount(2);
        Table_G->setObjectName(QString::fromUtf8("Table_G"));
        Table_G->setRowCount(2);
        Table_G->setColumnCount(2);

        gridLayout_2->addWidget(Table_G, 1, 1, 1, 1);

        Table_X = new QTableWidget(page_2);
        if (Table_X->columnCount() < 2)
            Table_X->setColumnCount(2);
        if (Table_X->rowCount() < 2)
            Table_X->setRowCount(2);
        Table_X->setObjectName(QString::fromUtf8("Table_X"));
        Table_X->setRowCount(2);
        Table_X->setColumnCount(2);

        gridLayout_2->addWidget(Table_X, 0, 0, 1, 1);

        Table_Y = new QTableWidget(page_2);
        if (Table_Y->columnCount() < 2)
            Table_Y->setColumnCount(2);
        if (Table_Y->rowCount() < 2)
            Table_Y->setRowCount(2);
        Table_Y->setObjectName(QString::fromUtf8("Table_Y"));
        Table_Y->setRowCount(2);
        Table_Y->setColumnCount(2);

        gridLayout_2->addWidget(Table_Y, 0, 1, 1, 1);

        Table_K = new QTableWidget(page_2);
        if (Table_K->columnCount() < 2)
            Table_K->setColumnCount(2);
        if (Table_K->rowCount() < 2)
            Table_K->setRowCount(2);
        Table_K->setObjectName(QString::fromUtf8("Table_K"));
        Table_K->setRowCount(2);
        Table_K->setColumnCount(2);

        gridLayout_2->addWidget(Table_K, 0, 2, 1, 1);

        Table_F = new QTableWidget(page_2);
        if (Table_F->columnCount() < 2)
            Table_F->setColumnCount(2);
        if (Table_F->rowCount() < 2)
            Table_F->setRowCount(2);
        Table_F->setObjectName(QString::fromUtf8("Table_F"));
        Table_F->setRowCount(2);
        Table_F->setColumnCount(2);

        gridLayout_2->addWidget(Table_F, 1, 0, 1, 1);

        widget = new QWidget(page_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        Btn_Up = new QPushButton(widget);
        Btn_Up->setObjectName(QString::fromUtf8("Btn_Up"));
        Btn_Up->setGeometry(QRect(80, 170, 99, 27));
        Btn_Cancel2 = new QPushButton(widget);
        Btn_Cancel2->setObjectName(QString::fromUtf8("Btn_Cancel2"));
        Btn_Cancel2->setGeometry(QRect(80, 130, 99, 27));
        Btn_Sure = new QPushButton(widget);
        Btn_Sure->setObjectName(QString::fromUtf8("Btn_Sure"));
        Btn_Sure->setGeometry(QRect(80, 220, 99, 27));

        gridLayout_2->addWidget(widget, 1, 2, 1, 1);

        stackedWidget->addWidget(page_2);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(Best_ProductEditor);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Best_ProductEditor);
    } // setupUi

    void retranslateUi(QDialog *Best_ProductEditor)
    {
        Best_ProductEditor->setWindowTitle(QApplication::translate("Best_ProductEditor", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Best_ProductEditor", "\344\272\247\345\223\201\345\236\213\345\217\267", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Best_ProductEditor", "F\347\272\277\346\225\260", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Best_ProductEditor", "Y\347\272\277\346\225\260", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Best_ProductEditor", "XL\347\272\277\346\225\260", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Best_ProductEditor", "K\347\272\277\346\225\260", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Best_ProductEditor", "X\347\272\277\346\225\260", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Best_ProductEditor", "G\347\272\277\346\225\260", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Best_ProductEditor", "YL\347\272\277\346\225\260", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Best_ProductEditor", "\345\233\276\346\241\210\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        Ck_AutoAdd->setText(QApplication::translate("Best_ProductEditor", "\351\200\232\351\201\223\350\207\252\345\212\250\345\212\2401", 0, QApplication::UnicodeUTF8));
        Btn_Cancel1->setText(QApplication::translate("Best_ProductEditor", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Btn_Next->setText(QApplication::translate("Best_ProductEditor", "\344\270\213\344\270\200\346\255\245", 0, QApplication::UnicodeUTF8));
        Btn_Up->setText(QApplication::translate("Best_ProductEditor", "\344\270\212\344\270\200\346\255\245", 0, QApplication::UnicodeUTF8));
        Btn_Cancel2->setText(QApplication::translate("Best_ProductEditor", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Btn_Sure->setText(QApplication::translate("Best_ProductEditor", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Best_ProductEditor: public Ui_Best_ProductEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEST_PRODUCTEDITOR_H
