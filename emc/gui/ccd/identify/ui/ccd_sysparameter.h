#ifndef CCD_SYSPARAMETER_H
#define CCD_SYSPARAMETER_H

#include "cd_inisettings.h"

class CCD_SysParameter
{
    friend class CCD_SysParEditor;
public:
    CCD_SysParameter(int d);
    CCD_SysParameter(const QString &name,int id);
    void saveToDisk( CD_IniSettings &set);
    void readFromDisk( CD_IniSettings &set);
    void setGroupName(const QString & name);
    QString getGroupName();
    int getId() {return id;}
    void copyCommonParameterTo(CCD_SysParameter *d);

    void push();
    void pop();
    bool checkChange();



    double camera_height;
    double offset_x;
    double offset_y;
    double offset_v;
    double offset_h;
    double global_offset_v;
    double global_offset_h;
    double offset_angle;
    double camera_magx;
    double camera_magy;

    int getimg_delaytime;
    double two_points_w;
    double two_points_h;

    int marknum;

    int origin_pos;
    int img_resolution;
    int sample_num;

    double tool_size;
    double  circle_distance;


    int array_with;
    int array_height;
    double array_distance_hor;
    double array_distance_ver;
    bool enable_array_identify;

    double correct_offset_y;
    double correct_offset_x;
    double correct_max_a;

    double compensate_mat[3][3];
    bool compensate_img_enable;

    double init_x_angle;
    double compute_rotate_offset;

private:
    double camera_height_bk;
    double offset_x_bk;
    double offset_y_bk;
    double offset_v_bk;
    double offset_h_bk;
    double global_offset_v_bk;
    double global_offset_h_bk;
    double offset_angle_bk;
    double camera_magx_bk;
    double camera_magy_bk;
    int marknum_bk;
    int getimg_delaytime_bk;
    double two_points_w_bk;
    double two_points_h_bk;

    int origin_pos_bk;
    int img_resolution_bk;
    int sample_num_bk;

    double tool_size_bk;

    int array_with_bk;
    int array_height_bk;
    double array_distance_hor_bk;
    double array_distance_ver_bk;
    bool enable_array_identify_bk;

    double correct_offset_x_bk;
    double correct_offset_y_bk;
    double correct_max_a_bk;

    double compensate_mat_bk[3][3];
    bool compensate_img_enable_bk;

    double init_x_angle_bk;
    double compute_rotate_offset_bk;

public:
    int camergain;
    int shutter;
    int blacklevel;
    bool mirrorV;
    bool mirrorH;

private:
    QString groupname;
    int id;//camera id

};

#endif // CCD_SYSPARAMETER_H
