################################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
################################################################

include(../../realtime/hal/hal.pri)
include(../nml_intf/nml_intf.pri)
include(../../common.pri)

TARGET       = $$ECNC_BIN_PATH/curvetracker
TEMPLATE  = app

HEADERS = \
    curvetracker.h \
    plot.h \
    getdata.h

SOURCES = \
    curvetracker.cpp \
    plot.cpp \
    main.cpp \
    getdata.cpp

INCLUDEPATH += /usr/local/qwt-6.1.1/include
LIBS += -L/usr/local/qwt-6.1.1/lib -l qwt
LIBS +=  -L$$ECNC_MODULES_PATH   -lhal_mod

DEFINES += RTAPI
