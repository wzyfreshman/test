include (./usr_intf.pri)
include(../nml_intf/nml_intf.pri)
include(../../libnml/libnml.pri)
include(../../common.pri)
include(../motion/motion.pri)
include(../rs274ngc/rs274ngc.pri)

TEMPLATE = lib

TARGET = $$CNEXT_LIB_PATH/usr_intf

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    shcom.hh \
    emcstatus.hh

SOURCES += \
    emcstatus.cc \
    shcom.cc
