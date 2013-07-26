#include "NarrowBandFilterFactory.h"

AbstractDevice* NarrowBandFilterFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags)
{
	return new NarrowBandFilter(parent, flags);
}