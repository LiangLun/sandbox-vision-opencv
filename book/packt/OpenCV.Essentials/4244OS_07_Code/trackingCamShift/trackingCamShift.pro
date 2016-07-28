//trackingCamShift.pro
QT       += core
QT       -= gui

TARGET = trackingCamShift
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += C:\\opencv-buildQt\\install\\include

LIBS += -LC:\\opencv-buildQt\\install\\x64\mingw\\lib \
        -lopencv_core300 \
        -lopencv_highgui300 \
        -lopencv_imgproc300 \
        -lopencv_features2d300 \
        -lopencv_video300 \
        -lopencv_calib3d300 \



SOURCES += ./trackingCamShift.cpp


