#include "WaveguidePowerDividerFactory.h"

AbstractDevice* WaveguidePowerDividerFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags) {
	return new WaveguidePowerDivider(parent, flags);
}