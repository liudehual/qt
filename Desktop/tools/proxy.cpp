#include "proxy.h"
#include <QStackedWidget>
#include "rightWidget/systemSetDialog.h"
#include <QDomDocument>

#define DEBUG 1
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
#include <QTimer>
Proxy *Proxy::pro=NULL;
Proxy::Proxy(QObject *parent) : QObject(parent)
{
  oldString.clear();
  //test
  timer=new QTimer;
  // QObject::connect(timer,SIGNAL(timeout()),this,SLOT(recvMessageFromTimer()));
  timer->start(1000);
}
Proxy::~Proxy()
{

}
Proxy *Proxy::instance()
{
  if(pro==NULL){
      pro=new Proxy;
    }
  return pro;
}
void Proxy::recvMessageFromThread(QString message)
{
  QStringList list=message.split("!end!");
  if(list.size()==0){
      return;
    }
  if(list.size()==1){
      oldString.append(list.at(0));
      return;
    }
  if(list.size()>1){
      for(int i=0;i<list.size()-1;++i){
          oldString.append(list.at(i));
          //添加XML解析
          QDomDocument doc;
          doc.setContent(oldString);
          QDomElement docElem = doc.documentElement();  //返回根元素
          QDomNode n = docElem.firstChild();   //返回根节点的第一个子节点
          while(!n.isNull()) {   //如果节点不为空
              if (n.isElement()){//如果节点是元素
                  QDomElement e = n.toElement();  //将其转换为元素
                  sendMessageToDialog(e.tagName(),oldString);
                }
              n = n.nextSibling();  //下一个兄弟节点
            }
          oldString.clear();
        }
      oldString.append(list.at(list.size()-1));
    }
}
void Proxy::recvMessageFromSecondDialog(QString message)
{
  QString xml;
  xml.append("<?xml version='1.0' encoding='UTF-8'?>");
  xml.append("<Request>");
  xml.append(message);
  xml.append("</Request>");
  xml.append("!end!");
  emit sendMessageToThread(xml);
}
void Proxy::sendMessageToDialog(QString objectName,QString string)
{
  if(objectName=="SystemSet"){
      emit sendMessageToSystemSetDialog(string);
    }else if(objectName=="YunTaiControl"){
      emit sendMessageToYunTaiDialog(string);
    }else if(objectName=="PlayBack"){

    }else if(objectName=="Sensor"){

    }else if(objectName=="Controller"){
      emit sendMessageToControllerDialog(string);
    }else if(objectName=="PicLayout"){

    }else if(objectName=="Shutdown"){

    }else if(objectName=="SensorData"){

    }
}
void Proxy::recvMessageFromTimer()
{
  QString Message="<YunTai>"
                  " <Up>2</Up>"
                  "<Down>2</Down>"
                  "<Left>2</Left>"
                  "<Right>2</Right>"
                  "<!-- 光圈-->"
                  "<AddAperture>2</AddAperture>"
                  "<SubAperture>2</SubAperture>"
                  "<!-- 变焦-->"
                  "<AddZoom>2</AddZoom>"
                  "<SubZoom>2</SubZoom>"
                  "<!-- 变倍-->"
                  "<AddZoomed>2</AddZoomed>"
                  "<SubZoomed>2</SubZoomed>"
                  "<ChannelId>2</ChannelId>"
                  "<Rate>2</Rate>"
                  "</YunTai>";
  QString xml;
  xml.append("<?xml version='1.0' encoding='UTF-8'?>");
  xml.append("<Request>");
  xml.append(Message);
  xml.append("</Request>");
  xml.append("!end!");
  emit sendMessageToThread(xml);
}
