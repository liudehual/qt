#-------------------------------------------------
#
# Project created by QtCreator 2017-03-11T10:20:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mp3Player
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    player.cpp \
    readthread.cpp \
    sounddecodethread.cpp \
    metadata.cpp

HEADERS  += widget.h \
    player.h \
    readthread.h \
    sounddecodethread.h \
    metadata.h

FORMS    += widget.ui

LIBS += -L/usr/local/lib -lavformat -lavcodec -lswscale -lavutil -lswresample -lSDL
