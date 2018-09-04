#include "edituserdialog.h"
#include "ui_edituserdialog.h"
#include "keyboard/softkeyboard.h"
#include <QDesktopWidget>

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

dialog_null_editUserDialog::dialog_null_editUserDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_editUserDialog)
{
  ui->setupUi(this);
  QObject::connect(ui->checkBox_editUserDialog_videoManager,SIGNAL(stateChanged(int)),this,SLOT(checkBoxEditUserDialogVideoManager(int)));
  QObject::connect(ui->checkBox_editUserDialog_conntionManager,SIGNAL(stateChanged(int)),this,SLOT(checkBoxEditUserDialogConntionManager(int)));
  QObject::connect(ui->checkBox_editUserDialog_shutdown,SIGNAL(stateChanged(int)),this,SLOT(checkBoxEditUserDialogShutdown(int)));
  QObject::connect(ui->checkBox_editUserDialog_yunTaiControl,SIGNAL(stateChanged(int)),this,SLOT(checkBoxEditUserDialogYunTaiControl(int)));
  QObject::connect(ui->checkBox_editUserDialog_systemSet,SIGNAL(stateChanged(int)),this,SLOT(checkBoxEditUserDialogSystemSet(int)));
  QObject::connect(ui->checkBox_editUserDialog_videoExchange,SIGNAL(stateChanged(int)),this,SLOT(checkBoxEditUserDialogVideoExchange(int)));
  QObject::connect(ui->checkBox_editUserDialog_capturePicManager,SIGNAL(stateChanged(int)),this,SLOT(checkBoxEditUserDialogCapturePicManager(int)));
  QObject::connect(ui->checkBox_editUserDialog_userManager,SIGNAL(stateChanged(int)),this,SLOT(checkBoxEditUserDialogUserManager(int)));
  QObject::connect(ui->pushButton_editUserDialog_noSelectAll,SIGNAL(clicked()),this,SLOT(pushButtonEditUserDialogNoSelectAll()));
  QObject::connect(ui->pushButton_editUserDialog_sure,SIGNAL(clicked()),this,SLOT(pushButtonEditUserDialogSure()));
  QObject::connect(ui->pushButton_editUserDialog_cancel,SIGNAL(clicked()),this,SLOT(pushButtonEditUserDialogCancel()));
  QObject::connect(ui->pushButton_editUserDialog_selectAll,SIGNAL(clicked()),this,SLOT(pushButtonEditUserDialogSelectAll()));
  ui->lineEdit_editUserDialog_userName->installEventFilter(this);
  ui->lineEdit_editUserDialog_userpassword->installEventFilter(this);

  operation.clear();
  this->move(150,150);
  // testFile();
}

dialog_null_editUserDialog::~dialog_null_editUserDialog()
{
  delete ui;
}
bool dialog_null_editUserDialog::eventFilter(QObject *object, QEvent *e)
{
  if(e->type()==QEvent::MouseButtonPress){
      if(object->objectName()== "lineEdit_editUserDialog_userName" ||
         object->objectName()== "lineEdit_editUserDialog_userpassword"){
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
          sk->setLineText(lEdit);
          sk->show();
        }else{
          SoftKeyboard *sk=SoftKeyboard::instance();
          sk->hide();
        }
    }
  return QDialog::eventFilter(object,e);
}

void dialog_null_editUserDialog::testFile()
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

void dialog_null_editUserDialog::createFunctionAndConnection(QString objectName)
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

      // debug("%s\n",functionDeclaration.toStdString().c_str());
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
      debug("object->objectName()== \"%s\" || ",objectName.toStdString().c_str());
    }
}

void dialog_null_editUserDialog::checkBoxEditUserDialogVideoManager(int flags)
{
  operation+="<VideoManager>"+QString::number(flags)+"</VideoManager>";
}

void dialog_null_editUserDialog::checkBoxEditUserDialogConntionManager(int flags)
{
  operation+="<ConnectionManager>"+QString::number(flags)+"</ConnectionManager>";
}

void dialog_null_editUserDialog::checkBoxEditUserDialogShutdown(int flags)
{
  operation+="<Shutdown>"+QString::number(flags)+"<Shutdown>";
}

