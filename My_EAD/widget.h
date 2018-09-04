#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class Proxy;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

 public slots:
    void md5RButton(bool);
    void base64RButton(bool);
private:
    Ui::Widget *ui;
    Proxy *pro;
};

#endif // WIDGET_H
