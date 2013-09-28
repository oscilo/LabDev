#ifndef MICROSTRIPRESONATORFACTORY_H
#define MICROSTRIPRESONATORFACTORY_H

#include "WaveguideSlotAntennaGlobal.h"

#include <AbstractFactory.h>

#include "WaveguideSlotAntenna.h"

class WAVEGUIDESLOTANTENNA_EXPORT WaveguideSlotAntennaFactory : public AbstractFactory {
	Q_OBJECT
	Q_INTERFACES(AbstractFactory)
	Q_PLUGIN_METADATA(IID "AbstractFactory")

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WindowFlags flags = 0);
};

#endif