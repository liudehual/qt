#include "yuntaisubdialog.h"
#include "ui_yuntaisubdialog.h"
#include <QMouseEvent>
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

dialog_null_yunTaiSubDialog::dialog_null_yunTaiSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_yunTaiSubDialog)
{
  ui->setupUi(this);
  ui->lineEdit_yunTaiSubDialog_startTime->installEventFilter(this);
  ui->lineEdit_yunTaiSubDialog_cruiseSpeed->installEventFilter(this);
  ui->lineEdit_yunTaiSubDialog_cruiseNumbers->installEventFilter(this);
}

dialog_null_yunTaiSubDialog::~dialog_null_yunTaiSubDialog()
{
  delete ui;
}
bool dialog_null_yunTaiSubDialog::eventFilter(QObject *object, QEvent *e)
{
  if(object->objectName()=="shutdown"){
      if(e->type()==QEvent::MouseButtonPress){
          QMouseEvent *mouse=dynamic_cast<QMouseEvent *>(e);
          if(mouse->button()==Qt::LeftButton){
              this->hide();
            }
        }
    }
  if(e->type()==QEvent::MouseButtonPress){
      if(object->objectName()== "lineEdit_yunTaiSubDialog_startTime" ||
         object->objectName()== "lineEdit_yunTaiSubDialog_cruiseSpeed" ||
         object->objectName()== "lineEdit_yunTaiSubDialog_cruiseNumbers"){
          debug("%s\n","=============");
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
          sk->show();
          sk->setLineText(lEdit);
        }
    }
  return QDialog::eventFilter(object,e);
}

void dialog_null_yunTaiSubDialog::checkBoxYunTaiSubDialogSelectAll(int flags)
{

}

void dialog_null_yunTaiSubDialog::pushButtonYunTaiSubDialogAdd()
{

}

void dialog_null_yunTaiSubDialog::pushButtonYunTaiSubDialogDel()
{

}
