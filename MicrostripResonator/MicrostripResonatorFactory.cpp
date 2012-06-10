#include "MicrostripResonatorFactory.h"

AbstractDevice* MicrostripResonatorFactory::CreateDevice(QWidget *parent, Qt::WFlags flags) {
	return new MicrostripResonator(parent, flags);
}

Q_EXPORT_PLUGIN2(MicrostripResonatorFactory, MicrostripResonatorFactory);