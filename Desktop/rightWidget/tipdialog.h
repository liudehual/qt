#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_TipDialog;
}

class dialog_null_TipDialog : public QDialog
{
  Q_OBJECT

public:
  static dialog_null_TipDialog *instance();
  void setPushButton(QPushButton *);
  void show();
private slots:
  void cancel();

protected:
  bool eventFilter(QObject *, QEvent *);
public:
  explicit dialog_null_TipDialog(QWidget *parent = 0);
  ~dialog_null_TipDialog();

private:
  Ui::dialog_null_TipDialog *ui;
  static dialog_null_TipDialog *tip;
  QPushButton *button;
};

#endif // TIPDIALOG_H
