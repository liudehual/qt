#ifndef DECODETHREAD_H
#define DECODETHREAD_H

#include <QThread>
#include <QImage>
extern"C"
{
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"

}
class Dialog;
class DecodeThread : public QThread
{
    Q_OBJECT
public:
    explicit DecodeThread(QObject *parent = 0);

    virtual void run();

    int readData(unsigned char *buffer,unsigned int bufferSize);
    static int read_buffer(void *opaque, uint8_t *buf, int buf_size);
signals:
    void displaVideo(QImage img);
public slots:
    void startVideo();
    void pauseVideo();
    void stopVideo();
private:
        bool playerState;

        AVFormatContext *pFormatContext;
        AVCodecContext *pCodecContext;
        AVCodec *pCodec;
        AVPacket *pPacket;
        AVFrame *pFrame;
        AVIOContext *pIOContext;
        SwsContext * pSwsContext;
        AVPicture *pPic;
        int vIndex;
private:
    Dialog *dia;

};

#endif // DECODETHREAD_H
