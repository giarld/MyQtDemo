#-------------------------------------------------
#
# Project created by QtCreator 2014-11-12T17:31:07
#
#-------------------------------------------------

QT       += core gui
QT      += opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = texture
TEMPLATE = app

LIBS += -lglut -lGLU

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    glwidget.h
