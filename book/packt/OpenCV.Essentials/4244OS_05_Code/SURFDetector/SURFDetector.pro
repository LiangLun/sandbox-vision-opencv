TARGET: SURFDetector
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
	SURFDetector.cpp

INCLUDEPATH += "c:\\Program Files (x86)\\opencv-master\\buildQT\\install\\include"
LIBS += -L"c:\\Program Files (x86)\\opencv-master\\buildQT\\install\\x64\\mingw\\lib" \
    -lopencv_core300 \
    -lopencv_highgui300 \
    -lopencv_imgproc300 \
    -lopencv_nonfree300 


