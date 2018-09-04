#include "sensordialog.h"
#include "ui_sensordialog.h"
#include <QMouseEvent>
#include <QFile>

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
dialog_null_sensorDialog::dialog_null_sensorDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_sensorDialog)
{
  ui->setupUi(this);
  initWidget();
  loadQssFile("qss/sensor.qss");
  //testFile();
}

dialog_null_sensorDialog::~dialog_null_sensorDialog()
{
  delete ui;
}
bool dialog_null_sensorDialog::eventFilter(QObject *object, QEvent *e)
{
  if(object->objectName()=="label_sensorDialog_shutdown"){
      if(e->type()==QEvent::MouseButtonPress){
          QMouseEvent *mouse=dynamic_cast<QMouseEvent *>(e);
          if(mouse->button()==Qt::LeftButton){
              this->hide();
            }
        }
    }
  return QDialog::eventFilter(object,e);
}
void dialog_null_sensorDialog::initWidget()
{
  ui->label_sensorDialog_shutdown->installEventFilter(this);
  QObject::connect(ui->pushButton_general_use,SIGNAL(clicked()),this,SLOT(pushButtonGeneralUse()));
  QObject::connect(ui->pushButton_general_sure,SIGNAL(clicked()),this,SLOT(pushButtonGeneralSure()));
  QObject::connect(ui->pushButton_general_cancel,SIGNAL(clicked()),this,SLOT(pushButtonGeneralCancel()));
  QObject::connect(ui->pushButton_warn_use,SIGNAL(clicked()),this,SLOT(pushButtonWarnUse()));
  QObject::connect(ui->pushButton_warn_sure,SIGNAL(clicked()),this,SLOT(pushButtonWarnSure()));
  QObject::connect(ui->pushButton_warn_cancel,SIGNAL(clicked()),this,SLOT(pushButtonWarnCancel()));
}
void dialog_null_sensorDialog::testFile()
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
void dialog_null_sensorDialog::createFunctionAndConnection(QString objectName)
{
 #if 0
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
    // debug("%s\n",connection.toStdString().c_str());
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
     debug("%s\n",filter.toStdString().c_str());
      //debug("object->objectName()== \"%s\" || ",objectName.toStdString().c_str());
    }
#endif
}
void dialog_null_sensorDialog::pushButtonGeneralUse()
{

}

void dialog_null_sensorDialog::pushButtonGeneralSure()
{

}

void dialog_null_sensorDialog::pushButtonGeneralCancel()
{

}

void dialog_null_sensorDialog::pushButtonWarnUse()
{

}

void dialog_null_sensorDialog::pushButtonWarnSure()
{

}

void dialog_null_sensorDialog::pushButtonWarnCancel()
{

}
void dialog_null_sensorDialog::loadQssFile(QString fileName)
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
