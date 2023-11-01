#include "gl_viewwidget.h"
#include "actionfactory.h"
#include "extern.h"
#include "shcom.hh"
#include "emc.hh"
#include "emc_nml.hh"
#include "emcstatus.hh"
#include "linuxcnc.h"

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include <QWidget>
#include <QFileInfo>
#include <QThread>
#include <QMessageBox>
#include <QWheelEvent>
#include <QDir>
#include <QDebug>
#include <QTime>

#include <GL/glu.h>

GL_ViewWidget::GL_ViewWidget(QWidget *parent, QString ini) :
    QGLWidget(parent) {

    inifile = ini;
    parsingfilelinecount = -1;
    errorcount = 0;
    gcodedrawlocal = NULL;

    lastwheeltime = QTime::currentTime();
    timeid = startTimer(280);
    state = new PositionLoggerContainer;
    state->c = emcStatusBuffer;

    for(int i=0;i<NUMCOLORS;i++) {
        state->colors[i].r = 255;
        state->colors[i].g = 255;
        state->colors[i].b = 0;
    }

    logger = new GL_PostionLogger();
    logger->initShareMem();

    updateColors();

}


GL_ViewWidget::~GL_ViewWidget() {

    state->exit = true;
    loggerthread.quit();
    loggerthread.wait();
}

void GL_ViewWidget::setMainWindow(linuxcnc *win) {

     mainwin = win;

}

void GL_ViewWidget::openNcFile(QString &s) {


    qDebug()<<"openNcFile"<<s;

}

int GL_ViewWidget::getErrorCount() {

    return errorcount;

}

void GL_ViewWidget::setNcFileName(const QString &s) {

    ncfile = s;

}

void GL_ViewWidget::reload() {

    if(ncfile.isEmpty()) {
        return;
    }

    QString parfile =  _ini_filepath;

    QFileInfo info(parfile);
    parfile = info.absolutePath();
    parfile.append("/");
    parfile.append(_ini->readEntry("PAR_RS274NGC_PARAMETER_FILE/VALUE"));

    qDebug()<<parfile;
    if(mainwin->isLoadingTextDisplay()) {
        qDebug()<<"now in isLoadingTextDisplay......................................................................";
        return;

    }
    int allcount = mainwin->getFileLineCount();

    //gcodedrawlocal;
    if(  gcodedrawlocal == nullptr )
        gcodedrawlocal = new GL_GcodeDraw(parfile,this);
    gcodedrawlocal->init();
    gcodedraw = gcodedrawlocal;
    setParsingFileLineCount(allcount);

  //  unsigned int ret =parse_file(gcodedrawlocal,ncfile.toLocal8Bit().data(),(char *)"/home/.config/subNgc-Sim/",(char *)"g21",0);
        unsigned int ret =parse_file(gcodedrawlocal,ncfile.toLocal8Bit().data(),(char *)"g21",0);

    if(ret == 0xffffffff) {
        //if(gcodedrawlocal != NULL)
        //    delete gcodedrawlocal;
        //gcodedrawlocal = NULL;
    } else {

        //if(gcodedraw != NULL) {
        //    delete gcodedraw;
        //}
        //gcodedraw = gcodedrawlocal;
        //gcodedrawlocal = NULL;
        QString resultstr = rs274_strerror(ret);
        if(!resultstr.isEmpty())
            parsingErrorInfo(resultstr);

        gcodedraw->rs274_calc_extents();
        if(!gcodedraw->isEmpty())
            updateModule(gcodedraw);
        viewP();

    }

    drawLimit();
    parsingFileLineNumber(allcount);

}

void GL_ViewWidget::zoomIn() {

    if(distance>0.005)
       distance = distance / 1.25;
    updateGL();

}

void GL_ViewWidget::zoomOut() {

     distance =  distance * 1.25;
     updateGL();

}

void GL_ViewWidget::setHighLine(int linenum) {

    int linenuml =  linenum+ 1;
    searchFromLineNum(linenuml);
    if(hasSelect()) {
        //int tmp = 0;
        ///for(int i=0;i<select_count;i++) {
        //}
    }

    updateGL();
}

