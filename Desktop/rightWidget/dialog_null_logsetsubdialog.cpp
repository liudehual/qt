#include "dialog_null_logsetsubdialog.h"
#include "ui_dialog_null_logsetsubdialog.h"

dialog_null_logSetSubDialog::dialog_null_logSetSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_logSetSubDialog)
{
  ui->setupUi(this);
}

dialog_null_logSetSubDialog::~dialog_null_logSetSubDialog()
{
  delete ui;
}
