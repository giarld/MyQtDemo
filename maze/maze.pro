#-------------------------------------------------
#
# Project created by QtCreator 2015-12-04T14:52:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = maze
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    paintwidget.cpp \
    editdialog.cpp

HEADERS  += widget.h \
    paintwidget.h \
    editdialog.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/res/drawable/ico.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
