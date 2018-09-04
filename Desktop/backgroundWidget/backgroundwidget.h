#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <QDialog>
#include <QMap>
namespace Ui {
  class dialog_null_backGroundWidget;
}
class RightWidget;
class BottonWidget;
class Label;

class dialog_null_backGroundWidget : public QDialog
{
  Q_OBJECT
public:
  explicit dialog_null_backGroundWidget(QWidget *parent = 0);
  ~dialog_null_backGroundWidget();
  static void addToMap(QString,QDialog*);
  static QDialog *getDialogFromMap(QString);

public:
  void onePicLayout(int index);
  void fourPicLayout();
  void eightPicLayout();
  void ninePicLayout();
  void sixteenPicLayout();
signals:

public slots:
  void testClicked();
  void timeoutSlot();
  void createNumbersWidget(int numbers);

protected:
  void paintEvent(QPaintEvent *);
  bool eventFilter(QObject *, QEvent *);
private:
  void createChildWidget();
  void hideAllFloatDialog();

private:
  Ui::dialog_null_backGroundWidget *ui;
  Label *lab; //时间
  int screenWidth; //屏幕宽
  int screenHeight; //屏幕高

  static QMap<QString,QWidget *> dialogMap; //顶级窗口

  QVector<QWidget *> dVector; //子窗口

};

#endif // dialog_null_backGroundWidget_H
