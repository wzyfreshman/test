#ifndef LINUXCNC_H
#define LINUXCNC_H

#include <QMainWindow>
#include <QString>
#include <QCloseEvent>
#include <QFileSystemWatcher>
#include <QDir>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QShortcut>
#include <QProcess>
#include <QElapsedTimer>

#include "tbl_dialog.h"
#include "homedialog.h"
#include "gl_viewwidget.h"
#include "offsetdialog.h"
#include "paradialog.h"
#include "iodialog.h"
#include "codeeditor.h"
#include "io_input.h"
#include "io_output.h"
#include "info_tabledialog.h"
#include "mywidget.h"
#include "fileeditor.h"
#include "smartdialog.h"
#include "extern.h"
#include "decrypt_dialog.h"
#include "infodialog.h"
#include "smartrundialog.h"
#include "multioffset.h"
#include "labeltimer.h"
#include "toolchanger.h"
#include "gl_property.h"
#include "pickz.h"
#include "tooltimes.h"
#include "system_setup.h"
#include "datum.h"
#include "datum_parameter.h"
#include "set_feed.h"
#include "touch_checklink.h"

#include<QTcpServer>
#include<QTcpSocket>

class QProgressBar;
class CNC_Manager;
class tooltimes;
class TCHPar;
class SerialzeroPos;

namespace Ui {
class linuxcnc;
}

enum JOG_TYPE{
    JOG_CONTINUE = 0,
    JOG_HANDLE,
    JOG_INCREMENT
};

enum CNEXT_STATE {
    CNEXT_IDLE = 0,

    CNEXT_RUN_A_START,
    CNEXT_RUN_A_START_VERIFY,
    CNEXT_Y2_GO_ORIGIN,
    CNEXT_Y2_GO_ORIGIN_VERIFY,
    CNEXT_SWITCH_G54,
    CNEXT_SWITCH_G54_VERIFY,

    CNEXT_RUN_B_START,
    CNEXT_RUN_B_START_VERIFY,
    CNEXT_Y1_GO_ORIGIN,
    CNEXT_Y1_GO_ORIGIN_VERIFY,
    CNEXT_SWITCH_G55,
    CNEXT_SWITCH_G55_VERIFY,
    CNEXT_RUN_NC_B,
    CNEXT_RUN_NC_B_VERIFY,

    CNEXT_RUN_NC_A,
    CNEXT_RUN_NC_A_VERIFY
};

typedef class Alarm{
public:
    int type;
    int polarity;
    bool exist;
    QString info;
} Alarm;

class ShortCut : public QObject{
    Q_OBJECT
public:
    ShortCut(QPushButton *btn);
    void send_pressed(QPushButton *btn);
    void send_release(QPushButton *btn);
signals:
    void btn_pressed();
    void btn_released();
};

class buildInstaller : public QObject
{
    Q_OBJECT
public:
    buildInstaller() {
    }
public slots:
    void buildinstall();
signals:
    void finished(int);
};

class BackupRestore : public QObject
{
    Q_OBJECT
public:
    BackupRestore() {
    }
public slots:
    void BackupandRestore();
signals:
    void finished(int);
};

class ReadNcLineThread : public QThread
{
    Q_OBJECT
public:
    ReadNcLineThread(QObject *parent = 0)
        : QThread(parent) { reading = false; }

    bool isReading() {return reading;}
    void waitForRead() {
        mut.lock();
        mut.unlock();
    }
    void setString(const QString &string) {
        countstring = string;
    }
protected:
    void run() {
        mut.lock();
        reading = true;
        QTextDocument *textdocument = new QTextDocument(countstring);
        int file_totalline = textdocument->lineCount();
        delete textdocument;
        emit done(file_totalline);
        emit done();
        reading = false;
        mut.unlock();
    }
signals:
    void done();
    void done(int);

private:
    QString countstring;
    QMutex mut;
    bool reading;
};

class unit_sharehal;
class linuxcnc : public QMainWindow
{
    Q_OBJECT
    enum infoType {
        ININFO = 0,
        INWARNING,
        INERROR,
        STATUS
    };
public:
    explicit linuxcnc(QWidget *parent = 0);
    ~linuxcnc();

    void connect_init();
    void param_init();
    void widget_init();
    void actions_init();
    void check_alm_sig();
    void effectInit();
    void autoSwitchhandle();
    void ini_get();
    void ini_customio_get();
    int OpenNgcFile(char *file);
    QString getGCfile(){    return realpath; }

