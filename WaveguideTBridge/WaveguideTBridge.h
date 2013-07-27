#ifndef WAVEGUIDETBRIDGE_H
#define WAVEGUIDETBRIDGE_H

#include "globals.h"

#include "Function.h"

#include <AbstractBlackBox.h>

class WaveguideTBridge : public AbstractBlackBox {
	Q_OBJECT
	Q_INTERFACES(AbstractDevice)

public:
	WaveguideTBridge(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~WaveguideTBridge();

	QString getDeviceName();
	QString getDeviceIDName();

private:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

	QPixmap *back;
	QPixmap resizedBack;
};

#endif // WAVEGUIDETBRIDGE_H