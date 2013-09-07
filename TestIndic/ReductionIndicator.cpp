#include "ReductionIndicator.h"

ReductionIndicator::ReductionIndicator(QWidget *parent, Qt::WindowFlags flags)
  :	AbstractIndicator((AbstractDrawArea *)new ReductionIndicatorDraw(parent, flags), parent, flags),
  scale_k(1)
{
	neededInputs << INDICATOR_INPUT_SIGNAL;

	back = new QPixmap("./Resources/Indicator.png");
	shk[0] = new QPixmap("./Resources/shk0.png");
	shk[5] = new QPixmap("./Resources/shk5.png");
	shk[10] = new QPixmap("./Resources/shk10.png");
	shk[15] = new QPixmap("./Resources/shk15.png");
	shk[20] = new QPixmap("./Resources/shk20.png");
	shk[25] = new QPixmap("./Resources/shk25.png");
	shk[30] = new QPixmap("./Resources/shk30.png");
	shkRisk = new QPixmap("./Resources/shkRisk.png");
	
	QSize temp = back->size();
	temp.scale(300, 300, Qt::KeepAspectRatioByExpanding);
	this->setMinimumSize(temp);

	scale_k /= back->width();
	(*back) = back->scaledToWidth(1310);
	scale_k *= back->width();
	
	(*shk[0]) = shk[0]->scaledToHeight(81);
	(*shk[5]) = shk[5]->scaledToHeight(shk[0]->height());
	(*shk[10]) = shk[10]->scaledToHeight(shk[0]->height());
	(*shk[15]) = shk[15]->scaledToHeight(shk[0]->height());
	(*shk[20]) = shk[20]->scaledToHeight(shk[0]->height());
	(*shk[25]) = shk[25]->scaledToHeight(shk[0]->height());
	(*shk[30]) = shk[30]->scaledToHeight(shk[0]->height());
	(*shkRisk) = shkRisk->scaledToHeight(shk[0]->height());

	draw->setParent(this);
	draw->setGeometry(135, 53, draw->width(), draw->height());
	drawInitSize = draw->size();

	markHSl = new CommonDial("./Resources/IndicCommonDial.png", scale_k, 1, this);
	markHSl->setGeometry(650, 213, markHSl->width(), markHSl->height());
	markHSl->setRange(0, 10);
	
	mLineSl = new CommonDial("./Resources/IndicCommonDial.png", scale_k, 5, this);
	mLineSl->setGeometry(1185, 115, mLineSl->width(), mLineSl->height());
	mLineSl->setRange(-1000, 200);

	limitSl = new UpperLimitDial(scale_k, this);
	limitSl->setGeometry(1060, 165, limitSl->width(), limitSl->height());

	connect(markHSl, SIGNAL(valueChanged(int)), this, SLOT(set_new_mark_h(int)));
	connect(mLineSl, SIGNAL(valueChanged(int)), this, SLOT(set_new_m_line(int)));
	connect(limitSl, SIGNAL(valueChanged(int)), this, SLOT(set_new_lim(int)));

	connect(this, SIGNAL(mark_h_changed(int)), draw, SLOT(set_new_mark_h(int)));
	connect(this, SIGNAL(m_line_changed(int)), draw, SLOT(set_new_m_line(int)));
	connect(this, SIGNAL(lim_changed(int)), draw, SLOT(set_new_lim(int)));

	connect(this, SIGNAL(repaintSignal()), draw, SLOT(repaint()));
	
	this->setMaximumSize(back->size());
}
ReductionIndicator::~ReductionIndicator()
{
	delete back;
	delete shk[0];
	delete shk[5];
	delete shk[10];
	delete shk[15];
	delete shk[20];
	delete shk[25];
	delete shk[30];
	delete shkRisk;
}
void ReductionIndicator::initProperties()
{
	set_new_lim(0);
	set_new_m_line(0);
	set_new_mark_h(5);
}
QString ReductionIndicator::getDeviceName()
{
	return QString::fromLocal8Bit("VSWR / Attenuation Scope");		//DS Индикатор КСВН и ослабления Я2Р-67
}
QString ReductionIndicator::getDeviceIDName() {
	return "ReductionIndicator";
}
void ReductionIndicator::paintEvent(QPaintEvent *event)
{
	QSize neededSize = back->size();
	neededSize.scale(this->size(), Qt::KeepAspectRatio);
	if(resizedBack.size() != neededSize)
		resizedBack = back->scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	
	QPixmap pix = resizedBack;

	QPoint offset(0, 0);
	QSize curSize = pix.size();
	float scale = ( (float)(pix.width()) ) / ( (float)(back->width()) );

	if(curSize.width() != this->width())
		offset.setX( (this->width()  - curSize.width()) / 2 );
	else
		offset.setY( (this->height()  - curSize.height()) / 2 );

	QPixmap *curShkala = shk[limitSl->value()];
	QPixmap shkala = curShkala->scaledToWidth(curShkala->width() * scale, Qt::SmoothTransformation);

	QPainter pntr(this);
	QPainter pntr_pix(&pix);
	QPainter pntr_shk(&shkala);

	int risk_st = 11 - shkRisk->width()/2;
	int risk_en = 473 - shkRisk->width()/2;

	float len = risk_en - risk_st;
	
	float risk_val = mLineSl->value();
	float risk_x = ((float)risk_st) + pow(BASE, (risk_val/100.)/BASE)*len / (pow(BASE, 2./BASE) + 0.01);

	pntr_shk.drawPixmap(risk_x * scale, 0, shkRisk->scaledToWidth(shkRisk->width() * scale, Qt::SmoothTransformation));

	pntr_pix.drawPixmap(599 * scale, 49 * scale, shkala);
	
	pntr.drawPixmap(offset.x(), offset.y(), pix);
}
void ReductionIndicator::resizeEvent(QResizeEvent *event)
{
	QSize curSize = back->size();
	curSize.scale(event->size(), Qt::KeepAspectRatio);

	QPoint offset(0, 0);
	float scale = ( (float)(curSize.width()) ) / ( (float)(back->width()) );

	if(curSize.width() != event->size().width())
		offset.setX( (event->size().width()  - curSize.width()) / 2 );
	else
		offset.setY( (event->size().height()  - curSize.height()) / 2 );

	markHSl->setGeometry(offset.x() + (650  * scale), offset.y() + (213 * scale), markHSl->getSizeOnInit().width() * scale,	markHSl->getSizeOnInit().height() * scale);
	mLineSl->setGeometry(offset.x() + (1185 * scale), offset.y() + (115 * scale), mLineSl->getSizeOnInit().width() * scale,	mLineSl->getSizeOnInit().height() * scale);
	limitSl->setGeometry(offset.x() + (1060 * scale), offset.y() + (165 * scale), limitSl->getSizeOnInit().width() * scale,	limitSl->getSizeOnInit().height() * scale);
	draw->setGeometry(offset.x() + (135 * scale), offset.y() + (53 * scale), drawInitSize.width() * scale, drawInitSize.height() * scale);
}
void ReductionIndicator::set_new_mark_h(int new_val)
{
	if(markHSl->value() != new_val)
		markHSl->setValue(new_val);

	emit mark_h_changed(new_val);
}
void ReductionIndicator::set_new_m_line(int new_val)
{
	if(mLineSl->value() != new_val)
		mLineSl->setValue(new_val);

	emit m_line_changed(new_val);

	this->repaint();
}
void ReductionIndicator::set_new_lim(int new_val)
{
	if(limitSl->value() != new_val)
		limitSl->setValue(new_val);

	emit lim_changed(new_val);

	this->repaint();
}