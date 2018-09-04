#include "piclayoutdialog.h"
#include "ui_piclayoutdialog.h"
#include <QMouseEvent>
#include "tools/widgetpool.h"
#include "backgroundWidget/backgroundwidget.h"
#include "tools/widgetpool.h"
#include <QFile>
#include "tools/proxy.h"
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

dialog_null_picLayoutDialog::dialog_null_picLayoutDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_picLayoutDialog)
{
  ui->setupUi(this);
  initWidget();
  messageContent.clear();
  mulPic=true;
  ifUse=false;
  QObject::connect(Proxy::instance(),SIGNAL(sendMessageToPiclayoutDialog(QString)),this,SLOT(recvMessageFromProxy(QString)));
  QObject::connect(this,SIGNAL(sendMessageToProxy(QString)),Proxy::instance(),SLOT(recvMessageFromSecondDialog(QString)));
  loadQssFile("qss/piclayout.qss");
}

dialog_null_picLayoutDialog::~dialog_null_picLayoutDialog()
{
  delete ui;
}
bool dialog_null_picLayoutDialog::eventFilter(QObject *object, QEvent *e)
{
  if(object->objectName()=="label_picLayoutDialog_shutdown"){
      if(e->type()==QEvent::MouseButtonPress){
          QMouseEvent *mouse=dynamic_cast<QMouseEvent *>(e);
          if(mouse->button()==Qt::LeftButton){
              this->hide();
            }
        }
    }
  return QDialog::eventFilter(object,e);
}

void dialog_null_picLayoutDialog::initWidget()
{
  ui->label_picLayoutDialog_shutdown->installEventFilter(this);
  QObject::connect(ui->pushButton_picLayoutDialog_picOne,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogPicOne()));
  QObject::connect(ui->pushButton_picLayoutDialog_picTwo,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogPicTwo()));
  QObject::connect(ui->pushButton_picLayoutDialog_picThree,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogPicThree()));
  QObject::connect(ui->pushButton_picLayoutDialog_picFour,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogPicFour()));
  QObject::connect(ui->pushButton_picLayoutDialog_picFive,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogPicFive()));
  QObject::connect(ui->pushButton_picLayoutDialog_picSix,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogPicSix()));
  QObject::connect(ui->pushButton_picLayoutDialog_picSeven,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogPicSeven()));
  QObject::connect(ui->comboBox_picLayoutDialog_picExchange,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxPicLayoutDialogPicExchange(int)));
  QObject::connect(ui->pushButton_picLayoutDialog_picEight,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogPicEight()));
  QObject::connect(ui->comboBox_picLayoutDialog_dataShow,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxPicLayoutDialogDataShow(int)));
  QObject::connect(ui->pushButton_picLayoutDialog_mulPicEight,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogMulPicEight()));
  QObject::connect(ui->pushButton_picLayoutDialog_mulPicFour,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogMulPicFour()));
  QObject::connect(ui->pushButton_picLayoutDialog_mulPicNine,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogMulPicNine()));
  QObject::connect(ui->pushButton_picLayoutDialog_mulPicSixteen,SIGNAL(clicked()),this,SLOT(pushButtonPicLayoutDialogMulPicSixteen()));
  QObject::connect(ui->pushButton_picLayouDialog_use,SIGNAL(clicked()),this,SLOT(pushButtonPicLayouDialogUse()));
  QObject::connect(ui->pushButton_picLayouDialog_sure,SIGNAL(clicked()),this,SLOT(pushButtonPicLayouDialogSure()));
  QObject::connect(ui->pushButton_picLayouDialog_cancel,SIGNAL(clicked()),this,SLOT(pushButtonPicLayouDialogCancel()));
}

