/********************************************************************************
** Form generated from reading UI file 'compensation.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPENSATION_H
#define UI_COMPENSATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_compensation
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *comptable;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_ensure;
    QPushButton *btn_cancel;

    void setupUi(QWidget *compensation)
    {
        if (compensation->objectName().isEmpty())
            compensation->setObjectName(QString::fromUtf8("compensation"));
        compensation->resize(400, 300);
        verticalLayout = new QVBoxLayout(compensation);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        comptable = new QTableWidget(compensation);
        if (comptable->columnCount() < 3)
            comptable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        comptable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        comptable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        comptable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        comptable->setObjectName(QString::fromUtf8("comptable"));
        comptable->setColumnCount(3);

        verticalLayout->addWidget(comptable);

        frame = new QFrame(compensation);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_ensure = new QPushButton(frame);
        btn_ensure->setObjectName(QString::fromUtf8("btn_ensure"));

        horizontalLayout->addWidget(btn_ensure);

        btn_cancel = new QPushButton(frame);
        btn_cancel->setObjectName(QString::fromUtf8("btn_cancel"));

        horizontalLayout->addWidget(btn_cancel);


        verticalLayout->addWidget(frame);


        retranslateUi(compensation);

        QMetaObject::connectSlotsByName(compensation);
    } // setupUi

    void retranslateUi(QWidget *compensation)
    {
        compensation->setWindowTitle(QApplication::translate("compensation", "Form", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = comptable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("compensation", "Tool", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = comptable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("compensation", "XComp", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = comptable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("compensation", "YComp", 0, QApplication::UnicodeUTF8));
        btn_ensure->setText(QApplication::translate("compensation", "Ensure", 0, QApplication::UnicodeUTF8));
        btn_cancel->setText(QApplication::translate("compensation", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class compensation: public Ui_compensation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPENSATION_H
