/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *generate;
    QLineEdit *Key1;
    QLineEdit *Key2;
    QLineEdit *Key3;
    QLineEdit *Key4;
    QLineEdit *SysNum;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *customer;
    QCheckBox *foreverCheck;
    QLabel *label_7;
    QLabel *label_8;
    QComboBox *Change_Mode;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLineEdit *info_day;
    QLineEdit *Info_hour;
    QLabel *label_10;
    QLineEdit *Info;
    QLabel *label_6;
    QLabel *label_9;
    QLabel *label;
    QWidget *page_2;
    QLabel *label_5;
    QDateEdit *GetDate;
    QLabel *label_11;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(587, 371);
        MainWindow->setMinimumSize(QSize(587, 371));
        MainWindow->setMaximumSize(QSize(587, 371));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        generate = new QPushButton(centralWidget);
        generate->setObjectName(QStringLiteral("generate"));
        generate->setGeometry(QRect(120, 270, 91, 41));
        Key1 = new QLineEdit(centralWidget);
        Key1->setObjectName(QStringLiteral("Key1"));
        Key1->setGeometry(QRect(120, 190, 91, 31));
        Key1->setMaxLength(4);
        Key1->setAlignment(Qt::AlignCenter);
        Key1->setReadOnly(true);
        Key2 = new QLineEdit(centralWidget);
        Key2->setObjectName(QStringLiteral("Key2"));
        Key2->setGeometry(QRect(230, 190, 91, 31));
        Key2->setMaxLength(4);
        Key2->setAlignment(Qt::AlignCenter);
        Key2->setReadOnly(true);
        Key3 = new QLineEdit(centralWidget);
        Key3->setObjectName(QStringLiteral("Key3"));
        Key3->setGeometry(QRect(340, 190, 91, 31));
        Key3->setMaxLength(4);
        Key3->setAlignment(Qt::AlignCenter);
        Key3->setReadOnly(true);
        Key4 = new QLineEdit(centralWidget);
        Key4->setObjectName(QStringLiteral("Key4"));
        Key4->setGeometry(QRect(450, 190, 91, 31));
        Key4->setMaxLength(4);
        Key4->setAlignment(Qt::AlignCenter);
        Key4->setReadOnly(true);
        SysNum = new QLineEdit(centralWidget);
        SysNum->setObjectName(QStringLiteral("SysNum"));
        SysNum->setGeometry(QRect(120, 150, 311, 31));
        SysNum->setMaxLength(16);
        SysNum->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 160, 71, 16));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(40, 200, 71, 16));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        customer = new QLabel(centralWidget);
        customer->setObjectName(QStringLiteral("customer"));
        customer->setGeometry(QRect(10, 10, 111, 16));
        foreverCheck = new QCheckBox(centralWidget);
        foreverCheck->setObjectName(QStringLiteral("foreverCheck"));
        foreverCheck->setGeometry(QRect(120, 50, 92, 19));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(230, 260, 341, 61));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(230, 240, 66, 14));
        Change_Mode = new QComboBox(centralWidget);
        Change_Mode->setObjectName(QStringLiteral("Change_Mode"));
        Change_Mode->setGeometry(QRect(330, 50, 111, 21));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(100, 90, 441, 41));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        info_day = new QLineEdit(page);
        info_day->setObjectName(QStringLiteral("info_day"));
        info_day->setGeometry(QRect(20, 10, 91, 31));
        info_day->setInputMethodHints(Qt::ImhDigitsOnly);
        info_day->setMaxLength(3);
        info_day->setAlignment(Qt::AlignCenter);
        Info_hour = new QLineEdit(page);
        Info_hour->setObjectName(QStringLiteral("Info_hour"));
        Info_hour->setGeometry(QRect(160, 10, 91, 31));
        label_10 = new QLabel(page);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(300, 20, 16, 16));
        Info = new QLineEdit(page);
        Info->setObjectName(QStringLiteral("Info"));
        Info->setGeometry(QRect(320, 10, 91, 31));
        Info->setInputMethodHints(Qt::ImhDigitsOnly);
        Info->setMaxLength(5);
        Info->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(page);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(120, 20, 16, 16));
        label_9 = new QLabel(page);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(140, 20, 16, 16));
        label = new QLabel(page);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(260, 10, 31, 31));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        label_5 = new QLabel(page_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 10, 121, 20));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        GetDate = new QDateEdit(page_2);
        GetDate->setObjectName(QStringLiteral("GetDate"));
        GetDate->setGeometry(QRect(150, 10, 111, 24));
        GetDate->setDate(QDate(2014, 12, 25));
        stackedWidget->addWidget(page_2);
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(230, 50, 91, 20));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 587, 22));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        generate->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\346\234\272\345\272\212\347\274\226\345\217\267", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\346\263\250\345\206\214\345\217\267\347\240\201", Q_NULLPTR));
        customer->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        foreverCheck->setText(QApplication::translate("MainWindow", "\346\260\270\344\271\205\346\234\211\346\225\210", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "1\343\200\201\346\234\200\345\244\247\346\234\211\346\225\210\346\227\266\351\227\264\344\270\2728760\345\260\217\346\227\266(365\345\244\251)\346\210\226\350\200\205\346\260\270\344\271\205\346\234\211\346\225\210\n"
"2\343\200\201\346\234\272\345\272\212\347\274\226\345\217\267\347\224\261\344\273\273\346\204\217\346\225\260\345\255\227\345\222\214\345\255\227\346\257\215\347\273\204\346\210\220\357\274\214\345\205\266\344\273\226\347\274\226\345\217\267\347\224\261 0\357\275\2369 \n"
"A\357\275\236F\347\273\204\346\210\220,  \351\203\275\344\270\215\345\214\272\345\210\206\345\244\247\345\260\217\345\206\231", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "\350\257\264\346\230\216", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "=", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "\345\244\251", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "X", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\345\260\217\346\227\266", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\345\210\260\346\234\237\346\227\245\346\234\237", Q_NULLPTR));
        GetDate->setDisplayFormat(QApplication::translate("MainWindow", "yyyy-M-d", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\350\256\241\346\227\266\346\226\271\345\274\217:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