void dialog_null_picLayoutDialog::testFile()
{

  QObjectList list=this->children();

  for(int a=0;a<list.size();++a){
      createFunctionAndConnection(list.at(a)->objectName());
      QObjectList list0=list.at(a)->children();
      for(int b=0;b<list0.size();++b){
          createFunctionAndConnection(list0.at(b)->objectName());
          QObjectList list1=list0.at(b)->children();
          for(int c=0;c<list1.size();++c){
              createFunctionAndConnection(list1.at(c)->objectName());
              QObjectList list2=list1.at(c)->children();
              for(int d=0;d<list2.size();++d){
                  createFunctionAndConnection(list2.at(d)->objectName());
                  QObjectList list3=list2.at(d)->children();
                  for(int e=0;e<list3.size();++e){
                      createFunctionAndConnection(list3.at(e)->objectName());
                      QObjectList list4=list3.at(e)->children();
                      for(int f=0;f<list4.size();++f){
                          createFunctionAndConnection(list4.at(f)->objectName());
                        }
                    }
                }
            }
        }
    }
  return;

}
void dialog_null_picLayoutDialog::createFunctionAndConnection(QString objectName)
{

  QString functionName;
  functionName.clear();

  QStringList stringList=objectName.split("_");
  if(stringList.size()<3){
      return;
    }
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
      // debug("%s\n",connection.toStdString().c_str());
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

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogPicTwo()
{
  messageContent.clear();
  messageContent.append("<Pic type=\"single\">"+QString::number(2)+"</Pic>");
  mulPic=false;
  pic=2;
}

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogPicThree()
{
  mulPic=false;
  messageContent.clear();
  messageContent.append("<Pic type=\"single\">"+QString::number(3)+"</Pic>");
  pic=3;
}

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogPicFour()
{
  mulPic=false;
  messageContent.clear();
  messageContent.append("<Pic type=\"single\">"+QString::number(4)+"</Pic>");
  pic=4;
}

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogPicFive()
{
  mulPic=false;
  messageContent.clear();
  messageContent.append("<Pic type=\"single\">"+QString::number(5)+"</Pic>");
  pic=5;
}

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogPicSix()
{
  mulPic=false;
  messageContent.clear();
  messageContent.append("<Pic type=\"single\">"+QString::number(6)+"</Pic>");
  pic=6;
}

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogPicSeven()
{
  mulPic=false;
  messageContent.clear();
  messageContent.append("<Pic type=\"single\">"+QString::number(7)+"</Pic>");
  pic=7;
}

void dialog_null_picLayoutDialog::comboBoxPicLayoutDialogPicExchange(int index)
{

}

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogPicEight()
{
  mulPic=false;
  messageContent.clear();
  messageContent.append("<Pic type=\"single\">"+QString::number(8)+"</Pic>");
  pic=8;
}

void dialog_null_picLayoutDialog::comboBoxPicLayoutDialogDataShow(int index)
{

}
void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogPicOne()
{
  mulPic=false;
  messageContent.clear();
  messageContent.append("<Pic type=\"single\">"+QString::number(1)+"</Pic>");
  pic=1;
}

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogMulPicEight()
{
  messageContent.clear();
  messageContent.append("<Pic type=\"mul\">"+QString::number(8)+"</Pic>");
  mulPic=true;
  pic=8;
}

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogMulPicFour()
{
  messageContent.clear();
  messageContent.append("<Pic type=\"mul\">"+QString::number(4)+"</Pic>");
  mulPic=true;
  pic=4;
}

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogMulPicNine()
{
  messageContent.clear();
  messageContent.append("<Pic type=\"mul\">"+QString::number(5)+"</Pic>");
  mulPic=true;
  pic=9;
}

void dialog_null_picLayoutDialog::pushButtonPicLayoutDialogMulPicSixteen()
{
  messageContent.clear();
  messageContent.append("<Pic type=\"mul\">"+QString::number(16)+"</Pic>");
  mulPic=true;
  pic=16;
}

void dialog_null_picLayoutDialog::pushButtonPicLayouDialogUse()
{
  ifUse=true;
  QString message;
  message.append("<PicLayout method=\"Set\">");
  message.append(messageContent);
  message.append("<OrderDisplay>"+ui->comboBox_picLayoutDialog_picExchange->currentText()+"</OrderDisplay>");
  message.append("<DataDisplay>"+ui->comboBox_picLayoutDialog_dataShow->currentText()+"</DataDisplay>");
  message.append("</PicLayout>");

  emit sendMessageToProxy(message);
  messageContent.clear();
  mulPic=true;
  pic=0;
}

void dialog_null_picLayoutDialog::pushButtonPicLayouDialogSure()
{
  if(!ifUse){
      pushButtonPicLayouDialogUse();
    }
  this->hide();
  ifUse=false;
}

void dialog_null_picLayoutDialog::pushButtonPicLayouDialogCancel()
{
  ifUse=false;
  this->hide();
}
void dialog_null_picLayoutDialog::loadQssFile(QString fileName)
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

void dialog_null_picLayoutDialog::recvMessageFromProxy(QString message)
{
  parseXml(message);
}
void dialog_null_picLayoutDialog::parseXml(QString message)
{
  QDomDocument doc;
  doc.setContent(message);
  QDomElement root=doc.documentElement();
  QDomElement e=root.firstChildElement();
  QDomElement child=e.firstChildElement();
  while(!child.isNull()){
      if(child.tagName()=="Pic"){

          if(child.attribute("type")=="mul"){
              mulPic=true;
            }else if(child.attribute("type")=="single"){
              mulPic=false;
            }
          pic=child.text().toInt();
          dialog_null_backGroundWidget *bWidget=(dialog_null_backGroundWidget *)(*(WidgetPool::topWidget.find("dialog_null_backGroundWidget")));
          if(mulPic){
              if(pic==4){
                  bWidget->fourPicLayout();
                }else if(pic==8){
                  bWidget->eightPicLayout();
                }else if(pic==9){
                  bWidget->ninePicLayout();
                }else if(pic==16){
                  bWidget->sixteenPicLayout();
                }
            }else{
              bWidget->onePicLayout(pic);
            }
        }else if(child.tagName()=="OrderDisplay"){
          setComboValue(e.attribute("method"),child.text(),ui->comboBox_picLayoutDialog_picExchange);
        }else if(child.tagName()=="DataDisplay"){
          setComboValue(e.attribute("method"),child.text(),ui->comboBox_picLayoutDialog_dataShow);
        }else if(child.tagName()=="Code"){
          if(child.text().toInt()==200){
              dialog_null_backGroundWidget *bWidget=(dialog_null_backGroundWidget *)(*(WidgetPool::topWidget.find("dialog_null_backGroundWidget")));
              if(mulPic){
                  if(pic==4){
                      bWidget->fourPicLayout();
                    }else if(pic==8){
                      bWidget->eightPicLayout();
                    }else if(pic==9){
                      bWidget->ninePicLayout();
                    }else if(pic==16){
                      bWidget->sixteenPicLayout();
                    }
                }else{
                  bWidget->onePicLayout(pic);
                }
            }
        }
      child=child.nextSiblingElement();
    }
}
void dialog_null_picLayoutDialog::setComboValue(QString method,QString text,QComboBox *combo)
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
