#include "gl_gcodedraw.h"
#include "gl_canondraw.h"

#include "GL/glew.h"

#include <QList>
#include <QDebug>
#include <math.h>

static double objx1,objy1,objz1;
static double objx2,objy2,objz2;

float GL_CanonDraw::color_background[4] = {0,0,1,1};
float GL_CanonDraw::color_tool[4] = {0,0,1,1};
float GL_CanonDraw::color_limit[4] = {0,0,1,1};
float GL_CanonDraw::color_origin[4] = {0,0,1,1};
float GL_CanonDraw::color_feed[4] = {0,0,1,1};
float GL_CanonDraw::color_arcfeed[4] = {0,0,1,1};
float GL_CanonDraw::color_traverse[4] = {0,0,1,1};
float GL_CanonDraw::color_selected[4] = {0,0,1,1};
float GL_CanonDraw::color_rtpath[4] = {0,0,1,1};

GL_CanonDraw::GL_CanonDraw() {

    machine_limit_min[0] = 0;
    machine_limit_min[1] = 0;
    machine_limit_min[2] = 0;

    machine_limit_max[0] = 0;
    machine_limit_max[1] = 0;
    machine_limit_max[2] = 0;

    g5x_offset[0] = 0;
    g5x_offset[1] = 0;
    g5x_offset[2] = 0;

    g92_offset[0] = 0;
    g92_offset[1] = 0;
    g92_offset[2] = 0;

    rotation_xy = 0;

    feedsize = 0;
    arcfeedsize = 0;
    dewellssize = 0;
    traversesize = 0;

    vectors_feed = NULL;
    vectors_arcfeed = NULL;
    vectors_dwells = NULL;
    vectors_traverse = NULL;

    gcodedraw = NULL;
    sampled_num = 5000;
    sampled = false;
    moving = false;


    has_select = false;
    vectors_select = new float[6];
    select_count = 1;


}

void GL_CanonDraw::canonDraw() {

    //machine_limit_min, machine_limit_max = self.soft_limits()

    glDisable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);

    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

    GLdouble obj0[3];
    GLdouble obj1[3];
    gluUnProject(0,0,0,
                 modelMatrix,projMatrix,viewport,
                 &obj0[0],&obj0[1],&obj0[2]);
    gluUnProject(50,0,0,
                 modelMatrix,projMatrix,viewport,
                 &obj1[0],&obj1[1],&obj1[2]);


    objdist = sqrt( (obj0[0]-obj1[0])*(obj0[0]-obj1[0]) +
                    (obj0[1]-obj1[1])*(obj0[1]-obj1[1]) +
                    (obj0[2]-obj1[2])*(obj0[2]-obj1[2]) );
    drawAxis();
    drawLimit();
    drawOrigin();
    drawModelList(sampled);
    drawRealtimePath();
    updateToolCoordinate();
    drawTool();
    drawSelect();
}

void GL_CanonDraw::drawRealtimePath() {
    glColor4fv(color_rtpath);
}

void GL_CanonDraw::drawSelect() {


    if(!has_select) return;

//    qDebug()<<vectors_select[0]<<vectors_select[1]<<vectors_select[2]<<vectors_select[3]<<vectors_select[4]<<vectors_select[5]<<select_count;
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    glColor3fv(color_selected);
    glLineWidth(3);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT,0, vectors_select);
    glDrawArrays(GL_LINES, 0, select_count*2);
    glDisableClientState(GL_VERTEX_ARRAY);
    glLineWidth(1);

}

void GL_CanonDraw::drawModelList(bool sampled) {

    glColor3fv(color_traverse);
    if(sampled)
        glCallList(listId_traverse_sampled);
    else
        glCallList(listId_traverse);


    glColor3fv(color_feed);
    if(sampled)
        glCallList(listId_feed_sampled);
    else
        glCallList(listId_feed);


    glColor3fv(color_arcfeed);
    if(sampled)
        glCallList(listId_arcfeed_sampled);
    else
        glCallList(listId_arcfeed);

    glColor4f(1,0,0,1);
    if(sampled)
        glCallList(listId_dewells_sampled);
    else
        glCallList(listId_dewells);

}

void GL_CanonDraw::drawModel() {

    // bind VBOs with IDs and set the buffer offsets of the bound VBOs
    // When buffer object is bound with its ID, all pointers in gl*Pointer()
    // are treated as offset instead of real pointer.



    if(traversesize+dewellssize+arcfeedsize+feedsize) {

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, 0);

        glColor4f(0.9,0.8,0.8,0.5);
        if(traversesize) {
            glDrawArrays(GL_LINES, 0, traversesize*2*3);
        }


        glColor4f(0.6,0,1,0.5);
        if(feedsize) {
            glDrawArrays(GL_LINES, traversesize*2*3, feedsize*2*3);
        }

        glColor4f(0.6,0,1,0.5);
        if(arcfeedsize) {
            glDrawArrays(GL_LINES,feedsize*2*3 + traversesize*2*3, arcfeedsize*2*3);
        }

        glColor4f(1,0,0,1);
        if(dewellssize) {
            glDrawArrays(GL_POINTS,arcfeedsize*2*3 +feedsize*2*3 + traversesize*2*3, dewellssize*3);
        }

        glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

    }




}

