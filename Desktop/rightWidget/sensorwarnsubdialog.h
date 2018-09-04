#ifndef DIALOG_NULL_SENSORWARNSUBDIALOG_H
#define DIALOG_NULL_SENSORWARNSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_sensorWarnSubDialog;
}

class dialog_null_sensorWarnSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_sensorWarnSubDialog(QWidget *parent = 0);
  ~dialog_null_sensorWarnSubDialog();

private:
  Ui::dialog_null_sensorWarnSubDialog *ui;
};

#endif // DIALOG_NULL_SENSORWARNSUBDIALOG_H
