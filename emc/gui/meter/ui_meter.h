/********************************************************************************
** Form generated from reading UI file 'meter.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_METER_H
#define UI_METER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Meter
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QTabWidget *tabWidget_2;
    QWidget *tab_5;
    QVBoxLayout *verticalLayout_5;
    QTableWidget *tNormal;
    QWidget *tab_4;
    QTableWidget *tShow;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout;
    QTableWidget *tPin;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tSig;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tPar;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Meter)
    {
        if (Meter->objectName().isEmpty())
            Meter->setObjectName(QStringLiteral("Meter"));
        Meter->resize(549, 656);
        centralWidget = new QWidget(Meter);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        tabWidget_2 = new QTabWidget(centralWidget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        verticalLayout_5 = new QVBoxLayout(tab_5);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        tNormal = new QTableWidget(tab_5);
        tNormal->setObjectName(QStringLiteral("tNormal"));

        verticalLayout_5->addWidget(tNormal);

        tabWidget_2->addTab(tab_5, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        tShow = new QTableWidget(tab_4);
        tShow->setObjectName(QStringLiteral("tShow"));
        tShow->setGeometry(QRect(5, 5, 517, 153));
        tabWidget = new QTabWidget(tab_4);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(5, 164, 264, 225));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout = new QHBoxLayout(tab);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tPin = new QTableWidget(tab);
        tPin->setObjectName(QStringLiteral("tPin"));

        horizontalLayout->addWidget(tPin);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(2, 2, 2, 2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tSig = new QTableWidget(tab_2);
        tSig->setObjectName(QStringLiteral("tSig"));

        verticalLayout_2->addWidget(tSig);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_3 = new QVBoxLayout(tab_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(2, 2, 2, 2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        tPar = new QTableWidget(tab_3);
        tPar->setObjectName(QStringLiteral("tPar"));

        verticalLayout_3->addWidget(tPar);

        tabWidget->addTab(tab_3, QString());
        tabWidget_2->addTab(tab_4, QString());

        verticalLayout_4->addWidget(tabWidget_2);

        Meter->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Meter);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 549, 25));
        Meter->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Meter);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Meter->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Meter);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Meter->setStatusBar(statusBar);

        retranslateUi(Meter);

        tabWidget_2->setCurrentIndex(0);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Meter);
    } // setupUi

    void retranslateUi(QMainWindow *Meter)
    {
        Meter->setWindowTitle(QApplication::translate("Meter", "Meter", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QApplication::translate("Meter", "\347\212\266\346\200\201", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Meter", "\347\256\241\350\204\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Meter", "\344\277\241\345\217\267", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Meter", "\345\217\202\346\225\260", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("Meter", "\345\210\227\350\241\250", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Meter: public Ui_Meter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_METER_H
