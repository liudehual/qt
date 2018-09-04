#ifndef FVLPARSERTHREAD_H
#define FVLPARSERTHREAD_H

#include <QThread>
#include <QQueue>
#include <QFile>
#include "flvtag.h"
#include <QString>
class MutexLocker
{
    public:
    MutexLocker(QMutex *tMutex):fMutex(tMutex)
    {
        if(fMutex!=NULL){
            fMutex->lock();
        }
    }
    ~MutexLocker()
    {
        if(fMutex!=NULL){
            fMutex->unlock();
        }
    }
private:
       QMutex *fMutex;
};
class FVLParserThread : public QThread
{
    Q_OBJECT
public:
    explicit FVLParserThread(QObject *parent = 0);
    ~FVLParserThread();
    FLVTag *GetTagByIndex(qint64 i);
    qint64 GetTagQueueLength(){return fTagQueue.length();}
protected:
    virtual void run();

signals:
    void updateListView();
public slots:
    void RecvFileName(QString fileName);
private:
    void Clean();
    void ParserFlv(QString fileName);
    void ParserFlvBody(unsigned char *bodyStart,qint64 readLen,unsigned int headerLen);
private:
    QQueue<FLVTag *> fTagQueue;
    QMutex fMuext;
    QFile *fFile;

    QString fFileName;
    bool fGetFileName;
};

#endif // FVLPARSERTHREAD_H
