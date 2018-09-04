#ifndef YUNTAIDIALOG_H
#define YUNTAIDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
  class dialog_null_yunTaiDialog;
}

class dialog_null_yunTaiDialog : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_yunTaiDialog(QWidget *parent = 0);
  ~dialog_null_yunTaiDialog();
  void myHide();
signals:
  void sendMessageToProxy(QString);
public slots:
  void recvMessageFromProxy(QString);
public slots:

  void setSpeed(int);

  void showSubDialog();
  void pushButtonYunTaiDialogCenter();
  void pushButtonYunTaiDialogDown();
  void pushButtonYunTaiDialogRight();
  void pushButtonYunTaiDialogLeft();
  void pushButtonYunTaiDialogUp();
  void pushButtonYunTaiDialogApertureSub();
  void pushButtonYunTaiDialogApertureAdd();
  void pushButtonYunTaiDialogZoomedAdd();
  void pushButtonYunTaiDialogZoomedSub();
  void pushButtonYunTaiDialogZoomingAdd();
  void pushButtonYunTaiDialogZoomingSub();
  void pushButtonYunTaiDialogMore();
  void comboBoxYunTaiDialogChannel(int);
protected:
  bool eventFilter(QObject *, QEvent *);

private:
  void initWidget();
  void loadQssFile(QString fileName);

private:
 QMap<QString,QDialog*> subMap;
 bool subDialogShowing;

 int aperture;
 int zoomed;
 int zooming;
private:
  Ui::dialog_null_yunTaiDialog *ui;

};

#endif // dialog_null_yunTaiDialog_H
