#include "linuxcnc.h"
#include "shcom.hh"
#include "ui_linuxcnc.h"
#include "QTimer"
#include "emc.hh"
#include "emc_nml.hh"
#include "emcstatus.hh"
#include "_timer.h"


#include <inifile.hh>
#include "homedialog.h"
#include "offsetdialog.h"
#include "extern.h"
#include "gl_viewwidget.h"
#include "io_input.h"
#include "io_output.h"
#include "info_tabledialog.h"
#include "mywidget.h"
#include "fileeditor.h"
#include "actionfactory.h"
#include "smartdialog.h"
#include "ini_settings.h"
#include "extern.h"
#include "pickz.h"
#include "tbl_dialog.h"
#include "tooltimes.h"
#include "rwmotor.h"
#include "datum.h"
#include "datum_parameter.h"
#include "set_feed.h"
#include "unit_sharehal.h"
#include "tchpar.h"
#include "unit_serialport.h"
#include "serialzeropos.h"

#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <QVBoxLayout>
#include <QTextCodec>
#include <QTextBlock>
#include <QInputDialog>
#include <QShortcut>
#include <QProgressBar>
#include <QMessageBox>
#include <QKeySequence>
#include <QKeyEvent>
#include <QFileSystemWatcher>
#include <QTextDocument>
#include <QPropertyAnimation>
#include <QDir>
#include <QThread>
#include <QColor>
#include <QtDebug>
#include <QFileDialog>
#include <QTime>
#include <QSplitter>
#include <QDesktopWidget>
#include <QLayout>
#include <QTabBar>

#include <timer.hh>

#define _TIMEOUT_  30000

#define _STATION_A_NC_PATH_  "/home/cnext/NC/station-A.ngc"
#define _STATION_B_NC_PATH_  "/home/cnext/NC/station-B.ngc"

static double incr_user_jog = 10.0;

static int old_real_alm_pin[DEFAULT_ALM_SIG];
static bool open_while_running_flag = false;

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

static QString sendToCamString;

linuxcnc::linuxcnc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::linuxcnc)
{

    ui->setupUi(this);


    //电机通讯提示
//    if(!SIMULATE) {
//       chklink = new touch_Checklink(0);
//       QTimer::singleShot(500, chklink, SLOT(exec()));
//    }

    fileedit = new FileEditor(this);
    showShortcut = new FileEditor(this);
    showHistoryrecord = new FileEditor(this);
    _ini = new Ini_Settings(_ini_filepath);
    _prefer = new Ini_Settings(PREFER_FILE);
    _alarm = new Ini_Settings(ALM_FILE);
    _customio = new Ini_Settings(CUSTOM_IO);
    _doublez = _ini->read("PAR_RS274NGC_AXES" ,"VALUE","0").toInt();


//    this->setWindowTitle(tr("CNEXT-Aluminum"));
//    this->setWindowIcon(QIcon(":/new/prefix1/images/logo.png"));

    code = QTextCodec::codecForName("utf-8");
    //QTextCodec::setCodecForTr(code );
    //QTextCodec::setCodecForLocale(code );
    //QTextCodec::setCodecForCStrings(code );

    this->setCentralWidget(ui->mainframe);
    loadingmodelflag = false;
    tooltabledlg = NULL;
    decrypydlg  = NULL;
    systeminfo = NULL;
    smartrunline = NULL;
    iowidget = NULL;
    smart_control = NULL;
    install = NULL;
    installer = NULL;
    backuper = NULL;
    backup_t = NULL;
    gl_pro = NULL;
    pickz = NULL;

    System_set = NULL;
    datum = NULL;

    cadM = NULL;

    toolchanger = NULL;
    ncycletimes=1;
    cycled=0;
    getalarmabort = 0;
    needparflush = 0;
    toolchangecheck[0] = 0;
    toolchangecheck[1] = 0;
    toolchangecheck[2] = 0;
    toolchangecheck[3] = 0;
    permanentflag = 0;
    permanent_tips = 0;
    air_alarm = 0;

    initCamSocket();



    param_init();
    initstatusbar();
    widget_init();
//    showTime();
    actions_init();
    connect_init();

    QTimer *graphtimer= new QTimer(this);
    connect(graphtimer, SIGNAL(timeout()), this, SLOT(clear_3d()));
    graphtimer->start(_ini->read("PAR_OPERATOR_3DREFRESHTIME" ,"VALUE","0").toInt()*60000);


    flush_time_status = new QTimer(this);
    connect(flush_time_status,SIGNAL(timeout()),this,SLOT(update_status()));
    flush_time_status->start(10);

     QTimer *timer= new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
     timer->start(600);

     watertime = new QTimer(this);
     connect(watertime, SIGNAL(timeout()), this, SLOT(setwaterstart()));
     watertime->start(waterdelay*1000);

     waterouttime = new QTimer(this);
     connect(waterouttime, SIGNAL(timeout()), this, SLOT(setwaterout()));

     statusbar_show = new QTimer(this);
     connect(statusbar_show,SIGNAL(timeout()),this,SLOT(clear_statusbar()));
     show_msg(0,tr("CNC Start"));

     //ui->actionHalmeter->setEnabled(false);

     this->installEventFilter(this);
     installAllFilter();

     flush_time = new QTimer(this);
     connect(flush_time,SIGNAL(timeout()),this,SLOT(update()));
     flush_time->start(100);

     QString str = "";
     get_system("rm /home/.tmp -rf",&str);

     if(0==_ini->read("PAR_OPERATOR_WATEROUTENABLE","VALUE","0").toInt()) { //janmi 2015/8/14 add water out
         ui->actionWaterOut->setVisible(false);
         ui->btn_waterout->setVisible(false);
     }

     if(0==_ini->read("PAR_OPERATOR_EXTERNIOSHOW","VALUE","0").toInt()) {//janmi 2015/8/14 add light control
         ui->actionLight->setVisible(false);
         ui->btn_light->setVisible(false);
     }




     //setprobedebug(false);
     ui->probe_debug->hide();
     if(_zmode > 0) {
         ui->btn_first_probe_Z3->setVisible(true);
         ui->btn_second_probe_Z3->setVisible(true);
     } else {
         ui->btn_first_probe_Z3->setVisible(false);
         ui->btn_second_probe_Z3->setVisible(false);
     }
    // ui->statusBar->hide();

    controlUI();

     this->showMaximized();

     if(par_get_var(4126) > 0)
         ui->toolSeperate->setChecked(true);
     else
         ui->toolSeperate->setChecked(false);

     if(par_get_var(4070) == 0)
        ui->mainToolBar->insertAction(ui->actionReset,ui->action_Home);


     if(par_get_var(4126) > 0)
         ui->toolSeperate->setChecked(true);
     else
         ui->toolSeperate->setChecked(false);

     ui->mainToolBar->hide();

     if(emcStatus->task.g5x_index == 0) {
         sendMdi();
         sendMdiCmd("G54",1);
     }


//     customizationUi();

     //sin
//     ui->btn_light->hide();
     ui->btn_waterout->hide();
     ui->actionSecondProbe->setVisible(false);

//     ui->mode_tab->tabBar()->removeTab(3);
//     ui->mode_tab->tabBar()->removeTab(1);
//     ui->array->hide();
     ui->label_27->hide();
     ui->label_32->hide();
     ui->show_toolpiece3->hide();
     ui->show_tool_comp->hide();

     if(par_get_var(3999)==6){
         ui->btn_spindle->hide();
         ui->actionSpindle->setVisible(false);
//         ui->btn_run->setText(tr(""));
     }


     pro_open(filepath);
     if(par_get_var(3120) > 0)
         filepath2="";
     pro_open2(filepath2);

     ui->btn_cyclinder_a1->hide();
     ui->btn_cyclinder_a2->hide();
     ui->btn_cyclinder_b1->hide();
     ui->btn_cyclinder_b2->hide();
     viewwidget->setNcFileName(realpath);
//     viewwidget->clearModel();
//     viewwidget->reload();

     if(par_get_var(3120) > 0) {     //单工位
         ui->label_y_2->hide();
         ui->show_abs_y_2->hide();
         ui->show_homey_pic_2->hide();
         ui->label_y->setText("Y:");
         show_offset_y_2->hide();

         ui->btn_y_plus_2->hide();
         ui->btn_y_decrease_2->hide();

         ui->btn_y_decrease->setText("Y-");
         ui->btn_y_plus->setText("Y+");

         ui->frame_12->hide();
         ui->show_code_widget_2->hide();

     }

     ui->groupBox->hide();
     ui->lockfirstprobe->hide();
//     ui->btn_lube->hide();

    ui->mode_tab->removeTab(ui->mode_tab->indexOf(ui->tab));



}

void linuxcnc::initstatusbar()     //zhong 2015-8-10
{

    //mode
    show_mode= new QLabel(this);
    show_mode->setStyleSheet("font: 15pt Sans Serif");
    if(QApplication::desktop()->screenGeometry().height() < 780)
        show_mode->setStyleSheet("font: 12pt Sans Serif");
    show_mode->setFrameStyle(QFrame::Panel | QFrame::Sunken); //设置外观
    show_mode->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    show_mode->setMinimumWidth(120);

    //status
    show_status= new QLabel(this);
    show_status->setFrameStyle(QFrame::Panel | QFrame::Sunken); //设置外观
    show_status->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    show_status->setMinimumWidth(100);

    //info
    show_info=new QLabel(this);
    show_info->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    //loadingbar
    loadingbar = new QProgressBar();
    loadingbar ->setMinimumHeight(10);
    loadingbar->setMinimumWidth(80);
    loadingbar->hide();

    //time
    show_time = new QLabel();
    show_time_date = new QLabel();
    show_time->setStyleSheet("font: 14pt Sans Serif");
    if(QApplication::desktop()->screenGeometry().height() < 780)
        show_time->setStyleSheet("font: 11pt Sans Serif");
    show_time->setFrameShape(QFrame::Panel);
    show_time->setFrameShadow(QFrame::Sunken);
    show_time->setMinimumWidth(80);


    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(show_mode);
    topLayout->addWidget(show_status);
    topLayout->addWidget(show_info);
    topLayout->addWidget(loadingbar,1);
    topLayout->addWidget(show_time);

    topLayout->setStretchFactor(show_mode,8);
    topLayout->setStretchFactor(show_status,3);
    topLayout->setStretchFactor(show_info,90);
    topLayout->setStretchFactor(loadingbar,90);
    //topLayout->setStretchFactor(show_time,16);

    QVBoxLayout *mainLayout = new QVBoxLayout(ui->widget_status);
    mainLayout->addLayout(topLayout);
    mainLayout->setContentsMargins(0,0,0,0);
//    mainLayout->addLayout(vblay);
}


void linuxcnc::effectInit()
{
     QPropertyAnimation *animation =
             new QPropertyAnimation(this, "windowOpacity");
     animation->setDuration(1000);
     animation->setStartValue(0);
     animation->setEndValue(1);
     animation->start();
}

void linuxcnc::updateCamWidget() {
    if(0) {

        //int dh = ui->main_tab->tabBar()->height();
//        int ah = ui->cam_widget->height();
//        int height_set = ah-dh;
//        int width_set = ui->cam_widget->width();
//        cadM->setGeometry(0,dh,width_set,height_set);
//        cadM->resize(width_set,height_set);
//        cadM->show();

    }

    if(cadM) {

//        QVBoxLayout *box = new QVBoxLayout(ui->cam_widget);
//        box->setContentsMargins(2,2,2,2);
//        ui->cam_widget->setLayout(box);
//        ui->cam_widget->layout()->addWidget(cadM);

    }
}

void linuxcnc::installAllFilter()
{
    jog_count = 0;
    btn_jog_count = 0;
}

void linuxcnc::widget_init()
{
    ui->mainToolBar->setFloatable(false);
    ui->mainToolBar->setMovable(true);


    tooltabledlg = new Tbl_Dialog(this);
    QString nn = _ini->readEntry("PAR_EMCIO_TOOL_TABLE/VALUE","tool.tbl");
    QFileInfo info(nn);
    QString final = info.absoluteDir().absolutePath();
    final = QString("%1/%2").arg(final).arg(nn);
    tooltabledlg->setTblFile(final);
    tooltabledlg->loadTool();
    tool_manage = tooltabledlg->toolmanager;


    /*********************************janmi 20150814
    ui->mainToolBar->addSeparator();
    QString defactname;
    for(int i=0;i<10;i++)
    {
        defactname=QString("btn%1").arg(i);
        defact[i]=new QAction(defactname,this);
        defact[i]->setIcon(QIcon(":/new/prefix1/images/greenlight.xpm"));
         ui->mainToolBar->addAction(defact[i]);
         connect(defact[i],SIGNAL(triggered()),this,SLOT(on_action_Custom()));
    }

******************************************************/
    ui->mainToolBar->show();

/*
    QShortcut *shortcut1 = new QShortcut(QKeySequence("F9"),ui->btn_run);
    shortcut1->setAutoRepeat(false);
    connect(shortcut1,SIGNAL(activated()),ui->btn_run,SIGNAL(pressed()));

    QShortcut *shortcut3 = new QShortcut(QKeySequence("F10"),ui->btn_pause);
    shortcut3->setAutoRepeat(false);
    connect(shortcut3,SIGNAL(activated()),ui->btn_pause,SIGNAL(pressed()));
    QShortcut *shortcut2 = new QShortcut(QKeySequence("F11"),ui->btn_abort);
    shortcut2->setAutoRepeat(false);
    connect(shortcut2,SIGNAL(activated()),ui->btn_abort,SIGNAL(pressed()));
*/

//    for(int i=0;(i<k_shortcut.size() && i<keys_read.size());i++){
//        k_shortcut.value(i)->setAutoRepeat(false);
//        k_shortcut.value(i)->setShortcutContext(Qt::ApplicationShortcut);
//        k_shortcut.value(i)->setShortcut(QKeySequence(keys_read.at(i)));
//    }





    ui->btn_gl_setp->setToolTip(tr("D_solid_map"));
    ui->btn_gl_fontview->setToolTip(tr("positive_view"));
    ui->btn_gl_sideview->setToolTip(tr("side_view"));
    ui->btn_gl_viewz->setToolTip(tr("top_view_1"));
    ui->btn_gl_zoomin->setToolTip(tr("blow_up"));
    ui->btn_gl_zoomout->setToolTip(tr("narrow"));
    ui->btn_gl_clear->setToolTip(tr("gl_clear"));
    ui->btn_gl_build->setToolTip(tr("gl_build"));
    ui->btn_gl_abort->setToolTip(tr("gl_abort"));
    ui->btn_gl_pro->setToolTip(tr("gl_property"));

    ui->main_tab->setFocusPolicy(Qt::NoFocus);
    ui->mode_tab->setFocusPolicy(Qt::NoFocus);


    QPalette showstatus;
    showstatus.setColor(QPalette::WindowText,Qt::black);


    datum = new Datum(this);
    show_datum = new QWidget(this);
    QVBoxLayout *Datumlay = new QVBoxLayout(show_datum) ;
    Datumlay->addWidget(datum);
    show_datum->hide();

    //init the parameter
    para = new Par_Dialog(0);


    //setoffsetdlg = new OffsetDialog(this);
    moffset = new multioffset(this);

    datumPara = new Datum_parameter(this);
    connect(datumPara,SIGNAL(show_msg(int,QString)),this,SLOT(show_msg(int,QString)));

    set_z_feed = new set_feed(this);
    connect(set_z_feed,SIGNAL(show_msg(int,QString)),this,SLOT(show_msg(int,QString)));

    //Init the OpenGL Widget
    QVBoxLayout *lay = new QVBoxLayout(this->ui->glshowwidget);
    viewwidget = new GL_ViewWidget(this->ui->glshowwidget,_ini_filepath);
    viewwidget->setMainWindow(this);
    lay->addWidget(viewwidget);
    lay->setContentsMargins(0,0,0,0);

    /************init the info table************/
    info_table = new Info_TableDIalog(this);
    QVBoxLayout *info_table_lay = new QVBoxLayout(ui->tab_info);
    info_table_lay->addWidget(info_table);
    info_table_lay->setContentsMargins(0,0,0,0);

    /***********mdi*************/
    QVBoxLayout *mdi_show_lay = new QVBoxLayout(ui->show_mdi_widget);
    mdieditor = new CodeEditor(this);
    mdi_show_lay->addWidget(mdieditor);
    mdi_show_lay->setContentsMargins(0,0,0,0);
    if(!mdi_list.isEmpty())
        mdieditor->setPlainText(mdi_list.join(""));
    mdieditor->menu_type = 1;
    mdieditor->setReadOnly(true);
    mdieditor->loading = false;
     write_data_to_file(MDI_HISTORY_FILE,"");

    /************input_io***********/
    QVBoxLayout *io_input_lay = new QVBoxLayout(ui->tab_4);
    io_input = new IO_input(this);
    io_input_lay->addWidget(io_input);
    /************output_io***********/
    QVBoxLayout *io_output_lay = new QVBoxLayout(ui->tab_5);
    io_output = new IO_output(this);
    io_output_lay->addWidget(io_output);

    /***********recent_file menu****************/
    Recent_files = ui->menu_file->addMenu(tr("Recent File"));
    updateRecentAction();
    connect(Recent_files,SIGNAL(triggered(QAction*)),this,SLOT(recent_file_triggered(QAction*)));

    /*******************code***************/
    QVBoxLayout *code_show_lay = new QVBoxLayout(ui->show_code_widget);
    codeeditor = new CodeEditor(this);
    connect(codeeditor,SIGNAL(ed_load_done()),this,SLOT(autoLoadModel()));
    codeeditor->setReadOnly(true);
    code_show_lay->addWidget(codeeditor);
    code_show_lay->setContentsMargins(0,0,0,0);
    codeeditor->setLineWrapMode(QPlainTextEdit::NoWrap);
    codeeditor->menu_type = 0;
    codeeditor->setCenterOnScroll(true);
    codeeditor->setFocusPolicy(Qt::NoFocus);


    QVBoxLayout *code_show_lay2 = new QVBoxLayout(ui->show_code_widget_2);
    codeeditor2 = new CodeEditor(this);
    connect(codeeditor2,SIGNAL(ed_load_done()),this,SLOT(autoLoadModel()));
    codeeditor2->setReadOnly(true);
    code_show_lay2->addWidget(codeeditor2);
    code_show_lay2->setContentsMargins(0,0,0,0);
    codeeditor2->setLineWrapMode(QPlainTextEdit::NoWrap);
    codeeditor2->menu_type = 0;
    codeeditor2->setCenterOnScroll(true);
    codeeditor2->setFocusPolicy(Qt::NoFocus);




    /****************override***********************/
    set_feed_override = new MySlider(CNC::SliderFeedOverride);
    QVBoxLayout *feed_override_lay = new QVBoxLayout(ui->feed_override_widget);
    feed_override_lay->addWidget(set_feed_override);

    QString feed_override_max_temp = _ini->read("PAR_DISPLAY_MAX_FEED_OVERRIDE","VALUE","1.2");
    set_feed_override->setMaximum((int)(feed_override_max_temp.toDouble()*100+0.5));
    set_feed_override->setFocusPolicy(Qt::NoFocus);


    lab_set_feed = new MyLabel(0);
    QVBoxLayout *lab_feed_lay = new QVBoxLayout(ui->show_set_feed);
    lab_feed_lay->addWidget(lab_set_feed);
    lab_feed_lay->setContentsMargins(0,0,0,0);
    lab_set_feed->setAlignment(Qt::AlignRight);
    lab_set_feed->setAlignment(Qt::AlignVCenter);
    set_spindle_override = new MySlider(CNC::SliderSpindleOverride);
    QVBoxLayout *spindle_override_lay = new QVBoxLayout(ui->spindle_override_widget);
    spindle_override_lay->addWidget(set_spindle_override);
    QString spindle_override_max_temp = _ini->read("PAR_DISPLAY_MAX_SPINDLE_OVERRIDE","VALUE","1.0");
    set_spindle_override->setMaximum((int)(spindle_override_max_temp.toDouble()*100+0.5));
    set_spindle_override->setFocusPolicy(Qt::NoFocus);

    lab_set_spindle = new MyLabel(0);
    lab_set_spindle->setAlignment(Qt::AlignRight);
    lab_set_spindle->setAlignment(Qt::AlignVCenter);

    QVBoxLayout *lab_spindle_lay = new QVBoxLayout(ui->show_set_spindle);
    lab_spindle_lay->addWidget(lab_set_spindle);
    lab_spindle_lay->setContentsMargins(0,0,0,0);



    lab_work_piece = new MyLabel(0);
//    QHBoxLayout *lay_work_piece = new QHBoxLayout(ui->show_piece_frame);
//    lay_work_piece->addWidget(lab_work_piece);
//    lay_work_piece->setContentsMargins(0,0,0,0);
//    lab_work_piece->setText(QString::number(work_piece,10));


    T3piece = new QLabel(0);
    QHBoxLayout *lay_work_piece3 = new QHBoxLayout(ui->show_toolpiece3);
    lay_work_piece3->addWidget(T3piece);
    lay_work_piece3->setContentsMargins(0,0,0,0);

    lab_feed_over = new MyLabel(0);
    QVBoxLayout *lab_feedover_lay = new QVBoxLayout(ui->show_feed_override);
    lab_feedover_lay->addWidget(lab_feed_over);
    lab_feedover_lay->setContentsMargins(0,0,0,0);
    lab_spindle_over = new MyLabel(0);
    QVBoxLayout *lab_spindleover_lay = new QVBoxLayout(ui->show_spindle_override);
    lab_spindleover_lay->addWidget(lab_spindle_over);
    lab_spindleover_lay->setContentsMargins(0,0,0,0);


    /***cord label**/
    show_offset_x = new MyLabel("0.000");
    show_offset_y = new MyLabel("0.000");
    show_offset_y_2 = new MyLabel("0.000");
    show_offset_z1 = new MyLabel("0.000");
    show_offset_a = new MyLabel("0.000");
    show_offset_c = new MyLabel("0.000");
    show_offset_b = new MyLabel("0.000");

    show_offset_x->setTextFormat(Qt::AutoText);
    show_offset_x->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    show_offset_x->setFrameStyle(QFrame::Sunken);
    show_offset_x->setFrameShadow(QFrame::Plain);
    show_offset_x->setAxis(AXIS_X);

    show_offset_y->setTextFormat(Qt::AutoText);
    show_offset_y->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    show_offset_y->setFrameStyle(QFrame::Sunken);
    show_offset_y->setFrameShadow(QFrame::Plain);
    show_offset_y->setAxis(AXIS_Y);

    show_offset_y_2->setTextFormat(Qt::AutoText);
    show_offset_y_2->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    show_offset_y_2->setFrameStyle(QFrame::Sunken);
    show_offset_y_2->setFrameShadow(QFrame::Plain);
    show_offset_y_2->setAxis(AXIS_Y2);

    show_offset_z1->setTextFormat(Qt::AutoText);
    show_offset_z1->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    show_offset_z1->setFrameStyle(QFrame::Sunken);
    show_offset_z1->setFrameShadow(QFrame::Plain);
    show_offset_z1->setAxis(AXIS_Z1);

    show_offset_a->setTextFormat(Qt::AutoText);
    show_offset_a->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    show_offset_a->setFrameStyle(QFrame::Sunken);
    show_offset_a->setFrameShadow(QFrame::Plain);
    show_offset_a->setAxis(AXIS_A);

    show_offset_b->setStyleSheet("font: 15pt Sans Serif");
    show_offset_b->setTextFormat(Qt::AutoText);
    show_offset_b->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    show_offset_b->setFrameStyle(QFrame::Sunken);
    show_offset_b->setFrameShadow(QFrame::Plain);
    show_offset_b->setAxis(AXIS_B);

    show_offset_c->setStyleSheet("font: 15pt Sans Serif");
    show_offset_c->setTextFormat(Qt::AutoText);
    show_offset_c->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    show_offset_c->setFrameStyle(QFrame::Sunken);
    show_offset_c->setFrameShadow(QFrame::Plain);
    show_offset_c->setAxis(AXIS_C);

    ui->cordgridLayout->addWidget(show_offset_x,1,3);
    ui->cordgridLayout->addWidget(show_offset_y,2,3);
    ui->cordgridLayout->addWidget(show_offset_y_2,3,3);
    ui->cordgridLayout->addWidget(show_offset_z1,4,3);

    ui->cordgridLayout->addWidget(show_offset_a,5,3);
    ui->cordgridLayout->addWidget(show_offset_b,6,3);
    ui->cordgridLayout->addWidget(show_offset_c,7,3);

    for(int i=0;i<9;i++) {
        show_reference_coor[i] = new MyLabel("0.000");
        show_reference_coor[i]->setAxis((AXIS_NAME)i);
        show_reference_coor[i]->setStyleSheet("font: 15pt Sans Serif");
        show_reference_coor[i]->setTextFormat(Qt::AutoText);
        show_reference_coor[i]->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        connect(show_reference_coor[i],SIGNAL(leftClick(int)),this,SLOT(SetReferencePos(int)));
    }

    ui->cordgridLayout->setHorizontalSpacing(5);


    ui->cordgridLayout->addWidget(show_reference_coor[0],1,4);
    ui->cordgridLayout->addWidget(show_reference_coor[1],2,4);
//    ui->cordgridLayout->addWidget(show_reference_coor[6],3,4);
    ui->cordgridLayout->addWidget(show_reference_coor[2],4,4);
//    ui->cordgridLayout->addWidget(show_reference_coor[3],5,4);
//    ui->cordgridLayout->addWidget(show_reference_coor[5],7,4);

//    ui->cordgridLayout->setColumnStretch(0,2);
//    ui->cordgridLayout->setColumnStretch(1,1);
//    ui->cordgridLayout->setColumnStretch(2,3);
//    ui->cordgridLayout->setColumnStretch(3,3);
//    ui->cordgridLayout->setColumnStretch(4,2);
//    ui->cordgridLayout->setColumnStretch(5,2);
//    ui->cordgridLayout->setColumnStretch(6,1);
//    ui->cordgridLayout->setColumnStretch(7,3);
//    ui->cordgridLayout->setColumnStretch(8,3);
//    ui->cordgridLayout->setColumnStretch(9,1);

    readhal = new unit_sharehal();

    runLock = new myLock();
    nclinereader = new ReadNcLineThread(this);
    connect(nclinereader,SIGNAL(done()),this,SLOT(autoLoadModel()));

    nclinereader2  = new ReadNcLineThread(this);
    connect(nclinereader2,SIGNAL(done()),this,SLOT(autoLoadModel2()));


    msgshow = new QMessageBox(this);
    msgshow->setWindowTitle(tr("info"));
    ui->mode_tab->setCurrentIndex(0);

    int usetoolchanger = _ini->read("PAR_RS274_USETOOLCHANGER" ,"VALUE","0").toInt();
    sendSetParameter(4299,usetoolchanger);
   if(usetoolchanger > 0) {
       QWidget *newwidget = new QWidget();
        int index = ui->mode_tab->addTab(newwidget,tr("手动换刀"));
         toolchanger = new ToolChanger(this);
        QWidget *newtab = ui->mode_tab->widget(index);
        QVBoxLayout *lay_newtab = new QVBoxLayout(newtab);
        lay_newtab->addWidget(toolchanger);
        lay_newtab->setContentsMargins(0,0,0,0);
        connect(toolchanger,SIGNAL(send_msg(int,QString)),this,SLOT(show_msg(int,QString)));

        QWidget *tchwidget = new QWidget();
        index = ui->main_tab->addTab(tchwidget,tr("刀库参数"));
        QWidget *tchtab = ui->main_tab->widget(index);
        QVBoxLayout *lay_toolpar = new QVBoxLayout(tchtab);
        toolpar = new TCHPar(this);
        lay_toolpar->addWidget(toolpar);
        connect(toolpar,SIGNAL(showMsg(int,QString)),this,SLOT(show_msg(int,QString)));
        lay_toolpar->setContentsMargins(0,0,0,0);

    } else {
       ui->btn_b_decrease->hide();
       ui->btn_b_plus->hide();
       ui->btn_c_decrease->hide();
       ui->btn_c_plus->hide();
       sendSetParameter(4127,1);        //关闭换刀功能
       sendSetParameter(4290,0);        //取消换刀的程序切换
       sendSetParameter(4227,0);        //取消第二限位
       sendMultiFunction(20,0,0);
   }

   //设置偏置放出来
//   QWidget *SetOffset = new QWidget();
//    int index = ui->mode_tab->addTab(SetOffset,tr("设置偏置"));
//    QPushButton *setOffset = new QPushButton("设置偏置");
//    QWidget *newtaboffset = ui->mode_tab->widget(index);
//    QVBoxLayout *lay_newtaboffset = new QVBoxLayout(newtaboffset);
//    lay_newtaboffset->addWidget(setOffset);
//    lay_newtaboffset->setContentsMargins(100,0,100,0);
//    connect(setOffset,SIGNAL(released()),this,SLOT(on_actionSet_Offset_triggered()));
//    setOffset->setMinimumSize(200,200);
//    setOffset->setMaximumSize(150,150);

   useprobeflag = _ini->read("PAR_OPERATOR_USEPROBE","VALUE","1").toInt();
   if(0 == useprobeflag)
   {
       ui->gBox_fixed_probe->setVisible(false);
       ui->gBox_floating_probe->setVisible(false);
   }else if(1 == useprobeflag){
       ui->gBox_floating_probe->setVisible(false);
   }else if(2 == useprobeflag){
       ui->gBox_fixed_probe->setVisible(false);
   }

   if(_doublez < 4){
       ui->btn_a_plus->setText("");
       ui->btn_a_decrease->setText("");
   }

   ui->lockfirstprobe->setChecked(_prefer->read("LOCKPROBE","VALUE").toInt());


   if(_ini->read("PAR_HOME_USE_ABSPOS","VALUE","0").toInt() > 0) {
       serialzerop = new SerialzeroPos();
       connect(serialzerop,SIGNAL(show_msg(int,QString)),this,SLOT(show_msg(int,QString)));
   } else {
       home = new HomeDialog();
       connect(home,SIGNAL(send_msg(int,QString)),this,SLOT(show_msg(int,QString)));
   }

   ui->frame_5->setVisible(_ini->read("PAR_GUI_HIDE_RUN_PANEL","VALUE","0").toInt() == 0);

}

int linuxcnc::OpenNgcFile(char *file) {
    if(1 == air_alarm){
        show_msg(2,tr("Alarm status can not run"));
        return -1;
    }

    int count = 1;
    sendProgramOpen(file);
    while(0 != strcmp(file,emcStatus->task.file)) {
        sendProgramOpen(file);
        count++;
        usleep(1000*40);
        updateStatus();
        if(count > 5) {
            show_msg(2,tr("now cannot read the correct file"));
            return -1;
        }
    }
    return 0;
}
void linuxcnc::ini_customio_get()
{

}

void linuxcnc::ini_get()
{
    static QString jogspeedget = "";
    QString jogspeedtemp = _ini->read("PAR_TRAJ_DEFAULT_VELOCITY" ,"VALUE","30");
    if(0 != jogspeedget.compare(jogspeedtemp)) {
        defspeed = (double)jogspeedtemp.toDouble()*60+0.5;
        jogspeed = (double)jogspeedtemp.toDouble()*60+0.5;
        jogspeedget = jogspeedtemp;
    }
    static QString fastspeedget = "";
    QString fastspeedtemp = _ini->read("PAR_TRAJ_HIGH_VELOCITY" ,"VALUE","150");
    if(0 != fastspeedget.compare(fastspeedtemp)) {
        fastspeed = fastspeedtemp.toDouble()*60;
        fastspeedget = fastspeedtemp;
    }
    static QString spindlespeedget = "";
    QString spindlespeedgtemp = _ini->read("PAR_TRAJ_DEFAULT_SPINDLE_SPEED" ,"VALUE","50000");
    if(0 != spindlespeedget.compare(spindlespeedgtemp)) {
        spindle_speed = spindlespeedgtemp.toDouble();
        spindlespeedget = spindlespeedgtemp;
    }

    maxspindlespeed = _ini->read("PAR_TRAJ_MAX_SPINDLE_SPEED","VALUE","60000").toDouble();
    INFO_FLAG = _ini->read("PAR_DISPLAY_INFO" ,"VALUE","7").toInt();
    externabortfunc = _ini->read("PAR_OPERATOR_ABORTFUNC","VALUE","0").toInt();
    _zmode = _ini->read("PAR_RS274NGC_Z_MODE","VALUE","0").toInt();
    externIoexit = _ini->read("PAR_TASK_EXTERN_IO","VALUE","0").toInt();
    zmode = _ini->read("PAR_RS274NGC_Z_MODE","VALUE","0").toInt();

    waterdelay = _ini->read("PAR_OPERATOR_WATERDELAY","VALUE","20").toInt();
    wateroutdelay = _ini->read("PAR_OPERATOR_WATEROUTDELAY","VALUE","3").toInt();

    if( 0 == _ini->read("PAR_OPERATOR_NC_OPEN_PATH_FLAG","VALUE","1").toInt())
         _ncdir = "";
    else
        _ncdir = _ini->read("PAR_DISPLAY_NCDIR","VALUE","/home/cnext/NC/");

    keychoice = _ini->read("PAR_OPERATOR_KEYCHOICE","VALUE","0").toInt();

    goOriginSpeed = _ini->read("PAR_TASK_SPEED_OF_MOVE","VALUE","6000").toInt();

    useArraydistance = _prefer->read("DEFAULT","useArraydistance","0").toInt();

//    static QString arrayBuildmodeget = "";
//    QString arrayBuildmodetemp = _ini->read("PAR_OPERATOR_ARRAY_BUILDMODE" ,"VALUE","0",true);
//    if(0 != arrayBuildmodeget.compare(arrayBuildmodetemp)) {
//        arrayBuildmode = arrayBuildmodetemp.toInt();
//        if(arrayBuildmode) {
//            ui->arraysimple->setChecked(true);
//            ui->arraycomplex->setChecked(false);
//            setArraymode(0);
//        } else {
//            ui->arraycomplex->setChecked(true);
//            ui->arraysimple->setChecked(false);
//            setArraymode(1);
//        }
//        arrayBuildmodeget = arrayBuildmodetemp;
//    }
    setArraymode(1);    //默认循环模式
    ui->arraycomplex->hide();
    ui->arraysimple->hide();
    ui->label_28->hide();
    ui->label_31->hide();
}

void linuxcnc::param_init()
{
    PassWord::initKey();

    sendManual();
    ini_get();
    jog_mode = JOG_CONTINUE;
    old_jog_mode = JOG_CONTINUE;
    jog_increment = 1.0;
    jog_user_active = false;
    _debug_flag = DEBUG_ON;
    start_verify = 0;
    infoForbid = false;
    firsthomeflag=0;  //janmi20150807
    warming_flag=false;
    ncycletimes=0;
    cycled=0;
    keyboardcontrolIO1 = 0;
    keyboardcontrolIO2 = 0;
    keyboardcontrolIO3 = 0;
    keyboardcontrolIO4 = 0;
    cState = CNEXT_IDLE;





    emcWaitType = EMC_WAIT_DONE;
    emcTimeout = 0.2;
    sendSetParameter(4050,0);

    coor_list.clear();
    for(int i=0;i<10;i++) {
        QString temp = "coor";
        temp.append(QString::number(i+1,10));
        QString value = QString("%1:").arg(i+1);
        if(_prefer->read("SAVE_COORS",
                       temp) == NULL)
            value.append("null (0 0 0) null");
        else
            value.append(_prefer->read("SAVE_COORS",temp,"0"));
        coor_list.append(value);
    }

    path_list.clear();
    bool firstFileExist = false;
    for(int i=0;i<10;i++) {
        QString temp = "path";
        temp.append(QString::number(i,10));
        QString filename = _prefer->read("FILE_PATH",temp,"0");
        QFile file(filename);

        if(file.exists()) {
            path_list.append(filename);
            if(i == 0)
                firstFileExist = true;
        }

    }

    filepath2 = _prefer->read("FILE_PATH2","path");

    if( path_list.size() >0 ) {
        if(firstFileExist)
            filepath = path_list.at(0);
        else
            filepath.clear();
    }

    mdi_list.clear();
    QFile mdifile(MDI_HISTORY_FILE);
    if(mdifile.open(QIODevice::ReadOnly)){
        while(!mdifile.atEnd()){
            mdi_list.append(mdifile.readLine());
        }
        mdifile.close();
    }

    for(int i=0;i<DEFAULT_ALM_SIG;i++){
        QString temp;
        temp = QString("WARNING_%1").arg(i);
        alm_sig[i] = new Alarm();
        alm_sig[i]->info = _alarm->read(temp,"info","0");
        if( _alarm->read(temp,"polarity","0").toInt() != 1)
            alm_sig[i]->polarity = 0;
        else
            alm_sig[i]->polarity = 1;
        alm_sig[i]->exist = _alarm->read(temp,"exist","0").toInt();
        alm_sig[i]->type = strtol(_alarm->read(temp,"type","0").toLatin1().data(),NULL,16);

    }


    work_piece = _prefer->read("DEFAULT","work_piece","0",true).toInt();

    paneltype = _ini->read("PAR_OPERATOR_PANEALTYPE","VALUE",0).toInt();

    keys_read.clear();
    read_keys_from_file(KEY_FILE,&keys_read);

    _coor_system[1]="G54";
    _coor_system[2]="G55";
    _coor_system[3]="G56";
    _coor_system[4]="G57";
    _coor_system[5]="G58";
    _coor_system[6]="G59";

    //shortcut key
    QList<QAction *> actionlist;
    actionlist.append(ui->action_Home);
    actionlist.append(ui->actionGO_Offset);
    actionlist.append(ui->actionGO_Origin);
    actionlist.append(ui->actionSpindle);
    actionlist.append(ui->actionCool);
    actionlist.append(ui->actionLube);
    actionlist.append(ui->action_CAM);
    actionlist.append(ui->actionZ2);
    actionlist.append(ui->actionZ3);
    actionlist.append(ui->actionZ4);
    actionlist.append(ui->actionZ1_Z2);
    actionlist.append(ui->actionHandle_guide);
    actionlist.append(ui->actionSmart_Control);
    actionlist.append(ui->actionProEdit);
    actionlist.append(ui->action_Open);
    actionlist.append(ui->actionUnload);
    actionlist.append(ui->actionVerify);
    actionlist.append(ui->actionSet_IO);
    actionlist.append(ui->actionSet_Offset);
    actionlist.append(ui->actionSet_Parameters);
    actionlist.append(ui->actionSystem_Info);
    actionlist.append(ui->actionHistory_record);
    actionlist.append(ui->actionHalconfigure);
    actionlist.append(ui->actionHalmeter);
    actionlist.append(ui->actionHalScope);

    actionlist.append(ui->actionEdit_Tools);
    actionlist.append(ui->actionRepeat);
    actionlist.append(ui->actionRunLine);

    actionlist.append(ActionFactory::instance()->getAction(CNC::ActionTrigleManual));
    actionlist.append(ActionFactory::instance()->getAction(CNC::ActionTrigleMdi));
    actionlist.append(ActionFactory::instance()->getAction(CNC::ActionTrigleProbe));

    actionlist.append(ActionFactory::instance()->getAction(CNC::ActionTrigleCoor));
    actionlist.append(ActionFactory::instance()->getAction(CNC::ActionTrigleModel));
    actionlist.append(ActionFactory::instance()->getAction(CNC::ActionTrigleInfo));
    actionlist.append(ActionFactory::instance()->getAction(CNC::ActionTrigleIoIn));
    actionlist.append(ActionFactory::instance()->getAction(CNC::ActionTrigleIoOut));
    actionlist.append(ActionFactory::instance()->getAction(CNC::ActionTrigleArray));


    optionalstop = _ini->read("PAR_TASK_OPTIONSTOP","VALUE","0").toInt();

//    useprobeflag = _ini->read("PAR_OPERATOR_USEPROBE","VALUE","1").toInt();
}

