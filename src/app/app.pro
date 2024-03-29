QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fc2mppatcher
TEMPLATE = app
CONFIG += \
    c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    console.h \
    fileutils.h \
    installdir.h \
    patcher.h \
    pefile.h \
    widget.h

SOURCES += \
    console.cpp \
    fileutils.cpp \
    installdir.cpp \
    main.cpp \
    patcher.cpp \
    pefile.cpp \
    widget.cpp

FORMS += widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(../common/common.pri)

# Including 3rd party libraries.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/libpebliss/pe_lib/release/ -lpebliss
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/libpebliss/pe_lib/debug/ -lpebliss
else:unix: LIBS += -L$$OUT_PWD/../../lib/libpebliss/pe_lib/ -lpebliss

INCLUDEPATH += $$PWD/../../lib/libpebliss/pe_lib
DEPENDPATH += $$PWD/../../lib/libpebliss/pe_lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/libpebliss/pe_lib/release/libpebliss.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/libpebliss/pe_lib/debug/libpebliss.a
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../lib/libpebliss/pe_lib/libpebliss.a
