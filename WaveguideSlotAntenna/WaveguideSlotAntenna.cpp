#include "WaveguideSlotAntenna.h"

////////////////////////////////////////////////////////////////////////////////////////////////
PaintingWidget::PaintingWidget(QWidget* parent) : QWidget(parent) {
	pixName = "./Resources/MicrostripResonator/s 1, l 18.PNG";

	this->setMinimumSize(200, 200);
}
void PaintingWidget::SetPicture(int s, int l) {
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
	sLabel = new QLabel("1", this);
	lLabel = new QLabel("18", this);

	sSlider->setRange(10, 19);
	lSlider->setRange(18, 25);

	lay->addWidget(new QLabel("s", this),	0, 0);
	lay->addWidget(sSlider,					1, 0);
	lay->addWidget(sLabel,					2, 0);
	lay->addWidget(new QLabel("l", this),	0, 1);
	lay->addWidget(lSlider,					1, 1);
	lay->addWidget(lLabel,					2, 1);
	lay->addWidget(paintingWidget,			0, 2, 3, 1);

	connect(sSlider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect(lSlider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect((PaintingWidget*)paintingWidget, SIGNAL(repaintSignal()), this, SIGNAL(repaintSignal()));

	((PaintingWidget*)paintingWidget)->SetPicture(S_START, L_START);
}
QString WaveguideSlotAntenna::getDeviceName() {
	return RUS("Waveguide Slot Antenna");		//DS
}
QString WaveguideSlotAntenna::getDeviceIDName() {
	return "WaveguideSlotAntenna";
}
void WaveguideSlotAntenna::SlidersUpdate() {
	int sValue = sSlider->value();
	int lValue = lSlider->value();

	QString sStr = QString::number(sValue / 10.);
	QString lStr = QString::number(lValue / 1);

	sLabel->setText(sStr);
	lLabel->setText(lStr);

	((PaintingWidget*)paintingWidget)->SetPicture(sValue, lValue);
	
	waveguideSlotAntennaFunc->set_s(sValue);
	waveguideSlotAntennaFunc->set_l(lValue);

	this->repaint();
	emit repaintSignal();
}