#include "videomanagerdialog.h"
#include "ui_videomanagerdialog.h"
#include <QMouseEvent>

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

dialog_null_videoManagerDialog::dialog_null_videoManagerDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_videoManagerDialog)
{
  ui->setupUi(this);
  QObject::connect(ui->pushButton_videoManagerDialog_findDevice,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogFindDevice()));
  QObject::connect(ui->pushButton_videoManagerDialog_addDevice,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogAddDevice()));
  QObject::connect(ui->pushButton_videoManagerDialog_addAll,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogAddAll()));
  QObject::connect(ui->pushButton_videoManagerDialog_move,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogMove()));
  QObject::connect(ui->pushButton_videoManagerDialog_moveall,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogMoveall()));
  QObject::connect(ui->pushButton_videoManagerDialog_nvrNetworkSet,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogNvrNetworkSet()));
  QObject::connect(ui->pushButton_videoManagerDialog_setIp,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogSetIp()));
  QObject::connect(ui->pushButton_videoManagerDialog_IpcVideoSource,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogIpcVideoSource()));
  QObject::connect(ui->pushButton_videoManagerDialog_channelSort,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogChannelSort()));
  QObject::connect(ui->pushButton_videoManagerDialog_sure,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogSure()));
  QObject::connect(ui->pushButton_videoManagerDialog_cancel,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogCancel()));
  QObject::connect(ui->pushButton_videoManagerDialog_use,SIGNAL(clicked()),this,SLOT(pushButtonVideoManagerDialogUse()));

}

dialog_null_videoManagerDialog::~dialog_null_videoManagerDialog()
{
  delete ui;
}
bool dialog_null_videoManagerDialog::eventFilter(QObject *object, QEvent *e)
{

  if(object->objectName()=="shutdown"){
      if(e->type()==QEvent::MouseButtonPress){
          QMouseEvent *mouse=dynamic_cast<QMouseEvent *>(e);
          if(mouse->button()==Qt::LeftButton){
                this->hide();
            }
        }
    }
  return QDialog::eventFilter(object,e);
}
void dialog_null_videoManagerDialog::initWidget()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogFindDevice()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogAddDevice()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogAddAll()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogMove()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogMoveall()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogNvrNetworkSet()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogSetIp()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogIpcVideoSource()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogChannelSort()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogSure()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogCancel()
{

}

void dialog_null_videoManagerDialog::pushButtonVideoManagerDialogUse()
{

}
