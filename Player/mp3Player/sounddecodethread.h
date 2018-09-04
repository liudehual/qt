#ifndef SOUNDDECODETHREAD_H
#define SOUNDDECODETHREAD_H

#include <QThread>
#include "metadata.h"
#include <QQueue>

#include <QQueue>

class QWaitCondition;
class QMutex;

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/time.h"
#include "SDL/SDL.h"
#include "libavutil/time.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
}
class SoundDecodeThread : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit SoundDecodeThread(void *pRoot,void *pParent,QObject *parent = 0);
    void run();
    AVFrame getFrameQueue();
    int getFrameQueueLen(){return fFrameQueue.length();}
    void addFrameToQueue(AVFrame frame);
    void wakeUp();
    int init();
    int getVolume(){return fAudioVolume;}
    static void sdl_callBack(void *opaque,unsigned char *stream,int buf_size);
private:
    QWaitCondition *fFrameCond;
    QMutex *fFrameMutex;
    QQueue<AVFrame> fFrameQueue;
    int fAudioVolume;
    static AVPacket fPacket;
    static AVFrame frame;
};

#endif // SOUNDDECODETHREAD_H
