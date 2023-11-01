#include "ccd_sysparameter.h"
#include <QDebug>
#include <math.h>

CCD_SysParameter::CCD_SysParameter(int i) {
    id = i;
}

CCD_SysParameter::CCD_SysParameter(const QString &name,int i) {
    groupname = name;
    id = i;
}

void CCD_SysParameter::setGroupName(const QString &name) {
    groupname = name;
}

QString CCD_SysParameter::getGroupName() {
    return groupname;
}

void CCD_SysParameter::copyCommonParameterTo(CCD_SysParameter *d) {

    d->getimg_delaytime = getimg_delaytime;
    //d->two_points_w = two_points_w;
    //d->origin_pos = origin_pos;
    d-> img_resolution = img_resolution;
    d-> sample_num = sample_num;

    d->array_with = array_with;
    d->array_height = array_height;
    d->array_distance_hor = array_distance_hor;
    d->array_distance_ver = array_distance_ver;
    d->enable_array_identify = enable_array_identify;

    //d->correct_offset_x = correct_offset_x;
    //d->correct_offset_y = correct_offset_y;
    //d->correct_max_a = correct_max_a;
    //d->compute_rotate_offset = compute_rotate_offset;

    d->correct_max_a = correct_max_a;
    d->global_offset_h = global_offset_h;
    d->global_offset_v = global_offset_v;

}

void CCD_SysParameter::saveToDisk( CD_IniSettings &set) {


    set.beginGroup(groupname);
    set.setValue("camera_height",camera_height);
    set.setValue("offset_v", offset_v);
    set.setValue("offset_h",offset_h);
    set.setValue("global_offset_v", global_offset_v);
    set.setValue("global_offset_h",global_offset_h);
    set.setValue("offset_angle",offset_angle);
    set.setValue("camera_magx",camera_magx);
    set.setValue("camera_magy", camera_magy);
    set.setValue("mark_num", marknum);
    set.setValue("getimg_delaytime", getimg_delaytime);
    set.setValue("two_points_w", two_points_w);
    set.setValue("origin_pos", origin_pos);
    set.setValue("img_resolution", img_resolution);
    set.setValue("sample_num", sample_num);
    set.setValue("tool_size", tool_size);
    set.setValue("circle_distance", circle_distance);
    set.setValue("two_points_h", two_points_h);
    set.setValue("array_with", array_with);
    set.setValue("array_height", array_height);
    set.setValue("array_distance_hor", array_distance_hor);
    set.setValue("array_distance_ver", array_distance_ver);
    set.setValue("enable_array_identify",enable_array_identify);
    set.setValue("correct_offset_x", correct_offset_x);
    set.setValue("correct_offset_y", correct_offset_y);
    set.setValue("correct_max_a", correct_max_a);
    set.setValue("init_x_angle",init_x_angle);
    set.setValue("compute_rotate_offset",compute_rotate_offset);
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            set.setValue(QString("compensate_mat%1%2").arg(i).arg(j), compensate_mat[i][j]);
        }
    }
    set.setValue("compensate_img_enable", compensate_img_enable);

    set.endGroup();

}

void CCD_SysParameter::readFromDisk( CD_IniSettings &set) {


    set.beginGroup(groupname);
    camera_height = set.value("camera_height",0).toDouble();
    offset_v = set.value("offset_v", 0).toDouble();
    offset_h = set.value("offset_h",0).toDouble();
    global_offset_v = set.value("global_offset_v", 0).toDouble();
    global_offset_h = set.value("global_offset_h",0).toDouble();
    offset_angle = set.value("offset_angle",0).toDouble();
    camera_magx = set.value("camera_magx", 100).toDouble();
    camera_magy = set.value("camera_magy",100).toDouble();
    marknum = set.value("mark_num",3).toInt();
    getimg_delaytime = set.value("getimg_delaytime",200).toInt();
    two_points_w = set.value("two_points_w",1.0).toDouble();
    origin_pos = set.value("origin_pos",0).toInt();
    img_resolution = set.value("img_resolution",8).toInt();
    sample_num = set.value("sample_num",10).toInt();
    tool_size = set.value("tool_size",0).toDouble();
    circle_distance = set.value("circle_distance",0.0).toDouble();
    two_points_h = set.value("two_points_h",0.0).toDouble();
    array_with = set.value("array_with",1.0).toInt();
    array_height = set.value("array_height",1.0).toInt();
    array_distance_ver = set.value("array_distance_ver",0).toDouble();
    array_distance_hor = set.value("array_distance_hor",0).toDouble();
    enable_array_identify = set.value("enable_array_identify",false).toBool();

    correct_offset_x = set.value("correct_offset_x",0).toDouble();
    correct_offset_y = set.value("correct_offset_y",0).toDouble();
    correct_max_a = set.value("correct_max_a",(5*M_PI)/180.0).toDouble();

    init_x_angle = set.value("init_x_angle",0.0).toDouble();
    compute_rotate_offset = set.value("compute_rotate_offset",0.0).toDouble();
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            compensate_mat[i][j] = set.value(QString("compensate_mat%1%2").arg(i).arg(j),10).toDouble();
        }
    }
    compensate_img_enable = set.value("compensate_img_enable",false).toBool();
    set.endGroup();


}

