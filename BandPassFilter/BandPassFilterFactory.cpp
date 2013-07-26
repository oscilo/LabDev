#include "BandPassFilterFactory.h"

AbstractDevice* BandPassFilterFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags) {
	return new BandPassFilter(parent, flags);
}