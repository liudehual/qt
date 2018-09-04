#include "qtffmpeg.h"
#include "ui_qtffmpeg.h"

QtFFmpeg::QtFFmpeg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtFFmpeg),
    fCore(NULL)
{
    ui->setupUi(this);
    fCore=new Core();
    fCore->initFFmpegLib();
    fCore->start();
}

QtFFmpeg::~QtFFmpeg()
{
    delete ui;
}
