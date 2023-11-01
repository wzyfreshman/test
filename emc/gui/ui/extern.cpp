#include <QSettings>
#include <QTextCodec>
#include <QByteArray>
#include <QString>
#include <QThread>
#include <QFile>
#include <QFileDialog>
#include <QStringList>
#include <QInputDialog>
#include <QtDebug>
#include <QObject>
#include <QProcess>
#include <QDir>
#include <QTextCodec>

#include <unistd.h>
#include "extern.h"
#include "shcom.hh"
#include "emc.hh"
#include "emc_nml.hh"
#include "rs274ngc.hh"
#include "rs274ngc_return.hh"
#include "interp_internal.hh"	// interpreter private definitions
#include "interp_queue.hh"
#include "rs274ngc_interp.hh"
#include "ini_settings.h"

#include "linuxcnc.h"
#include "emcstatus.hh"

int INFO_FLAG = 0X07;

version_struct *versiondata;

QString _time_now;   //  get the time now
QString _time_date;
QString _time_hour;
QString _ini_filepath;   //the ini filepath
QString _ini_filebak;
QString _var_filepath;   //the var filepath
QString _var_filebak;
QString _zero_filepath;   //the var filepath
QString _zero_filebak;    //zero备份文件
QString _tbl_filepath;   //the var filepath
QString _tbl_filebak;    //tbl备份文件

Ini_Settings *_ini;
Ini_Settings *_prefer;
Ini_Settings *_var;
Ini_Settings *_alarm;
Ini_Settings *_customio;

int _zmode = 0;
int _debug_flag = 1;
bool _forbid_run = false;
bool _forbid_spindle = false;
bool _get_alm = false;
bool _all_homed = false;
int _doublez = 3;
QString _ncdir = "";
QString _subDir = "";

QMap<QString,bool> _io_map;
QMap<QString,int> _customio_map;
QMap<QString,bool> _polarity_change;
QMap<QString,QString> _tran_map;    // read the translate value
QMap<int,QString> _coor_system;

int _real_alm_pin[DEFAULT_ALM_SIG]; //alm_pins real signal
QString _real_alm_info[DEFAULT_ALM_SIG]; //alm_pin info

double par_get_var(int name){
    return emcStatus->task.parameters[name];
}

int get_system(QString cmd,QString *ret) {
    QStringList allchart = cmd.split(" ");
    QStringList para = allchart;
    para.removeAt(0);
    QProcess pro;
    pro.setProcessChannelMode(QProcess::MergedChannels);
    pro.start(allchart.at(0),para);
    if (pro.waitForFinished(-1)) {
        *ret = QString(pro.readAllStandardOutput());
        return 0;
    } else
        return -1;
    return 0;
}

int cmd_system(const char *cmd)
{
    int status = 0;
    if(NULL == cmd){
        return 0;
    }
    status = system(cmd);
    if(status < 0){
        qDebug("system cmd exec error");
    }
    if(127 == status){
        qDebug("system cmd is not exist or exec fail");
    }
    return status;
}
/*
QString par_get_ini(const char *ini,const char *first,const char *second)
{
    QSettings *key = new QSettings(ini,QSettings::IniFormat);
    key->beginGroup(QString::fromLocal8Bit(first));
    QString val = key->value(QString::fromLocal8Bit(second)).toString();
    key->endGroup();
    delete key;
    return val;
}
*/

int par_write_ini(const char *ini, const char *par, char *value)
{


    FILE *fp;
    if((fp=fopen(ini,"r"))==NULL)
    {
      //  qDebug("cannot open file %s\n",ini);
        return -1;
    }
    char file[10000][200];
    int line=0;
    while(fgets(file[line],200,fp)!=NULL)
    {
        line++;	//获取行数
    }
    fclose(fp);

    int flagset=0;  //判断是否存在section
    char *p,buf[100],buf1[50],buf2[50];
    strcpy(buf,par);
    p=strtok(buf,"//");
    if(p) strcpy(buf1,p);
    p=strtok(NULL,"//");
    if(p) strcpy(buf2,p);
    char compar1[50],compar2[50];
    sprintf(compar1,"%s ",buf2);
    sprintf(compar2,"%s=",buf2);
    for(int i=0;i<line;i++)
    {
        char ss[51];
        sprintf(ss,"[%s]",buf1);
        if(!strncmp(ss,file[i],strlen(ss))) flagset=1;
        if((!strncmp(compar1,file[i],strlen(compar1))||!strncmp(compar2,file[i],strlen(compar2))) && flagset==1) {sprintf(file[i],"%s = %s\n",buf2,value);break;}
    }
    FILE *tp;
    if((tp=fopen(ini,"w"))==NULL)
    {
        printf("cannot open file %s\n",ini);
        return -1;
    }
    for(int i=0;i<line;i++)
    {
        fprintf(tp,"%s",file[i]);
    }
    fclose(tp);
    return 0;   

}

