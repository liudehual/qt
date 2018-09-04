#ifndef DIALOG_NULL_VIDEOMANAGERADDEDDIALOG_H
#define DIALOG_NULL_VIDEOMANAGERADDEDDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_videoManagerAddedDialog;
}

class dialog_null_videoManagerAddedDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_videoManagerAddedDialog(QWidget *parent = 0);
  ~dialog_null_videoManagerAddedDialog();

private:
  Ui::dialog_null_videoManagerAddedDialog *ui;
};

#endif // DIALOG_NULL_VIDEOMANAGERADDEDDIALOG_H
