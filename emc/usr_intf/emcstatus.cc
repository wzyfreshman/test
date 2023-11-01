#include "emcstatus.hh"
#include "emc.hh"		// EMC NML
#include "emc_nml.hh"
#include "canon.hh"		// CANON_UNITS, CANON_UNITS_INCHES,MM,CM
#include "emcglb.h"		// EMC_NMLFILE, TRAJ_MAX_VELOCITY, etc.
///var/work/linuxcnc/src/emc/nml_intf/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int get_actual_feed()
{
    if(get_task_mode() == EMC_TASK_MODE_AUTO) {
        return (int)(emcStatus->motion.traj.current_vel*60+0.9);
    } else {
        for(int i = 0;i<EMCMOT_MAX_JOINTS;i++) {
            if(emcStatus->motion.axis[i].velocity != 0) {
                return (int)(emcStatus->motion.axis[i].velocity*60+0.9);
            }
        }
        return 0;
    }
}

int get_program_running_time()
{
    return emcStatus->task.program_running_time;
}

int get_actual_spindle_vel()
{
    return (int)(emcStatus->motion.spindle.speed *
            emcStatus->motion.traj.spindle_scale + 0.9);
}

int get_tool_now()
{
    if(emcStatus->task.mode != EMC_TASK_MODE_AUTO)
        return emcStatus->task.toolnow;
    else
        return emcStatus->motion.traj.tool;
}

int get_motion_paused()
{
    return emcStatus->motion.pausefinish;
    //return emcStatus->motion.traj.paused;
}

double get_tool_compnow(int val)
{
    if(0 == val) {
    if(1 == emcStatus->motion.traj.compside || 2 == emcStatus->motion.traj.compside) {
        return emcStatus->io.tool.toolcompvalue[emcStatus->motion.traj.tool]*2;          //show the dia not the rad
    } else
        return 0.0;
    }
    return 0.0;
}

double get_tool_length_comp() {
    return emcStatus->motion.traj.tool_length_comp;
}

double get_abs_pos(int axis)
{
    //return emcStatus->motion.axis[axis].output - emcStatus->motion.axis[axis].backlash_filt;
    return emcStatus->motion.axis[axis].output;
}

double get_offset_pos(int axis, int doublez = 0)
{

    switch(axis)
    {
        case AXIS_X: return  get_abs_pos(AXIS_X)
                            - emcStatus->task.g5x_offset.tran.x
                            - emcStatus->task.g92_offset.tran.x
                            + emcStatus->task.rotationModex;
                    break;
        case AXIS_Y: return get_abs_pos(AXIS_Y)
                    - emcStatus->task.g5x_offset.tran.y
                    - emcStatus->task.g92_offset.tran.y
                    + emcStatus->task.rotationModey;
                    break;
        case AXIS_Z1:return get_abs_pos(AXIS_Z1)
                - emcStatus->task.g5x_offset.tran.z
                - emcStatus->task.g92_offset.tran.z;
                break;
        case AXIS_A:return get_abs_pos(AXIS_A)
                - emcStatus->task.g5x_offset.a
                - emcStatus->task.g92_offset.tran.z;
                break;
    case AXIS_B:return get_abs_pos(AXIS_B)
	    - emcStatus->task.g5x_offset.b
	    - emcStatus->task.g92_offset.b;
	    break;
    case AXIS_C:return get_abs_pos(AXIS_C)
	    - emcStatus->task.g5x_offset.c
	    - emcStatus->task.g92_offset.c;
	    break;
    case AXIS_Y2:return get_abs_pos(AXIS_Y2)
            - emcStatus->task.g5x_offset.tran.y
            - emcStatus->task.g92_offset.tran.y
            + emcStatus->task.rotationModey;
        break;
        default:break;
    }
    return 0;
}

double get_left_pos(int axis)
{
    switch(axis) {
        case AXIS_X: return emcStatus->motion.traj.distance_to_go_x; break;
        case AXIS_Y: return emcStatus->motion.traj.distance_to_go_y; break;
        case AXIS_Z1: return emcStatus->motion.traj.distance_to_go_z; break;
        case AXIS_A: return emcStatus->motion.traj.distance_to_go_z; break;
    }
    return 0.0;

}

