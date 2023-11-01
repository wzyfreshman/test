include(./rs274ngc.pri)
include(../nml_intf/nml_intf.pri)
include(../../libnml/libnml.pri)
include(../../common.pri)
include(../motion/motion.pri)

TEMPLATE = lib
TARGET =  $$CNEXT_LIB_PATH/rs274ngc

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    interp_internal.hh \
    interp_queue.hh \
    rs274ngc_interp.hh \
    rs274ngc_return.hh \
    rs274ngc.hh \
    tool_parse.h \
    units.h \
    norm.h \
    computerCircleWith3point.h

SOURCES += \
    interp_arc.cc \
    interp_array.cc \
    interp_check.cc \
    interp_convert.cc \
    interp_cycles.cc \
    interp_execute.cc \
    interp_find.cc \
    interp_internal.cc \
    interp_inverse.cc \
    interp_o_word.cc \
    interp_queue.cc \
    interp_read.cc \
    interp_write.cc \
    nurbs_additional_functions.cc \
    rs274ngc_pre.cc \
    tool_parse.cc \
    norm.c \
    computerCircleWith3point.c

