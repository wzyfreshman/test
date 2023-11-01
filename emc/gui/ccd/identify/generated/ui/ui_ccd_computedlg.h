/********************************************************************************
** Form generated from reading UI file 'ccd_computedlg.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_COMPUTEDLG_H
#define UI_CCD_COMPUTEDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CCD_ComputeDlg
{
public:
    QDoubleSpinBox *Z_1;
    QDoubleSpinBox *X_1;
    QDoubleSpinBox *Y_1;
    QDoubleSpinBox *A_1;
    QDoubleSpinBox *A_2;
    QDoubleSpinBox *Y_2;
    QDoubleSpinBox *X_2;
    QDoubleSpinBox *Z_2;
    QDoubleSpinBox *A_3;
    QDoubleSpinBox *Y_3;
    QDoubleSpinBox *X_3;
    QDoubleSpinBox *Z_3;
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QDialog *CCD_ComputeDlg)
    {
        if (CCD_ComputeDlg->objectName().isEmpty())
            CCD_ComputeDlg->setObjectName(QString::fromUtf8("CCD_ComputeDlg"));
        CCD_ComputeDlg->resize(449, 272);
        Z_1 = new QDoubleSpinBox(CCD_ComputeDlg);
        Z_1->setObjectName(QString::fromUtf8("Z_1"));
        Z_1->setGeometry(QRect(70, 50, 111, 26));
        Z_1->setDecimals(4);
        Z_1->setMinimum(-500);
        Z_1->setMaximum(500);
        Z_1->setSingleStep(0.01);
        X_1 = new QDoubleSpinBox(CCD_ComputeDlg);
        X_1->setObjectName(QString::fromUtf8("X_1"));
        X_1->setGeometry(QRect(70, 90, 111, 26));
        X_1->setDecimals(4);
        X_1->setMinimum(-500);
        X_1->setMaximum(500);
        X_1->setSingleStep(0.01);
        Y_1 = new QDoubleSpinBox(CCD_ComputeDlg);
        Y_1->setObjectName(QString::fromUtf8("Y_1"));
        Y_1->setGeometry(QRect(70, 130, 111, 26));
        Y_1->setDecimals(4);
        Y_1->setMinimum(-500);
        Y_1->setMaximum(500);
        Y_1->setSingleStep(0.01);
        A_1 = new QDoubleSpinBox(CCD_ComputeDlg);
        A_1->setObjectName(QString::fromUtf8("A_1"));
        A_1->setGeometry(QRect(70, 170, 111, 26));
        A_1->setDecimals(4);
        A_1->setMinimum(-500);
        A_1->setMaximum(500);
        A_1->setSingleStep(0.01);
        A_2 = new QDoubleSpinBox(CCD_ComputeDlg);
        A_2->setObjectName(QString::fromUtf8("A_2"));
        A_2->setGeometry(QRect(190, 170, 111, 26));
        A_2->setDecimals(4);
        A_2->setMinimum(-500);
        A_2->setMaximum(500);
        A_2->setSingleStep(0.01);
        Y_2 = new QDoubleSpinBox(CCD_ComputeDlg);
        Y_2->setObjectName(QString::fromUtf8("Y_2"));
        Y_2->setGeometry(QRect(190, 130, 111, 26));
        Y_2->setDecimals(4);
        Y_2->setMinimum(-500);
        Y_2->setMaximum(500);
        Y_2->setSingleStep(0.01);
        X_2 = new QDoubleSpinBox(CCD_ComputeDlg);
        X_2->setObjectName(QString::fromUtf8("X_2"));
        X_2->setGeometry(QRect(190, 90, 111, 26));
        X_2->setDecimals(4);
        X_2->setMinimum(-500);
        X_2->setMaximum(500);
        X_2->setSingleStep(0.01);
        Z_2 = new QDoubleSpinBox(CCD_ComputeDlg);
        Z_2->setObjectName(QString::fromUtf8("Z_2"));
        Z_2->setGeometry(QRect(190, 50, 111, 26));
        Z_2->setDecimals(4);
        Z_2->setMinimum(-500);
        Z_2->setMaximum(500);
        Z_2->setSingleStep(0.01);
        A_3 = new QDoubleSpinBox(CCD_ComputeDlg);
        A_3->setObjectName(QString::fromUtf8("A_3"));
        A_3->setGeometry(QRect(310, 170, 111, 26));
        A_3->setDecimals(4);
        A_3->setMinimum(-500);
        A_3->setMaximum(500);
        A_3->setSingleStep(0.01);
        Y_3 = new QDoubleSpinBox(CCD_ComputeDlg);
        Y_3->setObjectName(QString::fromUtf8("Y_3"));
        Y_3->setGeometry(QRect(310, 130, 111, 26));
        Y_3->setDecimals(4);
        Y_3->setMinimum(-500);
        Y_3->setMaximum(500);
        Y_3->setSingleStep(0.01);
        X_3 = new QDoubleSpinBox(CCD_ComputeDlg);
        X_3->setObjectName(QString::fromUtf8("X_3"));
        X_3->setGeometry(QRect(310, 90, 111, 26));
        X_3->setDecimals(4);
        X_3->setMinimum(-500);
        X_3->setMaximum(500);
        X_3->setSingleStep(0.01);
        Z_3 = new QDoubleSpinBox(CCD_ComputeDlg);
        Z_3->setObjectName(QString::fromUtf8("Z_3"));
        Z_3->setGeometry(QRect(310, 50, 111, 26));
        Z_3->setDecimals(4);
        Z_3->setMinimum(-500);
        Z_3->setMaximum(500);
        Z_3->setSingleStep(0.01);
        pushButton = new QPushButton(CCD_ComputeDlg);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(340, 220, 80, 25));
        label = new QLabel(CCD_ComputeDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 55, 51, 17));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2 = new QLabel(CCD_ComputeDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 94, 51, 17));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(CCD_ComputeDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 136, 51, 17));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(CCD_ComputeDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 174, 51, 17));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        retranslateUi(CCD_ComputeDlg);

        QMetaObject::connectSlotsByName(CCD_ComputeDlg);
    } // setupUi

    void retranslateUi(QDialog *CCD_ComputeDlg)
    {
        CCD_ComputeDlg->setWindowTitle(QApplication::translate("CCD_ComputeDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("CCD_ComputeDlg", "Compute", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CCD_ComputeDlg", "Z", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CCD_ComputeDlg", "X", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CCD_ComputeDlg", "Y", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CCD_ComputeDlg", "A", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CCD_ComputeDlg: public Ui_CCD_ComputeDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_COMPUTEDLG_H
