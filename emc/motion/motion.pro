include(./motion.pri)
include(../../common.pri)
include(../../realtime/hal/hal.pri)
include(../nml_intf/nml_intf.pri)
include(../kinematics/segment/segment.pri)

TEMPLATE = lib
TARGET =  $$CNEXT_MODULES_PATH/motmod

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += RTAPI

LIBS +=  -L$$CNEXT_MODULES_PATH   -lhal_mod -llkt5103
LIBS += -L$$CNEXT_MODULES_PATH -lseg

HEADERS += \
    dbuf.h \
    des.h \
    emcmotcfg.h \
    emcmotglb.h \
    mot_priv.h \
    motion_debug.h \
    motion_struct.h \
    motion.h \
    stashf.h \
#    ../kinematics/cubic.h \
#    ../kinematics/segmentqueue.h \
#    ../kinematics/tc.h \
#    ../kinematics/tp.h \
    posemath.h \
    sincos.h \
    lkt5103.h \
    epp.h

SOURCES += \
    command.c \
    control.c \
    dbuf.c \
    des.c \
    emcmotglb.c \
    emcmotutil.c \
    homing.c \
    motion.c \
    stashf.c \
#    ../kinematics/cubic.c \
#    ../kinematics/segmentqueue.c \
#    ../kinematics/tc.c \
#    ../kinematics/tp.c \
    _posemath.c \
    sincos.c \
    epp.c

OTHER_FILES += \
    ../../po/zh_CN.po
