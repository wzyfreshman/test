include(../../hal/hal.pri)
include(../../../common.pri)


TEMPLATE = lib
TARGET = $$CNEXT_MODULES_PATH/scope_rt

LIBS += -L$$CNEXT_MODULES_PATH  -lhal_mod

INCLUDEPATH += ../../halscope

DEFINES += RTAPI

CONFIG += console
CONFIG -= app_bundle
CONFIG += qt

HEADERS += \
    scope_rt.h

SOURCES += \
    scope_rt.c
