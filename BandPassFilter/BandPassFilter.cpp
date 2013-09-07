#include "BandPassFilter.h"

////////////////////////////////////////////////////////////////////////////////////////////////
PaintingWidget::PaintingWidget(QWidget* parent) : QWidget(parent) {
	pixNames[0][0]		= "./Resources/BandPassFilter/h1 0, h2 0.PNG";
	pixNames[0][20]		= "./Resources/BandPassFilter/h1 0, h2 2.PNG";
	pixNames[0][40]		= "./Resources/BandPassFilter/h1 0, h2 4.PNG";
	pixNames[0][60]		= "./Resources/BandPassFilter/h1 0, h2 6.PNG";
	pixNames[0][80]		= "./Resources/BandPassFilter/h1 0, h2 8.PNG";
	pixNames[0][100]	= "./Resources/BandPassFilter/h1 0, h2 10.PNG";
	pixNames[20][0]		= "./Resources/BandPassFilter/h1 2, h2 0.PNG";
	pixNames[20][20]	= "./Resources/BandPassFilter/h1 2, h2 2.PNG";
	pixNames[20][40]	= "./Resources/BandPassFilter/h1 2, h2 4.PNG";
	pixNames[20][60]	= "./Resources/BandPassFilter/h1 2, h2 6.PNG";
	pixNames[20][80]	= "./Resources/BandPassFilter/h1 2, h2 8.PNG";
	pixNames[20][100]	= "./Resources/BandPassFilter/h1 2, h2 10.PNG";
	pixNames[40][0]		= "./Resources/BandPassFilter/h1 4, h2 0.PNG";
	pixNames[40][20]	= "./Resources/BandPassFilter/h1 4, h2 2.PNG";
	pixNames[40][40]	= "./Resources/BandPassFilter/h1 4, h2 4.PNG";
	pixNames[40][60]	= "./Resources/BandPassFilter/h1 4, h2 6.PNG";
	pixNames[40][80]	= "./Resources/BandPassFilter/h1 4, h2 8.PNG";
	pixNames[40][100]	= "./Resources/BandPassFilter/h1 4, h2 10.PNG";
	pixNames[60][0]		= "./Resources/BandPassFilter/h1 6, h2 0.PNG";
	pixNames[60][20]	= "./Resources/BandPassFilter/h1 6, h2 2.PNG";
	pixNames[60][40]	= "./Resources/BandPassFilter/h1 6, h2 4.PNG";
	pixNames[60][60]	= "./Resources/BandPassFilter/h1 6, h2 6.PNG";
	pixNames[60][80]	= "./Resources/BandPassFilter/h1 6, h2 8.PNG";
	pixNames[60][100]	= "./Resources/BandPassFilter/h1 6, h2 10.PNG";
	pixNames[80][0]		= "./Resources/BandPassFilter/h1 8, h2 0.PNG";
	pixNames[80][20]	= "./Resources/BandPassFilter/h1 8, h2 2.PNG";
	pixNames[80][40]	= "./Resources/BandPassFilter/h1 8, h2 4.PNG";
	pixNames[80][60]	= "./Resources/BandPassFilter/h1 8, h2 6.PNG";
	pixNames[80][80]	= "./Resources/BandPassFilter/h1 8, h2 8.PNG";
	pixNames[80][100]	= "./Resources/BandPassFilter/h1 8, h2 10.PNG";
	pixNames[100][0]	= "./Resources/BandPassFilter/h1 10, h2 0.PNG";
	pixNames[100][20]	= "./Resources/BandPassFilter/h1 10, h2 2.PNG";
	pixNames[100][40]	= "./Resources/BandPassFilter/h1 10, h2 4.PNG";
	pixNames[100][60]	= "./Resources/BandPassFilter/h1 10, h2 6.PNG";
	pixNames[100][80]	= "./Resources/BandPassFilter/h1 10, h2 8.PNG";
	pixNames[100][100]	= "./Resources/BandPassFilter/h1 10, h2 10.PNG";

	this->setMinimumSize(200, 200);
}
void PaintingWidget::SetPicture(int h1, int h2) {
	h1 = H1_START + (((h1 - H1_START) / H1_STEP) * H1_STEP);
	h2 = H2_START + (((h2 - H2_START) / H2_STEP) * H2_STEP);

	if(pixNames.contains(h1) && pixNames[h1].contains(h2)) {
		pix = QPixmap(pixNames[h1][h2]);
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
BandPassFilter::BandPassFilter(QWidget *parent, Qt::WindowFlags flags) :
	AbstractBlackBox(parent, flags)
{
	bandPassFilterFunc = new BandPassFilterFunc;
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal", bandPassFilterFunc);

	QGridLayout *lay = new QGridLayout(this);

	paintingWidget = new PaintingWidget(this);

	h1Slider = new QSlider(Qt::Vertical, this);
	h2Slider = new QSlider(Qt::Vertical, this);
	h1Label = new QLabel("0", this);
	h2Label = new QLabel("0", this);

	h1Slider->setRange(0, 99);
	h2Slider->setRange(0, 99);

	lay->addWidget(new QLabel("h1", this),	0, 0);
	lay->addWidget(h1Slider,				1, 0);
	lay->addWidget(h1Label,					2, 0);
	lay->addWidget(new QLabel("h2", this),	0, 1);
	lay->addWidget(h2Slider,				1, 1);
	lay->addWidget(h2Label,					2, 1);
	lay->addWidget(paintingWidget,			0, 2, 3, 1);

	connect(h1Slider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect(h2Slider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect((PaintingWidget*)paintingWidget, SIGNAL(repaintSignal()), this, SIGNAL(repaintSignal()));

	((PaintingWidget*)paintingWidget)->SetPicture(H1_START, H2_START);
}
QString BandPassFilter::getDeviceName() {
	return RUS("Band Pass Filter");	//DS
}
QString BandPassFilter::getDeviceIDName() {
	return "BandPassFilter";
}
void BandPassFilter::SlidersUpdate() {
	int h1Value = h1Slider->value();
	int h2Value = h2Slider->value();

	QString h1Str = QString::number(h1Value / 10.);
	QString h2Str = QString::number(h2Value / 10.);

	h1Label->setText(h1Str);
	h2Label->setText(h2Str);

	((PaintingWidget*)paintingWidget)->SetPicture(h1Value, h2Value);
	
	bandPassFilterFunc->set_h1(h1Value);
	bandPassFilterFunc->set_h2(h2Value);

	this->repaint();
	emit repaintSignal();
}