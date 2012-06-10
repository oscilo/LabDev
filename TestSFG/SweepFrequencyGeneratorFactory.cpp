#include "SweepFrequencyGeneratorFactory.h"

AbstractDevice* SweepFrequencyGeneratorFactory::CreateDevice(QWidget *parent, Qt::WFlags flags)
{
	return new SweepFrequencyGenerator(parent, flags);
}

Q_EXPORT_PLUGIN2(SweepFrequencyGeneratorFactory, SweepFrequencyGeneratorFactory);