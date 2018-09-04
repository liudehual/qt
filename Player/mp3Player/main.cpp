#include "widget.h"
#include <QApplication>
#include "player.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Player p;
    p.start();
    return a.exec();
}
