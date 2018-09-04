#include "mainwindow.h"
#include <QApplication>
#include "qtplayer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QtPlayer player;
    /// 初始化播放器
    int theErr=player.initPlayer();
    if(!theErr){
        return 0;
    }
    /// 启动播放器
    player.start();
    return a.exec();
}
