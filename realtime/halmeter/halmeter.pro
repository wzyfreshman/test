include(../hal/hal.pri)
include(../../common.pri)


TEMPLATE = app
TARGET = $$CNEXT_BIN_PATH/halmeter

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    meter.c \
    miscgtk.c

HEADERS += \
    miscgtk.h


INCLUDEPATH += ../ \
    ../hal/rtapi \
    ../hal \
    /usr/include/gtk-2.0/


QMAKE_CFLAGS += $(shell pkg-config --cflags gtk+-2.0 )
QMAKE_LFLAGS += $(shell pkg-config --libs gtk+-2.0 )


LIBS += -L$$CNEXT_LIB_PATH  -lhal_usr -ldl  -lreadline

LIBS += -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0  \
                 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 \
                 -lcairo -lpango-1.0 -lfontconfig \
                 -lgobject-2.0 -lglib-2.0 -lfreetype

DEFINES +=ULAPI










