#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <QObject>

class QStackedWidget;
class dialog_null_systemSetDialog;
class Proxy : public QObject
{
  Q_OBJECT
public:
  explicit Proxy(QObject *parent = 0);

signals:

public slots:

private:

};

#endif // CONNECTOR_H
