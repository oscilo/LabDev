#ifndef WAVEGUIDEPOWERDIVIDERFACTORY_H
#define WAVEGUIDEPOWERDIVIDERFACTORY_H

#include "WaveguidePowerDividerGlobal.h"

#include <AbstractFactory.h>

#include "WaveguidePowerDivider.h"

class WAVEGUIDEPOWERDIVIDER_EXPORT WaveguidePowerDividerFactory : public AbstractFactory {
	Q_INTERFACES(AbstractFactory)
	Q_PLUGIN_METADATA(IID "AbstractFactory")

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WindowFlags flags = 0);
};

#endif