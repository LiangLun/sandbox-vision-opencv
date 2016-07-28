TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += edgesCPU.cpp

INCLUDEPATH += C:\opencv-buildCudaQt\install\include

LIBS += -LC:\opencv-buildCudaQt\install\x86\vc11\lib \
    -lopencv_core249d \
    -lopencv_highgui249d \
    -lopencv_imgproc249d
