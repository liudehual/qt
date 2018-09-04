#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>


/*
    在此设定一个默认原则:
        1.父对象设定字对象的信号与槽的连接
        2.除了最顶级的父对象外其他所有对象必需有父对象(形成对象树，方便管理)
        3.父对象必需知道子对象的类型，子对象也必需知道父对象的类型
        4.2/3原则适用于其他程序设计
        5.所有对象都拥有根对象指针或引用(意味着所有对象都拥有自顶向下查询数据或者自下而上提供服务的能力)
*/

#define DEFAULT_SOUND_VALUE 32
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new VideoPlayer(this);

    connect(player,SIGNAL(readOneFrame(QImage)),this,SLOT(slotgetFrame(QImage)));
    connect(player,SIGNAL(updateTime(long)),this,SLOT(slotUpdateTime(long)));
    connect(player,SIGNAL(stateChanged(VideoPlayer::State)),this,SLOT(slotStateChanged(VideoPlayer::State)));


    ui->horizontalSlider->setRange(0,100);
    /*
        该类处理所有空间发过来的信号
    */
    connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),this,SLOT(slotValueChanged(int)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slotOpenBtnClicked()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(slotPlayBtnClicked()));
    connect(ui->sound,SIGNAL(sliderMoved(int)),this,SLOT(soundValueChanged(int)));

    connect(this,SIGNAL(soundChanged(int)),player,SLOT(soundSlotChanged(int)));

    /*
       set default sound value


    */
    ui->sound->setRange(0,128);
    ui->sound->setValue(DEFAULT_SOUND_VALUE);
    soundValueChanged(DEFAULT_SOUND_VALUE);


  //  setWindowOpacity(0.5);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::soundValueChanged(int value)
{
    emit soundChanged(value);
}
void MainWindow::slotgetFrame(QImage image)
{
    QPixmap pixmap = QPixmap::fromImage(image.scaled(ui->label->size(), Qt::KeepAspectRatio) );
    ui->label->setPixmap(pixmap);
}

void MainWindow::slotValueChanged(int value)
{
    qint64 v = (value/100.0)*player->totalTime();
    player->seek(v);
}

void MainWindow::slotUpdateTime(long time)
{
    double v = time*1000.0/player->totalTime()*100.0;
    ui->horizontalSlider->setValue(v);
}

void MainWindow::slotOpenBtnClicked()
{
    QString support_format(".flv");
    QString s = QFileDialog::getOpenFileName(
                this, "open file dialog",
                "/",//初始目录
                "Movie files (*.flv);;All files (*)");
    if (!s.isEmpty()
            && s.endsWith(support_format,Qt::CaseInsensitive)){

        ui->label->clear();
        player->setSource(s);
        qDebug()<<s<<player->totalTime()<<player->state()<<player->type();
        player->play();
    }
}

void MainWindow::slotPlayBtnClicked()
{
    if (player->state() == VideoPlayer::PlayingState){
        player->pause();
    }
    else if (player->state() == VideoPlayer::PausedState){
        player->play();
    }
}

void MainWindow::slotStateChanged(VideoPlayer::State state)
{
    if (state == VideoPlayer::PlayingState){
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_2->setText("Pause");
    }
    if (state == VideoPlayer::PausedState){
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_2->setText("Play");
    }
    if (state == VideoPlayer::StoppedState){
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_2->setText("Start");
    }
}
