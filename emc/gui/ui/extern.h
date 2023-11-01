#ifndef EXTERN_H
#define EXTERN_H
#include <QString>
#include <QMap>
#include <QThread>
#include <QString>

#include <QWidget>

#include "ini_settings.h"

#define MINSCALE 0.001
#define ALM_FILE "/home/.config/alarm"
#define PREFER_FILE "/home/.config/preference"
#define KEY_FILE "/home/.cnc/gui/key"
#define MDI_HISTORY_FILE "/home/.cnc/gui/mdiHistory"
#define INFO_FILE "/home/.cnc/info"
#define TRANSLATE_INTER_FILE "/home/.cnc/gui/po"
#define EXTERN_VAR "/home/.cnc/gui/var.ini"
#define SAVE_SYSNUM "/home/.sysnum"
#define CUSTOM_IO "/home/.config/custom_io"

extern int INFO_FLAG;
#define INFO_STATUS 0X00
#define INFO_TEXT 0X01
#define INFO_WARNING 0X02
#define INFO_ERROR 0X04


#define READ_MAX_LEN 1024*1024*50
#define DEFAULT_ALM_SIG 55

typedef struct version_struct {
    char errorbuf[1024*10];
    int ethercatVersion;
    int IOboardVersion;
    int segVersion;
} version_struct;
extern version_struct *versiondata;

typedef unsigned int uint;

enum DEBUG_MODE{
  DEBUG_ON = 0,
    DEBUG_OFF
};

enum MESSAGEBOX{
    Ok = 0,
    Cancel
};

extern int _debug_flag;
extern bool _forbid_run;
extern bool _forbid_spindle;
extern bool _get_alm;
extern bool _all_homed;
extern int _doublez;
extern int _zmode;
extern QString _ncdir;

extern QString _time_now;   //  get the time now
extern QString _time_date;
extern QString _time_hour;
extern QString _ini_filepath;   //the ini filepath
extern QString _ini_filebak;    //ini备份文件
extern QString _var_filepath;   //the var filepath
extern QString _var_filebak;    //var备份文件
extern QString _zero_filepath;   //the var filepath
extern QString _zero_filebak;    //zero备份文件
extern QString _tbl_filepath;   //the var filepath
extern QString _tbl_filebak;    //tbl备份文件
extern QString _subDir;

extern Ini_Settings *_ini;
extern Ini_Settings *_prefer;
extern Ini_Settings *_var;
extern Ini_Settings *_alarm;
extern Ini_Settings *_customio;

extern int _real_alm_pin[DEFAULT_ALM_SIG]; //alm_pins real signal
extern QString _real_alm_info[DEFAULT_ALM_SIG]; //alm_pin info

extern QMap<QString,bool> _io_map;
extern QMap<QString,int> _customio_map;
extern QMap<QString,bool> _polarity_change;

extern QMap<QString,QString> _tran_map;
extern QMap<int,QString> _coor_system;

bool compare_double(double first,double second);

double par_get_var(int name);               //get the var parameters
//QString par_get_ini(const char *ini, const char *first, const char *second); //get the ini parameters
int par_write_ini(const char *ini, const char *par, char *value);   //write the ini parameters value
void init_io_status(int needpor = 0);  //map the io value
int ensure_mode(int mode);  //set the mode it needs
int cmd_system(const char *cmd);
int get_system(QString cmd,QString *ret=0);

int write_data_to_file(QString file, QString data);
QString read_data_from_file(QString);
double inputdialog_get_double(QWidget *parent,const QString title,const QString label,
                double default_val,double min,double max,int decimals, QString Ok, QString Cancel);
int inputdialog_get_int(QWidget *parent,const QString title,
                const QString label,int default_val,int min,int max, QString ok, QString cancel);
int insert_line_to_file(QString file,QString line); //inset a line in the top of file;

int read_keys_from_file(QString filepath, QStringList *list);
int inter_translate_init();
int inter_translate(QString *);

QString getfileName(QString caption,
                 QString dir,
                 QString filter,
                    QWidget *parent = 0);
int wait_for_mode(int mode);
int wait_for_state(int state);
bool version_checkok();
#endif // EXTERN_H
