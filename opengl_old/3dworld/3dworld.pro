#-------------------------------------------------
#
# Project created by QtCreator 2014-11-14T19:44:23
#
#-------------------------------------------------

QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3dworld
TEMPLATE = app

LIBS += -lglut -lGLU

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h
