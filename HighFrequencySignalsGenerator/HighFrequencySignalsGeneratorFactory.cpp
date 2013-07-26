#include "HighFrequencySignalsGeneratorFactory.h"

AbstractDevice* HighFrequencySignalsGeneratorFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags)
{
	return new HighFrequencySignalsGenerator(parent, flags);
}
