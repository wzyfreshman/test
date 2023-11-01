include(./gcode.pri)
include(../nml_intf/nml_intf.pri)
include(../../libnml/libnml.pri)
include(../rs274ngc/rs274ngc.pri)
include(../../common.pri)
include(../motion/motion.pri)

TEMPLATE = lib
TARGET =  $$CNEXT_LIB_PATH/gcode

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    gcodedraw.h

SOURCES += \
    gcodedraw.cc \
    gcodemodule.cc
