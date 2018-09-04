#-------------------------------------------------
#
# Project created by QtCreator 2017-02-01T23:17:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FLVParser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fvlparserthread.cpp \
    flvtag.cpp

HEADERS  += mainwindow.h \
    fvlparserthread.h \
    flvtag.h

FORMS    += mainwindow.ui
