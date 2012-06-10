#include "UpperLimitDial.h"

#define M_PI_2     1.57079632679489661923
#define M_PI_14    0.22439947525641380274
#define M_PI_7     0.44879895051282760549

UpperLimitDial::UpperLimitDial(float scale, QWidget *parent)
	: QAbstractSlider(parent),
	is_dragged(false)
{
	this->setRange(0, 30);

	face = new QPixmap("./Resources/UpperLimit.png");
	(*face) = face->scaledToHeight((int)(face->height() * scale));

	sizeOnInit = face->size();
}
UpperLimitDial::~UpperLimitDial()
{
	delete face;
}
QSize UpperLimitDial::getSizeOnInit()
{
	return sizeOnInit;
}
void UpperLimitDial::resizeEvent(QResizeEvent *event)
{
	scaledFace = face->scaled(event->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}
void UpperLimitDial::paintEvent(QPaintEvent * event)
{
	QPainter pntr(this);
	int w = this->width();
	int h = this->height();

	float degrees;

	switch(this->value())
	{
		case 0:
			degrees = 0;
			break;
		case 5:
			degrees = 25;
			break;
		case 10:
			degrees = 55;
			break;
		case 15:
			degrees = 87;
			break;
		case 20:
			degrees = 120;
			break;
		case 25:
			degrees = 150;
			break;
		case 30:
			degrees = 180;
			break;
	}

	QTransform mtrx;
	mtrx.translate(w/2, h/2);
	mtrx.rotate(degrees);

	QPixmap pix = scaledFace.transformed(mtrx, Qt::SmoothTransformation);

	pntr.drawPixmap(0, 0, pix.copy(pix.width()/2-w/2, pix.height()/2-h/2, w, h));
}
void UpperLimitDial::mouseMoveEvent(QMouseEvent * event)
{
	if(is_dragged)
	{
		int x = event->x() - this->width()/2;
		int y = this->height()/2 - event->y();

		if( (x <= 0) && (y <= 0) )
		{
			this->setValue(0);
			return;
		}

		if( (x >= 0) && (y <= 0) )
		{
			this->setValue(30);
			return;
		}

		float angle = M_PI_2 - atan((float)x / (float)y);

		if( angle <= M_PI_14)
			this->setValue(30);
		else if( (angle > M_PI_14) && (angle <= 3*M_PI_14) )
			this->setValue(25);
		else if( (angle > 3*M_PI_14) && (angle <= 5*M_PI_14) )
			this->setValue(20);
		else if( (angle > 5*M_PI_14) && (angle <= 7*M_PI_14) )
			this->setValue(15);
		else if( (angle > 7*M_PI_14) && (angle <= 9*M_PI_14) )
			this->setValue(10);
		else if( (angle > 9*M_PI_14) && (angle <= 11*M_PI_14) )
			this->setValue(5);
		else if( (angle > 11*M_PI_14) && (angle <= 13*M_PI_14) )
			this->setValue(0);
	}
}
void UpperLimitDial::mousePressEvent(QMouseEvent * event)
{
	is_dragged = true;
}
void UpperLimitDial::mouseReleaseEvent(QMouseEvent * event)
{
	is_dragged = false;
}