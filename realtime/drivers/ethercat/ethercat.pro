include(../../hal/hal.pri)
include(../../../common.pri)


TEMPLATE = lib
TARGET = $$CNEXT_MODULES_PATH/hal_ethercat
LIBS += -L/opt/etherlab/lib
LIBS += -L$$CNEXT_MODULES_PATH  -lhal_mod

LIBS += -fPIC
INCLUDEPATH += /opt/etherlab/include

DEFINES += RTAPI

CONFIG += console
CONFIG -= app_bundle
CONFIG += qt

QT  += core xml

SOURCES += \
    hal_ethercat.c




