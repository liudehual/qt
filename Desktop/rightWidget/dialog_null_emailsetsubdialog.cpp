#include "dialog_null_emailsetsubdialog.h"
#include "ui_dialog_null_emailsetsubdialog.h"

dialog_null_emailSetSubDialog::dialog_null_emailSetSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_emailSetSubDialog)
{
  ui->setupUi(this);
}

dialog_null_emailSetSubDialog::~dialog_null_emailSetSubDialog()
{
  delete ui;
}
