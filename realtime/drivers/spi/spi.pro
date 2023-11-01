include(../../hal/hal.pri)
include(../../../common.pri)

TEMPLATE = lib
TARGET = $$CNEXT_MODULES_PATH/hal_spi
LIBS += -L$$CNEXT_MODULES_PATH  -lhal_mod
DEFINES += RTAPI

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += hal_spi.c

