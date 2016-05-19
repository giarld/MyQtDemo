#-------------------------------------------------
#
# Project created by QtCreator 2014-11-16T14:31:21
#
#-------------------------------------------------

QT       += core gui
QT      += opengl core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 17imagetext
TEMPLATE = app

LIBS    += -lGLU -lglut

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h
