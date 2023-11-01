/********************************************************************************
** Form generated from reading UI file 'bitmap_editor.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BITMAP_EDITOR_H
#define UI_BITMAP_EDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BitMap_Editor
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QGridLayout *gridLayout;
    QPushButton *Btn_Eraser;
    QPushButton *Btn_Redo;
    QPushButton *Btn_Undo;
    QPushButton *Btn_FreeDraw;
    QSlider *Sld_EraserSize;
    QPushButton *Btn_Reset;
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
            BitMap_Editor->setObjectName(QStringLiteral("BitMap_Editor"));
        BitMap_Editor->resize(715, 837);
        verticalLayout = new QVBoxLayout(BitMap_Editor);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(BitMap_Editor);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        Btn_Eraser = new QPushButton(frame);
        Btn_Eraser->setObjectName(QStringLiteral("Btn_Eraser"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/bitmap_images/draw-eraser.svg"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_Eraser->setIcon(icon);
        Btn_Eraser->setCheckable(true);
        Btn_Eraser->setAutoExclusive(true);

        gridLayout->addWidget(Btn_Eraser, 3, 2, 1, 1);

        Btn_Redo = new QPushButton(frame);
        Btn_Redo->setObjectName(QStringLiteral("Btn_Redo"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/bitmap_images/redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_Redo->setIcon(icon1);

        gridLayout->addWidget(Btn_Redo, 3, 5, 1, 1);

        Btn_Undo = new QPushButton(frame);
        Btn_Undo->setObjectName(QStringLiteral("Btn_Undo"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/bitmap_images/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_Undo->setIcon(icon2);

        gridLayout->addWidget(Btn_Undo, 3, 4, 1, 1);

        Btn_FreeDraw = new QPushButton(frame);
        Btn_FreeDraw->setObjectName(QStringLiteral("Btn_FreeDraw"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/bitmap_images/format-stroke-color.svg"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_FreeDraw->setIcon(icon3);
        Btn_FreeDraw->setCheckable(true);
        Btn_FreeDraw->setAutoExclusive(true);

        gridLayout->addWidget(Btn_FreeDraw, 3, 3, 1, 1);

        Sld_EraserSize = new QSlider(frame);
        Sld_EraserSize->setObjectName(QStringLiteral("Sld_EraserSize"));
        Sld_EraserSize->setMaximumSize(QSize(150, 16777215));
        Sld_EraserSize->setMinimum(5);
        Sld_EraserSize->setValue(30);
        Sld_EraserSize->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(Sld_EraserSize, 4, 2, 1, 1);

        Btn_Reset = new QPushButton(frame);
        Btn_Reset->setObjectName(QStringLiteral("Btn_Reset"));

        gridLayout->addWidget(Btn_Reset, 4, 5, 1, 1);


        verticalLayout->addWidget(frame);

        Frame_Edit = new QFrame(BitMap_Editor);
        Frame_Edit->setObjectName(QStringLiteral("Frame_Edit"));
        Frame_Edit->setMinimumSize(QSize(640, 640));
        Frame_Edit->setFrameShape(QFrame::StyledPanel);
        Frame_Edit->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(Frame_Edit);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        unused = new QLabel(Frame_Edit);
        unused->setObjectName(QStringLiteral("unused"));

        verticalLayout_2->addWidget(unused);


        verticalLayout->addWidget(Frame_Edit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        Btn_Sure = new QPushButton(BitMap_Editor);
        Btn_Sure->setObjectName(QStringLiteral("Btn_Sure"));

        horizontalLayout_2->addWidget(Btn_Sure);

        Btn_Cancel = new QPushButton(BitMap_Editor);
        Btn_Cancel->setObjectName(QStringLiteral("Btn_Cancel"));

        horizontalLayout_2->addWidget(Btn_Cancel);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(BitMap_Editor);

        QMetaObject::connectSlotsByName(BitMap_Editor);
    } // setupUi

    void retranslateUi(QDialog *BitMap_Editor)
    {
        BitMap_Editor->setWindowTitle(QApplication::translate("BitMap_Editor", "Dialog", Q_NULLPTR));
        Btn_Eraser->setText(QApplication::translate("BitMap_Editor", "\346\251\241\347\232\256\346\223\246 F2", Q_NULLPTR));
        Btn_Eraser->setShortcut(QApplication::translate("BitMap_Editor", "F2", Q_NULLPTR));
        Btn_Redo->setText(QApplication::translate("BitMap_Editor", "\351\207\215\345\201\232", Q_NULLPTR));
        Btn_Undo->setText(QApplication::translate("BitMap_Editor", "\346\222\244\351\224\200", Q_NULLPTR));
        Btn_FreeDraw->setText(QApplication::translate("BitMap_Editor", "\347\273\230\345\233\276 F1", Q_NULLPTR));
        Btn_FreeDraw->setShortcut(QApplication::translate("BitMap_Editor", "F1", Q_NULLPTR));
        Btn_Reset->setText(QApplication::translate("BitMap_Editor", "<--->", Q_NULLPTR));
        unused->setText(QApplication::translate("BitMap_Editor", "TextLabel", Q_NULLPTR));
        Btn_Sure->setText(QApplication::translate("BitMap_Editor", "\347\241\256\345\256\232", Q_NULLPTR));
        Btn_Cancel->setText(QApplication::translate("BitMap_Editor", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BitMap_Editor: public Ui_BitMap_Editor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BITMAP_EDITOR_H
