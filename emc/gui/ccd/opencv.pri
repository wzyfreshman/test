win32 {

    INCLUDEPATH += E:/opencv-2.4.3/vc9/include \
        E:/opencv-2.4.3/vc9/include/opencv

    LIBS += -LE:/opencv-2.4.3/vc9/lib \
         -lopencv_imgproc243 \
         -lopencv_highgui243 \
         -lopencv_core243

}


unix {

INCLUDEPATH += /usr/local/include \
                 /usr/local/include/opencv

LIBS +=  -L /usr/local/lib \
         -lopencv_imgproc -lopencv_highgui -lopencv_core

}

