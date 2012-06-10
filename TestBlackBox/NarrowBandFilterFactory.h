#ifndef NARROW_BAND_FILTER_FACTORY_H
#define NARROW_BAND_FILTER_FACTORY_H

#include "NarrowBandFilterGlobal.h"

#include <AbstractFactory.h>

#include "NarrowBandFilter.h"

class NARROWBANDFILTER_EXPORT NarrowBandFilterFactory : public AbstractFactory
{
	Q_INTERFACES(AbstractFactory)

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WFlags flags = 0);
};

#endif