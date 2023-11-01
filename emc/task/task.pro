include(./task.pri)
include(../rs274ngc/rs274ngc.pri)
include(../motion/motion.pri)
include(../nml_intf/nml_intf.pri)
include(../../common.pri)
include(../../libnml/libnml.pri)
include(../../realtime/hal/hal.pri)

TEMPLATE = app
TARGET = $$CNEXT_BIN_PATH/milltask

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L$$CNEXT_LIB_PATH  -lnml_intf -lnml -lrs274ngc -lhal_usr
LIBS +=  -L$$CNEXT_MODULES_PATH -llkt5103

HEADERS += \
    des.h \
    hash.h \
    task.hh \
    ../motion/emcmotglb.h \
    ../motion/dbuf.h \
    ../motion/stashf.h \
    ../motion/usrmotintf.h \
    epp.h

SOURCES += \
    taskintf.cc \
    emccanon.cc \
    emctask.cc \
    emctaskmain.cc \
    hash.cpp \
    iotaskintf.cc \
    des.cc \
    ../motion/emcmotglb.c \
    ../motion/dbuf.c \
    ../motion/emcmotutil.c \
    ../motion/stashf.c \
    ../motion/usrmotintf.cc \
    epp.cpp
