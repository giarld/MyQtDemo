#-------------------------------------------------
#
# Project created by QtCreator 2014-11-16T13:46:23
#
#-------------------------------------------------

QT       += core gui
QT      += opengl core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 16wu
TEMPLATE = app

LIBS    += -lglut -lGLU

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h
