#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>

class XmlParser : public QObject
{
  Q_OBJECT
public:
  explicit XmlParser(QObject *parent = 0);

signals:

public slots:
};

#endif // XMLPARSER_H
