#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T10:42:56
#
#-------------------------------------------------

QT       -= core gui

TARGET = libtolua
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    tolua_event.c \
    tolua_is.c \
    tolua_map.c \
    tolua_push.c \
    tolua_to.c

HEADERS += \
    tolua.h \
    tolua_event.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../liblua/release/ -lliblua
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../liblua/debug/ -lliblua
else:unix: LIBS += -L$$OUT_PWD/../liblua/ -lliblua

INCLUDEPATH += $$PWD/../liblua
DEPENDPATH += $$PWD/../liblua

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../liblua/release/libliblua.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../liblua/debug/libliblua.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../liblua/release/liblua.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../liblua/debug/liblua.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../liblua/libliblua.a