void linuxcnc::actions_init()
{
    ActionFactory *factory = ActionFactory::instance();
    factory->createAction(CNC::ActionViewX,viewwidget,NULL);
    factory->createAction(CNC::ActionViewY,viewwidget,NULL);
    factory->createAction(CNC::ActionViewP,viewwidget,NULL);
    factory->createAction(CNC::ActionViewZ,viewwidget,NULL);
    factory->createAction(CNC::ActionViewZ2,viewwidget,NULL);
    factory->createAction(CNC::ActionPathClear,viewwidget,NULL);
    factory->createAction(CNC::ActionModelReload,viewwidget,NULL);
    factory->createAction(CNC::ActionZoomIn,viewwidget,NULL);
    factory->createAction(CNC::ActionZoomOut,viewwidget,NULL);
    factory->createAction(CNC::ActionLoadAbort,viewwidget,NULL);
    factory->createAction(CNC::ActionSaveCoor1,NULL,NULL);
    factory->createAction(CNC::ActionSaveCoor2,NULL,NULL);
    factory->createAction(CNC::ActionSaveCoor3,NULL,NULL);
    factory->createAction(CNC::ActionSaveCoor4,NULL,NULL);
    factory->createAction(CNC::ActionSaveCoor5,NULL,NULL);
    factory->createAction(CNC::ActionSaveCoor6,NULL,NULL);
    factory->createAction(CNC::ActionSaveCoor7,NULL,NULL);
    factory->createAction(CNC::ActionSaveCoor8,NULL,NULL);
    factory->createAction(CNC::ActionSaveCoor9,NULL,NULL);
    factory->createAction(CNC::ActionSaveCoor10,NULL,NULL);

    factory->createAction(CNC::ActionLoadCoor1,NULL,NULL);
    factory->createAction(CNC::ActionLoadCoor2,NULL,NULL);
    factory->createAction(CNC::ActionLoadCoor3,NULL,NULL);
    factory->createAction(CNC::ActionLoadCoor4,NULL,NULL);
    factory->createAction(CNC::ActionLoadCoor5,NULL,NULL);
    factory->createAction(CNC::ActionLoadCoor6,NULL,NULL);
    factory->createAction(CNC::ActionLoadCoor7,NULL,NULL);
    factory->createAction(CNC::ActionLoadCoor8,NULL,NULL);
    factory->createAction(CNC::ActionLoadCoor9,NULL,NULL);
    factory->createAction(CNC::ActionLoadCoor10,NULL,NULL);
    /**********************Save Coor*******************************/
        Save_coors = new QMenu(this);
        ActionFactory *geta = ActionFactory::instance();
        QAction *action1 = geta->getAction(CNC::ActionSaveCoor1);
        action1->setText(coor_list.at(0));
        Save_coors->addAction(action1);
        QAction *action2 = geta->getAction(CNC::ActionSaveCoor2);
        action2->setText(coor_list.at(1));
        Save_coors->addAction(action2);
        QAction *action3 = geta->getAction(CNC::ActionSaveCoor3);
        action3->setText(coor_list.at(2));
        Save_coors->addAction(action3);
        QAction *action4 = geta->getAction(CNC::ActionSaveCoor4);
        action4->setText(coor_list.at(3));
        Save_coors->addAction(action4);
        QAction *action5 = geta->getAction(CNC::ActionSaveCoor5);
        action5->setText(coor_list.at(4));
        Save_coors->addAction(action5);
        QAction *action6 = geta->getAction(CNC::ActionSaveCoor6);
        action6->setText(coor_list.at(5));
        Save_coors->addAction(action6);
        QAction *action7 = geta->getAction(CNC::ActionSaveCoor7);
        action7->setText(coor_list.at(6));
        Save_coors->addAction(action7);
        QAction *action8 = geta->getAction(CNC::ActionSaveCoor8);
        action8->setText(coor_list.at(7));
        Save_coors->addAction(action8);
        QAction *action9 = geta->getAction(CNC::ActionSaveCoor9);
        action9->setText(coor_list.at(8));
        Save_coors->addAction(action9);
        QAction *action10 = geta->getAction(CNC::ActionSaveCoor10);
        action10->setText(coor_list.at(9));
        Save_coors->addAction(action10);
        ui->actionSave_Offset->setMenu(Save_coors);

        Load_coors = new QMenu(this);

        QAction *actionload1 = geta->getAction(CNC::ActionLoadCoor1);
        actionload1->setText(coor_list.at(0));
        Load_coors->addAction(actionload1);
        QAction *actionload2 = geta->getAction(CNC::ActionLoadCoor2);
        actionload2->setText(coor_list.at(1));
        Load_coors->addAction(actionload2);
        QAction *actionload3 = geta->getAction(CNC::ActionLoadCoor3);
        actionload3->setText(coor_list.at(2));
        Load_coors->addAction(actionload3);
        QAction *actionload4 = geta->getAction(CNC::ActionLoadCoor4);
        actionload4->setText(coor_list.at(3));
        Load_coors->addAction(actionload4);
        QAction *actionload5 = geta->getAction(CNC::ActionLoadCoor5);
        actionload5->setText(coor_list.at(4));
        Load_coors->addAction(actionload5);
        QAction *actionload6 = geta->getAction(CNC::ActionLoadCoor6);
        actionload6->setText(coor_list.at(5));
        Load_coors->addAction(actionload6);
        QAction *actionload7 = geta->getAction(CNC::ActionLoadCoor7);
        actionload7->setText(coor_list.at(6));
        Load_coors->addAction(actionload7);
        QAction *actionload8 = geta->getAction(CNC::ActionLoadCoor8);
        actionload8->setText(coor_list.at(7));
        Load_coors->addAction(actionload8);
        QAction *actionload9 = geta->getAction(CNC::ActionLoadCoor9);
        actionload9->setText(coor_list.at(8));
        Load_coors->addAction(actionload9);
        QAction *actionload10 = geta->getAction(CNC::ActionLoadCoor10);
        actionload10->setText(coor_list.at(9));
        Load_coors->addAction(actionload10);
        ui->actionLoad_Offset->setMenu(Load_coors);
}

void linuxcnc::connect_init()
{
    connect(datum,SIGNAL(send_msg(int,QString)),this,SLOT(show_msg(int,QString)));

    connect(ui->btn_x_plus,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_x_plus,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_y_plus,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_y_plus,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_y_plus_2,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_y_plus_2,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_z_plus,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_z_plus,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_a_plus,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_a_plus,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_b_plus,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_b_plus,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_c_plus,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_c_plus,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_x_decrease,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_x_decrease,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_y_decrease,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_y_decrease,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_y_decrease_2,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_y_decrease_2,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_z_decrease,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_z_decrease,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_a_decrease,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_a_decrease,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_b_decrease,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_b_decrease,SIGNAL(released()),this,SLOT(coor_decrease()));
    connect(ui->btn_c_decrease,SIGNAL(pressed()),this,SLOT(coor_plus()));
    connect(ui->btn_c_decrease,SIGNAL(released()),this,SLOT(coor_decrease()));

    connect(codeeditor,SIGNAL(ed_run_from_line(int)),this,SLOT(pro_run_from(int)));
    connect(codeeditor,SIGNAL(ed_edit()),this,SLOT(on_actionProEdit_triggered()));
    connect(codeeditor,SIGNAL(ed_unload()),this,SLOT(pro_unload()));

    connect(codeeditor2,SIGNAL(ed_run_from_line(int)),this,SLOT(pro_run_from(int)));
    connect(codeeditor2,SIGNAL(ed_edit()),this,SLOT(code_actionProEdit_func2()));
    connect(codeeditor2,SIGNAL(ed_unload()),this,SLOT(pro_unload2()));

//    connect(ui->btn_run,SIGNAL(pressed()),this,SLOT(pro_run()));
    connect(ui->btn_pause,SIGNAL(pressed()),this,SLOT(pro_pause()));
    connect(ui->btn_abort,SIGNAL(pressed()),this,SLOT(pro_abort()));


    connect(set_feed_override,SIGNAL(value_change()),this,SLOT(set_feed_override_fun()));
    connect(set_spindle_override,SIGNAL(value_change()),this,SLOT(set_spindle_override_fun()));

    /******************open file****************/
    connect(ui->btn_open,SIGNAL(released()),this,SLOT(on_action_Open_triggered()));
    connect(ui->btn_open_2,SIGNAL(released()),this,SLOT(btn_open_file2()));
    connect(nclinereader,SIGNAL(done(int)),this,SLOT(set_ncfile_linecount(int)));

    /***************msg signal****************/
//
    connect(para,SIGNAL(send_msg(int,QString)),this,SLOT(show_msg(int,QString)));
    //connect(this,SIGNAL(signalMdiState(QString)),setoffsetdlg,SLOT(mdiState(QString)));
    connect(this,SIGNAL(signalMdiState(QString)),moffset,SLOT(mdiState(QString)));
    connect(moffset,SIGNAL(send_msg(int,QString)),this,SLOT(show_msg(int,QString)));
    connect(this,SIGNAL(info_to_table(int,QString)),info_table,SLOT(insert_info_line(int,QString)));
    connect(ui->statusBar,SIGNAL(messageChanged(QString)),this,SLOT(statusbar_message_change(QString)));
    connect(tool_manage,SIGNAL(send_msg(int,QString)),this,SLOT(show_msg(int,QString)));



    /********opengl*******/
    QAction *act;
    ActionFactory *fct = ActionFactory::instance();
    act = fct->getAction(CNC::ActionModelReload);
    connect(ui->btn_gl_build,SIGNAL(released()),act,SLOT(trigger()));
    act = fct->getAction(CNC::ActionPathClear);
    connect(ui->btn_gl_clear,SIGNAL(released()),act,SLOT(trigger()));
    act = fct->getAction(CNC::ActionViewP);
    connect(ui->btn_gl_setp,SIGNAL(released()),act,SLOT(trigger()));
    act = fct->getAction(CNC::ActionViewY);
    connect(ui->btn_gl_fontview,SIGNAL(released()),act,SLOT(trigger()));
    act = fct->getAction(CNC::ActionViewX);
    connect(ui->btn_gl_sideview,SIGNAL(released()),act,SLOT(trigger()));
    act = fct->getAction(CNC::ActionViewZ);
    connect(ui->btn_gl_viewz,SIGNAL(released()),act,SLOT(trigger()));
    act = fct->getAction(CNC::ActionZoomIn);
    connect(ui->btn_gl_zoomin,SIGNAL(released()),act,SLOT(trigger()));
    act = fct->getAction(CNC::ActionZoomOut);
    connect(ui->btn_gl_zoomout,SIGNAL(released()),act,SLOT(trigger()));
    act = fct->getAction(CNC::ActionLoadAbort);
    connect(ui->btn_gl_abort,SIGNAL(released()),act,SLOT(trigger()));

    connect(viewwidget,SIGNAL(signalSetLoadingMax(int)),this,SLOT(set_loading_max_value(int)));
    connect(viewwidget,SIGNAL(signalSetLoadingValue(int)),this,SLOT(set_loading_value(int)));
    connect(viewwidget,SIGNAL(signalErrorInfo(int,QString)),this,SLOT(show_msg(int,QString)));
    connect(viewwidget,SIGNAL(signalSelectHighLine(int)),this->codeeditor,SLOT(set_line_pos(int)));
    connect(this->codeeditor,SIGNAL(ed_opengl_highline(int)),viewwidget,SLOT(setHighLine(int)));


    /*****************************Offsets***********************/
    connect(Save_coors,SIGNAL(triggered(QAction*)),this,SLOT(save_coors_triggered(QAction*)));
    connect(Load_coors,SIGNAL(triggered(QAction*)),this,SLOT(load_coors_triggered(QAction*)));

    /********mdi*******/
    connect(mdieditor,SIGNAL(cursorPositionChanged()),this,SLOT(mdi_text_change()));
    connect(ui->btn_mdi_send,SIGNAL(released()),this,SLOT(mdi_text_send()));
    connect(mdieditor,SIGNAL(ed_clear_data()),this,SLOT(mdi_clear()));

    /*****************************probe**************************/
    connect(ui->btn_first_probe,SIGNAL(released()),this,SLOT(firstProbe()));
    connect(ui->btn_second_probe,SIGNAL(released()),this,SLOT(secondProbe()));

    connect(ui->mode_tab,SIGNAL(currentChanged(int)),this,SLOT(tab_switch(int)));
    connect(ui->btn_continue,SIGNAL(released()),this,SLOT(jog_set_continue()));
    connect(ui->btn_handle,SIGNAL(released()),this,SLOT(jog_set_handle()));
    connect(ui->btn_incr1,SIGNAL(released()),this,SLOT(jog_set_incr1()));
    connect(ui->btn_incr01,SIGNAL(released()),this,SLOT(jog_set_incr01()));
    connect(ui->btn_incr001,SIGNAL(released()),this,SLOT(jog_set_incr001()));
    connect(ui->btn_incr10,SIGNAL(released()),this,SLOT(jog_set_incr10()));
    connect(ui->btn_incruser,SIGNAL(released()),this,SLOT(jog_set_incruser()));
    connect(ui->btn_incr_set,SIGNAL(released()),this,SLOT(jog_set_incrset()));

    connect(lab_set_feed,SIGNAL(clicked()),this,SLOT(set_feed_rate()));
    connect(lab_set_spindle,SIGNAL(clicked()),this,SLOT(set_spindle_feed()));
    connect(lab_work_piece,SIGNAL(clicked()),this,SLOT(clear_work_piece()));
    connect(lab_feed_over,SIGNAL(clicked()),this,SLOT(set_specify_feedover()));
    connect(lab_spindle_over,SIGNAL(clicked()),this,SLOT(set_specify_spindleover()));




    /******************IO action*****************/
    connect(ui->actionSpindle,SIGNAL(triggered()),this,SLOT(set_spindle()));
    connect(ui->actionLube,SIGNAL(triggered()),this,SLOT(set_lube()));
    connect(ui->actionCool,SIGNAL(triggered()),this,SLOT(set_flood()));

    connect(this,SIGNAL(goble_key_get(uint,uint)),this,SLOT(get_goble_key(uint,uint)));
    connect(this,SIGNAL(help_goble_key()),ui->btn_x_plus,SIGNAL(pressed()));

    /********************others***********************/
    act = fct->getAction(CNC::ActionTrigleManual);
    connect(act,SIGNAL(triggered()),this,SLOT(turnToManualTab()));
    addAction(act);
    act = fct->getAction(CNC::ActionTrigleMdi);
    connect(act,SIGNAL(triggered()),this,SLOT(turnToMdiTab()));
    addAction(act);
    act = fct->getAction(CNC::ActionTrigleProbe);
    connect(act,SIGNAL(triggered()),this,SLOT(turnToProbeTab()));
    addAction(act);

/***********hide-sin-2019-01-22***************/
    if(par_get_var(3999) != 4 && par_get_var(3999) != 5 && par_get_var(3999) != 1){
        connect(show_offset_x,SIGNAL(leftClick(int)),this,SLOT(set_offset_direct(int)));
        connect(show_offset_y,SIGNAL(leftClick(int)),this,SLOT(set_offset_direct(int)));
        connect(show_offset_z1,SIGNAL(leftClick(int)),this,SLOT(set_offset_direct(int)));

        if(par_get_var(3999)!=6){
            connect(show_offset_a,SIGNAL(leftClick(int)),this,SLOT(set_offset_direct(int)));
        }

        connect(show_offset_c,SIGNAL(leftClick(int)),this,SLOT(set_offset_direct(int)));
        connect(show_offset_b,SIGNAL(leftClick(int)),this,SLOT(set_offset_direct(int)));
    }




    if(0 == keychoice) {
        /*
        act = fct->getAction(CNC::ActionFeed10);
        connect(act,SIGNAL(trigleValue(int)),this,SLOT(shortcut_feed_set(int)));
        addAction(act);
        act = fct->getAction(CNC::ActionFeed20);
        connect(act,SIGNAL(trigleValue(int)),this,SLOT(shortcut_feed_set(int)));
        addAction(act);
        act = fct->getAction(CNC::ActionFeed30);
        connect(act,SIGNAL(trigleValue(int)),this,SLOT(shortcut_feed_set(int)));
        addAction(act);
        act = fct->getAction(CNC::ActionFeed40);
        connect(act,SIGNAL(trigleValue(int)),this,SLOT(shortcut_feed_set(int)));
        addAction(act);
        act = fct->getAction(CNC::ActionFeed50);
        connect(act,SIGNAL(trigleValue(int)),this,SLOT(shortcut_feed_set(int)));
        addAction(act);
        act = fct->getAction(CNC::ActionFeed60);
        connect(act,SIGNAL(trigleValue(int)),this,SLOT(shortcut_feed_set(int)));
        addAction(act);
        act = fct->getAction(CNC::ActionFeed70);
        connect(act,SIGNAL(trigleValue(int)),this,SLOT(shortcut_feed_set(int)));
        addAction(act);
        act = fct->getAction(CNC::ActionFeed80);
        connect(act,SIGNAL(trigleValue(int)),this,SLOT(shortcut_feed_set(int)));
        addAction(act);
        act = fct->getAction(CNC::ActionFeed90);
        connect(act,SIGNAL(trigleValue(int)),this,SLOT(shortcut_feed_set(int)));
        addAction(act);
        act = fct->getAction(CNC::ActionFeed100);
        connect(act,SIGNAL(trigleValue(int)),this,SLOT(shortcut_feed_set(int)));
        addAction(act);
        */
    } else {
        ui->btn_x_plus->setText("");
        ui->btn_x_plus->setIconSize(QSize(ui->btn_x_plus->height(),ui->btn_x_plus->height()));
        ui->btn_x_plus->setIcon(QIcon(":/new/prefix1/images/6.png"));
        ui->btn_x_decrease->setText("");
        ui->btn_x_decrease->setIconSize(QSize(ui->btn_x_plus->height(),ui->btn_x_plus->height()));
        ui->btn_x_decrease->setIcon(QIcon(":/new/prefix1/images/4.png"));
        ui->btn_y_plus->setText("");
        ui->btn_y_plus->setIconSize(QSize(ui->btn_x_plus->height(),ui->btn_x_plus->height()));
        ui->btn_y_plus->setIcon(QIcon(":/new/prefix1/images/8.png"));
        ui->btn_y_decrease->setText("");
        ui->btn_y_decrease->setIconSize(QSize(ui->btn_x_plus->height(),ui->btn_x_plus->height()));
        ui->btn_y_decrease->setIcon(QIcon(":/new/prefix1/images/2.png"));
        ui->btn_z_plus->setText("");
        ui->btn_z_plus->setIconSize(QSize(ui->btn_x_plus->height(),ui->btn_x_plus->height()));
        ui->btn_z_plus->setIcon(QIcon(":/new/prefix1/images/9.png"));
        ui->btn_z_decrease->setText("");
        ui->btn_z_decrease->setIconSize(QSize(ui->btn_x_plus->height(),ui->btn_x_plus->height()));
        ui->btn_z_decrease->setIcon(QIcon(":/new/prefix1/images/1.png"));
//        ui->btn_a_plus->setText("");
//        ui->btn_a_plus->setIconSize(QSize(ui->btn_x_plus->height(),ui->btn_x_plus->height()));
//        ui->btn_a_plus->setIcon(QIcon(":/new/prefix1/images/9.png"));
//        ui->btn_a_decrease->setText("");
//        ui->btn_a_decrease->setIconSize(QSize(ui->btn_x_plus->height(),ui->btn_x_plus->height()));
//        ui->btn_a_decrease->setIcon(QIcon(":/new/prefix1/images/3.png"));
    }



    act = fct->getAction(CNC::ActionSpindleDec);
    connect(act,SIGNAL(triggered()),this,SLOT(shortcut_spindle_dec()));
    addAction(act);
    act = fct->getAction(CNC::ActionSpindlePlus);
    connect(act,SIGNAL(triggered()),this,SLOT(shortcut_spindle_plus()));
    addAction(act);



    act = fct->getAction(CNC::ActionTrigleCoor);
    connect(act,SIGNAL(trigleValue(int)),this,SLOT(switchMainTab(int)));
    addAction(act);
    act = fct->getAction(CNC::ActionTrigleModel);
    connect(act,SIGNAL(trigleValue(int)),this,SLOT(switchMainTab(int)));
    addAction(act);
    act = fct->getAction(CNC::ActionTrigleInfo);
    connect(act,SIGNAL(trigleValue(int)),this,SLOT(switchMainTab(int)));
    addAction(act);
    act = fct->getAction(CNC::ActionTrigleIoIn);
    connect(act,SIGNAL(trigleValue(int)),this,SLOT(switchMainTab(int)));
    addAction(act);
    act = fct->getAction(CNC::ActionTrigleIoOut);
    connect(act,SIGNAL(trigleValue(int)),this,SLOT(switchMainTab(int)));
    addAction(act);
    act = fct->getAction(CNC::ActionTrigleArray);
    connect(act,SIGNAL(trigleValue(int)),this,SLOT(switchMainTab(int)));
    addAction(act);

    /***********************array***********************/
    connect(ui->arrayOpen,SIGNAL(released()),this,SLOT(arraySlotOpen()));
    connect(ui->arrayBuild,SIGNAL(released()),this,SLOT(arraySlotBuild()));
    connect(ui->arraysimple,SIGNAL(clicked()),this,SLOT(arraysimpleclick()));
    connect(ui->arraycomplex,SIGNAL(clicked()),this,SLOT(arraycomplexclick()));

    connect(ui->btn_cen,SIGNAL(clicked()),this,SLOT(switchjogspeed()));

    connect(ui->probe_debug,SIGNAL(toggled(bool)),this,SLOT(setprobedebug(bool)));
    connect(ui->btn_warm,SIGNAL(clicked()),this,SLOT(run_warm()));  //janmi20150807


    //janmi20190724
    connect(show_offset_x,SIGNAL(leftClick(int)),this,SLOT(set_offset_direct(int)));
    connect(show_offset_z1,SIGNAL(leftClick(int)),this,SLOT(set_offset_direct(int)));
    connect(show_offset_y,SIGNAL(leftClick(int)),this,SLOT(set_offset_direct(int)));
    connect(show_offset_y_2,SIGNAL(leftClick(int)),this,SLOT(set_offset_direct(int)));


}

void linuxcnc::gl_build()
{
    viewwidget->openNcFile(filepath);           //build the 3d model
}


void linuxcnc::showTime()
{
    QDateTime datatime = QDateTime::currentDateTime();
    _time_now = datatime.toString("yyyy-MM-dd hh:mm:ss");
    _time_date = datatime.toString("yyyy-MM-dd");
    _time_hour = datatime.toString("hh:mm:ss");

    show_time_date->setText(_time_date);
    show_time->setText(_time_now);

    static int app_start_up = 0;
     if (app_start_up ==0){
       if(!_forbid_run && !_forbid_spindle && !_get_alm
               && !infoForbid && emcStatus->motion.traj.linktoEth > 0) {
           QString strtemp;
           get_system("killall popimage",&strtemp);
           if(get_task_state() != EMC_TASK_STATE_ESTOP) {
               sendMachineOn();
               sendMachineOff();
               sendMachineOn();
           }

        //this->on_action_Home_triggered();
           if(_ini->read("PAR_AXIS_0_USE_ABSPOS","VALUE","1").toInt() < 1 ||
                   _ini->read("PAR_AXIS_1_USE_ABSPOS","VALUE","1").toInt() < 1 ||
                   _ini->read("PAR_AXIS_2_USE_ABSPOS","VALUE","1").toInt() < 1) {
               if(_ini->read("PAR_HOME_USE_ABSPOS","VALUE","0").toInt() == 0)
                          this->on_action_Home_triggered();
           }
           firsthomeflag=1;  //janmi20150807

           change_z();


           if(_ini->read("PAR_RS274_TOOLTABLE_TYPE","VALUE","0").toInt() == 3 && _ini->read("PAR_RS274_USETOOLCHANGER" ,"VALUE","0").toInt() > 0) {
               show_msg(1,"请确认刀盘号是否正确,如不正确,请进行刀盘回零");
           }

           app_start_up ++;
       }
     }

     if((register_timeout() || authorize_timeout() )&& 1 == app_start_up){
//         this->on_actionRegister_triggered();
         app_start_up ++;
     }

}

void linuxcnc::setwaterstart(){

    int enable = _ini->read("PAR_OPERATOR_WATEROUTENABLE","VALUE","0").toInt();
    if(enable>0){
        sendSetDIO(1,1);
        waterouttime->start(wateroutdelay*1000);
        watertime->stop();
    }
}

void linuxcnc::setwaterout(){

    sendSetDIO(1,0);
    watertime->start(waterdelay*1000);
    waterouttime->stop();
}


void linuxcnc::set_spindle()
{
    if(0 == get_spindle_status()){
        if((emcStatus->motion.set_which_z & 0x01<<2) > 0)
            sendMultiFunction(2,0,1);
        else
            sendMultiFunction(2,0,0);
        if((emcStatus->motion.set_which_z & 0x01<<3) > 0)
            sendMultiFunction(2,1,1);
        else
            sendMultiFunction(2,1,0);
        if((emcStatus->motion.set_which_z & 0x01<<4) > 0)
            sendMultiFunction(2,2,1);
        else
            sendMultiFunction(2,2,0);
        if((emcStatus->motion.set_which_z & 0x01<<5) > 0)
            sendMultiFunction(2,3,1);
        else
            sendMultiFunction(2,3,0);
        show_msg(0,tr("Spindle On"));

        if(emcStatus->task.mode == EMC_TASK_MODE_AUTO) {
            sendSpindleForward((double)recordspindlespeed);
        }else{
            sendSpindleForward((double)spindle_speed);
        }
    }else{

        if(emcStatus->task.runState == EMC_PAUSED) {
            pausespindlefeed = emcStatus->motion.spindle.speed;
        }

        if((emcStatus->motion.set_which_z & 0x01<<2) > 0)
            sendMultiFunction(2,0,0);
        if((emcStatus->motion.set_which_z & 0x01<<3) > 0)
            sendMultiFunction(2,1,0);
        if((emcStatus->motion.set_which_z & 0x01<<4) > 0)
            sendMultiFunction(2,2,0);
        if((emcStatus->motion.set_which_z & 0x01<<5) > 0)
            sendMultiFunction(2,3,0);
        show_msg(0,tr("Spindle Off"));
        sendSpindleOff();
    }
}

void linuxcnc::set_lube()
{
    if(0 == get_lube_status()){
        if(get_task_state() != EMC_TASK_STATE_ON)
            return;
        show_msg(0,tr("Lube On"));
        sendLubeOn();
    }else{
        show_msg(0,tr("Lube Off"));
        sendLubeOff();
    }
}

void linuxcnc::set_flood()
{
    if(0 == get_flood_status()){
        show_msg(0,tr("Flood On"));
        sendFloodOn();
    }else{
        show_msg(0,tr("Flood Off"));
        sendFloodOff();
    }
}

void linuxcnc::set_feed_override_fun()
{
    static int od_value = -10;
    int value = set_feed_override->value();
    if(value != od_value) {
        sendFeedOverride(value*0.01);
        show_msg(0,tr("Set Feed Override %1%").arg(set_feed_override->value()));
    }
    od_value = value;

}

void linuxcnc::set_spindle_override_fun()
{

    static int od_value = -1;
    int value = set_spindle_override->value();
    if(value != od_value) {
        sendSpindleOverride(value*0.01);
        show_msg(0,tr("Set Spindle Override %1%").
                 arg(set_spindle_override->value()));
    }
    od_value = value;

}

void linuxcnc::set_feed_rate()
{
    SetFeed *feed = new SetFeed((int)defspeed,(int)fastspeed);
    feed->exec();
    if((int)defspeed != feed->defvel) {
        defspeed = feed->defvel;
        _ini->writeEntry("PAR_TRAJ_DEFAULT_VELOCITY/VALUE",QString::number(defspeed/60));
        show_msg(0,tr("Set Feed Rate %1").arg((int)defspeed));
    }
    if((int)fastspeed != feed->fastervel) {
        fastspeed = feed->fastervel;
        show_msg(0,tr("Set HighFeed Rate %1").arg((int)fastspeed));
    }
    jogspeed = defspeed;
    sendChoiceDefaultFeed(defspeed);
    delete feed;
}

void linuxcnc::set_spindle_feed()
{
    int val = 0;
    MyInput input(0,(QVariant)spindle_speed,(QVariant)0,(QVariant)maxspindlespeed,tr("Set Spindle Feed"),tr("Set Spindle Feed"),0);
    input.exec();
    val = input.val.toInt();
    if(val >= 0 && val <= maxspindlespeed){
        spindle_speed = val;
        show_msg(0,tr("Set Spindle feedrate %1").arg(spindle_speed));
        sendSpindleFeed((double)spindle_speed);
    }
}

void linuxcnc::pro_run_from(int line)
{
    if(2 != emcStatus->task.mode)
    {
        sendAuto();
    }if(1 == emcStatus->task.interpState)
    {
        sendProgramRun(line,0);
    }
    else if(3 == emcStatus->task.interpState)
    {
        sendProgramResume();
    }
}


void linuxcnc::pro_run()
{

    if(pausewaitsp > 0) {
        return;
    }

    if(paneltype == 1 && (ui->mode_tab->currentIndex() != 1 || ui->main_tab->currentIndex() == 4)) {
        show_msg(1,"必须在自动模式下才能运行程序");
        return;
    }

    if(par_get_var(3999)==6){
        if( 0 ==emcStatus->motion.synch_di[7]){

            int ret = QMessageBox::information(this,tr("tips"),tr("Please switch external input button to auto"),QMessageBox::Cancel,QMessageBox::Ok);
            if(ret == QMessageBox::Cancel) {

            }
            return;
         }
    }

    sendSetDIO(6,0);        //关闭蜂鸣器
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

/*    if(runLock->isLock())
        return;
     else
        runLock->setLock(15);
*/
    if(filepath == NULL) {
        show_msg(1,tr("can not run with a null filepath"));
        return ;
    }

    qDebug()<<"toolchangecheck[0] == "<<toolchangecheck[0] ;
    qDebug()<<"toolchangecheck[1] == "<<toolchangecheck[1] ;
    qDebug()<<"toolchangecheck[2] == "<<toolchangecheck[2] ;
    qDebug()<<"toolchangecheck[4] == "<<toolchangecheck[3] ;

    //hong: toolchange signal check
    if(1==_ini->read("PAR_NEED_TOOLCHANGE_CHECK","VALUE","1").toInt()) {
        if(toolchangecheck[0] > 0){
            int ret=QMessageBox::information(this,tr("info"),
                                         tr("Please reset the z1 offset or probing after changing tool"),
                                         QMessageBox::Ok);
            return;
        }
        if(toolchangecheck[1] > 0){
            int ret=QMessageBox::information(this,tr("info"),
                                             tr("Please reset the z2 offset or probing after changing tool"),
                                             QMessageBox::Ok);
            return;
        }
        if(toolchangecheck[2] > 0){
            int ret=QMessageBox::information(this,tr("info"),
                                             tr("Please reset the z3 offset or probing after changing tool"),
                                             QMessageBox::Ok);
           return;
        }

        else if(toolchangecheck[3] > 0) {
            int ret=QMessageBox::information(this,tr("info"),
                                             tr("Please reset the z4 offset or probing after changing tool"),
                                             QMessageBox::Ok);
            return;
        }

    }


    //hong:pause signal check
    if(0 < _real_alm_pin[23]) {
        show_msg(2,tr("Can not run while pause signal is active"));
        return;
    }


    if(1 == emcStatus->task.interpState)
    {
        if(0 != wait_for_state(RCS_DONE))
            return ;
        //if(0 != wait_for_mode(EMC_TASK_MODE_MANUAL))
        //    return ;
        if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
            ensure_mode(EMC_TASK_MODE_MANUAL);
            return;
        }

        QString openPath = realpath;
        if(origin == 2 && par_get_var(3120) == 0)  openPath = realpath2;
        if(0 !=OpenNgcFile(openPath.toLocal8Bit().data())) {
            ensure_mode(EMC_TASK_MODE_MANUAL);
            return;
        }

        ui->btn_run->setChecked(true);
        ui->btn_pause->setEnabled(true);
        ui->mode_tab->setCurrentIndex(1);

        show_msg(0,tr("Start Program Run"));
        sendProgramRun(0,0);        

        clear_3d();     //清除黄色轨迹
    }
    else if(3 == emcStatus->task.interpState)
    {
        if(1 == get_motion_paused()) {
            if(pausespindlefeed == 0) {
                show_msg(0,tr("Start Program Resume"));
                sendProgramResume();
            } else {
                pausespindlefeed = 0;
                pausewaitsp = 1;
                set_spindle();
                show_msg(0,tr("恢复主轴开"));
            }

        } else {
            show_msg(0,tr("Now the program is running"));
        }
    }else{
    }
}

void linuxcnc::pro_pause()
{
/*    if(runLock->isLock())
        return;
     else
        runLock->setLock(15);
*/

    //ghf-2014/8/26
    if(emcStatus->motion.traj.scale == 0) {
        show_msg(0,tr("feedoverrate is zero"));
        return;
    }

     ui->btn_pause->setChecked(true);

    if(0 == get_motion_paused()) {
        show_msg(0,tr("Start Program Pause"));
        sendProgramPause();
    } else {
        show_msg(0,tr("Now the program is Paused already"));
    }
}

void linuxcnc::pro_abort()
{
/*    if(runLock->isLock())
        return;
     else
        runLock->setLock(200);
*/

    //关刀库
    if(_ini->readEntry("PAR_RS274_TOOLTABLE_TYPE/VALUE","0").toInt() == 3 && par_get_var(4127) == 0) {
        sendSetExternIO(7,0);
        if(emcStatus->motion.synch_di[24] == 0 && emcStatus->motion.synch_di[25] == 0 ) {
                sendSetAnalogyIO(1,0); //  设置选刀数为0一直输出
                sendSetExternIO(3,0);
                sendSetExternIO(4,0);
        }

    }
    ui->btn_abort->setDown(true);
    cState = CNEXT_IDLE;
//    ui->btn_abort->setFocus();
//    ui->btn_abort->setDown(true);

  /*

    if(get_autorepeat()) {
        sendAutoRepeat(false);
        ui->actionRepeat->setChecked(false);
    }

    */
    if(get_task_mode() != EMC_TASK_MODE_MANUAL)
        show_msg(0,tr("Program Abort"));
    else
        show_msg(0,tr("停止"));
    sendAbort();


    if(0 == getalarmabort) {
//        sendSetDIO(5,0);
    } else {
        getalarmabort = 0;
    }

    if(get_handle_guide())
        this->on_actionHandle_guide_triggered();

    sendSetDIO(6,0);        //关闭蜂鸣器

    sendSetDIO(14,0);//关闭护罩

    pausewaitsp = 0;
    pausespindlefeed = 0;
    sendSpindleOff();
}


