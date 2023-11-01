/********************************************************************************
** Form generated from reading UI file 'array.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARRAY_H
#define UI_ARRAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsView>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Array
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *functrionframe;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpinBox *sprownum;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QSpinBox *spcolnum;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QDoubleSpinBox *spdisx;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QDoubleSpinBox *spdisy;
    QPushButton *btn_buildarray;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_8;
    QHBoxLayout *horizontalLayout_8;
    QDoubleSpinBox *totalcompx;
    QPushButton *btn_totalcompx;
    QFrame *frame_9;
    QHBoxLayout *horizontalLayout_9;
    QDoubleSpinBox *totalcompy;
    QPushButton *btn_totalcompy;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_6;
    QSpinBox *sprowcomp;
    QPushButton *btn_rowcomp;
    QFrame *frame_7;
    QHBoxLayout *horizontalLayout_7;
    QSpinBox *spcolumncomp;
    QPushButton *btn_columncomp;
    QGraphicsView *graphicsView;

    void setupUi(QWidget *Array)
    {
        if (Array->objectName().isEmpty())
            Array->setObjectName(QString::fromUtf8("Array"));
        Array->resize(708, 425);
        horizontalLayout = new QHBoxLayout(Array);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        functrionframe = new QFrame(Array);
        functrionframe->setObjectName(QString::fromUtf8("functrionframe"));
        functrionframe->setFrameShape(QFrame::StyledPanel);
        functrionframe->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(functrionframe);
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setContentsMargins(2, 2, 2, 2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame_2 = new QFrame(functrionframe);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        sprownum = new QSpinBox(frame_2);
        sprownum->setObjectName(QString::fromUtf8("sprownum"));
        sprownum->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sprownum->setMinimum(1);
        sprownum->setMaximum(10);

        horizontalLayout_2->addWidget(sprownum);


        verticalLayout_3->addWidget(frame_2);

        frame_3 = new QFrame(functrionframe);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_3);
        horizontalLayout_3->setSpacing(2);
        horizontalLayout_3->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(frame_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        spcolnum = new QSpinBox(frame_3);
        spcolnum->setObjectName(QString::fromUtf8("spcolnum"));
        spcolnum->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spcolnum->setMinimum(1);
        spcolnum->setMaximum(10);

        horizontalLayout_3->addWidget(spcolnum);


        verticalLayout_3->addWidget(frame_3);

        frame_4 = new QFrame(functrionframe);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_4);
        horizontalLayout_4->setSpacing(2);
        horizontalLayout_4->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(frame_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        spdisx = new QDoubleSpinBox(frame_4);
        spdisx->setObjectName(QString::fromUtf8("spdisx"));
        spdisx->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spdisx->setDecimals(3);
        spdisx->setMinimum(-1000);
        spdisx->setMaximum(1000);

        horizontalLayout_4->addWidget(spdisx);


        verticalLayout_3->addWidget(frame_4);

        frame_5 = new QFrame(functrionframe);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_5);
        horizontalLayout_5->setSpacing(2);
        horizontalLayout_5->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(frame_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_5->addWidget(label_4);

        spdisy = new QDoubleSpinBox(frame_5);
        spdisy->setObjectName(QString::fromUtf8("spdisy"));
        spdisy->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spdisy->setDecimals(3);
        spdisy->setMinimum(-1000);
        spdisy->setMaximum(1000);

        horizontalLayout_5->addWidget(spdisy);


        verticalLayout_3->addWidget(frame_5);

        btn_buildarray = new QPushButton(functrionframe);
        btn_buildarray->setObjectName(QString::fromUtf8("btn_buildarray"));

        verticalLayout_3->addWidget(btn_buildarray);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        groupBox_2 = new QGroupBox(functrionframe);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(1, 15, 1, 1);
        frame_8 = new QFrame(groupBox_2);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        horizontalLayout_8 = new QHBoxLayout(frame_8);
        horizontalLayout_8->setSpacing(2);
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        totalcompx = new QDoubleSpinBox(frame_8);
        totalcompx->setObjectName(QString::fromUtf8("totalcompx"));
        totalcompx->setButtonSymbols(QAbstractSpinBox::NoButtons);
        totalcompx->setDecimals(3);
        totalcompx->setMinimum(-100);
        totalcompx->setMaximum(100);

        horizontalLayout_8->addWidget(totalcompx);

        btn_totalcompx = new QPushButton(frame_8);
        btn_totalcompx->setObjectName(QString::fromUtf8("btn_totalcompx"));
        btn_totalcompx->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_8->addWidget(btn_totalcompx);


        verticalLayout_2->addWidget(frame_8);

        frame_9 = new QFrame(groupBox_2);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        horizontalLayout_9 = new QHBoxLayout(frame_9);
        horizontalLayout_9->setSpacing(2);
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        totalcompy = new QDoubleSpinBox(frame_9);
        totalcompy->setObjectName(QString::fromUtf8("totalcompy"));
        totalcompy->setButtonSymbols(QAbstractSpinBox::NoButtons);
        totalcompy->setDecimals(3);
        totalcompy->setMinimum(-100);
        totalcompy->setMaximum(100);

        horizontalLayout_9->addWidget(totalcompy);

        btn_totalcompy = new QPushButton(frame_9);
        btn_totalcompy->setObjectName(QString::fromUtf8("btn_totalcompy"));
        btn_totalcompy->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_9->addWidget(btn_totalcompy);


        verticalLayout_2->addWidget(frame_9);


        verticalLayout_3->addWidget(groupBox_2);

        groupBox = new QGroupBox(functrionframe);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(1);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 15, 1, 1);
        frame_6 = new QFrame(groupBox);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(frame_6);
        horizontalLayout_6->setSpacing(2);
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        sprowcomp = new QSpinBox(frame_6);
        sprowcomp->setObjectName(QString::fromUtf8("sprowcomp"));
        sprowcomp->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sprowcomp->setMaximum(9);

        horizontalLayout_6->addWidget(sprowcomp);

        btn_rowcomp = new QPushButton(frame_6);
        btn_rowcomp->setObjectName(QString::fromUtf8("btn_rowcomp"));
        btn_rowcomp->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_6->addWidget(btn_rowcomp);


        verticalLayout->addWidget(frame_6);

        frame_7 = new QFrame(groupBox);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        horizontalLayout_7 = new QHBoxLayout(frame_7);
        horizontalLayout_7->setSpacing(2);
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        spcolumncomp = new QSpinBox(frame_7);
        spcolumncomp->setObjectName(QString::fromUtf8("spcolumncomp"));
        spcolumncomp->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spcolumncomp->setMaximum(9);

        horizontalLayout_7->addWidget(spcolumncomp);

        btn_columncomp = new QPushButton(frame_7);
        btn_columncomp->setObjectName(QString::fromUtf8("btn_columncomp"));
        btn_columncomp->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_7->addWidget(btn_columncomp);


        verticalLayout->addWidget(frame_7);


        verticalLayout_3->addWidget(groupBox);


        horizontalLayout->addWidget(functrionframe);

        graphicsView = new QGraphicsView(Array);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setInteractive(true);

        horizontalLayout->addWidget(graphicsView);


        retranslateUi(Array);

        QMetaObject::connectSlotsByName(Array);
    } // setupUi

    void retranslateUi(QWidget *Array)
    {
        Array->setWindowTitle(QApplication::translate("Array", "Array", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Array", "Row:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Array", "Col:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Array", "Disx:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Array", "Disy:", 0, QApplication::UnicodeUTF8));
        btn_buildarray->setText(QApplication::translate("Array", "BuildArray", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Array", "totalcomp", 0, QApplication::UnicodeUTF8));
        btn_totalcompx->setText(QApplication::translate("Array", "Set", 0, QApplication::UnicodeUTF8));
        btn_totalcompy->setText(QApplication::translate("Array", "Set", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Array", "rowcomp", 0, QApplication::UnicodeUTF8));
        btn_rowcomp->setText(QApplication::translate("Array", "Set", 0, QApplication::UnicodeUTF8));
        btn_columncomp->setText(QApplication::translate("Array", "Set", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Array: public Ui_Array {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARRAY_H
