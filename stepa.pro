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
include(src/include.pri)


OTHER_FILES += \
    LICENSE \
    data/shaders/text.f.glsl \
    data/shaders/text.v.glsl \
    data/font/FreeSans.ttf \
    README.md \
    doc/potsetnik.org \
    data/shaders/shaderhex2.frag \
    data/shaders/shaderhex2.vert \
    data/textures/hextex.png \
    data/textures/hexselect.png



