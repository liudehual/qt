#include "backgroundwidget.h"
#include "ui_backgroundwidget.h"
#include <QPainter>
#include "rightWidget/rightwidget.h"
#include "bottomWidget/bottonwidget.h"
#include "tools/pushbutton.h"
#include "tools/label.h"
#include <QDateTime>
#include <QTimer>
#include <QStringList>
#include <QString>
#include <QDesktopWidget>
#include "floatdialog.h"
#include "tools/widgetpool.h"
#define BACKGROUNDW 1600
#define BACKGROUNDH 1024

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
QMap<QString,QWidget *> dialog_null_backGroundWidget::dialogMap;

dialog_null_backGroundWidget::dialog_null_backGroundWidget(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_backGroundWidget)
{

  ui->setupUi(this);
  this->resize(BACKGROUNDW,BACKGROUNDH);

  this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );

  dVector.clear();
  QDesktopWidget *dWidget=qApp->desktop();
  screenWidth=dWidget->width();
  screenHeight=dWidget->height();

  /*设置背景色*/
  QPalette palette(this->palette());
  palette.setColor(QPalette::Background, Qt::black);
  this->setPalette(palette);
  dialog_null_backGroundWidget::addToMap(this->objectName(),this);

  this->installEventFilter(this);
  move(-10,-10);

  createChildWidget();

  /*显示时间日期*/
  lab=new Label(this);
  lab->resize(230,50);
  lab->move(10,10);
  lab->setStyleSheet("color:#FFFF00;");
  lab->setObjectName("label_backGroudnWidget_time");
  QTimer *timer=new QTimer(this);
  QObject::connect(timer,SIGNAL(timeout()),this,SLOT(timeoutSlot()));
  timer->start(1000);
  for(int i=0;i<16;++i){
      dialog_null_floatDialog *dialog=new dialog_null_floatDialog(this);
      dialog->setIndex(i);
      dVector.append(dialog);
      //dialog->resize(screenWidth/8,screenHeight/8);
      dialog->setWindowFlags(Qt::CustomizeWindowHint);
      /*此处应该获取该Dialog对应的通道号，暂未实现*/
      dialog->cteateChildWidget();
      dialog->installEventFilter(this);
      /*将悬浮框加入空间池*/
      QString objectName="Dialog_backgroundWidget_floatDialog";
      objectName+=QString::number(dVector.count());
      dialog->setObjectName(objectName);
    }
  hideAllFloatDialog();
}

dialog_null_backGroundWidget::~dialog_null_backGroundWidget()
{
  delete ui;
}
void dialog_null_backGroundWidget::addToMap(QString objectName,QDialog* dialog)
{
  if(!objectName.isEmpty() && dialog!=NULL){
      dialogMap.insert(objectName,dialog);
    }
}
QDialog *dialog_null_backGroundWidget::getDialogFromMap(QString objectName)
{
  if(!objectName.isEmpty()){
      return static_cast<QDialog *>(*(dialogMap.find(objectName)));
    }
  return NULL;
}

void dialog_null_backGroundWidget::testClicked()
{

}
void dialog_null_backGroundWidget::timeoutSlot()
{
  //  星期一 星期二 星期三 星期四 星期五 星期六 星期日
  QDateTime current_date_time = QDateTime::currentDateTime();
  QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
  QStringList QSList=current_date.split(" ");
  if(QSList[2]=="Mon"){
      QSList[2]=QString::fromUtf8("星期一");
    }else if(QSList[2]=="Tue"){
      QSList[2]=QString::fromUtf8("星期二");
    }else  if(QSList[2]=="Wed"){
      QSList[2]=QString::fromUtf8("星期三");
    }else if(QSList[2]=="Thu"){
      QSList[2]=QString::fromUtf8("星期四");
    } else if(QSList[2]=="Fri"){
      QSList[2]=QString::fromUtf8("星期五");
    }else if(QSList[2]=="Sat"){
      QSList[2]=QString::fromUtf8("星期六");
    }else if(QSList[2]=="Sun"){
      QSList[2]=QString::fromUtf8("星期日");
    }

  QString date;
  date+=QSList[0]+" "+QSList[1]+" "+QSList[2];
  lab->setFont(QFont("Times",20,QFont::Bold));
  lab->setText(date);
}

void dialog_null_backGroundWidget::paintEvent(QPaintEvent *pEvent)
{

  QDialog::paintEvent(pEvent);
}
bool dialog_null_backGroundWidget::eventFilter(QObject *object, QEvent *event)
{
  return QDialog::eventFilter(object,event);
}
void dialog_null_backGroundWidget::createChildWidget()
{

}
void dialog_null_backGroundWidget::hideAllFloatDialog()
{
  for(QVector<QWidget *>::iterator it=dVector.begin();it!=dVector.end();++it){
      (*it)->hide();
    }
}

void dialog_null_backGroundWidget::onePicLayout(int index)
{
  hideAllFloatDialog();
  int wid=screenWidth;
  int hei=screenHeight;
  dialog_null_floatDialog *fDialog=(dialog_null_floatDialog *)dVector.at(index);
  fDialog->resize(wid,hei);
  fDialog->layout();
  fDialog->move(10,10);
  fDialog->show();
}