    friend class tooltimes;
    tooltimes *tool_manage;

    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist = true);

    void initCamSocket();
    int cam_exit = 0;
public slots:
    void update();
    void updateNcView(QString &);

    void openCamNC(QString file_path);
    void pro_open_slot(QString file_path);
    void pro_open(QString file_path);
    void pro_open2(QString file_path);

    void file_change(QString file);
    void set_loading_max_value(int value);
    void set_loading_value(int value);
    void set_ncfile_linecount(int c);

    /*********msg change********/
    void statusbar_message_change(QString);
    void show_msg(int,QString);

    /********set IO***************/
    void set_spindle();
    void set_lube();
    void set_flood();

    void get_goble_key(uint,uint);

    void connectNcToModel(bool);
    int getFileLineCount();
    bool isLoadingTextDisplay();
    void autoLoadModel();
    void autoLoadModel2();

    void controlUI();

    void customizationUi();
    void setapp(QApplication *App);

    void messageFromCam(int type,QString msg);

private slots:
    void coor_plus();
    void coor_decrease();
    void pro_run();
    void pro_unload();
    void pro_unload2();
    void pro_run_from(int);
    void pro_pause();
    void pro_abort();
    void set_feed_override_fun();
    void set_spindle_override_fun();
    void showTime();
    void set_feed_rate();
    void set_spindle_feed();
    void update_status();
    void clear_3d();
    void clear_work_piece();
    void clear_statusbar();
    void set_specify_feedover();
    void set_specify_spindleover();

    void setwaterstart();
    void setwaterout();

    void turnToArrayTab();
    void turnToManualTab();
    void turnToMdiTab();
    void turnToProbeTab();
    void switchMainTab(int);
    /***********mdi*********/
    void mdi_clear();
    void mdi_text_change();
    void mdi_text_send();

    /***********gl***********/
    void gl_build();

    /***********action************/
    void btn_open_file2();

    void on_action_Open_triggered();
    void on_action_Home_triggered();
    void on_actionZ1_triggered();
    void on_actionZ2_triggered();
    void on_actionZ1_Z2_triggered();
    void on_actionSet_Offset_triggered();
    void on_actionSet_Parameters_triggered();
    void on_actionSet_IO_triggered();
    void on_actionHalmeter_triggered();
    void on_actionHalconfigure_triggered();
    void on_actionHandle_guide_triggered();

//    void on_action_Custom();   //janmi 20150814

    /***********recent files*************/
    void recent_file_triggered(QAction*);

    /**********Save Coors****************/
    void save_coors_triggered(QAction*);
    void load_coors_triggered(QAction*);

    /***********tab_switch**************/
    void tab_switch(int);

    /***********shortFeed****************/
    void shortcut_feed_set(int);
    void shortcut_spindle_plus();
    void shortcut_spindle_dec();

    /***********probe*******************/
    void firstProbe();
    void secondProbe();

    /***********jog_mode****************/
    void jog_set_continue();
    void jog_set_handle();
    void jog_set_incr001();
    void jog_set_incr01();
    void jog_set_incr1();
    void jog_set_incr10();
    void jog_set_incruser();
    void jog_set_incrset();

    void show_proinfo();
    void run_warm();

    void on_actionProEdit_triggered();
    void code_actionProEdit_func2();
    void on_actionHalScope_triggered();
    void on_actionUnload_triggered();
    void on_actionVerify_triggered();
    void on_actionSmart_Control_triggered();


    void on_actionGO_Origin_triggered();
    void on_actionGO_Offset_triggered();
    void on_actionReset_triggered();
    void on_actionRepeat_triggered();
    void on_actionEdit_Tools_triggered();
    void set_offset_direct(int);
    void on_actionHistory_record_triggered();
    void on_actionShortCut_triggered();

    /***************Array*******************/
    void arraySlotOpen();
    void arraySlotBuild();
    void on_actionLimitRelease_triggered();
    bool isArrayfile(QString);
    void setArraymode(bool);
    void arraysimpleclick();
    void arraycomplexclick();

    void on_actionRegister_triggered();

    void on_actionSystem_Info_triggered();

    void on_actionUpdate_triggered();

    void on_actionRunLine_triggered();

    void switchjogspeed();

    void setprobedebug(bool);


    void on_actionBuildInstaller_triggered();

    void on_actionBackUpRestore_triggered();


    void on_actionNgcInfo_triggered();

    void on_actionWaterOut_triggered();

    void on_actionLight_triggered();

    void on_actionBackUpMasterCam_triggered();

    void on_btn_floatingprobe_clicked();

    void on_btn_gl_pro_clicked();

    void on_actionPickZ_triggered();

    void on_lockfirstprobe_toggled(bool checked);

    void on_btn_first_probe_Z3_clicked();

    void on_btn_second_probe_Z3_clicked();

    void on_actionSecondProbe_triggered();

    void on_z1firstprobe_clicked();

    void on_z2firstprobe_clicked();

    void on_z3firstprobe_clicked();

    void on_z4firstprobe_clicked();

    void on_z1secondprobe_clicked();

    void on_z2secondprobe_clicked();

    void on_z3secondprobe_clicked();

    void on_z4secondprobe_clicked();

    void on_actionZ3_triggered();

    void on_actionZ4_triggered();

    void on_actionSetZ1_triggered();

    void on_actionSetZ2_triggered();

    void on_actionSetZ3_triggered();

    void on_actionSetZ4_triggered();

    int check_zdiff();
    void change_z();

    void on_toolSeperate_toggled(bool checked);

    void on_actionAllZ_triggered();
    void on_actionMotorPar_triggered();



    void on_btn_reset_clicked();

    void on_btn_verify_clicked();

    void on_btn_repeat_clicked();

