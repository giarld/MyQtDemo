TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

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
