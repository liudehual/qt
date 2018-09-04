#include "packetqueue.h"
#include "qtplayer.h"
PacketQueue::PacketQueue()
{
    fMutex=new QMutex;
    fCond=new QWaitCondition;
    fQueue.clear();
}
PacketQueue::~PacketQueue()
{

}
void PacketQueue::putPacketToQueue(AVPacket *packet)
{
    QMutexLocker theLocker(fMutex);
    fQueue.enqueue(packet);
    fCond->wakeOne();
}

int PacketQueue::getPacketFromQueue(AVPacket *&packet)
{
    {
        QMutexLocker theLocker(fMutex);
        if(fQueue.length()==0){
            fCond->wait(fMutex);
            return 0;
        }
        packet= fQueue.dequeue();
    }
    return 1;
}
void PacketQueue::cleanPacketQueue()
{
    QMutexLocker theLocker(fMutex);
    for(int i=0;i!=fQueue.length();i++){
        AVPacket *packet=fQueue[i];
        av_packet_free(&packet);
    }
    fQueue.clear();
}
int PacketQueue::getPacketQueueLen()
{
    QMutexLocker theLocker(fMutex);
    return fQueue.length();
}
