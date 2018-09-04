#include "backgroundWidget/backgroundwidget.h"
#include <QApplication>
#include <QTextCodec>
//#include <QWSServer>
#include "bottomWidget/bottonwidget.h"
#include "rightWidget/rightwidget.h"
#include <QFontDatabase>
#include <QFont>
#include "tools/widgetpool.h"
#include <QFile>
#include "tools/networkthread.h"
#include "rightWidget/edituserdialog.h"

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

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  /*设置透明背景*/
 // QWSServer::setBackground(QColor(0,0,0,0));

  NetworkThread *thread=new NetworkThread;
  thread->setObjectName("NetworkThread");
  debug("%s\n",thread->objectName().toStdString().c_str());
  thread->start();

  /*设置编码格式*/
  //QTextCodec::setCodecForTr( QTextCodec::codecForName("UTF-8") );

  /*背景窗口*/
  dialog_null_backGroundWidget w;
  w.setObjectName("dialog_null_backGroundWidget");
  WidgetPool::topWidget.insert("dialog_null_backGroundWidget",&w);
  w.show();

  /*底侧窗口*/
  dialog_null_bottomWidget bWidget;
  bWidget.setObjectName("dialog_null_bottomWidget");
  dialog_null_backGroundWidget::addToMap(bWidget.objectName(),&bWidget);
  WidgetPool::topWidget.insert("dialog_null_bottomWidget",&bWidget);
  bWidget.show();

  /*右侧窗口*/
  dialog_null_rightWidget pWidget;
  pWidget.setObjectName("dialog_null_rightWidget");
  dialog_null_backGroundWidget::addToMap(pWidget.objectName(),&pWidget);
  WidgetPool::topWidget.insert("dialog_null_rightWidget",&pWidget);
  pWidget.show();

  return a.exec();
}
