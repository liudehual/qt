#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_PreviewSetItemDialog;
}

class dialog_null_PreviewSetItemDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_PreviewSetItemDialog(QWidget *parent = 0);
  ~dialog_null_PreviewSetItemDialog();

private:
  Ui::dialog_null_PreviewSetItemDialog *ui;
};

#endif // PREVIEWDIALOG_H
