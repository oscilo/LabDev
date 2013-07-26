#include "ReductionIndicatorFactory.h"

AbstractDevice* ReductionIndicatorFactory::CreateDevice(QWidget *parent, Qt::WindowFlags flags)
{
	return new ReductionIndicator(parent, flags);
}