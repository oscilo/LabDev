#include "WaveguideTBridgeFactory.h"

AbstractDevice* WaveguideTBridgeFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags) {
	return new WaveguideTBridge(parent, flags);
}