#include "widget.h"
#include "ui_widget.h"
#include "player.h"
#include "readthread.h"
#include <QString>
#include <QFileDialog>
#include <QDebug>
Widget::Widget(void *pRoot,void *pParent,QWidget *parent) :
    QWidget(parent),
    MetaData(pRoot,pParent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QObject::connect(ui->open,SIGNAL(clicked()),this,SLOT(open()));
}

Widget::~Widget()
{
    delete ui;
}
void Widget::open()
{
    QString support_format(".mp3");
    QString s = QFileDialog::getOpenFileName(
                this, "open file dialog",
                "/",//初始目录
                "Movie files (*.mp3);;All files (*)");
    if (!s.isEmpty()
            && s.endsWith(support_format,Qt::CaseInsensitive)){
        Player *p=(Player *)this->getParent();
        qDebug()<<s;
        p->getReadThread()->init(s);
        p->getReadThread()->startThread();
    }
}
