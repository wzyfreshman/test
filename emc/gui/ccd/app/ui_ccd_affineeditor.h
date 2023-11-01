/********************************************************************************
** Form generated from reading UI file 'ccd_affineeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_AFFINEEDITOR_H
#define UI_CCD_AFFINEEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCD_AffineEditor
{
public:
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QPushButton *Btn_Origin;
    QPushButton *Btn_Dest;
    QSpacerItem *verticalSpacer;
    QPlainTextEdit *Brs_Log;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *Btn_SaveMark;
    QFrame *line;
    QHBoxLayout *horizontalLayout;
    QPushButton *Btn_Sure;
    QPushButton *Btn_Cancel;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *page_4;
    QHBoxLayout *horizontalLayout_5;
    QGridLayout *gridLayout_9;
    QPushButton *Btn_IdOriginMark2;
    QPushButton *Btn_IdOriginMark3;
    QPushButton *Btn_GetOriginMark2;
    QPushButton *Btn_GetOriginMark1;
    QPushButton *Btn_IdOriginMark1;
    QPushButton *Btn_GetOriginMark3;
    QDoubleSpinBox *Affine_PositionY;
    QSpacerItem *horizontalSpacer;
    QWidget *page;
    QHBoxLayout *horizontalLayout_6;
    QGridLayout *gridLayout_8;
    QPushButton *Btn_GetDestMark3;
    QPushButton *Btn_IdDestMark3;
    QPushButton *Btn_GetDestMark2;
    QPushButton *Btn_IdDestMark1;
    QPushButton *Btn_GetDestMark1;
    QPushButton *Btn_CalAffineMat;
    QPushButton *Btn_IdDestMark2;
    QDoubleSpinBox *Mat00;
    QDoubleSpinBox *Mat01;
    QDoubleSpinBox *Mat02;
    QDoubleSpinBox *Mat10;
    QDoubleSpinBox *Mat11;
    QDoubleSpinBox *Mat12;
    QSpacerItem *horizontalSpacer_2;
    QFrame *MainWidget;

    void setupUi(QDialog *CCD_AffineEditor)
    {
        if (CCD_AffineEditor->objectName().isEmpty())
            CCD_AffineEditor->setObjectName(QStringLiteral("CCD_AffineEditor"));
        CCD_AffineEditor->resize(1014, 754);
        CCD_AffineEditor->setMinimumSize(QSize(921, 657));
        CCD_AffineEditor->setMaximumSize(QSize(1280, 1024));
        horizontalLayout_2 = new QHBoxLayout(CCD_AffineEditor);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        frame = new QFrame(CCD_AffineEditor);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 2, 4, -1);
        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        Btn_Origin = new QPushButton(widget);
        Btn_Origin->setObjectName(QStringLiteral("Btn_Origin"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Btn_Origin->sizePolicy().hasHeightForWidth());
        Btn_Origin->setSizePolicy(sizePolicy);
        Btn_Origin->setMaximumSize(QSize(16777215, 50));
        Btn_Origin->setStyleSheet(QLatin1String("QPushButton:checked{\n"
"	background-color: rgb(255, 139, 7);\n"
"}"));
        Btn_Origin->setCheckable(true);
        Btn_Origin->setChecked(true);
        Btn_Origin->setAutoExclusive(true);
        Btn_Origin->setAutoDefault(false);

        verticalLayout_4->addWidget(Btn_Origin);

        Btn_Dest = new QPushButton(widget);
        Btn_Dest->setObjectName(QStringLiteral("Btn_Dest"));
        sizePolicy.setHeightForWidth(Btn_Dest->sizePolicy().hasHeightForWidth());
        Btn_Dest->setSizePolicy(sizePolicy);
        Btn_Dest->setMaximumSize(QSize(16777215, 50));
        Btn_Dest->setStyleSheet(QLatin1String("QPushButton:checked{\n"
"	background-color: rgb(255, 139, 7);\n"
"}"));
        Btn_Dest->setCheckable(true);
        Btn_Dest->setAutoExclusive(true);
        Btn_Dest->setAutoDefault(true);
        Btn_Dest->setFlat(false);

        verticalLayout_4->addWidget(Btn_Dest);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);


        verticalLayout->addWidget(widget);

        Brs_Log = new QPlainTextEdit(frame);
        Brs_Log->setObjectName(QStringLiteral("Brs_Log"));
        Brs_Log->setReadOnly(true);

        verticalLayout->addWidget(Brs_Log);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));

        verticalLayout->addLayout(horizontalLayout_12);

        Btn_SaveMark = new QPushButton(frame);
        Btn_SaveMark->setObjectName(QStringLiteral("Btn_SaveMark"));

        verticalLayout->addWidget(Btn_SaveMark);

        line = new QFrame(frame);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Btn_Sure = new QPushButton(frame);
        Btn_Sure->setObjectName(QStringLiteral("Btn_Sure"));

        horizontalLayout->addWidget(Btn_Sure);

        Btn_Cancel = new QPushButton(frame);
        Btn_Cancel->setObjectName(QStringLiteral("Btn_Cancel"));

        horizontalLayout->addWidget(Btn_Cancel);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addWidget(frame);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        stackedWidget = new QStackedWidget(CCD_AffineEditor);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setStyleSheet(QStringLiteral(""));
        stackedWidget->setFrameShape(QFrame::Panel);
        stackedWidget->setFrameShadow(QFrame::Plain);
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        horizontalLayout_5 = new QHBoxLayout(page_4);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        Btn_IdOriginMark2 = new QPushButton(page_4);
        Btn_IdOriginMark2->setObjectName(QStringLiteral("Btn_IdOriginMark2"));

        gridLayout_9->addWidget(Btn_IdOriginMark2, 1, 1, 1, 1);

        Btn_IdOriginMark3 = new QPushButton(page_4);
        Btn_IdOriginMark3->setObjectName(QStringLiteral("Btn_IdOriginMark3"));

        gridLayout_9->addWidget(Btn_IdOriginMark3, 1, 2, 1, 1);

        Btn_GetOriginMark2 = new QPushButton(page_4);
        Btn_GetOriginMark2->setObjectName(QStringLiteral("Btn_GetOriginMark2"));

        gridLayout_9->addWidget(Btn_GetOriginMark2, 0, 1, 1, 1);

        Btn_GetOriginMark1 = new QPushButton(page_4);
        Btn_GetOriginMark1->setObjectName(QStringLiteral("Btn_GetOriginMark1"));

        gridLayout_9->addWidget(Btn_GetOriginMark1, 0, 0, 1, 1);

        Btn_IdOriginMark1 = new QPushButton(page_4);
        Btn_IdOriginMark1->setObjectName(QStringLiteral("Btn_IdOriginMark1"));

        gridLayout_9->addWidget(Btn_IdOriginMark1, 1, 0, 1, 1);

        Btn_GetOriginMark3 = new QPushButton(page_4);
        Btn_GetOriginMark3->setObjectName(QStringLiteral("Btn_GetOriginMark3"));

        gridLayout_9->addWidget(Btn_GetOriginMark3, 0, 2, 1, 1);

        Affine_PositionY = new QDoubleSpinBox(page_4);
        Affine_PositionY->setObjectName(QStringLiteral("Affine_PositionY"));
        Affine_PositionY->setDecimals(4);
        Affine_PositionY->setMinimum(-1000);
        Affine_PositionY->setMaximum(1000);

        gridLayout_9->addWidget(Affine_PositionY, 0, 3, 1, 1);


        horizontalLayout_5->addLayout(gridLayout_9);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        stackedWidget->addWidget(page_4);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        horizontalLayout_6 = new QHBoxLayout(page);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        Btn_GetDestMark3 = new QPushButton(page);
        Btn_GetDestMark3->setObjectName(QStringLiteral("Btn_GetDestMark3"));

        gridLayout_8->addWidget(Btn_GetDestMark3, 0, 2, 1, 1);

        Btn_IdDestMark3 = new QPushButton(page);
        Btn_IdDestMark3->setObjectName(QStringLiteral("Btn_IdDestMark3"));

        gridLayout_8->addWidget(Btn_IdDestMark3, 1, 2, 1, 1);

        Btn_GetDestMark2 = new QPushButton(page);
        Btn_GetDestMark2->setObjectName(QStringLiteral("Btn_GetDestMark2"));

        gridLayout_8->addWidget(Btn_GetDestMark2, 0, 1, 1, 1);

        Btn_IdDestMark1 = new QPushButton(page);
        Btn_IdDestMark1->setObjectName(QStringLiteral("Btn_IdDestMark1"));

        gridLayout_8->addWidget(Btn_IdDestMark1, 1, 0, 1, 1);

        Btn_GetDestMark1 = new QPushButton(page);
        Btn_GetDestMark1->setObjectName(QStringLiteral("Btn_GetDestMark1"));

        gridLayout_8->addWidget(Btn_GetDestMark1, 0, 0, 1, 1);

        Btn_CalAffineMat = new QPushButton(page);
        Btn_CalAffineMat->setObjectName(QStringLiteral("Btn_CalAffineMat"));

        gridLayout_8->addWidget(Btn_CalAffineMat, 0, 3, 1, 1);

        Btn_IdDestMark2 = new QPushButton(page);
        Btn_IdDestMark2->setObjectName(QStringLiteral("Btn_IdDestMark2"));

        gridLayout_8->addWidget(Btn_IdDestMark2, 1, 1, 1, 1);

        Mat00 = new QDoubleSpinBox(page);
        Mat00->setObjectName(QStringLiteral("Mat00"));
        Mat00->setDecimals(6);
        Mat00->setMinimum(-100000);
        Mat00->setMaximum(100000);

        gridLayout_8->addWidget(Mat00, 0, 4, 1, 1);

        Mat01 = new QDoubleSpinBox(page);
        Mat01->setObjectName(QStringLiteral("Mat01"));
        Mat01->setDecimals(6);
        Mat01->setMinimum(-100000);
        Mat01->setMaximum(100000);

        gridLayout_8->addWidget(Mat01, 0, 5, 1, 1);

        Mat02 = new QDoubleSpinBox(page);
        Mat02->setObjectName(QStringLiteral("Mat02"));
        Mat02->setDecimals(6);
        Mat02->setMinimum(-100000);
        Mat02->setMaximum(100000);

        gridLayout_8->addWidget(Mat02, 0, 6, 1, 1);

        Mat10 = new QDoubleSpinBox(page);
        Mat10->setObjectName(QStringLiteral("Mat10"));
        Mat10->setDecimals(6);
        Mat10->setMinimum(-100000);
        Mat10->setMaximum(100000);

        gridLayout_8->addWidget(Mat10, 1, 4, 1, 1);

        Mat11 = new QDoubleSpinBox(page);
        Mat11->setObjectName(QStringLiteral("Mat11"));
        Mat11->setDecimals(6);
        Mat11->setMinimum(-100000);
        Mat11->setMaximum(100000);

        gridLayout_8->addWidget(Mat11, 1, 5, 1, 1);

        Mat12 = new QDoubleSpinBox(page);
        Mat12->setObjectName(QStringLiteral("Mat12"));
        Mat12->setDecimals(6);
        Mat12->setMinimum(-100000);
        Mat12->setMaximum(100000);

        gridLayout_8->addWidget(Mat12, 1, 6, 1, 1);


        horizontalLayout_6->addLayout(gridLayout_8);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        stackedWidget->addWidget(page);

        verticalLayout_2->addWidget(stackedWidget);

        MainWidget = new QFrame(CCD_AffineEditor);
        MainWidget->setObjectName(QStringLiteral("MainWidget"));
        MainWidget->setMinimumSize(QSize(800, 640));
        MainWidget->setMaximumSize(QSize(800, 640));
        MainWidget->setFrameShape(QFrame::StyledPanel);
        MainWidget->setFrameShadow(QFrame::Raised);

        verticalLayout_2->addWidget(MainWidget);


        horizontalLayout_2->addLayout(verticalLayout_2);


        retranslateUi(CCD_AffineEditor);

        Btn_Dest->setDefault(false);
        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CCD_AffineEditor);
    } // setupUi

    void retranslateUi(QDialog *CCD_AffineEditor)
    {
        CCD_AffineEditor->setWindowTitle(QApplication::translate("CCD_AffineEditor", "Dialog", Q_NULLPTR));
        Btn_Origin->setText(QApplication::translate("CCD_AffineEditor", "\347\233\270\346\234\2721", Q_NULLPTR));
        Btn_Dest->setText(QApplication::translate("CCD_AffineEditor", "\347\233\270\346\234\2722", Q_NULLPTR));
        Btn_SaveMark->setText(QApplication::translate("CCD_AffineEditor", "\344\277\235\345\255\230\350\257\206\345\210\253\344\277\241\346\201\257", Q_NULLPTR));
        Btn_Sure->setText(QApplication::translate("CCD_AffineEditor", "\344\277\235\345\255\230\345\271\266\351\200\200\345\207\272", Q_NULLPTR));
        Btn_Cancel->setText(QApplication::translate("CCD_AffineEditor", "\345\217\226\346\266\210", Q_NULLPTR));
        Btn_IdOriginMark2->setText(QApplication::translate("CCD_AffineEditor", "\350\257\206\345\210\2532", Q_NULLPTR));
        Btn_IdOriginMark3->setText(QApplication::translate("CCD_AffineEditor", "\350\257\206\345\210\2533", Q_NULLPTR));
        Btn_GetOriginMark2->setText(QApplication::translate("CCD_AffineEditor", "\351\200\211\345\217\2262", Q_NULLPTR));
        Btn_GetOriginMark1->setText(QApplication::translate("CCD_AffineEditor", "\351\200\211\345\217\2261", Q_NULLPTR));
        Btn_IdOriginMark1->setText(QApplication::translate("CCD_AffineEditor", "\350\257\206\345\210\2531", Q_NULLPTR));
        Btn_GetOriginMark3->setText(QApplication::translate("CCD_AffineEditor", "\351\200\211\345\217\2263", Q_NULLPTR));
        Btn_GetDestMark3->setText(QApplication::translate("CCD_AffineEditor", "\351\200\211\345\217\2263", Q_NULLPTR));
        Btn_IdDestMark3->setText(QApplication::translate("CCD_AffineEditor", "\350\257\206\345\210\2533", Q_NULLPTR));
        Btn_GetDestMark2->setText(QApplication::translate("CCD_AffineEditor", "\351\200\211\345\217\2262", Q_NULLPTR));
        Btn_IdDestMark1->setText(QApplication::translate("CCD_AffineEditor", "\350\257\206\345\210\2531", Q_NULLPTR));
        Btn_GetDestMark1->setText(QApplication::translate("CCD_AffineEditor", "\351\200\211\345\217\2261", Q_NULLPTR));
        Btn_CalAffineMat->setText(QApplication::translate("CCD_AffineEditor", "\350\256\241\347\256\227", Q_NULLPTR));
        Btn_IdDestMark2->setText(QApplication::translate("CCD_AffineEditor", "\350\257\206\345\210\2532", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_AffineEditor: public Ui_CCD_AffineEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_AFFINEEDITOR_H
