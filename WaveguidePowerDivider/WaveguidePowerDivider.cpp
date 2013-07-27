#include "WaveguidePowerDivider.h"

////////////////////////////////////////////////////////////////////////////////////////////////
PaintingWidget::PaintingWidget(QWidget* parent) : QWidget(parent) {
	pixNames[20][10] = "./Resources/WaveguidePowerDivider/h2  r1.PNG";
	pixNames[20][20] = "./Resources/WaveguidePowerDivider/h2  r2.PNG";
	pixNames[40][10] = "./Resources/WaveguidePowerDivider/h4  r1.PNG";
	pixNames[40][20] = "./Resources/WaveguidePowerDivider/h4  r2.PNG";
	pixNames[40][30] = "./Resources/WaveguidePowerDivider/h4  r3.PNG";
	pixNames[40][40] = "./Resources/WaveguidePowerDivider/h4  r4.PNG";
	pixNames[60][10] = "./Resources/WaveguidePowerDivider/h6  r1.PNG";
	pixNames[60][20] = "./Resources/WaveguidePowerDivider/h6  r2.PNG";
	pixNames[60][30] = "./Resources/WaveguidePowerDivider/h6  r3.PNG";
	pixNames[60][40] = "./Resources/WaveguidePowerDivider/h6  r4.PNG";
	pixNames[60][50] = "./Resources/WaveguidePowerDivider/h6  r5.PNG";
	pixNames[80][10] = "./Resources/WaveguidePowerDivider/h8  r1.PNG";
	pixNames[80][20] = "./Resources/WaveguidePowerDivider/h8  r2.PNG";
	pixNames[80][30] = "./Resources/WaveguidePowerDivider/h8  r3.PNG";
	pixNames[80][40] = "./Resources/WaveguidePowerDivider/h8  r4.PNG";
	pixNames[80][50] = "./Resources/WaveguidePowerDivider/h8  r5.PNG";
	pixNames[100][10] = "./Resources/WaveguidePowerDivider/h10  r1.PNG";
	pixNames[100][20] = "./Resources/WaveguidePowerDivider/h10  r2.PNG";
	pixNames[100][30] = "./Resources/WaveguidePowerDivider/h10  r3.PNG";
	pixNames[100][40] = "./Resources/WaveguidePowerDivider/h10  r4.PNG";
	pixNames[100][50] = "./Resources/WaveguidePowerDivider/h10  r5.PNG";

	this->setMinimumSize(200, 200);
}
void PaintingWidget::SetPicture(int h, int r) {
	h = H_START + (((h - H_START) / H_STEP) * H_STEP);
	r = R_START + (((r - R_START) / R_STEP) * R_STEP);

	if(pixNames.contains(h) && pixNames[h].contains(r)) {
		pix = QPixmap(pixNames[h][r]);
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
WaveguidePowerDivider::WaveguidePowerDivider(QWidget *parent, Qt::WindowFlags flags) :
	AbstractBlackBox(parent, flags)
{
	waveguidePowerDividerLeft = new WaveguidePowerDividerFunc;
	waveguidePowerDividerRight = new WaveguidePowerDividerFunc;
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal_left", waveguidePowerDividerLeft);
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal_right", waveguidePowerDividerRight);

	QGridLayout *lay = new QGridLayout(this);

	paintingWidget = new PaintingWidget(this);

	hSlider = new QSlider(Qt::Vertical, this);
	rSlider = new QSlider(Qt::Vertical, this);
	hLabel = new QLabel("2", this);
	rLabel = new QLabel("1", this);

	hSlider->setRange(20, 99);
	rSlider->setRange(10, 19);

	lay->addWidget(new QLabel("h", this),	0, 0);
	lay->addWidget(hSlider,					1, 0);
	lay->addWidget(hLabel,					2, 0);
	lay->addWidget(new QLabel("r", this),	0, 1);
	lay->addWidget(rSlider,					1, 1);
	lay->addWidget(rLabel,					2, 1);
	lay->addWidget(paintingWidget,			0, 2, 3, 1);

	connect(hSlider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect(rSlider, SIGNAL(valueChanged(int)), this, SLOT(SlidersUpdate()));
	connect((PaintingWidget*)paintingWidget, SIGNAL(repaintSignal()), this, SIGNAL(repaintSignal()));

	((PaintingWidget*)paintingWidget)->SetPicture(H_START, R_START);
}
QString WaveguidePowerDivider::getDeviceName() {
	return RUS("Делитель мощности");
}
QString WaveguidePowerDivider::getDeviceIDName() {
	return "WaveguidePowerDivider";
}
void WaveguidePowerDivider::SlidersUpdate() {
	int hValue = hSlider->value();
	int rValue = rSlider->value();

	QString hStr = QString::number(hValue / 10.);
	QString rStr = QString::number(rValue / 10.);

	hLabel->setText(hStr);
	rLabel->setText(rStr);

	int rEnd = 19;
	if(hValue >= 40)
		rEnd = 39;
	if(hValue >= 60)
		rEnd = 49;

	if(rSlider->maximum() != rEnd)
		rSlider->setMaximum(rEnd);

	((PaintingWidget*)paintingWidget)->SetPicture(hValue, rValue);
	
	waveguidePowerDividerLeft->set_h(hValue);
	waveguidePowerDividerLeft->set_r(rValue);
	waveguidePowerDividerRight->set_h(hValue);
	waveguidePowerDividerRight->set_r(rValue);

	this->repaint();
	emit repaintSignal();
}