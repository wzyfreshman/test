//    This is a component of AXIS, a front-end for emc
//    Copyright 2004, 2005, 2006 Jeff Epler <jepler@unpythonic.net> and
//    Chris Radek <chris@timeguy.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#include <libintl.h>
#include <locale.h>

#include "stdlib.h"
#include "stdio.h"
#include "memory.h"
#include "sys/time.h"


#include "gcodedraw.h"
#include "rs274ngc.hh"
#include "rs274ngc_interp.hh"
#include "interp_return.hh"
#include "canon.hh"

#include "config.h"		// LINELEN
//#include "gcodeview.h"

char _parameter_file_name[LINELEN];

//static GCodeView *gcodeview;

static int interp_error;
static int last_sequence_number;
static bool metric;
static double _pos_x, _pos_y, _pos_z, _pos_a, _pos_b, _pos_c, _pos_u, _pos_v, _pos_w;
EmcPose tool_offset;


static double rotation_offsetx = 0;
static double rotation_offsety = 0;

static Interp interp_new;
static GCodeDraw *gCodedraw;

static int maybe_new_line(int sequence_number=interp_new.sequence_number());
static int maybe_new_line(int sequence_number) {
    int ret = 0;
    if(sequence_number == last_sequence_number)
        return 0;
    LineCode new_line_code;
    interp_new.active_settings(new_line_code.activeSettings);
    interp_new.active_g_codes(new_line_code.activeGCodes);
    interp_new.active_m_codes(new_line_code.activeMCodes);
    new_line_code.activeGCodes[0] = sequence_number;
    last_sequence_number = sequence_number;
    ret = gCodedraw->next_line(new_line_code);
    return ret;
}

void NURBS_FEED(int line_number, std::vector<CONTROL_POINT> nurbs_control_points, unsigned int k) {
    double u = 0.0;
    unsigned int n = nurbs_control_points.size() - 1;
    double umax = n - k + 2;
    unsigned int div = nurbs_control_points.size()*15;
    std::vector<unsigned int> knot_vector = knot_vector_creator(n, k);
    PLANE_POINT P1;
    while (u+umax/div < umax) {
        PLANE_POINT P1 = nurbs_point(u+umax/div,k,nurbs_control_points,knot_vector);
        STRAIGHT_FEED(line_number, P1.X,P1.Y, _pos_z, _pos_a, _pos_b, _pos_c, _pos_u, _pos_v, _pos_w);
        u = u + umax/div;
    }
    P1.X = nurbs_control_points[n].X;
    P1.Y = nurbs_control_points[n].Y;
    STRAIGHT_FEED(line_number, P1.X,P1.Y, _pos_z, _pos_a, _pos_b, _pos_c, _pos_u, _pos_v, _pos_w);
    knot_vector.clear();
}

void THREE_2_CIRCLE(int lineno,double x,double y,double z,  \
                    double a,double b,double c,\
                    double centerx,double centery,double centerz,\
                    double normalx,double normaly,double normalz) {

}

void ARC_FEED(int line_number,
              double first_end, double second_end, double first_axis,
              double second_axis, int rotation, double axis_end_point,
              double a_position, double b_position, double c_position,
              double u_position, double v_position, double w_position) {
    // XXX: set _pos_*
    if(!metric) {
        first_end /= 25.4;
        second_end /= 25.4;
        first_axis /= 25.4;
        second_axis /= 25.4;
        axis_end_point /= 25.4;
        u_position /= 25.4;
        v_position /= 25.4;
        w_position /= 25.4;
    }
    if(maybe_new_line(line_number) < 0) return;

    gCodedraw->arc_feed(first_end, second_end, first_axis, second_axis,
                       rotation, axis_end_point,
                       a_position, b_position, c_position,
                       u_position, v_position, w_position);

}

void STRAIGHT_FEED(int line_number,
                   double x, double y, double z,
                   double a, double b, double c,
                   double u, double v, double w) {
    _pos_x=x; _pos_y=y; _pos_z=z;
    _pos_a=a; _pos_b=b; _pos_c=c;
    _pos_u=u; _pos_v=v; _pos_w=w;
    if(!metric) { x /= 25.4; y /= 25.4; z /= 25.4; u /= 25.4; v /= 25.4; w /= 25.4; }
    if(maybe_new_line(line_number) < 0) return;
    gCodedraw->straight_feed(x, y, z, a, b, c, u, v, w);
}

