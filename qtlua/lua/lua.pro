#-------------------------------------------------
#
# Project created by QtCreator 2017-01-22T11:30:59
#
#-------------------------------------------------

QT       -= core gui

TARGET = lua
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    lapi.c \
    lauxlib.c \
    lbaselib.c \
    lbitlib.c \
    lcode.c \
    lcorolib.c \
    lctype.c \
    ldblib.c \
    ldebug.c \
    ldo.c \
    ldump.c \
    lfunc.c \
    lgc.c \
    linit.c \
    liolib.c \
    llex.c \
    lmathlib.c \
    lmem.c \
    loadlib.c \
    lobject.c \
    lopcodes.c \
    loslib.c \
    lparser.c \
    lstate.c \
    lstring.c \
    lstrlib.c \
    ltable.c \
    ltablib.c \
    ltm.c \
    lua.c \
    lundump.c \
    lutf8lib.c \
    lvm.c \
    lzio.c

HEADERS += \
    lapi.h \
    lauxlib.h \
    lcode.h \
    lctype.h \
    ldebug.h \
    ldo.h \
    lfunc.h \
    lgc.h \
    llex.h \
    llimits.h \
    lmem.h \
    lobject.h \
    lopcodes.h \
    lparser.h \
    lprefix.h \
    lstate.h \
    lstring.h \
    ltable.h \
    ltm.h \
    lua.h \
    lua.hpp \
    luaconf.h \
    lualib.h \
    lundump.h \
    lvm.h \
    lzio.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
