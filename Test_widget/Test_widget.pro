#-------------------------------------------------
#
# Project created by QtCreator 2015-09-25T12:00:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test_widget
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mybutton.cpp

HEADERS  += widget.h \
    mybutton.h

#LIBS += F:\QT-5.5\5.5\msvc2010\lib\Qt5Cored.lib
LIBS += F:\QT-5.5\5.5\msvc2010\lib\Qt5Core.lib
FORMS    += widget.ui

RESOURCES += \
    my.qrc

