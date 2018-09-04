#include<QTextCodec>
#include "widget.h"
#include "ui_widget.h"
#include "proxy.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));

    ui->decoder->setEnabled(false);
    ui->encoder->setEnabled(false);

    pro =new Proxy;
    pro->setTextEdit1(ui->textEdit);
    pro->setTextEdit2(ui->textEdit_2);
    QObject::connect(ui->md5,SIGNAL(clicked(bool)),this,SLOT(md5RButton(bool)));
    QObject::connect(ui->base64,SIGNAL(clicked(bool)),this,SLOT(base64RButton(bool)));
}

Widget::~Widget()
{
    if(pro!=NULL) {delete pro;pro=NULL;}
    delete ui;

}
void Widget::md5RButton(bool)
{
    ui->decoder->setEnabled(false);
    ui->encoder->setEnabled(true);

    QObject::disconnect(ui->encoder,SIGNAL(clicked(bool)),pro,SLOT(encoderBase64Slot(bool)));
    QObject::connect(ui->encoder,SIGNAL(clicked(bool)),pro,SLOT(encoderMD5Slot(bool)));
}

void Widget::base64RButton(bool)
{
    ui->decoder->setEnabled(true);
    ui->encoder->setEnabled(true);

    QObject::disconnect(ui->encoder,SIGNAL(clicked(bool)),pro,SLOT(encoderMD5Slot(bool)));
    QObject::connect(ui->encoder,SIGNAL(clicked(bool)),pro,SLOT(encoderBase64Slot(bool)));
    QObject::connect(ui->decoder,SIGNAL(clicked(bool)),pro,SLOT(decoderBase64Slot(bool)));
}
