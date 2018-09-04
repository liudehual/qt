#-------------------------------------------------
#
# Project created by QtCreator 2015-09-28T13:24:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = My_EAD
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp\
        md5.c \
        proxy.cpp\
        base64.cpp

HEADERS  += widget.h\
                            md5.h \
                            proxy.h\
                            base64.h

FORMS    += widget.ui