void GL_CanonDraw::generateToolShape() {

    listId_tool = glGenLists(1);
    GLUquadric* q = gluNewQuadric();
    glNewList(listId_tool, GL_COMPILE);
    gluCylinder(q, .01, .01, 0.10, 32, 32);
    glPushMatrix();
    glTranslatef(0,0,.10);
    gluDisk(q, 0, .01, 32, 1);
    glPopMatrix();
    glEndList();
    gluDeleteQuadric(q);

}

void GL_CanonDraw::drawTool() {

    glColor3fv(color_tool);
    glPushMatrix();
    glTranslatef(tool_pos[0],tool_pos[1],tool_pos[2]);
    glRotatef(90, 0, 0, 1);
    glScalef(objdist*5.5,objdist*5.5,objdist*5.5);
    glCallList(listId_tool);
    glPopMatrix();

}

void GL_CanonDraw::generateOriginShape() {

    listId_origin = glGenLists(1);
    GLUquadric* q = gluNewQuadric();
    glNewList(listId_origin, GL_COMPILE);
    gluSphere(q,0.1,16,16);
    glEndList();
    gluDeleteQuadric(q);

}

void GL_CanonDraw::drawOrigin() {

    glPushMatrix();
    glColor3fv(color_origin);
    glLineStipple(1, 0x1111);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3fv(g5x_offset);
    glEnd();
    glTranslatef(g5x_offset[0],g5x_offset[1],g5x_offset[2]);

    glRotatef(rotation_xy, 0, 0, 1);

    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3fv(g92_offset);
    glEnd();

    glTranslatef(g92_offset[0],g92_offset[1],g92_offset[2]);

    glPushMatrix();
    glScalef(objdist,objdist,objdist);
    glCallList(listId_origin);
    glPopMatrix();
    glPopMatrix();
    glDisable(GL_LINE_STIPPLE);
}

