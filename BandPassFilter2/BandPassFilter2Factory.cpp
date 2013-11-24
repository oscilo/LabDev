#include "BandPassFilter2Factory.h"

AbstractDevice* BandPassFilter2Factory::CreateDevice(QWidget *parent, Qt::WindowFlags flags) {
	return new BandPassFilter2(parent, flags);
}