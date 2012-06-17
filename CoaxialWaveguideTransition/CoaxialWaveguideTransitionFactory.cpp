#include "CoaxialWaveguideTransitionFactory.h"

AbstractDevice* CoaxialWaveguideTransitionFactory::CreateDevice(QWidget *parent, Qt::WFlags flags) {
	return new CoaxialWaveguideTransition(parent, flags);
}

Q_EXPORT_PLUGIN2(CoaxialWaveguideTransitionFactory, CoaxialWaveguideTransitionFactory);