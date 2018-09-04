#include "readthread.h"

ReadThread *ReadThread::createNewReadThread()
{
    return new ReadThread();
}

ReadThread::ReadThread(QObject *parent) :
    QThread(parent)
{
    memset(fFileName,'\0',sizeof(fFileName));
}

void ReadThread::run()
{
    char dataBuf[512*1024];
    while(1){
        int size=fread(dataBuf,1,sizeof(dataBuf),fp);

        if(size<=0){
            ///fprintf(stderr,"[%s] [%d] size %d \n",__FUNCTION__,__LINE__,size);
            fseek(fp,0L,SEEK_SET);
            continue;
        }
        while(1){
            int theErr =fModule->addVideoFrame(dataBuf,size);
            ///fprintf(stderr,"[%s] [%d] file name  %s \n",__FUNCTION__,__LINE__,fFileName);
            if(!theErr){
                QThread::usleep(1000*10);
                continue;
            }else{
                break;
            }
        }
    }
}

int ReadThread::initReadThread(Module *m)
{
    fp=fopen(fFileName,"rb");
    if(!fp){
        return 0;
    }
    fModule=m;

    /// 获取SPS/PPS帧
    /// test
    ///getSPSAndPPSFrame();
    return 1;
}

int ReadThread::setFileName(char *fileName)
{
    memcpy(fFileName,fileName,strlen(fileName));
    return 1;
}

int ReadThread::getSPSAndPPSFrame()
{
    ///fseek(fp,0L,SEEK_SET);
    char dataBuf[100*1024];
    char *startFramePos=NULL;
    char *endFramePos=NULL;

    int findSps=0;
    int findPps=0;
    int findI=0;

    char sps[256]={'\0'};
    int spsLen=0;
    char pps[256]={'\0'};
    int ppsLen=0;
    char spsAndpps[512]={'\0'};
    int spsAndppsLen=0;
    char iFrame[64*1024]={'\0'};
    int iFrameLen=0;

    while(1){
        int size=fread(dataBuf,1,sizeof(dataBuf),fp);
        if(size<=0){
            fprintf(stderr,"[%s] [%d] read over or something is wrong \n",__FUNCTION__,__LINE__);
            break;
        }
        if(!findSps){
            startFramePos=dataBuf;
            endFramePos=&dataBuf[size];
            /// 查找SPS帧
            for(int i=0;i<size-4;i++){
                if((unsigned char)startFramePos[0]==0x00
                        && (unsigned char)startFramePos[1]==0x00
                        && (unsigned char)startFramePos[2]==0x00
                        && (unsigned char)startFramePos[3]==0x01
                        && ((unsigned char)startFramePos[4] & 0x1F)==7){ /// find frame head
                    fprintf(stderr,"[%s] [%d] find one SPS frame \n",__FUNCTION__,__LINE__);
                    int len=getFrameLen(startFramePos,endFramePos);
                    if(len>0){
                        memcpy(sps,startFramePos,len);
                        spsLen=len;
                        ///fModule->addVideoFrame(startFramePos,len);
                        findSps=1;
                        fprintf(stderr,"[%s] [%d] find sps succeed\n",__FUNCTION__,__LINE__);
                        break;
                    }
                }
                startFramePos++;
            }
        }

        if(!findPps){
            startFramePos=dataBuf;
            endFramePos=&dataBuf[size];

            /// 查找PPS帧
            for(int i=0;i<size-4;i++){
                if((unsigned char)startFramePos[0]==0x00
                        && (unsigned char)startFramePos[1]==0x00
                        && (unsigned char)startFramePos[2]==0x00
                        && (unsigned char)startFramePos[3]==0x01
                        && ((unsigned char)startFramePos[4] & 0x1F)==8){ /// find frame head
                    fprintf(stderr,"[%s] [%d] find one PPS frame \n",__FUNCTION__,__LINE__);
                    int len=getFrameLen(startFramePos,endFramePos);
                    if(len>0){
                        memcpy(pps,startFramePos,len);
                        ppsLen=len;
                        ///fModule->addVideoFrame(startFramePos,len);
                        findPps=1;
                        fprintf(stderr,"[%s] [%d] find pps \n",__FUNCTION__,__LINE__);
                        break;
                    }
                }
                startFramePos++;
            }
        }

        if(!findI){
            startFramePos=dataBuf;
            endFramePos=&dataBuf[size];

            /// 查找I帧
            for(int i=0;i<size-4;i++){
                if((unsigned char)startFramePos[0]==0x00
                        && (unsigned char)startFramePos[1]==0x00
                        && (unsigned char)startFramePos[2]==0x00
                        && (unsigned char)startFramePos[3]==0x01
                        && ((unsigned char)startFramePos[4] & 0x1F)==5){ /// find frame head
                    fprintf(stderr,"[%s] [%d] find one PPS frame \n",__FUNCTION__,__LINE__);
                    int len=getFrameLen(startFramePos,endFramePos);
                    if(len>0){
                        memcpy(iFrame,startFramePos,len);
                        iFrameLen=len;
                        /// fModule->addVideoFrame(startFramePos,len);
                        findI=1;
                        fprintf(stderr,"[%s] [%d] find I \n",__FUNCTION__,__LINE__);
                        break;
                    }
                }
                startFramePos++;
            }
        }



        /// sps/pps均已查到，跳出循环
        if(findSps && findPps && findI){
            char dataBuf[256*1024]={'\0'};
            int dataBufLen=0;
            memcpy(dataBuf,sps,spsLen);
            dataBufLen+=spsLen;
            memcpy(&dataBuf[spsLen],pps,ppsLen);
            dataBufLen+=ppsLen;
            memcpy(&dataBuf[spsLen+ppsLen],iFrame,iFrameLen);
            dataBufLen+=iFrameLen;
            fModule->addVideoFrame(dataBuf,dataBufLen);
            fprintf(stderr,"[%s] [%d] find sps and pps %d\n",__FUNCTION__,__LINE__,spsAndppsLen);
            break;
        }
    }
    fprintf(stderr,"[%s] [%d] function is over \n",__FUNCTION__,__LINE__);
    fseek(fp,0L,SEEK_SET);
    return 1;
}

int ReadThread::getFrameLen(char *startPos,char *endPos)
{
    int dataLen=endPos-startPos;
    ///fprintf(stderr,"[%s] [%d] dataLen %d \n",__FUNCTION__,__LINE__,dataLen);
    if(dataLen<=0){
        return 0;
    }
    char *pos=startPos;
    pos+=4;
    for(int i=0;i<dataLen-4;i++){
        if((unsigned char)pos[0]==0x00
                && (unsigned char)pos[1]==0x00
                && (unsigned char)pos[2]==0x00
                && (unsigned char)pos[3]==0x01){
            int frameLen=(int)(pos-startPos);
            fprintf(stderr,"[%s] [%d] frameLen %d \n",__FUNCTION__,__LINE__,frameLen);
            return frameLen;
        }
        pos++;
    }
    return 0;
}
