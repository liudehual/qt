#include <QString>
#include <stdio.h>
#include <stdlib.h>

#include "proxy.h"
#include "base64.h"
Proxy::Proxy():QObject()
{
    tEdit1=NULL;
    tEdit2=NULL;
}
Proxy::~Proxy()
{
    tEdit1=NULL;
    tEdit2=NULL;
}

void Proxy::encoderMD5Slot(bool)
{
    if(tEdit1==NULL || tEdit2==NULL) return;

    QString source=tEdit1->toPlainText();
     if(source.isEmpty())return;

    QByteArray ba = source.toLatin1();
    char *src=ba.data();

    MD5_CTX context;
    MD5_Init(&context); //初始化 MD5上下文环境
    MD5_Update(&context,(unsigned char *)src,strlen(src)); //更新数据
    char dest[32]={0};
    MD5_Final((unsigned char *)dest,&context);  //加密结束并将结果放到dest中

     fprintf(stderr,"--%d\n",strlen(dest));
    QString destStr(dest);
    tEdit2->setText(destStr);
}
void Proxy::encoderBase64Slot(bool)
{
    if(tEdit1==NULL || tEdit2==NULL) return;

    QString source=tEdit1->toPlainText();
     if(source.isEmpty())return;

     QByteArray ba = source.toLatin1();
    char *src=ba.data();

    base64 b64;
    char dest[1024*2];
    b64.Base64encode(dest,src,strlen(src));
    QString QDest(dest);
    tEdit2->setText(QDest);
}
void Proxy::decoderBase64Slot(bool)
{
    if(tEdit1==NULL || tEdit2==NULL) return;

    QString source=tEdit2->toPlainText();
    if(source.isEmpty())return;

    QByteArray ba = source.toLatin1();
    char *src=ba.data();

    base64 b64;
    char dest[1024*2];
    b64.Base64decode(dest,src);
    QString QDest(dest);
    tEdit1->setText(QDest);
}
