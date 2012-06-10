#ifndef _DEVICE_EXCEPTION_H
#define _DEVICE_EXCEPTION_H

#include <QtCore>

#include <QString>

class DeviceException : public QtConcurrent::Exception
{
public:
	DeviceException(const QString &_mess) : mess(_mess) {}
	DeviceException(const DeviceException &a) : QtConcurrent::Exception(a), mess(a.getMess()) {}

	DeviceException *clone() const
		{ return new DeviceException(*this); }

	void raise() const
		{ throw *this; }

	const QString getMess() const
		{ return mess; }
private:
	QString mess;
};

#endif