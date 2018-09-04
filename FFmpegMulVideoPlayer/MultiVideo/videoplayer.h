#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H
#include <QLabel>
#include "include/Module.h"
#include "readthread.h"

class VideoPlayer
{
public:
    static VideoPlayer *createNewVideoPlayer();
    ~VideoPlayer();
    int initVideoPlayer(QLabel *label);
    int setFileName(char *fileName);
    static void playCallback(void *arg,char *buf,int width,int height);
    int startPlayer();
    QLabel *getVideoLabel();
protected:
    VideoPlayer();
private:
    QLabel *fVideoLabel;
    Module *fVideoDecodeModule;
    ReadThread *fReadThread;
};

#endif // VIDEOPLAYER_H
