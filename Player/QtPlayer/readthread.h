#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include <QString>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include "metadata.h"

class AudioThread;
class VideoThread;

class ReadThread : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit ReadThread(void *qRoot,void *qParent,QObject *parent = 0);

    /// 线程执行体
    void run();

    ///清除函数
    virtual void clean();

    /// 初始化数据读取线程
    int initReadThread(QString fileName);

    /// 获取音频解码线程
    AudioThread *getAudioThread(){return fAudioThread;}

    /// 获取视频解码线程
    VideoThread *getVideoThread(){return fVideoThread;}

    /// 获取视频解码器索引
    int getVideoIndex(){return vIndex;}

    /// 获取音频解码器索引
    int getAudioIndex(){return aIndex;}

    /// 获取格式上下文环境
    AVFormatContext *getFormatContext(){return fFormatCtx;}

    /// 获取最大帧播放时间
    double getFrameMaxDuration(){return fMaxFrameDuration;}

    /// 定位播放位置
    int seekPos(double pos);
signals:

public slots:

private:
    /// 音频解码线程
    AudioThread     *fAudioThread;

    /// 视频解码线程
    VideoThread     *fVideoThread;

    /// 文件格式上下文
    AVFormatContext *fFormatCtx;

    QMutex *fMutex;
    QWaitCondition *fCond;

    /// 音频流索引
    int aIndex;

    /// 视频流索引
    int vIndex;

    /// 播放总时间
    double fTotaltime;

    /// 帧最大持续时间
    double fMaxFrameDuration;

    /// 播放重定位
    bool seekReq;

    /// 当前播放位置
    double realPos;
};

#endif // READTHREAD_H
