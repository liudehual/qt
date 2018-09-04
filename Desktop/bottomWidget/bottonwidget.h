#ifndef BOTTONWIDGET_H
#define BOTTONWIDGET_H

#include <QDialog>
#include <QString>
#include <QMap>

class QEvent;
class PushButton;
class dialog_null_backGroundWidget;
class QTabWidget;
class QTabBar;
namespace Ui {
  class dialog_null_bottomWidget;
}

class dialog_null_bottomWidget : public QDialog
{
  Q_OBJECT

public:
  explicit dialog_null_bottomWidget(QWidget *parent = 0);
  ~dialog_null_bottomWidget();
  int getSelfHeight()const{return selfHeight;}
signals:
  void sendMessageToProxy(QString);
public slots:
  void recvMessageFromProxy(QString);
protected:
  void enterEvent(QEvent *);
  void leaveEvent(QEvent *);
  //  bool event(QEvent *);
  bool eventFilter(QObject *, QEvent *);
private:
  void loadQssFile(QString);
  void createChildWidget();
  void startXml(QString);

private slots:
  void createNewWidget(int);
private:
  bool lockWindow;
  int width;
  int height;
  int childHeight;
  int childWidth;
  int selfHeight;
private:
  Ui::dialog_null_bottomWidget *ui;
  dialog_null_backGroundWidget *bgWidget;

  QMap<QString,QWidget*> tab;
  QMap<QString,QMap<QString,QString> > sensorData;
  QTabBar *bar;
};

#endif // dialog_null_bottonWidget_H
