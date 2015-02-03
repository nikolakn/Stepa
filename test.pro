TEMPLATE = app
CONFIG -= console
CONFIG += static
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += ./src/
 win32 {
    #CONFIG += opengl
    LIBS += -L/usr/local/lib -lSDL2 -lGLEW32 -lfreeimage -lfreetype -lopengl32
 }
 !win32 {
    INCLUDEPATH += /usr/include/freetype2/
    LIBS += -L/usr/local/lib -lSDL2 -lGLEW -lGL -lfreeimage -lfreetype
 }



SOURCES += \
    src/test.cpp \
    src/opengl/camera.cpp

HEADERS += \
    src/opengl/camera.h \
    src/opengl/testcamera.h
