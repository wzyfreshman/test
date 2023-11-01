include(../hal/hal.pri)
include(../../common.pri)

TEMPLATE = app
TARGET = $$CNEXT_BIN_PATH/halcmd

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    halcmd_commands.c \
    halcmd_completion.c \
    halcmd_main.c \
    halcmd.c \
    inifile.cc

INCLUDEPATH += ../ \
    ../hal/rtapi \
    ../hal


LIBS += -L$$CNEXT_LIB_PATH  -lhal_usr -ldl  -lreadline

HEADERS += \
    halcmd_commands.h \
    halcmd_completion.h \
    halcmd.h \
    inifile.hh

DEFINES +=ULAPI

