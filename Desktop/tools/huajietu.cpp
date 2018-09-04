#include "huajietu.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QMath.h>
#include <QAction>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>

HuaJieTu::HuaJieTu(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_nStatus = initShot;

    // 初始化起始点和终点
    m_beginPoint.setX(0);
    m_beginPoint.setY(0);

    m_endPoint.setX(0);
    m_endPoint.setY(0);


    m_shadowColor= QColor(0,0,0,100); //阴影颜色设置
    m_loadPixmap = QPixmap::grabWindow(QApplication::desktop()->winId()); //抓取当前屏幕的图片

    this->setCursor( Qt::ArrowCursor);
}

HuaJieTu::~HuaJieTu()
{

}

// 绘图事件
void HuaJieTu::paintEvent(QPaintEvent *event)
{
	QPainter painter;
	painter.begin(this);    // 开始绘制

	painter.setPen(QPen(Qt::blue,2,Qt::SolidLine,Qt::FlatCap));//设置画笔
	painter.drawPixmap( 0, 0,m_loadPixmap); //将背景图片画到窗体
	painter.fillRect( QRect( 0, 0, this->width(), this->height()) ,m_shadowColor); //画影罩效果


	switch( m_nStatus)
	{
	case beginShot:
	case finishShot:
		{
			QRect rect;
			rect = getSelectedRect();

			painter.drawRect(rect);

			m_shotPixmap = m_loadPixmap.copy(rect);
			if( rect.width() > 0 && rect.height() > 0)
				painter.drawPixmap( rect.topLeft(), m_shotPixmap);

		}break;
	case initShot:
		break;
	default:
		break;
	}


	if( m_nStatus == finishShot)
		m_nStatus = initShot;
	

	painter.end();          // 结束绘制
}

// 鼠标左键按下事件
void HuaJieTu::mousePressEvent(QMouseEvent *event)
{
	if( (event->button() == Qt::LeftButton) && (m_nStatus == initShot) )
	{
		m_beginPoint = event->pos();  // 获得开始点
		m_nStatus = beginShot;        // 更新状态
	}
}

// 鼠标左键释放事件
void HuaJieTu::mouseReleaseEvent(QMouseEvent *event)
{
	if( (event->button() == Qt::LeftButton) && (m_nStatus == beginShot) )
	{
		m_endPoint = event->pos();    // 获得结束点
		m_nStatus = finishShot;       // 更新状态
		this->update();
	}
}

// 鼠标移动事件
void HuaJieTu::mouseMoveEvent(QMouseEvent *event)
{
	// 左键拖动的判断一般放在mouseMoveEvent函数中，但是你不能向上例一样来判断，因为该函数的event参数总是返回Qt::NoButton。你可以这样做：
	// 注意：在mouseMoveEvent()事件中，不要判断event->button()是否等于Qt::LeftButton了。 因为总是返回Qt::NoButton。
	if( m_nStatus == beginShot )
	{
		m_endPoint = event->pos();
		this->update();
	}

	if( m_nStatus == endShot)
	{
		QRect rect;
		rect = getSelectedRect();
	}

	updateMouseShape( this->pos() );
	this->setMouseTracking(true);

}

// 鼠标左键双击事件
void HuaJieTu::mouseDoubleClickEvent(QMouseEvent *event)
{
	QRect rect;
	rect = getSelectedRect();
	
	if( (m_nStatus == finishShot) && (rect.contains(event->pos())) )
	{			
		m_nStatus = initShot;
		this->update();
	}
}

// 上下文菜单事件
void HuaJieTu::contextMenuEvent(QContextMenuEvent *event)
{
	//return;
	m_contentMenu = new QMenu(this);

	savePixmapAction = new QAction(tr("Save"), this);
	connect( savePixmapAction, SIGNAL(triggered()), this, SLOT(savePixmap()));

	m_contentMenu->addAction(savePixmapAction);
	m_contentMenu->exec( this->cursor().pos() );
}

//-----------------------------------------------------------------------------------------------------

// 保存图片
void HuaJieTu::savePixmap()
{
	QString fileName;
	fileName = QFileDialog::getSaveFileName(this, tr("保存"), QDir::currentPath(),
		tr("Images (*.jpg *.png"));
	if(fileName.isNull())
		return;

	m_shotPixmap.save(fileName);

	m_nStatus = initShot;
	this->update();
}

// 更新鼠标形状
void HuaJieTu::updateMouseShape(const QPoint &point)
{
	switch(m_nStatus)
	{
	case initShot:
	case beginShot:
		this->setCursor(Qt::CrossCursor);
		break;
	case finishShot:
		{
			QRect rect;
			rect = getSelectedRect();

			if(rect.contains(point))
				this->setCursor(Qt::OpenHandCursor);
			break;
		}
	default:
		break;
	}
}

// 得到选中的矩形区域
QRect HuaJieTu::getSelectedRect()
{
	int x = m_beginPoint.x() < m_endPoint.x() ? m_beginPoint.x() : m_endPoint.x();
	int y = m_beginPoint.y() < m_endPoint.y() ? m_beginPoint.y() : m_endPoint.y();
	int nWidth = qAbs( m_beginPoint.x() - m_endPoint.x());
	int nHeight = qAbs( m_beginPoint.y() - m_endPoint.y());

	QRect rect;
	rect.setX(x);
	rect.setY(y);
	rect.setWidth(nWidth);
	rect.setHeight(nHeight);

	return rect;
}