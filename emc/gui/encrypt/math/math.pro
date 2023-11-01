QT       += core gui
QT       += widgets

#TARGET = ../math
include(../../../../common.pri)

TEMPLATE = lib
TARGET =  $$CNEXT_LIB_PATH/math

#CONFIG += staticlib

LIBS += -L/usr/local/ssl/lib/  -lcrypto  -lssl -ldl
LIBS += -L$$CNEXT_LIB_PATH -llkt5103

INCLUDEPATH += /usr/local/ssl/include


HEADERS += \
    des.h \
    rsa.h \
    hash.h \
    aes.h \
    udf.h \
    hexvalidator.h \
    epp.h \
    customer.h \
    lkt5103.h

SOURCES += \
    des.cpp \
    rsa.cpp \
    hash.cpp \
    aes.cpp \
    udf.cpp \
    hexvalidator.cpp \
    epp.cpp \
    customer.cpp

