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
include(../ccd/ccd.pri)

QT       += core gui opengl
QT       += widgets
QT += testlib

QT += network

TARGET = $$CNEXT_GUI_PATH/ui
TEMPLATE  =  app

LIBS += -L$$CNEXT_LIB_PATH -lgcode -lusr_intf -lnml_intf -lnml -lrs274ngc -lhal_usr  -lmath  -llkt5103
#LIBS += -L$$CNEXT_MODULE_PATH  -llkt5103

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
#    fileeditor.cpp \
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
    tooltimes.cpp \
    system_setup.cpp \
    rwmotor.cpp \
    datum.cpp \
#    cnc_manager.cpp \
    pickz.cpp \
    datum_parameter.cpp \
    set_feed.cpp \
    datum_explain.cpp \
    fileeditor.cpp \
    unit_sharehal.cpp \
    tchpar.cpp \
    unit_serialport.cpp \
    unit_rs485setting.cpp \
    serialzeropos.cpp \
    touch_checklink.cpp \


HEADERS  += linuxcnc.h \
    extern.h \
    offsetdialog.h \
    gl_viewwidget.h \
    io_input.h \
    codeeditor.h \
    io_output.h \
    info_tabledialog.h \
    mywidget.h \
#    fileeditor.h \
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
    tooltimes.h \
    system_setup.h \
    rwmotor.h \
    datum.h \
#    cnc_manager.h \
    pickz.h \
    datum_parameter.h \
    set_feed.h \
    datum_explain.h \
    fileeditor.h \
    unit_sharehal.h \
    tchpar.h \
    unit_serialport.h \
    unit_rs485setting.h \
    serialzeropos.h \
    math/epp.h \
    math/hexvalidator.h \
    math/customer.h \
    math/des.h \
    math/aes.h \
    math/lkt5103.h \
    math/rsa.h \
    math/udf.h \
    math/hash.h \
    touch_checklink.h \


FORMS    += linuxcnc.ui \
    offsetdialog.ui \
    io_input.ui \
    io_output.ui \
    info_tabledialog.ui \
#    fileeditor.ui \
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
    tooltimes.ui \
    system_setup.ui \
    rwmotor.ui \
    datum.ui \
    pickz.ui \
    datum_parameter.ui \
    set_feed.ui \
    datum_explain.ui \
    fileeditor.ui \
    tchpar.ui \
    unit_rs485setting.ui \
    serialzeropos.ui \
    touch_checklink.ui \



#LIBS += -L../ccd/lib -lccd -lcamera -lccdcore

INCLUDEPATH +=\
               ../encrypt/math \
               /usr/local/ssl/include \
               ../../usr_intf/ \
               math \
               /home/linaro/3568/qwt-6.2.0/include

INCLUDEPATH +=/usr/local/qwt-6.3.0-svn/include

LIBS += -L/usr/local/ssl/lib/  -lcrypto  -lssl -ldl

LIBS +=  -L/home/linaro/3568/qwt-6.2.0/lib -lqwt -lGLEW -lGLU -lOpenGL


RESOURCES += \
    source.qrc \
    Image/icons.qrc \
    ../source/log.qrc \
    ../source/system.qrc



#QMAKE_POST_LINK +=  cp $$OUT_PWD/../../../cnc-install/.cnc/gui/ui /home/.cnc/gui/


TRANSLATIONS += \
    tran_zh_CN.ts

OTHER_FILES += \
    example.py \
    tran_zh_CN.ts




DEFINES += ULAPI

