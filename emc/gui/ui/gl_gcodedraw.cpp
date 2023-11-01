#include <QDebug>
#include <QApplication>

#include "gl_gcodedraw.h"
#include "gl_viewwidget.h"

GL_GcodeDraw::GL_GcodeDraw(QString &parameter,GL_ViewWidget *v) {

    file = parameter;

    plane = 1;
    lastp.x = 0;
    lastp.y = 0;
    lastp.z = 0;
    lastp.a = 0;
    lastp.b = 0;
    lastp.c = 0;
    lastp.u = 0;
    lastp.v = 0;
    lastp.w = 0;

    offset.x = 0;
    offset.y = 0;
    offset.z = 0;
    offset.a = 0;
    offset.b = 0;
    offset.c = 0;
    offset.u = 0;
    offset.v = 0;
    offset.w = 0;

    firstmove = true;

    widget = v;

}

void GL_GcodeDraw::init()
{
    plane = 1;
    lastp.x = 0;
    lastp.y = 0;
    lastp.z = 0;
    lastp.a = 0;
    lastp.b = 0;
    lastp.c = 0;
    lastp.u = 0;
    lastp.v = 0;
    lastp.w = 0;

    offset.x = 0;
    offset.y = 0;
    offset.z = 0;
    offset.a = 0;
    offset.b = 0;
    offset.c = 0;
    offset.u = 0;
    offset.v = 0;
    offset.w = 0;

    traverse.clear();
    feed.clear();
    arcfeed.clear();
    dewells.clear();

    firstmove = true;
}

char *GL_GcodeDraw::parameter_file() {

    return file.toLocal8Bit().data();

}


int GL_GcodeDraw::next_line(LineCode &st) {

    static int localcount = 0;
    this->lineno = st.activeGCodes[0];

    if(localcount >= 650) {
        widget->parsingFileLineNumber(lineno);
        QApplication::processEvents();
        localcount = 0;
    } else {
        localcount++;
    }

    return 0 ;

}

int GL_GcodeDraw::arc_feed(double first_end, double second_end, double first_axis,
             double second_axis,int rotation, double axis_end_point,
             double a_position,double b_position,double c_position,
             double u_position,double v_position,double w_position) {

    //qDebug()<<"arc_feed"<< first_end<< second_end<< first_axis<<
    //         second_axis<< rotation<< axis_end_point;

    firstmove = false;
    QList<GCODEPOS> segs;
    rs274_arc_to_segments(first_end,second_end,first_axis,second_axis,
                          rotation, axis_end_point,
                           a_position, b_position, c_position,
                           u_position, v_position, w_position,segs);


    straightArcsegments(segs);

    return 0;
}

void GL_GcodeDraw::straightArcsegments(QList<GCODEPOS> &segs) {

    firstmove = false;
    GCODEPOS lo = lastp;

    int linenol = lineno;
    float feedratel = feedrate;

    for(int i = 0; i<segs.size(); i++) {

        GL_GcodeElement el(GL_GcodeElement::ElementFeed);
        el.linenum = linenol;
        el.start = lo;
        el.end = segs.at(i);
        el.offset = offset;
        el.feedrate = feedratel;
        arcfeed.append(el);
        lo = segs.at(i);

    }

    lastp = lo;


}

int GL_GcodeDraw::straight_feed(double x,double y,double z,
                  double a,double b,double c,
                  double u,double v,double w) {

//    qDebug()<<"straight_feed"<<g5x_offset_x<<g5x_offset_y<<x<<y<<z;

    GCODEPOS l;
    l.x = x;
    l.y = y;
    l.z = z;
    l.a = a;
    l.b = b;
    l.c = c;
    l.u = u;
    l.v = v;
    l.w = w;
    rotate_and_translate(&l.x,&l.y,&l.z,&l.a,&l.b,&l.c,&l.u,&l.v,&l.w);

    if(!firstmove) {

        GL_GcodeElement el(GL_GcodeElement::ElementFeed);
        el.start = lastp;
        el.end = l;
        el.linenum = lineno;
        el.offset = offset;
        el.feedrate = feedrate;
        feed.append(el);

    }

    firstmove = false;
    lastp = l;

    return 0;
}

