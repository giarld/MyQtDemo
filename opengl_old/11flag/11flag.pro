#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T14:53:58
#
#-------------------------------------------------

QT       += core gui
QT      += opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 11flag
TEMPLATE = app

LIBS    += -lglut -lGLU

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h