void GL_ViewWidget::selectLineNum(int linenum) {

    mainwin->connectNcToModel(false);
    emit signalSelectHighLine(linenum-1);
    mainwin->connectNcToModel(true);

}

void GL_ViewWidget::parsingFileLineNumber(int linenum) {

    emit signalSetLoadingValue(linenum);

}

void GL_ViewWidget::setParsingFileLineCount(int linect) {

    parsingfilelinecount = linect;
    errorcount = 0;
    emit signalSetLoadingMax(parsingfilelinecount);

}

void GL_ViewWidget::parsingErrorInfo(QString str) {

     errorcount ++;
     emit signalErrorInfo(2,str);

}

void GL_ViewWidget::viewP() {

    GL_ViewDraw::viewP();
    updateGL();

}

void GL_ViewWidget::viewX() {

    GL_ViewDraw::viewX();
    updateGL();

}

void GL_ViewWidget::viewY() {

    GL_ViewDraw::viewY();
    updateGL();

}

void GL_ViewWidget::viewZ() {

    GL_ViewDraw::viewZ();
    updateGL();

}

void GL_ViewWidget::viewZ2() {

    GL_ViewDraw::viewZ2();
    updateGL();
}

void GL_ViewWidget::clearModel() {

    if(gcodedraw != NULL) {
        gcodedraw->traverse.clear();
        gcodedraw->dewells.clear();
        gcodedraw->feed.clear();
        gcodedraw->arcfeed.clear();
        gcodedraw->min_x = machine_limit_min[0];
        gcodedraw->min_y = machine_limit_min[1];
        gcodedraw->min_z = machine_limit_min[2];
        gcodedraw->max_x = machine_limit_max[0];
        gcodedraw->max_y = machine_limit_max[1];
        gcodedraw->max_z = machine_limit_max[2];
        updateModule(gcodedraw);
        updateGL();
    }

}

void GL_ViewWidget::abortLoad() {

    if(gcodedrawlocal != NULL) {
        gcodedrawlocal->abortParse();
    }

}

void GL_ViewWidget::debugPrint(const QString &str) {

    if(!str.isEmpty())
       printf("%s",str.toLatin1().data());

}

void GL_ViewWidget::clear() {

    logger->clear();
    update();

}




void GL_ViewWidget::initializeGL()
{
    glClearColor(0.0,0.0,0.3,0.3);
    glDrawBuffer(GL_BACK);
    glDisable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    //glBlendFunc(GL_ONE, GL_CONSTANT_ALPHA);

    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    //static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    //glewInit(); FIXME

    for(int i=0;i<3;i++) {
        minextents[i] = emcStatus->motion.axis[i].minPositionLimit;
        maxextents[i] = emcStatus->motion.axis[i].maxPositionLimit;
    }
    GL_ViewDraw::viewZ();

}

void GL_ViewWidget::drawRealtimePath() {
    GL_CanonDraw::drawRealtimePath();
    logger->loggerCall();
}

void GL_ViewWidget::updateColors(const QString &t, const QString &dft) {

    QString name = QString("OpenGL/%1").arg(t);
    QString tmp;

    if(dft.isEmpty())
       tmp = _prefer->readEntry(name,"1.0|0.0|0.0|1",true);
    else
       tmp = _prefer->readEntry(name,dft,true);

    QStringList cls = tmp.split("|");
    float c[4];
    if(cls.size() == 4) {
        c[0] = cls.at(0).toFloat();
        c[1] = cls.at(1).toFloat();
        c[2] = cls.at(2).toFloat();
        c[3] = cls.at(3).toFloat();;
        setColors(t,c);
    }

}

void GL_ViewWidget::updateColors() {


    updateColors("ColorTool","1|0|0|1");
    updateColors("ColorBackGround","0.0|0.0|0.3|0.3");
    updateColors("ColorLimit","0.0|0.5|0.5|1");
    updateColors("ColorFeed","0.0|1|1|1");
    updateColors("ColorArcFeed","0.0|1|0.6|1");
    updateColors("ColorOrigin","0.3|0.8|0.8|1");
    updateColors("ColorTraverse","0.8|0.8|0.8|1");
    updateColors("ColorSelected","1|0|0|1");
    updateColors("ColorRealTimePath","1|1|0|1");


}

