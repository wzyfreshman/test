/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *Generate;
    QLabel *label;
    QPushButton *WriteDown;
    QLineEdit *lineEditH;
    QLineEdit *lineEditL;
    QLabel *label_2;
    QLineEdit *lineEdit_HD;
    QLineEdit *lineEdit_FPGA;
    QLineEdit *lineEdit_ChIP;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *pushButton;
    QComboBox *customers;
    QLabel *label_6;
    QLineEdit *lineEdit_Mac;
    QLabel *label_7;
    QLineEdit *itemCode1;
    QLineEdit *itemCode2;
    QLineEdit *itemCode3;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(652, 400);
        MainWindow->setMinimumSize(QSize(652, 400));
        MainWindow->setMaximumSize(QSize(652, 400));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Generate = new QPushButton(centralWidget);
        Generate->setObjectName(QStringLiteral("Generate"));
        Generate->setGeometry(QRect(320, 310, 151, 41));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 230, 631, 16));
        WriteDown = new QPushButton(centralWidget);
        WriteDown->setObjectName(QStringLiteral("WriteDown"));
        WriteDown->setGeometry(QRect(490, 310, 151, 41));
        lineEditH = new QLineEdit(centralWidget);
        lineEditH->setObjectName(QStringLiteral("lineEditH"));
        lineEditH->setGeometry(QRect(30, 150, 591, 31));
        lineEditH->setAlignment(Qt::AlignCenter);
        lineEditH->setReadOnly(true);
        lineEditL = new QLineEdit(centralWidget);
        lineEditL->setObjectName(QStringLiteral("lineEditL"));
        lineEditL->setGeometry(QRect(30, 200, 591, 31));
        lineEditL->setAlignment(Qt::AlignCenter);
        lineEditL->setReadOnly(true);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 10, 81, 16));
        lineEdit_HD = new QLineEdit(centralWidget);
        lineEdit_HD->setObjectName(QStringLiteral("lineEdit_HD"));
        lineEdit_HD->setGeometry(QRect(80, 60, 251, 31));
        lineEdit_HD->setAlignment(Qt::AlignCenter);
        lineEdit_HD->setReadOnly(true);
        lineEdit_FPGA = new QLineEdit(centralWidget);
        lineEdit_FPGA->setObjectName(QStringLiteral("lineEdit_FPGA"));
        lineEdit_FPGA->setGeometry(QRect(80, 100, 251, 31));
        lineEdit_FPGA->setAlignment(Qt::AlignCenter);
        lineEdit_FPGA->setReadOnly(true);
        lineEdit_ChIP = new QLineEdit(centralWidget);
        lineEdit_ChIP->setObjectName(QStringLiteral("lineEdit_ChIP"));
        lineEdit_ChIP->setGeometry(QRect(380, 100, 241, 31));
        lineEdit_ChIP->setAlignment(Qt::AlignCenter);
        lineEdit_ChIP->setReadOnly(true);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 70, 41, 16));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 110, 41, 16));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(340, 110, 41, 16));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 300, 87, 24));
        customers = new QComboBox(centralWidget);
        customers->setObjectName(QStringLiteral("customers"));
        customers->setGeometry(QRect(440, 30, 181, 21));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(370, 30, 61, 16));
        lineEdit_Mac = new QLineEdit(centralWidget);
        lineEdit_Mac->setObjectName(QStringLiteral("lineEdit_Mac"));
        lineEdit_Mac->setGeometry(QRect(380, 60, 241, 31));
        lineEdit_Mac->setAlignment(Qt::AlignCenter);
        lineEdit_Mac->setReadOnly(true);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(340, 70, 31, 20));
        itemCode1 = new QLineEdit(centralWidget);
        itemCode1->setObjectName(QStringLiteral("itemCode1"));
        itemCode1->setGeometry(QRect(80, 260, 111, 31));
        itemCode1->setMaxLength(4);
        itemCode1->setAlignment(Qt::AlignCenter);
        itemCode1->setReadOnly(true);
        itemCode2 = new QLineEdit(centralWidget);
        itemCode2->setObjectName(QStringLiteral("itemCode2"));
        itemCode2->setGeometry(QRect(210, 260, 191, 31));
        itemCode2->setMaxLength(8);
        itemCode2->setAlignment(Qt::AlignCenter);
        itemCode3 = new QLineEdit(centralWidget);
        itemCode3->setObjectName(QStringLiteral("itemCode3"));
        itemCode3->setGeometry(QRect(420, 260, 111, 31));
        itemCode3->setMaxLength(4);
        itemCode3->setAlignment(Qt::AlignCenter);
        itemCode3->setReadOnly(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 652, 35));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        Generate->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220\345\257\206\347\240\201", Q_NULLPTR));
        label->setText(QString());
        WriteDown->setText(QApplication::translate("MainWindow", "\345\206\231\345\205\245\345\257\206\347\240\201", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\350\275\257\344\273\266\345\212\240\345\257\206\346\226\271\346\241\210", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "HDID", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "FPGA", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "ChipId", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "\345\256\242\346\210\267\351\200\211\346\213\251", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "mac", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
