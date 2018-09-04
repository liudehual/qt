#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QPainter>
#include <QToolButton>
#include <QDialog>
class PlotSettings;


namespace Ui {
class plotter;
}

typedef struct _curveParam
{
    QPointF curPoint;
    QRect curRect;

}curveParam, *pCurveParam;


class plotter : public QDialog
{
    Q_OBJECT
public:
    explicit plotter(QWidget *parent = 0);
    ~plotter();

protected:
    void paintEvent(QPaintEvent *event);        // 绘图事件
    void timerEvent(QTimerEvent *event);        // 定时器事件
private:
    QVector<PlotSettings> m_zoomStack;  // 堆栈存储放大缩小的级别

    int m_curZoom;      // 存储当前的缩放级别 0表示未缩放

    int m_nMargin;      // 曲线图和控件边框的距离
    int m_nHuaTuWidth;  // 绘图区域宽度
    int m_nHuaTuHeight; // 绘图区域高度
    int m_nTimerID;     // 定时器ID

    QPoint m_lastPoint; // 最后一个点
    QPoint m_newPoint;  // 最新产生的点

    // 缓冲区
    QPixmap m_CompletePix;      // 用来保存已经完成的绘制

    // 临时缓冲区
    QPixmap m_tempPix;          // 当要绘制时，先将内容绘制到tempPix上

    bool m_bIsFirst;            // 是否是第一次绘制
    int m_nNum;                 // 进入定时器次数

private:
    Ui::plotter *ui;
};



// 绘图设置类(缩放级别类)
class PlotSettings
{
public:
    PlotSettings();
    PlotSettings(double dMaxY, double dMinY, int num, int nMaxX);   //带三个参数的构造函数

    // 参数分别为：X轴最小坐标、X轴最大坐标、X轴点数、Y轴最小坐标、Y轴最大坐标、Y轴点数
    PlotSettings(double minX, double maxX, int numXTicks,
                 double minY, double maxY, int numYTicks);

public:
    void scroll(int dx, int dy);    // 使成卷形
    void adjust();  // 调整大小
    double spanX() const { return m_maxX - m_minX; }//X轴的范围
    double spanY() const { return m_maxY - m_minY; }//Y轴的范围


public:
    double m_minX;   //X轴最小坐标
    double m_maxX;   //X轴最大坐标
    double m_minY;	//Y轴最小坐标
    double m_maxY;	//Y轴最大坐标
    int m_numXTicks;  //X轴的坐标点数，默认都会减掉1，例如numXTicks是5，实际是6个坐标点
    int m_numYTicks;  //Y轴的坐标点数，叙述同上

private:
    static void adjustAxis(double &min, double &max, int &numTicks);    // 调整轴线
};


#endif // PLOTTER_H