void GL_ViewWidget::paintGL()
{
    //QPainter pt(this);

    static bool onece = false;
    if(!onece) {
        onece = true;
        generateToolShape();
        generateOriginShape();
        generateAxisShape();
    }
    redrawPerspective();
    //pt.setPen(QColor(255,0,0));
    //pt.drawText(QRectF(12,10,width()/2,35),Qt::AlignLeft|Qt::AlignVCenter,
    //            QString("%1 mm").arg(objdist));
    //pt.drawLine(15,38,15,42);
    //pt.drawLine(65,38,65,42);
    //pt.drawLine(15,40,65,40);
    //pt.end();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, width(), 0.0, height(), -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0,height()-80,0);
    glColor3f(1.0,0,0);
    glBegin(GL_LINES);
    glVertex3f(15, 38, 0);
    glVertex3f(15, 42, 0);
    glVertex3f(65, 38, 0);
    glVertex3f(65, 42, 0);
    glVertex3f(15, 40, 0);
    glVertex3f(65, 40, 0);
    glEnd();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    QFont font;
    font.setBold(true);
    font.setPixelSize(12);
    this->renderText(12,26,QString("%1 mm").arg(objdist),font);


}

int GL_ViewWidget::getWindowHeight() {
    return height();
}
int GL_ViewWidget::getWindowWidth() {
    return width();
}

void GL_ViewWidget::resizeGL(int width, int height)
{

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);


}

void GL_ViewWidget::wheelEvent(QWheelEvent *e) {

    QTime curtm = QTime::currentTime();
    if(lastwheeltime.msecsTo(curtm) < 250)
        sampled = true;
    lastwheeltime = curtm;

    if (e->delta() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }

}


void GL_ViewWidget::mouseReleaseEvent(QMouseEvent *event) {

    if(event->button() == Qt::LeftButton) {
        sampled = false;
        moving = false;
        updateGL();
    } else if(event->button() == Qt::MidButton) {
        sampled = false;
        moving = false;
        updateGL();
    } else if(event->button() == Qt::RightButton) {

    }

}

void GL_ViewWidget::mousePressEvent(QMouseEvent *event) {


    if(event->buttons() & Qt::LeftButton) {

       recordMouse(event->x(),event->y());
       sampled = true;
       moving = true;

    } else if(event->buttons() & Qt::MidButton) {

       recordMouse(event->x(),event->y());
       sampled = true;
       moving = true;

    } else if(event->buttons() & Qt::RightButton) {
        select(event->x(),event->y());
        updateGL();
    }

}

void GL_ViewWidget::mouseMoveEvent(QMouseEvent *event) {

    if(event->buttons() & Qt::LeftButton) {
       translate(event->x(),event->y());
       updateGL();
    }  else if(event->buttons() & Qt::MidButton) {
        rotate(event->x(),event->y());
        updateGL();
    }

}
void GL_ViewWidget::updateToolCoordinate() {

    struct logger_point *pp = logger->lastPosition();
    if(pp == NULL) return;
    tool_pos[0] = pp->x;
    tool_pos[1] = pp->y;
    tool_pos[2] = pp->z;

}

void GL_ViewWidget::timerEvent(QTimerEvent *) {
    glUpdate();
}

