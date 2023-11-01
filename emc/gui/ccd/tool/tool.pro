TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt



INCLUDEPATH += /usr/local/include \
                 /usr/local/include/opencv \
                /usr/local/mvux_camera_x86/include

LIBS += -lMVSDK  -lpthread\
        -L /usr/local/lib \
         -lopencv_imgproc -lopencv_highgui -lopencv_core


SOURCES += main.cpp

