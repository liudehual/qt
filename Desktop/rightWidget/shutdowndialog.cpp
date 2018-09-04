#include "shutdowndialog.h"
#include "ui_shutdowndialog.h"
#include <QMouseEvent>
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

QString dialog_null_shutdownDialog::xmlHead="<Shutdown>";
QString dialog_null_shutdownDialog::xmlEnd="</Shutdown>";
dialog_null_shutdownDialog::dialog_null_shutdownDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_shutdownDialog)
{
  ui->setupUi(this);
  initWidget();


  QObject::connect(Proxy::instance(),SIGNAL(sendMessageToShutdownDialog(QString)),this,SLOT(recvMessageFromProxy()));
  QObject::connect(this,SIGNAL(sendMessageToProxy(QString)),Proxy::instance(),SLOT(recvMessageFromSecondDialog(QString)));
  this->setStyleSheet("QPushButton{color:#FFFF00;}");
  loadQssFile("qss/shutdown.qss");
}

dialog_null_shutdownDialog::~dialog_null_shutdownDialog()
{
  delete ui;
}
bool dialog_null_shutdownDialog::eventFilter(QObject *object, QEvent *e)
{
  if(object->objectName()=="label_shutdownDialog_shutdown"){
      if(e->type()==QEvent::MouseButtonPress){
          QMouseEvent *mouse=dynamic_cast<QMouseEvent *>(e);
          if(mouse->button()==Qt::LeftButton){
              this->hide();
            }
        }
    }
  return QDialog::eventFilter(object,e);
}
void dialog_null_shutdownDialog::initWidget()
{
  ui->label_shutdownDialog_shutdown->installEventFilter(this);
  QObject::connect(ui->pushButton_shutdownDialog_restart,SIGNAL(clicked()),this,SLOT(pushButtonShutdownDialogRestart()));
  QObject::connect(ui->pushButton_shutdownDialog_loginout,SIGNAL(clicked()),this,SLOT(pushButtonShutdownDialogLoginout()));
  QObject::connect(ui->pushButton_shutdownDialog_shutdown,SIGNAL(clicked()),this,SLOT(pushButtonShutdownDialogShutdown()));
}
void dialog_null_shutdownDialog::recvMessageFromProxy(QString message)
{
  QDomDocument doc;
  doc.setContent(message);
  QDomElement root=doc.documentElement();
  QDomElement e=root.firstChildElement();
  QDomElement child=e.firstChildElement();

  int code=child.text().toInt();
}
void dialog_null_shutdownDialog::pushButtonShutdownDialogRestart()
{
  QString message;
  message.append(xmlHead);
  message.append("<Action>"+QString::number(1)+"</Action>");
  message.append(xmlEnd);
  emit sendMessageToProxy(message);
}

void dialog_null_shutdownDialog::pushButtonShutdownDialogLoginout()
{
  QString message;
  message.append(xmlHead);
  message.append("<Action>"+QString::number(2)+"</Action>");
  message.append(xmlEnd);
  emit sendMessageToProxy(message);
}
void dialog_null_shutdownDialog::pushButtonShutdownDialogShutdown()
{
  QString message;
  message.append(xmlHead);
  message.append("<Action>"+QString::number(0)+"</Action>");
  message.append(xmlEnd);
  emit sendMessageToProxy(message);
  qApp->quit();
}
void dialog_null_shutdownDialog::loadQssFile(QString fileName)
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