void MIN_STRAIGHT_FEED(int *linenumber,double *minx,
                       double *miny, double *minz, double *minvel,int level) {

    double *min_x;
    double *min_y;
    double *min_z;
    double *min_vel;
    int *line_number;
    line_number = linenumber;
    min_x = minx;
    min_y = miny;
    min_z = minz;
    min_vel = minvel;
    double a = 0.0,b = 0.0,c = 0.0;
    double u = 0.0,v = 0.0,w = 0.0;

    for(int i=0;i<level;i++) {
        STRAIGHT_FEED(line_number[i],min_x[i],min_y[i],min_z[i],a,b,c,u,v,w);
    }
}

void STRAIGHT_TRAVERSE(int line_number,
                       double x, double y, double z,
                       double a, double b, double c,
                       double u, double v, double w) {
    _pos_x=x; _pos_y=y; _pos_z=z;
    _pos_a=a; _pos_b=b; _pos_c=c;
    _pos_u=u; _pos_v=v; _pos_w=w;
    if(!metric) { x /= 25.4; y /= 25.4; z /= 25.4; u /= 25.4; v /= 25.4; w /= 25.4; }
    if(maybe_new_line(line_number)< 0) return;
    gCodedraw->straight_traverse(x, y, z, a, b, c, u, v, w);

}

void SET_G5X_OFFSET(int g5x_index,
                    double x, double y, double z,
                    double a, double b, double c,
                    double u, double v, double w) {
    if(!metric) { x /= 25.4; y /= 25.4; z /= 25.4; u /= 25.4; v /= 25.4; w /= 25.4; }
    if(maybe_new_line()< 0) return;
    gCodedraw->set_g5x_offset(g5x_index, x, y, z, a, b, c, u, v, w);
}

void SET_G92_OFFSET(double x, double y, double z,
                    double a, double b, double c,
                    double u, double v, double w) {
    if(!metric) { x /= 25.4; y /= 25.4; z /= 25.4; u /= 25.4; v /= 25.4; w /= 25.4; }
    if(maybe_new_line()< 0) return;
    gCodedraw->set_g92_offset(x, y, z, a, b, c, u, v, w);
}

void SET_XY_ROTATION(double t,double offsetx,double offsety) {
    if(maybe_new_line()< 0) return;
    gCodedraw->set_xy_rotation(t,offsetx,offsety);
    rotation_offsetx = offsetx;
    rotation_offsety = offsety;
}

void USE_LENGTH_UNITS(CANON_UNITS u) {

    metric = u == CANON_UNITS_MM;

}


void SELECT_PLANE(CANON_PLANE pl) {
    if(maybe_new_line()< 0) return;
    gCodedraw->set_plane(pl);
}

int GET_SINGLE_OFFSET_TOOL()
{
    return 0;
}


void SET_G0_MODE(int val, int index)
{
    val = 0;
}

void SET_WAIT_SIGNAL(double delay,char *str)
{
}

void SET_ROTATION_ZMODE(int zmode) {}

void SET_ROTATION_DIS(double x,double y,double z) {}
void SET_MULTI_DEF(int type,int val,double val2){}

void SET_COOR_Z(double *x,double *y,double *z,int *gmode) {}
void CLEAR_ENDPOINTS()
{
}

double GET_Z_DIFF() {
    return 0;
}
int GET_Z_NOW() {
    return 2;
}

double GET_Z_AXIS_OFFSET(int i){}

double GET_Z_OFFSET() {
    return 0;
}
void SET_SYNCH_CMD(){}

void SET_SINGLE_OFFSET_TOOL(int tool)
{
}
void SET_LINEARAUX(int num,double val)
{}
void SET_TRAVERSE_RATE(double rate) {
    if(maybe_new_line()< 0) return;
    gCodedraw->set_traverse_rate(rate);
}

void SET_FEED_MODE(int mode) {

}


void CHANGE_TOOL(int pocket) {
    if(maybe_new_line()< 0) return;
    gCodedraw->change_tool(pocket);
}


void CHANGE_TOOL_NUMBER(int pocket,int compflag,double compval) {
    if(maybe_new_line()< 0) return;
}

/* XXX: This needs to be re-thought.  Sometimes feed rate is not in linear
 * units--e.g., it could be inverse time feed mode.  in that case, it's wrong
 * to convert from mm to inch here.  but the gcode time estimate gets inverse
 * time feed wrong anyway..
 */
void SET_FEED_RATE(double rate) {
    if(maybe_new_line()< 0) return;
    if(!metric) rate /= 25.4;
    gCodedraw->set_feed_rate(rate);
}

void DWELL(double time,int spindleflag) {
    if(maybe_new_line()< 0) return;
    gCodedraw->dwell(time);
}

void MESSAGE(char *comment) {
    if(maybe_new_line()< 0) return;
    gCodedraw->message(comment);
}

