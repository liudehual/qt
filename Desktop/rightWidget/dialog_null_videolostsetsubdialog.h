#ifndef DIALOG_NULL_VIDEOLOSTSETSUBDIALOG_H
#define DIALOG_NULL_VIDEOLOSTSETSUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_videoLostSetSubDialog;
}

class dialog_null_videoLostSetSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_videoLostSetSubDialog(QWidget *parent = 0);
  ~dialog_null_videoLostSetSubDialog();

private:
  Ui::dialog_null_videoLostSetSubDialog *ui;
};

#endif // DIALOG_NULL_VIDEOLOSTSETSUBDIALOG_H
