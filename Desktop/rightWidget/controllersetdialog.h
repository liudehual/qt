#ifndef CONTROLLERSETDIALOG_H
#define CONTROLLERSETDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_controllerSetDialog;
}

class dialog_null_controllerSetDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_controllerSetDialog(QWidget *parent = 0);
  ~dialog_null_controllerSetDialog();
  void createFunctionAndConnection(QString objectName);
  void testFile();
protected:
  bool eventFilter(QObject *, QEvent *);
public slots:
  void comboBoxControllerSetDialogSensorLow(int);
  void comboBoxControllerSetDialogSensorHigh(int);
  void pushButtonControllerSetDialogUse();
  void pushButtonControllerSetDialogSure();
  void pushButtonControllerSetDialogCancel();
private:
  Ui::dialog_null_controllerSetDialog *ui;
};

#endif // CONTROLLERSETDIALOG_H
