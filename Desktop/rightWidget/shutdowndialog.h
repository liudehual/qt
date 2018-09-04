#ifndef SHUTDOWNDIALOG_H
#define SHUTDOWNDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_shutdownDialog;
}

class dialog_null_shutdownDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_shutdownDialog(QWidget *parent = 0);
  ~dialog_null_shutdownDialog();
signals:
  void sendMessageToProxy(QString);
public slots:
  void recvMessageFromProxy(QString);
  void pushButtonShutdownDialogRestart();
  void pushButtonShutdownDialogLoginout();
  void pushButtonShutdownDialogShutdown();
protected:
  bool eventFilter(QObject *, QEvent *);
private:
  void initWidget();
  void loadQssFile(QString fileName);

private:
  Ui::dialog_null_shutdownDialog *ui;
  static QString xmlHead;
  static QString xmlEnd;
};

#endif // dialog_null_shutdownDialog_H
