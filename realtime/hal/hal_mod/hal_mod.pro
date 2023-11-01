include(../../../common.pri)
include(../hal.pri)

TARGET = $$CNEXT_MODULES_PATH/hal_mod
TEMPLATE = lib

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG_PATH = ../../

HEADERS += \
    ../rtapi/rtapi_app.h \
    ../rtapi/rtapi_bitops.h \
    ../rtapi/rtapi_common.h \
    ../rtapi/rtapi_errno.h \
    ../rtapi/rtapi_implementate.h \
    ../rtapi/rtapi_string.h \
    ../rtapi/rtapi.h \
    ../hal_priv.h \
    ../hal.h \
    ../rtapi/rtapi_math.h \
    ../hal_parport.h

SOURCES += \
    ../rtapi/rtapi_common.c \
    ../rtapi/rtapi_implementate.c \
    ../rtapi/rtapi_task.c \
    ../hal_lib.c


INCLUDEPATH += ../ \
    ../rtapi \
    $$CONFIG_PATH

DEFINES += RTAPI


