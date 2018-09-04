#include "playbackdialog.h"
#include "ui_playbackdialog.h"
#include <QMouseEvent>
#include <QFile>
#include <QDomDocument>
#include "tools/proxy.h"
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

dialog_null_playbackDialog::dialog_null_playbackDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_playbackDialog)
{
  ui->setupUi(this);
  initWidget();

  ui->horizontalSlider_playbackDialog_play->setMinimum(0);
  ui->horizontalSlider_playbackDialog_play->setMaximum(100);
  ui->horizontalSlider_playbackDialog_play->setValue(0);

  startPlay=0;
  backPlay=0;
  forwardPlay=0;


  loadQssFile("qss/playback.qss");
  QObject::connect(Proxy::instance(),SIGNAL(sendMessageToPlaybackDialog(QString)),this,SLOT(recvMessageFromProxy(QString)));
  QObject::connect(this,SIGNAL(sendMessageToProxy(QString)),Proxy::instance(),SLOT(recvMessageFromSecondDialog(QString)));
  // testFile();
}

dialog_null_playbackDialog::~dialog_null_playbackDialog()
{
  delete ui;
}
bool dialog_null_playbackDialog::eventFilter(QObject *object, QEvent *e)
{
  if(object->objectName()=="shutdown"){
      if(e->type()==QEvent::MouseButtonPress){
          QMouseEvent *mouse=dynamic_cast<QMouseEvent *>(e);
          if(mouse->button()==Qt::LeftButton){
              this->hide();
            }
        }
    }
  return QDialog::eventFilter(object,e);
}
void dialog_null_playbackDialog::initWidget()
{
  QObject::connect(ui->pushButton_playbackDialog_back,SIGNAL(clicked()),this,SLOT(pushButtonPlaybackDialogBack()));
  QObject::connect(ui->pushButton_playbackDialog_play,SIGNAL(clicked()),this,SLOT(pushButtonPlaybackDialogPlay()));
  QObject::connect(ui->pushButton_playbackDialog_forward,SIGNAL(clicked()),this,SLOT(pushButtonPlaybackDialogForward()));
  QObject::connect(ui->pushButton_playbackDialog_speedUp,SIGNAL(clicked()),this,SLOT(pushButtonPlaybackDialogSpeedUp()));
  QObject::connect(ui->comboBox_playbackDialog_selectChannel,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxPlaybackDialogSelectChannel(int)));
  QObject::connect(ui->pushButton_playbackDialog_hide,SIGNAL(clicked()),this,SLOT(pushButtonPlaybackDialogHide()));
  QObject::connect(ui->horizontalSlider_playbackDialog_play,SIGNAL(valueChanged(int)),this,SLOT(sliderValueChanged(int)));

}
void dialog_null_playbackDialog::testFile()
{

  QObjectList list=this->children();

  for(int a=0;a<list.size();++a){
      //     debug("--------> %s\n",list.at(a)->objectName().toStdString().c_str());
      createFunctionAndConnection(list.at(a)->objectName());
      QObjectList list0=list.at(a)->children();
      for(int b=0;b<list0.size();++b){
          //    debug(" %s\n",list0.at(b)->objectName().toStdString().c_str());
          createFunctionAndConnection(list0.at(b)->objectName());
          QObjectList list1=list0.at(b)->children();
          for(int c=0;c<list1.size();++c){
              //           debug("== %s\n",list1.at(c)->objectName().toStdString().c_str());
              createFunctionAndConnection(list1.at(c)->objectName());
              QObjectList list2=list1.at(c)->children();
              for(int d=0;d<list2.size();++d){
                  //            debug("\t %s\n",list2.at(d)->objectName().toStdString().c_str());
                  createFunctionAndConnection(list2.at(d)->objectName());
                  QObjectList list3=list2.at(d)->children();
                  for(int e=0;e<list3.size();++e){
                      //              debug("\t\t %s\n",list3.at(e)->objectName().toStdString().c_str());
                      createFunctionAndConnection(list3.at(e)->objectName());
                      QObjectList list4=list3.at(e)->children();
                      for(int f=0;f<list4.size();++f){
                          //                 debug("\t\t\t %s\n",list4.at(f)->objectName().toStdString().c_str());
                          createFunctionAndConnection(list4.at(f)->objectName());
                        }
                    }
                }
            }
        }
    }
  return;
}

