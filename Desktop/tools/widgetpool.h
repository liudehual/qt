#ifndef WIDGETPOOL_H
#define WIDGETPOOL_H
#include <QObject>
#include <QMap>
#include <QString>


class WidgetPool
{
public:
  static QMap<QString,QObject*> topWidget;//顶级widget
  static QMap<QObject *,QString> lWidget;//语言widget池
};
class GlobalVar
{
public:
  static QString    xmlHead;
  static QMap<QString,QString> sensorDataMap;
};
#endif // WIDGETPOOL_H
