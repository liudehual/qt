#include "fvlparserthread.h"
#include <QByteArray>
#include <QDebug>

#define DEFAULT_FLV_HEADER_LEN 9
FVLParserThread::FVLParserThread(QObject *parent) :
    QThread(parent),fFile(NULL),fGetFileName(false)
{
    qDebug()<<"Created FVLParserThread \n";
}
FVLParserThread::~FVLParserThread()
{

}
FLVTag *FVLParserThread::GetTagByIndex(qint64 i)
{
    if(i>fTagQueue.length()
            || i<0){
        return NULL;
    }
    MutexLocker tMutex(&fMuext);
    return fTagQueue[i];
}
void FVLParserThread::ParserFlv(QString fileName)
{
    fFile=new QFile(fileName);
    if (!fFile->open(QIODevice::ReadOnly)){
        qDebug()<<"=================";
        return;
    }

    float fileLen=fFile->size();
    unsigned char *dataBuf=new unsigned char[fileLen];
    qint64 readLen=fFile->read((char *)dataBuf,fileLen);

    qDebug()<<fileLen<<"================";
    qDebug()<<readLen<<"================";

    /* 开始分析数据 */
    unsigned char *pos=dataBuf;

    /* 检查是不是真的flv文件 */
    if((pos[0]=='f' || pos[0]=='F')
            && (pos[1]=='l' || pos[1]=='L')
            && (pos[2]=='v' || pos[2]=='V')){
        qDebug()<<"This is Flv Media File";

        /* 清除上一个文件内容 */
        Clean();

        /* 获取版本号 */
        unsigned char flvVersion=pos[3];
        qDebug()<<"Flv Version "<<flvVersion;

        /* 获取文件头长度 */
        unsigned int headerLen=pos[5]<<24 | pos[6]<<16 | pos[7]<<8 | pos[8];
        qDebug()<<"Flv Header Length "<<headerLen;

        if(headerLen>DEFAULT_FLV_HEADER_LEN){
            qDebug()<<"Flv Media File has Extend Information";

            unsigned char *extendInfoStart=&pos[9];
            /* do nothing */
        }


        unsigned char *bodyStart=&pos[headerLen];

        /* 分析文件数据体 */
        ParserFlvBody(bodyStart,readLen,headerLen);
    }else{
        qDebug()<<"This is Not Flv Media File";
    }

    delete [] dataBuf;

}
void FVLParserThread::Clean()
{
    /*
        清楚上一次数据
    */
    for(QQueue<FLVTag *>::Iterator it=fTagQueue.begin();
        it!=fTagQueue.end();
        it++){
        FLVTag *ft=*it;
        delete ft;
    }
    fTagQueue.clear();
    if(fFile!=NULL){
        delete fFile;
        fFile=NULL;
    }
}
#if 1
void FVLParserThread::run()
{
    QThread::sleep(1);
  //  qDebug()<<"FVLParserThread Run\n";

    MutexLocker tMutex(&fMuext);
    /*
        读取文件并分析
    */
    if(fGetFileName){
        ParserFlv(fFileName);
    }

}
#endif
void FVLParserThread::RecvFileName(QString fileName)
{
    /*
        从UI线程接收文件名
    */
    {
        MutexLocker tMutex(&fMuext);
        fFileName=fileName;
        fGetFileName=true;
    }
    this->start();
}
void FVLParserThread::ParserFlvBody(unsigned char *bodyStart,qint64 readLen,unsigned int headerLen)
{
    /*
     *
     *
     处理整体结构
    --------------------------------------------------
    |           |            |           |           |   上一个Tag大小 4字节
    --------------------------------------------------
    |                                                |   Tag类型      1字节
    --------------------------------------------------
    |              |              |                  |   Tag 数据大小  3字节
    --------------------------------------------------
    |           |            |           |           |   时间戳        4字节
    --------------------------------------------------
    |              |              |                  |   流ID         4 字节
    --------------------------------------------------

    */
    /*  */
    quint32 metaDataLen=15; /* */
    qint64 bodyLen=readLen-headerLen;

    qint64 leftPos=0;
    while(true){

        unsigned char *pos=&bodyStart[leftPos];
        /* 获取数据长度 */
        quint32 dataSize=pos[5]<<16 | pos[6]<<8 | pos[7];
     //   qDebug()<<"=========== dataSize "<<dataSize;
        /* 设置Tag信息 */
        FLVTag *tag=new FLVTag;
        tag->fTagBuf=new quint8[dataSize];
        memcpy(tag->fTagBuf,&pos[15],dataSize);
        tag->fTagHeader.fTagLen=dataSize+11;
        tag->fTagHeader.fStreamId=pos[12]<<16 | pos[13]<<8 | pos[14];
        tag->fTagHeader.fTagType=pos[4];
        tag->fTagHeader.fTimeStamp=pos[8]<<16 | pos[9]<<8 | pos[10];

  //      qDebug()<<tag->fTagHeader.fTagLen<<" "<<tag->fTagHeader.fTagType<<" "<<tag->fTagHeader.fStreamId;
        /* 入队列 */
        fTagQueue.push_back(tag);

        leftPos+=metaDataLen;
        leftPos+=dataSize;
      //  qDebug()<<"============== leftPos "<<leftPos<<" ====== bodyLen "<<bodyLen;

        if(leftPos>=bodyLen
                || (bodyLen-leftPos)<metaDataLen){

       //     qDebug()<<fTagQueue.length();
            break;
        }

    }
    emit updateListView();
}
