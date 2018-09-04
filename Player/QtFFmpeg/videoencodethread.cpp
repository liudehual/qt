#include "videoencodethread.h"

VideoEncodeThread::VideoEncodeThread(QObject *parent) :
    QThread(parent)
{
}
VideoEncodeThread::VideoEncodeThread(void *parent,void *root):MetaData(parent,root)
{

}
void VideoEncodeThread::run()
{

}
