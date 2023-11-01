INCLUDEPATH += $$PWD \
    $$PWD/rtapi \

DEPENDPATH += $$PWD \
    $$PWD/rtapi \

USE_XENOMAI {

    DEFINES += __USE_PREEMPT__

    QMAKE_LFLAGS += $(shell /usr/xenomai/bin/xeno-config --skin=native --ldflags)
    QMAKE_CXXFLAGS += $(shell /usr/xenomai/bin/xeno-config --skin=native --cflags)
    QMAKE_CFLAGS += $(shell /usr/xenomai/bin/xeno-config --skin=native --cflags)

    INCLUDEPATH += \
        /usr/xenomai/include

    LIBS += -L/usr/xenomai/lib -lnative -lxenomai

}

#USE_PREEMPT {

#QMAKE_CFLAGS += -I/usr/xenomai/include/cobalt -I/usr/xenomai/include -D_GNU_SOURCE -D_REENTRANT -fasynchronous-unwind-tables -D__COBALT__ -D__COBALT_WRAP__ -I/usr/xenomai/include/alchemy
#QMAKE_LFLAGS += -Wl,--no-as-needed -Wl,@/usr/xenomai/lib/cobalt.wrappers -Wl,@/usr/xenomai/lib/modechk.wrappers -lalchemy -lcopperplate /usr/xenomai/lib/xenomai/bootstrap.o -Wl,--wrap=main -Wl,--dynamic-list=/usr/xenomai/lib/dynlist.ld -L/usr/xenomai/lib -lcobalt -lmodechk -lpthread -lrt
DEFINES += __USE_PREEMPT__
DEFINES += USE_GCC_ATOMIC_OPS
LIBS += -lpthread

#}