void dialog_null_backGroundWidget::fourPicLayout()
{
  hideAllFloatDialog();
  for(int i=0;i<4;++i){
      dialog_null_floatDialog *fDialog=(dialog_null_floatDialog *)dVector.at(i);
      fDialog->resize(screenWidth/2,screenHeight/2);
      fDialog->layout();
    }
  dVector.at(0)->move(10,10);
  dVector.at(0)->show();
  dVector.at(1)->move(screenWidth/2,10);
  dVector.at(1)->show();
  dVector.at(2)->move(10,screenHeight/2);
  dVector.at(2)->show();
  dVector.at(3)->move(screenWidth/2,screenHeight/2);
  dVector.at(3)->show();
}
void dialog_null_backGroundWidget::eightPicLayout()
{
  hideAllFloatDialog();
  int wid=screenWidth/4;
  int hei=screenHeight/4;
  dialog_null_floatDialog *fDialog=(dialog_null_floatDialog *)dVector.at(0);
  fDialog->resize(wid*3,hei*3);
  fDialog->layout();
  for(int i=1;i<8;++i){
      dialog_null_floatDialog *smallDialog=(dialog_null_floatDialog *)dVector.at(i);
      smallDialog->resize(wid,hei);
      smallDialog->layout();
    }
  /*悬浮框的位置需要重新计算 */
  dVector.at(0)->move(10,10);
  dVector.at(0)->show();
  dVector.at(1)->move(wid*3+10,10);
  dVector.at(1)->show();
  dVector.at(2)->move(wid*3+10,hei*1+10);
  dVector.at(2)->show();
  dVector.at(3)->move(wid*3+10,hei*2+10);
  dVector.at(3)->show();
  dVector.at(4)->move(10,hei*3+10);
  dVector.at(4)->show();
  dVector.at(5)->move(wid*1+10,hei*3+10);
  dVector.at(5)->show();
  dVector.at(6)->move(wid*2+10,hei*3+10);
  dVector.at(6)->show();
  dVector.at(7)->move(wid*3+10,hei*3+10);
  dVector.at(7)->show();
}
void dialog_null_backGroundWidget::ninePicLayout()
{
  hideAllFloatDialog();
  int wid=screenWidth/3;
  int hei=screenHeight/3;
  for(int i=0;i<9;++i){
      dialog_null_floatDialog *fDialog=(dialog_null_floatDialog *)dVector.at(i);
      fDialog->resize(wid,hei);
      fDialog->layout();
    }

  dVector.at(0)->move(10,10);
  dVector.at(0)->show();
  dVector.at(1)->move(wid*1+10,10);
  dVector.at(1)->show();
  dVector.at(2)->move(wid*2+10,10);
  dVector.at(2)->show();
  dVector.at(3)->move(10,hei*1+10);
  dVector.at(3)->show();
  dVector.at(4)->move(wid*1+10,hei*1+10);
  dVector.at(4)->show();
  dVector.at(5)->move(wid*2+10,hei*1+10);
  dVector.at(5)->show();
  dVector.at(6)->move(10,hei*2+10);
  dVector.at(6)->show();
  dVector.at(7)->move(wid*1+10,hei*2+10);
  dVector.at(7)->show();
  dVector.at(8)->move(wid*2+10,hei*2+10);
  dVector.at(8)->show();
}
void dialog_null_backGroundWidget::sixteenPicLayout()
{
  hideAllFloatDialog();
  int wid=screenWidth/4;
  int hei=screenHeight/4;
  for(int i=0;i<16;++i){
      dialog_null_floatDialog *fDialog=(dialog_null_floatDialog *)dVector.at(i);
      fDialog->resize(wid,hei);
      fDialog->layout();
    }

  dVector.at(0)->move(10,10);
  dVector.at(0)->show();
  dVector.at(1)->move(wid*1+10,10);
  dVector.at(1)->show();
  dVector.at(2)->move(wid*2+10,10);
  dVector.at(2)->show();
  dVector.at(3)->move(wid*3+10,10);
  dVector.at(3)->show();

  dVector.at(4)->move(10,hei*1+10);
  dVector.at(4)->show();
  dVector.at(5)->move(wid*1+10,hei*1+10);
  dVector.at(5)->show();
  dVector.at(6)->move(wid*2+10,hei*1+10);
  dVector.at(6)->show();
  dVector.at(7)->move(wid*3+10,hei*1+10);
  dVector.at(7)->show();

  dVector.at(8)->move(10,hei*2+10);
  dVector.at(8)->show();
  dVector.at(9)->move(wid*1+10,hei*2+10);
  dVector.at(9)->show();
  dVector.at(10)->move(wid*2+10,hei*2+10);
  dVector.at(10)->show();
  dVector.at(11)->move(wid*3+10,hei*2+10);
  dVector.at(11)->show();

  dVector.at(12)->move(10,hei*3+10);
  dVector.at(12)->show();
  dVector.at(13)->move(wid*1+10,hei*3+10);
  dVector.at(13)->show();
  dVector.at(14)->move(wid*2+10,hei*3+10);
  dVector.at(14)->show();
  dVector.at(15)->move(wid*3+10,hei*3+10);
  dVector.at(15)->show();
}
void dialog_null_backGroundWidget::createNumbersWidget(int numbers)
{
  for(QVector<QWidget *>::iterator it=dVector.begin();it!=dVector.end();++it){
      (*it)->hide();
    }
}
