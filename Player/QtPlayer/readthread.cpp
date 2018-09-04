#include "readthread.h"
#include "audiothread.h"
#include "videothread.h"
#include "qtplayer.h"
#include "mainwindow.h"

#define MIN_PACKET_NUMBERS 25

#define INT64_MAX 0xEFFFFFFFFFFFFFFF
#define INT64_MIN 0xFFFFFFFFFFFFFFFF

ReadThread::ReadThread(void *qRoot,void *qParent,QObject *parent) :MetaData(qRoot,qParent),
    QThread(parent),
    fAudioThread(NULL),
    fVideoThread(NULL),
    aIndex(-1),
    vIndex(-1),
    seekReq(false),
    realPos(-1),
    fFormatCtx(NULL)
{
    //// 视频解码线程
    fVideoThread=new VideoThread(this->getRoot(),this);

    //// 音频解码线程
    fAudioThread=new AudioThread(this->getRoot(),this);

    //// 数据读取线程同步锁及条件变量
    fMutex=new QMutex;
    fCond=new QWaitCondition;
}
void ReadThread::run()
{
    QtPlayer *player=(QtPlayer *)this->getRoot();

    //// 进入循环读取
    for(;;){

        //// 快进或快退
        if(seekReq){
            do{

                av_log(NULL,AV_LOG_INFO,"######## %0.8f \n",realPos);

                int theErr= avformat_seek_file(fFormatCtx,-1,0,realPos,realPos,AVSEEK_FLAG_ANY);
                if(theErr<0){
                    char buf[1024]={0};
                    av_strerror(theErr,buf,1024);
                    av_log(NULL,AV_LOG_INFO,"avformat_seek_file failed buf %s\n",buf);
                    break;
                }

                /// 清空音频队列
                player->getAudioPacketQueue()->cleanPacketQueue();
                player->getAudioFrameQueue()->cleanFrameQueue();

                /// 清空视频队列
                player->getVideoPacketQueue()->cleanPacketQueue();
                player->getVideoFrameQueue()->cleanFrameQueue();


                avcodec_flush_buffers(fFormatCtx->streams[aIndex]->codec);
                avcodec_flush_buffers(fFormatCtx->streams[vIndex]->codec);
            }while(0);

            seekReq=false;
        }

        ////    读取线程一对多，其是否暂停不应受其他线程的影响，否则将难于控制
        ////    读取线程是否暂停取决于音频队列的长度是否超过最大限制
        if(player->getAudioPacketQueue()->getPacketQueueLen()>MIN_PACKET_NUMBERS
                && player->getVideoPacketQueue()->getPacketQueueLen()>MIN_PACKET_NUMBERS) {
            av_usleep(10*1000.0);
            continue;
        }

        //// 申请AVPacket内存
        AVPacket *packet=av_packet_alloc();
        av_init_packet(packet);

        //// 读取一包数据（音频含有多帧数据，视频包含一帧数据）
        int ret=av_read_frame(fFormatCtx,packet);
        if(ret<0){
            if ((ret == AVERROR_EOF || avio_feof(fFormatCtx->pb))) {
                /* do something */
                av_log(NULL,AV_LOG_INFO,"something error \n");
            }
            QMutexLocker theLocker(fMutex);
            fCond->wait(fMutex,10);
            continue;
        }

        ///av_log(NULL,AV_LOG_INFO,"read packet size %d \n",packet->size);

        //// 加入到音频队列
        if(aIndex==packet->stream_index){
            player->getAudioPacketQueue()->putPacketToQueue(packet);
            continue;
        }

        //// 加入到视频队列
        if(vIndex==packet->stream_index){

            player->getVideoPacketQueue()->putPacketToQueue(packet);
            continue;
        }
        //// 释放包数据
        av_packet_unref(packet);
        av_packet_free(&packet);
    }
}
void ReadThread::clean()
{

}

