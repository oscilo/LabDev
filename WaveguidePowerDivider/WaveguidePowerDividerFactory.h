#ifndef WAVEGUIDEPOWERDIVIDERFACTORY_H
#define WAVEGUIDEPOWERDIVIDERFACTORY_H

#include "WaveguidePowerDividerGlobal.h"

#include <AbstractFactory.h>

#include "WaveguidePowerDivider.h"

class WAVEGUIDEPOWERDIVIDER_EXPORT WaveguidePowerDividerFactory : public AbstractFactory {
	Q_INTERFACES(AbstractFactory)

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WFlags flags = 0);
};

#endif