#include "HighFrequencySignalsGeneratorFactory.h"

AbstractDevice* HighFrequencySignalsGeneratorFactory::CreateDevice(QWidget *parent, Qt::WFlags flags)
{
	return new HighFrequencySignalsGenerator(parent, flags);
}

Q_EXPORT_PLUGIN2(HighFrequencySignalsGeneratorFactory, HighFrequencySignalsGeneratorFactory);