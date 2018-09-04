#include "dialog.h"
#include "ui_dialog.h"


#include <stdio.h>
#include <stdlib.h>
#include <QPixmap>
#include "decodethread.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),dt(NULL),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    dt=new DecodeThread;
    QObject::connect(ui->pushButton_start,SIGNAL(clicked()),dt,SLOT(startVideo()));
    QObject::connect(ui->pushButton_pause,SIGNAL(clicked()),dt,SLOT(pauseVideo()));
    QObject::connect(ui->pushButton_stop,SIGNAL(clicked()),dt,SLOT(stopVideo()));
    QObject::connect(dt,SIGNAL(displaVideo(QImage)),this,SLOT(displayVideo(QImage)));
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::displayVideo(QImage img)
{
    ui->label_video1->setPixmap(QPixmap::fromImage( img.scaled(size(), Qt::KeepAspectRatio)));
}
