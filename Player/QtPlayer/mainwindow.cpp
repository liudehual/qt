#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "qtplayer.h"
#include "readthread.h"
#include "videothread.h"
#include "videorefreshthread.h"
#include "audiothread.h"
#include <QMessageBox>

MainWindow::MainWindow(void*qRoot,void *qParent,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    MetaData(qRoot,qParent),
    fSingleValue(0.0),
    timer(NULL),
    fSliderPos(0)
{
    ui->setupUi(this);

    QObject::connect(ui->actionOpen_file,SIGNAL(triggered()),this,SLOT(getFileName()));
    QObject::connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(exitProgram()));

    QtPlayer *player=(QtPlayer *)this->getRoot();
    VideoRefreshThread  *vrThread=player->getReadThread()->getVideoThread()->getVideoRefreshThread();

    QObject::connect(vrThread,SIGNAL(sendImage(QImage *)),this,SLOT(recvImage(QImage *)));

    /// 设置音量条帧条
    ui->horizontalSlider_sound_level->setRange(0,128);
    QObject::connect(ui->horizontalSlider_sound_level,SIGNAL(valueChanged(int)),this,SLOT(changeSound(int)));
    ui->horizontalSlider_sound_level->setValue(32);


    /// 设置播放进度条
    ui->horizontalSlider_play_pos->setRange(0,QSLIDER_LEN);
    ui->horizontalSlider_play_pos->setValue(0);
    ui->horizontalSlider_play_pos->setPageStep(30);
    ui->horizontalSlider_play_pos->setSingleStep(30);
    ui->horizontalSlider_play_pos->setTickInterval(QSlider::TicksAbove);

    QObject::connect(ui->horizontalSlider_play_pos,SIGNAL(sliderReleased()),this,SLOT(setRelease()));
    QObject::connect(ui->horizontalSlider_play_pos,SIGNAL(sliderPressed()),this,SLOT(setPress()));
    timer=new QTimer();
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(timeoutSlot()));
    timer->start(3*1000);

    QObject::connect(ui->actionAbout_Qt,SIGNAL(triggered()),this,SLOT(qtInfo()));
    QObject::connect(ui->actionAbout_QtPlayer,SIGNAL(triggered()),this,SLOT(aboutQtPlayer()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::getVideoLabelWidth()
{
    return ui->label_video->width();
}

int MainWindow::getVideoLabelHeight()
{
    return ui->label_video->height();
}
QLabel * MainWindow::getVideoLabel()
{
    return ui->label_video;
}

void MainWindow::getFileName()
{
#if 0
    QString s=QFileDialog::getOpenFileUrl(this,"open file dialog","/",
                                                    "Movie files *.aac *.wav *.mp3 *.rmvb *.mkv *.avi *.flv *.rm *.mp4;;All files (*)");
#endif
#if 1
    QString s = QFileDialog::getOpenFileName(
                this, "open file dialog",
                "/",//初始目录
                "Movie files *.aac *.wav *.mp3 *.rmvb *.mkv *.avi *.flv *.rm *.mp4;;All files (*)");
#endif
    ///直播测试
#if 0
    s.clear();
    s="rtmp://live.hkstv.hk.lxdns.com/live/hks";

#endif
    if (!s.isEmpty()){
        QtPlayer *player=(QtPlayer *)this->getRoot();
        player->getReadThread()->clean();

        //// 初始化数据读取线程
        player->getReadThread()->initReadThread(s);
    }else{
        av_log(NULL,AV_LOG_INFO,"String is empty\n");
    }
}
void MainWindow::exitProgram()
{
    qApp->exit(0);
}
void MainWindow::recvImage(QImage *image)
{
    /// 显示
    ui->label_video->setPixmap(QPixmap::fromImage(image->scaled(ui->label_video->size(), Qt::KeepAspectRatio)));
    delete image;
}
void MainWindow::changeSound(int value)
{
    /// 音量调整
    QtPlayer *player=(QtPlayer *)this->getRoot();
    player->getReadThread()->getAudioThread()->setVolume(value);
}

void MainWindow::setSingleValue(double value)
{
    fSingleValue=value; /*  */
}

double MainWindow::getSingleValue()
{
    return fSingleValue;
}
void MainWindow::setRelease()
{
    int value=ui->horizontalSlider_play_pos->value();
    setValue(value);
}
void MainWindow::setValue(int value)
{
    QtPlayer *player=(QtPlayer *)this->getRoot();
    ReadThread *rThread=player->getReadThread();
    av_log(NULL,AV_LOG_INFO,"value %d\n",value);

    /// 获取视频宽度
    double width=player->getScreenWidth()*1.0;

    /// 计算百分比
    double frac=value/width;
    double ts=frac*rThread->getFormatContext()->duration;
    if (rThread->getFormatContext()->start_time != AV_NOPTS_VALUE){
        ts += rThread->getFormatContext()->start_time;

        av_log(NULL,AV_LOG_INFO,"-------> ts %0.8f file duration %0.8f \n",ts,
               rThread->getFormatContext()->duration*1.0);
        /// 调用读取线程定位到多媒体文件某一位置
        player->getReadThread()->seekPos(ts);
    }
}

void MainWindow::updateSlider(int value)
{
    av_log(NULL,AV_LOG_INFO,"fSliderPos %d \n",value);
    ui->horizontalSlider_play_pos->setValue(value);
}
void MainWindow::setSliderPos(int pos)
{
    fSliderPos=pos;
}
void MainWindow::timeoutSlot()
{
    updateSlider(fSliderPos);
}
void MainWindow::qtInfo()
{
    qApp->aboutQt();
}

void MainWindow::aboutQtPlayer()
{
    QMessageBox::about(this,"QtPlayer",
                        "QtPlayer \n"
                        "author:GCT \n"
                        "time:2017-5-29");
}
