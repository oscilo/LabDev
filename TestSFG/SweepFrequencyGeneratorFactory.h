#ifndef SWEEPFREQUENCYGENERATORFACTORY_H
#define SWEEPFREQUENCYGENERATORFACTORY_H

#include "SweepFrequencyGeneratorGlobal.h"

#include <AbstractFactory.h>

#include "SweepFrequencyGenerator.h"

class SweepFrequencyGeneratorFactory : public AbstractFactory
{
	Q_INTERFACES(AbstractFactory)
	Q_PLUGIN_METADATA(IID "AbstractFactory")

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WindowFlags flags = 0);
};

#endif