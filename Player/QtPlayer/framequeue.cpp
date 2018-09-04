#include "framequeue.h"
#include "qtplayer.h"
CustomFrameQueue::CustomFrameQueue(int f):flags(f)
{
    fMutex=new QMutex;
    fCond=new QWaitCondition;
    fQueue.clear();
}

CustomFrameQueue::~CustomFrameQueue()
{

}
int CustomFrameQueue::putFrameToQueue(Frame *frame)
{
    QMutexLocker theLocker(fMutex);
    if(flags==VIDEO_FRAME){
        if(fQueue.length()>=VIDEO_FRAME_QUEUE_LEN){
            fCond->wait(fMutex);
        }
    }else{
        if(fQueue.length()>=AUDIO_FRAME_QUEUE_LEN){
            fCond->wait(fMutex);
        }
    }

    fQueue.enqueue(frame);
    return 1;
}

int CustomFrameQueue::getFrameFromQueue(Frame *&frame)
{
    QMutexLocker theLocker(fMutex);
    if(fQueue.length()==0){
        fCond->wakeOne();
        return 0;
    }
    frame= fQueue.dequeue();
    fCond->wakeOne();

    return 1;
}
void CustomFrameQueue::cleanFrameQueue()
{
    QMutexLocker theLocker(fMutex);
    for(int i=0;i!=fQueue.length();i++){
        Frame *f=fQueue[i];
        av_frame_free(&(f->frame));
        delete f;
    }
    fQueue.clear();
}
int CustomFrameQueue::getFrameQueueLen()
{
    QMutexLocker theLocker(fMutex);
    return fQueue.length();
}
void     CustomFrameQueue::frameQueueNext()
{
    QMutexLocker theLocker(fMutex);

    fQueue.dequeue();
}
