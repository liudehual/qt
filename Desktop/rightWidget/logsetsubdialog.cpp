#include "logsetsubdialog.h"
#include "ui_logsetsubdialog.h"

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
QLabel *dialog_null_logSetSubDialog::getEvent()
{
  return ui->label_logSetSubDialog_event;
}
QLabel *dialog_null_logSetSubDialog::getTime()
{
  return ui->label_logSetSubDialog_Time;
}