void GL_CanonDraw::drawLimit() {

    glLineWidth(1);
    glColor3fv(color_limit);
    glLineStipple(1, 0x1111);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);

    glVertex3f(machine_limit_min[0], machine_limit_min[1], machine_limit_max[2]);
    glVertex3f(machine_limit_min[0], machine_limit_min[1], machine_limit_min[2]);

    glVertex3f(machine_limit_min[0], machine_limit_min[1], machine_limit_min[2]);
    glVertex3f(machine_limit_min[0], machine_limit_max[1], machine_limit_min[2]);

    glVertex3f(machine_limit_min[0], machine_limit_max[1], machine_limit_min[2]);
    glVertex3f(machine_limit_min[0], machine_limit_max[1], machine_limit_max[2]);

    glVertex3f(machine_limit_min[0], machine_limit_max[1], machine_limit_max[2]);
    glVertex3f(machine_limit_min[0], machine_limit_min[1], machine_limit_max[2]);


    glVertex3f(machine_limit_max[0], machine_limit_min[1], machine_limit_max[2]);
    glVertex3f(machine_limit_max[0], machine_limit_min[1], machine_limit_min[2]);

    glVertex3f(machine_limit_max[0], machine_limit_min[1], machine_limit_min[2]);
    glVertex3f(machine_limit_max[0], machine_limit_max[1], machine_limit_min[2]);

    glVertex3f(machine_limit_max[0], machine_limit_max[1], machine_limit_min[2]);
    glVertex3f(machine_limit_max[0], machine_limit_max[1], machine_limit_max[2]);

    glVertex3f(machine_limit_max[0], machine_limit_max[1], machine_limit_max[2]);
    glVertex3f(machine_limit_max[0], machine_limit_min[1], machine_limit_max[2]);


    glVertex3f(machine_limit_min[0], machine_limit_min[1], machine_limit_min[2]);
    glVertex3f(machine_limit_max[0], machine_limit_min[1], machine_limit_min[2]);

    glVertex3f(machine_limit_min[0], machine_limit_max[1], machine_limit_min[2]);
    glVertex3f(machine_limit_max[0], machine_limit_max[1], machine_limit_min[2]);

    glVertex3f(machine_limit_min[0], machine_limit_max[1], machine_limit_max[2]);
    glVertex3f(machine_limit_max[0], machine_limit_max[1], machine_limit_max[2]);

    glVertex3f(machine_limit_min[0], machine_limit_min[1], machine_limit_max[2]);
    glVertex3f(machine_limit_max[0], machine_limit_min[1], machine_limit_max[2]);


    if(fabs(sec_limit_max[2]) > 0.001) {
        //left view
        glVertex3f(sec_limit_min[0], sec_limit_min[1], sec_limit_max[2]);
        glVertex3f(sec_limit_min[0], sec_limit_min[1], machine_limit_min[2]);

        glVertex3f(sec_limit_min[0], sec_limit_min[1], machine_limit_min[2]);
        glVertex3f(sec_limit_min[0], sec_limit_max[1], machine_limit_min[2]);

        glVertex3f(sec_limit_min[0], sec_limit_max[1], machine_limit_min[2]);
        glVertex3f(sec_limit_min[0], sec_limit_max[1], sec_limit_max[2]);

        glVertex3f(sec_limit_min[0], sec_limit_max[1], sec_limit_max[2]);
        glVertex3f(sec_limit_min[0], sec_limit_min[1], sec_limit_max[2]);
        //right view
        glVertex3f(sec_limit_max[0], sec_limit_min[1], sec_limit_max[2]);
        glVertex3f(sec_limit_max[0], sec_limit_min[1], machine_limit_min[2]);

        glVertex3f(sec_limit_max[0], sec_limit_min[1], machine_limit_min[2]);
        glVertex3f(sec_limit_max[0], sec_limit_max[1], machine_limit_min[2]);

        glVertex3f(sec_limit_max[0], sec_limit_max[1], machine_limit_min[2]);
        glVertex3f(sec_limit_max[0], sec_limit_max[1], sec_limit_max[2]);

        glVertex3f(sec_limit_max[0], sec_limit_max[1], sec_limit_max[2]);
        glVertex3f(sec_limit_max[0], sec_limit_min[1], sec_limit_max[2]);
        // other line
        glVertex3f(sec_limit_min[0], sec_limit_min[1], machine_limit_min[2]);
        glVertex3f(sec_limit_max[0], sec_limit_min[1], machine_limit_min[2]);

        glVertex3f(sec_limit_min[0], sec_limit_max[1], machine_limit_min[2]);
        glVertex3f(sec_limit_max[0], sec_limit_max[1], machine_limit_min[2]);

        glVertex3f(sec_limit_min[0], sec_limit_max[1], sec_limit_max[2]);
        glVertex3f(sec_limit_max[0], sec_limit_max[1], sec_limit_max[2]);

        glVertex3f(sec_limit_min[0], sec_limit_min[1], sec_limit_max[2]);
        glVertex3f(sec_limit_max[0], sec_limit_min[1], sec_limit_max[2]);


        /************************************************/
        //left view
        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], sec_limit_max[2]);
        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], machine_limit_min[2]);

        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], machine_limit_min[2]);
        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], machine_limit_min[2]);

        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], machine_limit_min[2]);
        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], sec_limit_max[2]);

        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], sec_limit_max[2]);
        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], sec_limit_max[2]);
        //right view
        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], sec_limit_max[2]);
        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], machine_limit_min[2]);

        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], machine_limit_min[2]);
        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], machine_limit_min[2]);

        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], machine_limit_min[2]);
        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], sec_limit_max[2]);

        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], sec_limit_max[2]);
        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], sec_limit_max[2]);
        // other line
        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], machine_limit_min[2]);
        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], machine_limit_min[2]);

        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], machine_limit_min[2]);
        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], machine_limit_min[2]);

        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], sec_limit_max[2]);
        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_max[1]-sec_limit_offset[1], sec_limit_max[2]);

        glVertex3f(sec_limit_min[0]+sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], sec_limit_max[2]);
        glVertex3f(sec_limit_max[0]-sec_limit_offset[0], sec_limit_min[1]+sec_limit_offset[1], sec_limit_max[2]);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x5555);

}

void GL_CanonDraw::generateAxisShape() {

    listId_axis = glGenLists(1);
    GLUquadric* q = gluNewQuadric();
    glNewList(listId_axis, GL_COMPILE);

    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glEnd();

    glPushMatrix();
    glTranslatef(0.76, 0.0, 0.0);
    glRotatef(90.0,0.0,1.0,0.0);
    gluCylinder(q, 0.1, 0.0, .35, 32, 1);
    glRotatef(-180,0.0,1.0,0.0);
    gluDisk(q, 0, .1, 32, 1);
    glPopMatrix();


    glColor3f(0,1,0);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,1.0,0.0);
    glEnd();

    glPushMatrix();
    glTranslatef(0.0, 0.76, 0.0);
    glRotatef(-90.0,1.0,0.0,0.0);
    gluCylinder(q, 0.1, 0.0, .35, 32, 1);
    glRotatef(180,1.0,0.0,0.0);
    gluDisk(q, 0, .1, 32, 1);
    glPopMatrix();

    glColor3f(0,0,1);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,0.0,1.0);
    glEnd();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.76);
    gluCylinder(q, 0.1, 0.0, .35, 32, 1);
    glRotatef(-180,0.0,0.0,1.0);
    gluDisk(q, 0, .1, 32, 1);
    glPopMatrix();


    glEndList();
    gluDeleteQuadric(q);

}

void GL_CanonDraw::drawAxis() {


    glPushMatrix();
    glScalef(objdist,objdist,objdist);
    glCallList(listId_axis);
    glPopMatrix();

}

void GL_CanonDraw::updateToolCoordinate() {

}

int GL_CanonDraw::getWindowHeight() {
    return 0;
}

int GL_CanonDraw::getWindowWidth() {
    return 0;
}

void GL_CanonDraw::selectLineNum(int) {

}


