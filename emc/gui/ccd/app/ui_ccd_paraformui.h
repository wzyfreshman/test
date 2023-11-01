/********************************************************************************
** Form generated from reading UI file 'ccd_paraformui.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_PARAFORMUI_H
#define UI_CCD_PARAFORMUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCD_ParaFormUI
{
public:
    QGridLayout *gridLayout;
    QFrame *line_3;
    QLabel *label;
    QCheckBox *RevY;
    QSpinBox *Anum;
    QCheckBox *RevA;
    QCheckBox *RevX;
    QSpinBox *Xnum;
    QLabel *label_3;
    QFrame *line;
    QLabel *label_2;
    QFrame *line_2;
    QSpinBox *Ynum;
    QLabel *label_4;
    QSpinBox *CH;
    QLabel *Id;

    void setupUi(QWidget *CCD_ParaFormUI)
    {
        if (CCD_ParaFormUI->objectName().isEmpty())
            CCD_ParaFormUI->setObjectName(QStringLiteral("CCD_ParaFormUI"));
        CCD_ParaFormUI->resize(480, 44);
        gridLayout = new QGridLayout(CCD_ParaFormUI);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(-1, 0, -1, 0);
        line_3 = new QFrame(CCD_ParaFormUI);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 0, 12, 1, 1);

        label = new QLabel(CCD_ParaFormUI);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        RevY = new QCheckBox(CCD_ParaFormUI);
        RevY->setObjectName(QStringLiteral("RevY"));

        gridLayout->addWidget(RevY, 0, 7, 1, 1);

        Anum = new QSpinBox(CCD_ParaFormUI);
        Anum->setObjectName(QStringLiteral("Anum"));

        gridLayout->addWidget(Anum, 0, 10, 1, 1);

        RevA = new QCheckBox(CCD_ParaFormUI);
        RevA->setObjectName(QStringLiteral("RevA"));

        gridLayout->addWidget(RevA, 0, 11, 1, 1);

        RevX = new QCheckBox(CCD_ParaFormUI);
        RevX->setObjectName(QStringLiteral("RevX"));

        gridLayout->addWidget(RevX, 0, 3, 1, 1);

        Xnum = new QSpinBox(CCD_ParaFormUI);
        Xnum->setObjectName(QStringLiteral("Xnum"));

        gridLayout->addWidget(Xnum, 0, 2, 1, 1);

        label_3 = new QLabel(CCD_ParaFormUI);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 9, 1, 1);

        line = new QFrame(CCD_ParaFormUI);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 0, 4, 1, 1);

        label_2 = new QLabel(CCD_ParaFormUI);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 5, 1, 1);

        line_2 = new QFrame(CCD_ParaFormUI);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 0, 8, 1, 1);

        Ynum = new QSpinBox(CCD_ParaFormUI);
        Ynum->setObjectName(QStringLiteral("Ynum"));

        gridLayout->addWidget(Ynum, 0, 6, 1, 1);

        label_4 = new QLabel(CCD_ParaFormUI);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 0, 13, 1, 1);

        CH = new QSpinBox(CCD_ParaFormUI);
        CH->setObjectName(QStringLiteral("CH"));

        gridLayout->addWidget(CH, 0, 14, 1, 1);

        Id = new QLabel(CCD_ParaFormUI);
        Id->setObjectName(QStringLiteral("Id"));

        gridLayout->addWidget(Id, 0, 0, 1, 1);


        retranslateUi(CCD_ParaFormUI);

        QMetaObject::connectSlotsByName(CCD_ParaFormUI);
    } // setupUi

    void retranslateUi(QWidget *CCD_ParaFormUI)
    {
        CCD_ParaFormUI->setWindowTitle(QApplication::translate("CCD_ParaFormUI", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("CCD_ParaFormUI", "X", Q_NULLPTR));
        RevY->setText(QApplication::translate("CCD_ParaFormUI", "Rev", Q_NULLPTR));
        RevA->setText(QApplication::translate("CCD_ParaFormUI", "Rev", Q_NULLPTR));
        RevX->setText(QApplication::translate("CCD_ParaFormUI", "Rev", Q_NULLPTR));
        label_3->setText(QApplication::translate("CCD_ParaFormUI", "A", Q_NULLPTR));
        label_2->setText(QApplication::translate("CCD_ParaFormUI", "Y", Q_NULLPTR));
        label_4->setText(QApplication::translate("CCD_ParaFormUI", "CH", Q_NULLPTR));
        Id->setText(QApplication::translate("CCD_ParaFormUI", "ID", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_ParaFormUI: public Ui_CCD_ParaFormUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_PARAFORMUI_H
