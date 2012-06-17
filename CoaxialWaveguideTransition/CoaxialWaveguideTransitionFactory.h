#ifndef COAXIALWAVEGUIDETRANSITIONFACTORY_H
#define COAXIALWAVEGUIDETRANSITIONFACTORY_H

#include "CoaxialWaveguideTransitionGlobal.h"

#include <AbstractFactory.h>

#include "CoaxialWaveguideTransition.h"

class COAXIALWAVEGUIDETRANSITION_EXPORT CoaxialWaveguideTransitionFactory : public AbstractFactory {
	Q_INTERFACES(AbstractFactory)

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WFlags flags = 0);
};

#endif