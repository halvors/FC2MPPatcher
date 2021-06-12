VERSION = 0.1.15
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/defs.h \
    $$PWD/entry.h \
    $$PWD/patch_defs.h \
    $$PWD/utils.h

SOURCES += \
    $$PWD/utils.cpp
