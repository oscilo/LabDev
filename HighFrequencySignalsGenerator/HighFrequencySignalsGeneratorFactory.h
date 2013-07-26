#ifndef HIGHFREQUENCYSIGNALSGENERATORFACTORY_H
#define HIGHFREQUENCYSIGNALSGENERATORFACTORY_H

#include "HighFrequencySignalsGeneratorGlobal.h"

#include <AbstractFactory.h>

#include "HighFrequencySignalsGenerator.h"

class HIGHFREQUENCYSIGNALSGENERATOR_EXPORT HighFrequencySignalsGeneratorFactory : public AbstractFactory {
	Q_OBJECT
	Q_INTERFACES(AbstractFactory)
	Q_PLUGIN_METADATA(IID "AbstractFactory")

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WindowFlags flags = 0);
};

#endif