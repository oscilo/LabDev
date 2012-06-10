#include "SubRangeSelector.h"

#include <math.h>

#define M_PI_2		1.57079632679489661923
#define M_PI_14		0.22439947525641380274
#define M_PI_7		0.44879895051282760549
#define M_PI_6		0.52359877559829887308

SubRangeSelector::SubRangeSelector(float scale, QWidget *parent)
	: QAbstractSlider(parent),
	is_dragged(false)
{
	this->setRange(1, 2);

	face = new QPixmap("./Resources/HighFrequencySignalsGenerator/sub_range_selector.png");
	(*face) = face->scaledToHeight((int)(face->height() * scale));

	sizeOnInit = face->size();
}
SubRangeSelector::~SubRangeSelector()
{
	delete face;
}
QSize SubRangeSelector::getSizeOnInit()
{
	return sizeOnInit;
}
void SubRangeSelector::resizeEvent(QResizeEvent *event)
{
	scaledFace = face->scaled(event->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}
void SubRangeSelector::paintEvent(QPaintEvent * event)
{
	QPainter pntr(this);
	int w = this->width();
	int h = this->height();

	float degrees;

	switch(this->value())
	{
		case 1:
			degrees = 0;
			break;
		case 2:
			degrees = 50;
			break;
	}

	QTransform mtrx;
	mtrx.translate(w/2, h/2);
	mtrx.rotate(degrees);

	QPixmap pix = scaledFace.transformed(mtrx, Qt::SmoothTransformation);

	pntr.drawPixmap(0, 0, pix.copy(pix.width()/2-w/2, pix.height()/2-h/2, w, h));
}
void SubRangeSelector::mouseMoveEvent(QMouseEvent * event)
{
	if(is_dragged)
	{
		int x = event->x() - this->width()/2;
		int y = this->height()/2 - event->y();

		if( x <= 0 )
		{
			this->setValue(1);
			return;
		}

		if( y <= 0 )
		{
			this->setValue(2);
			return;
		}

		float angle = atan((float)x / (float)y);

		if(angle < M_PI_6)
			this->setValue(1);
		else
			this->setValue(2);
		/*
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
		//*/
	}
}
void SubRangeSelector::mousePressEvent(QMouseEvent * event)
{
	is_dragged = true;
}
void SubRangeSelector::mouseReleaseEvent(QMouseEvent * event)
{
	is_dragged = false;
}