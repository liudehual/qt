#ifndef MYBUTTON_H
#define MYBUTTON_H
#include <QObject>

class MyButton:public QObject
{
    Q_OBJECT
public:
    MyButton();

signals:
    void myAddClieckedSignal(int);
     void mySubClieckedSignal(int);
public slots:
    void myAddClieckedSlot(bool);
    void mySubClieckedSlot(bool);
public:
  int a;
};

#endif // MYBUTTON_H
