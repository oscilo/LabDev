#include "microstripresonator.h"

////////////////////////////////////////////////////////////////////////////////////////////////
PaintingWidget::PaintingWidget(QWidget* parent) : QWidget(parent) {
	pixNames[10][18] = "./Resources/MicrostripResonator/s 1, l 18.PNG";
	pixNames[10][20] = "./Resources/MicrostripResonator/s 1, l 20.PNG";
	pixNames[10][22] = "./Resources/MicrostripResonator/s 1, l 22.PNG";
	pixNames[10][24] = "./Resources/MicrostripResonator/s 1, l 24.PNG";
	pixNames[10][26] = "./Resources/MicrostripResonator/s 1, l 26.PNG";
	pixNames[15][18] = "./Resources/MicrostripResonator/s 1.5, l 18.PNG";
	pixNames[15][20] = "./Resources/MicrostripResonator/s 1.5, l 20.PNG";
	pixNames[15][22] = "./Resources/MicrostripResonator/s 1.5, l 22.PNG";
	pixNames[15][24] = "./Resources/MicrostripResonator/s 1.5, l 24.PNG";
	pixNames[15][26] = "./Resources/MicrostripResonator/s 1.5, l 26.PNG";
	pixNames[20][18] = "./Resources/MicrostripResonator/s 2, l 18.PNG";
	pixNames[20][20] = "./Resources/MicrostripResonator/s 2, l 20.PNG";
	pixNames[20][22] = "./Resources/MicrostripResonator/s 2, l 22.PNG";
	pixNames[20][24] = "./Resources/MicrostripResonator/s 2, l 24.PNG";
	pixNames[20][26] = "./Resources/MicrostripResonator/s 2, l 26.PNG";

	this->setMinimumSize(200, 200);
}
void PaintingWidget::SetPicture(int s, int l) {
	s = S_START + (((s - S_START) / S_STEP) * S_STEP);
	l = L_START + (((l - L_START) / L_STEP) * L_STEP);

	if(pixNames.contains(s) && pixNames[s].contains(l)) {
		pix = QPixmap(pixNames[s][l]);
		ScalePixmap();
	}
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
MicrostripResonator::MicrostripResonator(QWidget *parent, Qt::WindowFlags flags) :
	AbstractBlackBox(parent, flags)
{
	microstripResonatorFunc = new MicrostripResonatorFunc;
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal", microstripResonatorFunc);

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
QString MicrostripResonator::getDeviceName() {
	return RUS("Microstrip Resonator");		//DS
}
QString MicrostripResonator::getDeviceIDName() {
	return "MicrostripResonator";
}
void MicrostripResonator::SlidersUpdate() {
	int sValue = sSlider->value();
	int lValue = lSlider->value();

	QString sStr = QString::number(sValue / 10.);
	QString lStr = QString::number(lValue / 1);

	sLabel->setText(sStr);
	lLabel->setText(lStr);

	((PaintingWidget*)paintingWidget)->SetPicture(sValue, lValue);
	
	microstripResonatorFunc->set_s(sValue);
	microstripResonatorFunc->set_l(lValue);

	this->repaint();
	emit repaintSignal();
}