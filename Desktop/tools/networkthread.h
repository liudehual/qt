#ifndef NetworkThread_H
#define NetworkThread_H
#include <QThread>
#include <QObject>
#include <QString>
#include <QVector>
#include <QMutex>

class QTcpServer;
class QTcpSocket;
class QTimer;
class Proxy;
class NetworkThread : public QThread
{
  Q_OBJECT
public:
  NetworkThread(QObject *parent=0);
  ~NetworkThread();
  void run();

private:

signals:
  void sendMessageToProxy(QString);
public slots:
  void acceptConnection();
  void readMessage();
  void recvMessageFromProxy(QString);
  void timeToCheckMessage();
  void disconnect();
private:
  QVector<QString> messageBuffer;
  QTcpServer *m_SocketServer;
  QTcpSocket *m_Socket;
  bool connected;
  QTimer *timeout;
  QMutex mutex;
private:
  Proxy *p;
  QString m_Ip;
  int m_Port;
};

#endif // NetworkThread_H
