#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <QObject>
class QStackedWidget;
class Connector : public QObject
{
  Q_OBJECT
public:
  explicit Connector(QStackedWidget *widget,QObject *parent = 0);

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
};

#endif // CONNECTOR_H
