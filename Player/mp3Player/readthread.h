#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include "metadata.h"
#include <QQueue>

class QWaitCondition;
class QMutex;
extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "SDL/SDL.h"
#include "libavutil/time.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
}
class SoundDecodeThread;
class ReadThread : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit ReadThread(void *pRoot,void *pParent,QObject *parent = 0);
    AVPacket getPacketQueue();
    void addPacketToQueue(AVPacket packet);
    int getPacketQueueLen(){return fPacketQueue.length();}
    virtual void run();
    AVCodecContext *getCodecContext(){return fCodecCtx;}
    SwrContext *getSwrContext(){return fSCtx;}
    void startThread();
    int init(QString fName);
private:
    QWaitCondition *fPacketCond;
    QMutex *fPacketMutex;
    SoundDecodeThread *fSoundDecodeThread;
    QQueue<AVPacket> fPacketQueue;
    AVPacket fFlushPkt;
    AVFormatContext *fFormatCtx;
    AVCodecContext *fCodecCtx;
    AVCodec        *fCodec;
    SwrContext *fSCtx;
};

#endif // READTHREAD_H
