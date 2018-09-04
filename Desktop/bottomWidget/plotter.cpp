#include "plotter.h"
#include "ui_plotter.h"

#include <QtGui>
#include <cmath>
#include <QDebug>

#define PI  (3.1415926)

plotter::plotter(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::plotter)
{
  ui->setupUi(this);
  this->resize(1000, 220);

  m_curZoom = 0;  // 当前缩放级别
  m_lastPoint.setX(0);
  m_lastPoint.setY(0);

  m_nMargin = 50;
  m_nHuaTuWidth = 950/10*9;   // 绘图区域宽度
  m_nHuaTuHeight = 170/10*8; // 绘图区域高度
  this->setBackgroundRole( QPalette::Dark);    // 设置曲线图使用”暗“分量显示
  this->setAutoFillBackground(true);           // 设置是否自动填充背景色

  m_nNum = 0;
  m_bIsFirst = true;
  m_CompletePix = QPixmap( this->size());
  m_CompletePix.fill( this, 0, 0);
  m_tempPix = m_CompletePix;

  // 设置绘图区域范围
  m_zoomStack.clear();    // 清空堆栈
  m_zoomStack.append( PlotSettings(0.0, 24.0, 24, 0.0, 14.0, 7));  // 重新加载绘图设置

  m_nTimerID = this->startTimer(5000);
}

plotter::~plotter()
{
  delete ui;
}

// 定时器事件
void plotter::timerEvent(QTimerEvent *event)
{
  if( event->timerId() == m_nTimerID)
    {
      int xValue = rand() % 12 + 1;
      int yValue = rand() % 12 + 1;

      m_newPoint.setX(xValue);
      m_newPoint.setY(yValue);

      m_nNum++;
      m_tempPix = m_CompletePix;
      this->update();
    }
}

