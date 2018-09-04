#include "controllerdialog.h"
#include "ui_controllerdialog.h"
#include <QMouseEvent>
#include "controllersetdialog.h"
#include <QFile>
#include <QDesktopWidget>
#include "keyboard/softkeyboard.h"
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
dialog_null_controllerDialog::dialog_null_controllerDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_controllerDialog)
{
  ui->setupUi(this);
  /*设置背景色*/
  QPalette palette(this->palette());
  palette.setColor(QPalette::Background, Qt::white);
  this->setPalette(palette);
  initWidget();
  childMap.clear();
  widgetMap.clear();
  loadQssFile("qss/controller.qss");
  QObject::connect(Proxy::instance(),SIGNAL(sendMessageToControllerDialog(QString)),this,SLOT(recvMessageFromProxy(QString)));
  QObject::connect(this,SIGNAL(sendMessageToProxy()),Proxy::instance(),SLOT(recvMessageFromSecondDialog(QString)));
}

dialog_null_controllerDialog::~dialog_null_controllerDialog()
{
  delete ui;

}
bool dialog_null_controllerDialog::eventFilter(QObject *object, QEvent *e)
{
  if(object->objectName()=="label_controllerDialog_shutdown"){
      if(e->type()==QEvent::MouseButtonPress){
          QMouseEvent *mouse=dynamic_cast<QMouseEvent *>(e);
          if(mouse->button()==Qt::LeftButton){
              this->hide();
            }
        }
    }
  if(e->type()==QEvent::MouseButtonPress){
      if(object->objectName()== "lineEdit_controllerDialog_controllerName_1" || object->objectName()== "lineEdit_controllerDialog_controllerName_2" ||
         object->objectName()== "lineEdit_controllerDialog_controllerName_3" || object->objectName()== "lineEdit_controllerDialog_controllerName_4"){
          //debug("%s\n","=============");
          QLineEdit *lEdit=dynamic_cast<QLineEdit *>(object);
          int width=QApplication::desktop()->width();
          int height=QApplication::desktop()->height();
          QPoint point=QCursor::pos();;
          int x=point.x();
          int y=point.y()+20;
          SoftKeyboard *sk=SoftKeyboard::instance();
          sk->hide();
          if(width-x>=sk->width()){
              sk->move(x-20,y);
            }else{
              sk->move(width-sk->width()-20,y);
            }
          sk->show();
          sk->setLineText(lEdit);
        }
    }
  return QDialog::eventFilter(object,e);
}
void dialog_null_controllerDialog::initWidget()
{

  ui->label_controllerDialog_shutdown->installEventFilter(this);
  QObject::connect(ui->comboBox_controllerDialog_controller_1,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxControllerDialogControllerOne(int)));
  QObject::connect(ui->comboBox_controllerDialog_controller_2,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxControllerDialogControllerTwo(int)));
  QObject::connect(ui->comboBox_controllerDialog_controller_3,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxControllerDialogControllerThree(int)));
  QObject::connect(ui->comboBox_controllerDialog_controller_4,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxControllerDialogControllerFour(int)));
  QObject::connect(ui->pushButton_controllerDialog_sure,SIGNAL(clicked()),this,SLOT(pushButtonControllerDialogSure()));
  QObject::connect(ui->pushButton_controllerDialog_cancel,SIGNAL(clicked()),this,SLOT(pushButtonControllerDialogCancel()));
  QObject::connect(ui->pushButton_controllerDialog_use,SIGNAL(clicked()),this,SLOT(pushButtonControllerDialogUse()));

  ui->lineEdit_controllerDialog_controllerName_1->installEventFilter(this);
  ui->lineEdit_controllerDialog_controllerName_2->installEventFilter(this);
  ui->lineEdit_controllerDialog_controllerName_3->installEventFilter(this);
  ui->lineEdit_controllerDialog_controllerName_4->installEventFilter(this);

  //   testFile();
}
void dialog_null_controllerDialog::testFile()
{

  QObjectList list=this->children();

  for(int a=0;a<list.size();++a){
      QObjectList list=this->children();

      for(int a=0;a<list.size();++a){
          widgetMap.insert(list.at(a)->objectName(),list.at(a));
          //  createFunctionAndConnection(list.at(a)->objectName());
          QObjectList list0=list.at(a)->children();
          for(int b=0;b<list0.size();++b){
              widgetMap.insert(list0.at(b)->objectName(),list0.at(b));

              // createFunctionAndConnection(list0.at(b)->objectName());
              QObjectList list1=list0.at(b)->children();
              for(int c=0;c<list1.size();++c){
                  widgetMap.insert(list1.at(c)->objectName(),list1.at(c));

                  //  createFunctionAndConnection(list1.at(c)->objectName());
                  QObjectList list2=list1.at(c)->children();
                  for(int d=0;d<list2.size();++d){
                      widgetMap.insert(list2.at(d)->objectName(),list2.at(d));
                      // createFunctionAndConnection(list2.at(d)->objectName());
                      QObjectList list3=list2.at(d)->children();
                      for(int e=0;e<list3.size();++e){
                          widgetMap.insert(list3.at(e)->objectName(),list3.at(e));
                          //  createFunctionAndConnection(list3.at(e)->objectName());
                          QObjectList list4=list3.at(e)->children();
                          for(int f=0;f<list4.size();++f){
                              widgetMap.insert(list4.at(f)->objectName(),list4.at(f));
                              //   createFunctionAndConnection(list4.at(f)->objectName());
                            }
                        }
                    }
                }
            }
        }}

  return;
}

