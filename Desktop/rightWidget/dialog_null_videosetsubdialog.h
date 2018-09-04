#ifndef DIALOG_NULL_VIDEOSETSUBDIALOG_H
#define DIALOG_NULL_VIDEOSETSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_videoSetSubDialog;
}

class dialog_null_videoSetSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_videoSetSubDialog(QWidget *parent = 0);
  ~dialog_null_videoSetSubDialog();

private:
  Ui::dialog_null_videoSetSubDialog *ui;
};

#endif // DIALOG_NULL_VIDEOSETSUBDIALOG_H