//    void on_btn_z1_clicked();

//    void on_btn_z2_clicked();

//    void on_btn_z1z2_clicked();

    void on_btn_cool_clicked();

    void on_btn_spindle_clicked();

    void on_btn_lube_clicked();

    void on_btn_handguide_clicked();

    void on_btn_light_clicked();

    void on_btn_waterout_clicked();

//    void on_btn_z3_clicked();

//    void on_btn_z4_clicked();

    void chk_cfg_files(QString filepath,QString filebak);
    void bak_cfg_files(QString filepath, QString bkfile);


    void on_btn_GO_Origin_clicked();

    void on_btn_ValveE_clicked();

    void on_btn_ValveF_clicked();


    void on_actionChinese_triggered();

    void on_actionEnglish_triggered();

    void on_btn_run_clicked();


    void on_main_tab_currentChanged(int index);

    void on_btn_set_fixed_clicked();


    void on_btn_setFeed_clicked();


    void on_btn_cyclinder_a1_released();

    void on_btn_cyclinder_b1_released();

    void on_btn_cyclinder_b2_released();

    void on_btn_cyclinder_a2_released();

    void getPanelNew();
    void sendHightSignal(int index);
    void sendLowSignal(int index);
    void sendHightSignalv2(int index);
    void sendLowSignalv2(int index);
    void sendSwitchChange();
    void SetReferencePos(int);

    void on_btn_edit_clicked();

    void on_btn_orderSave_clicked();

    void on_btn_cool2_clicked();

    void on_btn_centerwater_clicked();


    void on_useDistance_clicked(bool checked);


    void on_btn_goZero_clicked();


    void ServerNewConnection();
    void ServerReadData();
    void ServerDisConnection();

    void on_action_CAM_triggered();

    void on_chk_useoptionstop_clicked();

signals:
    void info_to_table(int,QString);
    void goble_key_get(uint,uint);
    void help_goble_key();
    void signalMdiState(QString);
    void sendMessageToCAM(int,QString);
protected:
    void closeEvent(QCloseEvent *);
    bool eventFilter(QObject *, QEvent *);
    void globalkeyPressEvent(QKeyEvent *);
    void globalkeyReleaseEvent(QKeyEvent *);
    void globalNumkeyPressEvent(QKeyEvent *);
    void globalNumkeyReleaseEvent(QKeyEvent *);
    void updateRecentAction();
    void resizeEvent(QResizeEvent *);

