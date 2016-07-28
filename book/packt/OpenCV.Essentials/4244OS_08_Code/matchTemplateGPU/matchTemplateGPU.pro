TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += matchTemplateGPU.cpp

INCLUDEPATH += C:\opencv-buildCudaQt\install\include

LIBS += -LC:\opencv-buildCudaQt\install\x86\vc11\lib \
    -lopencv_core249d \
    -lopencv_highgui249d \
    -lopencv_features2d249d \
    -lopencv_nonfree249d \
    -lopencv_gpu249d
