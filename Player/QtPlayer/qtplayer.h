#ifndef QTPLAYER_H
#define QTPLAYER_H
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include "metadata.h"
#include "framequeue.h"
#include "packetqueue.h"
class MainWindow;
class ReadThread;

/// 时钟描述
typedef struct Clock {
    ///显示时间戳
    double pts;
    ///记录偏移时间段
    /// 音频（解码转换完成和播放完成之间的时间差）
    /// 视频（）
    double pts_drift;
    double last_updated;
} Clock;

///
/// 播放器类
///
class QtPlayer:public MetaData
{
public:
    QtPlayer();
    virtual ~QtPlayer();

    /// 初始化播放类
    int initPlayer();

    /// 启动窗口
    void start();

    /// 初始化时钟
    int initClock();
    virtual void clean();
    int     getScreenWidth(){return fWidth;}
    int     getScreenHeight(){return fHeight;}
    void    setScreenWidthAndHeight(int w,int h){fWidth=w;fHeight=h;}

    PacketQueue *   getAudioPacketQueue(){return &fAudioPacketQueue;}
    CustomFrameQueue  *   getAudioFrameQueue(){return &fAudioFrameQueue;}
    PacketQueue *   getVideoPacketQueue(){return &fVideoPacketQueue;}
    CustomFrameQueue  *   getVideoFrameQueue(){return &fVideoFrameQueue;}

    ReadThread *getReadThread(){return fReadThread;}
    MainWindow *getMainWindow(){return fMainWindow;}

    Clock *getAudioClock(){return &fAudioClock;}
    Clock *getVideoClock(){return &fVideoClock;}

    double getClock(Clock *c);
    int setClock(Clock *c,double pts,double time);
private:

    /// 主窗口
    MainWindow *fMainWindow;

    /// 数据读取线程
    ReadThread *fReadThread;

    /// 音频包队列
    PacketQueue fAudioPacketQueue;

    /// 数据包队列
    PacketQueue fVideoPacketQueue;

    /// 音频帧队列
    CustomFrameQueue  fAudioFrameQueue;

    /// 视频帧队列
    CustomFrameQueue  fVideoFrameQueue;

    /// 音频时钟
    Clock fAudioClock;

    /// 视频时钟
    Clock fVideoClock;

    /// 视频宽高
    int         fWidth;
    int         fHeight;
};

#endif // QTPLAYER_H
