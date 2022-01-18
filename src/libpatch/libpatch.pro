QT -= gui

QT += network

TARGET = mppatch
TEMPLATE = lib
CONFIG += \
    c++latest \
    skip_target_version_ext

DEFINES += MPPATCH_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    cryptoutils.h \
    helper.h \
    mppatch.h \
    mppatch_global.h

SOURCES += mppatch.cpp \
    cryptoutils.cpp \
    helper.cpp

include(../common/common.pri)

# Argon2
INCLUDEPATH += $$PWD/../../lib/libargon2/include
DEPENDPATH += $$PWD/../../lib/libargon2/include

LIBS += -L$$PWD/../../lib/libargon2 -largon2

# Including needed Windows libraries.
LIBS += \
    -lws2_32 \
    -lIPHLPAPI
