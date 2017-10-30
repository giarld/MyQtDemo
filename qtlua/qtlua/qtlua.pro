#-------------------------------------------------
#
# Project created by QtCreator 2017-01-22T11:37:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtlua
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    luaengine.cpp \
    ctest.cpp \
    aclass.cpp \
    bclass.cpp

HEADERS  += widget.h \
    luaengine.h \
    ctest.h \
    aclass.h \
    bclass.h

# liblua
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lua/release/ -llua
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lua/debug/ -llua
else:unix: LIBS += -L$$OUT_PWD/../lua/ -llua

INCLUDEPATH += $$PWD/../lua
DEPENDPATH += $$PWD/../lua

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lua/release/liblua.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lua/debug/liblua.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lua/release/lua.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lua/debug/lua.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../lua/liblua.a
