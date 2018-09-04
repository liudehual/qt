#ifndef VIDEOREFRESHTHREAD_H
#define VIDEOREFRESHTHREAD_H

#include <QThread>
#include "metadata.h"
#include <QImage>

/// 视频刷新线程
class VideoRefreshThread : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit VideoRefreshThread(void *qRoot,void *qParent,QObject *parent = 0);

    /// 线程体函数
    virtual void run();

    /// 计算视频帧延时时长
    double compute_target_delay(double delay);
signals:
    /// 发送图片
    void sendImage(QImage *);
public slots:
private:
    /// 视频时钟
    double video_clock;

    /// 当前帧播放时间
    double fFrameTimer;
};

#endif // VIDEOREFRESHTHREAD_H
