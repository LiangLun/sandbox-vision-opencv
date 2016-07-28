TARGET: RFClassifier
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
	RFClassifier.cpp

INCLUDEPATH += C:\\opencv-buildQt\\install\\include
LIBS += -LC:\\opencv-buildQt\\install\\x64\\mingw\\lib \
    -lopencv_core300 \
    -lopencv_highgui300 \
    -lopencv_ml300 

