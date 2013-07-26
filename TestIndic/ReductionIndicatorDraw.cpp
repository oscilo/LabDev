#include "ReductionIndicatorDraw.h"

#define DI			300

ReductionIndicatorDraw::ReductionIndicatorDraw(QWidget *parent, Qt::WindowFlags flags) : AbstractDrawArea(parent, flags),
	LEN(1390), TL(126), scale_k(1)
{
	area_pic = new QPixmap("./Resources/DrawArea.png");
	point_pic = new QPixmap("./Resources/point.png");

	scale_k /= area_pic->width();
	(*area_pic) = area_pic->scaledToHeight(330);
	scale_k *= area_pic->width();

	(*point_pic) = point_pic->scaledToHeight(point_pic->height() * scale_k);

	LEN *= scale_k;
	TL *= scale_k;

	this->resize(area_pic->size());
	
	sizeOnInit = area_pic->size();
}
ReductionIndicatorDraw::~ReductionIndicatorDraw()
{
	delete area_pic;
	delete point_pic;
}
void ReductionIndicatorDraw::paintEvent(QPaintEvent * event)
{
	QPixmap pix = areaScaled;

	QSize curSize = pix.size();
	float scale = ( (float)(pix.width()) ) / ( (float)(area_pic->width()) );

	QPainter pntr(this);
	QPainter pntr_pix(&pix);

	pntr.setPen(Qt::green);

	float m_line_h = TL + LEN - pow(BASE, (m_line/100.)/BASE)*LEN / (pow(BASE, 2./BASE) + 0.01);

	if(inputSignals->contains(INDICATOR_INPUT_SIGNAL))
	{
		AbstractFunc *func_to_visualise = (*inputSignals)[INDICATOR_INPUT_SIGNAL];

		float abc_len = fen - fst;
		if( (abc_len > 0) && (lim >= 0) )
		{
			float dx = LEN / (float)DI;

			float x, y;

			for(int i = 0; i <= DI; i++)
			{
				x = i * dx;
					
				float f_ret = (*func_to_visualise)(fst + (abc_len*i/DI), fst, fen) + lim;// * pow(10., lim/10.);
				//if(f_ret <= 0) f_ret = 0.00001;

				//y = LEN - f_ret * LEN / (pow(BASE, 2./BASE) + 0.01);
				y = LEN - pow(BASE, (f_ret/*/100.*/)/BASE)*LEN / (pow(BASE, 2./BASE) + 0.01);

				if(y >= 0)
					pntr_pix.drawPixmap(QPointF((TL + x) * scale, (TL + y) * scale), pointScaled);
		
				pntr_pix.drawPixmap(QPointF((TL + x) * scale, m_line_h * scale), pointScaled);
			}

			for(float i = (m_line_h - mark_h*5); i < m_line_h; i += dx)
				if( (mark1 >= fst) && (mark1 <= fen) && (i >= TL))
					pntr_pix.drawPixmap(QPointF((TL + (mark1 - fst)*LEN/abc_len) * scale, i * scale), pointScaled);
			
			for(float i = (m_line_h - mark_h*5); i < m_line_h; i += dx)
				if( (mark2 >= fst) && (mark2 <= fen) && (i >= TL))
					pntr_pix.drawPixmap(QPointF((TL + (mark2 - fst)*LEN/abc_len) * scale, i * scale), pointScaled);
			
			pntr_pix.drawPixmap(QPointF((TL + x) * scale, m_line_h * scale), pointScaled);
		}
	}
	
	pntr.drawPixmap(0, 0, pix);
}
void ReductionIndicatorDraw::resizeEvent(QResizeEvent *event)
{
	QSize areaSize = area_pic->size();
	areaSize.scale(this->size(), Qt::KeepAspectRatio);

	float scale = ( (float)(areaSize.width()) ) / ( (float)(area_pic->size().width()) );

	pointScaled = point_pic->scaledToWidth(point_pic->width() * scale, Qt::SmoothTransformation);
	areaScaled = area_pic->scaled(event->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}
void ReductionIndicatorDraw::set_new_mark_h(int new_val)
{
	mark_h = new_val;
	this->repaint();
}
void ReductionIndicatorDraw::set_new_m_line(int new_val)
{
	m_line = new_val;
	this->repaint();
}
void ReductionIndicatorDraw::set_new_lim(int new_val)
{
	lim = new_val;
	this->repaint();
}