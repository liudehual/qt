#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include <stdio.h>
#include <stdlib.h>
#include "include/Module.h"

class ReadThread : public QThread
{
    Q_OBJECT
public:
    static ReadThread *createNewReadThread();
    virtual void run();

    int initReadThread(Module *m);

    /// 该函数在initReadThread之前调用
    int setFileName(char *fileName);

    int getFrameLen(char *startPos,char *endPos);
protected:
    int getSPSAndPPSFrame();
    explicit ReadThread(QObject *parent = 0);
signals:

public slots:

private:
    char fFileName[256];
    FILE *fp;

    Module *fModule;
};

#endif // READTHREAD_H
