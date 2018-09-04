#ifndef FLOATDIALOG_H
#define FLOATDIALOG_H

#include <QDialog>
#include <QMap>
/*该窗口主要用于backgroundWidget上的浮动窗口，不包括rightWidget和bottomWidget*/
class Label;
class dialog_null_floatDialog : public QDialog
{
  Q_OBJECT
public:
  explicit dialog_null_floatDialog(QWidget *parent=0);
  void setChannelId(int id);
  void cteateChildWidget();
  void layoutModel(int model);
  void layout();
  void setIndex(int index);
private:
protected:
  bool eventFilter(QObject *, QEvent *);
  void enterEvent(QEvent *);
  void leaveEvent(QEvent *);
private:

private:
  int myIndex;

  bool isVideo;
  bool isSound;
  bool isWarn;
  bool isNet;

  QMap<QString,QWidget *> LMap;//label
  QMap<QString,QWidget *> WMap;//QDialog

};
#endif // FLOATDIALOG_H
