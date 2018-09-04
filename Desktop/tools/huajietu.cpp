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

    // ��ʼ����ʼ����յ�
    m_beginPoint.setX(0);
    m_beginPoint.setY(0);

    m_endPoint.setX(0);
    m_endPoint.setY(0);


    m_shadowColor= QColor(0,0,0,100); //��Ӱ��ɫ����
    m_loadPixmap = QPixmap::grabWindow(QApplication::desktop()->winId()); //ץȡ��ǰ��Ļ��ͼƬ

    this->setCursor( Qt::ArrowCursor);
}

HuaJieTu::~HuaJieTu()
{

}

// ��ͼ�¼�
void HuaJieTu::paintEvent(QPaintEvent *event)
{
	QPainter painter;
	painter.begin(this);    // ��ʼ����

	painter.setPen(QPen(Qt::blue,2,Qt::SolidLine,Qt::FlatCap));//���û���
	painter.drawPixmap( 0, 0,m_loadPixmap); //������ͼƬ��������
	painter.fillRect( QRect( 0, 0, this->width(), this->height()) ,m_shadowColor); //��Ӱ��Ч��


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
	

	painter.end();          // ��������
}

// �����������¼�
void HuaJieTu::mousePressEvent(QMouseEvent *event)
{
	if( (event->button() == Qt::LeftButton) && (m_nStatus == initShot) )
	{
		m_beginPoint = event->pos();  // ��ÿ�ʼ��
		m_nStatus = beginShot;        // ����״̬
	}
}

// �������ͷ��¼�
void HuaJieTu::mouseReleaseEvent(QMouseEvent *event)
{
	if( (event->button() == Qt::LeftButton) && (m_nStatus == beginShot) )
	{
		m_endPoint = event->pos();    // ��ý�����
		m_nStatus = finishShot;       // ����״̬
		this->update();
	}
}

// ����ƶ��¼�
void HuaJieTu::mouseMoveEvent(QMouseEvent *event)
{
	// ����϶����ж�һ�����mouseMoveEvent�����У������㲻��������һ�����жϣ���Ϊ�ú�����event�������Ƿ���Qt::NoButton���������������
	// ע�⣺��mouseMoveEvent()�¼��У���Ҫ�ж�event->button()�Ƿ����Qt::LeftButton�ˡ� ��Ϊ���Ƿ���Qt::NoButton��
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

// ������˫���¼�
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

// �����Ĳ˵��¼�
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

// ����ͼƬ
void HuaJieTu::savePixmap()
{
	QString fileName;
	fileName = QFileDialog::getSaveFileName(this, tr("����"), QDir::currentPath(),
		tr("Images (*.jpg *.png"));
	if(fileName.isNull())
		return;

	m_shotPixmap.save(fileName);

	m_nStatus = initShot;
	this->update();
}

// ���������״
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

// �õ�ѡ�еľ�������
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