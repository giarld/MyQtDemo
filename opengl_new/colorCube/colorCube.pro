#-------------------------------------------------
#
# Project created by QtCreator 2016-12-23T13:11:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = colorCube
TEMPLATE = app


SOURCES += main.cpp \
    glwindow.cpp

HEADERS  += \
    glwindow.h

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle-wrapper.jar \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle-wrapper.properties \
    android/.DS_Store \
    android/gradle/.DS_Store \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

