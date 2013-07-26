#include "CoaxialWaveguideTransitionFactory.h"

AbstractDevice* CoaxialWaveguideTransitionFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags) {
	return new CoaxialWaveguideTransition(parent, flags);
}