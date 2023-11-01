/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLabel *label_2;
    QComboBox *Baul;
    QSpinBox *receiveLen;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QLineEdit *DevName;
    QCheckBox *setloop;
    QPushButton *pushButton_2;
    QComboBox *direction;
    QTextBrowser *showText;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpinBox *slaveno;
    QLineEdit *addr;
    QLineEdit *data;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(628, 342);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        Baul = new QComboBox(widget_2);
        Baul->setObjectName(QStringLiteral("Baul"));

        gridLayout->addWidget(Baul, 1, 1, 1, 1);

        receiveLen = new QSpinBox(widget_2);
        receiveLen->setObjectName(QStringLiteral("receiveLen"));
        receiveLen->setSingleStep(1);
        receiveLen->setValue(8);

        gridLayout->addWidget(receiveLen, 2, 1, 1, 1);

        label = new QLabel(widget_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 1, 1, 1);

        DevName = new QLineEdit(widget_2);
        DevName->setObjectName(QStringLiteral("DevName"));

        gridLayout->addWidget(DevName, 0, 1, 1, 1);

        setloop = new QCheckBox(widget_2);
        setloop->setObjectName(QStringLiteral("setloop"));

        gridLayout->addWidget(setloop, 3, 1, 1, 1);

        pushButton_2 = new QPushButton(widget_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 3, 0, 1, 1);

        direction = new QComboBox(widget_2);
        direction->setObjectName(QStringLiteral("direction"));

        gridLayout->addWidget(direction, 4, 0, 1, 1);


        gridLayout_2->addWidget(widget_2, 0, 0, 1, 1);

        showText = new QTextBrowser(centralWidget);
        showText->setObjectName(QStringLiteral("showText"));

        gridLayout_2->addWidget(showText, 0, 1, 1, 1);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        slaveno = new QSpinBox(widget);
        slaveno->setObjectName(QStringLiteral("slaveno"));
        slaveno->setValue(1);

        horizontalLayout->addWidget(slaveno);

        addr = new QLineEdit(widget);
        addr->setObjectName(QStringLiteral("addr"));

        horizontalLayout->addWidget(addr);

        data = new QLineEdit(widget);
        data->setObjectName(QStringLiteral("data"));

        horizontalLayout->addWidget(data);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        gridLayout_2->addWidget(widget, 1, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 628, 28));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label_3->setText(QApplication::translate("MainWindow", "\346\216\245\346\224\266\346\225\260\346\215\256\351\225\277\345\272\246", 0));
        label_2->setText(QApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", 0));
        label->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207", 0));
        DevName->setText(QApplication::translate("MainWindow", "/dev/ttyUSB0", 0));
        setloop->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\276\252\347\216\257", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "\346\270\205\351\231\244\346\227\245\345\277\227", 0));
        pushButton->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
