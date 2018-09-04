#-------------------------------------------------
#
# Project created by QtCreator 2015-11-13T19:51:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4):
QT += widgets
QT+=xml
QT+=network

TARGET = Desktop
TEMPLATE = app

#CONFIG+=Release
SOURCES += main.cpp\
    backgroundWidget/backgroundwidget.cpp \
    rightWidget/rightwidget.cpp \
    tools/pushbutton.cpp \
    tools/label.cpp \
    keyboard/softkeyboard.cpp \
     keyboard/numberdialog.cpp \
    tools/lineedit.cpp \
    #tools/combobox.cpp \
    bottomWidget/bottonwidget.cpp \
    tools/tabwidget.cpp \
    backgroundWidget/floatdialog.cpp \
    tools/stackedwidget.cpp \
    tools/networkthread.cpp \
    rightWidget/yuntaidialog.cpp \
    rightWidget/yuntaisubdialog.cpp \
    rightWidget/shutdowndialog.cpp \
    rightWidget/videomanagerdialog.cpp \
    rightWidget/playbackdialog.cpp \
    rightWidget/sensordialog.cpp \
    rightWidget/controllerdialog.cpp \
    rightWidget/piclayoutdialog.cpp \
    rightWidget/warnoutdialog.cpp \
    tools/widgetpool.cpp \
    rightWidget/previewdialog.cpp \
    rightWidget/tipdialog.cpp \
    bottomWidget/plotter.cpp \
    rightWidget/edituserdialog.cpp \
    rightWidget/emailsetsubdialog.cpp \
    rightWidget/logsetsubdialog.cpp \
    rightWidget/memorydevicesetsubdialog.cpp \
    rightWidget/picmanagersubdialog.cpp \
    rightWidget/sensorgeneralsubdialog.cpp \
    rightWidget/sensorwarnsubdialog.cpp \
    rightWidget/videomanageraddeddialog.cpp \
    rightWidget/videomanagerfounddialog.cpp \
    rightWidget/yuntaicruisedialog.cpp \
    rightWidget/yuntaipresetdialog.cpp \
    tools/xmlparser.cpp \
    rightWidget/systemSetDialog.cpp \
    tools/connector.cpp \
    tools/proxy.cpp

HEADERS  +=  backgroundWidget/backgroundwidget.h \
    rightWidget/rightwidget.h \
    tools/pushbutton.h \
    tools/label.h \
    keyboard/softkeyboard.h \
    keyboard/numberdialog.h \
    tools/lineedit.h \
    bottomWidget/bottonwidget.h \
    tools/tabwidget.h \
    backgroundWidget/floatdialog.h \
    tools/stackedwidget.h \
    tools/networkthread.h\
    rightWidget/yuntaidialog.h \
    rightWidget/yuntaisubdialog.h \
    rightWidget/shutdowndialog.h \
    rightWidget/videomanagerdialog.h \
    rightWidget/playbackdialog.h \
    rightWidget/sensordialog.h \
    rightWidget/controllerdialog.h \
    rightWidget/piclayoutdialog.h \
    rightWidget/warnoutdialog.h \
    tools/widgetpool.h \
    rightWidget/previewdialog.h \
    rightWidget/tipdialog.h \
    bottomWidget/plotter.h \
    rightWidget/edituserdialog.h \
    rightWidget/emailsetsubdialog.h \
    rightWidget/logsetsubdialog.h \
    rightWidget/memorydevicesetsubdialog.h \
    rightWidget/picmanagersubdialog.h \
    rightWidget/sensorgeneralsubdialog.h \
    rightWidget/sensorwarnsubdialog.h \
    rightWidget/videomanageraddeddialog.h \
    rightWidget/videomanagerfounddialog.h \
    rightWidget/yuntaicruisedialog.h \
    rightWidget/yuntaipresetdialog.h \
    tools/xmlparser.h \
    rightWidget/systemSetDialog.h \
    tools/connector.h \
    tools/proxy.h


FORMS    +=  backgroundWidget/backgroundwidget.ui \
    rightWidget/rightwidget.ui \
    keyboard/softkeyboard.ui \
    keyboard/numberdialog.ui \
    bottomWidget/bottonwidget.ui \
    rightWidget/yuntaidialog.ui \
    rightWidget/yuntaisubdialog.ui \
    rightWidget/shutdowndialog.ui \
    rightWidget/videomanagerdialog.ui \
    rightWidget/playbackdialog.ui \
    rightWidget/sensordialog.ui \
    rightWidget/controllerdialog.ui \
    rightWidget/piclayoutdialog.ui \
    rightWidget/warnoutdialog.ui \
    rightWidget/previewsetdialog.ui \
    rightWidget/tipdialog.ui \
    bottomWidget/plotter.ui \
    rightWidget/edituserdialog.ui \
    rightWidget/emailsetsubdialog.ui \
    rightWidget/logsetsubdialog.ui \
    rightWidget/memorydevicesetsubdialog.ui \
    rightWidget/picmanagersubdialog.ui \
    rightWidget/sensorgeneralsubdialog.ui \
    rightWidget/sensorwarnsubdialog.ui \
    rightWidget/videomanageraddeddialog.ui \
    rightWidget/videomanagerfounddialog.ui \
    rightWidget/yuntaicruisedialog.ui \
    rightWidget/yuntaipresetdialog.ui \
    rightWidget/systemSetDialog.ui \
    rightWidget/subdialog.ui