int GL_GcodeDraw::straight_traverse(double x,double y,double z,
                      double a,double b,double c,
                      double u,double v,double w) {

    //qDebug()<<"straight_traverse"<<x<<y<<z;

    GCODEPOS l;
    l.x = x;
    l.y = y;
    l.z = z;
    l.a = a;
    l.b = b;
    l.c = c;
    l.u = u;
    l.v = v;
    l.w = w;

    rotate_and_translate(&l.x,&l.y,&l.z,&l.a,&l.b,&l.c,&l.u,&l.v,&l.w);

    if(!firstmove) {

        GL_GcodeElement el(GL_GcodeElement::ElementTraverse);
        el.start = lastp;
        el.end = l;
        el.linenum = lineno;
        el.offset = offset;
        traverse.append(el);
    }
    firstmove = false;
    lastp = l;
    return 0;
}

int GL_GcodeDraw::set_g5x_offset(int index,double x,double y,
                   double z,double a,double b,
                   double c,double u,double v, double w) {


    g5x_index = index;
    g5x_offset_x = x;
    g5x_offset_y = y;
    g5x_offset_z = z;
    g5x_offset_a = a;
    g5x_offset_b = b;
    g5x_offset_c = c;
    g5x_offset_u = u;
    g5x_offset_v = v;
    g5x_offset_w = w;

    return 0;
}

int GL_GcodeDraw::set_g92_offset(double x,double y,double z,
                   double a,double b,double c,
                   double u,double v,double w) {


    g92_offset_x = x;
    g92_offset_y = y;
    g92_offset_z = z;
    g92_offset_a = a;
    g92_offset_b = b;
    g92_offset_c = c;
    g92_offset_u = u;
    g92_offset_v = v;
    g92_offset_w = w;

    return 0;
}

int GL_GcodeDraw::set_xy_rotation(double rotate,double offsetx,double offsety) {


    rotation_xy = rotate;
    double t = rotate*M_PI/180;
    rotation_sin = sin(t);
    rotation_cos = cos(t);

    rotation_offsetx = offsetx;
    rotation_offsety = offsety;
    return 0;
}

int GL_GcodeDraw::set_plane(int pl) {

    //qDebug()<<"set_plane"<<pl;
    plane = pl;
    return 0;
}

int GL_GcodeDraw::set_traverse_rate(double ) {

    //qDebug()<<"set_traverse_rate";
    return 0;
}

int GL_GcodeDraw::change_tool(int ) {

    //qDebug()<<"change_tool";
    firstmove = true;
    return 0;
}

int GL_GcodeDraw::set_feed_rate(double rate) {

    feedrate = rate/60.0;
    return 0;
}

int GL_GcodeDraw::dwell(int time) {

    GL_GcodeElement el(GL_GcodeElement::ElementDwells);
    el.start = lastp;
    el.end = lastp;
    el.linenum = lineno;
    el.offset = offset;
    el.dwelltime  = time;
    dewells.append(el);

    return 0;
}

int GL_GcodeDraw::message(char *msg) {

    qDebug()<<"message:"<<msg;
    return 0;

}

int GL_GcodeDraw::comment(const char *) {

    //qDebug()<<"comment"<<msg;
    return 0;
}

int GL_GcodeDraw::tool_offset(double xo,double yo,double zo,
                double ao,double bo,double co,
                double uo,double vo,double wo) {

    firstmove = true;
    GCODEPOS bk = lastp;

    lastp.x = bk.x - xo + offset.x;
    lastp.y = bk.y - yo + offset.y;
    lastp.z = bk.z - zo + offset.z;

    lastp.a = bk.a - ao + offset.a;
    lastp.b = bk.b - bo + offset.b;
    lastp.c = bk.c - co + offset.c;

    lastp.u = bk.u - uo + offset.u;
    lastp.v = bk.v - vo + offset.v;
    lastp.w = bk.w - wo + offset.w;


    offset.x = xo;
    offset.x = xo;
    offset.y = yo;
    offset.z = zo;
    offset.a = ao;
    offset.b = bo;
    offset.c = co;
    offset.u = uo;
    offset.v = vo;
    offset.w = wo;
    return 0;
}

