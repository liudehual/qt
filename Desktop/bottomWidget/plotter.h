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
    void paintEvent(QPaintEvent *event);        // ��ͼ�¼�
    void timerEvent(QTimerEvent *event);        // ��ʱ���¼�
private:
    QVector<PlotSettings> m_zoomStack;  // ��ջ�洢�Ŵ���С�ļ���

    int m_curZoom;      // �洢��ǰ�����ż��� 0��ʾδ����

    int m_nMargin;      // ����ͼ�Ϳؼ��߿�ľ���
    int m_nHuaTuWidth;  // ��ͼ������
    int m_nHuaTuHeight; // ��ͼ����߶�
    int m_nTimerID;     // ��ʱ��ID

    QPoint m_lastPoint; // ���һ����
    QPoint m_newPoint;  // ���²����ĵ�

    // ������
    QPixmap m_CompletePix;      // ���������Ѿ���ɵĻ���

    // ��ʱ������
    QPixmap m_tempPix;          // ��Ҫ����ʱ���Ƚ����ݻ��Ƶ�tempPix��

    bool m_bIsFirst;            // �Ƿ��ǵ�һ�λ���
    int m_nNum;                 // ���붨ʱ������

private:
    Ui::plotter *ui;
};



// ��ͼ������(���ż�����)
class PlotSettings
{
public:
    PlotSettings();
    PlotSettings(double dMaxY, double dMinY, int num, int nMaxX);   //�����������Ĺ��캯��

    // �����ֱ�Ϊ��X����С���ꡢX��������ꡢX�������Y����С���ꡢY��������ꡢY�����
    PlotSettings(double minX, double maxX, int numXTicks,
                 double minY, double maxY, int numYTicks);

public:
    void scroll(int dx, int dy);    // ʹ�ɾ���
    void adjust();  // ������С
    double spanX() const { return m_maxX - m_minX; }//X��ķ�Χ
    double spanY() const { return m_maxY - m_minY; }//Y��ķ�Χ


public:
    double m_minX;   //X����С����
    double m_maxX;   //X���������
    double m_minY;	//Y����С����
    double m_maxY;	//Y���������
    int m_numXTicks;  //X������������Ĭ�϶������1������numXTicks��5��ʵ����6�������
    int m_numYTicks;  //Y����������������ͬ��

private:
    static void adjustAxis(double &min, double &max, int &numTicks);    // ��������
};


#endif // PLOTTER_H
