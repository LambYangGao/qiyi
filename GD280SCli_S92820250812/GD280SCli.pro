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
    ctlloopthd.cpp \
    dlgAutoHide.cpp \
    fullscreenform.cpp \
    hidapi/usb_listener.cpp \
    main.cpp \
    mainwindow.cpp \
    mpeg/vidoeplayer.cpp \
    hidapi/HidApi.cpp \
    networkcomm.cpp \
    pid/pid.cpp \
    pod_cmd.cpp \
    tracker/centroidtracker.cpp \
    tracker/dlcmtracker.cpp \
    tracker/fhog.cpp \
    tracker/kcftracker.cpp \
    tracker/tophattracker.cpp \
    trackerthread.cpp \

HEADERS += \
    HidThread.h \
    centercom/CommonDef.h \
    centercom/CoordTranser.h \
    ctlloopthd.h \
    dlgAutoHide.h \
    fullscreenform.h \
    gd280slensdata.h \
    hidapi/usb_listener.h \
    mainwindow.h \
    mpeg/vidoeplayer.h \
    hidapi/HidApi.h  \
    networkcomm.h \
    pod_cmd.h \
    Theo_PP_DLL/include/theo_pp_dll.h \
    #CommonDef.h \
    #CoordTranser.h \
    pid/pid.h \
    tracker/centroidtracker.h \
    tracker/dlcmtracker.h \
    tracker/kcftracker.h \
    tracker/tophattracker.h \
    trackerthread.h \
    detector/cdetector.h \
    detector/cdetector_global.h \
    # pid2/pid_ex5.h \
    pid3/pid_ex5.h

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
INCLUDEPATH += $$PWD/tracker
INCLUDEPATH += $$PWD/pid
INCLUDEPATH += $$PWD/Theo_PP_DLL/include
INCLUDEPATH += $$PWD/centercom
INCLUDEPATH += $$PWD/lamb/Include

#INCLUDEPATH += E:\OpenCV\MinGW32_version\OpenCV-MinGW-Build-OpenCV-3.4.1\include
#LIBS += E:\OpenCV\MinGW32_version\OpenCV-MinGW-Build-OpenCV-3.4.1\bin\libopencv_*.dll
#INCLUDEPATH += F:\OpenCV\MinGW32_OpenCV\OpenCV-MinGW-Build-OpenCV-4.5.0-with-contrib\include
#LIBS += F:\OpenCV\MinGW32_OpenCV\OpenCV-MinGW-Build-OpenCV-4.5.0-with-contrib\x64\mingw\bin\libopencv_*.dll
INCLUDEPATH += $$PWD/opencv/include
LIBS += $$PWD/opencv/bin/libopencv_*.dll

#LIBS += $$PWD/mpeg/ffmpeg/lib/avcodec.lib\
#        $$PWD/mpeg/ffmpeg/lib/avdevice.lib\
#        $$PWD/mpeg/ffmpeg/lib/avfilter.lib\
#        $$PWD/mpeg/ffmpeg/lib/avformat.lib\
#        $$PWD/mpeg/ffmpeg/lib/avutil.lib\
#        $$PWD/mpeg/ffmpeg/lib/postproc.lib\
#        $$PWD/mpeg/ffmpeg/lib/swresample.lib\
#        $$PWD/mpeg/ffmpeg/lib/swscale.lib\

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

LIBS += -L$${PWD}/Theo_PP_DLL/lib \
    -lTheo_PP_DLL

LIBS += -L$${PWD}/detector \
     -lCDetector

# LIBS += -L$${PWD}/pid2 \
#     -lPID_EX5
LIBS += -L$${PWD}/pid3 \
    -lPID_EX5
# LIBS += L:\A.WStar\7.work\June\gy\GD280SCli_S92820240315\GD280SCli_S928\pid2\pid_ex5.o
# LIBS += L:/A.WStar/7.work/June/gy/GD280SCli_S92820240315/GD280SCli_S928/pid/pid_ex3.o
# OBJECTS += $$quote(L:/A.WStar/7.work/June/gy/GD280SCli_S92820240315/GD280SCli_S928/pid/pid_ex3.o)

LIBS += -L$${PWD}/lamb/libcomponents.a




LIBS += -lDbgHelp

win32:{
#        QMAKE_LFLAGS_RELEASE += /MAP
#        QMAKE_CFLAGS_RELEASE += /Zi
#        QMAKE_LFLAGS_RELEASE += /debug /opt:ref
#        QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#        QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}
