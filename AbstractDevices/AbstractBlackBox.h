#ifndef ABSTRACTBLACKBOX_H
#define ABSTRACTBLACKBOX_H

#include <QWidget>

#include <AbstractDevice.h>

class AbstractBlackBox: public AbstractDevice
{
	Q_OBJECT

public:
	AbstractBlackBox(QWidget *parent = 0, Qt::WindowFlags flags = 0) : AbstractDevice(parent, flags) {}
	~AbstractBlackBox() {}

	DeviceType getDeviceType()
	{
		return DeviceType::DT_BLACK_BOX;
	}
};

Q_DECLARE_INTERFACE(AbstractBlackBox, "AbstractBlackBox");

#endif