int GL_GcodeDraw::get_block_delete() {

    return 0;
}

int GL_GcodeDraw::straight_probe(double x,double y,double z,
                   double a,double b,double c,
                   double u,double v,double w) {

    straight_feed(x,y,z,a,b,c,u,v,w);
    return 0;
}

int GL_GcodeDraw::rigid_tap(double x,double y,double z) {

    firstmove = false;

    GCODEPOS l;
    l.x = x;
    l.y = y;
    l.z = z;
    l.a = 0;
    l.b = 0;
    l.c = 0;
    l.u = 0;
    l.v = 0;
    l.w = 0;
    rotate_and_translate(&l.x,&l.y,&l.z,&l.a,&l.b,&l.c,&l.u,&l.v,&l.w);

    l.a = lastp.a;
    l.b = lastp.b;
    l.c = lastp.c;
    l.u = lastp.u;
    l.v = lastp.v;
    l.w = lastp.w;

    GL_GcodeElement el(GL_GcodeElement::ElementFeed);
    el.start = lastp;
    el.end = l;
    el.linenum = lineno;
    el.offset = offset;
    feed.append(el);

    GL_GcodeElement el2(GL_GcodeElement::ElementFeed);
    el2.start = l;
    el2.end = lastp;
    el2.linenum = lineno;
    el2.offset = offset;
    feed.append(el2);

    //self.feed_append((self.lineno, self.lo, l, self.feedrate, [self.xo, self.yo, self.zo]))
    //self.feed_append((self.lineno, l, self.lo, self.feedrate, [self.xo, self.yo, self.zo]))
    return 0;
}



int GL_GcodeDraw::get_tool(int tool,CANON_TOOL_TABLE *table) {

    //qDebug()<<"getTool"<<tool;

    table->toolno = tool;

    table->offset.tran.x = 0;
    table->offset.tran.y = 0;
    table->offset.tran.z = 0;

    table->offset.a = 0;
    table->offset.b = 0;
    table->offset.c = 0;

    table->offset.u = 0;
    table->offset.v = 0;
    table->offset.w = 0;
    table-> diameter = 0;
    table-> frontangle = 0;
    table-> backangle= 0;
    table-> orientation= 0;

    return 0;
}

int GL_GcodeDraw::user_defined_function(int ,double ,double ) {

    //qDebug()<<"user_defined_function";
    return 0;
}

int GL_GcodeDraw::get_axis_mask() {

    return 0;
}

bool GL_GcodeDraw::check_abort() {

    return 0;
}

double GL_GcodeDraw::get_external_angular_units() {

    return 1.0;
}

double GL_GcodeDraw::get_external_length_units() {

    //return 0.03937007874016;
    return 1;
}


static void unrotate(double &x, double &y, double c, double s) {
    double tx = x * c + y * s;
    y = -x * s + y * c;
    x = tx;
}

static void rotate(double &x, double &y, double c, double s,double offsetx,double offsety) {
    //x += offsetx;
    //y += offsety;
    double tx = x * c - y * s;
    y = x * s + y * c;
    x = tx;
    //x -= offsetx;
    //y -= offsety;
}


