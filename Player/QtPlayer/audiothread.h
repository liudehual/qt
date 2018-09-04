#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QThread>
#include <QQueue>
#include "metadata.h"

/// 音频参数
typedef struct AudioParams {
    int freq;                 /// 采样率
    int channels;             /// 通道数量
    int64_t channel_layout;   /// 通道布局
    enum AVSampleFormat fmt;  /// 音频格式
    int frame_size;           /// 帧大小
    int bytes_per_sec;        /// 码流
} AudioParams;

/// 音频解码线程
class AudioThread:public QThread,public MetaData
{
public:
    AudioThread(void *qRoot,void *qParent,QObject *parent = 0);
    virtual ~AudioThread();
    void    run();

    /// 初始化音频解码线程
    int     init(AVCodecContext *ctx,AVCodec *codec);

    /// 初始化SDL线程
    int     initSDLThread();

    /// 获取当前音量
    int     getVolume(){return fVolume;}

    /// 设置音量
    void    setVolume(int volume);

    /// 解码帧数据
    int     audio_decode_frame(void *is);

    /// 音频数据缓存
    unsigned char * getAudioBuf(){return audio_buf;}

    /// SDL音频回调
    static void     sdl_audio_callback(void *opaque, Uint8 *stream, int len);

    /// 获取音频已播放时长
    double getAudioClock(){return audio_clock;}
private:
    /// 音频解码器上下文
    AVCodecContext  *   fCtx;

    /// 音频解码器
    AVCodec *           fCodec;

    /// SDL 音频参数结构（期望）
    SDL_AudioSpec       fWantedSpec;

    /// SDL 音频参数结构（实际）
    SDL_AudioSpec       fHardwareSpec;

    /// 音频参数（期望）
    AudioParams         audio_src;

    /// 音频参数（实际）
    AudioParams         audio_tgt;
public:
    /// 音频重采样上下文
    struct SwrContext * swr_ctx;

    /// 音频播放缓冲区（用于SDL线程播放） 及大小
    uint8_t *           audio_buf;
    unsigned int        audio_buf_size;

    /// 音频重采样缓冲区 及大小
    uint8_t *           audio_buf1;
    unsigned int        audio_buf1_size;

    /// 音量值
    int                 fVolume;

    /// 已播放音频时长
    double audio_clock;

    /// SDL回调播放时间
    int64_t audio_callback_time;

};

#endif // AUDIOTHREAD_H
