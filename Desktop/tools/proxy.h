#ifndef PROXY_H
#define PROXY_H
#include <QObject>
#include <QVector>
#include <QString>

class QTimer;
class Proxy : public QObject
{
  Q_OBJECT
public:
  static Proxy *instance();

signals:
  void sendMessageToThread(QString);
  void sendMessageToSystemSetDialog(QString);
  void sendMessageToYunTaiDialog(QString);
  void sendMessageToPlaybackDialog(QString);
  void sendMessageToSensorDialog(QString);
  void sendMessageToControllerDialog(QString);
  void sendMessageToPiclayoutDialog(QString);
  void sendMessageToShutdownDialog(QString);

  void sendMessageToBottomDialog(QString);
  void sendMessageToYunTaiSubDialog(QString);
public slots:
  void recvMessageFromThread(QString);
  void recvMessageFromSecondDialog(QString);

  //test
  void recvMessageFromTimer();
private:
  void sendMessageToDialog(QString ,QString );
private:
  explicit Proxy(QObject *parent = 0);
  virtual ~Proxy();
private:
  static Proxy *pro;
  QString oldString;
  QTimer *timer;
};

#endif // CONNECTOR_H
