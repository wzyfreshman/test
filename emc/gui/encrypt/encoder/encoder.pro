#-------------------------------------------------
#
# Project created by QtCreator 2014-07-09T17:36:10
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

TARGET = ../encode-v2
TEMPLATE = app

INCLUDEPATH += \
        ../math

LIBS += -L$$CNEXT_LIB_PATH  -lmath


LIBS += -L/usr/local/ssl/lib/  -lcrypto  -lssl -ldl
INCLUDEPATH += /usr/local/ssl/include


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
