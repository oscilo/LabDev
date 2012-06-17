#include "HighFrequencySignalsGenerator.h"

#define SUBRANGEDIAL1_X_OFFSET		380
#define SUBRANGEDIAL1_Y_OFFSET		55
#define SUBRANGEDIAL1_START			850
#define SUBRANGEDIAL1_END			1030

#define SUBRANGEDIAL2_X_OFFSET		935
#define SUBRANGEDIAL2_Y_OFFSET		55
#define SUBRANGEDIAL2_START			1030
#define SUBRANGEDIAL2_END			1210

#define RED_ARROW1_8_5				181
#define RED_ARROW1_10_3				305
#define RED_ARROW1_RANGE			0.06889
#define RED_ARROW2_10_3				747
#define RED_ARROW2_12_1				845
#define RED_ARROW2_RANGE			0.05444
#define RED_ARROW_Y					93
#define RED_ARROW_HEIGHT			16

#define SUBRANGESELECTOR_X_OFFSET	130
#define SUBRANGESELECTOR_Y_OFFSET	305

#define FREQMETER_X_OFFSET			430
#define FREQMETER_Y_OFFSET			750
#define FREQMETER_START				845
#define FREQMETER_END				1220
#define FREQMETER_V_X				505
#define FREQMETER_V_Y_START			352
#define FREQMETER_V_Y_END			495
#define FREQMETER_H_Y_START			478
#define FREQMETER_H_Y_STEP			18
#define FREQMETER_H_X_START			438
#define FREQMETER_H_X_END			576

#define DISK_X_OFFSET				505
#define DISK_Y_OFFSET				580
#define DISK_DIAMETER				460

#define AMPERMETER_X_OFFSET			1350
#define AMPERMETER_Y_OFFSET			327
#define AMPERMETER_START_LENGTH		110
#define AMPERMETER_END_LENGTH		220
#define AMPERMETER_MIN_ANGLE		43.
#define AMPERMETER_MAX_ANGLE		135.
#define AMPERMETER_ANGLE_STEP		9.2

