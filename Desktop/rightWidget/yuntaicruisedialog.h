#ifndef DIALOG_NULL_YUNTAICRUISEDIALOG_H
#define DIALOG_NULL_YUNTAICRUISEDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_yunTaiCruiseDialog;
}

class dialog_null_yunTaiCruiseDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_yunTaiCruiseDialog(QWidget *parent = 0);
  ~dialog_null_yunTaiCruiseDialog();

private:
  Ui::dialog_null_yunTaiCruiseDialog *ui;
};

#endif // DIALOG_NULL_YUNTAICRUISEDIALOG_H
