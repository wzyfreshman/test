win32 {

    INCLUDEPATH += E:/camera-dev/include \

    LIBS += E:/camera-dev/lib/MVCAMSDK.lib

}

unix {

    INCLUDEPATH += /usr/local/mvux_camera_x86/include

    LIBS += -lMVSDK

    QMAKE_CXXFLAGS += -fPIC

}


