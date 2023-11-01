include(./libnml.pri)
include(../common.pri)

TEMPLATE = lib
TARGET = $$CNEXT_LIB_PATH/nml

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    buffer/locmem.hh \
    buffer/memsem.hh \
    buffer/phantom.hh \
    buffer/physmem.hh \
    buffer/recvn.h \
    buffer/rem_msg.hh \
    buffer/sendn.h \
    buffer/shmem.hh \
    buffer/tcpmem.hh \
    cms/cms_aup.hh \
    cms/cms_cfg.hh \
    cms/cms_dup.hh \
    cms/cms_srv.hh \
    cms/cms_up.hh \
    cms/cms_user.hh \
    cms/cms_xup.hh \
    cms/cms.hh \
    cms/cmsdiag.hh \
    cms/tcp_opts.hh \
    cms/tcp_srv.hh \
    inifile/inifile.hh \
    linklist/linklist.hh \
    nml/cmd_msg.hh \
    nml/nml_mod.hh \
    nml/nml_oi.hh \
    nml/nml_srv.hh \
    nml/nml_type.hh \
    nml/nml.hh \
    nml/nmldiag.hh \
    nml/nmlmsg.hh \
    nml/stat_msg.hh \
    os_intf/_sem.h \
    os_intf/_shm.h \
    os_intf/_timer.h \
    os_intf/inetfile.hh \
    os_intf/sem.hh \
    os_intf/shm.hh \
    os_intf/timer.hh \
    posemath/gomath.h \
    posemath/gotypes.h \
    posemath/posemath.h \
    posemath/sincos.h \
    rcs/rcs_exit.hh \
    rcs/rcs_print.hh \
    rcs/rcs.hh \
    rcs/rcsversion.h

SOURCES += \
    buffer/locmem.cc \
    buffer/memsem.cc \
    buffer/phantom.cc \
    buffer/physmem.cc \
    buffer/shmem.cc \
    buffer/tcpmem.cc \
    buffer/recvn.c \
    buffer/sendn.c \
    cms/cms_aup.cc \
    cms/cms_cfg.cc \
    cms/cms_dup.cc \
    cms/cms_in.cc \
    cms/cms_pm.cc \
    cms/cms_srv.cc \
    cms/cms_up.cc \
    cms/cms_xup.cc \
    cms/cms.cc \
    cms/cmsdiag.cc \
    cms/cmssvrp.cc \
    cms/tcp_opts.cc \
    cms/tcp_srv.cc \
    inifile/inifile.cc \
    linklist/linklist.cc \
    nml/cmd_msg.cc \
    nml/nml_mod.cc \
    nml/nml_oi.cc \
    nml/nml_srv.cc \
    nml/nml.cc \
    nml/nmldiag.cc \
    nml/nmlmsg.cc \
    nml/stat_msg.cc \
    os_intf/inetnull.cc \
    os_intf/sem.cc \
    os_intf/shm.cc \
    os_intf/timer.cc \
    os_intf/_sem.c \
    os_intf/_shm.c \
    os_intf/_timer.c \
    posemath/posemath.cc \
    posemath/_posemath.c \
    posemath/gomath.c \
    posemath/sincos.c \
    rcs/rcs_exit.cc \
    rcs/rcs_print.cc

OTHER_FILES +=

