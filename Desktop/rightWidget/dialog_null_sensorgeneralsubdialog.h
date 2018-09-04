#ifndef DIALOG_NULL_SENSORGENERALSUBDIALOG_H
#define DIALOG_NULL_SENSORGENERALSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_sensorGeneralSubDialog;
}

class dialog_null_sensorGeneralSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_sensorGeneralSubDialog(QWidget *parent = 0);
  ~dialog_null_sensorGeneralSubDialog();

private:
  Ui::dialog_null_sensorGeneralSubDialog *ui;
};

#endif // DIALOG_NULL_SENSORGENERALSUBDIALOG_H
