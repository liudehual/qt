#include "memorydevicesetsubdialog.h"
#include "ui_memorydevicesetsubdialog.h"

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
QLabel *dialog_null_memoryDeviceSetSubDialog::getName()
{
  return ui->label_null_memoryDeviceSetSubDialog_device;
}
QLabel *dialog_null_memoryDeviceSetSubDialog::getSize()
{
  return ui->label_null_memoryDeviceSetSubDialog_size;
}
QLabel *dialog_null_memoryDeviceSetSubDialog::getUsed()
{
  return ui->label_null_memoryDeviceSetSubDialog_used;
}
QLabel *dialog_null_memoryDeviceSetSubDialog::getStatus()
{
  return ui->label_null_memoryDeviceSetSubDialog_status;
}
