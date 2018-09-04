#ifndef PROXY_H
#define PROXY_H

#include <QObject.h>
#include<QTextEdit>
#include "md5.h"

class Proxy:public QObject
{
    Q_OBJECT
public:
    Proxy();
    virtual ~Proxy();
    void setTextEdit1(QTextEdit *edit){tEdit1=edit;}
    void setTextEdit2(QTextEdit *edit){tEdit2=edit;}

signals:

public slots:
    void encoderMD5Slot(bool);
    void encoderBase64Slot(bool);
    void decoderBase64Slot(bool);
private:
    QTextEdit *tEdit1;
    QTextEdit *tEdit2;

    unsigned enType;//加密类型

};

#endif // PROXY_H
