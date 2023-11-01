/********************************************************************************
** Form generated from reading UI file 'best_preciselocationdlg.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEST_PRECISELOCATIONDLG_H
#define UI_BEST_PRECISELOCATIONDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Best_PreciseLocationDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QWidget *Array_Place;
    QGridLayout *gridLayout_2;
    QFrame *Hide1;
    QWidget *Down_Place;
    QHBoxLayout *horizontalLayout_3;
    QFrame *Hide2;
    QWidget *Right_Place;
    QVBoxLayout *verticalLayout_3;
    QFrame *Hide3;
    QSpacerItem *horizontalSpacer_2;
    QGridLayout *gridLayout;
    QPushButton *Btn_CCD;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label;
    QPushButton *SetCoordinate;
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
    QCheckBox *Ck_Single;
    QCheckBox *Ck_All;
    QDoubleSpinBox *value_offa;
    QLabel *label_10;
    QPushButton *Btn_DoOffset;
    QPushButton *Btn_OpenHandle;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *Btn_Cancel;
    QPushButton *Btn_Sure;

    void setupUi(QDialog *Best_PreciseLocationDlg)
    {
        if (Best_PreciseLocationDlg->objectName().isEmpty())
            Best_PreciseLocationDlg->setObjectName(QString::fromUtf8("Best_PreciseLocationDlg"));
        Best_PreciseLocationDlg->resize(859, 558);
        verticalLayout_2 = new QVBoxLayout(Best_PreciseLocationDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        Array_Place = new QWidget(Best_PreciseLocationDlg);
        Array_Place->setObjectName(QString::fromUtf8("Array_Place"));
        gridLayout_2 = new QGridLayout(Array_Place);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        Hide1 = new QFrame(Array_Place);
        Hide1->setObjectName(QString::fromUtf8("Hide1"));
        Hide1->setFrameShape(QFrame::HLine);
        Hide1->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(Hide1, 0, 0, 1, 1);


        verticalLayout->addWidget(Array_Place);

        Down_Place = new QWidget(Best_PreciseLocationDlg);
        Down_Place->setObjectName(QString::fromUtf8("Down_Place"));
        horizontalLayout_3 = new QHBoxLayout(Down_Place);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        Hide2 = new QFrame(Down_Place);
        Hide2->setObjectName(QString::fromUtf8("Hide2"));
        Hide2->setFrameShape(QFrame::VLine);
        Hide2->setFrameShadow(QFrame::Sunken);

        horizontalLayout_3->addWidget(Hide2);


        verticalLayout->addWidget(Down_Place);


        horizontalLayout->addLayout(verticalLayout);

        Right_Place = new QWidget(Best_PreciseLocationDlg);
        Right_Place->setObjectName(QString::fromUtf8("Right_Place"));
        verticalLayout_3 = new QVBoxLayout(Right_Place);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        Hide3 = new QFrame(Right_Place);
        Hide3->setObjectName(QString::fromUtf8("Hide3"));
        Hide3->setFrameShape(QFrame::HLine);
        Hide3->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(Hide3);


        horizontalLayout->addWidget(Right_Place);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        Btn_CCD = new QPushButton(Best_PreciseLocationDlg);
        Btn_CCD->setObjectName(QString::fromUtf8("Btn_CCD"));

        gridLayout->addWidget(Btn_CCD, 0, 0, 1, 1);

        label_2 = new QLabel(Best_PreciseLocationDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 3, 1, 1);

        label_4 = new QLabel(Best_PreciseLocationDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 1, 5, 1, 1);

        label = new QLabel(Best_PreciseLocationDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        SetCoordinate = new QPushButton(Best_PreciseLocationDlg);
        SetCoordinate->setObjectName(QString::fromUtf8("SetCoordinate"));

        gridLayout->addWidget(SetCoordinate, 0, 4, 1, 1);

        pos_y = new QDoubleSpinBox(Best_PreciseLocationDlg);
        pos_y->setObjectName(QString::fromUtf8("pos_y"));
        pos_y->setDecimals(3);
        pos_y->setMinimum(-1000);
        pos_y->setMaximum(1000);

        gridLayout->addWidget(pos_y, 2, 1, 1, 1);

        label_6 = new QLabel(Best_PreciseLocationDlg);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        Btn_Go = new QPushButton(Best_PreciseLocationDlg);
        Btn_Go->setObjectName(QString::fromUtf8("Btn_Go"));

        gridLayout->addWidget(Btn_Go, 0, 1, 1, 1);

        label_7 = new QLabel(Best_PreciseLocationDlg);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        label_9 = new QLabel(Best_PreciseLocationDlg);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_9, 3, 3, 1, 1);

        pos_x_off = new QDoubleSpinBox(Best_PreciseLocationDlg);
        pos_x_off->setObjectName(QString::fromUtf8("pos_x_off"));
        pos_x_off->setDecimals(3);
        pos_x_off->setMinimum(-1000);
        pos_x_off->setMaximum(1000);

        gridLayout->addWidget(pos_x_off, 1, 4, 1, 1);

        pos_y_off = new QDoubleSpinBox(Best_PreciseLocationDlg);
        pos_y_off->setObjectName(QString::fromUtf8("pos_y_off"));
        pos_y_off->setDecimals(3);
        pos_y_off->setMinimum(-1000);
        pos_y_off->setMaximum(1000);

        gridLayout->addWidget(pos_y_off, 2, 4, 1, 1);

        pos_a_off = new QDoubleSpinBox(Best_PreciseLocationDlg);
        pos_a_off->setObjectName(QString::fromUtf8("pos_a_off"));
        pos_a_off->setDecimals(3);
        pos_a_off->setMinimum(-1000);
        pos_a_off->setMaximum(1000);

        gridLayout->addWidget(pos_a_off, 3, 4, 1, 1);

        label_8 = new QLabel(Best_PreciseLocationDlg);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_8, 2, 3, 1, 1);

        pos_x = new QDoubleSpinBox(Best_PreciseLocationDlg);
        pos_x->setObjectName(QString::fromUtf8("pos_x"));
        pos_x->setDecimals(3);
        pos_x->setMinimum(-1000);
        pos_x->setMaximum(1000);

        gridLayout->addWidget(pos_x, 1, 1, 1, 1);

        Piece_Index = new QSpinBox(Best_PreciseLocationDlg);
        Piece_Index->setObjectName(QString::fromUtf8("Piece_Index"));
        Piece_Index->setMaximum(999);

        gridLayout->addWidget(Piece_Index, 0, 3, 1, 1);

        pos_a = new QDoubleSpinBox(Best_PreciseLocationDlg);
        pos_a->setObjectName(QString::fromUtf8("pos_a"));
        pos_a->setDecimals(3);
        pos_a->setMinimum(-1000);
        pos_a->setMaximum(1000);

        gridLayout->addWidget(pos_a, 3, 1, 1, 1);

        value_offx = new QDoubleSpinBox(Best_PreciseLocationDlg);
        value_offx->setObjectName(QString::fromUtf8("value_offx"));
        value_offx->setDecimals(3);
        value_offx->setMinimum(-1000);
        value_offx->setMaximum(1000);

        gridLayout->addWidget(value_offx, 1, 6, 1, 1);

        label_5 = new QLabel(Best_PreciseLocationDlg);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 2, 5, 1, 1);

        value_offy = new QDoubleSpinBox(Best_PreciseLocationDlg);
        value_offy->setObjectName(QString::fromUtf8("value_offy"));
        value_offy->setDecimals(3);
        value_offy->setMinimum(-1000);
        value_offy->setMaximum(1000);

        gridLayout->addWidget(value_offy, 2, 6, 1, 1);

        Ck_Single = new QCheckBox(Best_PreciseLocationDlg);
        Ck_Single->setObjectName(QString::fromUtf8("Ck_Single"));
        Ck_Single->setAutoExclusive(true);

        gridLayout->addWidget(Ck_Single, 1, 7, 1, 1);

        Ck_All = new QCheckBox(Best_PreciseLocationDlg);
        Ck_All->setObjectName(QString::fromUtf8("Ck_All"));
        Ck_All->setAutoExclusive(true);

        gridLayout->addWidget(Ck_All, 2, 7, 1, 1);

        value_offa = new QDoubleSpinBox(Best_PreciseLocationDlg);
        value_offa->setObjectName(QString::fromUtf8("value_offa"));
        value_offa->setDecimals(3);
        value_offa->setMinimum(-1000);
        value_offa->setMaximum(1000);

        gridLayout->addWidget(value_offa, 3, 6, 1, 1);

        label_10 = new QLabel(Best_PreciseLocationDlg);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_10, 3, 5, 1, 1);

        Btn_DoOffset = new QPushButton(Best_PreciseLocationDlg);
        Btn_DoOffset->setObjectName(QString::fromUtf8("Btn_DoOffset"));

        gridLayout->addWidget(Btn_DoOffset, 3, 7, 1, 1);

        Btn_OpenHandle = new QPushButton(Best_PreciseLocationDlg);
        Btn_OpenHandle->setObjectName(QString::fromUtf8("Btn_OpenHandle"));

        gridLayout->addWidget(Btn_OpenHandle, 0, 5, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        Btn_Cancel = new QPushButton(Best_PreciseLocationDlg);
        Btn_Cancel->setObjectName(QString::fromUtf8("Btn_Cancel"));

        horizontalLayout_2->addWidget(Btn_Cancel);

        Btn_Sure = new QPushButton(Best_PreciseLocationDlg);
        Btn_Sure->setObjectName(QString::fromUtf8("Btn_Sure"));

        horizontalLayout_2->addWidget(Btn_Sure);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(Best_PreciseLocationDlg);

        QMetaObject::connectSlotsByName(Best_PreciseLocationDlg);
    } // setupUi

    void retranslateUi(QDialog *Best_PreciseLocationDlg)
    {
        Best_PreciseLocationDlg->setWindowTitle(QApplication::translate("Best_PreciseLocationDlg", "Form", 0, QApplication::UnicodeUTF8));
        Btn_CCD->setText(QApplication::translate("Best_PreciseLocationDlg", "CCD\345\256\232\344\275\215", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Best_PreciseLocationDlg", "X\345\201\217\345\267\256", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Best_PreciseLocationDlg", "X\345\201\217\347\247\273", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Best_PreciseLocationDlg", "X\347\220\206\350\256\272\345\235\220\346\240\207", 0, QApplication::UnicodeUTF8));
        SetCoordinate->setText(QApplication::translate("Best_PreciseLocationDlg", "\350\256\276\345\256\232\345\235\220\346\240\207", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Best_PreciseLocationDlg", "Y\347\220\206\350\256\272\345\235\220\346\240\207", 0, QApplication::UnicodeUTF8));
        Btn_Go->setText(QApplication::translate("Best_PreciseLocationDlg", "Go", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Best_PreciseLocationDlg", "A\347\220\206\350\256\272\345\235\220\346\240\207", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Best_PreciseLocationDlg", "A\345\201\217\345\267\256", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Best_PreciseLocationDlg", "Y\345\201\217\345\267\256", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Best_PreciseLocationDlg", "Y\345\201\217\347\247\273", 0, QApplication::UnicodeUTF8));
        Ck_Single->setText(QApplication::translate("Best_PreciseLocationDlg", "\345\215\225\347\211\207", 0, QApplication::UnicodeUTF8));
        Ck_All->setText(QApplication::translate("Best_PreciseLocationDlg", "\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Best_PreciseLocationDlg", "A\345\201\217\347\247\273", 0, QApplication::UnicodeUTF8));
        Btn_DoOffset->setText(QApplication::translate("Best_PreciseLocationDlg", "\346\211\247\350\241\214", 0, QApplication::UnicodeUTF8));
        Btn_OpenHandle->setText(QApplication::translate("Best_PreciseLocationDlg", "\346\211\223\345\274\200\346\211\213\350\275\256", 0, QApplication::UnicodeUTF8));
        Btn_Cancel->setText(QApplication::translate("Best_PreciseLocationDlg", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Btn_Sure->setText(QApplication::translate("Best_PreciseLocationDlg", "\347\241\256\345\256\232\350\275\250\350\277\271", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Best_PreciseLocationDlg: public Ui_Best_PreciseLocationDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEST_PRECISELOCATIONDLG_H
