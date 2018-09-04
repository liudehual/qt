#ifndef VIDEOMANAGERDIALOG_H
#define VIDEOMANAGERDIALOG_H

#include <QDialog>

namespace Ui {
  class dialog_null_videoManagerDialog;
}

class dialog_null_videoManagerDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_videoManagerDialog(QWidget *parent = 0);
  ~dialog_null_videoManagerDialog();

public slots:
  void pushButtonVideoManagerDialogFindDevice();
  void pushButtonVideoManagerDialogAddDevice();
  void pushButtonVideoManagerDialogAddAll();
  void pushButtonVideoManagerDialogMove();
  void pushButtonVideoManagerDialogMoveall();
  void pushButtonVideoManagerDialogNvrNetworkSet();
  void pushButtonVideoManagerDialogSetIp();
  void pushButtonVideoManagerDialogIpcVideoSource();
  void pushButtonVideoManagerDialogChannelSort();
  void pushButtonVideoManagerDialogSure();
  void pushButtonVideoManagerDialogCancel();
  void pushButtonVideoManagerDialogUse();
protected:
  bool eventFilter(QObject *, QEvent *);
  private:
  void initWidget();
private:
  Ui::dialog_null_videoManagerDialog *ui;
};

#endif // VIDEOMANAGERDIALOG_H
