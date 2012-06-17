#include "WaveguideTBridgeFactory.h"

AbstractDevice* WaveguideTBridgeFactory::CreateDevice(QWidget *parent, Qt::WFlags flags) {
	return new WaveguideTBridge(parent, flags);
}

Q_EXPORT_PLUGIN2(WaveguideTBridgeFactory, WaveguideTBridgeFactory);