QString read_data_from_file(QString file_path)
{
    QTextCodec *code = QTextCodec::codecForName("utf-8");
    QString file_text;
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "ERROR";
    file_text = code->toUnicode(file.read(READ_MAX_LEN));
    file.close();
//    if(file_text.isEmpty())
//        return "ERROR";
    return file_text;
}

int write_data_to_file(QString file,QString data)
{
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
    QByteArray ba_file = file.toLocal8Bit();
    QByteArray ba_data = data.toLocal8Bit();

    int rev = 0;
    FILE *tp;
    if((tp=fopen(ba_file.data(),"w"))==NULL)
    {
        printf("cannot open file %s\n",ba_file.data());
        rev = -1;
        return rev;
    }
    fprintf(tp,"%s",ba_data.data());
    fclose(tp);
    return rev;
}

bool compare_double(double first,double second)
{
    if(fabs(first - second) < MINSCALE)
        return true;
    else
        return false;
}

int ensure_mode(int mode)
{
    if(wait_for_state(RCS_DONE) != 0)
        return -1;
    if(mode == emcStatus->task.mode)
        return 0;
    switch(mode)
    {
        case EMC_TASK_MODE_MANUAL: sendManual();
        break;
        case EMC_TASK_MODE_AUTO: sendAuto();
        break;
        case EMC_TASK_MODE_MDI: sendMdi();
        break;
        default:sendManual();break;
    }
    int count = 0;
    while(mode != emcStatus->task.mode) {
        updateStatus();
        count++;
        updateStatus();
        usleep(1000*10);
        if(count > 50) {
            return -1;
        }
    }
    return 0;
}

void init_io_status(int needpor)
{
    _io_map.clear();

    if(needpor == 1) {
        for(int i=0;i<EMCMOT_MAX_USR_INIO;i++) {
            QString buf = "";
            buf.sprintf("IO_INPUT%d",i);
            _io_map[buf]=(_polarity_change.value(buf)?
            (0 == emcStatus->motion.user_defin[i]?1:0):emcStatus->motion.user_defin[i]!=0?1:0);
        }
    } else {
        for(int i=0;i<EMCMOT_MAX_USR_INIO;i++) {
            QString buf = "";
            buf.sprintf("IO_INPUT%d",i);
            _io_map[buf]=(emcStatus->motion.user_defin[i]!=0?1:0);
        }
    }

    for(int i=0;i<EMCMOT_MAX_USR_OUTIO;i++) {
        QString buf = "";
        buf.sprintf("IO_OUTPUT%d",i);
        _io_map[buf]=emcStatus->motion.user_defout[i];
    }

/*********janmi20150811*************/


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
       _real_alm_pin[35] = emcStatus->motion.user_defwarn[11]; // user define start
       _real_alm_pin[36] = emcStatus->motion.user_defwarn[12]; // user define start
       _real_alm_pin[37] = emcStatus->motion.user_defwarn[13]; // user define start
       _real_alm_pin[38] = emcStatus->motion.user_defwarn[14]; // user define start
       _real_alm_pin[39] = emcStatus->motion.user_defwarn[15]; // user define start
       _real_alm_pin[40] = emcStatus->motion.user_defwarn[16]; // user define start
       _real_alm_pin[41] = emcStatus->motion.user_defwarn[17]; // user define start
       _real_alm_pin[42] = emcStatus->motion.user_defwarn[18]; // user define start
       _real_alm_pin[43] = emcStatus->motion.user_defwarn[19]; // user define start
       _real_alm_pin[44] = emcStatus->motion.user_defwarn[20]; // user define start




       _real_alm_info[0] = "aux-estop";
       _real_alm_info[1] = "axisX-fault";
       _real_alm_info[2] = "axisY-fault";
       _real_alm_info[3] = "axisZ1-fault";
       _real_alm_info[4] = "axisZ2-fault";
       _real_alm_info[5] = "X-maxHardLimit";
       _real_alm_info[6] = "X-minHardLimit";
       _real_alm_info[7] = "Y-maxHardLimit";
       _real_alm_info[8] = "Y-minHardLimit";
       _real_alm_info[9] = "Z1-maxHardLimit";
       _real_alm_info[10] = "Z1-minHardLimit";
       _real_alm_info[11] = "Z1-maxHardLimit";
       _real_alm_info[12] = "Z2-minHardLimit";
       _real_alm_info[13] = "Cabinet-Alarm";
       _real_alm_info[14] = "Spindle0_Overcurrent";
       _real_alm_info[15] = "Spindle1_Overcurrent";
       _real_alm_info[16] = "Spindle0_Converter";
       _real_alm_info[17] = "Spindle1_Converter";
       _real_alm_info[18] = "Probe_Alarm";
       _real_alm_info[19] = "Pressure_Alarm";
       _real_alm_info[20] = "Coolant_Alarm";
       _real_alm_info[21] = "Lube_Alarm";
       _real_alm_info[22] = "Run_On";
       _real_alm_info[23] = "Pause_On";
       _real_alm_info[24] = "User_Defwarn0";
       _real_alm_info[25] = "User_Defwarn1";
       _real_alm_info[26] = "User_Defwarn2";
       _real_alm_info[27] = "User_Defwarn3";
       _real_alm_info[28] = "User_Defwarn4";
       _real_alm_info[29] = "User_Defwarn5";
       _real_alm_info[30] = "User_Defwarn6";
       _real_alm_info[31] = "User_Defwarn7";
       _real_alm_info[32] = "User_Defwarn8";
       _real_alm_info[33] = "User_Defwarn9";
       _real_alm_info[34] = "User_Defwarn10";
       _real_alm_info[35] = "User_Defwarn11";
       _real_alm_info[36] = "User_Defwarn12";
       _real_alm_info[37] = "User_Defwarn13";
       _real_alm_info[38] = "User_Defwarn14";
       _real_alm_info[39] = "User_Defwarn15";
       _real_alm_info[40] = "User_Defwarn16";
       _real_alm_info[41] = "User_Defwarn17";
       _real_alm_info[42] = "User_Defwarn18";
       _real_alm_info[43] = "User_Defwarn19";
       _real_alm_info[44] = "User_Defwarn20";


//repair IOdialog--IOwarn Info display bug
/************janmi20150811*****^*/
}

