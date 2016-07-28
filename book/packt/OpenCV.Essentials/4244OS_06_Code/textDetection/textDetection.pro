TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += C:\\opencv-3.0\\build\\install\\include

LIBS += -LC:\\opencv-buildQt\\install\\x64\\mingw\\lib \
	-lopencv_core300 \
	-lopencv_highgui300 \
	-lopencv_imgproc300 \
	-lopencv_objdetect300
