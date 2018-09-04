#include "qtffmpeg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtFFmpeg w;
    w.show();

    return a.exec();
}
