TARGET: SVMClassifier
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
	SVMClassifier.cpp

INCLUDEPATH += C:\\opencv-buildQt\\install\\include
LIBS += -LC:\\opencv-buildQt\\install\\x64\\mingw\\lib \
    -lopencv_core300 \
    -lopencv_highgui300 \
    -lopencv_imgproc300 \
    -lopencv_ml300 \
    -lopencv_features2d300 \
    -lopencv_nonfree300



 