char *get_inter_file()
{
    return emcStatus->task.file;
}

int get_task_mode()
{
    return emcStatus->task.mode;
}

int get_task_state()
{
    return emcStatus->task.state;
}

int get_task_status()
{
    return emcStatus->status;
}

int get_task_interp()
{
    return emcStatus->task.interpState;
}

bool get_changing_z()
{
    return emcStatus->motion.doublez.z_changing;
}

int get_task_z()
{
    return emcStatus->motion.set_which_z;
}

int get_task_g1feed()
{
    return (int)(emcStatus->motion.traj.velocity+0.95);
}

int get_run_spindle_feed()
{
    return (int)(emcStatus->motion.spindle.speed);
}

int get_task_homed(int axis)
{
    return emcStatus->motion.axis[axis].homed;
}

int get_spindle_status() // 0 means stop 1 forward, -1 reverse
{
    return emcStatus->motion.spindle.direction;
}
int get_lube_status() // 0 means off 1 on
{
    return emcStatus->io.lube.on;
}
int get_flood_status() // 0 off, 1 on
{
    return emcStatus->io.coolant.flood;
}
bool get_handle_guide() //0 off,1 on
{
    return emcStatus->task.handlewheel_enable;
}

int get_handlewheel_value()
{
    return emcStatus->motion.jog_state;
}

bool get_verify()  // 0 means verify is over,1 means running
{
    return emcStatus->task.only_reading;
}

bool get_autorepeat()
{
    return emcStatus->task.autorepeat;
}

int get_task_work_piece()
{
    return emcStatus->task.work_piece;
}

bool get_pro_running()
{
    return emcStatus->task.mode == EMC_TASK_MODE_AUTO &&
            emcStatus->task.interpState != EMC_TASK_INTERP_IDLE &&
            !emcStatus->task.only_reading;
}

bool get_mdi_running()
{
    return emcStatus->task.mode == EMC_TASK_MODE_MDI &&
            emcStatus->status != RCS_DONE;
}

int get_pro_interpline()
{
    return emcStatus->task.setstartline;
}



int get_time_left()
{
        return (int)emcStatus->task.timeleft;
}

bool register_timeout() {
    if(SIMULATE)
        return false;
        if(0 <= emcStatus->task.timeleft && emcStatus->task.timeleft < 60 )
            return true;
        else
            return false;

}

bool authorize_timeout(){
    if(SIMULATE)
        return false;

    if( emcStatus->motion.dateerror == -4 )
        return true;
    else
        return false;

}

//读LKT单片机程序版本
char *get_chip_ver() {

    static char str3[1024];
    sprintf(str3,"%02x%02x",emcStatus->motion.fpgaversion[6],emcStatus->motion.fpgaversion[7]);
    return str3;
}


char *get_customid() {
    static char id[512];
    memset(id,0,sizeof(id));
    sprintf(id,"%02x%02x%02x%02x",emcStatus->motion.customid[4],
                          emcStatus->motion.customid[5],
                          emcStatus->motion.customid[6],
                          emcStatus->motion.customid[7]);
    return id;
}

char *get_boardid() {
    static char id[512];
    memset(id,0,sizeof(id));
    sprintf(id,"%02x%02x%02x%02x",emcStatus->task.getcpuid[3],
                          emcStatus->task.getcpuid[2],
                          emcStatus->task.getcpuid[1],
                          emcStatus->task.getcpuid[0]);
    return id;
}

bool get_all_homed(int axisnum) {
    bool homed = true;
    for(int i = 0;i<axisnum;i++) {
        if(emcStatus->motion.axis[i].homed == 0) {
                homed = false;
                break;
        }
    }
    return homed;
}

bool get_homing(int axisnum) {
    bool homing = false;
    for(int i = 0;i<axisnum;i++) {
        if(emcStatus->motion.axis[i].homing > 0) {
                homing = true;
                break;
        }
    }
    return homing;
}

