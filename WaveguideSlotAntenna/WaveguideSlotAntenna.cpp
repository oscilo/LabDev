#include "WaveguideSlotAntenna.h"

////////////////////////////////////////////////////////////////////////////////////////////////
PaintingWidget::PaintingWidget(QWidget* parent) : QWidget(parent) {
	pixName = "./Resources/WaveguideSlotAntenna/WaveguideSlotAntenna.PNG";

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
WaveguideSlotAntenna::WaveguideSlotAntenna(QWidget *parent, Qt::WindowFlags flags) :
	AbstractBlackBox(parent, flags)
{
	waveguideSlotAntennaFunc = new WaveguideSlotAntennaFunc;
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal", waveguideSlotAntennaFunc);

	QGridLayout *lay = new QGridLayout(this);

	paintingWidget = new PaintingWidget(this);

	sSlider = new QSlider(Qt::Vertical, this);
	lSlider = new QSlider(Qt::Vertical, this);
	skSlider = new QSlider(Qt::Vertical, this);
	xSlider = new QSlider(Qt::Vertical, this);
	sLabel = new QLabel("21.28", this);
	lLabel = new QLabel("11.48", this);
	skLabel = new QLabel("14.68", this);
	xLabel = new QLabel("3.68", this);

	sSlider->setRange(0, 9);
	lSlider->setRange(0, 9);
	skSlider->setRange(0, 9);
	xSlider->setRange(0, 9);

	lay->addWidget(new QLabel("s", this),	0, 0);
	lay->addWidget(sSlider,					1, 0);
	lay->addWidget(sLabel,					2, 0);
	lay->addWidget(new QLabel("l", this),	0, 1);
	lay->addWidget(lSlider,					1, 1);
	lay->addWidget(lLabel,					2, 1);
	lay->addWidget(new QLabel("sk", this),	0, 2);
	lay->addWidget(skSlider,				1, 2);
	lay->addWidget(skLabel,					2, 2);
	lay->addWidget(new QLabel("x", this),	0, 3);
	lay->addWidget(xSlider,					1, 3);
	lay->addWidget(xLabel,					2, 3);
	lay->addWidget(paintingWidget,			0, 4, 3, 1);

	connect(sSlider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect(lSlider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect(skSlider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect(xSlider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect((PaintingWidget*)paintingWidget, SIGNAL(repaintSignal()), this, SIGNAL(repaintSignal()));

	((PaintingWidget*)paintingWidget)->SetPicture();
}
QString WaveguideSlotAntenna::getDeviceName() {
	return RUS("Waveguide Slot Antenna");		//DS
}
QString WaveguideSlotAntenna::getDeviceIDName() {
	return "WaveguideSlotAntenna";
}
void WaveguideSlotAntenna::SlidersUpdate() {
	int sValue = S_START + (sSlider->value() * S_STEP);
	int skValue = SK_START + (skSlider->value() * SK_STEP);
	int lValue = L_START + (lSlider->value() * L_STEP);
	int xValue = X_START + (xSlider->value() * X_STEP);

	QString sStr = QString::number(sValue / 100.);
	QString lStr = QString::number(lValue / 100.);
	QString skStr = QString::number(skValue / 100.);
	QString xStr = QString::number(xValue / 100.);

	sLabel->setText(sStr);
	lLabel->setText(lStr);
	skLabel->setText(skStr);
	xLabel->setText(xStr);

	waveguideSlotAntennaFunc->set_s(sValue);
	waveguideSlotAntennaFunc->set_l(lValue);
	waveguideSlotAntennaFunc->set_sk(skValue);
	waveguideSlotAntennaFunc->set_x(xValue);

	this->repaint();
	emit repaintSignal();
}