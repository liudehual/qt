#include "readthread.h"
#include "sounddecodethread.h"
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
extern "C"
{
#include "libavutil/time.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avio.h"
#include "libavformat/avformat.h"
}
ReadThread::ReadThread(void *pRoot,void *pParent,QObject *parent) :
    QThread(parent),
    MetaData(pRoot,pParent),
    fCodecCtx(NULL),
    fSCtx(NULL)
{
    av_register_all();

    fSoundDecodeThread=new SoundDecodeThread(this->getRoot(),this);
    fPacketCond=new QWaitCondition;
    fPacketMutex=new QMutex;
    av_init_packet(&fFlushPkt);
}
void ReadThread::run()
{
    QThread::usleep(1000*1000);

    static AVPacket packet;
    av_init_packet(&packet);
    for(;;){
        av_init_packet(&packet);
        int ret=av_read_frame(fFormatCtx,&packet);
        if(ret<0){
            if ((ret == AVERROR_EOF || avio_feof(fFormatCtx->pb))) {
                addPacketToQueue(fFlushPkt);
            }
            fPacketCond->wait(fPacketMutex,1000*10);
            continue;
        }
        addPacketToQueue(packet);
    }
}



AVPacket ReadThread::getPacketQueue()
{

    QMutexLocker theLocker(fPacketMutex);
    if(fPacketQueue.length()==0){
        return fFlushPkt;
    }
    AVPacket packet=fPacketQueue.dequeue();
    fPacketCond->wakeOne();

    static int num=0;
    //av_log(NULL,AV_LOG_INFO,"%d %d \n",packet.size,num++);

    return packet;
}

void ReadThread::addPacketToQueue(AVPacket packet)
{
    QMutexLocker theLocker(fPacketMutex);
#if 1
    if(fPacketQueue.length()>20){
      //  av_log(NULL,AV_LOG_INFO,"ReadThread is waiting \n");
        fPacketCond->wait(fPacketMutex);
    }
#endif
    fPacketQueue.enqueue(packet);
}
void ReadThread::startThread()
{
    this->start();
    fSoundDecodeThread->start();
}
int ReadThread::init(QString fName)
{
    if(!fFormatCtx){
          avformat_free_context(fFormatCtx);
    }
    fFormatCtx=avformat_alloc_context();

    int theErr=avformat_open_input(&fFormatCtx,fName.toStdString().c_str(),NULL,NULL);
    if(theErr<0){
        return -1;
    }
    if(fFormatCtx->nb_streams>=AVMEDIA_TYPE_NB){
        return -1;
    }
    av_log(NULL,AV_LOG_INFO,"nb_streams %d\n",fFormatCtx->nb_streams);
    theErr=avformat_find_stream_info(fFormatCtx,NULL);
    if(theErr<0){
        return -1;
    }
    int aIndex=-1;
    for(int i=0;i<fFormatCtx->nb_streams;i++){
        if(fFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
            aIndex=i;
            break;
        }
    }
    if(aIndex==-1){
        return -1;
    }
    fCodecCtx=fFormatCtx->streams[aIndex]->codec;
    fCodec=avcodec_find_decoder(fCodecCtx->codec_id);
    theErr=avcodec_open2(fCodecCtx,fCodec,NULL);
    if(theErr<0){
        return -1;
    }
    av_log(NULL,AV_LOG_INFO,"ReadThread::init succeed \n");
    fSCtx=swr_alloc();
    int64_t in_channel_layout=av_get_default_channel_layout(fCodecCtx->channels);
    fSCtx=swr_alloc_set_opts(fSCtx,
                             fCodecCtx->channel_layout,
                             AV_SAMPLE_FMT_S16,
                             fCodecCtx->sample_rate,
                             in_channel_layout,
                             fCodecCtx->sample_fmt ,
                             fCodecCtx->sample_rate,
                             0,
                             NULL);
    swr_init(fSCtx);

    fSoundDecodeThread->init();
    return 0;
}
