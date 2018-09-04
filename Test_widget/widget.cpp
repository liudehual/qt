#include "widget.h"
#include "ui_widget.h"
#include "mybutton.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    MyButton *button=new MyButton;
    ui->pushButton->setStyleSheet("border:3px groove gray;"
                                                           "border-radius:10px;"
                                                           "padding:2px 4px;");
    ui->pushButton_2->setStyleSheet("border:3px groove gray;"
                                    "border-radius:10px;"
                                    "padding:2px 4px;");


    QObject::connect(ui->pushButton,SIGNAL(clicked(bool)),button,SLOT(myAddClieckedSlot(bool)));
    QObject::connect(button,SIGNAL(myAddClieckedSignal(int)),ui->lcdNumber,SLOT(display(int)));
    QObject::connect(ui->pushButton_2,SIGNAL(clicked(bool)),button,SLOT(mySubClieckedSlot(bool)));
    QObject::connect(button,SIGNAL(mySubClieckedSignal(int)),ui->lcdNumber,SLOT(display(int)));

    //设置widget背景
    this->setAutoFillBackground(true);
    QPalette palette;
    //palette.setColor(QPalette::Background, QColor(100,255,200));
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/auto_demo.jpg")));

    this->setPalette(palette);
}

Widget::~Widget()
{
    delete ui;
}
