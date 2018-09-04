#include "dialog_null_sensorgeneralsubdialog.h"
#include "ui_dialog_null_sensorgeneralsubdialog.h"

dialog_null_sensorGeneralSubDialog::dialog_null_sensorGeneralSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_sensorGeneralSubDialog)
{
  ui->setupUi(this);
}

dialog_null_sensorGeneralSubDialog::~dialog_null_sensorGeneralSubDialog()
{
  delete ui;
}
