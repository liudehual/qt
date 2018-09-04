#ifndef VIDEODECODETHREAD_H
#define VIDEODECODETHREAD_H

#include <QThread>
#include "global.h"
#include "metadata.h"
#include "videoencodethread.h"

class VideoDecodeThread : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit VideoDecodeThread(QObject *parent = 0);
    explicit VideoDecodeThread(void *parent = 0,void *root=0);
    void run();
signals:

public slots:

private:
    VideoEncodeThread *fVideoEncodeThread;
};

#endif // VIDEODECODETHREAD_H
