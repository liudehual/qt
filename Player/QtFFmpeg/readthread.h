#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include "global.h"
#include "metadata.h"
#include "videodecodethread.h"
#include "audiodecodethread.h"

class ReadThread : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit ReadThread(QObject *parent = 0);
    explicit ReadThread(void *parent = 0,void *root=0);
    void run();
signals:

public slots:

private:
    VideoDecodeThread *fVideoDecodeThread;
    AudioDecodeThread *fAudioDecodeThread;
};

#endif // READTHREAD_H
