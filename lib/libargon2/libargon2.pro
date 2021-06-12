QT -= \
    core \
    gui

TARGET = argon2
DESTDIR = $$PWD
TEMPLATE = lib
CONFIG += \
        c++20 \
        staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/src/argon2.c \
    $$PWD/src/core.c \
    $$PWD/src/blake2/blake2b.c \
    $$PWD/src/thread.c \
    $$PWD/src/encoding.c \
    $$PWD/src/ref.c

HEADERS += \
    $$PWD/include/argon2.h \
    $$PWD/src/core.h \
    $$PWD/src/blake2/blake2.h \
    $$PWD/src/blake2/blake2-impl.h \
    $$PWD/src/blake2/blamka-round-ref.h \
    $$PWD/src/thread.h \
    $$PWD/src/encoding.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
