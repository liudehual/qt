#ifndef RightWidget_H
#define RightWidget_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QMap>

namespace Ui {
  class dialog_null_rightWidget;
}
class PushButton;
class BackGroundWidget;
class QMenu;
class QDialog;
class dialog_null_rightWidget : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_rightWidget(QWidget *parent = 0);

  virtual ~dialog_null_rightWidget();

protected:
  void enterEvent(QEvent *);
  void leaveEvent(QEvent *);
  bool event(QEvent *);
  bool eventFilter(QObject *, QEvent *);
  void paintEvent(QPaintEvent *);
private:
  void loadQssFile(QString);
  void createConnection();
  void createChildWidget();
  void hideOtherDialog();
public slots:
  void lockRightWidget();
  void putOutRightWidget();

  void systemSetSlot();
  void yunTaiCtrloSlot();
  void playBackSlot();
  void remoteManagerSlot();
  void sensorSlot();
  void controllerSlot();
  void picLayoutSlot();
  void shutdownSlot();

private:
  bool lockWindow;
  bool ifPutOut;
  int width;
  int height;
  int childHeight;
  int childWidth;

private:
  QVector<PushButton *>pVector; //控件
  QMap<QString,QDialog*> dMap;  //下一级窗口
private:
  Ui::dialog_null_rightWidget *ui;
  BackGroundWidget *bgWidget;
};

#endif // RightWidget_H
