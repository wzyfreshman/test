#include <GL/glew.h>

#include <math.h>
#include <stdio.h>
#include <QTime>
#include <QDebug>

#include "gl_viewdraw.h"
#include "gl_gcodedraw.h"

GL_ViewDraw::GL_ViewDraw() {
    distance = 1;
    xcenter = 0;
    ycenter = 0;
    zcenter = 0;

    maxlat = 0;
    minlat = -90;
    lat = 0;
    lon = 0;

    maxextents[0]=0.5;
    minextents[0]=0;

    maxextents[1]=0.5;
    minextents[1]=0;

    maxextents[2]=0;
    minextents[2]=0;

}


void GL_ViewDraw::redrawPerspective() {

    int h = getWindowHeight();
    int w = getWindowWidth();
    glViewport(0, 0, w, h);
    glClearColor(color_background[0],
                 color_background[1],
                 color_background[2],
                color_background[3]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double ztran = distance;
    if(ztran <= 0)
        ztran = 0;

    double k = pow( fabs(ztran),0.55555);
    double l = k * h / w;
    glOrtho(-k, k, -l, l, -1000, 1000.);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    canonRedraw();

    glFlush();
    glPopMatrix();


}

int GL_ViewDraw::getWindowHeight() {
    return 0;
}
int GL_ViewDraw::getWindowWidth() {
    return 0;
}

void GL_ViewDraw::viewX() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    float mid[3];
    float size[3];
    for(int i=0;i<3;i++)
        mid[i] = (maxextents[i]+minextents[i])/2.0;
    for(int i=0;i<3;i++)
        size[i] = (maxextents[i]-minextents[i]);

    glTranslatef(-mid[0], -mid[1], -mid[2]);
    setEyePointFromExtent(size[1], size[2]);
    lat = -90;
    lon = 270;


}

void GL_ViewDraw::viewY() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 1, 0, 0);

    float mid[3];
    float size[3];
    for(int i=0;i<3;i++)
        mid[i] = (maxextents[i]+minextents[i])/2.0;
    for(int i=0;i<3;i++)
        size[i] = (maxextents[i]-minextents[i]);

    glTranslatef(-mid[0], -mid[1], -mid[2]);
    setEyePointFromExtent(size[0], size[2]);
    lat = -90;
    lon = 0;

}

void GL_ViewDraw::reset() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void GL_ViewDraw::viewZ() {


    float mid[3];
    float size[3];
    reset();

    for(int i=0;i<3;i++)
        mid[i] = (maxextents[i]+minextents[i])/2.0;
    for(int i=0;i<3;i++)
        size[i] = (maxextents[i]-minextents[i]);

    glTranslatef(-mid[0], -mid[1], -mid[2]);
    setEyePointFromExtent(size[0], size[1]);

    lat = lon = 0.0;


}

void GL_ViewDraw::viewP() {


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 1, 0, 0);

    float mid[3];
    float size[3];
    for(int i=0;i<3;i++)
        mid[i] = (maxextents[i]+minextents[i])/2.0;
    for(int i=0;i<3;i++)
        size[i] = (maxextents[i]-minextents[i]);

    glTranslatef(-mid[0], -mid[1], -mid[2]);

    int w,h;
    w = getWindowWidth();
    h = getWindowHeight();

    double sizef = sqrt(size[0]*size[0] + size[1]*size[1] +size[2]*size[2]);
    setEyepoint(sizef*(sizef)*0.75);

    lat = -60;
    lon = 335;
    float x = (minextents[0] + maxextents[0])/2;
    float y = (minextents[1] + maxextents[1])/2;
    float z = (minextents[2] + maxextents[2])/2;
    glRotateScene(1.0, x, y, z, 0, 0, 0, 0);



}

void GL_ViewDraw::viewZ2() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 0, 0, 1);

    float mid[3];
    float size[3];
    for(int i=0;i<3;i++)
        mid[i] = (maxextents[i]+minextents[i])/2.0;
    for(int i=0;i<3;i++)
        size[i] = (maxextents[i]-minextents[i]);

    glTranslatef(-mid[0], -mid[1], -mid[2]);
    setEyePointFromExtent(size[1], size[0]);
    lat = 0;
    lon = 270;

}

