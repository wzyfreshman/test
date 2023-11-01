/********************************************************************************
** Form generated from reading UI file 'backup.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BACKUP_H
#define UI_BACKUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Backup
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_build;
    QPushButton *btn_delete;
    QPushButton *btn_update;
    QWidget *listwidget;

    void setupUi(QDialog *Backup)
    {
        if (Backup->objectName().isEmpty())
            Backup->setObjectName(QStringLiteral("Backup"));
        Backup->resize(400, 300);
        verticalLayout = new QVBoxLayout(Backup);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(Backup);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(15);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(10, 0, 10, 0);
        btn_build = new QPushButton(frame);
        btn_build->setObjectName(QStringLiteral("btn_build"));

        horizontalLayout->addWidget(btn_build);

        btn_delete = new QPushButton(frame);
        btn_delete->setObjectName(QStringLiteral("btn_delete"));

        horizontalLayout->addWidget(btn_delete);

        btn_update = new QPushButton(frame);
        btn_update->setObjectName(QStringLiteral("btn_update"));

        horizontalLayout->addWidget(btn_update);


        verticalLayout->addWidget(frame);

        listwidget = new QWidget(Backup);
        listwidget->setObjectName(QStringLiteral("listwidget"));

        verticalLayout->addWidget(listwidget);


        retranslateUi(Backup);

        QMetaObject::connectSlotsByName(Backup);
    } // setupUi

    void retranslateUi(QDialog *Backup)
    {
        Backup->setWindowTitle(QApplication::translate("Backup", "Backup", Q_NULLPTR));
        btn_build->setText(QApplication::translate("Backup", "\345\244\207\344\273\275\350\275\257\344\273\266", Q_NULLPTR));
        btn_delete->setText(QApplication::translate("Backup", "\345\210\240\351\231\244\345\244\207\344\273\275\346\226\207\344\273\266", Q_NULLPTR));
        btn_update->setText(QApplication::translate("Backup", "\350\277\230\345\216\237\350\275\257\344\273\266", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Backup: public Ui_Backup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BACKUP_H
