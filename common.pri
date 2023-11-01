INCLUDEPATH +=$$PWD
DEPENDPATH += $$PWD

CNEXT_INSTALL_PATH = $$PWD/cnc-install
CNEXT_GUI_PATH =$$PWD/cnc-install/.cnc/gui
CNEXT_LIB_PATH = $$PWD/cnc-install/.cnc/lib
CNEXT_BIN_PATH = $$PWD/cnc-install/.cnc/bin
CNEXT_MODULES_PATH = $$PWD/cnc-install/.cnc/modules

QMAKE_CFLAGS += -fPIC
QMAKE_CXXFLAGS += -fPIC  -std=gnu++11 -g

CONFIG += USE_PREEMPT
