#include "floatdialog.h"
#include "tools/label.h"
#include <QEvent>
#include <QPicture>
#include <QMap>
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
#define  SCALE 20 //比例因子

dialog_null_floatDialog::dialog_null_floatDialog(QWidget *parent):QDialog(parent)
{
  //cteateChildWidget();

  /*设置背景色*/
  QPalette palette(this->palette());
  palette.setColor(QPalette::Background, Qt::black);
  this->setPalette(palette);
}

void dialog_null_floatDialog::setChannelId(int id)
{

}

bool dialog_null_floatDialog::eventFilter(QObject *object, QEvent *event)
{
  if(event->type()==QEvent::MouseButtonDblClick \
     ||event->type()==QEvent::MouseButtonPress){
      if(object->objectName()=="label_floatDialog_capture"){

        }else if(object->objectName()=="label_floatDialog_playback"){

        }else if(object->objectName()=="label_floatDialog_yuntai"){

        }else if(object->objectName()=="label_floatDialog_videoSet"){

        }
    }
  return QDialog::eventFilter(object,event);
}
void dialog_null_floatDialog::cteateChildWidget()
{
  /*控制*/
  int cx=width()/2-60;
  int cy=height()-27;

  Label *lab=new Label(this);
  lab->setObjectName("label_floatDialog_capture");
  lab->setPixmap(QPixmap("newImage/capture.png"));
  lab->resize(25,25);
  lab->setScaledContents(true);
  lab->move(cx,cy);
  LMap.insert("label_floatDialog_capture",lab);

  lab=new Label(this);
  lab->setObjectName("label_floatDialog_playback");
  lab->setStyleSheet("background:transparent;");
  lab->setScaledContents(true);
  lab->setPixmap(QPixmap("newImage/playback.png"));
  lab->resize(25,25);
  lab->move(cx+30,cy);
  LMap.insert("label_floatDialog_playback",lab);

  lab=new Label(this);
  lab->setObjectName("label_floatDialog_yuntai");
  lab->setPixmap(QPixmap("newImage/yuntai.png").scaled(25,25,Qt::KeepAspectRatio));
  lab->resize(25,25);
  lab->move(cx+60,cy);
  LMap.insert("label_floatDialog_yuntai",lab);

  lab=new Label(this);
  lab->setObjectName("label_floatDialog_videoSet");
  lab->setPixmap(QPixmap("newImage/videoset.png").scaled(25,25,Qt::KeepAspectRatio));
  lab->resize(25,25);
  lab->installEventFilter(this);
  lab->move(cx+90,cy);
  LMap.insert("label_floatDialog_videoSet",lab);

  /*报警*/
  lab=new Label(this);
  lab->setObjectName("label_floatDialog_videoing");
  lab->setPixmap(QPixmap("newImage/video.png").scaled(15,15,Qt::KeepAspectRatio));
  lab->resize(23,23);
  lab->move(width()-25,height()/2-30);
  LMap.insert("label_floatDialog_videoing",lab);

  lab=new Label(this);
  lab->setObjectName("label_floatDialog_sound");
  lab->setPixmap(QPixmap("newImage/sound.png").scaled(15,15,Qt::KeepAspectRatio));
  lab->resize(23,23);
  lab->move(width()-25,height()/2);
  LMap.insert("label_floatDialog_sound",lab);

  lab=new Label(this);
  lab->setObjectName("label_floatDialog_warn");
  lab->setPixmap(QPixmap("newImage/warn.png").scaled(15,15,Qt::KeepAspectRatio));
  lab->resize(23,23);
  lab->move(width()-30,height()/2+30);
  LMap.insert("label_floatDialog_warn",lab);

  lab=new Label(this);
  lab->setObjectName("label_floatDialog_net");
  lab->setPixmap(QPixmap("newImage/net.png").scaled(15,15,Qt::KeepAspectRatio));
  lab->resize(23,23);
  lab->move(width()-30,height()/2+60);
  LMap.insert("label_floatDialog_net",lab);

  for(QMap<QString,QWidget *>::iterator it=LMap.begin();it!=LMap.end();++it){
      (*it)->installEventFilter(this);
      (*it)->hide();
    }
}

void dialog_null_floatDialog::enterEvent(QEvent *)
{
  for(QMap<QString,QWidget *>::iterator it=LMap.begin();it!=LMap.end();++it){
      (*it)->show();
    }
}
void dialog_null_floatDialog::leaveEvent(QEvent *)
{
  for(QMap<QString,QWidget *>::iterator it=LMap.begin();it!=LMap.end();++it){
      (*it)->hide();
    }
}
void dialog_null_floatDialog::layoutModel(int model)
{

}
void dialog_null_floatDialog::setIndex(int index)
{
  myIndex=index;
}
void dialog_null_floatDialog::layout()
{

  int iconW=this->height()/12;
  int iconH=this->height()/12;

  Label *lab=(Label *)(*(LMap.find("label_floatDialog_capture")));
  lab->setPixmap(QPixmap("newImage/capture.png").scaled(iconW+5,iconH+5,Qt::KeepAspectRatio));
  lab->resize(iconW+5,iconH+5);
  lab->setScaledContents(true);
  lab->move(width()/2,height()-iconW-12);

  lab=(Label *)(*(LMap.find("label_floatDialog_playback")));
  lab->setScaledContents(true);
  lab->setPixmap(QPixmap("newImage/playback.png").scaled(iconW+5,iconH+5,Qt::KeepAspectRatio));
  lab->resize(iconW+5,iconH+5);
  lab->move(width()/2+iconW+5,height()-iconW-12);

  lab=(Label *)(*(LMap.find("label_floatDialog_yuntai")));
  lab->setPixmap(QPixmap("newImage/yuntai.png").scaled(iconW+5,iconH+5,Qt::KeepAspectRatio));
  lab->resize(iconW+5,iconH+5);
  lab->move(width()/2+(iconW+5)*2,height()-iconW-12);

  lab=(Label *)(*(LMap.find("label_floatDialog_videoSet")));
  LMap.insert("label_floatDialog_videoSet",lab);
  lab->setPixmap(QPixmap("newImage/videoset.png").scaled(iconW+5,iconH+5,Qt::KeepAspectRatio));
  lab->resize(iconW+5,iconH+5);
  lab->move(width()/2+(iconW+5)*3,height()-iconW-12);

  lab=(Label *)(*(LMap.find("label_floatDialog_videoing")));
  lab->setPixmap(QPixmap("newImage/video.png").scaled(iconW,iconH,Qt::KeepAspectRatio));
  lab->resize(iconW,iconH);
  lab->move(width()-iconW-5,height()/3);

  lab=(Label *)(*(LMap.find("label_floatDialog_sound")));
  lab->setPixmap(QPixmap("newImage/sound.png").scaled(iconW,iconH,Qt::KeepAspectRatio));
  lab->resize(iconW,iconH);
  lab->move(width()-iconW-5,height()/3+iconH+5);

  lab=(Label *)(*(LMap.find("label_floatDialog_warn")));
  lab->setPixmap(QPixmap("newImage/warn.png").scaled(iconW,iconH,Qt::KeepAspectRatio));
  lab->resize(iconW,iconH);
  lab->move(width()-iconW-5,height()/3+(iconH+5)*2);


  lab=(Label *)(*(LMap.find("label_floatDialog_net")));
  lab->setObjectName("label_floatDialog_net");
  lab->setPixmap(QPixmap("newImage/net.png").scaled(iconW,iconH,Qt::KeepAspectRatio));
  lab->resize(iconW,iconH);
  lab->move(width()-iconW-5,height()/3+(iconH+5)*3);
}
