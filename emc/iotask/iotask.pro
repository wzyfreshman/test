include(../rs274ngc/rs274ngc.pri)
include(../motion/motion.pri)
include(../nml_intf/nml_intf.pri)
include(../../common.pri)
include(../../libnml/libnml.pri)
include(../../realtime/hal/hal.pri)

TEMPLATE = app
TARGET =  $$CNEXT_BIN_PATH/iocontrol

LIBS +=  -L$$CNEXT_LIB_PATH  -lnml_intf   -lnml  -lhal_usr


CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ioControl.cc \
    ../rs274ngc/tool_parse.cc

DEFINES += ULAPI

HEADERS += \
    ../rs274ngc/tool_parse.h
