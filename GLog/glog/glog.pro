#-------------------------------------------------
#
# Project created by QtCreator 2018-04-29T12:00:15
#
#-------------------------------------------------

include(glog.pri)

TARGET = glog
TEMPLATE = lib

unix {
    target.path = /usr/lib
    INSTALLS += target
}
