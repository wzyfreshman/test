/********************************************************************************
** Form generated from reading UI file 'multioffset.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTIOFFSET_H
#define UI_MULTIOFFSET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_multioffset
{
public:
    QWidget *PUBLIC_FRAME;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label;
    QDoubleSpinBox *public_offset_x;
    QLabel *label_2;
    QDoubleSpinBox *public_offset_y;
    QLabel *label_p_z1;
    QDoubleSpinBox *public_offset_z1;
    QLabel *label_p_z2;
    QDoubleSpinBox *public_offset_z2;
    QWidget *INCR_FRAME;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QPushButton *btn_raise_1;
    QLabel *label_13;
    QPushButton *btn_dec_001;
    QPushButton *btn_dec_01;
    QPushButton *btn_dec_1;
    QLabel *label_14;
    QPushButton *btn_raise_001;
    QPushButton *btn_raise_01;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout;
    QLabel *label_15;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_set_x;
    QPushButton *btn_set_y;
    QWidget *CHO_FAME;
    QWidget *widget_7;
    QVBoxLayout *verticalLayout_5;
    QWidget *widget_8;
    QVBoxLayout *verticalLayout_13;
    QLabel *label_4;
    QLabel *system_now;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_17;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_record_x;
    QLabel *show_record_x;
    QPushButton *btn_dichotomy_x;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_record_y;
    QLabel *show_record_y;
    QPushButton *btn_dichotomy_y;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_ensure;
    QPushButton *btn_cancel;
    QWidget *PIECE_FRAME;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QWidget *group56;
    QVBoxLayout *verticalLayout_8;
    QDoubleSpinBox *g56_x;
    QDoubleSpinBox *g56_y;
    QDoubleSpinBox *g56_z;
    QDoubleSpinBox *g56_a;
    QWidget *widget_15;
    QVBoxLayout *verticalLayout_12;
    QLabel *label_6;
    QLabel *label_8;
    QLabel *label_o_z1;
    QLabel *label_o_z2;
    QRadioButton *set_g59;
    QWidget *group58;
    QVBoxLayout *verticalLayout_10;
    QDoubleSpinBox *g58_x;
    QDoubleSpinBox *g58_y;
    QDoubleSpinBox *g58_z;
    QDoubleSpinBox *g58_a;
    QWidget *group55;
    QVBoxLayout *verticalLayout_7;
    QDoubleSpinBox *g55_x;
    QDoubleSpinBox *g55_y;
    QDoubleSpinBox *g55_z;
    QDoubleSpinBox *g55_a;
    QWidget *group54;
    QVBoxLayout *verticalLayout_6;
    QDoubleSpinBox *g54_x;
    QDoubleSpinBox *g54_y;
    QDoubleSpinBox *g54_z;
    QDoubleSpinBox *g54_a;
    QWidget *group57;
    QVBoxLayout *verticalLayout_9;
    QDoubleSpinBox *g57_x;
    QDoubleSpinBox *g57_y;
    QDoubleSpinBox *g57_z;
    QDoubleSpinBox *g57_a;
    QWidget *group59;
    QVBoxLayout *verticalLayout_11;
    QDoubleSpinBox *g59_x;
    QDoubleSpinBox *g59_y;
    QDoubleSpinBox *g59_z;
    QDoubleSpinBox *g59_a;
    QRadioButton *set_g54;
    QRadioButton *set_g55;
    QRadioButton *set_g56;
    QRadioButton *set_g57;
    QRadioButton *set_g58;
    QWidget *COOR_FRAME;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_12;
    QLabel *piece_coor_x54;
    QLabel *piece_coor_x55;
    QLabel *piece_coor_x56;
    QLabel *piece_coor_x57;
    QLabel *piece_coor_x58;
    QLabel *piece_coor_x59;
    QLabel *label_11;
    QLabel *piece_coor_y54;
    QLabel *piece_coor_y55;
    QLabel *piece_coor_y56;
    QLabel *piece_coor_y57;
    QLabel *piece_coor_y58;
    QLabel *piece_coor_y59;
    QLabel *label_c_z1;
    QLabel *piece_coor_z154;
    QLabel *piece_coor_z155;
    QLabel *piece_coor_z156;
    QLabel *piece_coor_z157;
    QLabel *piece_coor_z158;
    QLabel *piece_coor_z159;
    QLabel *label_c_z2;
    QLabel *piece_coor_z254;
    QLabel *piece_coor_z255;
    QLabel *piece_coor_z256;
    QLabel *piece_coor_z257;
    QLabel *piece_coor_z258;
    QLabel *piece_coor_z259;

    void setupUi(QDialog *multioffset)
    {
        if (multioffset->objectName().isEmpty())
            multioffset->setObjectName(QString::fromUtf8("multioffset"));
        multioffset->resize(562, 727);
        PUBLIC_FRAME = new QWidget(multioffset);
        PUBLIC_FRAME->setObjectName(QString::fromUtf8("PUBLIC_FRAME"));
        PUBLIC_FRAME->setGeometry(QRect(9, 9, 521, 181));
        groupBox = new QGroupBox(PUBLIC_FRAME);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(9, 2, 121, 155));
        groupBox->setStyleSheet(QString::fromUtf8(""));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        public_offset_x = new QDoubleSpinBox(groupBox);
        public_offset_x->setObjectName(QString::fromUtf8("public_offset_x"));
        public_offset_x->setButtonSymbols(QAbstractSpinBox::NoButtons);
        public_offset_x->setDecimals(3);
        public_offset_x->setMinimum(-10000);
        public_offset_x->setMaximum(10000);

        formLayout->setWidget(0, QFormLayout::FieldRole, public_offset_x);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        public_offset_y = new QDoubleSpinBox(groupBox);
        public_offset_y->setObjectName(QString::fromUtf8("public_offset_y"));
        public_offset_y->setButtonSymbols(QAbstractSpinBox::NoButtons);
        public_offset_y->setDecimals(3);
        public_offset_y->setMinimum(-10000);
        public_offset_y->setMaximum(10000);

        formLayout->setWidget(1, QFormLayout::FieldRole, public_offset_y);

        label_p_z1 = new QLabel(groupBox);
        label_p_z1->setObjectName(QString::fromUtf8("label_p_z1"));
        label_p_z1->setFont(font);
        label_p_z1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_p_z1);

        public_offset_z1 = new QDoubleSpinBox(groupBox);
        public_offset_z1->setObjectName(QString::fromUtf8("public_offset_z1"));
        public_offset_z1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        public_offset_z1->setDecimals(3);
        public_offset_z1->setMinimum(-10000);
        public_offset_z1->setMaximum(10000);

        formLayout->setWidget(2, QFormLayout::FieldRole, public_offset_z1);

        label_p_z2 = new QLabel(groupBox);
        label_p_z2->setObjectName(QString::fromUtf8("label_p_z2"));
        label_p_z2->setFont(font);
        label_p_z2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_p_z2);

        public_offset_z2 = new QDoubleSpinBox(groupBox);
        public_offset_z2->setObjectName(QString::fromUtf8("public_offset_z2"));
        public_offset_z2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        public_offset_z2->setDecimals(3);
        public_offset_z2->setMinimum(-10000);
        public_offset_z2->setMaximum(10000);

        formLayout->setWidget(3, QFormLayout::FieldRole, public_offset_z2);

        INCR_FRAME = new QWidget(PUBLIC_FRAME);
        INCR_FRAME->setObjectName(QString::fromUtf8("INCR_FRAME"));
        INCR_FRAME->setGeometry(QRect(140, 0, 370, 91));
        groupBox_4 = new QGroupBox(INCR_FRAME);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(9, 2, 352, 77));
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(6);
        gridLayout_2->setVerticalSpacing(2);
        gridLayout_2->setContentsMargins(-1, 2, -1, 1);
        btn_raise_1 = new QPushButton(groupBox_4);
        btn_raise_1->setObjectName(QString::fromUtf8("btn_raise_1"));
        btn_raise_1->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn_raise_1, 1, 3, 1, 1);

        label_13 = new QLabel(groupBox_4);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        QFont font1;
        font1.setPointSize(14);
        label_13->setFont(font1);

        gridLayout_2->addWidget(label_13, 0, 0, 1, 1);

        btn_dec_001 = new QPushButton(groupBox_4);
        btn_dec_001->setObjectName(QString::fromUtf8("btn_dec_001"));
        btn_dec_001->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn_dec_001, 0, 1, 1, 1);

        btn_dec_01 = new QPushButton(groupBox_4);
        btn_dec_01->setObjectName(QString::fromUtf8("btn_dec_01"));
        btn_dec_01->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn_dec_01, 0, 2, 1, 1);

        btn_dec_1 = new QPushButton(groupBox_4);
        btn_dec_1->setObjectName(QString::fromUtf8("btn_dec_1"));
        btn_dec_1->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn_dec_1, 0, 3, 1, 1);

        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font1);

        gridLayout_2->addWidget(label_14, 1, 0, 1, 1);

        btn_raise_001 = new QPushButton(groupBox_4);
        btn_raise_001->setObjectName(QString::fromUtf8("btn_raise_001"));
        btn_raise_001->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn_raise_001, 1, 1, 1, 1);

        btn_raise_01 = new QPushButton(groupBox_4);
        btn_raise_01->setObjectName(QString::fromUtf8("btn_raise_01"));
        btn_raise_01->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(btn_raise_01, 1, 2, 1, 1);

        groupBox_5 = new QGroupBox(PUBLIC_FRAME);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(140, 90, 352, 91));
        verticalLayout = new QVBoxLayout(groupBox_5);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 5, 2, 2);
        label_15 = new QLabel(groupBox_5);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setWordWrap(true);

        verticalLayout->addWidget(label_15);

        widget = new QWidget(groupBox_5);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, 0);
        btn_set_x = new QPushButton(widget);
        btn_set_x->setObjectName(QString::fromUtf8("btn_set_x"));
        btn_set_x->setMinimumSize(QSize(0, 30));
        btn_set_x->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(btn_set_x);

        btn_set_y = new QPushButton(widget);
        btn_set_y->setObjectName(QString::fromUtf8("btn_set_y"));
        btn_set_y->setMinimumSize(QSize(0, 30));
        btn_set_y->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(btn_set_y);


        verticalLayout->addWidget(widget);

        CHO_FAME = new QWidget(multioffset);
        CHO_FAME->setObjectName(QString::fromUtf8("CHO_FAME"));
        CHO_FAME->setGeometry(QRect(9, 528, 481, 162));
        widget_7 = new QWidget(CHO_FAME);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        widget_7->setGeometry(QRect(9, 9, 131, 111));
        verticalLayout_5 = new QVBoxLayout(widget_7);
        verticalLayout_5->setSpacing(2);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(-1, 0, -1, 0);
        widget_8 = new QWidget(widget_7);
        widget_8->setObjectName(QString::fromUtf8("widget_8"));
        verticalLayout_13 = new QVBoxLayout(widget_8);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        verticalLayout_13->setContentsMargins(-1, 0, -1, 0);
        label_4 = new QLabel(widget_8);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_13->addWidget(label_4);

        system_now = new QLabel(widget_8);
        system_now->setObjectName(QString::fromUtf8("system_now"));
        QFont font2;
        font2.setPointSize(25);
        system_now->setFont(font2);
        system_now->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        verticalLayout_13->addWidget(system_now);

        verticalLayout_13->setStretch(1, 1);

        verticalLayout_5->addWidget(widget_8);

        groupBox_6 = new QGroupBox(CHO_FAME);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(173, 9, 293, 144));
        verticalLayout_2 = new QVBoxLayout(groupBox_6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_17 = new QLabel(groupBox_6);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setWordWrap(true);

        verticalLayout_2->addWidget(label_17);

        widget_2 = new QWidget(groupBox_6);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        btn_record_x = new QPushButton(widget_2);
        btn_record_x->setObjectName(QString::fromUtf8("btn_record_x"));
        btn_record_x->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_2->addWidget(btn_record_x);

        show_record_x = new QLabel(widget_2);
        show_record_x->setObjectName(QString::fromUtf8("show_record_x"));
        show_record_x->setFont(font1);
        show_record_x->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        show_record_x->setFrameShape(QFrame::Box);
        show_record_x->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(show_record_x);

        btn_dichotomy_x = new QPushButton(widget_2);
        btn_dichotomy_x->setObjectName(QString::fromUtf8("btn_dichotomy_x"));
        btn_dichotomy_x->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_2->addWidget(btn_dichotomy_x);


        verticalLayout_2->addWidget(widget_2);

        widget_3 = new QWidget(groupBox_6);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 0, -1, 0);
        btn_record_y = new QPushButton(widget_3);
        btn_record_y->setObjectName(QString::fromUtf8("btn_record_y"));
        btn_record_y->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_3->addWidget(btn_record_y);

        show_record_y = new QLabel(widget_3);
        show_record_y->setObjectName(QString::fromUtf8("show_record_y"));
        show_record_y->setFont(font1);
        show_record_y->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        show_record_y->setFrameShape(QFrame::Box);
        show_record_y->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(show_record_y);

        btn_dichotomy_y = new QPushButton(widget_3);
        btn_dichotomy_y->setObjectName(QString::fromUtf8("btn_dichotomy_y"));
        btn_dichotomy_y->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_3->addWidget(btn_dichotomy_y);


        verticalLayout_2->addWidget(widget_3);

        frame = new QFrame(multioffset);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(9, 696, 212, 26));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame);
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        btn_ensure = new QPushButton(frame);
        btn_ensure->setObjectName(QString::fromUtf8("btn_ensure"));
        btn_ensure->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_4->addWidget(btn_ensure);

        btn_cancel = new QPushButton(frame);
        btn_cancel->setObjectName(QString::fromUtf8("btn_cancel"));
        btn_cancel->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_4->addWidget(btn_cancel);

        PIECE_FRAME = new QWidget(multioffset);
        PIECE_FRAME->setObjectName(QString::fromUtf8("PIECE_FRAME"));
        PIECE_FRAME->setGeometry(QRect(9, 179, 521, 183));
        groupBox_2 = new QGroupBox(PIECE_FRAME);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(9, 9, 321, 161));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(5, -1, 2, 2);
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        group56 = new QWidget(groupBox_2);
        group56->setObjectName(QString::fromUtf8("group56"));
        verticalLayout_8 = new QVBoxLayout(group56);
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(1, 2, 2, 2);
        g56_x = new QDoubleSpinBox(group56);
        g56_x->setObjectName(QString::fromUtf8("g56_x"));
        g56_x->setWrapping(false);
        g56_x->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g56_x->setAccelerated(false);
        g56_x->setDecimals(3);
        g56_x->setMinimum(-10000);
        g56_x->setMaximum(10000);

        verticalLayout_8->addWidget(g56_x);

        g56_y = new QDoubleSpinBox(group56);
        g56_y->setObjectName(QString::fromUtf8("g56_y"));
        g56_y->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g56_y->setDecimals(3);
        g56_y->setMinimum(-10000);
        g56_y->setMaximum(10000);

        verticalLayout_8->addWidget(g56_y);

        g56_z = new QDoubleSpinBox(group56);
        g56_z->setObjectName(QString::fromUtf8("g56_z"));
        g56_z->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g56_z->setDecimals(3);
        g56_z->setMinimum(-10000);
        g56_z->setMaximum(10000);

        verticalLayout_8->addWidget(g56_z);

        g56_a = new QDoubleSpinBox(group56);
        g56_a->setObjectName(QString::fromUtf8("g56_a"));
        g56_a->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g56_a->setDecimals(3);
        g56_a->setMinimum(-10000);
        g56_a->setMaximum(10000);

        verticalLayout_8->addWidget(g56_a);


        gridLayout->addWidget(group56, 1, 3, 1, 1);

        widget_15 = new QWidget(groupBox_2);
        widget_15->setObjectName(QString::fromUtf8("widget_15"));
        widget_15->setMaximumSize(QSize(40, 16777215));
        verticalLayout_12 = new QVBoxLayout(widget_15);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(4, -1, -1, -1);
        label_6 = new QLabel(widget_15);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_12->addWidget(label_6);

        label_8 = new QLabel(widget_15);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_12->addWidget(label_8);

        label_o_z1 = new QLabel(widget_15);
        label_o_z1->setObjectName(QString::fromUtf8("label_o_z1"));
        label_o_z1->setFont(font);
        label_o_z1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_12->addWidget(label_o_z1);

        label_o_z2 = new QLabel(widget_15);
        label_o_z2->setObjectName(QString::fromUtf8("label_o_z2"));
        label_o_z2->setFont(font);
        label_o_z2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_12->addWidget(label_o_z2);


        gridLayout->addWidget(widget_15, 1, 0, 1, 1);

        set_g59 = new QRadioButton(groupBox_2);
        set_g59->setObjectName(QString::fromUtf8("set_g59"));

        gridLayout->addWidget(set_g59, 0, 6, 1, 1);

        group58 = new QWidget(groupBox_2);
        group58->setObjectName(QString::fromUtf8("group58"));
        verticalLayout_10 = new QVBoxLayout(group58);
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(1, 2, 2, 2);
        g58_x = new QDoubleSpinBox(group58);
        g58_x->setObjectName(QString::fromUtf8("g58_x"));
        g58_x->setWrapping(false);
        g58_x->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g58_x->setAccelerated(false);
        g58_x->setDecimals(3);
        g58_x->setMinimum(-10000);
        g58_x->setMaximum(10000);

        verticalLayout_10->addWidget(g58_x);

        g58_y = new QDoubleSpinBox(group58);
        g58_y->setObjectName(QString::fromUtf8("g58_y"));
        g58_y->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g58_y->setDecimals(3);
        g58_y->setMinimum(-10000);
        g58_y->setMaximum(10000);

        verticalLayout_10->addWidget(g58_y);

        g58_z = new QDoubleSpinBox(group58);
        g58_z->setObjectName(QString::fromUtf8("g58_z"));
        g58_z->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g58_z->setDecimals(3);
        g58_z->setMinimum(-10000);
        g58_z->setMaximum(10000);

        verticalLayout_10->addWidget(g58_z);

        g58_a = new QDoubleSpinBox(group58);
        g58_a->setObjectName(QString::fromUtf8("g58_a"));
        g58_a->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g58_a->setDecimals(3);
        g58_a->setMinimum(-10000);
        g58_a->setMaximum(10000);

        verticalLayout_10->addWidget(g58_a);


        gridLayout->addWidget(group58, 1, 5, 1, 1);

        group55 = new QWidget(groupBox_2);
        group55->setObjectName(QString::fromUtf8("group55"));
        verticalLayout_7 = new QVBoxLayout(group55);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(1, 2, 2, 2);
        g55_x = new QDoubleSpinBox(group55);
        g55_x->setObjectName(QString::fromUtf8("g55_x"));
        g55_x->setWrapping(false);
        g55_x->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g55_x->setAccelerated(false);
        g55_x->setDecimals(3);
        g55_x->setMinimum(-10000);
        g55_x->setMaximum(10000);

        verticalLayout_7->addWidget(g55_x);

        g55_y = new QDoubleSpinBox(group55);
        g55_y->setObjectName(QString::fromUtf8("g55_y"));
        g55_y->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g55_y->setDecimals(3);
        g55_y->setMinimum(-10000);
        g55_y->setMaximum(10000);

        verticalLayout_7->addWidget(g55_y);

        g55_z = new QDoubleSpinBox(group55);
        g55_z->setObjectName(QString::fromUtf8("g55_z"));
        g55_z->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g55_z->setDecimals(3);
        g55_z->setMinimum(-10000);
        g55_z->setMaximum(10000);

        verticalLayout_7->addWidget(g55_z);

        g55_a = new QDoubleSpinBox(group55);
        g55_a->setObjectName(QString::fromUtf8("g55_a"));
        g55_a->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g55_a->setDecimals(3);
        g55_a->setMinimum(-10000);
        g55_a->setMaximum(10000);

        verticalLayout_7->addWidget(g55_a);


        gridLayout->addWidget(group55, 1, 2, 1, 1);

        group54 = new QWidget(groupBox_2);
        group54->setObjectName(QString::fromUtf8("group54"));
        group54->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_6 = new QVBoxLayout(group54);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(1, 2, 2, 2);
        g54_x = new QDoubleSpinBox(group54);
        g54_x->setObjectName(QString::fromUtf8("g54_x"));
        g54_x->setWrapping(false);
        g54_x->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g54_x->setAccelerated(false);
        g54_x->setDecimals(3);
        g54_x->setMinimum(-10000);
        g54_x->setMaximum(10000);
        g54_x->setSingleStep(0);

        verticalLayout_6->addWidget(g54_x);

        g54_y = new QDoubleSpinBox(group54);
        g54_y->setObjectName(QString::fromUtf8("g54_y"));
        g54_y->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g54_y->setDecimals(3);
        g54_y->setMinimum(-10000);
        g54_y->setMaximum(10000);

        verticalLayout_6->addWidget(g54_y);

        g54_z = new QDoubleSpinBox(group54);
        g54_z->setObjectName(QString::fromUtf8("g54_z"));
        g54_z->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g54_z->setDecimals(3);
        g54_z->setMinimum(-10000);
        g54_z->setMaximum(10000);

        verticalLayout_6->addWidget(g54_z);

        g54_a = new QDoubleSpinBox(group54);
        g54_a->setObjectName(QString::fromUtf8("g54_a"));
        g54_a->setEnabled(true);
        g54_a->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g54_a->setDecimals(3);
        g54_a->setMinimum(-10000);
        g54_a->setMaximum(10000);
        g54_a->setSingleStep(0);

        verticalLayout_6->addWidget(g54_a);


        gridLayout->addWidget(group54, 1, 1, 1, 1);

        group57 = new QWidget(groupBox_2);
        group57->setObjectName(QString::fromUtf8("group57"));
        verticalLayout_9 = new QVBoxLayout(group57);
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(1, 2, 2, 2);
        g57_x = new QDoubleSpinBox(group57);
        g57_x->setObjectName(QString::fromUtf8("g57_x"));
        g57_x->setWrapping(false);
        g57_x->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g57_x->setAccelerated(false);
        g57_x->setDecimals(3);
        g57_x->setMinimum(-10000);
        g57_x->setMaximum(10000);

        verticalLayout_9->addWidget(g57_x);

        g57_y = new QDoubleSpinBox(group57);
        g57_y->setObjectName(QString::fromUtf8("g57_y"));
        g57_y->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g57_y->setDecimals(3);
        g57_y->setMinimum(-10000);
        g57_y->setMaximum(10000);

        verticalLayout_9->addWidget(g57_y);

        g57_z = new QDoubleSpinBox(group57);
        g57_z->setObjectName(QString::fromUtf8("g57_z"));
        g57_z->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g57_z->setDecimals(3);
        g57_z->setMinimum(-10000);
        g57_z->setMaximum(10000);

        verticalLayout_9->addWidget(g57_z);

        g57_a = new QDoubleSpinBox(group57);
        g57_a->setObjectName(QString::fromUtf8("g57_a"));
        g57_a->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g57_a->setDecimals(3);
        g57_a->setMinimum(-10000);
        g57_a->setMaximum(10000);

        verticalLayout_9->addWidget(g57_a);


        gridLayout->addWidget(group57, 1, 4, 1, 1);

        group59 = new QWidget(groupBox_2);
        group59->setObjectName(QString::fromUtf8("group59"));
        verticalLayout_11 = new QVBoxLayout(group59);
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(1, 2, 2, 2);
        g59_x = new QDoubleSpinBox(group59);
        g59_x->setObjectName(QString::fromUtf8("g59_x"));
        g59_x->setWrapping(false);
        g59_x->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g59_x->setAccelerated(false);
        g59_x->setDecimals(3);
        g59_x->setMinimum(-10000);
        g59_x->setMaximum(10000);

        verticalLayout_11->addWidget(g59_x);

        g59_y = new QDoubleSpinBox(group59);
        g59_y->setObjectName(QString::fromUtf8("g59_y"));
        g59_y->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g59_y->setDecimals(3);
        g59_y->setMinimum(-10000);
        g59_y->setMaximum(10000);

        verticalLayout_11->addWidget(g59_y);

        g59_z = new QDoubleSpinBox(group59);
        g59_z->setObjectName(QString::fromUtf8("g59_z"));
        g59_z->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g59_z->setDecimals(3);
        g59_z->setMinimum(-10000);
        g59_z->setMaximum(10000);

        verticalLayout_11->addWidget(g59_z);

        g59_a = new QDoubleSpinBox(group59);
        g59_a->setObjectName(QString::fromUtf8("g59_a"));
        g59_a->setButtonSymbols(QAbstractSpinBox::NoButtons);
        g59_a->setDecimals(3);
        g59_a->setMinimum(-10000);
        g59_a->setMaximum(10000);

        verticalLayout_11->addWidget(g59_a);


        gridLayout->addWidget(group59, 1, 6, 1, 1);

        set_g54 = new QRadioButton(groupBox_2);
        set_g54->setObjectName(QString::fromUtf8("set_g54"));
        set_g54->setChecked(true);

        gridLayout->addWidget(set_g54, 0, 1, 1, 1);

        set_g55 = new QRadioButton(groupBox_2);
        set_g55->setObjectName(QString::fromUtf8("set_g55"));

        gridLayout->addWidget(set_g55, 0, 2, 1, 1);

        set_g56 = new QRadioButton(groupBox_2);
        set_g56->setObjectName(QString::fromUtf8("set_g56"));

        gridLayout->addWidget(set_g56, 0, 3, 1, 1);

        set_g57 = new QRadioButton(groupBox_2);
        set_g57->setObjectName(QString::fromUtf8("set_g57"));

        gridLayout->addWidget(set_g57, 0, 4, 1, 1);

        set_g58 = new QRadioButton(groupBox_2);
        set_g58->setObjectName(QString::fromUtf8("set_g58"));

        gridLayout->addWidget(set_g58, 0, 5, 1, 1);

        COOR_FRAME = new QWidget(multioffset);
        COOR_FRAME->setObjectName(QString::fromUtf8("COOR_FRAME"));
        COOR_FRAME->setGeometry(QRect(9, 368, 461, 157));
        groupBox_3 = new QGroupBox(COOR_FRAME);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(9, 9, 386, 139));
        groupBox_3->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(2, -1, -1, -1);
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMaximumSize(QSize(32, 16777215));
        label_12->setFont(font);
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_12, 0, 0, 1, 1);

        piece_coor_x54 = new QLabel(groupBox_3);
        piece_coor_x54->setObjectName(QString::fromUtf8("piece_coor_x54"));
        piece_coor_x54->setMaximumSize(QSize(16777215, 30));
        QFont font3;
        font3.setPointSize(12);
        piece_coor_x54->setFont(font3);
        piece_coor_x54->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_x54->setFrameShape(QFrame::Box);
        piece_coor_x54->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        piece_coor_x54->setWordWrap(false);
        piece_coor_x54->setMargin(0);
        piece_coor_x54->setIndent(-1);
        piece_coor_x54->setOpenExternalLinks(false);

        gridLayout_3->addWidget(piece_coor_x54, 0, 1, 1, 1);

        piece_coor_x55 = new QLabel(groupBox_3);
        piece_coor_x55->setObjectName(QString::fromUtf8("piece_coor_x55"));
        piece_coor_x55->setMaximumSize(QSize(16777215, 30));
        piece_coor_x55->setFont(font3);
        piece_coor_x55->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_x55->setFrameShape(QFrame::Box);
        piece_coor_x55->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        piece_coor_x55->setWordWrap(false);
        piece_coor_x55->setMargin(0);
        piece_coor_x55->setIndent(-1);
        piece_coor_x55->setOpenExternalLinks(false);

        gridLayout_3->addWidget(piece_coor_x55, 0, 2, 1, 1);

        piece_coor_x56 = new QLabel(groupBox_3);
        piece_coor_x56->setObjectName(QString::fromUtf8("piece_coor_x56"));
        piece_coor_x56->setMaximumSize(QSize(16777215, 30));
        piece_coor_x56->setFont(font3);
        piece_coor_x56->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_x56->setFrameShape(QFrame::Box);
        piece_coor_x56->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        piece_coor_x56->setWordWrap(false);
        piece_coor_x56->setMargin(0);
        piece_coor_x56->setIndent(-1);
        piece_coor_x56->setOpenExternalLinks(false);

        gridLayout_3->addWidget(piece_coor_x56, 0, 3, 1, 1);

        piece_coor_x57 = new QLabel(groupBox_3);
        piece_coor_x57->setObjectName(QString::fromUtf8("piece_coor_x57"));
        piece_coor_x57->setMaximumSize(QSize(16777215, 30));
        piece_coor_x57->setFont(font3);
        piece_coor_x57->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_x57->setFrameShape(QFrame::Box);
        piece_coor_x57->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        piece_coor_x57->setWordWrap(false);
        piece_coor_x57->setMargin(0);
        piece_coor_x57->setIndent(-1);
        piece_coor_x57->setOpenExternalLinks(false);

        gridLayout_3->addWidget(piece_coor_x57, 0, 4, 1, 1);

        piece_coor_x58 = new QLabel(groupBox_3);
        piece_coor_x58->setObjectName(QString::fromUtf8("piece_coor_x58"));
        piece_coor_x58->setMaximumSize(QSize(16777215, 30));
        piece_coor_x58->setFont(font3);
        piece_coor_x58->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_x58->setFrameShape(QFrame::Box);
        piece_coor_x58->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        piece_coor_x58->setWordWrap(false);
        piece_coor_x58->setMargin(0);
        piece_coor_x58->setIndent(-1);
        piece_coor_x58->setOpenExternalLinks(false);

        gridLayout_3->addWidget(piece_coor_x58, 0, 5, 1, 1);

        piece_coor_x59 = new QLabel(groupBox_3);
        piece_coor_x59->setObjectName(QString::fromUtf8("piece_coor_x59"));
        piece_coor_x59->setMaximumSize(QSize(16777215, 30));
        piece_coor_x59->setFont(font3);
        piece_coor_x59->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_x59->setFrameShape(QFrame::Box);
        piece_coor_x59->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        piece_coor_x59->setWordWrap(false);
        piece_coor_x59->setMargin(0);
        piece_coor_x59->setIndent(-1);
        piece_coor_x59->setOpenExternalLinks(false);

        gridLayout_3->addWidget(piece_coor_x59, 0, 6, 1, 1);

        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMaximumSize(QSize(40, 16777215));
        label_11->setFont(font);
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_11, 1, 0, 1, 1);

        piece_coor_y54 = new QLabel(groupBox_3);
        piece_coor_y54->setObjectName(QString::fromUtf8("piece_coor_y54"));
        piece_coor_y54->setMaximumSize(QSize(16777215, 30));
        piece_coor_y54->setFont(font3);
        piece_coor_y54->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_y54->setFrameShape(QFrame::Box);
        piece_coor_y54->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_y54, 1, 1, 1, 1);

        piece_coor_y55 = new QLabel(groupBox_3);
        piece_coor_y55->setObjectName(QString::fromUtf8("piece_coor_y55"));
        piece_coor_y55->setMaximumSize(QSize(16777215, 30));
        piece_coor_y55->setFont(font3);
        piece_coor_y55->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_y55->setFrameShape(QFrame::Box);
        piece_coor_y55->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_y55, 1, 2, 1, 1);

        piece_coor_y56 = new QLabel(groupBox_3);
        piece_coor_y56->setObjectName(QString::fromUtf8("piece_coor_y56"));
        piece_coor_y56->setMaximumSize(QSize(16777215, 30));
        piece_coor_y56->setFont(font3);
        piece_coor_y56->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_y56->setFrameShape(QFrame::Box);
        piece_coor_y56->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_y56, 1, 3, 1, 1);

        piece_coor_y57 = new QLabel(groupBox_3);
        piece_coor_y57->setObjectName(QString::fromUtf8("piece_coor_y57"));
        piece_coor_y57->setMaximumSize(QSize(16777215, 30));
        piece_coor_y57->setFont(font3);
        piece_coor_y57->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_y57->setFrameShape(QFrame::Box);
        piece_coor_y57->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_y57, 1, 4, 1, 1);

        piece_coor_y58 = new QLabel(groupBox_3);
        piece_coor_y58->setObjectName(QString::fromUtf8("piece_coor_y58"));
        piece_coor_y58->setMaximumSize(QSize(16777215, 30));
        piece_coor_y58->setFont(font3);
        piece_coor_y58->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_y58->setFrameShape(QFrame::Box);
        piece_coor_y58->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_y58, 1, 5, 1, 1);

        piece_coor_y59 = new QLabel(groupBox_3);
        piece_coor_y59->setObjectName(QString::fromUtf8("piece_coor_y59"));
        piece_coor_y59->setMaximumSize(QSize(16777215, 30));
        piece_coor_y59->setFont(font3);
        piece_coor_y59->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_y59->setFrameShape(QFrame::Box);
        piece_coor_y59->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_y59, 1, 6, 1, 1);

        label_c_z1 = new QLabel(groupBox_3);
        label_c_z1->setObjectName(QString::fromUtf8("label_c_z1"));
        label_c_z1->setMaximumSize(QSize(40, 16777215));
        label_c_z1->setFont(font);
        label_c_z1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_c_z1, 2, 0, 1, 1);

        piece_coor_z154 = new QLabel(groupBox_3);
        piece_coor_z154->setObjectName(QString::fromUtf8("piece_coor_z154"));
        piece_coor_z154->setMaximumSize(QSize(16777215, 30));
        piece_coor_z154->setFont(font3);
        piece_coor_z154->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z154->setFrameShape(QFrame::Box);
        piece_coor_z154->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z154, 2, 1, 1, 1);

        piece_coor_z155 = new QLabel(groupBox_3);
        piece_coor_z155->setObjectName(QString::fromUtf8("piece_coor_z155"));
        piece_coor_z155->setMaximumSize(QSize(16777215, 30));
        piece_coor_z155->setFont(font3);
        piece_coor_z155->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z155->setFrameShape(QFrame::Box);
        piece_coor_z155->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z155, 2, 2, 1, 1);

        piece_coor_z156 = new QLabel(groupBox_3);
        piece_coor_z156->setObjectName(QString::fromUtf8("piece_coor_z156"));
        piece_coor_z156->setMaximumSize(QSize(16777215, 30));
        piece_coor_z156->setFont(font3);
        piece_coor_z156->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z156->setFrameShape(QFrame::Box);
        piece_coor_z156->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z156, 2, 3, 1, 1);

        piece_coor_z157 = new QLabel(groupBox_3);
        piece_coor_z157->setObjectName(QString::fromUtf8("piece_coor_z157"));
        piece_coor_z157->setMaximumSize(QSize(16777215, 30));
        piece_coor_z157->setFont(font3);
        piece_coor_z157->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z157->setFrameShape(QFrame::Box);
        piece_coor_z157->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z157, 2, 4, 1, 1);

        piece_coor_z158 = new QLabel(groupBox_3);
        piece_coor_z158->setObjectName(QString::fromUtf8("piece_coor_z158"));
        piece_coor_z158->setMaximumSize(QSize(16777215, 30));
        piece_coor_z158->setFont(font3);
        piece_coor_z158->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z158->setFrameShape(QFrame::Box);
        piece_coor_z158->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z158, 2, 5, 1, 1);

        piece_coor_z159 = new QLabel(groupBox_3);
        piece_coor_z159->setObjectName(QString::fromUtf8("piece_coor_z159"));
        piece_coor_z159->setMaximumSize(QSize(16777215, 30));
        piece_coor_z159->setFont(font3);
        piece_coor_z159->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z159->setFrameShape(QFrame::Box);
        piece_coor_z159->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z159, 2, 6, 1, 1);

        label_c_z2 = new QLabel(groupBox_3);
        label_c_z2->setObjectName(QString::fromUtf8("label_c_z2"));
        label_c_z2->setMaximumSize(QSize(40, 16777215));
        label_c_z2->setFont(font);
        label_c_z2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_c_z2, 3, 0, 1, 1);

        piece_coor_z254 = new QLabel(groupBox_3);
        piece_coor_z254->setObjectName(QString::fromUtf8("piece_coor_z254"));
        piece_coor_z254->setMaximumSize(QSize(16777215, 30));
        piece_coor_z254->setFont(font3);
        piece_coor_z254->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z254->setFrameShape(QFrame::Box);
        piece_coor_z254->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z254, 3, 1, 1, 1);

        piece_coor_z255 = new QLabel(groupBox_3);
        piece_coor_z255->setObjectName(QString::fromUtf8("piece_coor_z255"));
        piece_coor_z255->setMaximumSize(QSize(16777215, 30));
        piece_coor_z255->setFont(font3);
        piece_coor_z255->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z255->setFrameShape(QFrame::Box);
        piece_coor_z255->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z255, 3, 2, 1, 1);

        piece_coor_z256 = new QLabel(groupBox_3);
        piece_coor_z256->setObjectName(QString::fromUtf8("piece_coor_z256"));
        piece_coor_z256->setMaximumSize(QSize(16777215, 30));
        piece_coor_z256->setFont(font3);
        piece_coor_z256->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z256->setFrameShape(QFrame::Box);
        piece_coor_z256->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z256, 3, 3, 1, 1);

        piece_coor_z257 = new QLabel(groupBox_3);
        piece_coor_z257->setObjectName(QString::fromUtf8("piece_coor_z257"));
        piece_coor_z257->setMaximumSize(QSize(16777215, 30));
        piece_coor_z257->setFont(font3);
        piece_coor_z257->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z257->setFrameShape(QFrame::Box);
        piece_coor_z257->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z257, 3, 4, 1, 1);

        piece_coor_z258 = new QLabel(groupBox_3);
        piece_coor_z258->setObjectName(QString::fromUtf8("piece_coor_z258"));
        piece_coor_z258->setMaximumSize(QSize(16777215, 30));
        piece_coor_z258->setFont(font3);
        piece_coor_z258->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z258->setFrameShape(QFrame::Box);
        piece_coor_z258->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z258, 3, 5, 1, 1);

        piece_coor_z259 = new QLabel(groupBox_3);
        piece_coor_z259->setObjectName(QString::fromUtf8("piece_coor_z259"));
        piece_coor_z259->setMaximumSize(QSize(16777215, 30));
        piece_coor_z259->setFont(font3);
        piece_coor_z259->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(95, 95, 95);"));
        piece_coor_z259->setFrameShape(QFrame::Box);
        piece_coor_z259->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(piece_coor_z259, 3, 6, 1, 1);


        retranslateUi(multioffset);

        QMetaObject::connectSlotsByName(multioffset);
    } // setupUi

    void retranslateUi(QDialog *multioffset)
    {
        multioffset->setWindowTitle(QApplication::translate("multioffset", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("multioffset", "PublicOffset", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("multioffset", "X:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("multioffset", "Y:", 0, QApplication::UnicodeUTF8));
        label_p_z1->setText(QApplication::translate("multioffset", "Z1:", 0, QApplication::UnicodeUTF8));
        label_p_z2->setText(QApplication::translate("multioffset", "Z2:", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("multioffset", "Raise/Fall", 0, QApplication::UnicodeUTF8));
        btn_raise_1->setText(QApplication::translate("multioffset", "+1mm", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("multioffset", "DECENT:", 0, QApplication::UnicodeUTF8));
        btn_dec_001->setText(QApplication::translate("multioffset", "-0.01mm", 0, QApplication::UnicodeUTF8));
        btn_dec_01->setText(QApplication::translate("multioffset", "-0.1mm", 0, QApplication::UnicodeUTF8));
        btn_dec_1->setText(QApplication::translate("multioffset", "-1mm", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("multioffset", "RAISE:", 0, QApplication::UnicodeUTF8));
        btn_raise_001->setText(QApplication::translate("multioffset", "+0.01mm", 0, QApplication::UnicodeUTF8));
        btn_raise_01->setText(QApplication::translate("multioffset", "+0.1mm", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("multioffset", "Setting", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("multioffset", "      Read the current machine coordinate and set  to the current offset.", 0, QApplication::UnicodeUTF8));
        btn_set_x->setText(QApplication::translate("multioffset", "setx", 0, QApplication::UnicodeUTF8));
        btn_set_y->setText(QApplication::translate("multioffset", "sety", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("multioffset", "CurrentSys", 0, QApplication::UnicodeUTF8));
        system_now->setText(QApplication::translate("multioffset", "G54", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("multioffset", "chotomy", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("multioffset", "Record:  Record the current machine coordinate.\n"
"Chotomy:  Take an average of the current machine coordinate value with the record value and set it.", 0, QApplication::UnicodeUTF8));
        btn_record_x->setText(QApplication::translate("multioffset", "record X", 0, QApplication::UnicodeUTF8));
        show_record_x->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        btn_dichotomy_x->setText(QApplication::translate("multioffset", "dichotomy_x", 0, QApplication::UnicodeUTF8));
        btn_record_y->setText(QApplication::translate("multioffset", "record Y", 0, QApplication::UnicodeUTF8));
        show_record_y->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        btn_dichotomy_y->setText(QApplication::translate("multioffset", "dichotomy_y", 0, QApplication::UnicodeUTF8));
        btn_ensure->setText(QApplication::translate("multioffset", "Ensure", 0, QApplication::UnicodeUTF8));
        btn_cancel->setText(QApplication::translate("multioffset", "Quit", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("multioffset", "Piece Offset", 0, QApplication::UnicodeUTF8));
        label_3->setText(QString());
        label_6->setText(QApplication::translate("multioffset", "X:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("multioffset", "Y:", 0, QApplication::UnicodeUTF8));
        label_o_z1->setText(QApplication::translate("multioffset", "Z1:", 0, QApplication::UnicodeUTF8));
        label_o_z2->setText(QApplication::translate("multioffset", "Z2:", 0, QApplication::UnicodeUTF8));
        set_g59->setText(QApplication::translate("multioffset", "G59", 0, QApplication::UnicodeUTF8));
        set_g54->setText(QApplication::translate("multioffset", "G54", 0, QApplication::UnicodeUTF8));
        set_g55->setText(QApplication::translate("multioffset", "G55", 0, QApplication::UnicodeUTF8));
        set_g56->setText(QApplication::translate("multioffset", "G56", 0, QApplication::UnicodeUTF8));
        set_g57->setText(QApplication::translate("multioffset", "G57", 0, QApplication::UnicodeUTF8));
        set_g58->setText(QApplication::translate("multioffset", "G58", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("multioffset", "Piece Coor", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("multioffset", "X:", 0, QApplication::UnicodeUTF8));
        piece_coor_x54->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_x55->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_x56->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_x57->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_x58->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_x59->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("multioffset", "Y:", 0, QApplication::UnicodeUTF8));
        piece_coor_y54->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_y55->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_y56->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_y57->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_y58->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_y59->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        label_c_z1->setText(QApplication::translate("multioffset", "Z1:", 0, QApplication::UnicodeUTF8));
        piece_coor_z154->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_z155->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_z156->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_z157->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_z158->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_z159->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        label_c_z2->setText(QApplication::translate("multioffset", "Z2:", 0, QApplication::UnicodeUTF8));
        piece_coor_z254->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_z255->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_z256->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_z257->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_z258->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
        piece_coor_z259->setText(QApplication::translate("multioffset", "0.000", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class multioffset: public Ui_multioffset {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTIOFFSET_H
