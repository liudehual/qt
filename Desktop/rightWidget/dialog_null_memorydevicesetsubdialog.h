#ifndef DIALOG_NULL_MEMORYDEVICESETSUBDIALOG_H
#define DIALOG_NULL_MEMORYDEVICESETSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_memoryDeviceSetSubDialog;
}

class dialog_null_memoryDeviceSetSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_memoryDeviceSetSubDialog(QWidget *parent = 0);
  ~dialog_null_memoryDeviceSetSubDialog();

private:
  Ui::dialog_null_memoryDeviceSetSubDialog *ui;
};

#endif // DIALOG_NULL_MEMORYDEVICESETSUBDIALOG_H