private:
    void initstatusbar();
    void update_coor();
    void update_ui();
    void installAllFilter();
    void updateCamWidget();

    QApplication *app;
    QTimer *flush_time_status;
    QTimer *flush_time;

    double recordspindlespeed = 0;
    double maxspindlespeed;
    double jogspeed;
    double defspeed;
    double fastspeed;
    int spindle_speed;
    int jog_mode,old_jog_mode;
    double jog_increment;
    bool jog_user_active;
    int start_verify;

    int externabortfunc;
    int now_axis;
    int work_piece;
    int ncycletimes;
    int cycled;
    int file_totalline;
    int arrayBuildmode;
    int externIoexit;
    int zmode;
    int needparflush;

    int getalarmabort;
    int air_alarm;

    int wateroutdelay;//read _ini water out delay
    int waterdelay;//read _ini water delay

    bool infoForbid;
    int firsthomeflag;//fiest return home flag   //*janmi20150807*//
    bool warming_flag;//warming over flag

    int keyboardcontrolIO1,keyboardcontrolIO2,keyboardcontrolIO3,keyboardcontrolIO4;
    int toolchangecheck[4];

    int keychoice;

    int permanent_tips;
    QString permanent_text;
    int permanentflag;


    int recordIn0,readIn0;
    int recordIn1,readIn1;
    int externEstop = 0;
    int sprate;
    int feedrate;
    int paneltype = 0;
    int optionalstop = 0;

    int pausewaitsp = 0;
    double pausespindlefeed = 0; //暂停时关主轴时的转速

    QHash<int,QAction*> k_shortcut;
    QStringList keys_read;

    myLock *runLock;

    QTcpServer *mp_TCPServer=nullptr;
    QTcpSocket *mp_TCPSocket=nullptr;


    QFrame *show_cycletimes;

    QString filepath;
    QString realpath;
    QStringList path_list;
    QString filepath2;
    QString realpath2;
//    QStringList path_list2;
    QStringList mdi_list;
    QStringList coor_list;

    HomeDialog *home;
    GL_ViewWidget *viewwidget;
    Par_Dialog *para;
    IO_Dialog *iowidget;
    CodeEditor *codeeditor,*codeeditor2;
    CodeEditor *mdieditor;
    IO_input *io_input;
    IO_output *io_output;
    Info_TableDIalog *info_table;
    QTableWidget *info_table_widget;
    MySlider *set_feed_override;
    MySlider *set_spindle_override;
    MyLabel *lab_set_feed;
    MyLabel *lab_set_spindle;
    //MyLabel *lab_set_cycletimes;
    MyLabel *lab_work_piece;
    MyLabel *lab_feed_over;
    MyLabel *lab_spindle_over;
    QLabel *T3piece;
    FileEditor *fileedit;
    FileEditor *showShortcut;
    FileEditor *showHistoryrecord;
    Datum *datum;
    QWidget *show_datum;

    QAction *defact[10];
    /********Status Bar***************/
    QHBoxLayout *vblay;
    QLabel *show_time_date;
    QLabel *show_time;
    QLabel *show_mode;
    QLabel *show_status;
    QLabel *show_info;
//    labeltimer *show_info;

    QProgressBar *loadingbar;
    QMenu *Recent_files;
    QMenu *Save_coors;
    QMenu *Load_coors;
    SmartControl *smart_control;
    SmartRunDialog *smartrunline;
    Alarm *alm_sig[DEFAULT_ALM_SIG];

    QShortcut *shortcut;

    bool loadingmodelflag;
    int jog_count;
    int btn_jog_count;
    QTextCodec *code;
    int useprobeflag;

    Tbl_Dialog *tooltabledlg;
    Decrypt_Dialog *decrypydlg;
    Infodialog *systeminfo;
    gl_property *gl_pro;
    System_setup *System_set;


    MyLabel *show_offset_x;
    MyLabel *show_offset_y,*show_offset_y_2;
    MyLabel *show_offset_z1;
    MyLabel *show_offset_a;
    MyLabel *show_offset_b;
    MyLabel *show_offset_c;

    MyLabel *show_reference_coor[9];

    QTimer *statusbar_show;

    QThread *install,*backup_t;
    buildInstaller *installer;
    BackupRestore *backuper;
    ReadNcLineThread *nclinereader, *nclinereader2;

    ToolChanger *toolchanger;

    QMessageBox *msgshow;
    multioffset *moffset;

    Ui::linuxcnc *ui;
    QTimer *watertime;
    QTimer *waterouttime;

    PickZ *pickz;
    QMainWindow* cadM;

    unit_sharehal *readhal;

    int oldsyncdi1;
    int oldsyncdi2;
    int oldsyncdi3;
    int oldsyncdi4;
    int oldsyncdi5;
    int oldsyncdi6;

    int origin;

    Datum_parameter *datumPara;
    set_feed *set_z_feed;


    QElapsedTimer timer_m;
    CNEXT_STATE cState;
    int goOriginSpeed;
    TCHPar *toolpar;
    SerialzeroPos *serialzerop;

    touch_Checklink *chklink = NULL;

    bool enable_type_1 = false;
    bool enable_type_2 = false;
    bool enable_type_3 = false;

    int useArraydistance = 0;
};







#endif // LINUXCNC_H



