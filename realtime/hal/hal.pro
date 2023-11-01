TEMPLATE = subdirs
CONFIG += ordered

include(./hal.pri)

SUBDIRS = \
    hal_usr \
    hal_mod \
    rtscope
