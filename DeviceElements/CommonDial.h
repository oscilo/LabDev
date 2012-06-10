#ifndef COMMON_DIAL_H
#define COMMON_DIAL_H

#include <QAbstractSlider>

#include <QResizeEvent>
#include <QMouseEvent>

#include <QPainter>

class CommonDial : public QAbstractSlider
{
	Q_OBJECT

public:
	CommonDial(QString dialName, float scale = 1, int in_step = 1, QWidget *parent = 0);
	~CommonDial();

	QSize getSizeOnInit();

private:
	void paintEvent(QPaintEvent * event);
	void resizeEvent(QResizeEvent *event);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);

	QPixmap *face;
	QPixmap scaledFace;
	bool is_dragged;
	float old_angle;
	float rot_angle;
	int step;

	QSize sizeOnInit;
};

#endif