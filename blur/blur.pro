#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T12:56:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = blur
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    makeblurimage.cpp

HEADERS  += widget.h \
    makeblurimage.h

RESOURCES += \
    res.qrc
