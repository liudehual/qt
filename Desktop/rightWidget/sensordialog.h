#ifndef SENSORDIALOG_H
#define SENSORDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_sensorDialog;
}

class dialog_null_sensorDialog : public QDialog
{
  Q_OBJECT
public:
  explicit dialog_null_sensorDialog(QWidget *parent = 0);
  ~dialog_null_sensorDialog();
  void createFunctionAndConnection(QString objectName);
  void testFile();
public slots:
  void pushButtonGeneralUse();
  void pushButtonGeneralSure();
  void pushButtonGeneralCancel();
  void pushButtonWarnUse();
  void pushButtonWarnSure();
  void pushButtonWarnCancel();
protected:
  bool eventFilter(QObject *, QEvent *);
private:
  void initWidget();
  void loadQssFile(QString fileName);

private:
  Ui::dialog_null_sensorDialog *ui;
};

#endif // dialog_null_sensorDialog_H