double GL_CanonDraw::pointToLine(double *p1, double *p2,
                   double *p3) {

    double a[3],b[3];
    double c[3],ab[3];
    double up,down;
    a[0] = p1[0]-p2[0];
    a[1] = p1[1]-p2[1];
    a[2] = p1[2]-p2[2];

    b[0] = p1[0]-p3[0];
    b[1] = p1[1]-p3[1];
    b[2] = p1[2]-p3[2];

    c[0] = p3[0]-p2[0];
    c[1] = p3[1]-p2[1];
    c[2] = p3[2]-p2[2];

    ab[0] = a[1]*b[2]-b[1]*a[2];
    ab[1] = a[2]*b[0]-b[2]*a[0];
    ab[2] = a[0]*b[1]-b[0]*a[1];

    up = sqrt(ab[0]*ab[0]+ab[1]*ab[1]+ab[2]*ab[2]);
    down = sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]);

    if(down == 0.0)
        return -5;
    return fabs(up/down);

}

void GL_CanonDraw::updateSelectBuffer(QList<GL_GcodeElement> *els,
                                      int index) {

    if(index < 0 || index >= els->size()) return;
    int start = index,end = index;
    int num = els->at(index).linenum;

    for(int i=index+1;i<els->size();i++) {
        int getnum = els->at(i).linenum;
        if((getnum!=num)) {
            end = i-1;
            break;
        }
        if((i+1)==els->size()) {
            end = i;
        }
    }

    for(int i=index-1; i>=0; i--) {

        int getnum = els->at(i).linenum;
        if((getnum!=num)) {
            start = i+1;
            break;
        }
        if(i == 0) {
            start = i;
        }
    }

    updateSelectBuffer(els,start,end);


}


void GL_CanonDraw::updateSelectBuffer(QList<GL_GcodeElement> *els,
                                      int start, int end) {


    if(start < 0 || start >= els->size()) return;
    if(end < 0 || end >= els->size()) return;
    if(start > end) return;
    int count = end-start+1;

    if(count != select_count) {
        delete vectors_select;
        vectors_select = new float[count*6];
        select_count = count;
    }


    int indexi = 0;
    for(int i=start;i<=end;i++) {

        GL_GcodeElement el = els->at(i);
        vectors_select[indexi*6] = el.start.x;
        vectors_select[indexi*6+1] = el.start.y;
        vectors_select[indexi*6+2] = el.start.z;
        vectors_select[indexi*6+3] = el.end.x;
        vectors_select[indexi*6+4] = el.end.y;
        vectors_select[indexi*6+5] = el.end.z;

        //printf("%f %f %f %f %f %f\n",vectors_select[indexi*6],vectors_select[indexi*6+1],
        //       vectors_select[indexi*6+2],vectors_select[indexi*6+3],
        //       vectors_select[indexi*6+4],vectors_select[indexi*6+5]);
        indexi ++;
    }
    has_select = true;

}

bool GL_CanonDraw::hasSelect() {
    return has_select;
}

void GL_CanonDraw::searchFromLineNum(int num) {

    if(gcodedraw == NULL) return;

    //qDebug()<<"searchFromLineNum"<<num;
    QList< GL_GcodeElement > *els = &gcodedraw->feed;
    int start = -1,end = -1;
    bool trigle = false;

    for(int i = 0;i<els->size();i++) {
        int getnum = els->at(i).linenum;

        if(getnum == num && !trigle) {
            start = i;
            trigle = true;
        }
        if((trigle && getnum!=num)) {
            end = i-1;
            break;
        }
        if((trigle && (i+1)==els->size())) {
            end = i;
        }
    }
    if(start != -1 && end !=-1) {
        updateSelectBuffer(els,start,end);
        return;
    }

    els = &gcodedraw->arcfeed;
    start = -1;
    end = -1;
    trigle = false;

    for(int i = 0;i<els->size();i++) {
        int getnum = els->at(i).linenum;

        if(getnum == num && !trigle) {
            start = i;
            trigle = true;
        }
        if((trigle && getnum!=num)) {
            end = i-1;
            break;
        }
        if((trigle && (i+1)==els->size())) {
            end = i;
        }
    }
    if(start != -1 && end !=-1) {
        updateSelectBuffer(els,start,end);
        return;
    }

    els = &gcodedraw->traverse;
    start = -1;
    end = -1;
    trigle = false;

    for(int i = 0;i<els->size();i++) {
        int getnum = els->at(i).linenum;
        if(getnum == num && !trigle) {
            start = i;
            trigle = true;
        }
        if((trigle && getnum!=num)) {
            end = i-1;
            break;
        }
        if((trigle && (i+1)==els->size())) {
            end = i;
        }
    }
    if(start != -1 && end !=-1) {
        updateSelectBuffer(els,start,end);
        return;
    }


    if(start == -1 || end ==-1) has_select = false;

}

