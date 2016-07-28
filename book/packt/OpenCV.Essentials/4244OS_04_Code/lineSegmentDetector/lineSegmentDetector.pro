TARGET: lineSegmentDetector
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    lineSegmentDetector.cpp
	
INCLUDEPATH += "C:\\opencv-buildQt\\install\\include"
LIBS += -L"C:\\opencv-buildQt\\install\\x64\\mingw\\lib" \
    -lopencv_core300 \
    -lopencv_highgui300 \
    -lopencv_imgproc300

