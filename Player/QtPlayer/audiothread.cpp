#include "audiothread.h"
#include "qtplayer.h"

#include "framequeue.h"
#include "mainwindow.h"

/// 解码器最大帧限制
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 10*1024*1024

/// 最小SDK音频缓冲区
#define SDL_AUDIO_MIN_BUFFER_SIZE 1024

/// 为防止音频解码线程频繁调用回调函数
#define SDL_AUDIO_MAX_CALLBACKS_PER_SEC 30

/// 静音缓冲区长度
#define SLICE_BUF_SIZE 512

AudioThread::AudioThread(void *qRoot,void *qParent,QObject *parent):
    MetaData(qRoot,qParent),
    swr_ctx(NULL),
    fVolume(128),
    audio_buf(NULL),
    audio_buf_size(0),
    audio_buf1(NULL),
    audio_buf1_size(0)

{
    audio_buf1=new uint8_t[AVCODEC_MAX_AUDIO_FRAME_SIZE];
}

AudioThread::~AudioThread()
{

}

void AudioThread::run()
{
    QtPlayer *player=(QtPlayer *)this->getRoot();

    ///从音频包队列中读取数据
    for(;;){
        AVPacket *packet;

        /// 从音频包队列中获取数据包
        int theFlags=player->getAudioPacketQueue()->getPacketFromQueue(packet);
        if(!theFlags){
            av_usleep(1000*10);
            continue;
        }

        /// 进入循环解码（音频一包数据中可能含有多帧数据）
        for(;;){
            int got_frame=0;
            Frame *aFrame=new Frame;
            aFrame->frame=av_frame_alloc();

            ///
            /// 解码
            ///
            int ret = avcodec_decode_audio4(fCtx, aFrame->frame, &got_frame,packet);
            if (ret <0) {
                break;
            }

            /// 解码失败，删除当前帧
            if(ret ==0){
                delete aFrame;
                av_frame_free(&(aFrame->frame));
                continue;
            }

            ///解码成功
            if (got_frame) {
                AVRational tb = (AVRational){1, aFrame->frame->sample_rate};
                if (aFrame->frame->pts != AV_NOPTS_VALUE){
                    aFrame->frame->pts = av_rescale_q(aFrame->frame->pts, fCtx->time_base, tb);
                }else if (aFrame->frame->pkt_pts != AV_NOPTS_VALUE){
                    aFrame->frame->pts = av_rescale_q(aFrame->frame->pkt_pts, av_codec_get_pkt_timebase(fCtx), tb);
                }

                /// 当前帧的显示时间戳
                aFrame->pts=(aFrame->frame->pts == AV_NOPTS_VALUE) ? NAN : (double)(aFrame->frame->pts * av_q2d(tb));

                /// 将解码后的音频AVFrame数据放入AVFrame队列中
                player->getAudioFrameQueue()->putFrameToQueue(aFrame);
            }

            /// 计算数据包是否已经解析完毕
            ///
            if (packet->size>0) {
                packet->data += ret;
                packet->size -= ret;
                if(packet->size<=0){
                    av_free_packet(packet);
                    break;
                }
            }else{
                av_free_packet(packet);
                break;
            }

        }
    }
}

#define SAMPLE_AGAIN 1

///SDL 音频解码线程
void AudioThread::sdl_audio_callback(void *opaque, Uint8 *stream, int len)
{
    AudioThread *st=(AudioThread *)opaque;
    static unsigned char audio_buf[(192000*3)/2]={0};
    static unsigned char slice_buf[SLICE_BUF_SIZE]={0};
    static int audio_buf_size=0;

    ///记录音频缓冲中已播放的位置
    static int audio_buf_index=0;
    int len1=0;
    int audio_size=0;

    /// 获取当前时间
    double audio_callback_time = av_gettime_relative();
    while(len>0){
        /// 音频缓冲区已播放索引是否大于等于音频缓冲大小
        /// 大于则重新获取一帧数据
        /// 小于则继续播放
        if (audio_buf_index >= audio_buf_size) {
            audio_size=st->audio_decode_frame((void *)st);
            if(audio_size<=0){
                memcpy(audio_buf,slice_buf,SLICE_BUF_SIZE);
                audio_buf_size=SLICE_BUF_SIZE;
            }else{
                /*  */
                memset(audio_buf,0,sizeof(audio_buf));
                memcpy(audio_buf,st->getAudioBuf(),audio_size);
                audio_buf_size = audio_size;
            }
            audio_buf_index = 0;
        }
        ///计算可用播放缓冲区长度
        len1 = audio_buf_size - audio_buf_index;

        ///如果大于SDL所要求的，则用SDL需要的长度
        if (len1 > len){
            len1 = len;
        }
        ///播放音频
        SDL_MixAudio(stream, (unsigned char *)audio_buf + audio_buf_index, len1,st->getVolume());
        len -= len1;///减小SDL播放流仍需播放的剩余长度
        stream += len1; ///增大SDL播放流位置
        audio_buf_index += len1;/// 增大已播放缓冲区位置
    }

    ///计算播放时间偏移
    QtPlayer *player=(QtPlayer *)st->getRoot();
    player->setClock(player->getAudioClock(),st->audio_clock,audio_callback_time/1000000.0);

}
int AudioThread::initSDLThread()
{
    ///可能的通道数
    static const int next_nb_channels[] = {0, 0, 1, 6, 2, 6, 4, 6};

    ///可能的采样率
    static const int next_sample_rates[] = {0, 44100, 48000, 96000, 192000};

    int next_sample_rate_idx = FF_ARRAY_ELEMS(next_sample_rates) - 1;
    const char *env=NULL;

    ///获取通道布局
    int64_t wanted_channel_layout=fCtx->channel_layout;

    ///获取通道数
    int wanted_nb_channels=fCtx->channels;
    SDL_LockAudio();

    /// 获取硬件支持的音频通道数
    env = SDL_getenv("SDL_AUDIO_CHANNELS");
    if (env) {
        wanted_nb_channels = atoi(env);
        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
    }
    ///获取期望的通道布局
    if (!wanted_channel_layout ///通道布局为0
            || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout) ///当前通道数与根据通道布局获取的通道数不同
            ) {
        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
        wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
    }

    ///根据通道布局获取期望的通道数
    wanted_nb_channels = av_get_channel_layout_nb_channels(wanted_channel_layout);

