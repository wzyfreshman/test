#-------------------------------------------------
#
# Project created by QtCreator 2014-01-16T05:12:30
#
#-------------------------------------------------

QT       += core gui opengl

win32 {
    TARGET = ../../bin/ccd_demo
}
unix {
    TARGET = ../lib/ccd
}

TEMPLATE = lib

include(../ccd.pri)
include(../common.pri)
include(../opencv.pri)
include(../camera_api.pri)

#DEFINES += HAS_BOOST=1

SOURCES +=\
    cd_application.cpp \
    cnc_listener.cpp \
    ui/ccd_mainwidget.cpp \
    ui/ccd_syspareditor.cpp \
    ui/ccd_markeditor.cpp \
    ui/ccd_sysparameter.cpp \
    ui/ccd_marksface.cpp \
    ui/ccd_cncmanager.cpp \
    ui/ccd_mark.cpp \
    ui/ccd_sysparcommoneditor.cpp \
    ui/ccd_magviewdlg.cpp



HEADERS  += \
    cnc_listener.h \
    cd_application.h \
    ui/ccd_mainwidget.h \
    ui/ccd_syspareditor.h \
    ui/ccd_markeditor.h \
    ui/ccd_sysparameter.h \
    ui/ccd_marksface.h \
    ui/ccd_cncmanager.h \
    ui/ccd_mark.h \
    ui/ccd_sysparcommoneditor.h \
    ui/ccd_magviewdlg.h


#***********boost****
INCLUDEPATH +=\
    /usr/include/boost

unix {
INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv

LIBS += -L /usr/local/lib \
    -lopencv_imgproc -lopencv_highgui -lopencv_core -lopencv_legacy -lopencv_nonfree -lopencv_calib3d



INCLUDEPATH += ../camera
INCLUDEPATH += ../include/

LIBS += -L../lib/ -lcamera
LIBS += ../lib/libccdcore.so.1

}


LIBS += -L../lib/ -lcamera

INCLUDEPATH += ../camera


FORMS    += \
    ui/ccd_editwidget.ui \
    ui/ccd_syspareditor.ui \
    ui/ccd_sysparcommoneditor.ui \
    ui/ccd_computedlg.ui \
    ui/ccd_magviewdlg.ui \
    ui/widget.ui

OTHER_FILES +=

RESOURCES += \
    resource/resource.qrc
