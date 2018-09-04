#include "dialog_null_picmanagersubdialog.h"
#include "ui_dialog_null_picmanagersubdialog.h"

dialog_null_picManagerSubDialog::dialog_null_picManagerSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_picManagerSubDialog)
{
  ui->setupUi(this);
}

dialog_null_picManagerSubDialog::~dialog_null_picManagerSubDialog()
{
  delete ui;
}
