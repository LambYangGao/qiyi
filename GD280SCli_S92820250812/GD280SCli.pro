QT       += core gui concurrent serialport
QT      += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GD280S

CONFIG += c++11
CONFIG += c++14

CONFIG += link_pkgconfig
DEFINES += _WIN32
#DEFINES += __MINGW32__
#DEFINES += UNICODE
#DEFINES += QT_NO_DEBUG_OUTPUT

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += OS_LINUX
#DEFINES += OS_KYLIN
#DEFINES += MULTICASTCOMM
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEFINES += SIMUCAMERA

SOURCES += \
    HidThread.cpp \
    centercom/CoordTranser.cpp \
    dlgAutoHide.cpp \
    fullscreenform.cpp \
    hidapi/usb_listener.cpp \
    main.cpp \
    mainwindow.cpp \
    mpeg/vidoeplayer.cpp \
    hidapi/HidApi.cpp \
    networkcomm.cpp \
    pod_cmd.cpp \


HEADERS += \
    HidThread.h \
    centercom/CommonDef.h \
    centercom/CoordTranser.h \
    dlgAutoHide.h \
    fullscreenform.h \
    gd280slensdata.h \
    hidapi/usb_listener.h \
    mainwindow.h \
    mpeg/vidoeplayer.h \
    hidapi/HidApi.h  \
    networkcomm.h \
    pod_cmd.h \


FORMS += \
    dlgAutoHide.ui \
    fullscreenform.ui \
    mainwindow.ui

TRANSLATIONS += \
    Hi3559UdpCli_en_AS.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/mpeg
INCLUDEPATH += $$PWD/mpeg/ffmpeg/include
INCLUDEPATH += $$PWD/hidapi
INCLUDEPATH += $$PWD/centercom


LIBS += -L$${PWD}/mpeg/ffmpeg/lib \
    -lavcodec \
    -lavutil \
    -lpostproc \
    -lswresample \
    -lswscale \
    -lavdevice \
    -lavfilter \
    -lavformat \

LIBS += -L$${PWD}/hidapi/lib \
    -lsetupapi

LIBS += -lDbgHelp

win32:{
#        QMAKE_LFLAGS_RELEASE += /MAP
#        QMAKE_CFLAGS_RELEASE += /Zi
#        QMAKE_LFLAGS_RELEASE += /debug /opt:ref
#        QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#        QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}
