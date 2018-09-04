#include "rightwidget.h"
#include "ui_rightwidget.h"
#include <QDesktopWidget>
#include "tools/pushbutton.h"
#include <QFile>
#include <stdio.h>
#include <stdlib.h>
#include "tools/label.h"
#include <QPainter>
#include "backgroundWidget/backgroundwidget.h"
#include <QMenu>
#include <QAction>
#include "systemSetDialog.h"
#include "yuntaidialog.h"
#include "shutdowndialog.h"
#include "playbackdialog.h"
#include "sensordialog.h"
#include "controllerdialog.h"
#include "piclayoutdialog.h"
#include "warnoutdialog.h"
#include "videomanagerdialog.h"
#include "tools/proxy.h"

#define POWidgetW 160
#define POWidgetH 720
#define ICONWH 35

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

dialog_null_rightWidget::dialog_null_rightWidget(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_rightWidget)
{
  ui->setupUi(this);

  setWindowFlags(Qt::FramelessWindowHint  | Qt::WindowStaysOnTopHint);
  QDesktopWidget *dWidget=qApp->desktop();
  width=dWidget->width();
  height=dWidget->height();

  childHeight=POWidgetH/9;
  childWidth=POWidgetW;

  resize(POWidgetW,POWidgetH);
  move(width-20,0);

  /*设置背景色*/
  QPalette palette(this->palette());
  palette.setColor(QPalette::Background, Qt::black);
  this->setPalette(palette);

  lockWindow=false;
  ifPutOut=false;
  dMap.clear();

  createChildWidget();
  createConnection();
  loadQssFile("qss/RightWidget.qss");

  dialog_null_videoManagerDialog *di=new dialog_null_videoManagerDialog;
}

