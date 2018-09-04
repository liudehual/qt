#ifndef DECODETHREAD_H
#define DECODETHREAD_H

#include "OSThread.h"
#include "Module.h"
#include "OSMutex.h"
#include <queue>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavformat/version.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avio.h"
#include "libavutil/time.h"
#include "libavutil/log.h"
#include "libavutil/avutil.h"
#include "libavutil/error.h"
#include "libavutil/fifo.h"
#include "libavutil/file.h"
#include "libavutil/frame.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavutil/mem.h"
#include "libavutil/mathematics.h"
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/avutil.h"
#include "libavutil/channel_layout.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/fifo.h"
#include "libavutil/mathematics.h"
#include "libavutil/opt.h"
#include "libavutil/parseutils.h"
#include "libavutil/pixdesc.h"
#include "libavutil/pixfmt.h"
#include "libavutil/frame.h"
#include <limits.h>
}

#define FRAME_CACHE_LEN 512*1024

class H264Frame
{
public:
    H264Frame();
    ~H264Frame();
    int getValidDataLen();
    int getVaildData(char *&buf,int &len);
    int recountVaildData(int usedLen);
    int setVaildData(int validLen);
    char *getVaildData();
public:
    char data[FRAME_CACHE_LEN];
    int dataLen;
    int fValidDataLen;
    char *fValidData;
};

class VideoDecodeThread:public OSThread
{
public:
    ~VideoDecodeThread();

    static VideoDecodeThread *createNewVideoDecodeThread();

    static int lockmgrCallback(void **mutex, enum AVLockOp op);

    static int initOS();

    static int fillIoBuffer(void *opaque, uint8_t *buf, int buf_size);

    int initVideoDecodeThread(void *arg,videoFrameHandler handler);

    virtual void Entry();

    int killSelf();

    int addFrame(char *buf,int len);

    H264Frame * getFrame();

    int setWidthAndHeight(int w,int h);

    int setFrameRate(int rate);

    int setFileName(char *fileName);

public:
    H264Frame *fFrame;
protected:
    VideoDecodeThread();

private:

    void *fArg;

    videoFrameHandler fHandler;

    bool fLive;

    char fFileName[128];

    static OSMutex fMutex;

    uint8_t * bufferRGB;

    AVFrame *fRGBFrame;

    AVFrame *fDecodeFrame;

    AVFormatContext *fFormatCtx;

    AVCodec *pCodec;

    AVCodecContext *pCodecCtx;

    AVFrame	*pFrame;

    AVPacket packet;

    int fFrameRate;

    OSMutex fFramePoolMutex;
    std::queue<H264Frame *> fFramePool;

    OSMutex fAvailableFramePoolMutex;
    std::queue<H264Frame *> fAvailableFramePool;

    int fWidth;
    int fHeight;

};

#endif // DECODETHREAD_H
