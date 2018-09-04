#include "VideoDecodeThread.h"
#include "Log.h"
#include "OSMutex.h"
#include "Module.h"

#include <queue>

H264Frame::H264Frame():fValidData(data),fValidDataLen(0)
{

}

H264Frame::~H264Frame()
{

}

int H264Frame::getValidDataLen()
{
    return fValidDataLen;
}
int H264Frame::getVaildData(char *&buf,int &len)
{
    buf=fValidData;
    len=fValidDataLen;
    return 1;
}

int H264Frame::recountVaildData(int usedLen)
{
    if(usedLen>=fValidDataLen){
        fValidDataLen=0;
        fValidData=data;
        return 2;
    }
    fValidDataLen-=usedLen;
    fValidData+=usedLen;

    return 1;
}

int H264Frame::setVaildData(int validLen)
{
    fValidDataLen=validLen;
    fValidData=data;
    return 1;
}

char *H264Frame::getVaildData()
{
    return fValidData;
}


OSMutex VideoDecodeThread::fMutex;
VideoDecodeThread *VideoDecodeThread::createNewVideoDecodeThread()
{
    return new VideoDecodeThread();
}
VideoDecodeThread::VideoDecodeThread():fLive(1),fFrameRate(25),fWidth(480),fHeight(320),fFrame(NULL)
{
    memset(fFileName,'\0',sizeof(fFileName));

    for(int i=0;i<3;i++){
        H264Frame *theFrame=new H264Frame;
        fFramePool.push(theFrame);
    }
}
VideoDecodeThread::~VideoDecodeThread()
{
    fprintf(stderr,"VideoDecodeThread exit \n");
    av_frame_free(&fDecodeFrame);
    av_frame_free(&fRGBFrame);

    {
        OSMutexLocker theLocker(&fFramePoolMutex);
        for(int i=0;i<fFramePool.size();i++){
            H264Frame *theFrame=fFramePool.front();
            fFramePool.pop();
            delete theFrame;
        }
    }

    {
        OSMutexLocker theLocker(&fAvailableFramePoolMutex);
        for(int i=0;i<fAvailableFramePool.size();i++){
            H264Frame *theFrame=fAvailableFramePool.front();
            fAvailableFramePool.pop();
            delete theFrame;
        }
    }


}

void VideoDecodeThread::Entry()
{
    int ret;
    int got_picture;

    avpicture_fill((AVPicture *)fRGBFrame,
                   bufferRGB,
                   AV_PIX_FMT_RGB24,
                   fWidth,
                   fHeight);

    SwsContext *convert_ctx =NULL;
    convert_ctx = sws_getCachedContext(convert_ctx,
                                       pCodecCtx->width,
                                       pCodecCtx->height,
                                       pCodecCtx->pix_fmt,
                                       fWidth,
                                       fHeight,
                                       AV_PIX_FMT_RGB24,
                                       SWS_BICUBIC,
                                       NULL,
                                       NULL,
                                       NULL);
    if(!convert_ctx){
        return;
    }
    while (1) {

        OSThread::Sleep(1000/fFrameRate);

        av_init_packet(&packet);

        if(av_read_frame(fFormatCtx,&packet)<0){
            continue;
        }

        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
        if (ret <= 0){
            break;
        }
        if (got_picture){
            sws_scale(convert_ctx,
                      pFrame->data,
                      pFrame->linesize,
                      0,
                      pFrame->height,
                      fRGBFrame->data,
                      fRGBFrame->linesize);

            fHandler(fArg,(char *)fRGBFrame->data[0],fWidth,fHeight);
        }
        av_free_packet(&packet);
    }

    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    av_free(pCodecCtx);
}

int VideoDecodeThread::initVideoDecodeThread(void *arg,videoFrameHandler handler)
{
    fArg=arg;
    fHandler=handler;

    {
        OSMutexLocker theLocker(&VideoDecodeThread::fMutex);

        fFormatCtx = avformat_alloc_context();
        unsigned char * iobuffer=(unsigned char *)av_malloc(32*1024);
        AVIOContext *avio =avio_alloc_context(iobuffer,32*1024,0,this,VideoDecodeThread::fillIoBuffer,NULL,NULL);
        fFormatCtx->pb=avio;

        int theErr=0;
        if((theErr=avformat_open_input(&fFormatCtx,NULL,NULL,NULL))!=0){
            printf("Couldn't open input stream.\n");
            char errorBuf[1024]={'\0'};
            av_strerror(theErr, errorBuf, 1024);
            fprintf(stderr,"[%s] [%d] avformat_open_input error %s \n",__FUNCTION__,__LINE__,errorBuf);
            return 0;
        }else{
            fprintf(stderr,"avformat_open_input is succeed \n");
        }

        if(avformat_find_stream_info(fFormatCtx,NULL)<0){
            char errorBuf[1024]={'\0'};
            av_strerror(theErr, errorBuf, 1024);
            fprintf(stderr,"[%s] [%d] avformat_find_stream_info error %s \n",__FUNCTION__,__LINE__,errorBuf);
            return 0;
        }else{
            fprintf(stderr,"avformat_find_stream_info is succeed \n");
        }

        int videoindex=-1;
        for(int i=0; i<fFormatCtx->nb_streams; i++){
            if(fFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
                videoindex=i;
                break;
            }
        }

        if(videoindex==-1){
            printf("Didn't find a video stream.\n");
            return 0;
        }

        pCodecCtx=fFormatCtx->streams[videoindex]->codec;
        pCodec=avcodec_find_decoder(pCodecCtx->codec_id);

        if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
            char errorBuf[1024]={'\0'};
            av_strerror(theErr, errorBuf, 1024);
            fprintf(stderr,"[%s] [%d] avcodec_open2 error %s \n",__FUNCTION__,__LINE__,errorBuf);
            return 0;
        }

        pFrame = av_frame_alloc();

        av_init_packet(&packet);

    }

    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB24,
                                      fWidth,
                                      fHeight);
    bufferRGB = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    fDecodeFrame=av_frame_alloc();
    fRGBFrame=av_frame_alloc();

    return 1;
}

