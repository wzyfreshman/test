/********************************************************************************
** Form generated from reading UI file 'bitmap_editor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BITMAP_EDITOR_H
#define UI_BITMAP_EDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BitMap_Editor
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QGridLayout *gridLayout;
    QPushButton *Btn_Undo;
    QSlider *Sld_EraserSize;
    QPushButton *Btn_FreeDraw;
    QPushButton *Btn_Generator;
    QPushButton *Btn_Redo;
    QPushButton *Btn_SetStartP;
    QSpacerItem *horizontalSpacer;
    QPushButton *Btn_Eraser;
    QPushButton *Btn_None;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *Btn_Rect;
    QPushButton *Btn_Circle;
    QLabel *label;
    QSpinBox *Sbx_Sample;
    QLabel *label_2;
    QFrame *Frame_Edit;
    QVBoxLayout *verticalLayout_2;
    QLabel *unused;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *Btn_Sure;
    QPushButton *Btn_Cancel;

    void setupUi(QDialog *BitMap_Editor)
    {
        if (BitMap_Editor->objectName().isEmpty())
            BitMap_Editor->setObjectName(QString::fromUtf8("BitMap_Editor"));
        BitMap_Editor->resize(658, 795);
        verticalLayout = new QVBoxLayout(BitMap_Editor);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(BitMap_Editor);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        Btn_Undo = new QPushButton(frame);
        Btn_Undo->setObjectName(QString::fromUtf8("Btn_Undo"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/bitmap_images/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_Undo->setIcon(icon);

        gridLayout->addWidget(Btn_Undo, 0, 5, 1, 1);

        Sld_EraserSize = new QSlider(frame);
        Sld_EraserSize->setObjectName(QString::fromUtf8("Sld_EraserSize"));
        Sld_EraserSize->setMaximumSize(QSize(150, 16777215));
        Sld_EraserSize->setMinimum(5);
        Sld_EraserSize->setValue(30);
        Sld_EraserSize->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(Sld_EraserSize, 3, 0, 1, 1);

        Btn_FreeDraw = new QPushButton(frame);
        Btn_FreeDraw->setObjectName(QString::fromUtf8("Btn_FreeDraw"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/bitmap_images/format-stroke-color.svg"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_FreeDraw->setIcon(icon1);
        Btn_FreeDraw->setCheckable(true);
        Btn_FreeDraw->setAutoExclusive(true);

        gridLayout->addWidget(Btn_FreeDraw, 0, 1, 1, 1);

        Btn_Generator = new QPushButton(frame);
        Btn_Generator->setObjectName(QString::fromUtf8("Btn_Generator"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/bitmap_images/draw-spiral.svg"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_Generator->setIcon(icon2);

        gridLayout->addWidget(Btn_Generator, 0, 3, 1, 1);

        Btn_Redo = new QPushButton(frame);
        Btn_Redo->setObjectName(QString::fromUtf8("Btn_Redo"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/bitmap_images/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_Redo->setIcon(icon3);

        gridLayout->addWidget(Btn_Redo, 2, 5, 1, 1);

        Btn_SetStartP = new QPushButton(frame);
        Btn_SetStartP->setObjectName(QString::fromUtf8("Btn_SetStartP"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/bitmap_images/color-picker.svg"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_SetStartP->setIcon(icon4);
        Btn_SetStartP->setCheckable(true);
        Btn_SetStartP->setAutoExclusive(true);

        gridLayout->addWidget(Btn_SetStartP, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 7, 1, 1);

        Btn_Eraser = new QPushButton(frame);
        Btn_Eraser->setObjectName(QString::fromUtf8("Btn_Eraser"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/bitmap_images/draw-eraser.svg"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_Eraser->setIcon(icon5);
        Btn_Eraser->setCheckable(true);
        Btn_Eraser->setAutoExclusive(true);

        gridLayout->addWidget(Btn_Eraser, 2, 1, 1, 1);

        Btn_None = new QPushButton(frame);
        Btn_None->setObjectName(QString::fromUtf8("Btn_None"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/bitmap_images/edit-select.svg"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_None->setIcon(icon6);
        Btn_None->setCheckable(true);
        Btn_None->setAutoExclusive(true);

        gridLayout->addWidget(Btn_None, 2, 2, 1, 1);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        Btn_Rect = new QPushButton(widget);
        Btn_Rect->setObjectName(QString::fromUtf8("Btn_Rect"));
        Btn_Rect->setCheckable(false);
        Btn_Rect->setAutoExclusive(false);

        horizontalLayout->addWidget(Btn_Rect);

        Btn_Circle = new QPushButton(widget);
        Btn_Circle->setObjectName(QString::fromUtf8("Btn_Circle"));
        Btn_Circle->setCheckable(false);
        Btn_Circle->setAutoExclusive(false);

        horizontalLayout->addWidget(Btn_Circle);


        gridLayout->addWidget(widget, 2, 0, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        Sbx_Sample = new QSpinBox(frame);
        Sbx_Sample->setObjectName(QString::fromUtf8("Sbx_Sample"));
        Sbx_Sample->setMinimum(2);
        Sbx_Sample->setMaximum(20);
        Sbx_Sample->setValue(4);

        gridLayout->addWidget(Sbx_Sample, 2, 4, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 2, 3, 1, 1);


        verticalLayout->addWidget(frame);

        Frame_Edit = new QFrame(BitMap_Editor);
        Frame_Edit->setObjectName(QString::fromUtf8("Frame_Edit"));
        Frame_Edit->setMinimumSize(QSize(640, 640));
        Frame_Edit->setFrameShape(QFrame::StyledPanel);
        Frame_Edit->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(Frame_Edit);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        unused = new QLabel(Frame_Edit);
        unused->setObjectName(QString::fromUtf8("unused"));

        verticalLayout_2->addWidget(unused);


        verticalLayout->addWidget(Frame_Edit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        Btn_Sure = new QPushButton(BitMap_Editor);
        Btn_Sure->setObjectName(QString::fromUtf8("Btn_Sure"));

        horizontalLayout_2->addWidget(Btn_Sure);

        Btn_Cancel = new QPushButton(BitMap_Editor);
        Btn_Cancel->setObjectName(QString::fromUtf8("Btn_Cancel"));

        horizontalLayout_2->addWidget(Btn_Cancel);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(BitMap_Editor);

        QMetaObject::connectSlotsByName(BitMap_Editor);
    } // setupUi

    void retranslateUi(QDialog *BitMap_Editor)
    {
        BitMap_Editor->setWindowTitle(QApplication::translate("BitMap_Editor", "Dialog", 0, QApplication::UnicodeUTF8));
        Btn_Undo->setText(QApplication::translate("BitMap_Editor", "\346\222\244\351\224\200", 0, QApplication::UnicodeUTF8));
        Btn_FreeDraw->setText(QApplication::translate("BitMap_Editor", "\347\273\230\345\233\276 F1", 0, QApplication::UnicodeUTF8));
        Btn_FreeDraw->setShortcut(QApplication::translate("BitMap_Editor", "F1", 0, QApplication::UnicodeUTF8));
        Btn_Generator->setText(QApplication::translate("BitMap_Editor", "\347\224\237\346\210\220\350\267\257\345\276\204 F5", 0, QApplication::UnicodeUTF8));
        Btn_Generator->setShortcut(QApplication::translate("BitMap_Editor", "F5", 0, QApplication::UnicodeUTF8));
        Btn_Redo->setText(QApplication::translate("BitMap_Editor", "\351\207\215\345\201\232", 0, QApplication::UnicodeUTF8));
        Btn_SetStartP->setText(QApplication::translate("BitMap_Editor", "\350\256\276\347\275\256\350\265\267\347\202\271 F3", 0, QApplication::UnicodeUTF8));
        Btn_SetStartP->setShortcut(QApplication::translate("BitMap_Editor", "F3", 0, QApplication::UnicodeUTF8));
        Btn_Eraser->setText(QApplication::translate("BitMap_Editor", "\346\251\241\347\232\256\346\223\246 F2", 0, QApplication::UnicodeUTF8));
        Btn_Eraser->setShortcut(QApplication::translate("BitMap_Editor", "F2", 0, QApplication::UnicodeUTF8));
        Btn_None->setText(QApplication::translate("BitMap_Editor", "  \351\200\211\346\213\251 F4", 0, QApplication::UnicodeUTF8));
        Btn_None->setShortcut(QApplication::translate("BitMap_Editor", "F4", 0, QApplication::UnicodeUTF8));
        Btn_Rect->setText(QApplication::translate("BitMap_Editor", "\346\226\271\346\241\206", 0, QApplication::UnicodeUTF8));
        Btn_Circle->setText(QApplication::translate("BitMap_Editor", "\345\234\206\345\275\242", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("BitMap_Editor", "\346\251\241\347\232\256\346\223\246\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("BitMap_Editor", "\351\207\207\346\240\267\345\217\202\346\225\260", 0, QApplication::UnicodeUTF8));
        unused->setText(QApplication::translate("BitMap_Editor", "TextLabel", 0, QApplication::UnicodeUTF8));
        Btn_Sure->setText(QApplication::translate("BitMap_Editor", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        Btn_Cancel->setText(QApplication::translate("BitMap_Editor", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BitMap_Editor: public Ui_BitMap_Editor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BITMAP_EDITOR_H
