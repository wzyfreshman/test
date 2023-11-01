include(../../libnml/libnml.pri)
include(../../common.pri)
include(./nml_intf.pri)
include(../motion/motion.pri)
include(../rs274ngc/rs274ngc.pri)

TEMPLATE = lib

TARGET = $$CNEXT_LIB_PATH/nml_intf

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += static

HEADERS += \
    emc.hh \
    emccfg.h \
    emcglb.h \
    emcpos.h \
    emctool.h \
    interp_return.hh \
    interp.hh \
    interpl.hh \
    motion_types.h \
    ../ini/emcIniFile.hh \
    ../ini/iniaxis.hh \
    ../ini/initool.hh \
    ../ini/initraj.hh

SOURCES += \
    emcargs.cc \
    emcops.cc \
    interpl.cc \
    emcglb.c \
    emc.cc \
    ../ini/emcIniFile.cc \
    ../ini/iniaxis.cc \
    ../ini/initool.cc \
    ../ini/initraj.cc


