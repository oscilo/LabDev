#ifndef MICROSTRIPRESONATORFACTORY_H
#define MICROSTRIPRESONATORFACTORY_H

#include "MicrostripResonatorGlobal.h"

#include <AbstractFactory.h>

#include "MicrostripResonator.h"

class MICROSTRIPRESONATOR_EXPORT MicrostripResonatorFactory : public AbstractFactory {
	Q_INTERFACES(AbstractFactory)

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WFlags flags = 0);
};

#endif