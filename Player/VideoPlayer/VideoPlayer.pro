#-------------------------------------------------
#
# Project created by QtCreator 2012-12-31T15:02:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videoplayer.cpp

HEADERS  += mainwindow.h \
    videoplayer.h

FORMS    += mainwindow.ui
LIBS += -L/usr/local/lib -lavcodec -lavformat -lavdevice -lswscale -lavutil
LIBS+= -L/usr/lib -lSDL
