#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T14:13:58
#
#-------------------------------------------------

QT       += core gui widgets
QT       += widgets

TARGET = ../InitHardware_eth
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp


HEADERS  += mainwindow.h \
    lkt5103.h


INCLUDEPATH += \
        ../math
LIBS += -L../ -lmath
LIBS += -L../ -llkt5103
LIBS += -L/usr/local/ssl/lib/  -lcrypto  -lssl -ldl
INCLUDEPATH += /usr/local/ssl/include



FORMS    += mainwindow.ui
