#include "CommonDial.h"

#include <math.h>

#define M_PI_2     1.57079632679489661923
#define M_PI       3.14159265358979323846

CommonDial::CommonDial(QString dialName, float scale, int in_step, QWidget *parent)
	: QAbstractSlider(parent),
	is_dragged(false), old_angle(0), step(in_step), rot_angle(0)
{
	face = new QPixmap(dialName);
	(*face) = face->scaledToHeight((int)(face->height() * scale));

	sizeOnInit = face->size();
}
CommonDial::~CommonDial()
{
	delete face;
}
void CommonDial::paintEvent(QPaintEvent * event)
{
	QPainter pntr(this);
	int w = this->width();
	int h = this->height();

	float degrees = 0;

	QTransform mtrx;
	mtrx.translate(w/2, h/2);
	mtrx.rotateRadians(-rot_angle);

	QPixmap pix = scaledFace.transformed(mtrx, Qt::SmoothTransformation);

	pntr.drawPixmap(0, 0, pix.copy(pix.width()/2-w/2, pix.height()/2-h/2, w, h));
}
void CommonDial::resizeEvent(QResizeEvent *event)
{
	scaledFace = face->scaled(event->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}
void CommonDial::mouseMoveEvent(QMouseEvent * event)
{
	if(is_dragged)
	{
		float x = event->x() - this->width()/2;
		float y = this->height()/2 - event->y();

		float angle = acos(x / sqrt(x*x + y*y));

		if(y < 0)
			angle = -angle;

		float diff = angle - old_angle;

		if(diff == 0)
			return;

		int incr = (step < 0) ? (1 + rand() % (-step)) : step;

		if( (diff > 0) && (this->value() != this->minimum()) )
			this->setValue(this->value() - incr);
		else if( (diff < 0) && (this->value() != this->maximum()) )
			this->setValue(this->value() + incr);
		else
			return;

		rot_angle += diff;
		old_angle = angle;
	}
}
void CommonDial::mousePressEvent(QMouseEvent * event)
{
	is_dragged = true;

	float x = event->x() - this->width()/2;
	float y = this->height()/2 - event->y();

	old_angle = acos(x / sqrt(x*x + y*y));

	if(y < 0)
		old_angle = -old_angle;
}
void CommonDial::mouseReleaseEvent(QMouseEvent * event)
{
	is_dragged = false;
}
QSize CommonDial::getSizeOnInit()
{
	return sizeOnInit;
}