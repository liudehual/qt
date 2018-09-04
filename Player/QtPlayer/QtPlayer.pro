#-------------------------------------------------
#
# Project created by QtCreator 2017-03-19T09:52:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    metadata.cpp \
    qtplayer.cpp \
    readthread.cpp \
    videothread.cpp \
    audiothread.cpp \
    packetqueue.cpp \
    framequeue.cpp \
    videorefreshthread.cpp

HEADERS  += mainwindow.h \
    metadata.h \
    qtplayer.h \
    readthread.h \
    videothread.h \
    audiothread.h \
    global.h \
    packetqueue.h \
    framequeue.h \
    videorefreshthread.h

FORMS    += mainwindow.ui

LIBS +=-L/usr/local/lib -lavformat -lavcodec -lavutil -lavfilter -lavdevice -lswscale -lswresample -lSDL
