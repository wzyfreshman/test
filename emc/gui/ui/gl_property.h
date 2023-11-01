#ifndef GL_PROPERTY_H
#define GL_PROPERTY_H

#include <QDialog>
#include <QToolButton>
#include <QDebug>
#include"mywidget.h"

namespace Ui {
class gl_property;
}

class gl_property : public QDialog
{
    Q_OBJECT

public:
    explicit gl_property(QWidget *parent = 0);
    ~gl_property();

    ColorCombox* Color_RealTime;
    ColorCombox* Color_Feed;
    ColorCombox* Color_Feedarc;
    ColorCombox* Color_BackGround;
    ColorCombox* Color_Origin;
    ColorCombox* Color_Selected;
    ColorCombox* Color_Traverse;
    ColorCombox* Color_limit;


    QColor reRealTime;
    QColor reFeed;
    QColor reBackGround;
    QColor reArcFeed;
    QColor reOrigin;
    QColor reTraverse;
    QColor reSelected;
    QColor reLimit;

    bool setcolor_flag;

public slots:
    void gl_init();
    void slotSetRealTimeColor(QColor) ;
    void slotSetFeedColor(QColor);
    void slotSetBackGroundColor(QColor);
    void slotSetArcFeedColor(QColor);
    void slotSetOriginColor(QColor);
    void slotSetTraverseColor(QColor);
    void slotSetSelectedColor(QColor);
    void slotSetLimitColor(QColor);

protected:
//    void closeEvent(QCloseEvent *);
    void showEvent(QShowEvent *);

private slots:
    void on_sure_btn_clicked();
    void on_cancle_btn_clicked();
    void on_reset_btn_clicked();

private:
    Ui::gl_property *ui;


};





#endif // GL_PROPERTY_H
