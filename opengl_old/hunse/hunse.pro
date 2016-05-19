#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T20:13:21
#
#-------------------------------------------------

QT       += core gui
QT  += opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hunse
TEMPLATE = app

LIBS += -lglut -lGLU

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h
