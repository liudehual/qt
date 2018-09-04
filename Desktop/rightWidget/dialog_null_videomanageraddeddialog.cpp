#include "dialog_null_videomanageraddeddialog.h"
#include "ui_dialog_null_videomanageraddeddialog.h"

dialog_null_videoManagerAddedDialog::dialog_null_videoManagerAddedDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_videoManagerAddedDialog)
{
  ui->setupUi(this);
}

dialog_null_videoManagerAddedDialog::~dialog_null_videoManagerAddedDialog()
{
  delete ui;
}
