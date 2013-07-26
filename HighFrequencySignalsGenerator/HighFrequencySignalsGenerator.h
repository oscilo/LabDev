#ifndef HIGHFREQUENCYSIGNALSGENERATOR_H
#define HIGHFREQUENCYSIGNALSGENERATOR_H

#include "globals.h"

#include "Function.h"

#include <CommonDial.h>
#include <SubRangeSelector.h>

#include <AbstractDevice.h>

class HighFrequencySignalsGenerator : public AbstractDevice {
	Q_OBJECT
	Q_INTERFACES(AbstractDevice)

public:
	HighFrequencySignalsGenerator(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~HighFrequencySignalsGenerator();

	QString getDeviceName();
	DeviceType getDeviceType();

signals:
	void SetFrequency(float);

private slots:
	void SetSubRange1(int);
	void SetSubRange2(int);
	void SelectSubRange(int);
	void SetFreqMeter(int);
	void AmperMeterTimerTimeout();

private:
	void RetransformDisk();
	void UpdateIncreasing();
	void CreateWidgets();
	CommonDial* CreateSubRange1();
	CommonDial* CreateSubRange2();
	CommonDial* CreateFreqMeter();
	SubRangeSelector* CreateSubRangeSelector();

	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void mousePressEvent(QMouseEvent *event);

	CommonDial *subRangeDial1;
	CommonDial *subRangeDial2;
	CommonDial *freqMeter;
	SubRangeSelector *subRangeSelector;

	float scale_k;

	QPixmap *back;
	QPixmap *redArrow;
	QPixmap *disk;
	QPixmap scaledDisk;
	QPixmap transformedDisk;

	//QSize curSize;
	QPixmap resizedBack;
	qreal diskAngle;
	qreal amperMeterAngle;
	bool isIncreasing;

	int subRange;
	float subFrequency1;
	float subFrequency2;
	float frequency;
	float freqMeterVal;
};

#endif // HIGHFREQUENCYSIGNALSGENERATOR_H
