#ifndef TOUCH_CHECKLINK_H
#define TOUCH_CHECKLINK_H

#include <QDialog>

namespace Ui {
class touch_Checklink;
}
class CNCControl;
class touch_Checklink : public QDialog
{
    Q_OBJECT

   public:
       explicit touch_Checklink(QWidget *parent = 0);
       ~touch_Checklink();
       void updateUI();

   private slots:
       void on_btn_ignore_clicked();

   protected:
       void showEvent(QShowEvent *event);
   //    void timerEvent(QTimerEvent *event)override;
   private:
       Ui::touch_Checklink *ui;
};

#endif // TOUCH_CHECKLINK_H
