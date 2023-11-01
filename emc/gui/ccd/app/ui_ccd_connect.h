/********************************************************************************
** Form generated from reading UI file 'ccd_connect.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCD_CONNECT_H
#define UI_CCD_CONNECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ccd_connect
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *Host_IP;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpinBox *Host_Port;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *Local_IP;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_7;
    QLabel *Local_name;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_7;
    QComboBox *portNameComboBox;
    QLabel *label_8;
    QComboBox *baudRateComboBox;
    QLabel *label_9;
    QComboBox *dataBitsComboBox;
    QLabel *label_10;
    QComboBox *parityComboBox;
    QLabel *label_11;
    QComboBox *stopBitsComboBox;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *dialogButtonBox;

    void setupUi(QDialog *ccd_connect)
    {
        if (ccd_connect->objectName().isEmpty())
            ccd_connect->setObjectName(QStringLiteral("ccd_connect"));
        ccd_connect->resize(365, 430);
        verticalLayout = new QVBoxLayout(ccd_connect);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(ccd_connect);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget_3 = new QWidget(groupBox);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setSpacing(2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(2, 2, 2, 2);
        label_3 = new QLabel(widget_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(40, 0));

        horizontalLayout_3->addWidget(label_3);

        widget = new QWidget(widget_3);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        Host_IP = new QLineEdit(widget);
        Host_IP->setObjectName(QStringLiteral("Host_IP"));
        Host_IP->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(Host_IP);


        horizontalLayout_3->addWidget(widget);

        widget_2 = new QWidget(widget_3);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        Host_Port = new QSpinBox(widget_2);
        Host_Port->setObjectName(QStringLiteral("Host_Port"));
        Host_Port->setButtonSymbols(QAbstractSpinBox::NoButtons);
        Host_Port->setMaximum(10000);

        horizontalLayout_2->addWidget(Host_Port);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 10);

        horizontalLayout_3->addWidget(widget_2);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 5);
        horizontalLayout_3->setStretch(2, 1);

        verticalLayout_2->addWidget(widget_3);

        widget_4 = new QWidget(groupBox);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        horizontalLayout_4 = new QHBoxLayout(widget_4);
        horizontalLayout_4->setSpacing(2);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(2, 2, 2, 2);
        label_4 = new QLabel(widget_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(40, 0));

        horizontalLayout_4->addWidget(label_4);

        widget_5 = new QWidget(widget_4);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        horizontalLayout_5 = new QHBoxLayout(widget_5);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(2, 2, 2, 2);
        label_5 = new QLabel(widget_5);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        Local_IP = new QLineEdit(widget_5);
        Local_IP->setObjectName(QStringLiteral("Local_IP"));

        horizontalLayout_5->addWidget(Local_IP);


        horizontalLayout_4->addWidget(widget_5);

        horizontalLayout_4->setStretch(1, 3);

        verticalLayout_2->addWidget(widget_4);

        widget_6 = new QWidget(groupBox);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        horizontalLayout_6 = new QHBoxLayout(widget_6);
        horizontalLayout_6->setSpacing(2);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(2, 2, 2, 2);
        label_6 = new QLabel(widget_6);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMinimumSize(QSize(40, 0));

        horizontalLayout_6->addWidget(label_6);

        widget_7 = new QWidget(widget_6);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        horizontalLayout_7 = new QHBoxLayout(widget_7);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(2, 2, 2, 2);
        Local_name = new QLabel(widget_7);
        Local_name->setObjectName(QStringLiteral("Local_name"));

        horizontalLayout_7->addWidget(Local_name);


        horizontalLayout_6->addWidget(widget_7);

        horizontalLayout_6->setStretch(1, 3);

        verticalLayout_2->addWidget(widget_6);


        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(ccd_connect);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setStyleSheet(QStringLiteral(""));
        groupBox_3->setFlat(false);
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label_7);

        portNameComboBox = new QComboBox(groupBox_3);
        portNameComboBox->setObjectName(QStringLiteral("portNameComboBox"));

        verticalLayout_3->addWidget(portNameComboBox);

        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label_8);

        baudRateComboBox = new QComboBox(groupBox_3);
        baudRateComboBox->setObjectName(QStringLiteral("baudRateComboBox"));

        verticalLayout_3->addWidget(baudRateComboBox);

        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QStringLiteral("label_9"));
        sizePolicy.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label_9);

        dataBitsComboBox = new QComboBox(groupBox_3);
        dataBitsComboBox->setObjectName(QStringLiteral("dataBitsComboBox"));

        verticalLayout_3->addWidget(dataBitsComboBox);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        sizePolicy.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label_10);

        parityComboBox = new QComboBox(groupBox_3);
        parityComboBox->setObjectName(QStringLiteral("parityComboBox"));

        verticalLayout_3->addWidget(parityComboBox);

        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QStringLiteral("label_11"));
        sizePolicy.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label_11);

        stopBitsComboBox = new QComboBox(groupBox_3);
        stopBitsComboBox->setObjectName(QStringLiteral("stopBitsComboBox"));

        verticalLayout_3->addWidget(stopBitsComboBox);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer);


        verticalLayout->addWidget(groupBox_3);

        dialogButtonBox = new QDialogButtonBox(ccd_connect);
        dialogButtonBox->setObjectName(QStringLiteral("dialogButtonBox"));
        dialogButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(dialogButtonBox);


        retranslateUi(ccd_connect);

        baudRateComboBox->setCurrentIndex(12);
        dataBitsComboBox->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(ccd_connect);
    } // setupUi

    void retranslateUi(QDialog *ccd_connect)
    {
        ccd_connect->setWindowTitle(QApplication::translate("ccd_connect", "Dialog", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("ccd_connect", "TCP\351\200\232\350\256\257\350\256\276\347\275\256", Q_NULLPTR));
        label_3->setText(QApplication::translate("ccd_connect", "\347\233\256\346\240\207\344\270\273\346\234\272:", Q_NULLPTR));
        label->setText(QApplication::translate("ccd_connect", "IP:", Q_NULLPTR));
        label_2->setText(QApplication::translate("ccd_connect", "\347\253\257\345\217\243:", Q_NULLPTR));
        label_4->setText(QApplication::translate("ccd_connect", "\346\234\254\345\234\260\344\270\273\346\234\272:", Q_NULLPTR));
        label_5->setText(QApplication::translate("ccd_connect", "IP:", Q_NULLPTR));
        label_6->setText(QApplication::translate("ccd_connect", "\346\234\254\345\234\260\350\256\276\345\244\207\345\220\215\347\247\260:", Q_NULLPTR));
        Local_name->setText(QString());
        groupBox_3->setTitle(QApplication::translate("ccd_connect", "\344\270\262\345\217\243\351\200\232\350\256\257\350\256\276\347\275\256", Q_NULLPTR));
        label_7->setText(QApplication::translate("ccd_connect", "\344\270\262\345\217\243", Q_NULLPTR));
        label_8->setText(QApplication::translate("ccd_connect", "\346\263\242\347\211\271\347\216\207", Q_NULLPTR));
        baudRateComboBox->clear();
        baudRateComboBox->insertItems(0, QStringList()
         << QApplication::translate("ccd_connect", "50", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "75", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "110", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "134", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "150", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "200", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "300", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "600", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "1200", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "1800", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "2400", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "4800", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "9600", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "14400", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "19200", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "38400", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "56000", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "57600", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "76800", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "115200", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "128000", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "256000", Q_NULLPTR)
        );
        label_9->setText(QApplication::translate("ccd_connect", "\346\225\260\346\215\256\344\275\215", Q_NULLPTR));
        dataBitsComboBox->clear();
        dataBitsComboBox->insertItems(0, QStringList()
         << QApplication::translate("ccd_connect", "5", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "6", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "7", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "8", Q_NULLPTR)
        );
        label_10->setText(QApplication::translate("ccd_connect", "\346\240\241\351\252\214\344\275\215", Q_NULLPTR));
        parityComboBox->clear();
        parityComboBox->insertItems(0, QStringList()
         << QApplication::translate("ccd_connect", "\346\227\240", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "\345\245\207", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "\345\201\266", Q_NULLPTR)
        );
        label_11->setText(QApplication::translate("ccd_connect", "\345\201\234\346\255\242\344\275\215", Q_NULLPTR));
        stopBitsComboBox->clear();
        stopBitsComboBox->insertItems(0, QStringList()
         << QApplication::translate("ccd_connect", "1", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "1.5", Q_NULLPTR)
         << QApplication::translate("ccd_connect", "2", Q_NULLPTR)
        );
    } // retranslateUi

};

namespace Ui {
    class ccd_connect: public Ui_ccd_connect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCD_CONNECT_H
