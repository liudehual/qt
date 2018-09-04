#ifndef DIALOG_NULL_DEVICEWARNSETSUBDIALOG_H
#define DIALOG_NULL_DEVICEWARNSETSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_deviceWarnSetSubDialog;
}

class dialog_null_deviceWarnSetSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_deviceWarnSetSubDialog(QWidget *parent = 0);
  ~dialog_null_deviceWarnSetSubDialog();

private:
  Ui::dialog_null_deviceWarnSetSubDialog *ui;
};

#endif // DIALOG_NULL_DEVICEWARNSETSUBDIALOG_H
