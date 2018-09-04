#ifndef YUNTAISUBDIALOG_H
#define YUNTAISUBDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_yunTaiSubDialog;
}

class dialog_null_yunTaiSubDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_yunTaiSubDialog(QWidget *parent = 0);
  ~dialog_null_yunTaiSubDialog();
public slots:
  void checkBoxYunTaiSubDialogSelectAll(int);
  void pushButtonYunTaiSubDialogAdd();
  void pushButtonYunTaiSubDialogDel();

protected:
  bool eventFilter(QObject *, QEvent *);
private:

private:
  Ui::dialog_null_yunTaiSubDialog *ui;
};

#endif // dialog_null_yunTaiSubDialog_H
