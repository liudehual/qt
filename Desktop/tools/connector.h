#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>

class QStackedWidget;
class dialog_null_systemSetDialog;
class Connector : public QObject
{
  Q_OBJECT
public:
  explicit Connector(QStackedWidget *widget ,dialog_null_systemSetDialog *dialog,QObject *parent = 0);

signals:

public slots:
  void currentIndex0();
  void currentIndex1();
  void currentIndex2();
  void currentIndex3();
  void currentIndex4();
  void currentIndex5();
private:
  QStackedWidget *stackWidget;
  dialog_null_systemSetDialog *dia;
};
#endif // CONNECTOR_H
