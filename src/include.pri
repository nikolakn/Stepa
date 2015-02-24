SOURCES += \
    src/main.cpp \
    src/sdl/nksdl.cpp \
    src/opengl/camera.cpp \
    src/models/nkgltext.cpp \
    src/opengl/opengl.cpp \
    src/models/nkhex2.cpp \
    src/sdl/nktimer.cpp \
    src/models/nkselecthexmodel.cpp \
    src/models/nkbleckrect.cpp \
    src/models/nkmaptexture.cpp \
    src/models/nkmapa.cpp \
    $$PWD/models/nkloadscreen.cpp \
    src/models/nkunitrender.cpp \
    src/game/nkunits.cpp \
    src/opengl/nkhexcamera.cpp

HEADERS += \
    src/sdl/nksdl.h \
    src/opengl/camera.h \
    src/models/nkgltext.h \
    src/opengl/opengl.h \
    src/models/nkhex2.h \
    src/models/nkmodel.h \
    src/include/framework.h \
    src/bandit/assertion_exception.h \
    src/bandit/bandit.h \
    src/bandit/context.h \
    src/bandit/grammar.h \
    src/bandit/listener.h \
    src/bandit/options.h \
    src/bandit/runner.h \
    src/bandit/test_run_error.h \
    src/sdl/nktimer.h \
    src/models/nkselecthexmodel.h \
    src/models/nkbleckrect.h \
    src/models/nkmaptexture.h \
    src/models/nkmapa.h \
    $$PWD/models/nkloadscreen.h \
    src/models/nkunitrender.h \
    src/game/nkunits.h \
    src/opengl/nkhexcamera.h

