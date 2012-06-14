#include "WaveguidePowerDividerFactory.h"

AbstractDevice* WaveguidePowerDividerFactory::CreateDevice(QWidget *parent, Qt::WFlags flags) {
	return new WaveguidePowerDivider(parent, flags);
}

Q_EXPORT_PLUGIN2(WaveguidePowerDividerFactory, WaveguidePowerDividerFactory);