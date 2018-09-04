#include "dialog_null_devicewarnsetsubdialog.h"
#include "ui_dialog_null_devicewarnsetsubdialog.h"

dialog_null_deviceWarnSetSubDialog::dialog_null_deviceWarnSetSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_deviceWarnSetSubDialog)
{
  ui->setupUi(this);
}

dialog_null_deviceWarnSetSubDialog::~dialog_null_deviceWarnSetSubDialog()
{
  delete ui;
}
