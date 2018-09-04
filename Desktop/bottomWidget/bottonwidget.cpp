#include "bottonwidget.h"
#include "ui_bottonwidget.h"
#include "tools/pushbutton.h"
#include <QDesktopWidget>
#include <QFile>
#include <QToolTip>
#include <QTabWidget>
#include "plotter.h"
#include "tools/proxy.h"
#include <QDomDocument>
#define BOTTOMWIDGETH 240

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
dialog_null_bottomWidget::dialog_null_bottomWidget(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_bottomWidget)
{
  ui->setupUi(this);

  setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint );

  QDesktopWidget *dWidget=qApp->desktop();
  width=dWidget->width();
  height=dWidget->height();

  tab.clear();
  sensorData.clear();


  selfHeight=BOTTOMWIDGETH;
  resize(width+10,BOTTOMWIDGETH);

  bar=new QTabBar(this);
  bar->resize(width+10,31);
  bar->move(0,0);

  for(int i=0;i<10;++i){
      bar->addTab("Test"+QString::number(i));
    }

  QObject::connect(bar,SIGNAL(currentChanged(int)),this,SLOT(createNewWidget(int)));

  QObject::connect(Proxy::instance(),SIGNAL(sendMessageToBottomDialog(QString)),this,SLOT(recvMessageFromProxy(QString)));
  QObject::connect(this,SIGNAL(sendMessageToProxy(QString)),Proxy::instance(),SLOT(recvMessageFromSecondDialog(QString)));

  move(-5,height-BOTTOMWIDGETH);

  createChildWidget();
  loadQssFile("qss/BottomWidget.qss");
}

dialog_null_bottomWidget::~dialog_null_bottomWidget()
{
  delete ui;
}
void dialog_null_bottomWidget::enterEvent(QEvent *e)
{

}
void dialog_null_bottomWidget::leaveEvent(QEvent *e)
{

}
void dialog_null_bottomWidget::recvMessageFromProxy(QString message)
{
  startXml(message);
}
void dialog_null_bottomWidget::startXml(QString message)
{
  QDomDocument doc;
  doc.setContent(message);
  QDomElement root=doc.documentElement();

}
bool dialog_null_bottomWidget::eventFilter(QObject *object, QEvent *event)
{
  if(event->type()==QEvent::Enter){
      if(object->objectName()=="WaterLevel"){
          QPoint point=cursor().pos();
        }
    }
  return QDialog::eventFilter(object,event);
}
void dialog_null_bottomWidget::loadQssFile(QString fileName)
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
void dialog_null_bottomWidget::createChildWidget()
{

}
void dialog_null_bottomWidget::createNewWidget(int i)
{
  static QDialog *log=NULL;
  static  QPushButton *button=NULL;
  if(log==NULL){
      log=new QDialog(this);
      button=new QPushButton(log);
    }

  debug("%d\n",i);

  log->setWindowFlags(Qt::CustomizeWindowHint);
  log->resize(width+10,BOTTOMWIDGETH);

  button->setText("--->"+QString::number(i));
  button->move(10,20);
  log->move(0,31);
  log->show();
}
