#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "metadata.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget,public MetaData
{
    Q_OBJECT

public:
    explicit Widget(void *pRoot,void *pParent,QWidget *parent = 0);
    ~Widget();
public slots:
    void open();
signals:
    void startPlay(QString);
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
