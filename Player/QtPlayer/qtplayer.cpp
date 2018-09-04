#include "qtplayer.h"
#include "mainwindow.h"
#include "readthread.h"

QtPlayer::QtPlayer():MetaData(this,this),
    fMainWindow(NULL),
    fReadThread(NULL),
    fVideoFrameQueue(0),
    fAudioFrameQueue(1)
{
    initClock();
}
QtPlayer::~QtPlayer()
{
    SDL_CloseAudio();
}

void QtPlayer::start()
{
    fMainWindow->show();
}
int QtPlayer::initClock()
{
    /* 初始化音视频播放时长 */
    double time=av_gettime_relative()/1000000.0;
    fAudioClock.pts=NAN;
    /// 播放偏移
    fAudioClock.pts_drift=fAudioClock.pts-time;

    fVideoClock.pts=NAN;
    fVideoClock.pts_drift=fVideoClock.pts-time;
}

double QtPlayer::getClock(Clock *c)
{
    /// 获取当前时间
    double time=av_gettime_relative()/1000000.0;

    /// 当前时间加上偏移时间
    /// 注意：偏移时间可能为正，也可能为负
    return c->pts_drift+time;
}

int QtPlayer::setClock(Clock *c,double pts,double time)
{
    c->pts = pts;
    c->pts_drift = c->pts - time;
    return 1;
}

int QtPlayer::initPlayer()
{
    /// 初始化ffmpeg复用/解复用器，编码器/解码器等
    av_register_all();

    /// 初始化ffmpeg过滤器
    avfilter_register_all();

    /// 初始化ffmpeg设备
    avdevice_register_all();

    /// 初始化SDL
    int theFlags=SDL_INIT_AUDIO | SDL_INIT_VIDEO;
    int theErr=SDL_Init(theFlags);
    if(theErr<0){
        return 0;
    }

    /// 创建数据读取线程
    fReadThread=new ReadThread(this,this);

    /// 创建界面
    fMainWindow=new MainWindow(this,this);

    /// 设置视频界面的宽高
    setScreenWidthAndHeight(fMainWindow->getVideoLabelWidth(),fMainWindow->getVideoLabelHeight());
    av_log(NULL,AV_LOG_INFO,"Width %d Height %d\n",fMainWindow->getVideoLabelWidth(),fMainWindow->getVideoLabelHeight());

    return 1;
}
void QtPlayer::clean()
{

}
