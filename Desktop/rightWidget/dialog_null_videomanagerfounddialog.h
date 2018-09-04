#ifndef DIALOG_NULL_VIDEOMANAGERFOUNDDIALOG_H
#define DIALOG_NULL_VIDEOMANAGERFOUNDDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_videoManagerFoundDialog;
}

class dialog_null_videoManagerFoundDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_videoManagerFoundDialog(QWidget *parent = 0);
  ~dialog_null_videoManagerFoundDialog();

private:
  Ui::dialog_null_videoManagerFoundDialog *ui;
};

#endif // DIALOG_NULL_VIDEOMANAGERFOUNDDIALOG_H
