#ifndef DIALOG_NULL_PICMANAGERSUBDIALOG_H
#define DIALOG_NULL_PICMANAGERSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_picManagerSubDialog;
}

class dialog_null_picManagerSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_picManagerSubDialog(QWidget *parent = 0);
  ~dialog_null_picManagerSubDialog();

private:
  Ui::dialog_null_picManagerSubDialog *ui;
};

#endif // DIALOG_NULL_PICMANAGERSUBDIALOG_H