dialog_null_rightWidget::~dialog_null_rightWidget()
{
  delete ui;
}
void dialog_null_rightWidget::enterEvent(QEvent *e)
{
  QDialog::enterEvent(e);
}
void dialog_null_rightWidget::leaveEvent(QEvent *e)
{
  QDialog::leaveEvent(e);
}
bool dialog_null_rightWidget::event(QEvent *e)
{
  return QDialog::event(e);
}
void dialog_null_rightWidget::loadQssFile(QString fileName)
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
bool dialog_null_rightWidget::eventFilter(QObject *object, QEvent *event)
{
  if(object->objectName()=="SystemSet"){
      //     debug("%s","=+++++++++");
      QPoint point=cursor().pos();
      //  QToolTip::showText(point,"hello world",this);
    }
  return QDialog::eventFilter(object,event);
}
void dialog_null_rightWidget::paintEvent(QPaintEvent *e)
{
  QPainter painter(this);
  if(!ifPutOut){
      QPixmap pixmap;
      pixmap.fill(QColor(0x00,0x00,0x00,0x00));
      //pixmap.load("newImage/background.png");
      // pixmap=pixmap.scaled(160,720,Qt::KeepAspectRatio);
      painter.drawPixmap(QRect(0, 0, POWidgetW, POWidgetH), pixmap);
    }else{
      //QPixmap pixmap("newImage/background.png");
      QPixmap pixmap;
      // pixmap.fill(QColor(0x00,0x00,0x00,0x00));
      pixmap.load("newImage/background.png");
      pixmap=pixmap.scaled(160,720,Qt::KeepAspectRatio);
      painter.drawPixmap(QRect(0, 0, POWidgetW, POWidgetH), pixmap);
      //    }

      QDialog::paintEvent(e);
    }
}
void dialog_null_rightWidget::lockRightWidget()
{
  for(QVector<PushButton *>::iterator it=pVector.begin();it!=pVector.end();++it){
      PushButton *fix=*it;
      if(fix->objectName()=="Fix"){
          if(!lockWindow){
              lockWindow=true;
              fix->setIcon(QIcon("newImage/lock.png"));
              fix->setIconSize(QSize(15,15));
            }else if(lockWindow){
              lockWindow=false;
              fix->setIcon(QIcon("newImage/unlock.png"));
              fix->setIconSize(QSize(15,15));
            }
        }
    }
}
void dialog_null_rightWidget::createConnection()
{
  for(QVector<PushButton *>::iterator it=pVector.begin();it!=pVector.end();++it){
      PushButton *button=*it;
      if(button->objectName()=="SystemSet"){
          QObject::connect(button,SIGNAL(clicked()),this,SLOT(systemSetSlot()));
        }else if(button->objectName()=="YunTaiCtrl"){
          QObject::connect(button,SIGNAL(clicked()),this,SLOT(yunTaiCtrloSlot()));
        }else if(button->objectName()=="PlayBack"){
          QObject::connect(button,SIGNAL(clicked()),this,SLOT(playBackSlot()));
        }else if(button->objectName()=="Remote"){
          QObject::connect(button,SIGNAL(clicked()),this,SLOT(remoteManagerSlot()));
        }else if(button->objectName()=="Sensor"){
          QObject::connect(button,SIGNAL(clicked()),this,SLOT(sensorSlot()));
        }else if(button->objectName()=="Controller"){
          QObject::connect(button,SIGNAL(clicked()),this,SLOT(controllerSlot()));
        }else if(button->objectName()=="PictureLayout"){
          QObject::connect(button,SIGNAL(clicked()),this,SLOT(picLayoutSlot()));
        }else if(button->objectName()=="ShutDown"){
          QObject::connect(button,SIGNAL(clicked()),this,SLOT(shutdownSlot()));
        }else if(button->objectName()=="PullOut"){
          QObject::connect(button,SIGNAL(clicked()),this,SLOT(putOutRightWidget()));
        }else if(button->objectName()=="Fix"){
          //    QObject::connect(button,SIGNAL(clicked()),this,SLOT(lockRightWidget()));
        }
    }
}
void dialog_null_rightWidget::putOutRightWidget()
{
  // repaint();
  //  if(lockWindow){
  //      return;
  //    }
  //  if(!lockWindow){
  for(QVector<PushButton *>::iterator it=pVector.begin();it!=pVector.end();++it){
      PushButton *out=*it;
      if(out->objectName()=="PullOut"){
          if(ifPutOut){
              move(width-20,0);
              out->setIcon(QIcon("newImage/unpoint.png"));
              out->setIconSize(QSize(17,60));
              ifPutOut=false;
            }else{
              move(width-POWidgetW,0);
              out->setIcon(QIcon("newImage/point.png"));
              out->setIconSize(QSize(17,60));
              ifPutOut=true;
            }
        }
    }
  //    }
}
void dialog_null_rightWidget::systemSetSlot()
{

  // debug("%s\n","systemSetSlot");
  if(!dMap.contains("dialog_null_systemSetDialog")){
      dialog_null_systemSetDialog *log=new dialog_null_systemSetDialog();
      log->move(150,150);
      //debug("%s\n",log->objectName().toStdString().c_str());

      dMap.insert(log->objectName(),log);
    }
  dialog_null_systemSetDialog *dLog=(dialog_null_systemSetDialog *)(*dMap.constFind("dialog_null_systemSetDialog"));
  //   dLog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
  hideOtherDialog();
  dLog->show();

}

