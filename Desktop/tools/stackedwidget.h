#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H
#include <QStackedWidget>

class StackedWidget : public QStackedWidget
{
  Q_OBJECT
public:
  StackedWidget(QWidget *parent=0);
public slots:
  void currentIndex0();
  void currentIndex1();
  void currentIndex2();
  void currentIndex3();
  void currentIndex4();
  void currentIndex5();
  void currentIndex6();

};

#endif // STACKEDWIDGET_H
