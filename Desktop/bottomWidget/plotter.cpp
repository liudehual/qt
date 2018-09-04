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

  m_curZoom = 0;  // ��ǰ���ż���
  m_lastPoint.setX(0);
  m_lastPoint.setY(0);

  m_nMargin = 50;
  m_nHuaTuWidth = 950/10*9;   // ��ͼ������
  m_nHuaTuHeight = 170/10*8; // ��ͼ����߶�
  this->setBackgroundRole( QPalette::Dark);    // ��������ͼʹ�á�����������ʾ
  this->setAutoFillBackground(true);           // �����Ƿ��Զ���䱳��ɫ

  m_nNum = 0;
  m_bIsFirst = true;
  m_CompletePix = QPixmap( this->size());
  m_CompletePix.fill( this, 0, 0);
  m_tempPix = m_CompletePix;

  // ���û�ͼ����Χ
  m_zoomStack.clear();    // ��ն�ջ
  m_zoomStack.append( PlotSettings(0.0, 24.0, 24, 0.0, 14.0, 7));  // ���¼��ػ�ͼ����

  m_nTimerID = this->startTimer(5000);
}

plotter::~plotter()
{
  delete ui;
}

// ��ʱ���¼�
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

// ��ͼ�¼�
void plotter::paintEvent(QPaintEvent *event)
{
  if(m_bIsFirst && (m_nNum > 1) )
    {
      QPainter painter;
      painter.begin(&m_tempPix);

      // ȡ�û�ͼ���򣬴�СҪ��ȥ�԰�
      QRect rect;
      rect = QRect( m_nMargin, m_nMargin, m_nHuaTuWidth, m_nHuaTuHeight);

      // �������ż��𣬱���ɫ��������ɫ�����ʷ��
      PlotSettings tempSettings;
      tempSettings = m_zoomStack[m_curZoom];
      QPen quiteDark = this->palette().dark().color().light();
      QPen light = this->palette().light().color();

      //����X���ϵ����������(��һ���־��ǻ��ƣ�x���ϵĿ̶ȣ���x���ϵ�����)
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

              // ��X���־
              int x = nCurXPos;
              int y = rect.top()-20;

              int xOffset=10;
              QPoint arrowTopPoint( x, y);    // X���־����
              QPoint arrowLeftPoint( x-xOffset/tan(PI/3), y+xOffset);     // X���־����ĵ�
              QPoint arrowRightPoint( x+xOffset/tan(PI/3), y+xOffset);    // X���־����ĵ�

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

      // ����Y���ϵ����������
      for( int i = 0; i <= tempSettings.m_numYTicks; i++)
        {
          int nCurYPos = rect.bottom() - (i*(rect.height()-1)/tempSettings.m_numYTicks);

          double dYScale = tempSettings.m_minY + (i*tempSettings.spanY()/tempSettings.m_numYTicks);

          if( i == 0)
            {
              quiteDark.setColor( Qt::white);
              painter.setPen(quiteDark);
              painter.drawLine( rect.left(), nCurYPos, rect.right()+30, nCurYPos);

              // ��Y���־
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

          if( (i == 1) || (i == tempSettings.m_numYTicks-1)) // �0�9�0�9�0�8�0�5���0�6�0�8�0�1�0�0�0�8
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



      ///////////////////////////////////////////////��������///////////////////////////////////////////////
      // ���ͼ�����ò���
      PlotSettings tempSettings123 = m_zoomStack[m_curZoom];

      // ��û�ͼ����
      rect = QRect( m_nMargin, m_nMargin, m_nHuaTuWidth, m_nHuaTuHeight);
      painter.setClipRect( rect.adjusted( +1, +1, -1, -1));  // ����Painter�Ĳ�������


      double dx1 = m_lastPoint.x() - tempSettings123.m_minX;
      double dy1 = m_lastPoint.y() - tempSettings123.m_minY;

      double dXPos1 = rect.left() + (dx1*(rect.width()-1)/tempSettings123.spanX());
      double dYPos1 = rect.bottom() - (dy1*(rect.height()-1)/tempSettings123.spanY());


      double dx2 = m_newPoint.x() - tempSettings123.m_minX;
      double dy2 = m_newPoint.y() - tempSettings123.m_minY;

      double dXPos2 = rect.left() + (dx2*(rect.width()-1)/tempSettings123.spanX());
      double dYPos2 = rect.bottom() - (dy2*(rect.height()-1)/tempSettings123.spanY());

      // ����������ɫ
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

      // ���ͼ�����ò���
      PlotSettings tempSettings123 = m_zoomStack[m_curZoom];

      // ��û�ͼ����
      QRect rect = QRect( m_nMargin, m_nMargin, m_nHuaTuWidth, m_nHuaTuHeight);
      painter.setClipRect( rect.adjusted( +1, +1, -1, -1));  // ����Painter�Ĳ�������


      double dx1 = m_lastPoint.x() - tempSettings123.m_minX;
      double dy1 = m_lastPoint.y() - tempSettings123.m_minY;

      double dXPos1 = rect.left() + (dx1*(rect.width()-1)/tempSettings123.spanX());
      double dYPos1 = rect.bottom() - (dy1*(rect.height()-1)/tempSettings123.spanY());


      double dx2 = m_newPoint.x() - tempSettings123.m_minX;
      double dy2 = m_newPoint.y() - tempSettings123.m_minY;

      double dXPos2 = rect.left() + (dx2*(rect.width()-1)/tempSettings123.spanX());
      double dYPos2 = rect.bottom() - (dy2*(rect.height()-1)/tempSettings123.spanY());

      // ����������ɫ
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



















/////////////////////////////////////////��ͼ������///////////////////////////////////////

PlotSettings::PlotSettings()
{
  m_minX = 0.0;
  m_maxX = 10.0;
  m_numXTicks = 5;

  m_minY = 0.0;
  m_maxY = 10.0;
  m_numYTicks = 5;
}

//��������ϵ�������᳤�ȣ�x��y�ֱ��Ǹ�������ֵ�������Сֵ��num��ĳһ���Ԥ������
PlotSettings::PlotSettings(double dMaxY, double dMinY, int num, int nMaxX)
{
  m_minX=0.0;
  m_maxX=nMaxX;
  m_numXTicks=num;

  m_minY=dMinY;
  m_maxY=dMaxY;
  if (ceil(dMaxY-dMinY)<=10)  // ���ش��ڻ��ߵ���ָ�����ʽ����С����
    m_numYTicks=ceil(dMaxY-dMinY);
  else
    m_numYTicks=10;
}

// �ֱ�ΪX����С�㡢���㡢������  Y����С�㡢���㡢����
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

//ʹ���������֮��ľ��룬���Ը��������������Ӽ���minX,maxX,minY,maxY
//�ù�����Ҫ���������ֹ�������
void PlotSettings::scroll(int dx,int dy)
{
  double stepX=spanX()/m_numXTicks;
  m_minX=m_minX+dx*stepX;
  m_maxX=m_maxX+dx*stepX;

  double stepY=spanY()/m_numYTicks;
  m_minY=m_minY+dy*stepY;
  m_maxY=m_maxY+dy*stepY;
}

//����Ƥ���ͷ�֮�����������������ϵĿ̶Ⱥ����곤��
void PlotSettings::adjust()
{
  //����˽�к���������������
  adjustAxis(m_minX, m_maxX, m_numXTicks);
  adjustAxis(m_minY, m_maxY, m_numYTicks);
}

// �����ֱ�Ϊ����Сֵ�����ֵ��Ԥ������
void PlotSettings::adjustAxis(double &min, double &max, int &numTicks)
{
  const int MinTicks = 4;//������С�Ŀ̶�

  //��ȷ�����������֮��ľ��룬�³Ʋ���
  double grossStep = (max - min) / MinTicks;

  //����10��n�Σ�2X10��n�ε���ʽ��ʾ����
  double step = std::pow(10.0, std::floor(std::log10(grossStep)));    // pow(): ����x��y����   floor(): ȡ������x���������

  //ȷ������ʵĲ���
  if (5 * step < grossStep)
    {
      step *= 5;
    }
  else if (2 * step < grossStep)
    {
      step *= 2;
    }

  //ȷ������̶ȵ���
  numTicks = int(std::ceil(max / step) - std::floor(min / step));
  if (numTicks < MinTicks)
    numTicks = MinTicks;

  //�õ����������
  min = std::floor(min / step) * step;
  max = std::ceil(max / step) * step;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