#if 1
        av_log(NULL,AV_LOG_INFO,"wanted_nb_channels %d "
               "fCtx->sample_rate %d "
               "fCtx->sample_rate %d \n",
               wanted_nb_channels,
               fCtx->sample_rate,
               fCtx->sample_rate);
#endif

    /// 设置我们期望的音频参数

    /// 采样率
    fWantedSpec.freq = fCtx->sample_rate;

    /// 通道数
    fWantedSpec.channels =wanted_nb_channels;/// wanted_nb_channels;

    /// 检查采样率和通道数是否合法
    if (fWantedSpec.freq <= 0
            || fWantedSpec.channels <= 0) {
        av_log(NULL, AV_LOG_ERROR, "Invalid sample rate or channel count!\n");
        SDL_UnlockAudio();
        return -1;
    }
    ///
    while (next_sample_rate_idx
           && next_sample_rates[next_sample_rate_idx] >= fWantedSpec.freq){
        next_sample_rate_idx--;
    }
    /// 音频数据格式
    fWantedSpec.format = AUDIO_S16SYS;

    /// 静音值
    fWantedSpec.silence = 0;

    /// 音频缓冲区大小
    fWantedSpec.samples = FFMAX(SDL_AUDIO_MIN_BUFFER_SIZE, 2 << av_log2(fWantedSpec.freq / SDL_AUDIO_MAX_CALLBACKS_PER_SEC));


    /// 回调函数
    fWantedSpec.callback = sdl_audio_callback;

    /// 用户数据
    fWantedSpec.userdata = this;

    ///    打开SDL音频功能
    ///    [in]  参数1: 我们期望的音频参数
    ///    [out] 参数2: 硬件实际支持的参数
    while (SDL_OpenAudio(&fWantedSpec, &fHardwareSpec) < 0) {
        fWantedSpec.channels = next_nb_channels[FFMIN(7, fWantedSpec.channels)];
        if (!fWantedSpec.channels) {
            fWantedSpec.freq = next_sample_rates[next_sample_rate_idx--];
            fWantedSpec.channels = wanted_nb_channels;
            if (!fWantedSpec.freq) {
                SDL_UnlockAudio();
                return -1;
            }
        }
        wanted_channel_layout = av_get_default_channel_layout(fWantedSpec.channels);
    }

    /// 检查硬件支持数据格式
    if (fHardwareSpec.format != AUDIO_S16SYS) {
        SDL_UnlockAudio();
        return -1;
    }

    /// 期望的通道数与硬件支持的通道数是否相同
    if (fHardwareSpec.channels != fWantedSpec.channels) {
        wanted_channel_layout = av_get_default_channel_layout(fHardwareSpec.channels);
        if (!wanted_channel_layout) {
            SDL_UnlockAudio();
            return -1;
        }
    }

    ///    音频目标参数
    ///    根据硬件支持的参数,设置合适的音频参数
    audio_tgt.fmt = AV_SAMPLE_FMT_S16;
    audio_tgt.freq = fHardwareSpec.freq;
    audio_tgt.channel_layout = wanted_channel_layout;
    audio_tgt.channels =  fHardwareSpec.channels;
    audio_tgt.frame_size = av_samples_get_buffer_size(NULL, audio_tgt.channels, 1, audio_tgt.fmt, 1);
    audio_tgt.bytes_per_sec = av_samples_get_buffer_size(NULL, audio_tgt.channels, audio_tgt.freq, audio_tgt.fmt, 1);
    if (audio_tgt.bytes_per_sec <= 0
            || audio_tgt.frame_size <= 0) {
        SDL_UnlockAudio();
        return -1;
    }

    audio_src=audio_tgt;
    SDL_UnlockAudio();

    /// 启动SDL音频写入线程
    SDL_PauseAudio(0);

    return 0;
}

