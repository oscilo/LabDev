#ifndef SUBRANGESELECTOR_H
#define SUBRANGESELECTOR_H

#include <QPainter>
#include <QAbstractSlider>
#include <QResizeEvent>
#include <QMouseEvent>

class SubRangeSelector : public QAbstractSlider {
	Q_OBJECT

public:
	SubRangeSelector(float scale = 1, QWidget *parent = 0);
	~SubRangeSelector();

	QSize getSizeOnInit();

private:
	void paintEvent(QPaintEvent * event);
	void resizeEvent(QResizeEvent *event);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);

	QPixmap *face;
	bool is_dragged;
	QPixmap scaledFace;
	
	QSize sizeOnInit;
};

#endif