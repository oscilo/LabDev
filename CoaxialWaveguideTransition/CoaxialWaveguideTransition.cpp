#include "CoaxialWaveguideTransition.h"

////////////////////////////////////////////////////////////////////////////////////////////////
PaintingWidget::PaintingWidget(QWidget* parent) : QWidget(parent) {
	pixNames[0][35] = "./Resources/CoaxialWaveguideTransition/l 0 s 3.5.PNG";
	pixNames[0][65] = "./Resources/CoaxialWaveguideTransition/l 0 s 6.5.PNG";
	pixNames[0][95] = "./Resources/CoaxialWaveguideTransition/l 0 s 9.5.PNG";
	pixNames[0][125] = "./Resources/CoaxialWaveguideTransition/l 0 s 12.5.PNG";
	pixNames[0][155] = "./Resources/CoaxialWaveguideTransition/l 0 s 15.5.PNG";
	pixNames[20][35] = "./Resources/CoaxialWaveguideTransition/l 2 s 3.5.PNG";
	pixNames[20][65] = "./Resources/CoaxialWaveguideTransition/l 2 s 6.5.PNG";
	pixNames[20][95] = "./Resources/CoaxialWaveguideTransition/l 2 s 9.5.PNG";
	pixNames[20][125] = "./Resources/CoaxialWaveguideTransition/l 2 s 12.5.PNG";
	pixNames[20][155] = "./Resources/CoaxialWaveguideTransition/l 2 s 15.5.PNG";
	pixNames[40][35] = "./Resources/CoaxialWaveguideTransition/l 4 s 3.5.PNG";
	pixNames[40][65] = "./Resources/CoaxialWaveguideTransition/l 4 s 6.5.PNG";
	pixNames[40][95] = "./Resources/CoaxialWaveguideTransition/l 4 s 9.5.PNG";
	pixNames[40][125] = "./Resources/CoaxialWaveguideTransition/l 4 s 12.5.PNG";
	pixNames[40][155] = "./Resources/CoaxialWaveguideTransition/l 4 s 15.5.PNG";
	pixNames[60][35] = "./Resources/CoaxialWaveguideTransition/l 6 s 3.5.PNG";
	pixNames[60][65] = "./Resources/CoaxialWaveguideTransition/l 6 s 6.5.PNG";
	pixNames[60][95] = "./Resources/CoaxialWaveguideTransition/l 6 s 9.5.PNG";
	pixNames[60][125] = "./Resources/CoaxialWaveguideTransition/l 6 s 12.5.PNG";
	pixNames[60][155] = "./Resources/CoaxialWaveguideTransition/l 6 s 15.5.PNG";
	pixNames[80][35] = "./Resources/CoaxialWaveguideTransition/l 8 s 3.5.PNG";
	pixNames[80][65] = "./Resources/CoaxialWaveguideTransition/l 8 s 6.5.PNG";
	pixNames[80][95] = "./Resources/CoaxialWaveguideTransition/l 8 s 9.5.PNG";
	pixNames[80][125] = "./Resources/CoaxialWaveguideTransition/l 8 s 12.5.PNG";
	pixNames[80][155] = "./Resources/CoaxialWaveguideTransition/l 8 s 15.5.PNG";
	pixNames[100][35] = "./Resources/CoaxialWaveguideTransition/l 10 s 3.5.PNG";
	pixNames[100][65] = "./Resources/CoaxialWaveguideTransition/l 10 s 6.5.PNG";
	pixNames[100][95] = "./Resources/CoaxialWaveguideTransition/l 10 s 9.5.PNG";
	pixNames[100][125] = "./Resources/CoaxialWaveguideTransition/l 10 s 12.5.PNG";
	pixNames[100][155] = "./Resources/CoaxialWaveguideTransition/l 10 s 15.5.PNG";

	this->setMinimumSize(200, 200);
}
void PaintingWidget::SetPicture(int s, int l) {
	s = S_START + (((s - S_START) / S_STEP) * S_STEP);
	l = L_START + (((l - L_START) / L_STEP) * L_STEP);

	if(pixNames.contains(l) && pixNames[l].contains(s)) {
		pix = QPixmap(pixNames[l][s]);
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
CoaxialWaveguideTransition::CoaxialWaveguideTransition(QWidget *parent, Qt::WindowFlags flags) :
	AbstractBlackBox(parent, flags)
{
	coaxialWaveguideTransitionFunc = new CoaxialWaveguideTransitionFunc;
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal", coaxialWaveguideTransitionFunc);

	QGridLayout *lay = new QGridLayout(this);

	paintingWidget = new PaintingWidget(this);

	sSlider = new QSlider(Qt::Vertical, this);
	lSlider = new QSlider(Qt::Vertical, this);
	sLabel = new QLabel("3.5", this);
	lLabel = new QLabel("0", this);

	sSlider->setRange(35, 154);
	lSlider->setRange(0, 99);

	lay->addWidget(new QLabel("l", this),	0, 0);
	lay->addWidget(lSlider,					1, 0);
	lay->addWidget(lLabel,					2, 0);
	lay->addWidget(new QLabel("s", this),	0, 1);
	lay->addWidget(sSlider,					1, 1);
	lay->addWidget(sLabel,					2, 1);
	lay->addWidget(paintingWidget,			0, 2, 3, 1);

	connect(sSlider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect(lSlider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect((PaintingWidget*)paintingWidget, SIGNAL(repaintSignal()), this, SIGNAL(repaintSignal()));

	((PaintingWidget*)paintingWidget)->SetPicture(S_START, L_START);
}
QString CoaxialWaveguideTransition::getDeviceName() {
	return RUS("Coaxial - Waveguide Transition");		//DS
}
QString CoaxialWaveguideTransition::getDeviceIDName() {
	return "CoaxialWaveguideTransition";
}
void CoaxialWaveguideTransition::SlidersUpdate() {
	int sValue = sSlider->value();
	int lValue = lSlider->value();

	QString sStr = QString::number(sValue / 10.);
	QString lStr = QString::number(lValue / 10.);

	sLabel->setText(sStr);
	lLabel->setText(lStr);

	((PaintingWidget*)paintingWidget)->SetPicture(sValue, lValue);
	
	coaxialWaveguideTransitionFunc->set_s(sValue);
	coaxialWaveguideTransitionFunc->set_l(lValue);

	this->repaint();
	emit repaintSignal();
}