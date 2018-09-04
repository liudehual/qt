#include "readthread.h"

ReadThread::ReadThread(QObject *parent) :
    QThread(parent)
{
}
ReadThread::ReadThread(void *parent,void *root):
    MetaData(parent,root),
    fVideoDecodeThread(NULL),
    fAudioDecodeThread(NULL)
{

}

void ReadThread::run()
{

}
