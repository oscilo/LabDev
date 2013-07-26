#ifndef _DEVICE_EXCEPTION_H
#define _DEVICE_EXCEPTION_H

#include <QtCore>

#include <QString>

#include <QException>

class DeviceException : public QException
{
public:
	DeviceException(const QString &_mess) : mess(_mess) {}
	DeviceException(const DeviceException &a) : QException(a), mess(a.getMess()) {}

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