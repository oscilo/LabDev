#ifndef HIGHFREQUENCYSIGNALSGENERATORFACTORY_H
#define HIGHFREQUENCYSIGNALSGENERATORFACTORY_H

#include "HighFrequencySignalsGeneratorGlobal.h"

#include <AbstractFactory.h>

#include "HighFrequencySignalsGenerator.h"

class HIGHFREQUENCYSIGNALSGENERATOR_EXPORT HighFrequencySignalsGeneratorFactory : public AbstractFactory {
	Q_INTERFACES(AbstractFactory)

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WFlags flags = 0);
};

#endif