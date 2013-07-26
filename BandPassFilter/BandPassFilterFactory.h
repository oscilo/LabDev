#ifndef BANDPASSFILTERFACTORY_H
#define BANDPASSFILTERFACTORY_H

#include "BandPassFilterGlobal.h"

#include <AbstractFactory.h>

#include "BandPassFilter.h"

class BANDPASSFILTER_EXPORT BandPassFilterFactory : public AbstractFactory {
	Q_INTERFACES(AbstractFactory)
	Q_PLUGIN_METADATA(IID "AbstractFactory")

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WindowFlags flags = 0);
};

#endif