void dialog_null_editUserDialog::checkBoxEditUserDialogYunTaiControl(int flags)
{
  operation+="<YunTaiControl>"+QString::number(flags)+"<YunTaiControl>";
}

void dialog_null_editUserDialog::checkBoxEditUserDialogSystemSet(int flags)
{
  operation+="<SetSystem>"+QString::number(flags)+"</SetSystem>";
}

void dialog_null_editUserDialog::checkBoxEditUserDialogVideoExchange(int flags)
{
  operation+="<VideoSet>"+QString::number(flags)+"</VideoSet>";
}

void dialog_null_editUserDialog::checkBoxEditUserDialogCapturePicManager(int flags)
{
  operation+="<Capture>"+QString::number(flags)+"</Capture>";
}

void dialog_null_editUserDialog::checkBoxEditUserDialogUserManager(int flags)
{
  operation+="<UserManager>"+QString::number(flags)+"</UserManager>";
}

void dialog_null_editUserDialog::pushButtonEditUserDialogNoSelectAll()
{
  ui->checkBox_editUserDialog_capturePicManager->setChecked(false);
  ui->checkBox_editUserDialog_conntionManager->setChecked(false);
  ui->checkBox_editUserDialog_shutdown->setChecked(false);
  ui->checkBox_editUserDialog_systemSet->setChecked(false);
  ui->checkBox_editUserDialog_userManager->setChecked(false);
  ui->checkBox_editUserDialog_videoExchange->setChecked(false);
  ui->checkBox_editUserDialog_videoManager->setChecked(false);
  ui->checkBox_editUserDialog_yunTaiControl->setCheckable(false);
  operation.clear();
}

void dialog_null_editUserDialog::pushButtonEditUserDialogSure()
{
  if(ui->lineEdit_editUserDialog_userName->text().isEmpty() || ui->lineEdit_editUserDialog_userpassword->text().isEmpty()){
      return;
    }
  operation+="<SubUserName>"+ui->lineEdit_editUserDialog_userName->text()+"</SubUserName>";
  operation+="<SubUserPasswork>"+ui->lineEdit_editUserDialog_userpassword->text()+"</SubUserPassword>";
  this->hide();
}

void dialog_null_editUserDialog::pushButtonEditUserDialogCancel()
{
  this->hide();
  operation.clear();
}
void dialog_null_editUserDialog::pushButtonEditUserDialogSelectAll()
{
  operation.clear();

  ui->checkBox_editUserDialog_capturePicManager->setChecked(true);
  ui->checkBox_editUserDialog_conntionManager->setChecked(true);
  ui->checkBox_editUserDialog_shutdown->setChecked(true);
  ui->checkBox_editUserDialog_systemSet->setChecked(true);
  ui->checkBox_editUserDialog_userManager->setChecked(true);
  ui->checkBox_editUserDialog_videoExchange->setChecked(true);
  ui->checkBox_editUserDialog_videoManager->setChecked(true);
  ui->checkBox_editUserDialog_yunTaiControl->setCheckable(true);

  operation+="<VideoManager>"+QString::number(ui->checkBox_editUserDialog_videoManager->checkState())+"</VideoManager>";
  operation+="<ConnectionManager>"+QString::number(ui->checkBox_editUserDialog_conntionManager->checkState())+"</ConnectionManager>";
  operation+="<Shutdown>"+QString::number(ui->checkBox_editUserDialog_shutdown->checkState())+"<Shutdown>";
  operation+="<YunTaiControl>"+QString::number(ui->checkBox_editUserDialog_yunTaiControl->checkState())+"<YunTaiControl>";
  operation+="<SetSystem>"+QString::number(ui->checkBox_editUserDialog_systemSet->checkState())+"</SetSystem>";
  operation+="<VideoSet>"+QString::number(ui->checkBox_editUserDialog_videoExchange->checkState())+"</VideoSet>";
  operation+="<Capture>"+QString::number(ui->checkBox_editUserDialog_capturePicManager->checkState())+"</Capture>";
  operation+="<UserManager>"+QString::number(ui->checkBox_editUserDialog_userManager->checkState())+"</UserManager>";
}
