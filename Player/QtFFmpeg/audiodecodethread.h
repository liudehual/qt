#ifndef AUDIODECODETHREAD_H
#define AUDIODECODETHREAD_H

#include <QThread>
#include "global.h"
#include "metadata.h"
#include "audioencodethread.h"

class AudioDecodeThread : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit AudioDecodeThread(QObject *parent = 0);
    explicit AudioDecodeThread(void *parent = 0,void *root=0);
    void run();
signals:

public slots:
private:
    AudioEncodeThread *fAudioEncodeThread;
};

#endif // AUDIODECODETHREAD_H