int VideoDecodeThread::killSelf()
{
    fLive=false;
    return 1;
}
int VideoDecodeThread::addFrame(char *buf,int len)
{
    if(len>FRAME_CACHE_LEN){
        return 0;
    }
    H264Frame *theFrame=NULL;

    {
        OSMutexLocker theLocker(&fFramePoolMutex);
        if(fFramePool.size()==0){
            return 0;
        }
        theFrame=fFramePool.front();
        fFramePool.pop();

        memcpy(theFrame->data,buf,len);
        theFrame->dataLen=len;

        theFrame->setVaildData(len);
    }


    {
        OSMutexLocker theLocker(&fAvailableFramePoolMutex);
        fAvailableFramePool.push(theFrame);
    }
    return 1;
}

H264Frame * VideoDecodeThread::getFrame()
{
    OSMutexLocker theLocker(&fAvailableFramePoolMutex);

    if(fAvailableFramePool.size()==0){
        return NULL;
    }
    H264Frame *frame=NULL;
    {
        frame=fAvailableFramePool.front();
        fAvailableFramePool.pop();
    }
    return frame;
}

int VideoDecodeThread::setWidthAndHeight(int w,int h)
{
    fWidth=w;
    fHeight=h;
    return 1;
}

int VideoDecodeThread::setFrameRate(int rate)
{
    if(rate<20 || rate>100){
        return 0;
    }
    fFrameRate=rate;
    return 1;
}

int VideoDecodeThread::setFileName(char *fileName)
{
    memcpy(fFileName,fileName,strlen(fileName));
    return 1;
}

int VideoDecodeThread::lockmgrCallback(void **mutex, enum AVLockOp op)
{
    switch(op){
    case AV_LOCK_CREATE:{
        OSMutex *theMutex=new OSMutex;
        *mutex=theMutex;
    }
        break;
    case AV_LOCK_OBTAIN:{///< Lock the mutex
        if(mutex && ((OSMutex *)*mutex)){
            ((OSMutex*)*mutex)->Lock();
        }
    }
        break;
    case AV_LOCK_RELEASE:{///< Unlock the mutex
        if(mutex && ((OSMutex *)*mutex)){
            ((OSMutex*)*mutex)->Unlock();
        }
    }
        break;
    case AV_LOCK_DESTROY:{///< Free mutex resources
        if(mutex && ((OSMutex *)*mutex)){
            OSMutex *theMutex=(OSMutex *)*mutex;
            delete theMutex;
            *mutex=NULL;
        }
    }
        break;
    default:
        break;
    }
    return 0;
}

int VideoDecodeThread::initOS()
{
    av_register_all();

    avcodec_register_all();

    avformat_network_init();;

    av_lockmgr_register(lockmgrCallback);
    return 1;
}

int VideoDecodeThread::fillIoBuffer(void *opaque, uint8_t *buf, int buf_size)
{
    VideoDecodeThread *theThread=(VideoDecodeThread *)opaque;

    if(!theThread->fFrame){
        theThread->fFrame=theThread->getFrame();
    }
    if(theThread->fFrame){
        if(theThread->fFrame->getValidDataLen()>=buf_size){
            memcpy(buf,theThread->fFrame->getVaildData(),buf_size);
            theThread->fFrame->recountVaildData(buf_size);
            return buf_size;
        }

        int dataLen=0;
        if(theThread->fFrame->getValidDataLen()>0 && theThread->fFrame->getValidDataLen()<buf_size){
            memcpy(buf,theThread->fFrame->getVaildData(),theThread->fFrame->getValidDataLen());
            dataLen=theThread->fFrame->getValidDataLen();
            theThread->fFrame->recountVaildData(theThread->fFrame->getValidDataLen());
        }

        {
            OSMutexLocker theLocker(&(theThread->fFramePoolMutex));
            theThread->fFramePool.push(theThread->fFrame);
        }
        theThread->fFrame=NULL;

        return dataLen;
    }

    return -1;
}