void LOG(char *s) {}
void LOGOPEN(char *f) {}
void LOGAPPEND(char *f) {}
void LOGCLOSE() {}

void COMMENT(const char *comment) {
    if(maybe_new_line()< 0) return;
    gCodedraw->comment(comment);
}

void SET_TOOL_TABLE_ENTRY(int pocket, int toolno, EmcPose offset, double diameter,
                          double frontangle, double backangle, int orientation) {
}

void USE_TOOL_LENGTH_OFFSET(EmcPose offset) {
    tool_offset = offset;
    if(maybe_new_line()< 0) return;
    if(!metric) {
        offset.tran.x /= 25.4; offset.tran.y /= 25.4; offset.tran.z /= 25.4;
        offset.u /= 25.4; offset.v /= 25.4; offset.w /= 25.4; }
    gCodedraw->tool_offset(offset.tran.x, offset.tran.y, offset.tran.z,
        offset.a, offset.b, offset.c, offset.u, offset.v, offset.w);
}

void SET_FEED_REFERENCE(double reference) { }
void SET_CUTTER_RADIUS_COMPENSATION(double radius) {}
void START_CUTTER_RADIUS_COMPENSATION(int direction) {}
void STOP_CUTTER_RADIUS_COMPENSATION() {}
void START_SPEED_FEED_SYNCH() {}
void START_SPEED_FEED_SYNCH(double sync, int vel) {}
void STOP_SPEED_FEED_SYNCH() {}
void START_SPINDLE_COUNTERCLOCKWISE() {}
void START_SPINDLE_CLOCKWISE() {}
void SET_SPINDLE_MODE(double) {}
void STOP_SPINDLE_TURNING() {}
void SET_SPINDLE_SPEED(double rpm) {}
void ORIENT_SPINDLE(double d, int i) {}
void PROGRAM_STOP() {}
void PROGRAM_END() {}
void FINISH() {}
void PALLET_SHUTTLE() {}
void SELECT_POCKET(int tool) {}
void OPTIONAL_PROGRAM_STOP() {}
bool GET_BLOCK_DELETE() {return true;}

void DISABLE_FEED_OVERRIDE() {}
void DISABLE_FEED_HOLD() {}
void G0_HOLD_ENABLE(int mode) {}
void ENABLE_FEED_HOLD() {}
void DISABLE_SPEED_OVERRIDE() {}
void ENABLE_FEED_OVERRIDE() {}
void ENABLE_SPEED_OVERRIDE() {}
void MIST_OFF() {}
void FLOOD_OFF() {}
void MIST_ON() {}
void FLOOD_ON() {}
void LUBE_ON() {}
void LUBE_OFF() {}
void CLEAR_AUX_OUTPUT_BIT(int bit) {}
void SET_AUX_OUTPUT_BIT(int bit) {}
void SET_AUX_OUTPUT_VALUE(int index, double value) {}
void SET_DOUBLEZ_CHANGE(int axis,bool flag,double *zdiff){} // hong 2014-03-27
void SET_M99_EXECUTE(){} //hong 2014-04-15
void CLEAR_MOTION_OUTPUT_BIT(int bit) {}
void SET_EXTERN_OUTPUT_BIT(int index,bool status){}
void SET_MOTION_OUTPUT_BIT(int bit) {}
void SET_MOTION_OUTPUT_VALUE(int index, double value) {}
void TURN_PROBE_ON() {}
void TURN_PROBE_OFF() {}
int UNLOCK_ROTARY(int line_no, int axis) {return 0;}
int LOCK_ROTARY(int line_no, int axis) {return 0;}