void GL_ViewWidget::glUpdate() {

    if (sampled && !moving ) {
       QTime curt = QTime::currentTime();
       if (lastwheeltime.msecsTo(curt) > 250) {
          sampled = false;
          updateGL();
       }
    }

    bool needupdate = false;

    static int last_pts =0;
    if(state->lpts != last_pts) {
        last_pts = state->lpts;
        needupdate = true;
    }

    static double last_machine_limit_min[4];
    static double last_machine_limit_max[4];

    double machine_limit_min[4];
    double machine_limit_max[4];
    double sec_limit_min[4];
    double sec_limit_max[4];
    double sec_limit_offset[4];

    for(int i=0;i<4;i++) {
        machine_limit_min[i] = emcStatus->motion.axis[i].minPositionLimit;
        machine_limit_max[i] = emcStatus->motion.axis[i].maxPositionLimit;
        sec_limit_max[i] = emcStatus->motion.axis[i].secPosLimit;
        sec_limit_min[i] = emcStatus->motion.axis[i].secNegLimit;
        sec_limit_offset[i] = emcStatus->motion.axis[i].seclimitOffset;

        this->sec_limit_min[i] = sec_limit_min[i];
        this->sec_limit_offset[i] = sec_limit_offset[i];
        if(i == 2) {
            this->sec_limit_max[i] = emcStatus->task.parameters[4287];
        } else
            this->sec_limit_max[i] = sec_limit_max[i];
    }
    if(memcmp(last_machine_limit_min,machine_limit_min,
              sizeof(machine_limit_min)) != 0) {
        memcpy(last_machine_limit_min,machine_limit_min,sizeof(machine_limit_min));
        for(int i=0;i<4;i++) {
            this->machine_limit_min[i] = machine_limit_min[i];
        }
        needupdate = true;
    }
    if(memcmp(last_machine_limit_max,machine_limit_max,
              sizeof(machine_limit_max)) != 0) {
        memcpy(last_machine_limit_max,machine_limit_max,sizeof(machine_limit_max));
        for(int i=0;i<4;i++) {
            this->machine_limit_max[i] = machine_limit_max[i];            
        }
        needupdate = true;
    }


    static double last_positionx;
    static double last_positiony;
    static double last_positionz;
    static double last_positionz2;

    double tmp = get_abs_pos(0);
    if(last_positionx != tmp ) {
        last_positionx = tmp;
        needupdate = true;
    }
    tmp = get_abs_pos(1);
    if(last_positiony != tmp ) {
        last_positiony = tmp;
        needupdate = true;
    }
    tmp = get_abs_pos(2);
    if(last_positionz != tmp ) {
        last_positionz = tmp;
        needupdate = true;
    }
    tmp = get_abs_pos(3);
    if(last_positionz2 != tmp ) {
        last_positionz2 = tmp;
        needupdate = true;
    }


    static unsigned char last_homed[4];
    unsigned char homed[4];
    for(int i=0;i<4;i++)
        homed[i] = emcStatus->motion.axis[i].homed;
    if(memcmp(homed,last_homed,sizeof(homed)) != 0) {
        memcpy(last_homed,homed,sizeof(homed));
        needupdate = true;
    }


    static EmcPose last_g5x_offset;
    if(memcmp(&last_g5x_offset,&emcStatus->task.g5x_offset,
              sizeof(last_g5x_offset)) != 0) {
        last_g5x_offset = emcStatus->task.g5x_offset;
        this->g5x_offset[0] = last_g5x_offset.tran.x;
        this->g5x_offset[1] = last_g5x_offset.tran.y;
        this->g5x_offset[2] = last_g5x_offset.tran.z;
        needupdate = true;

        qDebug()<<"CHANGE1111111"<<this->g5x_offset[0]<<this->g5x_offset[1];
        emit signalSetMachineZeroPos(this->g5x_offset[0],this->g5x_offset[1]);
    }
    static int last_g5x_index;
    if(last_g5x_index != emcStatus->task.g5x_index) {
        last_g5x_index = emcStatus->task.g5x_index;
        needupdate = true;
        qDebug()<<"CHANGE222222";
        emit signalSetMachineZeroPos(this->g5x_offset[0],this->g5x_offset[1]);

    }

    static EmcPose last_g92_offset;
    if(memcmp(&last_g92_offset,&emcStatus->task.g92_offset,
              sizeof(last_g92_offset)) != 0) {
        last_g92_offset = emcStatus->task.g92_offset;
        this->g92_offset[0] = last_g92_offset.tran.x;
        this->g92_offset[1] = last_g92_offset.tran.y;
        this->g92_offset[2] = last_g92_offset.tran.z;

        needupdate = true;
    }

    static double last_rotation_xy;
    if(last_rotation_xy != emcStatus->task.rotation_xy) {
        last_rotation_xy  = emcStatus->task.rotation_xy;
        needupdate = true;
    }

    static int motion_mode;
    if(motion_mode != emcStatus->motion.traj.mode) {
        motion_mode = emcStatus->motion.traj.mode;
        needupdate = true;
    }


    static int last_limit[4];
    int limit[4];
    for(int i = 0; i < 4; i++) {
        limit[i] = 0;
        if(emcStatus->motion.axis[i].minHardLimit) limit[i] |= 1;
        if(emcStatus->motion.axis[i].maxHardLimit) limit[i] |= 2;
        if(emcStatus->motion.axis[i].minSoftLimit) limit[i] |= 4;
        if(emcStatus->motion.axis[i].maxSoftLimit) limit[i] |= 8;
    }
    if(memcmp(limit,last_limit,sizeof(last_limit)) != 0) {
        memcpy(last_limit,limit,sizeof(last_limit));
        needupdate = true;
    }

    static int last_speed;
    int speed = get_actual_feed();
    if(last_speed != speed) {
        last_speed = speed;
        needupdate = true;
    }


    if(needupdate) {
        updateGL();
    }


}

