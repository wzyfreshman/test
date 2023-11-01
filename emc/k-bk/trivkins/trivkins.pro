include(../../../common.pri)
include(../../../realtime/hal/hal.pri)
include(../../../libnml/libnml.pri)
include(../../nml_intf/nml_intf.pri)


TEMPLATE = lib
TARGET =  $$CNEXT_MODULES_PATH/trivkins

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += RTAPI

LIBS +=  -L$$CNEXT_MODULES_PATH   -lhal_mod

SOURCES += \
    ../trivkins.c

HEADERS += \
    ../kinematics.h