void STRAIGHT_PROBE(int line_number,
                    double x, double y, double z,
                    double a, double b, double c,
                    double u, double v, double w, unsigned char probe_type) {
    _pos_x=x; _pos_y=y; _pos_z=z;
    _pos_a=a; _pos_b=b; _pos_c=c;
    _pos_u=u; _pos_v=v; _pos_w=w;
    if(!metric) { x /= 25.4; y /= 25.4; z /= 25.4; u /= 25.4; v /= 25.4; w /= 25.4; }
    if(maybe_new_line(line_number)< 0) return;
    gCodedraw->straight_probe(x, y, z, a, b, c, u, v, w);
}
void RIGID_TAP(int line_number,
               double x, double y, double z) {
    if(!metric) { x /= 25.4; y /= 25.4; z /= 25.4; }
    if(maybe_new_line(line_number)< 0) return;
    gCodedraw->rigid_tap(x, y, z);
}
double GET_EXTERNAL_MOTION_CONTROL_TOLERANCE() { return 0.1; }
double GET_EXTERNAL_PROBE_POSITION_X() { return _pos_x; }
double GET_EXTERNAL_PROBE_POSITION_Y() { return _pos_y; }
double GET_EXTERNAL_PROBE_POSITION_Z() { return _pos_z; }
double GET_EXTERNAL_PROBE_POSITION_A() { return _pos_a; }
double GET_EXTERNAL_PROBE_POSITION_B() { return _pos_b; }
double GET_EXTERNAL_PROBE_POSITION_C() { return _pos_c; }
double GET_EXTERNAL_PROBE_POSITION_U() { return _pos_u; }
double GET_EXTERNAL_PROBE_POSITION_V() { return _pos_v; }
double GET_EXTERNAL_PROBE_POSITION_W() { return _pos_w; }
double GET_EXTERNAL_PROBE_VALUE() { return 0.0; }
int GET_EXTERNAL_PROBE_TRIPPED_VALUE() { return 0; }
double GET_EXTERNAL_POSITION_X() { return _pos_x; }
double GET_EXTERNAL_POSITION_Y() { return _pos_y; }
double GET_EXTERNAL_POSITION_Z() { return _pos_z; }
double GET_EXTERNAL_POSITION_A() { return _pos_a; }
double GET_EXTERNAL_POSITION_B() { return _pos_b; }
double GET_EXTERNAL_POSITION_C() { return _pos_c; }
double GET_EXTERNAL_POSITION_U() { return _pos_u; }
double GET_EXTERNAL_POSITION_V() { return _pos_v; }
double GET_EXTERNAL_POSITION_W() { return _pos_w; }
void INIT_CANON() {}
void GET_EXTERNAL_PARAMETER_FILE_NAME(char *name, int max_size) {
    char *nametmp;
    nametmp = gCodedraw->parameter_file();
    if(!nametmp) { name[0] = 0; return; }
    memset(name, 0, max_size);
    strncpy(name, nametmp, max_size - 1);
}
int GET_EXTERNAL_LENGTH_UNIT_TYPE() { return CANON_UNITS_MM; }
CANON_TOOL_TABLE GET_EXTERNAL_TOOL_TABLE(int pocket) {
    CANON_TOOL_TABLE t = {-1,{{0,0,0},0,0,0,0,0,0},0,0,0,0};
    if(gCodedraw->get_tool(pocket,&t) < 0) {
        //t = {-1,{{0,0,0},0,0,0,0,0,0},0,0,0,0};
    }
    return t;
}

int GET_EXTERNAL_DIGITAL_INPUT(int index, int def) { return def; }
double GET_EXTERNAL_ANALOG_INPUT(int index, double def) { return def; }
int WAIT(int index, int input_type, int wait_type, double timeout) { return 0;}

static void user_defined_function(int num, double arg1, double arg2) {
    if(maybe_new_line()< 0) return;
    gCodedraw->user_defined_function(num, arg1, arg2);
}

void SET_FEED_REFERENCE(int ref) {}
int GET_EXTERNAL_QUEUE_EMPTY() { return true; }
CANON_DIRECTION GET_EXTERNAL_SPINDLE() { return 0; }
int GET_EXTERNAL_TOOL_SLOT() { return 0; }
int GET_EXTERNAL_SELECTED_TOOL_SLOT() { return 0; }
double GET_EXTERNAL_FEED_RATE() { return 1; }
double GET_EXTERNAL_TRAVERSE_RATE() { return 0; }
int GET_EXTERNAL_FLOOD() { return 0; }
int GET_EXTERNAL_MIST() { return 0; }
CANON_PLANE GET_EXTERNAL_PLANE() { return 1; }
double GET_EXTERNAL_SPEED() { return 0; }
int GET_EXTERNAL_POCKETS_MAX() { return CANON_POCKETS_MAX; }
void DISABLE_ADAPTIVE_FEED() {}
void ENABLE_ADAPTIVE_FEED() {}

int GET_EXTERNAL_FEED_OVERRIDE_ENABLE() {return 1;}
int GET_EXTERNAL_SPINDLE_OVERRIDE_ENABLE() {return 1;}
int GET_EXTERNAL_ADAPTIVE_FEED_ENABLE() {return 0;}
int GET_EXTERNAL_FEED_HOLD_ENABLE() {return 1;}

int GET_EXTERNAL_AXIS_MASK() {
    return gCodedraw->get_axis_mask();
}

