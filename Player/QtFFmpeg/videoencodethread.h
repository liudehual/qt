#ifndef VIDEOENCODETHREAD_H
#define VIDEOENCODETHREAD_H

#include <QThread>
#include "global.h"
#include "metadata.h"

class VideoEncodeThread : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit VideoEncodeThread(QObject *parent = 0);
    explicit VideoEncodeThread(void *parent = 0,void *root=0);
    void run();
signals:

public slots:

};

#endif // VIDEOENCODETHREAD_H
