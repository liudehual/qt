#include "core.h"

Core::Core(QObject *parent) :
    QThread(parent)
{
}
Core::Core():MetaData(this,this)
{

}
Core::~Core()
{

}

void Core::run()
{
    while(1){
        QThread::sleep(1);
        av_log(NULL,AV_LOG_INFO,"%s\n","Core::run");
    }
}
int Core::initFFmpegLib()
{
    static bool registered=false;
    if(registered){
        return 1;
    }
    registered=true;

    /* 注册所有的编解码器 */
    avcodec_register_all();

    /* 注册所有的过滤器 */
    avfilter_register_all();

    /* 注册所有的复用器、解复用器、编解码器、协议等 */
    av_register_all();

    /* 初始化网络 */
    avformat_network_init();

    av_log(NULL,AV_LOG_INFO,"%s\n","initFFmpegLib");
}
