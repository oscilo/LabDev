#ifndef WAVEGUIDETBRIDGEFACTORY_H
#define WAVEGUIDETBRIDGEFACTORY_H

#include "WaveguideTBridgeGlobal.h"

#include <AbstractFactory.h>

#include "WaveguideTBridge.h"

class WAVEGUIDETBRIDGE_EXPORT WaveguideTBridgeFactory : public AbstractFactory {
	Q_INTERFACES(AbstractFactory)
	Q_PLUGIN_METADATA(IID "AbstractFactory")

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WindowFlags flags = 0);
};

#endif