#include "BandPassFilter2.h"

////////////////////////////////////////////////////////////////////////////////////////////////
PaintingWidget::PaintingWidget(QWidget* parent) : QWidget(parent) {
	pixName = "./Resources/BandPassFilter2/BandPassFilter2.PNG";

	this->setMinimumSize(200, 200);
}
void PaintingWidget::SetPicture() {
	pix = QPixmap(pixName);
	ScalePixmap();
}
void PaintingWidget::ScalePixmap() {
	if(pix.isNull())
		return;

	pixToDraw = pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

	this->repaint();
	emit repaintSignal();
}
void PaintingWidget::paintEvent(QPaintEvent *e) {
	QPainter pntr(this);
	pntr.drawPixmap((this->width() - pixToDraw.width()) / 2,
					(this->height() - pixToDraw.height()) / 2,
					 pixToDraw);
}
void PaintingWidget::resizeEvent(QResizeEvent *e) {
	ScalePixmap();
}
////////////////////////////////////////////////////////////////////////////////////////////////
BandPassFilter2::BandPassFilter2(QWidget *parent, Qt::WindowFlags flags) :
	AbstractBlackBox(parent, flags)
{
	bandPassFilter2Func = new BandPassFilter2Func;
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal", bandPassFilter2Func);

	QGridLayout *lay = new QGridLayout(this);

	paintingWidget = new PaintingWidget(this);

	l0_2Slider = new QSlider(Qt::Vertical, this);
	l0_3Slider = new QSlider(Qt::Vertical, this);
	l_2Slider = new QSlider(Qt::Vertical, this);
	l_3Slider = new QSlider(Qt::Vertical, this);

	l0_2Label = new QLabel("21.28", this);
	l0_3Label = new QLabel("11.48", this);
	l_2Label = new QLabel("14.68", this);
	l_3Label = new QLabel("3.68", this);

	l0_2Slider->setRange(0, 9);
	l0_3Slider->setRange(0, 9);
	l_2Slider->setRange(0, 9);
	l_3Slider->setRange(0, 9);

	lay->addWidget(new QLabel("l0_2", this),0, 0);
	lay->addWidget(l0_2Slider,				1, 0);
	lay->addWidget(l0_2Label,				2, 0);
	lay->addWidget(new QLabel("l0_3", this),0, 1);
	lay->addWidget(l0_3Slider,				1, 1);
	lay->addWidget(l0_3Label,				2, 1);
	lay->addWidget(new QLabel("l_2", this),	0, 2);
	lay->addWidget(l_2Slider,				1, 2);
	lay->addWidget(l_2Label,				2, 2);
	lay->addWidget(new QLabel("l_3", this),	0, 3);
	lay->addWidget(l_3Slider,				1, 3);
	lay->addWidget(l_3Label,				2, 3);
	lay->addWidget(paintingWidget,			0, 4, 3, 1);

	connect(l0_2Slider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect(l0_3Slider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect(l_2Slider,	SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect(l_3Slider,	SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect((PaintingWidget*)paintingWidget, SIGNAL(repaintSignal()), this, SIGNAL(repaintSignal()));

	((PaintingWidget*)paintingWidget)->SetPicture();
}
QString BandPassFilter2::getDeviceName() {
	return RUS("Band Pass Filter 2");		//DS
}
QString BandPassFilter2::getDeviceIDName() {
	return "BandPassFilter2";
}
void BandPassFilter2::SlidersUpdate() {
	int sValue	= L0_2_START +	(l0_2Slider->value() *	L0_2_STEP);
	int skValue = L0_3_START +	(l0_3Slider->value() *	L0_3_STEP);
	int lValue	= L_2_START +	(l_2Slider->value() *	L_2_STEP);
	int xValue	= L_3_START +	(l_3Slider->value() *	L_2_STEP);

	QString sStr = QString::number(sValue / 100.);
	QString lStr = QString::number(lValue / 100.);
	QString skStr = QString::number(skValue / 100.);
	QString xStr = QString::number(xValue / 100.);

	l0_2Label->setText(sStr);
	l0_3Label->setText(lStr);
	l_2Label->setText(skStr);
	l_3Label->setText(xStr);

	bandPassFilter2Func->set_l0_2(sValue);
	bandPassFilter2Func->set_l0_3(lValue);
	bandPassFilter2Func->set_l_2(skValue);
	bandPassFilter2Func->set_l_3(xValue);

	this->repaint();
	emit repaintSignal();
}