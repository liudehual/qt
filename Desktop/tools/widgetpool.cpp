#include "widgetpool.h"

QMap<QString,QObject*> WidgetPool::topWidget;
QMap<QObject *,QString> WidgetPool::lWidget;
QString GlobalVar::xmlHead="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
QMap<QString,QString> GlobalVar::sensorDataMap;
