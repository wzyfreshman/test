include(../hal/hal.pri)
include(../../common.pri)


TEMPLATE = app
TARGET = $$CNEXT_BIN_PATH/halscope

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt



QMAKE_CFLAGS += $(shell pkg-config --cflags gtk+-2.0 )
QMAKE_LFLAGS += $(shell pkg-config --libs gtk+-2.0 )


LIBS += -L$$CNEXT_LIB_PATH  -lhal_usr -ldl  -lreadline

LIBS += -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0  \
                 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 \
                 -lcairo -lpango-1.0 -lfontconfig \
                 -lgobject-2.0 -lglib-2.0 -lfreetype

DEFINES += ULAPI


HEADERS += \
    scope_shm.h \
    scope_usr.h \
    miscgtk.h

SOURCES += \
    miscgtk.c \
    scope.c \
    scope_disp.c \
    scope_files.c \
    scope_horiz.c \
    scope_trig.c \
    scope_vert.c