int ReadThread::initReadThread(QString fileName)
{

    qDebug()<<"ReadThread::initReadThread";

    //// 申请AVFormatContext 结构体内存
    fFormatCtx=avformat_alloc_context();

    //// 根据文件名打开文件（并获取文件大概信息）
    int theErr=avformat_open_input(&fFormatCtx,fileName.toStdString().c_str(),NULL,NULL);
    if(theErr<0){
        char errBuf[1024];
        av_strerror(theErr,errBuf,1024);
        av_log(NULL,AV_LOG_INFO,"avformat_find_stream_info error %d Resion\n",
               theErr,errBuf);
        return 0;
    }

    //// 获取文件详细信息
    theErr=avformat_find_stream_info(fFormatCtx,NULL);
    if(theErr<0){
        char errBuf[1024];
        av_strerror(theErr,errBuf,1024);
        av_log(NULL,AV_LOG_INFO,"avformat_find_stream_info error %d Resion\n",
               theErr,errBuf);
        return 0;
    }
    fMaxFrameDuration = (fFormatCtx->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;

    /// 获取音频索引
    qDebug()<<"nb_streams "<<fFormatCtx->nb_streams;
    for(int i=0;i<fFormatCtx->nb_streams;i++){
        if(fFormatCtx->streams[i]->codec->codec_type!=AVMEDIA_TYPE_AUDIO){
            continue;
        }
        qDebug()<<"Find Audio Type "<<i;
        aIndex=i;
        break;
    }

    //// 获取视频索引
    for(int i=0;i<fFormatCtx->nb_streams;i++){
        if(fFormatCtx->streams[i]->codec->codec_type!=AVMEDIA_TYPE_VIDEO){
            continue;
        }
        qDebug()<<"Find Video Type"<<i;
        vIndex=i;
        break;
    }

    //// 检查音频索引与视频索引是否合法
    if(aIndex<0 && vIndex<0){
        av_log(NULL,AV_LOG_INFO,"Not Find Audio or Video\n");
        return 0;
    }
    qDebug()<<aIndex<<" "<<vIndex;
    qDebug()<<fileName;

    //// 初始化视频解码线程
    if(vIndex>=0){
        AVCodecContext *vCodecCtx=fFormatCtx->streams[vIndex]->codec;
        AVCodec *vCodec=avcodec_find_decoder(vCodecCtx->codec_id);
        theErr=avcodec_open2(vCodecCtx,vCodec,NULL);
        if(theErr>=0){
            fVideoThread->init(vCodecCtx,vCodec);
        }
    }

    //// 初始化音频解码线程
    if(aIndex!=-1){
        AVCodecContext *aCodecCtx=fFormatCtx->streams[aIndex]->codec;
        AVCodec *aCodec=avcodec_find_decoder(aCodecCtx->codec_id);
        theErr=avcodec_open2(aCodecCtx,aCodec,NULL);
        if(theErr>=0){
            qDebug()<<"Open Audio CodecContex and Codec succeed";
            fAudioThread->init(aCodecCtx,aCodec);
        }
    }

    ////更新播放
#if 1
    if(fFormatCtx->duration != AV_NOPTS_VALUE){
        double secs, us;
        int64_t duration = fFormatCtx->duration + 5000;
        secs = duration / AV_TIME_BASE;
        us = duration % AV_TIME_BASE;
        av_log(NULL,AV_LOG_INFO,"secs %0.8f \n",secs+(100 * us) / AV_TIME_BASE);
        fTotaltime=secs+(100 * us) / AV_TIME_BASE;
    }
    QtPlayer *player=(QtPlayer *)this->getParent();
    MainWindow *mWindow=(MainWindow *)player->getMainWindow();
    mWindow->setSingleValue(fTotaltime/QSLIDER_LEN);
#endif

    av_log(NULL,AV_LOG_INFO,"Total Time %0.8f \n",fTotaltime);

    //// 启动数据读取线程
    this->start();

    return 1;
}

int ReadThread::seekPos(double pos)
{
    //int64_t step=fTotaltime/1000;
    realPos=pos;
    av_log(NULL,AV_LOG_INFO,"realPos %0.8f \n",realPos);
    seekReq=true;
}
