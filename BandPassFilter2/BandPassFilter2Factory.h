#ifndef BANDPASSFILTER2FACTORY_H
#define BANDPASSFILTER2FACTORY_H

#include "BandPassFilter2Global.h"

#include <AbstractFactory.h>

#include "BandPassFilter2.h"

class BANDPASSFILTER2_EXPORT BandPassFilter2Factory : public AbstractFactory {
	Q_OBJECT
	Q_INTERFACES(AbstractFactory)
	Q_PLUGIN_METADATA(IID "AbstractFactory")

public:
	AbstractDevice* CreateDevice(QWidget *parent = 0, Qt::WindowFlags flags = 0);
};

#endif