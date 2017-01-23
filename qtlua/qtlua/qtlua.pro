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
        widget.cpp

HEADERS  += widget.h

# liblua
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

# libtolua
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libtolua/release/ -llibtolua
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libtolua/debug/ -llibtolua
else:unix: LIBS += -L$$OUT_PWD/../libtolua/ -llibtolua

INCLUDEPATH += $$PWD/../libtolua
DEPENDPATH += $$PWD/../libtolua

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libtolua/release/liblibtolua.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libtolua/debug/liblibtolua.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libtolua/release/libtolua.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libtolua/debug/libtolua.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libtolua/liblibtolua.a
