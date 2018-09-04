#include "sounddecodethread.h"
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>
#include "readthread.h"
#include <QDebug>

AVPacket SoundDecodeThread::fPacket;
AVFrame SoundDecodeThread::frame;


SoundDecodeThread::SoundDecodeThread(void *pRoot,void *pParent,QObject *parent) :
    QThread(parent),
    MetaData(pRoot,pParent),
    fAudioVolume(128)
{
    fFrameCond=new QWaitCondition;
    fFrameMutex=new QMutex;
}
void SoundDecodeThread::run()
{

    QThread::usleep(2*1000*1000);
    ReadThread *rThread=(ReadThread *)getParent();
    AVCodecContext *cCtx=rThread->getCodecContext();

    int64_t rTime=0;

    for(;;){
        if(!rTime && fPacket.dts>0){
            rTime=fPacket.dts;
        }

        fPacket=rThread->getPacketQueue();
        while(1){
            int getPic=0;
            int len1=0;
            len1=avcodec_decode_audio4(cCtx,&frame,&getPic,&fPacket);
            if(len1<=0){
                break;
            }
            if(getPic){
                addFrameToQueue(frame);
            }
#if 1
            if (fPacket.data || fPacket.size>0){
                fPacket.data+=len1;
                fPacket.size-=len1;
                continue;
            }else{
                if(fPacket.data){
                    av_free_packet(&fPacket);
                }
                break;
            }
#endif
        }
        if(rTime<fPacket.dts){
            if(fPacket.dts-rTime>10*1000){
                int64_t sleepTime=(fPacket.dts-rTime)/1000.0;
                av_usleep(sleepTime);
                rTime+=fPacket.dts-rTime;
            }else{
                QThread::usleep(1000*10);
                rTime+=10;
            }
        }
    }
}
void SoundDecodeThread::wakeUp()
{

}
AVFrame SoundDecodeThread::getFrameQueue()
{
    QMutexLocker theLocker(fFrameMutex);
    if(fFrameQueue.length()==0){
        AVFrame tFrame;
        return tFrame;
    }
    AVFrame frame=fFrameQueue.dequeue();
    fFrameCond->wakeOne();
    return frame;
}

void SoundDecodeThread::addFrameToQueue(AVFrame frame)
{
    QMutexLocker theLocker(fFrameMutex);
    if(fFrameQueue.length()>20){
        fFrameCond->wait(fFrameMutex);
    }
    fFrameQueue.enqueue(frame);
}
void SoundDecodeThread::sdl_callBack(void *opaque,unsigned char *stream,int len)
{
    SoundDecodeThread *st=(SoundDecodeThread *)opaque;
    static unsigned char audio_buf[(19200/3)*2]={0};
    static unsigned char slice_buf[1024]={0};
    static int audio_buf_size=0;
    static int audio_buf_index=0;
    int len1=0;
    AVFrame frame;
    int audio_size=0;
    while(len>0){
        if (audio_buf_index >= audio_buf_size) {
            if(st->getFrameQueueLen()==0){
                memcpy(audio_buf,slice_buf,1024);
                audio_buf_size=1024;
            }else{
                frame=st->getFrameQueue();
                audio_size=frame.linesize[0];
                memcpy(audio_buf,frame.data[0],audio_size);
                audio_buf_size = audio_size;
            }
            audio_buf_index = 0;
        }
        len1 = audio_buf_size - audio_buf_index;
        if (len1 > len){
            len1 = len;
        }

        SDL_MixAudio(stream, (unsigned char *)audio_buf + audio_buf_index, len1,st->getVolume());
        len -= len1;
        stream += len1;
        audio_buf_index += len1;
    }
}
int SoundDecodeThread::init()
{
    ReadThread *rThread=(ReadThread *)getParent();
    AVCodecContext *cCtx=rThread->getCodecContext();
    if (cCtx == NULL){
        return -1;
    }
    SDL_LockAudio();
    SDL_AudioSpec spec;
    SDL_AudioSpec wanted_spec;
    wanted_spec.freq = cCtx->sample_rate; /* 音频数据的采样率。常用的有48000,44100等 */
    wanted_spec.format = AUDIO_S16SYS; /* 音频格式 */
    wanted_spec.channels = 1;		/* 通道数linux下默认为1，其他值会出现莫名其妙的错误 */
    wanted_spec.silence = 0;   /* 设置 0 为静音 */
    wanted_spec.samples = 4608;//SDL_AUDIO_BUFFER_SIZE; /* 设置SDL音频缓冲区大小 */
    wanted_spec.callback = SoundDecodeThread::sdl_callBack;  /* 设置回调函数 */
    wanted_spec.userdata = this;  /* 回调函数第一个参数 */
    if(SDL_OpenAudio(&wanted_spec, &spec) < 0){
        fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
        SDL_UnlockAudio();
        return -1;
    }
    SDL_UnlockAudio();

    av_log(NULL,AV_LOG_INFO,"Open SDL \n");
    SDL_PauseAudio(0);

    return 0;
}
