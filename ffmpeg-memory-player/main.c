
/**
 * 最简单的基于FFmpeg的内存读写例子（内存播放器）
 * Simplest FFmpeg mem Player
 *
 * 雷霄骅
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序实现了对内存中的视频数据的播放。
 * 是最简单的使用FFmpeg读内存的例子。
 *
 * This software play video data in memory (not a file).
 * It's the simplest example to use FFmpeg to read from memory.
 *
 */


#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "SDL/SDL.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <SDL/SDL.h>
#ifdef __cplusplus
};
#endif
#endif

//Output YUV420P
#define OUTPUT_YUV420P 0
FILE *fp_open=NULL;

//Callback
/// we should write data to buffer before ffmpeg call this function
/// 在ffmpeg调用该回调函数前我们需要先向内存中写入数据，该回调函数会在avformat_find_stream_info函数中调用，确定视频格式
int read_buffer(void *opaque, uint8_t *buf, int buf_size)
{
    fprintf(stderr,"================\n");
    if(!feof(fp_open)){
        int true_size=fread(buf,1,buf_size,fp_open);
        return true_size;
    }else{
        return -1;
    }
}


int main(int argc, char* argv[])
{

    AVFormatContext	*pFormatCtx;
    int				i, videoindex;
    AVCodecContext	*pCodecCtx;
    AVCodec			*pCodec;
    char filepath[]="/home/gct/Videos/test.h264";

    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    fp_open=fopen(filepath,"rb+");
    if(!fp_open){
        fprintf(stderr,"[%s] [%d] not find file . file name is %s \n",__FUNCTION__,__LINE__,filepath);
        return -1;
    }

    /// 注册内存读取回调函数
    //Init AVIOContext
    unsigned char *aviobuffer=(unsigned char *)av_malloc(32768);
    AVIOContext *avio =avio_alloc_context(aviobuffer, 32768,0,NULL,read_buffer,NULL,NULL);
    pFormatCtx->pb=avio;

    /// 打开文件格式
    if(avformat_open_input(&pFormatCtx,NULL,NULL,NULL)!=0){
        printf("Couldn't open input stream.\n");
        return -1;
    }
    /// 发现文件中的流(音频,视频，字幕等)等的信息，此处会调用回调函数读取数据流
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        printf("Couldn't find stream information.\n");
        return -1;
    }

    /// 查找视频流并初始化AVCodecContext/AVCodec 结构体
    fprintf(stderr,"hello -------------\n");
    videoindex=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoindex=i;
            break;
        }
    if(videoindex==-1){
        printf("Didn't find a video stream.\n");
        return -1;
    }
    pCodecCtx=pFormatCtx->streams[videoindex]->codec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        printf("Codec not found.\n");
        return -1;
    }

    /// 打开视频解码器
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        printf("Could not open codec.\n");
        return -1;
    }

    AVFrame	*pFrame,*pFrameYUV;
    pFrame=av_frame_alloc();
    pFrameYUV=av_frame_alloc();

    /// 初始化SDL，同时打开音频／视频／定时器
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf( "Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    /// 设置视频显示的宽高
    int screen_w=0,screen_h=0;
    SDL_Surface *screen;
    screen_w = pCodecCtx->width;
    screen_h = pCodecCtx->height;
    screen = SDL_SetVideoMode(screen_w, screen_h, 0,0);

    if(!screen) {
        printf("SDL: could not set video mode - exiting:%s\n",SDL_GetError());
        return -1;
    }

    /// 创建SDL显示层
    SDL_Overlay *bmp;
    bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height,SDL_YV12_OVERLAY, screen);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = screen_w;
    rect.h = screen_h;
    //SDL End------------------------
    int ret, got_picture;

    AVPacket *packet=(AVPacket *)av_malloc(sizeof(AVPacket));

#if OUTPUT_YUV420P
    FILE *fp_yuv=fopen("output.yuv","wb+");
#endif
    SDL_WM_SetCaption("Simplest FFmpeg Mem Player",NULL);

    /// libswscale是一个主要用于处理图片像素数据的类库。可以完成图片像素格式的转换，图片的拉伸等工作
    /// 初始化图像转换结构
    struct SwsContext *img_convert_ctx;
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    //------------------------------

    /// 读取数据
    while(av_read_frame(pFormatCtx, packet)>=0){
        if(packet->stream_index==videoindex){
            /// 解码视频
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if(ret < 0){
                printf("Decode Error.\n");
                return -1;
            }
            if(got_picture){
                SDL_LockYUVOverlay(bmp);
                pFrameYUV->data[0]=bmp->pixels[0];
                pFrameYUV->data[1]=bmp->pixels[2];
                pFrameYUV->data[2]=bmp->pixels[1];
                pFrameYUV->linesize[0]=bmp->pitches[0];
                pFrameYUV->linesize[1]=bmp->pitches[2];
                pFrameYUV->linesize[2]=bmp->pitches[1];

                /// 将解码后的H264数据转换成YUV格式
                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
#if OUTPUT_YUV420P
                int y_size=pCodecCtx->width*pCodecCtx->height;
                fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y
                fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
                fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
#endif
                SDL_UnlockYUVOverlay(bmp);

                /// 显示图片
                SDL_DisplayYUVOverlay(bmp, &rect);
                //Delay 40ms
                SDL_Delay(40);
            }
        }
        av_free_packet(packet);
    }
    sws_freeContext(img_convert_ctx);

#if OUTPUT_YUV420P
    fclose(fp_yuv);
#endif

    fclose(fp_open);

    SDL_Quit();

    //av_free(out_buffer);
    av_free(pFrameYUV);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}

