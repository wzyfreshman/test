#ifndef CCD_MARKSFACE_H
#define CCD_MARKSFACE_H

#include <QString>

class QWidget;
class CD_MainScreen;
class CD_ImageScreen;

namespace cv {
class Mat;
}

class CCD_MarksFace
{
public:
    CCD_MarksFace();
    virtual void activeEditeScreen();
    virtual void activeSelectScreen();
    virtual void activeMainScreen();
    virtual void putImageScreenToWidget(CD_ImageScreen *s,QWidget *w);
    virtual void activeScreen(QWidget *scplace,CD_ImageScreen *sc);


    virtual void showInMainScreen(cv::Mat &mat);
    virtual void showInMainScreen(cv::Mat *mat);
    virtual void showInMainScreen(QString &filename);

protected:
    CD_MainScreen *mainscreen;
    QWidget *mainscreenplace;

};

#endif // CCD_MARKSFACE_H
