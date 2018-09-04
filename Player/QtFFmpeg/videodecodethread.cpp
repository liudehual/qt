#include "videodecodethread.h"

VideoDecodeThread::VideoDecodeThread(QObject *parent) :
    QThread(parent)
{
}
VideoDecodeThread::VideoDecodeThread(void *parent,void *root):
    MetaData(parent,root),
    fVideoEncodeThread(NULL)
{

}
void VideoDecodeThread::run()
{

}
