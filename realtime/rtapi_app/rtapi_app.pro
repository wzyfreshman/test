include(../hal/hal.pri)
include(../../common.pri)

TEMPLATE = app
TARGET = $$CNEXT_BIN_PATH/rtapi_app

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    rtapi_app.cpp

INCLUDEPATH += ../ \
    ../hal/rtapi \
    ../hal

LIBS += -L$$CNEXT_MODULES_PATH -lhal_mod -ldl

DEFINES += ULAPI
