/********************************************************************************
** Form generated from reading UI file 'inicheck.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INICHECK_H
#define UI_INICHECK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IniCheck
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *IniCheck)
    {
        if (IniCheck->objectName().isEmpty())
            IniCheck->setObjectName(QStringLiteral("IniCheck"));
        IniCheck->resize(400, 300);
        menuBar = new QMenuBar(IniCheck);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        IniCheck->setMenuBar(menuBar);
        mainToolBar = new QToolBar(IniCheck);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        IniCheck->addToolBar(mainToolBar);
        centralWidget = new QWidget(IniCheck);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        IniCheck->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(IniCheck);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        IniCheck->setStatusBar(statusBar);

        retranslateUi(IniCheck);

        QMetaObject::connectSlotsByName(IniCheck);
    } // setupUi

    void retranslateUi(QMainWindow *IniCheck)
    {
        IniCheck->setWindowTitle(QApplication::translate("IniCheck", "IniCheck", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class IniCheck: public Ui_IniCheck {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INICHECK_H
