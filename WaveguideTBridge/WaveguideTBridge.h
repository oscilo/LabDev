#ifndef WAVEGUIDETBRIDGE_H
#define WAVEGUIDETBRIDGE_H

#include "globals.h"

//#include "Function.h"

#include <AbstractDevice.h>

class WaveguideTBridge : public AbstractDevice {
	Q_OBJECT
	Q_INTERFACES(AbstractDevice)

public:
	WaveguideTBridge(QWidget *parent = 0, Qt::WFlags flags = 0);
	~WaveguideTBridge();

	QString getDeviceName();
	DeviceType getDeviceType();

private:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

	QPixmap *back;
	QPixmap resizedBack;
};

#endif // WAVEGUIDETBRIDGE_H