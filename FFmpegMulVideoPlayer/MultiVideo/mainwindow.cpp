#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fVideoPlayerArray.clear();

    Module::initOS();

    VideoPlayer *thePlayer1=VideoPlayer::createNewVideoPlayer();
    thePlayer1->setFileName("/home/gct/Videos/my.264");
    thePlayer1->initVideoPlayer(ui->label_video1);
    thePlayer1->startPlayer();
    fVideoPlayerArray.push_back(thePlayer1);

    VideoPlayer *thePlayer2=VideoPlayer::createNewVideoPlayer();
    thePlayer2->setFileName("/home/gct/Videos/my.264");
    thePlayer2->initVideoPlayer(ui->label_video2);
    thePlayer2->startPlayer();
    fVideoPlayerArray.push_back(thePlayer2);

    VideoPlayer *thePlayer3=VideoPlayer::createNewVideoPlayer();
    thePlayer3->setFileName("/home/gct/Videos/my.264");
    thePlayer3->initVideoPlayer(ui->label_video3);
    thePlayer3->startPlayer();
    fVideoPlayerArray.push_back(thePlayer3);

    VideoPlayer *thePlayer4=VideoPlayer::createNewVideoPlayer();
    thePlayer4->setFileName("/home/gct/Videos/my.264");
    thePlayer4->initVideoPlayer(ui->label_video4);
    thePlayer4->startPlayer();
    fVideoPlayerArray.push_back(thePlayer4);
}

MainWindow::~MainWindow()
{
    for(int i=0;i<fVideoPlayerArray.size();i++){
        VideoPlayer *thePlayer=fVideoPlayerArray.at(i);
        delete thePlayer;
    }
    fVideoPlayerArray.clear();
    delete ui;
}

