#ifndef HUAJIETU_H
#define HUAJIETU_H

#include <QtWidgets/QMainWindow>
#include "ui_huajietu.h"

#include <QWidget>
class QMenu;

class HuaJieTu : public QMainWindow
{
	Q_OBJECT

public:
	HuaJieTu(QWidget *parent = 0);
	~HuaJieTu();

public:
	//״̬
	enum shotState
	{
		initShot,
		beginShot,
		finishShot,
		endShot
	};



private:
	Ui::HuaJieTuClass ui;


private:
	QPoint  m_beginPoint, m_endPoint;   // ѡ��������ʼ����յ�
	int     m_nStatus;                  // ״̬

	QColor  m_shadowColor;
	QPixmap m_loadPixmap, m_shotPixmap; // ����Ľ���ͼƬ

	QMenu * m_contentMenu;              // ѡ�������Ҽ��˵�
	QAction *savePixmapAction;          // ����ͼƬ����


protected:
	//��д���෽��
	void paintEvent(QPaintEvent *event);

	void mousePressEvent(QMouseEvent *event);         // �����������¼�
	void mouseReleaseEvent(QMouseEvent *event);       // �������ͷ��¼�
	void mouseMoveEvent(QMouseEvent *event);          // ����ƶ��¼�
	void mouseDoubleClickEvent(QMouseEvent *event);   // ������˫���¼�

	void contextMenuEvent(QContextMenuEvent *event);  // �����Ĳ˵��¼�


	public slots:
		void savePixmap();                  // ����ͼƬ

public:
	void updateMouseShape(const QPoint &point);     // ���������״
	QRect getSelectedRect();                        // �õ�ѡ�еľ�������

};

#endif // HUAJIETU_H
