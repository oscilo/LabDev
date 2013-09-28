#include "WaveguideSlotAntennaFactory.h"

AbstractDevice* WaveguideSlotAntennaFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags) {
	return new WaveguideSlotAntenna(parent, flags);
}