#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H

#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include "metadata.h"

///    数据包队列
class PacketQueue
{
public:
    PacketQueue();
    ~PacketQueue();
    void        putPacketToQueue(AVPacket *packet);
    int         getPacketFromQueue(AVPacket *&packet);
    void        cleanPacketQueue();
    int         getPacketQueueLen();
private:
    QQueue<AVPacket *>    fQueue;
    QMutex *            fMutex;
    QWaitCondition *    fCond;
};

#endif // PACKETQUEUE_H
