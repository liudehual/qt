#ifndef FRAMEQUEUE_H
#define FRAMEQUEUE_H


#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include "metadata.h"

/// 视频帧标志
#define VIDEO_FRAME 0

/// 音频帧标志
#define AUDIO_FRAME 1

/// 此处音频帧队列长度是视频帧队列长的3倍，是经验值？

/// 视频帧队列长度
#define VIDEO_FRAME_QUEUE_LEN 3

/// 音频帧队列长度
#define AUDIO_FRAME_QUEUE_LEN 9

/// 帧描述
struct Frame
{
    double pts;
    AVFrame *frame;
    double duration;
};
/// 自定义帧队列
class CustomFrameQueue
{
public:
    CustomFrameQueue(int f);
    ~CustomFrameQueue();

    /// 压入帧到队列
    int     putFrameToQueue(Frame *frame);

    /// 从队列获取一帧
    int     getFrameFromQueue(Frame  *&frame);

    /// 清除帧队列
    void    cleanFrameQueue();

    /// 抛弃一帧数据
    void     frameQueueNext();

    /// 获取帧队列长度
    int getFrameQueueLen();

public:
    QQueue<Frame *> fQueue;    /// 帧队列
    QMutex *            fMutex; /// 锁
    QWaitCondition *    fCond; /// 条件变量
    ///
    ///    帧类别标志
    ///    0 视频帧
    ///    1 音频帧
    ///
    int flags;
};
#endif // FRAMEQUEUE_H
