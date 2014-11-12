#-------------------------------------------------
#
# Project created by QtCreator 2014-10-29T17:43:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtadb
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    adb_socket.cpp \
    screenform.cpp

HEADERS  += mainwindow.h \
    adbsocket.h \
    screenform.h

FORMS    += mainwindow.ui \
    screenform.ui
