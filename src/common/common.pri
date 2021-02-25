VERSION = 0.1.12
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/defs.h \
    $$PWD/entry.h \
    $$PWD/patch_defs.h
