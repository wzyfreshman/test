#ifndef GL_GCODEDRAW_H
#define GL_GCODEDRAW_H

#include <QString>
#include <QList>

#include "../gcode/gcodedraw.h"


typedef struct GcodePos {
    float x,y,z,a,b,c,u,v,w;
} GCODEPOS;

class GL_GcodeElement {

public:
    enum ElementType{
        ElementTraverse,
        ElementFeed,
        ElementDwells
    };

    GL_GcodeElement(ElementType t) :type(t) {
        linenum = 0;
        feedrate = 0;
        dwelltime = 0;
        start.x = 0;
    }

    int linenum;
    float feedrate;
    int dwelltime;
    GCODEPOS start;
    GCODEPOS end;
    GCODEPOS offset;

    ElementType getType() {return type;}

private:
    ElementType type;


};

class GL_ViewWidget;

class GL_GcodeDraw : public GCodeDraw
{
public:
    GL_GcodeDraw(QString &parameter,GL_ViewWidget *v);
    void init();

    int next_line(LineCode &);
    int arc_feed(double first_end, double second_end, double first_axis,
             double second_axis,int rotation, double axis_end_point,
             double a_position,double b_position,double c_position,
             double u_position,double v_position,double w_position);
    int straight_feed(double x,double y,double z,
                  double a,double b,double c,
                  double u,double v,double w);
    int straight_traverse(double x,double y,double z,
                      double a,double b,double c,
                      double u,double v,double w);
    int set_g5x_offset(int index,double x,double y,double z,
                   double a,double b,double c,
                   double u,double v,double w);
    int set_g92_offset(double x,double y,double z,
                   double a,double b,double c,
                   double u,double v,double w);
    int tool_offset(double x,double y,double z,
                double a,double b,double c,
                double u,double v,double w);
    int straight_probe(double x,double y,double z,
                   double a,double b,double c,
                   double u,double v,double w);
    int rigid_tap(double x,double y,double z);

    int set_xy_rotation(double rotate, double offsetx, double offsety);
    int set_plane(int pl);
    int set_traverse_rate(double rate);
    int change_tool(int pocket);
    int set_feed_rate(double rate);
    int dwell(int time);
    int message(char *msg);
    int comment(const char *msg);
    int get_block_delete();
    char *parameter_file();
    int get_tool(int tool,CANON_TOOL_TABLE *table);
    int user_defined_function(int num,double arg1,double arg2);
    int get_axis_mask();
    bool check_abort();

    double get_external_angular_units();
    double get_external_length_units();

    void rotate_and_translate(float *x,float *y,float *z,
                float *a,float *b,float *c,
                float *u,float *v,float *w);
    void rs274_arc_to_segments(double x1,double y1,double cx,double cy,int rotation,
                               double z1,double a,double b,double c,
                               double u,double v,double w,QList<GCODEPOS> &segs);

    void rs274_calc_extents();
    void straightArcsegments(QList<GCODEPOS> &segs);
    bool isEmpty();


private:
    QString file;

    double rotation_xy;
    double rotation_sin;
    double rotation_cos;
    double rotation_offsetx;
    double rotation_offsety;

    int g5x_index;
    double g5x_offset_x;
    double g5x_offset_y;
    double g5x_offset_z;
    double g5x_offset_a;
    double g5x_offset_b;
    double g5x_offset_c;
    double g5x_offset_u;
    double g5x_offset_v;
    double g5x_offset_w;

    double g92_offset_x;
    double g92_offset_y;
    double g92_offset_z;
    double g92_offset_a;
    double g92_offset_b;
    double g92_offset_c;
    double g92_offset_u;
    double g92_offset_v;
    double g92_offset_w;


    GCODEPOS offset;
    float feedrate;
    bool firstmove;
    GCODEPOS lastp;
    int plane;

    GL_ViewWidget *widget;

public:
    QList< GL_GcodeElement > traverse;
    QList< GL_GcodeElement > feed;
    QList< GL_GcodeElement > arcfeed;
    QList< GL_GcodeElement > dewells;

    double min_x, min_y, min_z,
           min_xt, min_yt, min_zt,
           max_x, max_y, max_z,
           max_xt, max_yt, max_zt;

};

#endif // GL_GCODEDRAW_H
