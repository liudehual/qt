#include "tipdialog.h"
#include "ui_tipdialog.h"
dialog_null_TipDialog *dialog_null_TipDialog::tip=NULL;
dialog_null_TipDialog::dialog_null_TipDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_TipDialog)
{
  ui->setupUi(this);
  ui->label_tipDialog_shutdown->installEventFilter(this);
  QObject::connect(ui->pushButton_tipDialog_cancel,SIGNAL(clicked()),this,SLOT(cancel()));
}

dialog_null_TipDialog::~dialog_null_TipDialog()
{
  delete ui;
}
dialog_null_TipDialog *dialog_null_TipDialog::instance()
{
  if(tip==NULL){
      tip=new dialog_null_TipDialog;
    }
  return tip;
}
bool dialog_null_TipDialog::eventFilter(QObject *object, QEvent *e)
{
  if(object->objectName()=="label_tipDialog_shutdown"){
      if(e->type()==QEvent::MouseButtonPress){
          this->hide();
        }
    }
}
void dialog_null_TipDialog::setPushButton(QPushButton *b){
  QObject::disconnect(ui->pushButton_tipDialog_sure,SIGNAL(clicked()),button,SIGNAL(clicked()));
  button=b;
  QObject::connect(ui->pushButton_tipDialog_sure,SIGNAL(clicked()),button,SIGNAL(clicked()));
}
void dialog_null_TipDialog::cancel()
{
  this->hide();
}
void dialog_null_TipDialog::show()
{
  this->setWindowModality(Qt::ApplicationModal);
  QDialog::show();
}
