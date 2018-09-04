#include "sensorwarnsubdialog.h"
#include "ui_sensorwarnsubdialog.h"

dialog_null_sensorWarnSubDialog::dialog_null_sensorWarnSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_sensorWarnSubDialog)
{
  ui->setupUi(this);
}

dialog_null_sensorWarnSubDialog::~dialog_null_sensorWarnSubDialog()
{
  delete ui;
}