void GL_CanonDraw::select(int x,int y) {

    if(gcodedraw == NULL) return;

    double mvmatrix[16];
    double projmatrix[16];
    int viewport[4];
    double dClickY;


    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
    dClickY = double (getWindowHeight() - y);

    gluUnProject((double) x, dClickY, 0.0, mvmatrix, projmatrix, viewport, &objx1, &objy1, &objz1);
    gluUnProject((double) x, dClickY, 1.0, mvmatrix, projmatrix, viewport, &objx2, &objy2, &objz2);


    double x0[3],x1[3],x2[3],x3[3],mid[3];
    double tmp,angl,cmp,linelen;
    double actdist = objdist/2.6;
    double mergethrould = objdist/6.0;

    int record_feed = -1;
    int record_arcfeed = -1;
    int record_travers = -1;
    double dist_feed = -1;
    double dist_arcfeed = -1;
    double dist_travers = -1;


    x0[0] = objx1; x0[1] = objy1; x0[2] = objz1;
    x1[0] = objx2; x1[1] = objy2; x1[2] = objz2;

    GL_GcodeElement el(GL_GcodeElement::ElementFeed);

    for(int i = 0;i<gcodedraw->feed.size();i++) {
        el = gcodedraw->feed.at(i);
        x2[0] = el.start.x;
        x2[1] = el.start.y;
        x2[2] = el.start.z;
        x3[0] = el.end.x;
        x3[1] = el.end.y;
        x3[2] = el.end.z;

        linelen = lineLength(x2,x3);
        if( linelen <= mergethrould) {
            angl = -1;
            mid[0] = (x2[0] + x3[0])/2;
            mid[1] = (x2[1] + x3[1])/2;
            mid[2] = (x2[2] + x3[2])/2;
            tmp = pointToLine(mid,x0,x1);
        } else {
            tmp = twoLinesDistance(x0,x1,x2,x3);
            angl = twoPointAngleToLine(x0,x1,x2,x3);
        }
        cmp = fabs( angl + 1 );
        if( tmp <= actdist && (cmp>=0 && cmp<=0.18)) {
            record_feed = i;
            dist_feed = tmp;
            break;
        }
    }

    for(int i = 0;i<gcodedraw->arcfeed.size();i++) {
        el = gcodedraw->arcfeed.at(i);
        x2[0] = el.start.x;
        x2[1] = el.start.y;
        x2[2] = el.start.z;
        x3[0] = el.end.x;
        x3[1] = el.end.y;
        x3[2] = el.end.z;
        linelen = lineLength(x2,x3);
        if( linelen <= mergethrould) {
            angl = -1;
            mid[0] = (x2[0] + x3[0])/2;
            mid[1] = (x2[1] + x3[1])/2;
            mid[2] = (x2[2] + x3[2])/2;
            tmp = pointToLine(mid,x0,x1);
        } else {
            tmp = twoLinesDistance(x0,x1,x2,x3);
            angl = twoPointAngleToLine(x0,x1,x2,x3);
        }
        cmp = fabs( angl + 1 );
        if( tmp <= actdist && (cmp>=0 && cmp<=0.18)) {
            record_arcfeed = i;
            dist_arcfeed = tmp;
            break;
        }
    }

    for(int i = 0;i<gcodedraw->traverse.size();i++) {
        el = gcodedraw->traverse.at(i);
        x2[0] = el.start.x;
        x2[1] = el.start.y;
        x2[2] = el.start.z;
        x3[0] = el.end.x;
        x3[1] = el.end.y;
        x3[2] = el.end.z;
        linelen = lineLength(x2,x3);
        if( linelen <= mergethrould) {
            angl = -1;
            mid[0] = (x2[0] + x3[0])/2;
            mid[1] = (x2[1] + x3[1])/2;
            mid[2] = (x2[2] + x3[2])/2;
            tmp = pointToLine(mid,x0,x1);
        } else {
            tmp = twoLinesDistance(x0,x1,x2,x3);
            angl = twoPointAngleToLine(x0,x1,x2,x3);
        }
        cmp = fabs( angl + 1 );
        if( tmp <= actdist && (cmp>=0 && cmp<=0.18)) {
            record_travers = i;
            dist_travers = tmp;
            break;
        }
    }


    if( record_feed>=0 || record_arcfeed>=0 || record_travers>=0) {

        double finaldist = -1;
        QString strid = "";

        if( dist_feed > -1 ) {
            el = gcodedraw->feed.at(record_feed);
            finaldist = dist_feed;
            strid = "feed";
        }

        if( dist_arcfeed > -1 ) {
            if(finaldist == -1) {
                finaldist = dist_arcfeed;
                el = gcodedraw->arcfeed.at(record_arcfeed);
                strid = "arcfeed";
            } else {
                if(dist_arcfeed < finaldist) {
                    finaldist = dist_arcfeed;
                    el = gcodedraw->arcfeed.at(record_arcfeed);
                    strid = "arcfeed";
                }
            }
        }

        if( dist_travers > -1 ) {
            if(finaldist == -1) {
                finaldist = dist_travers;
                el = gcodedraw->traverse.at(record_travers);
                strid = "traverse";
            } else {
                if(dist_travers < finaldist) {
                    finaldist = dist_travers;
                    el = gcodedraw->traverse.at(record_travers);
                    strid = "traverse";
                }
            }
        }


        if(strid == "traverse") {
            updateSelectBuffer(&gcodedraw->traverse,record_travers);
        } else if(strid == "arcfeed") {
            updateSelectBuffer(&gcodedraw->arcfeed,record_arcfeed);
        } else if(strid == "feed") {
            updateSelectBuffer(&gcodedraw->feed,record_feed);

        }
        selectLineNum(el.linenum);

    } else
        has_select = false;

}

