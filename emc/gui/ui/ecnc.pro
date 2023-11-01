#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T07:48:17
#
#-------------------------------------------------

include(../../rs274ngc/rs274ngc.pri)
include(../../motion/motion.pri)
include(../../nml_intf/nml_intf.pri)
include(../../../common.pri)
include(../../../libnml/libnml.pri)
include(../../../realtime/hal/hal.pri)
include(../../usr_intf/usr_intf.pri)


QT       += core gui opengl

QT       += widgets

TARGET = $$ECNC_GUI_PATH/ecnc
TEMPLATE = app
LIBS += -L$$ECNC_LIB_PATH -lgcode -lusr_intf -lnml_intf -lnml -lrs274ngc


SOURCES += main.cpp\
        linuxcnc.cpp \
    extern.cpp \
    offsetdialog.cpp \
    gl_viewwidget.cpp \
    io_input.cpp \
    codeeditor.cpp \
    io_output.cpp \
    info_tabledialog.cpp \
    mywidget.cpp \
    fileeditor.cpp \
    actionfactory.cpp \
    tbl_dialog.cpp \
    ini_settings.cpp \
    smartdialog.cpp \
    paradialog.cpp \
    iodialog.cpp \
    homedialog.cpp \
    decrypt_dialog.cpp \
    infodialog.cpp \
    smartrundialog.cpp \
    gl_canondraw.cpp \
    gl_gcodedraw.cpp \
    gl_viewdraw.cpp \
    numstr_inputdialog.cpp \
    multioffset.cpp \
    labeltimer.cpp \
    toolchanger.cpp \
    gl_property.cpp \
    grouptablewidget.cpp \
    mytabbar.cpp \
    pickz.cpp   \
    tooltimes.cpp \
    system_setup.cpp \
    rwmotor.cpp \
    datum.cpp




HEADERS  += linuxcnc.h \
    extern.h \
    offsetdialog.h \
    gl_viewwidget.h \
    io_input.h \
    codeeditor.h \
    io_output.h \
    info_tabledialog.h \
    mywidget.h \
    fileeditor.h \
    actionfactory.h \
    actiontype.h \
    tbl_dialog.h \
    ini_settings.h \
    smartdialog.h \
    paradialog.h \
    iodialog.h \
    homedialog.h \
    decrypt_dialog.h \
    infodialog.h \
    smartrundialog.h \
    gl_canondraw.h \
    gl_viewdraw.h \
    gl_gcodedraw.h \
    numstr_inputdialog.h \
    multioffset.h \
    labeltimer.h \
    toolchanger.h \
    gl_property.h \
    mytabbar.h \
    grouptablewidget.h \
    pickz.h\
    tooltimes.h \
    system_setup.h \
    rwmotor.h \
    datum.h



FORMS    += linuxcnc.ui \
    offsetdialog.ui \
    io_input.ui \
    io_output.ui \
    info_tabledialog.ui \
    fileeditor.ui \
    tbl_dialog.ui \
    smartdialog.ui \
    paradialog.ui \
    iodialog.ui \
    homedialog.ui \
    decrypt_dialog.ui \
    infodialog.ui \
    smartrundialog.ui \
    numstr_inputdialog.ui \
    multioffset.ui \
    toolchanger.ui \
    gl_property.ui \
    pickz.ui\
    tooltimes.ui \
    system_setup.ui \
    rwmotor.ui \
    datum.ui

DIR = /var/work/linuxcnc

LIBS += -L../encrypt -lmath
LIBS += -L../ -llkt5103




INCLUDEPATH += /usr/local/qwt-6.1.1/include \
               ../encrypt/math \
               /usr/local/ssl/include

LIBS += -L/usr/local/ssl/lib/  -lcrypto  -lssl -ldl

LIBS += -L/usr/local/qwt-6.1.1/lib -l qwt -lGLEW -lGLU -lGL


RESOURCES += \
    Image/icons.qrc \
    ../source/log.qrc \
    source.qrc\
    ../source/system.qrc\


TRANSLATIONS += \
    tran_zh_CN.ts

OTHER_FILES += \
    example.py \
    tran_zh_CN.ts



