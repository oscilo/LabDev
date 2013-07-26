#include "MicrostripResonatorFactory.h"

AbstractDevice* MicrostripResonatorFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags) {
	return new MicrostripResonator(parent, flags);
}