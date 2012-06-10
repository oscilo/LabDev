#include "NarrowBandFilterFactory.h"

AbstractDevice* NarrowBandFilterFactory::CreateDevice(QWidget *parent, Qt::WFlags flags)
{
	return new NarrowBandFilter(parent, flags);
}

Q_EXPORT_PLUGIN2(NarrowBandFilterFactory, NarrowBandFilterFactory);