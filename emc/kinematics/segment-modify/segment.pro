include(../../../common.pri)
include(../../../realtime/hal/hal.pri)
include(../../../libnml/libnml.pri)
include(../../nml_intf/nml_intf.pri)


TEMPLATE = lib
TARGET =  $$CNEXT_MODULES_PATH/seg

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += RTAPI

LIBS +=  -L$$CNEXT_MODULES_PATH   -lhal_mod

SOURCES += \
    cubic.c \
    segmentqueue.c \
    segment_five.c \
    segment_seven.c

HEADERS += \
    segmentqueue.h \
    cubic.h \
    segment_five.h \
    segment_seven.h
