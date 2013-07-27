#ifndef SelectiveMicrovoltmeter_H
#define SelectiveMicrovoltmeter_H

#include "globals.h"

//#include "Function.h"

#include <AbstractDevice.h>

class SelectiveMicrovoltmeter : public AbstractDevice {
	Q_OBJECT
	Q_INTERFACES(AbstractDevice)

public:
	SelectiveMicrovoltmeter(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~SelectiveMicrovoltmeter();

	QString getDeviceName();
	DeviceType getDeviceType();
	QString getDeviceIDName();

private:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

	QPixmap *back;
	QPixmap resizedBack;
};

#endif // WAVEGUIDETBRIDGE_H