// 绘图事件
void plotter::paintEvent(QPaintEvent *event)
{
  if(m_bIsFirst && (m_nNum > 1) )
    {
      QPainter painter;
      painter.begin(&m_tempPix);

      // 取得绘图区域，大小要减去旁白
      QRect rect;
      rect = QRect( m_nMargin, m_nMargin, m_nHuaTuWidth, m_nHuaTuHeight);

      // 设置缩放级别，背景色，画笔颜色，画笔风格
      PlotSettings tempSettings;
      tempSettings = m_zoomStack[m_curZoom];
      QPen quiteDark = this->palette().dark().color().light();
      QPen light = this->palette().light().color();

      //绘制X轴上的网格和坐标(这一部分就是绘制，x轴上的刻度，和x轴上的网格)
      for( int i = 0; i <= tempSettings.m_numXTicks; i++)
        {
          int nCurXPos = rect.left() + (i*(rect.width()-1)/tempSettings.m_numXTicks);
          double dXScale = tempSettings.m_minX + (i*tempSettings.spanX()/tempSettings.m_numXTicks);

          painter.setPen(quiteDark);
          if( i != 0)
            painter.drawLine( nCurXPos, rect.top(), nCurXPos, rect.bottom() );
          else
            {
              painter.drawLine( nCurXPos, rect.top()-20,nCurXPos, rect.bottom() );

              // 画X轴标志
              int x = nCurXPos;
              int y = rect.top()-20;

              int xOffset=10;
              QPoint arrowTopPoint( x, y);    // X轴标志顶点
              QPoint arrowLeftPoint( x-xOffset/tan(PI/3), y+xOffset);     // X轴标志左面的点
              QPoint arrowRightPoint( x+xOffset/tan(PI/3), y+xOffset);    // X轴标志右面的点

              static const QPoint arrowPoints[3] = { arrowTopPoint, arrowLeftPoint, arrowRightPoint };
              painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
              painter.drawPolygon(arrowPoints,3);
              painter.drawText( x-xOffset/tan(PI/3)-10, y+xOffset,tr("Y"));
            }

          if( i < 10)
            painter.drawText(   nCurXPos-3, rect.bottom()+5, 20, 20,
                                Qt::AlignLeft | Qt::AlignTop,
                                QString::number(dXScale));
          else
            painter.drawText(   nCurXPos-5, rect.bottom()+5, 20, 20,
                                Qt::AlignLeft | Qt::AlignTop,
                                QString::number(dXScale));
        }

      /////////////////////////////////////////////////////////////////

      // 绘制Y轴上的网格和坐标
      for( int i = 0; i <= tempSettings.m_numYTicks; i++)
        {
          int nCurYPos = rect.bottom() - (i*(rect.height()-1)/tempSettings.m_numYTicks);

          double dYScale = tempSettings.m_minY + (i*tempSettings.spanY()/tempSettings.m_numYTicks);

          if( i == 0)
            {
              quiteDark.setColor( Qt::white);
              painter.setPen(quiteDark);
              painter.drawLine( rect.left(), nCurYPos, rect.right()+30, nCurYPos);

              // 画Y轴标志
              int x = rect.right()+30;
              int y = nCurYPos;

              int xOffset=10;
              QPoint arrowRightPoint( x, y);
              QPoint arrowTopPoint( x-10, y-10*tan(PI/6) );
              QPoint arrowBottomPoint( x-10, y+10*tan(PI/6) );

              static const QPoint arrowPoints[3] = { arrowRightPoint, arrowTopPoint, arrowBottomPoint };
              painter.setBrush(QBrush(Qt::black,Qt::SolidPattern));
              painter.drawPolygon(arrowPoints,3);
              painter.drawText( x-10, y-10*tan(PI/6)-5, tr("X"));
            }

          if( (i == 1) || (i == tempSettings.m_numYTicks-1)) // ÕâÊÇ×îµÍÖµ
            {
              quiteDark.setColor( Qt::red);
              painter.setPen(quiteDark);
              painter.drawLine( rect.left(), nCurYPos, rect.right(), nCurYPos);
            }


          if( (i != 0) && (i != tempSettings.m_numYTicks) )
            {
              quiteDark.setColor( Qt::white);
              painter.setPen(quiteDark);
              painter.drawText(  rect.left()-20, nCurYPos-10, 20, 20,
                                 Qt::AlignLeft | Qt::AlignVCenter,
                                 QString::number(dYScale));
            }
        }



      ///////////////////////////////////////////////绘制曲线///////////////////////////////////////////////
      // 获得图形设置参数
      PlotSettings tempSettings123 = m_zoomStack[m_curZoom];

      // 获得绘图区域
      rect = QRect( m_nMargin, m_nMargin, m_nHuaTuWidth, m_nHuaTuHeight);
      painter.setClipRect( rect.adjusted( +1, +1, -1, -1));  // 设置Painter的操作区域


      double dx1 = m_lastPoint.x() - tempSettings123.m_minX;
      double dy1 = m_lastPoint.y() - tempSettings123.m_minY;

      double dXPos1 = rect.left() + (dx1*(rect.width()-1)/tempSettings123.spanX());
      double dYPos1 = rect.bottom() - (dy1*(rect.height()-1)/tempSettings123.spanY());


      double dx2 = m_newPoint.x() - tempSettings123.m_minX;
      double dy2 = m_newPoint.y() - tempSettings123.m_minY;

      double dXPos2 = rect.left() + (dx2*(rect.width()-1)/tempSettings123.spanX());
      double dYPos2 = rect.bottom() - (dy2*(rect.height()-1)/tempSettings123.spanY());

      // 设置曲线颜色
      painter.setPen( Qt::yellow );
      painter.drawLine( QPointF(dXPos1, dYPos1), QPoint(dXPos2, dYPos2));


      painter.end();

      painter.begin(this);
      painter.drawPixmap( 0, 0, m_tempPix);

      m_CompletePix = m_tempPix;
      m_lastPoint = m_newPoint;

      m_bIsFirst = false;

      painter.end();
    }

  if(!m_bIsFirst)
    {
      QPainter painter;
      painter.begin(&m_tempPix);

      // 获得图形设置参数
      PlotSettings tempSettings123 = m_zoomStack[m_curZoom];

      // 获得绘图区域
      QRect rect = QRect( m_nMargin, m_nMargin, m_nHuaTuWidth, m_nHuaTuHeight);
      painter.setClipRect( rect.adjusted( +1, +1, -1, -1));  // 设置Painter的操作区域


      double dx1 = m_lastPoint.x() - tempSettings123.m_minX;
      double dy1 = m_lastPoint.y() - tempSettings123.m_minY;

      double dXPos1 = rect.left() + (dx1*(rect.width()-1)/tempSettings123.spanX());
      double dYPos1 = rect.bottom() - (dy1*(rect.height()-1)/tempSettings123.spanY());


      double dx2 = m_newPoint.x() - tempSettings123.m_minX;
      double dy2 = m_newPoint.y() - tempSettings123.m_minY;

      double dXPos2 = rect.left() + (dx2*(rect.width()-1)/tempSettings123.spanX());
      double dYPos2 = rect.bottom() - (dy2*(rect.height()-1)/tempSettings123.spanY());

      // 设置曲线颜色
      painter.setPen( Qt::yellow );
      painter.drawLine( QPointF(dXPos1, dYPos1), QPoint(dXPos2, dYPos2));



      painter.end();



      painter.begin(this);
      painter.drawPixmap(0, 0, m_tempPix);

      m_CompletePix = m_tempPix;
      m_lastPoint = m_newPoint;

      painter.end();
    }



}



















