#include "gcodedraw.h"

#include "canon.hh"

GCodeDraw::GCodeDraw() {
    state = new LineCode();
    abort = false;
}

GCodeDraw::~GCodeDraw() {
    delete state;

}

int GCodeDraw::next_line(LineCode &st) {
    //int ret = 0;
    //this->state = st;
    //this->lineno = state->gCode->sequence_number;
    return 0;
}

int GCodeDraw::arc_feed(double first_end, double second_end, double first_axis,
             double second_axis,int rotation, double axis_end_point,
             double a_position,double b_position,double c_position,
             double u_position,double v_position,double w_position) {
    return 0;
}

int GCodeDraw::straight_feed(double x,double y,double z,
                  double a,double b,double c,
                  double u,double v,double w) {
    return 0;
}

int GCodeDraw::straight_traverse(double x,double y,double z,
                      double a,double b,double c,
                      double u,double v,double w){
    return 0;
}

int GCodeDraw::set_g5x_offset(int g5x_index,double x,double y,
                   double z,double a,double b,
                   double c,double u,double v, double w) {
    return 0;
}

int GCodeDraw::set_g92_offset(double x,double y,double z,
                   double a,double b,double c,
                   double u,double v,double w) {
    return 0;
}

int GCodeDraw::set_xy_rotation(double rotate,double offsetx,double offsety) {

    return 0;
}

int GCodeDraw::set_plane(int plane) {
    return 0;
}

int GCodeDraw::set_traverse_rate(double rate) {
    return 0;
}

int GCodeDraw::change_tool(int pocket) {
    return 0;
}

int GCodeDraw::set_feed_rate(double rate) {

    return 0;
}

int GCodeDraw::dwell(int time) {

    return 0;
}

int GCodeDraw::message(char *msg) {

    return 0;
}

int GCodeDraw::comment(const char *msg) {

    return 0;
}

int GCodeDraw::tool_offset(double x,double y,double z,
                double a,double b,double c,
                double u,double v,double w) {

    return 0;
}

int GCodeDraw::get_block_delete() {

    return 0;
}

int GCodeDraw::straight_probe(double x,double y,double z,
                   double a,double b,double c,
                   double u,double v,double w) {

    return 0;
}

int GCodeDraw::rigid_tap(double x,double y,double z) {

    return 0;
}

char *GCodeDraw::parameter_file() {

    return 0;
}

int GCodeDraw::get_tool(int tool,CANON_TOOL_TABLE *table) {

    return 0;
}

int GCodeDraw::user_defined_function(int num,double arg1,double arg2) {

    return 0;
}

int GCodeDraw::get_axis_mask() {

    return 0;
}

bool GCodeDraw::check_abort() {

    return 0;
}

double GCodeDraw::get_external_angular_units() {

    return 1.0;
}

double GCodeDraw::get_external_length_units() {

    return 0.03937007874016;
}


void GCodeDraw::abortParse() {
    abort = true;
}

bool GCodeDraw::isAborted() {
    return abort;
}










