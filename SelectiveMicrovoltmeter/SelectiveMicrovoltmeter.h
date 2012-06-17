#ifndef SelectiveMicrovoltmeter_H
#define SelectiveMicrovoltmeter_H

#include "globals.h"

//#include "Function.h"

#include <AbstractDevice.h>

class SelectiveMicrovoltmeter : public AbstractDevice {
	Q_OBJECT
	Q_INTERFACES(AbstractDevice)

public:
	SelectiveMicrovoltmeter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SelectiveMicrovoltmeter();

	QString getDeviceName();
	DeviceType getDeviceType();

private:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);

	QPixmap *back;
	QPixmap resizedBack;
};

#endif // WAVEGUIDETBRIDGE_H