void dialog_null_controllerDialog::createFunctionAndConnection(QString objectName)
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

  //debug("%s\n",stringList.at(0).toStdString().c_str());
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
      debug("%s\n",connection.toStdString().c_str());
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
      // debug("%s\n",connection.toStdString().c_str());
    }else if(stringList.at(0)=="lineEdit"){
      QString filter;
      filter.append("ui->");
      filter.append(objectName);
      filter.append("->installEventFilter(this);");
      // debug("%s\n",filter.toStdString().c_str());
      //debug("object->objectName()== \"%s\" || ",objectName.toStdString().c_str());
    }
}
void dialog_null_controllerDialog::comboBoxControllerDialogControllerOne(int index)
{

}

void dialog_null_controllerDialog::comboBoxControllerDialogControllerTwo(int index)
{

}

void dialog_null_controllerDialog::comboBoxControllerDialogControllerThree(int index)
{

}

void dialog_null_controllerDialog::comboBoxControllerDialogControllerFour(int index)
{

}
void dialog_null_controllerDialog::pushButtonControllerDialogSure()
{
  QString xmlHead="<Controller method=\"Set\">";
  QString xmlEnd="</Controller>";
  QString message;
  message.clear();
  message.append(xmlHead);
  message.append("<ControllerId>"+QString::number(1)+"<ControllerId>");
  message.append("<ControllerName>"+ui->lineEdit_controllerDialog_controllerName_1->text()+"</ControllerName>");
  message.append("<ControllerStatus>"+ui->comboBox_controllerDialog_controller_1->currentText()+"</ControllerStatus>");
  message.append("<ControllerId>"+QString::number(2)+"<ControllerId>");
  message.append("<ControllerName>"+ui->lineEdit_controllerDialog_controllerName_2->text()+"</ControllerName>");
  message.append("<ControllerStatus>"+ui->comboBox_controllerDialog_controller_2->currentText()+"</ControllerStatus>");
  message.append("<ControllerId>"+QString::number(3)+"<ControllerId>");
  message.append("<ControllerName>"+ui->lineEdit_controllerDialog_controllerName_3->text()+"</ControllerName>");
  message.append("<ControllerStatus>"+ui->comboBox_controllerDialog_controller_3->currentText()+"</ControllerStatus>");
  message.append("<ControllerId>"+QString::number(4)+"<ControllerId>");
  message.append("<ControllerName>"+ui->lineEdit_controllerDialog_controllerName_4->text()+"</ControllerName>");
  message.append("<ControllerStatus>"+ui->comboBox_controllerDialog_controller_4->currentText()+"</ControllerStatus>");
  message.append(xmlEnd);
}

void dialog_null_controllerDialog::pushButtonControllerDialogCancel()
{

}

void dialog_null_controllerDialog::pushButtonControllerDialogUse()
{

}
void dialog_null_controllerDialog::loadQssFile(QString fileName)
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
void dialog_null_controllerDialog::recvMessageFromProxy(QString message)
{
  startXml(message);
}

void dialog_null_controllerDialog::startXml(QString message)
{
  QDomDocument doc;
  doc.setContent(message);
  QDomElement root=doc.documentElement();
  QDomElement e=root.firstChildElement();
  if(e.tagName()=="Controller"){
      QDomElement qe=e.firstChildElement();
      int index;
      while(!qe.isNull()){
          if(qe.tagName()=="ControllerId"){
              if(qe.text().toInt()>4 || qe.text().toInt()<1){
                  break;
                }else{
                  index=qe.text().toInt();
                }
            }else if(qe.tagName()=="ControllerName"){
              QString name="lineEdit_controllerDialog_controllerName_"+QString::number(index);
              QLineEdit *edit=dynamic_cast<QLineEdit *>(widgetMap.find(name).value());
              edit->setText(qe.text());
            }else if(qe.tagName()=="ControllerStatus"){
              QString name="comboBox_controllerDialog_controller_"+QString::number(index);
              QComboBox *box=dynamic_cast<QComboBox *>(widgetMap.find(name).value());
              setComboValue(e.attribute("method"),qe.text(),box);
            }
          qe=qe.nextSiblingElement();
        }
    }
}
void dialog_null_controllerDialog::setComboValue(QString method,QString text,QComboBox *combo)
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
