#ifndef EDITUSERDIALOG_H
#define EDITUSERDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_editUserDialog;
}

class dialog_null_editUserDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_editUserDialog(QWidget *parent = 0);
  ~dialog_null_editUserDialog();
public:
  void testFile();
  void createFunctionAndConnection(QString objectName);

protected:
  bool eventFilter(QObject *, QEvent *);
public slots:
  void checkBoxEditUserDialogVideoManager(int);
  void checkBoxEditUserDialogConntionManager(int);
  void checkBoxEditUserDialogShutdown(int);
  void checkBoxEditUserDialogYunTaiControl(int);
  void checkBoxEditUserDialogSystemSet(int);
  void checkBoxEditUserDialogVideoExchange(int);
  void checkBoxEditUserDialogCapturePicManager(int);
  void checkBoxEditUserDialogUserManager(int);
  void pushButtonEditUserDialogNoSelectAll();
  void pushButtonEditUserDialogSure();
  void pushButtonEditUserDialogCancel();
  void pushButtonEditUserDialogSelectAll();
private:
  Ui::dialog_null_editUserDialog *ui;
  QString operation;
};

#endif // EDITUSERDIALOG_H
