#-------------------------------------------------
#
# Project created by QtCreator 2016-11-09T21:33:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simplePlayer
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    decodethread.cpp

HEADERS  += dialog.h \
    decodethread.h

FORMS    += dialog.ui

LIBS += /root/QT/simplePlayer/extraLib/libuntil.a
LIBS += -lavcodec -lavformat -lavdevice -lavfilter -lswscale -lavutil # -lavutil 放在最后面


INCLUDEPATH += /root/QT/simplePlayer/extraInclude/