double GL_CanonDraw::lineLength(double *p1, double *p2) {

    return sqrt( (p1[0]-p2[0])*(p1[0]-p2[0]) + (p1[1]-p2[1])*(p1[1]-p2[1]) +(p1[2]-p2[2])*(p1[2]-p2[2]) );

}

double GL_CanonDraw::twoLinesDistance(double *p1, double *p2,
                                      double *p3, double *p4) {

    double a[3],b[3];
    double c[3],ab[3];
    double up,down;
    a[0] = p2[0]-p1[0];
    a[1] = p2[1]-p1[1];
    a[2] = p2[2]-p1[2];

    b[0] = p4[0]-p3[0];
    b[1] = p4[1]-p3[1];
    b[2] = p4[2]-p3[2];

    c[0] = p3[0]-p1[0];
    c[1] = p3[1]-p1[1];
    c[2] = p3[2]-p1[2];

    //设a=(X1,Y1,Z1),b=(X2,Y2,Z2),
    //a×b=（Y1Z2-Y2Z1，Z1X2-Z2X1，X1Y2-X2Y1）
    ab[0] = a[1]*b[2]-b[1]*a[2];
    ab[1] = a[2]*b[0]-b[2]*a[0];
    ab[2] = a[0]*b[1]-b[0]*a[1];


    up = c[0]*ab[0]+c[1]*ab[1]+c[2]*ab[2];
    down = sqrt(ab[0]*ab[0]+ab[1]*ab[1]+ab[2]*ab[2]);

    if(down == 0.0)
        return -5;

    return fabs(up/down);

}

void GL_CanonDraw::flushModelList(GL_GcodeDraw *m) {


    int sampled_route = 1;
    static bool once = false;
    if(!once) {
        listId_traverse = glGenLists(1);
        listId_feed = glGenLists(1);
        listId_arcfeed = glGenLists(1);
        listId_dewells = glGenLists(1);

        listId_traverse_sampled = glGenLists(1);
        listId_feed_sampled = glGenLists(1);
        listId_arcfeed_sampled = glGenLists(1);
        listId_dewells_sampled = glGenLists(1);

    }

    feedsize = m->feed.size();
    arcfeedsize = m->arcfeed.size();
    dewellssize = m->dewells.size();
    traversesize = m->traverse.size();


    //*******************feed******************************/
    glNewList(listId_feed, GL_COMPILE);
    glBegin(GL_LINES);
    for(unsigned int i = 0;i<feedsize;i++) {

        GL_GcodeElement el = m->feed.at(i);
        glVertex3f(el.start.x,el.start.y,el.start.z);
        glVertex3f(el.end.x,el.end.y,el.end.z);

    }
    glEnd();
    glEndList();


    glNewList(listId_feed_sampled, GL_COMPILE);
    glBegin(GL_LINES);
    sampled_route = 1;
    while ((feedsize / sampled_route) > sampled_num)
                sampled_route ++;
    for(unsigned int i = 0;i<feedsize;i++) {
        if((i+1)%sampled_route == 0) {
        GL_GcodeElement el = m->feed.at(i);
        glVertex3f(el.start.x,el.start.y,el.start.z);
        glVertex3f(el.end.x,el.end.y,el.end.z);
        }
    }
    glEnd();
    glEndList();


    //*******************arcfeed******************************/
    glNewList(listId_arcfeed, GL_COMPILE);
    glBegin(GL_LINES);
    for(unsigned int i = 0;i<arcfeedsize;i++) {

        GL_GcodeElement el = m->arcfeed.at(i);
        glVertex3f(el.start.x,el.start.y,el.start.z);
        glVertex3f(el.end.x,el.end.y,el.end.z);

    }
    glEnd();
    glEndList();

    glNewList(listId_arcfeed_sampled, GL_COMPILE);
    glBegin(GL_LINES);
    sampled_route = 1;
    while ((arcfeedsize / sampled_route) > sampled_num)
                sampled_route ++;
    for(unsigned int i = 0;i<arcfeedsize;i++) {
        if((i+1)%sampled_route == 0) {
        GL_GcodeElement el = m->arcfeed.at(i);
        glVertex3f(el.start.x,el.start.y,el.start.z);
        glVertex3f(el.end.x,el.end.y,el.end.z);
        }
    }
    glEnd();
    glEndList();

    //*******************dewell******************************/
    glNewList(listId_dewells, GL_COMPILE);
    glBegin(GL_LINES);
    for(unsigned int i = 0;i<dewellssize;i++) {

        GL_GcodeElement el = m->dewells.at(i);
        glVertex3f(el.start.x,el.start.y,el.start.z);
        glVertex3f(el.end.x,el.end.y,el.end.z);

    }
    glEnd();
    glEndList();

    glNewList(listId_dewells_sampled, GL_COMPILE);
    glBegin(GL_LINES);
    sampled_route = 1;
    while ((dewellssize / sampled_route) > sampled_num)
                sampled_route ++;
    for(unsigned int i = 0;i<dewellssize;i++) {
        if((i+1)%sampled_route == 0) {
        GL_GcodeElement el = m->dewells.at(i);
        glVertex3f(el.start.x,el.start.y,el.start.z);
        glVertex3f(el.end.x,el.end.y,el.end.z);
        }
    }
    glEnd();
    glEndList();
    //*******************traverse******************************/
    glNewList(listId_traverse, GL_COMPILE);
    glBegin(GL_LINES);
    for(unsigned int i = 0;i<traversesize;i++) {

        GL_GcodeElement el = m->traverse.at(i);
        glVertex3f(el.start.x,el.start.y,el.start.z);
        glVertex3f(el.end.x,el.end.y,el.end.z);

    }
    glEnd();
    glEndList();

    glNewList(listId_traverse_sampled, GL_COMPILE);
    glBegin(GL_LINES);
    sampled_route = 1;
    while ((traversesize / sampled_route) > sampled_num)
                sampled_route ++;
    for(unsigned int i = 0;i<traversesize;i++) {
        if((i+1)%sampled_route == 0) {
        GL_GcodeElement el = m->traverse.at(i);
        glVertex3f(el.start.x,el.start.y,el.start.z);
        glVertex3f(el.end.x,el.end.y,el.end.z);
        }
    }
    glEnd();
    glEndList();

}

