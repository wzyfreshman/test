#ifndef GET_EMCSTATUS_H
#define GET_EMCSTATUS_H

#include <QString>

enum AXIS_NAME{
    AXIS_X = 0,
    AXIS_Y,
    AXIS_Z1,
    AXIS_Z2
};

int get_actual_feed(); //get the emc actual feed rate
int get_program_running_time(); //get the program running time return seconds
int get_actual_spindle_vel();    //get the actual_spindle_speed
int get_tool_now();             //get the tool no in spindle
int get_motion_paused();    //get the paused flag of the motion
double get_tool_compnow(int val);      //get the tool compensation,val '1' means g43,'0' means g41,g42;
double get_abs_pos(int axis);   //get the absolute pos of the axis
double get_offset_pos(int axis); // get the offset position of the axis
double get_left_pos(int axis);  // get the DTG of the axis
int get_spindle_status();
int get_lube_status();
int get_flood_status();
int get_task_mode();     //get the emcStatus->task.mode
int get_task_state();  //get the emcStatus->task.state
int get_task_status(); //RCS_DONE RCS_ERROR RCS_EXEC
int get_task_interp();
int get_task_work_piece();
int get_task_g1feed();
int get_run_spindle_feed();
bool get_changing_z();  //add by hong
int get_task_z();
int get_task_homed(int axis);
bool get_mdi_running();
bool get_pro_running();
bool get_handle_guide(); //0 off,1 on
int get_handlewheel_value();
bool get_verify();
bool get_autorepeat();
char *get_inter_file();
void get_alm_pins();
int wait_for_state(int state);
int wait_for_mode(int mode);
int get_time_left();
bool register_timeout();
QString get_fpga_ver();
QString get_customid();
QString get_boardid();
QString get_chip_ver();
bool get_fpgacheck(int i);
bool hard_checkok();
int get_pro_interpline();

#endif // GET_EMCSTATUS_H
