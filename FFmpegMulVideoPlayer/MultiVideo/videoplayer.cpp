#include "videoplayer.h"
#include <QImage>
#include <QDebug>

VideoPlayer *VideoPlayer::createNewVideoPlayer()
{
    return new VideoPlayer();
}

VideoPlayer::~VideoPlayer()
{

}

VideoPlayer::VideoPlayer():fVideoLabel(NULL)
{
    fVideoDecodeModule=Module::createNewModule();
    fReadThread=ReadThread::createNewReadThread();
}

int VideoPlayer::initVideoPlayer(QLabel *label)
{
    fVideoLabel=label;

    /// 初始化读取线程
    fReadThread->initReadThread(fVideoDecodeModule);

    /// 启动读取线程
    fReadThread->start();

    QThread::sleep(3);

    /// 初始化解码模块
    fVideoDecodeModule->initModule(this,VideoPlayer::playCallback);
}

int VideoPlayer::setFileName(char *fileName)
{
    fReadThread->setFileName(fileName);
    return 1;
}

void VideoPlayer::playCallback(void *arg,char *buf,int width,int height)
{
    if(!width || !height){
        return;
    }
    if(!buf){
        return;
    }

    /// 显示
    VideoPlayer *thePlayer=(VideoPlayer *)arg;

    QImage theImage((uchar *)buf,width,height,QImage::Format_RGB888);

    thePlayer->getVideoLabel()->setPixmap(QPixmap::fromImage(theImage.scaled(thePlayer->getVideoLabel()->size(), Qt::KeepAspectRatio)));
    ///qDebug()<<"===============";
    ///thePlayer->getVideoLabel()->show();
    QWidget *theWidget=(QWidget *)(thePlayer->getVideoLabel()->parent());
    theWidget->show();
}

int VideoPlayer::startPlayer()
{

    /// 启动解码线程
    fVideoDecodeModule->startModule();
    return 1;
}

QLabel *VideoPlayer::getVideoLabel()
{
    return fVideoLabel;
}
