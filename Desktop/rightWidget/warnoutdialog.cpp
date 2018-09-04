#include "warnoutdialog.h"
#include "ui_warnoutdialog.h"

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
dialog_null_warnOutDialog::dialog_null_warnOutDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_warnOutDialog)
{
  ui->setupUi(this);

  QObject::connect(ui->checkBox_warnOutDialog_sendToOther,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWarnOutDialogSendToOther(int)));
  QObject::connect(ui->checkBox_warnOutDialog_emailWarn,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWarnOutDialogEmailWarn(int)));
  QObject::connect(ui->checkBox_warnOutDialog_openVideo,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWarnOutDialogOpenVideo(int)));
  QObject::connect(ui->checkBox_warnOutDialog_sendPic,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWarnOutDialogSendPic(int)));
  QObject::connect(ui->checkBox_warnOutDialog_capturePic,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWarnOutDialogCapturePic(int)));
  QObject::connect(ui->checkBox_warnOutDialog_callPreset,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWarnOutDialogCallPreset(int)));
  QObject::connect(ui->checkBox_warnOutDialog_openWarnOut,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWarnOutDialogOpenWarnOut(int)));
  QObject::connect(ui->checkBox_warnOutDialog_warn,SIGNAL(stateChanged(int)),this,SLOT(checkBoxWarnOutDialogWarn(int)));
  QObject::connect(ui->comboBox_warnOutDialog_sendToOther,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxWarnOutDialogSendToOther(int)));
  QObject::connect(ui->comboBox_warnOutDialog_sendPic,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxWarnOutDialogSendPic(int)));
  QObject::connect(ui->comboBox_warnOutDialog_openVideo,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxWarnOutDialogOpenVideo(int)));
  QObject::connect(ui->comboBox_warnOutDialog_callPreset,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxWarnOutDialogCallPreset(int)));
  QObject::connect(ui->comboBox_warnOutDialog_capturePic,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxWarnOutDialogCapturePic(int)));
  QObject::connect(ui->comboBox_warnOutDialog_id,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxWarnOutDialogId(int)));
  QObject::connect(ui->pushButton_warnOutDialog_use,SIGNAL(clicked()),this,SLOT(pushButtonWarnOutDialogUse()));
  QObject::connect(ui->pushButton_warnOutDialog_sure,SIGNAL(clicked()),this,SLOT(pushButtonWarnOutDialogSure()));
  QObject::connect(ui->pushButton_warnOutDialog_cancel,SIGNAL(clicked()),this,SLOT(pushButtonWarnOutDialogCancel()));

  ui->lineEdit_warnOutDialog_continueTime->installEventFilter(this);

}

dialog_null_warnOutDialog::~dialog_null_warnOutDialog()
{
  delete ui;
}

bool dialog_null_warnOutDialog::eventFilter(QObject *object, QEvent *e)
{
  if(object->objectName()== "lineEdit_warnOutDialog_continueTime"){

    }
  return QDialog::eventFilter(object,e);
}
void dialog_null_warnOutDialog::checkBoxWarnOutDialogSendToOther(int flags)
{

}

void dialog_null_warnOutDialog::checkBoxWarnOutDialogEmailWarn(int flags)
{

}

void dialog_null_warnOutDialog::checkBoxWarnOutDialogOpenVideo(int flags)
{

}

void dialog_null_warnOutDialog::checkBoxWarnOutDialogSendPic(int flags)
{

}

void dialog_null_warnOutDialog::checkBoxWarnOutDialogCapturePic(int flags)
{

}

void dialog_null_warnOutDialog::checkBoxWarnOutDialogCallPreset(int flags)
{

}

void dialog_null_warnOutDialog::checkBoxWarnOutDialogOpenWarnOut(int flags)
{

}

void dialog_null_warnOutDialog::checkBoxWarnOutDialogWarn(int flags)
{

}

void dialog_null_warnOutDialog::comboBoxWarnOutDialogSendToOther(int index)
{

}

void dialog_null_warnOutDialog::comboBoxWarnOutDialogSendPic(int index)
{

}

void dialog_null_warnOutDialog::comboBoxWarnOutDialogOpenVideo(int index)
{

}

void dialog_null_warnOutDialog::comboBoxWarnOutDialogCallPreset(int index)
{

}

void dialog_null_warnOutDialog::comboBoxWarnOutDialogCapturePic(int index)
{

}

void dialog_null_warnOutDialog::comboBoxWarnOutDialogId(int index)
{

}

void dialog_null_warnOutDialog::pushButtonWarnOutDialogUse()
{

}

void dialog_null_warnOutDialog::pushButtonWarnOutDialogSure()
{

}

void dialog_null_warnOutDialog::pushButtonWarnOutDialogCancel()
{

}
