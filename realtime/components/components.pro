CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

mytarget.target = .buildfile
mytarget.commands = touch $$mytarget.target
mytarget.depends = mytarget2
mytarget2.commands = @echo Building+++++++++++++++ $$mytarget.target
QMAKE_EXTRA_TARGETS += mytarget mytarget2

SOURCES += \
    main.c
