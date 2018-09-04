#include "decodethread.h"
extern"C"
{
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"

}
#include "ClientInterface.h"
DecodeThread::DecodeThread(QObject *parent) :
    QThread(parent),playerState(false),pFormatContext(NULL),
    pCodecContext(NULL),pCodec(NULL),pPacket(NULL),pFrame(NULL),
    pIOContext(NULL),vIndex(-1),pSwsContext(NULL)
{
    av_register_all();

    avformat_network_init();
    pCodec=avcodec_find_decoder(AV_CODEC_ID_H264);
    if(pCodec==NULL){
        printf("Codec not found.\n");
    }
    pCodecContext = avcodec_alloc_context3(pCodec);

    pCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecContext->pix_fmt = PIX_FMT_YUV420P;

    if(avcodec_open2(pCodecContext, pCodec,NULL)<0){
        printf("Could not open codec.\n");
    }

    pFrame=av_frame_alloc();
    pPacket=(AVPacket *)av_malloc(sizeof(AVPacket));
    av_init_packet(pPacket);
    pPic=(AVPicture *)av_frame_alloc();
    avpicture_alloc(pPic,PIX_FMT_RGB24,640,320);


    pSwsContext = sws_getContext(640,320,PIX_FMT_YUV420P,640,320,PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);


}
void DecodeThread::startVideo()
{
    if(playerState){
        return;
    }
    this->start();
    /*
    	在此处修改 
    	ip 地址：192.168.31.103 
    	端口号 ：554
    	流名：live1.264
    
    */
    ::startClient("192.168.31.103",554,"live1.264");
    playerState=true;
    fprintf(stderr,"startVideo SLOT \n");
}
void DecodeThread::pauseVideo()
{
    fprintf(stderr,"pauseVideo SLOT \n");

}
void DecodeThread::stopVideo()
{
    if(!playerState){
        return;
    }
    playerState=false;
    ::stopClient();
    fprintf(stderr,"stopVideo SLOT \n");

}

void DecodeThread::run()
{

    while(1){
        msleep(15);
        av_init_packet(pPacket);
        unsigned char *frame=NULL;
        unsigned int frameSize=0;

        ::readData(&frame,&frameSize);
        if(frame==NULL || frameSize==0){
            continue;
        }
        pPacket->size=frameSize;
        pPacket->data=frame;
        int got_picture=0;

        int ret = avcodec_decode_video2(pCodecContext, pFrame, &got_picture, pPacket);
#if 1
        if(ret < 0){
            printf("Decode Error.\n");
            continue;
        }
#endif
        fprintf(stderr,"%d\n",got_picture);
        if(got_picture){
            fprintf(stderr,"Decode Video is succeed\n");
            sws_scale(pSwsContext,(const uint8_t* const *)pFrame->data,pFrame->linesize,0,320,pPic->data,pPic->linesize);
            QImage image(pPic->data[0],640,320,QImage::Format_RGB888);
            emit displaVideo(image);
        }
    }

}
int DecodeThread::readData(unsigned char *buffer,unsigned int bufferSize)
{


}
int DecodeThread::read_buffer(void *opaque, uint8_t *buf, int buf_size)
{
    DecodeThread *dt=(DecodeThread *)opaque;
    unsigned char *frame=NULL;
    unsigned int frameSize=0;
    ::readData(&frame,&frameSize);
    if(frame==NULL){
        return -1;
    }
    memcpy(buf,frame,frameSize);
    return frameSize;
}
