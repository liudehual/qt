#include "audiodecodethread.h"

AudioDecodeThread::AudioDecodeThread(QObject *parent) :
    QThread(parent)
{
}
AudioDecodeThread::AudioDecodeThread(void *parent,void *root):
    MetaData(parent,root),
    fAudioEncodeThread(NULL)
{

}
void AudioDecodeThread::run()
{

}
