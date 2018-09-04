#include "connector.h"
#include <QStackedWidget>
#include "rightWidget/systemSetDialog.h"
Connector::Connector(QStackedWidget *widget,dialog_null_systemSetDialog *dialog,QObject *parent) : QObject(parent)
{
  stackWidget=widget;
  dia=dialog;
}

void Connector::currentIndex0()
{
  if(stackWidget->count()>0){
      stackWidget->setCurrentIndex(0);
    }
}
void Connector::currentIndex1()
{
  if(stackWidget->count()>1){
      stackWidget->setCurrentIndex(1);
    }
}
void Connector::currentIndex2()
{
  if(stackWidget->count()>2){
      stackWidget->setCurrentIndex(2);
    }
}
void Connector::currentIndex3()
{
  if(stackWidget->count()>3){
      stackWidget->setCurrentIndex(3);
    }
}
void Connector::currentIndex4()
{
  if(stackWidget->count()>4){
      stackWidget->setCurrentIndex(4);
    }
}
void Connector::currentIndex5()
{
  if(stackWidget->count()>5){
      stackWidget->setCurrentIndex(5);
    }
}
