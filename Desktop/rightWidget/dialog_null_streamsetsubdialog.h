#ifndef DIALOG_NULL_STREAMSETSUBDIALOG_H
#define DIALOG_NULL_STREAMSETSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_streamSetSubDialog;
}

class dialog_null_streamSetSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_streamSetSubDialog(QWidget *parent = 0);
  ~dialog_null_streamSetSubDialog();

private:
  Ui::dialog_null_streamSetSubDialog *ui;
};

#endif // DIALOG_NULL_STREAMSETSUBDIALOG_H
