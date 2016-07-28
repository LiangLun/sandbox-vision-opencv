TARGET: Filter_Retina
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Filter_Retina.cpp
INCLUDEPATH += "c:\\opencv-buildQt\\install\\include"
LIBS += -L"c:\\opencv-buildQt\\install\\x64\mingw\\lib" \
    -lopencv_core249 \
    -lopencv_highgui249 \
    -lopencv_imgproc249 \
    -lopencv_contrib249