double inputdialog_get_double(QWidget *,const QString title,
                              const QString label,double default_val,
                              double min,double max,int decimals,
                              QString Ok,QString Cancel)
{
    QString str = "";
    str = QString("%1:%2 - %3").arg(label).arg(min).arg(max);
    QInputDialog *getval = new QInputDialog();
    getval->setOkButtonText(Ok);
    getval->setCancelButtonText(Cancel);
    bool ok;
    double d = getval->getDouble(getval, title,
                                           str, default_val,min,max,decimals, &ok);
    delete getval;
    if (ok)
        return d;
    else
        return 0;
}

int inputdialog_get_int(QWidget *,const QString title,
                        const QString label,int default_val,
                        int min,int max,QString ok,QString cancel)
{
    QString str = "";
    str = QString("%1:%2 - %3").arg(label).arg(min).arg(max);
    bool Ok;
    QInputDialog *getval = new QInputDialog();
    getval->setOkButtonText(ok);
    getval->setCancelButtonText(cancel);
    int d = QInputDialog::getInt(getval, title,
                                 str, default_val,min,max,1,&Ok);
    delete getval;
    if (Ok)
        return d;
    else
        return -1;
}



int insert_line_to_file(QString file,QString line)
{
    int rev = 0;
    if(_debug_flag == DEBUG_ON)
        qDebug()<<line;
    QString text = line;
    QStringList list = read_data_from_file(file).split("\n");
    text.remove("\n");
    if(2500 < list.count())
    {
        QString cmdstring = "",temp = "";
        QString infofilename = "/home/.cnc/info_bk";
        QDir dir("/home/.cnc/");
        QStringList namelist;
        namelist << "info_bk.*";
        dir.setNameFilters(namelist);
        for(int i=dir.count();i>0;i--) {

            QString fname = QString("%1.%2").arg(infofilename).arg(i);
            QString sname = QString("%1.%2").arg(infofilename).arg(i+1);
            if(i > 50) {
                cmdstring = QString("rm %1").arg(fname).arg(sname);
            } else
            cmdstring = QString("mv %1 %2").arg(fname).arg(sname);
            QFile file(fname);
            if(file.exists())
                get_system(cmdstring,&temp);

            qDebug()<<temp;
        }
        get_system("cp /home/.cnc/info /home/.cnc/info_bk.1",&temp);
        list.clear();
        list.insert(0,text);
        if(-1 == write_data_to_file(file,list.join("\n")))
            rev = -1;
        return rev;
    }
    list.insert(0,text);
    if(-1 == write_data_to_file(file,list.join("\n")))
        rev = -1;

    return rev;
}

