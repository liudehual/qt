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
	//状态
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
	QPoint  m_beginPoint, m_endPoint;   // 选中区域起始点和终点
	int     m_nStatus;                  // 状态

	QColor  m_shadowColor;
	QPixmap m_loadPixmap, m_shotPixmap; // 保存的截屏图片

	QMenu * m_contentMenu;              // 选中区域右键菜单
	QAction *savePixmapAction;          // 保存图片操作


protected:
	//重写基类方法
	void paintEvent(QPaintEvent *event);

	void mousePressEvent(QMouseEvent *event);         // 鼠标左键按下事件
	void mouseReleaseEvent(QMouseEvent *event);       // 鼠标左键释放事件
	void mouseMoveEvent(QMouseEvent *event);          // 鼠标移动事件
	void mouseDoubleClickEvent(QMouseEvent *event);   // 鼠标左键双击事件

	void contextMenuEvent(QContextMenuEvent *event);  // 上下文菜单事件


	public slots:
		void savePixmap();                  // 保存图片

public:
	void updateMouseShape(const QPoint &point);     // 更新鼠标形状
	QRect getSelectedRect();                        // 得到选中的矩形区域

};

#endif // HUAJIETU_H
