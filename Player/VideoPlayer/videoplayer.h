#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QThread>
#include <QTimer>
#include <QEventLoop>
#include <QLabel>
#include <QImage>

extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libavdevice/avdevice.h>
    #include <libavformat/avformat.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <SDL/SDL_stdinc.h>
    #include <SDL/SDL_mutex.h>
}

/*������Ƶ���߳�*/
class DecodeVideo :public QThread
{
    Q_OBJECT
public:
    explicit DecodeVideo();
    ~DecodeVideo();
    void setPacket(AVPacket pkt){packet = pkt;}
    void setAVCodecContext(AVCodecContext*ctx);
    void setMutex(SDL_mutex *m){mutex = m;}
protected:
    void run();
signals:
    void readOneFrame(QImage);
private:
    AVCodecContext  *pCodecCtx; //��Ƶ������
    AVPacket packet;
    uint8_t *bufferRGB;


    int width;
    int height;
    PixelFormat pix_fmt;

    SDL_mutex *mutex;
};

/*��ȡ��Ƶ���߳�*/
class VideoPlayer : public QThread
{
    Q_OBJECT

public:
    explicit VideoPlayer(QObject *parent = 0);
    ~VideoPlayer();

    enum State//��ʾ����״̬��ֵ
    {
        PlayingState,
        PausedState,
        StoppedState
    };
    enum Type //��ʾ����ģʽ��ֵ
    {
        VideoType, //��Ƶģʽ
        AudioType,  //��Ƶģʽ
        NoneType  //��ʾ��ǰ����ֹͣ״̬
    };

    void setSource(QString str); //���ò����ļ�
    void play();
    void pause();
    void stop();
    void setVolume(int value); //����������0-128��
    void seek(qint64); //����Ϳ��ˣ�΢�룩
    qint64 totalTime(){return totaltime;} //��ȡ��ʱ�� ��΢�룩
    qint64 currentTime(){return currenttime;} //��ȡ��ǰʱ�� �����룩

    State state(){return curState;} //��ǰ����״̬
    Type type(){return curType;}//��ǰ����ģʽ
signals:
    void updateTime(long currentTime); //����ʱ��(����)
    void finished(); //�������
    void stateChanged(VideoPlayer::State); //����״̬�����ı�
    void soundSignalChanged(int);
    void readOneFrame(QImage); 
public slots:
    void soundSlotChanged(int);
protected:
    void run();
private:
    AVFormatContext *pFormatCtx;
    AVCodecContext  *pCodecCtx; //��Ƶ������
    AVCodecContext  *aCodecCtx; //��Ƶ������

    int videoStream;
    int audioStream;

    qint64 totaltime; //��ʱ��
    qint64 currenttime; //��ǰʱ��
    AVPacket nextPacket;//������Ҫ���ŵİ�

    State curState; //����״̬
    Type curType; //����ģʽ

    QEventLoop *eventloop;
    SDL_mutex *mutex;
    SDL_mutex *decodeVideoMutex;

    /*�����õ��߳�,������ܿ�ͨ�����Ӵ��̵߳�������ʵ��*/
    DecodeVideo *decodeVideoThread;

    void initAvcodec();
    bool openVideo(char *filename);
    void closeVideo(); //�ر���Ƶ

    bool openSDL();  //������
    bool closeSDL(); //�ر�����
    void clearQuene(); //����������е�����

private slots:
    void readPacket();
};

#endif // VIDEOPLAYER_H