double GET_EXTERNAL_TOOL_LENGTH_XOFFSET() {
    return tool_offset.tran.x;
}
double GET_EXTERNAL_TOOL_LENGTH_YOFFSET() {
    return tool_offset.tran.y;
}
double GET_EXTERNAL_TOOL_LENGTH_ZOFFSET() {
    return tool_offset.tran.z;
}
double GET_EXTERNAL_TOOL_LENGTH_AOFFSET() {
    return tool_offset.a;
}
double GET_EXTERNAL_TOOL_LENGTH_BOFFSET() {
    return tool_offset.b;
}
double GET_EXTERNAL_TOOL_LENGTH_COFFSET() {
    return tool_offset.c;
}
double GET_EXTERNAL_TOOL_LENGTH_UOFFSET() {
    return tool_offset.u;
}
double GET_EXTERNAL_TOOL_LENGTH_VOFFSET() {
    return tool_offset.v;
}
double GET_EXTERNAL_TOOL_LENGTH_WOFFSET() {
    return tool_offset.w;
}
double GET_EXTERNAL_ANGLE_UNITS() {
    return gCodedraw->get_external_angular_units();
}

double GET_EXTERNAL_LENGTH_UNITS() {
    return gCodedraw->get_external_length_units();
}

static bool check_abort() {
    return gCodedraw->check_abort();
}

void FLUSH_Z_OFFSET(int axis, double *zdiff);

USER_DEFINED_FUNCTION_TYPE USER_DEFINED_FUNCTION[USER_DEFINED_FUNCTION_NUM];

CANON_MOTION_MODE motion_mode;
void SET_MOTION_CONTROL_MODE(CANON_MOTION_MODE mode, double tolerance) { motion_mode = mode; }
void SET_MOTION_CONTROL_MODE(double tolerance) { }
void SET_MOTION_CONTROL_MODE(CANON_MOTION_MODE mode) { motion_mode = mode; }
CANON_MOTION_MODE GET_EXTERNAL_MOTION_CONTROL_MODE() { return motion_mode; }
void SET_NAIVECAM_TOLERANCE(double tolerance) { }

#define RESULT_OK (result == INTERP_OK || result == INTERP_EXECUTE_FINISH)

int parse_file(GCodeDraw *ldraw,char *file,char *unit,char *init) {
    char *f;
    char *unitcode=0, *initcode=0;
    int error_line_offset = 0;
    struct timeval t0, t1;
    int wait = 1;
    f = file;
    unitcode = unit;
    initcode = init;

    gCodedraw = ldraw;

    bindtextdomain("linuxcnc", EMC2_PO_DIR);
    setlocale(LC_MESSAGES, "");
    setlocale(LC_CTYPE, "");
    textdomain("linuxcnc");

    for(int i=0; i<USER_DEFINED_FUNCTION_NUM; i++)
        USER_DEFINED_FUNCTION[i] = user_defined_function;

    gettimeofday(&t0, NULL);

    metric=false;
    last_sequence_number = -1;

    _pos_x = _pos_y = _pos_z = _pos_a = _pos_b = _pos_c = 0;
    _pos_u = _pos_v = _pos_w = 0;

    interp_new.init("/home/.config/subNgc-Sim/");
    interp_new.interopen(f);

    maybe_new_line();
    interp_new.set_gl_flag(1);

    int result = INTERP_OK;
    if(unitcode) {
        result = interp_new.read(unitcode);
        if(!RESULT_OK) goto out_error;
        result = interp_new.execute();
    }

    if(initcode && RESULT_OK) {
        result = interp_new.read(initcode);
        if(!RESULT_OK) goto out_error;
        result = interp_new.execute();
    }

    while(!interp_error && RESULT_OK) {
        error_line_offset = 1;
        result = interp_new.read();
        gettimeofday(&t1, NULL);
        if(t1.tv_sec > t0.tv_sec + wait) {
            if(check_abort()) return NULL;
            t0 = t1;
        }

        if(!RESULT_OK) break;

        error_line_offset = 0;
        result = interp_new.execute();

        if(interp_new.get_m99_flag()) break;

        if(gCodedraw->isAborted()) {
            result = 0xffffffff;
            break;
        }

    }
out_error:
    interp_new.interclose();
    interp_new.set_gl_flag(0);


    maybe_new_line();
    return result;
}


static int maxerror = -1;

static char savedError[LINELEN+1];
char *rs274_strerror(int err) {
    interp_new.error_text(err, savedError, LINELEN);
    return &savedError[0];
}

static void unrotate(double &x, double &y, double c, double s) {
    double tx = x * c + y * s;
    y = -x * s + y * c;
    x = tx;
}

static void rotate(double &x, double &y, double c, double s) {
    double tx = x * c - y * s;
    y = x * s + y * c;
    x = tx;
}

// vim:ts=8:sts=4:sw=4:et:
