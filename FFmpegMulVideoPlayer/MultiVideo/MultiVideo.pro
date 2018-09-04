#-------------------------------------------------
#
# Project created by QtCreator 2018-02-28T17:19:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MultiVideo
TEMPLATE = app

INCLUDEPATH+=-I/root/MultiVideo/include
LIBS+=-L/root/MultiVideo/lib -lDecodeModule
LIBS +=-L/usr/local/lib -lavformat -lavcodec -lavutil -lavfilter -lavdevice -lswscale -lswresample

SOURCES += main.cpp\
        mainwindow.cpp \
    videoplayer.cpp \
    readthread.cpp

HEADERS  += mainwindow.h \
    videoplayer.h \
    readthread.h

FORMS    += mainwindow.ui


