TARGET: connectedComponents
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    connectedComponents.cpp
	
# Note: Check your local paths to proper compilation
	
INCLUDEPATH += "C:\\opencv-buildQt\\install\\include"
LIBS += -L"C:\\opencv-buildQt\\install\\x64\\mingw\\lib" \
    -lopencv_core300 \
    -lopencv_highgui300 \
    -lopencv_imgproc300

