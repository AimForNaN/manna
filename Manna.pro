TEMPLATE = app
CONFIG += console
QT += core

#RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH =

# Default rules for deployment.
#include(deployment.pri)

OTHER_FILES +=

SOURCES += main.cpp \
    src/mongoose.c \
    src/library.cpp \
    src/api.cpp \
    src/server.cpp

HEADERS += \
    src/mongoose.h \
    src/library.h \
    src/api.h \
    src/server.h

unix {
    target.path = /usr/lib
    INSTALLS += target

    INCLUDEPATH += \
        /usr/local/include/sword \
        /usr/include/sword

    LIBS += \
        -L/usr/local/lib \
        -lsword
}

RESOURCES += \
    resources.qrc
