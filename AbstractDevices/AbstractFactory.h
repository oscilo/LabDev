#ifndef ABSTRACT_FACTORY_H
#define ABSTRACT_FACTORY_H

#include <AbstractDevice.h>

class AbstractFactory : public QObject
{
	Q_OBJECT

public:
	virtual AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WindowFlags flags = 0) = 0;
};

Q_DECLARE_INTERFACE(AbstractFactory, "AbstractFactory");

#endif