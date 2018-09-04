#ifndef DIALOG_NULL_LOGSETSUBDIALOG_H
#define DIALOG_NULL_LOGSETSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_logSetSubDialog;
}
class QLabel;
class dialog_null_logSetSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_logSetSubDialog(QWidget *parent = 0);
  ~dialog_null_logSetSubDialog();
  QLabel *getEvent();
  QLabel *getTime();
private:
  Ui::dialog_null_logSetSubDialog *ui;
};

#endif // DIALOG_NULL_LOGSETSUBDIALOG_H