bool get_fpgacheck(int i) {

    return ((0 == emcStatus->task.fpgacheck[i])?true:false);
}

bool hard_checkok() {
    if(0 > emcStatus->task.hardwarecheck)
        return false;
    else
        return true;
}

double par_get_var(int name){
    return emcStatus->task.parameters[name];
}

int get_axis_alarm(char *str) {
    int ret =0;
    for(int i=0;i<4;i++) {
        if(emcStatus->motion.axis[i].errorcode != 0) {
            ret = -1;
            sprintf(str,"code:%d-%X",i,emcStatus->motion.axis[i].errorcode);
            break;
        }
    }
    return ret;
}

void get_alm_pins(int *_real_alm_pin)
{
    _real_alm_pin[0] = emcStatus->io.aux.estop;
    _real_alm_pin[1] = emcStatus->motion.axis[0].fault; //x_server
    _real_alm_pin[2] = emcStatus->motion.axis[1].fault;
    _real_alm_pin[3] = emcStatus->motion.axis[2].fault;
    _real_alm_pin[4] = emcStatus->motion.axis[3].fault;
    _real_alm_pin[5] = emcStatus->motion.axis[0].maxHardLimit;
    _real_alm_pin[6] = emcStatus->motion.axis[0].minHardLimit;
    _real_alm_pin[7] = emcStatus->motion.axis[1].maxHardLimit;
    _real_alm_pin[8] = emcStatus->motion.axis[1].minHardLimit;
    _real_alm_pin[9] = emcStatus->motion.axis[2].maxHardLimit;
    _real_alm_pin[10] = emcStatus->motion.axis[2].minHardLimit;
    _real_alm_pin[11] = emcStatus->motion.axis[3].maxHardLimit;
    _real_alm_pin[12] = emcStatus->motion.axis[3].minHardLimit;
    _real_alm_pin[13] = emcStatus->motion.traj.electric_cabinet_alarm; //console
    _real_alm_pin[14] = emcStatus->motion.traj.spindle0_overcurrent_alarm; //spindle1_overcurrent
    _real_alm_pin[15] = emcStatus->motion.traj.spindle1_overcurrent_alarm; //spindle2_overcurrent
    _real_alm_pin[16] = emcStatus->motion.traj.spindle0_converter_alarm; //Spindle1_Frequency_converter
    _real_alm_pin[17] = emcStatus->motion.traj.spindle1_converter_alarm; //Spindle2_Frequency_converter
    _real_alm_pin[18] = emcStatus->motion.traj.probe_alarm; //probe
    _real_alm_pin[19] = emcStatus->motion.traj.pressure_alarm; //pressure
    _real_alm_pin[20] = emcStatus->motion.traj.coolant_alarm; //coolant
    _real_alm_pin[21] = emcStatus->motion.traj.lube_alarm; //lube
    _real_alm_pin[22] = emcStatus->motion.traj.run_on; //program run
    _real_alm_pin[23] = emcStatus->motion.traj.pause_on; //program pause
    _real_alm_pin[24] = emcStatus->motion.user_defwarn[0]; // user define start
    _real_alm_pin[25] = emcStatus->motion.user_defwarn[1]; // user define start
    _real_alm_pin[26] = emcStatus->motion.user_defwarn[2]; // user define start
    _real_alm_pin[27] = emcStatus->motion.user_defwarn[3]; // user define start
    _real_alm_pin[28] = emcStatus->motion.user_defwarn[4]; // user define start
    _real_alm_pin[29] = emcStatus->motion.user_defwarn[5]; // user define start
    _real_alm_pin[30] = emcStatus->motion.user_defwarn[6]; // user define start
    _real_alm_pin[31] = emcStatus->motion.user_defwarn[7]; // user define start
    _real_alm_pin[32] = emcStatus->motion.user_defwarn[8]; // user define start
    _real_alm_pin[33] = emcStatus->motion.user_defwarn[9]; // user define start
    _real_alm_pin[34] = emcStatus->motion.user_defwarn[10]; // user define start

}


