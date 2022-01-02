VERSION = 0.1.18
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/defs.h \
    $$PWD/entry.h \
    $$PWD/netutils.h \
    $$PWD/patch_defs.h

SOURCES += \
    $$PWD/netutils.cpp
