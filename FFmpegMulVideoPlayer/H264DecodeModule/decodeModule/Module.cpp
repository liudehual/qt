#include "Module.h"
#include "VideoDecodeThread.h"
#include "Log.h"

Module *Module::createNewModule()
{
    return new Module();
}
int Module::initModule(void *arg,videoFrameHandler handler)
{
    int theErr=((VideoDecodeThread *)fVideoDecodeThread)->initVideoDecodeThread(arg,handler);
    return theErr;
}

Module::Module():fVideoDecodeThread(NULL)
{
    fVideoDecodeThread=(void *)VideoDecodeThread::createNewVideoDecodeThread();
}

Module::~Module()
{
    this->closeModule();
}

int Module::addVideoFrame(char *buf,int len)
{
    return ((VideoDecodeThread *)fVideoDecodeThread)->addFrame(buf,len);
}

int Module::closeModule()
{
    return ((VideoDecodeThread *)fVideoDecodeThread)->killSelf();
}

int Module::setWidthAndHeight(int w,int h)
{
    return ((VideoDecodeThread *)fVideoDecodeThread)->setWidthAndHeight(w,h);
}
int Module::setFrameRate(int rate)
{
    return ((VideoDecodeThread *)fVideoDecodeThread)->setFrameRate(rate);
}

int Module::startModule()
{
    ((VideoDecodeThread *)fVideoDecodeThread)->Start();
    return 1;
}
int Module::setFileName(char *fileName)
{
    return ((VideoDecodeThread *)fVideoDecodeThread)->setFileName(fileName);
}
int Module::initOS()
{
    VideoDecodeThread::Initialize();
    VideoDecodeThread::initOS();
    return 1;
}
