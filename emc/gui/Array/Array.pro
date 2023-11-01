#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T17:45:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Array
TEMPLATE = lib

DIR = /var/work/linuxcnc

INCLUDEPATH +=  +=  $$DIR/src/emc/nml_intf \
                $$DIR/include \
                $$DIR/src/emc/usr_intf/

SOURCES += main.cpp\
        array.cpp \
    node.cpp \
    compensation.cpp \
    ini_settings.cpp

HEADERS  += array.h \
    node.h \
    compensation.h \
    ini_settings.h

FORMS    += array.ui \
    compensation.ui