void dialog_null_rightWidget::yunTaiCtrloSlot()
{
  //debug("%s\n","yunTaiCtrloSlot");
  if(!dMap.contains("dialog_null_yunTaiDialog")){
      dialog_null_yunTaiDialog *log=new dialog_null_yunTaiDialog();
      //  log->setObjectName("YunTaiCtrlDialog");
      log->move(150,150);
      //debug("%s\n",log->objectName().toStdString().c_str());
      dMap.insert("dialog_null_yunTaiDialog",log);
    }
  dialog_null_yunTaiDialog *dLog=(dialog_null_yunTaiDialog *)(*dMap.constFind("dialog_null_yunTaiDialog"));
  //   dLog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
  hideOtherDialog();

  dLog->show();
}
void dialog_null_rightWidget::playBackSlot()
{

  // debug("%s\n","palyBackSlot");

  if(!dMap.contains("dialog_null_playbackDialog")){
      dialog_null_playbackDialog *log=new dialog_null_playbackDialog();
      log->setObjectName("dialog_null_playbackDialog");
      QDesktopWidget *widget=qApp->desktop();
      int height=widget->height();
      log->move(0,height-240);
      //debug("%s\n",log->objectName().toStdString().c_str());
      dMap.insert("dialog_null_playbackDialog",log);
    }
  dialog_null_playbackDialog *dLog=(dialog_null_playbackDialog *)(*dMap.constFind("dialog_null_playbackDialog"));
  //   dLog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
  hideOtherDialog();

  dLog->show();

}
void dialog_null_rightWidget::remoteManagerSlot()
{
  //debug("%s\n","remoteManagerSlot");
  //  if(!dMap.contains("PlaybackDialog")){
  //      PlaybackDialog *log=new PlaybackDialog();
  //      log->setObjectName("PlaybackDialog");
  //   //   log->resize(500,500);
  //      log->move(300,150);
  //      debug("%s\n",log->objectName().toStdString().c_str());
  //      //log->setStyleSheet("background:#AAAAAA;");
  //      dMap.insert("PlaybackDialog",log);
  //    }
  //  Dialog *dLog=(Dialog *)(*dMap.constFind("PlaybackDialog"));
  //  //   dLog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
  //  dLog->show();
}
void dialog_null_rightWidget::sensorSlot()
{
  //debug("%s\n","sensorSlot");
  if(!dMap.contains("dialog_null_sensorDialog")){
      dialog_null_sensorDialog *log=new dialog_null_sensorDialog();
      log->setObjectName("dialog_null_sensorDialog");
      //   log->resize(500,500);
      log->move(150,150);
      //debug("%s\n",log->objectName().toStdString().c_str());
      dMap.insert("dialog_null_sensorDialog",log);
    }
  dialog_null_sensorDialog *dLog=(dialog_null_sensorDialog *)(*dMap.constFind("dialog_null_sensorDialog"));
  //   dLog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
  hideOtherDialog();

  dLog->show();
}
void dialog_null_rightWidget::controllerSlot()
{
  //debug("%s\n","controllerSlot");
  if(!dMap.contains("dialog_null_controllerDialog")){
      dialog_null_controllerDialog *log=new dialog_null_controllerDialog();
      log->setObjectName("dialog_null_controllerDialog");
      log->move(150,150);
      // debug("%s\n",log->objectName().toStdString().c_str());
      dMap.insert("dialog_null_controllerDialog",log);
    }
  dialog_null_controllerDialog *dLog=(dialog_null_controllerDialog *)(*dMap.constFind("dialog_null_controllerDialog"));
  //   dLog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
  hideOtherDialog();

  dLog->show();
}
void dialog_null_rightWidget::picLayoutSlot()
{
  // debug("%s\n","picLayoutSlot");
  if(!dMap.contains("dialog_null_picLayoutDialog")){
      dialog_null_picLayoutDialog *log=new dialog_null_picLayoutDialog();
      log->setObjectName("dialog_null_picLayoutDialog");
      log->move(150,150);
      //debug("%s\n",log->objectName().toStdString().c_str());
      dMap.insert("dialog_null_picLayoutDialog",log);
    }
  dialog_null_picLayoutDialog *dLog=(dialog_null_picLayoutDialog *)(*dMap.constFind("dialog_null_picLayoutDialog"));
  //   dLog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
  hideOtherDialog();

  dLog->show();
  //  QPoint pos; //获取按键菜单的坐标
  //  for(QVector<PushButton *>::iterator it=pVector.begin();it!=pVector.end();++it){
  //      if((*it)->objectName()=="PictureLayout"){
  //          pos.setX((*it)->x()-80);
  //          pos.setY((*it)->y()+10);
  //        }
  //    }
  //  QObjectList list=this->children();

  //  for(int i=0;i<list.count();++i){
  //      if(list.at(i)->objectName()=="PicMenu"){
  //          QMenu *menu=static_cast<QMenu *>(list.at(i));
  //          menu->exec(this->mapToGlobal(pos));
  //        }
  //    }

  //exec(QCursor::pos());  当前光标的坐标
}
void dialog_null_rightWidget::shutdownSlot()
{
  if(!dMap.contains("dialog_null_shutdownDialog")){
      dialog_null_shutdownDialog *log=new dialog_null_shutdownDialog();
      log->setObjectName("dialog_null_shutdownDialog");
      log->move(250,250);
      //debug("%s\n",log->objectName().toStdString().c_str());
      dMap.insert("dialog_null_shutdownDialog",log);
    }
  dialog_null_shutdownDialog *dLog=(dialog_null_shutdownDialog *)(*dMap.constFind("dialog_null_shutdownDialog"));
  //   dLog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
  hideOtherDialog();

  dLog->show();

}
void dialog_null_rightWidget::createChildWidget(){

  pVector.clear();

  PushButton *lab=new PushButton(this);
  lab->setText("");
  lab->resize(POWidgetW,childHeight);
  lab->setEnabled(false);
  lab->setObjectName("labButton");
  lab->move(20,0);
  pVector.append(lab);

  PushButton *button=new PushButton(this);
  button->setText(QString::fromUtf8(" 系统设置"));
  button->resize(POWidgetW,childHeight);
  button->setObjectName("SystemSet");
  button->move(20,childHeight);
  button->setIcon(QIcon("newImage/system.png"));
  button->setIconSize(QSize(ICONWH,ICONWH));
  button->installEventFilter(this);
  pVector.append(button);

  button=new PushButton(this);
  button->setText(QString::fromUtf8(" 云台控制"));//
  button->resize(POWidgetW,childHeight);
  button->setObjectName("YunTaiCtrl");
  button->setIcon(QIcon("newImage/yuntai.png"));
  button->setIconSize(QSize(ICONWH,ICONWH));
  button->move(20,childHeight*2);
  pVector.append(button);

  button=new PushButton(this);
  button->setText(QString::fromUtf8(" 录像回放"));
  button->resize(POWidgetW,childHeight);
  button->setObjectName("PlayBack");
  button->setIcon(QIcon("newImage/playback.png"));
  button->setIconSize(QSize(ICONWH,ICONWH));
  button->move(20,childHeight*3);
  pVector.append(button);

  button=new PushButton(this);
  button->setText(QString::fromUtf8(" 远程管理"));
  button->resize(POWidgetW,childHeight);
  button->setObjectName("Remote");
  button->setIcon(QIcon("newImage/remote.png"));
  button->setIconSize(QSize(ICONWH,ICONWH));
  button->move(20,childHeight*4);
  pVector.append(button);

  button=new PushButton(this);
  button->setText(QString::fromUtf8("   传感器"));
  button->resize(POWidgetW,childHeight);
  button->setObjectName("Sensor");
  button->setIcon(QIcon("newImage/sensor.png"));
  button->setIconSize(QSize(ICONWH,ICONWH));
  button->move(20,childHeight*5);
  pVector.append(button);

  button=new PushButton(this);
  button->setText(QString::fromUtf8("   控制器"));
  button->resize(POWidgetW,childHeight);
  button->setObjectName("Controller");
  button->setIcon(QIcon("newImage/controller.png"));
  button->setIconSize(QSize(ICONWH,ICONWH));
  button->move(20,childHeight*6);
  pVector.append(button);

  button=new PushButton(this);
  button->setText(QString::fromUtf8(" 画面布局"));
  button->resize(POWidgetW,childHeight);
  button->setObjectName("PictureLayout");
  button->setIcon(QIcon("newImage/piclayout.png"));
  button->setIconSize(QSize(ICONWH,ICONWH));
  button->move(20,childHeight*7);
  pVector.append(button);

  button=new PushButton(this);
  button->setText(QString::fromUtf8(" 关闭系统"));
  button->resize(POWidgetW,childHeight);
  button->setObjectName("ShutDown");
  button->setIcon(QIcon("newImage/shutdown.png"));
  button->setIconSize(QSize(ICONWH,ICONWH));
  button->move(20,childHeight*8);
  pVector.append(button);

  PushButton *out=new PushButton(this);
  out->setButtonProperty();
  out->setObjectName("PullOut");
  out->setIcon(QIcon("newImage/unpoint.png"));
  out->setIconSize(QSize(17,60));
  out->setFocusPolicy(Qt::NoFocus);
  out->resize(20,250);
  out->move(0,POWidgetH/2-125);
  pVector.append(out);

  //  PushButton *fix=new PushButton(this);
  //  fix->setButtonProperty();
  //  fix->setObjectName("Fix");
  //  fix->setIcon(QIcon("newImage/unlock.png"));
  //  fix->setIconSize(QSize(15,15));
  //  fix->resize(17,20);
  //  fix->move(3,POWidgetH/2-30);
  //  pVector.append(fix);

  /**/
  dialog_null_systemSetDialog *log=new dialog_null_systemSetDialog();
  log->setObjectName("dialog_null_systemSetDialog");

  //   log->resize(500,500);
  log->move(150,150);
  //debug("%s\n",log->objectName().toStdString().c_str());
  //  log->setStyleSheet("background:#AAAAAA;");
  dMap.insert("dialog_null_systemSetDialog",log);

  dialog_null_shutdownDialog *shutdownDialog=new dialog_null_shutdownDialog();
  shutdownDialog->setObjectName("dialog_null_shutdownDialog");

  shutdownDialog->move(250,250);
  //debug("%s\n",shutdownDialog->objectName().toStdString().c_str());
  dMap.insert("dialog_null_shutdownDialog",shutdownDialog);

  dialog_null_picLayoutDialog *picLayoutDialog=new dialog_null_picLayoutDialog();
  picLayoutDialog->setObjectName("dialog_null_picLayoutDialog");
  picLayoutDialog->move(150,150);
  //debug("%s\n",picLayoutDialog->objectName().toStdString().c_str());
  dMap.insert("dialog_null_picLayoutDialog",picLayoutDialog);

  dialog_null_controllerDialog *controllerDialog=new dialog_null_controllerDialog();
  controllerDialog->setObjectName("dialog_null_controllerDialog");
  controllerDialog->move(150,150);
  // debug("%s\n",controllerDialog->objectName().toStdString().c_str());
  dMap.insert("dialog_null_controllerDialog",controllerDialog);

  dialog_null_sensorDialog *sensorDialog=new dialog_null_sensorDialog();
  sensorDialog->setObjectName("dialog_null_sensorDialog");
  //   log->resize(500,500);
  sensorDialog->move(150,150);
  //debug("%s\n",sensorDialog->objectName().toStdString().c_str());
  dMap.insert("dialog_null_sensorDialog",sensorDialog);

  dialog_null_playbackDialog *playbackDialog=new dialog_null_playbackDialog();
  playbackDialog->setObjectName("dialog_null_playbackDialog");
  QDesktopWidget *widget=qApp->desktop();
  int height=widget->height();
  playbackDialog->move(0,height-240);
  //debug("%s\n",playbackDialog->objectName().toStdString().c_str());
  dMap.insert("dialog_null_playbackDialog",playbackDialog);

  dialog_null_yunTaiDialog *yunTaiDialog=new dialog_null_yunTaiDialog();
  log->setObjectName("dialog_null_yunTaiDialog");
  yunTaiDialog->move(150,150);
  //debug("%s\n",yunTaiDialog->objectName().toStdString().c_str());
  dMap.insert("dialog_null_yunTaiDialog",yunTaiDialog);
}

void dialog_null_rightWidget::hideOtherDialog()
{
  for( QMap<QString,QDialog*>::iterator it=dMap.begin();it!=dMap.end();++it){
      if(it.key()=="dialog_null_yunTaiDialog"){
          dialog_null_yunTaiDialog *yuntai=dynamic_cast<dialog_null_yunTaiDialog *>(it.value());
          yuntai->myHide();
        }
      it.value()->hide();
    }
}
