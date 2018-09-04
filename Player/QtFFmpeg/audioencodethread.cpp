#include "audioencodethread.h"

AudioEncodeThread::AudioEncodeThread(QObject *parent) :
    QThread(parent)
{
}
AudioEncodeThread::AudioEncodeThread(void *parent,void *root):MetaData(parent,root)
{

}
void AudioEncodeThread::run()
{

}
