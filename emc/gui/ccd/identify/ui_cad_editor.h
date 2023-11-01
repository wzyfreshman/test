/********************************************************************************
** Form generated from reading UI file 'cad_editor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAD_EDITOR_H
#define UI_CAD_EDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Cad_Editor
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *Cad_MainFram;
    QVBoxLayout *verticalLayout;
    QFrame *unused;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QDoubleSpinBox *SB_MagTran;
    QLabel *label_3;
    QLabel *label;
    QDoubleSpinBox *SB_MagResult;
    QSpacerItem *horizontalSpacer;
    QPushButton *Btn_Sure;
    QPushButton *Btn_Cancel;

    void setupUi(QDialog *Cad_Editor)
    {
        if (Cad_Editor->objectName().isEmpty())
            Cad_Editor->setObjectName(QString::fromUtf8("Cad_Editor"));
        Cad_Editor->resize(1166, 638);
        verticalLayout_2 = new QVBoxLayout(Cad_Editor);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        Cad_MainFram = new QFrame(Cad_Editor);
        Cad_MainFram->setObjectName(QString::fromUtf8("Cad_MainFram"));
        Cad_MainFram->setFrameShape(QFrame::StyledPanel);
        Cad_MainFram->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(Cad_MainFram);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        unused = new QFrame(Cad_MainFram);
        unused->setObjectName(QString::fromUtf8("unused"));
        unused->setFrameShape(QFrame::HLine);
        unused->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(unused);


        verticalLayout_2->addWidget(Cad_MainFram);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(Cad_Editor);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        SB_MagTran = new QDoubleSpinBox(Cad_Editor);
        SB_MagTran->setObjectName(QString::fromUtf8("SB_MagTran"));
        SB_MagTran->setDecimals(6);
        SB_MagTran->setMaximum(10000);

        horizontalLayout->addWidget(SB_MagTran);

        label_3 = new QLabel(Cad_Editor);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        label = new QLabel(Cad_Editor);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        SB_MagResult = new QDoubleSpinBox(Cad_Editor);
        SB_MagResult->setObjectName(QString::fromUtf8("SB_MagResult"));
        SB_MagResult->setDecimals(6);
        SB_MagResult->setMaximum(10000);

        horizontalLayout->addWidget(SB_MagResult);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Btn_Sure = new QPushButton(Cad_Editor);
        Btn_Sure->setObjectName(QString::fromUtf8("Btn_Sure"));

        horizontalLayout->addWidget(Btn_Sure);

        Btn_Cancel = new QPushButton(Cad_Editor);
        Btn_Cancel->setObjectName(QString::fromUtf8("Btn_Cancel"));

        horizontalLayout->addWidget(Btn_Cancel);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(Cad_Editor);

        QMetaObject::connectSlotsByName(Cad_Editor);
    } // setupUi

    void retranslateUi(QDialog *Cad_Editor)
    {
        Cad_Editor->setWindowTitle(QApplication::translate("Cad_Editor", "Dialog", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Cad_Editor", "\345\200\215\347\216\207\350\275\254\346\215\242:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Cad_Editor", "mm", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Cad_Editor", "----->", 0, QApplication::UnicodeUTF8));
        Btn_Sure->setText(QApplication::translate("Cad_Editor", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        Btn_Cancel->setText(QApplication::translate("Cad_Editor", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Cad_Editor: public Ui_Cad_Editor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAD_EDITOR_H
