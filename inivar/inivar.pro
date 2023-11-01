include(../common.pri)

TEMPLATE = app
TARGET = $$CNEXT_BIN_PATH/inivar

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    inifile.hh

SOURCES += \
    inifile.cc \
    inivar.cc