void dialog_null_playbackDialog::createFunctionAndConnection(QString objectName)
{
  QString functionName;
  functionName.clear();

  QStringList stringList=objectName.split("_");
  if(stringList.size()<3){
      return;
    }
  // debug("%s_%s_%s\n",stringList.at(0).toStdString().c_str(),stringList.at(1).toStdString().c_str(),stringList.at(2).toStdString().c_str());
  functionName.append(stringList.at(0));

  QString fatherName=stringList.at(1);
  QString selfName=stringList.at(2);

  QChar fUp=stringList.at(1).at(0).toUpper();
  QChar sUp=stringList.at(2).at(0).toUpper();

  fatherName.replace(0,1,fUp);
  selfName.replace(0,1,sUp);

  functionName.append(fatherName);
  functionName.append(selfName);

  if(stringList.at(0)=="comboBox"){

      QString functionDeclaration;
      functionDeclaration.append("void ");
      functionDeclaration.append(functionName);
      functionDeclaration.append("(int);");

      QString functionDefinition;
      functionDefinition.append("void ");
      functionDefinition.append(this->objectName());
      functionDefinition.append("::");
      functionDefinition.append(functionName);
      functionDefinition.append("(int index)");
      functionDefinition.append("\n{\n\n}\n");

      /*创建连接关系*/
      QString connection;
      connection.append("QObject::connect(ui->");
      connection.append(objectName.toStdString().c_str());
      connection.append(",SIGNAL(currentIndexChanged(int)),this,SLOT(");
      connection.append(functionName.toStdString().c_str());
      connection.append("(int)));");

      //debug("%s\n",functionDeclaration.toStdString().c_str());
      //debug("%s\n",functionDefinition.toStdString().c_str());
      //debug("%s\n",connection.toStdString().c_str());
    }else if(stringList.at(0)=="pushButton"){
      QString functionDeclaration;
      functionDeclaration.append("void ");
      functionDeclaration.append(functionName);
      functionDeclaration.append("();");

      QString functionDefinition;
      functionDefinition.append("void ");
      functionDefinition.append(this->objectName());
      functionDefinition.append("::");
      functionDefinition.append(functionName);
      functionDefinition.append("()");
      functionDefinition.append("\n{\n\n}\n");

      /*创建连接关系*/
      QString connection;
      connection.append("QObject::connect(ui->");
      connection.append(objectName.toStdString().c_str());
      connection.append(",SIGNAL(clicked()),this,SLOT(");
      connection.append(functionName.toStdString().c_str());
      connection.append("()));");

      //debug("%s\n",functionDeclaration.toStdString().c_str());
      //debug("%s\n",functionDefinition.toStdString().c_str());
      //debug("%s\n",connection.toStdString().c_str());
    }else if(stringList.at(0)=="checkBox"){
      QString functionDeclaration;
      functionDeclaration.append("void ");
      functionDeclaration.append(functionName);
      functionDeclaration.append("(int);");

      QString functionDefinition;
      functionDefinition.append("void ");
      functionDefinition.append(this->objectName());
      functionDefinition.append("::");
      functionDefinition.append(functionName);
      functionDefinition.append("(int flags)");
      functionDefinition.append("\n{\n\n}\n");

      /*创建连接关系*/
      QString connection;
      connection.append("QObject::connect(ui->");
      connection.append(objectName.toStdString().c_str());
      connection.append(",SIGNAL(stateChanged(int)),this,SLOT(");
      connection.append(functionName.toStdString().c_str());
      connection.append("(int)));");
      //debug("%s\n",functionDeclaration.toStdString().c_str());
      //debug("%s\n",functionDefinition.toStdString().c_str());
      //debug("%s\n",connection.toStdString().c_str());
    }else if(stringList.at(0)=="lineEdit"){
      QString filter;
      filter.append("ui->");
      filter.append(objectName);
      filter.append("->installEventFilter(this);");
      //debug("%s\n",filter.toStdString().c_str());
      //debug("object->objectName()== \"%s\" || ",objectName.toStdString().c_str());
    }
}
void dialog_null_playbackDialog::loadQssFile(QString fileName)
{
  QFile qssFile(fileName);
  QString qss;
  qssFile.open(QFile::ReadOnly);
  if(qssFile.isOpen()){
      qss = QLatin1String(qssFile.readAll());
      this->setStyleSheet(qss);
      qssFile.close();
    }
}
void dialog_null_playbackDialog::recvMessageFromProxy(QString message)
{
  parseXml(message);
}
void dialog_null_playbackDialog::sliderValueChanged(int value)
{

  createMessage();
}
void dialog_null_playbackDialog::createMessage()
{
  QString xmlHead="<Playback method=\"Set\">";
  QString xmlEnd="</Playback>";

  QString message;
  message.clear();
  message.append(xmlHead);
  message.append("<ChannelId>"+ui->comboBox_playbackDialog_selectChannel->currentText()+"</ChannelId>");
  QStringList rate=ui->pushButton_playbackDialog_speedUp->text().split("X");
  message.append("<PlayRate>"+rate.at(1)+"</PlayRate>");
  message.append("<StartPlay>"+QString::number(startPlay)+"</StartPlay>");
  message.append("<BackPlay>"+QString::number(backPlay)+"</BackPlay>");
  message.append("<ForwardPlay>"+QString::number(forwardPlay)+"</ForwardPlay>");
  message.append("<WherePlay>"+QString::number(ui->horizontalSlider_playbackDialog_play->value())+"</WherePlay>");
  message.append(xmlEnd);
  emit sendMessageToProxy(message);
}
void dialog_null_playbackDialog::parseXml(QString message)
{
  QDomDocument doc;
  doc.setContent(message);
  QDomElement root=doc.documentElement();
  QDomElement e=root.firstChildElement();
  QDomElement child=e.firstChildElement();
  while(!child.isNull()){
      if(child.tagName()=="SensorData"){
          QStringList list=child.text().split(":");
          int time=list.at(0).toInt();
          int value=list.at(1).toInt();

        }else if(child.tagName()=="ChannelId"){
          setComboValue(e.attribute("method"),child.text(),ui->comboBox_playbackDialog_selectChannel);
        }else if(child.tagName()=="PlayRate"){
          QString rate="X"+child.text();
          ui->pushButton_playbackDialog_speedUp->setText(rate);
        }else if(child.tagName()=="StartPlay"){

        }else if(child.tagName()=="BackPlay"){

        }else if(child.tagName()=="ForwardPlay"){

        }
      child=child.nextSiblingElement();
    }

}
void dialog_null_playbackDialog::setComboValue(QString method,QString text,QComboBox *combo)
{
  QStringList list=text.split(":");
  if(method=="Get"){
      for(int i=0;i<list.size();++i){
          combo->addItem(list.at(i));
        }
      combo->setCurrentIndex(0);
    }else if(method=="Set"){
      for(int i=0;i<combo->count();++i){
          combo->setCurrentIndex(i);
          if(text==combo->currentText()){
              combo->setCurrentIndex(i);
              break;
            }
        }
    }
}

void dialog_null_playbackDialog::pushButtonPlaybackDialogBack()
{
  if(startPlay==1){
      backPlay=1;
      forwardPlay=0;
    }
  createMessage();
}

void dialog_null_playbackDialog::pushButtonPlaybackDialogPlay()
{
  if(startPlay){
      startPlay=0;
    }else{
      startPlay=1;
    }
  createMessage();
}

void dialog_null_playbackDialog::pushButtonPlaybackDialogForward()
{
  if(startPlay==1){
      backPlay=0;
      forwardPlay=1;
    }
  createMessage();
}

void dialog_null_playbackDialog::pushButtonPlaybackDialogSpeedUp()
{
  static int speed=1;
  if(speed>4){
      speed=1;
    }
  ++speed;
  QString strSpeed="X"+speed;
  ui->pushButton_playbackDialog_speedUp->setText(strSpeed);
  createMessage();
}

void dialog_null_playbackDialog::comboBoxPlaybackDialogSelectChannel(int index)
{

}

void dialog_null_playbackDialog::pushButtonPlaybackDialogHide()
{
  this->hide();
}
