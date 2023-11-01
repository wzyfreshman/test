#ifndef EMCSTATUS_H
#define EMCSTATUS_H

enum AXIS_NAME{
    AXIS_X = 0,
    AXIS_Y,
    AXIS_Z1,
    AXIS_A,
    AXIS_B,
    AXIS_C,
    AXIS_Y2
};

extern int get_actual_feed(); //get the emc actual feed rate
extern int get_program_running_time(); //get the program running time return seconds
extern int get_actual_spindle_vel();    //get the actual_spindle_speed
extern int get_tool_now();             //get the tool no in spindle
extern int get_motion_paused();    //get the paused flag of the motion
extern double get_tool_compnow(int val);      //get the tool compensation ,'0' means g41,g42;
extern double get_tool_length_comp(); // get the tool length compensation in realtime
extern double get_abs_pos(int axis);   //get the absolute pos of the axis
extern double get_offset_pos(int axis,int doublez); // get the offset position of the axis
extern double get_left_pos(int axis);  // get the DTG of the axis
extern int get_spindle_status();
extern int get_lube_status();
extern int get_flood_status();
extern int get_task_mode();     //get the emcStatus->task.mode
extern int get_task_state();  //get the emcStatus->task.state
extern int get_task_status(); //RCS_DONE RCS_ERROR RCS_EXEC
extern int get_task_interp();
extern int get_task_work_piece();
extern int get_task_g1feed();
extern int get_run_spindle_feed();
extern bool get_changing_z();  //add by hong
extern int get_task_z();
extern int get_task_homed(int axis);
extern bool get_mdi_running();
extern bool get_pro_running();
extern bool get_handle_guide(); //0 off,1 on
extern int get_handlewheel_value();
extern bool get_verify();
extern bool get_autorepeat();
extern char *get_inter_file();
extern void get_alm_pins(int *_real_alm_pin);
extern int get_time_left();
extern bool register_timeout();
extern bool authorize_timeout();
extern char * get_customid();
extern char * get_boardid();
extern char * get_chip_ver();
extern bool get_fpgacheck(int i);
extern bool hard_checkok();
extern int get_pro_interpline();
double par_get_var(int name);
bool get_all_homed(int axisnum);
bool get_homing(int axisnum);
int get_axis_alarm(char *str);
#endif // EMCSTATUS_H
