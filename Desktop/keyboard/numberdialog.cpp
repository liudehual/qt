#include "numberdialog.h"
#include "ui_numberdialog.h"

#include <QLineEdit>
#include <QPoint>
#define DEBUG 0

#if (DEBUG==0)
#include <stdio.h>
#include <stdlib.h>
#define debug(fmt,...) fprintf(stderr," "fmt"File: %s LineNumber:%d",__VA_ARGS__,__FILE__,__LINE__)
#elif (DEBUG==1)
#include <stdio.h>
#include <stdlib.h>
#define debug(fmt,...) fprintf(stderr,fmt,__VA_ARGS__)
#elif (DEBUG==2)
#define debug(fmt,...)
#endif

NumberDialog *NumberDialog::m_instance=NULL;

NumberDialog::NumberDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NumberDialog)
{
  ui->setupUi(this);
  m_edit=NULL;
  setWindowFlags(Qt::CustomizeWindowHint);
  createConnection();
  QPoint point=cursor().pos();
  move(point);
}

NumberDialog::~NumberDialog()
{
  delete ui;
}
NumberDialog *NumberDialog::instance()
{
  if(m_instance==NULL){
      m_instance=new NumberDialog;
    }
  return m_instance;
}
void NumberDialog::pushButtonOneSlot()
{
  QString text=ui->pushButton_one->text();
  debug("%s ",text.toStdString().c_str());
  setLineEditText(text);
}
void NumberDialog::pushButtonTwoSlot()
{
  QString text=ui->pushButton_two->text();
  debug("%s ",text.toStdString().c_str());
  setLineEditText(text);
}
void NumberDialog::pushButtonThreeSlot()
{
  QString text=ui->pushButton_three->text();
  debug("%s ",text.toStdString().c_str());
  setLineEditText(text);

}
void NumberDialog::pushButtonFourSlot()
{
  QString text=ui->pushButton_four->text();
  debug("%s ",text.toStdString().c_str());
  setLineEditText(text);

}
void NumberDialog::pushButtonFiveSlot()
{
  QString text=ui->pushButton_five->text();
  debug("%s ",text.toStdString().c_str());
  setLineEditText(text);

}
void NumberDialog::pushButtonSixSlot()
{
  QString text=ui->pushButton_six->text();
  debug("%s",text.toStdString().c_str());
  setLineEditText(text);

}
void NumberDialog::pushButtonSevenSlot()
{
  QString text=ui->pushButton_seven->text();
  debug("%s ",text.toStdString().c_str());
  setLineEditText(text);

}
void NumberDialog::pushButtonEightSlot()
{
  QString text=ui->pushButton_eight->text();
  debug("%s ",text.toStdString().c_str());
  setLineEditText(text);

}
void NumberDialog::pushButtonNineSlot()
{
  QString text=ui->pushButton_nine->text();
  debug("%s ",text.toStdString().c_str());
  setLineEditText(text);

}
void NumberDialog::pushButtonZeroSlot()
{
  QString text=ui->pushButton_zero->text();
  debug("%s ",text.toStdString().c_str());
  setLineEditText(text);

}
void NumberDialog::pushButtonBackspaceSlot()
{
  debug("%s ",ui->pushButton_backspace->text().toStdString().c_str());
  if(m_edit==NULL){
      return;
    }
  m_edit->backspace();
}
void NumberDialog::pushButtonOkSlot()
{
  debug("%s ",ui->pushButton_ok->text().toStdString().c_str());
  this->close();
}
void NumberDialog::pushButtonDelSlot()
{
  debug("%s ",ui->pushButton_del->text().toStdString().c_str());
  if(m_edit==NULL){
      return;
    }
  m_edit->del();
}
void NumberDialog::pushButtonPointSlot()
{
  QString text=ui->pushButton_point->text();
  debug("%s ",text.toStdString().c_str());
  setLineEditText(text);
}

void NumberDialog::createConnection()
{
  QObject::connect(ui->pushButton_one,SIGNAL(clicked()),this,SLOT(pushButtonOneSlot()));
  QObject::connect(ui->pushButton_two,SIGNAL(clicked()),this,SLOT(pushButtonTwoSlot()));
  QObject::connect(ui->pushButton_three,SIGNAL(clicked()),this,SLOT(pushButtonThreeSlot()));
  QObject::connect(ui->pushButton_four,SIGNAL(clicked()),this,SLOT(pushButtonFourSlot()));
  QObject::connect(ui->pushButton_five,SIGNAL(clicked()),this,SLOT(pushButtonFiveSlot()));
  QObject::connect(ui->pushButton_six,SIGNAL(clicked()),this,SLOT(pushButtonSixSlot()));
  QObject::connect(ui->pushButton_seven,SIGNAL(clicked()),this,SLOT(pushButtonSevenSlot()));
  QObject::connect(ui->pushButton_eight,SIGNAL(clicked()),this,SLOT(pushButtonEightSlot()));
  QObject::connect(ui->pushButton_nine,SIGNAL(clicked()),this,SLOT(pushButtonNineSlot()));
  QObject::connect(ui->pushButton_zero,SIGNAL(clicked()),this,SLOT(pushButtonZeroSlot()));
  QObject::connect(ui->pushButton_backspace,SIGNAL(clicked()),this,SLOT(pushButtonBackspaceSlot()));
  QObject::connect(ui->pushButton_ok,SIGNAL(clicked()),this,SLOT(pushButtonOkSlot()));
  QObject::connect(ui->pushButton_del,SIGNAL(clicked()),this,SLOT(pushButtonDelSlot()));
  QObject::connect(ui->pushButton_point,SIGNAL(clicked()),this,SLOT(pushButtonPointSlot()));

}
void NumberDialog::setLineEditText(QString text)
{
  if(m_edit==NULL){
      return;
    }
  QString editContent=m_edit->text();
  editContent+=text;
  m_edit->setText(editContent);
}
