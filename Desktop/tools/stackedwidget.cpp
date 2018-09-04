#include "stackedwidget.h"

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

StackedWidget::StackedWidget(QWidget *parent):QStackedWidget(parent)
{

}
void StackedWidget::currentIndex0()
{
  debug("%s\n","StackedWidget::currentIndex0");
  if(this->count()>0){
       this->setCurrentIndex(0);
    }
}
void StackedWidget::currentIndex1()
{
  debug("%s\n","StackedWidget::currentIndex1");

  if(this->count()>1){
       this->setCurrentIndex(1);
    }
}
void StackedWidget::currentIndex2()
{
  debug("%s\n","StackedWidget::currentIndex2");

  if(this->count()>2){
       this->setCurrentIndex(2);
    }
}
void StackedWidget::currentIndex3()
{
  debug("%s\n","StackedWidget::currentIndex3");

  if(this->count()>3){
       this->setCurrentIndex(3);
    }
}
void StackedWidget::currentIndex4()
{
  debug("%s\n","StackedWidget::currentIndex4");

  if(this->count()>4){
       this->setCurrentIndex(4);
    }
}
void StackedWidget::currentIndex5()
{
  debug("%s\n","StackedWidget::currentIndex5");

  if(this->count()>5){
       this->setCurrentIndex(5);
    }
}
void StackedWidget::currentIndex6()
{
  debug("%s\n","StackedWidget::currentIndex6");

  if(this->count()>6){
       this->setCurrentIndex(6);
    }
}