bool CCD_SysParameter ::checkChange() {

    bool debug = false;


    if(debug) qDebug()<<"0";
    if(camera_height_bk != camera_height) return true;
    if(debug) qDebug()<<"1";
    if(offset_x_bk != offset_x) return true;
    if(debug) qDebug()<<"2"<<offset_y_bk<<offset_y;
    if(offset_y_bk != offset_y) return true;
    if(debug) qDebug()<<"3";
    if(offset_v_bk != offset_v) return true;
    if(debug) qDebug()<<"4";
    if(offset_h_bk != offset_h) return true;
    if(debug) qDebug()<<"5";
    if(global_offset_v_bk != global_offset_v) return true;
    if(debug) qDebug()<<"6";
    if(global_offset_h_bk != global_offset_h) return true;
    if(debug) qDebug()<<"7";
    if(offset_angle_bk != offset_angle) return true;
    if(debug) qDebug()<<"8";
    if(camera_magx_bk != camera_magx) return true;
    if(debug) qDebug()<<"9";
    if(camera_magy_bk != camera_magy) return true;
    if(debug) qDebug()<<"10";
    if(marknum_bk != marknum) return true;
    if(debug) qDebug()<<"11";
    if(getimg_delaytime_bk != getimg_delaytime) return true;
    if(debug) qDebug()<<"12";
    if(two_points_w_bk != two_points_w) return true;
    if(debug) qDebug()<<"13";
    if(origin_pos != origin_pos_bk) return true;
    if(debug) qDebug()<<"14";
    if(img_resolution != img_resolution_bk) return true;
    if(debug) qDebug()<<"15";
    if(sample_num != sample_num_bk) return true;
    if(debug) qDebug()<<"16";
    if(tool_size != tool_size_bk) return true;
    if(debug) qDebug()<<"17";
    if(two_points_h != two_points_h_bk) return true;
    if(debug) qDebug()<<"18";
    if(array_with != array_with_bk) return true;
    if(debug) qDebug()<<"19";
    if(array_height != array_height_bk) return true;
    if(debug) qDebug()<<"20";
    if(correct_offset_x != correct_offset_x_bk) return true;
    if(debug) qDebug()<<"21";
    if(correct_offset_y != correct_offset_y_bk) return true;
    if(debug) qDebug()<<"22";
    if(correct_max_a != correct_max_a_bk) return true;
    if(array_distance_hor != array_distance_hor_bk) return true;
    if(array_distance_ver != array_distance_ver_bk) return true;
    if(enable_array_identify != enable_array_identify_bk) return true;

    if(memcmp(compensate_mat,compensate_mat_bk,sizeof(compensate_mat)) != 0) return true;
    if(compensate_img_enable != compensate_img_enable_bk) return true;
    if(compute_rotate_offset != compute_rotate_offset_bk) return true;
    return false;

}

void CCD_SysParameter::push() {

    camera_height_bk = camera_height;
    offset_x_bk = offset_x;
    offset_y_bk = offset_y;
    offset_v_bk = offset_v;
    offset_h_bk = offset_h;
    global_offset_v_bk = global_offset_v;
    global_offset_h_bk = global_offset_h;
    offset_angle_bk = offset_angle;
    camera_magx_bk = camera_magx;
    camera_magy_bk = camera_magy;
    marknum_bk = marknum;
    getimg_delaytime_bk = getimg_delaytime;
    two_points_w_bk = two_points_w;
    origin_pos_bk = origin_pos;
    img_resolution_bk = img_resolution;
    sample_num_bk = sample_num;
    tool_size_bk = tool_size;
    two_points_h_bk =  two_points_h;
    array_with_bk = array_with;
    array_height_bk = array_height;
    array_distance_hor_bk = array_distance_hor;
    array_distance_ver_bk = array_distance_ver;
    enable_array_identify_bk = enable_array_identify;
    correct_offset_x_bk = correct_offset_x;
    correct_offset_y_bk = correct_offset_y;
    correct_max_a_bk = correct_max_a;

    memcpy(compensate_mat_bk,compensate_mat,sizeof(compensate_mat));
    compensate_img_enable_bk = compensate_img_enable;
    compute_rotate_offset_bk = compute_rotate_offset;
}

void CCD_SysParameter::pop() {

    camera_height = camera_height_bk;
    offset_x = offset_x_bk;
    offset_y = offset_y_bk;
    offset_v = offset_v_bk;
    offset_h = offset_h_bk;
    global_offset_v = global_offset_v_bk;
    global_offset_h = global_offset_h_bk;
    offset_angle = offset_angle_bk;
    camera_magx = camera_magx_bk;
    camera_magy = camera_magy_bk;
    marknum = marknum_bk;
    getimg_delaytime = getimg_delaytime_bk;
    two_points_w = two_points_w_bk;
    origin_pos = origin_pos_bk;
    img_resolution = img_resolution_bk;
    sample_num = sample_num_bk;
    tool_size = tool_size_bk;
    two_points_h =  two_points_h_bk;
    array_with = array_with_bk;
    array_height = array_height_bk;
    array_distance_hor = array_distance_hor_bk;
    array_distance_ver = array_distance_ver_bk;
    enable_array_identify = enable_array_identify_bk;
    correct_offset_x = correct_offset_x_bk;
    correct_offset_y = correct_offset_y_bk;
    correct_max_a = correct_max_a_bk;

    memcpy(compensate_mat,compensate_mat_bk,sizeof(compensate_mat));
    compensate_img_enable = compensate_img_enable_bk;
    compute_rotate_offset = compute_rotate_offset_bk;
}
