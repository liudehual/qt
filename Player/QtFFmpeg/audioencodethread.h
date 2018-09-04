#ifndef AUDIOENCODETHREAD_H
#define AUDIOENCODETHREAD_H

#include <QThread>
#include "global.h"
#include "metadata.h"

class AudioEncodeThread : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit AudioEncodeThread(QObject *parent = 0);
    explicit AudioEncodeThread(void *parent = 0,void *root=0);
    void run();
signals:

public slots:

};

#endif // AUDIOENCODETHREAD_H
