#ifndef SWEEPFREQUENCYGENERATORFACTORY_H
#define SWEEPFREQUENCYGENERATORFACTORY_H

#include "SweepFrequencyGeneratorGlobal.h"

#include <AbstractFactory.h>

#include "SweepFrequencyGenerator.h"

class SweepFrequencyGeneratorFactory : public AbstractFactory
{
	Q_INTERFACES(AbstractFactory)

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WFlags flags = 0);
};

#endif