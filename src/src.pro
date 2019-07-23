QT += core \
    gui \
    network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fc2mppatcher
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += main.cpp \
    widget.cpp \
    pe.cpp \
    patcher.cpp

HEADERS += widget.h \
    constants.h \
    pe.h \
    patcher.h

FORMS += widget.ui

# Including 3rd party PeLib library.
INCLUDEPATH += $$PWD/../lib/pelib/include
DEPENDPATH += $$PWD/../lib/pelib/include

#LIBS += -L$$PWD/../lib/pelib/build/src/pelib -lpelib

SOURCES += $$PWD/../lib/pelib/src/pelib/BoundImportDirectory.cpp \
    $$PWD/../lib/pelib/src/pelib/CoffSymbolTable.cpp \
    $$PWD/../lib/pelib/src/pelib/ComHeaderDirectory.cpp \
    $$PWD/../lib/pelib/src/pelib/DebugDirectory.cpp \
    $$PWD/../lib/pelib/src/pelib/ExportDirectory.cpp \
    $$PWD/../lib/pelib/src/pelib/IatDirectory.cpp \
    $$PWD/../lib/pelib/src/pelib/InputBuffer.cpp \
    $$PWD/../lib/pelib/src/pelib/MzHeader.cpp \
    $$PWD/../lib/pelib/src/pelib/OutputBuffer.cpp \
    $$PWD/../lib/pelib/src/pelib/PeFile.cpp \
    $$PWD/../lib/pelib/src/pelib/PeHeader.cpp \
    $$PWD/../lib/pelib/src/pelib/PeLibAux.cpp \
    $$PWD/../lib/pelib/src/pelib/RelocationsDirectory.cpp \
    $$PWD/../lib/pelib/src/pelib/ResourceDirectory.cpp \
    $$PWD/../lib/pelib/src/pelib/RichHeader.cpp \
    $$PWD/../lib/pelib/src/pelib/SecurityDirectory.cpp

HEADERS += $$PWD/../lib/pelib/include/pelib/BoundImportDirectory.h \
    $$PWD/../lib/pelib/include/pelib/CoffSymbolTable.h \
    $$PWD/../lib/pelib/include/pelib/ComHeaderDirectory.h \
    $$PWD/../lib/pelib/include/pelib/DebugDirectory.h \
    $$PWD/../lib/pelib/include/pelib/DelayImportDirectory.h \
    $$PWD/../lib/pelib/include/pelib/ExportDirectory.h \
    $$PWD/../lib/pelib/include/pelib/IatDirectory.h \
    $$PWD/../lib/pelib/include/pelib/ImportDirectory.h \
    $$PWD/../lib/pelib/include/pelib/InputBuffer.h \
    $$PWD/../lib/pelib/include/pelib/MzHeader.h \
    $$PWD/../lib/pelib/include/pelib/OutputBuffer.h \
    $$PWD/../lib/pelib/include/pelib/PeFile.h \
    $$PWD/../lib/pelib/include/pelib/PeHeader.h \
    $$PWD/../lib/pelib/include/pelib/PeLib.h \
    $$PWD/../lib/pelib/include/pelib/PeLibAux.h \
    $$PWD/../lib/pelib/include/pelib/PeLibInc.h \
    $$PWD/../lib/pelib/include/pelib/RelocationsDirectory.h \
    $$PWD/../lib/pelib/include/pelib/ResourceDirectory.h \
    $$PWD/../lib/pelib/include/pelib/RichHeader.h \
    $$PWD/../lib/pelib/include/pelib/SecurityDirectory.h \
    $$PWD/../lib/pelib/include/pelib/TlsDirectory.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
