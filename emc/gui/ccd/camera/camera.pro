TEMPLATE = lib

win32 {

    TARGET = ../../lib/camera
    CONFIG += qt static

}

unix {

    TARGET = ../lib/camera
    CONFIG += qt shared

}

include(../camera_api.pri)
include(../opencv.pri)
include(../common.pri)

SOURCES += camera.cpp
HEADERS += \
    camera.h