void GL_GcodeDraw::rotate_and_translate(float *x, float *y, float *z,
                                        float *a, float *b, float *c,
                                        float *u, float *v, float *w) {



    *x += g92_offset_x;
    *y += g92_offset_y;
    *z += g92_offset_z;
    *a += g92_offset_a;
    *b += g92_offset_b;
    *c += g92_offset_c;
    *u += g92_offset_u;
    *v += g92_offset_v;
    *w += g92_offset_w;
/*
    //Rotate around the X axis
    double rotation_a = (*a) * 2 * M_PI / 360;
    *x = *x;
    *y = (*y)*cos(rotation_a)-(*z)*sin(rotation_a);
    *z = (*y)*sin(rotation_a)+(*z)*cos(rotation_a);

     //Rotate around the Z axis
    double rotation_c = (*c) * 2 * M_PI / 360;
    *x =  (*x) * cos(rotation_c) - (*y) * sin(rotation_c);
    *y = (*x)*sin(rotation_c)+(*y)*cos(rotation_c);
    *z = *z;
*/

    if (rotation_xy != 0) {
        double rotx = (*x) * rotation_cos - (*y) * rotation_sin;
        *y = (*x) * rotation_sin + (*y) * rotation_cos;
        *x = rotx;
    }


    *x += g5x_offset_x;
    *y += g5x_offset_y;
    *z += g5x_offset_z;
    *a += g5x_offset_a;
    *b += g5x_offset_b;
    *c += g5x_offset_c;
    *u += g5x_offset_u;
    *v += g5x_offset_v;
    *w += g5x_offset_w;

}

void GL_GcodeDraw::rs274_arc_to_segments(double x1,double y1,double cx,double cy,int rotation,
                             double z1,double a,double b,double c,
                             double u,double v,double w,QList<GCODEPOS> &segs) {

    double o[9], n[9], g5xoffset[9], g92offset[9];
    int rot, plane;
    int X, Y, Z;
    double rotation_cos, rotation_sin;
    int max_segments = 128;

    rot = rotation;



    rotation_cos = this->rotation_cos;
    rotation_sin = this->rotation_sin;
    plane = this->plane;
    g5xoffset[0] = this->g5x_offset_x;
    g5xoffset[1] = this->g5x_offset_y;
    g5xoffset[2] = this->g5x_offset_z;
    g5xoffset[3] = this->g5x_offset_a;
    g5xoffset[4] = this->g5x_offset_b;
    g5xoffset[5] = this->g5x_offset_c;
    g5xoffset[6] = this->g5x_offset_u;
    g5xoffset[7] = this->g5x_offset_v;
    g5xoffset[8] = this->g5x_offset_w;

    g92offset[0] = this->g92_offset_x;
    g92offset[1] = this->g92_offset_y;
    g92offset[2] = this->g92_offset_z;
    g92offset[3] = this->g92_offset_a;
    g92offset[4] = this->g92_offset_b;
    g92offset[5] = this->g92_offset_c;
    g92offset[6] = this->g92_offset_u;
    g92offset[7] = this->g92_offset_v;
    g92offset[8] = this->g92_offset_w;


    if(plane == 1) {
        X=0; Y=1; Z=2;
    } else if(plane == 3) {
        X=2; Y=0; Z=1;
    } else {
        X=1; Y=2; Z=0;
    }

    o[0] = lastp.x;
    o[1] = lastp.y;
    o[2] = lastp.z;
    o[3] = lastp.a;
    o[4] = lastp.b;
    o[5] = lastp.c;
    o[6] = lastp.u;
    o[7] = lastp.v;
    o[8] = lastp.w;

    n[X] = x1;
    n[Y] = y1;
    n[Z] = z1;
    n[3] = a;
    n[4] = b;
    n[5] = c;
    n[6] = u;
    n[7] = v;
    n[8] = w;
    for(int ax=0; ax<9; ax++) o[ax] -= g5xoffset[ax];
    unrotate(o[0], o[1], rotation_cos, rotation_sin);
    for(int ax=0; ax<9; ax++) o[ax] -= g92offset[ax];

    double theta1 = atan2(o[Y]-cy, o[X]-cx);
    double theta2 = atan2(n[Y]-cy, n[X]-cx);

    if(rot < 0) {
        while(theta2 - theta1 > -0.0) theta2 -= 2*M_PI;
    } else {
        while(theta2 - theta1 < 0.0) theta2 += 2*M_PI;
    }

    // if multi-turn, add the right number of full circles
    if(rot < -1) theta2 += 2*M_PI*(rot+1);
    if(rot > 1) theta2 += 2*M_PI*(rot-1);


    int steps = std::max(3, int(max_segments * fabs(theta1 - theta2) / M_PI));
    double rsteps = 1. / steps;

    double dtheta = theta2 - theta1;
    double d[9] = {0, 0, 0, n[3]-o[3], n[4]-o[4], n[5]-o[5], n[6]-o[6], n[7]-o[7], n[8]-o[8]};
    d[Z] = n[Z] - o[Z];

    double tx = o[X] - cx, ty = o[Y] - cy, dc = cos(dtheta*rsteps), ds = sin(dtheta*rsteps);
    for(int i=0; i<steps-1; i++) {
        double f = (i+1) * rsteps;
        double p[9];
        rotate(tx, ty, dc, ds,rotation_offsetx,rotation_offsety);
        p[X] = tx + cx;
        p[Y] = ty + cy;
        p[Z] = o[Z] + d[Z] * f;
        p[3] = o[3] + d[3] * f;
        p[4] = o[4] + d[4] * f;
        p[5] = o[5] + d[5] * f;
        p[6] = o[6] + d[6] * f;
        p[7] = o[7] + d[7] * f;
        p[8] = o[8] + d[8] * f;
        for(int ax=0; ax<9; ax++) p[ax] += g92offset[ax];
        rotate(p[0], p[1], rotation_cos, rotation_sin,rotation_offsetx,rotation_offsety);
        for(int ax=0; ax<9; ax++) p[ax] += g5xoffset[ax];

        GCODEPOS el;
        el.x = p[0];
        el.y = p[1];
        el.z = p[2];
        el.a = p[3];
        el.b = p[4];
        el.c = p[5];
        el.u = p[6];
        el.v = p[7];
        el.w = p[8];
        segs.append(el);
        //PyList_SET_ITEM(segs, i,
        //    Py_BuildValue("ddddddddd", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]));


    }
    for(int ax=0; ax<9; ax++) n[ax] += g92offset[ax];
    rotate(n[0], n[1], rotation_cos, rotation_sin,rotation_offsetx,rotation_offsety);
    for(int ax=0; ax<9; ax++) n[ax] += g5xoffset[ax];

    //PyList_SET_ITEM(segs, steps-1,
    //    Py_BuildValue("ddddddddd", n[0], n[1], n[2], n[3], n[4], n[5], n[6], n[7], n[8]));

    GCODEPOS el;
    el.x = n[0];
    el.y = n[1];
    el.z = n[2];
    el.a = n[3];
    el.b = n[4];
    el.c = n[5];
    el.u = n[6];
    el.v = n[7];
    el.w = n[8];
    segs.append(el);

    //return segs;
}

