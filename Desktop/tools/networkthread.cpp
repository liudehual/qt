#include "networkthread.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include <QVariant>
#include <QTcpSocket>
#include <stdio.h>
#include <stdlib.h>
#include "tools/proxy.h"
#define DEBUG 2
#if (DEBUG==0 || DEBUG==1)
#include <stdio.h>
#include <stdlib.h>
#endif
#if (DEBUG==0)
#define debug(fmt,...) fprintf(stderr," "fmt"File: %s Function:%s LineNumber:%d",__VA_ARGS__,__FILE__,__FUNCTION__,__LINE__)
#elif (DEBUG==1)
#define debug(fmt,...) fprintf(stderr,fmt,__VA_ARGS__)
#elif (DEBUG==2)
#define debug(fmt,...)
#endif

NetworkThread::NetworkThread(QObject *parent):QThread(parent)
{
  p=Proxy::instance();
  QObject::connect(p,SIGNAL(sendMessageToThread(QString)),this,SLOT(recvMessageFromProxy(QString)));
  QObject::connect(this,SIGNAL(sendMessageToProxy(QString)),p,SLOT(recvMessageFromThread(QString)));

  m_SocketServer=new QTcpServer;
  m_SocketServer->listen(QHostAddress::Any, 55555);
  QObject::connect(m_SocketServer, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
  connected=false;
  messageBuffer.clear();
  timeout=new QTimer;
//  QObject::connect(timeout,SIGNAL(timeout()),this,SLOT(timeToCheckMessage()));
//  timeout->start(33);
}

NetworkThread::~NetworkThread()
{
  delete m_SocketServer;
  delete timeout;
}

void NetworkThread::run()
{
exec();
}
void NetworkThread::acceptConnection()
{
  debug("%s\n","NetworkThread::acceptConnection");
  m_Socket=m_SocketServer->nextPendingConnection();
  connected=true;
  QObject::connect(m_Socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
  QObject::connect(m_Socket,SIGNAL(disconnected()),this,SLOT(disconnect()));
}
void NetworkThread::readMessage()
{
  QByteArray message=m_Socket->readAll();
  QString sendMessag;
  sendMessag.prepend(message);
  debug("NetworkThread::readMessage %s\n",sendMessag.toStdString().c_str());
  emit sendMessageToProxy(sendMessag);
}
void NetworkThread::recvMessageFromProxy(QString message)
{
  if(!connected){
      return;
    }
  mutex.lock();
  messageBuffer.append(message);
  mutex.unlock();
}
void NetworkThread::timeToCheckMessage()
{

  static QString tmpMessage;
  static int hSend;
  if(!connected || (tmpMessage.isEmpty() && messageBuffer.isEmpty())){
      return;
    }

  if(tmpMessage.isEmpty() && !messageBuffer.isEmpty()){
      QVector<QString>::iterator it;
      mutex.lock();
      it=messageBuffer.begin();
      tmpMessage=*it;
      messageBuffer.erase(it);
      hSend=0;
      mutex.unlock();
    }
  if(hSend==tmpMessage.size()){
      tmpMessage.clear();
      hSend=0;
    }
  int toSendLen=tmpMessage.size()-hSend;
  QString toSendMessage=tmpMessage.mid(hSend,toSendLen);
  if(toSendLen!=toSendMessage.size()){
      return;
    }

  int sendNum=m_Socket->write(toSendMessage.toAscii(),toSendLen);
  if(sendNum>=0){
        hSend+=sendNum;
    }else if(sendNum<0){
        connected=false;
    }

}
void NetworkThread::disconnect()
{
    connected=false;
}
