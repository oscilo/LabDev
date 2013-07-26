#ifndef REDUCTIONINDICATORFACTORY_H
#define REDUCTIONINDICATORFACTORY_H

#include "ReductionIndicatorGlobal.h"

#include <AbstractFactory.h>

#include "ReductionIndicator.h"

class REDUCTIONINDICATOR_EXPORT ReductionIndicatorFactory : public AbstractFactory
{
	Q_INTERFACES(AbstractFactory)
	Q_PLUGIN_METADATA(IID "AbstractFactory")

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WindowFlags flags = 0);
};

#endif