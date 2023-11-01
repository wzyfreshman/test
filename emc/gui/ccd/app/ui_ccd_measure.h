/********************************************************************************
** Form generated from reading UI file 'ccd_measure.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_MEASURE_H
#define UI_CCD_MEASURE_H

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
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCD_Measure
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout;
    QRadioButton *rBtn_1;
    QRadioButton *rBtn_3;
    QRadioButton *rBtn_2;
    QRadioButton *rBtn_4;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QDoubleSpinBox *Lbl_camera_rate;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *Brs_Log;
    QPushButton *Btn_cleanMsg;
    QPushButton *Btn_cleanWidget;
    QFrame *line;
    QFrame *MainWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_show;

    void setupUi(QDialog *CCD_Measure)
    {
        if (CCD_Measure->objectName().isEmpty())
            CCD_Measure->setObjectName(QStringLiteral("CCD_Measure"));
        CCD_Measure->resize(1059, 664);
        CCD_Measure->setMinimumSize(QSize(1059, 664));
        CCD_Measure->setMaximumSize(QSize(1059, 664));
        horizontalLayout = new QHBoxLayout(CCD_Measure);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(CCD_Measure);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMinimumSize(QSize(230, 642));
        frame->setMaximumSize(QSize(230, 642));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        rBtn_1 = new QRadioButton(frame);
        rBtn_1->setObjectName(QStringLiteral("rBtn_1"));

        gridLayout->addWidget(rBtn_1, 0, 0, 1, 1);

        rBtn_3 = new QRadioButton(frame);
        rBtn_3->setObjectName(QStringLiteral("rBtn_3"));

        gridLayout->addWidget(rBtn_3, 2, 0, 1, 1);

        rBtn_2 = new QRadioButton(frame);
        rBtn_2->setObjectName(QStringLiteral("rBtn_2"));

        gridLayout->addWidget(rBtn_2, 1, 0, 1, 1);

        rBtn_4 = new QRadioButton(frame);
        rBtn_4->setObjectName(QStringLiteral("rBtn_4"));

        gridLayout->addWidget(rBtn_4, 3, 0, 1, 1);


        verticalLayout_3->addLayout(gridLayout);

        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);

        verticalLayout_3->addWidget(widget);

        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);

        verticalLayout_3->addWidget(label);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        Lbl_camera_rate = new QDoubleSpinBox(frame);
        Lbl_camera_rate->setObjectName(QStringLiteral("Lbl_camera_rate"));
        Lbl_camera_rate->setDecimals(4);
        Lbl_camera_rate->setMaximum(99999);

        horizontalLayout_4->addWidget(Lbl_camera_rate);


        verticalLayout_3->addLayout(horizontalLayout_4);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        Brs_Log = new QPlainTextEdit(frame);
        Brs_Log->setObjectName(QStringLiteral("Brs_Log"));
        Brs_Log->setMinimumSize(QSize(0, 300));
        Brs_Log->setReadOnly(true);

        verticalLayout->addWidget(Brs_Log);

        Btn_cleanMsg = new QPushButton(frame);
        Btn_cleanMsg->setObjectName(QStringLiteral("Btn_cleanMsg"));

        verticalLayout->addWidget(Btn_cleanMsg);

        Btn_cleanWidget = new QPushButton(frame);
        Btn_cleanWidget->setObjectName(QStringLiteral("Btn_cleanWidget"));

        verticalLayout->addWidget(Btn_cleanWidget);


        verticalLayout_3->addLayout(verticalLayout);

        line = new QFrame(frame);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);


        horizontalLayout->addWidget(frame);

        MainWidget = new QFrame(CCD_Measure);
        MainWidget->setObjectName(QStringLiteral("MainWidget"));
        MainWidget->setMinimumSize(QSize(800, 640));
        MainWidget->setMaximumSize(QSize(800, 640));
        MainWidget->setFrameShape(QFrame::StyledPanel);
        MainWidget->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(MainWidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_show = new QLabel(MainWidget);
        label_show->setObjectName(QStringLiteral("label_show"));

        horizontalLayout_2->addWidget(label_show);


        horizontalLayout->addWidget(MainWidget);

        MainWidget->raise();
        frame->raise();

        retranslateUi(CCD_Measure);

        QMetaObject::connectSlotsByName(CCD_Measure);
    } // setupUi

    void retranslateUi(QDialog *CCD_Measure)
    {
        CCD_Measure->setWindowTitle(QApplication::translate("CCD_Measure", "Dialog", Q_NULLPTR));
        rBtn_1->setText(QApplication::translate("CCD_Measure", "\347\233\270\346\234\2721", Q_NULLPTR));
        rBtn_3->setText(QApplication::translate("CCD_Measure", "\347\233\270\346\234\2723", Q_NULLPTR));
        rBtn_2->setText(QApplication::translate("CCD_Measure", "\347\233\270\346\234\2722", Q_NULLPTR));
        rBtn_4->setText(QApplication::translate("CCD_Measure", "\347\233\270\346\234\2724", Q_NULLPTR));
        label->setText(QApplication::translate("CCD_Measure", "\350\276\223\345\205\245\345\275\223\345\211\215\347\233\270\346\234\272\347\232\204\346\224\276\345\244\247\345\200\215\347\216\207\357\274\232", Q_NULLPTR));
        Btn_cleanMsg->setText(QApplication::translate("CCD_Measure", "\346\270\205\351\231\244\344\277\241\346\201\257", Q_NULLPTR));
        Btn_cleanWidget->setText(QApplication::translate("CCD_Measure", "\346\270\205\351\231\244\347\224\273\351\235\242", Q_NULLPTR));
        label_show->setText(QApplication::translate("CCD_Measure", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CCD_Measure: public Ui_CCD_Measure {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_MEASURE_H
