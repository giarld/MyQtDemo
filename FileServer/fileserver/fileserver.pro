#-------------------------------------------------
#
# Project created by QtCreator 2018-04-29T12:32:04
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = fileserver
TEMPLATE = lib

DEFINES += FILESERVER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        fileserver.cpp

HEADERS += \
        fileserver.h \
        fileserver_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#libglog
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../glog/release/ -lglog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../glog/debug/ -lglog
else:unix: LIBS += -L$$OUT_PWD/../glog/ -lglog

INCLUDEPATH += $$PWD/../glog
DEPENDPATH += $$PWD/../glog
