#include <QApplication>
#include <QtDebug>
#include <QTextCodec>
#include <QTimer>
#include <QTranslator>
#include <signal.h>		// signal(), SIGINT
#include <QStyle>
#include <QStyleFactory>
#include <QFont>

#include "emcglb.h"
#include "linuxcnc.h"
#include "rcs_print.hh"
#include "_timer.h"
#include "shcom.hh"
#include "extern.h"
#include  <unistd.h>

void UiExit(int sig) {
    printf("now is exit(%d)\n",sig);
    exit(0);
}

int main(int argc, char *argv[])
{
         QApplication a(argc, argv);
        QTranslator translator;
        translator.load("tran_zh_CN","/home/.cnc/gui");
        a.installTranslator(&translator);

        QTranslator translatora;
        translatora.load("qtbase_zh","/home/.cnc/gui");
        a.installTranslator(&translatora);

        QTranslator translatorB;
        translatorB.load("qt_zh","/home/.cnc/gui");
        a.installTranslator(&translatorB);

    //QStyleFactory *bb = new QStyleFactory();
    //qDebug()<<bb->keys();
    a.setStyle("gtk");
    //a.setFont(QFont("SansSerif", 10, QFont::Bold));
    //a.setStyleSheet("checked:");
    // trap ^C
    signal(SIGINT, UiExit);
    // and SIGTERM (used by runscript to shut down)
    signal(SIGTERM, UiExit);

    double start;
    int good;
    if(0 != emcGetArgs(argc, argv))
    {
        exit(0);
    }
    iniLoad(EMC_INIFILE);

    // init NML

  #define RETRY_TIME 10.0         // seconds to wait for subsystems to come up
  #define RETRY_INTERVAL 0.01      // seconds between wait tries for a subsystem


    if (! (EMC_DEBUG & EMC_DEBUG_NML)) {
        set_rcs_print_destination(RCS_PRINT_TO_NULL);     // inhibit diag messages
    }
    start = etime();
    good = 0;

    do {
      if (0 == emcTaskNmlGet()) {
        good = 1;
        break;
      }
      esleep(RETRY_INTERVAL);
    } while (etime() - start < RETRY_TIME);

    good = 1;
     if (! good) {
      rcs_print_error("can't establish communication with emc\n");
      exit(1);
    }

    start = etime();
    good = 0;
    do {
      if (0 == emcErrorNmlGet()) {
        good = 1;
        break;
      }
      esleep(RETRY_INTERVAL);
    } while (etime() - start < RETRY_TIME);

      if (! good) {
      rcs_print_error("can't establish communication with emc\n");
      exit(1);
    }

    _ini_filepath = argv[2];
    QStringList temp = _ini_filepath.split("/");
    temp.removeAt(temp.count()-1);
    _subDir = temp.join("/").append("/subNgc/");
    _ini_filebak = _ini_filepath;
    _ini_filebak.append(".bak");
    _var_filepath = temp.join("/").append("/interp.var");
    _var_filebak = _var_filepath;
    _var_filebak.append(".bak");
    _zero_filepath = temp.join("/").append("/zero_pos");
    _zero_filebak = _zero_filepath;
    _zero_filebak.append(".bak");
    _tbl_filepath = temp.join("/").append("/tool.tbl");
    _tbl_filebak = _tbl_filepath;
    _tbl_filebak.append(".bak");
    inter_translate_init();

/*
    int nIndex = QFontDatabase::addApplicationFont("/home/hong/font.TTF");
    if (nIndex != -1) {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0)  {
            QFont fontThis(strList.at(0));
            fontThis.setPointSize(12);
            w.setFont(fontThis);
        }
    }
*/

    QFile  *settings = new QFile("/home/cnext/Title/title.txt");
      QString value;
      if( settings->open(QFile::ReadOnly)) {
          QTextCodec *codec = QTextCodec::codecForName("utf-8");
          value=codec->toUnicode(settings->readAll());

      } else {
          static const QDate buildDate = QLocale( QLocale::English ).toDate( QString( __DATE__ ).replace( "  ", " 0" ), "MMM dd yyyy");
          value = "CNEXT-A809-CAM-";
          value.append(buildDate.toString("yyyyMMdd"));
      }
      delete settings;

      value.remove("\n");

//      static const QDate buildDate = QLocale( QLocale::English ).toDate( QString( __DATE__ ).replace( "  ", " 0" ), "MMM dd yyyy");
//      value = "CNEXT-A809-CAM-";
      //      value.append(buildDate.toString("yyyyMMdd"));

      system("chmod 777 /home/.cam -R");
      system("/home/.cam/libraryrun &");



      linuxcnc w;
      w.cam_exit =1;
      w.setWindowTitle(value);
      w.setWindowIcon(QIcon("/home/cnext/Title/icon.png"));
      w.show();
      w.setapp(&a);
      a.installEventFilter(&w);


//    system("chmod 777 /home/.cam -R");
//    w.cam_exit =1;
//    int ret = system("/home/.cam/libraryrun &");
//    if(ret ==-1){
//        w.cam_exit = 0;
//    }

    return a.exec();
}