int AudioThread::init(AVCodecContext *ctx,AVCodec *codec)
{
    fCtx=ctx;
    fCodec=codec;

    initSDLThread();

    this->start();

    return 0;
}
int AudioThread::audio_decode_frame(void *is)
{
    int data_size, resampled_data_size;
    int64_t dec_channel_layout;
    int wanted_nb_samples;
    QtPlayer *player=(QtPlayer *)this->getRoot();

    CustomFrameQueue *audioFrameQueue=player->getAudioFrameQueue();
    Frame *aFrame;

    /// AVFrame队列中获取AVFrame
    int theErr=audioFrameQueue->getFrameFromQueue(aFrame);
    if(!theErr){
        return 0;
    }

    /// 获取数据大小
    data_size = av_samples_get_buffer_size(NULL,
                                           av_frame_get_channels(aFrame->frame),
                                           aFrame->frame->nb_samples,
                                           (AVSampleFormat)aFrame->frame->format,
                                           1);

    /// 获取通道布局
    dec_channel_layout =
            (aFrame->frame->channel_layout && av_frame_get_channels(aFrame->frame) == av_get_channel_layout_nb_channels(aFrame->frame->channel_layout)) ?
                aFrame->frame->channel_layout : av_get_default_channel_layout(av_frame_get_channels(aFrame->frame));

    /// 获取该AVFrame中的采样数
    wanted_nb_samples =aFrame->frame->nb_samples;

    ///   检查当前AVFrame与目标格式是否相同，
    ///   不相同则需要进行音频转换（一般情况下只检查一次，用于将AVFrame格式转换成硬件可以接受的格式（这里指SDL））
    if (          aFrame->frame->format               != audio_src.fmt
                  || dec_channel_layout       != audio_src.channel_layout
                  || aFrame->frame->sample_rate       != audio_src.freq
                  || (wanted_nb_samples       != aFrame->frame->nb_samples && !swr_ctx)){

        swr_free(&swr_ctx);

        /// 根据目标格式与当前AVFrame格式创建SwrContext结构体
        swr_ctx = swr_alloc_set_opts(NULL,
                                     audio_tgt.channel_layout,
                                     (AVSampleFormat)audio_tgt.fmt,
                                     audio_tgt.freq,
                                     dec_channel_layout,
                                     (AVSampleFormat)aFrame->frame->format,
                                     aFrame->frame->sample_rate,
                                     (AVSampleFormat)0,
                                     NULL);
        /// 初始化swrContext
        if (!swr_ctx || swr_init(swr_ctx) < 0) {
            swr_free(&swr_ctx);
            return -1;
        }

        /// 同步一下，防止再次初始化swrContext
        audio_src.channel_layout = dec_channel_layout;
        audio_src.channels       = fCtx->channels;
        audio_src.freq           = fCtx->sample_rate;
        audio_src.fmt            = (AVSampleFormat)fCtx->sample_fmt;
    }
    if (swr_ctx){
        int len2;

        /// 计算输出数据大小
        int out_count = (int64_t)wanted_nb_samples * audio_tgt.freq / aFrame->frame->sample_rate + 256;

        /// 开始转换
        len2 = swr_convert(swr_ctx,
                           &audio_buf1,
                           out_count,
                           (const uint8_t **)(aFrame->frame->extended_data),
                           aFrame->frame->nb_samples);
        if (len2 < 0) {
            return 0;
        }
#if 1
        if (len2 == out_count) {
            if (swr_init(swr_ctx) < 0){
                swr_free(&swr_ctx);
            }
        }
#endif
        /// 向外传出buffer及buffer大小
        audio_buf = audio_buf1;
        resampled_data_size = len2 * audio_tgt.channels * av_get_bytes_per_sample(audio_tgt.fmt);
    } else {
        /// 向外传出buffer及buffer大小
        audio_buf = aFrame->frame->data[0];
        resampled_data_size = data_size;
    }

    /// 调整一下已播放时长
    audio_clock = (double)aFrame->pts + (double)aFrame->frame->nb_samples /aFrame->frame->sample_rate;

    /// 根据已播放时长，计算进度条的位置
    MainWindow *mWindow=player->getMainWindow();
    int pos=audio_clock/mWindow->getSingleValue();
    mWindow->setSliderPos(pos);

    ///    av_log(NULL,AV_LOG_INFO,"audio_clock %0.8f \n",audio_clock);
    ///    av_log(NULL,AV_LOG_INFO,"frame->pts %0.8f ",frame->pts);
    ///    av_log(NULL,AV_LOG_INFO,"(double)frame->nb_samples /frame->sample_rate %0.8f\n",
    ///                             (double)frame->nb_samples /frame->sample_rate);

    /// 返回实际需要播放的数据大小
    return resampled_data_size;
}
void AudioThread::setVolume(int volume)
{
    if(volume<0){
        return;
    }
    if(volume>128){
        volume=128;
    }
    fVolume=volume;
}
