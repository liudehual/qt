#ifndef DIALOG_NULL_YUNTAIPRESETDIALOG_H
#define DIALOG_NULL_YUNTAIPRESETDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_yunTaiPresetDialog;
}

class dialog_null_yunTaiPresetDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_yunTaiPresetDialog(QWidget *parent = 0);
  ~dialog_null_yunTaiPresetDialog();

private:
  Ui::dialog_null_yunTaiPresetDialog *ui;
};

#endif // DIALOG_NULL_YUNTAIPRESETDIALOG_H
