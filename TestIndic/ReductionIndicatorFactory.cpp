#include "ReductionIndicatorFactory.h"

AbstractDevice* ReductionIndicatorFactory::CreateDevice(QWidget *parent, Qt::WFlags flags)
{
	return new ReductionIndicator(parent, flags);
}

Q_EXPORT_PLUGIN2(ReductionIndicatorFactory, ReductionIndicatorFactory);