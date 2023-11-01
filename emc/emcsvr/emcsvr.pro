include(../rs274ngc/rs274ngc.pri)
include(../motion/motion.pri)
include(../nml_intf/nml_intf.pri)
include(../../common.pri)
include(../../libnml/libnml.pri)
include(../../realtime/hal/hal.pri)

TEMPLATE = app
TARGET =  $$CNEXT_BIN_PATH/linuxcncsvr

LIBS += -L$$CNEXT_LIB_PATH  -lnml_intf -lnml

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    emcsvr.cc

