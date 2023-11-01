/********************************************************************************
** Form generated from reading UI file 'best_detectcorrect.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEST_DETECTCORRECT_H
#define UI_BEST_DETECTCORRECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Best_DetectCorrect
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QPushButton *Btn_CCD;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label;
    QDoubleSpinBox *pos_y;
    QLabel *label_6;
    QPushButton *Btn_Go;
    QLabel *label_7;
    QLabel *label_9;
    QDoubleSpinBox *pos_x_off;
    QDoubleSpinBox *pos_y_off;
    QDoubleSpinBox *pos_a_off;
    QLabel *label_8;
    QDoubleSpinBox *pos_x;
    QSpinBox *Piece_Index;
    QDoubleSpinBox *pos_a;
    QDoubleSpinBox *value_offx;
    QLabel *label_5;
    QDoubleSpinBox *value_offy;
    QDoubleSpinBox *value_offa;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *Btn_Cancel;
    QPushButton *Btn_DoOffset;

    void setupUi(QDialog *Best_DetectCorrect)
    {
        if (Best_DetectCorrect->objectName().isEmpty())
            Best_DetectCorrect->setObjectName(QString::fromUtf8("Best_DetectCorrect"));
        Best_DetectCorrect->resize(620, 200);
        verticalLayout = new QVBoxLayout(Best_DetectCorrect);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        Btn_CCD = new QPushButton(Best_DetectCorrect);
        Btn_CCD->setObjectName(QString::fromUtf8("Btn_CCD"));

        gridLayout->addWidget(Btn_CCD, 0, 0, 1, 1);

        label_2 = new QLabel(Best_DetectCorrect);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 3, 1, 1);

        label_4 = new QLabel(Best_DetectCorrect);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 1, 5, 1, 1);

        label = new QLabel(Best_DetectCorrect);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        pos_y = new QDoubleSpinBox(Best_DetectCorrect);
        pos_y->setObjectName(QString::fromUtf8("pos_y"));
        pos_y->setDecimals(3);
        pos_y->setMinimum(-1000);
        pos_y->setMaximum(1000);

        gridLayout->addWidget(pos_y, 2, 1, 1, 1);

        label_6 = new QLabel(Best_DetectCorrect);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        Btn_Go = new QPushButton(Best_DetectCorrect);
        Btn_Go->setObjectName(QString::fromUtf8("Btn_Go"));

        gridLayout->addWidget(Btn_Go, 0, 1, 1, 1);

        label_7 = new QLabel(Best_DetectCorrect);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        label_9 = new QLabel(Best_DetectCorrect);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_9, 3, 3, 1, 1);

        pos_x_off = new QDoubleSpinBox(Best_DetectCorrect);
        pos_x_off->setObjectName(QString::fromUtf8("pos_x_off"));
        pos_x_off->setDecimals(3);
        pos_x_off->setMinimum(-1000);
        pos_x_off->setMaximum(1000);

        gridLayout->addWidget(pos_x_off, 1, 4, 1, 1);

        pos_y_off = new QDoubleSpinBox(Best_DetectCorrect);
        pos_y_off->setObjectName(QString::fromUtf8("pos_y_off"));
        pos_y_off->setDecimals(3);
        pos_y_off->setMinimum(-1000);
        pos_y_off->setMaximum(1000);

        gridLayout->addWidget(pos_y_off, 2, 4, 1, 1);

        pos_a_off = new QDoubleSpinBox(Best_DetectCorrect);
        pos_a_off->setObjectName(QString::fromUtf8("pos_a_off"));
        pos_a_off->setDecimals(3);
        pos_a_off->setMinimum(-1000);
        pos_a_off->setMaximum(1000);

        gridLayout->addWidget(pos_a_off, 3, 4, 1, 1);

        label_8 = new QLabel(Best_DetectCorrect);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_8, 2, 3, 1, 1);

        pos_x = new QDoubleSpinBox(Best_DetectCorrect);
        pos_x->setObjectName(QString::fromUtf8("pos_x"));
        pos_x->setDecimals(3);
        pos_x->setMinimum(-1000);
        pos_x->setMaximum(1000);

        gridLayout->addWidget(pos_x, 1, 1, 1, 1);

        Piece_Index = new QSpinBox(Best_DetectCorrect);
        Piece_Index->setObjectName(QString::fromUtf8("Piece_Index"));
        Piece_Index->setMaximum(999);

        gridLayout->addWidget(Piece_Index, 0, 3, 1, 1);

        pos_a = new QDoubleSpinBox(Best_DetectCorrect);
        pos_a->setObjectName(QString::fromUtf8("pos_a"));
        pos_a->setDecimals(3);
        pos_a->setMinimum(-1000);
        pos_a->setMaximum(1000);

        gridLayout->addWidget(pos_a, 3, 1, 1, 1);

        value_offx = new QDoubleSpinBox(Best_DetectCorrect);
        value_offx->setObjectName(QString::fromUtf8("value_offx"));
        value_offx->setDecimals(3);
        value_offx->setMinimum(-1000);
        value_offx->setMaximum(1000);

        gridLayout->addWidget(value_offx, 1, 6, 1, 1);

        label_5 = new QLabel(Best_DetectCorrect);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 2, 5, 1, 1);

        value_offy = new QDoubleSpinBox(Best_DetectCorrect);
        value_offy->setObjectName(QString::fromUtf8("value_offy"));
        value_offy->setDecimals(3);
        value_offy->setMinimum(-1000);
        value_offy->setMaximum(1000);

        gridLayout->addWidget(value_offy, 2, 6, 1, 1);

        value_offa = new QDoubleSpinBox(Best_DetectCorrect);
        value_offa->setObjectName(QString::fromUtf8("value_offa"));
        value_offa->setDecimals(3);
        value_offa->setMinimum(-1000);
        value_offa->setMaximum(1000);

        gridLayout->addWidget(value_offa, 3, 6, 1, 1);

        label_10 = new QLabel(Best_DetectCorrect);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_10, 3, 5, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        Btn_Cancel = new QPushButton(Best_DetectCorrect);
        Btn_Cancel->setObjectName(QString::fromUtf8("Btn_Cancel"));

        horizontalLayout_2->addWidget(Btn_Cancel);

        Btn_DoOffset = new QPushButton(Best_DetectCorrect);
        Btn_DoOffset->setObjectName(QString::fromUtf8("Btn_DoOffset"));

        horizontalLayout_2->addWidget(Btn_DoOffset);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(Best_DetectCorrect);

        QMetaObject::connectSlotsByName(Best_DetectCorrect);
    } // setupUi

    void retranslateUi(QDialog *Best_DetectCorrect)
    {
        Best_DetectCorrect->setWindowTitle(QApplication::translate("Best_DetectCorrect", "Dialog", 0, QApplication::UnicodeUTF8));
        Btn_CCD->setText(QApplication::translate("Best_DetectCorrect", "CCD\345\256\232\344\275\215", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Best_DetectCorrect", "X\345\201\217\345\267\256", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Best_DetectCorrect", "X\345\201\217\347\247\273\350\256\276\345\256\232", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Best_DetectCorrect", "X\347\220\206\350\256\272\345\235\220\346\240\207", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Best_DetectCorrect", "Y\347\220\206\350\256\272\345\235\220\346\240\207", 0, QApplication::UnicodeUTF8));
        Btn_Go->setText(QApplication::translate("Best_DetectCorrect", "Go", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Best_DetectCorrect", "A\347\220\206\350\256\272\345\235\220\346\240\207", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Best_DetectCorrect", "A\345\201\217\345\267\256", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Best_DetectCorrect", "Y\345\201\217\345\267\256", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Best_DetectCorrect", "Y\345\201\217\347\247\273\350\256\276\345\256\232", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Best_DetectCorrect", "A\345\201\217\347\247\273\350\256\276\345\256\232", 0, QApplication::UnicodeUTF8));
        Btn_Cancel->setText(QApplication::translate("Best_DetectCorrect", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Btn_DoOffset->setText(QApplication::translate("Best_DetectCorrect", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Best_DetectCorrect: public Ui_Best_DetectCorrect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEST_DETECTCORRECT_H
