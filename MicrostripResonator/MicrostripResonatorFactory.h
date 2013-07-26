#ifndef MICROSTRIPRESONATORFACTORY_H
#define MICROSTRIPRESONATORFACTORY_H

#include "MicrostripResonatorGlobal.h"

#include <AbstractFactory.h>

#include "MicrostripResonator.h"

class MICROSTRIPRESONATOR_EXPORT MicrostripResonatorFactory : public AbstractFactory {
	Q_OBJECT
	Q_INTERFACES(AbstractFactory)
	Q_PLUGIN_METADATA(IID "AbstractFactory")

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WindowFlags flags = 0);
};

#endif