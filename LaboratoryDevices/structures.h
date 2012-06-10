#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#include <AbstractDevice.h>

/*
struct DeviceID {
	int id;
	int order;

	DeviceID() : id(-1), order(-1) {}
	DeviceID(int id_, int order_) : id(id_), order(order_) {}

	bool operator == (const DeviceID &a) const{
		return (this->id == a.id) && (this->order == a.order);
	}
};
//*/

struct DeviceInfo {
	int id;
	QString name;
	AbstractDevice::DeviceType type;
	QStringList inputs;
	QStringList outputs;
	int order;

	QUuid uuid;
	DeviceInfo() : id(-1), order(-1), uuid(QUuid::createUuid()) {}
	//DeviceID GetDeveceID() { return DeviceID(this->id, this->order); }
};

Q_DECLARE_METATYPE(QUuid)
//Q_DECLARE_METATYPE(DeviceID)

#endif