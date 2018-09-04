#include "videothread.h"
#include "qtplayer.h"
#include "videorefreshthread.h"
#include "readthread.h"
VideoThread::VideoThread(void *qRoot,void *qParent,QObject *parent) :MetaData(qRoot,qParent),
    QThread(parent)
{
    //// 创建视频刷新线程
    fRefreshThread=new VideoRefreshThread(this->getRoot(),this);
}
void VideoThread::run()
{
    AVPacket *packet;
    QtPlayer *player=(QtPlayer *)this->getRoot();
    PacketQueue *videoPQ=player->getVideoPacketQueue();
    CustomFrameQueue  *videoFQ=player->getVideoFrameQueue();

    ReadThread *rThread=(ReadThread *)player->getReadThread();
    AVRational tb = rThread->getFormatContext()->streams[rThread->getVideoIndex()]->time_base;

    AVCodecContext *cCtx=this->getVideoContext();

    //// 根据 原视频宽/高/格式及目标视频宽/高/格式创建转换结构体 SwsContext
    SwsContext *convert_ctx =NULL;

    //// 根据屏幕宽高及色彩格式计算所需要的数据量
    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24,
                                      player->getScreenWidth(),
                                      player->getScreenHeight());

    //// 创建存放转换后数据的buffer
    uint8_t * bufferRGB = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
    AVFrame *frame=av_frame_alloc();

    for(;;){
        //// 从视频AVPacket队列中获取AVPacket
        int flags=videoPQ->getPacketFromQueue(packet);
        if(!flags){
            av_usleep(1000*10);
            continue;
        }

        int got_frame=0;

        //// 解码视频
        int ret=avcodec_decode_video2(fCtx,frame,&got_frame,packet);
        if(ret<=0){
            continue;
        }

        //// 计算数据包是否已经解析完毕
        if (packet->size>0) {
            packet->data += ret;
            packet->size -= ret;
            if(packet->size<=0){
                av_free_packet(packet);
            }
        }else{
            av_free_packet(packet);
            continue;
        }

        //// 获取帧显示时间戳（获取最佳时间戳）
        frame->pts=av_frame_get_best_effort_timestamp(frame);
        double pts=(double)(frame->pts * av_q2d(tb));
        if(got_frame){
            Frame *pFrameRGB=new Frame();
            pFrameRGB->frame=av_frame_alloc();

            //// 填充AVFrame（即初始化pFrameRGB 帧数据）
            /// 以RGB格式填充
            avpicture_fill((AVPicture *)pFrameRGB->frame,
                           bufferRGB,
                           AV_PIX_FMT_RGB24,
                           player->getScreenWidth(),
                           player->getScreenHeight());

            /// 初始化转换结构
            /// 该结构用于将解码后的帧数据
            /// 转换成其他格式的帧数据（如RGB/YUV格式等）
            convert_ctx = sws_getCachedContext(convert_ctx,
                                               cCtx->width,
                                               cCtx->height,
                                               cCtx->pix_fmt,
                                               player->getScreenWidth(),
                                               player->getScreenHeight(),
                                               AV_PIX_FMT_RGB24,
                                               SWS_BICUBIC,
                                               NULL,
                                               NULL,
                                               NULL);

            //// 开始转换
            ///  将解码后的帧数据转换成RGB格式的数据
            sws_scale(convert_ctx,
                      frame->data,
                      frame->linesize,
                      0,
                      frame->height,
                      pFrameRGB->frame->data,
                      pFrameRGB->frame->linesize);

            //// 设置新生成帧的显示时间戳
            pFrameRGB->pts = pts;

#if 0
            static double time_old=0;
            av_log(NULL,AV_LOG_INFO,"pFrameRGB->pts %0.8f %0.8f \n",
                   pFrameRGB->pts,
                   pts-time_old);
            time_old=pts;
#endif
            //// 将视频AVFrame放入到视频AVFrame队列中
            videoFQ->putFrameToQueue(pFrameRGB);
        }
    }
}

int VideoThread::init(AVCodecContext *ctx,AVCodec *codec)
{
    fCtx=ctx;
    fCodec=codec;

    //// 启动视频刷新线程
    fRefreshThread->start();
    //// 启动视频解码线程
    this->start();

    return 0;
}
