#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T08:44:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

TARGET = my_NoteBook
TEMPLATE = app


SOURCES += main.cpp\
                          mainwindow.cpp \
                          background.cpp

HEADERS  += mainwindow.h \
    background.h

FORMS    += mainwindow.ui
