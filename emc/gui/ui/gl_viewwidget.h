#ifndef GL_VIEWWIDGET_H
#define GL_VIEWWIDGET_H

#include <QWidget>
#include <stdio.h>
#include <QGLWidget>
#include <QTime>
#include <QThread>
#include <QMutex>

#include "gl_gcodedraw.h"
#include "gl_viewdraw.h"
//#include "GL/glew.h"

typedef struct GL_Color {
    unsigned char r, g, b, a;
    bool operator==(const GL_Color &o) const {
        return r == o.r && g == o.g && b == o.b && a == o.a;
    }
    bool operator!=(const GL_Color &o) const {
        return r != o.r || g != o.g || b != o.b || a != o.a;
    }
} GL_Color;

struct logger_point {
    float x, y, z;
    float rx, ry, rz;
    struct GL_Color c;
};

#define NUMCOLORS (6)
#define MAX_POINTS (10000)
typedef struct {
    int npts, mpts, lpts;
    struct logger_point *p;
    struct GL_Color colors[NUMCOLORS];
    bool exit, clear, changed;
    char *geometry;
    RCS_STAT_CHANNEL *c;

} PositionLoggerContainer;


typedef struct emcdata_struct {
    EmcPose setpos[EMCMOT_MAX_SAVEPOS];
    EmcPose lastPos;
    int index;
    int locksig;
} emcdata_struct;


#define OPENGL_DRAW_SIZE 30000

class GL_PostionLogger : public QObject {

    Q_OBJECT

    PositionLoggerContainer *state;
public:
    GL_PostionLogger();

public slots:
     int initShareMem();
     void doLogger();
     void loggerCall();
     void clear();
     struct logger_point *lastPosition();
     void setLoggerState(PositionLoggerContainer *s);
signals:
     void resultReady(const QString &result);

private:
     QMutex mutx;
     void *shmem;
     emcdata_struct * emc_data;
     EmcPose *local_pos;
     int base_index;//for clear


};

class linuxcnc;

class GL_ViewWidget : public QGLWidget ,public GL_ViewDraw
{
    Q_OBJECT
    

public:
    explicit GL_ViewWidget(QWidget *parent = 0,QString ini = "");
    ~GL_ViewWidget();

    void setMainWindow(linuxcnc *win);
    void setNcFileName(const QString &name);
    void openNcFile(QString &file);

    int getErrorCount();
    void updateToolCoordinate();
    void drawRealtimePath();
    void updateColors();
    void updateColors(const QString &,const QString &dft = "");
    void glUpdate();
    void updateMachineZeroPos();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *);

    virtual int getWindowHeight();
    virtual int getWindowWidth();
    virtual void selectLineNum(int linenum);
public slots:
    void viewX();
    void viewY();
    void viewZ();
    void viewP();
    void viewZ2();
    void clear();
    void reload();
    void zoomIn();
    void zoomOut();
    void setHighLine(int linenum);
    void clearModel();
    void abortLoad();

    void parsingFileLineNumber(int linenum);
    void setParsingFileLineCount(int);
    void parsingErrorInfo(QString str);
    void debugPrint(const QString &str);

signals:
    void signalSetLoadingMax(int value);
    void signalSetLoadingValue(int value);
    void signalErrorInfo(int,QString);
    void signalSelectHighLine(int);
    void signalSetMachineZeroPos(double x,double y);

    void signalStartThread();

private:

    void timerEvent(QTimerEvent *);

    linuxcnc *mainwin;
    int timeid;

    QString pythonfile;
    QString inifile;
    QString ncfile;
    int parsingfilelinecount;
    int errorcount;
    QTime lastwheeltime;

    QThread loggerthread;
    GL_PostionLogger *logger;
    PositionLoggerContainer *state;
    GL_GcodeDraw *gcodedrawlocal=nullptr;

};



#endif //GL_VIEWWIDGET_H
