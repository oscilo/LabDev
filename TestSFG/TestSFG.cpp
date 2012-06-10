#include "TestSFG.h"

TestSFG::TestSFG(QWidget *parent, Qt::WFlags flags)
  :	AbstractSFGenerator(parent, flags),
  scale_k(1), dstate(DS_F_ST), back(0)
{
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal", new SFGFunc);
	fmin = 8150;
	fmax = 12050;

	back = new QPixmap("./Resources/SFG.png");
	lamp = new QPixmap("./Resources/lamp.png");
	d[0] = new QPixmap("./Resources/0.png");
	d[1] = new QPixmap("./Resources/1.png");
	d[2] = new QPixmap("./Resources/2.png");
	d[3] = new QPixmap("./Resources/3.png");
	d[4] = new QPixmap("./Resources/4.png");
	d[5] = new QPixmap("./Resources/5.png");
	d[6] = new QPixmap("./Resources/6.png");
	d[7] = new QPixmap("./Resources/7.png");
	d[8] = new QPixmap("./Resources/8.png");
	d[9] = new QPixmap("./Resources/9.png");
	d_ = new QPixmap("./Resources/_.png");

	QSize temp = back->size();
	temp.scale(200, 200, Qt::KeepAspectRatioByExpanding);
	this->setMinimumSize(temp);

	scale_k /= back->width();
	(*back) = back->scaledToWidth(1310, Qt::SmoothTransformation);
	scale_k *= back->width();

	curSize = back->size();

	(*lamp) = lamp->scaledToHeight(15);

	(*d[0]) = d[0]->scaledToHeight(73);
	(*d[1]) = d[1]->scaledToHeight(d[0]->height());
	(*d[2]) = d[2]->scaledToHeight(d[0]->height());
	(*d[3]) = d[3]->scaledToHeight(d[0]->height());
	(*d[4]) = d[4]->scaledToHeight(d[0]->height());
	(*d[5]) = d[5]->scaledToHeight(d[0]->height());
	(*d[6]) = d[6]->scaledToHeight(d[0]->height());
	(*d[7]) = d[7]->scaledToHeight(d[0]->height());
	(*d[8]) = d[8]->scaledToHeight(d[0]->height());
	(*d[9]) = d[9]->scaledToHeight(d[0]->height());
	(*d_) = d_->scaledToHeight(10);

	doffsets << QPoint(430, 78) << QPoint(370, 78) << QPoint(310, 78) << QPoint(250, 78) << QPoint(190, 78);
	
	fStSl = new CommonDial("./Resources/SFGFreqDial.png", scale_k, 1, this);
	fStSl->setGeometry(85, 237, fStSl->width(), fStSl->height());
	fStSl->setRange((int)fmin, (int)fmax);

	fMark1Sl = new CommonDial("./Resources/SFGCommonDial.png", scale_k, 1, this);
	fMark1Sl->setGeometry(195, 237, fMark1Sl->width(), fMark1Sl->height());
	fMark1Sl->setRange((int)fmin, (int)fmax);

	fMark2Sl = new CommonDial("./Resources/SFGCommonDial.png", scale_k, 1, this);
	fMark2Sl->setGeometry(302, 237, fMark2Sl->width(), fMark2Sl->height());
	fMark2Sl->setRange((int)fmin, (int)fmax);

	fEnSl = new CommonDial("./Resources/SFGFreqDial.png", scale_k, 1, this);
	fEnSl->setGeometry(407, 237, fEnSl->width(), fEnSl->height());
	fEnSl->setRange((int)fmin, (int)fmax);

	connect(fStSl, SIGNAL(valueChanged(int)), this, SLOT(set_new_fst(int)));
	connect(fEnSl, SIGNAL(valueChanged(int)), this, SLOT(set_new_fen(int)));
	connect(fMark1Sl, SIGNAL(valueChanged(int)), this, SLOT(set_new_mark1(int)));
	connect(fMark2Sl, SIGNAL(valueChanged(int)), this, SLOT(set_new_mark2(int)));

	this->setMaximumSize(back->size());
}
TestSFG::~TestSFG()
{
	delete back;
	delete lamp;
	delete d[0];
	delete d[1];
	delete d[2];
	delete d[3];
	delete d[4];
	delete d[5];
	delete d[6];
	delete d[7];
	delete d[8];
	delete d[9];
	delete d_;
}
void TestSFG::initProperties()
{	
	set_new_fen(9067);
	set_new_fst(8943);
	set_new_mark1(8950);
	set_new_mark2(9050);
}
QString TestSFG::getDeviceName()
{
	return QString::fromLocal8Bit("√енератор качаюшейс€ частоты 61 (8.15 - 12.05 √√ц)");
}
void TestSFG::resizeEvent(QResizeEvent *event)
{
	QSize curSize = back->size();
	curSize.scale(event->size(), Qt::KeepAspectRatio);

	QPoint offset(0, 0);
	float scale = ( (float)(curSize.width()) ) / ( (float)(back->width()) );

	if(curSize.width() != event->size().width())
		offset.setX( (event->size().width()  - curSize.width()) / 2 );
	else
		offset.setY( (event->size().height()  - curSize.height()) / 2 );

	fStSl->setGeometry		(offset.x() + (85  * scale), offset.y() + (237 * scale), fStSl->getSizeOnInit().width() * scale,	fStSl->getSizeOnInit().height() * scale);
	fMark1Sl->setGeometry	(offset.x() + (195 * scale), offset.y() + (237 * scale), fMark1Sl->getSizeOnInit().width() * scale, fMark1Sl->getSizeOnInit().height() * scale);
	fMark2Sl->setGeometry	(offset.x() + (302 * scale), offset.y() + (237 * scale), fMark2Sl->getSizeOnInit().width() * scale, fMark2Sl->getSizeOnInit().height() * scale);
	fEnSl->setGeometry		(offset.x() + (407 * scale), offset.y() + (237 * scale), fEnSl->getSizeOnInit().width() * scale,	fEnSl->getSizeOnInit().height() * scale);
}
void TestSFG::paintEvent(QPaintEvent *event)
{
	//QPixmap pix(back->width(), back->height());
	//pix = back->copy(0, 0, back->width(), back->height());
	QPixmap pix = back->scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

	QPoint offset(0, 0);
	QSize curSize = pix.size();
	float scale = ( (float)(pix.width()) ) / ( (float)(back->width()) );

	if(curSize.width() != this->width())
		offset.setX( (this->width()  - curSize.width()) / 2 );
	else
		offset.setY( (this->height()  - curSize.height()) / 2 );

	QPainter pntr(this);
	QPainter pntr_pix(&pix);

	int lamp_x, value;

	switch(dstate)
	{
		case DS_F_ST:
			lamp_x = 628 * scale;
			value = fStSl->value();
			break;
		case DS_MARK1:
			lamp_x = 672 * scale;
			value = fMark1Sl->value();
			break;
		case DS_MARK2:
			lamp_x = 713 * scale;
			value = fMark2Sl->value();
			break;
		case DS_F_EN:
			lamp_x = 752 * scale;
			value = fEnSl->value();
			break;
	}

	int digits[5];

	digits[0] = value % 10;
	digits[1] = (value / 10) % 10;
	digits[2] = (value / 100) % 10;
	digits[3] = (value / 1000) % 10;
	digits[4] = (value / 10000) % 10;

	pntr_pix.drawPixmap(lamp_x, 88 * scale, lamp->scaledToWidth(lamp->width() * scale, Qt::SmoothTransformation));

	for(int i = 0; i < 5; i++)
		if(!( (i == 4) && (digits[i] == 0) ))
		{
			QPixmap *digit = d[digits[i]];
			pntr_pix.drawPixmap(doffsets[i] * scale, digit->scaledToWidth(digit->width() * scale, Qt::SmoothTransformation));
		}

	pntr_pix.drawPixmap(320 * scale, 130 * scale, d_->scaledToWidth(d_->width() * scale, Qt::SmoothTransformation));
	
	pntr.drawPixmap(offset, pix);
}
void TestSFG::mousePressEvent(QMouseEvent *event)
{
	QSize curSize = back->size();
	curSize.scale(this->size(), Qt::KeepAspectRatio);

	QPoint offset(0, 0);
	float scale = ( (float)(curSize.width()) ) / ( (float)(back->width()) );

	if(curSize.width() != this->width())
		offset.setX( (this->width()  - curSize.width()) / 2 );
	else
		offset.setY( (this->height()  - curSize.height()) / 2 );

	int x = event->x() - offset.x();
	int y = event->y() - offset.y();

	if( (y > 151 * scale) && (y < 171 * scale) && (x > 615 * scale) )
	{
		if(x < 659 * scale)
			dstate = DS_F_ST;
		else if(x < 702 * scale)
			dstate = DS_MARK1;
		else if(x < 741 * scale)
			dstate = DS_MARK2;
		else if(x < 786 * scale)
			dstate = DS_F_EN;

		this->repaint();
	}
}
void TestSFG::set_new_fst(int new_val)
{
	fst = new_val;

	if(fStSl->value() != new_val)
		fStSl->setValue(new_val);

	emit fst_changed(new_val);

	this->repaint();
}
void TestSFG::set_new_fen(int new_val)
{
	fen = new_val;

	if(fEnSl->value() != new_val)
		fEnSl->setValue(new_val);

	emit fen_changed(new_val);

	this->repaint();
}
void TestSFG::set_new_mark1(int new_val)
{
	mark1 = new_val;

	if(fMark1Sl->value() != new_val)
		fMark1Sl->setValue(new_val);

	emit mark1_changed(new_val);

	this->repaint();
}
void TestSFG::set_new_mark2(int new_val)
{
	mark2 = new_val;

	if(fMark2Sl->value() != new_val)
		fMark2Sl->setValue(new_val);

	emit mark2_changed(new_val);

	this->repaint();
}

Q_EXPORT_PLUGIN2(TestSFG, TestSFG);