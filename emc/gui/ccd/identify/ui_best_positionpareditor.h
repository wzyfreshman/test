/********************************************************************************
** Form generated from reading UI file 'best_positionpareditor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEST_POSITIONPAREDITOR_H
#define UI_BEST_POSITIONPAREDITOR_H

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

class Ui_Best_PositionParEditor
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QFrame *line;
    QGridLayout *gridLayout;
    QDoubleSpinBox *test_speed_global;
    QLabel *label_7;
    QDoubleSpinBox *test_position_z;
    QLabel *label_6;
    QDoubleSpinBox *test_getup_distance;
    QDoubleSpinBox *test_speed_z;
    QDoubleSpinBox *pos_global_offsetx;
    QDoubleSpinBox *pos_global_offsety;
    QPushButton *Btn_ReadZ;
    QLabel *label_9;
    QPushButton *Btn_SaveQuit;
    QLabel *label_5;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_2;
    QPushButton *Btn_DetectCorrect;
    QGridLayout *gridLayout_2;
    QDoubleSpinBox *pos_y2;
    QSpinBox *conf_array_rows;
    QLabel *label_12;
    QSpinBox *conf_array_cols;
    QDoubleSpinBox *pos_y1;
    QLabel *label_10;
    QDoubleSpinBox *pos_x1;
    QLabel *label_11;
    QLabel *label_13;
    QDoubleSpinBox *pos_x2;
    QCheckBox *conf_array_enable;
    QLabel *label_8;
    QLabel *label_14;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3;
    QGridLayout *gridLayout_3;
    QLabel *label_15;
    QLabel *label_19;
    QSpinBox *conf_right_num;
    QDoubleSpinBox *pos_y4;
    QLabel *label_17;
    QCheckBox *conf_right_enable;
    QDoubleSpinBox *pos_y5;
    QLabel *label_18;
    QDoubleSpinBox *pos_x3;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_4;
    QLabel *label_23;
    QCheckBox *conf_down_enable;
    QLabel *label_25;
    QLabel *label_21;
    QDoubleSpinBox *pos_x5;
    QLabel *label_24;
    QSpinBox *conf_down_num;
    QDoubleSpinBox *pos_x4;
    QDoubleSpinBox *pos_y3;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *dddddd;
    QPushButton *Btn_OpenHandle;
    QPushButton *Cancel;
    QPushButton *Sure;

    void setupUi(QDialog *Best_PositionParEditor)
    {
        if (Best_PositionParEditor->objectName().isEmpty())
            Best_PositionParEditor->setObjectName(QString::fromUtf8("Best_PositionParEditor"));
        Best_PositionParEditor->resize(766, 503);
        verticalLayout = new QVBoxLayout(Best_PositionParEditor);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(Best_PositionParEditor);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        line = new QFrame(widget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);


        verticalLayout->addWidget(widget);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        test_speed_global = new QDoubleSpinBox(Best_PositionParEditor);
        test_speed_global->setObjectName(QString::fromUtf8("test_speed_global"));
        test_speed_global->setDecimals(3);
        test_speed_global->setMinimum(-10000);
        test_speed_global->setMaximum(10000);

        gridLayout->addWidget(test_speed_global, 1, 4, 1, 1);

        label_7 = new QLabel(Best_PositionParEditor);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 1, 5, 1, 1);

        test_position_z = new QDoubleSpinBox(Best_PositionParEditor);
        test_position_z->setObjectName(QString::fromUtf8("test_position_z"));
        test_position_z->setDecimals(3);
        test_position_z->setMinimum(-10000);
        test_position_z->setMaximum(10000);

        gridLayout->addWidget(test_position_z, 1, 1, 1, 1);

        label_6 = new QLabel(Best_PositionParEditor);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 1, 3, 1, 1);

        test_getup_distance = new QDoubleSpinBox(Best_PositionParEditor);
        test_getup_distance->setObjectName(QString::fromUtf8("test_getup_distance"));
        test_getup_distance->setDecimals(3);
        test_getup_distance->setMinimum(0.01);
        test_getup_distance->setMaximum(10000);

        gridLayout->addWidget(test_getup_distance, 2, 1, 1, 1);

        test_speed_z = new QDoubleSpinBox(Best_PositionParEditor);
        test_speed_z->setObjectName(QString::fromUtf8("test_speed_z"));
        test_speed_z->setDecimals(3);
        test_speed_z->setMinimum(-10000);
        test_speed_z->setMaximum(10000);

        gridLayout->addWidget(test_speed_z, 2, 4, 1, 1);

        pos_global_offsetx = new QDoubleSpinBox(Best_PositionParEditor);
        pos_global_offsetx->setObjectName(QString::fromUtf8("pos_global_offsetx"));
        pos_global_offsetx->setDecimals(3);
        pos_global_offsetx->setMinimum(-10000);
        pos_global_offsetx->setMaximum(10000);

        gridLayout->addWidget(pos_global_offsetx, 1, 6, 1, 1);

        pos_global_offsety = new QDoubleSpinBox(Best_PositionParEditor);
        pos_global_offsety->setObjectName(QString::fromUtf8("pos_global_offsety"));
        pos_global_offsety->setDecimals(3);
        pos_global_offsety->setMinimum(-10000);
        pos_global_offsety->setMaximum(10000);

        gridLayout->addWidget(pos_global_offsety, 2, 6, 1, 1);

        Btn_ReadZ = new QPushButton(Best_PositionParEditor);
        Btn_ReadZ->setObjectName(QString::fromUtf8("Btn_ReadZ"));

        gridLayout->addWidget(Btn_ReadZ, 1, 2, 1, 1);

        label_9 = new QLabel(Best_PositionParEditor);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_9, 2, 5, 1, 1);

        Btn_SaveQuit = new QPushButton(Best_PositionParEditor);
        Btn_SaveQuit->setObjectName(QString::fromUtf8("Btn_SaveQuit"));

        gridLayout->addWidget(Btn_SaveQuit, 2, 2, 1, 1);

        label_5 = new QLabel(Best_PositionParEditor);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        label = new QLabel(Best_PositionParEditor);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_4 = new QLabel(Best_PositionParEditor);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 2, 3, 1, 1);

        label_2 = new QLabel(Best_PositionParEditor);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        Btn_DetectCorrect = new QPushButton(Best_PositionParEditor);
        Btn_DetectCorrect->setObjectName(QString::fromUtf8("Btn_DetectCorrect"));

        gridLayout->addWidget(Btn_DetectCorrect, 1, 7, 1, 1);


        verticalLayout->addLayout(gridLayout);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pos_y2 = new QDoubleSpinBox(Best_PositionParEditor);
        pos_y2->setObjectName(QString::fromUtf8("pos_y2"));
        pos_y2->setDecimals(3);
        pos_y2->setMinimum(-10000);
        pos_y2->setMaximum(10000);

        gridLayout_2->addWidget(pos_y2, 2, 8, 1, 1);

        conf_array_rows = new QSpinBox(Best_PositionParEditor);
        conf_array_rows->setObjectName(QString::fromUtf8("conf_array_rows"));

        gridLayout_2->addWidget(conf_array_rows, 1, 4, 1, 1);

        label_12 = new QLabel(Best_PositionParEditor);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_12, 2, 3, 1, 1);

        conf_array_cols = new QSpinBox(Best_PositionParEditor);
        conf_array_cols->setObjectName(QString::fromUtf8("conf_array_cols"));

        gridLayout_2->addWidget(conf_array_cols, 1, 6, 1, 1);

        pos_y1 = new QDoubleSpinBox(Best_PositionParEditor);
        pos_y1->setObjectName(QString::fromUtf8("pos_y1"));
        pos_y1->setDecimals(3);
        pos_y1->setMinimum(-10000);
        pos_y1->setMaximum(10000);

        gridLayout_2->addWidget(pos_y1, 2, 4, 1, 1);

        label_10 = new QLabel(Best_PositionParEditor);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_10, 1, 5, 1, 1);

        pos_x1 = new QDoubleSpinBox(Best_PositionParEditor);
        pos_x1->setObjectName(QString::fromUtf8("pos_x1"));
        pos_x1->setDecimals(3);
        pos_x1->setMinimum(-10000);
        pos_x1->setMaximum(10000);

        gridLayout_2->addWidget(pos_x1, 2, 2, 1, 1);

        label_11 = new QLabel(Best_PositionParEditor);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_11, 2, 1, 1, 1);

        label_13 = new QLabel(Best_PositionParEditor);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_13, 2, 5, 1, 1);

        pos_x2 = new QDoubleSpinBox(Best_PositionParEditor);
        pos_x2->setObjectName(QString::fromUtf8("pos_x2"));
        pos_x2->setDecimals(3);
        pos_x2->setMinimum(-10000);
        pos_x2->setMaximum(10000);

        gridLayout_2->addWidget(pos_x2, 2, 6, 1, 1);

        conf_array_enable = new QCheckBox(Best_PositionParEditor);
        conf_array_enable->setObjectName(QString::fromUtf8("conf_array_enable"));

        gridLayout_2->addWidget(conf_array_enable, 1, 2, 1, 1);

        label_8 = new QLabel(Best_PositionParEditor);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 1, 3, 1, 1);

        label_14 = new QLabel(Best_PositionParEditor);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_14, 2, 7, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 2, 9, 1, 1);

        label_3 = new QLabel(Best_PositionParEditor);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 2, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_15 = new QLabel(Best_PositionParEditor);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_15, 1, 3, 1, 1);

        label_19 = new QLabel(Best_PositionParEditor);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_19, 0, 3, 1, 1);

        conf_right_num = new QSpinBox(Best_PositionParEditor);
        conf_right_num->setObjectName(QString::fromUtf8("conf_right_num"));

        gridLayout_3->addWidget(conf_right_num, 0, 4, 1, 1);

        pos_y4 = new QDoubleSpinBox(Best_PositionParEditor);
        pos_y4->setObjectName(QString::fromUtf8("pos_y4"));
        pos_y4->setDecimals(3);
        pos_y4->setMinimum(-10000);
        pos_y4->setMaximum(10000);

        gridLayout_3->addWidget(pos_y4, 1, 4, 1, 1);

        label_17 = new QLabel(Best_PositionParEditor);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_17, 1, 1, 1, 1);

        conf_right_enable = new QCheckBox(Best_PositionParEditor);
        conf_right_enable->setObjectName(QString::fromUtf8("conf_right_enable"));

        gridLayout_3->addWidget(conf_right_enable, 0, 2, 1, 1);

        pos_y5 = new QDoubleSpinBox(Best_PositionParEditor);
        pos_y5->setObjectName(QString::fromUtf8("pos_y5"));
        pos_y5->setDecimals(3);
        pos_y5->setMinimum(-10000);
        pos_y5->setMaximum(10000);

        gridLayout_3->addWidget(pos_y5, 1, 6, 1, 1);

        label_18 = new QLabel(Best_PositionParEditor);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_18, 1, 5, 1, 1);

        pos_x3 = new QDoubleSpinBox(Best_PositionParEditor);
        pos_x3->setObjectName(QString::fromUtf8("pos_x3"));
        pos_x3->setDecimals(3);
        pos_x3->setMinimum(-10000);
        pos_x3->setMaximum(10000);

        gridLayout_3->addWidget(pos_x3, 1, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 1, 7, 1, 1);


        verticalLayout->addLayout(gridLayout_3);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_23 = new QLabel(Best_PositionParEditor);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_23, 1, 1, 1, 1);

        conf_down_enable = new QCheckBox(Best_PositionParEditor);
        conf_down_enable->setObjectName(QString::fromUtf8("conf_down_enable"));

        gridLayout_4->addWidget(conf_down_enable, 0, 2, 1, 1);

        label_25 = new QLabel(Best_PositionParEditor);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_25, 0, 3, 1, 1);

        label_21 = new QLabel(Best_PositionParEditor);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_21, 1, 3, 1, 1);

        pos_x5 = new QDoubleSpinBox(Best_PositionParEditor);
        pos_x5->setObjectName(QString::fromUtf8("pos_x5"));
        pos_x5->setDecimals(3);
        pos_x5->setMinimum(-10000);
        pos_x5->setMaximum(10000);

        gridLayout_4->addWidget(pos_x5, 1, 6, 1, 1);

        label_24 = new QLabel(Best_PositionParEditor);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_24, 1, 5, 1, 1);

        conf_down_num = new QSpinBox(Best_PositionParEditor);
        conf_down_num->setObjectName(QString::fromUtf8("conf_down_num"));

        gridLayout_4->addWidget(conf_down_num, 0, 4, 1, 1);

        pos_x4 = new QDoubleSpinBox(Best_PositionParEditor);
        pos_x4->setObjectName(QString::fromUtf8("pos_x4"));
        pos_x4->setDecimals(3);
        pos_x4->setMinimum(-10000);
        pos_x4->setMaximum(10000);

        gridLayout_4->addWidget(pos_x4, 1, 4, 1, 1);

        pos_y3 = new QDoubleSpinBox(Best_PositionParEditor);
        pos_y3->setObjectName(QString::fromUtf8("pos_y3"));
        pos_y3->setDecimals(3);
        pos_y3->setMinimum(-10000);
        pos_y3->setMaximum(10000);

        gridLayout_4->addWidget(pos_y3, 1, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 1, 7, 1, 1);


        verticalLayout->addLayout(gridLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        dddddd = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(dddddd);

        Btn_OpenHandle = new QPushButton(Best_PositionParEditor);
        Btn_OpenHandle->setObjectName(QString::fromUtf8("Btn_OpenHandle"));

        horizontalLayout->addWidget(Btn_OpenHandle);

        Cancel = new QPushButton(Best_PositionParEditor);
        Cancel->setObjectName(QString::fromUtf8("Cancel"));

        horizontalLayout->addWidget(Cancel);

        Sure = new QPushButton(Best_PositionParEditor);
        Sure->setObjectName(QString::fromUtf8("Sure"));

        horizontalLayout->addWidget(Sure);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Best_PositionParEditor);

        QMetaObject::connectSlotsByName(Best_PositionParEditor);
    } // setupUi

    void retranslateUi(QDialog *Best_PositionParEditor)
    {
        Best_PositionParEditor->setWindowTitle(QApplication::translate("Best_PositionParEditor", "Dialog", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Best_PositionParEditor", "\345\205\254\345\205\261\345\201\217\345\267\256X", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Best_PositionParEditor", "\346\265\213\350\257\225\351\200\237\345\272\246", 0, QApplication::UnicodeUTF8));
        Btn_ReadZ->setText(QApplication::translate("Best_PositionParEditor", "\350\257\273\345\217\226\345\275\223\345\211\215\345\200\274", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Best_PositionParEditor", "\345\205\254\345\205\261\345\201\217\345\267\256Y", 0, QApplication::UnicodeUTF8));
        Btn_SaveQuit->setText(QApplication::translate("Best_PositionParEditor", "\344\277\235\345\255\230\346\265\213\350\257\225\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Best_PositionParEditor", "\346\265\213\350\257\225\345\244\264\346\212\254\350\265\267\351\253\230\345\272\246", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Best_PositionParEditor", "\346\265\213\350\257\225\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Best_PositionParEditor", "\346\265\213\350\257\225\345\244\264\344\270\213\350\220\275\351\200\237\345\272\246", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Best_PositionParEditor", "\346\265\213\350\257\225Z\350\275\264\345\235\220\346\240\207", 0, QApplication::UnicodeUTF8));
        Btn_DetectCorrect->setText(QApplication::translate("Best_PositionParEditor", "\346\265\213\350\257\225\345\244\264\346\240\241\346\255\243", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Best_PositionParEditor", "Y1", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Best_PositionParEditor", "\350\241\214\346\225\260", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Best_PositionParEditor", "X1", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Best_PositionParEditor", "X2", 0, QApplication::UnicodeUTF8));
        conf_array_enable->setText(QApplication::translate("Best_PositionParEditor", "\346\240\207\345\207\206\351\230\265\345\210\227", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Best_PositionParEditor", "\345\210\227\346\225\260", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Best_PositionParEditor", "Y2", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Best_PositionParEditor", "\345\256\232\344\275\215\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("Best_PositionParEditor", "Y4", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("Best_PositionParEditor", "\344\270\252\346\225\260", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("Best_PositionParEditor", "X3", 0, QApplication::UnicodeUTF8));
        conf_right_enable->setText(QApplication::translate("Best_PositionParEditor", "\345\217\263\350\276\271\346\227\213\350\275\254", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Best_PositionParEditor", "Y5", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("Best_PositionParEditor", "Y3", 0, QApplication::UnicodeUTF8));
        conf_down_enable->setText(QApplication::translate("Best_PositionParEditor", "\344\270\213\346\226\271\346\227\213\350\275\254", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("Best_PositionParEditor", "\344\270\252\346\225\260", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("Best_PositionParEditor", "X4", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("Best_PositionParEditor", "X5", 0, QApplication::UnicodeUTF8));
        Btn_OpenHandle->setText(QApplication::translate("Best_PositionParEditor", "\346\211\223\345\274\200\346\211\213\350\275\256", 0, QApplication::UnicodeUTF8));
        Cancel->setText(QApplication::translate("Best_PositionParEditor", "\345\217\226\346\266\210\350\277\224\345\233\236", 0, QApplication::UnicodeUTF8));
        Sure->setText(QApplication::translate("Best_PositionParEditor", "\347\241\256\345\256\232\345\271\266\347\262\276\345\256\232\344\275\215", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Best_PositionParEditor: public Ui_Best_PositionParEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEST_POSITIONPAREDITOR_H
