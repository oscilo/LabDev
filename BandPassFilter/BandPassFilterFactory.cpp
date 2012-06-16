#include "BandPassFilterFactory.h"

AbstractDevice* BandPassFilterFactory::CreateDevice(QWidget *parent, Qt::WFlags flags) {
	return new BandPassFilter(parent, flags);
}

Q_EXPORT_PLUGIN2(BandPassFilterFactory, BandPassFilterFactory);