void GL_ViewDraw::updateModule(GL_GcodeDraw *m) {


    maxextents[0] = m->max_x;
    maxextents[1] = m->max_y;
    maxextents[2] = m->max_z;

    minextents[0] = m->min_x;
    minextents[1] = m->min_y;
    minextents[2] = m->min_z;

    xcenter =( maxextents[0]+minextents[0])/2;
    ycenter =( maxextents[1]+minextents[1])/2;
    zcenter =( maxextents[2]+minextents[2])/2;

    flushModelList(m);

}

void GL_ViewDraw::canonRedraw() {

    canonDraw();

}

void GL_ViewDraw::rotate(int x, int y) {

   glRotateScene(0.5, xcenter, ycenter, zcenter,
                 x, y, xmouse, ymouse);
   recordMouse(x,y);

}

void GL_ViewDraw::translate(int x, int y) {

    int win_height = getWindowHeight();
    if(win_height<1) win_height = 1;

    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

    GLdouble winx,winy,winz;
    GLdouble objx,objy,objz;
    gluProject(xcenter,ycenter,zcenter,
               modelMatrix,projMatrix,viewport,
               &winx,&winy,&winz);

    gluUnProject(winx,winy+0.5*win_height,winz,
                 modelMatrix,projMatrix,viewport,
                 &objx,&objy,&objz);

    double dist = sqrt( (objx-xcenter)*(objx-xcenter) +
                        (objy-ycenter)*(objy-ycenter) +
                        (objz-zcenter)*(objz-zcenter));

    double scale = fabs(dist / ( 0.5 * win_height ));
    glTranslateScene(scale,x, y, xmouse, ymouse);
    recordMouse(x,y);

}

void GL_ViewDraw::setEyePointFromExtent(float e1, float e2) {

   int w = getWindowWidth();
   int h = getWindowHeight();
   float ztran;
   ztran = 2.0<e1? e1:2.0;
   ztran = (e2 * w/h)<ztran? ztran:(e2 * w/h);
   ztran *= ztran;
   setEyepoint(ztran - zcenter);

}

void GL_ViewDraw::setEyepoint(float d) {
    distance = d;
}

void GL_ViewDraw::glTranslateScene(double s, int x, int y,
                                   int mousex, int mousey) {

    GLdouble mat[16];
    glMatrixMode(GL_MODELVIEW);
    glGetDoublev(GL_MODELVIEW_MATRIX,mat);
    glLoadIdentity();
    glTranslatef(s * (x - mousex), s * (mousey - y), 0.0);
    glMultMatrixd(mat);

}

void GL_ViewDraw::glRotateScene(double , double xc,
                                double yc, double zc,
                                int x, int y,
                                int mousex, int mousey) {

    GLdouble mat[16];
    float latl = min(maxlat, max(minlat, lat + (y - mousey)/2.0 ));
    float lonl = (lon + (x - mousex)/2.0);

    while(lonl>360.0) {
        lonl -= 360.0;
    }

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(xc, yc, zc);

    glGetDoublev(GL_MODELVIEW_MATRIX,mat);
    glLoadIdentity();
    double tx,ty,tz;
    tx = mat[12];ty = mat[13];tz = mat[14];

    glTranslatef(tx, ty, tz);
    glRotatef((latl),1.0,0.0,0.0);
    glRotatef((lonl), 0.0,0.0,1.0);
    glTranslatef(-xc, -yc, -zc);

    lat = latl;
    lon = lonl;

}


void GL_ViewDraw::recordMouse(int x, int y) {

    xmouse = x;
    ymouse = y;

}

double GL_ViewDraw::max(double v1, double v2) {

    if(v1>v2) return v1;
    else return v2;

}

double GL_ViewDraw::min(double v1, double v2) {

    if(v1<v2) return v1;
    else return v2;

}

int GL_ViewDraw::max(int v1, int v2) {

    if(v1>v2) return v1;
    else return v2;

}

int GL_ViewDraw::min(int v1, int v2) {

    if(v1<v2) return v1;
    else return v2;

}

float GL_ViewDraw::snap(float ) {

    return 0.0;

}
