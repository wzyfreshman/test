QT       += core gui
QT       += widgets

TARGET = ../decode
TEMPLATE = app


INCLUDEPATH += \
        ../math

LIBS += -L../ -lmath
LIBS += -L../ -llkt5103

SOURCES += main.cpp \
    dialog.cpp

HEADERS  += \
    dialog.h

LIBS += -L/usr/local/ssl/lib/  -lcrypto  -lssl -ldl
INCLUDEPATH += /usr/local/ssl/include


FORMS    += \
    dialog.ui