/////////////////////////////////////////绘图设置类///////////////////////////////////////

PlotSettings::PlotSettings()
{
  m_minX = 0.0;
  m_maxX = 10.0;
  m_numXTicks = 5;

  m_minY = 0.0;
  m_maxY = 10.0;
  m_numYTicks = 5;
}

//设置坐标系的坐标轴长度，x和y分别是负荷曲线值的最大最小值，num是某一天的预测点个数
PlotSettings::PlotSettings(double dMaxY, double dMinY, int num, int nMaxX)
{
  m_minX=0.0;
  m_maxX=nMaxX;
  m_numXTicks=num;

  m_minY=dMinY;
  m_maxY=dMaxY;
  if (ceil(dMaxY-dMinY)<=10)  // 返回大于或者等于指定表达式的最小整数
    m_numYTicks=ceil(dMaxY-dMinY);
  else
    m_numYTicks=10;
}

// 分别为X轴最小点、最大点、点数。  Y轴最小点、最大点、点数
PlotSettings::PlotSettings( double minX, double maxX, int numXTicks,
                            double minY, double maxY, int numYTicks)
{
  m_minX = minX;
  m_maxX = maxX;
  m_numXTicks = numXTicks;

  m_minY = minY;
  m_maxY = maxY;
  m_numYTicks = numYTicks;
}

//使用两个标记之间的距离，乘以给定的数字来增加减少minX,maxX,minY,maxY
//该功能主要用于鼠标滚轮滚动过程
void PlotSettings::scroll(int dx,int dy)
{
  double stepX=spanX()/m_numXTicks;
  m_minX=m_minX+dx*stepX;
  m_maxX=m_maxX+dx*stepX;

  double stepY=spanY()/m_numYTicks;
  m_minY=m_minY+dy*stepY;
  m_maxY=m_maxY+dy*stepY;
}

//当橡皮筋释放之后，用来调整坐标轴上的刻度和坐标长度
void PlotSettings::adjust()
{
  //调用私有函数处理单个坐标轴
  adjustAxis(m_minX, m_maxX, m_numXTicks);
  adjustAxis(m_minY, m_maxY, m_numYTicks);
}

// 参数分别为：最小值、最大值、预测点个数
void PlotSettings::adjustAxis(double &min, double &max, int &numTicks)
{
  const int MinTicks = 4;//设置最小的刻度

  //简单确定两个坐标点之间的距离，下称步长
  double grossStep = (max - min) / MinTicks;

  //采用10的n次，2X10的n次的形式表示步长
  double step = std::pow(10.0, std::floor(std::log10(grossStep)));    // pow(): 计算x的y次幂   floor(): 取不大于x的最大整数

  //确定最合适的步长
  if (5 * step < grossStep)
    {
      step *= 5;
    }
  else if (2 * step < grossStep)
    {
      step *= 2;
    }

  //确定坐标刻度点数
  numTicks = int(std::ceil(max / step) - std::floor(min / step));
  if (numTicks < MinTicks)
    numTicks = MinTicks;

  //得到坐标轴距离
  min = std::floor(min / step) * step;
  max = std::ceil(max / step) * step;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
