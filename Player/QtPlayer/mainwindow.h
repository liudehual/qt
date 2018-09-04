#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "metadata.h"
#include <QImage>
#include <QLabel>
#include <QTimer>

/// 播放进度条长度
#define QSLIDER_LEN 1000

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow,public MetaData
{
    Q_OBJECT

public:
    explicit MainWindow(void*qRoot,void *qParent,QWidget *parent = 0);
    ~MainWindow();

    /// 获取视频界面宽度
    int getVideoLabelWidth();

    /// 获取视频界面高度
    int getVideoLabelHeight();

    /// 获取视频显示Label
    QLabel * getVideoLabel();

    /// 设置拖动一格 Slider 的值
    void setSingleValue(double value);

    /// 获取拖动一格 Slider 的值
    double getSingleValue();

    /// 设置Slider位置
    void setSliderPos(int pos);

private:
    /// 定位到文件某一位置
    void setValue(int value);

    /// 更新Slider位置
    void updateSlider(int pos);

public slots:
    /// 获取文件名
    void getFileName();

    /// 推出程序
    void exitProgram();

    /// 接受一帧图像
    void recvImage(QImage *);

    /// 调整音频音量
    void changeSound(int);

    /// 检测Slider释放事件，并设置相应的值到读取线程，定位到多媒体文件的某一位置
    void setRelease();

    /// Slider更新时间间隔
    void timeoutSlot();

    /// 获取 qt 相关信息
    void qtInfo();

    /// 获取关于QtPlayer的相关信息
    void aboutQtPlayer();
private:
    Ui::MainWindow *ui;

    ///
    /// QSlider 一格代表几秒播放时间（QSlider默认有1000格）
    /// 用于根据播放时间更新QSlider进度
    ///
    double fSingleValue;

    /// Slider当前位置
    int fSliderPos;

    /// 用于更新Slider 值的定时器
    QTimer *timer;
};

#endif // MAINWINDOW_H
