#ifndef UPPER_LIMIT_DIAL_H
#define UPPER_LIMIT_DIAL_H

#include "globals.h"

class UpperLimitDial : public QAbstractSlider
{
	Q_OBJECT

public:
	UpperLimitDial(float scale = 1, QWidget *parent = 0);
	~UpperLimitDial();

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