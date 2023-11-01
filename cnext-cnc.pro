TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
        realtime \
        libnml \
        inivar \
        emc \
#        tool



include(./common.pri)

hal_comps.commands = make -C $$_PRO_FILE_PWD_/realtime/components/ OBJECT_PATH=$$CNEXT_MODULES_PATH

clean_comps.commands = make clean -C $$_PRO_FILE_PWD_/realtime/components/ OBJECT_PATH=$$CNEXT_MODULES_PATH

QMAKE_EXTRA_TARGETS +=  hal_comps clean_comps

OTHER_FILES += config.h
