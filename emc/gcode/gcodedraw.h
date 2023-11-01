#ifndef GCODEDRAW_H
#define GCODEDRAW_H

#include "rs274ngc.hh"		// ACTIVE_G_CODES, etc

class GCodeDraw;

int parse_file(GCodeDraw *ldraw,char *file,char *unit,char *init);
char *rs274_strerror(int err);

class GCode {
public:
    int sequence_number;
    int motion_mode;
    int block;
    int plane;
    int cutter_side;
    int units;
    int distance_mode;
    int feed_mode;
    int origin;
    int tool_length_offset;
    int retract_mode;
    int path_mode;
};
class MCode {
    int stopping;
    int spindle;
    int toolchange;
    int mist;
    int flood;
    int overrides;
};
class Settings {
    double feed_rate;
    double speed;
};

class LineCode {
public:
    GCode *gCode;
    MCode *mCode;
    Settings *settings;
    int activeGCodes[ACTIVE_G_CODES];
    int activeMCodes[ACTIVE_M_CODES];
    double activeSettings[ACTIVE_SETTINGS];
};

class GCodeDraw
{

public:
    GCodeDraw();
    ~GCodeDraw();
    virtual int next_line(LineCode &);
    virtual int arc_feed(double first_end, double second_end, double first_axis,
                 double second_axis,int rotation, double axis_end_point,
                 double a_position,double b_position,double c_position,
                 double u_position,double v_position,double w_position);
    virtual int straight_feed(double x,double y,double z,
                      double a,double b,double c,
                      double u,double v,double w);
    virtual int straight_traverse(double x,double y,double z,
                          double a,double b,double c,
                          double u,double v,double w);
    virtual int set_g5x_offset(int g5x_index,double x,double y,double z,
                       double a,double b,double c,
                       double u,double v,double w);
    virtual int set_g92_offset(double x,double y,double z,
                       double a,double b,double c,
                       double u,double v,double w);
    virtual int tool_offset(double x,double y,double z,
                    double a,double b,double c,
                    double u,double v,double w);
    virtual int straight_probe(double x,double y,double z,
                       double a,double b,double c,
                       double u,double v,double w);
    virtual int rigid_tap(double x,double y,double z);

    virtual int set_xy_rotation(double rotate, double offsetx, double offsety);
    virtual int set_plane(int plane);
    virtual int set_traverse_rate(double rate);
    virtual int change_tool(int pocket);
    virtual int set_feed_rate(double rate);
    virtual int dwell(int time);
    virtual int message(char *msg);
    virtual int comment(const char *msg);
    virtual int get_block_delete();
    virtual char *parameter_file();
    virtual int get_tool(int tool,CANON_TOOL_TABLE *table);
    virtual int user_defined_function(int num,double arg1,double arg2);
    virtual int get_axis_mask();
    virtual bool check_abort();

    virtual double get_external_angular_units();
    virtual double get_external_length_units();

    virtual void abortParse();
    virtual bool isAborted();
protected:
    int lineno;
    LineCode *state;
    bool abort;

};

#endif // GCODEDRAW_H
