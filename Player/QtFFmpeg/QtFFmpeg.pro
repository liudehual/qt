#-------------------------------------------------
#
# Project created by QtCreator 2017-06-10T23:55:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtFFmpeg
TEMPLATE = app


SOURCES += main.cpp\
        qtffmpeg.cpp \
    core.cpp \
    metadata.cpp \
    readthread.cpp \
    videodecodethread.cpp \
    videoencodethread.cpp \
    audiodecodethread.cpp \
    audioencodethread.cpp

HEADERS  += qtffmpeg.h \
    global.h \
    core.h \
    metadata.h \
    readthread.h \
    videodecodethread.h \
    videoencodethread.h \
    audiodecodethread.h \
    audioencodethread.h

FORMS    += qtffmpeg.ui
LIBS +=-L/usr/local/ -lavcodec -lavformat -lavfilter -lavutil