bool GL_GcodeDraw::isEmpty() {

    if(feed.isEmpty() && arcfeed.isEmpty()
            &&traverse.isEmpty())
        return true;
    else
        return false;
}

void GL_GcodeDraw::rs274_calc_extents() {

    min_x = 9e99;min_y = 9e99;min_z = 9e99;
           min_xt = 9e99; min_yt = 9e99; min_zt = 9e99;
           max_x = -9e99;max_y = -9e99; max_z = -9e99;
           max_xt = -9e99;max_yt = -9e99;max_zt = -9e99;


        int j;
        double xs, ys, zs, xe, ye, ze, xt, yt, zt;
        for(j=0; j<feed.size(); j++) {
            GL_GcodeElement  el = feed.at(j);

            xs = el.start.x;
            ys = el.start.y;
            zs = el.start.z;
            xe = el.end.x;
            ye = el.end.y;
            ze = el.end.z;
            xt = el.offset.x;
            yt = el.offset.y;
            zt = el.offset.z;

            max_x = std::max(max_x, xs);
            max_y = std::max(max_y, ys);
            max_z = std::max(max_z, zs);
            min_x = std::min(min_x, xs);
            min_y = std::min(min_y, ys);
            min_z = std::min(min_z, zs);
            max_xt = std::max(max_xt, xs+xt);
            max_yt = std::max(max_yt, ys+yt);
            max_zt = std::max(max_zt, zs+zt);
            min_xt = std::min(min_xt, xs+xt);
            min_yt = std::min(min_yt, ys+yt);
            min_zt = std::min(min_zt, zs+zt);
        }
        if(j > 0) {
            max_x = std::max(max_x, xe);
            max_y = std::max(max_y, ye);
            max_z = std::max(max_z, ze);
            min_x = std::min(min_x, xe);
            min_y = std::min(min_y, ye);
            min_z = std::min(min_z, ze);
            max_xt = std::max(max_xt, xe+xt);
            max_yt = std::max(max_yt, ye+yt);
            max_zt = std::max(max_zt, ze+zt);
            min_xt = std::min(min_xt, xe+xt);
            min_yt = std::min(min_yt, ye+yt);
            min_zt = std::min(min_zt, ze+zt);
        }


        //*********arcfeed
        for(j=0; j<arcfeed.size(); j++) {
            GL_GcodeElement  el = arcfeed.at(j);

            xs = el.start.x;
            ys = el.start.y;
            zs = el.start.z;
            xe = el.end.x;
            ye = el.end.y;
            ze = el.end.z;
            xt = el.offset.x;
            yt = el.offset.y;
            zt = el.offset.z;

            max_x = std::max(max_x, xs);
            max_y = std::max(max_y, ys);
            max_z = std::max(max_z, zs);
            min_x = std::min(min_x, xs);
            min_y = std::min(min_y, ys);
            min_z = std::min(min_z, zs);
            max_xt = std::max(max_xt, xs+xt);
            max_yt = std::max(max_yt, ys+yt);
            max_zt = std::max(max_zt, zs+zt);
            min_xt = std::min(min_xt, xs+xt);
            min_yt = std::min(min_yt, ys+yt);
            min_zt = std::min(min_zt, zs+zt);
        }
        if(j > 0) {
            max_x = std::max(max_x, xe);
            max_y = std::max(max_y, ye);
            max_z = std::max(max_z, ze);
            min_x = std::min(min_x, xe);
            min_y = std::min(min_y, ye);
            min_z = std::min(min_z, ze);
            max_xt = std::max(max_xt, xe+xt);
            max_yt = std::max(max_yt, ye+yt);
            max_zt = std::max(max_zt, ze+zt);
            min_xt = std::min(min_xt, xe+xt);
            min_yt = std::min(min_yt, ye+yt);
            min_zt = std::min(min_zt, ze+zt);
        }


        //*********arcfeed
        for(j=0; j<traverse.size(); j++) {
            GL_GcodeElement  el = traverse.at(j);

            xs = el.start.x;
            ys = el.start.y;
            zs = el.start.z;
            xe = el.end.x;
            ye = el.end.y;
            ze = el.end.z;
            xt = el.offset.x;
            yt = el.offset.y;
            zt = el.offset.z;

            max_x = std::max(max_x, xs);
            max_y = std::max(max_y, ys);
            max_z = std::max(max_z, zs);
            min_x = std::min(min_x, xs);
            min_y = std::min(min_y, ys);
            min_z = std::min(min_z, zs);
            max_xt = std::max(max_xt, xs+xt);
            max_yt = std::max(max_yt, ys+yt);
            max_zt = std::max(max_zt, zs+zt);
            min_xt = std::min(min_xt, xs+xt);
            min_yt = std::min(min_yt, ys+yt);
            min_zt = std::min(min_zt, zs+zt);
        }
        if(j > 0) {
            max_x = std::max(max_x, xe);
            max_y = std::max(max_y, ye);
            max_z = std::max(max_z, ze);
            min_x = std::min(min_x, xe);
            min_y = std::min(min_y, ye);
            min_z = std::min(min_z, ze);
            max_xt = std::max(max_xt, xe+xt);
            max_yt = std::max(max_yt, ye+yt);
            max_zt = std::max(max_zt, ze+zt);
            min_xt = std::min(min_xt, xe+xt);
            min_yt = std::min(min_yt, ye+yt);
            min_zt = std::min(min_zt, ze+zt);
        }

}
