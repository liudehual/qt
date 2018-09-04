#include "dialog_null_memorydevicesetsubdialog.h"
#include "ui_dialog_null_memorydevicesetsubdialog.h"

dialog_null_memoryDeviceSetSubDialog::dialog_null_memoryDeviceSetSubDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::dialog_null_memoryDeviceSetSubDialog)
{
  ui->setupUi(this);
}

dialog_null_memoryDeviceSetSubDialog::~dialog_null_memoryDeviceSetSubDialog()
{
  delete ui;
}