void linuxcnc::coor_plus()
{
    btn_jog_count ++;
    double jog_speed = 0;
    if(!_all_homed) {
        jog_speed = qMin(fabs(par_get_var(4017)),jogspeed)*60;
    } else
        jog_speed = jogspeed;
    if(ui->btn_x_plus->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(0,jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(0,jog_speed,jog_increment);
        now_axis = 0;
    }else if(ui->btn_y_plus->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(1,jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(1,jog_speed,jog_increment);
        now_axis = 1;
    }else if(ui->btn_y_plus_2->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(6,jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(6,jog_speed,jog_increment);
        now_axis = 6;
    }else if(ui->btn_z_plus->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(2,jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(2,jog_speed,jog_increment);
            now_axis = 2;
    }else if(ui->btn_a_plus->isDown())
    {
        if(par_get_var(3999)==1){
            if(0==emcStatus->motion.synch_do[10]){
                now_axis = 3;
                return;
            }
        }
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(3,jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(3,jog_speed,jog_increment);
        now_axis = 3;
    }else if(ui->btn_b_plus->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(4,jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(4,jog_speed,jog_increment);
        now_axis = 4;
    }else if(ui->btn_c_plus->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(5,jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(5,jog_speed,jog_increment);
        now_axis = 5;
    } else if(ui->btn_x_decrease->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(0,-jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(0,-jog_speed,jog_increment);
        now_axis = 0;
    }else if(ui->btn_y_decrease->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(1,-jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(1,-jog_speed,jog_increment);
        now_axis = 1;
    }else if(ui->btn_y_decrease_2->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(6,-jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(6,-jog_speed,jog_increment);
        now_axis = 6;
    } else if(ui->btn_a_decrease->isDown())
    {
        if(par_get_var(3999)==1){
            if(0==emcStatus->motion.synch_do[10]){
                now_axis = 3;
                return;
            }
        }
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(3,-jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(3,-jog_speed,jog_increment);
        now_axis = 3;
    } else if(ui->btn_b_decrease->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(4,-jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(4,-jog_speed,jog_increment);
        now_axis = 4;
    } else if(ui->btn_c_decrease->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(5,-jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(5,-jog_speed,jog_increment);
        now_axis = 5;
    } else if(ui->btn_z_decrease->isDown())
    {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(2,-jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(2,-jog_speed,jog_increment);
            now_axis = 2;
    }
}

void linuxcnc::coor_decrease()
{
    btn_jog_count --;
    //qDebug("btn_jog_count R=%d",btn_jog_count);
    if(jog_mode == JOG_CONTINUE){
        if(0 == now_axis)
            sendJogStop(0);
        else if(1 == now_axis)
            sendJogStop(1);
        else if(2 == now_axis)
            sendJogStop(2);
        else if(3 == now_axis){
            sendJogStop(3);
            if(par_get_var(3999)==1){
                if(0==emcStatus->motion.synch_do[10]){
                    QMessageBox::warning(this,tr("warning"),
                           tr("Please turn on rotation clamp！！"),tr("OK"));
                }
            }
        }
        else if(4 == now_axis)
            sendJogStop(4);
        else if(5 == now_axis)
            sendJogStop(5);
        else if(6 == now_axis) {
            sendJogStop(-1);
        }
        now_axis = 0;
    }


    if(jog_mode == JOG_INCREMENT){
        if(par_get_var(3999)==1){
            if(3 == now_axis){
                if(0==emcStatus->motion.synch_do[10]){
                    QMessageBox::warning(this,tr("warning"),
                           tr("Please turn on rotation clamp！！"),tr("OK"));
                }
            }
         }
    }

}


void linuxcnc::update()
{
    update_coor();
    update_ui();
    static CAutoMessageBox *msgv = new CAutoMessageBox(this,tr("tips"),QString("通讯异常:%1"),1);

    static int TIM = 0;
//    if(TIM>200|| emcStatus->task.runState != EMC_IDLE){
//        static int old_link = -1;
//        if(emcStatus->motion.traj.linktoEth != old_link) {
//            if(emcStatus->motion.traj.linktoEth==0){
//                //pro_abort();
//                sendAbort();
//            }
//            old_link = emcStatus->motion.traj.linktoEth;
//        }
//    }

    if(TIM==200){
        if(emcStatus->motion.traj.linktoEth == 0){
            //qDebug()<<"_____________timesc________________"<<timesc;
            //static CAutoMessageBox *msg = new CAutoMessageBox(this,tr("tips"),QString("通讯异常:%1"),1);
            if(msgv->isHidden()){
                // 执行命令并获取结果
                QProcess process;
                process.start("sudo ethercat slaves");
                process.waitForFinished();
                QString result = process.readAllStandardOutput();


                msgv->setTText("开机通讯异常\n"+result);
                msgv->show();
            }

        }

    }
    TIM++;
    datumPara->updateUI();

    chk_cfg_files(_ini_filepath,_ini_filebak);
    chk_cfg_files(_var_filepath,_var_filebak);
    chk_cfg_files(_zero_filepath,_zero_filebak);
    chk_cfg_files(_tbl_filepath,_tbl_filebak);


    static int link_flag = -1;
    if(link_flag != emcStatus->motion.traj.linktoEth && TIM>200){
        qDebug()<<"link_flag444"<<link_flag<<emcStatus->motion.traj.linktoEth;
        if(0 == emcStatus->motion.traj.linktoEth ){

            //static CAutoMessageBox *msg = new CAutoMessageBox(this,tr("tips"),QString("通讯异常d:%1"),1);
            if(msgv->isHidden()){
                QProcess process;
                process.start("sudo ethercat slaves");
                process.waitForFinished();
                QString result = process.readAllStandardOutput();


                msgv->setTText(result);
                msgv->show();
            }

        }else if(emcStatus->motion.traj.linktoEth==1){
            if(!msgv->isHidden()){
                msgv->hide();
            }
        }
        link_flag = emcStatus->motion.traj.linktoEth;
    }


    if(emcStatus->task.timeleft < 60*60*24*3 && emcStatus->task.timeleft > 60) {
        static int warnflagy = 12000;
        warnflagy++;
        if(warnflagy > 10*60*15) {
            show_msg(0,tr("the lefttime is less than %1 day!").arg((int)(emcStatus->task.timeleft/(60*60*24))+1));
            permanent_tips = 1;
            permanent_text = tr("the lefttime is less than %1 day!").arg((int)(emcStatus->task.timeleft/(60*60*24))+1);
            warnflagy  = 0;
        }
    }

    static int flushcount = 0,iniflag = 0,inicount = 0;
    if(needparflush > 0) {
        flushcount++;
        if(flushcount > 6) {
            flushcount = 0;
            needparflush = 0;
            if(get_task_mode() == EMC_TASK_MODE_MANUAL) {
                sendParaFlush();    //FLUSH THE PARAMETERS
                iniflag = 1;
            }
        }
    }

    if(iniflag > 0){
        inicount++;
        if(inicount > 6 && get_task_mode() == EMC_TASK_MODE_MANUAL){
            inicount = 0;
            iniflag = 0;
            ini_get();
            qDebug()<<"In flush Ini_get";
        }
    }

    static int oldextern3 = -1;
    if(oldextern3 != emcStatus->motion.mot_extern_in[2]) {
        if(emcStatus->motion.mot_extern_in[2] > 0 && emcStatus->task.runState != EMC_RUNNING) {
            if(emcStatus->motion.synch_do[5] > 0) {
                sendSetDIO(5,0);
            } else {
                sendSetDIO(5,1);
            }
        }

        oldextern3 = emcStatus->motion.mot_extern_in[2];
    }

    static int oldkeyboardio1 = -1,keycount1 = 0;
    if(keyboardcontrolIO1 != oldkeyboardio1) {
        keycount1++;
        if(keycount1 > 2) {
            keycount1 = 0;
            if(0 == keyboardcontrolIO1) {
                sendSetDIO(3,0);
            } else {
                if(emcStatus->task.runState == EMC_IDLE)
                    sendSetDIO(3,1);
            }
            oldkeyboardio1 = keyboardcontrolIO1;
        }
    }

    static int oldkeyboardio2 = -1,keycount2 = 0;
    if(keyboardcontrolIO2 != oldkeyboardio2) {
        keycount2++;
        if(keycount2 > 2) {
            keycount2 = 0;
        if(0 == keyboardcontrolIO2) {
            sendSetDIO(4,0);
        } else {
            if(emcStatus->task.runState == EMC_IDLE)
                sendSetDIO(4,1);
        }
        oldkeyboardio2 = keyboardcontrolIO2;
        }
    }
    static int oldkeyboardio3 = -1,keycount3 = 0;
    if(keyboardcontrolIO3 != oldkeyboardio3) {
        keycount3++;
        if(keycount3 > 2) {
            keycount3 = 0;
        if(0 == keyboardcontrolIO3) {
            sendSetDIO(7,0);
        } else {
            if(emcStatus->task.runState == EMC_IDLE)
                sendSetDIO(7,1);
        }
        oldkeyboardio3 = keyboardcontrolIO3;
        }
    }
    static int oldkeyboardio4 = -1,keycount4 = 0;
    if(keyboardcontrolIO4 != oldkeyboardio4) {
        keycount4++;
        if(keycount4 > 2) {
            keycount4 = 0;
        if(0 == keyboardcontrolIO4) {
            sendSetDIO(8,0);
        } else {
            if(emcStatus->task.runState == EMC_IDLE)
                sendSetDIO(8,1);
        }
        oldkeyboardio4 = keyboardcontrolIO4;
        }
    }

        static int old_spindle_mode = -1;
        static int old_spindle_set_feed = -1;
        static int old_spindle_g1_feed = -1;
        if(old_spindle_mode != get_task_mode() || (spindle_speed != old_spindle_set_feed
          && get_task_mode() == EMC_TASK_MODE_MANUAL)
          || (get_run_spindle_feed() != old_spindle_g1_feed &&
              get_task_mode() == EMC_TASK_MODE_AUTO))
        {
            if(get_task_mode() == EMC_TASK_MODE_MANUAL){
                lab_set_spindle->setText(QString("%1").arg(qMin(spindle_speed,(int)maxspindlespeed)));
            }
            if(get_task_mode() == EMC_TASK_MODE_AUTO)
                lab_set_spindle->setText(QString("%1").
                                         arg(qMin(get_run_spindle_feed(),(int)maxspindlespeed)));
        }
        old_spindle_set_feed = spindle_speed;
        old_spindle_mode = get_task_mode();
        old_spindle_g1_feed = get_run_spindle_feed();

/*********janmi20180810*********/
        static int old_taskmode=-1;
        if(old_taskmode!=get_task_mode()){
            if(get_task_mode()==EMC_TASK_MODE_MANUAL){
                if(warming_flag){
                    warming_flag=false;
//                    sendDoubleZcmd(0,1,par_get_var(5343)-par_get_var(5363));
                }
            }
            old_taskmode=get_task_mode();
         }
/*********janmi20180810*******^**/

    static int old_mode = -1;
    static int old_set_feed = -1;
    static int old_g1_feed = -1;
    if((old_mode != get_task_mode() || ((int)jogspeed != old_set_feed
      && get_task_mode() == EMC_TASK_MODE_MANUAL)
      || (get_task_g1feed() != old_g1_feed &&
          get_task_mode() == EMC_TASK_MODE_AUTO)))
    {
        if(get_task_mode() == EMC_TASK_MODE_MANUAL)
            lab_set_feed->setText(QString("%1").arg((int)jogspeed));
        if(get_task_mode() == EMC_TASK_MODE_AUTO)
            lab_set_feed->setText(QString("%1").arg(get_task_g1feed()));
    }
    old_set_feed = jogspeed;
    old_mode = get_task_mode();
    old_g1_feed = get_task_g1feed();

    static int old_actual_vel = -1;
    if(get_actual_feed() != old_actual_vel)
    {
        ui->show_act_feed->setText(QString("%1").arg(get_actual_feed()));
        old_actual_vel = get_actual_feed();
    }

    static double old_feed_override = -10000;
    if(!compare_double(emcStatus->motion.traj.scale,old_feed_override)
            && !get_handle_guide())
    {
        set_feed_override->setValue((100*emcStatus->motion.traj.scale));
        lab_feed_over->setText(QString("%1").arg((int)100*emcStatus->motion.traj.scale));
        old_feed_override = emcStatus->motion.traj.scale;
    }

    static double old_spindle_override = -10000;
    if(!compare_double(emcStatus->motion.traj.spindle_scale,old_spindle_override))
    {
        set_spindle_override->setValue((100*emcStatus->motion.traj.spindle_scale));
        lab_spindle_over->setText(QString("%1")
                           .arg((int)(100*emcStatus->motion.traj.spindle_scale)));
        old_spindle_override = emcStatus->motion.traj.spindle_scale;
    }

    static double old_act_spindle_vel = -10000;
    if(!compare_double(get_actual_spindle_vel(),old_act_spindle_vel))
    {
        ui->show_act_spindle->setText(QString("%1")
            .arg(qMin(get_actual_spindle_vel(),(int)maxspindlespeed)));
        old_act_spindle_vel = get_actual_spindle_vel();
    }

    if(emcStatus->task.state == EMC_TASK_STATE_ON
            && !_forbid_run && !_forbid_spindle && !_get_alm) {
        ui->actionReset->setIcon(QIcon(":/new/prefix1/images/greenlight.png"));
        ui->btn_reset->setIcon(QIcon(":/new/prefix1/images/greenlight.png"));
    }else {
        ui->actionReset->setIcon(QIcon(":/new/prefix1//images/redlight.png"));
        ui->btn_reset->setIcon(QIcon(":/new/prefix1//images/redlight.png"));
    }


    static int old_current_line = -1;
    QString smartfile = _subDir;
    if((emcStatus->task.mode == EMC_TASK_MODE_AUTO || 0 == emcStatus->task.motionLine) &&
            old_current_line != emcStatus->task.motionLine &&
            0 != strcmp(get_inter_file(),smartfile.append("smartControl.ngc").toLocal8Bit().data()))
    {
        QString str;
        str.setNum(emcStatus->task.motionLine);
        ui->show_current_line->setText(str);
        old_current_line = emcStatus->task.motionLine;

        if(origin == 1 || par_get_var(3120) > 0){
            int linenum = codeeditor->document()->findBlockByNumber(emcStatus->task.motionLine).position()-1;
            if(-1 == linenum)
                linenum = 0;
            QTextCursor cur = codeeditor->textCursor();
            cur.setPosition(linenum);
            codeeditor->setTextCursor(cur);

        }else if(origin == 2){
            int linenum = codeeditor2->document()->findBlockByNumber(emcStatus->task.motionLine).position()-1;
            if(-1 == linenum)
                linenum = 0;
            QTextCursor cur = codeeditor2->textCursor();
            cur.setPosition(linenum);
            codeeditor2->setTextCursor(cur);
        }

    }




    static int old_defalut_vel = -1;
    if(emcStatus->motion.traj.maxVelocity != old_defalut_vel) {
        old_defalut_vel = emcStatus->motion.traj.maxVelocity;
    }

    static int old_program_running_time = -1;
    if(get_program_running_time() != old_program_running_time && 0 != get_program_running_time())
    {
        QTime init_time(0,0,0);
        QTime runningtime;
        runningtime = init_time.addSecs(get_program_running_time());
        ui->show_run_time->setText(runningtime.toString("hh:mm:ss"));
        old_program_running_time = get_program_running_time();

    }
    static int old_jog_type = -1;
    static int old_jog_increment = -1;
    if(old_jog_type != jog_mode){
        jog_mode == JOG_CONTINUE ? ui->btn_continue->setChecked(1)
                                 : ui->btn_continue->setChecked(0);
        jog_mode == JOG_HANDLE ? ui->btn_handle->setChecked(1)
                               : ui->btn_handle->setChecked(0);
        old_jog_increment = -1;
        ui->btn_incr001->setChecked(0);
        ui->btn_incr01->setChecked(0);
        ui->btn_incr1->setChecked(0);
        ui->btn_incr10->setChecked(0);
        ui->btn_incruser->setChecked(0);
    }
    if(jog_mode == JOG_INCREMENT &&
            old_jog_increment != jog_increment){
        ui->btn_continue->setChecked(0);
        ui->btn_handle->setChecked(0);
        0.001 == jog_increment && !jog_user_active ?
                    ui->btn_incr001->setChecked(1)
                               : ui->btn_incr001->setChecked(0);
        0.01 == jog_increment && !jog_user_active ?
                    ui->btn_incr01->setChecked(1)
                               : ui->btn_incr01->setChecked(0);
        0.1 == jog_increment && !jog_user_active ?
                    ui->btn_incr1->setChecked(1)
                               : ui->btn_incr1->setChecked(0);
        1 == jog_increment && !jog_user_active ?
                    ui->btn_incr10->setChecked(1)
                               : ui->btn_incr10->setChecked(0);
        jog_user_active ? ui->btn_incruser->setChecked(1)
                                : ui->btn_incruser->setChecked(0);
    }

    static double old_g5x_x = emcStatus->task.g5x_offset.tran.x;
    static double old_g5x_y = emcStatus->task.g5x_offset.tran.y;
    static double old_g5x_z = emcStatus->task.g5x_offset.tran.z;
    static double old_g5x_a = emcStatus->task.g5x_offset.a;
    static double old_g5x_b = emcStatus->task.g5x_offset.b;
    static double old_g5x_c = emcStatus->task.g5x_offset.c;
    if(old_g5x_x != emcStatus->task.g5x_offset.tran.x ||
            old_g5x_y != emcStatus->task.g5x_offset.tran.y ||
       old_g5x_z != emcStatus->task.g5x_offset.tran.z ||
       old_g5x_a != emcStatus->task.g5x_offset.a ||
       old_g5x_b != emcStatus->task.g5x_offset.b ||
       old_g5x_c != emcStatus->task.g5x_offset.c) {
        QString temp = tr("pieceoffset ");
                if(old_g5x_x != emcStatus->task.g5x_offset.tran.x)
                    temp.append(QString("X:%1->%2").arg(QString::number(old_g5x_x,'f',3)
                                       ,QString::number(emcStatus->task.g5x_offset.tran.x,'f',3)));
                if(old_g5x_y != emcStatus->task.g5x_offset.tran.y)
                    temp.append(QString("Y:%1->%2").arg(QString::number(old_g5x_y,'f',3),
                                       QString::number(emcStatus->task.g5x_offset.tran.y,'f',3)));
                    if(old_g5x_b != emcStatus->task.g5x_offset.b)
                        temp.append(QString("B:%1->%2 ").arg(QString::number(old_g5x_b,'f',3),
                                           QString::number(emcStatus->task.g5x_offset.b,'f',3)));
                    if(old_g5x_c != emcStatus->task.g5x_offset.c)
                        temp.append(QString("C:%1->%2 ").arg(QString::number(old_g5x_c,'f',3),
                                           QString::number(emcStatus->task.g5x_offset.c,'f',3)));

                    if(old_g5x_z != par_get_var(5203+par_get_var(5220)*20) ) {
                        temp.append(QString("Z:%1->%2").arg(QString::number(old_g5x_z,'f',3),
                                           QString::number(emcStatus->task.g5x_offset.tran.z,'f',3)));
//                        if(toolchangecheck == 1)
//                            toolchangecheck = 0;
//                        else if(toolchangecheck == 3)
//                            toolchangecheck = 2;
                        toolchangecheck[0] = 0;
                     }
                    if(old_g5x_a != emcStatus->task.g5x_offset.a) {
                        temp.append(QString("A:%1->%2 ").arg(QString::number(old_g5x_a,'f',3),
                                           QString::number(emcStatus->task.g5x_offset.a,'f',3)));
//                        if(toolchangecheck == 2)
//                            toolchangecheck = 0;
//                        else if(toolchangecheck == 3)
//                            toolchangecheck = 1;
                        toolchangecheck[1] = 0;
                    }

                    if(old_g5x_b != emcStatus->task.g5x_offset.b) {
                        temp.append(QString("B:%1->%2 ").arg(QString::number(old_g5x_b,'f',3),
                                           QString::number(emcStatus->task.g5x_offset.b,'f',3)));
                        toolchangecheck[2] = 0;
                    }

                    if(old_g5x_c != emcStatus->task.g5x_offset.c) {
                        temp.append(QString("C:%1->%2 ").arg(QString::number(old_g5x_c,'f',3),
                                           QString::number(emcStatus->task.g5x_offset.c,'f',3)));
                        toolchangecheck[3] = 0;
                    }

                if(temp.contains("->"))
                    show_msg(1,temp);

    }
    old_g5x_x = emcStatus->task.g5x_offset.tran.x;
    old_g5x_y = emcStatus->task.g5x_offset.tran.y;
    old_g5x_z = emcStatus->task.g5x_offset.tran.z;
    old_g5x_a = emcStatus->task.g5x_offset.a;
    old_g5x_b = emcStatus->task.g5x_offset.b;
    old_g5x_c = emcStatus->task.g5x_offset.c;

    static int old_work_piece = get_task_work_piece();
    if(old_work_piece != get_task_work_piece()){
//        show_msg(0,tr("work_piece is %1").arg(work_piece));
        work_piece++;
        lab_work_piece->setText(QString::number(work_piece,10));
        _prefer->writeEntry("DEFAULT/work_piece",
                      QString::number(work_piece,10));
     }
    old_work_piece= get_task_work_piece();


    static double oldtoolcomp = -1;
    if(oldtoolcomp != get_tool_length_comp()) {
        T3piece->setText(QString::number(get_tool_length_comp(),'f',3));
        oldtoolcomp = get_tool_length_comp();
    }



    static bool old_running_flag = !get_pro_running();
    if(old_running_flag != get_pro_running() && !get_pro_running()){
        if(open_while_running_flag){
            open_while_running_flag = false;
            int ret =  QMessageBox::warning(this, tr("Info"),
                        tr("nc file has been changed,ensure to reload?"),
                                             tr("Ok"),tr("Cancel"));
            if(ret == Cancel) {
                return;
            }
            if(ret == Ok) {
                //pro_open(filepath);
            }
        }

    }
    old_running_flag = get_pro_running();


    if(start_verify == 1) {
        static int count = 0;
        count++;
        if(!get_verify() && count > 10) {
            count = 0;
            show_msg(0,tr("Program Verify Abort"));
            start_verify = 0;
        }
    }



    static int old_tool = -1;
    if(old_tool != par_get_var(4283)) {
        ui->show_tool_now->setText(QString::number(par_get_var(4283)));
    }
    old_tool = par_get_var(4283);

    static double old_tool_comp = -1;
    if(!compare_double(old_tool_comp,get_tool_compnow(0))) {
        ui->show_tool_comp->setText(QString::number(get_tool_compnow(0),'f',3));
    }
    old_tool_comp = get_tool_compnow(0);



    check_alm_sig();

    if(_ini->read("PAR_EMCMOT_PANEL_SIG" ,"VALUE","0").toInt()==1){
        getPanelNew();
    }

}

void linuxcnc::clear_3d()
{
    ActionFactory::instance()->
            getAction(CNC::ActionPathClear)->trigger();
}

void linuxcnc::connectNcToModel(bool c) {

    if(origin == 1 || par_get_var(3120) > 0){
        if(c)
            connect(this->codeeditor,SIGNAL(ed_opengl_highline(int)),viewwidget,SLOT(setHighLine(int)));
        else
            disconnect(this->codeeditor,SIGNAL(ed_opengl_highline(int)),viewwidget,SLOT(setHighLine(int)));
    }else if(origin == 2){
        if(c)
            connect(this->codeeditor2,SIGNAL(ed_opengl_highline(int)),viewwidget,SLOT(setHighLine(int)));
        else
            disconnect(this->codeeditor2,SIGNAL(ed_opengl_highline(int)),viewwidget,SLOT(setHighLine(int)));
    }

}

void linuxcnc::autoLoadModel() {

 //   qDebug()<<"linuxcnc::autoLoadMode+++++++++++++++++++++++++++++++++l"<<nclinereader->isReading()<<codeeditor->isLoading();
    if(nclinereader->isReading()) return;
    if(codeeditor->isLoading())  return;
    viewwidget->reload();

}

void linuxcnc::autoLoadModel2() {

 //   qDebug()<<"linuxcnc::autoLoadMode+++++++++++++++++++++++++++++++++l"<<nclinereader->isReading()<<codeeditor->isLoading();
    if(nclinereader2->isReading()) return;
    if(codeeditor2->isLoading())  return;
    viewwidget->reload();

}

bool linuxcnc::isLoadingTextDisplay() {

    if(nclinereader->isReading()) return true;
    if(codeeditor->isLoading())  return true;
    if(codeeditor2->isLoading())  return true;
    return false;

}

void linuxcnc::controlUI()
{
    int axestype = (int)par_get_var(3999);
    int tooltype = _ini->read("PAR_RS274_TOOLTABLE_TYPE","VALUE","1").toInt();
    printf("now axestype:%d \n",axestype);
    if(axestype == 1) {
        //四轴界面显示
        ui->label_c->setHidden(true);
        ui->show_homec_pic->setHidden(true);
        ui->show_abs_c->setHidden(true);
        show_offset_c->setHidden(true);
        ui->btn_c_plus->setHidden(true);
        ui->btn_c_decrease->setHidden(true);

        ui->label_b->setHidden(true);
        ui->show_homeb_pic->setHidden(true);
        ui->show_abs_b->setHidden(true);
        show_offset_b->setHidden(true);
        ui->btn_b_plus->setHidden(true);
        ui->btn_b_decrease->setHidden(true);

        ui->btn_ValveE->hide();
        ui->btn_ValveF->hide();

//        ui->frame_step->setHidden(true);

    } else if(axestype == 2) {
        //AB五轴界面显示
        ui->label_c->setHidden(true);
        ui->show_homec_pic->setHidden(true);
        ui->show_abs_c->setHidden(true);
        show_offset_c->setHidden(true);
        ui->btn_c_plus->setHidden(true);
        ui->btn_c_decrease->setHidden(true);
//        ui->show_left_c->hide();

    } else if(axestype == 3) {
        //AC五轴界面显示
        ui->label_b->setHidden(true);
        ui->show_homeb_pic->setHidden(true);
        ui->show_abs_b->setHidden(true);
        show_offset_b->setHidden(true);
        ui->btn_b_plus->setHidden(true);
        ui->btn_b_decrease->setHidden(true);
//        ui->show_left_b->hide();

    }else if(axestype == 4) {
        //3轴仿形界面显示
        ui->label_b->setHidden(true);
        ui->show_homeb_pic->setHidden(true);
        ui->show_abs_b->setHidden(true);
        show_offset_b->setHidden(true);
        ui->btn_b_plus->setHidden(true);
        ui->btn_b_decrease->setHidden(true);

        ui->label_c->setHidden(true);
        ui->show_homec_pic->setHidden(true);
        ui->show_abs_c->setHidden(true);
        show_offset_c->setHidden(true);
        ui->btn_c_plus->setHidden(true);
        ui->btn_c_decrease->setHidden(true);

        ui->label_a->setHidden(true);
        ui->show_homea_pic->setHidden(true);
        ui->show_abs_a->setHidden(true);
        show_offset_a->setHidden(true);
        ui->btn_a_plus->setHidden(true);
        ui->btn_a_decrease->setHidden(true);
//        ui->show_left_a->hide();
//        ui->show_left_b->hide();
//        ui->show_left_c->hide();

        ui->btn_ValveE->setText(tr("Press"));
        ui->btn_ValveF->hide();

    }else if(axestype == 5) {
        ui->label_b->setHidden(true);
        ui->show_homeb_pic->setHidden(true);
        ui->show_abs_b->setHidden(true);
        show_offset_b->setHidden(true);
        ui->btn_b_plus->setHidden( 2 != tooltype);
        ui->btn_b_decrease->setHidden( 2 != tooltype);

        ui->label_c->setHidden(true);
        ui->show_homec_pic->setHidden(true);
        ui->show_abs_c->setHidden(true);
        show_offset_c->setHidden(true);
        ui->btn_c_plus->setHidden(true);
        ui->btn_c_decrease->setHidden(true);

        ui->label_a->setHidden(true);
        ui->show_homea_pic->setHidden(true);
        ui->show_abs_a->setHidden(true);
        show_offset_a->setHidden(true);
        ui->btn_a_plus->setHidden(true);
        ui->btn_a_decrease->setHidden(true);

//        ui->show_left_a->hide();
//        ui->show_left_b->hide();
//        ui->show_left_c->hide();


        ui->btn_ValveE->setText(tr("Press"));
        ui->btn_ValveF->setText(tr("护罩"));

    }else if(axestype == 6) {
        //xya轴轮廓界面显示
        ui->label_b->setHidden(true);
        ui->show_homeb_pic->setHidden(true);
        ui->show_abs_b->setHidden(true);
        show_offset_b->setHidden(true);
        ui->btn_b_plus->setHidden(true);
        ui->btn_b_decrease->setHidden(true);

        ui->label_c->setHidden(true);
        ui->show_homec_pic->setHidden(true);
        ui->show_abs_c->setHidden(true);
        show_offset_c->setHidden(true);
        ui->btn_c_plus->setHidden(true);
        ui->btn_c_decrease->setHidden(true);


        ui->label_z1->setHidden(true);
        ui->show_homez1_pic->setHidden(true);
        ui->show_abs_z1->setHidden(true);
        show_offset_z1->setHidden(true);
        ui->btn_z_plus->setHidden(true);
        ui->btn_z_decrease->setHidden(true);

//        ui->show_left_z->hide();
//        ui->show_left_b->hide();
//        ui->show_left_c->hide();



        ui->btn_ValveE->hide();
        ui->btn_ValveF->hide();
//        ui->frame_step->setHidden(true);


        ui->btn_cool->hide();
        ui->actionCool->setVisible(false);

        QList<QPushButton *>allbtn = ui->frame_23->findChildren<QPushButton *>();

        for(int i=0;i<allbtn.size();i++){
            QPushButton *btn =allbtn.at(i);
            btn->setFixedHeight(60);
        }


        QList<QPushButton *>allbtn2 = ui->frame_5->findChildren<QPushButton *>();

        for(int i=0;i<allbtn2.size();i++){
            QPushButton *btn =allbtn2.at(i);
            btn->setFixedHeight(60);
        }


    }else if(axestype == 7) {
        ui->label_b->setHidden(true);
        ui->show_homeb_pic->setHidden(true);
        ui->show_abs_b->setHidden(true);
        show_offset_b->setHidden(true);
        ui->btn_b_plus->setHidden(true);
        ui->btn_b_decrease->setHidden(true);

        ui->label_c->setHidden(true);
        ui->show_homec_pic->setHidden(true);
        ui->show_abs_c->setHidden(true);
        show_offset_c->setHidden(true);
        ui->btn_c_plus->setHidden(true);
        ui->btn_c_decrease->setHidden(true);

        ui->label_a->setHidden(true);
        ui->show_homea_pic->setHidden(true);
        ui->show_abs_a->setHidden(true);
        show_offset_a->setHidden(true);
        ui->btn_a_plus->setHidden(true);
        ui->btn_a_decrease->setHidden(true);

//        ui->show_left_a->hide();
//        ui->show_left_b->hide();
//        ui->show_left_c->hide();


        ui->btn_ValveE->hide();
        ui->btn_ValveF->hide();

        ui->btn_go_a->hide();
        ui->btn_go_b->hide();
        ui->btn_go_c->hide();


    }


    ui->btn_continue->setMinimumSize(100,80);
    ui->btn_handle->setMinimumSize(100,80);
    ui->btn_cen->setMinimumSize(75,75);
    ui->btn_z_plus->setMinimumSize(75,75);
    ui->btn_z_decrease->setMinimumSize(75,75);
    ui->btn_y_plus->setMinimumSize(75,75);
    ui->btn_y_plus_2->setMinimumSize(75,75);
    ui->btn_y_decrease->setMinimumSize(75,75);
    ui->btn_y_decrease_2->setMinimumSize(75,75);
    ui->btn_x_plus->setMinimumSize(75,75);
    ui->btn_x_decrease->setMinimumSize(75,75);
    ui->btn_a_plus->setMinimumSize(75,75);
    ui->btn_a_decrease->setMinimumSize(75,75);



    ui->btn_pick_z2->setHidden(true);
    ui->btn_pick_z3->setHidden(true);
    ui->btn_pick_z4->setHidden(true);

    ui->btn_first_probe_Z3->setHidden(true);
    ui->btn_second_probe_Z3->setHidden(true);
    ui->z1firstprobe->setHidden(true);
    ui->z2firstprobe->setHidden(true);
    ui->z3firstprobe->setHidden(true);
    ui->z4firstprobe->setHidden(true);
    ui->z1secondprobe->setHidden(true);
    ui->z2secondprobe->setHidden(true);
    ui->z3secondprobe->setHidden(true);
    ui->z4secondprobe->setHidden(true);

//    ui->show_left_a->hide();
//    ui->show_left_b->hide();
//    ui->show_left_c->hide();


    //不显示剩余坐标janmi
//    ui->label_39->hide();
//    ui->show_left_x->hide();
//    ui->show_left_y->hide();
//    ui->show_left_z->hide();
//    ui->show_left_a->hide();
//    ui->show_left_b->hide();
//    ui->show_left_c->hide();
//    ui->show_left_y_2->hide();
    ui->btn_ValveE->hide();
    ui->btn_ValveF->hide();
    ui->btn_go_a->hide();
    ui->btn_go_b->hide();
    ui->btn_go_c->hide();



}

int linuxcnc::getFileLineCount() {

    this->nclinereader->waitForRead();
    return file_totalline;

}

void linuxcnc::update_status()
{
//    workStart();//janmi 20190722  自动切换流程先屏蔽

    //清空报警队列
    for(int i=9;i>=0;i--) {
        char tmp[1024];
        memcpy(tmp,&versiondata->errorbuf[i*1024],sizeof(char)*1024);
        if(strlen(tmp) != 0) {
            show_msg(1,tmp);
            memset(&versiondata->errorbuf[i*1024],0,sizeof(char)*1024);
            break;
        }
    }

    if(0 != updateError())
        qDebug("bad error status\n");
    else {
      if (error_string[0] == 0);
      else {
          QString err_str = error_string;
          if(err_str.contains("MDI_ERROR") || err_str.contains("MDI_OK")) {
              emit signalMdiState(err_str);
              memset(error_string,0x00,sizeof(error_string));
          } else {
            inter_translate(&err_str);
            show_msg(2,err_str);
            memset(error_string,0x00,sizeof(error_string));
            updateStatus();
            update_coor();
            this->repaint();
          }
      }

      if (operator_display_string[0] == 0);
      else {
              QString operStr = operator_display_string;
              int flag = 0;
              if(operStr.contains("MSG:")) {
                  operStr.remove("MSG:");
                  flag = 1;
              }
              inter_translate(&operStr);
              show_msg(1,operStr);
              operator_display_string[0] = 0;
              if(1 == flag) {
                msgshow->setText(operStr);
                msgshow->show();
              }
          }

      if (operator_text_string[0] == 0);
      else {

          QString textStr = operator_text_string;
          if(textStr.startsWith("M91:")) {

              textStr = textStr.mid(4);
              operator_text_string[0] = 0;

          } else {
               qDebug()<<"text"<<operator_text_string;
               inter_translate(&textStr);
               show_msg(0,textStr);
               operator_text_string[0] = 0;
          }
      }
    }
    if(0 != updateStatus())
        qDebug("bad status\n");

    static int fisrtwarming_info=1;
    if(emcStatus->motion.axis[0].homed && emcStatus->motion.axis[1].homed
     && emcStatus->motion.axis[2].homed){
        _all_homed = true;

    //*********janmi20150807********//

    if(firsthomeflag>0){
        firsthomeflag=0;
        fisrtwarming_info=0;
        QDateTime olddate=QDateTime::fromString("M1d1y201500:00:00",
                                                "'M'M'd'd'y'yyyyhh:mm:ss");
        QDateTime now=QDateTime::currentDateTime();
        double sec=olddate.secsTo(now);
        double oldsec=par_get_var(4066);
        int timeall=_ini->read("PAR_OPERATOR_HOMETIMEWARM","VALUE","0").toInt();
        qDebug()<<sec<<oldsec<<timeall<<(sec-oldsec);
        if((sec-oldsec)>timeall*3600){
            int flag=_ini->read("PAR_OPERATOR_FIRSTWARMFLAG","VALUE","0").toInt();
            if(1==flag){
                int ret=QMessageBox::information(this,tr("info"),
                                                 tr("Now is going to run warm,ensure continue?"),
                                                 QMessageBox::Ok,QMessageBox::Cancel);
                if(ret==QMessageBox::Ok){
                    run_warm();
                }else if(QMessageBox::Cancel){

                }
            }else if(flag==2){
                run_warm();
            }
        }
     }    
   } else
        _all_homed = false;

    static int oldallhome=-1;
    if(_all_homed!=oldallhome){
        if(_all_homed>0&&firsthomeflag==0){
            int flag=_ini->read("PAR_OPERATOR_HOMEWARM","VALUE","0").toInt();
            if(1==flag&&fisrtwarming_info==0){
                int ret=QMessageBox::information(this,tr("info"),
                                                 tr("Now is going to run warm,ensure continue1?"),
                                                 QMessageBox::Ok,QMessageBox::Cancel);
                if(ret==QMessageBox::Ok){
                    run_warm();
                }else if(QMessageBox::Cancel){
                }
            }
         }
        oldallhome=_all_homed;
    }
      //*********janmi20150807********^//
    //get the alm_signal
    get_alm_pins(&_real_alm_pin[0]);

    infoForbid = (register_timeout() || !hard_checkok() ||authorize_timeout() ||
                  get_fpgacheck(0) || get_fpgacheck(2)|| !version_checkok() ||
                  get_fpgacheck(3) || (get_fpgacheck(4)&&1 == externIoexit));
}


void linuxcnc::update_coor()
{




    ui->show_abs_b->setText(QString::number(get_abs_pos(AXIS_B),'f',3));
    ui->show_abs_c->setText(QString::number(get_abs_pos(AXIS_C),'f',3));
    show_offset_b->setText(QString::number(get_offset_pos(AXIS_B,0),'f',3));
    show_offset_c->setText(QString::number(get_offset_pos(AXIS_C,0),'f',3));


    //printf("now get offset %f  %f  %f\n",emcStatus->task.rotationModex,emcStatus->task.rotationModey,emcStatus->task.rotationModez);
    if(ui->show_abs_x->text() != QString::number(get_abs_pos(AXIS_X),'f',3)){
        ui->show_abs_x->setText(QString::number(get_abs_pos(AXIS_X),'f',3));
//        qDebug()<<"flush coor++++++++"<<ui->show_abs_x->text()<<"==="<<QString::number(get_abs_pos(AXIS_X),'f',3);
    }
    if(show_offset_x->text() != QString::number(get_offset_pos(AXIS_X,_doublez),'f',3))
        show_offset_x->setText(QString::number(get_offset_pos(AXIS_X,_doublez),'f',3));

//    if(ui->show_left_x->text() != QString::number(get_left_pos(AXIS_X),'f',3))
//        ui->show_left_x->setText(QString::number(get_left_pos(AXIS_X),'f',3));


    if(ui->show_abs_y->text() != QString::number(get_abs_pos(AXIS_Y),'f',3))
        ui->show_abs_y->setText(QString::number(get_abs_pos(AXIS_Y),'f',3));
    if(origin == 1 || par_get_var(3120) > 0){
        if(show_offset_y->text() != QString::number(get_offset_pos(AXIS_Y,_doublez),'f',3))
            show_offset_y->setText(QString::number(get_offset_pos(AXIS_Y,_doublez),'f',3));
    }else{
        if(show_offset_y->text() != "0.000")
            show_offset_y->setText("0.000");
    }


    if(ui->show_abs_y_2->text() != QString::number(get_abs_pos(AXIS_Y2),'f',3))
        ui->show_abs_y_2->setText(QString::number(get_abs_pos(AXIS_Y2),'f',3));
    if(origin == 2 &&  par_get_var(3120) == 0){
        if(show_offset_y_2->text() != QString::number(get_offset_pos(AXIS_Y2,_doublez),'f',3))
            show_offset_y_2->setText(QString::number(get_offset_pos(AXIS_Y2,_doublez),'f',3));
    }else{
        if(show_offset_y_2->text() != "0.000")
            show_offset_y_2->setText("0.000");
    }


//    if(ui->show_left_y->text() != QString::number(get_left_pos(AXIS_Y),'f',3))
//        ui->show_left_y->setText(QString::number(get_left_pos(AXIS_Y),'f',3));

//    if(ui->show_left_y_2->text() != QString::number(get_left_pos(AXIS_Y2),'f',3))
//        ui->show_left_y_2->setText(QString::number(get_left_pos(AXIS_Y2),'f',3));


    if(ui->show_abs_z1->text() != QString::number(get_abs_pos(AXIS_Z1),'f',3))
        ui->show_abs_z1->setText(QString::number(get_abs_pos(AXIS_Z1),'f',3));
    if(show_offset_z1->text() != QString::number(get_offset_pos(AXIS_Z1,_doublez),'f',3))
        show_offset_z1->setText(QString::number(get_offset_pos(AXIS_Z1,_doublez),'f',3));

//    if(ui->show_left_z->text() != QString::number(get_left_pos(AXIS_Z1),'f',3))
//        ui->show_left_z->setText(QString::number(get_left_pos(AXIS_Z1),'f',3));



    if(ui->show_abs_a->text() != QString::number(get_abs_pos(AXIS_A),'f',3))
        ui->show_abs_a->setText(QString::number(get_abs_pos(AXIS_A),'f',3));
    if(show_offset_a->text() != QString::number(get_offset_pos(AXIS_A,_doublez),'f',3))
        show_offset_a->setText(QString::number(get_offset_pos(AXIS_A,_doublez),'f',3));

//    if(ui->show_left_a->text() != QString::number(get_left_pos(AXIS_A),'f',3))
//        ui->show_left_a->setText(QString::number(get_left_pos(AXIS_A),'f',3));

//    if(ui->show_left_b->text() != QString::number(get_left_pos(AXIS_B),'f',3))
//        ui->show_left_b->setText(QString::number(get_left_pos(AXIS_B),'f',3));

//    if(ui->show_left_c->text() != QString::number(get_left_pos(AXIS_C),'f',3))
//        ui->show_left_c->setText(QString::number(get_left_pos(AXIS_C),'f',3));


    // coor less than 0.001
    QString  coors = "0.000";
    if(fabs(get_abs_pos(AXIS_X))<0.001 && ui->show_abs_x->text() != coors)
        ui->show_abs_x->setText("0.000");
    if(fabs(get_offset_pos(AXIS_X,_doublez))<0.001 && show_offset_x->text() != coors)
        show_offset_x->setText("0.000");


    if(fabs(get_abs_pos(AXIS_Y))<0.001 && ui->show_abs_y->text() != coors)
        ui->show_abs_y->setText("0.000");
    if(fabs(get_offset_pos(AXIS_Y,_doublez))<0.001 && show_offset_y->text() != coors)
        show_offset_y->setText("0.000");

    if(fabs(get_abs_pos(6))<0.001 && ui->show_abs_y_2->text() != coors)
        ui->show_abs_y_2->setText("0.000");
    if(fabs(get_offset_pos(6,_doublez))<0.001 && show_offset_y_2->text() != coors)
        show_offset_y_2->setText("0.000");


    if(fabs(get_abs_pos(AXIS_Z1))<0.001 && ui->show_abs_z1->text() != coors)
        ui->show_abs_z1->setText("0.000");
    if(fabs(get_offset_pos(AXIS_Z1,_doublez))<0.001 && show_offset_z1->text() != coors)
        show_offset_z1->setText("0.000");


    if(fabs(get_abs_pos(AXIS_A))<0.001 && ui->show_abs_a->text() != coors)
        ui->show_abs_a->setText("0.000");
    if(fabs(get_offset_pos(AXIS_A,_doublez))<0.001 && show_offset_a->text() != coors)
        show_offset_a->setText("0.000");



    //qDebug()<<"home 0 "<<emcStatus->motion.axis[0].homed;
    if(1 != emcStatus->motion.axis[0].homed){
        QPalette px;
        px.setColor(QPalette::WindowText,Qt::red);
        ui->show_abs_x->setPalette(px);
        show_offset_x->setPalette(px);
        //ui->show_left_x->setPalette(px);
        ui->label_x->setPalette(px);
        show_reference_coor[0]->setPalette(px);
        ui->show_homex_pic->setVisible(false);
    }else{
        QPalette px;
        px.setColor(QPalette::WindowText,Qt::black);
        ui->show_abs_x->setPalette(px);
        show_offset_x->setPalette(px);
        //ui->show_left_x->setPalette(px);
        ui->label_x->setPalette(px);
        show_reference_coor[0]->setPalette(px);
        ui->show_homex_pic->setVisible(true);
    }

    if(1 != emcStatus->motion.axis[1].homed){
        QPalette py;
        py.setColor(QPalette::WindowText,Qt::red);
        ui->show_abs_y->setPalette(py);
        show_offset_y->setPalette(py);
        //ui->show_left_y->setPalette(py);
        ui->label_y->setPalette(py);
        show_reference_coor[1]->setPalette(py);
        ui->show_homey_pic->setVisible(false);
    }else{
        QPalette py;
        py.setColor(QPalette::WindowText,Qt::black);
        ui->show_abs_y->setPalette(py);
        show_offset_y->setPalette(py);
        //ui->show_left_y->setPalette(py);
        ui->label_y->setPalette(py);
        show_reference_coor[1]->setPalette(py);
        ui->show_homey_pic->setVisible(true);
    }

    if(1 != emcStatus->motion.axis[6].homed){
        QPalette py;
        py.setColor(QPalette::WindowText,Qt::red);
        ui->show_abs_y_2->setPalette(py);
        show_offset_y_2->setPalette(py);
        //ui->show_left_y->setPalette(py);
        ui->label_y_2->setPalette(py);
        show_reference_coor[6]->setPalette(py);
        ui->show_homey_pic_2->setVisible(false);
    }else{
        QPalette py;
        py.setColor(QPalette::WindowText,Qt::black);
        ui->show_abs_y_2->setPalette(py);
        show_offset_y_2->setPalette(py);
        //ui->show_left_y->setPalette(py);
        ui->label_y_2->setPalette(py);
        if(par_get_var(3120) == 0) {
            ui->show_homey_pic_2->setVisible(true);
            show_reference_coor[6]->setPalette(py);
        }
    }

    if( par_get_var(3999) != 6){
        if(1 != get_task_homed(2) )
        {
            QPalette pz;
            pz.setColor(QPalette::WindowText,Qt::red);
            ui->label_z1->setPalette(pz);
            ui->show_abs_z1->setPalette(pz);
            show_offset_z1->setPalette(pz);
            //ui->show_left_z1->setPalette(pz);
            show_reference_coor[2]->setPalette(pz);
            ui->show_homez1_pic->setVisible(false);
        }else{
            QPalette pz;
            pz.setColor(QPalette::WindowText,Qt::black);
            ui->label_z1->setPalette(pz);
            ui->show_abs_z1->setPalette(pz);
            show_offset_z1->setPalette(pz);
            //ui->show_left_z1->setPalette(pz);
            show_reference_coor[2]->setPalette(pz);
            ui->show_homez1_pic->setVisible(true);
        }
    }

//3轴不显示
 if(par_get_var(3999) != 4 && par_get_var(3999) != 5 && par_get_var(3999) != 7) {
        if(1 != get_task_homed(3))
        {
            QPalette pa;
            pa.setColor(QPalette::WindowText,Qt::red);
            ui->label_a->setPalette(pa);
            ui->show_abs_a->setPalette(pa);
            show_offset_a->setPalette(pa);
            ui->show_homea_pic->setVisible(false);
//            ui->show_left_a->setPalette(pa);
            show_reference_coor[3]->setPalette(pa);
        }else{
            QPalette pa;
            pa.setColor(QPalette::WindowText,Qt::black);
            ui->label_a->setPalette(pa);
            ui->show_abs_a->setPalette(pa);
            show_offset_a->setPalette(pa);
            ui->show_homea_pic->setVisible(true);
//            ui->show_left_a->setPalette(pa);
            show_reference_coor[3]->setPalette(pa);
        }
 }


//3轴、四轴和AC五轴不显示
    if(par_get_var(3999) != 1 && par_get_var(3999) != 3 && par_get_var(3999) != 4 && par_get_var(3999) != 5 && par_get_var(3999) != 6 && par_get_var(3999) != 7) {
         if(1 != emcStatus->motion.axis[4].homed){
            QPalette pa;
            pa.setColor(QPalette::WindowText,Qt::red);
            ui->show_abs_b->setPalette(pa);
            show_offset_b->setPalette(pa);
            ui->label_b->setPalette(pa);
            ui->show_homeb_pic->setVisible(false);
//            ui->show_left_b->setPalette(pa);
            show_reference_coor[4]->setPalette(pa);
        }else{
            QPalette pa;
            pa.setColor(QPalette::WindowText,Qt::black);
            ui->show_abs_b->setPalette(pa);
            show_offset_b->setPalette(pa);
            ui->label_b->setPalette(pa);
            ui->show_homeb_pic->setVisible(true);
//            ui->show_left_b->setPalette(pa);
            show_reference_coor[4]->setPalette(pa);
        }
      }

//3轴、四轴和AB五轴不显示
    if(par_get_var(3999) != 1 && par_get_var(3999) != 2 && par_get_var(3999) != 4 && par_get_var(3999) != 5 && par_get_var(3999) != 6 && par_get_var(3999) != 7) {
       if(1 != emcStatus->motion.axis[5].homed){
            QPalette pb;
            pb.setColor(QPalette::WindowText,Qt::red);
            ui->show_abs_c->setPalette(pb);
            show_offset_c->setPalette(pb);
            ui->label_c->setPalette(pb);
            ui->show_homec_pic->setVisible(false);
//            ui->show_left_c->setPalette(pb);
            show_reference_coor[5]->setPalette(pb);
        }else{
            QPalette pb;
            pb.setColor(QPalette::WindowText,Qt::black);
            ui->show_abs_c->setPalette(pb);
            show_offset_c->setPalette(pb);
            ui->label_c->setPalette(pb);
            ui->show_homec_pic->setVisible(true);
//            ui->show_left_c->setPalette(pb);
            show_reference_coor[5]->setPalette(pb);
        }
    }

}

void linuxcnc::file_change(QString file)
{

    //open_while_running_flag = true;
    qDebug("file change %s",file.toUtf8().data());

}

void linuxcnc::updateNcView(QString &s)
{

    if(codeeditor->isEnabled())
        codeeditor->setPlainText(s);
    else if(codeeditor2->isEnabled())
        codeeditor2->setPlainText(s);

}

void linuxcnc::pro_unload() {

    if(get_pro_running()){
        return;
    }

    filepath .clear();
    realpath .clear();

    if(origin == 1 || par_get_var(3120) > 0){
        viewwidget->setNcFileName("");
        viewwidget->clearModel();
    }

    codeeditor->setPlainText("");
    codeeditor->clear();
    ui->show_pro_path->setToolTip("");
    ui->show_pro_path->setText("");

    show_msg(0,tr("Unload Current Program"));
    sendAbort();
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramOpen((char *)"");
    if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
        return;

}

void linuxcnc::pro_unload2() {

    if(get_pro_running()){
        return;
    }

    filepath2 .clear();
    realpath2 .clear();

    if(origin == 2 &&  par_get_var(3120) == 0){
        viewwidget->setNcFileName("");
        viewwidget->clearModel();
    }


    codeeditor2->setPlainText("");
    codeeditor2->clear();
    ui->show_pro_path_2->setToolTip("");
    ui->show_pro_path_2->setText("");

    show_msg(0,tr("Unload Current Program"));
    sendAbort();
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramOpen((char *)"");
    if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
        return;

}

bool linuxcnc::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{

    toDir.replace("\\","/");
    if(sourceDir == toDir) return true;

    if(!QFile::exists(sourceDir)) return false;
    QDir *createfile = new QDir;

    if (createfile->exists(toDir)){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }

    if(!QFile::copy(sourceDir, toDir)) return false;
    return true;

}

void linuxcnc::initCamSocket()
{
    mp_TCPServer = new QTcpServer();
    int port = 19999;
    if(!mp_TCPServer->listen(QHostAddress::Any, port)) {
        //QMessageBox::information(this, "QT网络通信", "服务器端监听失败！");
        return;
    } else {
        //QMessageBox::information(this, "QT网络通信", "服务器监听成功！");
    }
    connect(mp_TCPServer, SIGNAL(newConnection()), this, SLOT(ServerNewConnection()));
}

void linuxcnc::openCamNC(QString file_path){

    if(get_task_mode() == EMC_TASK_MODE_AUTO){
        show_msg(1,tr("Open file fail! System is running!"));
        return;
    }

    if(origin == 2 &&  par_get_var(3120) == 0){
        if(!copyFileToPath(file_path,_STATION_B_NC_PATH_,true)){
            show_msg(2,tr("Open file fail!"));
            return;
        }
        pro_open2(_STATION_B_NC_PATH_);
    }else{
        if(!copyFileToPath(file_path,_STATION_A_NC_PATH_,true)){
            show_msg(2,tr("Open file fail!"));
            return;
        }
        pro_open(_STATION_A_NC_PATH_);
    }

}
void linuxcnc::pro_open_slot(QString file_path){

    qDebug()<<"+++++++++++++++++++"<<origin;
    if(origin == 1 || par_get_var(3120) > 0){
        pro_open(file_path);
    }else if(origin == 2){
        pro_open2(file_path);
    }

}

void linuxcnc::pro_open(QString file_path)
{

    qDebug()<<"linuxcnc::pro_open1"<<file_path;

    if(get_pro_running()){
        return;
    }

    if(file_path.isEmpty()) return;

//    QString getret,getret2;
//    QStringList stret;
//    if(0 == get_system(QString("file %1").arg(file_path),&getret)) {
//        stret = getret.split(": ");
//        getret2 = stret.at(1);
//        if(getret2.contains("data") && !getret2.contains("Emacs")) {
//            show_msg(2,tr("file format is error or damage"));
//            return;
//        }
//    } else {
//        show_msg(2,tr("file detected fail"));
//        return;
//    }

    QString file_text = "";
    file_text = read_data_from_file(file_path);
    if(0 == file_text.compare("ERROR")){
         QMessageBox::warning(this,tr("warning"),
                              tr("the file is invalid!!"),tr("Ok"));

         if(path_list.contains(file_path)) {
             path_list.removeOne(file_path);
             while(path_list.size()>10) {
                 path_list.pop_back();
             }
         }
         updateRecentAction();
         return;
    }

    if(!path_list.contains(file_path)) {

        path_list.prepend(file_path);
        while(path_list.size()>10) {
            path_list.pop_back();
        }
        updateRecentAction();

    } else {

        path_list.removeOne(file_path);
        path_list.prepend(file_path);
        updateRecentAction();

    }

    filepath = file_path;

    QStringList temp = file_path.split("/");


    ui->show_pro_path->setToolTip(file_path);
    ui->show_pro_path->setText(temp.at(temp.count()-1));

    if(!isArrayfile(filepath))
        ui->arrayPath->setText(filepath);  //array path get

    this->nclinereader->waitForRead();
    codeeditor->setUnblockText(file_text);
    nclinereader->setString(file_text);
    nclinereader->start();

    QStringList temp_file;
    QString now_file;
    temp_file = file_path.split("/");
    now_file = temp_file.at(temp_file.count()-1);

    QDir dir("/home/.tmp/");
    int rett;
    if(!dir.exists())
        rett = system("mkdir /home/.tmp/");

    realpath.clear();
    realpath.append("/home/.tmp/").append(now_file);

    QFile ofile(file_path);
    QFile rfile(realpath);
    if(rfile.exists()) {
        if(! rfile.remove() ) {
           QMessageBox::warning(this,tr("warning"),
              tr("Some error occurred,Error code -101"),tr("Ok"));
        }
    }

    if(!ofile.copy(realpath)) {
        realpath = file_path;
        QMessageBox::warning(this,tr("warning"),
              tr("Some error occurred,Error code -102"),tr("Ok"));
    }
    show_msg(0,tr("A station open file:%1").arg(filepath));

//    qDebug()<<"++++++++++++++++++origin++++++++++++++++++++++"<<origin;
    if(origin == 1 || par_get_var(3120) > 0){
        qDebug()<<"++++++++++++++++++open++++++++++++++++++++++"<<realpath;
        viewwidget->setNcFileName(realpath);
        viewwidget->clearModel();
    }

    sendStartLine(0);



/*
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(0 != OpenNgcFile(realpath.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProReadOnly(2);
    */


}

void linuxcnc::pro_open2(QString file_path)
{

    qDebug()<<"linuxcnc::pro_open2"<<file_path;

    if(get_pro_running()){
        return;
    }

    if(file_path.isEmpty()) return;

    QString getret,getret2;
    QStringList stret;
    if(0 == get_system(QString("file %1").arg(file_path),&getret)) {
        stret = getret.split(": ");
        getret2 = stret.at(1);
        if(getret2.contains("data") && !getret2.contains("Emacs")) {
            show_msg(2,tr("file format is error or damage"));
            return;
        }
    } else {
        show_msg(2,tr("file detected fail"));
        return;
    }

    QString file_text = "";
    file_text = read_data_from_file(file_path);
    if(0 == file_text.compare("ERROR")){
         QMessageBox::warning(this,tr("warning"),
                              tr("the file is invalid!!"),tr("Ok"));

         if(path_list.contains(file_path)) {
             path_list.removeOne(file_path);
             while(path_list.size()>10) {
                 path_list.pop_back();
             }
         }
         updateRecentAction();
         return;
    }

    filepath2 = file_path;

    QString key = QString("FILE_PATH2/path");
    _prefer->writeEntry(key,filepath2);


    QStringList temp = file_path.split("/");


    ui->show_pro_path_2->setToolTip(file_path);
    ui->show_pro_path_2->setText(temp.at(temp.count()-1));



    this->nclinereader2->waitForRead();
    codeeditor2->setUnblockText(file_text);
    nclinereader2->setString(file_text);
    nclinereader2->start();

    QStringList temp_file;
    QString now_file;
    temp_file = file_path.split("/");
    now_file = temp_file.at(temp_file.count()-1);

    QDir dir("/home/.tmp/");
    int rett;
    if(!dir.exists())
        rett = system("mkdir /home/.tmp/");

    realpath2.clear();
    realpath2.append("/home/.tmp/").append(now_file);

    QFile ofile(file_path);
    QFile rfile(realpath2);
    if(rfile.exists()) {
        if(! rfile.remove() ) {
           QMessageBox::warning(this,tr("warning"),
              tr("Some error occurred,Error code -101"),tr("Ok"));
        }
    }

    if(!ofile.copy(realpath2)) {
        realpath2 = file_path;
        QMessageBox::warning(this,tr("warning"),
              tr("Some error occurred,Error code -102"),tr("Ok"));
    }

    show_msg(0,tr("B station open file:%1").arg(filepath2));


    if(origin == 2){
        viewwidget->setNcFileName(realpath2);
        viewwidget->clearModel();
    }



}


linuxcnc::~linuxcnc()
{
    delete ui;
}

void linuxcnc::statusbar_message_change(QString str)
{
    if(str == NULL) {
        QPalette msg_color;
        msg_color.setColor(QPalette::WindowText,Qt::black);
        ui->statusBar->setPalette(msg_color);
    }
}

void linuxcnc::show_msg(int type,QString str)
{
    qDebug()<<"set msg:"<<str;
    QString stylesheet = "";
    int fontsize = 15;
    if(QApplication::desktop()->screenGeometry().height() < 780)
        fontsize = 11;

    if((ININFO == type && INFO_FLAG & INFO_TEXT) ||
       (INWARNING == type && INFO_FLAG & INFO_WARNING) ||
       (INERROR == type && INFO_FLAG & INFO_ERROR)||
       (STATUS== type && INFO_FLAG & INFO_STATUS))
        emit info_to_table(type,str);
    if(!_forbid_run && !_forbid_spindle && !_get_alm
            && (type == INWARNING || type == INERROR)) {
        //ui->main_tab->setCurrentIndex(1);
    }

    QString text = QString::number(type,10);
    text.append("  ").append(_time_now).append("  ")
            .append(str).append("\n");
    insert_line_to_file(INFO_FILE,text);

    if(infoForbid)
        return;

    static int oldMsgtype = -1;
    if(!_forbid_run && !_forbid_spindle && !_get_alm) {
        if(((INERROR == oldMsgtype || (ININFO == type && oldMsgtype > ININFO))
                && (!show_info->text().isEmpty() && permanent_tips == 0)) ||
                (loadingmodelflag && ININFO == type))
            return;

        if(type > ININFO) {
            //show_info->clearMessage();
        }
        str.remove("\n");
        QPalette msg_color;
        if(INERROR == type) {
            show_info->setText(str);
            msg_color.setColor(QPalette::WindowText,QColor(0,0,0));
            show_info->setPalette(msg_color);
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:red;color:yellow",fontsize);
            show_info->setStyleSheet(stylesheet);
            statusbar_show->start(5000);
            permanent_tips = 0;
            permanentflag = 0;
        }
        else if(INWARNING == type) {
            msg_color.setColor(QPalette::WindowText,QColor(0,0,0));
            show_info->setText(str);
            show_info->setPalette(msg_color);
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:yellow;color:black",fontsize);
            show_info->setStyleSheet(stylesheet);
            statusbar_show->start(5000);
            permanent_tips = 0;
            permanentflag = 0;

        } else if(ININFO== type){
            msg_color.setColor(QPalette::WindowText,QColor(0,0,0));
            //ui->statusBar->setPalette(msg_color);
           // ui->statusBar->showMessage(str,5000);
            show_info->setText(str);                             //janmi 2015-9-1
            show_info->setPalette(msg_color);           //janmi 2015-9-1
            stylesheet.sprintf("font: %dpt Sans Serif;color:black",fontsize);
            show_info->setStyleSheet(stylesheet);
            statusbar_show->start(5000);
        }else{

        }
        oldMsgtype = type;

    }
}

void linuxcnc::clear_statusbar()
{
    QPalette msg_color;
    msg_color.setColor(QPalette::WindowText,Qt::black);
    ui->statusBar->setPalette(msg_color);
    show_info->setStyleSheet(0);
    show_info->setText(NULL);
    statusbar_show->stop();
}

void linuxcnc::set_specify_spindleover()
{
    int val = 0;
    MyInput input(0,(QVariant)set_spindle_override->value(),(QVariant)0,(QVariant)set_spindle_override->maximum(),tr("Set Spindle Override"),tr("Set Spindle Override"),0);
    input.exec();
    val = input.val.toInt();
    if(val >= 0 && val <= set_spindle_override->maximum()) {
        set_spindle_override->setValue(val);
        lab_spindle_over->setText(QString("%1").arg(val));
        set_spindle_override_fun();
    }
}

void linuxcnc::set_specify_feedover()
{
    int val = 0;
    MyInput input(0,(QVariant)set_feed_override->value(),(QVariant)0,(QVariant)set_feed_override->maximum(),tr("Set Feed Override"),tr("Set Feed Override"),0);
    input.exec();
    val = input.val.toInt();
    if(val >= 0 && val <= set_feed_override->maximum()) {
        set_feed_override->setValue(val);
        lab_feed_over->setText(QString("%1").arg(val));
        set_feed_override_fun();
    }
}

void linuxcnc::set_loading_max_value(int value) {

    loadingbar->show();
    loadingbar->setMaximumHeight(ui->widget_status->height()-1);
    loadingbar->setMaximum(value);
    loadingmodelflag = true;
    show_info->hide();
    setCursor(Qt::WaitCursor);
    ui->action_Open->setEnabled(false);
}

void linuxcnc::set_ncfile_linecount(int c) {
    file_totalline = c;
}

void linuxcnc::set_loading_value(int value) {

    loadingbar->setValue(value);
    if(value == loadingbar->maximum()) {

        loadingbar->hide();

        if(!viewwidget->getErrorCount()) {
            show_msg(0,tr("loading model finish"));
        } else {
            show_msg(2,tr("loading model error"));
        }
        show_info->show();
        loadingmodelflag = false;
        ui->action_Open->setEnabled(true);
        setCursor(Qt::ArrowCursor);
    }

}

void linuxcnc::btn_open_file2()
{
    QString file_temp = "";
    file_temp = getfileName(tr("Open File"),_ncdir,tr("Files(*.*);;NC Files(*.nc *.ngc *.pim)"),this);

    if(file_temp == NULL)
        return;
    if(file_temp.isEmpty())
        return;

    if(1==_ini->read("PAR_OPERATOR_TOOL_CLEAR","VALUE","1").toInt()){

            tooltabledlg->cleartool();
    }

    pro_open2(file_temp);
    ui->mode_tab->setCurrentIndex(1);
}

void linuxcnc::on_action_Open_triggered()
{
    QString file_temp = "";
    file_temp = getfileName(tr("Open File"),_ncdir,tr("Files(*.*);;NC Files(*.nc *.ngc *.pim)"),this);

    if(file_temp == NULL)
        return;
    if(file_temp.isEmpty())
        return;

    if(1==_ini->read("PAR_OPERATOR_TOOL_CLEAR","VALUE","1").toInt()){

            tooltabledlg->cleartool();
    }

    pro_open(file_temp);
    ui->mode_tab->setCurrentIndex(1);
}

void linuxcnc::on_action_Home_triggered()
{


    if(_ini->read("PAR_AXIS_0_USE_ABSPOS","VALUE","1").toInt() > 0 &&
            _ini->read("PAR_AXIS_1_USE_ABSPOS","VALUE","1").toInt() > 0 &&
            _ini->read("PAR_AXIS_2_USE_ABSPOS","VALUE","1").toInt() > 0) {

        if(show_datum->isHidden()){

            QString password = "cnext";
            QString input = "";
            QString tips = tr("Please input password:");
            bool ok;
            while(input != password){
                 input = QInputDialog::getText(this,tr("Input Password"),tips,
                                               QLineEdit::Password,"",&ok);
                 tips = tr("Password is wrong, input again:");
                 if(!ok){
                     return;
                 }
            }

            ui->main_tab->addTab(show_datum,tr("Set Datum"));

            ui->main_tab->setCurrentIndex(ui->main_tab->count()-1);
            show_datum->show();

        }else{
            ui->main_tab->setCurrentIndex(0);
            ui->main_tab->removeTab(ui->main_tab->count()-1);
            show_datum->hide();
        }



        show_msg(0,tr("In home Dialog"));
        jogspeed = defspeed;
        ui->btn_cen->setChecked(false);
        ui->btn_cen->setText(tr("Default Feed"));


    } else {

        if(_ini->read("PAR_HOME_USE_ABSPOS","VALUE","0").toInt() > 0)
            serialzerop->show();
        else
            home->show();
    }



}

void linuxcnc::on_actionZ1_triggered()
{
    _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",1);
    _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",0);
    _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",0);
    _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",0);
    if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
        return;

    int val = 4;

    if(!get_all_homed(_doublez)) {
        double offset[9] = {0};
        sendDoubleZcmd(0,val,&offset[0]);
        return;
    }
    if(0 == check_zdiff()) {

        if(EMC_TASK_MODE_AUTO == get_task_mode())
            return;
        if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
            ensure_mode(EMC_TASK_MODE_MANUAL);
            return;
        }
        if(get_handle_guide())
            on_actionHandle_guide_triggered();
        QString goOrigin = _subDir;
        goOrigin.append("goZero.ngc");
        QFile file(goOrigin);
             if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                 return;
         QTextStream out(&file);
         out << "M90P"<< val << "\n";
         file.close();

        if(0 !=OpenNgcFile(goOrigin.toLocal8Bit().data())){
            ensure_mode(EMC_TASK_MODE_MANUAL);
            return;
        }
        sendProgramRun(-1,0);
    } else {
        if(0 != ensure_mode(EMC_TASK_MODE_MDI))
            return;
        QString cmd = QString("M90P4");
        sendMdiCmd(cmd.toLatin1().data(),1);
    }
    show_msg(0,tr("Switch to Z1"));
}

void linuxcnc::on_actionZ2_triggered()
{
    _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",0);
    _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",1);
    _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",0);
    _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",0);
    if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
        return;
    int val = 8;

    if(!get_all_homed(_doublez)) {
        double offset[9] = {0};
        sendDoubleZcmd(0,val,&offset[0]);
        return;
    }
    if(0 == check_zdiff()) {
    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(get_handle_guide())
        on_actionHandle_guide_triggered();
    QString goOrigin = _subDir;
    goOrigin.append("goZero.ngc");
    QFile file(goOrigin);
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
     QTextStream out(&file);
     out << "M90P"<< val << "\n";
     file.close();

    if(0 !=OpenNgcFile(goOrigin.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    } else {
        if(0 != ensure_mode(EMC_TASK_MODE_MDI))
            return;
        QString cmd = QString("M90P8");
        sendMdiCmd(cmd.toLatin1().data(),1);
    }
    show_msg(0,tr("Switch to Z2"));
}


void linuxcnc::on_actionZ3_triggered()
{
    _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",0);
    _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",0);
    _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",1);
    _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",0);
    if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
        return;
    int val = 16;

    if(!get_all_homed(_doublez)) {
        double offset[9] = {0};
        sendDoubleZcmd(0,val,&offset[0]);
        return;
    }

    if(0 == check_zdiff()) {
    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(get_handle_guide())
        on_actionHandle_guide_triggered();
    QString goOrigin = _subDir;
    goOrigin.append("goZero.ngc");
    QFile file(goOrigin);
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
     QTextStream out(&file);
     out << "M90P"<< val << "\n";
     file.close();

    if(0 !=OpenNgcFile(goOrigin.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    } else {
        if(0 != ensure_mode(EMC_TASK_MODE_MDI))
            return;
        QString cmd = QString("M90P16");
        sendMdiCmd(cmd.toLatin1().data(),1);
    }
    show_msg(0,tr("Switch to Z3"));
}

void linuxcnc::on_actionZ4_triggered()
{
    int val = 0;
    _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",0);
    _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",0);
    _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",0);
    _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",1);
    if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
        return;

    val = 32;


    if(!get_all_homed(_doublez)) {
        double offset[9] = {0};
        sendDoubleZcmd(0,val,&offset[0]);
        return;
    }
if(0 == check_zdiff()) {
    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(get_handle_guide())
        on_actionHandle_guide_triggered();
    QString goOrigin = _subDir;
    goOrigin.append("goZero.ngc");
    QFile file(goOrigin);
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
     QTextStream out(&file);
     out << "M90P"<< val << "\n";
     file.close();

    if(0 !=OpenNgcFile(goOrigin.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
} else {
    if(0 != ensure_mode(EMC_TASK_MODE_MDI))
        return;
    QString cmd = QString("M90P32");
    sendMdiCmd(cmd.toLatin1().data(),1);
}
    show_msg(0,tr("Switch to Z4"));
}

void linuxcnc::on_actionZ1_Z2_triggered()
{
    int val = 0;
    _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",1);
    _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",1);
    _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",1);

    if(_doublez < 6)
        _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",0);
    else
        _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",1);

    if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
        return;
    if(_doublez < 6)
        val = 28;
    else
        val = 60;


    if(!get_all_homed(_doublez)) {
        double offset[9] = {0};
        sendDoubleZcmd(0,val,&offset[0]);
        return;
    }

    if(0 != ensure_mode(EMC_TASK_MODE_MDI))
        return;
    QString cmd = QString("M90P%1").arg(val);
    sendMdiCmd(cmd.toLatin1().data(),1);

//    if(EMC_TASK_MODE_AUTO == get_task_mode())
//        return;
//    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
//        ensure_mode(EMC_TASK_MODE_MANUAL);
//        return;
//    }
//    if(get_handle_guide())
//        on_actionHandle_guide_triggered();
//    QString goOrigin = _subDir;
//    goOrigin.append("goZero.ngc");
//    QFile file(goOrigin);
//         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//             return;
//     QTextStream out(&file);
//     out << "M90P"<< val << "\n";
//     file.close();

//    if(0 !=OpenNgcFile(goOrigin.toLocal8Bit().data())){
//        ensure_mode(EMC_TASK_MODE_MANUAL);
//        return;
//    }
//    sendProgramRun(-1,0);
    show_msg(0,tr("Switch to Zall"));
}

void linuxcnc::on_actionSet_Offset_triggered()
{
    jogspeed = defspeed;
    ui->btn_cen->setChecked(false);
    ui->btn_cen->setText(tr("Default Feed"));

    if(get_pro_running())
        return;
    show_msg(0,tr("In the Offset Dialog"));
    //setoffsetdlg->initValue();
   // setoffsetdlg->getTimer()->start(100);
   // setoffsetdlg->exec();

    jogspeed = defspeed;
    ui->btn_cen->setChecked(false);
    ui->btn_cen->setText(tr("Default Feed"));

    moffset->initValue();
    moffset->getTimer()->start(100);
    moffset->exec();
//    setoffsetdlg->getTimer()->stop();


}

void linuxcnc::set_offset_direct(int axis)
{
    if(get_pro_running())
        return;

    if(((origin == 1 && axis==6) || (origin == 2 && axis==1 )) && par_get_var(3120) == 0)
        return;

    if(!_all_homed) {
        show_msg(1,tr("Cannot clear the offsets while not home!"));
        return;
    }

    QMessageBox *msg;
    msg = new QMessageBox(this);
    int ret = msg->information(this,tr("Set the offset from the current position"),
    tr("Do you realy want to set it?,If the coordinate not change to zero,please check the public offset"),
    tr("Ok"),tr("Cancel"));
    switch (ret) {
    case Ok:
        moffset->setFromCurrentPos((AXIS_NAME)axis);
        break;
    case Cancel:

        break;
    default:
        break;
    }


}

void linuxcnc::on_actionSet_Parameters_triggered()
{


    int ret_par = para->switch_to_debug(true);
    if(ret_par == -1)
        return;

    //Init the parameters widgeT
    show_msg(0,tr("In the Parameter Dialog"));
    bak_cfg_files(_ini_filepath,_ini_filebak);

    jogspeed = defspeed;
    ui->btn_cen->setChecked(false);
    ui->btn_cen->setText(tr("Default Feed"));


    para->exec();

    needparflush = 1;
}

void linuxcnc::on_actionSet_IO_triggered()
{
    if(iowidget == NULL) {
        iowidget = new IO_Dialog(this);
    }

    int ret = iowidget->switch_to_debug(true);
    if(ret == -1)
        return;
    show_msg(0,tr("In the IO Dialog"));

    jogspeed = defspeed;
    ui->btn_cen->setChecked(false);
    ui->btn_cen->setText(tr("Default Feed"));

    iowidget->show();
}

void linuxcnc::on_actionReset_triggered()
{
    cState = CNEXT_IDLE;
    if(register_timeout() || !hard_checkok() || !version_checkok() ||
            get_fpgacheck(0) || get_fpgacheck(2) || authorize_timeout() ) {

        show_msg(1,tr("Some Commulication problems should be solved first"));
        return;
    }

    if(emcStatus->motion.traj.linktoEth == 0) {
        show_msg(1,tr("Ethercat is disconnect,please check the config"));
    }


    //关刀库
    if(_ini->readEntry("PAR_RS274_TOOLTABLE_TYPE/VALUE","0").toInt() == 3 && par_get_var(4127) == 0) {
        sendSetExternIO(7,0);

        if(emcStatus->motion.synch_di[24] > 0 || emcStatus->motion.synch_di[25] > 0 ) {
            show_msg(1,"刀盘选刀中断，请进行刀盘回零");
        }
        sendSetAnalogyIO(1,0); //  设置选刀数为0一直输出
        sendSetExternIO(3,0);
        sendSetExternIO(4,0);
    }

    show_msg(0,tr("Triggle Reset"));
    sendSetlightcmd(2);
    sendResetcmd();

    if(EMC_TASK_STATE_ON != get_task_state()) {
        show_msg(0,tr("Machine On"));
        sendMachineOn();
//        return;
    }

    sendSetDIO(6,0);        //关闭蜂鸣器
    sendSetDIO(10,0);   //关闭主轴定位
    sendSetDIO(11,0);   //关闭主轴位置模式

    if(externEstop > 0) {
        show_msg(2,tr("外部急停没松开"));
        sendMachineOff();
        return;
    }


    ensure_mode(EMC_TASK_MODE_MDI);
    sendMdiCmd("g92.1",0);
    ensure_mode(EMC_TASK_MODE_MANUAL);

    permanent_tips = 0;
    permanentflag = 0;
    pausespindlefeed = 0;
    pausewaitsp = 0;

}


void linuxcnc::recent_file_triggered(QAction *action)
{

    QString strm = action->text();
    if(strm.isEmpty()) return;
    //clear the tbl
    tooltabledlg->cleartool();

    path_list.prepend(strm);
    pro_open(strm);
    ui->mode_tab->setCurrentIndex(1);
}

void linuxcnc::save_coors_triggered(QAction* act)
{
    ActionFactory *geta = ActionFactory::instance();
    QString text = act->text();
    QStringList list1 = text.split(":");
    int index = list1.at(0).toInt()-1;
    QString new_val = QString("%1*").arg(index+1);
    QString whichz = "";
    QString now_coor = "";
    QStringList temp_file;
    QString now_file = "";


    if(_doublez < 6) {
        now_coor.sprintf("(X:%.3f Y:%.3f Z1:%.3f Z2:%.3f Z3:%.3f)",
                         emcStatus->task.g5x_offset.tran.x,
                         emcStatus->task.g5x_offset.tran.y,
                         emcStatus->task.g5x_offset.tran.z,
                         emcStatus->task.g5x_offset.a,
                         emcStatus->task.g5x_offset.b);
    } else if(_doublez >= 6) {
        now_coor.sprintf("(X:%.3f Y:%.3f Z1:%.3f Z2:%.3f Z3:%.3f Z4:%.3f)",
                         emcStatus->task.g5x_offset.tran.x,
                         emcStatus->task.g5x_offset.tran.y,
                         emcStatus->task.g5x_offset.tran.z,
                         emcStatus->task.g5x_offset.a,
                         emcStatus->task.g5x_offset.b,
                         emcStatus->task.g5x_offset.c);
    }


    temp_file = filepath.split("/");
    now_file = temp_file.at(temp_file.count()-1);
    new_val.append(whichz).append(now_coor).append(" ").append(now_file);
    QString name = "SAVE_COORS/coor";
    name.append(QString::number(index+1,10));
    QString val = new_val.split("*").at(1);
    QString newname  = QString("%1:%2").arg(index+1).arg(val);
    coor_list.replace(index,newname);
    act->setText(newname);
    QAction *actionchange = NULL;
    switch(index) {
    case 0:actionchange = geta->instance()->getAction(CNC::ActionLoadCoor1);break;
    case 1:actionchange = geta->instance()->getAction(CNC::ActionLoadCoor2);break;
    case 2:actionchange = geta->instance()->getAction(CNC::ActionLoadCoor3);break;
    case 3:actionchange = geta->instance()->getAction(CNC::ActionLoadCoor4);break;
    case 4:actionchange = geta->instance()->getAction(CNC::ActionLoadCoor5);break;
    case 5:actionchange = geta->instance()->getAction(CNC::ActionLoadCoor6);break;
    case 6:actionchange = geta->instance()->getAction(CNC::ActionLoadCoor7);break;
    case 7:actionchange = geta->instance()->getAction(CNC::ActionLoadCoor8);break;
    case 8:actionchange = geta->instance()->getAction(CNC::ActionLoadCoor9);break;
    case 9:actionchange = geta->instance()->getAction(CNC::ActionLoadCoor10);break;
    default :break;
    }
    actionchange->setText(newname);
    _prefer->writeEntry(name,val);
    show_msg(1,QString(tr("Save Coor:%1")).arg(newname));
}

void linuxcnc::load_coors_triggered(QAction *act)
{
    QString text = act->text();
    QString offset_val = "";
    QString temp = "",head = "";
    QStringList value;
    if(!text.contains("(") || !text.contains(")")) return;
    temp = text.split("(").at(1);
    head = text.split("(").at(0).split(":").at(1);
    offset_val = temp.split(")").at(0);
    value = offset_val.split(" ");
    if(value.count() < 5) {
        show_msg(2,tr("load coordinate format error"));
        return;
    }
    if(0 != ensure_mode(EMC_TASK_MODE_MDI))
        return;


    if(1==_ini->read("PAR_LOAD_COOR_Z","VALUE","1").toInt())
    {
	    temp.clear();
	    temp.append("G10L2P").append(QString::number(emcStatus->task.g5x_index))
                .append("X").append(value.at(0).split(':').at(1))
                .append("Y").append(value.at(1).split(':').at(1))
                .append("Z").append(value.at(2).split(':').at(1))
                .append("A").append(value.at(3).split(':').at(1))
                .append("B").append(value.at(4).split(':').at(1));
        if(value.count() > 5)
            temp.append("C").append(value.at(5).split(':').at(1));

	    sendMdiCmd(temp.toLatin1().data(),0);
	    temp.clear();

    }else{
        temp.clear();
        temp.append("G10L2P").append(QString::number(emcStatus->task.g5x_index))
                .append("X").append(value.at(0).split(':').at(1))
                .append("Y").append(value.at(1).split(':').at(1));
        sendMdiCmd(temp.toLatin1().data(),0);
        temp.clear();
    }

    show_msg(1,QString(tr("Load Coor:%1")).arg(text));

    _prefer->writeEntry("PUBLIC_OFFSET/offset_x",QString::number(0.0,'f',3));
    _prefer->writeEntry("PUBLIC_OFFSET/offset_y",QString::number(0.0,'f',3));
    if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
        return;

}

void linuxcnc::clear_work_piece()
{
    int val = 0;
    MyInput input(0,(QVariant)work_piece,(QVariant)0,(QVariant)99999,tr("Work Pieces set"),tr("Modify Pieces"),0);
    input.exec();
    val = input.val.toInt();
    if(val >= 0 && val <= 99999){
        work_piece = val;
        _prefer->writeEntry("DEFAULT/work_piece",
                      QString::number(work_piece,10));
        QString Pieces=QString("%1").arg(val);
        lab_work_piece->setText(Pieces);
        show_msg(1,tr("Modify Work Pieces %1").arg(val));
    }

   // janmi 2015-9-2. modify work pieces;

}


void linuxcnc::mdi_clear()
{
    mdi_list.clear();
    mdieditor->clear();
    show_msg(0,tr("Clear MDI"));
    write_data_to_file(MDI_HISTORY_FILE,"");
}

void linuxcnc::mdi_text_change()
{
    ui->mdi_text->setText(mdieditor->linetext);
}

void linuxcnc::mdi_text_send()
{
    if(ui->mdi_text->text().isEmpty())
        return;
    if(ui->mdi_text->text().contains("$")) {
        QString cmdstr = "";
        get_system(ui->mdi_text->text().remove("$"),&cmdstr);
        show_msg(0,cmdstr);
    } else {
        if(0 != ensure_mode(EMC_TASK_MODE_MDI))
            return;
        sendMdiCmd(ui->mdi_text->text().toLatin1().data(),1);

        show_msg(1,tr("Send MDI:%1").arg(ui->mdi_text->text()));

        QString file_text = "";
        file_text = read_data_from_file(MDI_HISTORY_FILE);
        if(0 == file_text.compare("ERROR")) {
             printf("OPEN ERROR!\n");
             return;
        }
        QString mdi_temp = ui->mdi_text->text();
        mdi_temp.append("\n");
        file_text.append(mdi_temp);
        mdi_list.append(mdi_temp);
        write_data_to_file(MDI_HISTORY_FILE,
                           file_text);
        mdieditor->setPlainText(mdi_list.join(""));

        QTextCursor cur = mdieditor->textCursor();
        cur.movePosition(QTextCursor::End);
        mdieditor->setTextCursor(cur);
    }
}

void linuxcnc::tab_switch(int i)
{
    if(0 == i) {
        if(jog_mode == JOG_HANDLE || jog_mode == JOG_INCREMENT)
            jog_set_continue();
    }
}

void linuxcnc::shortcut_feed_set(int i)
{
    //qDebug()<<"set_feed";
    set_feed_override->setValue(i);
    set_feed_override_fun();

}

void linuxcnc::shortcut_spindle_plus()
{
    static myLock locker;
    if(locker.isLock())
        return;
     else
        locker.setLock(300);

        set_spindle_override->setValue(set_spindle_override->value()+5>
                                       set_spindle_override->maximum()?
                                       set_spindle_override->maximum():
                                       set_spindle_override->value()+5);
        set_spindle_override_fun();
}

void linuxcnc::shortcut_spindle_dec()
{
    static myLock locker;
    if(locker.isLock())
        return;
     else
        locker.setLock(300);

    set_spindle_override->setValue(set_spindle_override->value()-5<0?0:
                                   set_spindle_override->value()-5);
    set_spindle_override_fun();
}

void linuxcnc::on_actionHalmeter_triggered()
{
    QDir dir("/home/.tmp/updater/cnc-install");
    if(!dir.exists()) {
        dir.mkpath("/home/.tmp/updater/cnc-install");
    }
    cmd_system("cp /home/.cnc/bin/halmeter /home/.tmp/updater/cnc-install/updater");
    cmd_system("sudo /home/.tmp/updater/cnc-install/updater &");
}

void linuxcnc::on_actionHalconfigure_triggered()
{
    char cmd[500];
    sprintf(cmd,"wish8.5 /home/.cnc/bin/halshow.tcl -ini  %s &",_ini_filepath.toLatin1().data());
    cmd_system(cmd);
}

void linuxcnc::on_actionHandle_guide_triggered()
{
    static myLock locker;
    if(locker.isLock()) {
        ui->actionHandle_guide->setChecked(get_handle_guide());
        ui->btn_handguide->setChecked(get_handle_guide());
        return;
     }else
        locker.setLock(400);

    if(get_handle_guide()){
        sendJogwheels_guidecmd('0');
        ui->actionHandle_guide->setChecked(false);
        ui->btn_handguide->setChecked(false);
        sendFeedOverride(set_feed_override->value()*0.01);
        if(get_task_mode() == EMC_TASK_MODE_AUTO &&
                get_task_interp() != EMC_TASK_INTERP_IDLE){
            show_msg(0,tr("Resume to Program"));
        }
    }else if(!get_handle_guide()){
        sendFeedOverride(0);
        sendJogwheels_guidecmd('1');
        ui->actionHandle_guide->setChecked(true);
        ui->btn_handguide->setChecked(true);
    }
}


void linuxcnc::autoSwitchhandle()
{
    static int old_handwheel_flag = 0;
    if(old_handwheel_flag != get_handlewheel_value() && 0 == ui->mode_tab->currentIndex()
            && get_task_mode() == EMC_TASK_MODE_MANUAL) {
        if(0 == old_handwheel_flag) {
            jog_set_handle();
        }
        if(1 == old_handwheel_flag) {
            switch(old_jog_mode) {
            case JOG_CONTINUE:jog_set_continue();break;
            case JOG_INCREMENT:
                if(0.1 == jog_increment) jog_set_incr1();
                else if(0.01 == jog_increment) jog_set_incr01();
                else if(0.001 == jog_increment) jog_set_incr001();
                else if(1 == jog_increment) jog_set_incr10();
                else  jog_set_incr01();
                break;
            }

        }
    }
    old_handwheel_flag = get_handlewheel_value();
}

void linuxcnc::jog_set_continue()
{
    show_msg(0,tr("Change to Continue Mode"));
    old_jog_mode = jog_mode;
    jog_mode = JOG_CONTINUE;
    jog_user_active = false;
    cmd_system("halcmd sets jog_enable 0");
}

void linuxcnc::jog_set_handle()
{
    show_msg(0,tr("Change to Handle Mode"));
    old_jog_mode = jog_mode;
    jog_mode = JOG_HANDLE;
    jog_user_active = false;
    cmd_system("halcmd sets jog_enable 1");
}

void linuxcnc::jog_set_incr1()
{
    show_msg(0,tr("Change to Increment Mode"));
    old_jog_mode = jog_mode;
    jog_mode = JOG_INCREMENT;
    jog_increment = 0.1;
    jog_user_active = false;
    cmd_system("halcmd sets jog_enable 0");
}

void linuxcnc::jog_set_incr01()
{
    old_jog_mode = jog_mode;
    jog_mode = JOG_INCREMENT;
    jog_increment = 0.01;
    jog_user_active = false;
    cmd_system("halcmd sets jog_enable 0");
}

void linuxcnc::jog_set_incr001()
{
    old_jog_mode = jog_mode;
    jog_mode = JOG_INCREMENT;
    jog_increment = 0.001;
    jog_user_active = false;
    cmd_system("halcmd sets jog_enable 0");
}

void linuxcnc::jog_set_incr10()
{
    old_jog_mode = jog_mode;
    jog_mode = JOG_INCREMENT;
    jog_increment = 1;
    jog_user_active = false;
    cmd_system("halcmd sets jog_enable 0");
}

void linuxcnc::jog_set_incruser()
{
    old_jog_mode = jog_mode;
    jog_mode = JOG_INCREMENT;
    jog_increment = incr_user_jog;
    jog_user_active = true;
    cmd_system("halcmd sets jog_enable 0");
}

void linuxcnc::jog_set_incrset()
{
    double val = 0;
    val = inputdialog_get_double(this,tr("Set Jogincr"),
                                 tr("Range"),incr_user_jog,
                                 -1000,1000,3,tr("Ok"),tr("Cancel"));
    if(0 != val){
        incr_user_jog = val;
    }
    ui->btn_incruser->setText(QString("%1").arg(incr_user_jog));
    if(ui->btn_incruser->isChecked()){
        jog_mode = JOG_INCREMENT;
        jog_increment = incr_user_jog;
        jog_user_active = true;
    }

}

  //*********janmi20150807********//
void linuxcnc::run_warm(){

    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO==get_task_mode())
        return;
    if(0!=ensure_mode(EMC_TASK_MODE_AUTO)){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(get_handle_guide())
        on_actionHandle_guide_triggered();
    QString goOrigin=_subDir;
    goOrigin.append("warm.ngc");
    if(0!=OpenNgcFile(goOrigin.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("start warm"));
    QDateTime olddate=QDateTime::fromString("M1d1y201500:00:00",
                                            "'M'M'd'd'y'yyyyhh:mm:ss");
    QDateTime now=QDateTime::currentDateTime();
    sendSetParameter(4066,olddate.secsTo(now));
    warming_flag=true;
}
  //*********janmi20150807********^//

void linuxcnc::show_proinfo() {
    QMessageBox msgBox;
    msgBox.setText(QString(tr("File:%1")).arg(filepath));
    QString str = "";
    str = QString(tr("g0_length:  %1mm\ng1_length:  %2mm\ng2_length:  %3mm\ntotal_length:  %4mm\ntotal_rows:  %5\n")).arg(par_get_var(4101)).arg(par_get_var(4102)).arg(par_get_var(4103)).arg(par_get_var(4104)).arg(this->getFileLineCount());
    msgBox.setInformativeText(str);
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();

}

void linuxcnc::on_actionProEdit_triggered()
{

    if(jog_count > 0) return;
    fileedit->reOpenFile(realpath,filepath,true);
    fileedit->show();

}

void linuxcnc::code_actionProEdit_func2()
{

    if(jog_count > 0) return;
    fileedit->reOpenFile(realpath2,filepath2,true);
    fileedit->show();

}

void linuxcnc::on_actionHalScope_triggered()
{
    cmd_system("halscope &");
}

void linuxcnc::on_actionSmart_Control_triggered()
{
    jogspeed = defspeed;
    ui->btn_cen->setChecked(false);
    ui->btn_cen->setText(tr("Default Feed"));

    if(smart_control == NULL) {
        smart_control = new SmartControl(this);
        connect(smart_control,SIGNAL(send_msg(int,QString)),this,SLOT(show_msg(int,QString)));
    }
    smart_control->exec();
}

void linuxcnc::on_actionUnload_triggered()
{

    this->pro_unload();

}

void linuxcnc::on_actionVerify_triggered()
{

    static myLock locker;
    if(locker.isLock())
        return;
     else
        locker.setLock(1000);

    if(start_verify > 0)
        return;

    ensure_mode(EMC_TASK_MODE_MANUAL);

    QString openPath = realpath;
    if(origin == 2)  openPath = realpath2;
    if(0 != OpenNgcFile(openPath.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProReadOnly(1);
    start_verify = 1;
    show_msg(0,tr("Start Program Verify"));
}

void linuxcnc::on_actionGO_Origin_triggered()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(get_handle_guide())
        on_actionHandle_guide_triggered();
    QString goOrigin = _subDir;
    goOrigin.append("goOrigin.ngc");
    if(0 !=OpenNgcFile(goOrigin.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Go to Origin"));
}

void linuxcnc::on_actionGO_Offset_triggered()
{
    if(1 == air_alarm)
        return;

    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(get_handle_guide())
        on_actionHandle_guide_triggered();
    QString goOffset = _subDir;
    goOffset.append("goOffset.ngc");
    if(0 !=OpenNgcFile(goOffset.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Go to Offset"));
}
void linuxcnc::on_actionRepeat_triggered()
{
    if(get_autorepeat()){
        ui->actionRepeat -> setChecked(false);
        ui->btn_repeat -> setChecked(false);
        sendAutoRepeat(false);
    } else {       
        int val = 0;
        MyInput input(0,(QVariant)ncycletimes,(QVariant)0,(QVariant)9999,tr("repreat_times set"),tr("times"),0);
        input.exec();
        val = input.val.toInt();
        if(val >= 0 && val <= 9999){
            ncycletimes = val;
            sendSetParameter(4050,val);
            updateStatus();
            QString strcycletimes=QString("%1/%2").arg(val).arg(ncycletimes);
            ui->show_cycle_times->setText(strcycletimes);
            show_msg(0,tr("Start the Program Repeat Function"));
            ui->actionRepeat -> setChecked(true);
            ui->btn_repeat -> setChecked(true);
            sendAutoRepeat(true);
        }else{
            ui->actionRepeat -> setChecked(false);
            ui->btn_repeat -> setChecked(false);
            sendAutoRepeat(false);
        }
    }
}

void linuxcnc::on_actionRunLine_triggered()
{
    jogspeed = defspeed;
    ui->btn_cen->setChecked(false);
    ui->btn_cen->setText(tr("Default Feed"));

    if(smartrunline == NULL) {
        smartrunline = new SmartRunDialog(this);
    }
    smartrunline->setfilename(filepath);
    smartrunline->settotlalines(this->getFileLineCount());
    smartrunline->exec();
}

void linuxcnc::update_ui()
{

    if(get_task_mode() == EMC_TASK_MODE_MANUAL && par_get_var(4294) > 0) {
        show_msg(2,"换刀未完成，手动移除刀库刀具");
        int ret = QMessageBox::warning(0,"警告","注意,换刀未完成，请手动移除刀库刀具",QMessageBox::Ok);
        if(ret == QMessageBox::Ok) {
            sendSetParameter(4294,0);
        }
    }

    static int waitspcount = 0;
    if(pausewaitsp > 0) {
        if(waitspcount > par_get_var(4071)*5) {
            pausewaitsp = 0;
            waitspcount = 0;
            show_msg(0,tr("Start Program Resume"));
            sendProgramResume();
        } else {
            waitspcount++;
        }
    }

    if(chklink != NULL) {
        chklink->updateUI();
    }

    ui->chk_useoptionstop->setChecked(optionalstop > 0);
    ui->arrayXdistance->setEnabled(useArraydistance == 1);
    ui->arrayYdistqance->setEnabled(useArraydistance == 1);

    ui->btn_centerwater->setChecked(emcStatus->motion.mot_extern_out[15] > 0);

    if(emcStatus->task.mode == EMC_TASK_MODE_AUTO && get_run_spindle_feed() > 0)
        recordspindlespeed = get_run_spindle_feed();

    ui->btn_cool2->setChecked(emcStatus->io.coolant.mist > 0);
    for(int i=0;i<9;i++) {
        if(show_reference_coor[i]->text().toDouble() != get_abs_pos(i)-par_get_var(3110+i)) {
            show_reference_coor[i]->setText(QString::number(get_abs_pos(i)-par_get_var(3110+i),'f',3));
        }
    }

    static bool old_handleguide = !get_handle_guide();
    static bool old_changingz = !get_changing_z();

    static int old_selectedz = -1,selectdelay=0;
    if(old_selectedz != emcStatus->motion.selectedz) {
        selectdelay ++;
        if(selectdelay > 0) {
            selectdelay = 0;
            ui->btn_pick_z2->setChecked((emcStatus->motion.selectedz & (0x01<<3)) > 0);
            ui->btn_pick_z3->setChecked((emcStatus->motion.selectedz & (0x01<<4)) > 0);
            ui->btn_pick_z4->setChecked((emcStatus->motion.selectedz & (0x01<<5)) > 0);
            old_selectedz = emcStatus->motion.selectedz;
        }
    }



    origin = emcStatus->task.g5x_index;

    static int old_origin = -1;
    if(old_origin != origin){
        if(0 == par_get_var(4061)) {
            ui->label_7->setText(tr("OFFSETCOOR"));
        } else  {

            if((par_get_var(3999)==1 || par_get_var(3999)==4 || par_get_var(3999)==5 || par_get_var(3999)==7) && par_get_var(3120) == 0){

                QString st = QString(tr("OFFSETCOOR"));
                if(emcStatus->task.g5x_index==1){
                    ui->label_7->setText(st.append("(A)"));
                    sendMultiFunction(31,0,0);
                    sendSetParameter(3001,0);
                    codeeditor->setEnabled(true);
                    codeeditor2->setEnabled(false);
                }else if(emcStatus->task.g5x_index==2){
                    ui->label_7->setText(st.append("(B)"));
                    sendMultiFunction(31,1,0);
                    sendSetParameter(3001,1);
                    codeeditor->setEnabled(false);
                    codeeditor2->setEnabled(true);
               } else if(emcStatus->task.g5x_index==3){
                    ui->label_7->setText(tr("(L)OFFSETCOOR"));
                }else if(emcStatus->task.g5x_index==4){
                    ui->label_7->setText(tr("(B)OFFSETCOOR"));
                }else if(emcStatus->task.g5x_index==5){
                    ui->label_7->setText(tr("(E)OFFSETCOOR"));
                }else if(emcStatus->task.g5x_index==6){
                    ui->label_7->setText(tr("(F)OFFSETCOOR"));
                }

             }else{

                ui->label_7->setText(tr("(%1)OFFSETCOOR").arg(_coor_system.value(emcStatus->task.g5x_index)));

            }
        }

    }

    old_origin = origin;





    static int old_autorepreat = -1;
    if(old_autorepreat != get_autorepeat()) {
        qDebug()<<"in debug change"<<get_autorepeat();
        ui->actionRepeat->setChecked(get_autorepeat());
        ui->btn_repeat->setChecked(get_autorepeat());
        old_autorepreat = get_autorepeat();
    }

    static bool enablereset = false;
    if(EMC_TASK_MODE_AUTO == get_task_mode()){
        enablereset = false;
    }else
        enablereset = true;
    ui->actionReset->setEnabled(enablereset);
    ui->btn_reset->setEnabled(enablereset);

    static bool enable_type_0 = false;
    if(EMC_TASK_MODE_AUTO == get_task_mode() || EMC_TASK_MODE_MDI == get_task_mode() ||
           get_changing_z() || loadingmodelflag || start_verify == 1){
        enable_type_0 = false;
    }else
        enable_type_0 = true;
    static bool old_type_0 = !enable_type_0;
    if(old_type_0 != enable_type_0) {
        ui->actionRegister->setEnabled(enable_type_0);
        ui->actionUpdate->setEnabled(enable_type_0);
        old_type_0 = enable_type_0;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode() || EMC_TASK_MODE_MDI == get_task_mode() ||
       EMC_TASK_INTERP_IDLE != emcStatus->task.interpState ||
            EMC_TASK_STATE_ON != get_task_state() || get_changing_z() || loadingmodelflag){
        enable_type_1 = false;
    }else
        enable_type_1 = true;

    ui->btn_spindle->setEnabled(enable_type_1 || emcStatus->task.runState == EMC_PAUSED);
    static bool old_type_1 = !enable_type_1;
    static int old_task_z  = -1,old_lockprobe = -1;
    int lockprobe = _prefer->read("LOCKPROBE","VALUE").toInt();
        if(old_type_1 != enable_type_1 || old_task_z != get_task_z() ||old_lockprobe != lockprobe) {
            ui->action_Open->setEnabled(enable_type_1);
            ui->btn_open->setEnabled(enable_type_1);
            ui->actionProEdit->setEnabled(enable_type_1);
            ui->actionUnload->setEnabled(enable_type_1);
            Recent_files->setEnabled(enable_type_1);
            ui->actionSet_Offset->setEnabled(enable_type_1);
            ui->actionLoad_Offset->setEnabled(enable_type_1);
            ui->actionSave_Offset->setEnabled(enable_type_1);
            ui->action_Home->setEnabled(enable_type_1 );
            ui->actionVerify->setEnabled(enable_type_1 );
            ui->actionSpindle->setEnabled(enable_type_1);
            codeeditor->act_edit->setEnabled(enable_type_1);
            codeeditor->act_unload->setEnabled(enable_type_1);
            codeeditor2->act_edit->setEnabled(enable_type_1);
            codeeditor2->act_unload->setEnabled(enable_type_1);
            ui->btn_edit->setEnabled(enable_type_1);//hsieb

//            ui->show_piece_frame->setEnabled(enable_type_1);
            ui->mode_tab->widget(0)->setEnabled(enable_type_1);
            ui->mode_tab->widget(1)->setEnabled(enable_type_1);
            ui->mode_tab->widget(2)->setEnabled(enable_type_1);
            ui->show_set_feed->setEnabled(enable_type_1);
            ui->show_set_spindle->setEnabled(enable_type_1);
            ui->actionMotorPar->setEnabled(enable_type_1);

            ui->btn_first_probe->setEnabled(enable_type_1 &&  lockprobe== 0);
            ui->btn_second_probe->setEnabled(enable_type_1);
            ui->btn_floatingprobe->setEnabled(enable_type_1);
            ui->btn_first_probe_Z3->setEnabled(enable_type_1 && useprobeflag > 0);
            ui->btn_second_probe_Z3->setEnabled(enable_type_1 && useprobeflag > 0);

            ui->btn_warm->setEnabled(enable_type_1);
            //ui->actionEdit_Tools->setEnabled(enable_type_1);
            ui->btn_gl_build->setEnabled(enable_type_1);
            ui->actionBackUpRestore->setEnabled(enable_type_1);
            ui->z1firstprobe->setEnabled(enable_type_1 &&  lockprobe== 0);
            ui->z2firstprobe->setEnabled(enable_type_1 &&  lockprobe== 0);
            ui->z3firstprobe->setEnabled(enable_type_1 &&  lockprobe== 0);
            ui->z4firstprobe->setEnabled(enable_type_1 &&  lockprobe== 0);
            ui->z1secondprobe->setEnabled(enable_type_1);
            ui->z2secondprobe->setEnabled(enable_type_1);
            ui->z3secondprobe->setEnabled(enable_type_1);
            ui->z4secondprobe->setEnabled(enable_type_1);

            ui->btn_cyclinder_a1->setEnabled(enable_type_1);
            ui->btn_cyclinder_a2->setEnabled(enable_type_1);
            ui->btn_cyclinder_b1->setEnabled(enable_type_1);
            ui->btn_cyclinder_b2->setEnabled(enable_type_1);
            ui->btn_ValveE->setEnabled(enable_type_1);
            ui->btn_ValveF->setEnabled(enable_type_1);


#if 0

            ui->btn_z1_plus->setEnabled(enable_type_1 && (get_task_z() & (0x01<<2)) > 0);
            ui->btn_a_plus->setEnabled(enable_type_1 && (get_task_z() & (0x01<<3)) > 0);
            ui->btn_b_plus->setEnabled(enable_type_1 && (get_task_z() & (0x01<<4)) > 0);
            ui->btn_c_plus->setEnabled(enable_type_1 && (get_task_z() & (0x01<<5)) > 0);
            ui->btn_z1_decrease->setEnabled(enable_type_1 && (get_task_z() & (0x01<<2)) > 0);
            ui->btn_a_decrease->setEnabled(enable_type_1 && (get_task_z() & (0x01<<3)) > 0);
            ui->btn_b_decrease->setEnabled(enable_type_1 && (get_task_z() & (0x01<<4)) > 0);
            ui->btn_c_decrease->setEnabled(enable_type_1 && (get_task_z() & (0x01<<5)) > 0);
            ui->btn_pick_z1->setEnabled(enable_type_1 && (get_task_z() & (0x01<<2)) > 0);
            ui->btn_pick_z2->setEnabled(enable_type_1 && (get_task_z() & (0x01<<3)) > 0);
            ui->btn_pick_z3->setEnabled(enable_type_1 && (get_task_z() & (0x01<<4)) > 0);
            ui->btn_pick_z4->setEnabled(enable_type_1 && (get_task_z() & (0x01<<5)) > 0);

            ui->actionPickZ->setEnabled(enable_type_1);
#endif

//            ui->btn_a_plus->setEnabled(get_task_z() != 0 && enable_type_1);
//            ui->btn_a_decrease->setEnabled(get_task_z() != 0 && enable_type_1);
//            ui->btn_z1_plus->setEnabled(get_task_z() != 1 && enable_type_1);
//            ui->btn_z1_decrease->setEnabled(get_task_z() != 1 && enable_type_1);

        }
        old_type_1 = enable_type_1;
        old_task_z = get_task_z();
        old_lockprobe = lockprobe;

        if((_all_homed && enable_type_1) &&
                EMC_TASK_STATE_ON == get_task_state())
            enable_type_2 = true;
        else
            enable_type_2 = false;

        static bool old_type_2 = !enable_type_2;

        if(old_type_2 != enable_type_2 || old_handleguide != get_handle_guide()){
            ui->actionGO_Origin->setEnabled(enable_type_2 );
            ui->btn_GO_Origin->setEnabled(enable_type_2);//sin
            ui->btn_goZero->setEnabled(enable_type_2);
            ui->actionGO_Offset->setEnabled(enable_type_2 );
            ui->actionSmart_Control->setEnabled(enable_type_2);
            ui->actionRunLine->setEnabled(enable_type_2);
            ui->actionVerify->setEnabled(enable_type_2);
            ui->btn_verify->setEnabled(enable_type_2 );
            ui->btn_go_a->setEnabled(enable_type_2 );
            ui->btn_go_b->setEnabled(enable_type_2 );
            ui->btn_go_c->setEnabled(enable_type_2 );
        }
        old_type_2 = enable_type_2;

        ui->btn_abort->setEnabled(!get_changing_z() && !loadingmodelflag);


        if(((EMC_TASK_MODE_MANUAL == emcStatus->task.mode ||
                EMC_IDLE == emcStatus->task.runState ||
                (EMC_PAUSED == emcStatus->task.runState && !get_handle_guide())) &&
                EMC_TASK_STATE_ON == get_task_state())
                && !(_forbid_run || _forbid_spindle)
                && !get_changing_z() && !loadingmodelflag
                && 0 == start_verify)
            enable_type_3 = true;
        else
            enable_type_3 = false;
        static bool old_type_3 = !enable_type_3;

        static int oldinfoforbid = -1;
        if(old_type_3 != enable_type_3 || old_handleguide != get_handle_guide() ||
                old_changingz != get_changing_z() || oldinfoforbid != infoForbid) {
            ui->btn_run->setEnabled(enable_type_3 && !infoForbid);

            oldinfoforbid = infoForbid;
        }

        ui->btn_pause->setEnabled(!enable_type_3 && !(_forbid_run || _forbid_spindle)
                                  && (EMC_TASK_STATE_ON == get_task_state()) &&
                                  !get_handle_guide() && !get_changing_z() &&
                                  EMC_IDLE != emcStatus->task.runState && 0 == start_verify);
        if(EMC_TASK_MODE_AUTO == emcStatus->task.mode &&
          ((EMC_IDLE != emcStatus->task.runState &&
          EMC_PAUSED != emcStatus->task.runState) ||
          (EMC_IDLE != emcStatus->task.runState &&
           get_handle_guide())) && 0 == start_verify) {
            ui->btn_run->setChecked(true);
        } else
            ui->btn_run->setChecked(false);

        if((EMC_PAUSED == emcStatus->task.runState
            && !get_handle_guide())) {
            ui->btn_pause->setChecked(true);
        } else
            ui->btn_pause->setChecked(false);
        old_type_3 = enable_type_3;

        static bool old_type_io_spindle = -1;
        if(get_task_state() == EMC_TASK_STATE_ON ) {
            ui->actionSpindle->setEnabled(enable_type_1);
//            ui->btn_spindle->setEnabled(enable_type_1);
            if(old_type_io_spindle != get_spindle_status()){
                if(0 == get_spindle_status()) {
                    ui->actionSpindle->setChecked(false);
                    ui->btn_spindle->setChecked(false);
                } else {
                    ui->actionSpindle->setChecked(true);
                    ui->btn_spindle->setChecked(true);
                }
            }
            old_type_io_spindle = get_spindle_status();
        }else{
            ui->actionSpindle->setChecked(false);
            ui->actionSpindle->setEnabled(false);
            ui->btn_spindle->setChecked(false);
//            ui->btn_spindle->setEnabled(false);
            old_type_io_spindle = !get_spindle_status();
        }

        static bool old_type_io_lube = -1;
        if(get_task_state() == EMC_TASK_STATE_ON) {
            ui->actionLube->setEnabled(true);
            ui->btn_lube->setEnabled(true);
            if(old_type_io_lube != get_lube_status()) {
                if(0 == get_lube_status()) {
                    ui->actionLube->setChecked(false);
                    ui->btn_lube->setChecked(false);
                } else {
                    ui->actionLube->setChecked(true);
                    ui->btn_lube->setChecked(true);
                }
            }
            old_type_io_lube = get_lube_status();
        }else{
            ui->actionLube->setChecked(false);
            ui->actionLube->setEnabled(false);
            ui->btn_lube->setChecked(false);
            ui->btn_lube->setEnabled(false);
            old_type_io_lube = !get_lube_status();
        }

        if(get_task_state() == EMC_TASK_STATE_ON) {
            ui->actionHandle_guide->setEnabled(true);
            ui->btn_handguide->setEnabled(true);
        } else {
            ui->actionHandle_guide->setEnabled(false);
            ui->btn_handguide->setEnabled(false);
        }

        static bool old_type_io_flood = -1;
        if(get_task_state() == EMC_TASK_STATE_ON) {
            ui->actionCool->setEnabled(true);
            ui->btn_cool->setEnabled(true);
            if(old_type_io_flood != get_flood_status()){
                if(0 == get_flood_status()) {
                    ui->actionCool->setChecked(false);
                    ui->btn_cool->setChecked(false);
                }else{
                    ui->actionCool->setChecked(true);
                    ui->btn_cool->setChecked(true);
                }
            }
            old_type_io_flood = get_flood_status();
        } else {
            ui->actionCool->setChecked(false);
            ui->actionCool->setEnabled(false);
            ui->btn_cool->setChecked(false);
            ui->btn_cool->setEnabled(false);
            old_type_io_flood = !get_flood_status();
        }

        //jianmi 2015-8-14 add water out
        static int oldwater=-1;
        if(oldwater!=emcStatus->motion.synch_do[1]){
            if(0==emcStatus->motion.synch_do[1]){
                ui->actionWaterOut->setChecked(false);
                ui->btn_waterout->setChecked(false);
            }else{
               ui->actionWaterOut->setChecked(true);
               ui->btn_waterout->setChecked(true);
            }
            oldwater=emcStatus->motion.synch_do[1];
        }


        //jianmi 2015-8-14 add light control
        static int oldlight=-1;
        if(oldlight!=emcStatus->motion.synch_do[2]){
            if(0==emcStatus->motion.synch_do[2]){
                ui->actionLight->setChecked(false);
                ui->btn_light->setChecked(false);
            }else{
               ui->actionLight->setChecked(true);
               ui->btn_light->setChecked(true);
            }
            oldlight=emcStatus->motion.synch_do[2];
        }


        //add valve control

        if(emcStatus->motion.synch_do[1] == 0){
            if(ui->btn_cyclinder_a1->isChecked()){
                ui->btn_cyclinder_a1->setChecked(false);
            }
        }else{
            if(!ui->btn_cyclinder_a1->isChecked()){
                ui->btn_cyclinder_a1->setChecked(true);
            }
        }

        if(emcStatus->motion.synch_do[2] == 0){
            if(ui->btn_cyclinder_a2->isChecked()){
                ui->btn_cyclinder_a2->setChecked(false);
            }
        }else{
            if(!ui->btn_cyclinder_a2->isChecked()){
                ui->btn_cyclinder_a2->setChecked(true);
            }
        }

        if(emcStatus->motion.synch_do[3] == 0){
            if(ui->btn_cyclinder_b1->isChecked()){
                ui->btn_cyclinder_b1->setChecked(false);
            }
        }else{
            if(!ui->btn_cyclinder_b1->isChecked()){
                ui->btn_cyclinder_b1->setChecked(true);
            }
        }

        if(emcStatus->motion.synch_do[4] == 0){
            if(ui->btn_cyclinder_b2->isChecked()){
                ui->btn_cyclinder_b2->setChecked(false);
            }
        }else{
            if(!ui->btn_cyclinder_b2->isChecked()){
                ui->btn_cyclinder_b2->setChecked(true);
            }
        }

        static int valveE=-1;
        if(valveE!=emcStatus->motion.synch_do[13]){
            if(0==emcStatus->motion.synch_do[13]){
                ui->btn_ValveE->setChecked(false);
            }else{
               ui->btn_ValveE->setChecked(true);
            }
            valveE=emcStatus->motion.synch_do[13];
        }
        static int valvef=-1;
        if(valvef!=emcStatus->motion.synch_do[14]){
            if(0==emcStatus->motion.synch_do[14]){
                ui->btn_ValveF->setChecked(false);
            }else{
               ui->btn_ValveF->setChecked(true);
            }
            valvef=emcStatus->motion.synch_do[14];
        }



        if(par_get_var(3999)==1){
            /*******************手动模式的外部输入****************/

                   if(oldsyncdi1 != emcStatus->motion.synch_di[1]) {
                       if(get_pro_running()){
                           if(oldsyncdi1 == 0 && emcStatus->motion.synch_di[1]==1){
                               pro_pause();
                               show_msg(0,"触发外部暂停输入信号");
                           }
                           oldsyncdi1 = emcStatus->motion.synch_di[1];
                       }
                   }

                   if(oldsyncdi2 != emcStatus->motion.synch_di[2]) {
                       if(oldsyncdi2 == 0 && emcStatus->motion.synch_di[2]==1){
                           if(emcStatus->motion.synch_do[9]==0)
                               sendSetDIO(9,1);
                       }
                       oldsyncdi2 = emcStatus->motion.synch_di[2];
                   }
                   if(oldsyncdi3 != emcStatus->motion.synch_di[3]) {
                       if(oldsyncdi3 == 0 && emcStatus->motion.synch_di[3]==1){
                           if(emcStatus->motion.synch_do[9]==1)
                               sendSetDIO(9,0);
                       }
                       oldsyncdi3 = emcStatus->motion.synch_di[3];
                   }

           }

        if(par_get_var(3999)==7){
            /*******************手动模式的外部输入****************/

                   if(oldsyncdi1 != emcStatus->motion.synch_di[1]) {
                       if(get_pro_running()){
                           if(oldsyncdi1 == 0 && emcStatus->motion.synch_di[1]==1){
                               pro_pause();
                               show_msg(0,"触发外部暂停输入信号");
                           }
                           oldsyncdi1 = emcStatus->motion.synch_di[1];
                       }
                   }


                   if(oldsyncdi2 != emcStatus->motion.synch_di[2]) {
                       if(oldsyncdi2 == 0 && emcStatus->motion.synch_di[2]==1){
                           if(!get_pro_running()){
                               if(emcStatus->motion.synch_do[12]==0)
                                   sendSetDIO(12,1);
                               else
                                   sendSetDIO(12,0);
                           }
                       }
                       oldsyncdi2 = emcStatus->motion.synch_di[2];
                   }

           }

        if(par_get_var(3999)==4){

                if(0==emcStatus->motion.synch_di[3]){
                    ui->btn_go_a->setChecked(false);
                }else{
                   ui->btn_go_a->setChecked(true);
                }

                if(0==emcStatus->motion.synch_di[4]){
                    ui->btn_go_c->setChecked(false);
                }else{
                   ui->btn_go_c->setChecked(true);
                }

                if(0==emcStatus->motion.synch_di[5]){
                    ui->btn_go_b->setChecked(false);
                }else{
                   ui->btn_go_b->setChecked(true);
                }


                if(oldsyncdi1 != emcStatus->motion.synch_di[6]) {
                    if(oldsyncdi1 == 0 && emcStatus->motion.synch_di[6]==1){
                        if(!get_pro_running()){
                            if(emcStatus->motion.synch_do[13]==0)
                                sendSetDIO(13,1);
                            else
                                sendSetDIO(13,0);
                        }
                    }
                    oldsyncdi1 = emcStatus->motion.synch_di[6];
                }

        }

        if(par_get_var(3999)==5){

                if(0==emcStatus->motion.synch_di[3]){
                    ui->btn_go_a->setChecked(false);
                }else{
                   ui->btn_go_a->setChecked(true);
                }

                if(0==emcStatus->motion.synch_di[4]){
                    ui->btn_go_c->setChecked(false);
                }else{
                   ui->btn_go_c->setChecked(true);
                }

                if(1==emcStatus->motion.synch_di[1] && 0==emcStatus->motion.synch_di[4]  && 0==emcStatus->motion.synch_di[3]){
                    ui->btn_go_b->setChecked(true);
                }else{
                   ui->btn_go_b->setChecked(false);
                }

                if(oldsyncdi1 != emcStatus->motion.synch_di[6]) {
                    if(oldsyncdi1 == 0 && emcStatus->motion.synch_di[6]==1){
                        if(!get_pro_running()){
                            if(emcStatus->motion.synch_do[13]==0)
                                sendSetDIO(13,1);
                            else
                                sendSetDIO(13,0);
                        }
                    }
                    oldsyncdi1 = emcStatus->motion.synch_di[6];
                }

        }

        if(par_get_var(3999)==6){
            /*******************手动模式的外部输入****************/
               if(emcStatus->motion.synch_di[7]==0){

                   if(oldsyncdi1 != emcStatus->motion.synch_di[1]) {
                       if(oldsyncdi1 == 0 && emcStatus->motion.synch_di[1]==1){
                           if(emcStatus->motion.synch_do[9]==0)
                               sendSetDIO(9,1);
                       }
                       oldsyncdi1 = emcStatus->motion.synch_di[1];
                   }
                   if(oldsyncdi2 != emcStatus->motion.synch_di[2]) {
                       if(oldsyncdi2 == 0 && emcStatus->motion.synch_di[2]==1){
                           if(emcStatus->motion.synch_do[9]==1)
                               sendSetDIO(9,0);
                       }
                       oldsyncdi2 = emcStatus->motion.synch_di[2];
                   }

                   if(oldsyncdi3 != emcStatus->motion.synch_di[3]) {
                       if(oldsyncdi3 == 0 && emcStatus->motion.synch_di[3]==1){
                           if(emcStatus->motion.synch_do[10]==0)
                               sendSetDIO(10,1);
                       }else if(oldsyncdi3 == 1 && emcStatus->motion.synch_di[3]==0){
                           if(emcStatus->motion.synch_do[10]==1)
                               sendSetDIO(10,0);
                       }
                       oldsyncdi3 = emcStatus->motion.synch_di[3];
                   }

                   if(oldsyncdi4 != emcStatus->motion.synch_di[4]) {
                       if(oldsyncdi4 == 0 && emcStatus->motion.synch_di[4]==1){
                           if(emcStatus->motion.synch_do[11]==0)
                               sendSetDIO(11,1);
                       }
                       oldsyncdi4 = emcStatus->motion.synch_di[4];
                   }
                   if(oldsyncdi5 != emcStatus->motion.synch_di[5]) {
                       if(oldsyncdi5 == 0 && emcStatus->motion.synch_di[5]==1){
                           if(emcStatus->motion.synch_do[11]==1)
                               sendSetDIO(11,0);
                       }
                       oldsyncdi5 = emcStatus->motion.synch_di[5];
                   }

               }


               /**********************外部运行启动************************/

               static int oldPedal_signal3 = 1;//开机前信号为1不进来
               if(oldPedal_signal3 != emcStatus->motion.synch_di[6]){
                   if(1 == emcStatus->motion.synch_di[6]){
//                           on_btn_run_clicked();
                           show_msg(0,"运行启动");
                   }
               }
               oldPedal_signal3 = emcStatus->motion.synch_di[6];

        }


/*
        if(0 == emcStatus->motion.doublez.whichz_flag){
            ui->actionZ1->setChecked(1);
            ui->actionZ1->setEnabled(0);
        }else{
            ui->actionZ1->setChecked(0);
            ui->actionZ1->setEnabled(enable_type_1);
        }
        if(1 == emcStatus->motion.doublez.whichz_flag){
            ui->actionZ2->setChecked(1);
            ui->actionZ2->setEnabled(0);
        }else{
            ui->actionZ2->setChecked(0);
            ui->actionZ2->setEnabled(enable_type_1);
        }
        if(2 == emcStatus->motion.doublez.whichz_flag){
            ui->actionZ1_Z2->setChecked(1);
            ui->actionZ1_Z2->setEnabled(0);
        }else{
            ui->actionZ1_Z2->setChecked(0);
            ui->actionZ1_Z2->setEnabled(enable_type_1 && _all_homed
                                        && !zmode);
        }
*/

        int cur_manual_feed = get_actual_feed();
        if(EMC_TASK_MODE_MANUAL == emcStatus->task.mode &&
                cur_manual_feed <=0 ) {
            jog_count = 0;
        }

        ui->actionRepeat->setEnabled(get_task_state() == EMC_TASK_STATE_ON);
        ui->btn_repeat->setEnabled(get_task_state() == EMC_TASK_STATE_ON);
        //ui->actionSet_Parameters->setEnabled(get_task_interp() == EMC_TASK_INTERP_IDLE);
        ui->actionSet_IO->setEnabled(get_task_interp() == EMC_TASK_INTERP_IDLE);

        static int overlimit_flag = 0;

        for(int i=0; i<EMC_AXIS_MAX; i++){
            if(emcStatus->motion.axis[i].minHardLimit != 0){
                overlimit_flag = 1;
                break;
            }
            if(emcStatus->motion.axis[i].maxHardLimit != 0){
                overlimit_flag = 1;
                break;
            }
        }


        if(1 == overlimit_flag) {
            if(!ui->actionLimitRelease->isEnabled()) ui->actionLimitRelease->setEnabled(true);
        } else
            if(ui->actionLimitRelease->isEnabled()) ui->actionLimitRelease->setEnabled(false);
        overlimit_flag = 0;


        old_handleguide = get_handle_guide();
        old_changingz = get_changing_z();


        static int old_air_alarm = -1;
        if(old_air_alarm != air_alarm){
            if(1== air_alarm)
                ui->tab_manual->setEnabled(false);
            else
                ui->tab_manual->setEnabled(true);
        }

        old_air_alarm = air_alarm;



}

void linuxcnc::check_alm_sig()
{
    for(int i=0;i<DEFAULT_ALM_SIG;i++) {
        if(alm_sig[i]->exist && (SIMULATE || emcStatus->motion.traj.linktoEth > 0)) {
            if((1 == alm_sig[i]->polarity && (0 < _real_alm_pin[i] &&
               old_real_alm_pin[i] != _real_alm_pin[i])) ||
               (0 == alm_sig[i]->polarity && (0 == _real_alm_pin[i] &&
               old_real_alm_pin[i] != _real_alm_pin[i]))){



                    if(alm_sig[i]->type & 0x0001) {
                        _forbid_run = true;
                    }
                    if(alm_sig[i]->type>>1 & 0x0001) { //if in this,that will in others
                        getalarmabort = 1;
                        if(i == 19) {                               //气压报警暂停
                            sendProgramPause();
                        } else {
                            pro_abort();
                            ui->btn_abort->setDown(false);
                        }
                        sendSetlightcmd(3);
                    }
                    if(alm_sig[i]->type>>2 & 0x0001){
                        sendSpindleOff();
                        if(i != 19) {
                            _forbid_spindle = true;
                        }
                    }

                    if(alm_sig[i]->type>>3 & 0x0001){
                        _get_alm = true;
                    }

                    if(alm_sig[i]->type>>4 & 0x0001){
                        if(!get_homing(EMC_AXIS_MAX)){
                            _get_alm = true;
                        }

                    }

                    if(alm_sig[i]->type>>7 & 0x0001){
                        show_msg(0,tr("Get extern run signal"));
                        if(_forbid_run || _forbid_spindle)
                            return;

                        pro_run();
                    }
                    if(alm_sig[i]->type>>6 & 0x0001) {
                        if(0 == externabortfunc) {
                            show_msg(0,tr("Get extern abort signal"));
                            pro_abort();
                            ui->btn_abort->setDown(false);
                        } else {
                            show_msg(0,tr("Get extern pause signal"));
                            if(get_task_mode() == EMC_TASK_MODE_AUTO)
                                pro_pause();
                        }
                    }
                    if(_forbid_run || _forbid_spindle || _get_alm) {
                        char errorcode[200];
                        memset(errorcode,0,sizeof(errorcode));
                        QString errorstring = "",alminfo  = "";
                        int errorret = get_axis_alarm(&errorcode[0]);
                        errorstring.clear();
                        alminfo = alm_sig[i]->info;
                        if(errorret != 0) {
                               errorstring = alminfo.append(" ").append(errorcode);
                        } else {
                                errorstring = alminfo;
                        }
                        show_msg(2,errorstring);
                        updateStatus();
                        update_coor();
                        this->repaint();
                    }
            }
            old_real_alm_pin[i] = _real_alm_pin[i];

            if(alm_sig[i]->type>>8 & 0x0001){
                if(_real_alm_pin[i] > 0)
                    air_alarm =1;
                else
                    air_alarm = 0;
            }

        }
    }

    int aux_forbid_run = 0;
    if(_forbid_run){
        for(int i=0;i<DEFAULT_ALM_SIG;i++){
            if(1 == alm_sig[i]->exist){
                if(alm_sig[i]->type & 0x0001){
                    if((1 == alm_sig[i]->polarity && 0 < _real_alm_pin[i]) ||
                       (0 == alm_sig[i]->polarity && 0 == _real_alm_pin[i])){
                        aux_forbid_run = 1;
                        break;
                    }
                }
            }
        }
        if(0 == aux_forbid_run){
            //sendMachineOn();
            _forbid_run = false;
            if(get_task_mode() != EMC_TASK_MODE_AUTO)
                sendSetlightcmd(2);
            else
                sendSetlightcmd(1);

        }
    }

    int aux_forbid_spindle = 0;
    if(_forbid_spindle) {
        for(int i=0;i<DEFAULT_ALM_SIG;i++){
            if(alm_sig[i]->exist){
                if(alm_sig[i]->type>>1 & 0x0001){
                    if((1 == alm_sig[i]->polarity && 0 < _real_alm_pin[i]) ||
                       (0 == alm_sig[i]->polarity && 0 == _real_alm_pin[i])){
                        aux_forbid_spindle = 1;
                        break;
                    }
                }
            }
        }
        if(0 == aux_forbid_spindle){
            _forbid_spindle = false;
        }
    }

    int aux_gelalm = 0;
    if(_get_alm){
        for(int i=0;i<DEFAULT_ALM_SIG;i++){
            if(alm_sig[i]->exist){
                if(alm_sig[i]->type>>3 & 0x0001 || alm_sig[i]->type>>4 & 0x0001){
                    if((1 == alm_sig[i]->polarity && 0 < _real_alm_pin[i]) ||
                       (0 == alm_sig[i]->polarity && 0 == _real_alm_pin[i])){
                        aux_gelalm = 1;
                        break;
                    }
                }
            }
        }
        if(0 == aux_gelalm){
            _get_alm = false;
        }
    }



    static int old_get_runon2 = -1;static int count = 0;
    if(old_get_runon2 != get_task_mode()) {
        if((get_task_mode() == EMC_TASK_MODE_AUTO) && !get_verify()) {
            count++;
            if(count > 10) {
                //sendSetlightcmd(1);
                count = 0;
                old_get_runon2 = get_task_mode();
            }
        }
        if((get_task_mode() == EMC_TASK_MODE_MANUAL) &&  !_forbid_run) {
            sendSetlightcmd(2);
            old_get_runon2 = get_task_mode();
        }
    }
    if(get_task_mode() == EMC_TASK_MODE_MANUAL)
        count = 0;


    static int old_get_machine = -1;
    if(old_get_machine != get_task_state() &&
            get_task_state() != EMC_TASK_STATE_ON) {
        sendAutoRepeat(false);
        ui->actionRepeat->setChecked(false);
        ui->btn_repeat->setChecked(false);
    }
    old_get_machine = get_task_state();

    QString stylesheet = "";
    show_mode->setStyleSheet("font: 15pt Sans Serif");
    int fontsize = 15;
    if(QApplication::desktop()->screenGeometry().height() < 780) {
        fontsize = 11;
        show_mode->setStyleSheet("font: 12pt Sans Serif");
    }

    if(get_task_mode() == EMC_TASK_MODE_MANUAL) {
        if(0 == ui->mode_tab->currentIndex()){
            if(get_autorepeat())
                show_mode->setText(tr("AUTO(REPEAT)"));
            else
                show_mode->setText(tr("AUTO"));
        }else
            show_mode->setText(tr("MANUAL"));

        if(_forbid_run || _forbid_spindle) {
            show_status->setText(tr("WARNNING"));
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:red",fontsize);
            show_status->setStyleSheet(stylesheet);
        } else if(get_actual_feed() != 0) {
            show_status->setText(tr("MANUAL_MOVE"));
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:lightblue",fontsize);
            show_status->setStyleSheet(stylesheet);
        }else if(get_handle_guide()){
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:yellow",fontsize);
            show_status->setStyleSheet(stylesheet);
            show_status->setText(tr("HANDLE_GUIDE"));
        }else if(jog_mode == JOG_HANDLE){
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:yellow",fontsize);
            show_status->setStyleSheet(stylesheet);
            show_status->setText(tr("HANDLE"));
        }else if(jog_mode == JOG_INCREMENT){
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:yellow",fontsize);
            show_status->setStyleSheet(stylesheet);
            show_status->setText(tr("INCREMENT"));
        }else if(jog_mode == JOG_CONTINUE){
            stylesheet.sprintf("font: %dpt Sans Serif;",fontsize);
            show_status->setStyleSheet(stylesheet);
            show_status->setText(tr("IDLE"));
        }

    } else {

        if(get_autorepeat())
            show_mode->setText(tr("AUTO(REPEAT)"));
        else
            show_mode->setText(tr("AUTO"));
        if(_forbid_run || _forbid_spindle) {
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:red",fontsize);
            show_status->setStyleSheet(stylesheet);
            show_status->setText(tr("WARNNING"));
        } else if(start_verify > 0) {
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:blue",fontsize);
            show_status->setStyleSheet(stylesheet);
            show_status->setText(tr("Verify"));
        }else if(get_handle_guide()) {
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:yellow",fontsize);
            show_status->setStyleSheet(stylesheet);
            show_status->setText(tr("HANDLE_GUIDE"));
        } else if(0 == set_feed_override->value()) {
            show_status->setText(tr("ZERO_FEED"));
        } else if(EMC_TASK_INTERP_PAUSED == get_task_interp()) {
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:yellow",fontsize);
            show_status->setStyleSheet(stylesheet);
            show_status->setText(tr("PAUSE"));
        } else if(EMC_TASK_INTERP_IDLE != get_task_interp()) {
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:green;color:white",fontsize);
            show_status->setStyleSheet(stylesheet);
            show_status->setText(tr("RUNNING"));

            cycled= par_get_var(4050);
            QString strcycletimes=QString("%1/%2").arg(cycled).arg(ncycletimes);
            ui->show_cycle_times->setText(strcycletimes);
        } else{
            stylesheet.sprintf("font: %dpt Sans Serif;",fontsize);
            show_status->setStyleSheet(stylesheet);
            show_status->setText(tr("IDLE"));
        }
    }

    if(emcStatus->motion.traj.linktoEth == 0 && SIMULATE == 0) {
        show_mode->setText(tr("通讯断开"));
        stylesheet.sprintf("background-color:red;color:yellow");
        show_mode->setStyleSheet(stylesheet);
    }


    static int infoflag = 0;
    if(infoForbid) {
        infoflag = 1;
        ui->statusBar->clearMessage();
        QPalette showstatus;
        showstatus.setColor(QPalette::WindowText,Qt::yellow);
        if(!hard_checkok()) {
            show_info->setText(tr("hardware is not compatible"));
        } else if (get_fpgacheck(0)) {
            show_info->setText(tr("EPP Communication is error"));
        } else if (get_fpgacheck(1)) {
            show_info->setText(tr("LTK Reset failed"));
        } else if (get_fpgacheck(3)) {
            show_info->setText(tr("IO_AA Communication is error"));
        } else if (get_fpgacheck(4) && 1 == externIoexit) {
            show_info->setText(tr("ExternIo Communication is error"));
        } else if(register_timeout()){
            show_info->setText(tr("registe time is timeout"));
        }else if(authorize_timeout()){
            show_info->setText(tr("authorize time is timeout"));
        }else if(!version_checkok())
            show_info->setText(tr("Version check failed !"));
        show_info->setPalette(showstatus);
        show_info->setStyleSheet("background-color:red;color:yellow");
    }

    static int alm_flag = 0;
    static int permanent_count = 0;
    if((_forbid_run || _forbid_spindle || _get_alm) &&
            !infoForbid) {
        alm_flag = 1;
        for(int i=0;i<DEFAULT_ALM_SIG;i++) {
            if(alm_sig[i]->exist) {
                if((1 == alm_sig[i]->polarity && 0 < _real_alm_pin[i]) ||
                        (0 == alm_sig[i]->polarity && 0 == _real_alm_pin[i])){

                        ui->statusBar->clearMessage();
                        inter_translate(&alm_sig[i]->info);
                        char errorcode[200];
                        memset(errorcode,0,sizeof(errorcode));
                        QString errorstring = "",alminfo  = "";
                        int errorret = get_axis_alarm(&errorcode[0]);
                        errorstring.clear();
                        alminfo = alm_sig[i]->info;
                        if(errorret != 0) {
                               errorstring = alminfo.append(" ").append(errorcode);
                        } else {
                                errorstring = alminfo;
                        }
                        show_info->setText(errorstring);
                        stylesheet.sprintf("font: %dpt Sans Serif;background-color:red;color:yellow",fontsize);
                        show_info->setStyleSheet(stylesheet);
                        break;
                }
            }
        }
    } else if(permanent_tips > 0) {
        permanentflag = 1;
        permanent_count++;
        if(permanent_count > 3 && permanent_tips == 1) {
            ushort ch[] = {0x25CF,0};
            permanent_text.append(QString::fromUtf16(ch));
            show_info->setText(permanent_text);
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:yellow;color:black",fontsize);
            show_info->setStyleSheet(stylesheet);
            permanent_count = 0;
            if(show_info->width() - permanent_text.size()*fontsize < 20) {
                permanent_text.remove(QString::fromUtf16(ch));
            }
        } else {
            show_info->setText(permanent_text);
            stylesheet.sprintf("font: %dpt Sans Serif;background-color:yellow;color:black",fontsize);
        }
    }  else if(!_forbid_run && !_forbid_spindle && (alm_flag || infoflag || permanentflag) &&
              !infoForbid ) {
        if(show_info->text().compare(alm_sig[39]->info) != 0) {
            show_info->setText("");
            QPalette showstatus;
            showstatus.setColor(QPalette::WindowText,Qt::black);
            show_info->setPalette(showstatus);
            stylesheet.sprintf("font: %dpt Sans Serif;",fontsize);
            show_info->setStyleSheet(stylesheet);
        }
        alm_flag = 0;
        infoflag = 0;
        permanentflag = 0;
    }

    autoSwitchhandle();

    static int oldtoolchange1 = -1;
    if(oldtoolchange1 != emcStatus->motion.mot_extern_in[0]) {
        if(emcStatus->motion.mot_extern_in[0] > 0) {
            permanent_tips = 1;
            permanent_text = tr("Now Spindle1 changing tool");
        }
        if(emcStatus->motion.mot_extern_in[0] == 0) {
            permanent_tips = 0;
        }
        oldtoolchange1 = emcStatus->motion.mot_extern_in[0];
    }

    static int oldtoolchange2 = -1;
    if(oldtoolchange2 != emcStatus->motion.mot_extern_in[1]) {
        if(emcStatus->motion.mot_extern_in[1] > 0) {
            permanent_tips = 1;
            permanent_text = tr("Now Spindle2 changing tool");
        }
        if(emcStatus->motion.mot_extern_in[1] == 0) {
            permanent_tips = 0;
        }
        oldtoolchange2 = emcStatus->motion.mot_extern_in[1];
    }

    static int oldtoolchange3 = -1;
    if(oldtoolchange3 != emcStatus->motion.mot_extern_in[2]) {
        if(emcStatus->motion.mot_extern_in[2] > 0) {
            permanent_tips = 1;
            permanent_text = tr("Now Spindle3 changing tool");
        }
        if(emcStatus->motion.mot_extern_in[2] == 0) {
            permanent_tips = 0;
        }
        oldtoolchange3 = emcStatus->motion.mot_extern_in[2];
    }

    static int oldtoolchange4 = -1;
    if(oldtoolchange4 != emcStatus->motion.mot_extern_in[3]) {
        if(emcStatus->motion.mot_extern_in[3] > 0) {
            permanent_tips = 1;
            permanent_text = tr("Now Spindle4 changing tool");
        }
        if(emcStatus->motion.mot_extern_in[3] == 0) {
            permanent_tips = 0;
        }
        oldtoolchange4 = emcStatus->motion.mot_extern_in[3];
    }

    if(1==_ini->read("PAR_NEED_TOOLCHANGE_CHECK","VALUE","1").toInt()) {
        static int oldsignal1 = 0,oldsignal2 = 0,oldsignal3 = 0,oldsignal4 = 0;
        if(emcStatus->motion.mot_extern_in[0] != oldsignal1) {
            if(emcStatus->motion.mot_extern_in[0] > 0) {
                qDebug()<<"now in set toolchange flag 1";
//                if(toolchangecheck == 0)
//                    toolchangecheck = 1;
//                else if(toolchangecheck == 2){
//                    toolchangecheck = 3;
//                }
                toolchangecheck[0] = 1;
            }
            oldsignal1 = emcStatus->motion.mot_extern_in[0];
        }
        if(emcStatus->motion.mot_extern_in[1] != oldsignal2) {
            if(emcStatus->motion.mot_extern_in[1] > 0) {
                qDebug()<<"now in set toolchange flag 2";
//                if(toolchangecheck == 0)
//                    toolchangecheck = 2;
//                else if(toolchangecheck == 1){
//                    toolchangecheck = 3;
//                }
                toolchangecheck[1] = 1;

            }
            oldsignal2 = emcStatus->motion.mot_extern_in[1];
        }
        if(emcStatus->motion.mot_extern_in[2] != oldsignal3) {
            if(emcStatus->motion.mot_extern_in[2] > 0) {
                qDebug()<<"now in set toolchange flag 3";
                toolchangecheck[2] = 1;
            }
            oldsignal3 = emcStatus->motion.mot_extern_in[2];
        }
        if(emcStatus->motion.mot_extern_in[3] != oldsignal4) {
            if(emcStatus->motion.mot_extern_in[3] > 0) {
                qDebug()<<"now in set toolchange flag 4";
                toolchangecheck[3] = 1;
            }
            oldsignal4 = emcStatus->motion.mot_extern_in[3];
        }
    }

    static int lube_flag = -1,lubeonflag = 0,timestart = 0;
    static double timerun = 0;
    if(lube_flag != get_lube_status()){
        if(1 == get_lube_status()) {
            permanent_tips = 2;
            lubeonflag = 1;
            timestart = _ini->read("PAR_OPERATOR_LUBEWORKTIME","VALUE").toInt();
            permanent_text = QString(tr("Lube On left time:%1 sec")).arg(timestart);
            timerun = etime();
        } else {
            permanent_tips = 0;
            lubeonflag = 0;
            timestart = 0;
        }
        lube_flag = get_lube_status();
    }

    if(lubeonflag > 0 && get_lube_status() > 0) {
        if(etime() - timerun > 1) {
            permanent_text = QString(tr("Lube On left time:%1 sec")).arg(--timestart);
            timerun = etime();
        }
    }

}


void linuxcnc::closeEvent(QCloseEvent *event)
{
    if(emcStatus->task.runState != EMC_IDLE && (emcStatus->motion.traj.linktoEth==1 && SIMULATE==0)){
        int ret=QMessageBox::information(this,tr("提示"),
                                         tr("请先退出运行!!"),
                                         QMessageBox::Ok);
        event->ignore();
        return;
    }
    int ret =  QMessageBox::warning(this, tr("WARNING"),
                                     tr("Ensure to Exit?"),
                                    tr("Ok"),tr("Cancel"));
    if(ret == Cancel) {
        event->ignore();
        return;
    }
    else if(ret == Ok)
    {
        pro_abort();
        flush_time_status->stop();
        flush_time->stop();
        usleep(100*1000);
        system("killall milltask");
        system("killall linuxsvr");
        system("/home/.cnc/bin/halcmd stop");
        system("/home/.cnc/bin/halcmd unload all");
        system("/home/.cnc/bin/realtime stop");
        usleep(100*1000);
        app->exit(0);

    }

}

bool linuxcnc::eventFilter(QObject *obj, QEvent *event)
{
//    if(par_get_var(3999) != 4 && par_get_var(3999) != 5 && par_get_var(3999) != 1){
        if(event->type() == QEvent::MouseButtonPress && obj == ui->coor_area) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::RightButton) {
                this->on_actionSet_Offset_triggered();
            }
        }
//    }

    if(event->type() == QEvent::KeyRelease ||
            event->type() == QEvent::MouseButtonRelease) {
        if(obj == ui->btn_abort) {
            ui->btn_abort->setDown(false);
        }
    }

    if(loadingmodelflag) {
        if(event->type() == QEvent::KeyPress)
            return true;
        if(obj == ui->btn_pause || obj == ui->btn_run ||
                obj == ui->btn_abort || obj == ui->btn_open ||
                obj == ui->action_Open) {
            if(event->type() == QEvent::MouseButtonPress ||
                    event->type() == QEvent::MouseButtonDblClick||
                    event->type() == QEvent::MouseButtonRelease)
               return true;
        }
    }

    if(jog_count >0 && btn_jog_count ==0 ) {
        if(event->type() == QEvent::Shortcut)
            return true;
        if(event->type() == QEvent::MouseButtonPress ||
                event->type() == QEvent::MouseButtonDblClick||
                event->type() == QEvent::MouseButtonRelease) {

            return true;
        if( obj == ui->btn_x_decrease ||obj == ui->btn_y_decrease_2 ||
                obj == ui->btn_y_decrease || obj == ui->btn_x_plus ||
                obj == ui->btn_y_plus||obj == ui->btn_y_plus_2
                || obj == ui->btn_handle || obj == ui->btn_open ||
                obj == ui->btn_run || obj == ui->btn_incr001 || obj == ui->btn_incr01
                || obj == ui->btn_incr1|| obj == ui->btn_incr10 || obj == ui->btn_incruser
                || obj == ui->btn_incr_set) {
                  return true;
            }
        }
    }

/*
    if((event->type() == QEvent::KeyPress ||
        event->type() == QEvent::KeyRelease)
            && (this->isActiveWindow()) ) {

        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        if(keyEvent->key() == Qt::Key_Space &&
                obj != ui->mdi_text)
            return true;


        if((keyEvent->key() == Qt::Key_Control && 0==keychoice) || (1 == keychoice && keyEvent->key() == Qt::Key_5)) {

            if(event->type() == QEvent::KeyPress) {
                jogspeed = fastspeed;
                ui->btn_cen->setText(tr("High Feed"));
                //ui->btn_velswitch_1->setChecked(true);
                //ui->btn_velswitch_1->setText(tr("High Feed"));
            }
            if(event->type() == QEvent::KeyRelease) {
                jogspeed = defspeed;
                ui->btn_cen->setText(tr("Default Feed"));
                //ui->btn_velswitch_1->setChecked(false);
                //ui->btn_velswitch_1->setText(tr("Default Feed"));
            }
        }

        if((keychoice== 1 &&  (keyEvent->key() == Qt::Key_8   ||
                               keyEvent->key() == Qt::Key_2 ||
                               keyEvent->key() == Qt::Key_4 ||
                               keyEvent->key() == Qt::Key_5 ||
                               keyEvent->key() == Qt::Key_6||
                               keyEvent->key() == Qt::Key_7 ||
                               keyEvent->key() == Qt::Key_1 ||
                                    keyEvent->key() == Qt::Key_9 ||
                                    keyEvent->key() == Qt::Key_3 ))
                || (keychoice== 0 &&  (keyEvent->key() == Qt::Key_Up   ||
           keyEvent->key() == Qt::Key_Down ||
           keyEvent->key() == Qt::Key_Left ||
           keyEvent->key() == Qt::Key_Right||
           keyEvent->key() == Qt::Key_PageUp ||
           keyEvent->key() == Qt::Key_PageDown ||
                keyEvent->key() == Qt::Key_Home ||
                keyEvent->key() == Qt::Key_End ))  ||
                keyEvent->key() == Qt::Key_Shift ||
                keyEvent->key() == Qt::Key_F1 ||
                keyEvent->key() == Qt::Key_F2 ||
                keyEvent->key() == Qt::Key_F3 ||
                keyEvent->key() == Qt::Key_F4) {

            if(ui->menu_file->isHidden() && ui->menu_Machine->isHidden() &&
                 ui->menu_Tool->isHidden() &&   ui->menu_Operation->isHidden()
                    && ui->menu_Help->isHidden() && ui->mode_tab->currentWidget() == ui->tab_manual) {

            if(event->type() == QEvent::KeyPress)
                globalkeyPressEvent(keyEvent);
            else
                globalkeyReleaseEvent(keyEvent);

            return true;
            }
        }

        if(obj == info_table_widget) {
                 return true;
        }
    }*/

    return QMainWindow::eventFilter(obj,event);


}



void linuxcnc::globalkeyPressEvent(QKeyEvent *e) {
    if(get_changing_z())
        return;

    /*
    if(e->modifiers() == Qt::ShiftModifier  && e->key() == Qt::Key_F1) {
        keyboardcontrolIO1 = 1;
    }
    if(e->modifiers() == Qt::ShiftModifier  && e->key() == Qt::Key_F2) {
        keyboardcontrolIO2 = 1;
    }
    if(e->modifiers() == Qt::ShiftModifier  && e->key() == Qt::Key_F3) {
        keyboardcontrolIO3 = 1;
    }
    if(e->modifiers() == Qt::ShiftModifier  && e->key() == Qt::Key_F4) {
        keyboardcontrolIO4 = 1;
    }
    */
    if(e->isAutoRepeat()) return;
    jog_count++;
    if(get_pro_running())
        return;
    if(btn_jog_count) return;
    if(ui->mode_tab->currentWidget()->objectName() != "tab_manual")
        return;
    if(! ui->btn_x_plus->isEnabled()) return;

    double jog_speed = 0;
    if(!_all_homed) {
        jog_speed = qMin(par_get_var(4017),jogspeed)*60;
    } else
        jog_speed = jogspeed;

    /*
    if((e->key() == Qt::Key_Right && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_6)) {
        if(e->isAutoRepeat()) return;
        ui->btn_x_plus->setDown(true);
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(0,jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(0,jog_speed,jog_increment);
    }


    if((e->key() == Qt::Key_Up && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_8)) {
        if(e->isAutoRepeat()) return;
        ui->btn_y_plus->setDown(true);

        if(jog_mode == JOG_CONTINUE)
            sendJogCont(1,jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(1,jog_speed,jog_increment);

    }

    if((e->key() == Qt::Key_Down && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_2)) {
        if(e->isAutoRepeat()) return;
        ui->btn_y_decrease->setDown(true);

        if(jog_mode == JOG_CONTINUE)
            sendJogCont(1,-jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(1,jog_speed,-jog_increment);

    }

    if((e->key() == Qt::Key_Left && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_4)) {
        if(e->isAutoRepeat()) return;
        ui->btn_x_decrease->setDown(true);

        if(jog_mode == JOG_CONTINUE)
            sendJogCont(0,-jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(0,-jog_speed,jog_increment);
    }


    if((e->key() == Qt::Key_PageUp && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_9)) {
        if(jog_mode == JOG_CONTINUE)
            sendJogCont(2,jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(2,jog_speed,jog_increment);
            now_axis = 2;
    }

    if((e->key() == Qt::Key_PageDown && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_1)) {

        if(jog_mode == JOG_CONTINUE)
            sendJogCont(2,-jog_speed);
        else if(jog_mode == JOG_INCREMENT)
            sendJogIncr(2,-jog_speed,jog_increment);
            now_axis = 2;

    }*/



//    if((e->key() == Qt::Key_Home && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_7)) {
//        if(get_task_z() == 0) return;
//        if(e->isAutoRepeat()) return;
//        ui->btn_a_plus->setDown(true);
//        if(jog_mode == JOG_CONTINUE)
//            sendJogCont(3,jog_speed);
//        else if(jog_mode == JOG_INCREMENT)
//            sendJogIncr(3,jog_speed,jog_increment);
//            now_axis = 3;
//    }

//    if((e->key() == Qt::Key_End && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_3)) {
//        if(get_task_z() == 0) return;
//        if(e->isAutoRepeat()) return;
//        ui->btn_a_decrease->setDown(true);
//        if(jog_mode == JOG_CONTINUE)
//            sendJogCont(3,-jog_speed);
//        else if(jog_mode == JOG_INCREMENT)
//            sendJogIncr(3,-jog_speed,jog_increment);
//            now_axis = 3;
//    }



}

void linuxcnc::updateRecentAction() {


    QStringList list_temp;
    QList<QAction *> acts= Recent_files->actions();
    while(!acts.isEmpty()) {
        QAction *tmp = acts.takeFirst();
        delete tmp;
    }
    Recent_files->clear();

    QString key;
    for(int i=0;i<path_list.size();i++) {
        if(!list_temp.contains(path_list.at(i))){
            QAction *action = new QAction(path_list.at(i),this);
            Recent_files->addAction(action);
            list_temp.append(path_list.at(i));
        }

        key = QString("FILE_PATH/path%1").arg(i);
        _prefer->writeEntry(key,path_list.at(i));

    }
    list_temp.clear();
}

void linuxcnc::resizeEvent(QResizeEvent *ev) {
    //qDebug()<<ev->size().width()<<ev->size().height();
    if(QApplication::desktop()->screenGeometry().height() < 780) {
        double scale = (double)ev->size().height() / 760.0;
        QString font = QString("font: %1pt Sans Serif").arg(qMax(11*scale*0.9,9.0));
        this->setStyleSheet(font);
        ui->btn_x_plus->setMinimumHeight(50*scale);
        ui->btn_y_plus->setMinimumHeight(50*scale);
        ui->btn_y_plus_2->setMinimumHeight(50*scale);
        ui->btn_z_plus->setMinimumHeight(50*scale);
        ui->btn_a_plus->setMinimumHeight(50*scale);
        ui->btn_b_plus->setMinimumHeight(50*scale);
        ui->btn_c_plus->setMinimumHeight(50*scale);
        ui->btn_x_decrease->setMinimumHeight(50*scale);
        ui->btn_y_decrease->setMinimumHeight(50*scale);
        ui->btn_y_decrease_2->setMinimumHeight(50*scale);
        ui->btn_z_decrease->setMinimumHeight(50*scale);
        ui->btn_a_decrease->setMinimumHeight(50*scale);
        ui->btn_b_decrease->setMinimumHeight(50*scale);
        ui->btn_c_decrease->setMinimumHeight(50*scale);
//        ui->btn_velswitch_1->setMinimumHeight(50*scale);
//        ui->btn_velswitch_2->setMinimumHeight(50*scale);
        ui->btn_cen->setMinimumHeight(50*scale);
       // ui->btn_pick_x->setMinimumHeight(30*scale);
        ui->btn_continue->setMinimumHeight(50*scale);
        ui->btn_handle->setMinimumHeight(50*scale);

        ui->btn_incr001->setMinimumHeight(50*scale);
        ui->btn_incr01->setMinimumHeight(50*scale);
        ui->btn_incr1->setMinimumHeight(50*scale);
        ui->btn_incr10->setMinimumHeight(50*scale);
        ui->btn_incr_set->setMinimumHeight(50*scale);
        ui->btn_incruser->setMinimumHeight(50*scale);


        font = QString("font: %1pt Sans Serif").arg(qMin(9*scale,20.0));
        for(int i=0;i<9;i++) {
            show_reference_coor[i]->setStyleSheet(font);
        }
        show_offset_x->setStyleSheet(font);
        show_offset_y->setStyleSheet(font);
        show_offset_y_2->setStyleSheet(font);
        show_offset_z1->setStyleSheet(font);
        show_offset_a->setStyleSheet(font);
        show_offset_c->setStyleSheet(font);
        show_offset_b->setStyleSheet(font);
        ui->label_x->setStyleSheet(font);
        ui->label_y->setStyleSheet(font);
        ui->label_y_2->setStyleSheet(font);
        ui->label_z1->setStyleSheet(font);
        ui->label_a->setStyleSheet(font);
        ui->label_b->setStyleSheet(font);
        ui->label_c->setStyleSheet(font);
        ui->show_abs_x->setStyleSheet(font);
        ui->show_abs_y->setStyleSheet(font);
        ui->show_abs_y_2->setStyleSheet(font);
        ui->show_abs_z1->setStyleSheet(font);
        ui->show_abs_a->setStyleSheet(font);
        ui->show_abs_b->setStyleSheet(font);
        ui->show_abs_c->setStyleSheet(font);
//        ui->show_left_x->setStyleSheet(font);
//        ui->show_left_y->setStyleSheet(font);
//        ui->show_left_y_2->setStyleSheet(font);
//        ui->show_left_z->setStyleSheet(font);
//        ui->show_left_a->setStyleSheet(font);
//        ui->show_left_b->setStyleSheet(font);
//        ui->show_left_c->setStyleSheet(font);
        ui->label_39->setStyleSheet(font);
        ui->label_6->setStyleSheet(font);
        ui->label_7->setStyleSheet(font);


        ui->mainToolBar->setIconSize(QSize(14,14));


        ui->label_13->setMinimumWidth(50*scale*1.1);
        ui->label_13->setMaximumWidth(50*scale*1.1);
        ui->label_14->setMinimumWidth(50*scale*1.1);
        ui->label_14->setMaximumWidth(50*scale*1.1);
        ui->label_16->setMinimumWidth(50*scale*1.1);
        ui->label_16->setMaximumWidth(50*scale*1.1);
        ui->label_18->setMinimumWidth(50*scale*1.1);
        ui->label_18->setMaximumWidth(50*scale*1.1);
        ui->label_20->setMinimumWidth(50*scale*1.1);
        ui->label_20->setMaximumWidth(50*scale*1.1);
        ui->label_32->setMinimumWidth(50*scale*1.1);
        ui->label_32->setMaximumWidth(50*scale*1.1);
        ui->label_25->setMinimumWidth(50*scale*1.1);
        ui->label_25->setMaximumWidth(50*scale*1.1);
        ui->label_26->setMinimumWidth(50*scale*1.1);
        ui->label_26->setMaximumWidth(50*scale*1.1);
        ui->label_27->setMinimumWidth(50*scale*1.1);
        ui->label_27->setMaximumWidth(50*scale*1.1);
        ui->label_29->setMinimumWidth(50*scale*1.1);
        ui->label_29->setMaximumWidth(50*scale*1.1);
        ui->spindle_override_widget->setMinimumWidth(100*scale*1.1);
        ui->feed_override_widget->setMinimumWidth(100*scale*1.1);

        ui->showframe2->setMinimumWidth(340*scale);
        ui->feed_override_widget->setMaximumHeight(35);
        ui->spindle_override_widget->setMaximumHeight(35);

        ui->show_run_time->setMinimumWidth(50);
        ui->show_tool_now->setMinimumWidth(50);
        ui->show_tool_comp->setMinimumWidth(26);
        ui->show_set_feed->setMinimumWidth(40);
        ui->show_set_spindle->setMinimumWidth(40);
        ui->label_333->setMinimumWidth(25);
        ui->label_24->setMinimumWidth(25);
        ui->show_act_feed->setMinimumWidth(50);
        ui->show_act_spindle->setMinimumWidth(50);
        ui->label_33->hide();


        ui->show_homex_pic->setFixedSize(16,16);
        ui->show_homey_pic->setFixedSize(16,16);
        ui->show_homey_pic_2->setFixedSize(16,16);
        ui->show_homez1_pic->setFixedSize(16,16);
        ui->show_homea_pic->setFixedSize(16,16);
        ui->show_homeb_pic->setFixedSize(16,16);
        ui->show_homec_pic->setFixedSize(16,16);
        ui->label_10->setFixedSize(16,16);

        ui->z1firstprobe->setMaximumWidth(65);
        ui->z2firstprobe->setMaximumWidth(65);
        ui->z3firstprobe->setMaximumWidth(65);
        ui->z4firstprobe->setMaximumWidth(65);
        ui->z1secondprobe->setMaximumWidth(65);
        ui->z2secondprobe->setMaximumWidth(65);
        ui->z3secondprobe->setMaximumWidth(65);
        ui->z4secondprobe->setMaximumWidth(65);



    } else if (QApplication::desktop()->screenGeometry().height() > 890) {
        double scale = (double)ev->size().height() / 760.0;
        scale *= scale;
        QString font = QString("font: %1pt Sans Serif").arg(qMin(11*scale,13.0));
        this->setStyleSheet(font);
        ui->btn_x_plus->setMinimumHeight(40*scale);
        ui->btn_y_plus->setMinimumHeight(40*scale);
        ui->btn_y_plus_2->setMinimumHeight(40*scale);
        ui->btn_z_plus->setMinimumHeight(40*scale);
        ui->btn_a_plus->setMinimumHeight(40*scale);
        ui->btn_b_plus->setMinimumHeight(40*scale);
        ui->btn_c_plus->setMinimumHeight(40*scale);
        ui->btn_x_decrease->setMinimumHeight(40*scale);
        ui->btn_y_decrease->setMinimumHeight(40*scale);
        ui->btn_y_decrease_2->setMinimumHeight(40*scale);
        ui->btn_z_decrease->setMinimumHeight(40*scale);
        ui->btn_a_decrease->setMinimumHeight(40*scale);
        ui->btn_b_decrease->setMinimumHeight(40*scale);
        ui->btn_c_decrease->setMinimumHeight(40*scale);
//        ui->btn_velswitch_1->setMinimumHeight(40*scale);
//        ui->btn_velswitch_2->setMinimumHeight(40*scale);
        ui->btn_cen->setMinimumHeight(40*scale);
        //ui->btn_pick_x->setMinimumHeight(40*scale);
        ui->btn_continue->setMinimumHeight(35*scale);
        ui->btn_handle->setMinimumHeight(35*scale);

        ui->mainToolBar->setIconSize(QSize(16,16));

        ui->show_homex_pic->setMaximumHeight(14*scale);
        ui->show_homex_pic->setMaximumWidth(14*scale);
        ui->show_homey_pic->setMaximumHeight(14*scale);
        ui->show_homey_pic->setMaximumWidth(14*scale);
        ui->show_homey_pic_2->setMaximumSize(14*scale,14*scale);
        ui->show_homez1_pic->setMaximumHeight(14*scale);
        ui->show_homez1_pic->setMaximumWidth(14*scale);
        ui->show_homea_pic->setMaximumHeight(14*scale);
        ui->show_homea_pic->setMaximumWidth(14*scale);
        ui->show_homea_pic->setMaximumHeight(14*scale);
        ui->show_homea_pic->setMaximumWidth(14*scale);
        ui->show_homeb_pic->setMaximumHeight(14*scale);
        ui->show_homeb_pic->setMaximumWidth(14*scale);
        ui->show_homec_pic->setMaximumHeight(14*scale);
        ui->show_homec_pic->setMaximumWidth(14*scale);

        ui->label_10->setMaximumHeight(30*scale);
        ui->label_10->setMaximumWidth(30*scale);

        font = QString("font: %1pt Sans Serif").arg(qMin(12*scale,20.0));
        for(int i=0;i<9;i++) {
            show_reference_coor[i]->setStyleSheet(font);
        }
        show_offset_x->setStyleSheet(font);
        show_offset_y->setStyleSheet(font);
        show_offset_y_2->setStyleSheet(font);
        show_offset_z1->setStyleSheet(font);
        show_offset_a->setStyleSheet(font);
        show_offset_c->setStyleSheet(font);
        show_offset_b->setStyleSheet(font);
        ui->label_x->setStyleSheet(font);
        ui->label_y->setStyleSheet(font);
        ui->label_y_2->setStyleSheet(font);
        ui->label_z1->setStyleSheet(font);
        ui->label_a->setStyleSheet(font);
        ui->label_b->setStyleSheet(font);
        ui->label_c->setStyleSheet(font);
        ui->show_abs_x->setStyleSheet(font);
        ui->show_abs_y->setStyleSheet(font);
        ui->show_abs_y_2->setStyleSheet(font);
        ui->show_abs_z1->setStyleSheet(font);
        ui->show_abs_a->setStyleSheet(font);
        ui->show_abs_b->setStyleSheet(font);
        ui->show_abs_c->setStyleSheet(font);
//        ui->show_left_x->setStyleSheet(font);
//        ui->show_left_y->setStyleSheet(font);
//        ui->show_left_y_2->setStyleSheet(font);
//        ui->show_left_z->setStyleSheet(font);
//        ui->show_left_a->setStyleSheet(font);
//        ui->show_left_b->setStyleSheet(font);
//        ui->show_left_c->setStyleSheet(font);
        ui->label_39->setStyleSheet(font);
        ui->label_6->setStyleSheet(font);
        ui->label_7->setStyleSheet(font);
        ui->show_run_time->setMinimumWidth(100);
        ui->show_tool_now->setMinimumWidth(100);
        ui->show_tool_comp->setMinimumWidth(54);
        ui->show_set_feed->setMinimumWidth(50);
        ui->show_set_spindle->setMinimumWidth(50);
        ui->label_333->setMinimumWidth(50);
        ui->label_24->setMinimumWidth(50);
        ui->show_act_feed->setMinimumWidth(70);
        ui->show_act_spindle->setMinimumWidth(70);
        ui->spindle_override_widget->setMinimumWidth(180);
        ui->feed_override_widget->setMinimumWidth(180);

        ui->show_run_time->setMinimumWidth(100);
        ui->show_tool_now->setMinimumWidth(100);
        ui->show_tool_comp->setMinimumWidth(54);
        ui->show_set_feed->setMinimumWidth(50);
        ui->show_set_spindle->setMinimumWidth(50);
        ui->label_333->setMinimumWidth(50);
        ui->label_24->setMinimumWidth(50);
        ui->show_act_feed->setMinimumWidth(70);
        ui->show_act_spindle->setMinimumWidth(70);
        ui->spindle_override_widget->setMinimumWidth(180);
        ui->feed_override_widget->setMinimumWidth(180);
        ui->feed_override_widget->setMaximumHeight(35);
        ui->spindle_override_widget->setMaximumHeight(35);


    } else {

        QString font = QString("font: %1pt Sans Serif").arg(11);
        this->setStyleSheet(font);
        ui->btn_x_plus->setMinimumHeight(40);
        ui->btn_y_plus->setMinimumHeight(40);
        ui->btn_y_plus_2->setMinimumHeight(40);
        ui->btn_z_plus->setMinimumHeight(40);
        ui->btn_a_plus->setMinimumHeight(40);
        ui->btn_b_plus->setMinimumHeight(40);
        ui->btn_c_plus->setMinimumHeight(40);
        ui->btn_x_decrease->setMinimumHeight(40);
        ui->btn_y_decrease->setMinimumHeight(40);
        ui->btn_y_decrease_2->setMinimumHeight(40);
        ui->btn_z_decrease->setMinimumHeight(40);
        ui->btn_a_decrease->setMinimumHeight(40);
        ui->btn_b_decrease->setMinimumHeight(40);
        ui->btn_c_decrease->setMinimumHeight(40);
//        ui->btn_velswitch_1->setMinimumHeight(40);
//        ui->btn_velswitch_2->setMinimumHeight(40);
        ui->btn_cen->setMinimumHeight(40);
        //ui->btn_pick_x->setMinimumHeight(40);
        ui->btn_continue->setMinimumHeight(30);
        ui->btn_handle->setMinimumHeight(30);

        ui->show_homex_pic->setMaximumHeight(16);
        ui->show_homex_pic->setMaximumWidth(16);
        ui->show_homey_pic->setMaximumHeight(16);
        ui->show_homey_pic->setMaximumWidth(16);
        ui->show_homey_pic_2->setMaximumSize(16,16);
        ui->show_homez1_pic->setMaximumHeight(16);
        ui->show_homez1_pic->setMaximumWidth(16);
        ui->show_homea_pic->setMaximumHeight(16);
        ui->show_homea_pic->setMaximumWidth(16);
        ui->show_homea_pic->setMaximumHeight(16);
        ui->show_homea_pic->setMaximumWidth(16);
        ui->show_homeb_pic->setMaximumHeight(16);
        ui->show_homeb_pic->setMaximumWidth(16);
        ui->show_homec_pic->setMaximumHeight(16);
        ui->show_homec_pic->setMaximumWidth(16);

    }

    QMainWindow::resizeEvent(ev);

}

void linuxcnc::globalNumkeyPressEvent(QKeyEvent *) {

    return;
}

void linuxcnc::globalNumkeyReleaseEvent(QKeyEvent *key) {

}

void linuxcnc::globalkeyReleaseEvent(QKeyEvent *e) {
    if(get_changing_z())
        return;
    if(e->isAutoRepeat()){

    }
    else {
        if(e->key() == Qt::Key_F1) {
            keyboardcontrolIO1 = 0;
        }
        if(e->key() == Qt::Key_F2) {
            keyboardcontrolIO2 = 0;
        }
        if(e->key() == Qt::Key_F3) {
            keyboardcontrolIO3 = 0;
        }
        if(e->key() == Qt::Key_F4) {
            keyboardcontrolIO4 = 0;
        }
    }


    if(e->isAutoRepeat()) return;
    jog_count--;
    if(get_pro_running())
        return;
    //qDebug("jog_count = %d",jog_count);
    if(btn_jog_count) return;


    if((e->key() == Qt::Key_Up && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_8)) {
        if(e->isAutoRepeat()) return;
        ui->btn_y_plus->setDown(false);

        sendJogStop(1);
    }

    if((e->key() == Qt::Key_Down && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_2)) {
        if(e->isAutoRepeat()) return;
        ui->btn_y_decrease->setDown(false);

        sendJogStop(1);
    }

    if((e->key() == Qt::Key_Left && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_4)) {
        if(e->isAutoRepeat()) return;
        ui->btn_x_decrease->setDown(false);

        sendJogStop(0);
    }

    if((e->key() == Qt::Key_Right && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_6)) {
        if(e->isAutoRepeat()) return;
        ui->btn_x_plus->setDown(false);

        sendJogStop(0);
    }


    if((e->key() == Qt::Key_PageUp && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_9)) {
        if(e->isAutoRepeat()) return;
        ui->btn_a_plus->setDown(false);
        ui->btn_b_plus->setDown(false);
        ui->btn_c_plus->setDown(false);
        ui->btn_z_plus->setDown(false);
        sendJogStop(-1);
        now_axis = 0;
    }


    if((e->key() == Qt::Key_PageDown && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_1)) {

        if(e->isAutoRepeat()) return;
        ui->btn_a_decrease->setDown(false);
        ui->btn_b_decrease->setDown(false);
        ui->btn_c_decrease->setDown(false);
        ui->btn_z_decrease->setDown(false);
        sendJogStop(-1);
        now_axis = 0;
    }

    if((e->key() == Qt::Key_Home && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_7)) {
        if(e->isAutoRepeat()) return;
        ui->btn_a_plus->setDown(false);
            sendJogStop(3);
        now_axis = 0;
    }

    if((e->key() == Qt::Key_End && keychoice == 0) || (keychoice == 1 && e->key() == Qt::Key_3)) {
        if(e->isAutoRepeat()) return;
        ui->btn_a_decrease->setDown(false);
            sendJogStop(3);
        now_axis = 0;
    }

}

void linuxcnc::get_goble_key(uint first, uint second)
{
    if(first == 0 && second == Qt::Key_6){
        ShortCut *obj = new ShortCut(ui->btn_x_plus);
        obj->send_pressed(ui->btn_x_plus);
    }
    if(first == 1 && second == Qt::Key_6){
        ShortCut *obj = new ShortCut(ui->btn_x_plus);
        obj->send_release(ui->btn_x_plus);
    }
    if(first == 0 && second == Qt::Key_4){
        ShortCut *obj = new ShortCut(ui->btn_x_decrease);
        obj->send_pressed(ui->btn_x_decrease);
    }
    if(first == 1 && second == Qt::Key_4){
        ShortCut *obj = new ShortCut(ui->btn_x_decrease);
        obj->send_release(ui->btn_x_decrease);
    }
    if(first == 0 && second == Qt::Key_8){
        ShortCut *obj = new ShortCut(ui->btn_y_plus);
        obj->send_pressed(ui->btn_y_plus);
    }
    if(first == 1 && second == Qt::Key_8){
        ShortCut *obj = new ShortCut(ui->btn_y_plus);
        obj->send_release(ui->btn_y_plus);
    }
    if(first == 0 && second == Qt::Key_2){
        ShortCut *obj = new ShortCut(ui->btn_y_decrease);
        obj->send_pressed(ui->btn_y_decrease);
    }
    if(first == 1 && second == Qt::Key_2){
        ShortCut *obj = new ShortCut(ui->btn_y_decrease);
        obj->send_release(ui->btn_y_decrease);
    }
    if(first == 0 && second == Qt::Key_9){

    }
    if(first == 1 && second == Qt::Key_9){

    }
    if(first == 0 && second == Qt::Key_1){

    }
    if(first == 1 && second == Qt::Key_1){

    }

}

ShortCut::ShortCut(QPushButton *btn)
{
    connect(this,SIGNAL(btn_pressed()),btn,SIGNAL(pressed()));
    connect(this,SIGNAL(btn_released()),btn,SIGNAL(released()));
}

void ShortCut::send_pressed(QPushButton *btn)
{
    if(btn->isEnabled()){
        btn->setDown(true);
        emit btn_pressed();
    }
}

void ShortCut::send_release(QPushButton *btn)
{
    if(btn->isEnabled()){
        btn->setDown(false);
        emit btn_released();
    }
}



void linuxcnc::on_actionEdit_Tools_triggered()
{

    if(tooltabledlg == NULL)
        tooltabledlg = new Tbl_Dialog(this);

    jogspeed = defspeed;
    ui->btn_cen->setChecked(false);
    ui->btn_cen->setText(tr("Default Feed"));


    QString nn = _ini->readEntry("PAR_EMCIO_TOOL_TABLE/VALUE","tool.tbl");
    QFileInfo info(nn);
    QString final = info.absoluteDir().absolutePath();
    final = QString("%1/%2").arg(final).arg(nn);


    bak_cfg_files(_tbl_filepath,_tbl_filebak);
    tooltabledlg->setTblFile(final);
    tooltabledlg->loadTool();
    tooltabledlg->getEmcStatues();
    tooltabledlg->exec();
}


void linuxcnc::turnToArrayTab()
{
//    ui->mode_tab->setCurrentIndex(2);
}
void linuxcnc::turnToManualTab()
{
    ui->mode_tab->setCurrentIndex(0);
}
void linuxcnc::turnToMdiTab()
{
    ui->mode_tab->setCurrentIndex(2);
}
void linuxcnc::turnToProbeTab()
{
    ui->mode_tab->setCurrentIndex(3);
}
void linuxcnc::switchMainTab(int i)
{
    ui->main_tab->setCurrentIndex(i);

}

void linuxcnc::on_main_tab_currentChanged(int index)
{
    if(index==4){
        //ui->showframe2->setHidden(true);
//        ui->widget_status->setHidden(true);
    }else{
        ui->showframe2->setHidden(false);
//        ui->widget_status->setHidden(false);
    }
}


void linuxcnc::firstProbe()
{

    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString firstprobefile = _subDir;
    firstprobefile.append("firstProbe.ngc");
    if(0 !=OpenNgcFile(firstprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start FirstProbe"));
}

void linuxcnc::secondProbe()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString secondprobefile = _subDir;
    secondprobefile.append("secondProbe.ngc");
    if(0 !=OpenNgcFile(secondprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start SecondProbe"));
}

void linuxcnc::on_btn_floatingprobe_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString floatingprobefile = _subDir;
    floatingprobefile.append("floatingProbe.ngc");
    if(0 !=OpenNgcFile(floatingprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start floatingProbe"));
}

void linuxcnc::on_actionHistory_record_triggered()
{
    QString temp = INFO_FILE;
    showHistoryrecord->reOpenFile(temp,temp,false);
    showHistoryrecord->show();
}

void linuxcnc::on_actionShortCut_triggered()
{
    QString temp = KEY_FILE;
    showShortcut->reOpenFile(temp,temp,false);
    showShortcut->show();
}

bool linuxcnc::isArrayfile(QString path)
{
    qDebug()<<path<<path.contains(QRegExp(tr("(array)")));
    if(path.contains(QRegExp(tr("(array)"))))
        return true;
    else
        return false;
}


void linuxcnc::arraySlotOpen()
{
    QString file_temp = "";
    //file_temp = QFileDialog::getOpenFileName(this,
    //           tr("Open File"),_ncdir,tr("Files(*.*)"));
    file_temp = getfileName(tr("Open File"),_ncdir,tr("Files(*.*)"),this);
    if(file_temp == NULL)
        return;
    if(file_temp.isEmpty())
        return;

    ui->arrayPath->setText(file_temp);
}

void linuxcnc::setArraymode(bool mode)
{
    if(!mode) {
        arrayBuildmode = 1;
    } else {
        arrayBuildmode = 0;
    }
}

void linuxcnc::arraysimpleclick() {
    setArraymode(0);
}
void linuxcnc::arraycomplexclick() {
    setArraymode(1);
}

void linuxcnc::arraySlotBuild()
{
    int m99flag = 0;
    int toolnumber=0;
    int m5flag = 0;
    int finishflag = 0;

    QStringList text;
    text.clear();
    QFile fileget(ui->arrayPath->text());
    if(!fileget.open(QIODevice::ReadOnly)) {
        qDebug()<<"array.ngc cannot open";
        return;
    }
    QString textline = "";

    if(useArraydistance == 0) {
        if(ui->arrayRows->value()*ui->arrayColumns->value() > 6 || ui->arrayRows->value()*ui->arrayColumns->value() < 1) {
            show_msg(1,"阵列数目设置错误,阵列数不能大于6");
            return;
        }
    }

    QString finishtext = "";
    QString temptext = "";
    while(!fileget.atEnd())
    {
        textline = fileget.readLine();

        if(finishflag == 0 && !textline.contains(QRegExp("/")) &&
                (textline.contains(QRegExp("[Mm]30")) ||
//                textline.contains(QRegExp("[Mm]2")) ||
                textline.contains(QRegExp("[Mm]99")) ||
                 textline.contains(QRegExp("[Mm]5")) ||
                 textline.contains(QRegExp("[Gg]28")) ||
                 textline.contains(QRegExp("[Gg]30"))
                 )) {
            if(textline.contains(QRegExp("[Mm]99")))
                m99flag = 1;

            if(textline.contains(QRegExp("[Mm]5")))
                m5flag = 1;
            break;
        }

        if(!textline.contains(QRegExp("/")) && textline.contains("O600 if")) {
            finishflag = 1;
        }

        if(!textline.contains(QRegExp("/")) &&
                ((textline.contains(QRegExp("[Mm]6"))) ||(textline.contains(QRegExp("[Mm]06"))))  &&
                par_get_var(4126) > 0) {
            if(toolnumber > 0) {
                text.append(temptext);
                temptext = "";
            }
            toolnumber++;
        }
        if(finishflag == 1)
            finishtext.append(textline);
        else
            temptext.append(textline);

        if(!textline.contains(QRegExp("/")) && textline.contains("O600 endif")) {
            finishflag = 0;
        }
    }
    fileget.close();

    if(0 == toolnumber) {
        text.append(temptext);
        toolnumber = 1;
    } else {
        text.append(temptext);
        toolnumber = text.size();
    }

    QString getfile = ui->arrayPath->text();
    QString arrayfile = getfile;
    arrayfile.append(tr("(array)%1X%2").arg(ui->arrayColumns->value())
                     .arg(ui->arrayRows->value()));

    QString no;
    QFile file(arrayfile);
    if(file.exists())
        get_system(QString("rm %1").arg(arrayfile),&no);

    if(!file.open(QIODevice::ReadWrite)) {
        qDebug()<<"array.ngc cannot open";
        return;
    }



    QTextStream arraytext(&file);
    for(int ii=0;ii<toolnumber;ii++) {

    arraytext<<"#<ROWS> = "<<ui->arrayRows->value()<<"\n"
            <<"#<COLUMNS> = "<<ui->arrayColumns->value()<<"\n";
    arraytext<<"#<DISTANCEX> = "<<-(ui->arrayXdistance->value())<<"\n"
            <<"#<DISTANCEY> = "<<-(ui->arrayYdistqance->value())<<"\n";
    arraytext<<"#<XCOUNT> = 0\n#<YCOUNT> = 0\n#<COUNTS> = 0\n\n";
    arraytext<<"#4041=0\n#4042=0\n#4043=0\n#4044=0\n";

    if(1 == arrayBuildmode) {
        for(int i=0;i<ui->arrayRows->value();i++) {
            for(int j=0;j<ui->arrayColumns->value();j++) {
                arraytext<<"#4041=-[#<XCOUNT>*#<DISTANCEX>]\n#4042=-[#<YCOUNT>*#<DISTANCEY>]\n";
                arraytext<<"#4043=#<COUNTS>\n#4044=#<COUNTS>\n\n";
                if(par_get_var(4069) == 0) {
                    arraytext<<"g92x[#<DISTANCEX>*#<XCOUNT>+#5420]y[#<DISTANCEY>*#<YCOUNT>+#5421]\n";
                } else {
                    arraytext<<"O500 if [ [#[#<COUNTS>*5+2104]] EQ 0]\n";
                    arraytext<<"g10l2p[#5220]x[#[#<COUNTS>*5+2100]]y[#[#<COUNTS>*5+2101]]\n";
                    arraytext<<"g68r[#[#<COUNTS>*5+2103]]\n";
                }
                if(par_get_var(4069) == 0) {
                    arraytext<<text.at(ii);
                } else {
                    arraytext<<text.at(ii);
                    arraytext<<"O500 endif\n";
                }
                arraytext<<"\nG92.1\n#<COUNTS> = [#<COUNTS>+1]\n#<XCOUNT> = [#<XCOUNT>+1]\ng53g90g0z0\n";
            }
            arraytext<<"g92.1\n#<XCOUNT> = 0\n#<YCOUNT> = [#<YCOUNT>+1]\n";
        }
    } else {
        arraytext<<"O"<<100+2*ii<<" while [#<YCOUNT> LT #<ROWS>]\n";
        arraytext<<"O"<<100+2*ii+1<<" while [#<XCOUNT> LT #<COLUMNS>]\n";
        arraytext<<"#4041=-[#<XCOUNT>*#<DISTANCEX>]\n#4042=-[#<YCOUNT>*#<DISTANCEY>]\n";
        arraytext<<"#4043=#<COUNTS>\n#4044=#<COUNTS>\n\n";
        if(useArraydistance == 1) {
            arraytext<<"g92x[#<DISTANCEX>*#<XCOUNT>+#5420]y[#<DISTANCEY>*#<YCOUNT>+#5421]\n";
        } else {
            arraytext<<QString("G%1").arg("[54+#<COUNTS>]")<<"\n";
        }
            arraytext<<text.at(ii);

        arraytext<<"\nG92.1\n#<COUNTS> = [#<COUNTS>+1]\n#<XCOUNT> = [#<XCOUNT>+1]\n";
        arraytext<<"O"<<100+2*ii+1<<" endwhile\ng92.1\n#<XCOUNT> = 0\n#<YCOUNT> = [#<YCOUNT>+1]\n";
        arraytext<<"O"<<100+2*ii<<" endwhile\ng92.1\n\n";
    }
    }
    arraytext<<"#4041=0\n#4042=0\n#4043=0\n#4044=0\n";


    if(1 == m99flag)
        arraytext<<"m99\n\n";

    if(1 == m5flag)
        arraytext << "M5 \n";

    arraytext << finishtext<<"\n";

    file.close();

    int ret = QMessageBox::information(this,tr("tips"),tr("确定把加载阵列文件:%1 ?").arg(arrayfile),QMessageBox::Ok,QMessageBox::Cancel);
    if(ret == QMessageBox::Ok) {
        pro_open(arrayfile);
    }
}

void linuxcnc::switchjogspeed()
{
    if(jogspeed == fastspeed) {
        jogspeed = defspeed;
        ui->btn_cen->setChecked(false);
        ui->btn_cen->setText(tr("Default Feed"));
    } else if(jogspeed == defspeed) {
        jogspeed = fastspeed;
        ui->btn_cen->setChecked(true);
        ui->btn_cen->setText(tr("High Feed"));
    }
}

void linuxcnc::setprobedebug(bool flag) {
    ui->btn_first_probe->setHidden(!flag);
}

void linuxcnc::on_actionLimitRelease_triggered()
{
    sendOverrideLimits(0);
    sendOverrideLimits(1);
    sendOverrideLimits(2);
    sendOverrideLimits(3);
    sendOverrideLimits(4);
    sendOverrideLimits(5);
    sendOverrideLimits(6);
    show_msg(0,tr("Release the overlimit"));
    sendMachineOn();
}

void linuxcnc::on_actionRegister_triggered()
{

    if(decrypydlg == NULL) {
        decrypydlg = new Decrypt_Dialog(this);
        connect(decrypydlg,SIGNAL(send_msg(int,QString)),this,SLOT(show_msg(int,QString)));
    }
    decrypydlg->init();
    decrypydlg->exec();
}

void linuxcnc::on_actionSystem_Info_triggered()
{
    if(systeminfo == NULL)
        systeminfo = new Infodialog(this);

    systeminfo->exec();
}

void linuxcnc::on_actionUpdate_triggered()
{


    QString updatefilepath;
    QDir dir("/home/.tmp/");
    int rett;
    if(!dir.exists())
        rett = system("mkdir /home/.tmp/");


    QString desktop = "/home/cnext/Desktop";
    QDir Desktop(desktop);
    if(!Desktop.exists())
        desktop = tr("/home/cnext/Desktop");

    //updatefilepath = QFileDialog::getOpenFileName(this,
     //    tr("Open Update File"), desktop, tr("Update Files (*.cnc)"));

    updatefilepath = getfileName(tr("Open Update File"),desktop,tr("Update Files (*.cnc)"),this);

    if(updatefilepath.isEmpty())
        return;

    int ret = QMessageBox::warning(this,tr("system update"),
                       tr("This operation will shutdown the application,\nDo you want to update ?"),
                        tr("Ok"),tr("Cancel"));
     switch (ret) {
        case Cancel:
            return;
            break;
        default:
            // should never be reached
            break;
      }

    //QFile rfile(updatefilepath);


    QDir dirt("/home/.tmp/updater");
    if(!dirt.exists())
        ret = system("mkdir /home/.tmp/updater");
    if(ret == -1) {
        QMessageBox::warning(this,tr("warning"),
               tr("Some error occurred,Error code -101"),tr("Ok"));
    }

    ret = system("rm -rf /home/.tmp/updater/*");
    if(ret == -1) {
        QMessageBox::warning(this,tr("warning"),
               tr("Some error occurred,Error code -102"),tr("Ok"));
    }


    QString cmd;
    cmd = QString("unzip -P CNEXT2018 '%1' -d /home/.tmp/updater").arg(updatefilepath);
    ret = system(cmd.toLocal8Bit().data());
    if(ret == -1) {
        QMessageBox::warning(this,tr("warning"),
               tr("Some error occurred,Error code -103"),tr("Ok"));
    }


    cmd = QString("tar -xmf /home/.tmp/updater/ALL.tar -C /home/.tmp/updater");
    ret = system(cmd.toLocal8Bit().data());
    if(ret == -1) {
        QMessageBox::warning(this,tr("warning"),
               tr("Some error occurred,Error code -104"),tr("Ok"));
    }

    QFile file_test("/home/.tmp/updater/cnc-install/pkg_version_v3");
    if( !file_test.exists() ) {
        QMessageBox::warning(this,tr("warning"),
               tr("Package version match fail,Please check the version"),tr("OK"));
        rett = system("rm /home/.tmp/updater/* -rf");
        return;
    }

    rett = system("chmod 4755 /home/.tmp/updater/cnc-install/update.sh");
    rett = system("chmod 4755 /home/.tmp/updater/cnc-install/updater");

    rett = system("sudo /home/.tmp/updater/cnc-install/updater &");


}

void linuxcnc::on_actionBackUpRestore_triggered()
{
    int aux = 0;
/*    if(backup_t == NULL) {
        backup_t = new QThread();
        aux = 1;
    }*/
    if(backuper == NULL) {
        backuper = new BackupRestore();
        aux = 1;
    }

        backuper->BackupandRestore();
//        connect(backup_t,SIGNAL(started()),backuper,SLOT(BackupandRestore()));
//        connect(backuper,SIGNAL(finished(int)),backup_t,SLOT(quit()));
//    backuper->moveToThread(backup_t);
//    backup_t->start();
}


void linuxcnc::on_actionBuildInstaller_triggered()
{
    int aux = 0;
    if(install == NULL) {
        install = new QThread();
        aux = 1;
    }
    if(installer == NULL) {
        installer = new buildInstaller();
        aux = 1;
    }
    if(1 == aux) {
        connect(install,SIGNAL(started()),installer,SLOT(buildinstall()));
    }
    QMessageBox msgbox(this);
    msgbox.setStandardButtons(QMessageBox::NoButton);
    msgbox.setWindowTitle(tr("build installer"));
    msgbox.setText(tr("now build the installer..."));
    installer->moveToThread(install);

    //connect(installer,SIGNAL(finished(int)),install,SLOT(quit()));
    connect(installer,SIGNAL(finished(int)),&msgbox,SLOT(done(int)));
    install->start();
    msgbox.exec();
    install->quit();
    QMessageBox::information(this,tr("tips"),
                 tr("now the new package is on Desktop"));
}

void buildInstaller::buildinstall()
{
    QDir dir("/home/.tmp/updater/cnc-install/");
    if(!dir.exists())
        dir.mkpath("/home/.tmp/updater/cnc-install/");
    QString str;
    get_system("cp /home/.cnc/bin/getInstall.sh /home/.tmp/updater/cnc-install/updater",&str);
    get_system("sudo /home/.tmp/updater/cnc-install/updater",&str);

    emit(finished(1));
}

void BackupRestore::BackupandRestore()
{
    system("/home/.cnc/gui/backup /home/.package/.backup &");
   // emit(finished(1));
}


void linuxcnc::on_actionNgcInfo_triggered()
{
    show_proinfo();
}

void linuxcnc::on_actionWaterOut_triggered() //janmi 2015/8/15
{
    if(0==emcStatus->motion.synch_do[1]){
        show_msg(0,tr("WaterOut On"));
        sendSetDIO(1,1);
    }else{
        show_msg(0,tr("WaterOut Off"));
        sendSetDIO(1,0);
    }
}

void linuxcnc::on_actionLight_triggered()  //janmi 2015/8/15
{
    if(0==emcStatus->motion.synch_do[2]){
        show_msg(0,tr("Light On"));
        sendSetDIO(2,1);
    }else{
        show_msg(0,tr("Light Off"));
        sendSetDIO(2,0);
    }
}



void linuxcnc::on_actionBackUpMasterCam_triggered()  //janmi 2515-9-5
{

    if(!PassWord::getCodecnc(this,1))
        return;

    if(System_set == NULL)
        System_set = new System_setup(this);
    System_set->exec();


}



void linuxcnc::on_btn_gl_pro_clicked()
{

    if(gl_pro == NULL)
        gl_pro = new gl_property(this);


    gl_pro->exec();

    if(gl_pro->setcolor_flag==1){
        QString name;
        QColor cl;
        QString value;
        float c[4];

        //实时轨迹
        cl = gl_pro->reRealTime;
        name = "OpenGL/ColorRealTimePath";
        value.sprintf("%f|%f|%f|%f",cl.redF(),cl.greenF(),cl.blueF(),cl.alphaF());
        _prefer->writeEntry(name,value);
        c[0] = cl.redF();
        c[1] = cl.greenF();
        c[2] = cl.blueF();
        c[3] = cl.alphaF();
        viewwidget->setColors("ColorRealTimePath",c);

        //G0轨迹
        name = "OpenGL/ColorTraverse";
        cl = gl_pro->reTraverse;
        value.sprintf("%f|%f|%f|%f",cl.redF(),cl.greenF(),cl.blueF(),cl.alphaF());
        _prefer->writeEntry(name,value);
        c[0] = cl.redF();
        c[1] = cl.greenF();
        c[2] = cl.blueF();
        c[3] = cl.alphaF();
        viewwidget->setColors("ColorTraverse",c);


        //G1轨迹
        cl = gl_pro->reFeed;
        name = "OpenGL/ColorFeed";
        value.sprintf("%f|%f|%f|%f",cl.redF(),cl.greenF(),cl.blueF(),cl.alphaF());
        _prefer->writeEntry(name,value);
        c[0] = cl.redF();
        c[1] = cl.greenF();
        c[2] = cl.blueF();
        c[3] = cl.alphaF();
        viewwidget->setColors("ColorFeed",c);


        //G2_G3轨迹
        cl = gl_pro->reArcFeed;
        name = "OpenGL/ColorArcFeed";
        value.sprintf("%f|%f|%f|%f",cl.redF(),cl.greenF(),cl.blueF(),cl.alphaF());
        _prefer->writeEntry(name,value);
        c[0] = cl.redF();
        c[1] = cl.greenF();
        c[2] = cl.blueF();
        c[3] = cl.alphaF();
        viewwidget->setColors("ColorArcFeed",c);


        //工件原点
        name = "OpenGL/ColorOrigin";
        cl = gl_pro->reOrigin;
        value.sprintf("%f|%f|%f|%f",cl.redF(),cl.greenF(),cl.blueF(),cl.alphaF());
        _prefer->writeEntry(name,value);
        c[0] = cl.redF();
        c[1] = cl.greenF();
        c[2] = cl.blueF();
        c[3] = cl.alphaF();
        viewwidget->setColors("ColorOrigin",c);

        //背景色
        name = "OpenGL/ColorBackGround";
        cl = gl_pro->reBackGround;
        value.sprintf("%f|%f|%f|%f",cl.redF(),cl.greenF(),cl.blueF(),cl.alphaF());
        _prefer->writeEntry(name,value);
        c[0] = cl.redF();
        c[1] = cl.greenF();
        c[2] = cl.blueF();
        c[3] = cl.alphaF();
        viewwidget->setColors("ColorBackGround",c);

        //被选轨迹颜色
        name = "OpenGL/ColorSelected";
        cl = gl_pro->reSelected;
        value.sprintf("%f|%f|%f|%f",cl.redF(),cl.greenF(),cl.blueF(),cl.alphaF());
        _prefer->writeEntry(name,value);
        c[0] = cl.redF();
        c[1] = cl.greenF();
        c[2] = cl.blueF();
        c[3] = cl.alphaF();
        viewwidget->setColors("ColorSelected",c);

        //限位颜色
        name = "OpenGL/ColorLimit";
        cl = gl_pro->reLimit;
        value.sprintf("%f|%f|%f|%f",cl.redF(),cl.greenF(),cl.blueF(),cl.alphaF());
        _prefer->writeEntry(name,value);
        c[0] = cl.redF();
        c[1] = cl.greenF();
        c[2] = cl.blueF();
        c[3] = cl.alphaF();
        viewwidget->setColors("ColorLimit",c);


        viewwidget->update();
    }else{
        return;
    }


}



void linuxcnc::on_actionPickZ_triggered()
{
    if(pickz == NULL ) {
        pickz = new PickZ(this);
    }
    pickz->exec();

    needparflush = 1;
}



void linuxcnc::on_lockfirstprobe_toggled(bool checked)
{
    if(checked) {
        _prefer->writeEntry("LOCKPROBE/VALUE",1);
    } else {
        _prefer->writeEntry("LOCKPROBE/VALUE",0);
    }
}

void linuxcnc::on_btn_first_probe_Z3_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString firstprobefile = _subDir;
    firstprobefile.append("Z3firstProbe.ngc");
    if(0 !=OpenNgcFile(firstprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start Z3FirstProbe"));
}

void linuxcnc::on_btn_second_probe_Z3_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString secondprobefile = _subDir;
    secondprobefile.append("Z3secondProbe.ngc");
    if(0 !=OpenNgcFile(secondprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start Z3SecondProbe"));
}

void linuxcnc::on_actionSecondProbe_triggered()
{
    secondProbe();
}

void linuxcnc::on_z1firstprobe_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString firstprobefile = _subDir;
    firstprobefile.append("Z1firstProbe.ngc");
    if(0 !=OpenNgcFile(firstprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start Z1firstProbe"));
}

void linuxcnc::on_z2firstprobe_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString firstprobefile = _subDir;
    firstprobefile.append("Z2firstProbe.ngc");
    if(0 !=OpenNgcFile(firstprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start Z2firstProbe"));
}

void linuxcnc::on_z3firstprobe_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString firstprobefile = _subDir;
    firstprobefile.append("Z3firstProbe.ngc");
    if(0 !=OpenNgcFile(firstprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start Z3firstProbe"));
}

void linuxcnc::on_z4firstprobe_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString firstprobefile = _subDir;
    firstprobefile.append("Z4firstProbe.ngc");
    if(0 !=OpenNgcFile(firstprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start Z4firstProbe"));
}

void linuxcnc::on_z1secondprobe_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString secondprobefile = _subDir;
    secondprobefile.append("Z1secondProbe.ngc");
    if(0 !=OpenNgcFile(secondprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start Z1SecondProbe"));
}

void linuxcnc::on_z2secondprobe_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString secondprobefile = _subDir;
    secondprobefile.append("Z2secondProbe.ngc");
    if(0 !=OpenNgcFile(secondprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Start Z2SecondProbe"));
}

void linuxcnc::on_z3secondprobe_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString secondprobefile = _subDir;
    secondprobefile.append("Z3secondProbe.ngc");
    if(0 !=OpenNgcFile(secondprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Z3Start SecondProbe"));
}

void linuxcnc::on_z4secondprobe_clicked()
{
    if(0 != check_zdiff()){
        show_msg(2,tr("Please Check Zdiff"));
        return;
    }

    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    QString secondprobefile = _subDir;
    secondprobefile.append("Z4secondProbe.ngc");
    if(0 !=OpenNgcFile(secondprobefile.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);
    show_msg(0,tr("Z4Start SecondProbe"));
}

void linuxcnc::customizationUi() {

//       ui->label_c->setHidden(true);
//       ui->show_homec_pic->setHidden(true);
//       ui->show_abs_c->setHidden(true);
//       show_offset_c->setHidden(true);
//       ui->btn_c_plus->setHidden(true);
//       ui->btn_c_decrease->setHidden(true);

//       ui->label_b->setHidden(true);
//       ui->show_homeb_pic->setHidden(true);
//       ui->show_abs_b->setHidden(true);
//       show_offset_b->setHidden(true);
//       ui->btn_b_plus->setHidden(true);
//       ui->btn_b_decrease->setHidden(true);

}

void linuxcnc::setapp(QApplication *App) {

//    app = App;

//    //#######################################CAD_Widget#############################

//    QString file_so ="/home/.cam/libcxcad.so";
//    void* handle = dlopen(file_so.toStdString().c_str(), RTLD_NOW|RTLD_LOCAL|RTLD_DEEPBIND);

//    if ( handle ) {

//        typedef QMainWindow* (*cadMain_t)(QApplication &app,QObject **b,int argc, char** argv);
//        dlerror();
//        cadMain_t cadMain = (cadMain_t) dlsym(handle, "cadMain");
//        const char *dlsym_error = dlerror();
//        if (dlsym_error) {
//            qDebug()<<" Can not open dynamic library libcxcad.so \n"<<dlsym_error;
//            dlclose(handle);
//        } else {

//            static char *ps[6]={0,0,0,0,0,0};
//            ps[0]="/home/.cam/cxcad";
//            ps[2]="";
//            ps[3]="";
//            ps[4]="";
//            ps[5]="";

//            ps[1]="4";

//            QObject *bridge = NULL;
//            cadM = cadMain(*App,&bridge,1,ps);
//            //cadM->menuWidget()->setVisible(false);
//            cadM->setParent(ui->cam_widget);

//            updateCamWidget();

//            connect(bridge,SIGNAL(reloadNcProgram(QString)),this,SLOT(openCamNC(QString)));
//            connect(viewwidget,SIGNAL(signalSetMachineZeroPos(double,double)),bridge,SLOT(slotSetMachineZeroPos(double,double)));

//            connect(bridge,SIGNAL(sendMessageToCNC(int,QString)),this,SLOT(messageFromCam(int,QString)));
//            connect(this,SIGNAL(sendMessageToCAM(int,QString)),bridge,SLOT(messageFromSystem(int,QString)));

//            viewwidget->updateMachineZeroPos();
//            this->update();

//       }

//    } else {

//        ui->cam_widget->hide();
//       const char *dlsym_error = dlerror();
//       if (dlsym_error)
//           qDebug()<<" Can not find dynamic library libcam.so: %s\n"<<dlsym_error;
//       else
//           qDebug()<<" Can not find dynamic\n";

//    }
    //if ( handle )
    //dlclose(handle);

    //电机通讯提示
    if(!SIMULATE) {
       chklink = new touch_Checklink(0);
       QTimer::singleShot(500, chklink, SLOT(exec()));
    }
}


void linuxcnc::on_actionSetZ1_triggered()
{
    on_actionZ1_triggered();

    /*

    int val = 0;
    if((emcStatus->motion.set_which_z & (0x01<<2)) == 0) {
        _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",1);
        val = emcStatus->motion.set_which_z | (0x01<<2);
        if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
            return;
        show_msg(0,tr("Z1 axis is enable"));
        //sendDoubleZcmd(0,val,&offset[0]);

    } else {
        _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",0);
        val = emcStatus->motion.set_which_z & ~(0x01<<2);
        if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
            return;
        show_msg(0,tr("Z1 axis is disable"));

        //sendDoubleZcmd(0,val,&offset[0]);
    }

    if(!get_all_homed(_doublez)) {
        double offset[9] = {0};
        sendDoubleZcmd(0,val,&offset[0]);
        return;
    }

    //usleep(1000*100);
    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(get_handle_guide())
        on_actionHandle_guide_triggered();
    QString goOrigin = _subDir;
    goOrigin.append("goZero.ngc");

    QFile file(goOrigin);
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
     QTextStream out(&file);
     out << "M90P"<< val << "\n";
     file.close();

    if(0 !=OpenNgcFile(goOrigin.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);

    */

}

void linuxcnc::on_actionSetZ2_triggered()
{
    on_actionZ2_triggered();
    /*
    int val = 0;
    if((emcStatus->motion.set_which_z & (0x01<<3)) == 0) {
        _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",1);
        val = emcStatus->motion.set_which_z | (0x01<<3);
        if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
            return;
        show_msg(0,tr("Z2 axis is enable"));
        double offset[9] = {0};
        //sendDoubleZcmd(0,val,&offset[0]);
    } else {
        _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",0);
        val = emcStatus->motion.set_which_z & ~(0x01<<3);
        if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
            return;
        show_msg(0,tr("Z2 axis is disable"));
        double offset[9] = {0};
        //sendDoubleZcmd(0,val,&offset[0]);
    }

    if(!get_all_homed(_doublez)) {
        double offset[9] = {0};
        sendDoubleZcmd(0,val,&offset[0]);
        return;
    }

    //usleep(1000*100);
    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(get_handle_guide())
        on_actionHandle_guide_triggered();
    QString goOrigin = _subDir;
    goOrigin.append("goZero.ngc");
    QFile file(goOrigin);
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
     QTextStream out(&file);
     out << "M90P"<< val << "\n";
     file.close();

    if(0 !=OpenNgcFile(goOrigin.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);

    */
}

void linuxcnc::on_actionSetZ3_triggered()
{
    on_actionZ3_triggered();
    /*
    int val = 0;
    if((emcStatus->motion.set_which_z & (0x01<<4)) == 0) {
        _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",1);
        val = emcStatus->motion.set_which_z | (0x01<<4);
        if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
            return;
        show_msg(0,tr("Z3 axis is enable"));
        double offset[9] = {0};
        //sendDoubleZcmd(0,val,&offset[0]);
    } else {
        _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",0);
        val = emcStatus->motion.set_which_z & ~(0x01<<4);
        if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
            return;
        show_msg(0,tr("Z3 axis is disable"));
        double offset[9] = {0};
        //sendDoubleZcmd(0,val,&offset[0]);
    }

    if(!get_all_homed(_doublez)) {
        double offset[9] = {0};
        sendDoubleZcmd(0,val,&offset[0]);
        return;
    }

    //usleep(1000*100);
    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(get_handle_guide())
        on_actionHandle_guide_triggered();
    QString goOrigin = _subDir;
    goOrigin.append("goZero.ngc");
    QFile file(goOrigin);
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
     QTextStream out(&file);
     out << "M90P"<< val << "\n";
     file.close();

    if(0 !=OpenNgcFile(goOrigin.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);


    */

}

void linuxcnc::on_actionSetZ4_triggered()
{
    on_actionZ4_triggered();

    /*
    int val = 0;
    if((emcStatus->motion.set_which_z & (0x01<<5)) == 0) {
        _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",1);
        val = emcStatus->motion.set_which_z | (0x01<<5);
        if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
            return;
        show_msg(0,tr("Z4 axis is enable"));
        double offset[9] = {0};
        //sendDoubleZcmd(0,val,&offset[0]);
    } else {
        _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",0);
        val = emcStatus->motion.set_which_z & ~(0x01<<5);
        if(0 != ensure_mode(EMC_TASK_MODE_MANUAL))
            return;
        show_msg(0,tr("Z4 axis is disable"));
        double offset[9] = {0};
        //sendDoubleZcmd(0,val,&offset[0]);
    }

    if(!get_all_homed(_doublez)) {
        double offset[9] = {0};
        sendDoubleZcmd(0,val,&offset[0]);
        return;
    }

    //usleep(1000*100);
    if(EMC_TASK_MODE_AUTO == get_task_mode())
        return;
    if(0 != ensure_mode(EMC_TASK_MODE_AUTO)) {
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    if(get_handle_guide())
        on_actionHandle_guide_triggered();
    QString goOrigin = _subDir;
    goOrigin.append("goZero.ngc");
    QFile file(goOrigin);
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;
     QTextStream out(&file);
     out << "M90P"<< val << "\n";
     file.close();

    if(0 !=OpenNgcFile(goOrigin.toLocal8Bit().data())){
        ensure_mode(EMC_TASK_MODE_MANUAL);
        return;
    }
    sendProgramRun(-1,0);

    */

}

int linuxcnc::check_zdiff()
{
    double zOffset_max=0;
    double zOffset_min=0;
    double zOffset[4] = {0};
    int ret = 0;
    int flag = 0;

    zOffset[0] = emcStatus->task.g5x_offset.tran.z;
    zOffset[1] = emcStatus->task.g5x_offset.a;
    zOffset[2] = emcStatus->task.g5x_offset.b;
    zOffset[3] = emcStatus->task.g5x_offset.c;

    for(int i=0;i<4;i++) {
        if((emcStatus->motion.set_which_z & 0x01<<i+2) > 0){
//            show_msg(1,tr("Z is %1oooo %2").arg(i).arg(emcStatus->motion.set_which_z & 0x01<<i+2));
            if(flag  == 0){
                zOffset_min =  zOffset[i];
                zOffset_max =  zOffset[i];
                flag++;
            }

            zOffset_max = zOffset[i]>zOffset_max? zOffset[i]:zOffset_max;
            zOffset_min =  zOffset[i]<zOffset_min? zOffset[i]:zOffset_min;
        }

    }

//    show_msg(1,tr("max is %1,min is %2").arg(zOffset_max).arg(zOffset_min));

    double diff_max = _ini->read("PAR_RS274NGC_MAXZDIFF","VALUE","10").toDouble();
    if((zOffset_max-zOffset_min)> diff_max){
        ret = -1;
    }

    return ret;
}

void linuxcnc::change_z(){
    int val = 0;


    if(1== _ini->read("PAR_AXIS_2_ACTIVE","VALUE","0").toInt())
        val += 4;

    if(1== _ini->read("PAR_AXIS_3_ACTIVE","VALUE","0").toInt())
        val += 8;

    if(1== _ini->read("PAR_AXIS_4_ACTIVE","VALUE","0").toInt())
        val += 16;

    if(_doublez > 5 && 1== _ini->read("PAR_AXIS_5_ACTIVE","VALUE","0").toInt())
        val += 32;

    qDebug()<<"now I change z M90P"<<val<<"  _doublez is "<<_doublez;

    if(0 != ensure_mode(EMC_TASK_MODE_MDI))
        return;

    qDebug()<<"now I change z M90P++++++"<<val<<"  _doublez is "<<_doublez;
    QString cmd = QString("M90P%1").arg(val);
    sendMdiCmd(cmd.toLatin1().data(),1);
}


void linuxcnc::on_toolSeperate_toggled(bool checked)
{
    if(checked) {
        show_msg(0,tr("Set Tool Seperate"));
        sendSetParameter(4126,1);
    } else {
        show_msg(0,tr("Cancel Tool Seperate"));
        sendSetParameter(4126,0);
    }
}

void linuxcnc::on_actionAllZ_triggered()
{
    int val = 28;
    _ini->writeEntry("PAR_AXIS_2_ACTIVE/VALUE",1);
    _ini->writeEntry("PAR_AXIS_3_ACTIVE/VALUE",1);
    _ini->writeEntry("PAR_AXIS_4_ACTIVE/VALUE",1);

    if(_doublez>5){
        val =60;
        _ini->writeEntry("PAR_AXIS_5_ACTIVE/VALUE",1);
    }
    if(0 != ensure_mode(EMC_TASK_MODE_MDI))
        return;
    QString cmd = QString("M90P%1").arg(val);
    sendMdiCmd(cmd.toLatin1().data(),1);


}
void linuxcnc::on_actionMotorPar_triggered()
{
    if(!PassWord::getCodecnc(this,1))
        return;

//    MULTIMOTOR mp(this);
//    mp.exec();
    system("/home/.cnc/bin/rwmotor &");
}
/*****************************界面按钮实现*********************/
void linuxcnc::on_btn_reset_clicked()
{
    this->on_actionReset_triggered();
}

void linuxcnc::on_btn_verify_clicked()
{
    this->on_actionVerify_triggered();
}

void linuxcnc::on_btn_repeat_clicked()
{
    this->on_actionRepeat_triggered();
}

void linuxcnc::on_btn_cool_clicked()
{
    this->set_flood();
}

void linuxcnc::on_btn_spindle_clicked()
{
    this->set_spindle();
}

void linuxcnc::on_btn_handguide_clicked()
{
    this->on_actionHandle_guide_triggered();
}

void linuxcnc::on_btn_lube_clicked()
{
    this->set_lube();
}

void linuxcnc::on_btn_light_clicked()
{
    this->on_actionLight_triggered();
}

void linuxcnc::on_btn_waterout_clicked()
{
    this->on_actionWaterOut_triggered();
}
/*
void linuxcnc::on_btn_z1_clicked()
{
    this->on_actionZ1_triggered();
}

void linuxcnc::on_btn_z2_clicked()
{
    this->on_actionZ2_triggered();
}


void linuxcnc::on_btn_z3_clicked()
{
    this->on_actionZ3_triggered();
}

void linuxcnc::on_btn_z4_clicked()
{
    this->on_actionZ4_triggered();
}
void linuxcnc::on_btn_z1z2_clicked()
{
    this->on_actionZ1_Z2_triggered();
}
*/
void linuxcnc::chk_cfg_files(QString filepath,QString filebak) {
    static int iniediting[5] = {0,0,0,0,0};
    static int inilost[5] = {0,0,0,0,0};
    int fileindex = 0;

    QString filename = "";
    if(filepath.compare(_ini_filepath) == 0) {
        filename = "配置文件1";
        fileindex = 0;
    } else if(filepath.compare(_var_filepath) == 0) {
        filename = "配置文件2";
        fileindex = 1;
    } else if(filepath.compare(_zero_filepath) == 0) {
        filename = "配置文件3";
        fileindex = 2;
    } else if(filepath.compare(_tbl_filepath) == 0) {
        filename = "配置文件4";
        fileindex = 3;
    } else {
        filename = filepath;
        fileindex = 4;
    }
    //INI文件检测
    QFile inifile(filepath);
    if(!inifile.exists()) {
        if(inilost[fileindex] > 20) {
            inilost[fileindex] = 0;
            this->show_msg(1,QString("%1检测异常 -1").arg(filename));
            QFile bakinifile(filebak);
            if(bakinifile.exists()) {
                if(QFile::copy(filebak,filepath)) {
                    this->show_msg(1,QString("%1修复成功").arg(filename));
                    if(fileindex == 3) {
                            sendLoadToolTable(_tbl_filepath.toUtf8().data());
                    } else if(fileindex == 2){

                    } else {
                        needparflush = 1;
                    }
                } else {
                    this->show_msg(1,QString("%1修复失败,请重新更新软件").arg(filename));
                }
            }
        }
        inilost[fileindex]++;
    } else {
         inilost[fileindex] = 0;
        if(inifile.size() < 1) {
            if(iniediting[fileindex] > 20) {
                iniediting[fileindex] = 0;
                this->show_msg(1,QString("%1检测异常 -2").arg(filename));
                QFile bakinifile(filebak);
                if(bakinifile.exists()) {
                    QFile::remove(filepath);
                    if(QFile::copy(filebak,filepath)) {
                        this->show_msg(1,QString("%1修复成功").arg(filename));

                        if(fileindex == 3) {
                                sendLoadToolTable(_tbl_filepath.toUtf8().data());
                        } else if(fileindex == 2){

                        } else {
                            needparflush = 1;
                        }
                    } else {
                        this->show_msg(1,QString("%1修复失败,请重新更新软件").arg(filename));
                    }
                }
            }
            iniediting[fileindex]++;
        } else {
            iniediting[fileindex] = 0;
        }
    }
}

void linuxcnc::bak_cfg_files(QString filepath,QString bkfile) {
    QString filename = "";
    if(filepath.compare(_ini_filepath) == 0) {
        filename = "配置文件1";
    } else if(filepath.compare(_var_filepath) == 0) {
        filename = "配置文件2";
    } else if(filepath.compare(_zero_filepath) == 0) {
        filename = "配置文件3";
    } else if(filepath.compare(_tbl_filepath) == 0) {
        filename = "配置文件4";
    } else {
        filename = filepath;
    }

    QFile inifile(filepath);
    if(inifile.exists()) {
        if(inifile.size() > 1) {
            QFile::remove(bkfile);
            if(QFile::copy(filepath,bkfile)) {
                this->show_msg(0,QString("%1备份成功").arg(filename));
            } else {
                this->show_msg(0,QString("%1备份失败").arg(filename));
            }
        }
    }
}
//valve a\b\c
void linuxcnc::on_btn_cyclinder_a1_released()
{


    int id = 21;
    int val = emcStatus->motion.synch_do[id] == 0 ? 1:0;
    sendSetDIO(id,val);
    qDebug()<<"sendSetDIO"<<id<<val;
}

void linuxcnc::on_btn_cyclinder_a2_released()
{
    int id = 22;
    int val = emcStatus->motion.synch_do[id] == 0 ?  1:0;
    sendSetDIO(id,val);
}


void linuxcnc::on_btn_cyclinder_b1_released()
{
    int id = 23;
    int val = emcStatus->motion.synch_do[id] == 0 ?  1:0;
    sendSetDIO(id,val);
}

void linuxcnc::on_btn_cyclinder_b2_released()
{
    int id = 24;
    int val = emcStatus->motion.synch_do[id] == 0 ?  1:0;
    sendSetDIO(id,val);
}


void linuxcnc::on_btn_ValveE_clicked()
{

        if(0==emcStatus->motion.synch_do[13]){
            show_msg(0,tr("trun on valve E"));
            sendSetDIO(13,1);
        }else{
            show_msg(0,tr("trun off valve E"));
            sendSetDIO(13,0);
        }

}
void linuxcnc::on_btn_ValveF_clicked()
{
        if(0==emcStatus->motion.synch_do[14]){
            show_msg(0,tr("打开护罩"));
            sendSetDIO(14,1);
        }else{
            show_msg(0,tr("关闭护罩"));
            sendSetDIO(14,0);
        }

}

void linuxcnc::on_btn_GO_Origin_clicked()
{
    on_actionGO_Origin_triggered();
}


void linuxcnc::on_actionChinese_triggered()
{
    _ini->writeEntry("PAR_DISPLAY_LANGUAGE/VALUE",0);
//    cnc->cmd->sendSetParameter(cnc->channel,3680,1);
    show_msg(1,tr("switch to Chinese"));

    int msg = QMessageBox::warning(this,tr("Rebort Tips"),
                       tr("Please restart to take effect."),
                        tr("Restart"),tr("Cancel"));
    if( msg == 0 ) {
        exit(-1);
    }
}

void linuxcnc::on_actionEnglish_triggered()
{
    _ini->writeEntry("PAR_DISPLAY_LANGUAGE/VALUE",1);
//    cnc->cmd->sendSetParameter(cnc->channel,3680,2);
    show_msg(1,tr("switch to English"));
    int msg = QMessageBox::warning(this,tr("Rebort Tips"),
                       tr("Please restart to take effect."),
                        tr("Restart"),tr("Cancel"));
    if( msg == 0 ) {
        exit(-1);
    }
}




void linuxcnc::on_btn_run_clicked()
{
    pro_run();
}

void linuxcnc::on_btn_set_fixed_clicked()
{
    datumPara->exec();
}

void linuxcnc::on_btn_setFeed_clicked()
{
    set_z_feed->show();
}




void linuxcnc::messageFromCam(int type,QString msg){

    qDebug()<<"++in cnc++++type"<<type<<"++++++msg"<<msg;
    if(type == 3){
        if(_all_homed){
            QString res = "";
            res.append(QString::number(get_offset_pos(AXIS_X,_doublez),'f',3));
            res.append(";");
            if(origin == 2 &&  par_get_var(3120) == 0) res.append(QString::number(get_offset_pos(AXIS_Y2,_doublez),'f',3));
            else res.append(QString::number(get_offset_pos(AXIS_Y,_doublez),'f',3));
            res.append(";");
            res.append(QString::number(get_offset_pos(AXIS_Z1,_doublez),'f',3));
            res.append(";");

            sendMessageToCAM(3,res);

        }
    }


}

void linuxcnc::getPanelNew()
{

    if(emcStatus->motion.traj.linktoEth == 0)
        return;

    readIn0 = (int)readhal->flushData("ethercat.master.planein0",0);
    readIn1 = (int)readhal->flushData("ethercat.master.planein1",0);

    for(int i=0;i<32;i++) {
        if((readIn0 & (0x01<<i)) > 0 && (recordIn0 & (0x01<<i)) == 0) {
            sendHightSignal(i);
        } else if((readIn0 & (0x01<<i)) == 0 && (recordIn0 & (0x01<<i)) > 0) {
            sendLowSignal(i);
        }
    }
    for(int i=0;i<32;i++) {
        if((readIn1 & (0x01<<i)) > 0 && (recordIn1 & (0x01<<i)) == 0) {
            sendHightSignalv2(i);
        } else if((readIn1 & (0x01<<i)) == 0 && (recordIn1 & (0x01<<i)) > 0) {
            sendLowSignalv2(i);
        }
    }

    QString showpin = "";
    QString showpin1 = "";
    QString pinnum = "";
    if(readIn0 != recordIn0 || readIn1 != recordIn1 ) {
        for(int i=0;i<32;i++) {
            if((readIn0 & (0x01<<i)) > 0) {
                showpin.append(" 01");
            } else if((readIn0 & (0x01<<i)) == 0) {
                 showpin.append(" 00");
            }

            if((readIn1 & (0x01<<i)) > 0) {
                showpin1.append(" 01");
            } else if((readIn1 & (0x01<<i)) == 0) {
                 showpin1.append(" 00");
            }

            QString temp="";
            temp.sprintf(" %02d",i);
            pinnum.append(temp);
        }
//        printf("read IN0 %x read in1 %x\n",readIn0,readIn1);
//        qDebug()<<"pinnum=="<<pinnum<<"\n"<<"showpin=="<<showpin<<"\n"<<"showpin1=="<<showpin1<<"\n";
    }

    recordIn0 = readIn0;
    recordIn1 = readIn1;
    sendSwitchChange();
    if( ((readIn1 >> 7) & 0x01) > 0)
        externEstop = 0;
    else
        externEstop = 1;

//    qDebug()<<"externEstop"<<externEstop<<externEstop;
}

void linuxcnc::sendHightSignal(int index)
{
    if(paneltype == 1) {
        switch(index) {
        case 02: break;
        case 03: break;
        case 04: ui->btn_abort->setDown(false);break;
        case 05:
            if(ui->main_tab->currentIndex() == 4)
                ui->main_tab->setCurrentIndex(0);
            ui->mode_tab->setCurrentIndex(1);
//            this->showMinimized();
//            this->showMaximized();
            //qDebug()<<"this0000000---"<<this->isHidden()<<this->isActiveWindow();
            if(!this->isActiveWindow()){
                this->activateWindow();
               // this->showMinimized();
                this->showMaximized();

            }

            break;
        case 06:
            this->set_flood();
            break;
        case 07:
            this->set_spindle();
            break;
        default:
            break;
        }
    } else {
        switch(index) {
        case 02: break;
        case 03: break;
        case 04: ui->btn_abort->setDown(false);break;
        case 05:
            if(ui->mode_tab->currentWidget()->objectName() != "tab_manual")
                break;
            ui->btn_z_plus->setDown(true);coor_plus();
            break;
        case 06:
            if(ui->mode_tab->currentWidget()->objectName() != "tab_manual")
                break;
            ui->btn_y_plus->setDown(true);coor_plus();
            break;
        case 07:
            if(ui->mode_tab->currentWidget()->objectName() != "tab_manual")
                break;
            ui->btn_x_plus->setDown(true);coor_plus();
            break;
        default:
            break;
        }
    }
}

void linuxcnc::sendLowSignal(int index)
{
    if(paneltype == 1) {
        switch(index) {
        case 04: pro_abort();break;
        case 05:
            break;
        case 06:
            break;
        case 07:
            break;
        default:
            break;
        }
    } else {

        switch(index) {
        case 04: pro_abort();break;
        case 05:
            ui->btn_z_plus->setDown(false);coor_decrease();
            break;
        case 06:
            ui->btn_y_plus->setDown(false);coor_decrease();
            break;
        case 07:
            ui->btn_x_plus->setDown(false);coor_decrease();
            break;
        default:
            break;
        }
    }
}

void linuxcnc::sendHightSignalv2(int index)
{
    if(paneltype == 1) {
        switch(index) {
        case 03: pro_run();break;
        case 04:
            if(ui->main_tab->currentIndex() == 4)
                ui->main_tab->setCurrentIndex(0);
             ui->mode_tab->setCurrentIndex(0);
            break;
        case 05:
            on_btn_cool2_clicked();
            break;
        case 06:{
//            this->on_actionLight_triggered();

                ui->mode_tab->setCurrentIndex(3);
//            int ret = QMessageBox::information(this,"提示","确定进行对刀操作?",QMessageBox::Ok,QMessageBox::Cancel);
//            if(ret == QMessageBox::Ok) {
//                secondProbe();
//            }
                static CAutoMessageBox *box=nullptr ;
                qDebug()<<"isbox------------------"<<bool(!box);
                if( !box ) {
                    box = new CAutoMessageBox(this,"提示","确定进行对刀操作?");
                    box->show();
                }else{
                    box->close();
                    delete box;
                    box = nullptr;
                    secondProbe();
                }




        }break;
        case 07:
            break;
        default:
            break;
        }
    } else {
        switch(index) {
        case 03: pro_run();break;
        case 04:
            if(ui->mode_tab->currentWidget()->objectName() != "tab_manual")
                break;
            ui->btn_z_decrease->setDown(true);coor_plus();
            break;
        case 05:
            if(ui->mode_tab->currentWidget()->objectName() != "tab_manual")
                break;
            ui->btn_y_decrease->setDown(true);coor_plus();
            break;
        case 06:
            if(ui->mode_tab->currentWidget()->objectName() != "tab_manual")
                break;
            ui->btn_x_decrease->setDown(true);coor_plus();
            break;
        case 07:  break;//estop
        default:
            break;
        }
    }
}

void linuxcnc::sendLowSignalv2(int index)
{
    if(paneltype == 1) {
        switch(index) {
        case 04:
            break;
        case 05:
            break;
        case 06:
            break;
        case 07:externEstop = 1;
            sendSetlightcmd(3);
            sendEstop();
            show_msg(2,"外部急停报警");
            break;//estop
        default:
            break;
        }
    } else {
        switch(index) {
        case 04:
            ui->btn_z_decrease->setDown(false);coor_decrease();
            break;
        case 05:
            ui->btn_y_decrease->setDown(false);coor_decrease();
            break;
        case 06:
            ui->btn_x_decrease->setDown(false);coor_decrease();
            break;
        case 07: ;
            break;
        default:
            break;
        }
    }
}

void linuxcnc::sendSwitchChange()
{
    QString s="";
    s.append(QString("%1%2%3").arg(((readIn0 >> 3) & 0x01)).arg(((readIn1 >> 2) & 0x01)).arg(((readIn0 >> 2) & 0x01)));
    QByteArray ba=s.toLatin1();
    char *c=ba.data();
    sprate = strtol(c,NULL,2);

    QString f="";
    f.append(QString("%1%2%3%4").arg(((readIn1 >> 1) & 0x01)).arg(((readIn0 >> 1) & 0x01)).arg((readIn1 & 0x01)).arg((readIn0 & 0x01)));
    QByteArray fa=f.toLatin1();
    char *fe=fa.data();
    feedrate = strtol(fe,NULL,2);

    static int recordSpindleRate = -1;
    if(recordSpindleRate != sprate) {
         set_spindle_override->setValue((sprate+5)*10);
        this->set_spindle_override_fun();
        recordSpindleRate = sprate;
//        qDebug()<<"sprate========"<<sprate;
    }

    static int recordFeedRate = -1;
    if(recordFeedRate != feedrate) {
        set_feed_override->setValue((feedrate)*10);
        this->set_feed_override_fun();
        recordFeedRate = feedrate;
//                qDebug()<<"feedrate========"<<feedrate;
    }



}



void linuxcnc::SetReferencePos(int i)
{
    qDebug()<<"Set pos:"<<i;
    int ret = QMessageBox::information(this,"提示","是否设置参考起点?",QMessageBox::Ok,QMessageBox::Cancel);
    if(ret == QMessageBox::Ok) {
        sendSetParameter(3110+i,get_abs_pos(i));
    }
}

void linuxcnc::on_btn_edit_clicked()
{
    on_actionProEdit_triggered();
}

void linuxcnc::on_btn_orderSave_clicked()
{
    if(ui->show_pro_path->text()=="") return;
    QFileDialog fileDialog;
    QString fileName = fileDialog.getSaveFileName(this,tr("Open File"),"/home/cnext",tr("NC File(*.nc *.ngc)"));
    if(fileName == "")
    {
        return;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,tr("错误"),tr("打开文件失败"));
        return;
    }
    else
    {
        QTextStream textStream(&file);
        QString str = codeeditor->toPlainText();
        textStream<<str;
        QMessageBox::warning(this,tr("提示"),tr("保存文件成功"));

        file.close();
    }
}

void linuxcnc::on_btn_cool2_clicked()
{
    if(emcStatus->io.coolant.mist > 0) {
        sendMistOff();
    } else {
        sendMistOn();
    }
}

void linuxcnc::on_btn_centerwater_clicked()
{
    if(emcStatus->motion.mot_extern_out[15] > 0) {
        sendSetExternIO(15,0);
    } else {
        sendSetExternIO(15,1);
    }
}

void linuxcnc::on_useDistance_clicked(bool checked)
{
    if(checked) {
        useArraydistance = 1;
    } else {
        useArraydistance = 0;
    }
    _prefer->writeEntry("DEFAULT/useArraydistance",QString::number(useArraydistance,10));
}


void linuxcnc::on_btn_goZero_clicked()
{
    on_actionGO_Offset_triggered();
}


void linuxcnc::ServerNewConnection()
{
    mp_TCPSocket = mp_TCPServer->nextPendingConnection();
    if(!mp_TCPSocket){
        //QMessageBox::information(this, "QT网络通信", "未正确获取客户端连接！");
        return;
    }
    else
    {
        //QMessageBox::information(this, "QT网络通信", "成功接受客户端的连接");
        connect(mp_TCPSocket, SIGNAL(readyRead()), this, SLOT(ServerReadData()));
        connect(mp_TCPSocket, SIGNAL(disconnected()), this, SLOT(ServerDisConnection()));
    }
}

void linuxcnc::ServerReadData()
{
    if( mp_TCPSocket == nullptr ) return;
     //qDebug()<<"ServerReadData";
     char buffer[1024] = {0};
     int ret = mp_TCPSocket->read(buffer, 1024);


     if( strlen(buffer) > 0) {

         bool shouldgetMsg = true;
         if( emcStatus->task.runState != EMC_IDLE || \
            get_task_state() == EMC_TASK_STATE_ESTOP ) {
             shouldgetMsg = false;
         }
         QString msg = QString::fromLocal8Bit(buffer,ret);
         if( msg.startsWith("Position:") || msg.startsWith("isCncRunning") )
             shouldgetMsg = true;

         if( shouldgetMsg ) {


             if(msg == "Position" ){

                     QString res = "";
                     res.append(QString::number(get_offset_pos(AXIS_X,0),'f',3));
                     res.append(";");
                     res.append(QString::number(get_offset_pos(AXIS_Y,0),'f',3));
                     res.append(";");
                     res.append(QString::number(get_offset_pos(AXIS_Z1,0),'f',3));
                     res.append(";");
                     res.append(QString::number(get_offset_pos(AXIS_A,0),'f',3));
                     res.append(";");
                     res.append(QString::number(get_offset_pos(AXIS_B,0),'f',3));
                     res.append(";");
                     res.append(QString::number(get_offset_pos(AXIS_C,0),'f',3));
                     res.append(";");
                     msg = res;
                 mp_TCPSocket->write(msg.toLocal8Bit());
             } else if( msg.startsWith("GetParameter:")) {

                     QStringList v = msg.split(":");
                     int index = v[1].toInt();
                     msg = QString::asprintf("%.3f",par_get_var(index));
                     qDebug()<<msg;
                     mp_TCPSocket->write(msg.toLocal8Bit());
             } else if( msg.startsWith("SetParameter:")) {
                 QStringList v = msg.split(":");
                 int index = v[1].toInt();
                 double vv = v[2].toDouble();
                 sendSetParameter(index,vv);
                 mp_TCPSocket->write("ok");
             } else if( msg.startsWith("JogCont:")) {
                 QStringList v = msg.split(":");
                 int index = v[1].toInt();
                 double vv = v[2].toDouble();
                 sendJogCont(index,vv);
                 mp_TCPSocket->write("ok");
             } else if( msg.startsWith("JogIncr:")) {
                 QStringList v = msg.split(":");
                 int index = v[1].toInt();
                 double vv = v[2].toDouble();
                 double inc = v[3].toDouble();
                 sendJogIncr(index,vv,inc);
                 mp_TCPSocket->write("ok");
             } else if( msg.startsWith("JogIncr:")) {
                 QStringList v = msg.split(":");
                 int index = v[1].toInt();
                 sendJogStop(index);
                 mp_TCPSocket->write("ok");
             }else if(msg.startsWith("reloadNcProgram:")){

                 QStringList v = msg.split(":");
                 if(v.size()>1){
                     openCamNC(v[1]);
                 }
                 mp_TCPSocket->write("ok");

             } else if(msg.startsWith("update-tool")){

                 mp_TCPSocket->write("ok");


             } else if(msg.startsWith("isCncToolUpdate")){


                 if( sendToCamString == "ToolParamter" ) {
                     mp_TCPSocket->write("ToolParamter");
                     sendToCamString.clear();
                 } else
                     mp_TCPSocket->write("ng");


             } else if(msg.startsWith("isCncRunning")){


                 if( emcStatus->task.runState != EMC_IDLE || \
                         get_task_state() == EMC_TASK_STATE_ESTOP ) {
                     mp_TCPSocket->write("Running");
                 } else
                     mp_TCPSocket->write("ng");


             } else {
                 mp_TCPSocket->write("ok");
             }

         } else {
             mp_TCPSocket->write("ok");
         }


     } else {
         mp_TCPSocket->write("ok");

     }
}

void linuxcnc::ServerDisConnection()
{
    cam_exit = 0;
    qDebug()<<"---------sServerDisConnection--------------";
}


void linuxcnc::on_action_CAM_triggered()
{
//    if(emcStatus->task.runState != EMC_IDLE ){
//        int ret=QMessageBox::information(this,tr("提示"),
//                                         tr("请先退出运行!!"),
//                                         QMessageBox::Ok);
//        return;
//    }
    if(cam_exit==1){
        int ret=QMessageBox::information(this,tr("提示"),
                                         tr("CAM已经打开!!"),
                                         QMessageBox::Ok);
        return;
    }
    int ret = system("/home/.cam/libraryrun &");
    if(ret ==-1){
        QMessageBox::information(this,tr("提示"),
                                                 tr("CAM打开ERROR!!"),
                                                 QMessageBox::Ok);
        return;
    }
    cam_exit=1;

}

void linuxcnc::on_chk_useoptionstop_clicked()
{
    if(optionalstop > 0) {
        sendSetOptionalStop(false);
        _ini->writeEntry("PAR_TASK_OPTIONSTOP/VALUE",0);
        optionalstop = 0;
    } else {
        sendSetOptionalStop(true);
        _ini->writeEntry("PAR_TASK_OPTIONSTOP/VALUE",1);
        optionalstop = 1;
    }
}
