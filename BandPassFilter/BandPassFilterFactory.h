#ifndef BANDPASSFILTERFACTORY_H
#define BANDPASSFILTERFACTORY_H

#include "BandPassFilterGlobal.h"

#include <AbstractFactory.h>

#include "BandPassFilter.h"

class BANDPASSFILTER_EXPORT BandPassFilterFactory : public AbstractFactory {
	Q_INTERFACES(AbstractFactory)

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WFlags flags = 0);
};

#endif