void GL_ViewWidget::updateMachineZeroPos()
{
    emit signalSetMachineZeroPos(this->g5x_offset[0],this->g5x_offset[1]);
}

static const double epsilon = 1e-4; // 1-cos(1 deg) ~= 1e-4
static const double tiny = 1e-10;
static inline bool colinear(float xa, float ya, float za, float xb, float yb, float zb, float xc, float yc, float zc) {
    double dx1 = xa-xb, dx2 = xb-xc;
    double dy1 = ya-yb, dy2 = yb-yc;
    double dz1 = za-zb, dz2 = zb-zc;
    double dp = sqrt(dx1*dx1 + dy1*dy1 + dz1*dz1);
    double dq = sqrt(dx2*dx2 + dy2*dy2 + dz2*dz2);
    if( fabs(dp) < tiny || fabs(dq) < tiny ) return true;
    double dot = (dx1*dx2 + dy1*dy2 + dz1*dz2) / dp / dq;
    if( fabs(1-dot) < epsilon) return true;
    return false;
}

void GL_PostionLogger::setLoggerState(PositionLoggerContainer *s) {

    state = s;

}

struct logger_point* GL_PostionLogger::lastPosition() {


    static logger_point point;
    point.x = emc_data->lastPos.tran.x;
    point.y = emc_data->lastPos.tran.y;
    point.z = emc_data->lastPos.tran.z;
    point.rx = 0;
    point.ry = 0;
    point.rz = 0;

    return &point;

    this->mutx.lock();
    if(state->npts>0 && state->lpts>0) {
        this->mutx.unlock();
        return &state->p[state->lpts-1];
    } else {
        this->mutx.unlock();
        return NULL;
   }

}

void GL_PostionLogger::clear() {

    base_index = emc_data->index;

}

