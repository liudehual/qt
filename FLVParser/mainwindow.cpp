#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include "flvtag.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QBrush>
#include <math.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    flvThread(NULL)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("FLV格式分析程序 "));

    flvThread=new FVLParserThread;

    QObject::connect(ui->actionAbout_FLVParser,SIGNAL(triggered()),this,SLOT(AboutFLVParser()));
    QObject::connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(Exit()));
    QObject::connect(ui->actionAbout_Qt,SIGNAL(triggered()),this,SLOT(AboutQt()));
    QObject::connect(ui->actionOpen_File,SIGNAL(triggered()),this,SLOT(OpenFile()));
    QObject::connect(this,SIGNAL(sendFileName(QString)),flvThread,SLOT(RecvFileName(QString)));

    QObject::connect(flvThread,SIGNAL(updateListView()),this,SLOT(UpdateListView()));

    ui->textEdit_2->setReadOnly(true);
    QColor c;
    c.setRgb(100,200,100);
    ui->textEdit_2->setTextBackgroundColor(c);

    ui->textEdit->setReadOnly(true);
    QColor c1;
    c1.setRgb(100,200,100);
    ui->textEdit->setTextBackgroundColor(c1);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::OpenFile()
{

    QString fileName =QFileDialog::getOpenFileName(this,
                                                   tr("open file"),
                                                   " ",
                                                   tr("All Files(*.*);;Flv File(*.flv)")
                                                   );
    qDebug()<<fileName;
    sendFileName(fileName);
}
void MainWindow::Exit()
{
    qApp->exit();
}
void MainWindow::AboutFLVParser()
{
    QMessageBox::about(this,
                       "AboutFLVParser",
                       "Created by GCT \n"
                       "Time:2017-02-08"
                       );
}
void MainWindow::AboutQt()
{
    QMessageBox::aboutQt(this,
                         "About Qt"
                         );
}
void MainWindow::UpdateListView()
{
    QStandardItemModel  *standardItemModel = new QStandardItemModel(this);

    qint64 tagNum=flvThread->GetTagQueueLength();
    for(int i=0;i<tagNum;i++){
        FLVTag *tag=flvThread->GetTagByIndex(i);
        QString itemContent;
        itemContent+=QString::number(i,10);
        itemContent+="\t";
        if(tag->fTagHeader.fTagType==0x08){
            itemContent+="Audio Tag";
        }else if(tag->fTagHeader.fTagType==0x09){
            itemContent+="Video Tag";
        }else if(tag->fTagHeader.fTagType==0x12){
            itemContent+="Script Tag";
        }
#if 0
        itemContent+="Tag StringId ";
        itemContent+=QString::number(tag->fTagHeader.fStreamId,10);
        itemContent+="\t Tag Len ";
        itemContent+=QString::number(tag->fTagHeader.fTagLen,10);
        itemContent+="\t Tag Type ";
        itemContent+=QString::number(tag->fTagHeader.fTagType,10);
        itemContent+="\t Tag TimeStamp ";
        itemContent+=QString::number(tag->fTagHeader.fTimeStamp,10);
#endif

        QStandardItem *item = new QStandardItem(itemContent);
        item->setEditable(false);
        QLinearGradient linearGrad(QPointF(0, 0), QPointF(200, 200));
        if(tag->fTagHeader.fTagType==0x08){
            linearGrad.setColorAt(0, Qt::darkGreen);
        }else if(tag->fTagHeader.fTagType==0x09){
            linearGrad.setColorAt(1, Qt::yellow);
        }else if(tag->fTagHeader.fTagType==0x12){
        }
        QBrush brush(linearGrad);
        item->setBackground(brush);
        standardItemModel->appendRow(item);

    }
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),this,SLOT(ParseSpecialTag(QModelIndex)));
    ui->listView->setModel(standardItemModel);
}
void MainWindow::ParseSpecialTag(QModelIndex i)
{
    QString str=i.data().toString();
    QString index=str.section('\t',0,0);
    int intIndex=index.toInt();
    FLVTag *tag=flvThread->GetTagByIndex(intIndex);

    QByteArray str2;
    for(int j=0;j<tag->fTagHeader.fTagLen;j++){
        str2.append(tag->fTagBuf[j]);
    }

    ui->textEdit->setText(str2.toHex().toUpper().data());

    unsigned char mediaInfo=tag->fTagBuf[0];

    if(tag->fTagHeader.fTagType==0x08){
        ParseAudio(mediaInfo,tag);
    }else if(tag->fTagHeader.fTagType==0x09){
        ParseVideo(mediaInfo,tag);
    }else if(tag->fTagHeader.fTagType==0x12){
        ParseScript(mediaInfo,tag);
    }
}
void MainWindow::ParseAudio(unsigned char first,FLVTag *tTag)
{
    qDebug()<<"ParseAudio";
    QString format("音频格式: ");
    QString samplingRate("采样频率: ");
    QString sampleRateLen("采样长度: ");
    QString audioType("音频长度: ");

    unsigned char af=first>>4 & 0x0F;
    unsigned char sr=first>>2 & 0x03;
    unsigned char srl=first>>1 & 0x01;
    unsigned char ay=first & 0x01;
    qDebug()<<"format "<<af;

    /* 音频格式 */
    switch(af){
    case 0:{
        format+="Linear PCM, platform endian";
        qDebug()<<"Linear PCM, platform endian";
        break;
    }
    case 1:{
        format+="ADPCM";
        qDebug()<<"ADPCM";
        break;
    }
    case 2:{
        format+="MP3";
        qDebug()<<"MP3";
        break;
    }
    case 3:{
        format+="Linear PCM, little endian";
        qDebug()<<"Linear PCM, little endian";
        break;
    }
    case 4:{
        format+="Nellymoser 16-kHz mono";
        qDebug()<<"Nellymoser 16-kHz mono";
        break;
    }
    case 5:{
        format+="Nellymoser 8-kHz mono";
        qDebug()<<"Nellymoser 8-kHz mono";
        break;
    }
    case 6:{
        format+="Nellymoser";
        qDebug()<<"Nellymoser";
        break;
    }
    case 7:{
        format+="G.711 A-law logarithmic PCM";
        qDebug()<<"G.711 A-law logarithmic PCM";
        break;
    }
    case 8:{
        format+="G.711 mu-law logarithmic PCM";
        qDebug()<<"G.711 mu-law logarithmic PCM";
        break;
    }
    case 9:{
        format+="reserved";
        qDebug()<<"reserved";
        break;
    }
    case 10:{
        format+="AAC";
        qDebug()<<"AAC";
        break;
    }
    case 11:{
        format+="Speex";
        qDebug()<<"Speex";
        break;
    }
    case 12:{
        qDebug()<<"MP3";
        break;
    }
    case 13:{
        qDebug()<<"MP3";
        break;
    }
    case 14:{
        format+="MP3 8-Khz";
        qDebug()<<"MP3 8-Khz";
        break;
    }
    case 15:{
        format+="Device-specific sound";
        qDebug()<<"Device-specific sound";
        break;
    }
    default:{
        break;
    }
    }
    qDebug()<<format;
    /* 采样率 */
    qDebug()<<"sr "<<sr;
    switch(sr){
    case 0:{
        samplingRate+="5.5-kHz";
        qDebug()<<"5.5-kHz";
        break;
    }
    case 1:{
        samplingRate+="11-kHz";
        qDebug()<<"11-kHz";
        break;
    }
    case 2:{
        samplingRate+="22-kHz";
        qDebug()<<"22-kHz";
        break;
    }
    case 3:{
        samplingRate+="44-kHz";
        qDebug()<<"44-kHz";
        break;
    }
    default:{
        break;
    }
    }
    qDebug()<<samplingRate;
    qDebug()<<"srl "<<srl;
    /* 采样长度 */
    switch(srl){
    case 0:{
        sampleRateLen+="snd8Bit";
        qDebug()<<"snd8Bit";
        break;
    }
    case 1:{
        sampleRateLen+="snd16Bit";
        qDebug()<<"snd16Bit";
        break;
    }

    }
    qDebug()<<sampleRateLen;
    qDebug()<<"ay "<<ay;
    /* 采样长度 */
    switch(ay){
    case 0:{
        audioType+="sndMono";
        qDebug()<<"sndMono";
        break;
    }
    case 1:{
        audioType+="sndStereo";
        qDebug()<<"sndStereo";
        break;
    }

    }
    qDebug()<<audioType;

    QString editStr;

    QString tagType("Tag 类型: 音频");
    editStr+=tagType;
    editStr+="\n";

    QString tagLen("Tag 长度: ");
    tagLen+=QString::number(tTag->fTagHeader.fTagLen,10);
    editStr+=tagLen;
    editStr+="\n";

    QString tagTimeStamp("时间戳值：");
    tagTimeStamp+=QString::number(tTag->fTagHeader.fTimeStamp,10);
    editStr+=tagTimeStamp;
    editStr+="\n";

    QString tagStreamId("流ID: ");
    tagStreamId+=QString::number(tTag->fTagHeader.fStreamId,10);
    editStr+=tagStreamId;
    editStr+="\n";

    editStr+=format;
    editStr+="\n";
    editStr+=samplingRate;
    editStr+="\n";
    editStr+=sampleRateLen;
    editStr+="\n";
    editStr+=audioType;


    ui->textEdit_2->setText(editStr.toUtf8());
}
void MainWindow::ParseVideo(unsigned char first,FLVTag *tTag)
{
    qDebug()<<"ParseVideo";

    QString frameType("数据类型: ");
    QString codeId("编码ID: ");

    unsigned char ft=first>>4 & 0x0F;
    unsigned char ci=first & 0x0F;

    switch(ft){
    case 1:{
        frameType+="keyframe (for AVC, a seekable frame)";
        break;
    }
    case 2:{
        frameType+="inter frame (for AVC, a non-seekable frame)";
        break;
    }
    case 3:{
        frameType+="disposable inter frame (H.263 only)";
        break;
    }
    case 4:{
        frameType+="generated keyframe (reserved for server use only)";
        break;
    }
    case 5:{
        frameType+="video info/command frame";
        break;
    }
    }
    qDebug()<<frameType;
    switch(ci){
    case 1:{
        codeId+="JPEG (currently unused)";
        break;
    }
    case 2:{
        codeId+="Sorenson H.263";
        break;
    }
    case 3:{
        codeId+="Screen video";
        break;
    }
    case 4:{
        codeId+="On2 VP6";
        break;
    }
    case 5:{
        codeId+="On2 VP6 with alpha channel";
        break;
    }
    case 6:{
        codeId+="Screen video version 2";
        break;
    }
    case 7:{
        codeId+="AVC";
        break;
    }

    }

    QString editStr;

    QString tagType("Tag 类型: 视频");
    editStr+=tagType;
    editStr+="\n";

    QString tagLen("Tag 长度: ");
    tagLen+=QString::number(tTag->fTagHeader.fTagLen,10);
    editStr+=tagLen;
    editStr+="\n";

    QString tagTimeStamp("时间戳值：");
    tagTimeStamp+=QString::number(tTag->fTagHeader.fTimeStamp,10);
    editStr+=tagTimeStamp;
    editStr+="\n";

    QString tagStreamId("流ID: ");
    tagStreamId+=QString::number(tTag->fTagHeader.fStreamId,10);
    editStr+=tagStreamId;
    editStr+="\n";
    editStr+=frameType;
    editStr+="\n";
    editStr+=codeId;

    ui->textEdit_2->setReadOnly(true);
    ui->textEdit_2->setText(editStr);
}
double char2Double(unsigned char*input,int len)
{
    double tmp=2;
    double output=0;
    double a = input[0];
    for (int i=1;i<len;i++){
        a = input[i] * ::pow(tmp,8*i);
        output += a;
    }
    return output;
}
unsigned int getbitu(const unsigned char *buff, int pos, int len)
{

    unsigned int bits=0;
    int i;
    for (i=pos;i<pos+len;i++){
        bits=(bits<<1)+((buff[i/8]>>(7-i%8))&1u);
    }
    return bits;
}

