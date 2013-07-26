#include "SweepFrequencyGeneratorFactory.h"

AbstractDevice* SweepFrequencyGeneratorFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags)
{
	return new SweepFrequencyGenerator(parent, flags);
}