void GL_PostionLogger::loggerCall() {

    int size = EMCMOT_MAX_SAVEPOS;
    int draw_size = OPENGL_DRAW_SIZE;

    int act_draw_size = 0;
    int start,end;
    int act_start;
    int need_cut = 0;


    end = emc_data->index;
    start = ( ( end - draw_size ) + size ) %size;

    if( start < end) { //normal

        if( base_index > start && base_index<=end) {
            act_start = base_index;
        } else {
            base_index = start;
            act_start = start;
        }

        act_draw_size = end-act_start;
        memcpy(local_pos,&emc_data->setpos[act_start],act_draw_size*sizeof(EmcPose));

    } else {  //tran

         if( base_index >= start && base_index <= (size-1) ) { //end
            act_start = base_index;
        } else if( base_index >= 0 && base_index <= end ) {
            need_cut = 1;
            act_start = base_index;
        } else {
            base_index = start;
            act_start = start;
        }

        if( need_cut ) {

            act_draw_size = end-act_start;
            memcpy(local_pos,&emc_data->setpos[act_start],act_draw_size*sizeof(EmcPose));

        } else {

            act_draw_size = size-act_start;
            memcpy(local_pos,&emc_data->setpos[act_start],act_draw_size*sizeof(EmcPose));
            memcpy(local_pos + act_draw_size,&emc_data->setpos[0],\
                (end)*sizeof(EmcPose));
            act_draw_size += (end);

        }

    }

    //static bool once = false;
    //qDebug()<<"act_draw_size"<<start<<end<<"base_in"<<base_index\
    //<<"act_start"<<act_start<<act_draw_size;

    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE,
                sizeof( EmcPose), &local_pos[0].tran.x);
    glLineWidth(2.3);
    glDrawArrays(GL_LINE_STRIP, 0, act_draw_size);
    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisable(GL_BLEND);
    glLineWidth(1);



    return;

    if(!state->clear) {
        mutx.lock();
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnableClientState(GL_VERTEX_ARRAY);
        if(state->changed) state->changed = 0;
            glVertexPointer(3, GL_FLOAT,
                    sizeof(struct logger_point), &state->p->x);  
        state->lpts = state->npts;
        glLineWidth(2.3);
        glDrawArrays(GL_LINE_STRIP, 0, state->npts);
        glDisableClientState(GL_VERTEX_ARRAY);
        //glDisable(GL_BLEND);
        glLineWidth(1);

        mutx.unlock();
    }

}



#define SHM_PERMISSIONS	0666
GL_PostionLogger::GL_PostionLogger()
    :QObject(0) {

    base_index = 0;
    local_pos = (EmcPose*)malloc(OPENGL_DRAW_SIZE*sizeof(EmcPose));

}


int GL_PostionLogger::initShareMem() {


    // try to attach
    emc_data = NULL;
    key_t key = 54321;
    int size = sizeof(emcdata_struct);
    int is_new = 0;
    int ret;
    struct shmid_ds d;

    int id = shmget(key, size, SHM_PERMISSIONS);

    if (id == -1) {
        if (errno == ENOENT) {
            id = shmget(key, size, SHM_PERMISSIONS | IPC_CREAT);
            is_new = 1;
        }
        if (id == -1) {
            return -ENOMEM;
        }
    }


    if ((ret = shmctl(id, IPC_STAT, &d)) < 0) {
        printf("GL_PostionLogger: shm_ctl(key=0x%x, IPC_STAT) "
            "failed: %d '%s'\n",
            key, errno, strerror(errno));
    } else {
    // drop permissions of shmseg to real userid/group id
    if (!d.shm_perm.uid) { // uh, root perms
        d.shm_perm.uid = getuid();
        d.shm_perm.gid = getgid();
        if ((ret = shmctl(id, IPC_SET, &d)) < 0) {
            printf( "GL_PostionLogger: shm_ctl(key=0x%x, IPC_SET) "
                "failed: %d '%s'\n",
                key, errno, strerror(errno));
        }
    }
    }
    /* and map it into process space */
    shmem = shmat(id, 0, 0);
    if ((ssize_t) (shmem) == -1) {

        printf("GL_PostionLogger: shmat(%d) failed: %d '%s'\n",
                id, errno, strerror(errno));
    return -errno;
    }
    /* Touch each page by either zeroing the whole mem (if it's a new
       SHM region), or by reading from it. */
        if (is_new) {
        memset(shmem, 0, size);
        } else {
         int i, pagesize;

        pagesize = sysconf(_SC_PAGESIZE);
        for (i = 0; i < size; i += pagesize) {
            unsigned int x = *(volatile unsigned int *)
            ((unsigned char *)shmem + i);
            /* Use rand_r to clobber the read so GCC won't optimize it
               out. */
            rand_r(&x);
        }
    }

     emc_data = (emcdata_struct *)shmem;

    return 0;

}

void GL_PostionLogger::doLogger() {

}