void GL_CanonDraw::flushModel(GL_GcodeDraw *m) {




    if(vectors_feed != NULL) {
        delete vectors_feed;
        vectors_feed = NULL;

    }
    if(vectors_arcfeed != NULL) {
        delete vectors_arcfeed;
        vectors_arcfeed = NULL;

    }
    if(vectors_dwells != NULL) {
        delete vectors_dwells;
        vectors_dwells = NULL;

    }
    if(vectors_traverse != NULL) {
        delete vectors_traverse;
        vectors_traverse = NULL;

    }


    if(vectors_traverse != NULL || vectors_dwells!=NULL||
            vectors_arcfeed != NULL || vectors_feed!=NULL)
        glDeleteBuffersARB(1,&vboId);


    feedsize = m->feed.size();
    arcfeedsize = m->arcfeed.size();
    dewellssize = m->dewells.size();
    traversesize = m->traverse.size();



    if( feedsize )
        vectors_feed = new float[feedsize*2*3+3];
    for(unsigned int i = 0;i<feedsize;i++) {

        GL_GcodeElement el = m->feed.at(i);
        vectors_feed[i*6] = el.start.x;
        vectors_feed[i*6+1] = el.start.y;
        vectors_feed[i*6+2] = el.start.z;
        vectors_feed[i*6+3] = el.end.x;
        vectors_feed[i*6+4] = el.end.y;
        vectors_feed[i*6+5] = el.end.z;

    }


    //*******************arcfeed******************************/
    if( arcfeedsize )
        vectors_arcfeed = new float[arcfeedsize*2*3+3];
    for(unsigned int i = 0;i<arcfeedsize;i++) {

        GL_GcodeElement el = m->arcfeed.at(i);
        vectors_arcfeed[i*6] = el.start.x;
        vectors_arcfeed[i*6+1] = el.start.y;
        vectors_arcfeed[i*6+2] = el.start.z;
        vectors_arcfeed[i*6+3] = el.end.x;
        vectors_arcfeed[i*6+4] = el.end.y;
        vectors_arcfeed[i*6+5] = el.end.z;

    }


    //*******************dewell******************************/
    if( dewellssize )
        vectors_dwells = new float[dewellssize*3+3];
    for(unsigned int i = 0;i<dewellssize;i++) {

        GL_GcodeElement el = m->dewells.at(i);
        vectors_dwells[i*3] = el.start.x;
        vectors_dwells[i*3+1] = el.start.y;
        vectors_dwells[i*3+2] = el.start.z;

    }


    //*******************traverse******************************/
    if( traversesize )
        vectors_traverse = new float[traversesize*2*3+3];
    for(unsigned int i = 0;i<traversesize;i++) {

        GL_GcodeElement el = m->traverse.at(i);
        vectors_traverse[i*6] = el.start.x;
        vectors_traverse[i*6+1] = el.start.y;
        vectors_traverse[i*6+2] = el.start.z;
        vectors_traverse[i*6+3] = el.end.x;
        vectors_traverse[i*6+4] = el.end.y;
        vectors_traverse[i*6+5] = el.end.z;

    }


    if(traversesize+dewellssize+arcfeedsize+feedsize) {

        glGenBuffersARB(1, &vboId);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,
                        traversesize*2*3*sizeof(float) \
                        + feedsize*2*3*sizeof(float) \
                        + arcfeedsize*2*3*sizeof(float) \
                        + dewellssize*3*sizeof(float) ,
                        0, GL_STATIC_DRAW_ARB);

        if(traversesize)
            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, traversesize*2*3*sizeof(float), vectors_traverse);        //copy vertices starting from 0 offest

        if(feedsize)
            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,
                               traversesize*2*3*sizeof(float)
                               ,feedsize*2*3*sizeof(float),
                               vectors_feed);        //copy vertices starting from 0 offest

        if(arcfeedsize)
            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,
                               traversesize*2*3*sizeof(float) \
                               + feedsize*2*3*sizeof(float) \
                               , arcfeedsize*2*3*sizeof(float),
                               vectors_arcfeed);        //copy vertices starting from 0 offest

        if(dewellssize)
            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,
                               traversesize*2*3*sizeof(float) \
                               + feedsize*2*3*sizeof(float) \
                               + arcfeedsize*2*3*sizeof(float) \
                               , dewellssize*3*sizeof(float),
                               vectors_dwells);        //copy vertices starting from 0 offest


    }



}