double HexToDouble(const unsigned char* buf)
{
    double tmpValue=2;
    double value = 0;
    unsigned int i = 0;
    unsigned int num,temp;
    int num2;
    bool flags1 = false;
    bool flags2 = false;

    num = getbitu(buf,i,1);             i += 1;
    num2 = getbitu(buf,i,11) - 1023;    i += 11;

    if(num2 >= 0){
        flags2 = true;
        while(1){
            flags1 = true;
            if(flags2){
                flags2 = false;
                value += 1 * pow(tmpValue,num2); num2--;
            }

            temp = getbitu(buf,i,1);    i += 1;
            value += temp * pow(tmpValue,num2); num2--;
            if(num2 < 0 || i == 64);
            break;
        }
    }
    while(1){
        if(flags1){
            temp = getbitu(buf,i,1);    i += 1;
            value += temp * pow(tmpValue,num2); num2--;
        }else{
            flags1 = true;
            value += 1 * pow(tmpValue,num2); num2--;
        }
        if(i == 64)
            break;
    }

    if(num == 1)
        value *= -1;

    return value;
}
void MainWindow::ParseScript(unsigned char first,FLVTag *tTag)
{
    QString scriptType("脚本格式: ");
    switch(first){
    case 0:{
        scriptType+="Number";
        break;
    }
    case 1:{
        scriptType+="Boolean";
        break;
    }
    case 2:{
        scriptType+="String";
        break;
    }
    case 3:{
        scriptType+="Object";
        break;
    }
    case 4:{
        scriptType+="MovieClip (reserved, not supported)";
        break;
    }
    case 5:{
        scriptType+="Null";
        break;
    }
    case 6:{
        scriptType+="Undefined";
        break;
    }
    case 7:{
        scriptType+="Reference";
        break;

    }
    case 8:{
        scriptType+="ECMA array";
        break;
    }
    case 9:{
        scriptType+="Object end marker";
        break;
    }
    case 10:{
        scriptType+="Strict array";
        break;
    }
    case 11:{
        scriptType+="Date";
        break;
    }
    case 12:{
        scriptType+="Long string";
        break;
    }

    }
    qDebug()<<"ParseScript          "<<scriptType;

    unsigned char *amf1=tTag->fTagBuf;

    /* 解析AFM1 */
    QString amfFirstPacket("");
    QString amfFirstPacketLen("AMF1 数据长度: ");

    unsigned short firstAMFLen=amf1[1]<<8 | amf1[2];

    amfFirstPacket+=amfFirstPacketLen;
    amfFirstPacket+=QString::number(firstAMFLen,10);
    amfFirstPacket+="\n";

    char buf[32]={0};
    memcpy(buf,&amf1[3],firstAMFLen);
    QString onMetaData = QString(QLatin1String(buf));;
    QString dstMetaData("onMetaData");

    /* 只解析 "onMetaData" */
    if(onMetaData!=dstMetaData){
        return;
    }

    QString afmFirstOnMetaData("AMF1 数据: ");
    amfFirstPacket+=afmFirstOnMetaData;
    amfFirstPacket+=onMetaData;
    amfFirstPacket+="\n";

    /* 解析AFM2 */
    unsigned char *amf2=&(tTag->fTagBuf[13]);
    qDebug()<<"amf2 Type "<<amf2[0];
    unsigned int elemNum=amf2[1]<<24 | amf2[2]<<16 | amf2[3]<<8 | amf2[4];
    qDebug()<<"Elem Numbers "<<elemNum;

    /* 分析script脚本内容 */
#if 1
    /* 解析脚本数组 */
    unsigned char *arrayStart=&amf2[5];

    QString allScriptInfo;
    allScriptInfo+="AMF2 数据: \n";
    for(int elemIndex=0;elemIndex<elemNum;elemIndex++){

        /*
            ---------------------------------------
            | 字符串长度 | 字符串内容(键) | 值类型 | 值(值)   |
            ---------------------------------------

        */
        QString itemInfo;
        unsigned short strLen=arrayStart[0] <<8 | arrayStart[1];
        arrayStart+=2;
        qDebug()<<"Str Len "<<strLen;
        char str[128]={0};
        if(strLen>128){
            memcpy(str,arrayStart,32);
        }else{
            memcpy(str,arrayStart,strLen);
        }
        itemInfo+=str;
        itemInfo+="    ";
        qDebug()<<"String ---->"<<str;
        arrayStart+=strLen;

        unsigned char valueType=arrayStart[0];
        arrayStart+=1;

        /*
            0 = Number
            1 = Boolean
            2 = String
            3 = Object
            4 = MovieClip (reserved, not supported)

            5 = Null
            6 = Undefined
            7 = Reference
            8 = ECMA array
            9 = Object end marker

            10 = Strict array

            11 = Date
            12 = Long string
        */
        switch(valueType){
        case 0x00:{ /* double */
            // double value=char2Double(arrayStart,8);
            double  value=HexToDouble((const unsigned char *)arrayStart);
            arrayStart+=8;

            fprintf(stderr,"Double %lf\n",value);
            itemInfo+=QString::number(value,10,8);

            break;
        }
        case 0x01:{ /* unsigned char */
            unsigned char value=arrayStart[0];
            arrayStart+=1;
            qDebug()<<"unsigned char "<<value;
            itemInfo+=QString::number(value,10);
            break;
        }
        case 0x02:{ /* SCRIPTDATASTRING */
            unsigned short strLen=arrayStart[0]<<8 | arrayStart[1];
            char strValue[32]={0};
            arrayStart+=2;
            memcpy(strValue,arrayStart,strLen);
            arrayStart+=strLen;
            qDebug()<<"SCRIPTDATASTRING "<<strValue;
            itemInfo+=strValue;
            break;
        }
        case 0x03:{ /* SCRIPTDATAOBJECT 值->3字节*/
            unsigned int dataObjectValue=arrayStart[0]<<16 | arrayStart[1]<<8 | arrayStart[2];
            arrayStart+=3;
            qDebug()<<"SCRIPTDATAOBJECT "<<dataObjectValue;
            itemInfo+=QString::number(dataObjectValue,10);
            break;
        }
        case 0x04:{ /* undefine */
            break;
        }
        case 0x05:{ /* undefine */
            break;
        }
        case 0x06:{ /* undefine */
            break;
        }
        case 0x07:{ /* unsignged short */
            unsigned short shortValue=arrayStart[0]<<8 | arrayStart[1];
            arrayStart+=2;
            qDebug()<<"unsignged short "<<shortValue;
            itemInfo+=QString::number(shortValue,10);
            break;
        }
        case 0x08:{ /* SCRIPTDATAECMAARRAY */
            unsigned int valueLen=arrayStart[0]<<24 | arrayStart[1]<<16 | arrayStart[2]<<8 | arrayStart[3];
            arrayStart+=4;
            arrayStart+=valueLen; /* 跳过,不做处理 */
            break;
        }
        case 0x09:{ /*  */
            break;
        }
        case 0x0A:{ /* SCRIPTDATASTRICTARRAY  */
            break;
        }
        case 0x0B:{ /* SCRIPTDATADATE */
            long long value=(long long)(arrayStart[0]<<56 | arrayStart[1] << 48
                              | arrayStart[2]<<40 | arrayStart[3]<<32 | arrayStart[4]<<24 | arrayStart[5]<<16
                                                                                                                                                       | arrayStart[6]<<8  | arrayStart[7]);
            arrayStart+=8;
            qDebug()<<"double "<<value;
            itemInfo+=QString::number(value,10);
            break;
        }
        case 0x0C:{ /* SCRIPTDATALONGSTRING */
            unsigned int valueLen=arrayStart[0]<<24 | arrayStart[1]<<16 | arrayStart[2]<<8 | arrayStart[3];
            arrayStart+=4;
            arrayStart+=valueLen; /* 跳过,不做处理 */
            break;
        }
        }
        itemInfo+="\n";
        allScriptInfo+=itemInfo;
    }
#endif

    /* 合并信息 */
    QString amfPacket;
    amfPacket+=scriptType;
    amfPacket+="\n";
    amfPacket+=amfFirstPacket;
    amfPacket+="\n";
    amfPacket+=allScriptInfo;
    // qDebug()<<"AMF Packet "<<amfPacket<<"amf1 Packet "<<amfFirstPacket;
    ui->textEdit_2->setText(amfPacket);
}
