#ifndef GL_VIEWDRAW_H
#define GL_VIEWDRAW_H

#include "gl_canondraw.h"

class GL_ViewDraw :public GL_CanonDraw
{
public:
    GL_ViewDraw();
    void redrawPerspective();
    void canonRedraw();
    void updateModule(GL_GcodeDraw *);

    void translate(int x, int y);
    void rotate(int x, int y);
    void recordMouse(int x, int y);

    virtual int getWindowHeight();
    virtual int getWindowWidth();
    virtual void viewX();
    virtual void viewY();
    virtual void viewZ();
    virtual void viewP();
    virtual void viewZ2();
    virtual void reset();



protected:

    void glTranslateScene(double scale,int x, int y,
                          int mousex, int mousey);

    void glRotateScene(double scale,double xc,double yc,double zc,
                       int x, int y,int mousex, int mousey);

    void setEyePointFromExtent(float e1,float e2);
    void setEyepoint(float);


    double distance;
    int xmouse;
    int ymouse;
    double xcenter;
    double ycenter;
    double zcenter;

    float maxlat;
    float minlat;
    float lat;
    float lon;

    double maxextents[3];
    double minextents[3];

private:
    double max(double v1,double v2);
    double min(double v1,double v2);
    int max(int v1,int v2);
    int min(int v1,int v2);
    float snap(float a);


};

#endif // GL_VIEWDRAW_H
