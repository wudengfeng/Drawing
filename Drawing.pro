QT += widgets opengl

LIBS += -lwinmm \
        -lopengl32 \
        -lglu32 \
        -lglut32 \
        -lgdi32

SOURCES += main.cpp \
    window.cpp

HEADERS += \
    window.h

RESOURCES += \
    resources.qrc
