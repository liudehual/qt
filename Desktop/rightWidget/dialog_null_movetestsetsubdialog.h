#ifndef DIALOG_NULL_MOVETESTSETSUBDIALOG_H
#define DIALOG_NULL_MOVETESTSETSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_moveTestSetSubDialog;
}

class dialog_null_moveTestSetSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_moveTestSetSubDialog(QWidget *parent = 0);
  ~dialog_null_moveTestSetSubDialog();

private:
  Ui::dialog_null_moveTestSetSubDialog *ui;
};

#endif // DIALOG_NULL_MOVETESTSETSUBDIALOG_H
