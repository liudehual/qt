#include "calculator.h"
#include "ui_calculator.h"

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);     //也是去掉标题栏的语句
}

Calculator::~Calculator()
{
    delete ui;
}
