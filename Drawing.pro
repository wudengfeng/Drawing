QT += widgets
QT += opengl

LIBS += -lwinmm \
        -lopengl32 \
        -lglu32 \
        -lglut32 \
        -lgdi32



SOURCES += main.cpp \
    window.cpp \
    mainwindow.cpp

HEADERS += \
    window.h \
    mainwindow.h

RESOURCES += \
    resources.qrc
