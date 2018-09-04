#include "videorefreshthread.h"
#include "qtplayer.h"
#include "videothread.h"
#include "audiothread.h"
#include "readthread.h"
#include "mainwindow.h"

/// 最小同步域
#define AV_SYNC_THRESHOLD_MIN 0.04

/// 最大同步域
#define AV_SYNC_THRESHOLD_MAX 0.1

#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1

VideoRefreshThread::VideoRefreshThread(void *qRoot,void *qParent,QObject *parent) :
    QThread(parent),
    MetaData(qRoot,qParent)
{
}
double VideoRefreshThread::compute_target_delay(double delay)
{
    QtPlayer *player=(QtPlayer *)this->getRoot();
    ReadThread *rThread=(ReadThread *)player->getReadThread();
    double sync_threshold;
    double diff = 0;

    /// 计算视频播放已播放时长与音频视频已播放时长
    double a_clock=player->getClock(player->getAudioClock());
    double v_clock=player->getClock(player->getVideoClock());

    /// 当前视频时钟减去音频时钟
    diff =  v_clock-a_clock;

    /// 计算同步域
    sync_threshold = FFMAX(AV_SYNC_THRESHOLD_MIN, FFMIN(AV_SYNC_THRESHOLD_MAX, delay));

#if 0
    av_log(NULL,AV_LOG_INFO,"delay %0.8f a_clock %0.8f v_clock %0.8f diff %0.8f sync_threshold %0.8f ",
           delay,
           a_clock,
           v_clock,
           diff,
           sync_threshold);
#endif
    if (!isnan(diff) && fabs(diff) < rThread->getFrameMaxDuration()) {
        if (diff <= -sync_threshold){  /// 小于同步域 ，计算最小延时（最小延时必须大于等于0）
            delay = FFMAX(0, delay + diff);
        }else if (diff >= sync_threshold && delay > AV_SYNC_FRAMEDUP_THRESHOLD){ /// 大于同步与，且大于最大允许的同步域
            delay = delay + diff;
        }else if (diff >= sync_threshold){ /// 大于同步域，双倍延时
            delay = 2 * delay;
        }
    }
#if 0
    av_log(NULL,AV_LOG_INFO,"delay %0.8f\n",delay);
#endif
    return delay;
}
void VideoRefreshThread::run()
{
    /// 获取帧队列
    QtPlayer *player=(QtPlayer *)this->getRoot();
    CustomFrameQueue  *videoFQ=player->getVideoFrameQueue();

    /// 获取视频线程
    VideoThread *vThread=(VideoThread *)this->getParent();

    ReadThread *rThread=(ReadThread *)player->getReadThread();
    AudioThread *aThread=(AudioThread *)rThread->getAudioThread();

    /// 猜测一下帧速率
    AVRational frame_rate = av_guess_frame_rate(rThread->getFormatContext(),rThread->getFormatContext()->streams[rThread->getVideoIndex()], NULL);

    /// 计算帧持续时间
    /// 根据我们猜测得到的帧速率来计算
    double duration;
    duration = (frame_rate.num && frame_rate.den ? av_q2d((AVRational){frame_rate.den, frame_rate.num}) : 0);

    Frame *curFrame=NULL;
    Frame *preFrame=NULL;
    fFrameTimer = av_gettime_relative()/1000000.0;
    double remaining_time = 0.0;

    for(;;){
        /// 从AVFrame队列中获取AVFrame
#if 0
        int queueLen=videoFQ->getFrameQueueLen();
        av_log(NULL,AV_LOG_INFO,"Queue Len %d\n",queueLen);
#endif
        int flags=videoFQ->getFrameFromQueue(curFrame);
        if(!flags){
            ///av_log(NULL,AV_LOG_INFO,"Not Find Frame \n");
            av_usleep(10*1000.0);
            continue;
        }
        ///
        if(preFrame==NULL){
            preFrame=curFrame;
            continue;
        }

        ///  av_log(NULL,AV_LOG_INFO,"================= >> %0.8f \n",curFrame->pts);
        ///  av_log(NULL,AV_LOG_INFO,"\n\n");


AGAIN:
        if (remaining_time > 0.0){
            av_usleep((int64_t)(remaining_time * 1000000.0));
        }
        remaining_time = 0.01;

        ///   计算上一帧数据的播放持续时间
        double targetDuration=curFrame->pts-preFrame->pts;

        /// 检查上一帧数据的播放持续时间是否合理
        /// 大于0.1或小于等于0均为不合理状态
        /// 处于不合理状态时，采用默认的帧播放持续时间
        if(isnan(targetDuration)
                || targetDuration<=0
                || targetDuration>0.1){
            targetDuration=duration;
        }

        ////    计算延时
        double delay=compute_target_delay(targetDuration);

        ///   获取当前时间
        double time = av_gettime_relative() / 1000000.0;

        ///    当前时间小于当前帧播放时间，继续循环
        ///    fFrameTimer + delay 等于 当前帧播放时刻
        if (time < fFrameTimer + delay) {
            remaining_time = FFMIN(fFrameTimer + delay - time, remaining_time);
#if 0
            av_log(NULL,AV_LOG_INFO,"remaining_time %0.8f delay %0.8f\n",
                   remaining_time,
                   delay);
#endif

            goto AGAIN;
        }

        /// 记录当前帧播放时间
        fFrameTimer += delay;

        ///  当前时间大于当前帧播放时间且大于最大同步域
        ///  （移动游标是会产生作用）
        if (delay > 0 && time - fFrameTimer > AV_SYNC_THRESHOLD_MAX){
            fFrameTimer = time;
        }

        time = av_gettime_relative() / 1000000.0;

        ///    设置视频已播放时长
        player->setClock(player->getVideoClock(),curFrame->pts,time);

        /// 根据目标视频宽高格式生成QImage对象
        QImage *tmpImage=new QImage((uchar *)curFrame->frame->data[0],player->getScreenWidth(),player->getScreenHeight(),QImage::Format_RGB888);

        /// 发送
        emit sendImage(tmpImage);
        av_frame_free(&preFrame->frame);
        delete preFrame;
        preFrame=curFrame;
#if 0
        static double time_old=0;
        double time_new=av_gettime_relative() / 1000000.0;
        if(time_new-time_old>0.043){
            av_log(NULL,AV_LOG_INFO,"----> delay %0.8f \n",time_new-time_old);

        }
        time_old=time_new;
#endif
    }
}