HighFrequencySignalsGenerator::HighFrequencySignalsGenerator(QWidget *parent, Qt::WFlags flags) :
	AbstractDevice(parent, flags),
	scale_k(1),
	subRange(1),
	freqMeterVal(845.),
	subFrequency1(SUBRANGEDIAL1_START),
	subFrequency2(SUBRANGEDIAL2_START),
	diskAngle(0.),
	amperMeterAngle(AMPERMETER_MIN_ANGLE),
	isIncreasing(false)
{
	this->setSignal(SignalDirection::SD_OUTPUT_SIGNAL, "output_signal", new HFSGFunc);

	back = new QPixmap("./Resources/HighFrequencySignalsGenerator/back.png");
	redArrow = new QPixmap("./Resources/HighFrequencySignalsGenerator/red_arrow.png");
	disk = new QPixmap("./Resources/HighFrequencySignalsGenerator/freq_meter_disk.png");

	QSize temp = back->size();
	temp.scale(500, 500, Qt::KeepAspectRatioByExpanding);
	this->setMinimumSize(temp);
	
	//curSize = back->size();

	CreateWidgets();

	QTimer *amperMeterTimer = new QTimer(this);
	amperMeterTimer->setInterval(10);
	connect(amperMeterTimer, SIGNAL(timeout()), this, SLOT(AmperMeterTimerTimeout()));
	amperMeterTimer->start();
}
HighFrequencySignalsGenerator::~HighFrequencySignalsGenerator() {
	delete back;
	delete redArrow;
	delete disk;
}
QString HighFrequencySignalsGenerator::getDeviceName() {
	return RUS("Генератор сигналов высокочастотный Г4-109");
}
AbstractDevice::DeviceType HighFrequencySignalsGenerator::getDeviceType() {
	return DeviceType::DT_HFSG;
}
void HighFrequencySignalsGenerator::paintEvent(QPaintEvent *event) {
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

	QPainter pntr(this);
	QPainter pntr_pix(&pix);

	QPixmap redArrowScaled = redArrow->scaledToHeight(RED_ARROW_HEIGHT * scale, Qt::SmoothTransformation);
	pntr_pix.drawPixmap( (RED_ARROW1_8_5 + (RED_ARROW1_RANGE * (subFrequency1 - SUBRANGEDIAL1_START*10.))) * scale - redArrowScaled.width() / 2.,
						 RED_ARROW_Y * scale,
						 redArrowScaled);
	pntr_pix.drawPixmap( (RED_ARROW2_10_3 + (RED_ARROW2_RANGE * (subFrequency2 - SUBRANGEDIAL2_START*10.))) * scale - redArrowScaled.width() / 2.,
						 RED_ARROW_Y * scale,
						 redArrowScaled);
	pntr_pix.drawLine(	FREQMETER_V_X * scale,
						FREQMETER_V_Y_START * scale,
						FREQMETER_V_X * scale,
						FREQMETER_V_Y_END * scale);
	int intAngle = diskAngle * 100;
	int doublePi = 2*M_PI*100;
	float freqMeterHY = FREQMETER_H_Y_START - (intAngle / doublePi)*FREQMETER_H_Y_STEP - (intAngle % doublePi)*FREQMETER_H_Y_STEP/(float)doublePi;
	pntr_pix.drawLine(	FREQMETER_H_X_START * scale,
						freqMeterHY * scale,
						FREQMETER_H_X_END * scale,
						freqMeterHY * scale);

	float amperMeterAngleRad = (180. - amperMeterAngle) * M_PI / 180.;
	pntr_pix.drawLine(	(AMPERMETER_X_OFFSET + AMPERMETER_START_LENGTH * cos(amperMeterAngleRad)) * scale,
						(AMPERMETER_Y_OFFSET - AMPERMETER_START_LENGTH * sin(amperMeterAngleRad)) * scale,
						(AMPERMETER_X_OFFSET + AMPERMETER_END_LENGTH * cos(amperMeterAngleRad)) * scale,
						(AMPERMETER_Y_OFFSET - AMPERMETER_END_LENGTH * sin(amperMeterAngleRad)) * scale);

	pntr.drawPixmap( offset.x() + DISK_X_OFFSET * scale - transformedDisk.width() / 2,
					 offset.y() + DISK_Y_OFFSET * scale - transformedDisk.height() / 2,
					 transformedDisk);
	pntr.drawPixmap(offset, pix);
}
void HighFrequencySignalsGenerator::resizeEvent(QResizeEvent *event) {
	QSize curSize = back->size();
	curSize.scale(event->size(), Qt::KeepAspectRatio);

	QPoint offset(0, 0);
	float scale = ( (float)(curSize.width()) ) / ( (float)(back->width()) );

	if(curSize.width() != event->size().width())
		offset.setX( (event->size().width()  - curSize.width()) / 2 );
	else
		offset.setY( (event->size().height()  - curSize.height()) / 2 );

	subRangeDial1->setGeometry(offset.x() + (SUBRANGEDIAL1_X_OFFSET  * scale), offset.y() + (SUBRANGEDIAL1_Y_OFFSET * scale),
		subRangeDial1->getSizeOnInit().width() * scale,	subRangeDial1->getSizeOnInit().height() * scale);
	subRangeDial2->setGeometry(offset.x() + (SUBRANGEDIAL2_X_OFFSET  * scale), offset.y() + (SUBRANGEDIAL2_Y_OFFSET * scale),
		subRangeDial2->getSizeOnInit().width() * scale,	subRangeDial2->getSizeOnInit().height() * scale);

	QSize sss = freqMeter->getSizeOnInit();
	freqMeter->setGeometry(offset.x() + (FREQMETER_X_OFFSET  * scale), offset.y() + (FREQMETER_Y_OFFSET * scale),
		freqMeter->getSizeOnInit().width() * scale,	freqMeter->getSizeOnInit().height() * scale);
	
	QSize subRangeSelectorSize = subRangeSelector->getSizeOnInit();
	subRangeSelector->setGeometry(	offset.x() + ( (SUBRANGESELECTOR_X_OFFSET - subRangeSelectorSize.width() / 2) * scale),
									offset.y() + ( (SUBRANGESELECTOR_Y_OFFSET - subRangeSelectorSize.height() / 2) * scale),
									subRangeSelectorSize.width() * scale,	subRangeSelectorSize.height() * scale);

	scaledDisk = disk->scaledToHeight(DISK_DIAMETER * scale, Qt::SmoothTransformation);
	RetransformDisk();
}
void HighFrequencySignalsGenerator::mousePressEvent(QMouseEvent *event) {
	//this->setWindowTitle(QString("X: %1, Y: %2").arg(event->x()).arg(event->y()));
}
CommonDial* HighFrequencySignalsGenerator::CreateSubRange1() {
	CommonDial *dial = new CommonDial("./Resources/HighFrequencySignalsGenerator/comfortable_grip.png", scale_k / 10., 1, this);
	dial->setGeometry(SUBRANGEDIAL1_X_OFFSET, SUBRANGEDIAL1_Y_OFFSET, dial->width(), dial->height());
	dial->setRange(SUBRANGEDIAL1_START, SUBRANGEDIAL1_END);

	SetSubRange1(SUBRANGEDIAL1_START);
	
	connect(dial, SIGNAL(valueChanged(int)), this, SLOT(SetSubRange1(int)));

	return dial;
}
CommonDial* HighFrequencySignalsGenerator::CreateSubRange2() {
	CommonDial *dial = new CommonDial("./Resources/HighFrequencySignalsGenerator/comfortable_grip.png", scale_k / 10., 1, this);
	dial->setGeometry(SUBRANGEDIAL2_X_OFFSET, SUBRANGEDIAL2_Y_OFFSET, dial->width(), dial->height());
	dial->setRange(SUBRANGEDIAL2_START, SUBRANGEDIAL2_END);
	
	SetSubRange2(SUBRANGEDIAL2_START);

	connect(dial, SIGNAL(valueChanged(int)), this, SLOT(SetSubRange2(int)));

	return dial;
}
CommonDial* HighFrequencySignalsGenerator::CreateFreqMeter() {
	CommonDial *dial = new CommonDial("./Resources/HighFrequencySignalsGenerator/comfortable_grip.png", scale_k / 10., 1, this);
	dial->setGeometry(FREQMETER_X_OFFSET, FREQMETER_Y_OFFSET, dial->width(), dial->height());
	dial->setRange(FREQMETER_START*10, FREQMETER_END*10);
	
	SetFreqMeter(FREQMETER_START*10);

	connect(dial, SIGNAL(valueChanged(int)), this, SLOT(SetFreqMeter(int)));

	return dial;
}
SubRangeSelector* HighFrequencySignalsGenerator::CreateSubRangeSelector() {
	SubRangeSelector *selector = new SubRangeSelector(scale_k / 10., this);
	selector->setGeometry(SUBRANGESELECTOR_X_OFFSET, SUBRANGESELECTOR_Y_OFFSET, selector->width(), selector->height());

	connect(selector, SIGNAL(valueChanged(int)), this, SLOT(SelectSubRange(int)));

	return selector;
}
void HighFrequencySignalsGenerator::CreateWidgets() {
	subRangeDial1 =		CreateSubRange1();
	subRangeDial2 =		CreateSubRange2();
	freqMeter =			CreateFreqMeter();
	subRangeSelector =	CreateSubRangeSelector();
}
void HighFrequencySignalsGenerator::SetSubRange1(int val) {
	subFrequency1 = val * 10.;

	if(1 == subRange) {
		frequency = subFrequency1;
		
		UpdateIncreasing();
		emit SetFrequency(frequency);
	}

	this->repaint();
}
void HighFrequencySignalsGenerator::SetSubRange2(int val) {
	subFrequency2 = val * 10.;

	if(2 == subRange) {
		frequency = subFrequency2;
		
		UpdateIncreasing();
		emit SetFrequency(frequency);
	}
	
	this->repaint();
}
void HighFrequencySignalsGenerator::SelectSubRange(int val) {
	subRange = val;

	SetSubRange1(subFrequency1 / 10.);
	SetSubRange2(subFrequency2 / 10.);

	this->repaint();
}
void HighFrequencySignalsGenerator::SetFreqMeter(int val) {
	freqMeterVal = val / 10.;

	UpdateIncreasing();

	RetransformDisk();

	this->repaint();
}
void HighFrequencySignalsGenerator::RetransformDisk() {
	QTransform mtrx;
	int scaledDiskWidth = scaledDisk.width();
	int scaledDiskHeight = scaledDisk.width();
	mtrx.translate(scaledDiskWidth/2, scaledDiskHeight/2);

	diskAngle = log((freqMeterVal-845.)/104 + 1)*104/log(53.5);
	mtrx.rotateRadians(-diskAngle);
	
	transformedDisk = scaledDisk.transformed(mtrx, Qt::SmoothTransformation);
	transformedDisk = transformedDisk.copy( transformedDisk.width()/2 - scaledDiskWidth/2,
											transformedDisk.height()/2 - scaledDiskHeight/2,
											scaledDiskWidth,
											scaledDiskHeight);
}
void HighFrequencySignalsGenerator::AmperMeterTimerTimeout() {
	if(isIncreasing)
		amperMeterAngle += AMPERMETER_ANGLE_STEP;
	else
		amperMeterAngle -= AMPERMETER_ANGLE_STEP;

	if(amperMeterAngle > AMPERMETER_MAX_ANGLE)
		amperMeterAngle = AMPERMETER_MAX_ANGLE;
	if(amperMeterAngle < AMPERMETER_MIN_ANGLE)
		amperMeterAngle = AMPERMETER_MIN_ANGLE;

	this->repaint();
}
void HighFrequencySignalsGenerator::UpdateIncreasing() {
	isIncreasing = ( abs(frequency - freqMeterVal*10.) < 0.0001 ) ? true : false;
}