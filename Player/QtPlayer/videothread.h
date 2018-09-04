#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <QQueue>

#include "metadata.h"

class VideoRefreshThread;

/// 视频解码线程
class VideoThread : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit VideoThread(void *qRoot,void *qParent,QObject *parent = 0);

    /// 线程体函数
    void run();

    /// 初始化视频解码线程
    int init(AVCodecContext *ctx,AVCodec *codec);

    /// 获取视频编解码上下文环境
    AVCodecContext *getVideoContext(){return fCtx;}

    /// 获取视频编解码器
    AVCodec *getVideoCodec(){return fCodec;}

    /// 获取视频刷新线程
    VideoRefreshThread *getVideoRefreshThread(){return fRefreshThread;}

private:

    /// 视频解码器上下文
    AVCodecContext *fCtx;

    /// 视频你解码器
    AVCodec *       fCodec;

    /// 视频刷新线程
    VideoRefreshThread *fRefreshThread;
};

#endif // VIDEOTHREAD_H