static double inline vectorAngle(double *p1, double *p2);
static void inline footPointToLine(double *p1, double *p2,
                        double *p0,double *pn);

double GL_CanonDraw::twoPointAngleToLine(double *p1, double *p2,
                                       double *p3, double *p4) {

    double angle;
    double foot1[3];
    footPointToLine(p1,p2,p3,foot1);

    double foot2[3];
    footPointToLine(p1,p2,p4,foot2);

    foot1[0] = p3[0] - foot1[0];
    foot1[1] = p3[1] - foot1[1];
    foot1[2] = p3[2] - foot1[2];

    foot2[0] = p4[0] - foot2[0];
    foot2[1] = p4[1] - foot2[1];
    foot2[2] = p4[2] - foot2[2];

    angle = vectorAngle(foot1,foot2);
    return angle;

}


#define x0 p0[0]
#define x1 p1[0]
#define x2 p2[0]
#define xn pn[0]

#define y0 p0[1]
#define y1 p1[1]
#define y2 p2[1]
#define yn pn[1]

#define z0 p0[2]
#define z1 p1[2]
#define z2 p2[2]
#define zn pn[2]

static double inline vectorAngle(double *p1, double *p2) {

    double cosangl = (x1*x2 + y1*y2 +z1*z2) / \
    ( sqrt(x1*x1 + y1*y1 + z1*z1) * sqrt( x2*x2 + y2*y2 + z2*z2));

    return cosangl;

}

static void inline footPointToLine(double *p1, double *p2,
                        double *p0,double *pn) {


    double k;
    k = -  ( (x1-x0)*(x2-x1) + (y1-y0)*(y2-y1) +(z1-z0)*(z2-z1))/ \
            ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

    xn = k*(x2-x1) + x1;
    yn = k*(y2-y1) + y1;
    zn = k*(z2-z1) + z1;

}


void GL_CanonDraw::setColors(const QString &type, float color[]) {


    //qDebug()<<"GL_CanonDraw::setColors"<<type
    //          <<color[0]<<color[1]<<color[2]<<sizeof(color_tool);

    if(type == "ColorTool")
        memcpy(color_tool,color,sizeof(color_tool));
    else if(type == "ColorBackGround")
        memcpy(color_background,color,sizeof(color_background));
    else if(type == "ColorLimit")
        memcpy(color_limit,color,sizeof(color_limit));
    else if(type == "ColorFeed")
        memcpy(color_feed,color,sizeof(color_feed));
    else if(type == "ColorArcFeed")
        memcpy(color_arcfeed,color,sizeof(color_arcfeed));
    else if(type == "ColorOrigin")
        memcpy(color_origin,color,sizeof(color_origin));
    else if(type == "ColorTraverse")
        memcpy(color_traverse,color,sizeof(color_traverse));
    else if(type == "ColorSelected")
        memcpy(color_selected,color,sizeof(color_selected));
    else if(type == "ColorRealTimePath")
        memcpy(color_rtpath,color,sizeof(color_rtpath));

}

void GL_CanonDraw::setColors(const QString &type, const QColor &col) {


    float color[4];
    color[0] = col.redF();
    color[1] = col.greenF();
    color[2] = col.blueF();
    color[3] = 1;

    if(type == "ColorTool")
        memcpy(color_tool,color,sizeof(color_tool));
    else if(type == "ColorBackGround")
        memcpy(color_background,color,sizeof(color_background));
    else if(type == "ColorLimit")
        memcpy(color_limit,color,sizeof(color_limit));
    else if(type == "ColorFeed")
        memcpy(color_feed,color,sizeof(color_feed));
    else if(type == "ColorArcFeed")
        memcpy(color_arcfeed,color,sizeof(color_arcfeed));
    else if(type == "ColorOrigin")
        memcpy(color_origin,color,sizeof(color_origin));
    else if(type == "ColorTraverse")
        memcpy(color_traverse,color,sizeof(color_traverse));
    else if(type == "ColorSelected")
        memcpy(color_selected,color,sizeof(color_selected));
    else if(type == "ColorRealTimePath")
        memcpy(color_rtpath,color,sizeof(color_rtpath));

}
