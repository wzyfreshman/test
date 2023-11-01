/********************************************************************************
** Form generated from reading UI file 'best_testfuneditor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEST_TESTFUNEDITOR_H
#define UI_BEST_TESTFUNEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Best_TestFunEditor
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout;
    QCheckBox *ResLinerK;
    QCheckBox *ResLinerF;
    QCheckBox *ResLinerG;
    QCheckBox *ResLinerEnable;
    QCheckBox *ResLinerX;
    QCheckBox *ResYNbh;
    QCheckBox *ResInsulateEnable;
    QCheckBox *ResEnable;
    QCheckBox *ResLinerY;
    QCheckBox *ResInsulateXY;
    QCheckBox *ResYApt;
    QCheckBox *ResXNbh;
    QCheckBox *ResXApt;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *xyInsulateModeFast;
    QCheckBox *xyInsulateModeCross;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *InsulateK;
    QTextEdit *ResBrowserK;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *InsulateF;
    QTextEdit *ResBrowserF;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *InsulateG;
    QTextEdit *ResBrowserG;
    QGridLayout *gridLayout_2;
    QComboBox *DestNum2;
    QComboBox *SourceNum;
    QCheckBox *onlyOne;
    QComboBox *DestType1;
    QComboBox *SourceType;
    QLabel *label;
    QLabel *label_3;
    QComboBox *DestType2;
    QPushButton *ResRemove;
    QComboBox *DestNum1;
    QPushButton *ResAdd;
    QLabel *label_2;
    QLabel *label_4;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *OpenEnable;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *OpenModeFast;
    QCheckBox *OpenModeArray;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *CapEnable;
    QCheckBox *CapX;
    QCheckBox *CapY;
    QCheckBox *CapK;
    QCheckBox *CapF;
    QCheckBox *CapG;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab_4;
    QHBoxLayout *horizontalLayout_7;
    QGridLayout *gridLayout_4;
    QPushButton *KeyAdd;
    QCheckBox *KeyModeCross;
    QComboBox *KeyViewType;
    QCheckBox *KeyModeSingle;
    QComboBox *KeyViewNum;
    QPushButton *KeyRemove;
    QCheckBox *KeyEnable;
    QSpacerItem *verticalSpacer_3;
    QComboBox *KeyViewK;
    QTextEdit *KeyBrowser;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *Btn_Cancel;
    QPushButton *Btn_Sure;

    void setupUi(QDialog *Best_TestFunEditor)
    {
        if (Best_TestFunEditor->objectName().isEmpty())
            Best_TestFunEditor->setObjectName(QString::fromUtf8("Best_TestFunEditor"));
        Best_TestFunEditor->resize(752, 508);
        verticalLayout = new QVBoxLayout(Best_TestFunEditor);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(Best_TestFunEditor);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_5 = new QVBoxLayout(tab);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        ResLinerK = new QCheckBox(tab);
        ResLinerK->setObjectName(QString::fromUtf8("ResLinerK"));

        gridLayout->addWidget(ResLinerK, 1, 3, 1, 1);

        ResLinerF = new QCheckBox(tab);
        ResLinerF->setObjectName(QString::fromUtf8("ResLinerF"));

        gridLayout->addWidget(ResLinerF, 1, 4, 1, 1);

        ResLinerG = new QCheckBox(tab);
        ResLinerG->setObjectName(QString::fromUtf8("ResLinerG"));

        gridLayout->addWidget(ResLinerG, 1, 5, 1, 1);

        ResLinerEnable = new QCheckBox(tab);
        ResLinerEnable->setObjectName(QString::fromUtf8("ResLinerEnable"));

        gridLayout->addWidget(ResLinerEnable, 1, 0, 1, 1);

        ResLinerX = new QCheckBox(tab);
        ResLinerX->setObjectName(QString::fromUtf8("ResLinerX"));

        gridLayout->addWidget(ResLinerX, 1, 1, 1, 1);

        ResYNbh = new QCheckBox(tab);
        ResYNbh->setObjectName(QString::fromUtf8("ResYNbh"));

        gridLayout->addWidget(ResYNbh, 2, 3, 1, 1);

        ResInsulateEnable = new QCheckBox(tab);
        ResInsulateEnable->setObjectName(QString::fromUtf8("ResInsulateEnable"));

        gridLayout->addWidget(ResInsulateEnable, 2, 0, 1, 1);

        ResEnable = new QCheckBox(tab);
        ResEnable->setObjectName(QString::fromUtf8("ResEnable"));

        gridLayout->addWidget(ResEnable, 0, 0, 1, 1);

        ResLinerY = new QCheckBox(tab);
        ResLinerY->setObjectName(QString::fromUtf8("ResLinerY"));

        gridLayout->addWidget(ResLinerY, 1, 2, 1, 1);

        ResInsulateXY = new QCheckBox(tab);
        ResInsulateXY->setObjectName(QString::fromUtf8("ResInsulateXY"));

        gridLayout->addWidget(ResInsulateXY, 2, 5, 1, 1);

        ResYApt = new QCheckBox(tab);
        ResYApt->setObjectName(QString::fromUtf8("ResYApt"));

        gridLayout->addWidget(ResYApt, 2, 4, 1, 1);

        ResXNbh = new QCheckBox(tab);
        ResXNbh->setObjectName(QString::fromUtf8("ResXNbh"));

        gridLayout->addWidget(ResXNbh, 2, 1, 1, 1);

        ResXApt = new QCheckBox(tab);
        ResXApt->setObjectName(QString::fromUtf8("ResXApt"));

        gridLayout->addWidget(ResXApt, 2, 2, 1, 1);

        widget = new QWidget(tab);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        xyInsulateModeFast = new QCheckBox(widget);
        xyInsulateModeFast->setObjectName(QString::fromUtf8("xyInsulateModeFast"));
        xyInsulateModeFast->setAutoExclusive(true);

        horizontalLayout_2->addWidget(xyInsulateModeFast);

        xyInsulateModeCross = new QCheckBox(widget);
        xyInsulateModeCross->setObjectName(QString::fromUtf8("xyInsulateModeCross"));
        xyInsulateModeCross->setAutoExclusive(true);

        horizontalLayout_2->addWidget(xyInsulateModeCross);


        gridLayout->addWidget(widget, 2, 6, 1, 1);


        verticalLayout_5->addLayout(gridLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        InsulateK = new QCheckBox(tab);
        InsulateK->setObjectName(QString::fromUtf8("InsulateK"));

        verticalLayout_2->addWidget(InsulateK);

        ResBrowserK = new QTextEdit(tab);
        ResBrowserK->setObjectName(QString::fromUtf8("ResBrowserK"));

        verticalLayout_2->addWidget(ResBrowserK);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        InsulateF = new QCheckBox(tab);
        InsulateF->setObjectName(QString::fromUtf8("InsulateF"));

        verticalLayout_3->addWidget(InsulateF);

        ResBrowserF = new QTextEdit(tab);
        ResBrowserF->setObjectName(QString::fromUtf8("ResBrowserF"));

        verticalLayout_3->addWidget(ResBrowserF);


        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        InsulateG = new QCheckBox(tab);
        InsulateG->setObjectName(QString::fromUtf8("InsulateG"));

        verticalLayout_4->addWidget(InsulateG);

        ResBrowserG = new QTextEdit(tab);
        ResBrowserG->setObjectName(QString::fromUtf8("ResBrowserG"));

        verticalLayout_4->addWidget(ResBrowserG);


        horizontalLayout_3->addLayout(verticalLayout_4);


        verticalLayout_5->addLayout(horizontalLayout_3);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        DestNum2 = new QComboBox(tab);
        DestNum2->setObjectName(QString::fromUtf8("DestNum2"));

        gridLayout_2->addWidget(DestNum2, 1, 6, 1, 1);

        SourceNum = new QComboBox(tab);
        SourceNum->setObjectName(QString::fromUtf8("SourceNum"));

        gridLayout_2->addWidget(SourceNum, 1, 1, 1, 1);

        onlyOne = new QCheckBox(tab);
        onlyOne->setObjectName(QString::fromUtf8("onlyOne"));

        gridLayout_2->addWidget(onlyOne, 1, 0, 1, 1);

        DestType1 = new QComboBox(tab);
        DestType1->setObjectName(QString::fromUtf8("DestType1"));

        gridLayout_2->addWidget(DestType1, 0, 4, 1, 1);

        SourceType = new QComboBox(tab);
        SourceType->setObjectName(QString::fromUtf8("SourceType"));

        gridLayout_2->addWidget(SourceType, 0, 1, 1, 1);

        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_3, 0, 5, 1, 1);

        DestType2 = new QComboBox(tab);
        DestType2->setObjectName(QString::fromUtf8("DestType2"));

        gridLayout_2->addWidget(DestType2, 0, 6, 1, 1);

        ResRemove = new QPushButton(tab);
        ResRemove->setObjectName(QString::fromUtf8("ResRemove"));

        gridLayout_2->addWidget(ResRemove, 1, 7, 1, 1);

        DestNum1 = new QComboBox(tab);
        DestNum1->setObjectName(QString::fromUtf8("DestNum1"));

        gridLayout_2->addWidget(DestNum1, 1, 4, 1, 1);

        ResAdd = new QPushButton(tab);
        ResAdd->setObjectName(QString::fromUtf8("ResAdd"));

        gridLayout_2->addWidget(ResAdd, 0, 7, 1, 1);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_2, 0, 3, 1, 1);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_4, 0, 2, 1, 1);


        verticalLayout_5->addLayout(gridLayout_2);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_7 = new QVBoxLayout(tab_2);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        OpenEnable = new QCheckBox(tab_2);
        OpenEnable->setObjectName(QString::fromUtf8("OpenEnable"));

        horizontalLayout_4->addWidget(OpenEnable);

        widget_2 = new QWidget(tab_2);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_5 = new QHBoxLayout(widget_2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 0, -1, 0);
        OpenModeFast = new QCheckBox(widget_2);
        OpenModeFast->setObjectName(QString::fromUtf8("OpenModeFast"));
        OpenModeFast->setAutoExclusive(true);

        horizontalLayout_5->addWidget(OpenModeFast);

        OpenModeArray = new QCheckBox(widget_2);
        OpenModeArray->setObjectName(QString::fromUtf8("OpenModeArray"));
        OpenModeArray->setAutoExclusive(true);

        horizontalLayout_5->addWidget(OpenModeArray);

        horizontalSpacer_3 = new QSpacerItem(461, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        horizontalLayout_4->addWidget(widget_2);


        verticalLayout_7->addLayout(horizontalLayout_4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_8 = new QVBoxLayout(tab_3);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        CapEnable = new QCheckBox(tab_3);
        CapEnable->setObjectName(QString::fromUtf8("CapEnable"));

        horizontalLayout_6->addWidget(CapEnable);

        CapX = new QCheckBox(tab_3);
        CapX->setObjectName(QString::fromUtf8("CapX"));

        horizontalLayout_6->addWidget(CapX);

        CapY = new QCheckBox(tab_3);
        CapY->setObjectName(QString::fromUtf8("CapY"));

        horizontalLayout_6->addWidget(CapY);

        CapK = new QCheckBox(tab_3);
        CapK->setObjectName(QString::fromUtf8("CapK"));

        horizontalLayout_6->addWidget(CapK);

        CapF = new QCheckBox(tab_3);
        CapF->setObjectName(QString::fromUtf8("CapF"));

        horizontalLayout_6->addWidget(CapF);

        CapG = new QCheckBox(tab_3);
        CapG->setObjectName(QString::fromUtf8("CapG"));

        horizontalLayout_6->addWidget(CapG);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout_8->addLayout(horizontalLayout_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_2);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        horizontalLayout_7 = new QHBoxLayout(tab_4);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        KeyAdd = new QPushButton(tab_4);
        KeyAdd->setObjectName(QString::fromUtf8("KeyAdd"));

        gridLayout_4->addWidget(KeyAdd, 4, 1, 1, 1);

        KeyModeCross = new QCheckBox(tab_4);
        KeyModeCross->setObjectName(QString::fromUtf8("KeyModeCross"));
        KeyModeCross->setAutoExclusive(true);

        gridLayout_4->addWidget(KeyModeCross, 2, 0, 1, 1);

        KeyViewType = new QComboBox(tab_4);
        KeyViewType->setObjectName(QString::fromUtf8("KeyViewType"));

        gridLayout_4->addWidget(KeyViewType, 3, 1, 1, 1);

        KeyModeSingle = new QCheckBox(tab_4);
        KeyModeSingle->setObjectName(QString::fromUtf8("KeyModeSingle"));
        KeyModeSingle->setAutoExclusive(true);

        gridLayout_4->addWidget(KeyModeSingle, 1, 0, 1, 1);

        KeyViewNum = new QComboBox(tab_4);
        KeyViewNum->setObjectName(QString::fromUtf8("KeyViewNum"));

        gridLayout_4->addWidget(KeyViewNum, 3, 2, 1, 1);

        KeyRemove = new QPushButton(tab_4);
        KeyRemove->setObjectName(QString::fromUtf8("KeyRemove"));

        gridLayout_4->addWidget(KeyRemove, 4, 2, 1, 1);

        KeyEnable = new QCheckBox(tab_4);
        KeyEnable->setObjectName(QString::fromUtf8("KeyEnable"));

        gridLayout_4->addWidget(KeyEnable, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_3, 5, 2, 1, 1);

        KeyViewK = new QComboBox(tab_4);
        KeyViewK->setObjectName(QString::fromUtf8("KeyViewK"));

        gridLayout_4->addWidget(KeyViewK, 3, 0, 1, 1);


        horizontalLayout_7->addLayout(gridLayout_4);

        KeyBrowser = new QTextEdit(tab_4);
        KeyBrowser->setObjectName(QString::fromUtf8("KeyBrowser"));

        horizontalLayout_7->addWidget(KeyBrowser);

        tabWidget->addTab(tab_4, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Btn_Cancel = new QPushButton(Best_TestFunEditor);
        Btn_Cancel->setObjectName(QString::fromUtf8("Btn_Cancel"));

        horizontalLayout->addWidget(Btn_Cancel);

        Btn_Sure = new QPushButton(Best_TestFunEditor);
        Btn_Sure->setObjectName(QString::fromUtf8("Btn_Sure"));

        horizontalLayout->addWidget(Btn_Sure);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Best_TestFunEditor);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Best_TestFunEditor);
    } // setupUi

    void retranslateUi(QDialog *Best_TestFunEditor)
    {
        Best_TestFunEditor->setWindowTitle(QApplication::translate("Best_TestFunEditor", "Dialog", 0, QApplication::UnicodeUTF8));
        ResLinerK->setText(QApplication::translate("Best_TestFunEditor", "K", 0, QApplication::UnicodeUTF8));
        ResLinerF->setText(QApplication::translate("Best_TestFunEditor", "F", 0, QApplication::UnicodeUTF8));
        ResLinerG->setText(QApplication::translate("Best_TestFunEditor", "G", 0, QApplication::UnicodeUTF8));
        ResLinerEnable->setText(QApplication::translate("Best_TestFunEditor", "\347\272\277\346\200\247", 0, QApplication::UnicodeUTF8));
        ResLinerX->setText(QApplication::translate("Best_TestFunEditor", "X", 0, QApplication::UnicodeUTF8));
        ResYNbh->setText(QApplication::translate("Best_TestFunEditor", "Y\347\233\270\351\202\273", 0, QApplication::UnicodeUTF8));
        ResInsulateEnable->setText(QApplication::translate("Best_TestFunEditor", "\347\273\235\347\274\230", 0, QApplication::UnicodeUTF8));
        ResEnable->setText(QApplication::translate("Best_TestFunEditor", "\346\265\213\350\257\225\344\275\277\350\203\275", 0, QApplication::UnicodeUTF8));
        ResLinerY->setText(QApplication::translate("Best_TestFunEditor", "Y", 0, QApplication::UnicodeUTF8));
        ResInsulateXY->setText(QApplication::translate("Best_TestFunEditor", " XY\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        ResYApt->setText(QApplication::translate("Best_TestFunEditor", "Y\347\233\270\351\232\224", 0, QApplication::UnicodeUTF8));
        ResXNbh->setText(QApplication::translate("Best_TestFunEditor", "X\347\233\270\351\202\273", 0, QApplication::UnicodeUTF8));
        ResXApt->setText(QApplication::translate("Best_TestFunEditor", "X\347\233\270\351\232\224", 0, QApplication::UnicodeUTF8));
        xyInsulateModeFast->setText(QApplication::translate("Best_TestFunEditor", "\345\277\253\351\200\237", 0, QApplication::UnicodeUTF8));
        xyInsulateModeCross->setText(QApplication::translate("Best_TestFunEditor", "\347\202\271\351\230\265", 0, QApplication::UnicodeUTF8));
        InsulateK->setText(QApplication::translate("Best_TestFunEditor", "\347\273\235\347\274\230K", 0, QApplication::UnicodeUTF8));
        InsulateF->setText(QApplication::translate("Best_TestFunEditor", "\347\273\235\347\274\230F", 0, QApplication::UnicodeUTF8));
        InsulateG->setText(QApplication::translate("Best_TestFunEditor", "\347\273\235\347\274\230G", 0, QApplication::UnicodeUTF8));
        onlyOne->setText(QApplication::translate("Best_TestFunEditor", "\344\273\205\345\257\271\344\270\200\344\270\252\351\200\232\351\201\223", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Best_TestFunEditor", "\351\200\232\351\201\2231", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Best_TestFunEditor", "\351\200\232\351\201\2233", 0, QApplication::UnicodeUTF8));
        ResRemove->setText(QApplication::translate("Best_TestFunEditor", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        ResAdd->setText(QApplication::translate("Best_TestFunEditor", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Best_TestFunEditor", "\351\200\232\351\201\2232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Best_TestFunEditor", "---->>>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Best_TestFunEditor", "\347\224\265\351\230\273", 0, QApplication::UnicodeUTF8));
        OpenEnable->setText(QApplication::translate("Best_TestFunEditor", "\346\265\213\350\257\225\344\275\277\350\203\275", 0, QApplication::UnicodeUTF8));
        OpenModeFast->setText(QApplication::translate("Best_TestFunEditor", "\345\277\253\351\200\237", 0, QApplication::UnicodeUTF8));
        OpenModeArray->setText(QApplication::translate("Best_TestFunEditor", "\347\202\271\351\230\265", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Best_TestFunEditor", "\344\272\222\347\224\265\345\256\271", 0, QApplication::UnicodeUTF8));
        CapEnable->setText(QApplication::translate("Best_TestFunEditor", "\346\265\213\350\257\225\344\275\277\350\203\275", 0, QApplication::UnicodeUTF8));
        CapX->setText(QApplication::translate("Best_TestFunEditor", "X", 0, QApplication::UnicodeUTF8));
        CapY->setText(QApplication::translate("Best_TestFunEditor", "Y", 0, QApplication::UnicodeUTF8));
        CapK->setText(QApplication::translate("Best_TestFunEditor", "K", 0, QApplication::UnicodeUTF8));
        CapF->setText(QApplication::translate("Best_TestFunEditor", "F", 0, QApplication::UnicodeUTF8));
        CapG->setText(QApplication::translate("Best_TestFunEditor", "G", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Best_TestFunEditor", "\350\207\252\347\224\265\345\256\271", 0, QApplication::UnicodeUTF8));
        KeyAdd->setText(QApplication::translate("Best_TestFunEditor", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        KeyModeCross->setText(QApplication::translate("Best_TestFunEditor", "\344\272\244\345\217\211\344\272\222\347\224\265\345\256\271\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        KeyModeSingle->setText(QApplication::translate("Best_TestFunEditor", "\345\215\225\347\213\254\350\207\252\347\224\265\345\256\271\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        KeyRemove->setText(QApplication::translate("Best_TestFunEditor", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        KeyEnable->setText(QApplication::translate("Best_TestFunEditor", "\346\265\213\350\257\225\344\275\277\350\203\275", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("Best_TestFunEditor", "\344\272\244\345\217\211\346\214\211\351\224\256", 0, QApplication::UnicodeUTF8));
        Btn_Cancel->setText(QApplication::translate("Best_TestFunEditor", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        Btn_Sure->setText(QApplication::translate("Best_TestFunEditor", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Best_TestFunEditor: public Ui_Best_TestFunEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEST_TESTFUNEDITOR_H
