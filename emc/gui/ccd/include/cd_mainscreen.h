#ifndef CD_MAINSCREEN_H
#define CD_MAINSCREEN_H

#include "cd_imagescreen.h"

class CCD_API CD_MainScreen : public CD_ImageScreen
{
public:
    CD_MainScreen(QWidget *parent);

protected:
    virtual void paintEvent(QPaintEvent *);
private:

};

#endif // CD_MAINSCREEN_H