int inter_translate_init()
{
    QTextCodec *code = QTextCodec::codecForName("UTF8");
/*
    QTextCodec *code = QTextCodec::codecForName("utf-8");
    QString file_text;
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "ERROR";
    file_text = code->toUnicode(file.read(READ_MAX_LEN));
    file.close();
*/
    int rev = 0;
    QStringList items;
    QString temp,line;
    _tran_map.clear();
    QFile file(TRANSLATE_INTER_FILE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        rev = -1;
        return rev;
    }
    //the chinese code read not be convert
    //so the Code still unrecognizable
    while(!file.atEnd()){
        temp = file.readLine();
        line = code->toUnicode(temp.toLatin1().data());
        if(line.contains("\n"))
            line.remove("\n");
        if(line.contains("=")){
            items = line.split("=");
            _tran_map[items.at(0)] = items.at(1);
        }
    }
    return rev;

}

int inter_translate(QString *str)
{
    //in this function,because the chinese code still not convert,
    //so the Code still unrecognizable
    int rev = 0;
    if(!(str->contains("(") && str->contains(")")))
        return rev;
    if(str == NULL)
        return rev;
    if(str->length() > NML_ERROR_LEN){
        rev = -1;
        return rev;
    }
    QStringList str1 = str->split("(");
    QStringList str2 = str1.at(1).split(")");
    QString text = "(";
    text.append(str2.at(0)).append(")");
    str->replace(text,_tran_map[str2.at(0)]);

    return rev;
}

int read_keys_from_file(QString filepath,QStringList *list)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)){
        printf("can read file %s\n",filepath.toLatin1().data());
        return -1;
    }

    while(!file.atEnd()){
        QString text = file.readLine();
        QStringList list_temp;
        if(text != NULL && (!text.contains("(") ||
                            !text.contains(")"))){
            list_temp = text.split(" ",QString::SkipEmptyParts);
            if(list_temp.size()>1){
                QString temp = list_temp.at(1);
                temp.remove("\n");
                list->append(temp);
            }
        }
    }
    file.close();
    return 0;
}

QString getfileName(QString caption,
                 QString dir,
                 QString filter, QWidget *parent) {

    QString filename = "";
    QFileDialog fileget(parent,caption,dir,filter);
    fileget.exec();
    filename = fileget.selectedFiles().at(0);

    QFile file(filename);
    QDir filedir;
    filedir.setPath(filename);
    if(file.exists() && !filedir.exists())
        return filename;
    else
        return "";
}


int wait_for_mode(int mode)
{
    int wait_count = 0;
    while(get_task_mode() != mode){
        updateStatus();
        if(wait_count > 100){
            wait_count = 0;
            if(_debug_flag == DEBUG_ON)
                printf("the mode is not the require mode\n");
            return -1;
        }
        usleep(10000);
        wait_count++;
    }
    return 0;
}

int wait_for_state(int state)
{
    int wait_count = 0;
    while(get_task_status() != state){
        updateStatus();
        if(wait_count > 100){
            wait_count = 0;
            if(_debug_flag == DEBUG_ON)
                printf("the state is not the require state\n");
            return -1;
        }
        usleep(10000);
        wait_count++;
    }
    return 0;
}

bool version_checkok(){
    QString verstr,soft,soft2;
    bool ret = false;

    soft2.sprintf("%02x%02x",emcStatus->motion.checkandsys[0],
            emcStatus->motion.checkandsys[1]);
    soft2 = soft2.toUpper();
    if(soft2.compare("C000") == 0)
        return true;
    verstr.sprintf("%02x%02x",emcStatus->motion.checkandsys[2],
              emcStatus->motion.checkandsys[3]);
    verstr = verstr.toUpper();


//    if(par_get_var(3999)==6){
//        soft.append("E803");
//    }else if(par_get_var(3999)==5){
//       soft.append("A809");
//    }else{
        soft.append("E801");
//     }


        if(0 == verstr.compare(soft) || 0 == verstr.compare("A809")|| 0 == verstr.compare("E809"))
        ret =  true;

    if(1 == SIMULATE)
        ret =  true;


    return ret;
}
