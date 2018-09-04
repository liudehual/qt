#include <QApplication>
#include "mainwindow.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("System"); //��ȡϵͳ����
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);


    MainWindow w;
    w.show();
    
    return a.exec();
}
