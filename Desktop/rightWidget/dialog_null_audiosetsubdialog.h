#ifndef DIALOG_NULL_AUDIOSETSUBDIALOG_H
#define DIALOG_NULL_AUDIOSETSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_audioSetSubDialog;
}

class dialog_null_audioSetSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_audioSetSubDialog(QWidget *parent = 0);
  ~dialog_null_audioSetSubDialog();

private:
  Ui::dialog_null_audioSetSubDialog *ui;
};

#endif // DIALOG_NULL_AUDIOSETSUBDIALOG_H
